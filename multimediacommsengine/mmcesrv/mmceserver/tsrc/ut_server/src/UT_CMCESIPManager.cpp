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
#include "ut_cmcesipmanager.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sipmanagedprofile.h>


//  INTERNAL INCLUDES
#include "MCETestHelper.h"

#include "mceactionset.h"
#include "mcesipsession.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "mcecomsession.h"


#define MCE_INCOMING_REQUEST_POSTCONDITION \
	MCE_CHECK_MEMORY_LEAVE( iStorage->iSipSentResponse && \
	iStorage->iSipSentResponse->StatusCode() == KMceSipServerInternalError )

// CONSTRUCTION
UT_CMceSipManager* UT_CMceSipManager::NewL()
    {
    UT_CMceSipManager* self = UT_CMceSipManager::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceSipManager* UT_CMceSipManager::NewLC()
    {
    UT_CMceSipManager* self = new( ELeave ) UT_CMceSipManager();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceSipManager::~UT_CMceSipManager()
    {
    }

// Default constructor
UT_CMceSipManager::UT_CMceSipManager()
    {
    }

// Second phase construct
void UT_CMceSipManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceSipManager::SetupL(  )
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    
    iIds.iAppUID = 1;
    
    
    iStorage = CMCETls::Storage();
    iServerCore = CMceServerCore::NewL();
    iSession = CMceCsSession::NewL( *iServerCore );
    iSession->SetClientUidL( iIds, RMessage2() );
    
    iManager = &iServerCore->Manager();
    
    iManager->ConnectionL( iManager->ProfileL( 1 ) );
    
    } 

void UT_CMceSipManager::Teardown(  )
    {
    CMCETls::Close();
   	delete iSession;
    delete iServerCore;

    SdpCodecStringPool::Close();

    }


void UT_CMceSipManager::UT_CMceSipManager_ProfileL_1_L()
    {
    TInt error = KErrNone;
    
  	CSIPProfile& profile = iManager->ProfileL( 1 );
  	EUNIT_ASSERT( &profile != NULL );
  	
  	TRAP( error, CSIPProfile& profile1 = iManager->ProfileL( 100 ) );
  	EUNIT_ASSERT( error != KErrNone );
  	
  	CSIPProfile& profile1 = iManager->ProfileL( 1 );
  	EUNIT_ASSERT( &profile1 != NULL );
  	
  	TRAP( error, CSIPProfile& profile2 = iManager->ProfileL( 100 ) );
  	EUNIT_ASSERT( error != KErrNone );
  	
    }


void UT_CMceSipManager::UT_CMceSipManager_ProfileL_2_L()
    {
    TInt error = KErrNone;
    
    CSIPAddress* defAor = CSIPAddress::DecodeL( KSIPProfileDefAOR );
    CleanupStack::PushL( defAor );
    
    CSIPAddress* reqUri = CSIPAddress::DecodeL( KRequestUri );
    CleanupStack::PushL( reqUri );
    
    CSIPAddress* forwarded = CSIPAddress::DecodeL( KForwarded );
    CleanupStack::PushL( forwarded );
    
    // Matches with existing profile by to header
    
    CSIPProfile& profile1 = iManager->ProfileL( 1, reqUri->Uri8() );
  	EUNIT_ASSERT( &profile1 != NULL );
    EUNIT_ASSERT( iManager->iProfiles.Count() == 1 );

    // Matches with existing profile by request uri
    CSIPProfile& profile2 = iManager->ProfileL( 1, reqUri->Uri8() );
  	EUNIT_ASSERT( &profile2 != NULL );
    EUNIT_ASSERT( iManager->iProfiles.Count() == 1 );
    
    
    // Matches with not existing profile by to header
    iManager->iProfiles.ResetAndDestroy();
    CSIPProfile& profile3 = iManager->ProfileL( 1, reqUri->Uri8() );
  	EUNIT_ASSERT( &profile3 != NULL );
  	EUNIT_ASSERT( iManager->iProfiles.Count() == 1 );
    
    // Matches with not existing profile by request uri
    iManager->iProfiles.ResetAndDestroy();
    CSIPProfile& profile4 = iManager->ProfileL( 1,  reqUri->Uri8() );
  	EUNIT_ASSERT( &profile4 != NULL );
    EUNIT_ASSERT( iManager->iProfiles.Count() == 1 );
    
    // Does not match
    TRAP( error, CSIPProfile& profile5 = iManager->ProfileL( 1, forwarded->Uri8() ) );
  	EUNIT_ASSERT( error != KErrNone );
  	
    CleanupStack::PopAndDestroy( forwarded );
    CleanupStack::PopAndDestroy( reqUri );
    CleanupStack::PopAndDestroy( defAor );
    }

void UT_CMceSipManager::UT_CMceSipManager_ConnectionLL()
    {
  	CSIPProfile& profile = iManager->ProfileL( 1 );
    CMceSipConnection& connection = iManager->ConnectionL( profile );
  	EUNIT_ASSERT( &connection != NULL );
    }

void UT_CMceSipManager::UT_CMceSipManager_IncomingRequestL()
    {
    
    CSIPServerTransaction* srvtransaction =
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
 
   	EUNIT_ASSERT( iManager->iClients[0]->Implementation().SubSessions().Count() == 0 );

  	iManager->IncomingRequest( 1, srvtransaction );
  	MCE_INCOMING_REQUEST_POSTCONDITION

  	iStorage->ExecuteCallback();
  	MCE_INCOMING_REQUEST_POSTCONDITION
  	
   	EUNIT_ASSERT( iManager->iClients[0]->Implementation().SubSessions().Count() == 1 );

    srvtransaction =
        MCETestHelper::ServerTransactionL( SipStrConsts::EMessage, EFalse );
 
  	iManager->IncomingRequest( 1, srvtransaction );  	
   	EUNIT_ASSERT( iManager->iClients[0]->Implementation().SubSessions().Count() == 1 );
   	
   	
    srvtransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite )); 
    CleanupStack::PushL( srvtransaction );
    
  	CUri8* uri = CUri8::NewLC(); //OK 
    MCE_SET_URIL( uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );

    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );
    
    requestelements->SetFromHeaderL(fromheader); 
    CleanupStack::Pop( fromheader );
    
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    srvtransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    CleanupStack::Pop( srvtransaction );
    
  	iManager->IncomingRequest( 1, srvtransaction );  	
   	EUNIT_ASSERT( iManager->iClients[0]->Implementation().SubSessions().Count() == 1 );
    }

