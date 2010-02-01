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
#include "UT_CMccH263NewPayloadEncoder.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <e32des8.h>

#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES
#include "MccH263NewPayloadEncoder.h"


#define INSERT_GOB_BOUNDARY( data, start_index ) { \
TInt s( start_index ); \
data[ s ] = 0x00; \
data[ ++s ] = 0x00; \
data[ ++s ] = 0xC0; \
}
    
// CONSTRUCTION
UT_CMccH263NewPayloadEncoder* UT_CMccH263NewPayloadEncoder::NewL()
    {
    UT_CMccH263NewPayloadEncoder* self = UT_CMccH263NewPayloadEncoder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccH263NewPayloadEncoder* UT_CMccH263NewPayloadEncoder::NewLC()
    {
    UT_CMccH263NewPayloadEncoder* self = new( ELeave ) UT_CMccH263NewPayloadEncoder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccH263NewPayloadEncoder::~UT_CMccH263NewPayloadEncoder()
    {
    }

// Default constructor
UT_CMccH263NewPayloadEncoder::UT_CMccH263NewPayloadEncoder()
    {
    }

// Second phase construct
void UT_CMccH263NewPayloadEncoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMccH263NewPayloadEncoder::ResetAndDestroy( TAny* aAny )
	{
	UT_CMccH263NewPayloadEncoder* encoder = 
		static_cast<UT_CMccH263NewPayloadEncoder*>( aAny );
	encoder->iBufContainer.ResetAndDestroy();
	}

void UT_CMccH263NewPayloadEncoder::SetupL(  )
    {
    iEncoder = CMccH263NewPayloadEncoder::NewL();
    }

void UT_CMccH263NewPayloadEncoder::Teardown(  )
    {
    delete iEncoder;
    iBufContainer.ResetAndDestroy();
    }

void UT_CMccH263NewPayloadEncoder::UT_CMccH263NewPayloadEncoder_NewLL(  )
    {
    CMccH263NewPayloadEncoder* encoder = CMccH263NewPayloadEncoder::NewL();
    CleanupStack::PushL( encoder );
    HBufC8* buf = HBufC8::NewL(10);
    delete buf;    buf = NULL;
    CleanupStack::PopAndDestroy( encoder );
    }

void UT_CMccH263NewPayloadEncoder::UT_CMccH263NewPayloadEncoder_EncodeLL(  )
    {
   
    ///---- case 1 start ---///
    // One small chuck without gobs
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    buffer->Data().Copy(_L8("any data.."));
    CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
    iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
    EUNIT_ASSERT_EQUALS( iBufContainer.Count(), 1 );
    iBufContainer.ResetAndDestroy();
    CleanupStack::Pop(); // TCleanupItem
    CleanupStack::PopAndDestroy( buffer );
    ///---- case 1 end ---///
    
    ///---- case 2 start ---///
    // One big chuck without gobs (has to be divided to several packets)
    buffer = NULL;
    buffer = CMMFDataBuffer::NewL( 5000 );
    CleanupStack::PushL( buffer );
    
    for (TInt i=0; i < 4000; i++ )
	    {
	    buffer->Data().Append(_L8("a"));	
	    }
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
    iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
    EUNIT_ASSERT_EQUALS( iBufContainer.Count(), 5 );
    iBufContainer.ResetAndDestroy();
    CleanupStack::Pop(); // TCleanupItem
    CleanupStack::PopAndDestroy( buffer );
    ///---- case 2 end ---///
    
    ///---- case 3 start ---///
    // One big chuck without gobs (has to be divided to several packets)
    buffer = NULL;
    buffer = CMMFDataBuffer::NewL( 5000 );
    CleanupStack::PushL( buffer );
    
    for (TInt i=0; i < 5000; i++ )
	    {
	    buffer->Data().Append(_L8("a"));	
	    }
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
	iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
	EUNIT_ASSERT_EQUALS( iBufContainer.Count(), 6 );
    iBufContainer.ResetAndDestroy();
	CleanupStack::Pop(); // TCleanupItem
	CleanupStack::PopAndDestroy( buffer );
    buffer = NULL;
    ///---- case 3 end ---///
    
    ///---- case 4 start ---///
    // Gobs in small buffer (no need to divide to several packets)
    buffer = NULL;
    buffer = CMMFDataBuffer::NewL( 200 );
    CleanupStack::PushL( buffer );
    
    for (TInt i=0; i < 200; i++ )
	    {
	    buffer->Data().Append(_L8("a"));	
	    }
	INSERT_GOB_BOUNDARY( buffer->Data(), 100 )
	INSERT_GOB_BOUNDARY( buffer->Data(), 150 )
	
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
	iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
	EUNIT_ASSERT_EQUALS( iBufContainer.Count(), 1 );
    iBufContainer.ResetAndDestroy();
	CleanupStack::Pop(); // TCleanupItem
	CleanupStack::PopAndDestroy( buffer );
    buffer = NULL;
    ///---- case 4 end ---///
    
    ///---- case 5 start ---///
    // Gobs in big buffer (needs to be divided to several packets)
    buffer = NULL;
    buffer = CMMFDataBuffer::NewL( 5000 );
    CleanupStack::PushL( buffer );
    
    for (TInt i=0; i < 4000; i++ )
	    {
	    buffer->Data().Append(_L8("a"));	
	    }
	
	INSERT_GOB_BOUNDARY( buffer->Data(), 100 )
	INSERT_GOB_BOUNDARY( buffer->Data(), 150 )
	INSERT_GOB_BOUNDARY( buffer->Data(), 1550 )
	INSERT_GOB_BOUNDARY( buffer->Data(), 2550 )
	INSERT_GOB_BOUNDARY( buffer->Data(), 3550 )
	
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
	iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
	EUNIT_ASSERT_EQUALS( iBufContainer.Count(), 3 );
    iBufContainer.ResetAndDestroy();
	CleanupStack::Pop(); // TCleanupItem
	CleanupStack::PopAndDestroy( buffer );
    buffer = NULL;
    ///---- case 5 end ---///
    
    ///---- case 6 start ---///
    // Gobs in big buffer but first gob too far from beginning.
    // Needs to be divided to several packets but cannot be done based on gob boundaries
    // (as in case 3)
    buffer = NULL;
    buffer = CMMFDataBuffer::NewL( 5000 );
    CleanupStack::PushL( buffer );
    
    for (TInt i=0; i < 5000; i++ )
	    {
	    buffer->Data().Append(_L8("a"));	
	    }
	INSERT_GOB_BOUNDARY( buffer->Data(), 2550 )
	INSERT_GOB_BOUNDARY( buffer->Data(), 3550 )
	
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
	iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
	EUNIT_ASSERT_EQUALS( iBufContainer.Count(), 6 );
    iBufContainer.ResetAndDestroy();
	CleanupStack::Pop(); // TCleanupItem
	CleanupStack::PopAndDestroy( buffer );
    buffer = NULL;
    ///---- case 6 end ---///
    
    }

void UT_CMccH263NewPayloadEncoder::UT_CMccH263NewPayloadEncoder_SetPayloadTypeL(  )
    {
   	iEncoder->SetPayloadType( 96 );
	EUNIT_ASSERT_EQUALS( iEncoder->iSendHeader.iPayloadType, 96);
    }
    
void UT_CMccH263NewPayloadEncoder::UT_CMccH263NewPayloadEncoder_WriteHeaderData()
	{
	TUint res = iEncoder->WriteHeaderData(0);
	res = iEncoder->WriteHeaderData(1);
	res = iEncoder->WriteHeaderData(2);
	}
         
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccH263NewPayloadEncoder,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccH263NewPayloadEncoder",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263NewPayloadEncoder_NewLL, Teardown)

EUNIT_TEST(
    "EncodeL - test ",
    "CMccH263NewPayloadEncoder",
    "EncodeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263NewPayloadEncoder_EncodeLL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccH263NewPayloadEncoder",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263NewPayloadEncoder_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "WriteHeaderData - test ",
    "CMccH263NewPayloadEncoder",
    "WriteHeaderData",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263NewPayloadEncoder_WriteHeaderData, Teardown)
  

EUNIT_END_TEST_TABLE

//  END OF FILE
