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
#include "UT_CMccRtpSender.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include "rtpheader.h"

// CONSTANTS

const TInt KMaxQueueSize = 30; // KMccMaxSendQueueSize
const TUint32 KTestRtpSendTimeoutLongMillisecs = 600; // KMccRtpSendTimeoutLongMillisecs

//  INTERNAL INCLUDES


UT_CMccRtpSender* UT_CMccRtpSender::NewL()
    {
    UT_CMccRtpSender* self = UT_CMccRtpSender::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccRtpSender* UT_CMccRtpSender::NewLC()
    {
    UT_CMccRtpSender* self = new( ELeave ) UT_CMccRtpSender();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }
    
UT_CMccRtpSender::UT_CMccRtpSender()
    {
    }

// Destructor (virtual by CBase)
UT_CMccRtpSender::~UT_CMccRtpSender()
    {
    }

// Second phase construct
void UT_CMccRtpSender::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CMccRtpSender::SetupL(  )
    {
    iLastError = KErrNone;
    iRtpApi = CRtpAPI::NewL( *this );
    iSender = CMccRtpSender::NewL( *this, *iRtpApi, 1, ETrue );
    } 

void UT_CMccRtpSender::Teardown(  )
    {
    delete iSender;
    iSender = NULL;
    delete iRtpApi;
    iRtpApi = NULL;
    }


void UT_CMccRtpSender::ErrorNotify( TInt /*aErrCode*/ )
    {
    }

void UT_CMccRtpSender::SendErrorOccured( TInt aError )
    {
    iLastError = aError;
    }
    
// TEST FUNCTIONs

void UT_CMccRtpSender::UT_CMccRtpSender_SendRtpPacketTest()
    {
    _LIT8( KMccRtpSenderTestData, "testadata" );
    TRtpId streamId( 2 );
    TRtpSendHeader sendHeader;
    TInt timestamp = 1000;
    sendHeader.iTimestamp = timestamp;
    sendHeader.iMarker = 1;
    TUint16 seqNum( 10 );
    
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    EUNIT_ASSERT( iSender->iSendQueue[ 0 ]->RtpStreamId() == streamId );
    EUNIT_ASSERT( iSender->iSendQueue[ 0 ]->RtpHeader().iTimestamp == timestamp );
    EUNIT_ASSERT( iSender->iSendQueue[ 0 ]->DataPtr() == KMccRtpSenderTestData );
    
    // Send is active, make queue full
    
    for ( TInt i = 1; i < KMaxQueueSize; i++ )
        {
        iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
        EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), i + 1 );
        }
    
    // As queue is full, packet is discarded
    TInt queueSize = iSender->iSendQueue.Count();
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), queueSize );
    
    // Sending completes successfully
    iSender->Cancel();
    iSender->RunL();
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), queueSize - 1 );
    EUNIT_ASSERT_EQUALS( iLastError, KErrNone );
     
    // Sending of next packet fails
    iSender->Cancel();
    iSender->iStatus = KErrGeneral;
    iSender->RunL();
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), queueSize - 2 );
    EUNIT_ASSERT_EQUALS( iLastError, KErrGeneral );
    }

