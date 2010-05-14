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
#include "UT_CMccVideoSourceImpl.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // end of EUNIT_ALLOC_TEST_ON

#include <CCMRMediaSink.h>

//  INTERNAL INCLUDES
#include "MccVideoSourceImpl.h"
#include "MccInternalCodecs.h"
#include "Mcculdatapath.h"
#include "mccresourcepool.h"
#include "mcccamerahandler_STUB.h"
#include "mccinternalevents.h"
#include "mccvideosourcewrapper.h"
#include "mmcccodecavc.h"
#include "mccunittestmacros.h"
#include "mccresourcepool_STUB.h"
#include "mcctesteventhandler.h"


#define SET_RECORDER_STATE( state ) \
if ( iSourceImpl && iSourceImpl->iMediaRecorder && iSourceImpl->iMediaRecorder->iMediaRecorder ) \
iSourceImpl->iMediaRecorder->iMediaRecorder->SetCurrentState( state ); \
else User::Leave( KErrNoMemory );      

#define SET_VIDEO_CODEC { \
TMccCodecInfo codecInfo; \
codecInfo.iSdpName = KH263SdpName; \
codecInfo.iFourCC.Set(KMccFourCCIdH263); \
codecInfo.iVideoHeight = 378; \
codecInfo.iVideoWidth = 352; \
codecInfo.iAverageBitrate = 30; \
codecInfo.iMaxBitrate = 122000; \
codecInfo.iCodecMode = 21; \
codecInfo.iBitrate = 7000; \
iSourceImpl->SetVideoCodecL( codecInfo ); }

