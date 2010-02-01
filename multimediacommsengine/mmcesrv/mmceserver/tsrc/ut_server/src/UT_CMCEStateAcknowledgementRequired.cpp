/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/




//  CLASS HEADER
#include "ut_cmcestateacknowledgementrequired.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MCETestHelper.h"

#include "mcesipsession.h"
#include "mcesipstatemachine.h"
#include "mcenatsipsession.h"
#include "mcenatsipstatemachine.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "mcecomsession.h"
#include "mcestateacknowledgementrequired.h"
#include "mceevent.h"
#include "mceserial.h"
#include "mcemediamanager.h"
#include "mcereliablesender.h"
#include <siprequireheader.h>
#include <siprseqheader.h>
#include <siprackheader.h>

#define MCE_ENTRYL_POSTCONDITION( responseCode ) \
MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || iStorage->iSipSentResponse->StatusCode() != responseCode )

// CONSTRUCTION
UT_CMceStateAcknowledgementRequired* UT_CMceStateAcknowledgementRequired::NewL()
    {
    UT_CMceStateAcknowledgementRequired* self = UT_CMceStateAcknowledgementRequired::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateAcknowledgementRequired* UT_CMceStateAcknowledgementRequired::NewLC()
    {
    UT_CMceStateAcknowledgementRequired* self = new( ELeave ) UT_CMceStateAcknowledgementRequired();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateAcknowledgementRequired::~UT_CMceStateAcknowledgementRequired()
    { 
    }

// Default constructor
UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired()
    {
    }

// Second phase construct
void UT_CMceStateAcknowledgementRequired::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceStateAcknowledgementRequired::SetupL()
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iServerCore = CMceServerCore::NewL();
    
    iStorage = CMCETls::Storage();
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    iSession = CMceCsSession::NewL(*iServerCore);
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipSession =  static_cast<CMceSipSession*>((iSession->Implementation().SubSessions())[0]);
    
    iSipSession->SetBodyL();
    // Set INVITE transaction
    CSIPServerTransaction* invite = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( invite );
    iSipSession->iPendingReceivedRequests.AppendL( invite );
    iSipSession->ReliableSender().iServerTransaction = invite;
    CleanupStack::Pop( invite );
    
    // Response 183
    CSIPResponseElements* response = CSIPResponseElements::NewL( 
        KMceSipSessionProgress, SIPStrings::StringF( SipStrConsts::EPhraseSessionProgress ) );
    iSipSession->ReliableSender().iResponseElements = response;
    // RSeq && Require:100rel header for response
    RPointerArray<CSIPHeaderBase> responseHdrs;
    CSIPHeaderBase::PushLC( &responseHdrs );
    RStringF require100rel = SIPStrings::Pool().OpenFStringL( KMceSip100Rel );
    CleanupClosePushL( require100rel );
    CSIPRequireHeader* require = CSIPRequireHeader::NewLC( require100rel );
    responseHdrs.AppendL( require );
    CleanupStack::Pop( require );
    CleanupStack::PopAndDestroy(); // require100rel
    
    CSIPRSeqHeader* rseq = new( ELeave ) CSIPRSeqHeader( KRSeq );
    CleanupStack::PushL( rseq );
    responseHdrs.AppendL( rseq );
    CleanupStack::Pop( rseq );
    
    response->MessageElements().SetUserHeadersL( responseHdrs );
    CleanupStack::Pop( &responseHdrs );
    
    // PRACK
    // // RAck header for PRACK
    RPointerArray<CSIPHeaderBase> prackHdrs;
    CSIPHeaderBase::PushLC( &prackHdrs );
    CSIPRAckHeader* rack = CSIPRAckHeader::NewLC( 
        KRSeq, KInviteCSeq, SIPStrings::StringF( SipStrConsts::EInvite ) );
    prackHdrs.AppendL( rack );
    CleanupStack::Pop( rack );
    
    CSIPServerTransaction* prack = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EPrack, prackHdrs, ETrue );
    CleanupStack::Pop( &prackHdrs );
    CleanupStack::PushL( prack );
    iSipSession->iPendingReceivedRequests.AppendL( prack );
    CleanupStack::Pop( prack );
    
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
    iState = static_cast<CMceStateAcknowledgementRequired*>(&iSipSession->CurrentState());
    }