void UT_CMceSipManager::UT_CMceSipManager_TimedOutL()
    {
    CSIPServerTransaction* trx = NULL;
    iManager->TimedOut( *trx );

    }

void UT_CMceSipManager::UT_CMceSipManager_ProfileRegistryEventOccurredL()
    {

    iManager->ProfileRegistryEventOccurred( 1, MSIPProfileRegistryObserver::EProfileUpdated );
  	
    }

void UT_CMceSipManager::UT_CMceSipManager_ProfileRegistryErrorOccurredL()
    {
    
    iManager->ProfileRegistryErrorOccurred( 1, KErrServerTerminated );
  	
    }
    
void UT_CMceSipManager::UT_CMceSipManager_MediaErrorL()
    {
    
    iManager->MediaError( KErrServerTerminated );
  	
    }

void UT_CMceSipManager::UT_CMceSipManager_NextDialogIdL()
    {

    iManager->iNextDialogId = 0;
  	EUNIT_ASSERT( iManager->NextDialogId() == 1 );
  	EUNIT_ASSERT( iManager->NextDialogId() == 2 );
  	EUNIT_ASSERT( iManager->NextDialogId() == 3 );
  	EUNIT_ASSERT( iManager->NextDialogId() == 4 );
    
  	
    }


void UT_CMceSipManager::UT_CMceSipManager_UnRegisterClientL()
    {
  	EUNIT_ASSERT( iManager->iClients.Count() == 1 );
    
    CMceCsSession* session = CMceCsSession::NewL( *iServerCore );
    CleanupStack::PushL( session );
    iIds.iAppUID = 2;
    session->SetClientUidL( iIds, RMessage2() );

    CMceCsSession* session2 = CMceCsSession::NewL( *iServerCore );
    CleanupStack::PushL( session2 );
    iIds.iAppUID = 3;
    session2->SetClientUidL( iIds, RMessage2() );


  	EUNIT_ASSERT( iManager->iClients.Count() == 3 );
  	
    CleanupStack::PopAndDestroy( session2 );
    
  	EUNIT_ASSERT( iManager->iClients.Count() == 2 );
  	
  	session = iManager->iClients[1];
    iManager->UnRegisterClient( *session );
  	EUNIT_ASSERT( iManager->iClients.Count() == 1 );
  	
    CleanupStack::PopAndDestroy( session );
    
  	EUNIT_ASSERT( iManager->iClients.Count() == 1 );
  	
  	
    }

void UT_CMceSipManager::UT_CMceSipManager_CreateSubSessionL_1L()
    {
    
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceCsSubSession* session = 
        iManager->CreateSubSessionL( *iSession, iIds, *params );
    
    CleanupStack::PopAndDestroy( params );
    CleanupStack::PushL( session );
  	EUNIT_ASSERT( session != NULL );
  	EUNIT_ASSERT( session->Dialog() != NULL );
  	EUNIT_ASSERT( session->DialogId() > 0 );
  	EUNIT_ASSERT( &session->SIPConnection() != NULL );
  	EUNIT_ASSERT( session->SIPConnection().iSessions.Count() == 1 );
  	
    CleanupStack::PopAndDestroy( session ); 	
    }

