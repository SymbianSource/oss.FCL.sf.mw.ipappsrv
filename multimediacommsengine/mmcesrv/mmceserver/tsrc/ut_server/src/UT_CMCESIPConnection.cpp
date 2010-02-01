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
#include "ut_cmcesipconnection.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "MCETestHelper.h"
#include "mceactionset.h"
#include "mcesipsession.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "mcecomsession.h"
#include "mcelocaladdrresolver.h"

// CONSTRUCTION
UT_CMceSipConnection* UT_CMceSipConnection::NewL()
    {
    UT_CMceSipConnection* self = UT_CMceSipConnection::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSipConnection* UT_CMceSipConnection::NewLC()
    {
    UT_CMceSipConnection* self = new( ELeave ) UT_CMceSipConnection();
    CleanupStack::PushL( self );

	self->ConstructL(); 


    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSipConnection::~UT_CMceSipConnection()
    {
    }

// Default constructor
UT_CMceSipConnection::UT_CMceSipConnection()
    {
    }

// Second phase construct
void UT_CMceSipConnection::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSipConnection::SetupL(  )
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
        
    iIds.iAppUID = 1;
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iStorage = CMCETls::Storage();
    
    iServerCore = CMceServerCore::NewL();
    
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    iSession = CMceCsSession::NewL( *iServerCore );
    iSession->SetClientUidL( iIds, RMessage2() );
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipSession =  static_cast<CMceSipSession*>((iSession->Implementation().SubSessions())[0]);
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iConnection = &iSipSession->SIPConnection();
    } 

void UT_CMceSipConnection::Teardown(  )
    {
    CMCETls::Close();
   	delete iSession;
   	delete iServerCore;
    SdpCodecStringPool::Close();
    }

void UT_CMceSipConnection::UT_CMceSipConnection_StateL()
    {
    EUNIT_ASSERT( iConnection->State() >= 0 );
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_ConnectionL()
    {
    EUNIT_ASSERT( &iConnection->Connection() != NULL );
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_AttachLL()
    {
    EUNIT_ASSERT( iConnection->iSessions.Count() == 1 );
  	EUNIT_ASSERT_LEAVE( iConnection->AttachL( *iSipSession ) );
    EUNIT_ASSERT( iConnection->iSessions.Count() == 1 );
    iConnection->Connection().SetState( CSIPConnection::EInactive );
  	EUNIT_ASSERT_LEAVE( iConnection->AttachL( *iSipSession ) );
    EUNIT_ASSERT( iConnection->iSessions.Count() == 1 );
      	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_DetachL()
    {
    EUNIT_ASSERT( iConnection->iSessions.Count() == 1 );
  	EUNIT_ASSERT( iConnection->Detach( *iSipSession ) == 0 );
    EUNIT_ASSERT( iConnection->iSessions.Count() == 0 );
  	
  	EUNIT_ASSERT( iConnection->Detach( *iSipSession ) == 0 );
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_IncomingRequest_1L()
    {
    CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );
    
  	iSipSession->NextState( KMceStateTerminated );
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 1 );

//OK case
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );

  	iConnection->IncomingRequest( srvtransaction );
  	iStorage->ExecuteCallback();
  	
  	MCE_CHECK_MEMORY_LEAVE( iSession->Implementation().SubSessions().Count() != 2 )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 2 );
  	EUNIT_ASSERT( &(iSession->Implementation().SubSessions()[1]->Request()) == srvtransaction );
  	
  	MCE_RESET_STUBS();
//MESSAGE is not accepted

    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EMessage, EFalse );

  	iConnection->IncomingRequest( srvtransaction );
  	MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
  	                        iStorage->iSipSentResponse->StatusCode() != KMceSipNotImplemented )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 2 );
  	MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
  	                  CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipNotImplemented /*sentResponse*/);
  	MCE_RESET_STUBS();

