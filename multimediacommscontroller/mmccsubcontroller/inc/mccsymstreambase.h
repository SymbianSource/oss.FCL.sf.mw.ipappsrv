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
* Description:    Provides a baseclass for symmetric subthread streams
*
*/



#ifndef MCCSYMSTREAMBASE_H
#define MCCSYMSTREAMBASE_H

//  INCLUDES
#include <e32base.h>

#include "mccinternalcodecs.h"
#include "mmccevents.h"


// FORWARD DECLARATIONS
class TMMFPrioritySettings;
class CMccDataPathBase;
class MDataSink;
class MDataSource;
class CMccRtpManager;
class CMMFDevSound;
class MMccResources;
class CMccMultiplexer;
class CMccRtpMediaClock;
class TMccCryptoContext;

// CLASS DECLARATION

/**
*  Mcc Symmetric subthread stream baseclass
*
*  @lib Mccsubthreads.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccSymStreamBase ) : public CBase, 
        public MAsyncEventHandler
    {
    public: // Enumerations
    
        enum TStreamState
            {
            EStateNone = 0,
            EStatePrepared = 1,
            EStateStreaming = 2,
            EStatePaused = 3,
            EStateStopped = 4
            };
    
        enum TCodecState
            {
            EStateCodecNone = 0,
            EStateCodecNegotiated = 1,
            EStateCodecPrepared = 2,
            EStateCodecLoaded = 3,
            EStateCodecLoadedAndUpdating = 4
            };
            
    public: // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CMccSymStreamBase();
        
    public: // Functions from MAsyncEventHandler
        
		TInt SendEventToClient( const TMMFEvent& aEvent );

    public: // New functions
    
        /**
        * Returns the streams identifier
        * @since Series 60 3.0
        * @param None
        * @return TInt stream identifier
        */
        inline TUint32 MccStreamId()
            {
            return iMccStreamId;
            };
    
        /**
        * Sets the priority settings for this stream
        * @since Series 60 3.0
        * @param aPriority MMF Priority settings
        * @return void
        */
        virtual void SetPrioritySettingsL( 
                const TMMFPrioritySettings& aPriority ) = 0;
                
        /**
        * Adds the sink and source to the stream
        * @since Series 60 3.0
        * @param aDatasink MDataSink
        * @param aDatasource MDataSource
        * @return void
        */
        virtual void AddSinkAndSourceL( MDataSink* aDatasink, 
                MDataSource* aDatasource ) = 0;
                
        /**
        * Loads a codec in the stream
        * @since Series 60 3.0
        * @param aCodecInfo TMccCodecInfo
        * @return void
        */
        virtual void LoadCodecL( const TMccCodecInfo& aCodecInfo,
                                 const TDesC8& aFmtp ) = 0;
        
        /**
        * Primes the audio stream ready
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void PrimeL( const TUint32 aEndpointId ) = 0;
        
        /**
        * Starts the audio streaming
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void PlayL( const TUint32 aEndpointId,
        					TBool aStreamPaused, 
                            TBool aEnableRtcp ) = 0;
        
        /**
        * Pauses the audio streaming
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void PauseL( const TUint32 aEndpointId,
        					 TBool aEnableRtcp ) = 0;
        
        /**
        * Resumes the audio streaming
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void ResumeL( const TUint32 aEndpointId,
        					  TBool aEnableRtcp ) = 0;
        
        /**
        * Stops the audio streaming
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void StopL( const TUint32 aEndpointId ) = 0;
        
        /**
        * Sets the current record gain
        * @since Series 60 3.0
        * @param aGain New gain
        * @return void
        */
        void SetGainL( TInt aGain );
        
        /**
        * Sets the current playback volume
        * @since Series 60 3.0
        * @param aVolume New volume
        * @return void
        */
        void SetVolumeL( TInt aVolume );
        
        /**
        * Returns the maximum volume
        * @since Series 60 3.0
        * @param None
        * @return TInt
        */
        TInt MaxVolumeL();
        
        /**
        * Returns the maximum gain
        * @since Series 60 3.0
        * @param None
        * @return TInt
        */
        TInt MaxGainL();
        
        /**
        * Returns the current volume
        * @since Series 60 3.0
        * @param None
        * @return TInt
        */
        TInt VolumeL();
        
        /**
        * Returns the current gain
        * @since Series 60 3.0
        * @param None
        * @return TInt
        */
        TInt GainL();
        
        /**
        * Resets Jitterbuffer
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void ResetCountersL() = 0;
        
        /**
        * Sets the current record/play balance depending on the stream type
        * @since Series 60 3.0
        * @param aLeftbal Left balance
        * @param aRightbal Right balance
        * @return void
        */
        virtual void SetBalanceL( TInt aLeftbal, TInt aRightbal ) = 0;
        
        /**
        * Reuturns the current record/play balance depending on the stream type
        * @since Series 60 3.0
        * @param aLeftbal Left balance
        * @param aRightbal Right balance
        * @return void
        */
        virtual void GetBalanceL( TInt& aLeftbal, TInt& aRightbal ) = 0;
        
        /**
        * Sends a media event in a uplink stream. Default implementation will
        * leave with error code KErrNotSupported
        * @since Series 60 3.0
        * @param aEvent Event to be sent
        * @return void
        */
        virtual void SendMediaSignalL( const TMccEvent& aEvent );

        /**
        * Sends RTCP receiver report to uplink if possible.
        * @param none
        * @return One of the standard system-wide error codes.
        */
        virtual void SendRTCPReceiverReportL();

        /**
        * Sends RTCP sender report to uplink if possible.
        * @param none
        * @return One of the standard system-wide error codes.
        */
        virtual void SendRTCPSenderReportL();

        /**
        * Sends Non-RTCP data to uplink if possible.
        * @param aData - [input] Data to send
        * @return One of the standard system-wide error codes.
        */
        virtual void SendRTCPDataL( const TDesC8& aData );
        
        /**
        * Starts inactivity timer for a stream in a given session.
        * @since Series 60 3.0
        * @param aMessage Message from client
        * @return TBool Was function Sync/Async
        */
        virtual void StartInactivityTimerL( TUint32 aTimeoutTime );

        /**
        * Stops inactivity timer for a stream in a given session.
        * @since Series 60 3.0
        * @param aMessage Message from client
        * @return TBool Was function Sync/Async
        */
        virtual void StopInactivityTimerL();
        
        virtual void GetSSRCL( TUint32& aSSRCValue ) = 0;

        /**
        * Function for dynamic bitrate change. This is used only for AMR
        * encoder, but this will trap the call to DoChangeBitrateL() function
        * which will be inherited by subclasses.
        * @param aBitrate New encoder bitrate
        * @return TInt System-wide error code
        */
        TInt ChangeBitrate( TInt aBitrate );
        
        /**
        * Fetches FMTP
        * @since Series 60 3.0
        */
        void GetFmtpL( TDes8& aFmtp );

        /**
        * Fetches codec info
        * @since Series 60 3.0
        */
        virtual void GetCodecL( TMccCodecInfo& aCodec );
        
        /**
        *
        */
        virtual TBool IsUplink() = 0;
        
        /**
        *
        */
        virtual void StandByL() = 0;
        
        /**
        *
        */
        virtual void SetAudioRouteL( TUint32 aRoutingDestination );
        
        /**
        *
        */
        virtual void GetAudioRouteL( TUint32& aRoutingDestination );

        /**
        *
        */
        MDataSink* DataSink() { return iDatasink; };

        /**
        *
        */
        MDataSource* DataSource() { return iDatasource; };
        
        /**
        *
        */
        const TUid Type();
        
        /**
        *
        */
        TBool LocalStream() const;
        
        /**
        *
        */
        TBool StandbyStream() const;
        
        /**
        *
        */
        void InformStreamStateChange( TMccEventType aEventType,
                                      TUint32 aEndpointId );

        /**
         * Bind context into stream.
         * @since Series 60 3.2
         * @param aEndpointId The endpoint id
         * @param aContextParams Crypto context
         * @return void
         */	
        virtual void BindContextIntoStreamL( TUint32 aEndpointId,
                                             const TMccCryptoContext& aContextParams );
        
        /**
         * Remove crypto context.
         * @since Series 60 3.2
         * @param aEndpointId The endpoint id, If 0
         * will remove context from all existing endpoints
         * @return Error code.
         */                                     
        virtual TInt RemoveContext( TUint32 aEndpointId );

    protected:  // New functions 
    
        /**
        * Gets sink of the multiplexer
        */
        virtual MDataSink* MultiplexerSink();
    
        /**
        * Sets the state of the stream
        * @since Series 60 3.0
        * @param TStreamState New state
        * @return void
        */
        TBool SetStateL( TStreamState aState, TUint32 aEndpointId );

        /**
        * Worker function for dynamic bitrate change
        * @since Series 60 3.0
        * @param aBitrate New bitrate
        * @return void
        */
        virtual void DoChangeBitrateL( TInt aBitrate );
        
        /**
        * Returns the current state of the stream
        * @since Series 60 3.0
        * @param None
        * @return TStreamState
        */
        TStreamState CurrentState();
        
        /**
        * Standby activation/deactivation
        */
        void HandleStandby( TUint32 aEndpointId, TBool aStreamPaused );
        
        /**
        * Send streaming events of uncontrolled/MMF endpoints.
        */
        TInt SendStreamEventToClient( TMccEventType aEventType, 
                                      TUint32 aEndpointId,
                                      TInt aError = KErrNone );
        
        /**
        * Updates multiplexer.
        */
        void HandleMultiplexerL();
        
        /**
        * Updates sink/source if needed.
        */
        void HandleSpecialSinksAndSources();
        
        /**
        *
        */
        void MultiplexerCleanup();
        
        void DoPrimeL();
        
        void DoPlayL();
        
        void DoPauseL();
        
        void DoResumeL();
        
        void DoStopL();

        void ParsePayloadTypesL( RArray<TUint>& aPayloadTypes,
            const TMccCodecInfo& aCodecInfo ) const;
            
        TBool IsControllingMmfEndpoint() const;
        
        TCodecState CurrentCodecState() const;
        
        void SetCodecState( TCodecState aState );
        
        void UpdateCodecLoadingPermission( TBool aLoadingAllowed );
        
        TBool CodecLoadingAllowed() const;
        
        TBool IsControlled( TUint32 aEndpointId ) const;
        
        TBool IsCodecOperationAllowed() const;
        
        void ClearStateChangeEvent();

        
    public: // Functions from base classes
		 // Set priority settings  
    	void SetMMFPriority (
                const TMMFPrioritySettings& aPriority );

    protected:  // Constructor

        /**
        * Constructor
        */
        CMccSymStreamBase( TUint32 aMccStreamId, 
                MAsyncEventHandler* aEventhandler, 
                MMccResources* aMccResources,
                CMccRtpManager* aManager, 
                TInt aStreamType );
        
    private:    // Consturctor
        
        /**
        * Private constructor
        */
        inline CMccSymStreamBase() : iDatapath( NULL ), iEventhandler( NULL ),
            iDatasink( NULL ), iDatasource( NULL ), 
            iCodecInfo( TMccCodecInfo() ), iRtpmanager( NULL ), 
            iSoundDevice( NULL ), iMccStreamId( (TUint) KErrNotFound ), 
            iState ( EStateNone )
            {   };

    protected:  // Data
    
        // Streams datapath
        CMccDataPathBase* iDatapath;
        
        // Eventhandler for the stream
        MAsyncEventHandler* iEventhandler;
        
        MMccResources* iMccResources;
        
        // Datasink
        MDataSink* iDatasink;
        
        // Datasource
        MDataSource* iDatasource;
        
        // Codec information
        TMccCodecInfo iCodecInfo;

        // fmtp string
        HBufC8* iFmtpAttr;
        
        // RTP Manager instance
        CMccRtpManager* iRtpmanager;
        
        // Devsound instance used by this stream
        CMMFDevSound* iSoundDevice;

        // Identifier of this stream
        TUint32 iMccStreamId;

        // Type of the stream        
        TUint8 iType;
        
        RArray<TUint32> iControlledEndpoints;
        
        RArray<TUint32> iUncontrolledEndpoints;
        
        // Multiplexer, not owned
        CMccMultiplexer* iMultiplexer;
        
        // Rtp media clock
        CMccRtpMediaClock* iRtpMediaClock;
        
        TMMFPrioritySettings iPrioritySettings;
        
    private:    // Data
        // Current state
        TStreamState iState;
        
        TCodecState iCodecState;
        
        TBool iCodecLoadingAllowed;
        
        // Currently used state change event
        TMccEvent iStreamStateChangeEvent;

    private:    // Friend classes
        #if defined( TEST_EUNIT )
        friend class UT_CMccSymDlStream;
        friend class UT_CMccSymUlStream;
        friend class UT_CMccSymSimpleDlStream;
        #endif
    };

#endif      // MCCSYMSTREAMBASE_H

// End of File
