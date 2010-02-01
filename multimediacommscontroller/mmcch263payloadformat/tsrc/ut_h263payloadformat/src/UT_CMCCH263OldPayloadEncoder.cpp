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
#include "UT_CMccH263OldPayloadEncoder.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES
#include "MccH263OldPayloadEncoder.h"

// CONSTRUCTION
UT_CMccH263OldPayloadEncoder* UT_CMccH263OldPayloadEncoder::NewL()
    {
    UT_CMccH263OldPayloadEncoder* self = UT_CMccH263OldPayloadEncoder::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccH263OldPayloadEncoder* UT_CMccH263OldPayloadEncoder::NewLC()
    {
    UT_CMccH263OldPayloadEncoder* self = new( ELeave ) UT_CMccH263OldPayloadEncoder();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccH263OldPayloadEncoder::~UT_CMccH263OldPayloadEncoder()
    {
    }

// Default constructor
UT_CMccH263OldPayloadEncoder::UT_CMccH263OldPayloadEncoder()
    {
    }

// Second phase construct
void UT_CMccH263OldPayloadEncoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMccH263OldPayloadEncoder::ResetAndDestroy(  TAny* aAny )
	{
	UT_CMccH263OldPayloadEncoder* encoder = 
		static_cast<UT_CMccH263OldPayloadEncoder*>( aAny );
	encoder->iBufContainer.ResetAndDestroy();
	}

void UT_CMccH263OldPayloadEncoder::SetupL(  )
    {
    iEncoder = CMccH263OldPayloadEncoder::NewL();
    }

void UT_CMccH263OldPayloadEncoder::Teardown(  )
    {
    delete iEncoder;
    
	iBufContainer.ResetAndDestroy(); 
    }

void UT_CMccH263OldPayloadEncoder::UT_CMccH263OldPayloadEncoder_NewLL(  )
    {
    CMccH263OldPayloadEncoder* encoder = CMccH263OldPayloadEncoder::NewL();
    CleanupStack::PushL( encoder );
    HBufC8* buf = HBufC8::NewL( 10 );
    delete buf; buf = NULL;
    CleanupStack::PopAndDestroy( encoder );
    }

void UT_CMccH263OldPayloadEncoder::UT_CMccH263OldPayloadEncoder_EncodeLL(  )
    {
    ///---- case one start ---///
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    buffer->Data().Copy(_L8("any data.."));
   	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
    iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
    CleanupStack::Pop(); // TCleanupItem
    CleanupStack::PopAndDestroy( buffer );
    ///---- case one end ---///
    
    ///---- case two start ---///
    buffer = NULL;
    buffer = CMMFDataBuffer::NewL( 5000 );
    CleanupStack::PushL( buffer );
    
    for (TInt i=0; i < 4000; i++ )
	    {
	    buffer->Data().Append(_L8("a"));	
	    }
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
    iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
    CleanupStack::Pop(); // TCleanupItem
    CleanupStack::PopAndDestroy( buffer );
    ///---- case two end ---///
    
    ///---- case three start ---///
    buffer = NULL;
    buffer = CMMFDataBuffer::NewL( 5000 );
    CleanupStack::PushL( buffer );
    
    for (TInt i=0; i < 5000; i++ )
	    {
	    buffer->Data().Append(_L8("a"));	
	    }
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, this ) );
    iEncoder->EncodeL( static_cast<CMMFBuffer*>(buffer), iBufContainer );
    CleanupStack::Pop(); // TCleanupItem
    CleanupStack::PopAndDestroy( buffer );
    ///---- case three end ---///

    }

void UT_CMccH263OldPayloadEncoder::UT_CMccH263OldPayloadEncoder_SetPayloadTypeL(  )
    {
   	iEncoder->SetPayloadType( 96 );
	EUNIT_ASSERT_EQUALS( iEncoder->iSendHeader.iPayloadType, 96);
    }
    
 void UT_CMccH263OldPayloadEncoder::UT_CMccH263OldPayloadEncoder_WriteHeaderData()
	 {
	 TUint res = iEncoder->WriteHeaderData(0);
	 res = iEncoder->WriteHeaderData(1);
	 res = iEncoder->WriteHeaderData(2);
	 res = iEncoder->WriteHeaderData(3);
	 res = iEncoder->WriteHeaderData(4);
	 }
         
void UT_CMccH263OldPayloadEncoder::UT_CMccH263OldPayloadEncoder_GetHeaderInfoL()
	{
    CMMFDataBuffer* buf = CMMFDataBuffer::NewL(10);
    CleanupStack::PushL( buf );
    iEncoder->GetHeaderInfoL( buf->Data() );
    CleanupStack::PopAndDestroy( buf );
	}


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccH263OldPayloadEncoder,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccH263OldPayloadEncoder",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263OldPayloadEncoder_NewLL, Teardown)

EUNIT_TEST(
    "EncodeL - test ",
    "CMccH263OldPayloadEncoder",
    "EncodeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263OldPayloadEncoder_EncodeLL, Teardown)

EUNIT_TEST(
    "SetPayloadType - test ",
    "CMccH263OldPayloadEncoder",
    "SetPayloadType",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263OldPayloadEncoder_SetPayloadTypeL, Teardown)

EUNIT_TEST(
    "WriteHeaderData - test ",
    "CMccH263OldPayloadEncoder",
    "WriteHeaderData",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263OldPayloadEncoder_WriteHeaderData, Teardown)
    
EUNIT_TEST(
    "GetHeaderInfoL - test ",
    "CMccH263OldPayloadEncoder",
    "GetHeaderInfoL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263OldPayloadEncoder_GetHeaderInfoL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
