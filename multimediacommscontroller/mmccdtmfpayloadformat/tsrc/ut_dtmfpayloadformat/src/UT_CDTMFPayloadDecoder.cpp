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
#include "UT_CDTMFPayloadDecoder.h"

//  EXTERNAL INCLUDES
#include <mmf/server/mmfdatabuffer.h>



#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "dtmfpayloaddecoder.h"

// CONSTRUCTION
UT_CDTMFPayloadDecoder* UT_CDTMFPayloadDecoder::NewL()
    {
    UT_CDTMFPayloadDecoder* self = UT_CDTMFPayloadDecoder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CDTMFPayloadDecoder* UT_CDTMFPayloadDecoder::NewLC()
    {
    UT_CDTMFPayloadDecoder* self = new( ELeave ) UT_CDTMFPayloadDecoder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CDTMFPayloadDecoder::~UT_CDTMFPayloadDecoder()
    {
    
    }

// Default constructor
UT_CDTMFPayloadDecoder::UT_CDTMFPayloadDecoder()
    {
    }

// Second phase construct
void UT_CDTMFPayloadDecoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CDTMFPayloadDecoder::SetupL()
    {
    iDecoder = CDTMFPayloadDecoder::NewL();
    }

void UT_CDTMFPayloadDecoder::Teardown()
    {
    delete iDecoder;
    }

void UT_CDTMFPayloadDecoder::UT_CDTMFPayloadDecoder_ConvertToChar()
    {
    TChar chara;
    EUNIT_ASSERT( iDecoder->ConvertToChar( 16, chara ) == KErrNotSupported );
        
    EUNIT_ASSERT( iDecoder->ConvertToChar( 8, chara ) == KErrNone );
    EUNIT_ASSERT( chara == '8');
    
    EUNIT_ASSERT( iDecoder->ConvertToChar( 9, chara ) == KErrNone );
    EUNIT_ASSERT( chara == '9');

    EUNIT_ASSERT( iDecoder->ConvertToChar( KEventCodeForAsterisk, chara ) == KErrNone );
    EUNIT_ASSERT( chara == '*');
    
    EUNIT_ASSERT( iDecoder->ConvertToChar( KEventCodeForHashMark, chara ) == KErrNone );
    EUNIT_ASSERT( chara == '#');
    
    EUNIT_ASSERT( iDecoder->ConvertToChar( KEventCodeForA, chara ) == KErrNone );
    EUNIT_ASSERT( chara == 'a');
    
    EUNIT_ASSERT( iDecoder->ConvertToChar( KEventCodeForB, chara ) == KErrNone );
    EUNIT_ASSERT( chara == 'b');
    
    EUNIT_ASSERT( iDecoder->ConvertToChar( KEventCodeForC, chara ) == KErrNone );
    EUNIT_ASSERT( chara == 'c');
      
    EUNIT_ASSERT( iDecoder->ConvertToChar( KEventCodeForD, chara ) == KErrNone );
    EUNIT_ASSERT( chara == 'd');
    }

void UT_CDTMFPayloadDecoder::UT_CDTMFPayloadDecoder_DecodeEventPayloadL(  )
    {
    RArray<TDTMFEventPayloadInfo> eventinfors; 
    CleanupClosePushL( eventinfors );

    EUNIT_ASSERT( iDecoder->DecodeEventPayload( NULL, eventinfors ) == KErrArgument );

    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    CleanupStack::PushL( buffer );
    buffer->Data().FillZ( KDTMFDefaultPayloadSize );
    
    EUNIT_ASSERT( iDecoder->DecodeEventPayload( buffer, eventinfors ) == KErrNone );
    
    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy(); // eventinfors
    }

void UT_CDTMFPayloadDecoder::UT_CDTMFPayloadDecoder_DecodeTonePayloadL(  )
    {
    RArray<TDTMFTonePayloadInfo> eventinfors; 
    CleanupClosePushL( eventinfors );
    CMMFDataBuffer* buffer = NULL;
    EUNIT_ASSERT( iDecoder->DecodeTonePayload( NULL, eventinfors ) == KErrArgument );

    buffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    CleanupStack::PushL( buffer );
    buffer->Data().FillZ( KDTMFDefaultPayloadSize );
    
    EUNIT_ASSERT( iDecoder->DecodeTonePayload( buffer, eventinfors ) == KErrNone );
    
    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy(); // eventinfors
    }

void UT_CDTMFPayloadDecoder::UT_CDTMFPayloadDecoder_PayloadFormatL(  )
    {
    TDTMFPayloadFormat payloadformat( EDTMFPayloadFormatNotDefined );
    EUNIT_ASSERT( iDecoder->PayloadFormat( ) == EDTMFPayloadFormatNotDefined );

    EUNIT_ASSERT( iDecoder->SetPayloadFormat( payloadformat ) == KErrNotSupported );
    
    payloadformat = EDTMFPayloadFormatEvent;
    EUNIT_ASSERT( iDecoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iDecoder->PayloadFormat(  ) == EDTMFPayloadFormatEvent );

    payloadformat = EDTMFPayloadFormatTone;
    EUNIT_ASSERT( iDecoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iDecoder->PayloadFormat(  ) == EDTMFPayloadFormatTone );

    payloadformat = EDTMFPayloadFormatRedEvents;
    EUNIT_ASSERT( iDecoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iDecoder->PayloadFormat(  ) == EDTMFPayloadFormatRedEvents );
    }

void UT_CDTMFPayloadDecoder::UT_CDTMFPayloadDecoder_SetPayloadFormatL(  )
    {
    TDTMFPayloadFormat payloadformat( EDTMFPayloadFormatNotDefined );
    
    EUNIT_ASSERT( iDecoder->SetPayloadFormat( payloadformat ) == KErrNotSupported );
    
    payloadformat = EDTMFPayloadFormatEvent;
    EUNIT_ASSERT( iDecoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iDecoder->PayloadFormat(  ) == EDTMFPayloadFormatEvent );

    payloadformat = EDTMFPayloadFormatTone;
    EUNIT_ASSERT( iDecoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iDecoder->PayloadFormat(  ) == EDTMFPayloadFormatTone );

    payloadformat = EDTMFPayloadFormatRedEvents;
    EUNIT_ASSERT( iDecoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iDecoder->PayloadFormat(  ) == EDTMFPayloadFormatRedEvents );
    }

void UT_CDTMFPayloadDecoder::UT_CDTMFPayloadDecoder_SetCurTimeStampL(  )
    {
    TTimeIntervalMicroSeconds32 timestamp( 5000 );
    iDecoder->SetCurTimeStamp( timestamp );
    }

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CDTMFPayloadDecoder,
    "MCC DTMF payload decoder",
    "UNIT" )

EUNIT_TEST(
    "ConvertToChar - test ",
    "CDTMFPayloadDecoder",
    "ConvertToChar",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadDecoder_ConvertToChar, Teardown)

EUNIT_TEST(
    "DecodeEventPayload - test ",
    "CDTMFPayloadDecoder",
    "DecodeEventPayload",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadDecoder_DecodeEventPayloadL, Teardown)

EUNIT_TEST(
    "DecodeTonePayload - test ",
    "CDTMFPayloadDecoder",
    "DecodeTonePayload",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadDecoder_DecodeTonePayloadL, Teardown)

EUNIT_TEST(
    "PayloadFormat - test ",
    "CDTMFPayloadDecoder",
    "PayloadFormat",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadDecoder_PayloadFormatL, Teardown)

EUNIT_TEST(
    "SetPayloadFormat - test ",
    "CDTMFPayloadDecoder",
    "SetPayloadFormat",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadDecoder_SetPayloadFormatL, Teardown)

EUNIT_TEST(
    "SetCurTimeStamp - test ",
    "CDTMFPayloadDecoder",
    "SetCurTimeStamp",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadDecoder_SetCurTimeStampL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
