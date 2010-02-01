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
#include "UT_TStreamEncoder.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "StreamFormatter.h"

// CONSTRUCTION
UT_TStreamEncoder* UT_TStreamEncoder::NewL()
    {
    UT_TStreamEncoder* self = UT_TStreamEncoder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_TStreamEncoder* UT_TStreamEncoder::NewLC()
    {
    UT_TStreamEncoder* self = new( ELeave ) UT_TStreamEncoder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_TStreamEncoder::~UT_TStreamEncoder()
    {
    }

// Default constructor
UT_TStreamEncoder::UT_TStreamEncoder()
    {
    }

// Second phase construct
void UT_TStreamEncoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_TStreamEncoder::SetupL(  )
    {

    }

void UT_TStreamEncoder::Teardown(  )
    {

    }

void UT_TStreamEncoder::UT_TStreamEncoder_InitializeL(  )
    {
    TStreamEncoder enc;
    enc.Initialize( NULL, 0, 0 );
    }

void UT_TStreamEncoder::UT_TStreamEncoder_EncodeL(  )
    {
    TStreamEncoder enc;
    HBufC8* buf = HBufC8::NewLC( 100 );
    enc.Initialize( const_cast<TUint8*>( buf->Des().Ptr() ), 0, 8 );
    enc.Encode( 0, 0 );
    enc.Encode( 0, 33 );

    enc.Encode( 5, 8 );

    CleanupStack::PopAndDestroy( buf );
    }

void UT_TStreamEncoder::UT_TStreamEncoder_Encode_1L(  )
    {
    TStreamEncoder enc;
    HBufC8* buf = HBufC8::NewLC( 100 );
    enc.Initialize( const_cast<TUint8*>( buf->Des().Ptr() ), 0, 0 );
    
    TBuf8<50> fromBuf;
    fromBuf.Format( _L8( "1234568901234568901234567890" ) );
    fromBuf.Trim();
    
    enc.Encode( fromBuf.Ptr(), 0, 8, 8 );
    
    CleanupStack::PopAndDestroy( buf );
    }

void UT_TStreamEncoder::UT_TStreamEncoder_ByteIndexL(  )
    {
    TStreamEncoder enc;
    enc.Initialize( NULL, 1, 2 );
    EUNIT_ASSERT_EQUALS( enc.ByteIndex(), 1 );
    }

void UT_TStreamEncoder::UT_TStreamEncoder_BitIndexL(  )
    {
    TStreamEncoder enc;
    enc.Initialize( NULL, 1, 2 );
    EUNIT_ASSERT_EQUALS( enc.BitIndex(), 2 );
    }

void UT_TStreamEncoder::UT_TStreamEncoder_SetByteIndexL(  )
    {
    TStreamEncoder enc;
    enc.Initialize( NULL, 1, 2 );
    enc.SetByteIndex( 3 );
    EUNIT_ASSERT_EQUALS( enc.ByteIndex(), 3 );
    }

void UT_TStreamEncoder::UT_TStreamEncoder_SetBitIndexL(  )
    {
    TStreamEncoder enc;
    enc.Initialize( NULL, 1, 2 );
    enc.SetBitIndex( 4 );
    EUNIT_ASSERT_EQUALS( enc.BitIndex(), 4 );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_TStreamEncoder,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "Initialize - test ",
    "TStreamEncoder",
    "Initialize",
    "FUNCTIONALITY",
    SetupL, UT_TStreamEncoder_InitializeL, Teardown)

EUNIT_TEST(
    "Encode - test ",
    "TStreamEncoder",
    "Encode",
    "FUNCTIONALITY",
    SetupL, UT_TStreamEncoder_EncodeL, Teardown)

EUNIT_TEST(
    "Encode - test ",
    "TStreamEncoder",
    "Encode",
    "FUNCTIONALITY",
    SetupL, UT_TStreamEncoder_Encode_1L, Teardown)

EUNIT_TEST(
    "ByteIndex - test ",
    "TStreamEncoder",
    "ByteIndex",
    "FUNCTIONALITY",
    SetupL, UT_TStreamEncoder_ByteIndexL, Teardown)

EUNIT_TEST(
    "BitIndex - test ",
    "TStreamEncoder",
    "BitIndex",
    "FUNCTIONALITY",
    SetupL, UT_TStreamEncoder_BitIndexL, Teardown)

EUNIT_TEST(
    "SetByteIndex - test ",
    "TStreamEncoder",
    "SetByteIndex",
    "FUNCTIONALITY",
    SetupL, UT_TStreamEncoder_SetByteIndexL, Teardown)

EUNIT_TEST(
    "SetBitIndex - test ",
    "TStreamEncoder",
    "SetBitIndex",
    "FUNCTIONALITY",
    SetupL, UT_TStreamEncoder_SetBitIndexL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
