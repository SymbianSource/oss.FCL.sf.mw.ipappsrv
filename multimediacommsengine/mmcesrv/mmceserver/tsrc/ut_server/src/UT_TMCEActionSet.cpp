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
#include "UT_TMceActionSet.h"

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
#include "siprequireheader.h"



// CONSTRUCTION
UT_TMceActionSet* UT_TMceActionSet::NewL()
    {
    UT_TMceActionSet* self = UT_TMceActionSet::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceActionSet* UT_TMceActionSet::NewLC()
    {
    UT_TMceActionSet* self = new( ELeave ) UT_TMceActionSet();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceActionSet::~UT_TMceActionSet()
    {
    }

// Default constructor
UT_TMceActionSet::UT_TMceActionSet()
    {
    }

// Second phase construct
void UT_TMceActionSet::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_TMceActionSet::SetupL(  )
    {
    CMCETls::OpenL();
    
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
    
    iSession = CMceCsSession::NewL( *iServerCore );
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipSession =  static_cast<CMceSipSession*>((iSession->Implementation().SubSessions())[0]);
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    } 

void UT_TMceActionSet::Teardown(  )
    {
    CMCETls::Close();
   	delete iSession;
    delete iServerCore;

    }


void UT_TMceActionSet::UT_TMceActionSet_UpdateBodyLL()
    {
  	MCE_DELETE( iSipSession->iBody );
  	TMceActionSet actionSet( *iSipSession );
  	
  	CMceComSession* session = CMceComSession::NewL( CMceComSession::EOutSession );
  	CleanupStack::PushL( session );
    CMceMsgObject<CMceComSession>* msg = new (ELeave) CMceMsgObject<CMceComSession>(
        *session, EMceItcMsgTypeSession );
  	CleanupStack::PushL( msg );
  	
  	TMceIds ids;
  	TMceStateTransitionEvent event( *iSipSession, EMceItcEstablishSession, ids, *msg );
    
  	// test handling of client provided aor
  	_LIT8( KMceClientProvidedAor, "alice@host");
  	session->SetOriginatorL( KMceClientProvidedAor() );
  	actionSet.UpdateBodyL( ids, *msg );
    EUNIT_ASSERT( 0 == session->iOriginator->Compare( KMceClientProvidedAor ) );
    
    // test aor reading from SIP profile
    session->SetOriginatorL( KNullDesC8() );
  	EUNIT_ASSERT( NULL == session->iOriginator || 
  	              0 == session->iOriginator->Length() );
  	actionSet.UpdateBodyL( ids, *msg );
  	EUNIT_ASSERT( 0 == session->iOriginator->Compare( KSIPProfileDefAOR ) );
    
  	CleanupStack::PopAndDestroy( msg );
    CleanupStack::Pop( session );
    }

void UT_TMceActionSet::UT_TMceActionSet_CreateBodyCandidateL()
    {
     // test the functionality of the server side session timer processing
  	TMceActionSet actionSet(*iSipSession);
  	actionSet.CreateBodyCandidateL( CMceComSession::EInSession );
  	CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    
    CleanupStack::PushL( srvtransaction );
    iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
    delete iSipSession->iNewBodyCandidate;
    iSipSession->iNewBodyCandidate = NULL;
   	actionSet.CreateBodyCandidateL( CMceComSession::EInSession );
    }

void UT_TMceActionSet::UT_TMceActionSet_ProcessReliableResponseL()
    {
    CSIPClientTransaction* transact183 = MCETestHelper::ClientTransactionLC(
        SipStrConsts::EPhraseSessionProgress, KMceSipSessionProgress, 
        SipStrConsts::EEmpty, ETrue );
    MCETestHelper::SetResponseL( *transact183,
        KMceSipSessionProgress, SipStrConsts::EPhraseSessionProgress, 
        EFalse, ETrue, 1  );
    iSipSession->iResponse = transact183;
    CleanupStack::Pop( transact183 );
    
    MCETestHelper::SetResponseL( *iSipSession->iResponse, KMceSipRinging, 
        SipStrConsts::EPhraseRinging, EFalse, ETrue, 1 );
    
    // TMceActionSet does not anymore update SIP session's RSeq value
    TMceStateTransitionEvent event( *iSipSession, EMceProvisionalResponse );
    TMceActionSet actionSet( *iSipSession );
    EUNIT_ASSERT( ETrue == actionSet.ProcessReliableResponse( 
        *iSipSession->iResponse->ResponseElements() ) );
    
    EUNIT_ASSERT( ETrue == actionSet.ProcessReliableResponse( 
        *iSipSession->iResponse->ResponseElements() ) );
    
    TUint rseqValueOfLatestResponse 
        = MceSip::RSeq( *iSipSession->Response().ResponseElements() );
    iSipSession->SetRSeq( rseqValueOfLatestResponse );
    
    EUNIT_ASSERT( EFalse == actionSet.ProcessReliableResponse( 
        *iSipSession->iResponse->ResponseElements() ) );
    }

void UT_TMceActionSet::UT_TMceActionSet_ProcessSessionTimerServerL_1L(  )
    {
    // test the functionality of the server side session timer processing
  	TMceActionSet actionSet(*iSipSession);  

    RPointerArray<CSIPHeaderBase> sipHeaders; 
    CSIPHeaderBase::PushLC( &sipHeaders );
    // Supported: timer 
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() ); 
	CleanupClosePushL( timerKey );
    
    CSIPSupportedHeader* supportedHeader = CSIPSupportedHeader::NewL( timerKey ); 
    CleanupStack::PopAndDestroy();//timerKey
    CleanupStack::PushL( supportedHeader );
  
    User::LeaveIfError( sipHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    // Session-Expires: 10    
    				
    CSIPExtensionHeader* sessionExpiresHeader = 
    						CSIPExtensionHeader::NewLC( KSessionExpires(), _L8("10") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader ) );
    CleanupStack::Pop( sessionExpiresHeader );
  	
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders, ETrue );
    CleanupStack::PopAndDestroy( &sipHeaders );
    CleanupStack::PushL( srvtransaction );
    
	actionSet.ProcessSessionTimerServerL( *srvtransaction );
	
    CMceComSession* session = iSipSession->Body();

    EUNIT_ASSERT(session->iRecipient->Des().Compare(_L8("User <user@host>")) != KErrNotFound );
    EUNIT_ASSERT(session->iOriginator->Des().Compare(_L8("User <user2@host>")) != KErrNotFound );
    EUNIT_ASSERT(session->iType == CMceComSession::EInSession );
    EUNIT_ASSERT(session->iSIPContentType->Des().Compare(_L8("application/sdp")) != KErrNotFound );
    EUNIT_ASSERT(session->iState == CMceSession::EIncoming );
    EUNIT_ASSERT(session->iTimeout == 10 );
    EUNIT_ASSERT(session->iRefresh == ETrue );
    EUNIT_ASSERT( iStorage->iSipSentResponse == NULL );
	
	actionSet.SendAnswerL( *srvtransaction );
    EUNIT_ASSERT( iStorage->iSipSentResponse != NULL );
	
	CSIPResponseElements* rspelements = iStorage->iSipSentResponse;
	CSIPMessageElements& rspmsgelements = rspelements->MessageElements();
	const RPointerArray<CSIPHeaderBase>& usrheaders = rspmsgelements.UserHeaders();
	// Session-Expires:  
    RStringF sessionexp = SIPStrings::Pool().OpenFStringL( KSessionExpires() ); 
	CleanupClosePushL( sessionexp );
	
	TUint count(0);
	TBool foundSupported = EFalse;
	for(int i = 0; i < usrheaders.Count(); i++)
		{
		HBufC8* value = usrheaders[i]->ToTextValueL();
		if(usrheaders[i]->Name() == SIPStrings::StringF(SipStrConsts::ESupportedHeader))
			{
			if ( value->Find( KMceSipTimer() ) != KErrNotFound )
			    {
			    foundSupported = ETrue;
			    }
			count++;
			}
		else if( usrheaders[i]->Name() == SIPStrings::StringF(SipStrConsts::ERequireHeader))
			{
			EUNIT_ASSERT(value->Find(KMceSipTimer() ) != KErrNotFound );
			count++;
			}
		else if( usrheaders[i]->Name() == sessionexp )
			{
			EUNIT_ASSERT(value->Find( _L8("10;refresher=uas")) != KErrNotFound );
			count++;
			}
		delete value;
		value = NULL;
		}
	EUNIT_ASSERT( count == 4);
    EUNIT_ASSERT( foundSupported );
    		
    CleanupStack::PopAndDestroy(); // sessionexp
   	CleanupStack::PopAndDestroy( srvtransaction );
  	
    }

