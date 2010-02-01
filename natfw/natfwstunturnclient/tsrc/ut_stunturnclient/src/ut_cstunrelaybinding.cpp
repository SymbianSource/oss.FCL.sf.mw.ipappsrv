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
#include "UT_CSTUNRelayBinding.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <e32debug.h>

//  INTERNAL INCLUDES
#include "natfwstunrelaybinding.h"
#include "natfwstunclientdefs.h"
#include "natfwunsafmappedaddressattribute.h"
#include "natfwunsafxormappedaddressattribute.h"
#include "natfwunsafrelayaddressattribute.h"
#include "natfwunsaflifetimeattribute.h"
#include "natfwunsafbandwidthattribute.h"
#include "natfwunsafsetactivedestinationresponse.h"
#include "natfwunsaftimervalattribute.h"
#include "natfwunsafallocateerrorresponse.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafsetactivedestinationerrorresponse.h"
#include "natfwunsafconnectresponse.h"
#include "natfwunsafconnecterrorresponse.h"
#include "natfwunsafdataindication.h"
#include "natfwunsafdataattribute.h"
#include "natfwunsafremoteaddressattribute.h"
#include "natfwunsaficecontrolledattribute.h"
#include "natfwunsaficecontrollingattribute.h"
#include "natfwunittestmacros.h"

// CONSTANTS
_LIT8( KServerAddress, "10.32.194.251" );
_LIT8( KUsername, "userAA" );
_LIT8( KPassword,"salasana");

const TUint KDefaultRtoValue = 0;