// CONSTRUCTION
UT_CMccVideoSourceImpl* UT_CMccVideoSourceImpl::NewL()
    {
    UT_CMccVideoSourceImpl* self = UT_CMccVideoSourceImpl::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccVideoSourceImpl* UT_CMccVideoSourceImpl::NewLC()
    {
    UT_CMccVideoSourceImpl* self = new( ELeave ) UT_CMccVideoSourceImpl();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccVideoSourceImpl::~UT_CMccVideoSourceImpl()
    {
    }

// Default constructor
UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl()
    {
    }

// Second phase construct
void UT_CMccVideoSourceImpl::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccVideoSourceImpl::SetupL(  )
    {
    CCMRMediaRecorderStub::SetBehavior( KMccTestRecorderDefaultBehavior, KErrNone );
    
	iSourceImpl = CMccVideoSourceImpl::NewL(0);
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;
    iEventHandler = CMccTestEventHandler::NewL();
    CMccResourcePool* resourcePool = NULL;

    iULDataPathStub = CMccUlDataPath::NewL( iEventHandler, resourcePool, media );
    iMediaBuffer =
    	new ( ELeave ) CCMRMediaBuffer();
    	
    iCameraHandler = CMccCameraHandlerStub::NewL();
    
    iSourceImpl->SetCameraHandler( *iCameraHandler );
    
    iMccResources = CMccResourcePoolStub::NewL();
    }

void UT_CMccVideoSourceImpl::Setup2L(  )
    {
    CCMRMediaRecorderStub::SetBehavior( KMccTestRecorderDefaultBehavior, KErrNone );
    
	iSourceImpl = CMccVideoSourceImpl::NewL(0);
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeAudio;
    iEventHandler = CMccTestEventHandler::NewL();
    CMccResourcePool* resourcePool = NULL;

    iULDataPathStub = CMccUlDataPath::NewL( iEventHandler, resourcePool, media );
    iMediaBuffer =
    	new ( ELeave ) CCMRMediaBuffer();
    	
    iCameraHandler = CMccCameraHandlerStub::NewL();
    }
    
void UT_CMccVideoSourceImpl::Teardown(  )
    {
    delete iMediaBuffer;
    delete iULDataPathStub;
    delete iBuffer;
	delete iSourceImpl;
	delete iCameraHandler;
	delete iMccResources;
	iMccResources = NULL;
	delete iEventHandler;
	iEventHandler = NULL;
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_NewLL(  )
    {
    /*
    CMccVideoSourceImpl* sourceImpl = CMccVideoSourceImpl::NewL();
    delete sourceImpl;
    sourceImpl = NULL;
   */
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_CMccVideoSourceImplL(  )
    {
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetCameraHandleL(  )
    {
	iSourceImpl->SetCameraHandler( *iCameraHandler );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetVideoCodecLL(  )
    {
    TMccCodecInfo codecInfo;
    iSourceImpl->SetVideoCodecL( codecInfo );
    
    // Set config key
    _LIT8( KTestConfigKey, "plaaplaa");
    codecInfo.iConfigKey = KTestConfigKey;
    iSourceImpl->SetVideoCodecL( codecInfo );
    
    EUNIT_ASSERT( iSourceImpl->iConfigKey && iSourceImpl->iConfigKey->Compare( KTestConfigKey ) == 0 );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_GetVideoCodecLL(  )
    {
    TMccCodecInfo codecInfo1, codecInfo2;
    iSourceImpl->SetVideoCodecL( codecInfo1 );
    iSourceImpl->GetVideoCodecL( codecInfo2 );
    EUNIT_ASSERT( TMccCodecInfo::Compare( codecInfo1, codecInfo2 ) );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_GetSupportedVideoCodecsLL(  )
    {
    RArray<TFourCC> fourCCArray;
    CleanupClosePushL(fourCCArray);
	iSourceImpl->GetSupportedVideoCodecsL( fourCCArray );
	EUNIT_ASSERT_EQUALS( fourCCArray.Count(), 0 );
	CleanupStack::PopAndDestroy( &fourCCArray );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetAudioCodecLL(  )
    {
    TMccCodecInfo codecInfo;
    iSourceImpl->SetAudioCodecL( codecInfo );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_GetAudioCodecLL(  )
    {
    TMccCodecInfo codecInfo1, codecInfo2;

    iSourceImpl->GetAudioCodecL( codecInfo1 );
    EUNIT_ASSERT( TMccCodecInfo::Compare( codecInfo1, codecInfo2 ) );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_GetSupportedAudioCodecsLL(  )
    {
    RArray<TFourCC> fourCCArray;
	iSourceImpl->GetSupportedAudioCodecsL( fourCCArray );
	EUNIT_ASSERT_EQUALS( fourCCArray.Count(), 0 );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetAudioEnabledLL(  )
    {
	iSourceImpl->SetAudioEnabledL( ETrue );
	EUNIT_ASSERT( iSourceImpl->AudioEnabledL());
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_AudioEnabledLL(  )
    {
   	iSourceImpl->SetAudioEnabledL( ETrue );
	EUNIT_ASSERT( iSourceImpl->AudioEnabledL());
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetGainLL(  )
    {
	iSourceImpl->SetGainL( 0 );
    EUNIT_ASSERT_EQUALS( iSourceImpl->GainL(), 0 );

    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_GainLL(  )
    {
	iSourceImpl->SetGainL( 5 );
    EUNIT_ASSERT_EQUALS( iSourceImpl->GainL(), 5 ); 
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_MaxGainLL(  )
    {
	iSourceImpl->SetGainL( 5 );
    EUNIT_ASSERT_EQUALS( iSourceImpl->MaxGainL(), 5 ); 
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourceDataTypeCodeL(  )
    {
    TMediaId mediaId;
    EUNIT_ASSERT(iSourceImpl->SourceDataTypeCode( mediaId ) == TFourCC(KMMFFourCCCodeNULL) );
                        
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT(iSourceImpl->SourceDataTypeCode( mediaId ) == TFourCC() );
     
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT(iSourceImpl->SourceDataTypeCode( mediaId ) == TFourCC() );            
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetSourceDataTypeCodeL(  )
    {  
    TMediaId mediaId( KUidMediaTypeVideo );
    TFourCC codec1( KMccFourCCIdH263 );
    EUNIT_ASSERT_EQUALS(
    	iSourceImpl->SetSourceDataTypeCode(codec1, mediaId),
        KErrNotSupported );
    TMccCodecInfo codecInfo;
    codecInfo.iFourCC.Set(KMccFourCCIdH263);
    iSourceImpl->SetAudioCodecL( codecInfo );
    iSourceImpl->SetVideoCodecL( codecInfo );

    EUNIT_ASSERT_EQUALS(
    	iSourceImpl->SetSourceDataTypeCode(codec1, mediaId),
        KErrNone );
    
    mediaId.iMediaType = KUidMediaTypeAudio;
    
    EUNIT_ASSERT_EQUALS(
    	iSourceImpl->SetSourceDataTypeCode(codec1, mediaId),
        KErrNone );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iSourceImpl->BufferEmptiedL(buffer) , KErrNotSupported );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_CanCreateSourceBufferL(  )
    {
    EUNIT_ASSERT( !iSourceImpl->CanCreateSourceBuffer() );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_CreateSourceBufferLL(  )
    {
    TMediaId mediaId;
    TBool bufRef = EFalse;
    EUNIT_ASSERT_SPECIFIC_LEAVE(
    	iSourceImpl->CreateSourceBufferL(mediaId, bufRef), KErrNotSupported );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourceThreadLogonL(  )
    {
    MAsyncEventHandler* eventHandler = NULL;
    EUNIT_ASSERT_EQUALS(
    	iSourceImpl->SourceThreadLogon(*eventHandler), KErrNone );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourceThreadLogoffL(  )
    {
	iSourceImpl->SourceThreadLogoff();
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourcePrimeLL(  )
    {
    // H263
    TMccCodecInfo codecInfo;
    codecInfo.iSdpName = KH263SdpName;
    codecInfo.iFourCC.Set(KMccFourCCIdH263);
    codecInfo.iVideoHeight = 378;
    codecInfo.iVideoWidth = 352;
    codecInfo.iAverageBitrate = 30;
    codecInfo.iMaxBitrate = 80000;
    codecInfo.iBitrate = 3000;

    codecInfo.iCodecMode = 2000;
  
    iSourceImpl->SetVideoCodecL( codecInfo );
    iSourceImpl->SourcePrimeL();
    EUNIT_ASSERT( iSourceImpl->iVideoCodecFourCC->Des().FindF( KH263SdpName ) != KErrNotFound );
    // Encoder uid should not be set in case of H263 is used
    CCMRMediaRecorderStub* stub = 
        reinterpret_cast<CCMRMediaRecorderStub*>( iSourceImpl->iMediaRecorder->iMediaRecorder );
    EUNIT_ASSERT( stub->iEncoderUid == KNullUid );
    EUNIT_ASSERT( stub->iEncapsulation != EDuElementaryStream ); // Not forced in H263 case
    }
    
void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourcePrimeL2L(  )
    {
    SET_VIDEO_CODEC
    
    iSourceImpl->SourcePrimeL();
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourcePrimeL3L()
    {
    iSourceImpl->SourceThreadLogon( *iEventHandler );
    
    SET_VIDEO_CODEC
    
    static_cast<CMccCameraHandlerStub*>(iSourceImpl->iCameraHandler)->iIsCameraReady = EFalse;
    
    // When camera is not yet ready, prepared event is sent and preparing is not
    // done for real (it is done once play is called)
    iEventHandler->iLastEvent.iEventType = KMccEventNone;
    iSourceImpl->SourcePrimeL(); 
    EUNIT_ASSERT( iEventHandler->iLastEvent.iEventType == KMccStreamPrepared );
    
    // Camera creation is waited in autoplay mode (play causes real prime)
    iEventHandler->iLastEvent.iEventType = KMccEventNone;
    iSourceImpl->SetAutoPlayMode( ETrue );
    iSourceImpl->SourcePrimeL();
    EUNIT_ASSERT( !iCameraHandler->iForce );
    EUNIT_ASSERT( iEventHandler->iLastEvent.iEventType == KMccEventNone );
    
    // Check that camera is forcefully created at prime phase if doing config key retrieval
    iEventHandler->iLastEvent.iEventType = KMccEventNone;
    iSourceImpl->SetAutoPlayMode( EFalse );
    iSourceImpl->iConfigKeyRetrieval = ETrue;
    iSourceImpl->SourcePrimeL();
    EUNIT_ASSERT( iCameraHandler->iForce );
    EUNIT_ASSERT( iEventHandler->iLastEvent.iEventType == KMccEventNone );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourcePrimeL4L(  )
    {
    // AVC without any extra info (config key missing, full prepare not yet allowed)
    iSourceImpl->SourceStopL();
    TMccCodecInfo codecInfo;
    codecInfo.iSdpName = KAVCSdpName;
    codecInfo.iFourCC.Set(KMccFourCCIdAVC);
    iSourceImpl->SetVideoCodecL( codecInfo );
    iSourceImpl->SourcePrimeL();
    EUNIT_ASSERT( iSourceImpl->iVideoCodecFourCC == NULL );
    
    // AVC without any extra info (config key exists)
    iSourceImpl->SourceStopL();
    TMccCodecInfo codecInfo2;
    codecInfo2.iSdpName = KAVCSdpName;
    codecInfo2.iFourCC.Set(KMccFourCCIdAVC);
    codecInfo2.iConfigKey = _L8("abcdefg");
    codecInfo2.iEncodingDecodingDevice = TUid::Uid( 0x20001c13 );
    iSourceImpl->SetVideoCodecL( codecInfo2 );
    
    iSourceImpl->SourcePrimeL();
    EUNIT_ASSERT( iSourceImpl->iVideoCodecFourCC != NULL );
    EUNIT_ASSERT( iSourceImpl->iVideoCodecFourCC->Des().FindF( KAVCSdpName ) != KErrNotFound );
    
     // AVC with profile-level-id
    iSourceImpl->SourceStopL();
    TMccCodecInfo codecInfo3;
    codecInfo3.iSdpName = KAVCSdpName;
    codecInfo3.iFourCC.Set(KMccFourCCIdAVC);
    codecInfo3.iBitrateMask = KMccAvcCodecProfileIdBaseline | 
                              KMccAvcCodecProfileIopConstraintSet | 
                              KMccAvcBitrateLevel1;
    codecInfo3.iConfigKey = _L8("abcdefg");
    codecInfo3.iEncodingDecodingDevice = TUid::Uid( 0x20001c13 );
    iSourceImpl->SetVideoCodecL( codecInfo3 );
    
    iSourceImpl->SourcePrimeL();
    EUNIT_ASSERT( iSourceImpl->iVideoCodecFourCC->Des().FindF( KAVCSdpName ) != KErrNotFound );
    
    // AVC with profile-level-id and sprop-parametersets
    iSourceImpl->SourceStopL();
    TMccCodecInfo codecInfo4;
    codecInfo4.iSdpName = KAVCSdpName;
    codecInfo4.iFourCC.Set(KMccFourCCIdAVC);
    codecInfo4.iBitrateMask = KMccAvcCodecProfileIdBaseline | 
                              KMccAvcCodecProfileIopConstraintSet | 
                              KMccAvcBitrateLevel1;
    codecInfo4.iConfigKey = _L8("abcdefg");
    codecInfo4.iEncodingDecodingDevice = TUid::Uid( 0x20001c13 );
    iSourceImpl->SetVideoCodecL( codecInfo4 );
    
    iSourceImpl->SourcePrimeL();
    EUNIT_ASSERT( iSourceImpl->iVideoCodecFourCC->Des().FindF( KAVCSdpName ) != KErrNotFound );
    EUNIT_ASSERT( iSourceImpl->iMediaRecorder->iMediaRecorder != NULL );
    
    // Encoder uid should be set in case of AVC is used
    CCMRMediaRecorderStub* stub = 
        reinterpret_cast<CCMRMediaRecorderStub*>( iSourceImpl->iMediaRecorder->iMediaRecorder );
    EUNIT_ASSERT( stub->iEncoderUid != KNullUid );
    EUNIT_ASSERT( stub->iEncapsulation == EDuElementaryStream ); // Forced in AVC case
    
    // Multiple codecs prepare
    iSourceImpl->SourceStopL();
    iSourceImpl->SourcePrimeL( ETrue );
    EUNIT_ASSERT( iSourceImpl->iMediaRecorder->iMediaRecorder == NULL );
    }
    
void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourcePlayLL(  )
    {
    SET_VIDEO_CODEC
    
    iSourceImpl->SourcePlayL();
    
    SET_RECORDER_STATE( CCMRMediaRecorder::EStatePaused )

    iSourceImpl->SourcePlayL();
    static_cast<CMccCameraHandlerStub*>(iSourceImpl->iCameraHandler)->iIsCameraReady = EFalse;
    iSourceImpl->SourcePlayL();

    SET_RECORDER_STATE( CCMRMediaRecorder::EStateReadyToRecord )

    iSourceImpl->SourcePlayL();
    iSourceImpl->SetAutoPlayMode(ETrue);
    EUNIT_ASSERT(iSourceImpl->AutoPlayMode());
    iSourceImpl->SourcePlayL();

    SET_RECORDER_STATE( CCMRMediaRecorder::EStateNone )
    
    iSourceImpl->SourcePlayL();
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourcePauseLL(  )
    {
    SET_VIDEO_CODEC
    
    iSourceImpl->SourcePauseL();
    EUNIT_ASSERT( iSourceImpl->iMediaRecorder->iMediaRecorder == NULL )
    
    // Test source pause while resume is pending (camera reserving)
    static_cast<CMccCameraHandlerStub*>(iSourceImpl->iCameraHandler)->iIsCameraReady = EFalse;
    
    iSourceImpl->SourcePlayL(); 
    EUNIT_ASSERT( iSourceImpl->AutoPlayMode() );
    EUNIT_ASSERT( iSourceImpl->iMediaRecorder->iMediaRecorder == NULL )
    
    iSourceImpl->SourcePauseL();
    EUNIT_ASSERT( !iSourceImpl->AutoPlayMode() );
    EUNIT_ASSERT( iSourceImpl->iMediaRecorder->iMediaRecorder == NULL )
    
    // Test source pause while resume is pending (preparing recorder)
    static_cast<CMccCameraHandlerStub*>(iSourceImpl->iCameraHandler)->iIsCameraReady = ETrue;
    
    iSourceImpl->SourcePlayL(); 
    EUNIT_ASSERT( iSourceImpl->AutoPlayMode() );
    EUNIT_ASSERT( iSourceImpl->iMediaRecorder->iMediaRecorder != NULL )
    
    iSourceImpl->SourcePauseL();
    EUNIT_ASSERT( !iSourceImpl->AutoPlayMode() );
    EUNIT_ASSERT( iSourceImpl->iMediaRecorder->iMediaRecorder == NULL )
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SourceStopLL(  )
    {
    iSourceImpl->SourceStopL();
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_FillBufferLL(  )
    {
    TMediaId mediaId( KUidMediaTypeAudio );
    EUNIT_ASSERT_SPECIFIC_LEAVE (
    	iSourceImpl->FillBufferL( NULL, NULL, mediaId), KErrNotSupported );
    EUNIT_ASSERT_SPECIFIC_LEAVE (
    	iSourceImpl->FillBufferL( iBuffer, NULL, mediaId), KErrArgument );	
    EUNIT_ASSERT_SPECIFIC_LEAVE (
    	iSourceImpl->FillBufferL( iBuffer, iULDataPathStub, mediaId), KErrNotSupported );	
    mediaId.iMediaType = KUidMediaTypeVideo;
    iSourceImpl->FillBufferL( iBuffer, iULDataPathStub, mediaId );	
    
    // Buffer is already waiting
    _LIT8( KTestOrigData, "orig" );
    _LIT8( KTestNewData, "new" );
    iBuffer->Data().Copy( KTestOrigData() );
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 5 );
    buffer->Data().Copy( KTestNewData() );
    CleanupStack::PushL( buffer );
    iSourceImpl->iVideoQueue.AppendL( buffer );
    CleanupStack::Pop( buffer );
    
    iSourceImpl->FillBufferL( iBuffer, iULDataPathStub, mediaId );	
    EUNIT_ASSERT_EQUALS( iSourceImpl->iVideoQueue.Count(), 0 );
    EUNIT_ASSERT( iBuffer->Data() == KTestNewData() );
    
    // Buffer is already waiting but it is too big for sinkbuffer
    iBuffer->Data().Copy( KTestOrigData() );
    CMMFDescriptorBuffer* buffer2 = CMMFDescriptorBuffer::NewL( 20 );
    buffer2->Data().Copy(_L("aaaaaaaaaaaaaaa"));
    CleanupStack::PushL( buffer2 );
    iSourceImpl->iVideoQueue.AppendL( buffer2 );
    CleanupStack::Pop( buffer2 );
    
    iSourceImpl->FillBufferL( iBuffer, iULDataPathStub, mediaId );
    EUNIT_ASSERT_EQUALS( iSourceImpl->iVideoQueue.Count(), 0 );
    EUNIT_ASSERT( iBuffer->Data() == KTestOrigData() );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_NegotiateSourceLL(  )
    {
    MDataSink* sink = NULL;
    iSourceImpl->NegotiateSourceL( *sink );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_WriteBufferLL(  )
    {
    EUNIT_ASSERT_SPECIFIC_LEAVE(iSourceImpl->WriteBufferL(NULL), KErrArgument);
        
    TMediaId mediaId( KUidMediaTypeVideo );
    iSourceImpl->FillBufferL( iBuffer, iULDataPathStub, mediaId );
    iSourceImpl->WriteBufferL(iMediaBuffer);
    
    // Fill queue
    const TInt KMccMaxVideoQueueSize = 10;
    for ( TInt i = 0; i < KMccMaxVideoQueueSize; i++ )
        {
        iSourceImpl->WriteBufferL(iMediaBuffer);
        EUNIT_ASSERT_EQUALS( iSourceImpl->iVideoQueue.Count(), i+1 );
        }
    
    // Buffer is dropped as queue is full
    iSourceImpl->WriteBufferL(iMediaBuffer);
    EUNIT_ASSERT_EQUALS( iSourceImpl->iVideoQueue.Count(), KMccMaxVideoQueueSize );
    
    // Buffer is dropped as it has too long data
    iSourceImpl->iVideoQueue.ResetAndDestroy();
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 2 );
    CleanupStack::PushL( buffer );
    HBufC8* data = _L8("plaaplaaplaaa").AllocLC();
    CCMRMediaBuffer* mediaBuf = 
        new ( ELeave ) CCMRMediaBuffer( *data, CCMRMediaBuffer::EVideoH263, data->Length(), EFalse, 1000 );
    CleanupStack::PushL( mediaBuf );
    iSourceImpl->FillBufferL( buffer, iULDataPathStub, mediaId );
    TInt bufCount = iSourceImpl->iVideoQueue.Count();
    iSourceImpl->WriteBufferL( mediaBuf );
    EUNIT_ASSERT( iSourceImpl->iVideoQueue.Count() == bufCount );
    
    CleanupStack::PopAndDestroy( mediaBuf );
    CleanupStack::PopAndDestroy( data );
    
    // AVC dec spec info is received
    //
    CMMFDescriptorBuffer* buffer2 = CMMFDescriptorBuffer::NewL( 100 );
    CleanupStack::PushL( buffer2 );
    iSourceImpl->iVideoCodec.iSdpName.Copy( KAVCSdpName );
    
    HBufC8* data2 = _L8("aaaaaaaaaaaaaaaaaaaaa").AllocLC();
    TPtr8 ptrData2( data2->Des() );
    AddConfKeyMarkerL( ptrData2, 0 );
    AddConfKeyMarkerL( ptrData2, 7 );
    CCMRMediaBuffer* mediaBuf2 = 
        new ( ELeave ) CCMRMediaBuffer( *data2, CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo, data2->Length(), EFalse, 1000 );
    CleanupStack::PushL( mediaBuf2 );
    iSourceImpl->FillBufferL( buffer2, iULDataPathStub, mediaId );
    iSourceImpl->WriteBufferL( mediaBuf2 );
    if ( !iSourceImpl->iConfigKeyData )
        {
        User::Leave( KErrNoMemory );
        }
    
    CleanupStack::PopAndDestroy( mediaBuf2 );

    // IFrame is received first time, config data is not put to queue before it
    //
    EUNIT_ASSERT( !iSourceImpl->iFirstIFrameQueued );
    TInt queueSize = iSourceImpl->iVideoQueue.Count();
    TInt keyFrameTimestamp = 11000;
    CCMRMediaBuffer* mediaBuf3 = 
        new ( ELeave ) CCMRMediaBuffer( *data2, CCMRMediaBuffer::EVideoH264Bytestream, data2->Length(), ETrue, keyFrameTimestamp );
    CleanupStack::PushL( mediaBuf3 );
    iSourceImpl->WriteBufferL( mediaBuf3 );
    EUNIT_ASSERT( iSourceImpl->iFirstIFrameQueued );
    EUNIT_ASSERT_EQUALS( iSourceImpl->iVideoQueue.Count(), queueSize + 1 );
    
    
    // Second IFrame is received, config data is put to queue before it.
    // Check also that keyframe info is stored to resourcepool if it exists.
    //
    
    iSourceImpl->iMccResources = iMccResources;
    queueSize = iSourceImpl->iVideoQueue.Count();
    iSourceImpl->WriteBufferL( mediaBuf3 );
    EUNIT_ASSERT_EQUALS( iSourceImpl->iVideoQueue.Count(), queueSize + 2 );
    EUNIT_ASSERT_EQUALS( iMccResources->iKeyFrameTimeStamp, keyFrameTimestamp )
    
    // Next IFrame is received, config data would be put to queue before it but
    // it does not fit to queue
    //
    while ( iSourceImpl->iVideoQueue.Count() < ( KMccMaxVideoQueueSize - 1 ) )
        {
        iSourceImpl->WriteBufferL(iMediaBuffer);
        }
    queueSize = iSourceImpl->iVideoQueue.Count();
    iSourceImpl->WriteBufferL( mediaBuf3 );
    EUNIT_ASSERT_EQUALS( iSourceImpl->iVideoQueue.Count(), queueSize + 1 );
    
    CleanupStack::PopAndDestroy( mediaBuf3 );
    
    CleanupStack::PopAndDestroy( data2 ); 
    CleanupStack::PopAndDestroy( buffer2 ); 
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetVideoFrameSizeL(  )
    {
    TSize size(100,100);
    EUNIT_ASSERT_EQUALS(
    	iSourceImpl->SetVideoFrameSize(size), KErrNone );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetAverageVideoBitRateL(  )
    {
    EUNIT_ASSERT_EQUALS(
    	iSourceImpl->SetAverageVideoBitRate(20), KErrNone);
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetMaxVideoBitRateL(  )
    {
    EUNIT_ASSERT_EQUALS(
    	iSourceImpl->SetMaxVideoBitRate(20), KErrNone);
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetAverageAudioBitRateL(  )
    {
   	EUNIT_ASSERT_EQUALS(
    	iSourceImpl->SetAverageAudioBitRate(20), KErrNone);  
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_MmroPrepareCompleteL(  )
    {
  //  EUNIT_ASSERT( EFalse );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_MmroStateChangeL(  )
    {
  //  EUNIT_ASSERT( EFalse );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_MmroTemporaryErrorL(  )
    {
 //   EUNIT_ASSERT( EFalse );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_MmroFatalErrorL(  )
    {
 //   EUNIT_ASSERT( EFalse );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_CameraReadyL()
    {
    SET_VIDEO_CODEC
    
    iSourceImpl->CameraReady( KErrNone ); 

    SET_RECORDER_STATE( CCMRMediaRecorder::EStatePaused )

    iSourceImpl->CameraReady( KErrNone ); 
    
    iSourceImpl->CameraReady( KErrGeneral ); 

    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_IsStoppedL()
    {
    EUNIT_ASSERT( iSourceImpl->IsStopped() );   
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_RateAdaptationRequestL()
    {   
    TMccCodecInfo codecInfo;
    codecInfo.iSdpName = KH263SdpName;
    codecInfo.iFourCC.Set(KMccFourCCIdH263);
    codecInfo.iVideoHeight = 378;
    codecInfo.iVideoWidth = 352;
    codecInfo.iAverageBitrate = 60000;
    codecInfo.iMaxBitrate = 80000;
    codecInfo.iBitrate = 128000;

    codecInfo.iCodecMode = 2000;
  
    iSourceImpl->SetVideoCodecL( codecInfo );
    iSourceImpl->SourcePrimeL();
   
    TMccRateAdaptationEventData inputData;
    inputData.iRateAdaptationAdvice = 0.9;
    TMccRateAdaptationEventDataPackage inputDatapkg( inputData );
 
    TMccEvent input;
    input.iEventData.Copy(inputDatapkg);
    TMccEvent output;
    
    SET_RECORDER_STATE( CCMRMediaRecorder::EStatePaused )
    
    iSourceImpl->RateAdaptationRequest( input, output ); 
    
    TMccRateAdaptationEventDataPackage eventData;
    eventData.Copy( output.iEventData );
    EUNIT_ASSERT( eventData().iBitrateOriginal != 0 );
    EUNIT_ASSERT( eventData().iBitrateModified < eventData().iBitrateOriginal );
    
    // Suggested bitrate is higher than recorder is able to provide and
    // which is higher than negotiated bitrate
    iSourceImpl->iVideoCodec.iMaxBitrate = 52000;
    iSourceImpl->iMaxNegBitRate = 64000;
    inputData.iRateAdaptationAdvice = 1.2;
    TMccRateAdaptationEventDataPackage inputDatapkg2( inputData );
    input.iEventData.Copy(inputDatapkg2);
    TMccEvent output2;
    
    iSourceImpl->RateAdaptationRequest( input, output2 ); 
    
    eventData.Copy( output2.iEventData );
    EUNIT_ASSERT( eventData().iBitrateOriginal != eventData().iBitrateModified );
    EUNIT_ASSERT( eventData().iBitrateModified == 52000 );
    
    // Suggest such bitrate that framerate will be lowered
    iSourceImpl->iVideoCodec.iMaxBitrate = 64000;
    inputData.iRateAdaptationAdvice = 0.7; // 0.7 * 64000 < KIPULFrameRate10Limit
    inputDatapkg2 = inputData;
    input.iEventData.Copy(inputDatapkg2);
    
    iSourceImpl->RateAdaptationRequest( input, output2 ); 
    
    // Suggest such bitrate that framerate will be lowered even more
    iSourceImpl->iVideoCodec.iMaxBitrate = 64000;
    inputData.iRateAdaptationAdvice = 0.3; // 0.3 * 64000 < KIPULFrameRate5Limit
    inputDatapkg2 = inputData;
    input.iEventData.Copy(inputDatapkg2);
    
    iSourceImpl->RateAdaptationRequest( input, output2 ); 
    
    
    // Cannot use recorder it it is not ready
    SET_RECORDER_STATE( CCMRMediaRecorder::EStateNone )
    
    TMccEvent output3;
    
    iSourceImpl->RateAdaptationRequest( input, output3 ); 
    
    EUNIT_ASSERT( output3.iEventData == KNullDesC8 );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_CallBackTestL()
    {
    iSourceImpl->iAutoPlayMode = ETrue;
    iSourceImpl->MmroPrepareComplete( KErrNone );
    
    UT_CMccVideoSourceImpl_SourcePrimeLL();
    iSourceImpl->MmroPrepareComplete( KErrNone );
    
    iSourceImpl->iVideoCodec.iBitrate = 35000;
    iSourceImpl->MmroPrepareComplete( KErrNone );
    CCMRMediaRecorderStub* stub = 
        reinterpret_cast<CCMRMediaRecorderStub*>( iSourceImpl->iMediaRecorder->iMediaRecorder );
    EUNIT_ASSERT_EQUALS( stub->iVideoBitrate, 35000 );
    if ( stub->iLastFunctionCall != CCMRMediaRecorderStub::ETestSetRateControl )
        {
        // Couldn't proceed to setting phase because of running out of memory
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT_EQUALS( stub->iLastFunctionCall, CCMRMediaRecorderStub::ETestSetRateControl );
    
    
    iSourceImpl->MmroPrepareComplete( KErrGeneral );
    
    iSourceImpl->MmroStateChange( 0, KErrGeneral );
    iSourceImpl->MmroStateChange( CCMRMediaRecorder::EStateNone, KErrNone );
    iSourceImpl->MmroStateChange( CCMRMediaRecorder::EStateOpen, KErrNone );
    iSourceImpl->MmroStateChange( CCMRMediaRecorder::EStatePreparing, KErrNone );
    iSourceImpl->MmroStateChange( CCMRMediaRecorder::EStateRecording, KErrNone );
    iSourceImpl->MmroStateChange( CCMRMediaRecorder::EStatePausing, KErrNone );
    iSourceImpl->MmroStateChange( CCMRMediaRecorder::EStateStopping, KErrNone );
    iSourceImpl->MmroStateChange( CCMRMediaRecorder::EStateReadyToRecord, KErrNone );
    iSourceImpl->MmroStateChange( CCMRMediaRecorder::EStatePaused, KErrNone );
    iSourceImpl->MmroStateChange( 1000, KErrNone );
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_GetConfigKeyTestL()
    {
    TBuf8<KMaxConfigKeyLen> confKey;
    TMccCodecInfo codecInfo;
    codecInfo.iSdpName = KAVCSdpName;
    codecInfo.iFourCC.Set(KMccFourCCIdAVC);
    
    // No key, no camera handler
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSourceImpl->GetConfigKeyL( codecInfo, confKey ), KErrNotReady );
    
    // No key, async not allowed
    iSourceImpl->SetCameraHandler( *iCameraHandler );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSourceImpl->GetConfigKeyL( codecInfo, confKey, EFalse ), KErrNotReady ); 
    
    // Key exists, too short output buf
    _LIT8( KMccTestConfKeyLong, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" );
    delete iSourceImpl->iConfigKey;
    iSourceImpl->iConfigKey = NULL;
    iSourceImpl->iConfigKey = KMccTestConfKeyLong().AllocL();
        
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSourceImpl->GetConfigKeyL( codecInfo, confKey ), KErrArgument ); 
    
    // Key exists, ok output buf
    _LIT8( KMccTestConfKeyShort, "12345678901234567890123456789012345");
    delete iSourceImpl->iConfigKey;
    iSourceImpl->iConfigKey = NULL;
    iSourceImpl->iConfigKey = KMccTestConfKeyShort().AllocL();
    iSourceImpl->GetConfigKeyL( codecInfo, confKey );
    EUNIT_ASSERT( confKey.CompareF( KMccTestConfKeyShort() ) == 0 );
    
    // No key, go for retrieval (retrieval will timeout due mediarecorder stub behavior,
    // succesful case is tested in CMccVideoSourceConfigKeyRetriever tests)
    delete iSourceImpl->iConfigKey;
    iSourceImpl->iConfigKey = NULL;
    TRAPD( err, iSourceImpl->GetConfigKeyL( codecInfo, confKey ) );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    }

void UT_CMccVideoSourceImpl::UT_CMccVideoSourceImpl_SetConfigKeyTestL()
    {
    // Set key, no part1
    TBuf8<KMaxConfigKeyLen> confKey;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSourceImpl->SetConfigKeyL( confKey ), KErrNotFound );

    // Set key, no part2
    confKey.SetLength( 30 );
    AddConfKeyMarkerL( confKey, 0 );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iSourceImpl->SetConfigKeyL( confKey ), KErrNotFound );

    // Set key, both parts ok
    AddConfKeyMarkerL( confKey, 10 );
    iSourceImpl->SetConfigKeyL( confKey );
    EUNIT_ASSERT( iSourceImpl->iConfigKey != NULL );
    
    // Set key, key already exists
    iSourceImpl->SetConfigKeyL( confKey );
    }

void UT_CMccVideoSourceImpl::AddConfKeyMarkerL( TDes8& aBuf, TInt aStartIndex )
    {
    const TInt KTestConfKeyMarkerLen = 3;
    __ASSERT_ALWAYS( aBuf.MaxLength() > aStartIndex + KTestConfKeyMarkerLen, User::Leave( KErrArgument ) );
    aBuf[ aStartIndex ] = 0x00;
    aStartIndex++;
    aBuf[ aStartIndex ] = 0x00;
    aStartIndex++;
    aBuf[ aStartIndex ] = 0x01;
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccVideoSourceImpl,
    "CMccVideoSourceImpl",
    "CMccVideoSourceImpl" )

EUNIT_TEST(
    "NewL - test ",
    "CMccVideoSourceImpl",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_NewLL, Teardown)

EUNIT_TEST(
    "CMccVideoSourceImpl - test ",
    "CMccVideoSourceImpl",
    "CMccVideoSourceImpl",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_CMccVideoSourceImplL, Teardown)

EUNIT_TEST(
    "SetCameraHandle - test ",
    "CMccVideoSourceImpl",
    "SetCameraHandle",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetCameraHandleL, Teardown)

EUNIT_TEST(
    "SetVideoCodecL - test ",
    "CMccVideoSourceImpl",
    "SetVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetVideoCodecLL, Teardown)

EUNIT_TEST(
    "GetVideoCodecL - test ",
    "CMccVideoSourceImpl",
    "GetVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_GetVideoCodecLL, Teardown)

EUNIT_TEST(
    "GetSupportedVideoCodecsL - test ",
    "CMccVideoSourceImpl",
    "GetSupportedVideoCodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_GetSupportedVideoCodecsLL, Teardown)

EUNIT_TEST(
    "SetAudioCodecL - test ",
    "CMccVideoSourceImpl",
    "SetAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetAudioCodecLL, Teardown)

EUNIT_TEST(
    "GetAudioCodecL - test ",
    "CMccVideoSourceImpl",
    "GetAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_GetAudioCodecLL, Teardown)

EUNIT_TEST(
    "GetSupportedAudioCodecsL - test ",
    "CMccVideoSourceImpl",
    "GetSupportedAudioCodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_GetSupportedAudioCodecsLL, Teardown)

EUNIT_TEST(
    "SetAudioEnabledL - test ",
    "CMccVideoSourceImpl",
    "SetAudioEnabledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetAudioEnabledLL, Teardown)

EUNIT_TEST(
    "AudioEnabledL - test ",
    "CMccVideoSourceImpl",
    "AudioEnabledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_AudioEnabledLL, Teardown)

EUNIT_TEST(
    "SetGainL - test ",
    "CMccVideoSourceImpl",
    "SetGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetGainLL, Teardown)

EUNIT_TEST(
    "GainL - test ",
    "CMccVideoSourceImpl",
    "GainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_GainLL, Teardown)

EUNIT_TEST(
    "MaxGainL - test ",
    "CMccVideoSourceImpl",
    "MaxGainL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_MaxGainLL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccVideoSourceImpl",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccVideoSourceImpl",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccVideoSourceImpl",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccVideoSourceImpl",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccVideoSourceImpl",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccVideoSourceImpl",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CMccVideoSourceImpl",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "SourcePrimeL - test ",
    "CMccVideoSourceImpl",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePrime2L - test ",
    "CMccVideoSourceImpl",
    "SourcePrime2L",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourcePrimeL2L, Teardown)
    
EUNIT_TEST(
    "SourcePrime3L - test ",
    "CMccVideoSourceImpl",
    "SourcePrime3L",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourcePrimeL3L, Teardown)
    
EUNIT_TEST(
    "SourcePrime4L - test ",
    "CMccVideoSourceImpl",
    "SourcePrime4L",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourcePrimeL4L, Teardown)
      
EUNIT_TEST(
    "SourcePlayL - test ",
    "CMccVideoSourceImpl",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CMccVideoSourceImpl",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CMccVideoSourceImpl",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SourceStopLL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccVideoSourceImpl",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_FillBufferLL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CMccVideoSourceImpl",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "WriteBufferL - test ",
    "CMccVideoSourceImpl",
    "WriteBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_WriteBufferLL, Teardown)

EUNIT_TEST(
    "SetVideoFrameSize - test ",
    "CMccVideoSourceImpl",
    "SetVideoFrameSize",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetVideoFrameSizeL, Teardown)

EUNIT_TEST(
    "SetAverageVideoBitRate - test ",
    "CMccVideoSourceImpl",
    "SetAverageVideoBitRate",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetAverageVideoBitRateL, Teardown)

EUNIT_TEST(
    "SetMaxVideoBitRate - test ",
    "CMccVideoSourceImpl",
    "SetMaxVideoBitRate",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetMaxVideoBitRateL, Teardown)

EUNIT_TEST(
    "SetAverageAudioBitRate - test ",
    "CMccVideoSourceImpl",
    "SetAverageAudioBitRate",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetAverageAudioBitRateL, Teardown)

EUNIT_TEST(
    "MmroPrepareComplete - test ",
    "CMccVideoSourceImpl",
    "MmroPrepareComplete",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_MmroPrepareCompleteL, Teardown)

EUNIT_TEST(
    "MmroStateChange - test ",
    "CMccVideoSourceImpl",
    "MmroStateChange",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_MmroStateChangeL, Teardown)

EUNIT_TEST(
    "MmroTemporaryError - test ",
    "CMccVideoSourceImpl",
    "MmroTemporaryError",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_MmroTemporaryErrorL, Teardown)

EUNIT_TEST(
    "MmroFatalError - test ",
    "CMccVideoSourceImpl",
    "MmroFatalError",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_MmroFatalErrorL, Teardown)
    
EUNIT_TEST(
    "CameraReadyL - test ",
    "CMccVideoSourceImpl",
    "CameraReadyL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_CameraReadyL, Teardown)
    
EUNIT_TEST(
    "IsStoppedL - test ",
    "CMccVideoSourceImpl",
    "IsStoppedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_IsStoppedL, Teardown)

EUNIT_TEST(
    "RateAdaptationRequestL - test ",
    "CMccVideoSourceImpl",
    "RateAdaptationRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_RateAdaptationRequestL, Teardown)

EUNIT_TEST(
    "CallBackTestL - test ",
    "CMccVideoSourceImpl",
    "CallBackTestL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_CallBackTestL, Teardown)

EUNIT_TEST(
    "GetConfigKeyTestL - test ",
    "CMccVideoSourceImpl",
    "GetConfigKeyTestL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccVideoSourceImpl_GetConfigKeyTestL, Teardown)

EUNIT_TEST(
    "SetConfigKeyTestL - test ",
    "CMccVideoSourceImpl",
    "SetConfigKeyTestL",
    "FUNCTIONALITY",
    SetupL, UT_CMccVideoSourceImpl_SetConfigKeyTestL, Teardown)
    

        
EUNIT_END_TEST_TABLE

//  END OF FILE