void UT_TMceActionSet::UT_TMceActionSet_ProcessSessionTimerServerL_2L(  )
    {
    // test the functionality of the server side session timer processing
  	TMceActionSet actionSet(*iSipSession);
  	
    RPointerArray<CSIPHeaderBase> sipHeaders; 
    CSIPHeaderBase::PushLC( &sipHeaders );
    // Supported: timer 
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() ); 
	CleanupClosePushL( timerKey );
    
    CSIPSupportedHeader* supportedHeader = CSIPSupportedHeader::NewL( timerKey ); 
    CleanupStack::PopAndDestroy();//timerKey
    CleanupStack::PushL( supportedHeader );
  
    User::LeaveIfError( sipHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    // Session-Expires: 10;refresher=uas    
    				
    CSIPExtensionHeader* sessionExpiresHeader = 
    						CSIPExtensionHeader::NewLC( KSessionExpires(), _L8("10;refresher=uas") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader ) );
    CleanupStack::Pop( sessionExpiresHeader );
  	
  	
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders, ETrue );
    CleanupStack::PopAndDestroy( &sipHeaders );
    CleanupStack::PushL( srvtransaction );

	actionSet.ProcessSessionTimerServerL( *srvtransaction );
	
    CMceComSession* session = iSipSession->Body();

    EUNIT_ASSERT(session->iTimeout == 10 );
    EUNIT_ASSERT(session->iRefresh );
    EUNIT_ASSERT( iStorage->iSipSentResponse == NULL );
    
    actionSet.SendAnswerL( *srvtransaction );
	
    EUNIT_ASSERT( iStorage->iSipSentResponse != NULL );
	
	CSIPResponseElements* rspelements = iStorage->iSipSentResponse;
	CSIPMessageElements& rspmsgelements = rspelements->MessageElements();
	const RPointerArray<CSIPHeaderBase>& usrheaders = rspmsgelements.UserHeaders();
	// Session-Expires:  
    RStringF sessionexp = SIPStrings::Pool().OpenFStringL( KSessionExpires() ); 
	CleanupClosePushL( sessionexp );
	
	TUint count(0);
	TBool foundSupported = EFalse;
	for(int i = 0; i < usrheaders.Count(); i++)
		{
		HBufC8* value = usrheaders[i]->ToTextValueL();
		if(usrheaders[i]->Name() == SIPStrings::StringF(SipStrConsts::ESupportedHeader))
			{
			if ( value->Find( KMceSipTimer() ) != KErrNotFound )
			    {
			    foundSupported = ETrue;
			    }
			count++;
			}
		else if( usrheaders[i]->Name() == SIPStrings::StringF(SipStrConsts::ERequireHeader))
			{
			EUNIT_ASSERT(value->Find(KMceSipTimer() ) != KErrNotFound );
			count++;
			}
		else if( usrheaders[i]->Name() == sessionexp )
			{
			EUNIT_ASSERT(value->Find( _L8("10;refresher=uas")) != KErrNotFound );
			count++;
			}
		delete value;
		value = NULL;
		}
	EUNIT_ASSERT( count == 4);
    EUNIT_ASSERT( foundSupported );
	CleanupStack::PopAndDestroy(); // sessionexp
   	CleanupStack::PopAndDestroy( srvtransaction );
  	
    }

void UT_TMceActionSet::UT_TMceActionSet_ProcessSessionTimerServerL_3L(  )
    {
    // test the functionality of the server side session timer processing
  	TMceActionSet actionSet(*iSipSession);

    RPointerArray<CSIPHeaderBase> sipHeaders;
    CSIPHeaderBase::PushLC( &sipHeaders );
    // Supported: timer
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() );
	CleanupClosePushL( timerKey );

    CSIPSupportedHeader* supportedHeader = CSIPSupportedHeader::NewL( timerKey );
    CleanupStack::PopAndDestroy();//timerKey
    CleanupStack::PushL( supportedHeader );

    User::LeaveIfError( sipHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    // x: 10

    CSIPExtensionHeader* sessionExpiresHeader = 
    						CSIPExtensionHeader::NewLC( KSessionExpiresCompactForm(), _L8("10") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader ) );
    CleanupStack::Pop( sessionExpiresHeader );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders, ETrue );
    CleanupStack::PopAndDestroy( &sipHeaders );
    CleanupStack::PushL( srvtransaction );

	actionSet.ProcessSessionTimerServerL( *srvtransaction );

    CMceComSession* session = iSipSession->Body();

    EUNIT_ASSERT(session->iRecipient->Des().Compare(_L8("User <user@host>")) != KErrNotFound );
    EUNIT_ASSERT(session->iOriginator->Des().Compare(_L8("User <user2@host>")) != KErrNotFound );
    EUNIT_ASSERT(session->iType == CMceComSession::EInSession );
    EUNIT_ASSERT(session->iSIPContentType->Des().Compare(_L8("application/sdp")) != KErrNotFound );
    EUNIT_ASSERT(session->iState == CMceSession::EIncoming );
    EUNIT_ASSERT(session->iTimeout == 10 );
    EUNIT_ASSERT(session->iRefresh == ETrue );
    EUNIT_ASSERT( iStorage->iSipSentResponse == NULL );

	actionSet.SendAnswerL( *srvtransaction );
    EUNIT_ASSERT( iStorage->iSipSentResponse != NULL );

	CSIPResponseElements* rspelements = iStorage->iSipSentResponse;
	CSIPMessageElements& rspmsgelements = rspelements->MessageElements();
	const RPointerArray<CSIPHeaderBase>& usrheaders = rspmsgelements.UserHeaders();
	// Session-Expires:
    RStringF sessionexp = SIPStrings::Pool().OpenFStringL( KSessionExpires() );
	CleanupClosePushL( sessionexp );

	TUint count(0);
	TBool foundSupported = EFalse;
	for(int i = 0; i < usrheaders.Count(); i++)
		{
		HBufC8* value = usrheaders[i]->ToTextValueL();
		if(usrheaders[i]->Name() == SIPStrings::StringF(SipStrConsts::ESupportedHeader))
			{
			if ( value->Find( KMceSipTimer() ) != KErrNotFound )
			    {
			    foundSupported = ETrue;
			    }
			count++;
			}
		else if( usrheaders[i]->Name() == SIPStrings::StringF(SipStrConsts::ERequireHeader))
			{
			EUNIT_ASSERT(value->Find(KMceSipTimer() ) != KErrNotFound );
			count++;
			}
		else if( usrheaders[i]->Name() == sessionexp )
			{
			EUNIT_ASSERT(value->Find( _L8("10;refresher=uas")) != KErrNotFound );
			count++;
			}
		delete value;
		value = NULL;
		}
	EUNIT_ASSERT( count == 4);
    EUNIT_ASSERT( foundSupported );

    CleanupStack::PopAndDestroy(); // sessionexp
   	CleanupStack::PopAndDestroy( srvtransaction );

    }

