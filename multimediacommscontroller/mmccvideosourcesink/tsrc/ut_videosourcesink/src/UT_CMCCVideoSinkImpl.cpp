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
#include <mmf/common/mmfbase.h>



#include "UT_CMccVideoSinkImpl.h"
#include "MMccInterfaceDef.h"
#include "MccInternalCodecs.h"
#include "rtpheader.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <videoplayer2.h>

#ifdef EUNIT_ALLOC_TEST_ON
	#include <digia/eunit/CEunitAllocTestCaseDecorator.h>  //for using EUNIT_ALLOC_TEST
#endif // end of EUNIT_ALLOC_TEST_ON

//  INTERNAL INCLUDES
#include "MccVideoSinkImpl.h"
#include "Mcctesteventhandler.h"
#include "mmcccodecamr.h"
#include "mccvideosinkuser.h"
#include "ws_stubs_helper.h"


const TInt KVideoSinkTestFramerateNormal = 15;
const TInt KVideoSinkTestFramerateLow = 12;

const TInt KTestRealTimeModeJitBufThreshold = 50;
const TInt KTestNormalModeJitBufThreshold = 100;

// MACROS
#define DO_EMPTY_BUFFER_L( b, s, m, h ) { \
TTime temporaryTime; \
temporaryTime.DayNoInYear( TTime( iCurrentTime ) ); \
iSinkImpl->EmptyBufferL( b, s, m, h ); \
iCurrentTime += TTimeIntervalMicroSeconds( iEmptyBufferInterval ); }

