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
#include "ut_cstunclient.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <commdbconnpref.h>
#include <e32debug.h>

//  INTERNAL INCLUDES
#include "natfwstunclient.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunittestmacros.h"
#include "ut_cstunbinding.h"

_LIT8( KServerAddress, "10.32.194.251" );
_LIT8( KUsername, "userAA" );
_LIT8( KPassword,"salasana");
_LIT8( KRealm, "example.realm.com" );

_LIT8(KRealmVal,"\"myrealm\"");

const TDesC8& dataDesc4(KRealmVal);

// CONSTRUCTION
ut_cstunclient* ut_cstunclient::NewL()
    {
    ut_cstunclient* self = ut_cstunclient::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstunclient* ut_cstunclient::NewLC()
    {
    ut_cstunclient* self = new( ELeave ) ut_cstunclient();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstunclient::~ut_cstunclient()
    {
    }

// Default constructor
ut_cstunclient::ut_cstunclient()
    {
    }

// Second phase construct
void ut_cstunclient::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
// from mstunclientbserver
void ut_cstunclient::STUNClientInitCompleted( const CSTUNClient& /*aClient*/,
                              TInt aCompletionCode )
    {
    iCallbackCalled = ETrue;
    RDebug::Print( _L("**NATFW STUNClientInitCompleted, code: %d"), aCompletionCode );
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
       {
       ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
       }
    }


void ut_cstunclient::STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                const CBinding& /*aBinding*/ )
    {
    iCallbackCalled = ETrue;
    RDebug::Print( _L("**NATFW STUNBindingEventOccurredL, code: %d"), aEvent );
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
       {
       ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
       }
    }


void ut_cstunclient::STUNBindingErrorOccurred( const CBinding& /*aBinding*/, 
                               TInt aError )
    {
    iCallbackCalled = ETrue;
    RDebug::Print( _L("**NATFW STUNBindingErrorOccurred, code: %d"), aError );
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
       {
       ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
       }
    }

void ut_cstunclient::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }
    
void ut_cstunclient::IcmpError( TUint /*aSessionId*/, TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ )
    {
    RDebug::Print( _L("**NATFW IcmpError") );
    }


void ut_cstunclient::SetupL(  )
    {
    iCallbackCalled = EFalse;
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    iDeltatimer = CDeltaTimer::NewL( 1 );
    User::LeaveIfError( iSocketServ.Connect() );
    iStreamId = 1;
    iSubstreamId = 1;
    TBool icmpUsed( EFalse );
    
    iMux = CNcmConnectionMultiplexer::NewL( *this );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );
    
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    iConnection.Start( pref, iStatus );
    User::WaitForRequest( iStatus );
    User::LeaveIfError( iStatus.Int() );    
    
    iStunclient = CSTUNClient::NewL( retransmitInterval,
                                     serveraddress,
                                     serverport,
                                     protocol,
                                     iSocketServ,
                                     iConnection,
                                     *iDeltatimer,
                                     *this,
                                     obtainsharedsecret,
                                     failifnoSRVrecordfound,
                                     icmpUsed,
                                     iMux );
                                     
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
       {
       ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
       }
    ut_cstunbinding::iActiveSchedulerWait.Start();    
    }

void ut_cstunclient::Teardown(  )
    {
    delete iStunclient;
    iStunclient = NULL;
    iSocket.Close();
    iConnection.Stop();
    iConnection.Close();
    iSocketServ.Close();
    delete iServeraddress;
    iServeraddress = NULL;
    delete iResponse;
    iResponse = NULL;
    delete iRespDataIndication;
    iRespDataIndication = NULL;
    delete iDeltatimer;
    iDeltatimer = NULL;
    delete iMux;
    iMux = NULL;
    }

void ut_cstunclient::UT_CSTUNClient_NewLL(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    iStreamId = 1;
    iSubstreamId = 1;
    TBool icmpUsed( EFalse );    
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    CSTUNClient* stunClient = NULL;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( stunClient = CSTUNClient::NewL(
        retransmitInterval,
        serveraddress,
        serverport,
        protocol,
        iSocketServ,
        iConnection,
        *iDeltatimer,
        *this,
        obtainsharedsecret,
        failifnoSRVrecordfound,
        icmpUsed,
        iMux ) );
                            
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
       {
       ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
       }
    ut_cstunbinding::iActiveSchedulerWait.Start();

    delete stunClient;        
    }

void ut_cstunclient::UT_CSTUNClient_NewL_1L(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    iStreamId = 1;
    iSubstreamId = 1;
    TBool icmpUsed( EFalse );    
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    CSTUNClient* stunClient = NULL; 
    NATFW_EUNIT_ASSERT_NO_LEAVE( stunClient = CSTUNClient::NewL( retransmitInterval,
                                     *iDeltatimer,
                                     *this,
                                     iMux ) );
                                     
    delete stunClient;
    }

