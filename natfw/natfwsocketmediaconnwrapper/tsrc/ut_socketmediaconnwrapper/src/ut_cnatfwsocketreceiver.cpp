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
#include "ut_cnatfwsocketreceiver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwsocketreceiver.h"
#include "socketmediaconnwrappertestdefs.h"

// CONSTRUCTION
ut_cnatfwsocketreceiver* ut_cnatfwsocketreceiver::NewL()
    {
    ut_cnatfwsocketreceiver* self = ut_cnatfwsocketreceiver::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cnatfwsocketreceiver* ut_cnatfwsocketreceiver::NewLC()
    {
    ut_cnatfwsocketreceiver* self = new( ELeave ) ut_cnatfwsocketreceiver();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cnatfwsocketreceiver::~ut_cnatfwsocketreceiver()
    {
    }

// Default constructor
ut_cnatfwsocketreceiver::ut_cnatfwsocketreceiver()
    {
    }

// Second phase construct
void ut_cnatfwsocketreceiver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void ut_cnatfwsocketreceiver::SetupL(  )
    {
    User::LeaveIfError( iServ.Connect() );    
    iReceiver = CNATFWSocketReceiver::NewL( *this, 250 );
    } 

void ut_cnatfwsocketreceiver::Teardown(  )
    {
    delete iReceiver;
    iReceiver = NULL;
    iSocket.Close();
    iServ.Close();   
    }

void ut_cnatfwsocketreceiver::ut_cnatfwsocketreceiver_StartListeningL(  )
    {    
    User::LeaveIfError( iSocket.Open( iServ, KAfInet, KSockDatagram, KProtocolInetUdp ));
    iReceiver->iProtocol = KProtocolInetUdp;
    iReceiver->StartListening( iSocket );

    Teardown();
    SetupL();
    User::LeaveIfError( iSocket.Open( iServ, KAfInet, KSockStream, KProtocolInetTcp ));
    iReceiver->iProtocol = KProtocolInetTcp;
    iReceiver->StartListening( iSocket );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cnatfwsocketreceiver,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "StartListening - test ",
    "CNATFWSocketReceiver",
    "StartListening",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwsocketreceiver_StartListeningL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
