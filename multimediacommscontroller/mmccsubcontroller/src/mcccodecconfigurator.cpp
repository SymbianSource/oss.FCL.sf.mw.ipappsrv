/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Codec configurator for Mcc subthreads
*
*/




// INCLUDE FILES
#include <mmf/server/mmfaudioinput.h>
#include <mmf/server/mmfaudiooutput.h>
#include <mmf/server/mmfformat.h>
#include <e32math.h>

#include <3gplibrary/mp4lib.h>

#include <SpeechEncoderConfig.h>
#include <ErrorConcealmentIntfc.h>
#include <G729DecoderIntfc.h>
#include <G729EncoderIntfc.h>
#include <G711DecoderIntfc.h>
#include <G711EncoderIntfc.h>
#include <IlbcDecoderIntfc.h>
#include <IlbcEncoderIntfc.h>

#include "mcccodecconfigurator.h"
#include "mccinternalcodecs.h"
#include "mmcccodecinformation.h"
#include "mmccinterfacedef.h"
#include "mccinternaldef.h"
#include "formatstatemachine.h"

#include "mccvideosource.h"
#include "mccvideosink.h"
#include "mccfilesink.h"

#include "mccsubcontrollerlogs.h"
#include "dtmfpayloadformatwrite.h"
#include "dtmfpayloadformatread.h"

// LOCAL CONSTANTS
// AMR-NB Codec possible bitrates
const TInt KAmrNbBitrate475 = 4750;
const TInt KAmrNbBitrate515 = 5150;
const TInt KAmrNbBitrate590 = 5900;
const TInt KAmrNbBitrate670 = 6700;
const TInt KAmrNbBitrate740 = 7400;
const TInt KAmrNbBitrate795 = 7950;
const TInt KAmrNbBitrate102 = 10200;
const TInt KAmrNbBitrate122 = 12200;

// AMR-WB Codec possible bitrates
const TInt KAmrWbBitrate660 = 6600;
const TInt KAmrWbBitrate885 = 8850;
const TInt KAmrWbBitrate1265 = 12650;
const TInt KAmrWbBitrate1425 = 14250;
const TInt KAmrWbBitrate1585 = 15850;
const TInt KAmrWbBitrate1825 = 18250;
const TInt KAmrWbBitrate1985 = 19850;
const TInt KAmrWbBitrate2305 = 23050;
const TInt KAmrWbBitrate2385 = 23850;

// iLBC Codec possible bitrates
const TInt KIlbcBitrate1520 = 15200;
const TInt KIlbcBitrate1333 = 13330;

// G.711 Modes
const TInt KG711ModeaLaw = 3;
const TInt KG711ModeuLaw = 2;

// MACROS
#define MCC_CODEC_CONF_SET_VAD_L( curr_vad, get_func, new_vad, set_func ) \
__SUBCONTROLLER( "MccCodecConfigurator:: setting vad" ) \
TBool curr_vad( EFalse ); \
TInt get_vad_err = get_func; \
__SUBCONTROLLER_INT1( "MccCodecConfigurator:: get vad err:", get_vad_err ) \
__SUBCONTROLLER_INT1( "MccCodecConfigurator:: curr vad:", curr_vad ) \
if ( !get_vad_err && curr_vad != new_vad ) \
    { \
    TRAP_IGNORE(User::LeaveIfError( set_func )); \
    __SUBCONTROLLER_INT1( "MccCodecConfigurator:: vad set to:", new_vad ) \
    }