// STUN Relay case    
void ut_cstunclient::UT_CSTUNClient_NewL_2L(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<10> service( KStunRelay );
    TBool obtainsharedsecret( ETrue );
    TBool failifnoSRVrecordfound( EFalse );
    iStreamId = 1;
    iSubstreamId = 1;
    TBool icmpUsed( EFalse );    
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    CSTUNClient* stunClient = NULL; 
    NATFW_EUNIT_ASSERT_NO_LEAVE( stunClient = CSTUNClient::NewL( retransmitInterval,
                                     serveraddress,
                                     serverport,
                                     service,
                                     iSocketServ,
                                     iConnection,
                                     *iDeltatimer,
                                     *this,
                                     obtainsharedsecret,
                                     failifnoSRVrecordfound,
                                     icmpUsed,
                                     iMux ) );
                                     
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
       {
       ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
       }
    ut_cstunbinding::iActiveSchedulerWait.Start();

    delete stunClient;    
    }    

void ut_cstunclient::UT_CSTUNClient_NewLCL(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    iStreamId = 1;
    iSubstreamId = 1;
    TBool icmpUsed( EFalse );    
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    CSTUNClient* stunClient = NULL; 
    stunClient = CSTUNClient::NewLC( retransmitInterval,
                                     serveraddress,
                                     serverport,
                                     protocol,
                                     iSocketServ,
                                     iConnection,
                                     *iDeltatimer,
                                     *this,
                                     obtainsharedsecret,
                                     failifnoSRVrecordfound,
                                     icmpUsed,
                                     iMux );
                                     
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
       {
       ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
       }
    ut_cstunbinding::iActiveSchedulerWait.Start();
    
    CleanupStack::PopAndDestroy( stunClient );  
    }

void ut_cstunclient::UT_CSTUNClient_NewLC_1L(  )
    {
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    iStreamId = 1;
    iSubstreamId = 1;
    TBool icmpUsed( EFalse );    
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    CSTUNClient* stunClient = CSTUNClient::NewLC( retransmitInterval,
                                                 *iDeltatimer,
                                                 *this,
                                                 iMux );
                                     
    CleanupStack::PopAndDestroy( stunClient ); 
    }

void ut_cstunclient::UT_CSTUNClient_IsInitializedL(  )
    {
    TBool initialized = iStunclient->IsInitialized();
    }

void ut_cstunclient::UT_CSTUNClient_STUNServerAddrLL(  )
    {
    TInetAddr addr = iStunclient->STUNServerAddrL();
    }

void ut_cstunclient::UT_CSTUNClient_SetCredentialsLL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<6> usernameA( KUsername );
    iStunclient->SetCredentialsL( usernameA, password );
    
    iStunclient->SetCredentialsL( usernameA, password );
    }

void ut_cstunclient::UT_CSTUNClient_SharedSecretObtainedL(  )
    {
    TBool sharedSecretused = iStunclient->SharedSecretObtained();
    }

/*void ut_cstunclient::UT_CSTUNClient_SetSTUNServerAddrLL(  )
    {
    TInetAddr addr;
    iStunclient->SetSTUNServerAddrL( addr );
    }*/

void ut_cstunclient::UT_CSTUNClient_ObtainTransactionIDLL(  )
    {
    TNATFWUNSAFTransactionID transactionID;   
    iStunclient->ObtainTransactionIDL( transactionID );
    }

void ut_cstunclient::UT_CSTUNClient_ImplementationL(  )
    {
    CSTUNClientImplementation& impl = iStunclient->Implementation();
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cstunclient,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CSTUNClient",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_NewLL, Teardown)
/*
EUNIT_TEST(
    "NewL - test ",
    "CSTUNClient",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_NewL_1L, Teardown)

EUNIT_TEST(
    "NewL_2 - test ",
    "CSTUNClient",
    "NewL_2",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_NewL_2L, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CSTUNClient",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_NewLCL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CSTUNClient",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_NewLC_1L, Teardown)

EUNIT_TEST(
    "IsInitialized - test ",
    "CSTUNClient",
    "IsInitialized",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_IsInitializedL, Teardown)

EUNIT_TEST(
    "STUNServerAddrL - test ",
    "CSTUNClient",
    "STUNServerAddrL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_STUNServerAddrLL, Teardown)

EUNIT_TEST(
    "SetCredentialsL - test ",
    "CSTUNClient",
    "SetCredentialsL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_SetCredentialsLL, Teardown)

EUNIT_TEST(
    "SharedSecretObtained - test ",
    "CSTUNClient",
    "SharedSecretObtained",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_SharedSecretObtainedL, Teardown)
*/
/*EUNIT_TEST(
    "SetSTUNServerAddrL - test ",
    "CSTUNClient",
    "SetSTUNServerAddrL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_SetSTUNServerAddrLL, Teardown)*/
/*
EUNIT_TEST(
    "ObtainTransactionIDL - test ",
    "CSTUNClient",
    "ObtainTransactionIDL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_ObtainTransactionIDLL, Teardown)

EUNIT_TEST(
    "Implementation - test ",
    "CSTUNClient",
    "Implementation",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNClient_ImplementationL, Teardown)

*/
EUNIT_END_TEST_TABLE

//  END OF FILE
