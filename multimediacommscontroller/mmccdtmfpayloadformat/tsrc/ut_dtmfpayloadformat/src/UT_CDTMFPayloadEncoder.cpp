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
#include "UT_CDTMFPayloadEncoder.h"

//  EXTERNAL INCLUDES
#include <mmf/server/mmfdatabuffer.h>



#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "dtmfpayloadencoder.h"
#include "dtmfeventpayloadinfo.h"
#include "dtmftonepayloadinfo.h"

// CONSTRUCTION
UT_CDTMFPayloadEncoder* UT_CDTMFPayloadEncoder::NewL()
    {
    UT_CDTMFPayloadEncoder* self = UT_CDTMFPayloadEncoder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CDTMFPayloadEncoder* UT_CDTMFPayloadEncoder::NewLC()
    {
    UT_CDTMFPayloadEncoder* self = new( ELeave ) UT_CDTMFPayloadEncoder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CDTMFPayloadEncoder::~UT_CDTMFPayloadEncoder()
    {
    }

// Default constructor
UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder()
    {
    }

// Second phase construct
void UT_CDTMFPayloadEncoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CDTMFPayloadEncoder::SetupL(  )
    {
    iEncoder = CDTMFPayloadEncoder::NewL();
    }

void UT_CDTMFPayloadEncoder::Teardown(  )
    {
    delete iEncoder;
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_EventAsInteger()
    {
    TChar ch;
    
    ch = '0';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == 0)
    
    ch = '8';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == 8)

    ch = '9';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == 9)
    
    ch = '*';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForAsterisk)
    
    ch = '#';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForHashMark)
    
    ch = 'a';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForA )
       
    ch = 'A';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForA )
    

    ch = 'b';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForB )
       
    ch = 'B';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForB )
    
    ch = 'c';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForC )
       
    ch = 'C';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForC )
    
    ch = 'd';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForD )
       
    ch = 'D';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KEventCodeForD )
    
    ch = 'e';
    EUNIT_ASSERT ( iEncoder->EventAsInteger( ch ) == KErrNotFound )
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_EncodeEventL(  )
    {
    TDTMFEventPayloadInfo payloadinfo;
 
    payloadinfo.SetEvent( '7' );
    payloadinfo.SetEndBit( 0 );
    payloadinfo.SetVolume( 4 );
    payloadinfo.SetDuration( 400 );
    
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    CleanupStack::PushL( buffer );
    buffer->Data().FillZ( KDTMFDefaultPayloadSize );
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( TInt64(5000) ) );
    
    iEncoder->EncodeEvent( payloadinfo, buffer );
    
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_EncodeToneL(  )
    {
    TDTMFTonePayloadInfo payloadinfo;
    payloadinfo.SetTone( '7' );
    payloadinfo.SetVolume( 4 );
    payloadinfo.SetDuration( 400 );
    
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    CleanupStack::PushL( buffer );
    buffer->Data().FillZ( KDTMFDefaultPayloadSize );
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( TInt64( 5000 ) ) );
    
    EUNIT_ASSERT( iEncoder->EncodeTone( payloadinfo, buffer ) == KErrNone );
    
    // Unknown tone
    payloadinfo.SetTone( '@' );
    EUNIT_ASSERT( iEncoder->EncodeTone( payloadinfo, buffer ) == KErrArgument );
     
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_UpdateEventPayloadL(  )
    {
    EUNIT_ASSERT( iEncoder->UpdateEventPayload(ETrue, 26800, NULL) == KErrArgument );
    
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    CleanupStack::PushL( buffer );
    buffer->Data().FillZ( KDTMFDefaultPayloadSize );
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( TInt64( 5000 ) ) );
    EUNIT_ASSERT( iEncoder->UpdateEventPayload( ETrue, 26800, buffer ) == KErrNone );  
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_UpdateTonePayloadL(  )
    {
    EUNIT_ASSERT( iEncoder->UpdateEventPayload(ETrue, 26800, NULL) == KErrArgument );
    
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( KDTMFDefaultPayloadSize );
    CleanupStack::PushL( buffer );
    buffer->Data().FillZ( KDTMFDefaultPayloadSize );
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( TInt64( 5000 ) ) );
    EUNIT_ASSERT( iEncoder->UpdateTonePayload( 26800, buffer ) == KErrNone );  
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_PayloadFormatL(  )
    {
    TDTMFPayloadFormat payloadformat( EDTMFPayloadFormatNotDefined );
    EUNIT_ASSERT( iEncoder->PayloadFormat( ) == EDTMFPayloadFormatNotDefined );

    EUNIT_ASSERT( iEncoder->SetPayloadFormat( payloadformat ) == KErrNotSupported );
    
    payloadformat = EDTMFPayloadFormatEvent;
    EUNIT_ASSERT( iEncoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iEncoder->PayloadFormat(  ) == EDTMFPayloadFormatEvent );

    payloadformat = EDTMFPayloadFormatTone;
    EUNIT_ASSERT( iEncoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iEncoder->PayloadFormat(  ) == EDTMFPayloadFormatTone );

    payloadformat = EDTMFPayloadFormatRedEvents;
    EUNIT_ASSERT( iEncoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iEncoder->PayloadFormat(  ) == EDTMFPayloadFormatRedEvents );
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_SetPayloadFormatL(  )
    {
    TDTMFPayloadFormat payloadformat( EDTMFPayloadFormatNotDefined );
    
    EUNIT_ASSERT( iEncoder->SetPayloadFormat( payloadformat ) == KErrNotSupported );
    
    payloadformat = EDTMFPayloadFormatEvent;
    EUNIT_ASSERT( iEncoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iEncoder->PayloadFormat(  ) == EDTMFPayloadFormatEvent );

    payloadformat = EDTMFPayloadFormatTone;
    EUNIT_ASSERT( iEncoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iEncoder->PayloadFormat(  ) == EDTMFPayloadFormatTone );

    payloadformat = EDTMFPayloadFormatRedEvents;
    EUNIT_ASSERT( iEncoder->SetPayloadFormat( payloadformat ) == KErrNone );
    EUNIT_ASSERT( iEncoder->PayloadFormat(  ) == EDTMFPayloadFormatRedEvents );
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_PayloadTypeL()
    {
    EUNIT_ASSERT_NO_LEAVE( iEncoder->SetPayloadType( 20 ) );
    EUNIT_ASSERT( iEncoder->PayloadType() == 20 );
    
    EUNIT_ASSERT_NO_LEAVE( iEncoder->SetPayloadType( 109 ) );
    EUNIT_ASSERT( iEncoder->PayloadType() == 109 );
    }

void UT_CDTMFPayloadEncoder::UT_CDTMFPayloadEncoder_SetPayloadTypeL()
    {
    EUNIT_ASSERT_NO_LEAVE( iEncoder->SetPayloadType( 20 ) );
    EUNIT_ASSERT( iEncoder->PayloadType() == 20 );
    
    EUNIT_ASSERT_NO_LEAVE( iEncoder->SetPayloadType( 109 ) );
    EUNIT_ASSERT( iEncoder->PayloadType() == 109 );
    }

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CDTMFPayloadEncoder,
    "MCC DTMF payload encoder",
    "UNIT" )

EUNIT_TEST(
    "EventAsInteger - test ",
    "CDTMFPayloadEncoder",
    "EventAsInteger",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_EventAsInteger, Teardown)

EUNIT_TEST(
    "EncodeEvent - test ",
    "CDTMFPayloadEncoder",
    "EncodeEvent",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_EncodeEventL, Teardown)

EUNIT_TEST(
    "EncodeTone - test ",
    "CDTMFPayloadEncoder",
    "EncodeTone",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_EncodeToneL, Teardown)

EUNIT_TEST(
    "UpdateEventPayload - test ",
    "CDTMFPayloadEncoder",
    "UpdateEventPayload",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_UpdateEventPayloadL, Teardown)

EUNIT_TEST(
    "UpdateTonePayload - test ",
    "CDTMFPayloadEncoder",
    "UpdateTonePayload",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_UpdateTonePayloadL, Teardown)

EUNIT_TEST(
    "PayloadFormat - test ",
    "CDTMFPayloadEncoder",
    "PayloadFormat",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_PayloadFormatL, Teardown)

EUNIT_TEST(
    "SetPayloadFormat - test ",
    "CDTMFPayloadEncoder",
    "SetPayloadFormat",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_SetPayloadFormatL, Teardown)

EUNIT_TEST(
    "PayloadType - test ",
    "CDTMFPayloadEncoder",
    "PayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_PayloadTypeL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CDTMFPayloadEncoder",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CDTMFPayloadEncoder_SetPayloadTypeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
