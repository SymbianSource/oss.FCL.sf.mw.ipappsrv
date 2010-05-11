/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCCVIDEOSINKIMPL_H
#define MCCVIDEOSINKIMPL_H

// INCLUDES
#include <e32std.h>
#include <w32std.h>
#include <in_sock.h>
#include <es_sock.h>
#include <mmf/common/mmfcontrollerframeworkbase.h>
#include <videoplayer.h>

#include "rtpdef.h"
#include "rtpheader.h"
#include "CXPSPktSinkObserver.h"
#include "mmccevents.h"
#include "mmccinterfacedef.h"

// Minimun packet size allowed, smaller packets are dropped.
const TInt KMccMinPacketSize = 4;


// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CVideoPlayerUtility2;
class MVideoLoadingObserver;
class MVideoPlayerUtilityObserver;
class TRtpSdesParams;
class TSetupParameters;
class RWsSession;
class RWindowGroup;
class RWindow;
class CWsScreenDevice;
class TMccCodecInfo;
class TMccVideoSinkSetting;
class CXPSPacketSink;
class CMccVideoSinkUser;
class MAsyncEventHandler;
class MDataSource;

// CLASS DECLARATION


/**
*  IP DL Media Interface class which is a wrapper of CVideoPlayerUtility class and 
*  provides services for downlink audio/video streaming and playing. 
*
*  @lib 
*  @since 
*/
class CMccVideoSinkImpl :
    public CBase, 
    public MVideoPlayerUtilityObserver, 
    public MVideoLoadingObserver, 
    public MXPSPktSinkObserver 
    {
    public:
    
        enum TMccVideoSinkState
            {
            ENotSet,
            EOpening,
            EOpened,
            EPrepared,
            EPlaying,
            EPaused,
            EStopped
            };
               
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @since 
        * @param TMccVideoSinkSetting aSettings - settings for the sink
        * @return CMccVideoSinkImpl* - Pointer to the interface object created; NULL if in error
        */
        static CMccVideoSinkImpl* NewL( const TMccVideoSinkSetting& aSettings,
                                        TUint32 aEndpointId );


        /**
        * Destructor.
        */
        virtual ~CMccVideoSinkImpl();

    public:
    
    	TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );
    	
    	/**
    	* Log off current user
    	* @return ETrue if there's no more users for the sink
    	*/
        TBool SinkThreadLogoff();
        
        void SetCurrentUser( MAsyncEventHandler* aEventHandler );

    public: // New functions

        /**
         * Open the Video Play Engine.
         * @since 
         * @param TInt aIapId - [input] An IAP Id. Default is KUserDefaultIap, which will cause 
         *                              the access point selection window popped up. 
         * @leave Can leave with one of the system wide error codes
         * @reutrn None.
         */
        void OpenL();

        /**
         * Make the Video Play Engine do preparation for playing.
         * @since Series-60 2.1
         * @reutrn None.
         */
        void Prepare();

        /**
        * The parameters needed for remote setup and swis communication.
        * @since 
        * @param TInetAddr& aRemoteAddrAudio - [input] Remote IP address and RTP port number to use
        *                                      for audio stream
        * @param TInetAddr& aRemoteAddrVideo - [input] Remote IP address and RTP port number to use
        *                                      for video stream
        * @param TDesC8& aSdpText - [input] Whole content of finalized SDP (Session Description Protocol) text
        * 
        * @param TTimeIntervalMicroSeconds32& aPausedTimeOut - [input] The time for detection of downlink 
        *                                            audio/video packet traffic has been paused by uplink.
        * @param TTimeIntervalMicroSeconds32& aDisconnectedTimeOut - [input] The time for detection of downlink
        *                                            audio/video packet traffic has been disconnected.
        * @return TInt - KErrNone if successful; system-wide error code otherwise
        */
        void RemoteSetupL();

        /**
        * Start to receive and play downlink audio/video streams.
        * @since 
        * @param None
        * @return TInt - KErrNone if successful; system-wide error code otherwise
        */ 
        void StartL();

        /**
        * Stop receiving and playing of downlink audio/video streams.
        * @since 
        * @param None
        * @return None
        */
        void Stop();

        /**
        * Pause receiving and playing of downlink audio/video streams.
        * @since 
        * @param None
        * @return None
        */
        void PauseL();

        /**
        * Close and shut down the IP DL Media subsystem.
        * @since 
        * @param None
        * @return None
        */
        void Close();

        /**
        * Set the Display Window. This is used to provide the video controller with an area of
        * the display to render the current video frame.
        * @since 
        * @param RWsSession& aWs - [input] A session with the window server
        * @param CWsScreenDevice& aScreenDevice - [input] The screen device
        * @param RWindowBase& aWindow - [input] The window to display
        * @param const TRect& aWindowRect - [input] The rectangle to display
        * @param const TRect& aClipRect - [input] The clip rectangle
        * @leave Can leave with one of the system wide error codes
        * @return None
        */
        void SetDisplayWindowL( RWsSession& aWs, 
                                CWsScreenDevice& aScreenDevice,
                                RWindow& aWindow );

        /**
        * Get the current audio playback priority and preference settings.
        * @since 
        * @param TInt& aPriority - [output] A priority value between EMdaPriorityMin and
        *                          EMdaPriorityMax
        * @param TInt& aPreference - [output] Time vs. quality preference
        *                                              as enumerated in TMdaPriorityPreference
        * @return None
        */
        void Priority( TInt& aPriority, TInt& aPreference );

        /**
        * Set audio playback priority.  This is used to arbitrate between simultaneous
        * accesses of the sound hardware.
        * @since 
        * @param TInt aPriority - [input] A priority value between EMdaPriorityMin and
        *                         EMdaPriorityMax
        * @param TInt aPreference - [input] Time vs. quality preference
        *                                             as enumerated in TMdaPriorityPreference
        * @return None
        */
        void SetPriority( TInt aPriority, TInt aPreference );

        /**
        * Get the audio playback volume.
        * @since 
        * @param None
        * @return TInt - Current volume setting between 0 and value returned by GetMaxVolume()
        */
        TInt Volume() const;

        /**
        * Get the maximum audio playback volume.
        * @since 
        * @param None
        * @return TInt - Maximum volume setting value
        */
        TInt MaxVolume() const;

        /**
        * Set the audio playback volume.
        * @since 
        * @param TInt aVolume - [input] The volume between 0 and the maximum value returned by
        *                       GetMaxVolume()
        * @return None
        */
        void SetVolumeL( TInt aVolume );
        
        /**
        * Set the max audio volume. This should correlate to values given in SetVolumeL
        * @since 
        * @param TInt aMaxVolume
        * @return None
        */
        void SetMaxVolume( TInt aMaxVolume );
        
        /**
        * Get the audio stereo balance.
        * @since 
        * @param None
        * @return TInt - Balance setting between KMMFBalanceMaxLeft and  KMMFBalanceMaxRight
        */
        TInt Balance() const;

        /**
        * Set the audio stereo balance.
        * @since 
        * @param TInt aBalance - [input] Balance between KMMFBalanceMaxLeft and KMMFBalanceMaxRight
        * @return None
        */
        void SetBalanceL( TInt aBalance );

        /**
        * Register call back function for video loading
        * @since Series-60 2.1
        * @param MVideoLoadingObserver& aCallback - [input] Call back observer of video loading
        * @return None
        */
        void RegisterForVideoLoadingNotification( MVideoLoadingObserver& aCallback );

        /**
        * Version of IP DL Media subsystem.
        * @since 
        * @param None
        * @return TVersion - Version of current subsystem
        */
        TVersion Version() const; 
   		

    public: // From MXPSPktSinkObserver
    
        /**
        * Call back function from helix
        */
   		void RestorePacketSupply( TUint aStreamId );
    
    public:
        
        void FormatVideoSDPInfoL( const TMccCodecInfo& aVideoCodec );
        
        void FormatAudioSDPInfoL( const TMccCodecInfo& aAudioCodec );
        
        void EmptyBufferL( CMMFBuffer* aBuffer, 
		                   MDataSource* aSupplier,
		                   TMediaId aMediaId,
		                   TRtpRecvHeader& aHeaderInfo );
        
        void UpdateSettingsL( const TMccVideoSinkSetting& aSettings, 
                              TBool aForceUpdate = EFalse );
        
        void GetCurrentSettings( TMccVideoSinkSetting& aSettings ) const;
        
        void GetActualPrerollL( TInt& aPreroll );
        
        TReal GetVideoFrameRate();
        
        void SetVideoFrameRate( TReal aFrameRate );
        
    private:

        /**
        * C++ default constructor.
        */
        CMccVideoSinkImpl( TUint32 aEndpointId );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( const TMccVideoSinkSetting& aSettings );
        
    	/**
        * Forward Open Complete event to the observer which is set up in NewL()
        */
        void MvpuoOpenComplete( TInt aError );

    	/**
        * Forward Prepare Complete event to the observer which is set in NewL()
        */
	    void MvpuoPrepareComplete( TInt aError );

    	/**
        * Forward Frame Ready event to the observer which is set in NewL()
        */
	    void MvpuoFrameReady( CFbsBitmap& aFrame,TInt aError );

    	/**
        * Forward Play Complete event to the observer which is set in NewL()
        */
	    void MvpuoPlayComplete( TInt aError );
	
    	/**
        * Forward all the other events except "Paused, Resumed, Disconnected" defined
        * in MIdmObserver class to the observer which is set in NewL();
        * For "Paused, Resumed, Disconnected" events, call their corresponding functions
        * in the same MIdmObserver object set in NewL().  
        */
	    void MvpuoEvent( const TMMFEvent& aEvent );

    	/**
        * Reserved for the future custom command sending. 
        */
        TInt SendCommand( const TDesC8& aDataTo1, const TDesC8& aDataTo2 );

        /**
        * Reserved for the future custom command sending.
        */
        TInt SendCommand( const TDesC8& aDataTo1, const TDesC8& aDataTo2, TDes8& aDataFrom );
        
        void MvloLoadingStarted();
		void MvloLoadingComplete();
		
		TBool SetVideoSinkState( TMccVideoSinkState aState );
		
		HBufC8* CreateModesetDesL( TUint aBitrateMask );
		
		void SendStreamEventToAllClients( TMccEventType aEventType, 
		                                  TInt aError = KErrNone,
		                                  TBool aAllEndpoints = EFalse );
		                              
		void SendStreamEventToClient( TMccEventType aEventType, 
		                              MAsyncEventHandler* aEventHandler,
		                              TInt aError = KErrNone,
		                              TBool aAllEndpoints = EFalse );
                              
    private:        
        
        TInt ResolveStreamId( TMediaId aMediaId, TUint& aStreamId );
           
        TInt ResolveStreamId( MAsyncEventHandler* aUser, TUint& aStreamId );
		
		TUint ResolveStreamId( TInt aUserIndex );
		
		TUint GenerateStreamId();
		
		TInt FindVideoSinkUserEntryByMediaType( TUid aMediaType );
		
		TInt FindStreamIdForVideoSinkUser( CMccVideoSinkUser& aEntry,
                                           TUint& aStreamId );
    
        CMccVideoSinkUser* FindVideoSinkUserEntryForCurrent();
        
        void RemoveVideoSinkUser( MAsyncEventHandler* aUser, TUint aStreamId );
        
        static TBool VideoSinkUserMatch( const CMccVideoSinkUser& aUser1, 
                                         const CMccVideoSinkUser& aUser2 );
        
        void ConstructSdpL();
        
        void HandleAudioSdpL();
        
        void HandleVideoSdpL();
        
        TBool AnalyzeTimeStampL( CMccVideoSinkUser& aUser, TRtpRecvHeader& aHeaderInfo );
        
        void DoResetL( TBool aFullReset = ETrue, CMccVideoSinkUser* aUser = NULL );
        
        TBool IsPlayerOpened() const;
        
        void ResetPacketSink();
        
        TBool IsStartedOnce( const CMccVideoSinkUser* aUser );
               
        void SetStartedOnce( CMccVideoSinkUser* aUser, TBool aIsStartedOnce );
        
        void SetRotationL( TMccVideoRotation aRotation, TBool aForceUpdate );
        
        TBool ResetOngoing() const;
        
        HBufC8* GetFmtpLC( const TMccCodecInfo& aCodecInfo );
        
        void DoPauseL( const CMccVideoSinkUser* aUser = NULL );
        
        void DoPlay( const CMccVideoSinkUser* aUser = NULL );
        
        void DoPauseForUser( const CMccVideoSinkUser* aUser );
        
        TBool MultipleMediaTypes();
        
        TBool RealTimeMode();
        
        void AddUserL( MAsyncEventHandler& aEventHandler );
        
        void DoMvloLoadingComplete( TBool aSimulated = EFalse );
        
        void DoMvloLoadingStarted( TBool aSimulated = EFalse );
        
        TBool AllUsersReady();
        
        void UpdateScreenTransparencyL(const TMccVideoSinkSetting& aSettings);
        void DeleteWindowingElements();
        void CreateWindowingElementsL(const TMccVideoSinkSetting& aSettings);
        
    private: // Data
    
        RWsSession           iRwSession;
		RWindowGroup*        iRwGroup;
		RWindow*             iRw;
		CWsScreenDevice*     iDev;
		CWindowGc*           iGc;
		CVideoPlayerUtility2* iVideoPlayer;
        CXPSPacketSink* 	 iPacketSink;

		HBufC8* iSdp;
        
        TUint32 iEndpointId;
        
        TMccVideoSinkState iState;
        
        RPointerArray<CMccVideoSinkUser> iUsers;
        
        MAsyncEventHandler* iCurrentUser;
        
        TUint iHelixStreamId;
        
        HBufC* iServerName;
        
        TMccVideoSinkSetting iSettings;
        
        TReal iVolumeAdaptation;
        
        TInt iVolume;
        
        TInt iMccMaxVolume;

        TInt iPreroll;
        
        TBool iPrerollSet;
        
        TUint32 iNumPacketsEnqueued;
        
        TTimeIntervalMicroSeconds iPos;
        
        CMccVideoSinkUser* iSearchTermUser;
        
        TBool iSimulatedStreamingEventSent;
        TBool iSimulatedBufferingEventSent;
        
    private:
    	#ifdef TEST_EUNIT
			friend class UT_CMccVideoSinkImpl;	
			friend class UT_CMccVideoSink;
   		#endif
   		
    }; // end of CMccVideoSinkImpl


#endif  // MCCVIDEOSINKIMPL_H
