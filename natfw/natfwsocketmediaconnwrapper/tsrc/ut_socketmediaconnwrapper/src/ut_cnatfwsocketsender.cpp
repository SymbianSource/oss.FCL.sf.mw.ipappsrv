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
#include "ut_cnatfwsocketsender.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwsocketsender.h"
#include "socketmediaconnwrappertestdefs.h"

_LIT8(KData,"qwerty");

// CONSTRUCTION
ut_cnatfwsocketsender* ut_cnatfwsocketsender::NewL()
    {
    ut_cnatfwsocketsender* self = ut_cnatfwsocketsender::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cnatfwsocketsender* ut_cnatfwsocketsender::NewLC()
    {
    ut_cnatfwsocketsender* self = new( ELeave ) ut_cnatfwsocketsender();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cnatfwsocketsender::~ut_cnatfwsocketsender()
    {
    }

// Default constructor
ut_cnatfwsocketsender::ut_cnatfwsocketsender()
    {
    }

// Second phase construct
void ut_cnatfwsocketsender::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void ut_cnatfwsocketsender::SetupL(  )
    {
    iSender = CNATFWSocketSender::NewL( *this, KProtocolInetUdp );
    User::LeaveIfError( iSocketSrv.Connect() );
    User::LeaveIfError( iSocket.Open( iSocketSrv, KAfInet, KSockDatagram, KProtocolInetUdp ));
    }    
    
void ut_cnatfwsocketsender::Setup2L(  )
    {
    iSender = CNATFWSocketSender::NewL( *this, KProtocolInetTcp );
    User::LeaveIfError( iSocketSrv.Connect() );
    User::LeaveIfError( iSocket.Open( iSocketSrv, KAfInet, KSockStream, KProtocolInetTcp ));
    }    

void ut_cnatfwsocketsender::Teardown(  )
    {
    delete iSender;
    iSender = NULL;
    iSocket.Close();
    iSocketSrv.Close();      
    }

void ut_cnatfwsocketsender::ut_cnatfwsocketsender_SetRemoteAddressL(  )
    {
    iSender->SetRemoteAddress( iAddr, iSocket );
    }
    
void ut_cnatfwsocketsender::ut_cnatfwsocketsender_RemoveFromQueueL()
    {
    iSender->RemoveFromQueue();
    }

void ut_cnatfwsocketsender::ut_cnatfwsocketsender_SendLL(  )
    {
    iSender->SetRemoteAddress( iAddr, iSocket );
    iSender->SendL( KData );
 
    Teardown();
    Setup2L();
    iSender->SetRemoteAddress( iAddr, iSocket );
    iSender->SendL( KData );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cnatfwsocketsender,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetRemoteAddress - test ",
    "CNATFWSocketSender",
    "SetRemoteAddress",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketsender_SetRemoteAddressL, Teardown)

EUNIT_TEST(
    "RemoveFromQueue - test ",
    "CNATFWSocketSender",
    "RemoveFromQueue",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketsender_RemoveFromQueueL, Teardown)

EUNIT_TEST(
    "SendL - test ",
    "CNATFWSocketSender",
    "SendL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketsender_SendLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
