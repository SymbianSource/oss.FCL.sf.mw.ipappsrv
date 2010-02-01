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
#include "UT_CAmrPayloadDecoder.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <mmf/server/mmfdatabuffer.h>





//  INTERNAL INCLUDES
#include "AmrPayloadDecoder.h"

// CONSTRUCTION
UT_CAmrPayloadDecoder* UT_CAmrPayloadDecoder::NewL()
    {
    UT_CAmrPayloadDecoder* self = UT_CAmrPayloadDecoder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CAmrPayloadDecoder* UT_CAmrPayloadDecoder::NewLC()
    {
    UT_CAmrPayloadDecoder* self = new( ELeave ) UT_CAmrPayloadDecoder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CAmrPayloadDecoder::~UT_CAmrPayloadDecoder()
    {
    }

// Default constructor
UT_CAmrPayloadDecoder::UT_CAmrPayloadDecoder()
    {
    }

// Second phase construct
void UT_CAmrPayloadDecoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CAmrPayloadDecoder::SetupL(  )
    {
	iDec = CAmrPayloadDecoder::NewL( ETrue );
    }

void UT_CAmrPayloadDecoder::SetupWbL(  )
    {
	iDec = CAmrPayloadDecoder::NewL( EFalse );
    }

void UT_CAmrPayloadDecoder::Teardown(  )
    {
    delete iDec;
    }

void UT_CAmrPayloadDecoder::UT_CAmrPayloadDecoder_DecodePayloadL(  )
    {
    CMMFDataBuffer* buf = CMMFDataBuffer::NewL( 50 );
    CleanupStack::PushL(buf);

    iDec->SetPayloadBuffer( buf->Data() );

    TUint32 ts(2);
    TUint32 tsInc( 5 );

	iDec->DecodePayload( ts, tsInc );
	CleanupStack::Pop(buf);
    delete buf;
    }

void UT_CAmrPayloadDecoder::UT_CAmrPayloadDecoder_ModeRequestL(  )
    {
    TAmrModeRequest foo;
    EUNIT_ASSERT_NO_LEAVE( foo = iDec->ModeRequest() );
    }

void UT_CAmrPayloadDecoder::UT_CAmrPayloadDecoder_FramesL(  )
    {
    TAmrFrame* foo;
    EUNIT_ASSERT_NO_LEAVE( foo = iDec->Frames(); );
    }

void UT_CAmrPayloadDecoder::UT_TStreamDecoder_CompareBuffersL()
    {
    HBufC8* buffer = HBufC8::NewLC(10);
    TAmrFrame frame;
    frame.iFrameType = EAmrFrameNoData;
    iDec->CompareBuffers( *buffer, frame, 10);
    frame.iFrameType = EAmrFrameSid;
    iDec->CompareBuffers( *buffer, frame, 10);
    buffer->Des().Copy(_L8("jjjjjj"));
    frame.iFrameType = EAmrFrameNoData;
    iDec->CompareBuffers( *buffer, frame, 2);
  
    CleanupStack::PopAndDestroy(buffer);
    
    }

void UT_CAmrPayloadDecoder::UT_TStreamDecoder_CalculateFramePositionL()
    {
/*    
    HBufC8* buf = HBufC8::NewLC(10);
    buf->Des().Copy(_L8("hhhhhhhhh"));
    iDec->CalculateFramePosition( buf->Ptr(),50 );
    CleanupStack::PopAndDestroy( buf );   
*/    
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CAmrPayloadDecoder,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "DecodePayload - test ",
    "CAmrPayloadDecoder",
    "DecodePayload",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadDecoder_DecodePayloadL, Teardown)

EUNIT_TEST(
    "ModeRequest - test ",
    "CAmrPayloadDecoder",
    "ModeRequest",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadDecoder_ModeRequestL, Teardown)

EUNIT_TEST(
    "Frames - test ",
    "CAmrPayloadDecoder",
    "Frames",
    "FUNCTIONALITY",
    SetupL, UT_CAmrPayloadDecoder_FramesL, Teardown)

EUNIT_TEST(
    "CompareBuffersL - test ",
    "CAmrPayloadDecoder",
    "CompareBuffersL",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_CompareBuffersL, Teardown)

EUNIT_TEST(
    "CalculateFramePositionL - test ",
    "CAmrPayloadDecoder",
    "CalculateFramePositionL",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_CalculateFramePositionL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