void UT_CMccRtpSender::UT_CMccRtpSender_SendDataLL()
    {
    _LIT8( KMccRtpSenderTestData, "testadata" );
    TRtpId sessionId( 1 );
    TBool useRtpSocket( EFalse );
    iSender->SendDataL( sessionId, useRtpSocket, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    EUNIT_ASSERT( iSender->iSendQueue[ 0 ]->RtpStreamId() == KNullId );
    EUNIT_ASSERT( iSender->iSendQueue[ 0 ]->SessionId() == sessionId );
    EUNIT_ASSERT( iSender->iSendQueue[ 0 ]->DataPtr() == KMccRtpSenderTestData );
    
    // Send is active, make queue full
    for ( TInt i = 1; i < KMaxQueueSize; i++ )
        {
        iSender->SendDataL( sessionId, useRtpSocket, KMccRtpSenderTestData );
        EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), i + 1 );
        }
    
    // As queue is full, packet is discarded
    TInt queueSize = iSender->iSendQueue.Count();
    EUNIT_ASSERT_LEAVE( 
        iSender->SendDataL( sessionId, useRtpSocket, KMccRtpSenderTestData ) );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), queueSize );
    
    // Sending completes successfully
    iSender->Cancel();
    iSender->RunL();
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), queueSize - 1 );
    EUNIT_ASSERT_EQUALS( iLastError, KErrNone );
    
    // Sending of next packet fails
    iSender->Cancel();
    iSender->iStatus = KErrGeneral;
    iSender->RunL();
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), queueSize - 2 );
    EUNIT_ASSERT_EQUALS( iLastError, KErrGeneral );    
    }

void UT_CMccRtpSender::UT_CMccRtpSender_ClearTest()
    {
    _LIT8( KMccRtpSenderTestData, "testadata" );
    TRtpId streamId( 2 );
    TRtpSendHeader sendHeader;
    TInt timestamp = 1000;
    sendHeader.iTimestamp = timestamp;
    TUint16 seqNum( 10 );
    
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    
    iSender->Clear();
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 0 );
    }    

void UT_CMccRtpSender::UT_CMccRtpSender_SendingTimeoutTest()
    {
    _LIT8( KMccRtpSenderTestData, "testadata" );
    TRtpId streamId( 2 );
    TRtpSendHeader sendHeader;
    TInt timestamp = 1000;
    sendHeader.iTimestamp = timestamp;
    TUint16 seqNum( 10 );
    
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    
    iSender->TimerExpiredL( 0, NULL );
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 0 );
    }  

void UT_CMccRtpSender::UT_CMccRtpSender_MarkerBasedClearingTest()
    {
    // Set to short timeout mode
    iSender->iTimeoutTimeMilliseconds = KTestRtpSendTimeoutLongMillisecs;
    
    _LIT8( KMccRtpSenderTestData, "testadata" );
    TRtpId streamId( 2 );
    TRtpSendHeader sendHeader;
    TInt timestamp = 1000;
    sendHeader.iTimestamp = timestamp;
    TUint16 seqNum( 10 );
    sendHeader.iMarker = 0;
    
    // Timeout occurs when sending zero marker packet and there's already the full
    // frame in the queue
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    
    seqNum++;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 2 );
    
    seqNum++;
    sendHeader.iMarker = 1;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 3 );
    
    iSender->TimerExpiredL( 0, NULL );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 0 );
    
    // Timeout occurs when sending marker packet
    seqNum++;
    sendHeader.iMarker = 1;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    
    seqNum++;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 2 );
    
    iSender->TimerExpiredL( 0, NULL );
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    
    seqNum++;
    sendHeader.iMarker = 0;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 2 );
    
    iSender->TimerExpiredL( 0, NULL );
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    
    // Timeout occurs when sending zero marker packet and there's no full frame
    // in the queue
    seqNum++;
    sendHeader.iMarker = 0;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 2 );
    
    iSender->TimerExpiredL( 0, NULL );
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 0 );
    EUNIT_ASSERT( iSender->iDiscarding == ETrue );
    
    // Next packets are discarded because of that
    seqNum++;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 0 );
    EUNIT_ASSERT( iSender->iDiscarding == ETrue );
    
    seqNum++;
    sendHeader.iMarker = 1;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 0 );
    EUNIT_ASSERT( iSender->iDiscarding == EFalse );
    
    // Buffer is full and dropped frame is zero marker frame and there's some
    // packets of same frame in the queue
    for ( TInt i = 0; i < KMaxQueueSize; i++ )
        {
        seqNum++;
        sendHeader.iMarker = 1;
        iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
        EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), i + 1 );
        }
    
    iSender->iSendQueue[ KMaxQueueSize - 1 ]->iRtpHeader.iMarker = 0;
    iSender->iSendQueue[ KMaxQueueSize - 2 ]->iRtpHeader.iMarker = 0;
    
    seqNum++;
    sendHeader.iMarker = 0;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), KMaxQueueSize - 2 );
    EUNIT_ASSERT( iSender->iDiscarding == ETrue );
    iSender->iDiscarding = EFalse;
    
    // Buffer is full and dropped frame is zero marker frame but there's no other packets
    // of the same frame in the queue
    seqNum++;
    sendHeader.iMarker = 1;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    seqNum++;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    seqNum++;
    sendHeader.iMarker = 0;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), KMaxQueueSize );
    EUNIT_ASSERT( iSender->iDiscarding == ETrue );
    iSender->iDiscarding = EFalse;
    
    // Buffer is full and dropped frame is marker frame but there's no other packets
    // of the same frame in the queue 
    seqNum++;
    sendHeader.iMarker = 1;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), KMaxQueueSize );
    EUNIT_ASSERT( iSender->iDiscarding == EFalse );
    
    // Buffer is full and dropped frame is marker frame and there's some other packets
    // of the same frame in the queue 
    iSender->iSendQueue[ KMaxQueueSize - 1 ]->iRtpHeader.iMarker = 0;
    seqNum++;
    sendHeader.iMarker = 1;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), KMaxQueueSize - 1 );
    EUNIT_ASSERT( iSender->iDiscarding == EFalse );
    }

