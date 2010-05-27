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
* Description:    Media recorder API
*
*/




#ifndef CCMRMEDIARECORDER_H
#define CCMRMEDIARECORDER_H

//  INCLUDES
#include <e32base.h>
#include <CCMRMediaRecorderObserver.h>
#include <mmf/common/mmffourcc.h>
#include <mmf/devvideo/devvideobase.h> // STUB stuff

// STUB stuff

const TInt KMccTestRecorderDefaultBehavior = 1;
const TInt KMccTestRecorderKeyRetrievalBehavior = 2;


// CONSTANTS
const TInt KCMRUseDefault(-1);

// FORWARD DECLARATIONS
class MDataSource;
class MCMRMediaSink;
class TCCMRVideoCodingOptions;
class CDesC8Array;
class TMMFPrioritySettings;

// CLASS DECLARATION


/**
*  Abstract Media recorder interface
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class CCMRMediaRecorder : public CBase
    {    
    
    public:  // Constants
        
        // Recorder states
        enum TRecorderState
            {   
                EStateNone = 0,
                EStateOpen,
                EStatePreparing,
                EStateReadyToRecord,
                EStateRecording,
                EStatePaused,
                EStateStopping,
                EStatePausing
            };

    public:        

        /**
        * Two-phased constructor.
        */
        static CCMRMediaRecorder* NewL();
        
        // CBase provides a virtual destructor        

    public:  // New functions
        
        /**
        * Open the recorder: Creates A/V recorders & other needed components
        * @since 2.1
        * @param aObserver          Mediarecorder observer
        * @param aAudioSource       MMF audio source
        * @param aSink              MCMRMediaSink for data output
        * @param aCameraHandle      Camera handle
        * @param aVideoMimeType     Video codec MIME-type (default: KNullDesC8 => use default codec)
        * @param aAudioType         Audio coded FourCC (default: KFourCCNULL => use default codec)
        */
        virtual void OpenL(MCMRMediaRecorderObserver *aObserver, 
                           MDataSource* aAudioSource, 
                           MCMRMediaSink *aSink, TInt aCameraHandle,
                           const TDesC8& aVideoMimeType = KNullDesC8,
                           TFourCC aAudioType = KFourCCNULL) = 0;

        /**
        * Returns the current state of the recorder  
        * @since 2.1
        * @param void
        * @return TRecorderState The State
        */
        virtual TRecorderState State() = 0;
        
        // Settings

        /**
        * Set video codec to be used (overrides the one given in OpenL)
        * @since 2.1
        * @param  aVideoMimeType    video mime type
        * @return void
        */
        virtual void SetVideoCodecL( const TDesC8& aVideoMimeType ) = 0;

        /**
        * Get the currently used video codec
        * @since 2.1
        * @param  aVideoMimeType    Used codec as mime type
        * @return void
        */
        virtual void GetVideoCodecL( TDes8& aVideoMimeType ) = 0;

        /**
        * Get supported & installed video codecs
        * This can be called also when the recorder is not open
        * @since 2.1
        * @param  aVideoMimeTypes   List of supported & installed video codecs as mime type
        * @return void
        */
        virtual void GetSupportedVideoCodecsL( CDesC8Array& aVideoMimeTypes ) = 0;

        /**
        * Set audio codec to be used (overrides the one given in OpenL)
        * @since 2.1
        * @param  aAudioType        Audio codec as FourCC
        * @return void
        */
        virtual void SetAudioCodecL( const TFourCC& aAudioType ) = 0;

        /**
        * Get the currently used audio codec
        * @since 2.1
        * @param  aAudioType        Used codec as FourCC
        * @return void
        */
        virtual void GetAudioCodecL( TFourCC& aAudioType ) = 0;

        /**
        * Get supported & installed audio codecs
        * @since 2.1
        * @param  aAudioTypes   List of supported & installed audio codecs
        * @return void
        */
        virtual void GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes ) = 0;

        /**
        * Set new maximum total (audio+video) bitrate
        * @since 2.1
        * @param aBitRate New bitrate
        * @return void
        */
        virtual void SetMaxTotalBitRateL(TInt aBitRate) = 0;

        /**
        * Get current maximum total (audio+video) bitrate
        * @since 2.1
        * @param void
        * @return TInt Bitrate (-1 means max bitrate has not been set)
        */
        virtual TInt MaxTotalBitRateL() const = 0;

        /**
        * Set new video frame size. Possible only when not recording
        * @since 2.1
        * @param aSize New size
        * @return void
        */
        virtual void SetVideoFrameSizeL(const TSize& aSize) = 0;

        /**
        * Get current video frame size
        * @since 2.1
        * @param aSize Current size (Output)        
        * @return void
        */
        virtual void GetVideoFrameSizeL(TSize& aSize) const = 0;

        /**
        * Set new video frame rate (encoding and capturing). 
        * Possible to change also when recording but then 
        * affects only to encoding rate and cannot increase 
        * higher than capturing rate set in the initialization.
        * @since 2.1
        * @param aFrameRate New frame rate
        * @return void
        */
        virtual void SetVideoFrameRateL(TReal32 aFrameRate) = 0;

        /**
        * Get current video frame rate
        * @since 2.1
        * @param void
        * @return TReal32 Frame rate
        */
        virtual TReal32 VideoFrameRateL() = 0;        

        /**
        * Set new target video bitrate. Possible to change
        * also when recording.
        * @since 2.1
        * @param aBitRate Target bitrate in bps, or KMMFVariableVideoBitRate
        * @return void
        */
        virtual void SetVideoBitRateL(TInt aBitRate) = 0;

        /**
        * Get current target video bitrate
        * @since 2.1
        * @param void
        * @return TInt Target bitrate in bps, or KMMFVariableVideoBitRate
        */
        virtual TInt VideoBitRateL() = 0;

        /**
        * Set misc video coding options
        * @since 2.1
        * @param  aOptions      video coding options
        * @return void
        */
        virtual void SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions) = 0;

        /**
        * Set bitrate for audio codec. If we use AMR-NB, the given bitrate
        * is interpreted proprietarily: bitrates that are not exactly AMR bitrates 
        * mean that voice activity detection is used and the actual AMR bitrate is 
        * the given bitrate rounded upwards to the next AMR bitrate. 
        * E.g. aBitRate = 5500 means the AMR bit-rate is 5900 and VAD is enabled
        * @since 2.1
        * @param aBitRate New bitrate
        * @return void
        */
        virtual void SetAudioBitRateL(TInt aBitRate) = 0;

        /**
        * Gets the current target audio bitrate  
        * @since 2.1
        * @param void
        * @return TInt Bitrate
        */
        virtual TInt AudioBitRateL() = 0;

        /**
        * Toggles audio recording on/off
        *      NOTE: This cannot be used during recording!
        * @since 2.1
        * @param aEnabled If ETrue, audio will be recorded
        * @return void        
        */
        virtual void SetAudioEnabledL(TBool aEnabled) = 0;

        /**
        * Query whether audio recording is enabled
        * @since 2.1
        * @param void
        * @return TBool Result
        */
        virtual TBool AudioEnabledL() const = 0;
        
        /**
        * Set new audio recording input gain
        * @since 2.1
        * @param aGain New gain
        * @return void
        */
        virtual void SetGainL(TInt aGain) = 0;

        /**
        * Get current audio recording gain
        * @since 2.1
        * @param void
        * @return TInt Gain
        */
        virtual TInt GainL() const = 0;

        /**
        * Get maximum audio recording gain
        * @since 2.1
        * @param void
        * @return TInt Max. gain value
        */
        virtual TInt MaxGainL() const = 0;   
        
        /**
        * Set audio priority settings
        * @since 2.1
        * @param  aPrioritySettings audio priority settings
        * @return void
        */
        virtual void SetAudioPriorityL(const TMMFPrioritySettings& aPrioritySettings) = 0;


        // Recording control
        
        /**
        * Prepares the recorder, for minimising latency in starting
        * @since 2.1
        * @param void
        * @return TInt Max. gain value
        */
        virtual void PrepareL() = 0;

        /**
        * Starts recording audio and video        
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void RecordL() = 0;

        /**
        * Stops recording audio and video
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void StopL() = 0;

        /**
        * Pauses recording
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void PauseL() = 0;

        /**
        * Resumes recording from Pause
        * @since 2.1
        * @param void
        * @return void
        */
        virtual void ResumeL() = 0;
        
        // Dynamic settings during recording

        /**
        * Sets the video bitrate control options for a whole bitstream.
        * This method can only be called after the media recorder has been prepared or during recording. 
        *
        * @since 3.2
        * @param	"aOptions"	"Bitrate control options."
        * @return void
        */
        virtual void SetVideoRateControlOptionsL(const TRateControlOptions& aOptions) = 0;
        
        /**
        * Gets current video bit-rate control options for a whole bitstream.
        * This method can only be called after the media recorder has been prepared or during recording. 
        *
        * @since 3.2
        * @param	"aOptions"	"Current bitrate control options."
        * @return void
        */
        virtual void GetVideoRateControlOptionsL(TRateControlOptions& aOptions) = 0;
        
        // Video encoder overrides

        /**
        * Set video encoder using its UID. Usage optional.
        * This overrides Media Recorder internal search for encoder based on set video mime type ( SetVideoCodecL() ).
        *
        * @since 3.2.3
        * @param    "aEncoder"  "Video encoder UID."
        * @return void
        */
        virtual void SetPreferredVideoEncoderL(TUid& aEncoder) = 0;

        /**
        * Set video encoder output format encapsulation. Usage optional.
        * This overrides Media Recorder internal default preferences for TVideoDataUnitEncapsulation
        * if used encoder supports multiple encapsulations.
        *
        * Defaults are:
        *   H.263 and MPEG-4 part 2 : EDuElementaryStream
        *               H.264 / AVC : EDuGenericPayload
        *
        * @since 3.2.3
        * @param    "aCapsulation"  "Encapsulation for coded video data units."
        * @return void
        */
        virtual void SetPreferredVideoEncapsulationL(TVideoDataUnitEncapsulation aCapsulation) = 0;

    // STUB STUFF
    
        virtual void SetCurrentState( TRecorderState aState ) = 0;
               
    };


