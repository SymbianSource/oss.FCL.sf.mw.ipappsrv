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
#include "ut_cnatfwunsaftlstransport.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <CommDbConnPref.h>

//  INTERNAL INCLUDES
#include "natfwunsaftransporttestutils.h"
#include "cnatfwunsaftlstransport.h"
#include "natfwunsafmessage.h"
#include "natfwunsafmessagefactory.h"

// CONSTRUCTION
UT_CNATFWUNSAFTlsTransport* UT_CNATFWUNSAFTlsTransport::NewL()
    {
    UT_CNATFWUNSAFTlsTransport* self = UT_CNATFWUNSAFTlsTransport::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFTlsTransport* UT_CNATFWUNSAFTlsTransport::NewLC()
    {
    UT_CNATFWUNSAFTlsTransport* self = new( ELeave ) UT_CNATFWUNSAFTlsTransport();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFTlsTransport::~UT_CNATFWUNSAFTlsTransport()
    {
    }

// Default constructor
UT_CNATFWUNSAFTlsTransport::UT_CNATFWUNSAFTlsTransport()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFTlsTransport::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFTlsTransport::SetupL()
    {
    iTlsConnected = EFalse;

    User::LeaveIfError( iServer.Connect( 8 ) );
    User::LeaveIfError( iConn.Open( iServer, KConnectionTypeDefault ) );

    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId(10);
    User::LeaveIfError( iConn.Start( prefs ) );

    TInetAddr addr( INET_ADDR( 127, 0, 0, 1 ) );

    iTransport = CNATFWUNSAFTlsTransport::NewL( iServer, iConn, addr, *this );
    }


void UT_CNATFWUNSAFTlsTransport::Teardown()
    {
    delete iTransport;
    iConn.Close();
    iServer.Close();
    }


// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsTransport::TlsConnectedL()
    {
    // Allocate some memory, so that this function leaves
    // in memory allocation failure simulation.
    HBufC8* temp = HBufC8::NewL( 100 );
    delete temp;

    iTlsConnected = ETrue;
    }


// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsTransport::IncomingMessageL( CNATFWUNSAFMessage* /*aMessage*/ )
    {
    // Allocate some memory, so that this function leaves
    // in memory allocation failure simulation.
    HBufC8* temp = HBufC8::NewL( 100 );
    delete temp;
    // TODO: delete aMessage
    }


// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsTransport::ErrorOccured( TInt aError )
    {
    iLastError = aError;
    }

void UT_CNATFWUNSAFTlsTransport::ConnectTestL()
    {
    CompleteTcpConnectL( KErrNone );
    CompleteTlsHandshakeL( KErrNone );

    EUNIT_ASSERT( iTlsConnected );
    EUNIT_ASSERT( iTransport->IsConnected() );
    }

void UT_CNATFWUNSAFTlsTransport::TcpConnectFailedTestL()
    {
    TInt error( KErrCouldNotConnect );
    CompleteTcpConnectL( error );

    EUNIT_ASSERT( iTransport->CurrentState().State() ==
                TNATFWUNSAFTlsTransportStateBase::ETcpConnecting );
    EUNIT_ASSERT( !iTlsConnected );
    EUNIT_ASSERT( !iTransport->IsConnected() );
    EUNIT_ASSERT( iLastError == error );
    }

void UT_CNATFWUNSAFTlsTransport::TlsHandshakeFailedTestL()
    {
    CompleteTcpConnectL( KErrNone );

    TInt error( KErrGeneral );
    CompleteTlsHandshakeL( error );

    EUNIT_ASSERT( iTransport->CurrentState().State() ==
                TNATFWUNSAFTlsTransportStateBase::ETlsConnecting );
    EUNIT_ASSERT( !iTlsConnected );
    EUNIT_ASSERT( !iTransport->IsConnected() );
    EUNIT_ASSERT( iLastError == error );
    }

void UT_CNATFWUNSAFTlsTransport::CancelTcpConnectTestL()
    {
    iTransport->Cancel();
    }

void UT_CNATFWUNSAFTlsTransport::CancelTlsHandshakeTestL()
    {
       CompleteTcpConnectL( KErrNone );

       iTransport->Cancel();
    }

void UT_CNATFWUNSAFTlsTransport::SendOverTlsTestL()
    {
    CNATFWUNSAFMessage* message =
        NATFWUNSAFTransportTestUtils::CreateUNSAFBindingRequestLC();

    TInt err( KErrNone );
    TRAP( err, iTransport->SendL( *message, KNullDesC8 ) );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( err == KErrNotReady );

    CompleteTcpConnectL( KErrNone );

    TRAP( err, iTransport->SendL( *message, KNullDesC8 ) );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( err == KErrNotReady );

    CompleteTlsHandshakeL( KErrNone );

    TRAP( err, iTransport->SendL( *message, KNullDesC8 ) );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT( err == KErrNone );
    CleanupStack::PopAndDestroy( message );
    }

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFTlsTransport::CompleteTcpConnectL( TInt aStatus )
    {
    // Tcp connect is initiated already at transport construction,
    // complete connect tcp request

    iTransport->Cancel();

    iTransport->iStatus = aStatus;

    TRAPD( err, iTransport->RunL() );
    if ( err )
        {
        // Simulate active scheduler behavior
        err = iTransport->RunError( err );
        User::LeaveIfError( err );
        }
    }

void UT_CNATFWUNSAFTlsTransport::CompleteTlsHandshakeL( TInt aStatus )
    {
    // After tcp has connected, tls handshake is started
    // complete tls handshake request

    // Canceling CSecureSocket will distract cppunit leave failure simulation
    // becuase of trap inside CSecureSocket
    iTransport->Cancel();

    iTransport->iStatus = aStatus;

    TRAPD( err, iTransport->RunL() );
    if ( err )
        {
        // Simulate active scheduler behavior
        err = iTransport->RunError( err );
        User::LeaveIfError( err );
        }
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFTlsTransport,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ConnectTestL - test ",
    "CNATFWUNSAFTlsTransport",
    "ConnectTestL",
    "FUNCTIONALITY",
    SetupL, ConnectTestL, Teardown)

EUNIT_TEST(
    "TcpConnectFailedTestL - test ",
    "CNATFWUNSAFTlsTransport",
    "TcpConnectFailedTestL",
    "FUNCTIONALITY",
    SetupL, TcpConnectFailedTestL, Teardown)

EUNIT_TEST(
    "TlsHandshakeFailedTestL - test ",
    "CNATFWUNSAFTlsTransport",
    "TlsHandshakeFailedTestL",
    "FUNCTIONALITY",
    SetupL, TlsHandshakeFailedTestL, Teardown)

EUNIT_TEST(
    "CancelTcpConnectTestL - test ",
    "CNATFWUNSAFTlsTransport",
    "CancelTcpConnectTestL",
    "FUNCTIONALITY",
    SetupL, CancelTcpConnectTestL, Teardown)

EUNIT_TEST(
    "CancelTlsHandshakeTestL - test ",
    "CNATFWUNSAFTlsTransport",
    "CancelTlsHandshakeTestL",
    "FUNCTIONALITY",
    SetupL, CancelTlsHandshakeTestL, Teardown)

EUNIT_TEST(
    "SendOverTlsTestL - test ",
    "CNATFWUNSAFTlsTransport",
    "SendOverTlsTestL",
    "FUNCTIONALITY",
    SetupL, SendOverTlsTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
