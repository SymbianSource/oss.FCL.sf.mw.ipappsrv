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
#include "UT_CMccH263PayloadFormatWrite.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES
#include "MccH263PayloadFormatWrite.h"
#include "MccRtpDataSink.h"
#include "MmccCodecH263.h"
#include "Mcculdatapath.h"
#include "MccH263OldPayloadEncoder.h"
#include "mccuids.hrh"
#include "mccresourcepool.h"
#include "mccrtpmediaclock.h"


const TUid KRtpDataSink              =  { KImplUidRtpDataSink  };


// CONSTRUCTION
UT_CMccH263PayloadFormatWrite* UT_CMccH263PayloadFormatWrite::NewL()
    {
    UT_CMccH263PayloadFormatWrite* self = UT_CMccH263PayloadFormatWrite::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccH263PayloadFormatWrite* UT_CMccH263PayloadFormatWrite::NewLC()
    {
    UT_CMccH263PayloadFormatWrite* self = new( ELeave ) UT_CMccH263PayloadFormatWrite();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccH263PayloadFormatWrite::~UT_CMccH263PayloadFormatWrite()
    {
    }

// Default constructor
UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite()
    {
    }

// Second phase construct
void UT_CMccH263PayloadFormatWrite::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMccH263PayloadFormatWrite::SetupL(  )
    {
    iRtpMediaClock = CMccRtpMediaClock::NewL();
    iRtpSinkStub = CMccRtpDataSink::NewSinkL( KRtpDataSink, KNullDesC8 );
    TMediaId media = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
     CMccResourcePool* iMccResource = NULL;
    iULDataPathStub = CMccUlDataPath::NewL( eventHandler,iMccResource, media );
    iWrite = CMccH263PayloadFormatWrite::NewL( iRtpSinkStub );
    }

void UT_CMccH263PayloadFormatWrite::Teardown(  )
    {
    delete iWrite;
    delete iULDataPathStub;
	delete iRtpSinkStub;
    delete iRtpMediaClock;
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_NewLL(  )
    {
    CMccH263PayloadFormatWrite* write = CMccH263PayloadFormatWrite::NewL( iRtpSinkStub );
    CleanupStack::PushL( write );
    HBufC8* buf = HBufC8::NewL( 10 );
    delete buf; buf = NULL;
    CleanupStack::PopAndDestroy( write );
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_EmptySourceBufferLL(  )
    {
    // test preconditions, iStateMachine is NULL
    delete iWrite->iStateMachine;
    iWrite->iStateMachine = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptySourceBufferL(), KErrArgument );    
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_EmptySourceBufferL2L()
	{
	const TInt KH263TestDefaultPayloadSize = 12000; // KH263DefaultPayloadSize
	
	iWrite->iClip = NULL;
	EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->EmptySourceBufferL(), KErrArgument );    
	
	iWrite->iClip = iRtpSinkStub;
	CMMFDataBuffer* newBuffer = CMMFDataBuffer::NewL( KH263TestDefaultPayloadSize );
    CleanupStack::PushL( newBuffer );
  
    for (TInt i=0; i < KH263TestDefaultPayloadSize; i++)
	    {
	    newBuffer->Data().Append(_L8("a"));
	    }
    
    CBufferContainer* cont = new (ELeave) CBufferContainer();
    CleanupStack::Pop( newBuffer );
    cont->iBuffer = newBuffer;
	newBuffer = NULL;
	CleanupStack::PushL( cont );
    iWrite->iBuffers.AppendL(cont);
    CleanupStack::Pop( cont );

    iWrite->EmptySourceBufferL();
	}

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SourceBufferEmptiedLL(  )
    {
    // test preconditions iDataSource is NULL
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->SourceBufferEmptiedL(), KErrArgument );

    // normal test
    TBool bufRef = EFalse;
    TMediaId mediaId( KUidMediaTypeVideo ,1 );
    // ownership of buffer is not transferred
    CMMFBuffer* buffer = iWrite->CreateSinkBufferL( mediaId, bufRef );
    iWrite->iPayloadEncoder = CMccH263OldPayloadEncoder::NewL();
    iWrite->EmptyBufferL( buffer, iULDataPathStub, mediaId);
    iWrite->BufferEmptiedL( buffer );   
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_FrameTimeIntervalL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    EUNIT_ASSERT_EQUALS( iWrite->FrameTimeInterval( mediaId ).Int64(), 0 );  
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_DurationL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    EUNIT_ASSERT_EQUALS( iWrite->Duration( mediaId ).Int64(), 0 );    
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_EmptyBufferLL(  )
    {
    // test preconditions for EmptyBufferL 
   
    //--- test aBuffer is NULL ---//
    CMMFBuffer* buffer = NULL;
    MDataSource* dataSource = NULL;
    TMediaId mediaId( KUidMediaTypeVideo ,1 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
    	iWrite->EmptyBufferL(buffer, dataSource, mediaId ),KErrArgument );
    	
    //--- test aSupplier( dataSource ) is NULL ---//
    buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
    	iWrite->EmptyBufferL(buffer, dataSource, mediaId ),KErrArgument );
    
    //--- test aBuffer != iSourceBuffer ---//
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
    	iWrite->EmptyBufferL(buffer, iULDataPathStub, mediaId ),KErrArgument );
    CleanupStack::PopAndDestroy( buffer );    
    buffer = NULL;
    //--- test iPayloadEncoder is NULL ---//
    TBool bufRef = EFalse;
    // ownership of buffer is not transfered
    // this make aBuffer == iSourceBuffer is true
    buffer = iWrite->CreateSinkBufferL( mediaId, bufRef);
    delete iWrite->iPayloadEncoder;
    iWrite->iPayloadEncoder = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
    	iWrite->EmptyBufferL(buffer, dataSource, mediaId ),KErrArgument );
    
    //--- test iStateMachine is NULL ---//
    // ownership of buffer is not transfered
    // this make aBuffer == iSourceBuffer is true
    buffer = iWrite->CreateSinkBufferL( mediaId, bufRef);
    if ( !iWrite->iPayloadEncoder )
    iWrite->iPayloadEncoder = CMccH263NewPayloadEncoder::NewL();
    delete iWrite->iStateMachine;
    iWrite->iStateMachine = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
    	iWrite->EmptyBufferL(buffer, dataSource, mediaId ),KErrArgument );
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_EmptyBufferL2L()
	{

    TBool bufRef = EFalse;
    TMediaId mediaId( KUidMediaTypeVideo ,1 );
    // ownership of buffer is not transferred
    CMMFBuffer* buffer = iWrite->CreateSinkBufferL( mediaId, bufRef );
    iWrite->iPayloadEncoder = CMccH263OldPayloadEncoder::NewL();
    iWrite->EmptyBufferL( buffer, iULDataPathStub, mediaId);
    iWrite->BufferEmptiedL( buffer );   
	}

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = NULL;
    iWrite->BufferEmptiedL( buffer );
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_CanCreateSinkBufferL(  )
    {
    EUNIT_ASSERT_EQUALS( ETrue, iWrite->CanCreateSinkBuffer() );
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_CreateSinkBufferLL(  )
    {
    TBool bufRef = EFalse;
    CMMFBuffer* buffer = iWrite->CreateSinkBufferL (KUidMediaTypeVideo, bufRef );
    EUNIT_ASSERT( buffer );
    EUNIT_ASSERT( bufRef == ETrue );
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SinkPrimeLL(  )
    {
    iWrite->SinkPrimeL();
    iWrite->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->SinkPrimeL(), KErrArgument );
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SinkPlayLL(  )
    {
    iWrite->SinkPlayL();
    iWrite->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->SinkPlayL(), KErrArgument );    
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SinkPauseLL(  )
    {
    iWrite->SinkPauseL();
    iWrite->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->SinkPauseL(), KErrArgument );    
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SinkStopLL(  )
    {
    iWrite->SinkStopL();
    iWrite->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iWrite->SinkStopL(), KErrArgument );        }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SinkThreadLogonL(  )
    {
    MAsyncEventHandler* eventHandler = NULL;
    iWrite->SinkThreadLogon( *eventHandler );
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SinkThreadLogoffL(  )
    {
    iWrite->SinkThreadLogoff();
    
    // iClip is NULL
    iWrite->iClip = NULL;
    iWrite->SinkThreadLogoff();
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SinkDataTypeCodeL(  )
    {
    TMediaId mediaId( KUidMediaTypeVideo, 1 );
    TFourCC fourcc( KMccFourCCIdH263 );
    EUNIT_ASSERT_EQUALS( KErrNone, iWrite->SetSinkDataTypeCode(fourcc, mediaId) );
    
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT_EQUALS( TFourCC(), iWrite->SinkDataTypeCode( mediaId ) );
    
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT_EQUALS( TFourCC( KMccFourCCIdH263 ), iWrite->SinkDataTypeCode( mediaId ) );
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_SetSinkDataTypeCodeL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio, 1 );
    TFourCC fourcc( KMccFourCCIdH263 );
    // media type is not KUidMediaTypeVideo
    EUNIT_ASSERT_EQUALS( KErrNotSupported, iWrite->SetSinkDataTypeCode(fourcc, mediaId) );

    TMediaId mediaId2( KUidMediaTypeVideo, 1 );
    TFourCC fourcc2( KMccFourCCIdDTMF );
    
    // the fourcc2 is not KMccFourCCIdH263
    EUNIT_ASSERT_EQUALS( KErrNotSupported, iWrite->SetSinkDataTypeCode(fourcc2, mediaId2) );

    TMediaId mediaId3( KUidMediaTypeVideo, 1 );
    TFourCC fourcc3( KMccFourCCIdH263 );
    EUNIT_ASSERT_EQUALS( KErrNone, iWrite->SetSinkDataTypeCode(fourcc3, mediaId3) );
    }


void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_ConfigurePayloadFormatLL(  )
    {
    TMccCodecInfo codec;
    codec.iFourCC.Set(TFourCC('H','2','6', '3') );
    codec.iBitrate = 12200;
    codec.iCodecMode = KH263FormatModeOld;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iEnableDTX = EFalse;
    codec.iAlgoUsed = 2;
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 1;
    
    TMccCodecInfoBuffer buffer( codec );
	iWrite->ConfigurePayloadFormatL( buffer, *iRtpMediaClock ); 
    EUNIT_ASSERT( iWrite->iPayloadEncoder );
    
    delete iWrite->iPayloadEncoder;
    iWrite->iPayloadEncoder = NULL;
    codec.iCodecMode = KH263FormatModeNew;
    TMccCodecInfoBuffer buffer2( codec );
   	iWrite->ConfigurePayloadFormatL( buffer2, *iRtpMediaClock ); 
    EUNIT_ASSERT( iWrite->iPayloadEncoder );
    
    delete iWrite->iPayloadEncoder;
    iWrite->iPayloadEncoder = NULL;
    // code mode neither KH263FormatModeNew nor
    // KH263FormatModeOld
    codec.iCodecMode = 3;  
    TMccCodecInfoBuffer buffer3( codec );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iWrite->ConfigurePayloadFormatL( buffer3, *iRtpMediaClock ), KErrNotSupported );
  
    }

void UT_CMccH263PayloadFormatWrite::UT_CMccH263PayloadFormatWrite_ResetAndDestroy()
	{
	iWrite->PayloadEncodeCleanup( iWrite );	
	}
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccH263PayloadFormatWrite,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccH263PayloadFormatWrite",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_NewLL, Teardown)

EUNIT_TEST(
    "EmptySourceBufferL - test ",
    "CMccH263PayloadFormatWrite",
    "EmptySourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_EmptySourceBufferLL, Teardown)
  
EUNIT_TEST(
    "EmptySourceBuffer2L - test ",
    "CMccH263PayloadFormatWrite",
    "EmptySourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_EmptySourceBufferL2L, Teardown)

EUNIT_TEST(
    "SourceBufferEmptiedL - test ",
    "CMccH263PayloadFormatWrite",
    "SourceBufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SourceBufferEmptiedLL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CMccH263PayloadFormatWrite",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CMccH263PayloadFormatWrite",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_DurationL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccH263PayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "EmptyBuffer2L - test ",
    "CMccH263PayloadFormatWrite",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_EmptyBufferL2L, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccH263PayloadFormatWrite",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSinkBuffer - test ",
    "CMccH263PayloadFormatWrite",
    "CanCreateSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_CanCreateSinkBufferL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CMccH263PayloadFormatWrite",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CMccH263PayloadFormatWrite",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CMccH263PayloadFormatWrite",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CMccH263PayloadFormatWrite",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CMccH263PayloadFormatWrite",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SinkStopLL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CMccH263PayloadFormatWrite",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CMccH263PayloadFormatWrite",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccH263PayloadFormatWrite",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test",
    "CMccH263PayloadFormatWrite",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CMccH263PayloadFormatWrite",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_ConfigurePayloadFormatLL, Teardown)

EUNIT_TEST(
    "ResetAndDestroy - test ",
    "CMccH263PayloadFormatWrite",
    "ResetAndDestroy",
    "FUNCTIONALITY",
    SetupL, UT_CMccH263PayloadFormatWrite_ResetAndDestroy, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
