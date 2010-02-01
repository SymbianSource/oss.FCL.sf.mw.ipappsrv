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





#ifndef MCCCONTROLLER_H
#define MCCCONTROLLER_H

//  INCLUDES
#include <mmf/common/mmfcontroller.h>
#include <mmf/common/mmfstandardcustomcommands.h>
#include "mmcctranscoder.h"

#ifdef FTD_ENABLED
#include <e32msgqueue.h>
#include "Mccstreamstats.h"
#include "Mcccodecstats.h"
#include "Mccjbufferstats.h"
#endif
        
#include "mccdef.h"
#include "mccinternaldef.h"
#include "mccrateadaptationobserver.h"
#include "masynclinkcreationobserver.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CMccUlDlClient;
class CMccResourcePool;
class CMccQosController;
class CMccAsyncLinkCreator;
class MMccSourceSink;

// CLASS DECLARATION

/**
*  Mcc Controller methods
*
*  MccController.dll
*  @since Series 60 3.0
*/
class CMccController : public CMMFController,
                       public MMMFAudioPlayDeviceCustomCommandImplementor,
                       public MMMFAudioRecordDeviceCustomCommandImplementor,
                       public MMccEventHandler,
                       public MMccRateAdaptationObserver,
                       public MAsyncLinkCreationObserver,
                         public MMccTranscoderObserver
                        
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMccController* NewL();
          
        /**
        * Destructor.
        */
        virtual ~CMccController();

    public: // New functions

    public: // Functions from base classes
    
        // From MMccEventHandler
        
        virtual TInt SendMccEventToClient( TMccEvent& aEvent );

	    virtual void StateChange( TInt aState, TUint32 aLinkId );

	    virtual void ErrorOccured( TInt aError, 
	                               TUint32 aSessionId, 
	                               TUint32 aLinkId,
	                               TUint32 aStreamId, 
	                               TUint32 aEndpointId );

        // From CMMFController
        virtual void CustomCommand( TMMFMessage& aMessage );

        // From MMMFAudioPlayDeviceCustomCommandImplementor
        virtual void MapdGetVolumeL( TInt& aVolume );
        virtual void MapdGetMaxVolumeL( TInt& aMaxVolume );
        virtual void MapdSetVolumeL( TInt aVolume );
        virtual void MapdGetBalanceL( TInt& aBalance );
        virtual void MapdSetBalanceL( TInt aBalance );
        virtual void MapdSetVolumeRampL( const TTimeIntervalMicroSeconds& aRampDuration );
      
        // From MMMFAudioRecordDeviceCustomCommandImplementor
        virtual void MardSetGainL( TInt aGain );
        virtual void MardGetMaxGainL( TInt& aMaxGain );
        virtual void MardGetGainL( TInt& aGain );
        virtual void MardSetBalanceL( TInt aRBalance );
        virtual void MardGetBalanceL( TInt& aRBalance );
        
        // From MMccRateAdaptationObserver
        virtual TInt RateAdaptationRequest( const TMccEvent& aInputData, 
                                            TMccEvent& aOutputData );                                   
        virtual TInt RateAdaptationAlert( const TMccEvent& aAdaptationData,
                                          TAlertType aAlertType );
        
        // From MAsyncLinkCreationObserver
        
        /**
        * Called when CMccAsyncLinkCreator has finished link creation
        * and can be deleted
        * @since Series 60 v3.1
        * @param aClientData Data for the client containing resolved 
        *                    IP addresses and ports
        * @param aCreator    The link creator which has finished
        * @param aError      System wide error code
        */
        virtual void CreationProcessEnd( const TMccCreateLinkPckg& aClientData, 
                                         CMccAsyncLinkCreator* aCreator, 
                                         TInt aError );
         
        // From MMccTranscoderObserver                                          
        virtual void MccTranscoderEventReceived( TMccEvent& aEvent );

    protected:  // Functions from base classes
    
        // From CMMFController
        virtual void AddDataSinkL( MDataSink& aSink );
        virtual void AddDataSourceL( MDataSource& aSource );
        virtual void SetPrioritySettings( const TMMFPrioritySettings& aPrioritySettings );

        // -- Not used --
        virtual void RemoveDataSourceL( MDataSource& aDataSource );
        virtual void RemoveDataSinkL( MDataSink& aDataSink );
        virtual void ResetL();
        virtual void PlayL();
        virtual void StopL();
        virtual void PrimeL();
        virtual void PauseL();
        virtual TTimeIntervalMicroSeconds PositionL() const;
        virtual void SetPositionL( const TTimeIntervalMicroSeconds& aPosition );
        virtual TTimeIntervalMicroSeconds DurationL() const;
        virtual void GetNumberOfMetaDataEntriesL( TInt& aNumberOfEntries );
        virtual CMMFMetaDataEntry* GetMetaDataEntryL( TInt aIndex );
        // --------------

    private:
        /**
        * C++ default constructor.
        */
        CMccController();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Gets the identifier of current data sink
        * @param aMessage - Message from clientside
        * @leave system wide error code if unsuccessful
        */
		void GetSinkIdL( TMMFMessage& aMessage );

        /**
        * Gets the identifier of current data source
        * @param aMessage - Message from clientside
        * @leave system wide error code if unsuccessful
        */
		void GetSourceIdL( TMMFMessage& aMessage );
		
        /**
        * Creates an Mcc session
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave system wide error code if unsuccessful
        * @return None
        */
        void CreateSessionL( TMMFMessage& aMessage );
       
        /**
        * Closes an Mcc session
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */    
        void CloseSessionL( TMMFMessage& aMessage );

        /**
        * Creates a Mcc link
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @return TInt - KErrNone if successful; system wide error code otherwise
        */
        void CreateLinkL( TMMFMessage& aMessage );

        /**
        * Closes a Mcc link
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */    
        void CloseLinkL( TMMFMessage& aMessage );

        /**
        * Starts stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void StartStreamL( TMMFMessage& aMessage );
        
        /**
        * Stops stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void StopStreamL( TMMFMessage& aMessage );
        
        /**
        * Starts inactivity timer for a stream in a given session.
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void StartInactivityTimerL( TMMFMessage& aMessage );
        
        /**
        * Stops inactivity timer for a stream in a given session.
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void StopInactivityTimerL( TMMFMessage& aMessage );
        
        /**
        * Request Event Notification
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */    
        void RequestEventNotificationL( TMMFMessage& aMessage );

        /**
        * Creates event to eventbuffer and sends event to client
        * @since Series 60 3.0
        */
        void SendEvent();
        void SendEventL();
        
        /**
        * Creates stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void CreateStreamL( TMMFMessage& aMessage );

        /**
        * Deletes stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void DeleteStreamL( TMMFMessage& aMessage );

        /**
        * Prepares stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void PrepareStreamL( TMMFMessage& aMessage );

        /**
        * Pauses stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void PauseStreamL( TMMFMessage& aMessage );

        /**
        * Resumes stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void ResumeStreamL( TMMFMessage& aMessage );
        
        /**
        * Checks if streams exists in certain link
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void StreamsExistsL( TMMFMessage& aMessage );
        

        /**
        * Get List of supported codecs
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void GetSupportedCodecsL( TMMFMessage& aMessage ) const;

        /**
        * Get current codec
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void GetCodecL( TMMFMessage& aMessage );
        
        /**
        * Get fmtp string
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void GetFmtpAttrL( TMMFMessage& aMessage );
        
        /**
        * Set Codec
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void SetCodecL( TMMFMessage& aMessage );

        /**
        * Get the supported bitrates from the codec of the specified stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void GetSupportedBitratesL( TMMFMessage& aMessage );
        
        /**
        * Get syncronisation source from the specified stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void GetSSRCL( TMMFMessage& aMessage );

        /**
        * Set Priority for stream
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void SetPriorityL( TMMFMessage& aMessage );

        /**
        * Sets stream settings
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        */
        void StreamSettingsL( TMMFMessage& aMessage );
        
        /**
        * Sends RTCP receiver report to uplink if possible.
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void SendRTCPReceiverReportL( TMMFMessage& aMessage ); 
        
        /**
        * Sends RTCP sender report to uplink if possible.
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void SendRTCPSenderReportL( TMMFMessage& aMessage ); 
        
        /**
        * Sends non-RTCP data to uplink if possible.
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void SendRTCPDataL( TMMFMessage& aMessage ); 

        /**
        * Sets remote address for given session
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void SetRemoteAddressL( TMMFMessage& aMessage );

		/**
        * Sets remote address for given session
        * @since Series 60 3.0
        * @param aMessage - [input] Message from clientside
        * @leave System wide error code if unsuccessful
        * @return None
        */
		void SetRemoteRtcpAddrL( TMMFMessage& aMessage );
		
        /**
        * Finds given session from session array
        * @since Series 60 3.0
        * @param aSessionID - [input] RTP session ID
        * @return TInt - Sessionarray index
        */
        TInt FindSessionL( TUint32 aSessionID );

        /**
        * CustomCommand 
        * @since Series 60 3.0
        * @param aMessage - [input] TMMFMessage
        * @leave System wide error code if unsuccessful
        * @return None
        */
        void CustomCommandL( TMMFMessage& aMessage );
        
        /**
        * Sends media signals to given stream in given session
        * @since Series 60 3.0
        * @param aMessage - [input] Message containing the media signal
        * @return void
        */
        void SendMediaSignalL( TMMFMessage& aMessage );

        /**
        * Enables camera viewfinder
        * @param aMessage - [input] Message containing the media signal
        * @return void
        */
        void EnableViewFinderL( TMMFMessage& aMessage );

        /**
        * Disables camera viewfinder
        * @param aMessage - [input] Message containing the media signal
        * @return void
        */
        void DisableViewFinderL( TMMFMessage& aMessage );

        /**
        * Gets current viewfinder settings
        * @param aMessage - [input]
        * @return void
        */
        void ViewFinderSettingsL( TMMFMessage& aMessage );
        
        /**
        * Get camera info
        * @param aMessage - [input] Message 
        * @return void
        */
        void GetCamInfoL( TMMFMessage& aMessage );
        
        /**
        * Get num of available cameras
        * @param aMessage - [input] Message 
        * @return void
        */
        void GetNumOfCamsL( TMMFMessage& aMessage );

        /**
        * Set camera configuration value
        * @param aMessage - [input] Message 
        * @return void
        */
        void SetCamValue1L( TMMFMessage& aMessage );

        /**
        * Get camera configuration value
        * @param aMessage - [input] Message 
        * @return void
        */
        void GetCamValue1L( TMMFMessage& aMessage );

        /**
        * Set camera configuration value
        * @param aMessage - [input] Message 
        * @return void
        */
        void SetCamValue2L( TMMFMessage& aMessage );

        /**
        * Get camera configuration value
        * @param aMessage - [input] Message 
        * @return void
        */
        void GetCamValue2L( TMMFMessage& aMessage );
        
        /**
        * Set audio routing
        * @param aMessage - [input] Message 
        * @return void
        */
        void SetAudioRouteL( TMMFMessage& aMessage );
        
        /**
        * Get current audio routing destination
        * @param aMessage - [input] Message 
        * @return void
        */
        void GetAudioRouteL( TMMFMessage& aMessage );
       
       /**
        * Get endpoint parameters
        * @param aMessage - [input] Message 
        * @return void
        */
        void GetEndpointParameterL( TMMFMessage& aMessage );
          
        /**
        * Set endpoint parameters
        * @param aMessage - [input] Message 
        * @return void
        */
        void SetEndpointParameterL( TMMFMessage& aMessage );
        
        /**
        * Updates endpoin
        * @param aMessage - [input] Message 
        * @return void
        */
        void UpdateEndpointL( TMMFMessage& aMessage );
        
        /**
        * Reuse endpoint
        * @param aMessage - [input] Message 
        * @return void
        */
        void ReuseL( TMMFMessage& aMessage );

        /**
        * Get referece count of endpoint
        * @param aMessage - [input] Message 
        * @return void
        */
        void ReferenceCountL( TMMFMessage& aMessage );
        
        /**
        * Removes endpoint, all streams, using the endpoint, are stopped.
        * @return void
        */
        void RemoveEndpointL( TUint32 aEndpointId );
        
        /**
        * Transcodes media file
        * @return void
        */
        void TranscodeFileL( TMMFMessage& aMessage );
        
        /**
        * Cancel transcodes media file
        * @return void
        */
        void CancelTranscodeFileL( TMMFMessage& aMessage );
        
        /**
         * Binds context into stream
         * @param aMessage - [input] Message
         * @return void
         */
        void BindContextIntoStreamL( TMMFMessage& aMessage );
        
        /**
         * Remove context.
         * @param aMessage - [input] Message
         * @return void
         */
        void RemoveContextL( TMMFMessage& aMessage );
        
    private:
        
        TInt StoreAndSendEvent( const TMccEvent& aEvent );
        
        void StoreEventL( const TMccEvent& aEvent );
        
        void StreamActionL( const TMccEvent& aEvent, const TMccEvent* aAdditionalEvent );
        
        void CancelMmfMessage();
    
        void PauseByEndPointIdL( const TMccEvent& aEvent, TUint32 aEndPointId );
        
        void ResumeByEndPointIdL( const TMccEvent& aEvent, TUint32 aEndPointId );
        
        TInt UpdateEndpointVolumeSettings( MMccSourceSink& aEndpoint );

        void AddCodec(
            const TUint32& aCodec,
            const RArray< TFourCC >& aSupportedOutCodecs,
            const RArray< TFourCC >& aSupportedInCodecs,
            TFixedArray<TFourCC, KMccFourCCArraySize>& aCodecs,
            TInt aIndex ) const;
        
    private:    // Data

        // Priority settings
        TMMFPrioritySettings    iPrioritySettings;

        // Type of stream EMccAudioUplink/EMccAudioDownlink
        TInt iStreamType;

        // IapID
        TUint iIapID;
        
        // MaxVolume
        TInt iMaxVolume;

        // Volume
        TInt iVolume;
        
        // MaxGain
        TInt iMaxGain;

        // Gain
        TInt iGain;

        // Session Index
        TUint32 iSessionIndex;

        TMMFMessage* iMessage;

        // Stream settings package
        TMccStreamPckg iStreamPckg;

        // Session PointerArray
        RPointerArray<CMccUlDlClient> iSessionArray;
        
        // Event Buffer
        RPointerArray<TMccEvent> iEventBuf;
		
		CMccResourcePool* iResourcePool;
		
		CMccQosController* iQosController;

		// Asynchronous Link Creator object
        RPointerArray<CMccAsyncLinkCreator> iCreatorArray;
		
		CMccTranscoder* iTranscoder;
		
		// key used to destroy transcoder implemetation
		TUid iTranscoderKey;
		
		// DTMF payload types indexed by link ID
		RArray< TMccKeyValuePair<TUint32, TUint8> > iDtmfPayloadTypes;
        
        #ifdef FTD_ENABLED
        
        // Message queue carrying stream statistics
        RMsgQueue<TMccStreamStats> iStreamStatsQueue;
        
        // Message queue carrying codec statistics
        RMsgQueue<TMccCodecStats> iCodecStatsQueue;
        
        // Message queue carrying jitter buffer statistics
        RMsgQueue<TMccJBufferStats> iJBufferStatsQueue;
        
        #endif
          
    private:    // Friend classes

    #ifdef TEST_EUNIT
    friend class UT_CMccController;
    #endif
    };

#endif
