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
#include "UT_CAmrPayloadEncoder.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "AmrPayloadEncoder.h"

// CONSTRUCTION
UT_CAmrPayloadEncoder* UT_CAmrPayloadEncoder::NewL()
    {
    UT_CAmrPayloadEncoder* self = UT_CAmrPayloadEncoder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CAmrPayloadEncoder* UT_CAmrPayloadEncoder::NewLC()
    {
    UT_CAmrPayloadEncoder* self = new( ELeave ) UT_CAmrPayloadEncoder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CAmrPayloadEncoder::~UT_CAmrPayloadEncoder()
    {
    }

// Default constructor
UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder()
    {
    }

// Second phase construct
void UT_CAmrPayloadEncoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CAmrPayloadEncoder::SetupL(  )
    {
	iEnc = CAmrPayloadEncoder::NewL( ETrue );
    }

void UT_CAmrPayloadEncoder::SetupWbL(  )
    {
	iEnc = CAmrPayloadEncoder::NewL( EFalse );
    }

void UT_CAmrPayloadEncoder::Teardown(  )
    {
	delete iEnc;
    }

void UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder_SetModeRequestL(  )
    {
    iEnc->SetModeRequest( EAmrModeReq0 );
    EUNIT_ASSERT_EQUALS( iEnc->iModeRequest, EAmrModeReq0 );
    iEnc->SetModeRequest( EAmrModeReq1 );
    EUNIT_ASSERT_EQUALS( iEnc->iModeRequest, EAmrModeReq1 );
    }

void UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder_EncodeFrameL(  )
    {
    //EUNIT_ASSERT_NO_LEAVE( iEnc->EncodeFrame( );
    }

void UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder_ResetPayloadBufferL(  )
    {
    iEnc->ResetPayloadBuffer();
    iEnc->iRedCount = 4;
    HBufC8* buf = HBufC8::NewLC(10);
    buf->Des().Copy(_L8("heeaaaaa"));
    iEnc->SaveRedundantFrame(buf->Des());
    iEnc->SaveRedundantFrame(buf->Des());
    CleanupStack::PopAndDestroy(buf);
    iEnc->iFrames[iEnc->iRedCount].iFrameData = (TUint8 *)(-1);
    iEnc->ResetPayloadBuffer();
    EUNIT_ASSERT_EQUALS( iEnc->iFrames[iEnc->iRedCount].iFrameData, (TUint8 *)NULL );
    }

void UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder_ReEncodeFrameLL(  )
    {
    //EUNIT_ASSERT( EFalse );
    }

void UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder_ReEncodeFrameL_1L(  )
    {
    //EUNIT_ASSERT( EFalse );
    }

void UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder_SetChannelCountL(  )
    {
    EUNIT_ASSERT_EQUALS( iEnc->SetChannelCount( 1 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iEnc->SetChannelCount( 0 ), KErrArgument );
    EUNIT_ASSERT_EQUALS( iEnc->SetChannelCount( 99 ), KErrArgument );
    }

void UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder_SetFrameBlockCountL(  )
    {
    EUNIT_ASSERT_EQUALS( iEnc->SetFrameBlockCount( 1 ), KErrNone );
    EUNIT_ASSERT_EQUALS( iEnc->SetFrameBlockCount( 0 ), KErrArgument );
    EUNIT_ASSERT_EQUALS( iEnc->SetFrameBlockCount( 99 ), KErrArgument );
    }

void UT_CAmrPayloadEncoder::UT_CAmrPayloadEncoder_SetRedFrameBlockCountL(  )
    {
    TInt count( KMaxAmrRedCount + 1 );
    EUNIT_ASSERT_EQUALS( iEnc->SetRedFrameBlockCount( count ), KErrArgument );
    
    count = 1;
    EUNIT_ASSERT_EQUALS( iEnc->SetRedFrameBlockCount( count ), KErrNone );
    EUNIT_ASSERT_EQUALS( iEnc->iRedCount, count );
    count = 0;
    EUNIT_ASSERT_EQUALS( iEnc->SetRedFrameBlockCount( count ), KErrNone );
    EUNIT_ASSERT_EQUALS( iEnc->iRedCount, count );
    count = -1;
    EUNIT_ASSERT_EQUALS( iEnc->SetRedFrameBlockCount( count ), KErrArgument );
    EUNIT_ASSERT_EQUALS( iEnc->iRedCount, 0 );
    }

void UT_CAmrPayloadEncoder::UT_TStreamEncoder_SaveRedundantFrameL(  )
    {
    HBufC8* buf = HBufC8::NewLC(10);
    iEnc->SetRedFrameBlockCount( 0 );
    iEnc->SaveRedundantFrame(buf->Des());
    iEnc->SaveRedundantFrame(buf->Des());
    EUNIT_ASSERT_EQUALS( iEnc->iFrameDatas.Count(), 0 );
    iEnc->SetRedFrameBlockCount( 1 );
    iEnc->SaveRedundantFrame(buf->Des());
    iEnc->SaveRedundantFrame(buf->Des());
    CleanupStack::PopAndDestroy(buf);
    }
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CAmrPayloadEncoder,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SetModeRequest - test ",
    "CAmrPayloadEncoder",
    "SetModeRequest",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadEncoder_SetModeRequestL, Teardown)

EUNIT_TEST(
    "EncodeFrame - test ",
    "CAmrPayloadEncoder",
    "EncodeFrame",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadEncoder_EncodeFrameL, Teardown)

EUNIT_TEST(
    "ResetPayloadBuffer - test ",
    "CAmrPayloadEncoder",
    "ResetPayloadBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadEncoder_ResetPayloadBufferL, Teardown)

EUNIT_TEST(
    "ReEncodeFrameL - test ",
    "CAmrPayloadEncoder",
    "ReEncodeFrameL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadEncoder_ReEncodeFrameLL, Teardown)

EUNIT_TEST(
    "ReEncodeFrameL - test ",
    "CAmrPayloadEncoder",
    "ReEncodeFrameL",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadEncoder_ReEncodeFrameL_1L, Teardown)

EUNIT_TEST(
    "SetChannelCount - test ",
    "CAmrPayloadEncoder",
    "SetChannelCount",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadEncoder_SetChannelCountL, Teardown)

EUNIT_TEST(
    "SetFrameBlockCount - test ",
    "CAmrPayloadEncoder",
    "SetFrameBlockCount",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadEncoder_SetFrameBlockCountL, Teardown)

EUNIT_TEST(
    "SetRedFrameBlockCount - test ",
    "CAmrPayloadEncoder",
    "SetRedFrameBlockCount",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadEncoder_SetRedFrameBlockCountL, Teardown)

EUNIT_TEST(
    "SaveRedundantFrameL - test ",
    "CAmrPayloadEncoder",
    "SaveRedundantFrameL",
    "FUNCTIONALITY",
    SetupL, UT_TStreamEncoder_SaveRedundantFrameL, Teardown)

EUNIT_TEST(
    "EncodeFrame WideBand - test ",
    "CAmrPayloadEncoder",
    "EncodeFrame",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadEncoder_EncodeFrameL, Teardown)

EUNIT_TEST(
    "ReEncodeFrameL WideBand - test ",
    "CAmrPayloadEncoder",
    "ReEncodeFrameL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadEncoder_ReEncodeFrameLL, Teardown)

EUNIT_TEST(
    "ReEncodeFrameL WideBand - test ",
    "CAmrPayloadEncoder",
    "ReEncodeFrameL",
    "FUNCTIONALITY",
    SetupWbL, UT_CAmrPayloadEncoder_ReEncodeFrameL_1L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
