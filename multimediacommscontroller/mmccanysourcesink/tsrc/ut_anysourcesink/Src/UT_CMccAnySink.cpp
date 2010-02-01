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
#include "UT_CMccAnySink.h"
#include "MmccInterfaceDef.h"



//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "mccanysink.h"
#include "mcctesteventhandler.h"


// CONSTRUCTION
UT_CMccAnySink* UT_CMccAnySink::NewL()
    {
    UT_CMccAnySink* self = UT_CMccAnySink::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccAnySink* UT_CMccAnySink::NewLC()
    {
    UT_CMccAnySink* self = new( ELeave ) UT_CMccAnySink();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccAnySink::~UT_CMccAnySink()
    {
    }

// Default constructor
UT_CMccAnySink::UT_CMccAnySink()
    {
    }

// Second phase construct
void UT_CMccAnySink::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccAnySink::SetupL(  )
    {
    iAnySink = static_cast<CMccAnySink*>( CMccAnySink::NewSinkL( KMccAnySinkUid, KNullDesC8 ) );
    iAnySink->ConstructSinkL( KNullDesC8 );
    
    iEventHandler = CMccTestEventHandler::NewL();
    iAnySink->SinkThreadLogon( *iEventHandler );
    }

void UT_CMccAnySink::Teardown(  )
    {
    delete iAnySink;
    delete iEventHandler;
    }

void UT_CMccAnySink::UT_CMccAnySink_SinkDataTypeCodeL(  )
    {
	TMediaId mediaId;
    EUNIT_ASSERT(iAnySink->SinkDataTypeCode( mediaId ) == TFourCC() );      
    }

void UT_CMccAnySink::UT_CMccAnySink_SetSinkDataTypeCodeL(  )
    {
    TMediaId mediaId( KUidMediaTypeVideo );
    TFourCC codec1( KMccFourCCIdH263 );
    EUNIT_ASSERT_EQUALS(
    	iAnySink->SetSinkDataTypeCode(codec1, mediaId),
        KErrNone );
    }

void UT_CMccAnySink::UT_CMccAnySink_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE(iAnySink->BufferEmptiedL(buffer), KErrNotSupported);
    }

void UT_CMccAnySink::UT_CMccAnySink_CanCreateSinkBufferL(  )
    {
    EUNIT_ASSERT(!iAnySink->CanCreateSinkBuffer() );
    }

void UT_CMccAnySink::UT_CMccAnySink_CreateSinkBufferLL(  )
    {
	TMediaId mediaId;
    TBool bufRef = EFalse;
    mediaId.iMediaType = KUidMediaTypeMidi;
	EUNIT_ASSERT_SPECIFIC_LEAVE(
		iAnySink->CreateSinkBufferL(mediaId,bufRef), KErrNotSupported );
    }

void UT_CMccAnySink::UT_CMccAnySink_SinkThreadLogonL(  )
    {
    MAsyncEventHandler* aHandler = NULL;
	iAnySink->SinkThreadLogon( *aHandler );
	
	// When no eventhandler, events cannot be sent anywhere
	EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccEventNone );
    iAnySink->SinkPrimeL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccEventNone );
    }

void UT_CMccAnySink::UT_CMccAnySink_SinkThreadLogoffL(  )
    {
	iAnySink->SinkThreadLogoff();
    }

void UT_CMccAnySink::UT_CMccAnySink_SinkPrimeLL(  )
    {
	iAnySink->SinkPrimeL();
	EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamPrepared );
    }

void UT_CMccAnySink::UT_CMccAnySink_SinkPlayLL(  )
    {
    iAnySink->SinkPlayL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamStarted );
    
    // Resume
    iAnySink->SinkPauseL();
    iAnySink->SinkPlayL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamResumed );
    EUNIT_ASSERT_EQUALS( iAnySink->iPaused, EFalse );
    }

void UT_CMccAnySink::UT_CMccAnySink_SinkPauseLL(  )
    {
    iAnySink->SinkPauseL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamPaused );
    EUNIT_ASSERT_EQUALS( iAnySink->iPaused, ETrue );
    }

void UT_CMccAnySink::UT_CMccAnySink_SinkStopLL(  )
    { 
    iAnySink->SinkStopL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamStopped );
    }

void UT_CMccAnySink::UT_CMccAnySink_EmptyBufferLL(  )
    {
    TMediaId media;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iAnySink->EmptyBufferL( NULL, NULL, media ), KErrNotSupported );
    }

void UT_CMccAnySink::UT_CMccAnySink_BufferFilledLL(  )
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iAnySink->BufferFilledL( NULL ), KErrNotSupported );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccAnySink,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccFileSink",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CMccFileSink",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccFileSink",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSinkBuffer - test ",
    "CMccFileSink",
    "CanCreateSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_CanCreateSinkBufferL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CMccFileSink",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CMccFileSink",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CMccFileSink",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CMccFileSink",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CMccFileSink",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CMccFileSink",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CMccFileSink",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_SinkStopLL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccFileSink",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccFileSink",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySink_BufferFilledLL, Teardown)
   
EUNIT_END_TEST_TABLE

//  END OF FILE
