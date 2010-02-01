/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "ut_cncmicmpreceiver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <commdbconnpref.h>


//  INTERNAL INCLUDES
#include "cncmicmpv4receiver.h"
#include "cncmicmpsender.h"

const TInt KIapId = 10;

// Just use own loopback address for pinging
const TInt KDestAddr( INET_ADDR(127,0,0,1) ); 
const TInt KDestPort( 1024 );

// CONSTRUCTION
UT_CNcmIcmpReceiver* UT_CNcmIcmpReceiver::NewL()
    {
    UT_CNcmIcmpReceiver* self = UT_CNcmIcmpReceiver::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNcmIcmpReceiver* UT_CNcmIcmpReceiver::NewLC()
    {
    UT_CNcmIcmpReceiver* self = new( ELeave ) UT_CNcmIcmpReceiver();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNcmIcmpReceiver::~UT_CNcmIcmpReceiver()
    {
    iTestConnection.Close();    
    iTestSocketSrv.Close();
    }

// Default constructor
UT_CNcmIcmpReceiver::UT_CNcmIcmpReceiver()
    {
    }

// Second phase construct
void UT_CNcmIcmpReceiver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    User::LeaveIfError( iTestSocketSrv.Connect() );

    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( KIapId );  
                                                 
    User::LeaveIfError( iTestConnection.Open( iTestSocketSrv ) );
    User::LeaveIfError( iTestConnection.Start( pref ) );
    
    }

// Observeres
void UT_CNcmIcmpReceiver::IcmpError( const TInetAddr& /*aAddress*/,
    TInetAddr& /*aLocalAddress*/, TInetAddr& /*aRemoteAddress*/ )
    {
    iReceived = ETrue;
    }
    
//  METHODS


void UT_CNcmIcmpReceiver::SetupL( )
    {
    iCNcmIcmpV4Receiver =
        CNcmIcmpV4Receiver::NewL( iTestSocketSrv, iTestConnection, *this );
        
    iCNcmIcmpV6Receiver =
        CNcmIcmpV6Receiver::NewL( iTestSocketSrv, iTestConnection, *this );
    }


void UT_CNcmIcmpReceiver::Teardown(  )
    {
    delete iCNcmIcmpV4Receiver; 
    delete iCNcmIcmpV6Receiver;
    }
    

void UT_CNcmIcmpReceiver::UT_IcmpUnreachMessageNoUDPTestL()
	{
	CreateICMPError(3, 1, 0x14); // Type is UNREACH, but there is no UDP data
	iReceived = EFalse;
	iCNcmIcmpV4Receiver->CheckError();
	EUNIT_ASSERT( !iReceived );
	// iAddress should be zero because it is not set if UDP is not found
	EUNIT_ASSERT_EQUALS( 0, iCNcmIcmpV4Receiver->iAddress.Address() );
	}

void UT_CNcmIcmpReceiver::UT_OtherIcmpMessageTestL()
	{
	CreateICMPError(2, 0, 0x11); // Type is not UNREACH
	iReceived = EFalse;
	iCNcmIcmpV4Receiver->CheckError();
	EUNIT_ASSERT( !iReceived );
	// iAddress should be zero because it is not set in other icmp-messages
	EUNIT_ASSERT_EQUALS( 0, iCNcmIcmpV4Receiver->iAddress.Address() );
	}



void UT_CNcmIcmpReceiver::UT_IcmpV6UnreachMessageTestL()
	{
	CreateIPv4CompatibleICMPv6Error(1, 1, 0x11); // Type is UNREACH and there is UDP data
	iReceived = EFalse;
	iCNcmIcmpV6Receiver->CheckError();
	EUNIT_ASSERT( iReceived );
	EUNIT_ASSERT_EQUALS( 0xffffffff, iCNcmIcmpV6Receiver->iAddress.Address() );
	}

void UT_CNcmIcmpReceiver::UT_IcmpV6UnreachMessageNoUDPTestL()
	{
	CreateIPv4CompatibleICMPv6Error(1, 1, 0x14); // Type is UNREACH, and no UDP data
	iReceived = EFalse;
	iCNcmIcmpV6Receiver->CheckError();
	EUNIT_ASSERT( !iReceived );
	// iAddress should be zero because it is not set if UDP is not found
	EUNIT_ASSERT_EQUALS( 0, iCNcmIcmpV6Receiver->iAddress.Address() );
	}
	
void UT_CNcmIcmpReceiver::UT_OtherIcmpV6MessageTestL()
	{
	CreateICMPv6Error(2, 0, 0x11); // Type is not UNREACH
	iReceived = EFalse;
	iCNcmIcmpV6Receiver->CheckError();
	EUNIT_ASSERT( !iReceived );
	// should return zero because address is not IPv4 compatible
	EUNIT_ASSERT_EQUALS( 0, iCNcmIcmpV6Receiver->iAddress.Address() );
	}

void UT_CNcmIcmpReceiver::CreateICMPError(TChar aType, TChar aCode, TChar aProtocol)
	{
	TBuf8<32> msg;
	msg.AppendFill(0, 32);
	msg[0] = aType;
	msg[1] = aCode;
	msg[17] = aProtocol;
	msg[24] = 10; //ip-address 10.21.32.70
	msg[25] = 21;
	msg[26] = 32;
	msg[27] = 70;
	msg[30] = 0x13; //port 5060
	msg[31] = 0xc4;
	iCNcmIcmpV4Receiver->iData.Copy( msg );
	}


void UT_CNcmIcmpReceiver::CreateIPv4CompatibleICMPv6Error(TChar aType, TChar aCode, TChar aProtocol)
	{
    TBuf8<52> msg;
	msg.AppendFill(0, 52);
	msg[0] = aType;
	msg[1] = aCode;
	msg[12] = 0;
	msg[13] = 40; // Payload length in octets
	msg[14] = aProtocol;
	msg[32] = 0x0; //ip-address 0:0:0:0:0:0:ffff:ffff (v4 compatible)
	msg[33] = 0x0;
	msg[34] = 0x0;
	msg[35] = 0x0;
	msg[36] = 0x0;
	msg[37] = 0x0;
	msg[38] = 0x0;
	msg[39] = 0x0;
	msg[40] = 0x0;
	msg[41] = 0x0;
	msg[42] = 0x0;
	msg[43] = 0x0;
	msg[44] = 0xFF;
	msg[45] = 0xFF;
	msg[46] = 0xFF;
	msg[47] = 0xFF;
	msg[50] = 0x13; //port 5060
	msg[51] = 0xc4;
	iCNcmIcmpV6Receiver->iData.Copy( msg );
	}
	
	
void UT_CNcmIcmpReceiver::CreateICMPv6Error(TChar aType, TChar aCode, TChar aProtocol)
	{
	TBuf8<52> msg;
	msg.AppendFill(0, 52);
	msg[0] = aType;
	msg[1] = aCode;
	msg[8] = 160; // version = IPv6 in 4 upper bits (1110000)
	msg[12] = 0;
	msg[13] = 40; // Payload length in octets
	msg[14] = aProtocol;
	msg[32] = 0x12; //ip-address 1234:5678:9ABC:DEF0:1234:5678:9ABC:DEF0
	msg[33] = 0x34;
	msg[34] = 0x56;
	msg[35] = 0x78;
	msg[36] = 0x9A;
	msg[37] = 0xBC;
	msg[38] = 0xDE;
	msg[39] = 0xF0;
	msg[40] = 0x12;
	msg[41] = 0x34;
	msg[42] = 0x56;
	msg[43] = 0x78;
	msg[44] = 0x9A;
	msg[45] = 0xBC;
	msg[46] = 0xDE;
	msg[47] = 0xF0;
	msg[50] = 0x13; //port 5060
	msg[51] = 0xc4;
	iCNcmIcmpV6Receiver->iData.Copy( msg );
	}
  

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNcmIcmpReceiver,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "UT_IcmpUnreachMessageNoUDPTestL - test0",
    "CNcmIcmpReceiver",
    "UT_IcmpUnreachMessageNoUDPTestL - test0",
    "FUNCTIONALITY",
    SetupL, UT_IcmpUnreachMessageNoUDPTestL, Teardown)
    
EUNIT_TEST(
    "UT_OtherIcmpMessageTestL - test1",
    "CNcmIcmpReceiver",
    "UT_OtherIcmpMessageTestL - test1",
    "FUNCTIONALITY",
    SetupL, UT_OtherIcmpMessageTestL, Teardown)

EUNIT_TEST(
    "UT_IcmpV6UnreachMessageTestL - test2",
    "CNcmIcmpReceiver",
    "UT_IcmpV6UnreachMessageTestL - test2",
    "FUNCTIONALITY",
    SetupL, UT_IcmpV6UnreachMessageTestL, Teardown)
    
EUNIT_TEST(
    "UT_IcmpV6UnreachMessageNoUDPTestL - test1",
    "CNcmIcmpReceiver",
    "UT_IcmpV6UnreachMessageNoUDPTestL - test1",
    "FUNCTIONALITY",
    SetupL, UT_IcmpV6UnreachMessageNoUDPTestL, Teardown)

EUNIT_TEST(
    "UT_OtherIcmpV6MessageTestL - test1",
    "CNcmIcmpReceiver",
    "UT_OtherIcmpV6MessageTestL - test1",
    "FUNCTIONALITY",
    SetupL, UT_OtherIcmpV6MessageTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