void UT_CMceStateAcknowledgementRequired::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;

    SdpCodecStringPool::Close();
    }

void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_AcceptLL()
    {
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EUpdate, KMceSipOK, SipStrConsts::EPhraseRinging, EFalse );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    
    iSipSession->iResponse = clitransaction;
    
    TMceStateTransitionEvent event1( *iSipSession, EMcePrack );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );
    
    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );

    TMceStateTransitionEvent event3( *iSipSession, EMceResponse );
    EUNIT_ASSERT_SPECIFIC_LEAVE ( iState->AcceptL( event3 ), KErrTotalLossOfPrecision );
    
    TMceStateTransitionEvent event4( *iSipSession, EMceNoPrackReceived );
    EUNIT_ASSERT ( iState->AcceptL( event4 ) );
    
    TMceStateTransitionEvent event5( *iSipSession, EMceReliableFailed );
    EUNIT_ASSERT ( iState->AcceptL( event5 ) );
    
    TMceIds ids;
    TMceStateTransitionEvent event6( *iSipSession, EMceItcAcceptSession, ids );
    EUNIT_ASSERT ( iState->AcceptL( event6 ) );
    
    TMceStateTransitionEvent event7( *iSipSession, EMceItcRejectSession, ids );
    EUNIT_ASSERT ( iState->AcceptL( event7 ) );
    }  

void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateFailsLL()
    {
    
 //   CMceComSession* tempBodyCandidate; 
    MCE_RESET_STUBS();
    // EMcePrack
    // update fails
    MCE_RESET_STUBS();
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    TMceStateTransitionEvent event1( *iSipSession, EMcePrack );
    iState->EntryL( event1 );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event1 /*event*/, 
                      EMcePrack /*code*/, 
                      KErrGeneral /*status*/ );                      
    }
    
void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateOkLL()
    {

    MCE_RESET_STUBS();
 
    // updating ready	

    TMceStateTransitionEvent event2( *iSipSession, EMcePrack );
    iState->EntryL( event2 );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT( event2 /*event*/, 
                      EMceMediaUpdated /*code*/,
                      KMceReady /*status*/ );                      
    }
    
