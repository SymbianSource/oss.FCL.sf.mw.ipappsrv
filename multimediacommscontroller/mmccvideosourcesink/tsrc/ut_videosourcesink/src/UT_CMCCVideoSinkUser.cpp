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



#include "UT_CMccVideoSinkUser.h"
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
#include "Mcctesteventhandler.h"
#include "mccvideosinkuser.h"

// CONSTANTS
const TInt KTestRealTimeModeJitBufThreshold = 50;
const TInt KTestNormalModeJitBufThreshold = 100;
const TInt KTestJitterBufferThresholdPacketTime = 20;

// MACROS


// CONSTRUCTION
UT_CMccVideoSinkUser* UT_CMccVideoSinkUser::NewL()
    {
    UT_CMccVideoSinkUser* self = UT_CMccVideoSinkUser::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccVideoSinkUser* UT_CMccVideoSinkUser::NewLC()
    {
    UT_CMccVideoSinkUser* self = new( ELeave ) UT_CMccVideoSinkUser();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccVideoSinkUser::~UT_CMccVideoSinkUser()
    {
    }

// Default constructor
UT_CMccVideoSinkUser::UT_CMccVideoSinkUser()
    {
    }

// Second phase construct
void UT_CMccVideoSinkUser::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CMccVideoSinkUser::SetupL(  )
    {
    iPacketSink = CXPSPacketSink::New();
    __ASSERT_ALWAYS( iPacketSink, User::Leave( KErrNoMemory ) );
    iUser = CMccVideoSinkUser::NewLC( NULL, KNullUid, 0, 15, *iPacketSink );
    CleanupStack::Pop( iUser );
    }

void UT_CMccVideoSinkUser::Teardown(  )
    {
    delete iUser;
    iUser = NULL;
    delete iPacketSink;
    iPacketSink = NULL;
    }

// TESTS

void UT_CMccVideoSinkUser::UT_CMccVideoSinkUser_EnqueueLL()
    {
    HBufC8* data = _L8("testdata").AllocLC();
    TRtpRecvHeader header;
    
    // Normal mode queuing
    EUNIT_ASSERT_EQUALS( iUser->EnqueueL( header, *data ), CMccVideoJitterBuffer::EPlaying );
    
    // Real time mode queueing
    //
    
    // Normal data
    MakeRealTimeUserL( *iUser );
    EUNIT_ASSERT_EQUALS( iUser->EnqueueL( header, *data ), CMccVideoJitterBuffer::EBuffering ); 
    EUNIT_ASSERT_EQUALS( iUser->iJitterBuf->iQueue.Count(), 1 );
    
    // Too big data (dropped silently)
    HBufC8* data2 = HBufC8::NewLC( 20000 );
    data2->Des().SetLength( 20000 );
    iUser->EnqueueL( header, *data2 ); 
    EUNIT_ASSERT_EQUALS( iUser->iJitterBuf->iQueue.Count(), 1 );
    EUNIT_ASSERT( iUser->iJitterBuf->iQueue[ 0 ]->DroppingAllowed() );
    
    // Avc config parameters
    // Avc user and buf contains config info
    data->Des()[ 0 ] = 0x7;
    iUser->EnqueueL( header, *data ); 
    EUNIT_ASSERT_EQUALS( iUser->iJitterBuf->iQueue.Count(), 2 );
    EUNIT_ASSERT( !iUser->iJitterBuf->iQueue[ 1 ]->DroppingAllowed() );
    
    CleanupStack::PopAndDestroy( data2 );
    CleanupStack::PopAndDestroy( data );
    }

void UT_CMccVideoSinkUser::UT_CMccVideoSinkUser_SetFrameRateLL()
    {
    // Set framerate in normal mode
    iUser->SetFrameRateL( 15 );
    EUNIT_ASSERT_EQUALS( iUser->iFrameRate, 15 );
    
    // Try setting same framerate (no effect)
    iUser->SetFrameRateL( 15 );
    EUNIT_ASSERT_EQUALS( iUser->iFrameRate, 15 );
    
    // Change framerate
    iUser->SetFrameRateL( 12 );
    EUNIT_ASSERT_EQUALS( iUser->iFrameRate, 12 );
    
    // Set framerate in realtime mode (jitterbuffer is configured)
    MakeRealTimeUserL( *iUser );
    iUser->SetFrameRateL( 14 );
    EUNIT_ASSERT_EQUALS( iUser->iFrameRate, 14 );
    EUNIT_ASSERT( iUser->iJitterBuf->PlayThresholdInMs() != 0 );
    }
    
void UT_CMccVideoSinkUser::UT_CMccVideoSinkUser_GetPrerollL()
    {
    // Preroll is low enough for real time mode
    iUser->iCodecInfo.iJitterBufThreshold = KTestRealTimeModeJitBufThreshold;
    EUNIT_ASSERT_EQUALS( iUser->GetPreroll(), 0 );
    
    // Preroll is too high for real time mode
    iUser->iCodecInfo.iJitterBufThreshold = KTestRealTimeModeJitBufThreshold + 2;
    EUNIT_ASSERT_EQUALS( iUser->GetPreroll(), 
        ( KTestRealTimeModeJitBufThreshold + 2 ) * KTestJitterBufferThresholdPacketTime );
    
    // Preroll is too low for multiple users
    iUser->SetMultipleMediaTypesL( ETrue );
    EUNIT_ASSERT( iUser->GetPreroll() >
        ( KTestRealTimeModeJitBufThreshold + 2 ) * KTestJitterBufferThresholdPacketTime );
    }

void UT_CMccVideoSinkUser::UT_CMccVideoSinkUser_GetActualPrerollL()
    {
    // In normal mode, value is defined by user
    iUser->iCodecInfo.iJitterBufThreshold = KTestRealTimeModeJitBufThreshold;
    EUNIT_ASSERT_EQUALS( iUser->GetActualPreroll(), 0 );
    
    // In real time mode, value is defined by jitterbuf
    MakeRealTimeUserL( *iUser );
    EUNIT_ASSERT( iUser->GetActualPreroll() > 0 );
    }

void UT_CMccVideoSinkUser::UT_CMccVideoSinkUser_PlayL()
    {
    iUser->Play();
    MakeRealTimeUserL( *iUser );
    iUser->Play();
    }

void UT_CMccVideoSinkUser::UT_CMccVideoSinkUser_PauseL()
    {
    iUser->Pause();
    MakeRealTimeUserL( *iUser );
    iUser->Pause();
    }
    
void UT_CMccVideoSinkUser::UT_CMccVideoSinkUser_IsResetNeededRealTimeModeL()
    {
    // First packet
    TUint16 seqNum( 10 );
    TRtpRecvHeader header;
    header.iSeqNum = seqNum;
    header.iTimestamp = 100;
    EUNIT_ASSERT( !iUser->IsResetNeededRealTimeMode( header ) );
    EUNIT_ASSERT_EQUALS( header.iSeqNum, seqNum );
    EUNIT_ASSERT_EQUALS( header.iTimestamp, 100 );
    }

// HELPERS

void UT_CMccVideoSinkUser::MakeRealTimeUserL( CMccVideoSinkUser& aUser )
    {
    TMccCodecInfo codec;
	codec.iFourCC.Set(KMccFourCCIdAVC);
	codec.iSdpName = KAVCSdpName;
	codec.iCodecMode = 0;
	codec.iBitrate = 50000;
	codec.iMaxBitrate = 72000;
	codec.iPayloadType = 97;
	codec.iFrameSize = 10000;
	codec.iJitterBufThreshold = KTestRealTimeModeJitBufThreshold;
    aUser.SetCodecInfoL( codec );
    aUser.SetMultipleMediaTypesL( EFalse );
    __ASSERT_ALWAYS( aUser.iJitterBuf, User::Leave( KErrNotReady ) );
    
    
    }
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccVideoSinkUser,
    "CMccVideoSinkUser",
    "CMccVideoSinkUser" )
		
EUNIT_TEST(
    "EnqueueL - test ",
    "CMccVideoSinkUser",
    "EnqueueL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSinkUser_EnqueueLL, Teardown)  

EUNIT_TEST(
    "SetFrameRateL - test ",
    "CMccVideoSinkUser",
    "SetFrameRateL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSinkUser_SetFrameRateLL, Teardown) 

EUNIT_TEST(
    "GetPreroll - test ",
    "CMccVideoSinkUser",
    "GetPreroll",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSinkUser_GetPrerollL, Teardown) 
    
EUNIT_TEST(
    "GetActualPreroll - test ",
    "CMccVideoSinkUser",
    "GetActualPreroll",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSinkUser_GetActualPrerollL, Teardown) 

EUNIT_TEST(
    "Play - test ",
    "CMccVideoSinkUser",
    "Play",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSinkUser_PlayL, Teardown) 
    
EUNIT_TEST(
    "Pause - test ",
    "CMccVideoSinkUser",
    "Pause",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSinkUser_PauseL, Teardown) 
    
EUNIT_TEST(
    "IsResetNeededRealTimeMode - test ",
    "CMccVideoSinkUser",
    "IsResetNeededRealTimeMode",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSinkUser_IsResetNeededRealTimeModeL, Teardown) 

            	    	    
EUNIT_END_TEST_TABLE

//  END OF FILE

