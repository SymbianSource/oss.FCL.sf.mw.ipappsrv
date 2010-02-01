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
#include "UT_CAnyPayloadFormatRead.h"
#include "MccRtpDataSink.h"
#include "mccuids.hrh"
#include "Mccdldatapath.h"
#include "MccRtpDataSource.h"
#include "mccresourcepool.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES
#include "AnyPayloadFormatRead.h"
#include <mmf/server/mmfdatabuffer.h>



#include <mmf/server/mmfvideoframebuffer.h>

const TUid KRtpDataSource          =  { KImplUidRtpDataSource   }; 


// CONSTRUCTION
UT_CAnyPayloadFormatRead* UT_CAnyPayloadFormatRead::NewL()
    {
    UT_CAnyPayloadFormatRead* self = UT_CAnyPayloadFormatRead::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CAnyPayloadFormatRead* UT_CAnyPayloadFormatRead::NewLC()
    {
    UT_CAnyPayloadFormatRead* self = new( ELeave ) UT_CAnyPayloadFormatRead();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CAnyPayloadFormatRead::~UT_CAnyPayloadFormatRead()
    {
    }

// Default constructor
UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead()
    {
    }

// Second phase construct
void UT_CAnyPayloadFormatRead::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CAnyPayloadFormatRead::SetupL(  )
    {
    iRtpDataSourceStub = CMccRtpDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    TMediaId media = KUidMediaTypeAudio;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( eventHandler, mccresource, media ); 
    iBuffer = CMMFDescriptorBuffer::NewL( 16 );
    iFormatRead = CAnyPayloadFormatRead::NewL( iRtpDataSourceStub );
    }

void UT_CAnyPayloadFormatRead::Teardown(  )
    {
	delete iFormatRead;

	delete iBuffer;	

    delete iDLDataPathStub;
    delete iRtpDataSourceStub;
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_NewL()
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( CAnyPayloadFormatRead::NewL( NULL ), KErrArgument );    
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_CreateClipBufferL()
    {
    TUint size = 100;
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_LastDlPacketCountL(  )
    {
    EUNIT_ASSERT_EQUALS ( iFormatRead->LastDlPacketCount(), 0);
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_Duration()
	{
    EUNIT_ASSERT_EQUALS( iFormatRead->Duration( KUidMediaTypeMidi ).Int64(), 0 );
	}

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_CancelDlRequestL(  )
    {
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );
	iFormatRead->iFrameBufferTwo = CMMFDataBuffer::NewL( 5000 );
    iFormatRead->CancelDlRequest() ;
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SendDataToSinkLL(  )
    {
    // iDataPath is empty
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->SendDataToSinkL(), KErrArgument );
    
    // iCurrentBuffer is NULL
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );
    iFormatRead->iDataPath = iDLDataPathStub;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->SendDataToSinkL(), KErrArgument );
       
    iFormatRead->iCurrentBuffer = iFormatRead->iFrameBufferOne;
    EUNIT_ASSERT_NO_LEAVE(iFormatRead->SendDataToSinkL());
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_FillSinkBufferL(  )
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillSinkBufferL(), KErrArgument );
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );
    iFormatRead->iCurrentBuffer = iFormatRead->iFrameBufferOne;
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->FillSinkBufferL() );
    
     // iStateMachine = NULL
    delete iFormatRead->iStateMachine;
    iFormatRead->iStateMachine = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillSinkBufferL(), KErrArgument );

    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_FillSourceBufferLL(  )
    {
    // iPayloadBuffer is NULL
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillSourceBufferL(), KErrArgument );

    iFormatRead->iPayloadBuffer 
        = iFormatRead->CreateClipBufferL( 500, iFormatRead->iSourceBufOwnership );

    EUNIT_ASSERT_NO_LEAVE( iFormatRead->FillSourceBufferL() );
    
    // iClip is NULL
    iFormatRead->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillSourceBufferL(), KErrArgument );    
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_StreamsL(  )
    {
    EUNIT_ASSERT_EQUALS( 1, iFormatRead->Streams( KUidMediaTypeVideo ) );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_FrameTimeIntervalL(  )
    {
    EUNIT_ASSERT_EQUALS(0, iFormatRead->FrameTimeInterval( KUidMediaTypeVideo ).Int64() );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_DurationL(  )
    {
    EUNIT_ASSERT_EQUALS(0, iFormatRead->FrameTimeInterval( KUidMediaTypeVideo ).Int64() );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SinkDataTypeCodeL(  )
    {
    EUNIT_ASSERT_EQUALS(KErrNotSupported, iFormatRead->SetSinkDataTypeCode( TFourCC( ' ','A','N','Y' ), KUidMediaTypeVideo ) );
    EUNIT_ASSERT_EQUALS( iFormatRead->SinkDataTypeCode( KUidMediaTypeVideo ), iFormatRead->iFourCC );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_FillBufferLL(  )
    {
    // aBuffer is NULL
    EUNIT_ASSERT_SPECIFIC_LEAVE(iFormatRead->FillBufferL( NULL, iDLDataPathStub, KUidMediaTypeVideo), KErrArgument);
        
    // aBuffer->Type() != KUidMmfDataBuffer  
    CMMFYUVBuffer* buffer = CMMFYUVBuffer::NewL();
    CleanupStack::PushL( buffer );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillBufferL( buffer, iDLDataPathStub, KUidMediaTypeVideo), KErrArgument );
    CleanupStack::PopAndDestroy( buffer );
    
    // aConsumer = NULL 
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillBufferL( iBuffer, NULL, KUidMediaTypeVideo), KErrArgument );

    // iFrameBufferOne = NULL
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillBufferL( iBuffer, iDLDataPathStub, KUidMediaTypeVideo), KErrArgument );

    // iFrameBufferTwo = NULL
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillBufferL( iBuffer, iDLDataPathStub, KUidMediaTypeVideo), KErrArgument );

    // aBuffer != iCurrentBuffer   
    iFormatRead->iFrameBufferTwo = CMMFDataBuffer::NewL( 5000 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillBufferL( iBuffer, iDLDataPathStub, KUidMediaTypeVideo), KErrArgument );
 
    //// No leaves
    iFormatRead->iCurrentBuffer = iFormatRead->iFrameBufferOne;
    iFormatRead->iPayloadBuffer = 
        iFormatRead->CreateClipBufferL( 500, iFormatRead->iSourceBufOwnership );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->FillBufferL( iFormatRead->iFrameBufferOne, iDLDataPathStub, KUidMediaTypeVideo) );
    
    // iStateMachine = NULL
    delete iFormatRead->iStateMachine;
    iFormatRead->iStateMachine = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillBufferL( iBuffer, iDLDataPathStub, KUidMediaTypeVideo), KErrArgument );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SourcePrimeLL(  )
    {
    // Detach iClip
    iFormatRead->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->SourcePrimeL(), KErrArgument );
    
    // Attach iClip
    iFormatRead->iClip = iRtpDataSourceStub;
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->SourcePrimeL() );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SourcePlayLL(  )
    {
    // Detach iClip
    iFormatRead->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->SourcePlayL(), KErrArgument );
    
    // Attach iClip
    iFormatRead->iClip = iRtpDataSourceStub;
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->SourcePlayL() );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SourcePauseLL(  )
    {
    // Detach iClip
    iFormatRead->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->SourcePauseL(), KErrArgument );
    
    // Attach iClip
    iFormatRead->iClip = iRtpDataSourceStub;
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );
    iFormatRead->iFrameBufferTwo = CMMFDataBuffer::NewL( 5000 );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->SourcePauseL() );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SourceStopLL(  )
    {
    // Detach iClip
    iFormatRead->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->SourceStopL(), KErrArgument );
    
    // Attach iClip
    iFormatRead->iClip = iRtpDataSourceStub;
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );
    iFormatRead->iFrameBufferTwo = CMMFDataBuffer::NewL( 5000 );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->SourceStopL() );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_CreateSourceBufferLL(  )
    {
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );
    TBool bufferRef;
    EUNIT_ASSERT_EQUALS( iFormatRead->iFrameBufferOne, iFormatRead->CreateSourceBufferL( KUidMediaTypeVideo, bufferRef));
    EUNIT_ASSERT_EQUALS(ETrue, bufferRef);
    CMMFBuffer* sinkBuf = NULL;
    EUNIT_ASSERT_EQUALS( iFormatRead->iFrameBufferOne, iFormatRead->CreateSourceBufferL( KUidMediaTypeVideo, *sinkBuf, bufferRef));
    EUNIT_ASSERT_EQUALS(ETrue, bufferRef);
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SourceDataTypeCodeL(  )
    {
    EUNIT_ASSERT_EQUALS(KErrNone, iFormatRead->SetSourceDataTypeCode( TFourCC( ' ','A','N','Y' ), KUidMediaTypeVideo ) );
    EUNIT_ASSERT_EQUALS(TFourCC( ' ','A','N','Y' ), iFormatRead->SourceDataTypeCode( KUidMediaTypeVideo ) );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SetSourceDataTypeCodeL(  )
    {
    // iClip is NULL
    iFormatRead->iClip = NULL;
    EUNIT_ASSERT_EQUALS(iFormatRead->SetSourceDataTypeCode( TFourCC( ' ','A','N','Y' ), KUidMediaTypeVideo ), KErrArgument);
    
    iFormatRead->iClip = iRtpDataSourceStub;
    EUNIT_ASSERT_EQUALS(KErrNone, iFormatRead->SetSourceDataTypeCode( TFourCC( ' ','A','N','Y' ), KUidMediaTypeVideo ) );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SourceThreadLogonL(  )
    {
    MAsyncEventHandler* eventHandler = NULL;
    EUNIT_ASSERT_EQUALS( iFormatRead->SourceThreadLogon( *eventHandler ), KErrNone );
    
    // iClip is NULL
    iFormatRead->iClip = NULL;
    EUNIT_ASSERT_EQUALS(KErrArgument, iFormatRead->SourceThreadLogon( *eventHandler ) );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_SourceThreadLogoffL(  )
    {
    iFormatRead->SourceThreadLogoff();
    iFormatRead->iClip = NULL;
    iFormatRead->SourceThreadLogoff();
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_NegotiateSourceLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->NegotiateSourceL( *iDLDataPathStub ) );
    iFormatRead->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->NegotiateSourceL( *iDLDataPathStub ), KErrArgument );
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_BufferFilledLL(  )
    {
    TRtpRecvHeader test;
    test.iPadding = 1;
    test.iMarker = 1;
    // aBuffer is NULL
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->DataBufferFilledL( NULL, test ), KErrArgument );
    
    // aBuffer->Type() != KUidMmfDataBuffer
    CMMFYUVBuffer* buffer = CMMFYUVBuffer::NewL();
    CleanupStack::PushL( buffer );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->DataBufferFilledL( buffer, test ), KErrArgument );
    CleanupStack::PopAndDestroy( buffer );
    
    // iPayloadBuffer != aBuffer
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->DataBufferFilledL( iBuffer, test ), KErrArgument );
    
    // iClip is NULL
    iFormatRead->iClip = NULL;
    iFormatRead->iPayloadBuffer = CMMFDataBuffer::NewL( 5000 );
    iFormatRead->iSourceBufOwnership = ETrue;
    iFormatRead->iPayloadBuffer->Data().Copy( _L8("somedata") );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ), KErrArgument );
    iFormatRead->iClip = iRtpDataSourceStub;
    
    // iFormatRead->iFrameBufferOne is NULL
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ), KErrArgument );
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );
    
    // iFormatRead->iFrameBufferTwo is NULL
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ), KErrArgument );
    
    iFormatRead->iFrameBufferTwo = CMMFDataBuffer::NewL( 5000 );
    
    // iFrameBufferOne->Status() != EAvailable &&
    // iFrameBufferTwo->Status() != EAvailable 
    iFormatRead->iFrameBufferOne->SetStatus( EUnAvailable );
    iFormatRead->iFrameBufferTwo->SetStatus( EUnAvailable );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ), KErrNotReady );

    iFormatRead->iFrameBufferOne->SetStatus( EAvailable );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ) );
  //  EUNIT_ASSERT(iFormatRead->iFrameBufferOne == iFormatRead->iCurrentBuffer);
    iFormatRead->iFrameBufferOne->SetStatus( EUnAvailable );
    iFormatRead->iFrameBufferTwo->SetStatus( EAvailable );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ) );
  //  EUNIT_ASSERT(iFormatRead->iFrameBufferTwo == iFormatRead->iCurrentBuffer);

    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_BufferFilledL2L(  )
    {
    // aBuffer is NULL
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->BufferFilledL( NULL), KErrArgument );
    
    // aBuffer->Type() != KUidMmfDataBuffer
    CMMFYUVBuffer* buffer = CMMFYUVBuffer::NewL();
    CleanupStack::PushL( buffer );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->BufferFilledL( buffer ), KErrArgument );
    CleanupStack::PopAndDestroy( buffer );
   
    // iPayloadBuffer != aBuffer
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->BufferFilledL( iBuffer ), KErrArgument );
    
   	TMccCodecInfo codec;
    codec.iFourCC.Set(TFourCC(' ','A','M','R') );
    codec.iBitrate = 12200;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iEnableDTX = EFalse;
    codec.iAlgoUsed = 2;
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 20;
    codec.iFrameSize = 20;
    
    TMccCodecInfoBuffer buf( codec );
    // iPayloadBuffer is created
 	iFormatRead->ConfigurePayloadFormatL( buf );
 	
 	// iClip is NULL
 	iFormatRead->iClip = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->BufferFilledL( iFormatRead->iPayloadBuffer ), KErrArgument );
    iFormatRead->iClip = iRtpDataSourceStub; 

    // iFormatRead->iFrameBufferOne is NULL
    delete iFormatRead->iFrameBufferOne;
    iFormatRead->iFrameBufferOne = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->BufferFilledL( iFormatRead->iPayloadBuffer ), KErrArgument );
    iFormatRead->iFrameBufferOne = CMMFDataBuffer::NewL( 5000 );    
    // iFormatRead->iFrameBufferTwo is NULL
    delete iFormatRead->iFrameBufferTwo;
    iFormatRead->iFrameBufferTwo = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->BufferFilledL( iFormatRead->iPayloadBuffer ), KErrArgument );
    
    iFormatRead->iFrameBufferTwo = CMMFDataBuffer::NewL( 5000 );
    iFormatRead->iPayloadBuffer->Data().Copy( _L8("somedata") );
    
    // iFrameBufferOne->Status() != EAvailable &&
    // iFrameBufferTwo->Status() != EAvailable 
    iFormatRead->iFrameBufferOne->SetStatus( EUnAvailable );
    iFormatRead->iFrameBufferTwo->SetStatus( EUnAvailable );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->BufferFilledL( iFormatRead->iPayloadBuffer ), KErrNotReady );

    iFormatRead->iFrameBufferOne->SetStatus( EAvailable );
    iFormatRead->iFrameBufferTwo->SetStatus( EAvailable );

    EUNIT_ASSERT_NO_LEAVE( iFormatRead->BufferFilledL( iFormatRead->iPayloadBuffer ) );
    iFormatRead->iFrameBufferOne->SetStatus( EUnAvailable );
    iFormatRead->iFrameBufferTwo->SetStatus( EAvailable );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->BufferFilledL( iFormatRead->iPayloadBuffer ) );
  
    }

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_FillBufferL_BufferFilledLL()
	{
	TRtpRecvHeader test;
    test.iPadding = 1;
    test.iMarker = 1;
    
	TMccCodecInfo codec;
    codec.iFourCC.Set(TFourCC(' ','A','M','R') );
    codec.iBitrate = 12200;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iEnableDTX = EFalse;
    codec.iAlgoUsed = 2;
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 20;
    codec.iFrameSize = 20;
    
    TMccCodecInfoBuffer buffer( codec );
    
 	iFormatRead->ConfigurePayloadFormatL( buffer );
 	TBool bufferRef;
 	iFormatRead->CreateSourceBufferL( KUidMediaTypeVideo, bufferRef);
 	
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->FillBufferL( iFormatRead->iCurrentBuffer, iDLDataPathStub, KUidMediaTypeVideo) );
    
    iFormatRead->iPayloadBuffer->Data().Copy( _L8("some data") );
    
 	EUNIT_ASSERT_NO_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ) );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ) );

    EUNIT_ASSERT_NO_LEAVE( iFormatRead->FillBufferL( iFormatRead->iCurrentBuffer, iDLDataPathStub, KUidMediaTypeVideo) );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ) );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ), KErrNotReady );

    EUNIT_ASSERT_NO_LEAVE( iFormatRead->FillBufferL( iFormatRead->iCurrentBuffer, iDLDataPathStub, KUidMediaTypeVideo) );
    EUNIT_ASSERT_NO_LEAVE( iFormatRead->FillBufferL( iFormatRead->iCurrentBuffer, iDLDataPathStub, KUidMediaTypeVideo) );
    // 
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->FillBufferL( iFormatRead->iCurrentBuffer, iDLDataPathStub, KUidMediaTypeVideo), KErrArgument );

    EUNIT_ASSERT_NO_LEAVE( iFormatRead->DataBufferFilledL( iFormatRead->iPayloadBuffer, test ) );

	}