void UT_CMceSipManager::UT_CMceSipManager_CreateSubSessionL_2L()
    {
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceCsSubSession* session = 
        iManager->CreateSubSessionL( *iSession, iIds, *params );
    
    CleanupStack::PopAndDestroy( params );
    CleanupStack::PushL( session );
  	EUNIT_ASSERT( session != NULL );
  	EUNIT_ASSERT( session->Dialog() != NULL );
  	EUNIT_ASSERT( session->DialogId() > 0 );
  	EUNIT_ASSERT( &session->SIPConnection() != NULL );
  	EUNIT_ASSERT( session->SIPConnection().iSessions.Count() == 1 );
  	
    CleanupStack::PopAndDestroy( session ); 	
    }

void UT_CMceSipManager::UT_CMceSipManager_CreateSubSessionL_3L()
    {
    CMceSipConnection& connection = iManager->ConnectionL( iManager->ProfileL( 1 ) );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( srvtransaction );    
    iManager->CreateSubSessionL( srvtransaction, *iSession, connection );
    CleanupStack::Pop( srvtransaction );
    }

void UT_CMceSipManager::UT_CMceSipManager_CreateSubSessionNoProfileL()
    {
    
    iIds.iProfileID = 0;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
	params->AppendL( _L8("6") );
    
    CMceCsSubSession* session = 
        iManager->CreateSubSessionL( *iSession, iIds, *params );
    
    CleanupStack::PopAndDestroy( params );
    CleanupStack::PushL( session );
  	EUNIT_ASSERT( session != NULL );
  	EUNIT_ASSERT( session->Dialog() != NULL );
  	EUNIT_ASSERT( session->DialogId() > 0 );
  	EUNIT_ASSERT( &session->SIPConnection() != NULL );
  	EUNIT_ASSERT( session->SIPConnection().iSessions.Count() == 1 );
  	
    CleanupStack::PopAndDestroy( session ); 	
    }

void UT_CMceSipManager::UT_CMceSipManager_UnRegisterSubSessionL()
    {
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    
    CMceSipConnection& connection = iManager->ConnectionL( iManager->ProfileL( 1 ) );

    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    TInt i = 0;
    for( i = 0;i<4;i++ )
        {
        CMceCsSubSession* session = 
            iManager->CreateSubSessionL( *iSession, iIds, *params );
        CleanupStack::PushL( session );
      	EUNIT_ASSERT( &session->SIPConnection() == &connection );
      	EUNIT_ASSERT( connection.iSessions.Count() == 1+i );            
        }
        
    for( i = 0;i<4;i++ )
        {
      	EUNIT_ASSERT( connection.iSessions.Count() == 4-i );            
        CleanupStack::PopAndDestroy();
        }
	  //After all CMceCsSubSession have been delete, all profile has been delete
	  EUNIT_ASSERT( iManager->iProfiles.Count() == 0 );
    CleanupStack::PopAndDestroy( params );
  	
    }

void UT_CMceSipManager::UT_CMceSipManager_UnRegisterSubSessionNoProfileL()
    {
    iIds.iProfileID = 0;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    
    CMceSipConnection& connection = iManager->ConnectionL( 6 );

    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
	params->AppendL( _L8("6") );
    TInt i = 0;
    for( i = 0;i<4;i++ )
        {
        CMceCsSubSession* session = 
            iManager->CreateSubSessionL( *iSession, iIds, *params );
        CleanupStack::PushL( session );
      	EUNIT_ASSERT( &session->SIPConnection() == &connection );
      	EUNIT_ASSERT( connection.iSessions.Count() == 1+i );            
        }
        
    for( i = 0;i<4;i++ )
        {
      	EUNIT_ASSERT( connection.iSessions.Count() == 4-i );            
        CleanupStack::PopAndDestroy();
        }

    CleanupStack::PopAndDestroy( params );
  	
    }

void UT_CMceSipManager::UT_CMceSipManager_MediaManagerL()
    {
  	EUNIT_ASSERT( &iManager->MediaManager() != NULL );
  	
    }

void UT_CMceSipManager::UT_CMceSipManager_SIPClientL()
    {
  	EUNIT_ASSERT( &iManager->SIPClient() != NULL );
    }

void UT_CMceSipManager::UT_CMceSipManager_OrphanSubSessionCountL()
    {
  	EUNIT_ASSERT( iManager->OrphanSubSessionCount() == 0 );
    }