#if ( defined __WINSCW__ ) || ( defined __WINS__ ) || ( defined TEST_EUNIT )
#define MCC_CODEC_CONF_DISABLE_CONFIGURATION
#endif   

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// MccCodecConfigurator::ConfigureCodecL
// Generic codec configuration for codecs
// -----------------------------------------------------------------------------
void MccCodecConfigurator::ConfigureCodecL( 
    MDataSource& aDatasource, 
    MDataSink& aDatasink, 
    const TMccCodecInfo& aCodec,
    CMccRtpMediaClock& aClock,
    CMccSymStreamBase::TCodecState aCodecState,
    MDataSink* aAssociatedDatasink )
    {
	__SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL" )

	__SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, checking codec" )
    User::LeaveIfError( TMccCodecInfo::CheckCodecInfo( aCodec ) );
    
    if( KUidMmfAudioInput == aDatasource.DataSourceType() )
        {        
		__SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, KUidMmfAudioInput" )
        MMMFAudioInput& record_device = static_cast<MMMFAudioInput&>( aDatasource );
        CPayloadFormatWrite& format_enc = static_cast<CPayloadFormatWrite&>( aDatasink );
        DoConfigureLiveAudioUplinkStreamL( record_device, format_enc, aCodec, aClock, aCodecState );
        }
    else if( KUidMmfAudioOutput == aDatasink.DataSinkType() )
        {
		__SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, KUidMmfAudioOutput" )
        MMMFAudioOutput& play_device = static_cast<MMMFAudioOutput&>( aDatasink );
        CPayloadFormatRead& format_dec = static_cast<CPayloadFormatRead&>( aDatasource );
        DoConfigureLiveAudioDownlinkStreamL( play_device, format_dec, aCodec, aCodecState );
        }
    else if( KMccFileSourceUid == aDatasource.DataSourceType() )
        {
        __SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, KMccFileSourceUID" )
        if ( KMccFileSinkUid != aDatasink.DataSinkType() )
            {
    	    CPayloadFormatWrite& format = static_cast<CPayloadFormatWrite&>( aDatasink );
    	    DoConfigureClipUplinkStreamL( format, aCodec, aAssociatedDatasink, aClock );
	        PassPrerollFromSinkToSourceL(aAssociatedDatasink, &aDatasource);
            }
        }
    else if ( KMccFileSinkUid == aDatasink.DataSinkType() )
	    {
        if ( KMccVideoSourceUid == aDatasource.DataSourceType() )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, KMccFileSinkUID, KMccVideoSourceUid" )
            DoConfigureCameraL( static_cast<CMccVideoSource&>( aDatasource ), aCodec );
            DoConfigureFileSinkL( static_cast<CMccFileSink&>( aDatasink ), aCodec );
            }
        else
            {
            __SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, KMccFileSinkUID, source decoder" )
            CPayloadFormatRead& formatDec = static_cast<CPayloadFormatRead&>( aDatasource );
		    TMccCodecInfoBuffer buffer( aCodec );
            formatDec.ConfigurePayloadFormatL( buffer );
            }
	    }
    else if( KMccVideoSourceUid == aDatasource.DataSourceType() )
        {
	    CPayloadFormatWrite& format = static_cast<CPayloadFormatWrite&>( aDatasink );
		__SUBCONTROLLER( "CMccCodecConfigurator::ConfigureCodecL, KMccVideoSourceUID, video" )
	    CMccVideoSource& recordDevice = static_cast<CMccVideoSource&>( aDatasource );
	    DoConfigureLiveVideoUplinkStreamL( recordDevice, format, aCodec, aAssociatedDatasink, aClock  );
        }
    else if( KMccVideoSinkUid == aDatasink.DataSinkType() )
        {
		__SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, KMccVideoSinkUID" )
		if ( KUidMmfFormatDecode == aDatasource.DataSourceType() )
		    {
		    __SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, source decoder" )
	        CPayloadFormatRead& formatDec = static_cast<CPayloadFormatRead&>( aDatasource );
            DoConfigureLiveVideoDownlinkStreamL( &aDatasink, formatDec, aCodec );
		    }
        }        
    else if ( aCodec.iFourCC == KMccFourCCIdDTMF )
        {
        __SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, KMccFourCCIdDTMF" )
        
        if ( KUidMmfFormatDecode == aDatasource.DataSourceType() )
            {
            CPayloadFormatRead& formatDec = static_cast<CPayloadFormatRead&>( aDatasource );
            DoConfigureDtmfFormatL( formatDec, aCodec );
            }
        else if ( KUidMmfFormatEncode == aDatasink.DataSinkType() )
            {
            CPayloadFormatWrite& formatEnc = static_cast<CPayloadFormatWrite&>( aDatasink );
            DoConfigureDtmfFormatL( formatEnc, aCodec, aClock );
            }
        else
            {
            User::Leave( KErrArgument );
            }
        }
    else
        {
		__SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, KErrArgument" )
        User::Leave( KErrArgument );
        }
	__SUBCONTROLLER( "MccCodecConfigurator::ConfigureCodecL, exit" )
    }


// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureAmrL
// Configures the AMR record device
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureAmrL( 
    MMMFAudioOutput& aPlaybackDev, 
    const TMccCodecInfo& aCodec )
    {
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureAmrL, decoder" )

    TFourCC fourcc( aCodec.iFourCC );
    const TUint32 fourccVal = fourcc.FourCC();
    if ( ( fourccVal == KMccFourCCIdAMRNB && 
           !IsSupportedAmrNbBitrate( aCodec.iBitrate ) ) ||
         ( fourccVal == KMccFourCCIdAMRWB && 
           !IsSupportedAmrWbBitrate( aCodec.iBitrate ) ) )
        {
        User::Leave( KErrNotSupported );
        }
        
    __SUBCONTROLLER( "MccCodecConfigurator, bitrate allowed" )
    
    CErrorConcealmentIntfc* errorConcealIf = 
        CErrorConcealmentIntfc::NewL( aPlaybackDev.SoundDevice() );
      
    errorConcealIf->SetFrameMode( ETrue );
    
    delete errorConcealIf;
    errorConcealIf = NULL;
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureAmrL, exit" )
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureAmrL
// Configures the AMR playback device
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureAmrL( 
    MMMFAudioInput& aRecordDev, 
    const TMccCodecInfo& aCodec )
    {
    
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureAmrL, encoder" )
    
    TFourCC fourcc( aCodec.iFourCC );
    const TUint32 fourccVal = fourcc.FourCC();
    if ( ( fourccVal == KMccFourCCIdAMRNB && 
           !IsSupportedAmrNbBitrate( aCodec.iBitrate ) ) ||
         ( fourccVal == KMccFourCCIdAMRWB && 
           !IsSupportedAmrWbBitrate( aCodec.iBitrate ) ) )
        {
        User::Leave( KErrNotSupported );
        }
        
    __SUBCONTROLLER( "MccCodecConfigurator, bitrate allowed" )
    
    CSpeechEncoderConfig* speechEncConfIf = 
            CSpeechEncoderConfig::NewL( aRecordDev.SoundDevice() );
    CleanupStack::PushL( speechEncConfIf );
    
	__SUBCONTROLLER( "MccCodecConfigurator, setting VAD" )
	
    TBool requestedEnableVad = aCodec.iEnableDTX;
	
	MCC_CODEC_CONF_SET_VAD_L( currentVad,
	                          speechEncConfIf->GetVadMode( currentVad ),
	                          requestedEnableVad, 
                        	  speechEncConfIf->SetVadMode( requestedEnableVad ) )

    __SUBCONTROLLER_INT1( "MccCodecConfigurator, setting bitrate", aCodec.iBitrate )
    TUint bitrate( 0 );
    User::LeaveIfError( speechEncConfIf->GetBitrate( bitrate ) );
    if ( bitrate != aCodec.iBitrate )
        {
        User::LeaveIfError( speechEncConfIf->SetBitrate( aCodec.iBitrate ) );
        }

    CleanupStack::PopAndDestroy( speechEncConfIf );
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureAmrL, exit" )
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureG711L
// Configures the G.711 playback device
// -----------------------------------------------------------------------------
//
void MccCodecConfigurator::DoConfigureG711L( 
    MMMFAudioOutput& aPlaybackDev, 
    const TMccCodecInfo& aCodec,
    CMccSymStreamBase::TCodecState aCodecState )
    {
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureG711L, decoder" )

    CG711DecoderIntfc* g711DecIf = 
            CG711DecoderIntfc::NewL( aPlaybackDev.SoundDevice() );
    CleanupStack::PushL( g711DecIf );
    
    // Cannot change mode on fly, 
    // changing it only right after the codec has been loaded
    if ( aCodecState == CMccSymStreamBase::EStateCodecLoaded )
        {
        if( KG711ModeaLaw == aCodec.iCodecMode )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureG711L, aLaw" )
            User::LeaveIfError( g711DecIf->SetDecoderMode(
                CG711DecoderIntfc::EDecALaw ) );
            }
        else if( KG711ModeuLaw == aCodec.iCodecMode )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureG711L, uLaw" )
            User::LeaveIfError( g711DecIf->SetDecoderMode(
                CG711DecoderIntfc::EDecULaw ) );  
            }
        else
            {
            User::Leave( KErrNotSupported );
            }
        }
          
    TBool enableCng = ConvertBoolean( aCodec.iEnableDTX );
    
    MCC_CODEC_CONF_SET_VAD_L( currentVad,
	                          g711DecIf->GetCng( currentVad ),
	                          enableCng, 
                        	  g711DecIf->SetCng( enableCng ) )
        
    CleanupStack::PopAndDestroy( g711DecIf );
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureG711L
// Configures the G.711 record device
// -----------------------------------------------------------------------------
//
void MccCodecConfigurator::DoConfigureG711L( 
    MMMFAudioInput& aRecordDev, 
    const TMccCodecInfo& aCodec,
    CMccSymStreamBase::TCodecState aCodecState )
    {
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureG711L, encoder" )

    CG711EncoderIntfc* g711EncIf = 
            CG711EncoderIntfc::NewL( aRecordDev.SoundDevice() );
    CleanupStack::PushL( g711EncIf );

    // Cannot change mode on fly, 
    // changing it only right after the codec has been loaded
    if ( aCodecState == CMccSymStreamBase::EStateCodecLoaded )
        {
        if( KG711ModeaLaw == aCodec.iCodecMode )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureG711L, aLaw" )
            User::LeaveIfError( g711EncIf->SetEncoderMode(
                CG711EncoderIntfc::EEncALaw ) );
            }
        else if ( KG711ModeuLaw == aCodec.iCodecMode )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureG711L, uLaw" )
            User::LeaveIfError( g711EncIf->SetEncoderMode(
                CG711EncoderIntfc::EEncULaw ) );
            }
        else
            {
            User::Leave( KErrNotSupported );
            }
        }
    
    TBool enableVad = ConvertBoolean( aCodec.iEnableDTX );
    
	MCC_CODEC_CONF_SET_VAD_L( currentVad,
	                          g711EncIf->GetVadMode( currentVad ),
	                          enableVad, 
                        	  g711EncIf->SetVadMode( enableVad ) )                      	  
 
    CleanupStack::PopAndDestroy( g711EncIf );
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureG729L
// Configures the G.729 playback device
// -----------------------------------------------------------------------------
//
void MccCodecConfigurator::DoConfigureG729( MMMFAudioOutput& /*aPlaybackDev*/ )
    {
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureG729, decoder, NOP" )
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureG729L
// Configures the G.729 record device
// -----------------------------------------------------------------------------
//
void MccCodecConfigurator::DoConfigureG729L( MMMFAudioInput& aRecordDev,
    TBool aEnableCng )
    {
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureG729, encoder" )
    
    CG729EncoderIntfc* encG729If = 
            CG729EncoderIntfc::NewL( aRecordDev.SoundDevice() );
    CleanupStack::PushL( encG729If );

    TBool enableVad = ConvertBoolean( aEnableCng );
   
   	MCC_CODEC_CONF_SET_VAD_L( currentVad,
	                          encG729If->GetVadMode( currentVad ),
	                          enableVad, 
                        	  encG729If->SetVadMode( enableVad ) )     
        
    CleanupStack::PopAndDestroy( encG729If );       
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureIlbcL
// Configures the iLBC playback device
// -----------------------------------------------------------------------------
//
void MccCodecConfigurator::DoConfigureIlbcL( 
    MMMFAudioOutput& aPlaybackDev, 
    const TMccCodecInfo& aCodec,
    CMccSymStreamBase::TCodecState aCodecState )
    {
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureIlbcL decoder" )
    __SUBCONTROLLER_INT1( "MccCodecConfigurator::DoConfigureIlbcL aCodecState: ",
        aCodecState )
    
    // Cannot change mode nor CNG state on fly, changing it only right after
    // the codec has been loaded. Same thing applies also to other parameters.
    if ( CMccSymStreamBase::EStateCodecLoaded == aCodecState )
        {
        CIlbcDecoderIntfc* decIlbcIf = 
            CIlbcDecoderIntfc::NewL( aPlaybackDev.SoundDevice() );
        CleanupStack::PushL( decIlbcIf );
    
        if( KIlbcBitrate1520 == aCodec.iBitrate )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureIlbcL, 20ms" )
            
            User::LeaveIfError( decIlbcIf->SetDecoderMode(
                CIlbcDecoderIntfc::E20msFrame ) );
            }
        else if( KIlbcBitrate1333 == aCodec.iBitrate )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureIlbcL, 30ms" )
            
            User::LeaveIfError( decIlbcIf->SetDecoderMode(
                CIlbcDecoderIntfc::E30msFrame ) );
            }
        else
            {
            User::Leave( KErrNotSupported );
            }
            
            
        TBool cngEnabled = ConvertBoolean( aCodec.iEnableDTX );
    
        MCC_CODEC_CONF_SET_VAD_L( currentVad,
	                              decIlbcIf->GetCng( currentVad ),
	                              cngEnabled, 
                        	      decIlbcIf->SetCng( cngEnabled ) ) 
        
        CleanupStack::PopAndDestroy( decIlbcIf );
        }
        
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureIlbcL decoder, Exit" )
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureIlbcL
// Configures the iLBC record device
// -----------------------------------------------------------------------------
//
void MccCodecConfigurator::DoConfigureIlbcL( 
    MMMFAudioInput& aRecordDev, 
    const TMccCodecInfo& aCodec,
    CMccSymStreamBase::TCodecState aCodecState )
    {
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureIlbcL encoder" )
    __SUBCONTROLLER_INT1( "MccCodecConfigurator::DoConfigureIlbcL aCodecState: ",
        aCodecState )
    
    // Cannot change mode nor VAD state on fly, changing it only right after
    // the codec has been loaded. Same thing applies also to other parameters.
    if ( CMccSymStreamBase::EStateCodecLoaded == aCodecState )
        {
        CIlbcEncoderIntfc* encIlbcIf = 
            CIlbcEncoderIntfc::NewL( aRecordDev.SoundDevice() );
        CleanupStack::PushL( encIlbcIf );
    
        if( KIlbcBitrate1520 == aCodec.iBitrate )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureIlbcL, 20ms" )
            
            User::LeaveIfError( encIlbcIf->SetEncoderMode(
                CIlbcEncoderIntfc::E20msFrame ) );
            }
        else if ( KIlbcBitrate1333 == aCodec.iBitrate )
            {
            __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureIlbcL, 30ms" )
            
            User::LeaveIfError( encIlbcIf->SetEncoderMode(
                CIlbcEncoderIntfc::E30msFrame ) );
            }
        else
            {
            User::Leave( KErrNotSupported );
            }
        
        TBool vadEnabled = ConvertBoolean(  aCodec.iEnableDTX );
        
        MCC_CODEC_CONF_SET_VAD_L( currentVad,
	                              encIlbcIf->GetVadMode( currentVad ),
	                              vadEnabled, 
                        	      encIlbcIf->SetVadMode( vadEnabled ) )
        
        CleanupStack::PopAndDestroy( encIlbcIf );
        }
        
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureIlbcL encoder, Exit" )
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureCameraL
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureCameraL( CMccVideoSource& aVideoSource, 
    const TMccCodecInfo& aCodec )
	{
	if( aCodec.iType == KUidMediaTypeVideo )
		{
		aVideoSource.SetVideoCodecL( aCodec );
		}
	else
		{
		aVideoSource.SetAudioCodecL( aCodec );
		}	
	}

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureCameraL
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureFileSinkL( CMccFileSink& aFileSink, 
    const TMccCodecInfo& aCodec )
    {
    if( aCodec.iType == KUidMediaTypeVideo )
		{
		aFileSink.SetVideoCodecL( aCodec );
		}
	else
		{
		aFileSink.SetAudioCodecL( aCodec );
		}	
    }
    
// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoGetCameraConfigurationL
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoGetCameraConfigurationL( 
    const TMccCodecInfo& /*aCodec*/, 
    const TDesC8& /*aParam*/ )
	{
	}

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoGetClipConfigurationL
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoGetClipConfigurationL( TDes8& aParam )
	{
	__SUBCONTROLLER( "MccCodecConfigurator::DoGetClipConfigurationL" )
	
	TMccFileSourceSetting& setting = 
	    (*reinterpret_cast<TMccFileSourceSettingBuf*>( &aParam ))();
	
	MP4Handle mp4Handle;
	MP4Err err = MP4_OK;
    
    TFileName fileName( setting.iFileName );
    
    const TInt KMccZeroTerminatorLen = 1;
    __ASSERT_ALWAYS( 
	    fileName.Length() + KMccZeroTerminatorLen <= fileName.MaxLength(),
	    User::Leave( KErrOverflow ) );
    
    err = MP4ParseOpen( &mp4Handle, (wchar_t *) fileName.PtrZ() );
    
    if ( err == MP4_OUT_OF_MEMORY )
        {
        User::Leave( KErrNoMemory );
        }
    
    __ASSERT_ALWAYS( err == MP4_OK, User::Leave( KErrNotFound ) );
    
    // Read audio settings
    //
	mp4_u32 audioLength = 0;
    mp4_u32 audioType;
	mp4_u8 framesPerSample;
	mp4_u32 audioTimeScale;
	mp4_u32 audioAverageBitRate = 0;
	
	err = MP4_OK;	
	err = MP4ParseRequestAudioDescription( mp4Handle,
	        ( mp4_u32* ) &audioLength, ( mp4_u32* ) &audioType,
	        ( mp4_u8* ) &framesPerSample, ( mp4_u32* ) &audioTimeScale,
	        ( mp4_u32* ) &audioAverageBitRate );
	    	
	if ( err == MP4_OK && audioType != MP4_TYPE_NONE )
	    {
	    if ( audioType == MP4_TYPE_AMR_NB )
	        {
	        __SUBCONTROLLER( 
	        "MccCodecConfigurator::DoGetClipConfigurationL, file contains AMR" )
	        setting.iAudioFourCC = TFourCC( KMccFourCCIdAMRNB );
	        setting.iAudioBitRate = audioAverageBitRate;
	        }
	    else
	        {
	        __SUBCONTROLLER( 
	        "MccCodecConfigurator::DoGetClipConfigurationL, file contains unsupported audio" )
	        setting.iAudioFourCC = TFourCC( KMccFourCCUnsupported );
	        }
	    }
	else 
	    {
	    __SUBCONTROLLER( "MccCodecConfigurator::DoGetClipConfigurationL, no audio" )
        setting.iAudioFourCC = TFourCC();
	    }
	
	
	// Read video settings 
	//  
	mp4_u32 videoLength = 0;
	TReal frameRate;
	mp4_u32 videoType;
	mp4_u32 videoWidth;
	mp4_u32 videoHeight;
	mp4_u32 videoTimeScale;
	mp4_u32 videoAverageBitRate;
	mp4_u32 streamSize;
	
	err = MP4_OK;
    err = MP4ParseRequestVideoDescription( mp4Handle,
		( mp4_u32* ) &videoLength,  &frameRate, ( mp4_u32* ) &videoType,
		( mp4_u32* ) &videoWidth,  ( mp4_u32* ) &videoHeight,
        ( mp4_u32* ) &videoTimeScale );

    if ( err == MP4_OK && videoType != MP4_TYPE_NONE )
        {
        if ( videoType == MP4_TYPE_H263_PROFILE_0 || 
             videoType == MP4_TYPE_AVC_PROFILE_BASELINE )
            {
            __SUBCONTROLLER( 
            "MccCodecConfigurator::DoGetClipConfigurationL, file contains supported video" )
            
            err = MP4ParseRequestStreamDescription( mp4Handle,
                ( mp4_u32* ) &streamSize, ( mp4_u32* ) &videoAverageBitRate );
            
            __ASSERT_ALWAYS( err == MP4_OK, User::Leave( KErrGeneral ) );
            
            if ( videoType == MP4_TYPE_H263_PROFILE_0 )
                {
                __SUBCONTROLLER( "H263" )
                setting.iVideoFourCC = TFourCC( KMccFourCCIdH263 );
                }
            else
                {
                __SUBCONTROLLER( "AVC" )
                setting.iVideoFourCC = TFourCC( KMccFourCCIdAVC );
                }

            MccConversionUtility::FrameRateSanitizeL( 
                setting.iVideoFrameRate, frameRate, videoType );
            
            setting.iVideoFrameSize.iWidth = videoWidth;
            setting.iVideoFrameSize.iHeight = videoHeight;
            
            // Average bitrate is for the whole stream, 
            // subtract audio average to get video average
            setting.iVideoBitRate = videoAverageBitRate - audioAverageBitRate;
            
            MccConversionUtility::BitRateSanitize( 
                            setting.iVideoBitRate, setting.iVideoBitRate, videoType );
            }
        else
            {
            __SUBCONTROLLER( 
            "MccCodecConfigurator::DoGetClipConfigurationL, file contains unsupported video" )
            setting.iVideoFourCC = TFourCC( KMccFourCCUnsupported );
            }
        }
    
    else 
    	{
    	__SUBCONTROLLER( 
    	"MccCodecConfigurator::DoGetClipConfigurationL, no video" )
        setting.iVideoFourCC = TFourCC();
    	}
    		
    MP4ParseClose( mp4Handle );
    
    // Audio and video might have different duration, take longer one
    //
    const TUint KMccMilliSecsToMicroSecs = 1000;
    TUint32 duration = videoLength > audioLength ? videoLength : audioLength;
    duration = duration * KMccMilliSecsToMicroSecs;
    setting.iDuration = duration;
    
    setting.iPosition = 0;
    
    __SUBCONTROLLER_INT1( "duration:", setting.iDuration.Int64() )  
    __SUBCONTROLLER_INT1( "position:", setting.iPosition.Int64() )  
    __SUBCONTROLLER_INT1( "videoframerate:", (TInt) setting.iVideoFrameRate )  
    __SUBCONTROLLER_INT1( "videobitrate:", setting.iVideoBitRate )  
    __SUBCONTROLLER_INT2( "videoframesize width:", 
                          setting.iVideoFrameSize.iWidth, 
                          " height:",
                          setting.iVideoFrameSize.iHeight )  
    __SUBCONTROLLER_INT1( "audiobitrate:", setting.iAudioBitRate ) 
    __SUBCONTROLLER_INT1( "audio fourcc:", setting.iAudioFourCC.FourCC() ) 
    __SUBCONTROLLER_INT1( "video fourcc:", setting.iVideoFourCC.FourCC() ) 
    
    __SUBCONTROLLER( "MccCodecConfigurator::DoGetClipConfigurationL, exit" )
	}
	
// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureLiveAudioUplinkStreamL
// Uplink/Record codec configuration
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureLiveAudioUplinkStreamL( 
    MMMFAudioInput& aRecordDevice, 
    CPayloadFormatWrite& aFormatter, 
    const TMccCodecInfo& aCodec,
    CMccRtpMediaClock& aClock,
    CMccSymStreamBase::TCodecState aCodecState )
    {
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureLiveAudioUplinkStreamL" )
    
    if ( aCodecState >= CMccSymStreamBase::EStateCodecLoaded )
        {
        TFourCC fourcc( aCodec.iFourCC );
        
        switch( fourcc.FourCC() )
            {
            case KMccFourCCIdAMRNB:
            case KMccFourCCIdAMRWB:
                {
                #ifdef MCC_CODEC_CONF_DISABLE_CONFIGURATION

                // Suppress compiler warnings
                aRecordDevice = aRecordDevice;
                
                #else
                
                DoConfigureAmrL( aRecordDevice, aCodec );
                
                #endif
                }
                break;

            case KMccFourCCIdG711:
                {
                #ifndef MCC_CODEC_CONF_DISABLE_CONFIGURATION
                
                DoConfigureG711L( aRecordDevice, aCodec, aCodecState );
                
                #endif
                }
                break;
            case KMccFourCCIdILBC:
                {
                #ifndef MCC_CODEC_CONF_DISABLE_CONFIGURATION
                
                DoConfigureIlbcL( aRecordDevice, aCodec, aCodecState );
                
                #endif
                }
                break;
            case KMccFourCCIdG729:
                {
                #ifndef MCC_CODEC_CONF_DISABLE_CONFIGURATION
                
                DoConfigureG729L( aRecordDevice, aCodec.iEnableDTX );
                
                #endif
                }
                break;
            default:
    			__SUBCONTROLLER_INT1( "MccCodecConfigurator, KErrNotSupported", fourcc.FourCC() )
                
                User::Leave( KErrNotSupported );
                break;
            }
        }

    const TMccCodecInfoBuffer buffer( aCodec );
    aFormatter.ConfigurePayloadFormatL( buffer, aClock );
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureLiveAudioUplinkStreamL, exit" )
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureLiveAudioDownlinkStreamL
// Downlink/Playback codec configuration
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureLiveAudioDownlinkStreamL( 
    MMMFAudioOutput& aPlayDevice, 
    CPayloadFormatRead& aFormatter, 
    const TMccCodecInfo& aCodec,
    CMccSymStreamBase::TCodecState aCodecState )
    {
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureLiveAudioDownlinkStreamL" )
    
    if ( aCodecState >= CMccSymStreamBase::EStateCodecLoaded )
        {
        TFourCC fourcc = aCodec.iFourCC;
        
        switch( fourcc.FourCC() )
            {
            case KMccFourCCIdAMRNB:
            case KMccFourCCIdAMRWB:
                {
                
                #ifndef MCC_CODEC_CONF_DISABLE_CONFIGURATION
                
                DoConfigureAmrL( aPlayDevice, aCodec );
                
                #endif
                }
                break;

            case KMccFourCCIdG711:
                {
                #ifndef MCC_CODEC_CONF_DISABLE_CONFIGURATION
                
                DoConfigureG711L( aPlayDevice, aCodec, aCodecState );
                
                #endif
                }
                break;
            case KMccFourCCIdILBC:
                {
                #ifndef MCC_CODEC_CONF_DISABLE_CONFIGURATION

                DoConfigureIlbcL( aPlayDevice, aCodec, aCodecState );
                
                #endif
                }
                break;
            case KMccFourCCIdG729:
                {
                // Nothing to do for G.729 decoder, but VoIP traces need that function
                // to be called. This is call is safe for WINSCW also.
                DoConfigureG729( aPlayDevice );
                }
                break;
            default:
                User::Leave( KErrNotSupported );
                break;
            }
        }
    
    const TMccCodecInfoBuffer buffer( aCodec );
    aFormatter.ConfigurePayloadFormatL( buffer );
	__SUBCONTROLLER( "MccCodecConfigurator::DoConfigureLiveAudioDownlinkStreamL, exit" )
    }
    
// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureDisplayL
// Uplink/Display codec configuration
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureDisplayL(  
    const TMccCodecInfo& aCodec,
    MDataSink* aDatasink )
    {        
    if ( aDatasink && aDatasink->DataSinkType() == KMccVideoSinkUid )
        {
        if ( aCodec.iType == KUidMediaTypeAudio )
            {
            static_cast<CMccVideoSink*>( aDatasink )->SetAudioCodecL( aCodec );
            }
        else
            {
            static_cast<CMccVideoSink*>( aDatasink )->SetVideoCodecL( aCodec );
            }
        }
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::PassPrerollFromSinkToSourceL
// Passes preroll value from sink to source
// -----------------------------------------------------------------------------
void MccCodecConfigurator::PassPrerollFromSinkToSourceL(
	MDataSink* aDataSink,  MDataSource* aDataSource)
    {
    if ( aDataSource &&
         aDataSink &&
         aDataSource->DataSourceType().iUid == KImplUidMccFileSource &&
         aDataSink->DataSinkType().iUid == KImplUidMccVideoSink )
        {
        // Pass preroll value from sink to source
        CMccDataSink* mccVideoSink = static_cast<CMccDataSink*>( aDataSink );
        __ASSERT_DEBUG( mccVideoSink, 
        User::Panic(
        	_L("MccCodecConfigurator::PassPrerollFromSinkToSourceL mccVideoSink == 0"), 
        	KErrArgument ) );
        __ASSERT_ALWAYS( mccVideoSink, User::Leave(KErrArgument) );
        CMccDataSource* mccFileSource = static_cast<CMccDataSource*>( aDataSource );
        __ASSERT_DEBUG( mccFileSource, 
        User::Panic(
        	_L("MccCodecConfigurator::PassPrerollFromSinkToSourceL mccFileSource == 0"), 
        	KErrArgument ) );
        __ASSERT_ALWAYS( mccFileSource, User::Leave(KErrArgument) );

        TPckgBuf<TInt> packageBuffer;
        mccVideoSink->GetParameterL(KMccPrerollTime, packageBuffer );
        mccFileSource->SetParameterL(KMccPrerollTime, packageBuffer );
        }   
    }    
    
// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureClipUplinkStreamL
// Uplink/display codec configuration
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureClipUplinkStreamL( 
	CPayloadFormatWrite& aFormatter, 
    const TMccCodecInfo& aCodec,
    MDataSink* aAssociatedSink,
    CMccRtpMediaClock& aClock )
    {
	TMccCodecInfoBuffer buffer( aCodec );
    aFormatter.ConfigurePayloadFormatL( buffer, aClock );
    DoConfigureDisplayL( aCodec, aAssociatedSink );
    }
    
// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureLiveVideoUplinkStreamL
// Uplink/Record codec configuration
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureLiveVideoUplinkStreamL( 
    CMccVideoSource& aDataSource,
	CPayloadFormatWrite& aFormatter, 
    const TMccCodecInfo& aCodec,
    MDataSink* aAssociatedSink,
    CMccRtpMediaClock& aClock )
    {
    DoConfigureCameraL( aDataSource, aCodec );
	TMccCodecInfoBuffer buffer( aCodec );
    aFormatter.ConfigurePayloadFormatL( buffer, aClock );
    DoConfigureDisplayL( aCodec, aAssociatedSink );
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureDtmfFormatL
// Configures DTMF mediaformat plugin.
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureDtmfFormatL(
	CPayloadFormatRead& aFormatDecoder,
    const TMccCodecInfo& aCodec )
    {
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureDtmfFormatL, downlink" )
    
    CDTMFPayloadFormatRead& formatRead 
        = reinterpret_cast<CDTMFPayloadFormatRead&>( aFormatDecoder );
    
    const TMccCodecInfoBuffer buffer( aCodec );
    formatRead.ConfigurePayloadFormatL( buffer );
    formatRead.SetPayloadFormat( EDTMFPayloadFormatEvent );
    
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureDtmfFormatL, exit" )
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureDtmfFormatL
// Configures DTMF mediaformat plugin.
// -----------------------------------------------------------------------------
void MccCodecConfigurator::DoConfigureDtmfFormatL(
    CPayloadFormatWrite& aFormatEncoder,
    const TMccCodecInfo& aCodec,
    CMccRtpMediaClock& aClock )
    {
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureDtmfFormatL, uplink" )
    
    CDTMFPayloadFormatWrite& formatWrite
        = reinterpret_cast<CDTMFPayloadFormatWrite&>( aFormatEncoder );
    
    const TMccCodecInfoBuffer buffer( aCodec );
    formatWrite.ConfigurePayloadFormatL( buffer, aClock );
    formatWrite.NotifyStopInDTMFString( ETrue );
    formatWrite.SetPayloadFormat( EDTMFPayloadFormatEvent );
    
    __SUBCONTROLLER( "MccCodecConfigurator::DoConfigureDtmfFormatL, exit" )
    }
    
// -----------------------------------------------------------------------------
// MccCodecConfigurator::DoConfigureLiveVideoDownlinkStreamL
// video Downlink codec configuration
// -----------------------------------------------------------------------------  
void MccCodecConfigurator::DoConfigureLiveVideoDownlinkStreamL(
	MDataSink* aDataSink,
	CPayloadFormatRead& aFormatter,
	const TMccCodecInfo& aCodec )
	{              
    TMccCodecInfoBuffer buffer( aCodec );
    aFormatter.ConfigurePayloadFormatL( buffer );
    DoConfigureDisplayL( aCodec, aDataSink );
	}
    
        
// -----------------------------------------------------------------------------
// MccCodecConfigurator::GetCodecDefaultsL
// Uplink/Record codec configuration
// -----------------------------------------------------------------------------
void MccCodecConfigurator::GetCodecDefaultsL( 
    const TUid aType,
    TDes8& aParam, 
    TMccCodecInfo& aCodec )
	{
    if( KMccVideoSourceUid == aType )
        {
	    DoGetCameraConfigurationL( aCodec, aParam );			
        }
    else if( KMccFileSourceUid == aType )
        {
	    DoGetClipConfigurationL( aParam );			
        }
    else
        {
        }
	}

// -----------------------------------------------------------------------------
// MccCodecConfigurator::ChangeBitrateL
// Changes the current codec bitrate
// -----------------------------------------------------------------------------
//
void MccCodecConfigurator::ChangeBitrateL( 
    const TMccCodecInfo& aCodec,
    TUint aBitrate, 
    CMMFDevSound& aSounddevice )
    {
    __SUBCONTROLLER( "MccCodecConfigurator::ChangeBitrateL" )
    
    // Create an instance of speech encoder config custom interface
    CSpeechEncoderConfig* speechEncConfIf = 
            CSpeechEncoderConfig::NewL( aSounddevice );
    
    CleanupStack::PushL( speechEncConfIf );
    
    // Only AMR supports bitrate change dynamically
    TFourCC fourcc( aCodec.iFourCC );
    const TUint32 fourccVal = fourcc.FourCC();
    if ( KMccFourCCIdAMRNB == fourccVal && IsSupportedAmrNbBitrate( aBitrate ) )
        {
        __SUBCONTROLLER_INT1( 
            "MccCodecConfigurator::ChangeBitrateL, setting amr nb bitrate:", 
            aBitrate )
            
        User::LeaveIfError( speechEncConfIf->SetBitrate( aBitrate ) );
        }
    else if ( KMccFourCCIdAMRWB == fourccVal && IsSupportedAmrWbBitrate( aBitrate ) )
        {
        __SUBCONTROLLER_INT1( 
            "MccCodecConfigurator::ChangeBitrateL, setting amr wb bitrate:", 
            aBitrate )
            
        User::LeaveIfError( speechEncConfIf->SetBitrate( aBitrate ) );
        }
    else
        {
        __SUBCONTROLLER( "MccCodecConfigurator::ChangeBitrateL, bitrate not supported!" )
        User::Leave( KErrNotSupported );
        }

    CleanupStack::PopAndDestroy( speechEncConfIf );
    
    __SUBCONTROLLER( "MccCodecConfigurator::ChangeBitrateL, exit" )
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::ConvertBoolean
// Converts possibly dirty boolean parameter to EFalse/ETrue 
// -----------------------------------------------------------------------------
//
TBool MccCodecConfigurator::ConvertBoolean( TBool aParam )
    {
    // As VoIP settings may contain negative values (-1 especially) for marking
    // the setting as "Not set", we need to convert it into a real Symbian
    // style ETrue/EFalse boolean. Especially in -1 param cases, we want to
    // convert it into EFalse.
    if( aParam > 0 )
        {    
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::IsSupportedAmrNbBitrate
// -----------------------------------------------------------------------------        
TBool MccCodecConfigurator::IsSupportedAmrNbBitrate( TInt aBitrate )
    {
    return ( aBitrate == KAmrNbBitrate475 ||
             aBitrate == KAmrNbBitrate515 ||
             aBitrate == KAmrNbBitrate590 ||
             aBitrate == KAmrNbBitrate670 ||
             aBitrate == KAmrNbBitrate740 ||
             aBitrate == KAmrNbBitrate795 ||
             aBitrate == KAmrNbBitrate102 ||
             aBitrate == KAmrNbBitrate122 );
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::IsSupportedAmrWbBitrate
// -----------------------------------------------------------------------------
TBool MccCodecConfigurator::IsSupportedAmrWbBitrate( TInt aBitrate )
    {
    return ( aBitrate == KAmrWbBitrate660 ||
             aBitrate == KAmrWbBitrate885 ||
             aBitrate == KAmrWbBitrate1265 ||
             aBitrate == KAmrWbBitrate1425 ||
             aBitrate == KAmrWbBitrate1585 ||
             aBitrate == KAmrWbBitrate1825 ||
             aBitrate == KAmrWbBitrate1985 ||
             aBitrate == KAmrWbBitrate2305 ||
             aBitrate == KAmrWbBitrate2385 );
    }

// -----------------------------------------------------------------------------
// MccCodecConfigurator::IlbcNeedsStopResume
// -----------------------------------------------------------------------------
TBool MccCodecConfigurator::IlbcNeedsStopResume( TInt aOrigBitrate,
    TInt aRequBitrate, const TFourCC& aCodec )
    {
    const TFourCC tmp( KMccFourCCIdILBC );
    if ( tmp == aCodec && aOrigBitrate != aRequBitrate )
        {
        __SUBCONTROLLER( "MccCodecConfigurator::IlbcNeedsStopResume ETrue" )
        
        return ETrue;
        }
    else
        {
        __SUBCONTROLLER( "MccCodecConfigurator::IlbcNeedsStopResume EFalse" )
        
        return EFalse;
        }
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
