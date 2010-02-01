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
#include <mmf/common/mmfbase.h>



#include "UT_CMccVideoJitterBuffer.h"
#include "MMccInterfaceDef.h"
#include "MccInternalCodecs.h"
#include "cxpspacketsink.h"
#include "rtpheader.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // end of EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES

const TInt KVideoSinkTestFramerateNormal = 15;
const TInt KVideoSinkTestFramerateLow = 12;

const TInt KVideoSinkTestOkInactivityTimeout = 1000;
const TInt KVideoSinkTestTooSmallInactivityTimeout = 200;

// MACROS
#define CLEAR_JITTER_BUFFER \
iJitBuf->iQueue.ResetAndDestroy(); \
iJitBuf->iFrameCount = 0;

// CONSTRUCTION
UT_CMccVideoJitterBuffer* UT_CMccVideoJitterBuffer::NewL()
    {
    UT_CMccVideoJitterBuffer* self = UT_CMccVideoJitterBuffer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccVideoJitterBuffer* UT_CMccVideoJitterBuffer::NewLC()
    {
    UT_CMccVideoJitterBuffer* self = new( ELeave ) UT_CMccVideoJitterBuffer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccVideoJitterBuffer::~UT_CMccVideoJitterBuffer()
    {
    }

// From MMccVideoJitterBufferObserver
void UT_CMccVideoJitterBuffer::ErrorOccured( TInt /*aError*/ )
    {
    }
    
// Default constructor
UT_CMccVideoJitterBuffer::UT_CMccVideoJitterBuffer()
    {
    }

// Second phase construct
void UT_CMccVideoJitterBuffer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccVideoJitterBuffer::SetupL(  )
    {
    iPacketSink = CXPSPacketSink::New();
    __ASSERT_ALWAYS( iPacketSink, User::Leave( KErrNoMemory ) );
    iJitBuf = CMccVideoJitterBuffer::NewL( *this, *iPacketSink, KVideoSinkTestOkInactivityTimeout );
     
    }

void UT_CMccVideoJitterBuffer::Setup2L(  )
    {
    iPacketSink = CXPSPacketSink::New();
    __ASSERT_ALWAYS( iPacketSink, User::Leave( KErrNoMemory ) );
    iJitBuf = CMccVideoJitterBuffer::NewL( *this, *iPacketSink, KVideoSinkTestTooSmallInactivityTimeout );
     
    }
    
void UT_CMccVideoJitterBuffer::Teardown(  )
    {
    delete iJitBuf;
    iJitBuf = NULL;
    delete iPacketSink;
    iPacketSink = NULL;
    }


// TESTS

void UT_CMccVideoJitterBuffer::UT_CMccVideoJitterBuffer_EnqueueLL()
    {
    HBufC8* data = _L8("testdata").AllocLC();
    TRtpRecvHeader header;
    header.iMarker = 1;
    header.iTimestamp = 100;
    
    // Pass data until play threshold has been reached
    CMccVideoJitterBuffer::TMccPacketBufferingStatus bufferingStatus( CMccVideoJitterBuffer::EBuffering );
    for ( TInt i = 0; i < 30 && ( bufferingStatus == CMccVideoJitterBuffer::EBuffering ); i++ )
        {
        bufferingStatus = iJitBuf->EnqueueL( 0, header, *data, EFalse );
        if ( bufferingStatus == CMccVideoJitterBuffer::EBuffering )
            {
            EUNIT_ASSERT( !iJitBuf->IsActive() );
            }
        header.iTimestamp += 100;
        }
    EUNIT_ASSERT( bufferingStatus == CMccVideoJitterBuffer::EPlayingStarted );
    EUNIT_ASSERT( iJitBuf->IsActive() );
    EUNIT_ASSERT( iJitBuf->iFrameCount > 0 );
    
    // Try exceeding max size, buffer should make space by pushing frames
    // towards helix, as buffer starts to get too full, posting
    // interval is increased
    TTimeIntervalMicroSeconds32 origInterval = iJitBuf->iCurrentInterval;
    EUNIT_ASSERT( origInterval.Int() == iJitBuf->iNormalInterval.Int() );
    TBool increasedInterval( EFalse );
    for ( TInt i = 0; i < 30 && !increasedInterval; i++ )
        {
        iJitBuf->EnqueueL( 0, header, *data, EFalse );
        
        if ( origInterval.Int() > iJitBuf->iCurrentInterval.Int() )
            {
            increasedInterval = ETrue;
            }
        header.iTimestamp += 100;
        }
    EUNIT_ASSERT( increasedInterval );
    EUNIT_ASSERT( !iJitBuf->MaxSizeExceeded() );
    
    // Running out of data, posting interval is increased
    iJitBuf->iQueue.ResetAndDestroy();
    CLEAR_JITTER_BUFFER
    iJitBuf->EnqueueL( 0, header, *data, EFalse );
    header.iTimestamp += 100;
    EUNIT_ASSERT( iJitBuf->iCurrentInterval.Int() == iJitBuf->iLowLimitInterval.Int() );
    
    // Gaining data, once there's enough data, posting interval is normalized
   TBool normalized( EFalse );
   for ( TInt i = 0; i < 30 && !normalized; i++ )
        {
        iJitBuf->EnqueueL( 0, header, *data, EFalse );
        if ( iJitBuf->iNormalInterval.Int() == iJitBuf->iCurrentInterval.Int())
            {
            normalized = ETrue;
            }
        header.iTimestamp += 100;
        }
    EUNIT_ASSERT( normalized );
        
    // Timer completion when there's data in buf
    TInt frameCount = iJitBuf->iFrameCount;
    iJitBuf->Cancel();
    iJitBuf->iStatus = KErrNone;
    iJitBuf->RunL();
    EUNIT_ASSERT_EQUALS( iJitBuf->iFrameCount, frameCount - 1 );
    EUNIT_ASSERT( iJitBuf->IsActive() );
    
    // Timer completed once no more data to post
    CLEAR_JITTER_BUFFER
    iJitBuf->Cancel();
    iJitBuf->iStatus = KErrNone;
    iJitBuf->RunL();
    EUNIT_ASSERT( !iJitBuf->IsActive() );
    
    
    // Enqueu packets in "incorrect" order (queueing is done seq num based)
    header.iSeqNum = 100;
    iJitBuf->EnqueueL( 0, header, *data, EFalse );
    header.iTimestamp += 100;
    
    header.iSeqNum = 98;
    iJitBuf->EnqueueL( 0, header, *data, EFalse );
    header.iTimestamp += 100;
    
    EUNIT_ASSERT_EQUALS( iJitBuf->iQueue.Count(), 2 );
    EUNIT_ASSERT_EQUALS( iJitBuf->iQueue[ 0 ]->HeaderInfo().iSeqNum, 98 );
    EUNIT_ASSERT_EQUALS( iJitBuf->iQueue[ 1 ]->HeaderInfo().iSeqNum, 100 );
    
    // Frame is divided into several packets and packet with marker is lost somewhere 
    // (other  packets for that frame are dropped silently as they are useless without marker packet)
    CLEAR_JITTER_BUFFER
    header.iMarker = 0;
    iJitBuf->EnqueueL( 0, header, *data, EFalse );
    iJitBuf->EnqueueL( 0, header, *data, EFalse );
    
    header.iTimestamp += 100;
    header.iMarker = 1;
    iJitBuf->EnqueueL( 0, header, *data, EFalse );
    EUNIT_ASSERT_EQUALS( iJitBuf->iQueue.Count(), 3 );
    
    iJitBuf->Cancel();
    iJitBuf->iStatus = KErrNone;
    iJitBuf->RunL();
    EUNIT_ASSERT_EQUALS( iJitBuf->iQueue.Count(), 0 );
    
    CleanupStack::PopAndDestroy( data );
    }

void UT_CMccVideoJitterBuffer::UT_CMccVideoJitterBuffer_ConfigureLL()
    {
    // Zero framerate
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJitBuf->ConfigureL( 100, 200, 150, 300, 0 ), KErrArgument );
    
    // Wrong low limit
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJitBuf->ConfigureL( 500, 200, 150, 300, 15 ), KErrArgument );
    
    // Wrong threshold
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJitBuf->ConfigureL( 100, 200, 250, 300, 15 ), KErrArgument );
    
    // Wrong highlimit
    EUNIT_ASSERT_SPECIFIC_LEAVE( iJitBuf->ConfigureL( 100, 350, 250, 300, 15 ), KErrArgument );
    
    // Ok
    iJitBuf->ConfigureL( 100, 200, 150, 300, 15 );
    EUNIT_ASSERT_EQUALS( iJitBuf->PlayThresholdInMs(), 150 );
    }
    