void UT_TMceActionSet::UT_TMceActionSet_ProcessSessionTimerClientL_1L(  )
    {
    // test the functionality of the client side session timer processing
    TMceActionSet actionSet(*iSipSession);

    RPointerArray<CSIPHeaderBase> sipHeaders; 
    CSIPHeaderBase::PushLC( &sipHeaders );
    
    // Supported: timer 
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() ); 
	CleanupClosePushL( timerKey );
    
    CSIPSupportedHeader* supportedHeader = CSIPSupportedHeader::NewL( timerKey ); 
    CleanupStack::PushL( supportedHeader );
    User::LeaveIfError( sipHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    // Session-Expires: 100;refresher=uac    
    				
    CSIPExtensionHeader* sessionExpiresHeader = 
    						CSIPExtensionHeader::NewLC( KSessionExpires(), _L8("100;refresher=uac") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader ) );
    CleanupStack::Pop( sessionExpiresHeader );

    CSIPClientTransaction* cltransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, sipHeaders, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
    
    
    
    actionSet.ProcessSessionTimerClientL( *cltransaction );
    
    CleanupStack::PopAndDestroy( cltransaction );
	
    CMceComSession* session = iSipSession->Body();

    EUNIT_ASSERT( !session->iRefresh );
    
    // Require: timer
    CSIPRequireHeader* requireHeader = 
    						CSIPRequireHeader::NewLC( timerKey  );
    User::LeaveIfError( sipHeaders.Append( requireHeader ) );
    CleanupStack::Pop( requireHeader );
    
    CSIPSupportedHeader* supportedHeader2 = CSIPSupportedHeader::NewL( timerKey ); 
    CleanupStack::PushL( supportedHeader2 );
    
    User::LeaveIfError( sipHeaders.Append( supportedHeader2 ) );
    CleanupStack::Pop( supportedHeader2 );
    // Session-Expires: 100;refresher=uac    
    				
    CSIPExtensionHeader* sessionExpiresHeader2 = 
    						CSIPExtensionHeader::NewLC( KSessionExpires(), _L8("100;refresher=uac") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader2 ) );
    CleanupStack::Pop( sessionExpiresHeader2 );

    CSIPClientTransaction* cltransaction2 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, sipHeaders, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
    
    
    actionSet.ProcessSessionTimerClientL( *cltransaction2 );
    
    CleanupStack::PopAndDestroy( cltransaction2 );

    EUNIT_ASSERT( session->iTimeout == 100 );
    EUNIT_ASSERT( session->iRefresh );
    
    // Require: timer
    CSIPRequireHeader* requireHeader2 = 
    						CSIPRequireHeader::NewLC( timerKey  );
    User::LeaveIfError( sipHeaders.Append( requireHeader2 ) );
    CleanupStack::Pop( requireHeader2 );
    
    CSIPSupportedHeader* supportedHeader3 = CSIPSupportedHeader::NewL( timerKey ); 
    CleanupStack::PushL( supportedHeader3 );
    
    User::LeaveIfError( sipHeaders.Append( supportedHeader3 ) );
    CleanupStack::Pop( supportedHeader3 );
    // Session-Expires: 100 <- Failure case  
    				
    CSIPExtensionHeader* sessionExpiresHeader3 = 
    						CSIPExtensionHeader::NewLC( KSessionExpires(), _L8("100") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader3 ) );
    CleanupStack::Pop( sessionExpiresHeader3 );

    CSIPClientTransaction* cltransaction3 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, sipHeaders, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
    
    
    EUNIT_ASSERT_LEAVE( actionSet.ProcessSessionTimerClientL( *cltransaction3 ));
    
    CleanupStack::PopAndDestroy( cltransaction3 );


    // Require: timer
    CSIPRequireHeader* requireHeader3 = 
    						CSIPRequireHeader::NewLC( timerKey  );
    User::LeaveIfError( sipHeaders.Append( requireHeader3 ) );
    CleanupStack::Pop( requireHeader3 );
    
    CSIPSupportedHeader* supportedHeader4 = CSIPSupportedHeader::NewL( timerKey ); 
    CleanupStack::PushL( supportedHeader4 );
    
    User::LeaveIfError( sipHeaders.Append( supportedHeader4 ) );
    CleanupStack::Pop( supportedHeader4 );
    // Session-Expires: 100;refresher=
    				
    CSIPExtensionHeader* sessionExpiresHeader4 = 
    						CSIPExtensionHeader::NewLC( KSessionExpires(), _L8("100;refresher=") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader4 ) );
    CleanupStack::Pop( sessionExpiresHeader4 );

    CSIPClientTransaction* cltransaction4 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, sipHeaders, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );
    
    
    actionSet.ProcessSessionTimerClientL( *cltransaction4 );
    
    CleanupStack::PopAndDestroy( cltransaction4 );
    EUNIT_ASSERT( session->iRefresh );
   
    CleanupStack::PopAndDestroy();//timerKey
    CleanupStack::PopAndDestroy( &sipHeaders );
    }

void UT_TMceActionSet::UT_TMceActionSet_ProcessSessionTimerClientL_2L(  )
    {
    // test the functionality of the client side session timer processing
    TMceActionSet actionSet(*iSipSession);
    
    RPointerArray<CSIPHeaderBase> sipHeaders; 
    CSIPHeaderBase::PushLC( &sipHeaders );
    // Require: timer
    CSIPExtensionHeader* minSEHeader = 
    						CSIPExtensionHeader::NewLC( KMinSE(), _L8("200") );
    User::LeaveIfError( sipHeaders.Append( minSEHeader ) );
    CleanupStack::Pop( minSEHeader );
    
    CSIPClientTransaction* cltransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, sipHeaders, KMceSipSessionIntervalTooSmall, SipStrConsts::EPhraseIntervalTooBrief, EFalse);
    CleanupStack::Pop( cltransaction );
    CleanupStack::PopAndDestroy( &sipHeaders );
    CleanupStack::PushL( cltransaction );
    
    
    actionSet.ProcessSessionTimerClientL( *cltransaction );
	
    CMceComSession* session = iSipSession->Body();
	EUNIT_ASSERT(session->iMinSE == 200 );
    EUNIT_ASSERT(session->iTimeout == 200 );

    CleanupStack::PopAndDestroy( cltransaction );
    
    }

