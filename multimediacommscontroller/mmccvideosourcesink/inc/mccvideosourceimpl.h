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



//  INCLUDES
#ifndef MCCVIDEOSOURCEIMPL_H
#define MCCVIDEOSOURCEIMPL_H

#include <e32base.h>
#include <f32file.h>
#include <e32std.h>
#include <mmf/server/mmfdatasource.h>
#include <mmf/server/mmfdatasink.h>
#include <CCMRMediaRecorder.h>
#include <CCMRMediaSink.h>

#include "mmccevents.h"
#include "mcccamerahandler.h"
#include "mccinternalcodecs.h"

class CMccVideoSourceWrapper;
class MMccResources;
// CLASS DECLARATION


// Try to maintain the frame-rate
// (1.0 means the picture quality is sacrificed 
// "all the way" to match the target fps
const TReal KMccLatencyQyalityTradeoff = 0.0;

/**
*  
*/
class CMccVideoSourceImpl: public CBase, 
                           public MCMRMediaSink, 
                           public MCMRMediaRecorderObserver,
                           public MMccCameraHandlerObserver
    {

    public: // Methods called internally or by the controller

        /**
        * Two-phased constructor.
        * @param aConfigKeyRetrieval, ETrue if source is used for config key retrieval
        */
        static CMccVideoSourceImpl* NewL( TUint32 aEndpointId,
                                          MMccResources* aMccResources = NULL,
                                          TBool aConfigKeyRetrieval = EFalse );
                                        		
		CMccVideoSourceImpl( TUint32 aEndpointId, 
                             MMccResources* aMccResources,
                             TBool aConfigKeyRetrieval );

		virtual ~CMccVideoSourceImpl();

    public: // new
    
        void SetResources( MMccResources* aResources );
        
    	void SetCameraHandler( MMccCameraHandler& aCameraHandler );
	
        void SetVideoCodecL( const TMccCodecInfo& aVideoCodec );

        void GetVideoCodecL( TMccCodecInfo& aVideoCodec  );

        void GetSupportedVideoCodecsL( RArray<TFourCC>& aVideoTypes );

        void SetAudioCodecL( const TMccCodecInfo& aAudioCodec );

        void GetAudioCodecL( TMccCodecInfo& aAudioCodec );

        void GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes );

        void SetAudioEnabledL( TBool aEnabled );

        TBool AudioEnabledL() const;

        void SetGainL( TInt aGain );

        TInt GainL() const;

        TInt MaxGainL() const;   
        
        TInt RateAdaptationRequest( const TMccEvent& aInputData, TMccEvent& aOutputData );
        
        TBool IsStopped() const;
        
        void GetConfigKeyL( const TMccCodecInfo& aVideoCodec, 
                            TDes8& aConfigKey,
                            TBool aAllowAsyncRetrieval = ETrue );
	
	    TBool HasConfigKey() const;
	    
    public:

        /**
        * Returns the data type as a fourCC code of the data source.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        * @return    The fourCC code identifying the source datatype for the specified aMediaId.
        */
        TFourCC SourceDataTypeCode( TMediaId /*aMediaId*/ );

        /**
        * Sets the data type as a fourCC code for the data source
        *
        * @since    Series 60 3.0
        * @param    "aSourceFourCC"
        *            This specifies the data type as a fourCC code to set the source to.
        *
        * @param    "aMediaId"
        *            This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        *
        * @return    KErrNone if successful, KErrNotSupported if the source does not support having
        *            it's data type set, otherwise a system wide error code.
        */
        TInt SetSourceDataTypeCode( TFourCC aCodec, 
                                    TMediaId aMedia );

        /**
        * Not used. Leaves with KErrNotSupported.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *            The buffer that has been emptied by a data sink and is now available for reuse
        * @return   None
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        /**
        * Method to indicate whether the data source can create a buffer.
        *
        * @since    Series 60 3.0
        * 
        * @return    EFalse
        */
        TBool CanCreateSourceBuffer();
        
        /**
        * Not used. Leaves with KErrNotSupported.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        *
        * @param    "aReference"
        *           This must be written to by the method to indicate whether the created buffer is
        *           a 'reference' buffer.  A 'reference' buffer is a buffer that is owned by the source
        *           and should be used in preference to the sink buffer provided the sink buffer
        *           is also not a reference buffer.
        * .  
        * @return   The created buffer
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, 
                                         TBool& aReference );
        
        /**
        * Method to 'logon' the data source to the same thread that source will be supplying data in.
        *
        * Jitterbuffer for incoming packets and downlink timeout timer are created here.
        * 
        * @param    "aEventHandler"
        *           This is an MAsyncEventHandler to handle asynchronous events that occur during the
        *           transfer of multimedia data.  The event handler must be in the same thread as the data transfer
        *           thread - hence the reason it is passed in the SourceThreadLogon as opposed to say the constructor.
        *
        *
        * @return   KErrNone if successful, otherwise a system wide error code.
        */
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
        
        /**
        * Method to 'logoff' the data source from the same thread that source supplies data in.
        *
        * Jitterbuffer for incoming packets and downlink timeout timer are deleted. Datasource deletes the 
        * stream(s) and unregisters itself as an observer for the RTP session for incoming RTP and RTCP packets.
        *
        * @return None
        */
        void SourceThreadLogoff();

        /**
        * Method to 'prime' the data source.
        *
        * Datasource is set to primed state. If multiple codecs are using the source,
        * prime is not done fully yet at this stage, instead it is done later once once of the codecs
        * starts using the source.
        *
        * @param aMultipleCodecs, ETrue if multiple codecs are using the source
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePrimeL( TBool aMultipleCodecs = EFalse );
        
        /**
        * Method to 'play' the data source.
        *
        * Downlink timeout timer is started and datasource is set to playing state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePlayL();
        
        /**
        * Method to 'pause' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to paused state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePauseL();
        
        /**
        * Method to 'stop' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to stopped state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourceStopL();

        /**
        * Method called by a MDataSink to request the data source to fill aBuffer with data.
        *
        * Datasource gets packet from jitterbuffer and forwards it to the payloadformatdecoder
        * for further processing. Datasource works synchronously; consumers BufferFilledL
        * is called when RTP packet is got succesfully from jitterbuffer.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *           The buffer that needs filling with data
        *
        * @param    "aConsumer" 
        *           The data sink that consumes the data. The data source needs this to make the BufferFilledL
        *           callback on aConsumer when the data source has completed filling the aBuffer.
        *
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        * @return   None
        */
        void FillBufferL( CMMFBuffer* aBuffer,
                          MDataSink* aConsumer,
                          TMediaId aMediaId );
                          
        /**
        * Method to negotiate sink.
        */
        void NegotiateSourceL( MDataSink& aDataSink );	
        
    public: // From MCMRMediaSink

        /**
        * Write a buffer to sink
        * @since 2.1
        * @param CCMRMediaBuffer* aBuffer   The buffer to write
        * @return TInt                      Error code
        */
        virtual void WriteBufferL(CCMRMediaBuffer* aBuffer);

        /**
        * Inform video frame size to sink
        * @since 2.1
        * @param TSize aSize    Video frame size
        * @return TInt          Error code
        */
        virtual TInt SetVideoFrameSize(TSize aSize);

        /**
        * Inform average video bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Average video bitrate
        * @return TInt          Error code
        */
        virtual TInt SetAverageVideoBitRate(TInt aBitRate);

        /**
        * Inform maximum video bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Maximum expected video bitrate
        * @return TInt          Error code
        */
        virtual TInt SetMaxVideoBitRate(TInt aBitRate);

        /**
        * Inform average audio bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Average audio bitrate
        * @return TInt          Error code
        */
        virtual TInt SetAverageAudioBitRate(TInt aBitRate);

    public: // From MCMRMediaRecorderObserver

        /**
        * Informs the user that PrepareL() has been completed with or
        * without errors. If aError != KErrNone, media recorder remains in 
        * open state
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroPrepareComplete(TInt aError);    

        /**
        * Informs the user about a state change. 
        * Useful to find out when a new prepare is needed after 
        * a setting was changed
        * @since 2.1
        * @param aState     New state (CCMRMediaRecorder - TRecorderState)
        * @param aError     Error code
        * @return void
        */
        virtual void MmroStateChange(TInt aState, TInt aError);

        /**
        * Informs the user about a temporary runtime error. 
        * Recording can't continue right now but it may
        * be possible to continue later (e.g. audio HW was 
        * taken by a higher priority application)
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroTemporaryError(TInt aError);    

        /**
        * Informs the user about a fatal runtime error (e.g. OOM). 
        * MediaRecorder must be deleted and recreated.
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroFatalError(TInt aError);

    public: // From MMccCameraHandlerObserver
    
        virtual void CameraReady( TInt aError );
        
    private:

        void SendStreamEventToClient( TMccEventType aEventType,
                                      TInt aError = KErrNone,
                                      TUint32 aEventNumData = 0 );
        
        void SetAutoPlayMode( TBool aIsInAutoPlayMode );
        
        TBool AutoPlayMode() const;
        
        TBool CopyBufferData( CMMFBuffer& aDstBuffer, 
                              CCMRMediaBuffer& aSrcBuffer );
                              
        TBool CopyBufferData( CMMFBuffer& aDstBuffer, 
                              CMMFDataBuffer& aSrcBuffer );
                              
        TBool QueueBufferL( CCMRMediaBuffer& aBuffer );
        
        CMMFDataBuffer* FetchQueuedBuffer();
        
        void DoBufferFilledL();
        
		void ConstructL();

        TInt SetVideoBitrate( TUint& aVideoBitrate );
        
        void RateAdaptationRequestL( const TMccEvent& aInputData, 
                                     TMccEvent& aOutputData );
                                     
        void DoCodecSpecificConfigurationL( TUid& aEncoderUid,TBool& aForceEncapsulationType );
        
        HBufC8* GetFmtpLC( const TMccCodecInfo& aCodecInfo ); 
        
        void SetConfigKeyL( const TDesC8& aData );
        
        TBool FullPrepareAllowed();
        
        void WriteFirstBufferL();
        
        /**
        * Adds buffer to queue
        * @param aBuffer, ownership is transferred
        */
        void AddIFrameToQueueL( CMMFDataBuffer* aBuffer );
        
        void HandleCameraReservationL();
        
        void StopCameraMonitoring();
                                     
    private:
    
		// video codec fourcc code
		HBufC8* iVideoCodecFourCC;
		TFourCC iVideoFourCC;
		TFourCC iAudioFourCC;
		
		CMccVideoSourceWrapper* iMediaRecorder;
		
        MDataSource* iAudioSource;
		MMccCameraHandler* iCameraHandler;
	    TMMFPrioritySettings iPrioritySettings;
	    
        TMccCodecInfo iVideoCodec;

        TInt iAverageAudioBitRate;
        TInt iAudioGain;
        TBool iAudioEnabled;
     
		TReal32 iFrameRate;

	    CMMFDataBuffer* iFrameVideoBufferOne;
	    MDataSink* iVideoConsumer;
	    CMMFBuffer* iVideoConsumerBuffer;
	    		
		MAsyncEventHandler* iAsyncEventHandler;
		
		TUint32 iEndpointId;
		MMccResources* iMccResources;
		TUint	iMaxNegBitRate;
		
		TBool iAutoPlayMode;
		
		RPointerArray<CMMFDataBuffer> iVideoQueue;
		
		TBool iMultipleCodecs;
		
		// Key as base64 encoded
		HBufC8* iConfigKey;
		
		// Key as raw data
		HBufC8* iConfigKeyData;
		
		TBool iConfigKeyRetrieval;
		
		TBool iFirstIFrameQueued;
		
	private:
		#ifdef TEST_EUNIT
			friend class UT_CMccVideoSourceImpl;
			friend class UT_CMccVideoSource;	
   		#endif
	};


#endif 


// End of file