void UT_CMccVideoJitterBuffer::UT_CMccVideoJitterBuffer_PausePlayL()
    {
    // Threshold not reached, not starting
    iJitBuf->Play();
    EUNIT_ASSERT( !iJitBuf->IsActive() );
    
    // Threshold reached, play started
    iJitBuf->iPlayThresholdExeeced = ETrue;
    iJitBuf->Play();
    EUNIT_ASSERT( iJitBuf->IsActive() );
    
    // Pausing
    iJitBuf->Pause();
    EUNIT_ASSERT( !iJitBuf->IsActive() );
    }

void UT_CMccVideoJitterBuffer::UT_CMccVideoJitterBuffer_InactivityTimeoutL()
    {
    // Too small inactivity timeout should have been modified to more suitable value
    EUNIT_ASSERT( iJitBuf->iInactivityTimeoutInMicroSecs != ( KVideoSinkTestTooSmallInactivityTimeout * 1000 ) );
    
    HBufC8* data = _L8("testdata").AllocLC();
    TRtpRecvHeader header;
    header.iMarker = 1;
    
    // Enqueu until play threshold reached
    CMccVideoJitterBuffer::TMccPacketBufferingStatus bufferingStatus( CMccVideoJitterBuffer::EBuffering );
    for ( TInt i = 0; i < 30 && ( bufferingStatus == CMccVideoJitterBuffer::EBuffering ); i++ )
        {
        bufferingStatus = iJitBuf->EnqueueL( 0, header, *data, EFalse );
        if ( bufferingStatus == CMccVideoJitterBuffer::EBuffering )
            {
            EUNIT_ASSERT( !iJitBuf->IsActive() );
            }
        header.iTimestamp += 100;
        }
    
    // Threshold exceeding is reported with playing started status
    EUNIT_ASSERT( bufferingStatus == CMccVideoJitterBuffer::EPlayingStarted );
    
    // All packets after that are reported with playing status
    TInt64 packetReceivingTime( 100 );
    TTime temporaryTime;
    temporaryTime.DayNoInYear( TTime( packetReceivingTime ) ); // Use timestub

    bufferingStatus = iJitBuf->EnqueueL( 0, header, *data, EFalse );
    EUNIT_ASSERT( bufferingStatus == CMccVideoJitterBuffer::EPlaying );
    
    iJitBuf->Cancel();
    iJitBuf->iStatus = KErrNone;
    iJitBuf->RunL();
    
    // Until inactivity timeout occurs which causes that playback started status is again given
    packetReceivingTime += ( iJitBuf->iInactivityTimeoutInMicroSecs + 100 );
    temporaryTime.DayNoInYear( packetReceivingTime ); // Use timestub
    
    iJitBuf->Cancel();
    iJitBuf->iStatus = KErrNone;
    iJitBuf->RunL();
    
    bufferingStatus = iJitBuf->EnqueueL( 0, header, *data, EFalse );
    EUNIT_ASSERT( bufferingStatus == CMccVideoJitterBuffer::EPlayingStarted );
    
    // Then again next ones will get playing status
    bufferingStatus = iJitBuf->EnqueueL( 0, header, *data, EFalse );
    EUNIT_ASSERT( bufferingStatus == CMccVideoJitterBuffer::EPlaying );
    
    CleanupStack::PopAndDestroy( data );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccVideoJitterBuffer,
    "CMccVideoJitterBuffer",
    "CMccVideoJitterBuffer" )   
    
EUNIT_TEST(
    "EnqueueL - test ",
    "CMccVideoJitterBuffer",
    "EnqueueL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoJitterBuffer_EnqueueLL, Teardown) 

EUNIT_TEST(
    "EnqueueL - test ",
    "CMccVideoJitterBuffer",
    "EnqueueL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoJitterBuffer_ConfigureLL, Teardown) 
    
EUNIT_TEST(
    "Pause and Play - test ",
    "CMccVideoJitterBuffer",
    "Pause and Play",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoJitterBuffer_PausePlayL, Teardown) 
    
EUNIT_TEST(
    "Inactivity timeout - test ",
    "CMccVideoJitterBuffer",
    "Inactivity timeout",
    "FUNCTIONALITY",
    Setup2L, UT_CMccVideoJitterBuffer_InactivityTimeoutL, Teardown) 
        	    
EUNIT_END_TEST_TABLE

//  END OF FILE