void UT_TMceActionSet::UT_TMceActionSet_ProcessSessionTimerClientL_3L(  )
    {
    // test the functionality of the client side session timer processing
    TMceActionSet actionSet(*iSipSession);

    RPointerArray<CSIPHeaderBase> sipHeaders;
    CSIPHeaderBase::PushLC( &sipHeaders );

    // Supported: timer
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() );
	CleanupClosePushL( timerKey );

    CSIPSupportedHeader* supportedHeader = CSIPSupportedHeader::NewL( timerKey );
    CleanupStack::PushL( supportedHeader );
    User::LeaveIfError( sipHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    // Session-Expires: 100;refresher=uac

    CSIPExtensionHeader* sessionExpiresHeader = 
    						CSIPExtensionHeader::NewLC( KSessionExpiresCompactForm(), _L8("100;refresher=uac") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader ) );
    CleanupStack::Pop( sessionExpiresHeader );

    CSIPClientTransaction* cltransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, sipHeaders, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );

    actionSet.ProcessSessionTimerClientL( *cltransaction );

    CleanupStack::PopAndDestroy( cltransaction );

    CMceComSession* session = iSipSession->Body();

    EUNIT_ASSERT( !session->iRefresh );

    // Require: timer
    CSIPRequireHeader* requireHeader = 
    						CSIPRequireHeader::NewLC( timerKey );
    User::LeaveIfError( sipHeaders.Append( requireHeader ) );
    CleanupStack::Pop( requireHeader );

    CSIPSupportedHeader* supportedHeader2 = CSIPSupportedHeader::NewL( timerKey );
    CleanupStack::PushL( supportedHeader2 );

    User::LeaveIfError( sipHeaders.Append( supportedHeader2 ) );
    CleanupStack::Pop( supportedHeader2 );
    // Session-Expires: 100;refresher=uac

    CSIPExtensionHeader* sessionExpiresHeader2 = 
    						CSIPExtensionHeader::NewLC( KSessionExpiresCompactForm(), _L8("100;refresher=uac") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader2 ) );
    CleanupStack::Pop( sessionExpiresHeader2 );

    CSIPClientTransaction* cltransaction2 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, sipHeaders, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );

    actionSet.ProcessSessionTimerClientL( *cltransaction2 );

    CleanupStack::PopAndDestroy( cltransaction2 );

    EUNIT_ASSERT( session->iTimeout == 100 );
    EUNIT_ASSERT( session->iRefresh );

    // Require: timer
    CSIPRequireHeader* requireHeader2 = 
    						CSIPRequireHeader::NewLC( timerKey );
    User::LeaveIfError( sipHeaders.Append( requireHeader2 ) );
    CleanupStack::Pop( requireHeader2 );

    CSIPSupportedHeader* supportedHeader3 = CSIPSupportedHeader::NewL( timerKey );
    CleanupStack::PushL( supportedHeader3 );

    User::LeaveIfError( sipHeaders.Append( supportedHeader3 ) );
    CleanupStack::Pop( supportedHeader3 );
    // Session-Expires: 100 <- Failure case

    CSIPExtensionHeader* sessionExpiresHeader3 = 
    						CSIPExtensionHeader::NewLC( KSessionExpiresCompactForm(), _L8("100") );
    User::LeaveIfError( sipHeaders.Append( sessionExpiresHeader3 ) );
    CleanupStack::Pop( sessionExpiresHeader3 );

    CSIPClientTransaction* cltransaction3 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, sipHeaders, KMceSipOK, SipStrConsts::EPhraseOk, ETrue );

    EUNIT_ASSERT_LEAVE( actionSet.ProcessSessionTimerClientL( *cltransaction3 ));

    CleanupStack::PopAndDestroy( cltransaction3 );
    CleanupStack::PopAndDestroy();//timerKey
    CleanupStack::PopAndDestroy( &sipHeaders );
    }

void UT_TMceActionSet::UT_TMceActionSet_AddSessionTimerFieldsClientL_1L(  )
    {
    TMceActionSet actionSet(*iSipSession);
    iSipSession->Body()->iTimeout = 10;
    RPointerArray<CSIPHeaderBase> sipHeaders;
    CSIPHeaderBase::PushLC( &sipHeaders );
    actionSet.AddSessionTimerFieldsClientL(sipHeaders);
    // Session-Expires:  
    RStringF sessionexp = SIPStrings::Pool().OpenFStringL( KSessionExpires() ); 
	CleanupClosePushL( sessionexp );
	
	TUint count(0);
	
    for(int i = 0; i < sipHeaders.Count(); i++)
		{
		HBufC8* value = sipHeaders[i]->ToTextValueL();
		if(sipHeaders[i]->Name() == SIPStrings::StringF(SipStrConsts::ESupportedHeader))
			{
			EUNIT_ASSERT(value->Find( KMceSipTimer() ) != KErrNotFound );
			count++;
			}
		else if( sipHeaders[i]->Name() == sessionexp )
			{
			EUNIT_ASSERT(value->Find( _L8("10")) != KErrNotFound );
			count++;
			}
		delete value;
		value = NULL;
		}
	EUNIT_ASSERT(count == 2);
	CleanupStack::PopAndDestroy(); //sessionexp
	CleanupStack::PopAndDestroy( &sipHeaders );
		
    }

            
    
void UT_TMceActionSet::UT_TMceActionSet_SendRejectOfferWithWarningLL()
    {
    TMceActionSet actionSet(*iSipSession);

    RPointerArray<CSIPHeaderBase> sipHeaders; 
    CSIPHeaderBase::PushLC( &sipHeaders );
  	
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders, ETrue );
    CleanupStack::PopAndDestroy( &sipHeaders );
    CleanupStack::PushL( srvtransaction );
    
    actionSet.SendRejectOfferWithWarningL( KMceSipWarnMediaTypeNotAvailable, *srvtransaction );

    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() == KMceSipNotAcceptableHere );
    
    CleanupStack::PopAndDestroy( srvtransaction );

    MCE_RESET_STUBS();
    
    RPointerArray<CSIPHeaderBase> sipHeaders1; 
    CSIPHeaderBase::PushLC( &sipHeaders1 );
    
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders1, ETrue );
    CleanupStack::PopAndDestroy( &sipHeaders1 );
    CleanupStack::PushL( srvtransaction );
    
    actionSet.SendRejectOfferWithWarningL( KErrNotFound, *srvtransaction );

    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() ==  KMceSipNotAcceptableHere );
    
    CleanupStack::PopAndDestroy( srvtransaction );

    MCE_RESET_STUBS();
    
    RPointerArray<CSIPHeaderBase> sipHeaders2; 
    CSIPHeaderBase::PushLC( &sipHeaders2 );
    
    srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, sipHeaders2, ETrue );
    CleanupStack::PopAndDestroy( &sipHeaders2 );
    CleanupStack::PushL( srvtransaction );
    
    actionSet.SendRejectOfferWithWarningL( KMceSipBadRequest, *srvtransaction );

    EUNIT_ASSERT ( iStorage->iSipSentResponse->StatusCode() ==  KMceSipBadRequest );
    
    CleanupStack::PopAndDestroy( srvtransaction );

    }


