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
#include "ut_cnatfwsockethandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwconnectivityapidefs.h"

//  INTERNAL INCLUDES
#include "natfwsockethandler.h"
#include "socketmediaconnwrappertestdefs.h"
#include "natfwunittestmacros.h"

_LIT8(KData,"qwerty");
const TUint KTestPortUdp = 5000;
const TUint KTestPortTcp = 5001;
const TUint KTestPort2 = 5010;

// CONSTRUCTION
ut_cnatfwsockethandler* ut_cnatfwsockethandler::NewL()
    {
    ut_cnatfwsockethandler* self = ut_cnatfwsockethandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cnatfwsockethandler* ut_cnatfwsockethandler::NewLC()
    {
    ut_cnatfwsockethandler* self = new( ELeave ) ut_cnatfwsockethandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cnatfwsockethandler::~ut_cnatfwsockethandler()
    {
    }

// Default constructor
ut_cnatfwsockethandler::ut_cnatfwsockethandler()
    {
    }

// Second phase construct
void ut_cnatfwsockethandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

// From observer interface
void ut_cnatfwsockethandler::GetNewFreePort( TUint& aPort )
    {
    aPort = KTestPort2;
    }
    
//  METHODS
void ut_cnatfwsockethandler::SetupL(  )
    {
    iHandler = CNATFWSocketHandler::NewL( *this, *this, KProtocolInetUdp, KTestPortUdp );
    User::LeaveIfError( iSocketServer.Connect() );
    }
    
void ut_cnatfwsockethandler::Setup2L(  )
    {
    iHandler = CNATFWSocketHandler::NewL( *this, *this, KProtocolInetTcp, KTestPortTcp );
    User::LeaveIfError( iSocketServer.Connect() );
    }    

void ut_cnatfwsockethandler::Teardown(  )
    {
    delete iHandler;
    iSocketServer.Close();
    }

void ut_cnatfwsockethandler::ut_cnatfwsockethandler_NewLL(  )
    {
    TUint port = 0;
    CNATFWSocketHandler* handler = NULL;

    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        handler = CNATFWSocketHandler::NewL( *this, *this, 0, port ), KErrArgument );        

    delete handler;
    }

void ut_cnatfwsockethandler::ut_cnatfwsockethandler_LocalAddressL(  )
    {
    iHandler->LocalAddress( iAddr );
    }

void ut_cnatfwsockethandler::ut_cnatfwsockethandler_SetAddrLL(  )
    {
    iHandler->SetLocalAddress( iSocketServer, iAddr );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iHandler->SetAddrL( iAddr ) );

    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iHandler->SetAddrL( iAddr ), KErrArgument );
        
    delete iHandler;
    iHandler = NULL;  
    iHandler = CNATFWSocketHandler::NewL( *this, *this, KProtocolInetTcp, KTestPortTcp );

    iHandler->SetLocalAddress( iSocketServer, iAddr );    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iHandler->SetAddrL( iAddr ) );
    }
    
void ut_cnatfwsockethandler::ut_cnatfwsockethandler_SetAddrL2L(  )
    {
    TSockAddr addr;
    RSocket socket;
	
    User::LeaveIfError( socket.Open( iSocketServer, KAfInet, KSockDatagram, KProtocolInetUdp ) );
    socket.SetLocalPort( KTestPortUdp );
    
    // RSocket is stubbed and currently the used ports are not stored
    // so we can make the next localportcall fail with this
    TBool shouldItFail = ETrue;
	socket.NextSetLocalPortShouldFail(ETrue, shouldItFail); // fails
      
    iHandler->SetMediaObserverL( this );
    iHandler->SetLocalAddress( iSocketServer, iAddr );
    // Will try to use same port as KTestPortUdp, but
    // since it's reserved, should switch to KTestPort2
    NATFW_EUNIT_ASSERT_NO_LEAVE( iHandler->SetAddrL( iAddr ) );

    iHandler->LocalAddress( addr );
    EUNIT_ASSERT_EQUALS( KTestPort2, addr.Port() );
    }
    
