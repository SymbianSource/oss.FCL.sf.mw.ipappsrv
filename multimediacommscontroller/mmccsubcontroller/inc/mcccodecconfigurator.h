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
* Description:    Provides static configuration functions for different codecs
*
*/



#ifndef MCCCODECCONFIGURATOR_H
#define MCCCODECCONFIGURATOR_H

//  INCLUDES
#include <e32base.h>
#include "mccsymstreambase.h"

// FORWARD DECLARATIONS
class MMMFAudioInput;
class MMMFAudioOutput;
class MDataSink;
class MDataSource;
class TMccCodecInfo;
class CMMFFormatEncode;
class CMMFFormatDecode;
class CMccVideoSource;
class CMccVideoSink;
class CPayloadFormatWrite;
class CPayloadFormatRead;
class TMccFileSourceSetting;
class CMccRtpMediaClock;
class CMMFDevSound;
class CMccFileSink;

// CLASS DECLARATION

/**
*  Mcc Codec configuration static methods declaration
*
*  @lib Mccsubthread.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( MccCodecConfigurator )
    {
    public: // New functions

        /**
        * Generic codec configuration function.
        * @since Series 60 3.0
        * @param aCodec Codec information
        * @return void
        */
        static void ConfigureCodecL( MDataSource& aDatasource,
                                     MDataSink& aDatasink, 
                                     const TMccCodecInfo& aCodec,
                                     CMccRtpMediaClock& aClock,
                                     CMccSymStreamBase::TCodecState aCodecState,
                                     MDataSink* aAssociatedDatasink = 0 );
        
        /**
         * Changes the current encoder bitrate
         * @since Series 60 3.0
         * @param aCodec Codec info
         * @param aBitrate New bitrate
         * @param aSounddevice Reference to DevSound
         * @return void
         */
        static void ChangeBitrateL( const TMccCodecInfo& aCodec,
                                    TUint aBitrate, 
                                    CMMFDevSound& aSounddevice );
                                     
        /**
        * Gets default values of codec
        */
        static void GetCodecDefaultsL( const TUid aType,
        		                       TDes8& aParam, 
        		                       TMccCodecInfo& aCodec );
        
        /**
         * Checks whether codec is iLBC and if bitrate change needs a
         * codec stop/resume in order for the reconfiguration to succeed.
         * @since S60 v3.2
         * @param aOrigBitrate Original bitrate
         * @param aRequBitrate Requested bitrate
         * @param aCodec FourCC code of the codec
         * @return TBool ETrue if stop/resume is needed for bitrate change
         *               and codec is iLBC
         */
        static TBool IlbcNeedsStopResume( TInt aOrigBitrate,
            TInt aRequBitrate, const TFourCC& aCodec );
        
    private:
        
        /**
        * Configures a codec that is used for uplink/recording
        * @since Series 60 3.0
        * @param aRecordDevice MMF Recording device
        * @param aFormatter Encoder for record data
        * @param aCodec Codec parameters
        * @param aCodecState
        * @return void
        */
        static void DoConfigureLiveAudioUplinkStreamL(
        	MMMFAudioInput& aRecordDevice,
            CPayloadFormatWrite& aFormatter,
            const TMccCodecInfo& aCodec,
            CMccRtpMediaClock& aClock,
            CMccSymStreamBase::TCodecState aCodecState );
            
        /**
        * Configures a codec that is used for downlink/playback
        * @since Series 60 3.0
        * @param aPlayDevice MMF Playback device
        * @param aFormatter Decoder for playback data
        * @param aCodec Codec parameters
        * @param aCodecState
        * @return void
        */
        static void DoConfigureLiveAudioDownlinkStreamL(
        	MMMFAudioOutput& aPlayDevice,
            CPayloadFormatRead& aFormatter,
            const TMccCodecInfo& aCodec,
            CMccSymStreamBase::TCodecState aCodecState );
            
       /**
        * Configures a codec that is used for video downlink
        * @since Series 60 3.0
        * @param aPlayDevice MMF Playback device
        * @param aFormatter Decoder for playback data
        * @param aCodec Codec parameters
        * @return void
        */
        static void DoConfigureLiveVideoDownlinkStreamL(
            MDataSink* aDataSink,
            CPayloadFormatRead& aFormatter,
            const TMccCodecInfo& aCodec );

        /**
        * Configures a codec for display sink
        * @since Series 60 3.0
        * @param aCodec Codec parameters
        * @param aDatasink
        * @return void
        */
        static void DoConfigureDisplayL( 
                        const TMccCodecInfo& aCodec,
                        MDataSink* aDatasink );
    
        /**
        * Configures a codec that is used for uplink video from clip
        * @since Series 60 3.0
        * @param aFormatter Encoder for record data
        * @param aCodec Codec parameters
        * @param aAssociatedSink
        * @return void
        */
        static void DoConfigureClipUplinkStreamL(
            CPayloadFormatWrite& aFormatter,
            const TMccCodecInfo& aCodec,
            MDataSink* aAssociatedSink,
            CMccRtpMediaClock& aClock );

        /**
        * Configures a codec that is used for uplink live video
        * @since Series 60 3.0
        * @param aFormatter Encoder for record data
        * @param aCodec Codec parameters
        * @param aAssociatedSink
        * @return void
        */
        static void DoConfigureLiveVideoUplinkStreamL( 
        	CMccVideoSource& aDataSource,
            CPayloadFormatWrite& aFormatter,
            const TMccCodecInfo& aCodec,
            MDataSink* aAssociatedSink,
            CMccRtpMediaClock& aClock );
            
        /**
        * Configures the uplink AMR codec
        * @since Series 60 3.0
        * @param [input] aPlaybackDev MMF Audio datasink
         * @param [input] aCodec Codec info
        * @return void
        */
        static void DoConfigureAmrL( MMMFAudioOutput& aPlaybackDev, 
                                     const TMccCodecInfo&  );
        
        /**
        * Configures the downlink AMR codec
        * @since Series 60 3.0
        * @param [input] aRecordDev MMF Audio datasource
        * @param [input] aCodec Codec info
        * @return void
        */
        static void DoConfigureAmrL( MMMFAudioInput& aRecordDev,  
                                     const TMccCodecInfo& aCodec );

        /**
        * Configures the uplink G.711 codec
        * @since Series 60 3.0
        * @param [input] aDatasink MMF Audio datasink
        * @param [input] aCodec Codec parameters
        * @param aCodecState
        * @return void
        */
        static void DoConfigureG711L( MMMFAudioOutput& aPlaybackDev, 
            const TMccCodecInfo& aCodec,
            CMccSymStreamBase::TCodecState aCodecState );
        
        /**
        * Configures the downlink G.711 codec
        * @since Series 60 3.0
        * @param [input] aDatasource MMF Audio datasource
        * @param [input] aCodec Codec parameters
        * @param aCodecState
        * @return void
        */
        static void DoConfigureG711L( MMMFAudioInput& aRecordDev,
            const TMccCodecInfo& aCodec,
            CMccSymStreamBase::TCodecState aCodecState );
        
         /**
        * Configures the uplink G.729 codec
        * @since Series 60 3.0
        * @param aDatasink MMF Audio datasink
        * @param aBitrate Bitrate for the codec
        * @return void
        */
        static void DoConfigureG729( MMMFAudioOutput& aPlaybackDev );
        
        /**
        * Configures the downlink G.729 codec
        * @since Series 60 3.0
        * @param [input] aDatasource    MMF Audio datasource
        * @param [input] aEnableCng     Is comfort noise generation activated
        * @return void
        */
        static void DoConfigureG729L( MMMFAudioInput& aRecordDev,
                                      TBool aEnableCng );
        
        /**
        * Configures the uplink iLBC codec
        * @since Series 60 3.0
        * @param aDatasink MMF Audio datasink
        * @param aCodec Codec parameters
        * @param aCodecState
        * @return void
        */
        static void DoConfigureIlbcL( MMMFAudioOutput& aPlaybackDev, 
                const TMccCodecInfo& aCodec,
                CMccSymStreamBase::TCodecState aCodecState );
        
        /**
        * Configures the downlink iLBC codec
        * @since Series 60 3.0
        * @param aDatasource MMF Audio datasource
        * @param aCodec Codec parameters
        * @param aCodecState
        * @return void
        */
        static void DoConfigureIlbcL( MMMFAudioInput& aRecordDev, 
                const TMccCodecInfo& aCodec,
                CMccSymStreamBase::TCodecState aCodecState );
        
        /**
        * Configures the camera source
        * @since Series 60 3.0
        * @param [input] aVideoSource source to be configured
        * @param [input] aCodec Codec parameters
        * @return void
        */
        static void DoConfigureCameraL( CMccVideoSource& aVideoSource, 
                const TMccCodecInfo& aCodec );

        
        /**
        * Configures the file sink
        * @since Series 60 3.0
        * @param [input] aFileSink sink to be configured
        * @param [input] aCodec Codec parameters
        * @return void
        */
        static void DoConfigureFileSinkL( CMccFileSink& aFileSink, 
                const TMccCodecInfo& aCodec );
    
        /**
        * Configures a DTMF mediaformat.
        *
        * @since   S60 3.0
        * @param   aPayloadDecoder     Decoder for DTMF payload
        * @param   aCodec              Mediaformat parameters
        * @return  void
        */
        static void DoConfigureDtmfFormatL(
        	CPayloadFormatRead& aPayloadDecoder,
            const TMccCodecInfo& aCodec );
            
        /**
        * Configures a DTMF mediaformat.
        *
        * @since   S60 3.0
        * @param   aPayloadEncoder     Encoder for DTMF payload
        * @param   aCodec              Mediaformat parameters
        * @param   aClock              Mediaclock for timestamp synchronization
        * @return  void
        */
        static void DoConfigureDtmfFormatL(
            CPayloadFormatWrite& aPayloadEncoder,
            const TMccCodecInfo& aCodec,
            CMccRtpMediaClock& aClock );
        
        /**
        * Gets configures of the camera source
        */
		static void DoGetCameraConfigurationL( const TMccCodecInfo& aCodec, 
		                                       const TDesC8& aParam );
		                                       
		/**
        * Gets settings of the clip source
        */                                    
	    static void DoGetClipConfigurationL( TDes8& aParam );

        /**
        * Converts a "boolean" setting parameter to a real Symbian style
        * boolean.
        * @since Series 60 3.0
        * @param aParam     Boolean parameter to convert
        * @return TBool     Converted parameter
        */
        static TBool ConvertBoolean( TBool aParam );
	    
	    /**
	    * Checks whether bitrate is supported
	    */ 
	    static TBool IsSupportedAmrNbBitrate( TInt aBitrate );
	    
	    /**
	    * Checks whether bitrate is supported
	    */ 
	    static TBool IsSupportedAmrWbBitrate( TInt aBitrate );
	    
	    static void PassPrerollFromSinkToSourceL(MDataSink* aDataSink,  MDataSource* aDataSource);
		 
        
    private:    // Friend classes
        #if defined( TEST_EUNIT )
        friend class UT_CMccCodecConfigurator;
        #endif
    };

#endif      // MCCCODECCONFIGURATOR_H
            
// End of File