// CONSTRUCTION
UT_CMccVideoSinkImpl* UT_CMccVideoSinkImpl::NewL()
    {
    UT_CMccVideoSinkImpl* self = UT_CMccVideoSinkImpl::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccVideoSinkImpl* UT_CMccVideoSinkImpl::NewLC()
    {
    UT_CMccVideoSinkImpl* self = new( ELeave ) UT_CMccVideoSinkImpl();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccVideoSinkImpl::~UT_CMccVideoSinkImpl()
    {
    }

// Default constructor
UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl()
    {
    }

// Second phase construct
void UT_CMccVideoSinkImpl::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMccVideoSinkImpl::DoSetupL( TInt aJitbufThreshold )
    {
    iJitterBufferThreshold = aJitbufThreshold;
    
    iSinkImpl->SetCurrentUser( iVideoUser );
    TMccCodecInfo codec;
    codec.iSdpName = KH263SdpName;
	codec.iFourCC.Set(KMccFourCCIdH263);
	codec.iCodecMode = 2000;
	codec.iBitrate = 50000;
	codec.iMaxBitrate = 72000;
	codec.iPayloadType = 97;
	codec.iJitterBufThreshold = iJitterBufferThreshold;
	iSinkImpl->FormatVideoSDPInfoL( codec );
	
	iSinkImpl->RemoteSetupL();
		
	iSinkImpl->StartL();
    }

void UT_CMccVideoSinkImpl::SetupL(  )
    {
	TMccVideoSinkSetting settings;// = settingsBuf();
	settings.iAudioPayloadType = 106;
    settings.iVideoPayloadType = 96;
    settings.iDeviceIndex = 0;
    settings.iLocation.iX = 0;
    settings.iLocation.iY = 83;
    settings.iSize.iWidth = 352;
    settings.iSize.iHeight = 378;
    settings.iIAPId = 2;
    settings.iPriority.iPref = EMdaPriorityPreferenceNone;
    settings.iPriority.iPriority = 0;
    settings.iPriority.iState = EMMFStateIdle;
    settings.iRotation = EMccNone;
    settings.iVideoFrameRate = 15;

	iSinkImpl = CMccVideoSinkImpl::NewL( settings, 0 );
	
	iAudioUser = CMccTestEventHandler::NewL();
	iVideoUser = CMccTestEventHandler::NewL();
	
	iSinkImpl->SinkThreadLogon( *iAudioUser );
	iSinkImpl->SinkThreadLogon( *iVideoUser );
	
	iSinkImpl->SetCurrentUser( iVideoUser );
	
	iCurrentTime = TTime( 1000 );
	iEmptyBufferInterval = 18000;
	
	iJitterBufferThreshold = KTestRealTimeModeJitBufThreshold;
	// Do sinkthread logon for two users
    }

void UT_CMccVideoSinkImpl::Setup2L()
    {
    SetupL();
    
    DoSetupL( KTestNormalModeJitBufThreshold );
    }

void UT_CMccVideoSinkImpl::Setup3L()
    {
    SetupL();
    
    DoSetupL( KTestRealTimeModeJitBufThreshold );
    }
        
void UT_CMccVideoSinkImpl::Teardown(  )
    {
	delete iSinkImpl;
    delete iAudioUser;
	delete iVideoUser;
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_NewLL(  )
    {
   	TMccVideoSinkSetting settings;// = settingsBuf();
	settings.iAudioPayloadType = 106;
    settings.iVideoPayloadType = 96;
    settings.iDeviceIndex = 0;
    settings.iLocation.iX = 0;
    settings.iLocation.iY = 83;
    settings.iSize.iWidth = 352;
    settings.iSize.iHeight = 378;
    settings.iIAPId = 2;
    settings.iPriority.iPref = EMdaPriorityPreferenceNone;
    settings.iPriority.iPriority = 0;
    settings.iPriority.iState = EMMFStateIdle;
    settings.iRotation = EMccNone;
    settings.iVideoFrameRate = 0; // Zero framerate is not taken in use

    CMccVideoSinkImpl* sink = NULL;
	TRAPD( err, sink = CMccVideoSinkImpl::NewL( settings, 0 ) );
	if ( err == KErrNotReady )
	    {
	    // Packet sink creation will not leave in case of memory running out but
	    // video sink leaves with KErrNotReady in that case
	    User::Leave( KErrNoMemory );
	    }
	User::LeaveIfError( err );
	CleanupStack::PushL( sink );
	EUNIT_ASSERT( sink->GetVideoFrameRate() != settings.iVideoFrameRate );
	CleanupStack::PopAndDestroy( sink );
	
	settings.iVideoFrameRate = 12;
	CMccVideoSinkImpl* sink2 = NULL;
	TRAP( err, sink2 = CMccVideoSinkImpl::NewL( settings, 0 ) );
	if ( err == KErrNotReady )
	    {
	    // Packet sink creation will not leave in case of memory running out but
	    // video sink leaves with KErrNotReady in that case
	    User::Leave( KErrNoMemory );
	    }
	User::LeaveIfError( err );
	CleanupStack::PushL( sink2 );
	EUNIT_ASSERT( sink2->GetVideoFrameRate() == settings.iVideoFrameRate );
	CleanupStack::PopAndDestroy( sink2 );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_SinkThreadLogoffL()
    {
    iSinkImpl->SetCurrentUser( iVideoUser );
    
    // Stopping not yet allowed as other users exist
    EUNIT_ASSERT( !iSinkImpl->SinkThreadLogoff() );
    
    iSinkImpl->SetCurrentUser( iAudioUser );
    
    // Stopping allowed as other users does exist
    EUNIT_ASSERT( iSinkImpl->SinkThreadLogoff() );
    }
    
void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_OpenLL(  )
    {
    // Any of the users are not yet ready, opening is not yet started
    iSinkImpl->OpenL();
    EUNIT_ASSERT( iSinkImpl->iState != CMccVideoSinkImpl::EOpening );
    
    // One of the users is not yet ready, opening is not yet started 
    iSinkImpl->SetCurrentUser( iVideoUser );
    TMccCodecInfo codec;
    codec.iSdpName = KH263SdpName;
	codec.iFourCC.Set(KMccFourCCIdH263);
	codec.iCodecMode = 2000;
	codec.iBitrate = 50000;
	codec.iMaxBitrate = 72000;
	codec.iPayloadType = 97;
	codec.iJitterBufThreshold = iJitterBufferThreshold;
	iSinkImpl->FormatVideoSDPInfoL( codec );
	
	iSinkImpl->OpenL();
    EUNIT_ASSERT( iSinkImpl->iState != CMccVideoSinkImpl::EOpening );
	
    // All users ready, opening is started
    iSinkImpl->SetCurrentUser( iAudioUser );
	TMccCodecInfo codec2;
	codec2.iSdpName = KAMRSdpName;
	codec2.iFourCC.Set(KMccFourCCIdAMRNB);
	codec2.iCodecMode = KAmrCodecModeOctetAlign;
	codec2.iMaxBitrate = 35000;
	codec2.iPtime = 20;
	codec2.iBitrateMask = KMccAllowedAmrNbBitrateAll;
	codec2.iPayloadType = 96;
	codec2.iJitterBufThreshold = iJitterBufferThreshold;
	iSinkImpl->FormatAudioSDPInfoL( codec2 );
	
	iSinkImpl->OpenL();
    EUNIT_ASSERT( iSinkImpl->iState == CMccVideoSinkImpl::EOpening );
	
    // Simulate completion
    iSinkImpl->MvpuoOpenComplete( KErrNone );
    EUNIT_ASSERT( iSinkImpl->iState == CMccVideoSinkImpl::EPrepared );
    
    // Simulate completion error
    iSinkImpl->MvpuoOpenComplete( KErrGeneral );

    // Try to open when already opening
    iSinkImpl->iState = CMccVideoSinkImpl::EOpened;
    iSinkImpl->OpenL();
    EUNIT_ASSERT( iSinkImpl->iState == CMccVideoSinkImpl::EOpening );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_PrepareL(  )
    {
    iSinkImpl->Prepare();
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_FormatSDPInfoL()
	{
	iSinkImpl->SetCurrentUser( iAudioUser );
	TMccCodecInfo codec;
	codec.iSdpName = KAMRSdpName;
	codec.iFourCC.Set(KMccFourCCIdAMRNB);
	codec.iCodecMode = KAmrCodecModeOctetAlign;
	codec.iMaxBitrate = 35000;
	codec.iPtime = 20;
	codec.iBitrateMask = KMccAllowedAmrNbBitrateAll;
	codec.iPayloadType = 96;
	codec.iJitterBufThreshold = iJitterBufferThreshold;
	iSinkImpl->FormatAudioSDPInfoL( codec );
   
    iSinkImpl->SetCurrentUser( iVideoUser );
    TMccCodecInfo codec2;
    codec2.iSdpName = KH263SdpName;
	codec2.iFourCC.Set(KMccFourCCIdH263);
	codec2.iCodecMode = 2000;
	codec2.iBitrate = 50000;
	codec2.iMaxBitrate = 72000;
	codec2.iPayloadType = 97;
	codec2.iJitterBufThreshold = iJitterBufferThreshold;
	iSinkImpl->FormatVideoSDPInfoL( codec2 );
	}
	
void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_RemoteSetupL(  )
    {
    iSinkImpl->RemoteSetupL();
    
    UT_CMccVideoSinkImpl_FormatSDPInfoL();
    
    // Add one more video user
    CMccTestEventHandler* videoUser = CMccTestEventHandler::NewL();
    CleanupStack::PushL( videoUser );
	User::LeaveIfError( iSinkImpl->SinkThreadLogon( *videoUser ) );
	iSinkImpl->SetCurrentUser( videoUser );
	TMccCodecInfo videoCodec;
    videoCodec.iSdpName = KH263SdpName;
	videoCodec.iFourCC.Set(KMccFourCCIdH263);
	videoCodec.iCodecMode = 2000;
	videoCodec.iBitrate = 80000;
	videoCodec.iMaxBitrate = 72000;
	videoCodec.iPayloadType = 97;
	iSinkImpl->FormatVideoSDPInfoL( videoCodec );
    
    iSinkImpl->RemoteSetupL();
    
    // Video users should get the same stream id
    TUint32 streamId( KMaxTUint32 );
    TInt videoUserCount( 0 );
    for ( TInt i = 0; i < iSinkImpl->iUsers.Count(); i++ )
        {
        if ( iSinkImpl->iUsers[ i ]->iMediaType == KUidMediaTypeVideo )
            {
            videoUserCount++;
            if ( streamId == KMaxTUint32 )
                {
                streamId = iSinkImpl->iUsers[ i ]->iStreamId;
                }
            else
                {
                EUNIT_ASSERT_EQUALS( iSinkImpl->iUsers[ i ]->iStreamId, streamId );
                }
            }
        }
    EUNIT_ASSERT_EQUALS( videoUserCount, 2 );
    
    // Try to setup with different audio bitrates
    iSinkImpl->SetCurrentUser( iAudioUser );
	TMccCodecInfo codec;
	codec.iFourCC.Set(KMccFourCCIdAMRNB);
	codec.iCodecMode = KAmrCodecModeOctetAlign;
	codec.iMaxBitrate = 35000;
	codec.iPtime = 20;
	codec.iBitrateMask = KMccAllowedAmrNbBitrate475;
	codec.iPayloadType = 96;
	iSinkImpl->FormatAudioSDPInfoL( codec ); 
	iSinkImpl->RemoteSetupL();
	
	codec.iBitrateMask = KMccAllowedAmrNbBitrate515;
	iSinkImpl->FormatAudioSDPInfoL( codec ); 
	iSinkImpl->RemoteSetupL();
	
	codec.iBitrateMask = KMccAllowedAmrNbBitrate590;
	iSinkImpl->FormatAudioSDPInfoL( codec ); 
	iSinkImpl->RemoteSetupL();
	
	codec.iBitrateMask = KMccAllowedAmrNbBitrate670;
	iSinkImpl->FormatAudioSDPInfoL( codec ); 
	iSinkImpl->RemoteSetupL();
	
	codec.iBitrateMask = KMccAllowedAmrNbBitrate740;
	iSinkImpl->FormatAudioSDPInfoL( codec ); 
	iSinkImpl->RemoteSetupL();
	
	codec.iBitrateMask = KMccAllowedAmrNbBitrate795;
	iSinkImpl->FormatAudioSDPInfoL( codec ); 
	iSinkImpl->RemoteSetupL();
	
	codec.iBitrateMask = KMccAllowedAmrNbBitrate102;
	iSinkImpl->FormatAudioSDPInfoL( codec ); 
	iSinkImpl->RemoteSetupL();
	
	codec.iBitrateMask = KMccAllowedAmrNbBitrate122;
	iSinkImpl->FormatAudioSDPInfoL( codec ); 
	iSinkImpl->RemoteSetupL();
	
	// Try to setup with different video bitrates
	iSinkImpl->SetCurrentUser( iVideoUser );
    TMccCodecInfo codec2;
    codec2.iSdpName = KH263SdpName;
	codec2.iFourCC.Set(KMccFourCCIdH263);
	codec2.iCodecMode = 2000;
	codec2.iBitrate = 80000;
	codec2.iMaxBitrate = 72000;
	codec.iPayloadType = 97;
	iSinkImpl->FormatVideoSDPInfoL( codec2 );
	iSinkImpl->RemoteSetupL();
   
    iSinkImpl->iVideoPlayer->Stop();	
	iSinkImpl->iVideoPlayer->Close();	
    delete iSinkImpl->iVideoPlayer;
    iSinkImpl->iVideoPlayer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iSinkImpl->RemoteSetupL(), KErrNotReady );   
    
    iSinkImpl->SetCurrentUser( videoUser );
    iSinkImpl->SinkThreadLogoff();
    CleanupStack::PopAndDestroy( videoUser );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_StartL(  )
    {
	iSinkImpl->StartL();
	EUNIT_ASSERT_EQUALS( iSinkImpl->iState, CMccVideoSinkImpl::EPlaying );
	iSinkImpl->StartL();
	EUNIT_ASSERT_EQUALS( iSinkImpl->iState, CMccVideoSinkImpl::EPlaying );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_StopL(  )
    {
	iSinkImpl->Stop();
	EUNIT_ASSERT_EQUALS( iSinkImpl->iState, CMccVideoSinkImpl::EStopped );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_PauseLL(  )
    {
   	iSinkImpl->StartL();
	iSinkImpl->PauseL();
	EUNIT_ASSERT_EQUALS( iSinkImpl->iState, CMccVideoSinkImpl::EPaused );
	iSinkImpl->Stop();
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_CloseL(  )
    {
    iSinkImpl->Close();
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_SetDisplayWindowLL(  )
    {
    EUNIT_ASSERT( iSinkImpl->iVideoPlayer->iTestWindow == NULL );
    iSinkImpl->SetDisplayWindowL(
    	iSinkImpl->iRwSession,
    	*( iSinkImpl->iDev ),
    	*( iSinkImpl->iRw ) );
    EUNIT_ASSERT( iSinkImpl->iVideoPlayer->iTestWindow == iSinkImpl->iRw );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_PriorityLL(  )
    {
    TInt priority = 0;
	TInt prioPreference = EMdaPriorityPreferenceNone;
	iSinkImpl->Priority( priority,  prioPreference );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_SetPriorityLL(  )
    {
    TInt priority = 0;
	TInt prioPreference = EMdaPriorityPreferenceNone;
	iSinkImpl->SetPriority(2, EMdaPriorityPreferenceQuality );
	iSinkImpl->Priority( priority,  prioPreference );
    EUNIT_ASSERT_EQUALS( priority, 2 );
    EUNIT_ASSERT_EQUALS( prioPreference, EMdaPriorityPreferenceQuality );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_VolumeL(  )
    {
    TInt volumn = iSinkImpl->Volume();
	iSinkImpl->iVideoPlayer->Stop();	
	iSinkImpl->iVideoPlayer->Close();	
    delete iSinkImpl->iVideoPlayer;
    iSinkImpl->iVideoPlayer = NULL;
    EUNIT_ASSERT_EQUALS( iSinkImpl->Volume(), KErrArgument );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_MaxVolumeL(  )
    {
	TInt maxVolumn = iSinkImpl->MaxVolume();
	iSinkImpl->iVideoPlayer->Stop();	
	iSinkImpl->iVideoPlayer->Close();	
    delete iSinkImpl->iVideoPlayer;
    iSinkImpl->iVideoPlayer = NULL;
    EUNIT_ASSERT_EQUALS( iSinkImpl->MaxVolume(), KErrArgument );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_SetVolumeLL(  )
    {
    iSinkImpl->SetVolumeL( 20 );
    EUNIT_ASSERT_EQUALS( iSinkImpl->iVolume, 20 ); 
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_BalanceL(  )
    {
    TInt balance = iSinkImpl->Balance();
    iSinkImpl->iVideoPlayer->Stop();	
	iSinkImpl->iVideoPlayer->Close();	
    delete iSinkImpl->iVideoPlayer;
    iSinkImpl->iVideoPlayer = NULL;
    EUNIT_ASSERT_EQUALS( iSinkImpl->Balance(), KErrArgument );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_SetBalanceLL(  )
    {
    iSinkImpl->SetBalanceL( 20 );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_RegisterForVideoLoadingNotificationL(  )
    {
    // Test loading started/completed in realtime mode
    //
    
    iAudioUser->iLastEvent.iEventType = KMccEventNone;
    iVideoUser->iLastEvent.iEventType = KMccEventNone;
        
	iSinkImpl->RegisterForVideoLoadingNotification(*this);
	
	// In real-time mode, loading started and completed events coming from helix
	// are ignored. Instead those events are simulated when MCC videojitterbuffer
	// states change.
	iSinkImpl->MvloLoadingStarted();
	
	EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType != KMccStreamBuffering );
	EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType != KMccStreamBuffering );
	EUNIT_ASSERT( iSinkImpl->iSimulatedBufferingEventSent == EFalse );
	
	iSinkImpl->MvloLoadingComplete();
	
	EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType != KMccStreamPlaying );
	EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType != KMccStreamPlaying );
	EUNIT_ASSERT( iSinkImpl->iSimulatedStreamingEventSent == EFalse );
	
	// Loading started simulated when MCC jitterbuffer starts buffering
	iSinkImpl->DoMvloLoadingStarted( ETrue );
	EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType == KMccStreamBuffering );
	EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType == KMccStreamBuffering );
	EUNIT_ASSERT( iSinkImpl->iSimulatedBufferingEventSent == ETrue );
	
	// Buffering events are not further simualted until loading completion has occured
	iAudioUser->iLastEvent.iEventType = KMccEventNone;
	iVideoUser->iLastEvent.iEventType = KMccEventNone;
	iSinkImpl->DoMvloLoadingStarted( ETrue );
	EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType == KMccEventNone );
	EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType == KMccEventNone );
	EUNIT_ASSERT( iSinkImpl->iSimulatedBufferingEventSent == ETrue );
	
	// Loading completed simulated when MCC jitterbuffer completes buffering
	iSinkImpl->DoMvloLoadingComplete( ETrue );
	EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType == KMccStreamPlaying );
	EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType == KMccStreamPlaying );
	EUNIT_ASSERT( iSinkImpl->iSimulatedStreamingEventSent == ETrue );
	
	// Streaming events are not further simualted until loading started has occured
	iAudioUser->iLastEvent.iEventType = KMccEventNone;
	iVideoUser->iLastEvent.iEventType = KMccEventNone;
	iSinkImpl->DoMvloLoadingComplete( ETrue );
	EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType == KMccEventNone );
	EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType == KMccEventNone );
	EUNIT_ASSERT( iSinkImpl->iSimulatedStreamingEventSent == ETrue );
	
	// Now buffering event can be again simulated
	iSinkImpl->DoMvloLoadingStarted( ETrue );
	EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType == KMccStreamBuffering );
	EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType == KMccStreamBuffering );
	EUNIT_ASSERT( iSinkImpl->iSimulatedBufferingEventSent == ETrue );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_RegisterForVideoLoadingNotification2L(  )
    {
    // Test loading started/completed in normal mode
    //
    UT_CMccVideoSinkImpl_FormatSDPInfoL();
    iSinkImpl->RemoteSetupL();
        
    iAudioUser->iLastEvent.iEventType = KMccEventNone;
    iVideoUser->iLastEvent.iEventType = KMccEventNone;
        
    iSinkImpl->RegisterForVideoLoadingNotification(*this);
    
    // In normal mode, loading started and completed events coming from helix
    // are passed as is. Simulated events are ignored.
    iSinkImpl->MvloLoadingStarted();
    
    EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType == KMccStreamBuffering );
    EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType == KMccStreamBuffering );
    
    iAudioUser->iLastEvent.iEventType = KMccEventNone;
    iVideoUser->iLastEvent.iEventType = KMccEventNone; 
    iSinkImpl->MvloLoadingStarted();
    EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType  == KMccStreamBuffering );
    EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType  == KMccStreamBuffering );
    
    iSinkImpl->MvloLoadingComplete();
    EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType  == KMccStreamPlaying );
    EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType  == KMccStreamPlaying );
       
    // Simulated event ignored
    iAudioUser->iLastEvent.iEventType = KMccEventNone;
    iVideoUser->iLastEvent.iEventType = KMccEventNone; 
    iSinkImpl->DoMvloLoadingStarted( ETrue );
    EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType == KMccEventNone );
    
    // Simulated event ignored
    iSinkImpl->DoMvloLoadingComplete( ETrue );
    EUNIT_ASSERT( iAudioUser->iLastEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iVideoUser->iLastEvent.iEventType == KMccEventNone );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_VersionL(  )
    {
    TVersion version = iSinkImpl->Version();
    EUNIT_ASSERT_EQUALS ( version.iMajor, 0 );
    EUNIT_ASSERT_EQUALS ( version.iMinor, 1 );
    EUNIT_ASSERT_EQUALS ( version.iBuild, 0 );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_EmptyBufferL()
	{
	UT_CMccVideoSinkImpl_FormatSDPInfoL();
	iSinkImpl->RemoteSetupL();
		
	iSinkImpl->StartL();
	
	// Test first passing ok buffers
	TInt time( 1000 );
	TInt timeIncrement( 5000 );
    
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
	CleanupStack::PushL( buffer );
    buffer->Data().Copy(_L("aaaaaaaaaa"));
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( 1 );
	
	TRtpRecvHeader header;
    header.iPayloadType = 97;
    
    TMediaId mediaId( KUidMediaTypeVideo );
    
    TInt sequenceNum( 1 );
	for ( TInt i = 0; i < KMccTimeStampDifferenceTreshold + 2; i++ )
	    {
	    
	    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	    buffer->SetFrameNumber( sequenceNum++ );
	    
	    header.iTimestamp = buffer->TimeToPlay().Int64();
	    header.iMarker = buffer->LastBuffer();
	    header.iSeqNum = buffer->FrameNumber();
	    
	    DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header );

	    EUNIT_ASSERT( header.iTimestamp == time );
	    
	    time += timeIncrement;
	    }
	
	// Try too small buffer
	buffer->Data().Copy(_L("a"));
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
	header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	time += timeIncrement;
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    
    // Test timestamp skew -> reset needed
    time = 0;
    buffer->Data().Copy(_L("aaaabbbbbb"));
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
	header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	time += timeIncrement;
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
	
	TInt index = iSinkImpl->FindVideoSinkUserEntryByMediaType( KUidMediaTypeVideo );
	EUNIT_ASSERT( index != KErrNotFound );
	// This user was resetted, others cannot enqueue data until timestamp skew
	// is detected in them
	EUNIT_ASSERT_EQUALS( iSinkImpl->iUsers[ index ]->iNumTimeStamps, 1 );
	for ( TInt i = 0; i < iSinkImpl->iUsers.Count(); i++ )
	    {
	    if ( i != index )
	        {
	        EUNIT_ASSERT( iSinkImpl->iUsers[ i ]->AllowFrame() == EFalse );
	        }
	    }
	
	// Receiving audio until timestamp skew detected also in it
	TRtpRecvHeader header2;
    header2.iPayloadType = 96;
    TInt time2( 0 );
	TInt timeIncrement2( 2000 );
	buffer->Data().Copy(_L("aaaaaaaaaa"));
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( 100 );
    
    TMediaId mediaId2( KUidMediaTypeAudio );
    
    TInt sequenceNum2( 1 );
	for ( TInt i = 0; i < KMccTimeStampDifferenceTreshold + 2; i++ )
	    {
	    
	    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time2 ) );
	    buffer->SetFrameNumber( sequenceNum2++ );
	    
	    header2.iTimestamp = buffer->TimeToPlay().Int64();
	    header2.iMarker = buffer->LastBuffer();
	    header2.iSeqNum = buffer->FrameNumber();
	    
	    DO_EMPTY_BUFFER_L( buffer, NULL, mediaId2, header2 )
	    EUNIT_ASSERT( header2.iTimestamp == time2 );
	    
	    time2 += timeIncrement2;
	    }  
	time2 = 0;
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time2 ) );
	buffer->SetFrameNumber( sequenceNum2++ );
	header2.iTimestamp = buffer->TimeToPlay().Int64();
	header2.iMarker = buffer->LastBuffer();
	header2.iSeqNum = buffer->FrameNumber();
	time2 += timeIncrement2;
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId2, header2 )
	index = iSinkImpl->FindVideoSinkUserEntryByMediaType( KUidMediaTypeAudio );
	EUNIT_ASSERT( index != KErrNotFound );
	// This user was resetted also, all users can enqueue data
	EUNIT_ASSERT_EQUALS( iSinkImpl->iUsers[ index ]->iNumTimeStamps, 1 );
	for ( TInt i = 0; i < iSinkImpl->iUsers.Count(); i++ )
	    {
	    EUNIT_ASSERT( iSinkImpl->iUsers[ i ]->AllowFrame() == ETrue );
	    }
	
	// Test emptybuffer while paused
	iSinkImpl->PauseL();
    DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
            	
	CleanupStack::PopAndDestroy( buffer );
	}
	
