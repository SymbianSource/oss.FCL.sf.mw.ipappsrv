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
#include "ut_cnatfwunsafudpsender.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <CommDbConnPref.h>

//  INTERNAL INCLUDES
#include "natfwunsaftransporttestutils.h"
#include "cnatfwunsafudpsender.h"
#include "natfwunsafmessage.h"

//  CONSTANTS
_LIT8( KTestSharedSecret, "fffoooffof" );

// CONSTRUCTION
UT_CNATFWUNSAFUdpSender* UT_CNATFWUNSAFUdpSender::NewL()
    {
    UT_CNATFWUNSAFUdpSender* self = UT_CNATFWUNSAFUdpSender::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFUdpSender* UT_CNATFWUNSAFUdpSender::NewLC()
    {
    UT_CNATFWUNSAFUdpSender* self = new( ELeave ) UT_CNATFWUNSAFUdpSender();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFUdpSender::~UT_CNATFWUNSAFUdpSender()
    {
    }

// Default constructor
UT_CNATFWUNSAFUdpSender::UT_CNATFWUNSAFUdpSender()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFUdpSender::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFUdpSender::SetupL()
    {
    User::LeaveIfError( iServer.Connect( 8 ) );
    User::LeaveIfError( iConn.Open( iServer, KConnectionTypeDefault ) );

    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId(10);
    User::LeaveIfError( iConn.Start( prefs ) );

    User::LeaveIfError( iSocket.Open( iServer,
                                      KAfInet,
                                      KSockDatagram,
                                      KProtocolInetUdp,
                                      iConn ) );

    iMsg = NATFWUNSAFTransportTestUtils::CreateUNSAFBindingRequestL();

    iAddr = TInetAddr( INET_ADDR( 127, 0, 0, 1 ) );

    iSender = CNATFWUNSAFUdpSender::NewL( iSocket, *this );

    iIsMsgSent = EFalse;
    }

void UT_CNATFWUNSAFUdpSender::Teardown()
    {
    delete iMsg;
    delete iSender;
    iSocket.Close();
    iConn.Close();
    iServer.Close();
    }


// From MNATFWUNSAFUdpSenderObserver:
void UT_CNATFWUNSAFUdpSender::UNSAFUdpMessageSentL()
    {
    // Allocate some memory, so that this function leaves
    // in memory allocation failure simulation.
    HBufC8* temp = HBufC8::NewL( 100 );
    delete temp;

    iIsMsgSent = ETrue;
    }


// From MNATFWUNSAFUdpSenderObserver:
void UT_CNATFWUNSAFUdpSender::UNSAFUdpMessageFailure( TInt aError )
    {
    iLastError = aError;
    }


void UT_CNATFWUNSAFUdpSender::SendTestL()
    {
    iSender->SendL( iAddr, *iMsg, KTestSharedSecret );

    EUNIT_ASSERT( iSender->iEncoded != 0 );

    CompleteSendL( KErrNone );

    EUNIT_ASSERT( iIsMsgSent );
    EUNIT_ASSERT( iSender->iEncoded == 0 );

    iSender->SendL( iAddr, *iMsg, KNullDesC8 );

    EUNIT_ASSERT( iSender->iEncoded != 0 );

    CompleteSendL( KErrNone );

    EUNIT_ASSERT( iIsMsgSent );
    EUNIT_ASSERT( iSender->iEncoded == 0 );
    }

void UT_CNATFWUNSAFUdpSender::SendFailedTestL()
    {
    iSender->SendL( iAddr, *iMsg, KTestSharedSecret );

    TInt error( KErrGeneral );
    CompleteSendL( error );

    EUNIT_ASSERT( iLastError == error );
    EUNIT_ASSERT( !iIsMsgSent );
    EUNIT_ASSERT( iSender->iEncoded == 0 );
    }

void UT_CNATFWUNSAFUdpSender::CancelSendTestL()
    {
    iSender->SendL( iAddr, *iMsg, KTestSharedSecret );

    iSender->Cancel();

    EUNIT_ASSERT( !iSender->IsActive() );
    }

void UT_CNATFWUNSAFUdpSender::QueueTestL()
    {
    iSender->SendL( iAddr, *iMsg, KTestSharedSecret );
    EUNIT_ASSERT( iSender->iEncoded != 0 );

    TRAPD( err, iSender->SendL( iAddr, *iMsg, KTestSharedSecret ) );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( err == KErrInUse );
    EUNIT_ASSERT( iSender->iEncoded != 0 );
    EUNIT_ASSERT( !iIsMsgSent );

    CompleteSendL( KErrNone );
    EUNIT_ASSERT( iSender->iEncoded == 0 );
    EUNIT_ASSERT( iIsMsgSent );
    }


// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFUdpSender::CompleteSendL( TInt aStatus )
    {
    iIsMsgSent = EFalse;

    // Complete send request
    iSender->Cancel();

    iSender->iStatus = aStatus;

    TRAPD( err, iSender->RunL() );
    if ( err )
        {
        // Simulate active scheduler behavior
        err = iSender->RunError( err );
        User::LeaveIfError( err );
        }
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFUdpSender,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SendTestL - test ",
    "CNATFWUNSAFUdpSender",
    "SendTestL",
    "FUNCTIONALITY",
    SetupL, SendTestL, Teardown)

EUNIT_TEST(
    "SendFailedTestL - test ",
    "CNATFWUNSAFUdpSender",
    "SendFailedTestL",
    "FUNCTIONALITY",
    SetupL, SendFailedTestL, Teardown)

EUNIT_TEST(
    "CancelSendTestL - test ",
    "CNATFWUNSAFUdpSender",
    "CancelSendTestL",
    "FUNCTIONALITY",
    SetupL, CancelSendTestL, Teardown)

EUNIT_TEST(
    "QueueTestL - test ",
    "CNATFWUNSAFUdpSender",
    "QueueTestL",
    "FUNCTIONALITY",
    SetupL, QueueTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
