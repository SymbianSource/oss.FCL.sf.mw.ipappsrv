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
#include "UT_TStreamDecoder.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "StreamFormatter.h"

// CONSTRUCTION
UT_TStreamDecoder* UT_TStreamDecoder::NewL()
    {
    UT_TStreamDecoder* self = UT_TStreamDecoder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_TStreamDecoder* UT_TStreamDecoder::NewLC()
    {
    UT_TStreamDecoder* self = new( ELeave ) UT_TStreamDecoder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_TStreamDecoder::~UT_TStreamDecoder()
    {
    }

// Default constructor
UT_TStreamDecoder::UT_TStreamDecoder()
    {
    }

// Second phase construct
void UT_TStreamDecoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_TStreamDecoder::SetupL(  )
    {

    }

void UT_TStreamDecoder::Teardown(  )
    {

    }

void UT_TStreamDecoder::UT_TStreamDecoder_InitializeL(  )
    {
    TStreamDecoder dec;
    dec.Initialize( NULL, 1, 2 );
    }

void UT_TStreamDecoder::UT_TStreamDecoder_DecodeL(  )
    {
    TStreamDecoder dec;
    TBuf8<50> data;
    data.Format( _L8( "foo42foo42foo42foo42" ) );
    data.Trim();
    HBufC8* buf = data.AllocLC();
    
    dec.Initialize( const_cast<TUint8*>( buf->Des().Ptr() ), 0, 8 );
    TUint32 dummy( 0 );
    dummy = dec.Decode( 0 );
    dummy = dec.Decode( 33 );
    dummy = dec.Decode( 8 );

    CleanupStack::PopAndDestroy( buf );
    }

void UT_TStreamDecoder::UT_TStreamDecoder_Decode_1L(  )
    {
    TStreamDecoder dec;
    TBuf8<50> data;
    data.Format( _L8( "foo42foo42foo42foo42" ) );
    data.Trim();
    HBufC8* buf = data.AllocLC();
    
    dec.Initialize( const_cast<TUint8*>( buf->Des().Ptr() ), 0, 0 );
    
    HBufC8* toBuf = HBufC8::NewLC( 50 );

    dec.Decode( const_cast<TUint8*>( toBuf->Des().Ptr() ), 0, 0, 16 );
    
    dec.Decode( const_cast<TUint8*>( toBuf->Des().Ptr() ), 0, 0, 16 );

    CleanupStack::PopAndDestroy( toBuf );
    CleanupStack::PopAndDestroy( buf );
    }

void UT_TStreamDecoder::UT_TStreamDecoder_ByteIndexL(  )
    {
    TStreamDecoder dec;
    dec.Initialize( NULL, 1, 2 );
    EUNIT_ASSERT_EQUALS( dec.ByteIndex(), 1 );
    }

void UT_TStreamDecoder::UT_TStreamDecoder_BitIndexL(  )
    {
    TStreamDecoder dec;
    dec.Initialize( NULL, 1, 2 );
    EUNIT_ASSERT_EQUALS( dec.BitIndex(), 2 );
    }

void UT_TStreamDecoder::UT_TStreamDecoder_SetByteIndexL(  )
    {
    TStreamDecoder dec;
    dec.Initialize( NULL, 1, 2 );
    dec.SetByteIndex( 3 );
    EUNIT_ASSERT_EQUALS( dec.ByteIndex(), 3 );
    }

void UT_TStreamDecoder::UT_TStreamDecoder_SetBitIndexL(  )
    {
    TStreamDecoder dec;
    dec.Initialize( NULL, 1, 2 );
    dec.SetBitIndex( 4 );
    EUNIT_ASSERT_EQUALS( dec.BitIndex(), 4 );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_TStreamDecoder,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "Initialize - test ",
    "TStreamDecoder",
    "Initialize",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_InitializeL, Teardown)

EUNIT_TEST(
    "Decode - test ",
    "TStreamDecoder",
    "Decode",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_DecodeL, Teardown)

EUNIT_TEST(
    "Decode - test ",
    "TStreamDecoder",
    "Decode",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_Decode_1L, Teardown)

EUNIT_TEST(
    "ByteIndex - test ",
    "TStreamDecoder",
    "ByteIndex",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_ByteIndexL, Teardown)

EUNIT_TEST(
    "BitIndex - test ",
    "TStreamDecoder",
    "BitIndex",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_BitIndexL, Teardown)

EUNIT_TEST(
    "SetByteIndex - test ",
    "TStreamDecoder",
    "SetByteIndex",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_SetByteIndexL, Teardown)

EUNIT_TEST(
    "SetBitIndex - test ",
    "TStreamDecoder",
    "SetBitIndex",
    "FUNCTIONALITY",
    SetupL, UT_TStreamDecoder_SetBitIndexL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
