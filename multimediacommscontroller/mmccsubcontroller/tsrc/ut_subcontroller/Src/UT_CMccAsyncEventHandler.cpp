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
#include "UT_CMccAsyncEventHandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "MccJitterBuffer.h"

//  INTERNAL INCLUDES
#include "Mccasynceventhandler.h"
#include "AmrPayloadFormatUtil.h"
#include "MmccDtmfSignal.h"

// CONSTRUCTION
UT_CMccAsyncEventHandler* UT_CMccAsyncEventHandler::NewL()
    {
    UT_CMccAsyncEventHandler* self = UT_CMccAsyncEventHandler::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccAsyncEventHandler* UT_CMccAsyncEventHandler::NewLC()
    {
    UT_CMccAsyncEventHandler* self = new( ELeave ) UT_CMccAsyncEventHandler();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccAsyncEventHandler::~UT_CMccAsyncEventHandler()
    {
    }

// Default constructor
UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler() :
    iHandler( NULL )
    {
    }

// Second phase construct
void UT_CMccAsyncEventHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccAsyncEventHandler::SetupL(  )
    {
    iHandler = CMccAsyncEventHandler::NewL();
    }

void UT_CMccAsyncEventHandler::Teardown(  )
    {
    if ( iHandler )
        {
        delete iHandler;
        }
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_EventReceiveRequestLL(  )
    {
    // Can't test this, because the RMessage2 can only be properly created
    // by a server
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_CancelEventReceiveRequestLL(  )
    {
    // No request
    EUNIT_ASSERT_LEAVE( iHandler->CancelEventReceiveRequestL() );
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_SendEventToClientL(  )
    {
    TMccRtpSinkEvent sinkEvent;
    TMccRtpSourceEvent sourceEvent;
    TMccJitterBufferEvent jitterEvent;
    TMccAmrEvent amrEvent;
    TMMFEvent mmfEvent( TUid::Uid( 0 ), KErrNone );

    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sinkEvent ), KErrNone );
    
    // Cancel the active object
    iHandler->Cancel();

    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sourceEvent ), KErrNone );
    iHandler->Cancel();

    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( jitterEvent ), KErrNone );
    iHandler->Cancel();

    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( mmfEvent ), KErrNone );
    iHandler->Cancel();
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_SendMccEventToClientL(  )
    {
    TMccEvent event;
    EUNIT_ASSERT_EQUALS( iHandler->SendMccEventToClient( event ), KErrNone );
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_SendMediaSignallingToClientL(  )
    {
    TMccDTMFSignal dtmfSig;
    iHandler->SendMediaSignallingToClient( dtmfSig );
    
    TMccRtcpSignal rtcpSig;
    
    const TUint KMaxSignalsInQueue( 20 );
    
    for ( TInt i = 0; i < KMaxSignalsInQueue; i++ )
        {
        iHandler->SendMediaSignallingToClient( rtcpSig );
        }
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_ReconstructMediaSignalL(  )
    {
    iHandler->ReconstructMediaSignal();
    }
    
void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_HandleAmrEventL(  )
    {
    TMccAmrEvent amrEvent;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( amrEvent ), KErrNone );
    iHandler->Cancel();
    
    // Use a valid event type
    amrEvent.iEvent = EMccAmrEventCmr;

    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( amrEvent ), KErrNone );
    iHandler->Cancel();
    
    iHandler->HandleAmrEvent();
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_HandleRtpSinkEventL(  )
    {
    TMccRtpSinkEvent sinkEvent;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sinkEvent ), KErrNone );
    iHandler->Cancel();

    // Use a valid event type that causes no action
    sinkEvent.iEvent = ERtpSinkLastBuffer;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sinkEvent ), KErrNone );
    iHandler->Cancel();
    iHandler->HandleRtpSinkEvent();

    // Use a valid event type
    sinkEvent.iEvent = ERtpSinkError;
    sinkEvent.iErrorCode = KErrNone;

    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sinkEvent ), KErrNone );
    iHandler->Cancel();
    iHandler->HandleRtpSinkEvent();

    // Set an error code that will actually do something
    sinkEvent.iErrorCode = KErrNone;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sinkEvent ), KErrNone );
    iHandler->Cancel();
    iHandler->HandleRtpSinkEvent();
    iHandler->Cancel();
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_HandleRtpSourceEventL(  )
    {
    TMccRtpSourceEvent sourceEvent;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sourceEvent ), KErrNone );
    iHandler->Cancel();

    // Use a valid event type that causes no action
    sourceEvent.iEvent = EMccRtpSrcJitterUpdate;

    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sourceEvent ), KErrNone );
    iHandler->Cancel();
    iHandler->HandleRtpSourceEvent();
    
    // Set an error code that will actually do something
    sourceEvent.iEvent = EMccRtpSrcMmfEvent;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( sourceEvent ), KErrNone );
    iHandler->Cancel();
    iHandler->HandleRtpSourceEvent();
    iHandler->Cancel();
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_HandleJitterEventL(  )
    {
    TMccJitterBufferEvent jitterEvent;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( jitterEvent ), KErrNone );
    iHandler->Cancel();

    // These two events should cause no action
    jitterEvent.iEvent = EJitterEventMmfEvent;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( jitterEvent ), KErrNone );
    iHandler->Cancel();
    iHandler->HandleJitterEvent();

    jitterEvent.iEvent = EJitterEventStatusReport;
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( jitterEvent ), KErrNone );
    iHandler->Cancel();
    iHandler->HandleJitterEvent();
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_HandleMmfEventL(  )
    {
    TMMFEvent mmfEvent( TUid::Uid( 0 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iHandler->SendEventToClient( mmfEvent ), KErrNone );
    iHandler->Cancel();

    iHandler->HandleMmfEvent();
    iHandler->Cancel();
    }

void UT_CMccAsyncEventHandler::UT_CMccAsyncEventHandler_CompleteClientRequestL(  )
    {
    iHandler->CompleteClientRequest();
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccAsyncEventHandler,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "EventReceiveRequestL - test ",
    "CMccAsyncEventHandler",
    "EventReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_EventReceiveRequestLL, Teardown)

EUNIT_TEST(
    "CancelEventReceiveRequestL - test ",
    "CMccAsyncEventHandler",
    "CancelEventReceiveRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_CancelEventReceiveRequestLL, Teardown)

EUNIT_TEST(
    "SendEventToClient - test ",
    "CMccAsyncEventHandler",
    "SendEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_SendEventToClientL, Teardown)

EUNIT_TEST(
    "SendMccEventToClient - test ",
    "CMccAsyncEventHandler",
    "SendMccEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_SendMccEventToClientL, Teardown)

EUNIT_TEST(
    "SendMediaSignallingToClient - test ",
    "CMccAsyncEventHandler",
    "SendMediaSignallingToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_SendMediaSignallingToClientL, Teardown)

EUNIT_TEST(
    "ReconstructMediaSignal - test ",
    "CMccAsyncEventHandler",
    "ReconstructMediaSignal",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_ReconstructMediaSignalL, Teardown)

EUNIT_TEST(
    "HandleAmrEvent - test ",
    "CMccAsyncEventHandler",
    "HandleAmrEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_HandleAmrEventL, Teardown)

EUNIT_TEST(
    "HandleRtpSinkEvent - test ",
    "CMccAsyncEventHandler",
    "HandleRtpSinkEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_HandleRtpSinkEventL, Teardown)

EUNIT_TEST(
    "HandleRtpSourceEvent - test ",
    "CMccAsyncEventHandler",
    "HandleRtpSourceEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_HandleRtpSourceEventL, Teardown)

EUNIT_TEST(
    "HandleJitterEvent - test ",
    "CMccAsyncEventHandler",
    "HandleJitterEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_HandleJitterEventL, Teardown)

EUNIT_TEST(
    "HandleMmfEvent - test ",
    "CMccAsyncEventHandler",
    "HandleMmfEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_HandleMmfEventL, Teardown)

EUNIT_TEST(
    "CompleteClientRequest - test ",
    "CMccAsyncEventHandler",
    "CompleteClientRequest",
    "FUNCTIONALITY",
    SetupL, UT_CMccAsyncEventHandler_CompleteClientRequestL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