void UT_CAnyPayloadFormatRead::UT_CAnyPayloadFormatRead_ConfigurePayloadFormatLL(  )
    {
    
    iFormatRead->iPayloadBuffer = CMMFDataBuffer::NewL( 5000 );
    iFormatRead->iSourceBufOwnership = ETrue;

    TMccCodecInfo codec;
    codec.iFourCC.Set(TFourCC(' ','A','M','R') );
    codec.iBitrate = 12200;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iEnableDTX = EFalse;
    codec.iAlgoUsed = 2;
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 1;
    codec.iFrameSize = 200;
    codec.iType = KUidMediaTypeVideo;
    
    TMccCodecInfoBuffer buffer( codec );

    iFormatRead->ConfigurePayloadFormatL( buffer );
    
    CMMFDataBuffer* payloadBuf = iFormatRead->iPayloadBuffer;
    iFormatRead->ConfigurePayloadFormatL( buffer );
    // Should not change payload buffer if there's no need
    EUNIT_ASSERT( payloadBuf == iFormatRead->iPayloadBuffer );
    
    // Bigger payloadbuffer needed
    codec.iFrameSize = 400;
    buffer = codec;
    TInt payloadBufSize = iFormatRead->iPayloadBuffer->Data().MaxLength();
    iFormatRead->ConfigurePayloadFormatL( buffer );
    EUNIT_ASSERT( payloadBufSize != iFormatRead->iPayloadBuffer->Data().MaxLength() );
 	
 	codec.iFourCC.Set(TFourCC(' ','A','M','R') );
    codec.iBitrate = 12200;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iEnableDTX = EFalse;
    codec.iAlgoUsed = 2;
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 0;
    codec.iType = KUidMediaTypeAudio;
    
    buffer = codec;
    
    // codec.iHwFrameTime = 0;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iFormatRead->ConfigurePayloadFormatL( buffer ), KErrArgument );

	
 	codec.iFourCC.Set(TFourCC(' ','A','M','R') );
    codec.iBitrate = 12200;
    codec.iCodecMode = KAmrCodecModeOctetAlign;
    codec.iPtime = 20;
    codec.iMaxPtime = 200;
    codec.iPayloadType = 96;
    codec.iEnableDTX = EFalse;
    codec.iAlgoUsed = 2;
    codec.iRedundancyCount = 1;
    codec.iRedundantPayload = 99;
    codec.iHwFrameTime = 10;
    codec.iType = KUidMediaTypeAudio;
    
    buffer = codec;

    iFormatRead->ConfigurePayloadFormatL( buffer );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CAnyPayloadFormatRead,
    "CAnypayloadFormatRead test cases",
    "UNIT" )