void UT_TMceActionSet::UT_TMceActionSet_CheckContactIsSecureLL(  )
    {
    // test the functionality of Checking Contact header if it is secure
    TMceActionSet actionSet(*iSipSession);
    
     /*
    _LIT8(KContactValueTls, "<sip:user@localhost;transport=tls>;expires=3600");
_LIT8(KContactValueSips, "<sips:user@localhost>;expires=3600");
_LIT8(KContactValueSipsAndTls,*/
    //Add contact header
    RPointerArray<CSIPHeaderBase> usrhdrs;
    CSIPHeaderBase::PushLC( &usrhdrs );
    MCETestHelper::AppendContactToUserHeaders( usrhdrs, KContactValue );
    
    
    
    
    CSIPClientTransaction* cltransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, usrhdrs, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
    CleanupStack::Pop( cltransaction );
    CleanupStack::PopAndDestroy( &usrhdrs );
    CleanupStack::PushL( cltransaction );
    
    
    actionSet.CheckContactIsSecureL( *cltransaction );
	
    CMceComSession& session = iSipSession->ActiveBody();
	EUNIT_ASSERT ( session.iSipContactAddrSecure == CMceSession::EControlPathUnsecure );	

    CleanupStack::PopAndDestroy( cltransaction );
    
   
    //Contact header with tls
    RPointerArray<CSIPHeaderBase> usrhdrsContactTls;
    CSIPHeaderBase::PushLC( &usrhdrsContactTls );
    MCETestHelper::AppendContactToUserHeaders( usrhdrsContactTls, KContactValueTls );
    
    CSIPClientTransaction* cltransaction2 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, usrhdrsContactTls, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
    CleanupStack::Pop( cltransaction2 );
    CleanupStack::PopAndDestroy( &usrhdrsContactTls );
    CleanupStack::PushL( cltransaction2 );
    
    
    actionSet.CheckContactIsSecureL( *cltransaction2 );
	
    
	EUNIT_ASSERT ( session.iSipContactAddrSecure == CMceSession::EControlPathUnsecure );	

    CleanupStack::PopAndDestroy( cltransaction2 );
    
    //Contact header with sips
    RPointerArray<CSIPHeaderBase> usrhdrsContactSips;
    CSIPHeaderBase::PushLC( &usrhdrsContactSips );
    MCETestHelper::AppendContactToUserHeaders( usrhdrsContactSips, KContactValueSips );
    
    CSIPClientTransaction* cltransaction3 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, usrhdrsContactSips, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
    CleanupStack::Pop( cltransaction3 );
    CleanupStack::PopAndDestroy( &usrhdrsContactSips );
    CleanupStack::PushL( cltransaction3 );
    
    
    actionSet.CheckContactIsSecureL( *cltransaction3 );
	
    
	EUNIT_ASSERT ( session.iSipContactAddrSecure == CMceSession::EControlPathUnsecure );	

    CleanupStack::PopAndDestroy( cltransaction3 );
    
      //Contact header with sips and tls
    RPointerArray<CSIPHeaderBase> usrhdrsContactSipsTls;
    CSIPHeaderBase::PushLC( &usrhdrsContactSipsTls );
    MCETestHelper::AppendContactToUserHeaders( usrhdrsContactSipsTls, KContactValueSipsAndTls );
    
    CSIPClientTransaction* cltransaction4 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, usrhdrsContactSipsTls, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
    CleanupStack::Pop( cltransaction4 );
    CleanupStack::PopAndDestroy( &usrhdrsContactSipsTls);
    CleanupStack::PushL( cltransaction3 );
    
    
    actionSet.CheckContactIsSecureL( *cltransaction4 );
	
    
	EUNIT_ASSERT ( session.iSipContactAddrSecure == CMceSession::EControlPathUnsecure );	

    CleanupStack::PopAndDestroy( cltransaction4 );
    }

void UT_TMceActionSet::UT_TMceActionSet_CheckContactIsSecureL2L(  )
    {
    // test the functionality of Checking Contact header if it is secure
    TMceActionSet actionSet(*iSipSession);
    //change profile contact header
    HBufC8* profileRegisContactTls = KContactValueTls().AllocLC();
    iSipSession->Profile().SetParam( KSIPRegisteredContact, profileRegisContactTls );
    CleanupStack::Pop(profileRegisContactTls );
     /*
    _LIT8(KContactValueTls, "<sip:user@localhost;transport=tls>;expires=3600");
_LIT8(KContactValueSips, "<sips:user@localhost>;expires=3600");
_LIT8(KContactValueSipsAndTls,*/
    //Add contact header
    RPointerArray<CSIPHeaderBase> usrhdrs;
    CSIPHeaderBase::PushLC( &usrhdrs );
    MCETestHelper::AppendContactToUserHeaders( usrhdrs, KContactValue );
    
    
    
    
    CSIPClientTransaction* cltransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, usrhdrs, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
    CleanupStack::Pop( cltransaction );
    CleanupStack::PopAndDestroy( &usrhdrs );
    CleanupStack::PushL( cltransaction );
    
    
    actionSet.CheckContactIsSecureL( *cltransaction );
	
    CMceComSession& session = iSipSession->ActiveBody();
	EUNIT_ASSERT ( session.iSipContactAddrSecure == CMceSession::EControlPathUnsecure );	

    CleanupStack::PopAndDestroy( cltransaction );
    
   
    //Contact header with tls
    RPointerArray<CSIPHeaderBase> usrhdrsContactTls;
    CSIPHeaderBase::PushLC( &usrhdrsContactTls );
    MCETestHelper::AppendContactToUserHeaders( usrhdrsContactTls, KContactValueTls );
    
    CSIPClientTransaction* cltransaction2 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, usrhdrsContactTls, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
    CleanupStack::Pop( cltransaction2 );
    CleanupStack::PopAndDestroy( &usrhdrsContactTls );
    CleanupStack::PushL( cltransaction2 );
    
    
    actionSet.CheckContactIsSecureL( *cltransaction2 );
	
    
	EUNIT_ASSERT ( session.iSipContactAddrSecure == CMceSession::EControlPathSecure );	

    CleanupStack::PopAndDestroy( cltransaction2 );
    
    //Contact header with sips
    RPointerArray<CSIPHeaderBase> usrhdrsContactSips;
    CSIPHeaderBase::PushLC( &usrhdrsContactSips );
    MCETestHelper::AppendContactToUserHeaders( usrhdrsContactSips, KContactValueSips );
    
    CSIPClientTransaction* cltransaction3 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, usrhdrsContactSips, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
    CleanupStack::Pop( cltransaction3 );
    CleanupStack::PopAndDestroy( &usrhdrsContactSips );
    CleanupStack::PushL( cltransaction3 );
    
    
    actionSet.CheckContactIsSecureL( *cltransaction3 );
	
    
	EUNIT_ASSERT ( session.iSipContactAddrSecure == CMceSession::EControlPathSecure );	

    CleanupStack::PopAndDestroy( cltransaction3 );
    
      //Contact header with sips and tls
    RPointerArray<CSIPHeaderBase> usrhdrsContactSipsTls;
    CSIPHeaderBase::PushLC( &usrhdrsContactSipsTls );
    MCETestHelper::AppendContactToUserHeaders( usrhdrsContactSipsTls, KContactValueSipsAndTls );
    
    CSIPClientTransaction* cltransaction4 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, usrhdrsContactSipsTls, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
    CleanupStack::Pop( cltransaction4 );
    CleanupStack::PopAndDestroy( &usrhdrsContactSipsTls);
    CleanupStack::PushL( cltransaction3 );
    
    
    actionSet.CheckContactIsSecureL( *cltransaction4 );
	
    
	EUNIT_ASSERT ( session.iSipContactAddrSecure == CMceSession::EControlPathSecure );	

    CleanupStack::PopAndDestroy( cltransaction4 );
    }


