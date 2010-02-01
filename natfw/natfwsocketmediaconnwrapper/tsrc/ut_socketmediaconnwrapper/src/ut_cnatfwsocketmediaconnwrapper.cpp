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
#include "ut_cnatfwsocketmediaconnwrapper.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwsocketmediaconnwrapper.h"
#include "natfwconnectivityapidefs.h"
#include "natfwunittestmacros.h"

// CONSTANTS
const TUint KStreamId = 10;
const TUint KPort = 250;

_LIT8(KData,"qwerty");
_LIT8(KAddress, "127.0.0.0");
_LIT8(KMessage,"Message");

// CONSTRUCTION
ut_cnatfwsocketmediaconnwrapper* ut_cnatfwsocketmediaconnwrapper::NewL()
    {
    ut_cnatfwsocketmediaconnwrapper* self = ut_cnatfwsocketmediaconnwrapper::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cnatfwsocketmediaconnwrapper* ut_cnatfwsocketmediaconnwrapper::NewLC()
    {
    ut_cnatfwsocketmediaconnwrapper* self = new( ELeave ) ut_cnatfwsocketmediaconnwrapper();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cnatfwsocketmediaconnwrapper::~ut_cnatfwsocketmediaconnwrapper()
    {
    }

// Default constructor
ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper()
    {
    }


void ut_cnatfwsocketmediaconnwrapper::SendingCompleted( const TInt aError )
    { 
    iError = aError;
    }
        
TInt ut_cnatfwsocketmediaconnwrapper::ReceivedFrom( const TUint aStreamId, 
    TDes8& /*aReceiveBuffer*/ )
    {
    iStreamId = aStreamId;

    return KErrNone;
    }

void ut_cnatfwsocketmediaconnwrapper::GetNewFreePort( TUint& aPort )
    {
    EUNIT_PRINT( _L8( "ut_cnatfwsocketmediaconnwrapper::GetNewFreePort" ) );
    
    TUint port( 250 );
    aPort = port;
    }


// Second phase construct
void ut_cnatfwsocketmediaconnwrapper::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void ut_cnatfwsocketmediaconnwrapper::SetupL(  )
    {
    iConnWrapper = CNATFWSocketMediaConnWrapper::NewL( 
        KPort, KProtocolInetUdp, KStreamId );
        
    User::LeaveIfError( iSocketServer.Connect() );
    TInetAddr localAddr;
    iConnWrapper->RegisterMediaWrapperObserverL( this, iSocketServer, localAddr ); 
    }

void ut_cnatfwsocketmediaconnwrapper::Teardown(  )
    {
    iSocketServer.Close();
    iConnWrapper->Close();  
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_StreamIdL(  )
    {
    EUNIT_ASSERT_NOT_EQUALS( iConnWrapper->StreamId(), NULL );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_OutgoingAddrL(  )
    {
    TSockAddr addr;
    iConnWrapper->OutgoingAddr( addr );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_SetIncomingAddrLL(  )
    {
    // tested in UT_MNATFWMediaWrapper
    EUNIT_ASSERT( ETrue );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_ConnectLL(  )
    {
    // tested in UT_MNATFWMediaWrapper
    EUNIT_ASSERT( ETrue );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_StartListeningLL(  )
    {
    // tested in UT_MNATFWMediaWrapper
    EUNIT_ASSERT( ETrue );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_SendLL(  )
    {
    // tested in UT_MNATFWMediaWrapper
    EUNIT_ASSERT( ETrue );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_DeactivateSendingL(  )
    {
    // tested in UT_MNATFWMediaWrapper
    EUNIT_ASSERT( ETrue );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_DeactivateReceivingL(  )
    {
    // tested in UT_MNATFWMediaWrapper
    EUNIT_ASSERT( ETrue );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_RegisterMediaWrapperObserverL(  )
    {
    //MNATFWMediaWrapperObserver* observer( NULL );
    TInetAddr localAddr;          
    iConnWrapper->RegisterMediaWrapperObserverL( this, iSocketServer, localAddr );
    
    MNATFWMediaWrapperObserver* observer( NULL );
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iConnWrapper->RegisterMediaWrapperObserverL( observer, iSocketServer, localAddr ), KErrNotReady );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_SendingCompletedL(  )
    {
    // tested in UT_MNATFWMediaWrapper
    EUNIT_ASSERT( ETrue );
    }

void ut_cnatfwsocketmediaconnwrapper::ut_cnatfwsocketmediaconnwrapper_DeliverBufferL(  )
    {
    // tested in UT_MNATFWMediaWrapper
    EUNIT_ASSERT( ETrue );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cnatfwsocketmediaconnwrapper,
    "CNATFWSocketMediaConnWrapper",
    "UNIT" )

EUNIT_TEST(
    "StreamId - test ",
    "CNATFWSocketMediaConnWrapper",
    "StreamId",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_StreamIdL, Teardown)

EUNIT_TEST(
    "OutgoingAddr - test ",
    "CNATFWSocketMediaConnWrapper",
    "OutgoingAddr",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_OutgoingAddrL, Teardown)

EUNIT_TEST(
    "SetIncomingAddrL - test ",
    "CNATFWSocketMediaConnWrapper",
    "SetIncomingAddrL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_SetIncomingAddrLL, Teardown)

EUNIT_TEST(
    "ConnectL - test ",
    "CNATFWSocketMediaConnWrapper",
    "ConnectL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_ConnectLL, Teardown)

EUNIT_TEST(
    "StartListeningL - test ",
    "CNATFWSocketMediaConnWrapper",
    "StartListeningL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_StartListeningLL, Teardown)

EUNIT_TEST(
    "SendL - test ",
    "CNATFWSocketMediaConnWrapper",
    "SendL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_SendLL, Teardown)

EUNIT_TEST(
    "DeactivateSending - test ",
    "CNATFWSocketMediaConnWrapper",
    "DeactivateSending",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_DeactivateSendingL, Teardown)

EUNIT_TEST(
    "DeactivateReceiving - test ",
    "CNATFWSocketMediaConnWrapper",
    "DeactivateReceiving",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_DeactivateReceivingL, Teardown)

EUNIT_TEST(
    "RegisterMediaWrapperObserver - test ",
    "CNATFWSocketMediaConnWrapper",
    "RegisterMediaWrapperObserver",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_RegisterMediaWrapperObserverL, Teardown)

EUNIT_TEST(
    "SendingCompleted - test ",
    "CNATFWSocketMediaConnWrapper",
    "SendingCompleted",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_SendingCompletedL, Teardown)

EUNIT_TEST(
    "DeliverBuffer - test ",
    "CNATFWSocketMediaConnWrapper",
    "DeliverBuffer",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketmediaconnwrapper_DeliverBufferL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
