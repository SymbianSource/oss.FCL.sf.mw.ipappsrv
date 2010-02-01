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
#include "ut_cnatfwicmprecv.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <in_sock.h>
#include <CommDbConnPref.h>

//  INTERNAL INCLUDES
#include "cnatfwunsaficmpv4receiver.h"
#include "cnatfwunsaficmpv6receiver.h"

//  CONSTANTS
// Just use own loopback address for pinging
const TInt KDestAddr( INET_ADDR( 127, 0, 0, 1 ) );
const TInt KDestPort( 1024 );

// CONSTRUCTION
UT_CNATFWUNSAFIcmpRecv* UT_CNATFWUNSAFIcmpRecv::NewL()
    {
    UT_CNATFWUNSAFIcmpRecv* self = UT_CNATFWUNSAFIcmpRecv::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFIcmpRecv* UT_CNATFWUNSAFIcmpRecv::NewLC()
    {
    UT_CNATFWUNSAFIcmpRecv* self = new( ELeave ) UT_CNATFWUNSAFIcmpRecv();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFIcmpRecv::~UT_CNATFWUNSAFIcmpRecv()
    {
    }

// Default constructor
UT_CNATFWUNSAFIcmpRecv::UT_CNATFWUNSAFIcmpRecv()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFIcmpRecv::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFIcmpRecv::SetupL()
    {
    User::LeaveIfError( iServer.Connect( KESockDefaultMessageSlots ) );

    User::LeaveIfError( iConnection.Open( iServer, KConnectionTypeDefault ) );
    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId(10);
    User::LeaveIfError( iConnection.Start( prefs ) );

    iReceiver = CIcmpV4Receiver::NewL( *this, iServer );
    iReceiverV6 = CIcmpV6Receiver::NewL( *this, iServer );

    // If iIsSchedulerBlocked is set to true, CActiveScheduler::Stop() is
    // called when IcmpError callback occurs or in tearDown. If there's no
    // previous call to CActiveScheduler::Start() and flag is set to true,
    // scheduler panics
    iIsSchedulerBlocked=EFalse;
    }

void UT_CNATFWUNSAFIcmpRecv::Teardown()
    {
    if( iIsSchedulerBlocked )
        {
        CActiveScheduler::Stop();
        iIsSchedulerBlocked=EFalse;
        }
    delete iReceiver;
    delete iReceiverV6;

    iConnection.Close();
    iServer.Close();
    }

void UT_CNATFWUNSAFIcmpRecv::IcmpUnreachMessageNoUDPTestL()
    {
    CreateICMPError( 3, 1, 0x14 ); // Type is UNREACH, but there is no UDP data
    iReceived = EFalse;
    iReceiver->CheckError();
    EUNIT_ASSERT( !Received() );
    // iAddress should be zero because it is not set if UDP is not found
    EUNIT_ASSERT( 0 == iReceiver->iAddress.Address() );
    }

void UT_CNATFWUNSAFIcmpRecv::OtherIcmpMessageTestL()
    {
    CreateICMPError( 2, 0, 0x11 ); // Type is not UNREACH
    iReceived = EFalse;
    iReceiver->CheckError();
    EUNIT_ASSERT( !Received() );
    // iAddress should be zero because it is not set in other icmp-messages
    EUNIT_ASSERT( 0 == iReceiver->iAddress.Address() );
    }

void UT_CNATFWUNSAFIcmpRecv::CreateICMPError( TChar aType, TChar aCode,
    TChar aProtocol )
    {
    TBuf8<32> msg;
    msg.AppendFill( 0, 32 );
    msg[0] = aType;
    msg[1] = aCode;
    msg[17] = aProtocol;
    msg[24] = 10; //ip-address 10.21.32.70
    msg[25] = 21;
    msg[26] = 32;
    msg[27] = 70;
    msg[30] = 0x13; //port 5060
    msg[31] = 0xc4;
    iReceiver->iData.Copy( msg );
    }

void UT_CNATFWUNSAFIcmpRecv::IcmpV6UnreachMessageTestL()
    {
    // Type is UNREACH and there is UDP data
    CreateIPv4CompatibleICMPv6Error( 1, 1, 0x11 );

    iReceived = EFalse;
    iReceiverV6->CheckError();
    EUNIT_ASSERT( Received() );
    EUNIT_ASSERT( 0xffffffff == iReceiverV6->iAddress.Address() );
    }

void UT_CNATFWUNSAFIcmpRecv::IcmpV6UnreachMessageNoUDPTestL()
    {
    // Type is UNREACH, and no UDP data
    CreateIPv4CompatibleICMPv6Error( 1, 1, 0x14 );

    iReceived = EFalse;
    iReceiverV6->CheckError();
    EUNIT_ASSERT( !Received() );
    // iAddress should be zero because it is not set if UDP is not found
    EUNIT_ASSERT( 0 == iReceiverV6->iAddress.Address() );
    }

void UT_CNATFWUNSAFIcmpRecv::OtherIcmpV6MessageTestL()
    {
    CreateICMPv6Error( 2, 0, 0x11 ); // Type is not UNREACH
    iReceived = EFalse;
    iReceiverV6->CheckError();
    EUNIT_ASSERT( !Received() );
    // should return zero because address is not IPv4 compatible
    EUNIT_ASSERT( 0 == iReceiverV6->iAddress.Address() );
    }

void UT_CNATFWUNSAFIcmpRecv::CreateIPv4CompatibleICMPv6Error( TChar aType, TChar aCode,
                                                     TChar aProtocol )
    {
    TBuf8<52> msg;
    msg.AppendFill( 0, 52 );
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
    iReceiverV6->iData.Copy( msg );
    }

void UT_CNATFWUNSAFIcmpRecv::CreateICMPv6Error( TChar aType, TChar aCode,
    TChar aProtocol )
    {
    TBuf8<52> msg;
    msg.AppendFill( 0, 52 );
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
    iReceiverV6->iData.Copy( msg );
    }


void UT_CNATFWUNSAFIcmpRecv::LiveTestL()
    {
    iReceived = EFalse;
    iReceivedAddr.SetAddress( 0 );

    SendUDPL();
    iIsSchedulerBlocked=ETrue;
    CActiveScheduler::Start();
    EUNIT_ASSERT( ETrue == iReceived );
    EUNIT_ASSERT( KDestAddr == iReceivedAddr.Address() );

    // Send another packet
    SendUDPL();
    iIsSchedulerBlocked=ETrue;
    CActiveScheduler::Start();
    EUNIT_ASSERT( ETrue == iReceived );
    EUNIT_ASSERT( KDestAddr == iReceivedAddr.Address() );
    }

void UT_CNATFWUNSAFIcmpRecv::IcmpError( const TInetAddr& aAddress )
    {
    if( iIsSchedulerBlocked )
        {
        CActiveScheduler::Stop();
        iIsSchedulerBlocked=EFalse;
        }
    iReceived = ETrue;
    iReceivedAddr = aAddress;
    }

void UT_CNATFWUNSAFIcmpRecv::SendUDPL()
    {
    TInetAddr addr( KDestPort );
    addr.SetAddress( KDestAddr );

    TRequestStatus status;
    iUdpSocket.Open( iServer, KAfInet, KSockDatagram, KProtocolInetUdp );
    iUdpSocket.SendTo( _L8( "Ping" ), addr, 0, status );
    User::WaitForRequest( status );
    iUdpSocket.Close();
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFIcmpRecv,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "IcmpUnreachMessageNoUDPTestL - test ",
    "CNATFWUNSAFIcmpRecveiver",
    "IcmpUnreachMessageNoUDPTestL",
    "FUNCTIONALITY",
    SetupL, IcmpUnreachMessageNoUDPTestL, Teardown)

EUNIT_TEST(
    "OtherIcmpMessageTestL - test ",
    "CNATFWUNSAFIcmpRecveiver",
    "OtherIcmpMessageTestL",
    "FUNCTIONALITY",
    SetupL, OtherIcmpMessageTestL, Teardown)

EUNIT_TEST(
    "IcmpV6UnreachMessageTestL - test ",
    "CNATFWUNSAFIcmpRecveiver",
    "IcmpV6UnreachMessageTestL",
    "FUNCTIONALITY",
    SetupL, IcmpV6UnreachMessageTestL, Teardown)

EUNIT_TEST(
    "IcmpV6UnreachMessageNoUDPTestL - test ",
    "CNATFWUNSAFIcmpRecveiver",
    "IcmpV6UnreachMessageNoUDPTestL",
    "FUNCTIONALITY",
    SetupL, IcmpV6UnreachMessageNoUDPTestL, Teardown)

EUNIT_TEST(
    "OtherIcmpV6MessageTestL - test ",
    "CNATFWUNSAFIcmpRecveiver",
    "OtherIcmpV6MessageTestL",
    "FUNCTIONALITY",
    SetupL, OtherIcmpV6MessageTestL, Teardown)

EUNIT_TEST(
    "LiveTestL - test ",
    "CNATFWUNSAFIcmpRecveiver",
    "LiveTestL",
    "FUNCTIONALITY",
    SetupL, LiveTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