void UT_TMceActionSet::UT_TMceActionSet_ProcessSessionTimerServerLL()
    {
    TMceActionSet actionSet( *iSipSession );
    iSipSession->SetSubState( CMceSipSession::EUpdating );
  	actionSet.CreateBodyCandidateL( CMceComSession::EInSession );
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( srvtransaction );
    
    CUri8* uri = CUri8::NewLC();
    MCE_SET_URIL( uri );
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri );
    CleanupStack::Pop( uri );
    CleanupStack::PushL(requestelements);
    CSIPMessageElements& msgelements = requestelements->MessageElements();
    
    // test handling of session update request, UAC is refresher
    _LIT8( KSessionExpires1, "90;refresher=uac");
    CSIPExtensionHeader* sessionExpiresHeader = CSIPExtensionHeader::NewLC(
        KMceSipHeaderSessionExpires(), KSessionExpires1() );
    msgelements.AddHeaderL( sessionExpiresHeader );
    CleanupStack::Pop( sessionExpiresHeader );
    CleanupStack::Pop( requestelements );
    srvtransaction->SetRequestElements( requestelements );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( !iSipSession->ActiveBody().iRefresh );
    
    // test handling of session update request, UAS is refresher
    RStringF expiresKey = SIPStrings::Pool().OpenFStringL( 
        KMceSipHeaderSessionExpires() );
    CleanupClosePushL( expiresKey );
    msgelements.RemoveHeaders( expiresKey );
    
    _LIT8( KSessionExpires2, "90;refresher=uas");
    sessionExpiresHeader = CSIPExtensionHeader::NewLC( 
        KMceSipHeaderSessionExpires(), KSessionExpires2() );
    msgelements.AddHeaderL( sessionExpiresHeader );
    CleanupStack::Pop( sessionExpiresHeader );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( iSipSession->ActiveBody().iRefresh );
    
    // test handling of session update request, 
    // UAC has not provided refresher info
    msgelements.RemoveHeaders( expiresKey );
    
    _LIT8( KSessionExpires3, "90");
    sessionExpiresHeader = CSIPExtensionHeader::NewLC( 
        KMceSipHeaderSessionExpires(), KSessionExpires3() );
    msgelements.AddHeaderL( sessionExpiresHeader );
    CleanupStack::Pop( sessionExpiresHeader );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( iSipSession->ActiveBody().iRefresh );
    
    // test handling of refresh request, role should not change
    iSipSession->ActiveBody().iRefresh = EFalse;
    iSipSession->SetSubState( CMceSipSession::ERefreshing );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( !iSipSession->ActiveBody().iRefresh );
    
    // test handling of refresh request, UAC requests role change
    msgelements.RemoveHeaders( expiresKey );
    
    sessionExpiresHeader = CSIPExtensionHeader::NewLC( 
        KMceSipHeaderSessionExpires(), KSessionExpires2() );
    msgelements.AddHeaderL( sessionExpiresHeader );
    CleanupStack::Pop( sessionExpiresHeader );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( iSipSession->ActiveBody().iRefresh );
    
    CleanupStack::PopAndDestroy( &expiresKey );
    CleanupStack::PopAndDestroy( srvtransaction );
    }


void UT_TMceActionSet::UT_TMceActionSet_ProcessSessionTimerServerL2L()
    {
    TMceActionSet actionSet( *iSipSession );
    iSipSession->SetSubState( CMceSipSession::EUpdating );
  	actionSet.CreateBodyCandidateL( CMceComSession::EInSession );
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( srvtransaction );

    CUri8* uri = CUri8::NewLC();
    MCE_SET_URIL( uri );
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri );
    CleanupStack::Pop( uri );
    CleanupStack::PushL(requestelements);
    CSIPMessageElements& msgelements = requestelements->MessageElements();

    // test handling of session update request, UAC is refresher
    _LIT8( KSessionExpires1, "90;refresher=uac");
    CSIPExtensionHeader* sessionExpiresHeader = CSIPExtensionHeader::NewLC(
    		KMceSipHeaderCompactFormOfSessionExpires(), KSessionExpires1() );
    msgelements.AddHeaderL( sessionExpiresHeader );
    CleanupStack::Pop( sessionExpiresHeader );
    CleanupStack::Pop( requestelements );
    srvtransaction->SetRequestElements( requestelements );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( !iSipSession->ActiveBody().iRefresh );

    // test handling of session update request, UAS is refresher
    RStringF expiresKey = SIPStrings::Pool().OpenFStringL(
    		KMceSipHeaderCompactFormOfSessionExpires() );
    CleanupClosePushL( expiresKey );
    msgelements.RemoveHeaders( expiresKey );

    _LIT8( KSessionExpires2, "90;refresher=uas");
    sessionExpiresHeader = CSIPExtensionHeader::NewLC(
    		KMceSipHeaderCompactFormOfSessionExpires(), KSessionExpires2() );
    msgelements.AddHeaderL( sessionExpiresHeader );
    CleanupStack::Pop( sessionExpiresHeader );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( iSipSession->ActiveBody().iRefresh );

    // test handling of session update request,
    // UAC has not provided refresher info
    msgelements.RemoveHeaders( expiresKey );

    _LIT8( KSessionExpires3, "90");
    sessionExpiresHeader = CSIPExtensionHeader::NewLC(
    		KMceSipHeaderCompactFormOfSessionExpires(), KSessionExpires3() );
    msgelements.AddHeaderL( sessionExpiresHeader );
    CleanupStack::Pop( sessionExpiresHeader );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( iSipSession->ActiveBody().iRefresh );

    // test handling of refresh request, role should not change
    iSipSession->ActiveBody().iRefresh = EFalse;
    iSipSession->SetSubState( CMceSipSession::ERefreshing );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( !iSipSession->ActiveBody().iRefresh );

    // test handling of refresh request, UAC requests role change
    msgelements.RemoveHeaders( expiresKey );

    sessionExpiresHeader = CSIPExtensionHeader::NewLC( 
    		KMceSipHeaderCompactFormOfSessionExpires(), KSessionExpires2() );
    msgelements.AddHeaderL( sessionExpiresHeader );
    CleanupStack::Pop( sessionExpiresHeader );
    actionSet.ProcessSessionTimerServerL( *srvtransaction );
    EUNIT_ASSERT( iSipSession->ActiveBody().iRefresh );

    CleanupStack::PopAndDestroy( &expiresKey );
    CleanupStack::PopAndDestroy( srvtransaction );
    }


void UT_TMceActionSet::UT_TMceActionSet_AddSessionTimerFieldsClientLL()
    {
    TMceActionSet actionSet( *iSipSession );
    RStringF expiresKey = SIPStrings::Pool().OpenFStringL( 
        KMceSipHeaderSessionExpires() );
    CleanupClosePushL( expiresKey );
    RPointerArray<CSIPHeaderBase> headers;
    CSIPHeaderBase::PushLC( &headers );
    
    // if not refreshing, refresher parameter should not be added
    iSipSession->SetSubState( CMceSipSession::EUpdating );
    iSipSession->ActiveBody().iTimeout = 90;
    actionSet.AddSessionTimerFieldsClientL( headers );
    CSIPExtensionHeader* header = static_cast<CSIPExtensionHeader*>( 
        MceSip::FindHeader( headers, expiresKey ) );
    _LIT8( KTimeOut, "90");
    EUNIT_ASSERT( 0 == header->Value().Compare( KTimeOut() ) );
    
    // in refreshing state refresher parameter should be added based on 
    // current role
    iSipSession->SetSubState( CMceSipSession::ERefreshing );
    iSipSession->ActiveBody().iRefresh = ETrue;
    CleanupStack::PopAndDestroy( &headers );
    CSIPHeaderBase::PushLC( &headers );
    actionSet.AddSessionTimerFieldsClientL( headers );
    _LIT8( KSessionExpiresValue, "90;refresher=uac");
    EUNIT_ASSERT( 0 == header->Value().Compare( KSessionExpiresValue() ) );
    
    CleanupStack::PopAndDestroy( &headers );
    CleanupStack::PopAndDestroy( &expiresKey );
    }


