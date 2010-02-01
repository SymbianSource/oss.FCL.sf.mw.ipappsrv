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
#include "ut_cncmmediasource.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include "natfwunittestmacros.h"


//  INTERNAL INCLUDES
#include "cncmmediasource.h"


void UT_CNcmMediaSource::OutgoingMessageAvailable(
    const TDesC8& /*aMessage*/, MNcmSenderObserver* /*aSenderObserver*/,
    const TInetAddr& /*aDestAddr*/ )
    {
    
    }

void UT_CNcmMediaSource::WrapperSenderError(
    TUint /*aStreamId*/, TInt /*aError*/ )
    {
    
    }

void UT_CNcmMediaSource::GetFreePort(
    TUint /*aStreamId*/, TUint& /*aPort*/ )
    {
    
    }

void UT_CNcmMediaSource::FirstMediaPacketSent(
    TUint /*aStreamId*/ )
    {
    
    }

void UT_CNcmMediaSource::SetReceivingStateForMuxWrapper(
        TNATFWStreamingState /*aState*/ )
    {
    
    }

void UT_CNcmMediaSource::SendL(
    const TDesC8& /*aStreamPortion*/ )
    {
    
    }

TUint UT_CNcmMediaSource::StreamId()
    {
    return 1;
    }


void UT_CNcmMediaSource::RegisterMediaWrapperObserverL(
        MNATFWMediaWrapperObserver* /*aObserver*/,
        const RSocketServ& /*aServer*/,
        const TInetAddr& /*aLocalAddr*/ )
    {
    
    }

void UT_CNcmMediaSource::Close()
    {
    
    }
    
void UT_CNcmMediaSource::GetAddresses(
        TInetAddr& /*aLocalAddr*/,
        TInetAddr& /*aRemoteAddr*/ )
    {
    
    }
    
// CONSTRUCTION
UT_CNcmMediaSource* UT_CNcmMediaSource::NewL()
    {
    UT_CNcmMediaSource* self = UT_CNcmMediaSource::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNcmMediaSource* UT_CNcmMediaSource::NewLC()
    {
    UT_CNcmMediaSource* self = new( ELeave ) UT_CNcmMediaSource();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNcmMediaSource::~UT_CNcmMediaSource()
    {
    }

// Default constructor
UT_CNcmMediaSource::UT_CNcmMediaSource()
    {
    }

// Second phase construct
void UT_CNcmMediaSource::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNcmMediaSource::SetupL(  )
    {
    iCNcmMediaSource =
        CNcmMediaSource::NewL( *this, *this, iSocketServ, iAddress );
    }


void UT_CNcmMediaSource::Teardown(  )
    {
    delete iCNcmMediaSource; 
     iCNcmMediaSource = NULL; 
    }

    
void UT_CNcmMediaSource::T_CNcmMediaSource_RegisterMessageObserverL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->RegisterMessageObserver( *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->RegisterMessageObserver( *this ) );
    }
    
void UT_CNcmMediaSource::T_CNcmMediaSource_UnregisterMessageObserverL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->RegisterMessageObserver( *this ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->UnregisterMessageObserver( *this ) );
    // No observer exist
    EUNIT_ASSERT_PANIC( iCNcmMediaSource->UnregisterMessageObserver( *this ), "", -6 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->RegisterMessageObserver( *this ) );
    }
    
void UT_CNcmMediaSource::T_CNcmMediaSource_SetReceivingStateL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->SetReceivingState( EStreamingStateActive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->SetReceivingState( EStreamingStatePassive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->SetReceivingState( EStreamingStateActive ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->SetReceivingState( EStreamingStatePassive ) );
    }
    
void UT_CNcmMediaSource::T_CNcmMediaSource_PutMessageLL(  )
    {
    _LIT8( KUDPMessage,"UDP_Message" );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->PutMessageL( KUDPMessage ) );
    }
    
void UT_CNcmMediaSource::T_CNcmMediaSource_SendingCompletedL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->SendingCompleted( KErrNone ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->SendingCompleted( KErrArgument ) );
    }
    
void UT_CNcmMediaSource::T_CNcmMediaSource_ReceivedFromL(  )
    {
    _LIT8( KUDPMessage,"UDP_Message" );
    TBuf8<20> data( KUDPMessage );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->ReceivedFrom( 1, data ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->ReceivedFrom( 1, data ) );
    }
    
void UT_CNcmMediaSource::T_CNcmMediaSource_GetNewFreePortL(  )
    {
    TUint port;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->GetNewFreePort( port ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->GetNewFreePort( port ) );    
    }
    
void UT_CNcmMediaSource::T_CNcmMediaSource_MessageSentL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->MessageSent( ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->MessageSent( ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->MessageSent( ) );
    }
    
void UT_CNcmMediaSource::T_CNcmMediaSource_MessageSentFailureL(  )
    {
    // iCNcmMediaSource->MessageSentFailure( <add parameters here > );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->MessageSentFailure( KErrNone ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->MessageSentFailure( KErrArgument) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNcmMediaSource->MessageSentFailure( KErrOverflow) );
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNcmMediaSource,
    "Add test suite description here.",
    "UNIT" )

    
EUNIT_TEST(
    "RegisterMessageObserver - test1",
    "CNcmMediaSource",
    "RegisterMessageObserver - test1",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_RegisterMessageObserverL, Teardown)
    
EUNIT_TEST(
    "UnregisterMessageObserver - test2",
    "CNcmMediaSource",
    "UnregisterMessageObserver - test2",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_UnregisterMessageObserverL, Teardown)
    
EUNIT_TEST(
    "SetReceivingState - test3",
    "CNcmMediaSource",
    "SetReceivingState - test3",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_SetReceivingStateL, Teardown)   

EUNIT_TEST(
    "PutMessageL - test4",
    "CNcmMediaSource",
    "PutMessageL - test4",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_PutMessageLL, Teardown)
    
EUNIT_TEST(
    "SendingCompleted - test5",
    "CNcmMediaSource",
    "SendingCompleted - test5",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_SendingCompletedL, Teardown)
    
EUNIT_TEST(
    "ReceivedFrom - test6",
    "CNcmMediaSource",
    "ReceivedFrom - test6",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_ReceivedFromL, Teardown)
    
EUNIT_TEST(
    "GetNewFreePort - test7",
    "CNcmMediaSource",
    "GetNewFreePort - test7",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_GetNewFreePortL, Teardown)
    
EUNIT_TEST(
    "MessageSent - test8",
    "CNcmMediaSource",
    "MessageSent - test8",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_MessageSentL, Teardown)
    
EUNIT_TEST(
    "MessageSentFailure - test9",
    "CNcmMediaSource",
    "MessageSentFailure - test9",
    "FUNCTIONALITY",
    SetupL, T_CNcmMediaSource_MessageSentFailureL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