void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_UpdateSettingsL()
    {
    TMccVideoSinkSetting settings;
	settings.iAudioPayloadType = 106;
    settings.iVideoPayloadType = 96;
    settings.iDeviceIndex = 0;
    settings.iLocation.iX = 0;
    settings.iLocation.iY = 83;
    settings.iSize.iWidth = 352;
    settings.iSize.iHeight = 378;
    settings.iIAPId = 2;
    settings.iPriority.iPref = EMdaPriorityPreferenceNone;
    settings.iPriority.iPriority = 0;
    settings.iPriority.iState = EMMFStateIdle;
    settings.iRotation = EMccNone;
    
    // No change
    TWsStubsHelper::Reset();
    TInt numWindowCreated = TWsStubsHelper::NumWindowsCreated();
    iSinkImpl->UpdateSettingsL( settings, EFalse );
    EUNIT_ASSERT( numWindowCreated == TWsStubsHelper::NumWindowsCreated() );
    EUNIT_ASSERT( TWsStubsHelper::CurrentDrawMode() != CGraphicsContext::EDrawModeWriteAlpha );
    
    // Force
    iSinkImpl->UpdateSettingsL( settings, ETrue );
    EUNIT_ASSERT( ++numWindowCreated == TWsStubsHelper::NumWindowsCreated() ); // Window was recreated
    EUNIT_ASSERT( TWsStubsHelper::CurrentDrawMode() == CGraphicsContext::EDrawModeWriteAlpha );
    
    // Settings changed
    settings.iRotation = EMccClockwise180Degree;
    iSinkImpl->UpdateSettingsL( settings, EFalse );
    
    TMccVideoSinkSetting settings2;
    iSinkImpl->GetCurrentSettings( settings2 );
    EUNIT_ASSERT( settings2.iRotation == settings.iRotation );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_TimestampCorrectionTestL()
    {
    // Test timestamp correction which is started at initial phase
    // Timestamp corrections are done only in normal mode
    //

	// First buffer
	TInt firsttime( 1000 );
	TInt time( firsttime );
	TInt timeIncrement( 900 );
	iCurrentTime = TTime( firsttime );
	iEmptyBufferInterval = 100000; // 100ms
    
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
	CleanupStack::PushL( buffer );
	buffer->SetLastBuffer( ETrue );
    buffer->Data().Copy(_L("aaaaaaaaaa"));
	
	TRtpRecvHeader header;
    header.iPayloadType = 97;
    
    TMediaId mediaId( KUidMediaTypeVideo );
    TInt sequenceNum( 1 );
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
	    
	header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	    
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
	EUNIT_ASSERT( header.iTimestamp == time );
	
	// Second buffer, timestamp increased too little, start doing corrections 
	// (however corrections do not affect yet this packet).   
	time += timeIncrement;
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
    
    DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp == time );
    TUint32 previousTime = header.iTimestamp;
    
    // This packet is affected by correction
    time += timeIncrement;
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
    
    DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp != time );
    
    // Timestamp should increase based on receiving times (previous timestamp + receive interval)
    EUNIT_ASSERT_EQUALS( header.iTimestamp, ( previousTime + iEmptyBufferInterval / 100 * 9 ) );
    previousTime = header.iTimestamp;
    
    // Third buffer, beginning of frame, timestamp increased too little, do correction    
    buffer->SetLastBuffer( EFalse );
	time += timeIncrement;
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp != time );
    EUNIT_ASSERT_EQUALS( header.iTimestamp, ( previousTime + iEmptyBufferInterval / 100 * 9 ) );
    
    previousTime = header.iTimestamp;
    
    // Fouth buffer, timestamp not increased from previous rtp packet as it is belonging
    // to the same frame
    buffer->SetLastBuffer( ETrue );
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp == previousTime );

    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_TimestampCorrectionTest2L()
    {
    // Test timestamp correction which is started after monitoring
    // Timestamp corrections are done only in normal mode
    //
	
	// First buffer
	TInt firsttime( 1000 );
	TInt time( firsttime );
	TInt timeIncrement( 11250 ); // 8fps
	iCurrentTime = TTime( firsttime );
	iEmptyBufferInterval = 125000; // 125ms
    
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
	CleanupStack::PushL( buffer );
	buffer->SetLastBuffer( ETrue );
    buffer->Data().Copy(_L("aaaaaaaaaa"));
    
    TRtpRecvHeader header;
    header.iPayloadType = 97;
    
    TMediaId mediaId( KUidMediaTypeVideo );
    TInt sequenceNum( 1 );
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
	    
	header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	    
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
	EUNIT_ASSERT( header.iTimestamp == time );
	
	// Second buffer, timestamp increased enough, no corrections needed
	time += timeIncrement;
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
    
    DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp == time );
    
    // Third buffer, timestamp increased enough, no corrections needed
    time += timeIncrement;
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
    
    DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp == time );
    
    // Next buffer after monitored short period, timestamps not increased enough within
    // monitoring period, modifying needed but is done for next buffer
    const TInt KTestMonitoringPeriod = 2000000; //KMccModifyMonitoringShortPeriodMicrosecs
    iCurrentTime += TTimeIntervalMicroSeconds( KTestMonitoringPeriod );
    
    time += ( ( KTestMonitoringPeriod / 1000000 * 8 ) * timeIncrement ) / 2;
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp == time );
    TUint32 previousTime = header.iTimestamp;
    
    // Now modifying takes place
    time += timeIncrement / 2;
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp != time );
    EUNIT_ASSERT_EQUALS( header.iTimestamp, ( previousTime + iEmptyBufferInterval / 100 * 9 ) );
    
    // Next buffer after monitored long period, timestamps increased anough within
    // monitoring period, modifying not needed anymore
    const TInt KTestMonitoringPeriodLong = 10000000; //KMccModifyMonitoringLongPeriodMicrosecs
    iCurrentTime += TTimeIntervalMicroSeconds( KTestMonitoringPeriodLong );
    
    time = iCurrentTime.Int64() / 1000000 * 90000; // Timestamp close to ideal
	buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
    header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
    EUNIT_ASSERT( header.iTimestamp == time );
    
    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_TimestampCorrectionTest3L()
    {
    // Test timestamp correction in real-time mode (timestamp corrections are not done
    // instead timestamps are set to zero before passing to helix, however zeroing is done at lower level)
    iJitterBufferThreshold = KTestRealTimeModeJitBufThreshold;
    UT_CMccVideoSinkImpl_FormatSDPInfoL();
	
	iSinkImpl->StartL();
	
	// First buffer
	TInt firsttime( 1000 );
	TInt time( firsttime );
	iCurrentTime = TTime( firsttime );
	iEmptyBufferInterval = 100000; // 100ms
    
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
	CleanupStack::PushL( buffer );
	buffer->SetLastBuffer( ETrue );
    buffer->Data().Copy(_L("aaaaaaaaaa"));
	
	TRtpRecvHeader header;
    header.iPayloadType = 97;
    
    TMediaId mediaId( KUidMediaTypeVideo );
    TInt sequenceNum( 1 );
    buffer->SetTimeToPlay( TTimeIntervalMicroSeconds( time ) );
	buffer->SetFrameNumber( sequenceNum++ );
	    
	header.iTimestamp = buffer->TimeToPlay().Int64();
	header.iMarker = buffer->LastBuffer();
	header.iSeqNum = buffer->FrameNumber();
	    
	DO_EMPTY_BUFFER_L( buffer, NULL, mediaId, header )
	EUNIT_ASSERT( header.iTimestamp == time );
	CleanupStack::PopAndDestroy( buffer );
    }
    
