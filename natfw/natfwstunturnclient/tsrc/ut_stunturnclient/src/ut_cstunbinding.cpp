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
#include "ut_cstunbinding.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <commdbconnpref.h>
#include <e32debug.h>

//  INTERNAL INCLUDES
#include "natfwstunbinding.h"
#include "natfwstunclientdefs.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunittestmacros.h"

_LIT8( KServerAddress, "10.32.194.251" );
const TUint KDefaultRtoValue = 0;
const TBool KNoFingerprint = EFalse;
//const TBool KFingerprint = ETrue;

CActiveSchedulerWait ut_cstunbinding::iActiveSchedulerWait;

// CONSTRUCTION
ut_cstunbinding* ut_cstunbinding::NewL()
    {
    ut_cstunbinding* self = ut_cstunbinding::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cstunbinding* ut_cstunbinding::NewLC()
    {
    ut_cstunbinding* self = new( ELeave ) ut_cstunbinding();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cstunbinding::~ut_cstunbinding()
    {
    }

// Default constructor
ut_cstunbinding::ut_cstunbinding()
    {
    }

// Second phase construct
void ut_cstunbinding::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
// from mstunclientbserver
void ut_cstunbinding::STUNClientInitCompleted( const CSTUNClient& /*aClient*/,
                              TInt aCompletionCode )
    {
    iCallbackCalled = ETrue;
    RDebug::Print( _L("**NATFW STUNClientInitCompleted, code: %d"), aCompletionCode );
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
        {
        ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
        }
    }


void ut_cstunbinding::STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                const CBinding& /*aBinding*/ )
    {
    iCallbackCalled = ETrue;
    RDebug::Print( _L("**NATFW STUNBindingEventOccurredL, code: %d"), aEvent );
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
        {
        ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
        }
    }


void ut_cstunbinding::STUNBindingErrorOccurred( const CBinding& /*aBinding*/, 
                               TInt aError )
    {
    iCallbackCalled = ETrue;
    RDebug::Print( _L("**NATFW STUNBindingErrorOccurred, code: %d"), aError );
    if ( ut_cstunbinding::iActiveSchedulerWait.IsStarted() )
        {
        ut_cstunbinding::iActiveSchedulerWait.AsyncStop();
        }
    }

void ut_cstunbinding::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }
    
void ut_cstunbinding::IcmpError( TUint /*aSessionId*/, TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ )
    {
    RDebug::Print( _L("**NATFW IcmpError") );
    }


void ut_cstunbinding::SetupL(  )
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
    if ( !iCallbackCalled )
        {
        // Callback not called because of memory running out inside callback execution
        User::Leave( KErrNoMemory );
        }
    
    User::LeaveIfError( iSocket.Open( iSocketServ, KAfInet, KSockDatagram,
                                     KProtocolInetUdp ));

    iBinding = CSTUNBinding::NewL( *iStunclient, iSocket );
    }

void ut_cstunbinding::Teardown(  )
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
    delete iBinding;
    iBinding = NULL;
    delete iDeltatimer;
    iDeltatimer = NULL;
    delete iMux;
    iMux = NULL;
    }