//OPTIONS is not accepted
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EOptions, EFalse );

  	iConnection->IncomingRequest( srvtransaction );
	MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
  	                        iStorage->iSipSentResponse->StatusCode() != KMceSipNotImplemented )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 2 );
  	MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/,
  	                  CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipNotImplemented /*sentResponse*/);
  	MCE_RESET_STUBS();
  	
//UPDATE-> 501 Not Implemented
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EUpdate, EFalse );

  	iConnection->IncomingRequest( srvtransaction );
  	MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
  	                        iStorage->iSipSentResponse->StatusCode() != KMceSipNotImplemented )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 2 );
  	MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
  	                  CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipNotImplemented /*sentResponse*/);

  	MCE_RESET_STUBS();

//Client resolver fails
    iStorage->SetLeave( KErrGeneral );
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, EFalse );

  	iConnection->IncomingRequest( srvtransaction );
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 2 );

    iStorage->SetLeave( KErrNone );
    
//fake situation, just for testing

    CSIPServerTransaction* tmp = iSession->Implementation().SubSessions()[1]->PopRequest();
    CleanupStack::PushL( tmp );
    
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EBye, EFalse );
    iConnection->IncomingRequest( srvtransaction );
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 2 );
    iSession->Implementation().SubSessions()[1]->iPendingReceivedRequests.AppendL( tmp );
    CleanupStack::Pop( tmp );

  	
//state leaves
    iStorage->iMediaManagerDecodeStatus = KMceSipWarnMiscellaneous;
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );

  	iConnection->IncomingRequest( srvtransaction );
  	iStorage->ExecuteCallback();
  	
  	TInt count = iSession->Implementation().SubSessions().Count();
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 2 );
  	

//INVITE does not got state machine due offer creation problem
    iStorage->SetLeave( KErrNone );

    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, EFalse );

  	iConnection->IncomingRequest( srvtransaction );
  	iStorage->ExecuteCallback();
  	
  	MCE_CHECK_MEMORY_LEAVE( iSession->Implementation().SubSessions().Count() != 3 )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 3 );
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_IncomingRequest_2L()
    {
  	
  	iSipSession->NextState( KMceStateTerminated );
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 1 );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EBye );
        
  	iConnection->IncomingRequest( srvtransaction, iSipSession->Dialog()->Dialog() );
  	MCE_CHECK_MEMORY_LEAVE( &iSipSession->Request() != srvtransaction )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 1 );
  	EUNIT_ASSERT( &iSipSession->Request() == srvtransaction );
    MCE_RESET_STUBS();
    
    // MESSAGE
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EMessage );
        
  	iConnection->IncomingRequest( srvtransaction, iSipSession->Dialog()->Dialog() );
  	
  	//Incomming Requests will be passed to the client 
  	//Responses to those requests will be created by the client itself.
  	
  	EUNIT_ASSERT( !iStorage->iSipSentResponse )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 1 );
    MCE_ASSERT_STUBS( CMCETls::ENone ,
                      CMCETls::ENone ,  
                      SipStrConsts::EEmpty , 
                      SipStrConsts::EEmpty );
    MCE_RESET_STUBS();
	
// OPTIONS -> 501 Not Implemented
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EOptions );
        
  	iConnection->IncomingRequest( srvtransaction, iSipSession->Dialog()->Dialog() );
  	EUNIT_ASSERT( !iStorage->iSipSentResponse )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 1 );
  	MCE_ASSERT_STUBS( CMCETls::ENone,
  	                  CMCETls::ENone ,  
                      SipStrConsts::EEmpty, 
                      SipStrConsts::EEmpty);
    MCE_RESET_STUBS();                  
		
// unknown -> 501 Not Implemented

    iSipSession->NextState( KMceStateEstablished );
    
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::ERequire );
        
  	iConnection->IncomingRequest( srvtransaction, iSipSession->Dialog()->Dialog() );

  	MCE_CHECK_MEMORY_LEAVE( iStorage->iSipSentResponse )
  	EUNIT_ASSERT( !iStorage->iSipSentResponse )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 1 );
  	MCE_ASSERT_STUBS( CMCETls::ENone,
  	                  CMCETls::ENone ,  
                      SipStrConsts::EEmpty , 
                      SipStrConsts::EEmpty );
    MCE_RESET_STUBS();                      