void UT_CMccVideoSinkImpl::UT_CMccVideoSinkImpl_GetActualPrerollL()
    {
    // No users, no preroll
    iSinkImpl->iUsers.ResetAndDestroy();
    
    TInt preroll = -1;
    iSinkImpl->GetActualPrerollL( preroll );
    EUNIT_ASSERT_EQUALS( preroll, -1 );
    
    // One user, preroll is taken from that user
    iSinkImpl->AddUserL( *iVideoUser );
    iSinkImpl->iUsers[ 0 ]->Set( iVideoUser, KUidMediaTypeVideo, 0 );
    iSinkImpl->GetActualPrerollL( preroll );
    EUNIT_ASSERT( preroll != -1 );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccVideoSinkImpl,
    "CMccVideoSinkImpl",
    "CMccVideoSinkImpl" )

	EUNIT_TEST(
		"NewL - test ",
		"CMccVideoSinkImpl",
		"NewL",
		"FUNCTIONALITY",
		SetupL, UT_CMccVideoSinkImpl_NewLL, Teardown)
	
	EUNIT_TEST(
		"SinkThreadLogoff - test ",
		"CMccVideoSinkImpl",
		"SinkThreadLogoff",
		"FUNCTIONALITY",
		Setup2L, UT_CMccVideoSinkImpl_SinkThreadLogoffL, Teardown)
			
	EUNIT_TEST(
	    "OpenL - alloc test ",
	    "CMccVideoSinkImpl",
	    "OpenL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_OpenLL, Teardown)	
	
	EUNIT_TEST(
	    "Prepare - test ",
	    "CMccVideoSinkImpl",
	    "Prepare",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_PrepareL, Teardown)
	
	EUNIT_TEST(
	    "RemoteSetup - test ",
	    "CMccVideoSinkImpl",
	    "RemoteSetup",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_RemoteSetupL, Teardown)

	EUNIT_TEST(
  		"Start - alloc test ",
	    "CMccVideoSinkImpl",
	    "Start",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_StartL, Teardown)

	EUNIT_TEST(
	    "Stop - test ",
	    "CMccVideoSinkImpl",
	    "Stop",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_StopL, Teardown)

	EUNIT_TEST(
	    "PauseL - test ",
	    "CMccVideoSinkImpl",
	    "PauseL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_PauseLL, Teardown)

	EUNIT_TEST(
	    "Close - test ",
	    "CMccVideoSinkImpl",
	    "Close",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_CloseL, Teardown)

	EUNIT_TEST(
	    "SetDisplayWindowL - test ",
	    "CMccVideoSinkImpl",
	    "SetDisplayWindowL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_SetDisplayWindowLL, Teardown)

	EUNIT_TEST(
	    "PriorityL - test ",
	    "CMccVideoSinkImpl",
	    "SetDisplayWindowL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_PriorityLL, Teardown)

	EUNIT_TEST(
	    "SetPriorityL - test ",
	    "CMccVideoSinkImpl",
	    "SetPriorityL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_SetPriorityLL, Teardown)

	EUNIT_TEST(
	    "Volume - test ",
	    "CMccVideoSinkImpl",
	    "Volume",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_VolumeL, Teardown)

	EUNIT_TEST(
	    "MaxVolume - test ",
	    "CMccVideoSinkImpl",
	    "MaxVolume",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_MaxVolumeL, Teardown)

    EUNIT_TEST(
	    "SetVolumeL - test ",
	    "CMccVideoSinkImpl",
	    "SetVolumeL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_SetVolumeLL, Teardown)

	EUNIT_TEST(
	    "Balance - test ",
	    "CMccVideoSinkImpl",
	    "Balance",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_BalanceL, Teardown)

	EUNIT_TEST(
	    "SetBalanceL - test ",
	    "CMccVideoSinkImpl",
	    "SetBalanceL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_SetBalanceLL, Teardown)

	EUNIT_TEST(
	    "RegisterForVideoLoadingNotification - test ",
	    "CMccVideoSinkImpl",
	    "RegisterForVideoLoadingNotification",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_RegisterForVideoLoadingNotificationL, Teardown)

	EUNIT_TEST(
        "RegisterForVideoLoadingNotification - test 2",
        "CMccVideoSinkImpl",
        "RegisterForVideoLoadingNotification",
        "FUNCTIONALITY",
        Setup2L, UT_CMccVideoSinkImpl_RegisterForVideoLoadingNotification2L, Teardown)
        
	EUNIT_TEST(
	    "Version - test ",
	    "CMccVideoSinkImpl",
	    "Version",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_VersionL, Teardown)
	
	EUNIT_TEST(
	    "FormatSDPInfoL - test ",
	    "CMccVideoSinkImpl",
	    "FormatSDPInfoL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_FormatSDPInfoL, Teardown)
	        	    	    	    	    	    	    	    	    	    	    	    	    	    	        	
	EUNIT_TEST(
	    "EmptyBufferL - test ",
	    "CMccVideoSinkImpl",
	    "EmptyBufferL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_EmptyBufferL, Teardown)
	    
	EUNIT_TEST(
	    "UpdateSettingsL - test ",
	    "CMccVideoSinkImpl",
	    "UpdateSettingsL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_UpdateSettingsL, Teardown)
	
	EUNIT_TEST(
	    "TimestampCorrectionTestL - test ",
	    "CMccVideoSinkImpl",
	    "TimestampCorrectionTestL",
	    "FUNCTIONALITY",
	    Setup2L, UT_CMccVideoSinkImpl_TimestampCorrectionTestL, Teardown)      
	
	EUNIT_TEST(
	    "TimestampCorrectionTest2L - test ",
	    "CMccVideoSinkImpl",
	    "TimestampCorrectionTest2L",
	    "FUNCTIONALITY",
	    Setup2L, UT_CMccVideoSinkImpl_TimestampCorrectionTest2L, Teardown)    
	    
	EUNIT_TEST(
	    "TimestampCorrectionTest3L - test ",
	    "CMccVideoSinkImpl",
	    "TimestampCorrectionTest3L",
	    "FUNCTIONALITY",
	    Setup3L, UT_CMccVideoSinkImpl_TimestampCorrectionTest3L, Teardown)   
	    	        
	EUNIT_TEST(
	    "GetActualPrerollL - test ",
	    "CMccVideoSinkImpl",
	    "GetActualPrerollL",
	    "FUNCTIONALITY",
	    SetupL, UT_CMccVideoSinkImpl_GetActualPrerollL, Teardown)      
	    
EUNIT_END_TEST_TABLE

//  END OF FILE