void ut_cnatfwsockethandler::ut_cnatfwsockethandler_ConnectLL(  )
    {
    iHandler->iSendState = iHandler->EConnected;
    }

void ut_cnatfwsockethandler::ut_cnatfwsockethandler_StartListeningLL(  )
    {
    iHandler->SetLocalAddress( iSocketServer, iAddr );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iHandler->SetAddrL( iAddr ) );    
    }

void ut_cnatfwsockethandler::ut_cnatfwsockethandler_SendLL(  )
    {
    iHandler->SetLocalAddress( iSocketServer, iAddr );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iHandler->SetAddrL( iAddr ) ); 
    iHandler->iSendState = iHandler->EConnected;
    iHandler->SetSendingStateL();           
    NATFW_EUNIT_ASSERT_NO_LEAVE( iHandler->SendL( KData ) );
    }

void ut_cnatfwsockethandler::ut_cnatfwsockethandler_SetLocalAddressL(  )
    {
    iHandler->SetLocalAddress( iSocketServer, iAddr );
    }

void ut_cnatfwsockethandler::ut_cnatfwsockethandler_DeactivateSendingL(  )
    {
    iHandler->DeactivateSending();
    }

void ut_cnatfwsockethandler::ut_cnatfwsockethandler_DeactivateReceivingL(  )
    {
    iHandler->DeactivateReceiving();
    }
    
void ut_cnatfwsockethandler::ut_cnatfwsockethandler_SetupTcpConnectionL(  )
    {
    iHandler->SetLocalAddress( iSocketServer, iAddr );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iHandler->SetAddrL( iAddr ) );
    iHandler->OpenTcpConnectionL( ETcpSetupActive, *this );

    delete iHandler;
    iHandler = NULL;  
    iHandler = CNATFWSocketHandler::NewL( *this, *this, KProtocolInetTcp, KTestPortTcp );
    
    iHandler->SetLocalAddress( iSocketServer, iAddr );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iHandler->SetAddrL( iAddr ) );
    iHandler->OpenTcpConnectionL( ETcpSetupPassive, *this );
        
    iHandler->iSendState = iHandler->EConnected;
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iHandler->OpenTcpConnectionL( ETcpSetupPassive, *this ), KErrInUse );
    }      
    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cnatfwsockethandler,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CNATFWSocketHandler",
    "NewL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_NewLL, Teardown)

EUNIT_TEST(
    "LocalAddress - test ",
    "CNATFWSocketHandler",
    "LocalAddress",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_LocalAddressL, Teardown)

EUNIT_TEST(
    "SetAddrL - test ",
    "CNATFWSocketHandler",
    "SetAddrL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_SetAddrLL, Teardown)

EUNIT_TEST(
    "SetAddrL2 - port in use test ",
    "CNATFWSocketHandler",
    "SetAddrL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_SetAddrL2L, Teardown)

EUNIT_TEST(
    "ConnectL - test ",
    "CNATFWSocketHandler",
    "ConnectL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_ConnectLL, Teardown)

EUNIT_TEST(
    "StartListeningL - test ",
    "CNATFWSocketHandler",
    "StartListeningL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_StartListeningLL, Teardown)

EUNIT_TEST(
    "SendL - test ",
    "CNATFWSocketHandler",
    "SendL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_SendLL, Teardown)

EUNIT_TEST(
    "SetLocalAddress - test ",
    "CNATFWSocketHandler",
    "SetLocalAddress",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_SetLocalAddressL, Teardown)

EUNIT_TEST(
    "DeactivateSending - test ",
    "CNATFWSocketHandler",
    "DeactivateSending",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_DeactivateSendingL, Teardown)

EUNIT_TEST(
    "DeactivateReceiving - test ",
    "CNATFWSocketHandler",
    "DeactivateReceiving",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsockethandler_DeactivateReceivingL, Teardown) 

EUNIT_TEST(
    "SetupTcpConnection - test ",
    "CNATFWSocketHandler",
    "SetupTcpConnection",
    "FUNCTIONALITY",
    Setup2L, ut_cnatfwsockethandler_SetupTcpConnectionL, Teardown)    


EUNIT_END_TEST_TABLE

//  END OF FILE