#ifndef EUNIT_ALLOC_TEST_ON


EUNIT_TEST(
    "NewL - test ",
    "CAnyPayloadFormatRead",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_NewL, Teardown)
    
EUNIT_TEST(
    "CreateClipBufferL - test ",
    "CAnyPayloadFormatRead",
    "CreateClipBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_CreateClipBufferL, Teardown)
    
    
EUNIT_TEST(
    "LastDlPacketCount - test ",
    "CAnyPayloadFormatRead",
    "LastDlPacketCount",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_LastDlPacketCountL, Teardown)
    
EUNIT_TEST(
    "Duration - test ",
    "CAnyPayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_Duration, Teardown)
    

EUNIT_TEST(
    "CancelDlRequest - test ",
    "CAnyPayloadFormatRead",
    "CancelDlRequest",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_CancelDlRequestL, Teardown)

EUNIT_TEST(
    "SendDataToSinkL - test ",
    "CAnyPayloadFormatRead",
    "SendDataToSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_TEST(
    "FillSinkBuffer - test ",
    "CAnyPayloadFormatRead",
    "FillSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_TEST(
    "FillSourceBufferL - test ",
    "CAnyPayloadFormatRead",
    "FillSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_TEST(
    "Streams - test ",
    "CAnyPayloadFormatRead",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_StreamsL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CAnyPayloadFormatRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CAnyPayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_DurationL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CAnyPayloadFormatRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CAnyPayloadFormatRead",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CAnyPayloadFormatRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CAnyPayloadFormatRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CAnyPayloadFormatRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CAnyPayloadFormatRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CAnyPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CAnyPayloadFormatRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CAnyPayloadFormatRead",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CAnyPayloadFormatRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CAnyPayloadFormatRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CAnyPayloadFormatRead",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CAnyPayloadFormatRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_BufferFilledLL, Teardown)
    
EUNIT_TEST(
    "BufferFilledL2 - test ",
    "CAnyPayloadFormatRead",
    "BufferFilledL2",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_BufferFilledL2L, Teardown)

   
EUNIT_TEST(
    "FillBufferL_BufferFilledLL - test ",
    "CAnyPayloadFormatRead",
    "FillBufferL_BufferFilledLL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FillBufferL_BufferFilledLL, Teardown)


EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CAnyPayloadFormatRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_ConfigurePayloadFormatLL, Teardown)

// alloc tests
#else

EUNIT_ALLOC_TEST(
    "NewL - alloc test ",
    "CAnyPayloadFormatRead",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_NewL, Teardown)
   
EUNIT_ALLOC_TEST(
    "CreateClipBufferL - alloc test ",
    "CAnyPayloadFormatRead",
    "CreateClipBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_CreateClipBufferL, Teardown)
     
    
EUNIT_ALLOC_TEST(
    "LastDlPacketCount - alloc test ",
    "CAnyPayloadFormatRead",
    "LastDlPacketCount",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_LastDlPacketCountL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CAnyPayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_Duration, Teardown)
    
EUNIT_ALLOC_TEST(
    "CancelDlRequest - alloc test ",
    "CAnyPayloadFormatRead",
    "CancelDlRequest",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_CancelDlRequestL, Teardown)

EUNIT_ALLOC_TEST(
    "SendDataToSinkL - alloc test ",
    "CAnyPayloadFormatRead",
    "SendDataToSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SendDataToSinkLL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSinkBuffer - alloc test ",
    "CAnyPayloadFormatRead",
    "FillSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FillSinkBufferL, Teardown)

EUNIT_ALLOC_TEST(
    "FillSourceBufferL - alloc test ",
    "CAnyPayloadFormatRead",
    "FillSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FillSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "Streams - alloc test ",
    "CAnyPayloadFormatRead",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_StreamsL, Teardown)

EUNIT_ALLOC_TEST(
    "FrameTimeInterval - alloc test ",
    "CAnyPayloadFormatRead",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FrameTimeIntervalL, Teardown)

EUNIT_ALLOC_TEST(
    "Duration - alloc test ",
    "CAnyPayloadFormatRead",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_DurationL, Teardown)

EUNIT_ALLOC_TEST(
    "SinkDataTypeCode - alloc test ",
    "CAnyPayloadFormatRead",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SinkDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "FillBufferL - alloc test ",
    "CAnyPayloadFormatRead",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FillBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePrimeL - alloc test ",
    "CAnyPayloadFormatRead",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourcePrimeLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePlayL - alloc test ",
    "CAnyPayloadFormatRead",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourcePlayLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourcePauseL - alloc test ",
    "CAnyPayloadFormatRead",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourcePauseLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceStopL - alloc test ",
    "CAnyPayloadFormatRead",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourceStopLL, Teardown)

EUNIT_ALLOC_TEST(
    "CreateSourceBufferL - alloc test ",
    "CAnyPayloadFormatRead",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_CreateSourceBufferLL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceDataTypeCode - alloc test ",
    "CAnyPayloadFormatRead",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SetSourceDataTypeCode - alloc test ",
    "CAnyPayloadFormatRead",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SetSourceDataTypeCodeL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogon - alloc test ",
    "CAnyPayloadFormatRead",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourceThreadLogonL, Teardown)

EUNIT_ALLOC_TEST(
    "SourceThreadLogoff - alloc test ",
    "CAnyPayloadFormatRead",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_SourceThreadLogoffL, Teardown)

EUNIT_ALLOC_TEST(
    "NegotiateSourceL - alloc test ",
    "CAnyPayloadFormatRead",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_NegotiateSourceLL, Teardown)

EUNIT_ALLOC_TEST(
    "BufferFilledL - alloc test ",
    "CAnyPayloadFormatRead",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_BufferFilledLL, Teardown)
  
EUNIT_ALLOC_TEST(
    "BufferFilledL2 - alloc test ",
    "CAnyPayloadFormatRead",
    "BufferFilledL2",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_BufferFilledL2L, Teardown)


EUNIT_ALLOC_TEST(
    "FillBufferL_BufferFilledLL - alloc test ",
    "CAnyPayloadFormatRead",
    "FillBufferL_BufferFilledLL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_FillBufferL_BufferFilledLL, Teardown)


EUNIT_ALLOC_TEST(
    "ConfigurePayloadFormatL - alloc test ",
    "CAnyPayloadFormatRead",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CAnyPayloadFormatRead_ConfigurePayloadFormatLL, Teardown)

#endif // end of EUNIT_ALLOC_TEST_ON

EUNIT_END_TEST_TABLE

//  END OF FILE
