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





#ifndef CMCEMEDIAMANAGER_H
#define CMCEMEDIAMANAGER_H

//  INCLUDES
#include <e32base.h>
#include <e32cmn.h>
#include "mcemediadefs.h"
#include "mcedefs.h"
#include "mcesip.h"
#include "mcesrv.h"
#include "mcecomfilesource.h"
#include "mcesrvstream.h"
#include "mcesecuremediasession.h"

// FORWARD DECLARATIONS
class MMceMediaGeneralObserver;
class CMceComSession;
class CMceComAudioStream;
class CMceComAudioCodec;
class CMceComMediaStream;
class CMceSdpSession;
class CSdpDocument;
class CMceComMediaSink;
class CMceComMediaSource;
class CMceComRtpSource;
class CMceComRtpSink;
class CMceComSpeakerSink;
class CMceComDisplaySink;
class CMceComCameraSource;
class CMceComAMRCodec;
class CSdpMediaField;
class CMceSdpCodec;
class RStringF;
class CMccInterface;
class CMccSecureInterface;
class CMceNatPluginManager;
// Constants

const TInt KRtcpRR = 1;
const TInt KRtcpSR = 2;
const TInt KRtcpUndefined = 3;



/**
*  Media negotiator for encoding and decoding SDP according to 
*  offer/answer model defined in RFC 3264 and managing the MCC interface
*/
class CMceMediaManager : public CBase, public MMccCtrlObserver
    {
        
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMceMediaManager* NewL( MMceMediaGeneralObserver& aObserver );

        /**
        * Two-phased constructor.
        */
        static CMceMediaManager* NewLC( MMceMediaGeneralObserver& aObserver );
        
        /**
        * Destructor.
        */
        ~CMceMediaManager();
        

    public: // new API for server

        CMceNatPluginManager& NatPluginManager();
        
        CSdpDocument* EncodeL( CMceComSession& aSession );

        TMceSipWarningCode DecodeL( CMceComSession& aSession,
                                    CSdpDocument& aSdpDocument );

        TMceReturnStatus UpdateL( CMceComSession& aSession );
                      
        TMceReturnStatus ReserveL( CMceComSession& aSession, const TDesC8& aToken );

        TBool ReservationNeeded( CMceComSession& aSession );

        TBool NeedToNegotiate( CMceComSession& aSession, CSdpDocument* aSdpDocument = NULL );

        TBool ConfirmationNeeded( CMceComSession& aSession );
        
        TMceNegotiationRole GetRole( CMceComSession& aSession );

        
    public: // API for server
        
        
        /**
        * Creates a rejected SDP answer with all media lines set to 0. 
        *
        * @param  aSession, server session
        * @return none.
        */
        void CreateRejectedAnswer( CMceComSession& aSession );
        
        /**
        * Pauses the sink 
        *
        * @param  aSink, server sink 
        * @leaves with a system-wide error code
        */
        void PauseL( CMceComMediaSink& aSink );
        
        /**
        * Pauses the source 
        *
        * @param  aSource, server source
        * @leaves with a system-wide error code
        */
        void PauseL( CMceComMediaSource& aSource );
        
        /**
        * Resumes the sink
        *
        * @param  aSink, server sink 
        * @leaves with a system-wide error code
        */
        void ResumeL( CMceComMediaSink& aSink );
        
        /**
        * Stops the source
        *
        * @param  aSource, server source 
        * @leaves with a system-wide error code
        */
        void ResumeL( CMceComMediaSource& aSource );
        
        /**
        * Closes the session
        *
        * @param  aSession, server session
        * @return One of the standard system-wide error codes.
        */
        TInt CloseSession( CMceComSession& aSession );
        
        /**
        * Starts inactivity timer for the given RTP source
        * @param aRtpSource
        * @param aInactivityTimeout inactivity timeout
        * @leaves with a system-wide error code
        */
        void StartInactivityTimerL( const CMceComRtpSource& aRtpSource,
                                    TUint32 aInactivityTimeout );

        /**
        * Stops inactivity timer for the given RTP source
        * @param aRtpSource
        * @leaves with a system-wide error code
        */
        void StopInactivityTimerL( const CMceComRtpSource& aRtpSource );
        
        /**
        * Sends RTCP any data
        * @param aMediaStream, stream 
        * @param aData, data payload   
        * @leaves with a system-wide error code
        */
        void SendRTCPAnyDataL( const CMceComMediaStream& aMediaStream, 
                              const TDesC8& aData );

        /**
        * Sends RTCP receiver report to uplink if possible.
        * @param aRtpSink, rtp source
        * @leaves with a system-wide error code
        */
        void SendRTCPReceiverReportL( const CMceComRtpSource& aRtpSource );

        /**
        * Sends RTCP sender report to uplink if possible.
        * @param aRtpSink, rtp sink
        * @leaves with a system-wide error code
        */
        void SendRTCPSenderReportL( const CMceComRtpSink& aRtpSink );
        
        /**
        * Gets current maximum volume supported.
        * @param aMaxVolume - [output] maximum volume supported
        * @leaves with a system-wide error code
        */
        void GetMaxVolumeL( TInt& aMaxVolume );
                           
        /**
        * Gets current volume.
        * @param aSink, sink
        * @param aVolume - [output] After completion, current volume
        * @leaves with a system-wide error code
        */
        void GetVolumeL( const CMceComMediaSink& aSink, 
                        TInt& aVolume );
        
        /**
        * Sets playback volume.
        * @param aSink, sink
        * @param aVolume - [input] Volume value to set
        * @leaves with a system-wide error code
        */
        void SetVolumeL( const CMceComMediaSink& aSink, 
                         TInt aVolume );
        

        /**
        * Gets audio routing.
        * @param aSpeaker - [input] speaker sink
        * @param aRoutingDestination on return contains destination where audio
        *        is currently routed. Usable values are defined in
        *        mmccinterfacedef.h (e.g. KMccAudioToPublic).
        * @leaves with a system-wide error code
        */
        void GetAudioRoutingL( const CMceComSpeakerSink& aSpeaker, 
                               TInt& aRoutingDestination );

        /**
        * Sets audio routing.
        * @param aSpeaker - [input] speaker sink
        * @param aRoutingDestination destination into which audio is routed.
        *        Usable values are defined in mmccinterfacedef.h 
        *        (e.g. KMccAudioToPublic).
        */
        void SetAudioRoutingL( const CMceComSpeakerSink& aSpeaker, 
                               TInt aRoutingDestination );
        
                              
        /**
        * Sets balance of a given sink.
        * @param aSink, sink
        * @param aBalance - [input] Balance value to set
        * @leaves with a system-wide error code
        */
        void SetBalanceL( const CMceComMediaSink& aSink, 
                         TInt aBalance );

        /**
        * Sets balance of a given source.
        * @param aSource, source
        * @param aBalance - [input] Balance value to set
        * @leaves with a system-wide error code
        */
        void SetBalanceL( const CMceComMediaSource& aSource, 
                         TInt aBalance );

        /**
        * Gets current gain used in recording.
        * @param aSource, source
        * @param aGain - [output] After completion, current gain
        * @leaves with a system-wide error code
        */
        void GetGainL( const CMceComMediaSource& aSource, 
                      TInt& aGain );

        /**
        * Gets current maximum gain supported.
        * @param aMaxGain - [output] After completion, current gain
        * @leaves with a system-wide error code
        */
        void GetMaxGainL( TInt& aMaxGain );

        /**
        * Sets uplink/record gain to all ongoing sessions.
        * @param aSource, source
        * @param aGain - [input] Gain value to set
        * @leaves with a system-wide error code
        */
        void SetGainL( const CMceComMediaSource& aSource,
                      TInt aGain );


        /**
        * Gets number of displays available
        * @param aDisplaysAvailable, return value
        * @return One of the standard system-wide error codes.
        */
        void GetDisplaysAvailableL( TInt& aDisplaysAvailable );

        /**
        * Gets display size
        * @param aSink, sink
        * @param aDisplayRect, return value
        * @leaves with a system-wide error code
        */
        void GetDisplayRectL( CMceComDisplaySink& aSink, 
                             TRect& aDisplayRect );

        /**
        * Sets display size
        * @param aSink, sink
        * @param aDisplayRect, input value
        * @leaves with a system-wide error code
        */
        void SetDisplayRectL( CMceComDisplaySink& aSink, 
                             TRect aDisplayRect );

        /**
        * Gets display priority
        * @param aSink, sink
        * @param aDisplayPriority, return value
        * @leaves with a system-wide error code
        */
        void GetDisplayPriorityL( CMceComDisplaySink& aSink, 
                                 TInt& aDisplayPriority );
                                 

        /**
        * Gets display rotation
        * @param aSink, sink
        * @param aDisplayRotation, return value
        * @leaves with a system-wide error code
        */
        void GetDisplayRotationL( CMceComDisplaySink& aSink, 
                                  TInt& aDisplayRotation );

        /**
        * Sets display priority
        * @param aSink, sink
        * @param aDisplayPriority, input value
        * @leaves with a system-wide error code
        */
        void SetDisplayPriorityL( CMceComDisplaySink& aSink, 
                                 TInt aDisplayPriority );

        /**
        * Sets display index
        * @param aSink, sink
        * @param aDisplayIndex, input value
        * @leaves with a system-wide error code
        */
        void SetDisplayIndexL( CMceComDisplaySink& aSink, 
                              TInt aDisplayIndex );

        /**
        * Sets display rotation
        * @param aSink, sink
        * @param aDisplayRotation, input value
        * @leaves with a system-wide error code
        */
        void SetDisplayRotationL( CMceComDisplaySink& aSink, 
                                  TInt aDisplayRotation );
         

        /**
        * Gets camera info
        * @param aCameraIndex, input value
        * @param aCameraInfo, return value
        * @leaves with a system-wide error code
        */
        void GetCameraInfoL( TInt aCameraIndex, 
                             TCameraInfo& aCameraInfo );

        /**
        * Gets available cameras
        * @param aCamerasAvailable, return value
        * @leaves with a system-wide error code
        */
        void GetCamerasAvailableL( TInt& aCamerasAvailable );

        /**
        * Gets zoom factor of camera
        * @param aSource, source
        * @param aZoomFactor, return value
        * @leaves with a system-wide error code
        */
        void GetZoomFactorL( CMceComCameraSource& aSource, 
                             TInt& aZoomFactor );

        /**
        * Gets digital zoom factor of camera
        * @param aSource, source
        * @param aDigitalZoomFactor, return value
        * @leaves with a system-wide error code
        */
        void GetDigitalZoomFactorL( CMceComCameraSource& aSource, 
                                    TInt& aDigitalZoomFactor );

        /**
        * Gets contrast of camera
        * @param aSource, source
        * @param aContrast, return value
        * @leaves with a system-wide error code
        */
        void GetContrastL( CMceComCameraSource& aSource, 
                           TInt& aContrast );

        /**
        * Gets brightness of camera
        * @param aSource, source
        * @param aBrightness, return value
        * @leaves with a system-wide error code
        */
        void GetBrightnessL( CMceComCameraSource& aSource, 
                             TInt& aBrightness );

        /**
        * Gets exposure of camera
        * @param aSource, source
        * @param aExposure, return value
        * @leaves with a system-wide error code
        */
        void GetExposureL( CMceComCameraSource& aSource, 
                           TInt& aExposure );

        /**
        * Gets white balance of camera
        * @param aSource, source
        * @param aWhiteBalance, return value
        * @leaves with a system-wide error code
        */
        void GetWhiteBalanceL( CMceComCameraSource& aSource, 
                               TInt& aWhiteBalance );

        /**
        * Sets camera index of camera
        * @param aSource, source
        * @param aCameraIndex, input value
        * @param aCameraInfo, return value
        * 
        * @leaves with a system-wide error code
        */
        void SetCameraIndexL( CMceComCameraSource& aSource, 
                              TInt aCameraIndex,
                              TCameraInfo& aCameraInfo );

        /**
        * Sets zoom factor of camera
        * @param aSource, source
        * @param aZoomFactor, input value
        * @leaves with a system-wide error code
        */
        void SetZoomFactorL( CMceComCameraSource& aSource, 
                             TInt aZoomFactor );

        /**
        * Sets digital zoom factor of camera
        * @param aSource, source
        * @param aDigitalZoomFactor, input value
        * @leaves with a system-wide error code
        */
        void SetDigitalZoomFactorL( CMceComCameraSource& aSource, 
                                   TInt aDigitalZoomFactor );

        /**
        * Sets contrast of camera
        * @param aSource, source
        * @param aContrast, input value
        * @leaves with a system-wide error code
        */
        void SetContrastL( CMceComCameraSource& aSource, 
                           TInt aContrast );

        /**
        * Sets brightness of camera
        * @param aSource, source
        * @param aBrightness, input value
        * @leaves with a system-wide error code
        */
        void SetBrightnessL( CMceComCameraSource& aSource, 
                             TInt aBrightness );

        /**
        * Sets exposure of camera
        * @param aSource, source
        * @param aExposure, input value
        * @leaves with a system-wide error code
        */
        void SetExposureL( CMceComCameraSource& aSource, 
                           TInt aExposure );

        /**
        * Sets white balance of camera
        * @param aSource, source
        * @param aWhiteBalance, input value
        * @leaves with a system-wide error code
        */
        void SetWhiteBalanceL( CMceComCameraSource& aSource, 
                               TInt aWhiteBalance );

        /**
        * Gets file info
        * @param aSource, source
        * @param aFileInfo, return value
        * @leaves with a system-wide error code
        */
        void GetFileInfoL( CMceComFileSource& aSource, 
                           TMceFileInfo& aFileInfo );
                           

        /**
        * Gets file info
        * @param aFileInfo, return value
        * @leaves with a system-wide error code
        */
       void GetFileInfoL( TMceFileInfo& aFileInfo );


        /**
        * Sets file info
        * @param aSource, source
        * @param aFileInfo, return value
        * @leaves with a system-wide error code
        */
       void SetFileInfoL( CMceComFileSource& aSource, 
                          TMceFileInfo& aFileInfo );
                           

        /**
        * Gets file position
        * @param aSource, source
        * @param aFilePosition, return value
        * @leaves with a system-wide error code
        */
        void GetFilePositionL( CMceComFileSource& aSource, 
                               TTimeIntervalMicroSeconds& aFilePosition );

        /**
        * Sets file position
        * @param aSource, source
        * @param aFilePosition, input value
        * @leaves with a system-wide error code
        */
        void SetFilePositionL( CMceComFileSource& aSource, 
                               TTimeIntervalMicroSeconds& aFilePosition );

        /**
        * Sets FF 
        * @param aSource, source
        * @param aOnOff, inout value
        * @leaves with a system-wide error code
        */
        void FastForwardL( CMceComFileSource& aSource, 
                           TBool& aOnOff );

        /**
        * Sets RW 
        * @param aSource, source
        * @param aOnOff, inout value
        * @leaves with a system-wide error code
        */
        void FastRewindL( CMceComFileSource& aSource, 
                          TBool& aOnOff );


        /**
        * Transcodes file
        * @param aFile, source
        * @return transcoding session id
        * @leaves with a system-wide error code
        */
        TUint32 TranscodeFileL( CMceComFileSource* aFile );

        /**
        * Cancels transcoding
        * @param aSessionId, transcoding session id
        * @leaves with a system-wide error code
        */
        void CancelTranscodeFileL( TUint32 aSessionId );

        /**
        * Get supported codecs.
        * @param aCodecs - [output] An array to be filled with FourCC codes
        * @return One of the standard system-wide error codes.
        */
        TInt GetSupportedCodecs( RArray<TFourCC>& aCodecs );
                                
        /**
        * Returns array of audio capabilities in aCapabilities. Note that data that
        * the aCapabilities contains before calling this function will be
        * lost during the excecution of this function. Note also that the CLIENT
        * is responsible for the destruction of the objects contained in the
        * parameter array.
        * @param aCapabilities Codec capabilities
        * @return None
        */
        void GetCapabilitiesL( RPointerArray<CMceComAudioCodec>& aCapabilities );

        /**
        * Returns array of video capabilities in aCapabilities. Note that data that
        * the aCapabilities contains before calling this function will be
        * lost during the excecution of this function. Note also that the CLIENT
        * is responsible for the destruction of the objects contained in the
        * parameter array.
        * @param aCapabilities Codec capabilities
        * @return None
        */
        void GetCapabilitiesL( RPointerArray<CMceComVideoCodec>& aCapabilities );
        
        /**
        * Handles dtmf sending.
        * @param aMediaSource
        * @param aEvent
        * @param aSequence
        * @return None
        */
        void DtmfL( CMceComMediaSource& aSource, 
                    TMceComEvent& aEvent, 
                    const TDesC8& aSequence = KNullDesC8 );

        /**
        * Sets CName used in RTP session
        * @param aEndpoint
        * @param aCName
        */
        void SetCNameL( const CMceComEndpoint& aEndpoint, const TDesC8& aCName );
        
        /**
        * Updates configuration key fetched from camera to all endpoints
        * within the same session which might need configuration key information,
        * @param aSource
        */
        void UpdateConfigKeyL( const CMceComMediaSource& aSource );
        void UpDateStreamStateL(CMceComSession& currentSession,
        		                    CMceComSession& forkSession );
        void ForceEnableSinkState(CMceComSession& aSession );

    private: // 

        /**
        * Callback function to receive media link created prepared event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aNetSettings network settings
        */
        void MccMediaLinkCreated( TUint32 aSessionId,
                                  TUint32 aLinkId,
                                  const TDesC8& aNetSettings );


    private: // from MMccCtrlObserver
  
        /**
        * Callback function to receive media signalling
        * @param aEvent Received media signal
        */
        void MccEventReceived( const TMccEvent& aEvent );
        

        /**
        * Callback function to receive media prepared event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        * @param aSinkSourceId Sink or source ID
        */
        void MccMediaPrepared( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId );
                                       
        /**
        * Callback function to receive media started event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        * @param aSinkSourceId Sink or source ID
        */
        void MccMediaStarted( TUint32 aSessionId,
                                      TUint32 aLinkId,     
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId );
        
        /**
        * Callback function to receive media stopped event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        * @param aSinkSourceId Sink or source ID
        */
        void MccMediaStopped( TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId );

        /**
        * Callback function to receive media paused event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        * @param aSinkSourceId Sink or source ID
        */
        void MccMediaPaused( TUint32 aSessionId,
                                     TUint32 aLinkId,
                                     TUint32 aStreamId,
                                     TUint32 aSinkSourceId );

        /**
        * Callback function to receive media resumed event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        * @param aSinkSourceId Sink or source ID
        */
        void MccMediaResumed( TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId );

 
        /**
        * Callback function to receive media inactivity event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        * @param aSinkSourceId Sink or source ID
        */
        void MccMediaInactive( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId );

        /**
        * Callback function to receive media activity event. The
        * function is called when the first RTP packet is received
        * by a downlink stream.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        * @param aSinkSourceId Sink or source ID
        */
        void MccMediaActive( TUint32 aSessionId,
                                     TUint32 aLinkId,
                                     TUint32 aStreamId,
                                     TUint32 aSinkSourceId );

        /**
        * Callback function to receive error event.
        * @param aError Error code
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aSinkSourceId Sink or source ID
        */
        void MccCtrlError( TInt aError,
                                   TUint32 aSessionId,
                                   TUint32 aLinkId,
                                   TUint32 aStreamId,
                                   TUint32 aSinkSourceId );

        
    public: // new functions
        
                
        CMceSdpSession& CreateSessionL( CMceComSession& aSession );
        CMceSdpSession* SdpSession( CMceComSession& aSession );
        
        void CreateMccSessionL( CMceComSession& aSession );
        
        void InitializeMccStreamL( CMceSrvStream& aStream );
        void CreateMccLinkL( CMceSrvStream& aStream );
        void CreateMccStreamL( CMceSrvStream& aStream, 
                               TMceNegotiationRole aRole );
        void PrepareMccStreamL( CMceSrvStream& aStream, 
                               TMceNegotiationRole aRole );
        void StartMccStreamL( CMceSrvStream& aStream );
        void AdjustMccStreamL( CMceSrvStream& aStream );
        void ValidateMccStreamL( CMceSrvStream& aStream );
        void StopMccStream( CMceSrvStream& aStream );
    
        void SetRemoteAddressL( CMceSrvStream& aStream );

        void PrepareUpdateL( CMceSdpSession& aSession );

        void CleanupL( CMceComSession& aSession );
        
        void DisposeMediaStreamL( CMceComMediaStream* aStream );
        void DisposeCodecL( CMceComCodec* aCodec );
        

        CMceSdpCodec* SdpCodec( const CSdpMediaField& aMedia );
        CMceSdpCodec* SdpCodec( CMceComMediaStream& aStream );
        
        void SynchronizeMccStreamL( CMceSrvStream& aStream, 
                                    TMceNegotiationRole aRole,
                                    TBool aSyncCodecOnly = EFalse );
                                    
        void SynchronizeMccStreamL( CMceSrvStream& aStream, 
                                    TUint32 aForceEnableEndpoint = 0, 
                                    TUint32 aForceDisableEndpoint = 0 );

        TUint32 GetSSRCL( CMceSrvStream& aStream );
        void EnableViewFinderL( const CMceComMediaSink& aMediaSink, const TDesC8& aParam );
        void DisableViewFinderL( const CMceComMediaSink& aMediaSink );
                
        TBool ReuseLink( CMceSrvStream& aStream );
        TBool ReuseSource( CMceSrvStream& aStream );
        TBool ReuseSink( CMceSrvStream& aStream );

        TBool CanReleaseEndpoint( CMceSrvStream& aStream, TUint32 aEndpointId );
        TInt ReleaseSink( CMceSrvStream& aStream, TUint32 aEndpointId );
        TInt ReleaseSource( CMceSrvStream& aStream, TUint32 aEndpointId );
        

        void SetMccParameterL( TUint32 aParam,
                               const TDesC8& aValue ) const;
        
        void GetMccParameterL( TUint32 aParam,
                               TDes8& aValue ) const;
                              
        void SetMccParameterL( const CMceComMediaSink& aMediaSink,
                              TUint32 aParam,
                              const TDesC8& aValue,
                              TBool aAllowNonPrepared = EFalse ) const;
        void SetMccParameterL( const CMceComMediaSource& aMediaSource,
                              TUint32 aParam,
                              const TDesC8& aValue,
                              TBool aAllowNonPrepared = EFalse ) const;
        void GetMccParameterL( const CMceComMediaSink& aMediaSink,
                              TUint32 aParam,
                              TDes8& aValue,
                              TBool aAllowNonPrepared = EFalse ) const;
        void GetMccParameterL( const CMceComMediaSource& aMediaSource,
                              TUint32 aParam,
                              TDes8& aValue,
                              TBool aAllowNonPrepared = EFalse ) const;
        
        void UpdateMccSourceL( CMceComCameraSource& aMediaSource );
        
        
    private: // for private usage only

        CMccCodecInformation* CreateMccCodecLC( CMceSrvStream& aStream,
                                                TMceNegotiationRole aRole );


        void UpdateAcceptedL( CMceSdpSession& aSdpSession );
        
        void UpdateRejectedL( CMceSdpSession& aSdpSession );
        

        TInt RequireSignallingL( CMceComSession& aSession,
                                  CMceComSession& aBackup );
                                 
    

        void CleanupSessionL( CMceComSession& aSession );
    

        /** 
        * Gets Sdp session
        * @param aMccSessionId.
        * @return session if found, NULL otherwise
        */
        CMceComSession* Session( TUint32 aSessionId );
        
        /** 
        * Removes corresponding Sdp session(s), which is no longer needed
        */
        void RemoveSession( CMceSdpSession* sdpSession );
        
        CMceComFileSource* TranscodingSession( const CMceComFileSource& aFile ) const;

        CMceComFileSource* TranscodingSession( TUint32 aTcSessionId ) const;
   
       
        void MccTranscodeEventReceived( const TMccEvent& aEvent );
        
        TBool UseRtcp( CMceSrvStream& aStream );
        
        void FillMceFileInfoL( TMccFileSourceSetting& aMccFileInfo,
                               TMceFileInfo& aMceFileInfo ) const;
                               
        TBool DoStartEndpointsL( CMceSrvStream& aStream, 
                                 TUint32 aForceEnableEndpoint,
                                 TUint32 aForceDisableEndpoint );
                               
        TBool DoResumeEndpointsL( CMceSrvStream& aStream, TUint32 aForceEnableEndpoint );
       
        TBool DoPauseEndpointsL( CMceSrvStream& aStream, 
                                 TUint32 aForceDisableeEndpoint );
                                
        void SetPendingState( CMceSrvStream& aStream, 
                              TUint32 aEndpointId, 
                              CMceSrvStream::TState aState );
                            
        TBool DoDtmfL( CMceComMediaSource& aSource, 
                       TMceComEvent& aEvent,
                       const TDesC8& aSequence,
                       TBool aUseBoundStream = EFalse );
        
        CMceComCodec* DoAdjustMccStreamL( CMceSrvStream& aStream );
        
        void DoConfigKeyUpdateL( CMceComMediaStream& aStream, HBufC8* aConfigKey );
        
        void CreateTranscodingOutputCodecsL( CMccCodecInformation** aAudioCodec ,
        								    CMccCodecInformation** aVideoCodec, 
        								    CMceComFileSource* aFile );
                
    private:
    
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( MMceMediaGeneralObserver& aObserver );
                                                    
        
        /**
        * C++ default constructor
        */
        CMceMediaManager();

	public :   // Data
		
		CMccSecureInterface* iMccSecureInterface; 

    private:    // Data
    
        // Pointer to MCCInterface, owned
        CMccInterface* iMccInterface; 
        
        // Pointer to CMceNatPluginManager, owned
        CMceNatPluginManager* iNatPluginManager;

        RPointerArray<CMceSdpSession> iMceSdpSessions;
        
        MMceMediaGeneralObserver* iObserver;

        // List of SDP codecs
        RPointerArray<CMceSdpCodec> iSdpCodecs;
        
        RPointerArray<CMceComFileSource> iTranscodeSessions;

        

    private:
    
        friend class CMceSdpSession;
        
    	//definitions for unit testing
    	MCEMM_UT_DEFINITIONS
    	friend class UT_CMceSecureDesStream;
    	friend class UT_CMceSecureMediaSession;
    };

#endif      // CMCEMEDIAMANAGER_H   
            
// End of File