void UT_TMceActionSet::UT_TMceActionSet_ClientMediaStateChangedL()
	{
	// Test handling of ICMP errors
	TMceActionSet actionSet(*iSipSession);
	TMceMediaEventCode eventCode( EMceMediaUpdated );

	// It must not pause the stream if a null pointer is passed in
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ENone )
	EUNIT_ASSERT( iStorage->iMMObject == NULL )
	actionSet.ClientMediaStateChangedL( 0, eventCode );
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ENone )
	EUNIT_ASSERT( iStorage->iMMObject == NULL )

	// It must not try to pause if there's no Itc event assigned
	TMceMccComEvent mccEvent( EMceItcNotAssigned, 1, 1, 1 );
	mccEvent.iItcEvent = EMceItcNotAssigned;
	actionSet.ClientMediaStateChangedL( &mccEvent , eventCode );
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ENone )
	EUNIT_ASSERT( iStorage->iMMObject == NULL )
	
	// It must not try to pause if there's no media stream
	mccEvent.iItcEvent = EMceItcStateChanged;
	mccEvent.iStream = NULL;
	actionSet.ClientMediaStateChangedL( &mccEvent , eventCode );
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ENone )
	EUNIT_ASSERT( iStorage->iMMObject == NULL )

	// It must not pause the stream if the event is not a EMceMediaError
	CMceComAudioStream* stream = CMceComAudioStream::NewLC();
	mccEvent.iStream = stream;
	actionSet.ClientMediaStateChangedL( &mccEvent , eventCode );
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ENone )
	EUNIT_ASSERT( iStorage->iMMObject == NULL )

	// It must not pause the stream if the MCC event's error code wasn't ICMP error
	eventCode = EMceMediaError;
	mccEvent.iError = KErrNone;
	actionSet.ClientMediaStateChangedL( &mccEvent , eventCode );
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ENone )
	EUNIT_ASSERT( iStorage->iMMObject == NULL )

	// It must not pause the stream if the stream doesn't have any sinks
	mccEvent.iError = KErrHostUnreach;
	actionSet.ClientMediaStateChangedL( &mccEvent , eventCode );
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ENone )
	EUNIT_ASSERT( iStorage->iMMObject == NULL )

	// It must not pause the stream if the stream doesn't have an RTP sink
	CMceComSpeakerSink* speakerSink = CMceComSpeakerSink::NewLC();
	stream->AddSinkL( speakerSink );
	CleanupStack::Pop( speakerSink );
	actionSet.ClientMediaStateChangedL( &mccEvent , eventCode );
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::ENone )
	EUNIT_ASSERT( iStorage->iMMObject == NULL )

	// It must pause the stream if the stream has an RTP sink
	CMceComRtpSink* rtpSink = CMceComRtpSink::NewLC();
	stream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );
	actionSet.ClientMediaStateChangedL( &mccEvent , eventCode );
	EUNIT_ASSERT( iStorage->iMediaManagerAction == CMCETls::EPauseSink )
	EUNIT_ASSERT( iStorage->iMMObject == rtpSink )

	CleanupStack::PopAndDestroy( stream );
	}
	
void UT_TMceActionSet::UT_TMceActionSet_ReceiveExtensionResponseL()
	{
	//Simulate Sending of INFO Arbitrary Request.
	_LIT8( KMceMethodINFO, "INFO");
  	TMceIds ids;
    TMceActionSet actionSet( *iSipSession );
	
	EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 0 );
	CMceMsgBase* msg = MCETestHelper::RequestMsg( KMceMethodINFO );
 	msg->PushL();
    CleanupStack::PushL( msg );
		
	actionSet.SendExtensionRequestL( *msg, ids );
	
	CleanupStack::PopAndDestroy( msg );
	EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 );
	
	EUNIT_ASSERT( ids.iTransactionID == 
		reinterpret_cast <TUint> ( iSipSession->PendingTransactions()[0] ) );
	

	//Creating a Response Received for the INFO request sent.			
	RPointerArray<CSIPHeaderBase> usrhdrs;
    CSIPHeaderBase::PushLC( &usrhdrs );
    MCETestHelper::AppendContactToUserHeaders( usrhdrs, KContactValue );

    CSIPClientTransaction* cltransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInfo, 
        usrhdrs, KMceSipOK, SipStrConsts::EPhraseOk, EFalse);
		
	actionSet.ReceiveExtensionResponseL( *cltransaction, *iSipSession->iBody);
	CleanupStack::PopAndDestroy( cltransaction );
	CleanupStack::PopAndDestroy( &usrhdrs );
	}
     
void UT_TMceActionSet::UT_TMceActionSet_ReceiveExtensionRequestL()
	{
  	TMceIds ids;
    TMceActionSet actionSet( *iSipSession );
	
	// No Pending Recevied Request yet
	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 0 );
  	
  	CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInfo, ETrue );
    
    CleanupStack::PushL( srvtransaction );

   	TMceStateTransitionEvent event( *iSipSession, EMceRequest );
	iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
	CleanupStack::Pop( srvtransaction );
    actionSet.ReceiveExtensionRequestL( event );	
	
	// INFO as a Pending Request Received
	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 1 );
	
	ids.iTransactionID = 
    reinterpret_cast <TUint> ( iSipSession->PendingReceivedRequests()[0] );
    
    
    //Test 2: Transition Event with dialog info:
 	CSIPServerTransaction* srvtransaction1 = 
    MCETestHelper::ServerTransactionL( SipStrConsts::EInfo, ETrue );
    
    CleanupStack::PushL( srvtransaction1 );

   	TMceStateTransitionEvent event1( *iSipSession, EMceRequest, iSipSession->Dialog()->Dialog() );
	iSipSession->iPendingReceivedRequests.AppendL( srvtransaction1 );
	CleanupStack::Pop( srvtransaction1 );
    actionSet.ReceiveExtensionRequestL( event1 );	
	
	// INFO as a Pending Request Received
	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 2 );
	
	ids.iTransactionID = 
    reinterpret_cast <TUint> ( iSipSession->PendingReceivedRequests()[1] );
	}
	 
void UT_TMceActionSet::UT_TMceActionSet_SendExtensionRequestL()
	{
	_LIT8( KMceMethodINFO, "INFO");
  	TMceIds ids;
    TMceActionSet actionSet( *iSipSession );
	
	EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 0 );
	CMceMsgBase* msg = MCETestHelper::RequestMsg( KMceMethodINFO );
 	msg->PushL();
    CleanupStack::PushL( msg );
		
	actionSet.SendExtensionRequestL( *msg, ids );
	EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 );
	
	EUNIT_ASSERT( ids.iTransactionID == 
		reinterpret_cast <TUint> ( iSipSession->PendingTransactions()[0] ) );
	
	CleanupStack::PopAndDestroy( msg );
	}
	
	 
void UT_TMceActionSet::UT_TMceActionSet_SendExtensionResponseL()
	{
	TUint32 code = 200;
  	TMceIds ids;
    TMceActionSet actionSet( *iSipSession );
	
	// No Pending Recevied Request yet
	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 0 );
  	
  	CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInfo, ETrue );
    
    CleanupStack::PushL( srvtransaction );
    
   	TMceStateTransitionEvent event( *iSipSession, EMceRequest );
	iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
	CleanupStack::Pop( srvtransaction );
    actionSet.ReceiveExtensionRequestL( event );	
	
	// INFO as a Pending Request Received
	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 1 );
	
	ids.iTransactionID = 
    reinterpret_cast <TUint> ( iSipSession->PendingReceivedRequests()[0] );
	
	//Creating a Response
	CMceMsgBase* msgResponse = MCETestHelper::ReplyMsg( code );
 	msgResponse->PushL();
    CleanupStack::PushL( msgResponse );
    
   	//Sending a Response for the received request with the same transaction ID
	actionSet.SendExtensionResponseL( *msgResponse,ids );	
	
	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 1 );
	
	CleanupStack::PopAndDestroy( msgResponse );
	}
	