/**
*  Video coding options used in SetVideoCodingOptionsL
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class TCCMRVideoCodingOptions
    {
    public:
        TCCMRVideoCodingOptions() : iSyncIntervalInPicture(0), iMinRandomAccessPeriodInSeconds(KCMRUseDefault), iExpectedBitErrorRate(0.0), iExpectedPacketLossRate(0.0)
            {}

    public:
        // Segment interval in picture. In H.263 baseline this means number of non-empty GOB headers (1=every GOB has a header), 
        // in MB-based systems number of MBs per segment. Default is 0 == no segments inside picture
        // Coding standard & used profile etc. limit the value.
        TInt              iSyncIntervalInPicture;
        // Time between random access points
        TInt              iMinRandomAccessPeriodInSeconds;
        // Bit error rate; if nonzero sets error resilience coding options (e.g. Adaptive Intra Refresh) on
        TReal             iExpectedBitErrorRate;
        // Packet loss rate; if nonzero sets error resilience coding options (e.g. Adaptive Intra Refresh) on
        TReal             iExpectedPacketLossRate;
    };



// STUB STUFF

class CCMRMediaRecorderStub : public CCMRMediaRecorder
    {    
    
    public:
    
    enum TCMRTestFunction
        {
        ETestNone,
        ETestPrepare,
        ETestRecord,
        ETestStop,
        ETestPause,
        ETestResume,
        ETestRateControl,
        ETestSetRateControl,
        ETestPreferredEncoder,
        ETestPreferredEncapsulation
        };
    
    CCMRMediaRecorderStub();
    
    ~CCMRMediaRecorderStub();
    
    void OpenL(MCMRMediaRecorderObserver *aObserver, 
                           MDataSource* aAudioSource, 
                           MCMRMediaSink *aSink, TInt aCameraHandle,
                           const TDesC8& aVideoMimeType = KNullDesC8,
                           TFourCC aAudioType = KFourCCNULL);

    TRecorderState State();
        
    void SetVideoCodecL( const TDesC8& aVideoMimeType );

    void GetVideoCodecL( TDes8& aVideoMimeType );

    void GetSupportedVideoCodecsL( CDesC8Array& aVideoMimeTypes );

    void SetAudioCodecL( const TFourCC& aAudioType );

    void GetAudioCodecL( TFourCC& aAudioType );

    void GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes );

    void SetMaxTotalBitRateL(TInt aBitRate);

    TInt MaxTotalBitRateL() const;

    void SetVideoFrameSizeL(const TSize& aSize);

    void GetVideoFrameSizeL(TSize& aSize) const;

    void SetVideoFrameRateL(TReal32 aFrameRate);

    TReal32 VideoFrameRateL();        

    void SetVideoBitRateL(TInt aBitRate);

    TInt VideoBitRateL();

    void SetVideoCodingOptionsL(const TCCMRVideoCodingOptions& aOptions);

    void SetAudioBitRateL(TInt aBitRate);

    TInt AudioBitRateL();

    void SetAudioEnabledL(TBool aEnabled);

    TBool AudioEnabledL() const;
        
    void SetGainL(TInt aGain);

    TInt GainL() const;

    TInt MaxGainL() const;   
        
    void SetAudioPriorityL(const TMMFPrioritySettings& aPrioritySettings);

    void PrepareL();

    void RecordL();

    void StopL();

    void PauseL();

    void ResumeL();
    
    void SetVideoRateControlOptionsL(const TRateControlOptions& aOptions);
        
    void GetVideoRateControlOptionsL(TRateControlOptions& aOptions);
    
    void SetPreferredVideoEncoderL(TUid& aEncoder);

    void SetPreferredVideoEncapsulationL(TVideoDataUnitEncapsulation aCapsulation);
        
    // STUB STUFF
    
    void SetCurrentState( TRecorderState aState );
    
    static void SetBehavior( TInt aBehavior, TInt aError );
 
public:

    TRecorderState iCurrentState;   
    
    TCMRTestFunction iLastFunctionCall;
    
    TInt iVideoBitrate;
    
    TRateControlOptions iRateControlOptions;
    
    MCMRMediaRecorderObserver* iObserver;
    
    MCMRMediaSink* iSink;
    
    TUid iEncoderUid;
    
    TVideoDataUnitEncapsulation iEncapsulation;
    
    };

#endif // CCMRMEDIARECORDER_H

// End of file