//there is pending response
  	iSipSession->NextState( KMceStateIdle );
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 1 );

    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EBye );
    
  	iConnection->IncomingRequest( srvtransaction, iSipSession->Dialog()->Dialog() );
  	MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
  	                        iStorage->iSipSentResponse->StatusCode() != KMceSipServerInternalError )
  	EUNIT_ASSERT( iSession->Implementation().SubSessions().Count() == 1 );
  	MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
  	                  CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipServerInternalError /*sentResponse*/ );
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_IncomingResponse_1L()
    {
  	iSipSession->NextState( KMceStateTerminated );
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::ECancel, KMceSipOK, SipStrConsts::EPhraseOk, EFalse );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );

    clitransaction->iState = CSIPTransactionBase::ETerminated;

  	iConnection->IncomingResponse( *clitransaction );
  	EUNIT_ASSERT( &iSipSession->Response() == clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );
  	
//SIP behaving badly
    clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

  	iConnection->IncomingResponse( *clitransaction );
  	EUNIT_ASSERT( &iSipSession->Response() != clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

    CleanupStack::PopAndDestroy( clitransaction );
  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_IncomingResponse_2L()
    {

  	iSipSession->NextState( KMceStateTerminated );
    
    CSIPClientTransaction* clitransaction = 
      MCETestHelper::ClientTransactionLC( SipStrConsts::EBye, KMceSipOK, SipStrConsts::EPhraseOk, EFalse );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
    
    clitransaction->iState = CSIPTransactionBase::ETerminated;

  	iConnection->IncomingResponse( *clitransaction, *iSipSession->Dialog() );
  	EUNIT_ASSERT( &iSipSession->Response() == clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

//SIP behaving badly
    clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

  	iConnection->IncomingResponse( *clitransaction, *iSipSession->Dialog() );
  	EUNIT_ASSERT( &iSipSession->Response() != clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

    CleanupStack::PopAndDestroy( clitransaction );

    
  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_IncomingResponse_3L()
    {
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( 
            SipStrConsts::EInvite, KMceSipRinging, 
            SipStrConsts::EPhraseRinging, EFalse );
      
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    EUNIT_ASSERT_EQUALS( 1, iSipSession->PendingTransactions().Count() )
    iSipSession->NextState( KMceStateOffering );
    
    // Callback for forked dialogs called with a NULL pointer
  	iConnection->IncomingResponse( *clitransaction, NULL );
  	EUNIT_ASSERT_EQUALS( 0, iSipSession->iForkedDialogs.Count() )
  	EUNIT_ASSERT_EQUALS( 1, iSipSession->PendingTransactions().Count() )
  	
  	// 180 response to the original dialog
  	iConnection->IncomingResponse( *clitransaction, *iSipSession->Dialog() );
  	EUNIT_ASSERT_EQUALS( 0, iSipSession->iForkedDialogs.Count() )
  	EUNIT_ASSERT_EQUALS( 1, iSipSession->PendingTransactions().Count() );
  	
  	// Forked dialog created for another 180 response
  	CUri8* uri = CUri8::NewLC(); //OK
  	uri->SetComponentL( _L8("sip"), EUriScheme );
  	uri->SetComponentL( _L8("user"), EUriUserinfo );
  	uri->SetComponentL( _L8("nokia.com"), EUriHost );
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );
    CSIPInviteDialogAssoc* forked = 
        CSIPInviteDialogAssoc::NewL( iSipSession->SIPConnection().Connection(),
                                     fromheader, uri );
    CleanupStack::Pop( fromheader );
    CleanupStack::Pop( uri );
    CleanupStack::PushL( forked );
  	iConnection->IncomingResponse( *clitransaction, forked );
    CleanupStack::Pop( forked ); 
    MCE_CHECK_MEMORY_LEAVE( iSipSession->iForkedDialogs.Count() != 1 )
  	EUNIT_ASSERT_EQUALS( 1, iSipSession->iForkedDialogs.Count() )
  	EUNIT_ASSERT_EQUALS( 1, iSipSession->PendingTransactions().Count() );

  	// 200 response for the forked dialog -> session established
  	MCETestHelper::SetResponseL( 
  	    *clitransaction, KMceSipOK, SipStrConsts::EPhraseOk , ETrue );
  	clitransaction->SetState( CSIPTransactionBase::ETerminated );
  	forked->Dialog().iState = CSIPDialog::EConfirmed;
  	iConnection->IncomingResponse( *clitransaction, *forked );
  	MCE_CHECK_MEMORY_LEAVE( iSipSession->PendingTransactions().Count() != 1 )
  	EUNIT_ASSERT_EQUALS( 1, iSipSession->iForkedDialogs.Count() );
  	EUNIT_ASSERT_EQUALS( 1, iSipSession->PendingTransactions().Count() );  	
  	
  	// Invite transaction completed
  	iConnection->InviteCompleted( *clitransaction );
  	EUNIT_ASSERT_EQUALS( 0, iSipSession->iForkedDialogs.Count() );
  	EUNIT_ASSERT_EQUALS( 0, iSipSession->PendingTransactions().Count() );
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_IncomingResponse_4L()
    {
    CSIPClientTransaction* trx = NULL;
    CSIPRegistrationBinding* bnd = NULL;
  	iConnection->IncomingResponse( *trx, *bnd );
  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_ErrorOccured_1L()
    {
    
    CSIPServerTransaction* srvtransaction = 
      MCETestHelper::ServerTransactionL( SipStrConsts::EBye, EFalse );
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
  	iConnection->ErrorOccured( KErrServerTerminated, *srvtransaction );
    
    CSIPClientTransaction* clitransaction = 
      MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
      
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );

    clitransaction->iState = CSIPTransactionBase::ETerminated;

  	iConnection->ErrorOccured( KErrServerTerminated, *clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );
  	
//SIP behaving badly
    clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

  	iConnection->ErrorOccured( KErrServerTerminated, *clitransaction );
  	EUNIT_ASSERT( &iSipSession->Response() != clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

    CleanupStack::PopAndDestroy( clitransaction );

  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_ErrorOccured_2L()
    {

    CSIPServerTransaction* srvtransaction = 
      MCETestHelper::ServerTransactionL( SipStrConsts::EBye, EFalse );
    
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
  	
  	iConnection->ErrorOccured( KErrServerTerminated, *srvtransaction, *iSipSession->Dialog() );


//SIP behaving badly
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

  		iConnection->ErrorOccured( KErrServerTerminated, *clitransaction, *iSipSession->Dialog() );
  	EUNIT_ASSERT( &iSipSession->Response() != clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

    CleanupStack::PopAndDestroy( clitransaction );

  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_ErrorOccured_3L()
    {
    CSIPDialogAssocBase* dlg = NULL;
    
  	iConnection->ErrorOccured( KErrServerTerminated, *iSipSession->Dialog() );

  	iConnection->ErrorOccured( KErrServerTerminated, *dlg );
  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_ErrorOccured_4L()
    {
    CSIPClientTransaction* trx = NULL;
    CSIPRegistrationBinding* bnd = NULL;
  	iConnection->ErrorOccured( KErrServerTerminated, *trx, *bnd );
  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_ErrorOccured_5L()
    {
  	CSIPRefresh* refresh = NULL;
  	iConnection->ErrorOccured( KErrServerTerminated, *refresh );
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_ErrorOccured_6L()
    {
    CSIPRegistrationBinding* bnd = NULL;
  	iConnection->ErrorOccured( KErrServerTerminated, *bnd );
  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_InviteCompletedL()
    {

    CSIPClientTransaction* clitransaction = 
      MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipOK, SipStrConsts::EPhraseOk );
	iSipSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );

    clitransaction->iState = CSIPTransactionBase::ETerminated;
  	
  	iConnection->IncomingResponse( *clitransaction );
  	EUNIT_ASSERT( &iSipSession->Response() == clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );

  	iConnection->InviteCompleted( *clitransaction );
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );
  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_InviteCanceledL()
    {
    CSIPServerTransaction* srvtransaction = 
      MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, EFalse );
      
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
  	
  	iConnection->InviteCanceled( *srvtransaction );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iStorage->iMediaManagerAction == CMCETls::ECloseSession );
    
  	
    }
    
void UT_CMceSipConnection::UT_CMceSipConnection_ConnectionStateChangedL()
    {
    iConnection->Connection().SetState( CSIPConnection::ESuspended );
  	iConnection->ConnectionStateChanged( CSIPConnection::ESuspended );
  	iConnection->ConnectionStateChanged( CSIPConnection::ESuspended );
    EUNIT_ASSERT( !iSipSession->Body()->iIsConnectionActive );
  	
    iConnection->Connection().SetState( CSIPConnection::EActive );
  	iConnection->ConnectionStateChanged( CSIPConnection::EActive );
    EUNIT_ASSERT( iSipSession->Body()->iIsConnectionActive );
  	
    iConnection->Connection().SetState( CSIPConnection::EInactive );
  	iConnection->ConnectionStateChanged( CSIPConnection::EInactive );
    EUNIT_ASSERT( !iSipSession->Body()->iIsConnectionActive );
  	
    iConnection->Connection().SetState( CSIPConnection::EUnavailable );
  	iConnection->ConnectionStateChanged( CSIPConnection::EUnavailable );
    EUNIT_ASSERT( !iSipSession->Body()->iIsConnectionActive );
  	
  	
    }
    
    
void UT_CMceSipConnection::UT_CMceSipConnection_FillWithMatchingAddrFamilyL()
    {
    TInetAddr addr;
    TInetAddr nextHop;
    
    // No next-hop, only IPv4 address available
    iConnection->iLocalAddrResolver->iAddr.SetAddress(INET_ADDR(2,3,4,5));
    iConnection->iLocalAddrResolver->iIPv4Addr.SetAddress(INET_ADDR(2,3,4,5));
    iConnection->FillWithMatchingAddrFamily(addr,NULL);
    EUNIT_ASSERT(addr.CmpAddr(iConnection->iLocalAddrResolver->iIPv4Addr));
    EUNIT_ASSERT(addr.CmpAddr(iConnection->iLocalAddrResolver->iAddr));
    
    // No next-hop, only IPv6 address available
    iConnection->iLocalAddrResolver->iIPv4Addr.Init(KAfInet);
    User::LeaveIfError(
        iConnection->iLocalAddrResolver->iAddr.Input(_L("::ffff:2.3.4.5")));    
    iConnection->FillWithMatchingAddrFamily(addr,NULL);
    EUNIT_ASSERT(!addr.CmpAddr(iConnection->iLocalAddrResolver->iIPv4Addr));
    EUNIT_ASSERT(addr.CmpAddr(iConnection->iLocalAddrResolver->iAddr));    
    
    // No next-hop, both IPv4 and IPv6 address available
    iConnection->iLocalAddrResolver->iIPv4Addr.SetAddress(INET_ADDR(2,3,4,5));
    iConnection->FillWithMatchingAddrFamily(addr,NULL);
    EUNIT_ASSERT(addr.CmpAddr(iConnection->iLocalAddrResolver->iIPv4Addr));
    EUNIT_ASSERT(!addr.CmpAddr(iConnection->iLocalAddrResolver->iAddr));    
    
    // Next-hop IPv4, local address IPv4
    addr.Init(KAfInet);
    nextHop.SetAddress(INET_ADDR(1,2,3,4));
    iConnection->iLocalAddrResolver->iIPv4Addr.SetAddress(INET_ADDR(2,3,4,5));
    iConnection->FillWithMatchingAddrFamily(addr,&nextHop);
    EUNIT_ASSERT(addr.CmpAddr(iConnection->iLocalAddrResolver->iIPv4Addr));
    
    // Next-hop IPv4, local address IPv6, but can be converted to IPv6
    addr.Init(KAfInet);
    iConnection->iLocalAddrResolver->iIPv4Addr.Init(KAfInet);
    User::LeaveIfError(
        iConnection->iLocalAddrResolver->iAddr.Input(_L("::ffff:2.3.4.5")));
    iConnection->FillWithMatchingAddrFamily(addr,&nextHop);
    TInetAddr tmp;
    tmp.SetAddress(INET_ADDR(2,3,4,5));
    EUNIT_ASSERT(addr.CmpAddr(tmp));
    
    // Next-hop IPv4, local address IPv6, cannot be converted to IPv6
    addr.Init(KAfInet);
    User::LeaveIfError(
        iConnection->iLocalAddrResolver->iAddr.Input(
            _L("2001:490:ff2:1002:0:dead:cafe:fe")));
    iConnection->FillWithMatchingAddrFamily(addr,&nextHop);
    EUNIT_ASSERT(addr.CmpAddr(iConnection->iLocalAddrResolver->iAddr));    
    
    // Next-hop IPv6, local address IPv6
    addr.Init(KAfInet);
    User::LeaveIfError(nextHop.Input(_L("2001:490:ff2:1002:0:dead:cafe:de")));
    iConnection->FillWithMatchingAddrFamily(addr,&nextHop);
    EUNIT_ASSERT(addr.CmpAddr(iConnection->iLocalAddrResolver->iAddr));
    
    // Next-hop IPv6, local address IPv4, convert to IPv6
    addr.Init(KAfInet);
    iConnection->iLocalAddrResolver->iAddr.SetAddress(INET_ADDR(2,3,4,5));
    iConnection->FillWithMatchingAddrFamily(addr,&nextHop);
    EUNIT_ASSERT_EQUALS(KAfInet6,addr.Family());	        
    }


void UT_CMceSipConnection::UT_CMceSipConnection_LocalIPAddressLL()
	{	
	TInetAddr nextHop;
	TInetAddr address = iConnection->LocalIPAddressL( iSipSession->NextHopL( nextHop ) );
	EUNIT_ASSERT( !address.Match( INET_ADDR( 0,0,0,0 ) ) );
	}

void UT_CMceSipConnection::UT_CMceSipConnection_ConnectionUpdateLL()
    {
    // Profile configured, tos is ignored
    TUint tosVal( 40 );
    
    /*
    
    MceSipConnection::UpdateConnectionL( ) - method is changed, now it doesn't
    handle profiles anymore so caller must check from SipSession if profile is
    configured or not. example is below.
    
    if( !iSipSession->ProfileConfigured )
    	{
	    iConnection->UpdateConnectionL( tosVal );
    	EUNIT_ASSERT( iConnection->iCurrentSignallingTypeOfService == tosVal );
    	}
    */

    // tos is set
    iConnection->UpdateConnectionL( tosVal );
    EUNIT_ASSERT( iConnection->iCurrentSignallingTypeOfService == tosVal );
    
    // Tos is not changing
    iConnection->UpdateConnectionL( tosVal );
    EUNIT_ASSERT( iConnection->iCurrentSignallingTypeOfService == tosVal );
    }


void UT_CMceSipConnection::UT_CMceSipConnection_IsProfileUsedInSessionL()
    {
    CMceSipManager& manager = iServerCore->Manager();
    TUint32 profileId = 1;
    CSIPProfile& profile = manager.ProfileL( profileId );

	TBool profileUsed = EFalse;
  	profileUsed = iConnection->IsProfileUsedInSession( profile );
    EUNIT_ASSERT( profileUsed == ETrue );
    
    UT_CMceSipConnection_DetachL();
    profileUsed = EFalse;
  	profileUsed = iConnection->IsProfileUsedInSession( profile );
    EUNIT_ASSERT( profileUsed == EFalse );
    }

    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSipConnection,
    "UT_CMceSipConnection",
    "UNIT" )

EUNIT_TEST(
    "Is Profile used in any session - test ",
    "CMceSipConnection",
    "Used profile",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_IsProfileUsedInSessionL, Teardown)

EUNIT_TEST(
    "State - test ",
    "CMceSipConnection",
    "State",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_StateL, Teardown)

EUNIT_TEST(
    "AttachL - test ",
    "CMceSipConnection",
    "AttachL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_AttachLL, Teardown)

EUNIT_TEST(
    "Detach - test ",
    "CMceSipConnection",
    "Detach",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_DetachL, Teardown)

EUNIT_TEST(
    "LocalIPAddressL - test ",
    "CMceSipConnection",
    "LocalIPAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_LocalIPAddressLL, Teardown)

EUNIT_TEST(
    "IncomingRequest - test ",
    "CMceSipConnection",
    "IncomingRequest",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_IncomingRequest_1L, Teardown)

EUNIT_TEST(
    "IncomingRequest - test 2 ",
    "CMceSipConnection",
    "IncomingRequest",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_IncomingRequest_2L, Teardown)

EUNIT_TEST(
    "IncomingResponse - test ",
    "CMceSipConnection",
    "IncomingResponse",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_IncomingResponse_1L, Teardown)

EUNIT_TEST(
    "IncomingResponse - test 2 ",
    "CMceSipConnection",
    "IncomingResponse",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_IncomingResponse_2L, Teardown)

EUNIT_TEST(
    "IncomingResponse - test 3 ",
    "CMceSipConnection",
    "IncomingResponse",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_IncomingResponse_3L, Teardown)

EUNIT_TEST(
    "IncomingResponse - test 4 ",
    "CMceSipConnection",
    "IncomingResponse",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_IncomingResponse_4L, Teardown)

EUNIT_TEST(
    "ErrorOccured - test ",
    "CMceSipConnection",
    "ErrorOccured",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_ErrorOccured_1L, Teardown)

EUNIT_TEST(
    "ErrorOccured - test 2 ",
    "CMceSipConnection",
    "ErrorOccured",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_ErrorOccured_2L, Teardown)

EUNIT_TEST(
    "ErrorOccured - test 3 ",
    "CMceSipConnection",
    "ErrorOccured",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_ErrorOccured_3L, Teardown)

EUNIT_TEST(
    "ErrorOccured - test 4 ",
    "CMceSipConnection",
    "ErrorOccured",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_ErrorOccured_4L, Teardown)

EUNIT_TEST(
    "ErrorOccured - test 5 ",
    "CMceSipConnection",
    "ErrorOccured",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_ErrorOccured_5L, Teardown)

EUNIT_TEST(
    "ErrorOccured - test 6 ",
    "CMceSipConnection",
    "ErrorOccured",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_ErrorOccured_6L, Teardown)

EUNIT_TEST(
    "InviteCompleted - test ",
    "CMceSipConnection",
    "InviteCompleted",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_InviteCompletedL, Teardown)

EUNIT_TEST(
    "InviteCanceled - test ",
    "CMceSipConnection",
    "InviteCanceled",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_InviteCanceledL, Teardown)

EUNIT_TEST(
    "ConnectionStateChanged - test ",
    "CMceSipConnection",
    "ConnectionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_ConnectionStateChangedL, Teardown)

EUNIT_TEST(
    "FillWithMatchingAddrFamily - test ",
    "CMceSipConnection",
    "FillWithMatchingAddrFamily",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_FillWithMatchingAddrFamilyL, Teardown)
   
EUNIT_TEST(
    "ConnectionUpdateL - test ",
    "CMceSipConnection",
    "ConnectionUpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipConnection_ConnectionUpdateLL, Teardown)   


EUNIT_END_TEST_TABLE

//  END OF FILE
