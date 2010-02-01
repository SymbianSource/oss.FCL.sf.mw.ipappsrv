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
#include "ut_cnatfwunsaftlssender.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <CommDbConnPref.h>

//  INTERNAL INCLUDES
#include "natfwunsaftransporttestutils.h"
#include "cnatfwunsaftlssender.h"
#include "natfwunsafmessage.h"

// CONSTRUCTION
UT_CNATFWUNSAFTlsSender* UT_CNATFWUNSAFTlsSender::NewL()
    {
    UT_CNATFWUNSAFTlsSender* self = UT_CNATFWUNSAFTlsSender::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFTlsSender* UT_CNATFWUNSAFTlsSender::NewLC()
    {
    UT_CNATFWUNSAFTlsSender* self = new( ELeave ) UT_CNATFWUNSAFTlsSender();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFTlsSender::~UT_CNATFWUNSAFTlsSender()
    {
    }

// Default constructor
UT_CNATFWUNSAFTlsSender::UT_CNATFWUNSAFTlsSender()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFTlsSender::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFTlsSender::SetupL()
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
                                      KSockStream,
                                      KProtocolInetTcp,
                                      iConn ) );

    _LIT( KTLS1,"TLS1.0" );
    iSecureSocket = CSecureSocket::NewL( iSocket, KTLS1 );

    iSender = CNATFWUNSAFTlsSender::NewL( *iSecureSocket, *this );

    iMsg = NATFWUNSAFTransportTestUtils::CreateUNSAFBindingRequestL();
    }


void UT_CNATFWUNSAFTlsSender::Teardown()
    {
    delete iMsg;
    delete iSender;

    delete iSecureSocket;
    iSocket.Close();
    iConn.Close();
    iServer.Close();
    }


// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsSender::TlsConnectedL()
    {
    // Allocate some memory, so that this function leaves
    // in memory allocation failure simulation.
    HBufC8* temp = HBufC8::NewL( 100 );
    delete temp;
    }

// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsSender::IncomingMessageL( CNATFWUNSAFMessage* /*aMessage*/ )
    {
    // Allocate some memory, so that this function leaves
    // in memory allocation failure simulation.
    HBufC8* temp = HBufC8::NewL( 100 );
    delete temp;
    // TODO: delete aMessage
    }

// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsSender::ErrorOccured( TInt aError )
    {
    iLastError = aError;
    }

void UT_CNATFWUNSAFTlsSender::SendTestL()
    {
    iSender->SendL( *iMsg, KNullDesC8 );

    EUNIT_ASSERT( iSender->iEncoded != 0 );

    CompleteSendL( KErrNone );

    EUNIT_ASSERT( iSender->iEncoded == 0 );
    }

void UT_CNATFWUNSAFTlsSender::SendFailedTestL()
    {
    iSender->SendL( *iMsg, KNullDesC8 );

    TInt error( KErrGeneral );
    CompleteSendL( error );

    EUNIT_ASSERT( iLastError == error );
    EUNIT_ASSERT( iSender->iEncoded == 0 );
    }

void UT_CNATFWUNSAFTlsSender::CancelSendTestL()
    {
    iSender->SendL( *iMsg, KNullDesC8 );

    iSender->Cancel();

    EUNIT_ASSERT( !iSender->IsActive() );
    }

void UT_CNATFWUNSAFTlsSender::QueueTestL()
    {
    iSender->SendL( *iMsg, KNullDesC8 );
    EUNIT_ASSERT( iSender->iEncoded != 0 );

    TRAPD( err, iSender->SendL( *iMsg, KNullDesC8 ) );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( err == KErrInUse );
    EUNIT_ASSERT( iSender->iEncoded != 0 );

    CompleteSendL( KErrNone );
    EUNIT_ASSERT( iSender->iEncoded == 0 );
    }

void UT_CNATFWUNSAFTlsSender::RunErrorTestL()
    {
    // Currently there's nothing in TLS sender's RunL which could leave
    // and cause RunError. Test the method anyway.

    TInt err = iSender->RunError( KErrGeneral );
    EUNIT_ASSERT( iLastError == KErrGeneral );
    EUNIT_ASSERT( err == KErrNone );

    err = iSender->RunError( KErrNoMemory );
    EUNIT_ASSERT( iLastError == KErrNoMemory );
    EUNIT_ASSERT( err == KErrNoMemory );
    }

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFTlsSender::CompleteSendL( TInt aStatus )
    {
    // Complete send request

    // Canceling CSecureSocket will distract cppunit leave failure simulation
    // becuase of trap inside CSecureSocket
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
    UT_CNATFWUNSAFTlsSender,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SendTestL - test ",
    "CNATFWUNSAFTlsSender",
    "SendTestL",
    "FUNCTIONALITY",
    SetupL, SendTestL, Teardown)

EUNIT_TEST(
    "SendFailedTestL - test ",
    "CNATFWUNSAFTlsSender",
    "SendFailedTestL",
    "FUNCTIONALITY",
    SetupL, SendFailedTestL, Teardown)

EUNIT_TEST(
    "CancelSendTestL - test ",
    "CNATFWUNSAFTlsSender",
    "CancelSendTestL",
    "FUNCTIONALITY",
    SetupL, CancelSendTestL, Teardown)

EUNIT_TEST(
    "QueueTestL - test ",
    "CNATFWUNSAFTlsSender",
    "QueueTestL",
    "FUNCTIONALITY",
    SetupL, QueueTestL, Teardown)

EUNIT_TEST(
    "RunErrorTestL - test ",
    "CNATFWUNSAFTlsSender",
    "RunErrorTestL",
    "FUNCTIONALITY",
    SetupL, RunErrorTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