void UT_CMccRtpSender::UT_CMccRtpSender_MarkerBasedClearingTest2()
    {
    // Set to long timeout mode
    iSender->iTimeoutTimeMilliseconds = KTestRtpSendTimeoutLongMillisecs + 200;
    
    _LIT8( KMccRtpSenderTestData, "testadata" );
    TRtpId streamId( 2 );
    TRtpSendHeader sendHeader;
    TInt timestamp = 1000;
    sendHeader.iTimestamp = timestamp;
    TUint16 seqNum( 10 );
    sendHeader.iMarker = 0;
    
    // Timeout occurs when there's already several packets in queue
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 1 );
    
    seqNum++;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 2 );
    
    seqNum++;
    sendHeader.iMarker = 1;
    iSender->SendRtpPacketL( streamId, sendHeader, KMccRtpSenderTestData );
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 3 );
    
    iSender->TimerExpiredL( 0, NULL );
    
    EUNIT_ASSERT_EQUALS( iSender->iSendQueue.Count(), 0 );
    }
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccRtpSender,
    "CMccRtpSender test",
    "UNIT" )

EUNIT_TEST(
    "SendRtpPacketL - test ",
    "CMccRtpSender",
    "SendRtpPacketL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpSender_SendRtpPacketTest, Teardown )

EUNIT_TEST(
    "SendDataL - test ",
    "CMccRtpSender",
    "SendDataL",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpSender_SendDataLL, Teardown )
    
EUNIT_TEST(
    "Clear - test ",
    "CMccRtpSender",
    "Clear",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpSender_ClearTest, Teardown )
    
EUNIT_TEST(
    "Timeout - test ",
    "CMccRtpSender",
    "Timeout",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpSender_SendingTimeoutTest, Teardown )
    
EUNIT_TEST(
    "MarkerBasedClearing - test ",
    "CMccRtpSender",
    "MarkerBasedClearing",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpSender_MarkerBasedClearingTest, Teardown )

EUNIT_TEST(
    "MarkerBasedClearing 2 - test ",
    "CMccRtpSender",
    "MarkerBasedClearing 2",
    "FUNCTIONALITY",
    SetupL, UT_CMccRtpSender_MarkerBasedClearingTest2, Teardown )
            
EUNIT_END_TEST_TABLE

//  END OF FILE
