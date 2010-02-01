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
#include "UT_CMccAnySource.h"
#include "MmccInterfaceDef.h"
#include "mcctesteventhandler.h"



//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccanysource.h"


// CONSTRUCTION
UT_CMccAnySource* UT_CMccAnySource::NewL()
    {
    UT_CMccAnySource* self = UT_CMccAnySource::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccAnySource* UT_CMccAnySource::NewLC()
    {
    UT_CMccAnySource* self = new( ELeave ) UT_CMccAnySource();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccAnySource::~UT_CMccAnySource()
    {
    }

// Default constructor
UT_CMccAnySource::UT_CMccAnySource()
    {
    }

// Second phase construct
void UT_CMccAnySource::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccAnySource::SetupL(  )
    {
    iAnySource = static_cast<CMccAnySource*>( CMccAnySource::NewSourceL( KMccAnySourceUid, KNullDesC8 ) );
    iAnySource->ConstructSourceL( KNullDesC8 );
  
    iEventHandler = CMccTestEventHandler::NewL();
    iAnySource->SourceThreadLogon( *iEventHandler );
    }

void UT_CMccAnySource::Teardown(  )
    {
    delete iAnySource;
    delete iEventHandler;
    }

void UT_CMccAnySource::UT_CMccAnySource_SourceDataTypeCodeL(  )
    {
    TMediaId mediaId (KUidMediaTypeAudio);
    TFourCC gotFourCC = iAnySource->SourceDataTypeCode(mediaId);
    EUNIT_ASSERT( gotFourCC == TFourCC() );
    }

void UT_CMccAnySource::UT_CMccAnySource_SetSourceDataTypeCodeL(  )
    { 
    TFourCC fourCC( KMccFourCCIdAMRNB ); 
    TMediaId mediaId (KUidMediaTypeAudio);
    iAnySource->SetSourceDataTypeCode( fourCC, mediaId );
    }

void UT_CMccAnySource::UT_CMccAnySource_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iAnySource->BufferEmptiedL(buffer) ,KErrNotSupported );
    }

void UT_CMccAnySource::UT_CMccAnySource_CanCreateSourceBufferL(  )
    {
    EUNIT_ASSERT( !iAnySource->CanCreateSourceBuffer() );
    }

void UT_CMccAnySource::UT_CMccAnySource_CreateSourceBufferLL(  )
    {
    TMediaId mediaId;
    TBool bufref = EFalse;
    EUNIT_ASSERT_SPECIFIC_LEAVE(iAnySource->CreateSourceBufferL(mediaId, bufref), KErrNotSupported);
    }

void UT_CMccAnySource::UT_CMccAnySource_SourceThreadLogonL(  )
    {
    MAsyncEventHandler* handler = NULL;
    EUNIT_ASSERT( KErrNone == iAnySource->SourceThreadLogon(*handler) );
    
    // When no eventhandler, events cannot be sent anywhere
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccEventNone );
    iAnySource->SourcePrimeL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccEventNone );
    }

void UT_CMccAnySource::UT_CMccAnySource_SourceThreadLogoffL(  )
    {
    iAnySource->SourceThreadLogoff();
    }

void UT_CMccAnySource::UT_CMccAnySource_SourcePrimeLL(  )
    {
    iAnySource->SourcePrimeL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamPrepared );
    }

void UT_CMccAnySource::UT_CMccAnySource_SourcePlayLL(  )
    {
    iAnySource->SourcePlayL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamStarted );
    
    // Resume
    iAnySource->SourcePauseL();
    iAnySource->SourcePlayL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamResumed );
    EUNIT_ASSERT_EQUALS( iAnySource->iPaused, EFalse );
    }

void UT_CMccAnySource::UT_CMccAnySource_SourcePauseLL(  )
    {
    iAnySource->SourcePlayL();
    EUNIT_ASSERT_EQUALS( iAnySource->iPaused, EFalse );
    iAnySource->SourcePauseL();
    EUNIT_ASSERT_EQUALS( iAnySource->iPaused, ETrue );
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamPaused );
    }

void UT_CMccAnySource::UT_CMccAnySource_SourceStopLL(  )
    {
    iAnySource->SourceStopL();
    EUNIT_ASSERT_EQUALS( iEventHandler->iLastEvent.iEventType, KMccStreamStopped );
    }

void UT_CMccAnySource::UT_CMccAnySource_FillBufferLL(  )
    {
    TMediaId mediaId(KUidMediaTypeAudio);
    EUNIT_ASSERT_SPECIFIC_LEAVE( iAnySource->FillBufferL( NULL, NULL, mediaId ), KErrNotSupported );
    }

void UT_CMccAnySource::UT_CMccAnySource_NegotiateSourceLL(  )
    {
    MDataSink* sink = NULL;
    iAnySource->NegotiateSourceL(*sink);
    }
    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccAnySource,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccFileSource",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccFileSource",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccFileSource",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccFileSource",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccFileSource",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccFileSource",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CMccFileSource",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CMccFileSource",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CMccFileSource",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CMccFileSource",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CMccFileSource",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_SourceStopLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccFileSource",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_FillBufferLL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CMccFileSource",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccAnySource_NegotiateSourceLL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