void UT_TMceActionSet::UT_TMceActionSet_ReceiveExtensionErrorL()
	{
	_LIT8( KMceMethodINFO, "INFO");
	TInt timeOutError = KErrTimedOut;
    TMceActionSet actionSet( *iSipSession );

	//Test1: Pending Server Transaction 
	// No Pending Recevied Request yet
	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 0 );
  	
  	CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInfo, ETrue );
    
    CleanupStack::PushL( srvtransaction );

	TMceStateTransitionEvent event( *iSipSession, EMceRequest );
	iSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
	CleanupStack::Pop( srvtransaction );
    actionSet.ReceiveExtensionRequestL( event );	

	EUNIT_ASSERT( iSipSession->PendingReceivedRequests().Count() == 1 );
	// Simulate that server transaction times out
	actionSet.ReceiveExtensionError( *srvtransaction, timeOutError);
	
	
	//Test2: Pending Client Transaction

  	TMceIds ids;
	EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 0 );
	CMceMsgBase* msg = MCETestHelper::RequestMsg( KMceMethodINFO );
 	msg->PushL();
    CleanupStack::PushL( msg );
		
	actionSet.SendExtensionRequestL( *msg, ids );
	EUNIT_ASSERT( iSipSession->PendingTransactions().Count() == 1 );
	
	EUNIT_ASSERT( ids.iTransactionID == 
		reinterpret_cast <TUint> ( iSipSession->PendingTransactions()[0] ) );
		
	//Simulate client transaction timesout.	
	actionSet.ReceiveExtensionError(*iSipSession->PendingTransactions()[0], timeOutError);
	
	CleanupStack::PopAndDestroy( msg );
	}

void UT_TMceActionSet::UT_TMceActionSet_IsExtensionRequest()
	{

	TMceActionSet actionSet( *iSipSession );
	TBool iExtensionRequest = ETrue;
	
	_LIT8( KInvite, "INVITE" );
	_LIT8( KInfo, "INFO" );
	
    RStringF invite = SIPStrings::Pool().OpenFStringL( KInvite );
    CleanupClosePushL( invite );
	
	EUNIT_ASSERT( actionSet.IsExtensionRequest( invite ) == EFalse );
    CleanupStack::PopAndDestroy(); // invite
    
    RStringF info = SIPStrings::Pool().OpenFStringL( KInfo );
    CleanupClosePushL( info );

	EUNIT_ASSERT( actionSet.IsExtensionRequest( info ) == ETrue );
    CleanupStack::PopAndDestroy(); // info

	}

	
	

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceActionSet,
    "UT_TMceActionSet",
    "UNIT" )

EUNIT_TEST(
    "UpdateBodyL - test ",
    "TMceActionSet",
    "UpdateBodyL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_UpdateBodyLL, Teardown)


EUNIT_TEST(
    "CreateBodyCandidateL - test ",
    "TMceActionSet",
    "CreateBodyCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_CreateBodyCandidateL, Teardown)

EUNIT_TEST(
    "ProcessReliableResponseL - test ",
    "TMceActionSet",
    "ProcessReliableResponseL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessReliableResponseL, Teardown)

EUNIT_TEST(
    "ProcessSessionTimerServerL - test ",
    "TMceActionSet",
    "ProcessSessionTimerServerL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessSessionTimerServerL_1L, Teardown)

EUNIT_TEST(
    "ProcessSessionTimerServerL - test ",
    "TMceActionSet",
    "ProcessSessionTimerServerL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessSessionTimerServerL_2L, Teardown)

EUNIT_TEST(
    "ProcessSessionTimerServerL - test ",
    "TMceActionSet",
    "ProcessSessionTimerServerL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessSessionTimerServerL_3L, Teardown)


EUNIT_TEST(
    "ProcessSessionTimerClientL - test ",
    "TMceActionSet",
    "ProcessSessionTimerClientL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessSessionTimerClientL_1L, Teardown)


EUNIT_TEST(
    "ProcessSessionTimerClientL - test ",
    "TMceActionSet",
    "ProcessSessionTimerClientL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessSessionTimerClientL_2L, Teardown)

EUNIT_TEST(
    "ProcessSessionTimerClientL - test ",
    "TMceActionSet",
    "ProcessSessionTimerClientL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessSessionTimerClientL_3L, Teardown)

EUNIT_TEST(
    "AddSessionTimerFieldsClientL - test ",
    "TMceActionSet",
    "AddSessionTimerFieldsClientL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_AddSessionTimerFieldsClientL_1L, Teardown)

EUNIT_TEST(
    "SendRejectOfferWithWarningL - test ",
    "TMceActionSet",
    "SendRejectOfferWithWarningL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_SendRejectOfferWithWarningLL, Teardown)


EUNIT_TEST(
    "CheckContactIsSecureL - test ",
    "TMceActionSet",
    "CheckContactIsSecureL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_CheckContactIsSecureLL, Teardown)
    
EUNIT_TEST(
    "CheckContactIsSecure2L - test ",
    "TMceActionSet",
    "CheckContactIsSecure2L",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_CheckContactIsSecureL2L, Teardown)    

EUNIT_TEST(
    "ProcessSessionTimerServerL - test ",
    "TMceActionSet",
    "ProcessSessionTimerServerL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessSessionTimerServerLL, Teardown)    

EUNIT_TEST(
    "ProcessSessionTimerServerL - test ",
    "TMceActionSet",
    "ProcessSessionTimerServerL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ProcessSessionTimerServerL2L, Teardown)

EUNIT_TEST(
    "AddSessionTimerFieldsClientL - test ",
    "TMceActionSet",
    "AddSessionTimerFieldsClientL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_AddSessionTimerFieldsClientLL, Teardown)    

EUNIT_TEST(
    "ClientMediaStateChanged - test ",
    "TMceActionSet",
    "ClientMediaStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ClientMediaStateChangedL, Teardown)

EUNIT_TEST(
    "ResponseReceivedL - test ",
    "TMceActionSet",
    "ResponseReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ReceiveExtensionResponseL, Teardown)
    
EUNIT_TEST(
    "RequestReceivedL - test ",
    "TMceActionSet",
    "RequestReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ReceiveExtensionRequestL, Teardown)


EUNIT_TEST(
    "SendSessionRequestL - test ",
    "TMceActionSet",
    "SendSessionRequestL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_SendExtensionRequestL, Teardown)


EUNIT_TEST(
    "SendSessionResponseL - test ",
    "TMceActionSet",
    "SendSessionResponseL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_SendExtensionResponseL, Teardown)
    
EUNIT_TEST(
    "ReceiveExtensionErrorL - test ",
    "TMceActionSet",
    "ReceiveExtensionErrorL",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_ReceiveExtensionErrorL, Teardown)


EUNIT_TEST(
    "IsExtensionRequest - test ",
    "TMceActionSet",
    "IsExtensionRequest",
    "FUNCTIONALITY",
    SetupL, UT_TMceActionSet_IsExtensionRequest, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