// CONSTRUCTION
UT_CSTUNRelayBinding* UT_CSTUNRelayBinding::NewL()
    {
    UT_CSTUNRelayBinding* self = UT_CSTUNRelayBinding::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CSTUNRelayBinding* UT_CSTUNRelayBinding::NewLC()
    {
    UT_CSTUNRelayBinding* self = new( ELeave ) UT_CSTUNRelayBinding();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CSTUNRelayBinding::~UT_CSTUNRelayBinding()
    {
    }

// Default constructor
UT_CSTUNRelayBinding::UT_CSTUNRelayBinding()
    {
    }

// Second phase construct
void UT_CSTUNRelayBinding::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CSTUNRelayBinding::SetupL(  )
    {
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
    // TUint portRangeStart(0);
    // TUint portRangeStop(999);
    iMux = CNcmConnectionMultiplexer::NewL( *this );
    
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iapID );
    
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    User::LeaveIfError( iConnection.Start( pref ) );
    
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
    
    CActiveScheduler::Start();

    }

void UT_CSTUNRelayBinding::Teardown(  )
    {
    delete iStunclient;
    iStunclient = NULL;
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
   
    
// from mstunclientbserver
void UT_CSTUNRelayBinding::STUNClientInitCompleted( const CSTUNClient& /*aClient*/,
                              TInt aCompletionCode )
    {
    RDebug::Print( _L("**NATFW STUNClientInitCompleted, code: %d"), aCompletionCode );
    CActiveScheduler::Stop();
    }


void UT_CSTUNRelayBinding::STUNBindingEventOccurredL( TSTUNBindingEvent aEvent, 
                                const CBinding& /*aBinding*/ )
    {
    RDebug::Print( _L("**NATFW STUNBindingEventOccurredL, code: %d"), aEvent );
    CActiveScheduler::Stop();
    }


void UT_CSTUNRelayBinding::STUNBindingErrorOccurred( const CBinding& /*aBinding*/, 
                               TInt aError )
    {
    RDebug::Print( _L("**NATFW STUNBindingErrorOccurred, code: %d"), aError );
    CActiveScheduler::Stop();
    }


void UT_CSTUNRelayBinding::Notify( TUint /*aSessionId*/, TUint /*aStreamId*/, 
    TNotifyType /*aType*/, TInt /*aError*/ )
    {
    RDebug::Print( _L("**NATFW Notify") );
    }
    
void UT_CSTUNRelayBinding::IcmpError( TUint /*aSessionId*/, TUint /*aStreamId*/, const TInetAddr& /*aAddress*/ )
    {
    RDebug::Print( _L("**NATFW IcmpError") );
    }
    
    
void UT_CSTUNRelayBinding::CreateBinding()
    {
    iBinding = CSTUNRelayBinding::NewL( *iStunclient, iStreamId, iSubstreamId );
    }
    
void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_NewLL(  )
    {
    CSTUNRelayBinding* binding;
    NATFW_EUNIT_ASSERT_NO_LEAVE( binding = 
        CSTUNRelayBinding::NewL( *iStunclient, iStreamId, iSubstreamId ) );
    CleanupStack::PushL( binding );
    CleanupStack::PopAndDestroy( binding );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_AllocateRequestLL(  )
    {
    TInetAddr relay;
    CreateBinding();
    NATFW_EUNIT_ASSERT_NO_LEAVE( iBinding->AllocateRequestL( KDefaultRtoValue ) );

    TBool isValid( EFalse );
    iResponse = GenerateResponseL( EAllocateResp );
    TInetAddr remoteaddr;
    HBufC8* buf = iBinding->HandleDataL( *iResponse, isValid, remoteaddr );
    buf = NULL;
    CActiveScheduler::Start();
    TBool isValid2 = iBinding->AddressResolved();
    relay = iBinding->RelayAddr();
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_AllocateRequestL_AllocL(  )
    {
    TInetAddr relay;
    CreateBinding();
    NATFW_EUNIT_ASSERT_NO_LEAVE( iBinding->AllocateRequestL( KDefaultRtoValue ) );

    TBool isValid( EFalse );
    iResponse = GenerateResponseL( EAllocateResp );
    TInetAddr remoteaddr;
    HBufC8* buf = iBinding->HandleDataL( *iResponse, isValid, remoteaddr );
    buf = NULL;
    TBool isValid2 = iBinding->AddressResolved();
    relay = iBinding->RelayAddr();
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_AllocateRequestErrLL(  )
    {
    CreateBinding();
    NATFW_EUNIT_ASSERT_NO_LEAVE( iBinding->AllocateRequestL( KDefaultRtoValue ) );
    /*
    if ( err )
        {
        RDebug::Print( _L("NATFW TEST / AllocateRequestErrL LEAVES %d"), err );
        }
    */
    TBool isValid( EFalse );
    iResponse = GenerateResponseL( EAllocateErr );
    TInetAddr remoteaddr;
    HBufC8* buf = iBinding->HandleDataL( *iResponse, isValid, remoteaddr );
    buf = NULL;
    CActiveScheduler::Start();
    EUNIT_ASSERT( isValid );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_CancelRequestL(  )
    {
    CreateBinding();
    iBinding->AllocateRequestL( KDefaultRtoValue );
    iBinding->CancelRequest();
    EUNIT_ASSERT( ETrue );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_SetICESpecificAttributesL( )
    {
    TUint priori( 11 );
    CreateBinding();
    TBool isValid( EFalse );
    TICEAttributes attr( 1, ETrue, 1, 0 );
    iBinding->SetICESpecificAttributes( attr );
    iBinding->AllocateRequestL( KDefaultRtoValue );

    iResponse = GenerateResponseL( EAllocateResp );
    TInetAddr remoteaddr;
    HBufC8* buf = iBinding->HandleDataL( *iResponse, isValid, remoteaddr );
    buf = NULL;
    CActiveScheduler::Start();
    EUNIT_ASSERT( isValid );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_TestHandleDataUsageLL(  )
    {
    TUint priori( 11 );
    CreateBinding();
    iBinding->AllocateRequestL( KDefaultRtoValue );
    TBool isValid( EFalse );
    
    _LIT8( KDummypacket, "oysescatechnologiesabFIN" );
    TBufC8<24> buffi( KDummypacket );
    iRespDataIndication = GenerateDataIndicationL( buffi );
    TInetAddr remoteaddr;
    HBufC8* buf = iBinding->HandleDataL( *iRespDataIndication, isValid, remoteaddr );
    if ( buf != NULL )
        {
        delete buf;
        }
    buf = NULL;
    iBinding->CancelRequest();
    EUNIT_ASSERT( !isValid );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_SetActiveDestinationRequestLL(  )
    {
    CreateBinding();
    TUint32 timer(1);
    TInetAddr addr;
    addr.SetAddress( INET_ADDR( 10,32,194,2 ) );
    
    iBinding->SetActiveDestinationRequestL( addr, timer );

    TBool isValid( EFalse );
    iResponse = GenerateResponseL( ESetActiveDestResp );
    iRespDataIndication = GenerateDataIndicationL( *iResponse );
    TInetAddr addr2;
    HBufC8* testdata = iBinding->HandleDataL( *iRespDataIndication, isValid, addr2 );
    testdata = NULL;
    CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( timer, 10 );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_SetActiveDestinationRequestL_AllocL(  )
    {
    CreateBinding();
    TUint32 timer(1);
    TInetAddr addr;
    addr.SetAddress( INET_ADDR( 10,32,194,2 ) );
    
    iBinding->SetActiveDestinationRequestL( addr, timer );
/*
    TBool isValid( EFalse );
    iResponse = GenerateResponseL( ESetActiveDestResp );
    iRespDataIndication = GenerateDataIndicationL( *iResponse );
    TInetAddr addr2;
    HBufC8* testdata = iBinding->HandleDataL( *iRespDataIndication, isValid, addr2 );
    testdata = NULL;
    */
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_SetActiveDestinationRequestErrLL(  )
    {
    CreateBinding();
    TUint32 timer;
    TInetAddr addr;
    addr.SetAddress( INET_ADDR( 10,32,194,2 ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iBinding->SetActiveDestinationRequestL( addr, timer ) );
    /*
    if ( err )
        {
        RDebug::Print( _L("NATFW TEST / ActiveDestinationRequestErr LEAVES %d"), err );
        }
    */
    TBool isValid( EFalse );
    iResponse = GenerateResponseL( ESetActiveDestErr );
    TInetAddr addr2;
    HBufC8* testdata = iBinding->HandleDataL( *iResponse, isValid, addr2 );
    testdata = NULL;
    CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( !testdata, isValid );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_SendIndicationLL(  )
    {
    TInetAddr addr;
    addr.SetAddress( INET_ADDR( 10,32,194,2 ) );
    CreateBinding();
    iBinding->AllocateRequestL( KDefaultRtoValue );
    iResponse = GenerateResponseL( EAllocateResp );
    TBool isValid;
    TInetAddr remoteaddr;
    HBufC8* buf = iBinding->HandleDataL( *iResponse, isValid, remoteaddr );
    buf = NULL;
    CActiveScheduler::Start();
    TBool addFingerPrint( EFalse );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iBinding->SendIndicationL( addr, *iResponse, addFingerPrint ) );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_ConnectRequestLL(  )
    {
    CreateBinding();
    TInetAddr addr;
    addr.SetAddress( INET_ADDR( 10,32,194,2 ) );
    
    iBinding->ConnectRequestL( addr );

    TBool isValid( EFalse );
    iResponse = GenerateResponseL( EConnectResp );
    TInetAddr addr2;
    HBufC8* testdata = iBinding->HandleDataL( *iResponse, isValid, addr2 );
    testdata = NULL;
    CActiveScheduler::Start();
    EUNIT_ASSERT( isValid );
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_ConnectRequestL_AllocL(  )
    {
    CreateBinding();
    TInetAddr addr;
    addr.SetAddress( INET_ADDR( 10,32,194,2 ) );
    
    iBinding->ConnectRequestL( addr );

    TBool isValid( EFalse );
    iResponse = GenerateResponseL( EConnectResp );
    TInetAddr addr2;
    HBufC8* testdata = iBinding->HandleDataL( *iResponse, isValid, addr2 );
    testdata = NULL;
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_ConnectRequestErrLL(  )
    {
    CreateBinding();
    TInetAddr addr;
    addr.SetAddress( INET_ADDR( 10,32,194,2 ) );
    
    iBinding->ConnectRequestL( addr );

    TBool isValid( EFalse );
    iResponse = GenerateResponseL( EConnectErr );
    TInetAddr addr2;
    HBufC8* testdata = iBinding->HandleDataL( *iResponse, isValid, addr2 );
    testdata = NULL;
    CActiveScheduler::Start();
    EUNIT_ASSERT( isValid );
    }


HBufC8* UT_CSTUNRelayBinding::GenerateResponseL( TMsgType aType )
    {
    TNATFWUNSAFTransactionID transactionID;
    transactionID.Append(TUint8(0));
    transactionID.Append(TUint8(1));
    transactionID.Append(TUint8(2));
    transactionID.Append(TUint8(3));
    transactionID.Append(TUint8(4));
    transactionID.Append(TUint8(5));
    transactionID.Append(TUint8(6));
    transactionID.Append(TUint8(7));
    transactionID.Append(TUint8(8));
    transactionID.Append(TUint8(9));
    transactionID.Append(TUint8(0xa));
    transactionID.Append(TUint8(0xb));
    
    TInetAddr relay;
    relay.SetAddress( INET_ADDR( 10,32,194,1 ) );
    TInetAddr xormapped;
    xormapped.SetAddress( INET_ADDR( 10,32,194,2 ) );
    TInt dummyvalue(10);
    TBufC8<16> password( KPassword );
    TBufC8<16> dummydata( KPassword );
    TInt errorvalue(446);
    
    CNATFWUNSAFMessage* msg;
    
    if ( EAllocateResp == aType )
        {
        msg = CNATFWUNSAFAllocateResponse::NewLC( transactionID );
        // attributes
        msg->AddAttributeL( CNATFWUNSAFRelayAddressAttribute::NewLC( relay ) );
        CleanupStack::Pop();
        msg->AddAttributeL( CNATFWUNSAFXorMappedAddressAttribute::NewLC( xormapped, transactionID ) );
        CleanupStack::Pop();
        msg->AddAttributeL( CNATFWUNSAFLifetimeAttribute::NewLC( dummyvalue ) );
        CleanupStack::Pop();
        msg->AddAttributeL( CNATFWUNSAFBandwidthAttribute::NewLC( dummyvalue ) );
        CleanupStack::Pop();
        }
    if ( EAllocateErr == aType )
        {
        msg = CNATFWUNSAFAllocateErrorResponse::NewLC( transactionID );
        // attributes
        msg->AddAttributeL( CNATFWUNSAFErrorCodeAttribute::NewLC( errorvalue, dummydata ) );
        CleanupStack::Pop();
        }
    if ( ESetActiveDestResp == aType )
        {
        TUint timerval( 10 );
        msg = CNATFWUNSAFSetActiveDestinationResponse::NewLC( transactionID );
        // attributes
        msg->AddAttributeL( CNATFWUNSAFTimerValAttribute::NewLC( timerval ) );
        CleanupStack::Pop();
        }
    if ( ESetActiveDestErr == aType )
        {
        msg = CNATFWUNSAFSetActiveDestinationErrorResponse::NewLC( transactionID );
        // attributes
        msg->AddAttributeL( CNATFWUNSAFErrorCodeAttribute::NewLC( errorvalue, dummydata ) );
        CleanupStack::Pop();
        }
    if ( EConnectResp == aType )
        {
        msg = CNATFWUNSAFConnectResponse::NewLC( transactionID );
        // attributes
        msg->AddAttributeL( CNATFWUNSAFRelayAddressAttribute::NewLC( relay ) );
        CleanupStack::Pop();
        msg->AddAttributeL( CNATFWUNSAFXorMappedAddressAttribute::NewLC( xormapped, transactionID ) );
        CleanupStack::Pop();
        msg->AddAttributeL( CNATFWUNSAFLifetimeAttribute::NewLC( dummyvalue ) );
        CleanupStack::Pop();
        msg->AddAttributeL( CNATFWUNSAFBandwidthAttribute::NewLC( dummyvalue ) );
        CleanupStack::Pop();
        }
    if ( EConnectErr == aType )
        {
        msg = CNATFWUNSAFConnectErrorResponse::NewLC( transactionID );
        // attributes
        msg->AddAttributeL( CNATFWUNSAFErrorCodeAttribute::NewLC( errorvalue, dummydata ) );
        CleanupStack::Pop();
        }
        
        
    CBufBase* msg1 = msg->EncodeL( password ); 
    CleanupStack::PushL( msg1 );
    
    TInt length1 = msg1->Size();
    HBufC8* readBuf1 = HBufC8::NewLC( length1 );
    TPtr8 writable1( readBuf1->Des() );
    msg1->Read( 0, writable1 );
    
    CleanupStack::Pop( readBuf1 );
    CleanupStack::PopAndDestroy( msg1 );
    CleanupStack::PopAndDestroy( msg );
    
    return readBuf1;
    }


HBufC8* UT_CSTUNRelayBinding::GenerateDataIndicationL( TDesC8& aResponse )
    {
    TInetAddr remote;
    remote.SetAddress( INET_ADDR( 10,32,194,1 ) );
    TBufC8<16> password( KPassword );
    CNATFWUNSAFMessage* msg;
    
    TNATFWUNSAFTransactionID transactionID;
    for( TInt i=0; i<11; i++ )
        {
        transactionID.Append( TUint8(i));
        }

    msg = CNATFWUNSAFDataIndication::NewLC( transactionID );
        // attributes
    msg->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( aResponse ) );
    CleanupStack::Pop();
    msg->AddAttributeL( CNATFWUNSAFRemoteAddressAttribute::NewLC( remote ) );
    CleanupStack::Pop();
    
    CBufBase* msg1 = msg->EncodeL( password ); 
    CleanupStack::PushL( msg1 );
    
    TInt length1 = msg1->Size();
    HBufC8* readBuf1 = HBufC8::NewLC( length1 );
    TPtr8 writable1( readBuf1->Des() );
    msg1->Read( 0, writable1 );
    
    CleanupStack::Pop( readBuf1 );
    CleanupStack::PopAndDestroy( msg1 );
    CleanupStack::PopAndDestroy( msg );
    
    return readBuf1;
    }

void UT_CSTUNRelayBinding::UT_CSTUNRelayBinding_TestGettersL()
    {
    CreateBinding();
    TUint streamId = iBinding->StreamId();
    TUint connectionId = iBinding->ConnectionId();
    const CSTUNClient* client = iBinding->STUNClient();
    TInetAddr add = iBinding->AlternateServerAddr();
    const HBufC8* response = iBinding->RealmFromResponse();
    TInetAddr addr = iBinding->PublicAddr();
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CSTUNRelayBinding,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CSTUNRelayBinding",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_NewLL, Teardown)

EUNIT_NOT_DECORATED_TEST(
    "AllocateRequestL - test ",
    "CSTUNRelayBinding",
    "AllocateRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_AllocateRequestLL, Teardown)

EUNIT_ALLOC_TEST(
    "AllocateRequestL - alloc test ",
    "CSTUNRelayBinding",
    "AllocateRequestL",
    "ERRORHANDLING",
    SetupL, UT_CSTUNRelayBinding_AllocateRequestL_AllocL, Teardown)

EUNIT_TEST(
    "AllocateRequestErrL - test ",
    "CSTUNRelayBinding",
    "AllocateRequestErrL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_AllocateRequestErrLL, Teardown)

EUNIT_TEST(
    "CancelRequest - test ",
    "CSTUNRelayBinding",
    "CancelRequest",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_CancelRequestL, Teardown)

EUNIT_TEST(
    "SetPriorityAttributeL - test ",
    "CSTUNRelayBinding",
    "SetPriorityAttributeL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_SetICESpecificAttributesL, Teardown)
   
EUNIT_TEST(
    "TestHandleDataUsageL - test ",
    "CSTUNRelayBinding",
    "TestHandleDataUsageL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_TestHandleDataUsageLL, Teardown)

EUNIT_NOT_DECORATED_TEST(
    "SetActiveDestinationRequestL - test ",
    "CSTUNRelayBinding",
    "SetActiveDestinationRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_SetActiveDestinationRequestLL, Teardown)

EUNIT_ALLOC_TEST(
    "SetActiveDestinationRequestL - alloc test ",
    "CSTUNRelayBinding",
    "SetActiveDestinationRequestL",
    "ERRORHANDLING",
    SetupL, UT_CSTUNRelayBinding_SetActiveDestinationRequestL_AllocL, Teardown)

EUNIT_TEST(
    "SetActiveDestinationRequestErrL - test ",
    "CSTUNRelayBinding",
    "SetActiveDestinationRequestErrL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_SetActiveDestinationRequestErrLL, Teardown)

EUNIT_TEST(
    "SendIndicationL - test ",
    "CSTUNRelayBinding",
    "SendIndicationL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_SendIndicationLL, Teardown)

EUNIT_NOT_DECORATED_TEST(
    "ConnectRequestL - test ",
    "CSTUNRelayBinding",
    "ConnectRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_ConnectRequestLL, Teardown)

EUNIT_ALLOC_TEST(
    "ConnectRequestL - test ",
    "CSTUNRelayBinding",
    "ConnectRequestL",
    "ERRORHANDLING",
    SetupL, UT_CSTUNRelayBinding_ConnectRequestL_AllocL, Teardown)

EUNIT_TEST(
    "ConnectRequestErrL - test ",
    "CSTUNRelayBinding",
    "ConnectRequestErrL",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_ConnectRequestErrLL, Teardown)

EUNIT_TEST(
    "TestGetters - test ",
    "CSTUNRelayBinding",
    "TestGetters",
    "FUNCTIONALITY",
    SetupL, UT_CSTUNRelayBinding_TestGettersL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