void UT_CMceSipManager::UT_CMceSipManager_UpdateProfileToSubSessionsL()
    {
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceCsSubSession* subsession = 
        iManager->CreateSubSessionL( *iSession, iIds, *params );
    CleanupStack::PushL( subsession );
    
    CSIPProfile& profileOld = subsession->Profile();
  	EUNIT_ASSERT( &profileOld != NULL );
    CSIPProfile& profileNew = iManager->ProfileL( 2 );
  	EUNIT_ASSERT( &profileNew != NULL );

  	CMceCsSessionImplementation& session = iManager->iClients[ 0 ]->Implementation();
  	RPointerArray<CMceCsSubSession>& subSessions = session.SubSessions();
  	subSessions.AppendL(subsession);
  	
  	iManager->UpdateProfileToSubSessions(profileNew, profileOld);
  	EUNIT_ASSERT(&subsession->Profile() == &profileNew)
  	subSessions.Remove(0);

  	iManager->AddOrphanSubSessionL( subsession );
  	iManager->UpdateProfileToSubSessions(profileOld, profileNew);
  	RPointerArray<CMceCsSubSession>& orphanSubSessions = iManager->iOrphanSubSessions->SubSessions();
  	EUNIT_ASSERT(&subsession->Profile() == &profileOld)

  	CleanupStack::Pop( subsession );
  	CleanupStack::PopAndDestroy( params );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceSipManager,
    "UT_CMceSipManager",
    "UNIT" )

EUNIT_TEST(
    "ProfileL - test ",
    "CMceSipManager",
    "ProfileL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_ProfileL_1_L, Teardown)

EUNIT_TEST(
    "ProfileL - test ",
    "CMceSipManager",
    "ProfileL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_ProfileL_2_L, Teardown)

EUNIT_TEST(
    "ConnectionL - test ",
    "CMceSipManager",
    "ConnectionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_ConnectionLL, Teardown)


EUNIT_TEST(
    "IncomingRequest - test ",
    "CMceSipManager",
    "IncomingRequest",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_IncomingRequestL, Teardown)

EUNIT_TEST(
    "TimedOut - test ",
    "CMceSipManager",
    "TimedOut",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_TimedOutL, Teardown)

EUNIT_TEST(
    "ProfileRegistryEventOccurred - test ",
    "CMceSipManager",
    "ProfileRegistryEventOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_ProfileRegistryEventOccurredL, Teardown)

EUNIT_TEST(
    "ProfileRegistryErrorOccurred - test ",
    "CMceSipManager",
    "ProfileRegistryErrorOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_ProfileRegistryErrorOccurredL, Teardown)

EUNIT_TEST(
    "MediaError - test ",
    "CMceSipManager",
    "MediaError",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_MediaErrorL, Teardown)

EUNIT_TEST(
    "NextDialogId - test ",
    "CMceSipManager",
    "NextDialogId",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_NextDialogIdL, Teardown)

EUNIT_TEST(
    "UnRegisterClient - test ",
    "CMceSipManager",
    "UnRegisterClient",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_UnRegisterClientL, Teardown)

EUNIT_TEST(
    "CreateSubSessionL - MO test ",
    "CMceSipManager",
    "CreateSubSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_CreateSubSessionL_1L, Teardown)

EUNIT_TEST(
    "CreateSubSessionL - MO test ",
    "CMceSipManager",
    "CreateSubSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_CreateSubSessionL_2L, Teardown)
EUNIT_TEST(
    "CreateSubSessionL - MT test ",
    "CMceSipManager",
    "CreateSubSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_CreateSubSessionL_3L, Teardown)

EUNIT_TEST(
    "CreateSubSessionL - MT test ",
    "CMceSipManager",
    "CreateSubSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_CreateSubSessionNoProfileL, Teardown)

EUNIT_TEST(
    "UnRegisterSubSession - test ",
    "CMceSipManager",
    "UnRegisterSubSession",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_UnRegisterSubSessionL, Teardown)

EUNIT_TEST(
    "UnRegisterSubSessionNoProfile - test ",
    "CMceSipManager",
    "UnRegisterSubSessionNoProfile",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_UnRegisterSubSessionNoProfileL, Teardown)

EUNIT_TEST(
    "MediaManager - test ",
    "CMceSipManager",
    "MediaManager",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_MediaManagerL, Teardown)

EUNIT_TEST(
    "SIPClient - test ",
    "CMceSipManager",
    "SIPClient",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_SIPClientL, Teardown)

EUNIT_TEST(
    "OrphanSubSessionCount - test ",
    "CMceSipManager",
    "OrphanSubSessionCount",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_OrphanSubSessionCountL, Teardown)

EUNIT_TEST(
    "UpdateProfileToSubSessions - test ",
    "CMceSipManager",
    "UpdateProfileToSubSessions",
    "FUNCTIONALITY",
    SetupL, UT_CMceSipManager_UpdateProfileToSubSessionsL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
