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
#include "UT_CAmrCommonUtility.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "AmrCommonUtil.h"

// CONSTRUCTION
UT_CAmrCommonUtility* UT_CAmrCommonUtility::NewL()
    {
    UT_CAmrCommonUtility* self = UT_CAmrCommonUtility::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CAmrCommonUtility* UT_CAmrCommonUtility::NewLC()
    {
    UT_CAmrCommonUtility* self = new( ELeave ) UT_CAmrCommonUtility();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CAmrCommonUtility::~UT_CAmrCommonUtility()
    {
    }

// Default constructor
UT_CAmrCommonUtility::UT_CAmrCommonUtility()
    {
    }

// Second phase construct
void UT_CAmrCommonUtility::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CAmrCommonUtility::SetupL(  )
    {
    iUtil = new ( ELeave ) CAmrCommonUtility;
    }

void UT_CAmrCommonUtility::Teardown(  )
    {
    delete iUtil;
    }

void UT_CAmrCommonUtility::UT_CAmrCommonUtility_FrameInfoL(  )
    {
    TAmrFrameInfo info;
    TInt length( 0 );
    length = iUtil->FrameInfo( NULL, 0, info, ETrue );
    EUNIT_ASSERT_EQUALS( length, 0 );
    
    HBufC8* buf = HBufC8::NewLC( 100 );
    iUtil->FrameInfo( const_cast<TUint8*>( buf->Des().Ptr() ), 2, info, EFalse );
    CleanupStack::PopAndDestroy( buf );
    
    }

void UT_CAmrCommonUtility::UT_CAmrCommonUtility_SeekSyncL(  )
    {
    TInt pos( 0 );
    pos = iUtil->SeekSync( NULL, 0, ETrue );
    EUNIT_ASSERT_EQUALS( pos, 0 );
    }

void UT_CAmrCommonUtility::UT_CAmrCommonUtility_FrameSizeL(  )
    {
    // const TInt KRawAmrFrameLength[16] = { 13,14,16,18,20,21,27,32,6,0,0,0,0,0,0,1 };
    TAmrFrameInfo info;
    info.iMode = 0;
    EUNIT_ASSERT_EQUALS( iUtil->FrameSize( info, ETrue ), 13 );
    }

void UT_CAmrCommonUtility::UT_CAmrCommonUtility_FrameInfoWbL(  )
    {
    TAmrFrameInfo info;
    TInt length( 0 );
    length = iUtil->FrameInfo( NULL, 0, info, EFalse );
    EUNIT_ASSERT_EQUALS( length, 0 );
    
    TUint8 buf[100];
    buf[0] = 0x80;
    length = iUtil->FrameInfo( buf, 20, info, EFalse );
    EUNIT_ASSERT_EQUALS( length, 0 );
    buf[0] = 0x00;
    buf[17] = 0xFF;
    length = iUtil->FrameInfo( buf, 20, info, EFalse );
    EUNIT_ASSERT_EQUALS( length, 0 );
    buf[17] = 0x00;
    length = iUtil->FrameInfo( buf, 20, info, EFalse );
    EUNIT_ASSERT_EQUALS( length, 18 );
    EUNIT_ASSERT_EQUALS( info.iMode, 0 );
    EUNIT_ASSERT_EQUALS( info.iFrameSize, 18 );
    EUNIT_ASSERT_EQUALS( info.iBitrate, 18*400 );
    EUNIT_ASSERT_EQUALS( info.iSamplingRate, 16000 );
    EUNIT_ASSERT_EQUALS( info.iChannels, 1 );
    EUNIT_ASSERT_EQUALS( info.iFrameSamples, 320 );
    EUNIT_ASSERT_EQUALS( info.iChannelsOut, 1 );
    EUNIT_ASSERT_EQUALS( info.iFrameSamplesOut, 320 );
    EUNIT_ASSERT_EQUALS( info.iSamplingRateOut, 16000 );
    }

void UT_CAmrCommonUtility::UT_CAmrCommonUtility_SeekSyncWbL(  )
    {
    TInt pos( 0 );
    pos = iUtil->SeekSync( NULL, 0, EFalse );
    EUNIT_ASSERT_EQUALS( pos, 0 );
    }

void UT_CAmrCommonUtility::UT_CAmrCommonUtility_FrameSizeWbL(  )
    {
    TInt KRawAmrWbFrameLength[16] = { 18,24,33,37,41,47,51,59,61,6,0,0,0,0,0,1 };
    TInt i;
    TAmrFrameInfo info;
    for ( i = 0; i < 16; i++)
        {
        info.iMode = i;
        EUNIT_ASSERT_EQUALS( iUtil->FrameSize( info, EFalse ), KRawAmrWbFrameLength[i] );
        }
    }

void UT_CAmrCommonUtility::UT_CAmrCommonUtility_FrameHeaderSizeL(  )
    {
    EUNIT_ASSERT_EQUALS( iUtil->FrameHeaderSize(), 1 );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CAmrCommonUtility,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "FrameInfo - test ",
    "CAmrCommonUtility",
    "FrameInfo",
    "FUNCTIONALITY",
    SetupL, UT_CAmrCommonUtility_FrameInfoL, Teardown)

EUNIT_TEST(
    "SeekSync - test ",
    "CAmrCommonUtility",
    "SeekSync",
    "FUNCTIONALITY",
    SetupL, UT_CAmrCommonUtility_SeekSyncL, Teardown)

EUNIT_TEST(
    "FrameSize - test ",
    "CAmrCommonUtility",
    "FrameSize",
    "FUNCTIONALITY",
    SetupL, UT_CAmrCommonUtility_FrameSizeL, Teardown)

EUNIT_TEST(
    "FrameInfo - Wideband test ",
    "CAmrCommonUtility",
    "FrameInfo",
    "FUNCTIONALITY",
    SetupL, UT_CAmrCommonUtility_FrameInfoWbL, Teardown)

EUNIT_TEST(
    "SeekSync - Wideband test ",
    "CAmrCommonUtility",
    "SeekSync",
    "FUNCTIONALITY",
    SetupL, UT_CAmrCommonUtility_SeekSyncWbL, Teardown)

EUNIT_TEST(
    "FrameSize - Wideband test ",
    "CAmrCommonUtility",
    "FrameSize",
    "FUNCTIONALITY",
    SetupL, UT_CAmrCommonUtility_FrameSizeWbL, Teardown)

EUNIT_TEST(
    "FrameHeaderSize - test ",
    "CAmrCommonUtility",
    "FrameHeaderSize",
    "FUNCTIONALITY",
    SetupL, UT_CAmrCommonUtility_FrameHeaderSizeL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
