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
#include "ut_cnatfwunsaftlsreceiver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <CommDbConnPref.h>

//  INTERNAL INCLUDES
#include "natfwunsaftransporttestutils.h"
#include "cnatfwunsaftlsreceiver.h"
#include "cnatfwunsafmsgassembler.h"
#include "natfwunsafmessage.h"
#include "natfwunsafaddressattribute.h"

// CONSTRUCTION
UT_CNATFWUNSAFTlsReceiver* UT_CNATFWUNSAFTlsReceiver::NewL()
    {
    UT_CNATFWUNSAFTlsReceiver* self = UT_CNATFWUNSAFTlsReceiver::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFTlsReceiver* UT_CNATFWUNSAFTlsReceiver::NewLC()
    {
    UT_CNATFWUNSAFTlsReceiver* self = new( ELeave ) UT_CNATFWUNSAFTlsReceiver();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFTlsReceiver::~UT_CNATFWUNSAFTlsReceiver()
    {
    }

// Default constructor
UT_CNATFWUNSAFTlsReceiver::UT_CNATFWUNSAFTlsReceiver()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFTlsReceiver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFTlsReceiver::SetupL()
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

    iReceiver = CNATFWUNSAFTlsReceiver::NewL( *iSecureSocket, *this );

    iLastReceivedMsg = 0;
    iLastError = KErrNone;
    iIncomingCount = 0;
    iDeleteReceiverInCallback = EFalse;
    }


void UT_CNATFWUNSAFTlsReceiver::Teardown()
    {
    delete iLastReceivedMsg;
    delete iReceiver;

    delete iSecureSocket;
    iSocket.Close();
    iConn.Close();
    iServer.Close();
    }


// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsReceiver::TlsConnectedL()
    {
    // Allocate some memory, so that this function leaves
    // in memory allocation failure simulation.
    HBufC8* temp = HBufC8::NewL( 100 );
    delete temp;
    }

// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsReceiver::IncomingMessageL( CNATFWUNSAFMessage* aMessage )
    {
    // Allocate some memory, so that this function leaves
    // in memory allocation failure simulation.
    HBufC8* temp = HBufC8::NewL( 100 );
    delete temp;

    iIncomingCount++;
    delete iLastReceivedMsg;
    iLastReceivedMsg = aMessage;

    if ( iDeleteReceiverInCallback )
        {
        delete iReceiver;
        iReceiver = 0;
        }
    }


// From MNATFWUNSAFTlsTransportObserver:
void UT_CNATFWUNSAFTlsReceiver::ErrorOccured( TInt aError )
    {
    iLastError = aError;
    }

void UT_CNATFWUNSAFTlsReceiver::ReceiveTestL()
    {
    // Receive one UNSAF message in one piece
    //
    CNATFWUNSAFMessage* message =
        NATFWUNSAFTransportTestUtils::CreateUNSAFBindingResponseLC();

    CBufBase* data = message->EncodeL();
    CleanupStack::PushL( data );

    HBufC8* dataPart1 = data->Ptr( 0 ).AllocLC();

    CompleteReceiveL( dataPart1->Des(), KErrNone );
    CleanupStack::PopAndDestroy( dataPart1 );

    EUNIT_ASSERT( iLastReceivedMsg != 0 );
    EUNIT_ASSERT( iLastReceivedMsg->Validate() );
    EUNIT_ASSERT( iReceiver->iNextLength ==
                CNATFWUNSAFMsgAssembler::EDefaultBufferSize );

    EUNIT_ASSERT( iReceiver->iMsgAssembler->iStateValue ==
                TNATFWUNSAFMsgStateBase::EMsgInit );

    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( message );
    }


void UT_CNATFWUNSAFTlsReceiver::ReceiveTest2L()
    {
    // Receive one UNSAF message in two pieces
    //
    CNATFWUNSAFMessage* message =
        NATFWUNSAFTransportTestUtils::CreateUNSAFBindingResponseLC();

    CBufBase* data = message->EncodeL();
    CleanupStack::PushL( data );

    TInt delim = data->Size() / 2;

    HBufC8* dataPart1 = data->BackPtr( delim ).AllocLC();
    CompleteReceiveL( dataPart1->Des(), KErrNone );
    CleanupStack::PopAndDestroy( dataPart1 );

    EUNIT_ASSERT( iLastReceivedMsg == 0 );
    EUNIT_ASSERT( iReceiver->iNextLength == data->Ptr( 0 ).Length() - delim );
    EUNIT_ASSERT( iReceiver->iMsgAssembler->iStateValue ==
                TNATFWUNSAFMsgStateBase::EMsgHeaderStart );

    HBufC8* dataPart2 = data->Ptr( delim ).AllocLC();
    CompleteReceiveL( dataPart2->Des(), KErrNone );
    CleanupStack::PopAndDestroy( dataPart2 );

    EUNIT_ASSERT( iLastReceivedMsg != 0 );
    EUNIT_ASSERT( iLastReceivedMsg->Validate() );
    EUNIT_ASSERT( iReceiver->iNextLength ==
                CNATFWUNSAFMsgAssembler::EDefaultBufferSize );

    EUNIT_ASSERT( iReceiver->iMsgAssembler->iStateValue ==
                TNATFWUNSAFMsgStateBase::EMsgInit );

    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( message );
    }


void UT_CNATFWUNSAFTlsReceiver::ReceiveTest3L()
    {
    // Receive one UNSAF message in three pieces
    //
    CNATFWUNSAFMessage* message =
        NATFWUNSAFTransportTestUtils::CreateUNSAFBindingResponseLC();

    CBufBase* data = message->EncodeL();
    CleanupStack::PushL( data );

    TInt delim = 2;

    HBufC8* dataPart1 = data->BackPtr( delim ).AllocLC();
    CompleteReceiveL( dataPart1->Des(), KErrNone );
    CleanupStack::PopAndDestroy( dataPart1 );

    EUNIT_ASSERT( iLastReceivedMsg == 0 );
    EUNIT_ASSERT( iReceiver->iNextLength ==
                CNATFWUNSAFMsgAssembler::EDefaultBufferSize );
    EUNIT_ASSERT( iReceiver->iMsgAssembler->iStateValue ==
                TNATFWUNSAFMsgStateBase::EMsgInit );

    TInt delim2 = ( data->Size() - delim ) / 2;

    HBufC8* dataPart2 = data->Ptr( delim ).Left( delim2 ).AllocLC();
    CompleteReceiveL( dataPart2->Des(), KErrNone );
    CleanupStack::PopAndDestroy( dataPart2 );

    EUNIT_ASSERT( iLastReceivedMsg == 0 );
    EUNIT_ASSERT( iReceiver->iNextLength ==
                data->Ptr( 0 ).Length() - delim - delim2 );

    EUNIT_ASSERT( iReceiver->iMsgAssembler->iStateValue ==
                TNATFWUNSAFMsgStateBase::EMsgHeaderStart );

    HBufC8* dataPart3 = data->Ptr( delim ).Mid( delim2 ).AllocLC();
    CompleteReceiveL( dataPart3->Des(), KErrNone );
    CleanupStack::PopAndDestroy( dataPart3 );

    EUNIT_ASSERT( iLastReceivedMsg != 0 );
    EUNIT_ASSERT( iLastReceivedMsg->Validate() );
    EUNIT_ASSERT( iReceiver->iNextLength ==
                CNATFWUNSAFMsgAssembler::EDefaultBufferSize );

    EUNIT_ASSERT( iReceiver->iMsgAssembler->iStateValue ==
                TNATFWUNSAFMsgStateBase::EMsgInit );

    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( message );
    }

void UT_CNATFWUNSAFTlsReceiver::ReceiveTest4L()
    {
    // Receive UNSAF message and delete receiver from callback.
    //
    iDeleteReceiverInCallback = ETrue;

    CNATFWUNSAFMessage* message =
        NATFWUNSAFTransportTestUtils::CreateUNSAFBindingResponseLC();

    CBufBase* data = message->EncodeL();
    CleanupStack::PushL( data );

    _LIT8( KScrap, "foffoffof" );
    HBufC8* dataPart1 =
        HBufC8::NewLC( data->Ptr( 0 ).Length() + KScrap().Length() );

    TPtr8 ptrDataPart1( dataPart1->Des() );
    ptrDataPart1.Append( data->Ptr( 0 ) );
    ptrDataPart1.Append( KScrap );

    CompleteReceiveL( ptrDataPart1, KErrNone );
    CleanupStack::PopAndDestroy( dataPart1 );

    EUNIT_ASSERT( iLastReceivedMsg != 0 );
    EUNIT_ASSERT( iLastReceivedMsg->Validate() );

    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( message );
    }

void UT_CNATFWUNSAFTlsReceiver::ReceiveFailureTestL()
    {
    TInt error( KErrDisconnected );

    CNATFWUNSAFMessage* message =
        NATFWUNSAFTransportTestUtils::CreateUNSAFBindingResponseLC();

    CBufBase* data = message->EncodeL();
    CleanupStack::PushL( data );

    HBufC8* dataPart1 = data->Ptr( 0 ).AllocLC();
    CompleteReceiveL( dataPart1->Des(), error );
    CleanupStack::PopAndDestroy( dataPart1 );

    EUNIT_ASSERT( iLastError == error );
    EUNIT_ASSERT( iLastReceivedMsg == 0 );

    EUNIT_ASSERT( iReceiver->iMsgAssembler->iStateValue ==
                TNATFWUNSAFMsgStateBase::EMsgInit );

    iLastError = KErrNone;
    error = KErrEof;
    dataPart1 = data->Ptr( 0 ).AllocLC();
    CompleteReceiveL( dataPart1->Des(), error );
    CleanupStack::PopAndDestroy( dataPart1 );

    // KErrEof is not reported.
    EUNIT_ASSERT( iLastError == KErrNone );
    EUNIT_ASSERT( iLastReceivedMsg == 0 );

    CleanupStack::PopAndDestroy( data );
    CleanupStack::PopAndDestroy( message );
    }

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFTlsReceiver::CompleteReceiveL( TPtr8 aData, TInt aStatus )
    {
    // Complete receive request

    // Canceling CSecureSocket will distract cppunit leave failure simulation
    // because of trap inside CSecureSocket
    iReceiver->Cancel();

    iReceiver->iStatus = aStatus;

    delete iReceiver->iReceiveBuffer;
    iReceiver->iReceiveBuffer = 0;
    iReceiver->iReceiveBuffer = aData.AllocL();

    TRAPD( err, iReceiver->RunL() );
    if ( err && iReceiver )
        {
        // Simulate active scheduler behavior
        err = iReceiver->RunError( err );

        User::LeaveIfError( err );
        }
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFTlsReceiver,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ReceiveTestL - test ",
    "CNATFWUNSAFTlsReceiver",
    "ReceiveTestL",
    "FUNCTIONALITY",
    SetupL, ReceiveTestL, Teardown)

EUNIT_TEST(
    "ReceiveTest2L - test ",
    "CNATFWUNSAFTlsReceiver",
    "ReceiveTest2L",
    "FUNCTIONALITY",
    SetupL, ReceiveTest2L, Teardown)

EUNIT_TEST(
    "ReceiveTest3L - test ",
    "CNATFWUNSAFTlsReceiver",
    "ReceiveTest3L",
    "FUNCTIONALITY",
    SetupL, ReceiveTest3L, Teardown)

EUNIT_TEST(
    "ReceiveTest4L - test ",
    "CNATFWUNSAFTlsReceiver",
    "ReceiveTest4L",
    "FUNCTIONALITY",
    SetupL, ReceiveTest4L, Teardown)

EUNIT_TEST(
    "ReceiveFailureTestL - test ",
    "CNATFWUNSAFTlsReceiver",
    "ReceiveFailureTestL",
    "FUNCTIONALITY",
    SetupL, ReceiveFailureTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