void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateAsyncLL()
    {  
    MCE_RESET_STUBS();
        
    // async
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    TMceStateTransitionEvent event3( *iSipSession, EMcePrack );
    iState->EntryL( event3 );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_EVENT( event3 /*event*/, 
                      EMcePrack /*code*/,
                      KMceAsync /*status*/ );                      
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryPrack_NoSdpLL()
    {  

    MCE_RESET_STUBS();

    // No sdp in matching PRACK
    RPointerArray<CSIPHeaderBase> prackHdrs1;
    CSIPHeaderBase::PushLC( &prackHdrs1 );
    CSIPRAckHeader* rack1 = CSIPRAckHeader::NewLC( 
        KRSeq, KInviteCSeq, SIPStrings::StringF( SipStrConsts::EInvite ) );
    prackHdrs1.AppendL( rack1 );
    CleanupStack::Pop( rack1 );
    
    CSIPServerTransaction* prack1 = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EPrack, prackHdrs1, EFalse);
    CleanupStack::Pop( &prackHdrs1 );
    CleanupStack::PushL( prack1 );
    iSipSession->iPendingReceivedRequests.AppendL( prack1 );
    CleanupStack::Pop( prack1 );
    
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    TMceStateTransitionEvent event3_1_1( *iSipSession, EMcePrack );
    iState->EntryL( event3_1_1 );
    MCE_ENTRYL_POSTCONDITION( KMceSipOK )

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT( event3_1_1 /*event*/, 
                      EMcePrack /*code*/,
                      KMceReady /*status*/ );    
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryPrack_NoMatchingPrackLL()
    { 
        
    MCE_RESET_STUBS();
  
    // Not matching PRACK
    RPointerArray<CSIPHeaderBase> prackHdrs;
    CSIPHeaderBase::PushLC( &prackHdrs );
    CSIPRAckHeader* rack = CSIPRAckHeader::NewLC( 
        KRSeq + 2, KInviteCSeq, SIPStrings::StringF( SipStrConsts::EInvite ) );
    prackHdrs.AppendL( rack );
    CleanupStack::Pop( rack );
    
    CSIPServerTransaction* prack = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EPrack, prackHdrs );
    CleanupStack::Pop( &prackHdrs );
    CleanupStack::PushL( prack );
    iSipSession->iPendingReceivedRequests.AppendL( prack );
    CleanupStack::Pop( prack );
    
    TMceStateTransitionEvent event3_2( *iSipSession, EMcePrack );
    iState->EntryL( event3_2 );
    MCE_ENTRYL_POSTCONDITION( KMceSipCallOrTransactionDoesNotExist )

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipCallOrTransactionDoesNotExist /*sentResponse*/);
    MCE_ASSERT_EVENT( event3_2 /*event*/, 
                      EMcePrack /*code*/,
                      KErrNotFound /*status*/ );    

    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_NoReservationLL()
    { 
     
    MCE_RESET_STUBS();
    
// EMceMediaUpdated
    // reservation not done
//    tempBodyCandidate = iSipSession->iNewBodyCandidate ;
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event4 );
    MCE_ENTRYL_POSTCONDITION( KMceSipOK )
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT( event4 /*event*/, 
                      EMceMediaUpdated /*code*/,
                      KMceReady /*status*/ );                      
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedLL()
    {   
    MCE_RESET_STUBS();
        
    // reservation ready, no need to negotiate
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    HBufC8* token = KMediaToken().AllocL();
    iSipSession->SetMediaAuthorizationToken( token );
    iStorage->iMediaManagerReserveStatus = KMceReady;
    TMceStateTransitionEvent event5( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event5 );
    MCE_ENTRYL_POSTCONDITION( KMceSipOK )
    MCE_ASSERT_STUBS( CMCETls::EReserve /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT( event5 /*event*/, 
                      EMceMediaUpdated /*code*/,
                      KMceReady /*status*/ );                      
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EProceeding /*clientState*/, 
                       KMceStateUpdated /*serverState*/ );  
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedNoNegotateLL()
    {      
    MCE_RESET_STUBS();
    
    // reservation ready, need to negotiate
    iStorage->iMediaManagerReserveStatus = KMceReady;
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    TMceStateTransitionEvent event6( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event6 );
    MCE_ENTRYL_POSTCONDITION( KMceSipOK )
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT( event6 /*event*/, 
                      EMceMediaUpdated /*code*/,
                      KMceReady /*status*/ );                      
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedAsyncLL()
    {     

    MCE_RESET_STUBS();
    
    // reservation async
    iStorage->iMediaManagerReserveStatus = KMceAsync;
    TMceStateTransitionEvent event6_2( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event6_2 );
    MCE_ENTRYL_POSTCONDITION( KMceSipOK )
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_EVENT( event6_2 /*event*/, 
                      EMceMediaUpdated /*code*/,
                      KMceReady /*status*/ );                      
                     
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservationErrorLL()
    { 	                       
  	MCE_RESET_STUBS();
    
    // reservation error
    iStorage->iMediaManagerReserveStatus = KErrGeneral;
    TMceStateTransitionEvent event7( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event7 );
    MCE_ENTRYL_POSTCONDITION( KMceSipOK /*KMceSipServerInternalError*/ )
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*KMceSipServerInternalError*/ /*sentResponse*/);
    MCE_ASSERT_EVENT( event7 /*event*/, 
                      EMceMediaUpdated /*code*/,
                      KMceReady /*KErrGeneral*/ /*status*/ );                      
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_WrongTransactionLL()
    {   
    MCE_RESET_STUBS();
    
    // wrong transaction
    MCETestHelper::RequestL( iSipSession, SipStrConsts::EBye, 0, EFalse );
    TMceStateTransitionEvent event8( *iSipSession, EMceMediaUpdated );
    MCE_EUNIT_ASSERT_SPECIFIC_LEAVE( iState->EntryL( event8 ), KErrTotalLossOfPrecision );
    MCE_ASSERT_EVENT( event8 /*event*/, 
                      EMceMediaUpdated /*code*/,
                      KErrNone /*status*/ );                      
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryNoPrackRecievedLL()
    {                
    MCE_RESET_STUBS();
            
    // EMceNoPrackReceived
    TMceStateTransitionEvent event9( *iSipSession, EMceNoPrackReceived );
    iState->EntryL( event9 );
    MCE_ASSERT_EVENT( event9 /*event*/, 
                      EMceNoPrackReceived /*code*/, 
                      KErrNone /*status*/ );   
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryReliableFailedLL()
    {                                          
    MCE_RESET_STUBS();
       
    // EMceReliableFailed
    TMceStateTransitionEvent event10( *iSipSession, EMceReliableFailed );
    iState->EntryL( event10 );
    MCE_ASSERT_EVENT( event10 /*event*/, 
                      EMceReliableFailed /*code*/, 
                      KErrNone /*status*/ );   
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryItcRejectSessionLL()
    {                       
                 
    MCE_RESET_STUBS();
    
    // EMceNoPrackReceived 
	TMceIds ids;
    TMceStateTransitionEvent event11( *iSipSession, EMceItcRejectSession, ids );
    iState->EntryL( event11 );
    MCE_ASSERT_EVENT( event11 /*event*/, 
                      EMceItcRejectSession /*code*/, 
                      KErrNone /*status*/ );   
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryResponseLL()
    {  
    MCE_RESET_STUBS();
        
    // default
    TMceStateTransitionEvent event12( *iSipSession, EMceResponse );
    iState->EntryL( event12 );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                      
    MCE_ASSERT_EVENT( event12 /*event*/, 
                      EMceResponse /*code*/,
                      KErrNone /*status*/ ); 
    }
    
   void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryItcAcceptSessionLL()
    {  
 
    MCE_RESET_STUBS();
    
    // Early accept
    TMceIds acceptIds;
    TMceStateTransitionEvent event13( *iSipSession, EMceItcAcceptSession, acceptIds );
    iState->EntryL( event13 );
    EUNIT_ASSERT( iSipSession->StoredAutoEvent() != NULL );
                      
    MCE_RESET_STUBS();
                      
    // Enable & disable    
    
    
    CMceMsgBase* msg = NULL;
    
//    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    }
    
void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_ExitLL()
    {
    // EMcePrack
    // error
    TMceStateTransitionEvent event1( *iSipSession, EMcePrack );
    event1.ParamStatus() = KErrGeneral;
    iState->ExitL( event1 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ETerminated /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
    
    // no error, need to negotiate
    TMceStateTransitionEvent event2( *iSipSession, EMcePrack );
    iState->ExitL( event2 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
                        
    // no error, no need to negotiate, media reaservation ongoing
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    TMceStateTransitionEvent event2_1( *iSipSession, EMcePrack );
    event2_1.ParamStatus() = KMceAsync;
    iState->ExitL( event2_1 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateAcknowledgementRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
    
    // no error, no need to negotiate, media reaservation ready
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    TMceStateTransitionEvent event2_2( *iSipSession, EMcePrack );
    event2_2.ParamStatus() = KMceReady;
    iState->ExitL( event2_2 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EProceeding /*clientState*/, 
                       KMceStateUpdated /*serverState*/ );
                       
    // automatic accept enabled
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
    TMceIds ids;
    TMceStateTransitionEvent acceptEvent( *iSipSession, EMceItcAcceptSession, ids );
    iSipSession->StoreAutoEventL( &acceptEvent, NULL );
    
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    TMceStateTransitionEvent event2_3( *iSipSession, EMcePrack );
    event2_3.ParamStatus() = KMceReady;
    iState->ExitL( event2_3 );
    
    MCE_CHECK_MEMORY_LEAVE( iSipSession->CurrentState().Id() != KMceStateAnswering )
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EProceeding /*clientState*/, 
                       KMceStateAnswering /*serverState*/ );                   
                       
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );

    // no error, need to negotiate, media reaservation ready
    iStorage->iMediaManagerNeedToNegotiate = ETrue;
    TMceStateTransitionEvent event2_4( *iSipSession, EMcePrack );
    event2_3.ParamStatus() = KMceReady;
    iState->ExitL( event2_4 );
    
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
     
    // EMceMediaUpdated
    // error
    TMceStateTransitionEvent event3( *iSipSession, EMceMediaUpdated );
    event3.ParamStatus() = KErrGeneral;
    iState->ExitL( event3 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ETerminated /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
    
    // KMceReady
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaUpdated );
    event4.ParamStatus() = KMceReady;
    iState->ExitL( event4 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateConfirmationRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
    
    // KMceAsync
    TMceStateTransitionEvent event5( *iSipSession, EMceMediaUpdated );
    event5.ParamStatus() = KMceAsync;
    iState->ExitL( event5 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateServerReserving /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
        
     // EMceReliableFailed
    TMceStateTransitionEvent event6( *iSipSession, EMceReliableFailed );
    iState->ExitL( event6 );
          
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::ETerminated /*clientState*/, 
                       KMceStateTerminated /*serverState*/ );                      
    
    MCE_SET_STATES( iSipSession,
                    CMceSession::EReserving, 
                    KMceStateAcknowledgementRequired );
             
    
    // EMceNoPrackReceived
    
    TMceStateTransitionEvent event7( *iSipSession, EMceNoPrackReceived );
    iState->ExitL( event7 );
           
    MCE_ASSERT_STATES( iSipSession, 
                       CMceSession::ETerminated, 
                       KMceStateTerminated );
    MCE_SET_STATES( iSipSession, 
                    CMceSession::EReserving, 
                    KMceStateAcknowledgementRequired);
                    

	// EMceItcRejectSession
    TMceStateTransitionEvent event8( *iSipSession, EMceItcRejectSession, ids );
    iState->ExitL( event8 );
           
    MCE_ASSERT_STATES( iSipSession, 
                       CMceSession::ETerminated, 
                       KMceStateTerminated );
    MCE_SET_STATES( iSipSession, 
                    CMceSession::EReserving, 
                    KMceStateAcknowledgementRequired);                    

	                    

     
    // default
    TMceStateTransitionEvent event9( *iSipSession, EMceResponse );
    iState->ExitL( event9 );
    MCE_ASSERT_STATES( iSipSession /*session*/, 
                       CMceSession::EReserving /*clientState*/, 
                       KMceStateAcknowledgementRequired /*serverState*/ );
    MCE_SET_STATES( iSipSession /*session*/, 
                    CMceSession::EReserving /*clientState*/, 
                    KMceStateAcknowledgementRequired /*serverState*/ );
    }
    
void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedLL()
	{
	TMceStateTransitionEvent event( *iSipSession, EMcePrack );
	
	// Set PRACK server transaction
    CSIPServerTransaction* prack = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EPrack );
    CleanupStack::PushL( prack );
    iSipSession->iPendingReceivedRequests.AppendL( prack );
    iSipSession->ReliableSender().iServerTransaction = prack;
    CleanupStack::Pop( prack );
    
    iState->EntryMediaUpdatedL( event );
	}

// SIP session
void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL2L()
	{
	CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
	CMceSipSession* sipSession =
		CMceSipSession::NewLC( *iSession,
                               iSipSession->SIPConnection(),
							   *iServerCore->Manager().iStateMachine,
							   profile );
	sipSession->iBody = CMceComSession::NewL();
	
	TMceStateTransitionEvent event( *sipSession, EMcePrack );
	
	// Set PRACK server transaction
    CSIPServerTransaction* prack = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EPrack );
    CleanupStack::PushL( prack );
    sipSession->iPendingReceivedRequests.AppendL( prack );
    sipSession->ReliableSender().iServerTransaction = prack;
    CleanupStack::Pop( prack );

    iState->EntryMediaUpdatedL( event );

    CleanupStack::PopAndDestroy( sipSession );
	}

// NAT session
void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL3L()
	{
	CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
	CMceNatSipSession* natSession =
		CMceNatSipSession::NewLC( *iSession,
                                  iSipSession->SIPConnection(),
							      *iServerCore->Manager().iStateMachine,
							      profile );
	natSession->iBody = CMceComSession::NewL();
	
	TMceStateTransitionEvent event( *natSession, EMcePrack );
	
	// Set PRACK server transaction
    CSIPServerTransaction* prack = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EPrack );
    CleanupStack::PushL( prack );
    natSession->iPendingReceivedRequests.AppendL( prack );
    natSession->ReliableSender().iServerTransaction = prack;
    CleanupStack::Pop( prack );

    iState->EntryMediaUpdatedL( event );

    CleanupStack::PopAndDestroy( natSession );
	}

// NAT session in state where must wait, and can not encode now
void UT_CMceStateAcknowledgementRequired::UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL4L()
	{
	CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
	CMceNatSipSession* natSession =
		CMceNatSipSession::NewLC( *iSession,
                               	  iSipSession->SIPConnection(),
							      *iServerCore->Manager().iStateMachine,
							      profile );
	natSession->iBody = CMceComSession::NewL();
	
	MCE_CHECK_MEMORY_LEAVE( !natSession->iNatStateMachine )
	natSession->iNatStateMachine->iCurrentState = KMceNatStateCreateAnswer;
	
	TMceStateTransitionEvent event( *natSession, EMcePrack );
	
	// Set PRACK server transaction
    CSIPServerTransaction* prack = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EPrack );
    CleanupStack::PushL( prack );
    natSession->iPendingReceivedRequests.AppendL( prack );
    natSession->ReliableSender().iServerTransaction = prack;
    CleanupStack::Pop( prack );

    iState->EntryMediaUpdatedL( event );

    CleanupStack::PopAndDestroy( natSession );
	}

                                                    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateAcknowledgementRequired,
    "UT_CMceStateAcknowledgementRequired",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateAcknowledgementRequired",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryPrackUpdateFailsL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryPrack_UpdateFailsL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateFailsLL, Teardown)
    
EUNIT_TEST(
    "EntryPrackUpdateOkL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryPrack_UpdateOKL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateOkLL, Teardown)
    
EUNIT_TEST(
    "EntryPrackUpdateAsyncL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryPrack_UpdateAsyncL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateAsyncLL, Teardown)
    
EUNIT_TEST(
    "EntryPrackNoSdpL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryPrack_NoSdpL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryPrack_NoSdpLL, Teardown)

EUNIT_TEST(
    "EntryPrackNoMatchingPrackL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryPrack_NoMatchingPrackL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryPrack_NoMatchingPrackLL, Teardown)

EUNIT_TEST(
    "EntryMediaUpdatedNoReservationL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdated_NoReservationL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_NoReservationLL, Teardown)
 
EUNIT_TEST(
    "EntryMediaUpdatedReservedL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdated_ReservedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedLL, Teardown)
 
EUNIT_TEST(
    "EntryMediaUpdatedReservedNoNegotateL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdated_ReservedNoNegotateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedNoNegotateLL, Teardown) 

EUNIT_TEST(
    "EntryMediaUpdatedReservedAsyncL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdated_ReservedAsyncL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedAsyncLL, Teardown) 

EUNIT_TEST(
    "EntryMediaUpdatedReservationErrorL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdated_ReservationErrorL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservationErrorLL, Teardown) 

EUNIT_TEST(
    "EntryMediaUpdatedWrongTransactionL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdated_WrongTransactionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_WrongTransactionLL, Teardown) 

EUNIT_TEST(
    "EntryNoPrackRecievedL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryNoPrackRecievedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryNoPrackRecievedLL, Teardown) 

EUNIT_TEST(
    "EntryReliableFailedL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryReliableFailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryReliableFailedLL, Teardown)

EUNIT_TEST(
    "EntryItcRejectSessionL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryItcRejectSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryItcRejectSessionLL, Teardown)
 
EUNIT_TEST(
    "EntryResponseL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryResponseL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryResponseLL, Teardown)
    
EUNIT_TEST(
    "EntryItcAcceptSessionL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryItcAcceptSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryItcAcceptSessionLL, Teardown)
   
EUNIT_TEST(
    "ExitL - test ",
    "CMceStateAcknowledgementRequired",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_ExitLL, Teardown)

EUNIT_TEST(
    "EntryMediaUpdatedL - test ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdatedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedLL, Teardown)
    
EUNIT_TEST(
    "EntryMediaUpdatedL - test SIP session ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdatedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL2L, Teardown)

EUNIT_TEST(
    "EntryMediaUpdatedL - test NAT session ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdatedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL3L, Teardown)

EUNIT_TEST(
    "EntryMediaUpdatedL - test NAT session, must wait ",
    "CMceStateAcknowledgementRequired",
    "EntryMediaUpdatedL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL4L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