void ut_cstunbinding::UT_CSTUNBinding_NewLL(  )
    {
    CSTUNClient* client = NULL;
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    TBool icmpUsed( EFalse );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    TRAPD( error, client = CSTUNClient::NewL( retransmitInterval,
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
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }
    CleanupStack::PushL( client );
    CSTUNBinding* binding = NULL;
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( binding = CSTUNBinding::NewL( 
        *client, iSocket ), KErrNotReady );
    CleanupStack::PopAndDestroy( client );
    }

void ut_cstunbinding::UT_CSTUNBinding_NewL_1L(  )
    {
    CSTUNClient* client = NULL;
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    TBool icmpUsed( EFalse );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    TRAPD( error, client = CSTUNClient::NewL( retransmitInterval,
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
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }
    CleanupStack::PushL( client );
    CSTUNBinding* binding = NULL;
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( binding = CSTUNBinding::NewL( 
        *client, iStreamId, iSubstreamId ), KErrNotReady );
    CleanupStack::PopAndDestroy( client );   
    }

void ut_cstunbinding::UT_CSTUNBinding_NewLCL(  )
    {
    CSTUNClient* client = NULL;
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    TBool icmpUsed( EFalse );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    TRAPD( error, client = CSTUNClient::NewL( retransmitInterval,
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
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }
    CleanupStack::PushL( client );
    CSTUNBinding* binding = CSTUNBinding::NewLC( *client, iSocket );      

    CleanupStack::PopAndDestroy( binding );
    CleanupStack::PopAndDestroy( client );
    }

void ut_cstunbinding::UT_CSTUNBinding_NewLC_1L(  )
    {
    CSTUNClient* client = NULL;
    TUint iapID( 6 );
    TInt retransmitInterval( 10 );
    TBufC8<13> serveraddress( KServerAddress );
    TUint serverport( 2000 );
    TBufC8<4> protocol( KStun );
    TBool obtainsharedsecret( EFalse );
    TBool failifnoSRVrecordfound( EFalse );
    TBool icmpUsed( EFalse );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );   
    
    TRAPD( error, client = CSTUNClient::NewL( retransmitInterval,
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
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }
    CleanupStack::PushL( client );
    CSTUNBinding* binding = CSTUNBinding::NewLC( *client, iStreamId, iSubstreamId );      
    
    CleanupStack::PopAndDestroy( binding );
    CleanupStack::PopAndDestroy( client );
    }

void ut_cstunbinding::UT_CSTUNBinding_SendRequestLL(  )
    {
    iBinding->SendRequestL();
    //CActiveScheduler::Start();
    }

void ut_cstunbinding::UT_CSTUNBinding_SendRequestL_1L(  )
    {
    TInetAddr addr;   
    iBinding->SendRequestL( addr, KNoFingerprint, KDefaultRtoValue );
    //CActiveScheduler::Start();
    }

void ut_cstunbinding::UT_CSTUNBinding_CancelRequestL(  )
    {
    iBinding->CancelRequest();
    }

void ut_cstunbinding::UT_CSTUNBinding_SocketL(  )
    {
    RSocket socket = iBinding->Socket();
    }

void ut_cstunbinding::UT_CSTUNBinding_AddressResolvedL(  )
    {
    TBool resolved = iBinding->AddressResolved();
    }

void ut_cstunbinding::UT_CSTUNBinding_PublicAddrL(  )
    {
    TInetAddr addr = iBinding->PublicAddr();
    }

void ut_cstunbinding::UT_CSTUNBinding_HandleDataLL(  )
    {
    _LIT8( KPassword,"aaaabbbbccccdddd");
    TBufC8<16> password( KPassword );
    _LIT8( KTransactionId,"aaaabbbb");
    TNATFWUNSAFTransactionID transactionID;
    transactionID.Copy( KTransactionId );    
    
    CNATFWUNSAFBindingRequest* request = CNATFWUNSAFBindingRequest::NewLC( transactionID );
    CBufBase* msg = request->EncodeL( password );
    CleanupStack::PushL( msg );        
    TInt length = msg->Size();
    HBufC8* readBuf1 = HBufC8::NewLC( length );
    TPtr8 writable1( readBuf1->Des() );
    msg->Read( 0, writable1 );
    
    TInetAddr remoteAddr;
    TBool consumed = EFalse;    
    HBufC8* ptr = iBinding->HandleDataL( *readBuf1, consumed, remoteAddr );    
    EUNIT_ASSERT( NULL == ptr );             
    EUNIT_ASSERT( EFalse == consumed );
    
    delete ptr;
    CleanupStack::PopAndDestroy( readBuf1 );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( request );  
    }

void ut_cstunbinding::UT_CSTUNBinding_SetICESpecificAttributesL(  )
    {
    TICEAttributes attributes;
    iBinding->SetICESpecificAttributes( attributes );
    }

void ut_cstunbinding::UT_CSTUNBinding_StreamIdL(  )
    {
    TUint streamId = iBinding->StreamId();
    }
    
void ut_cstunbinding::UT_CSTUNBinding_ConnectionIdL(  )
    {
    TUint connectionId = iBinding->ConnectionId();
    }

void ut_cstunbinding::UT_CSTUNBinding_STUNClientL(  )
    {
    const CSTUNClient* client = iBinding->STUNClient();
    }

/*
void ut_cstunbinding::UT_CSTUNBinding_ExecuteImmediateRetransmitLL(  )
    {
    iBinding->ExecuteImmediateRetransmitL();
    }
*/

void ut_cstunbinding::UT_CSTUNBinding_AlternateServerAddrL(  )
    {
    TInetAddr addr = iBinding->AlternateServerAddr();
    }

void ut_cstunbinding::UT_CSTUNBinding_CancelRetransmissionL()
    {
    iBinding->CancelRetransmission();
    }
        
void ut_cstunbinding::UT_CSTUNBinding_SendBindingIndicationLL()
    {
    _LIT8( KName, "test_name" ); 
    _LIT8( KPass, "test_password" );
    
    //remote address = KInetAddrAny
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iBinding->SendIndicationL( KInetAddrAny, KNullDesC8, EFalse ) );
    
    iStunclient->SetCredentialsL( KName, KPass ); 
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iBinding->SendIndicationL( KInetAddrAny, KNullDesC8, ETrue ) );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cstunbinding,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CSTUNBinding",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_NewLL, Teardown)

EUNIT_TEST(
    "NewL - test ",
    "CSTUNBinding",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_NewL_1L, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CSTUNBinding",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_NewLCL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CSTUNBinding",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_NewLC_1L, Teardown)

EUNIT_TEST(
    "SendRequestL - test ",
    "CSTUNBinding",
    "SendRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_SendRequestLL, Teardown)

EUNIT_TEST(
    "SendRequestL - test ",
    "CSTUNBinding",
    "SendRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_SendRequestL_1L, Teardown)

EUNIT_TEST(
    "CancelRequest - test ",
    "CSTUNBinding",
    "CancelRequest",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_CancelRequestL, Teardown)

EUNIT_TEST(
    "Socket - test ",
    "CSTUNBinding",
    "Socket",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_SocketL, Teardown)

EUNIT_TEST(
    "AddressResolved - test ",
    "CSTUNBinding",
    "AddressResolved",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_AddressResolvedL, Teardown)

EUNIT_TEST(
    "PublicAddr - test ",
    "CSTUNBinding",
    "PublicAddr",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_PublicAddrL, Teardown)

EUNIT_TEST(
    "HandleDataL - test ",
    "CSTUNBinding",
    "HandleDataL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_HandleDataLL, Teardown)

EUNIT_TEST(
    "SetICESpecificAttributes - test ",
    "CSTUNBinding",
    "SetICESpecificAttributes",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_SetICESpecificAttributesL, Teardown)

EUNIT_TEST(
    "StreamId - test ",
    "CSTUNBinding",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_StreamIdL, Teardown)

EUNIT_TEST(
    "ConnectionId - test ",
    "CSTUNBinding",
    "ConnectionId",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_ConnectionIdL, Teardown)

EUNIT_TEST(
    "STUNClient - test ",
    "CSTUNBinding",
    "STUNClient",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_STUNClientL, Teardown)

EUNIT_TEST(
    "AlternateServerAddr - test ",
    "CSTUNBinding",
    "AlternateServerAddr",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_AlternateServerAddrL, Teardown)

EUNIT_TEST(
    "CancelRetransmission - test ",
    "CSTUNBinding",
    "CancelRetransmission",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_CancelRetransmissionL, Teardown)
    
EUNIT_TEST(
    "SendBindingIndicationL - test ",
    "CSTUNBinding",
    "SendBindingIndicationL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNBinding_SendBindingIndicationLL, Teardown)
        
EUNIT_END_TEST_TABLE

//  END OF FILE
