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
#ifndef MCCFILESOURCEIMPL_H
#define MCCFILESOURCEIMPL_H

#include <e32base.h>
#include <f32file.h>
#include <e32std.h>
#include <mmf/server/mmfdatasource.h>
#include <mmf/server/mmfdatasink.h>
#include <3gplibrary/mp4lib.h>

#include "CCMRMediaSink.h"
#include "mccperiodicrunner.h"
#include "mmccevents.h"
#include "mcceoftimer.h"

// percentage that filereader is behind ROP for playback sync
const TUint KMccMicroToMilliConst = 1000;

// CLASS DECLARATION
class CMccFileSourceTypeBase;
class CMccFileAudio;
class CMccFileVideo;
class CMccEofTimer;
class MMccResources;

/**
*  
*/
class CMccFileSourceImpl : 
	public CBase, public MMccEofTimerObserver
    {

    public: // Methods called internally or by the controller

        /**
        * Two-phased constructor.
        */
        static CMccFileSourceImpl* NewL( TUint32 aEndpointId, MMccResources* aMccResources = NULL );
                                        		
		CMccFileSourceImpl( TUint32 aEndpointId );
		
		virtual ~CMccFileSourceImpl();

		/**
	    * Sets the media sink for output
	    * @since SWIS Lite
	    * @param MCMRMediaSink* aSink
	    */
	    virtual void AddDataSink( MCMRMediaSink* aSink );

	    /**
	    * Opens a 3GP file for streaming
	    * @since SWIS Lite
	    * @param TFileName aFileName
	    */
	    virtual void OpenFileL( TFileName aFileName, TBool aFileChangeMode = EFalse );

	    /**
	    * Closes the 3GP file
	    * @since SWIS Lite
	    */
	    virtual void CloseFileL( TBool aFileChangeMode = EFalse );

        /**
	    * Gets filename
	    */
        TFileName& FileNameL();
        
	    /**
	    * Seeks to given position in a file
	    * @since SWIS Lite
	    * @param TTimeIntervalMicroSeconds aPosition
	    */
	    virtual void SetPositionL( TTimeIntervalMicroSeconds aPosition );
	    
	    /**
	    * Returns the current video type
	    * @since SWIS Lite
	    * @param 
	    * @return CCMRMediaBuffer::TBufferType
	    */
	    virtual CCMRMediaBuffer::TBufferType VideoTypeL();
	    
	    /**
	    * Returns the current audio type
	    * @since SWIS Lite
	    * @return CCMRMediaBuffer::TBufferType
	    */
	    virtual CCMRMediaBuffer::TBufferType AudioTypeL();

	    /**
	    * Returns the current video framerate
	    * @since SWIS Lite
	    * @return TReal32
	    */
	    virtual TReal VideoFrameRateL();

	    /**
	    * Returns the current audio bitrate
	    * @since SWIS Lite
	    * @param 
	    * @return TUint
	    */
	    virtual TUint32 AudioBitRateL();

	    /**
	    * Returns the duration of media clip
	    * @since SWIS Lite
	    * @param 
	    * @return TTimeIntervalMicroSeconds
	    */
	    virtual TTimeIntervalMicroSeconds DurationL();

	    /**
	    * Returns the video framesize
	    * @since SWIS Lite
	    * @param 
	    * @return TSize
	    */
	    virtual TSize VideoFrameSizeL();

	    /**
	    * Returns the video bitrate
	    * @since SWIS Lite
	    * @param 
	    * @return TUint
	    */
	    virtual TUint32 VideoBitRateL();

	    /**
	    * Returns the current reading position
	    * @since SWIS Lite
	    * @param 
	    * @return TTimeIntervalMicroSeconds
	    */
	    virtual TTimeIntervalMicroSeconds PositionL();
	    
	    /**
	    * Returns the audio fourCC
	    * @return TFourCC
	    */
	    virtual TFourCC AudioFourCCL();
	    
	    /**
	    * Returns the video fourCC
	    * @return TFourCC
	    */
	    virtual TFourCC VideoFourCCL();
	    
	    /**
	    * Returns config key of opened clip
	    * @param aConfigKey, config key is set to this input param
	    */
	    void GetConfigKeyL( TDes8& aConfigKey );
	    

    public: // From MDataSource

        /**
        * From MDataSource. Returns the data type as a fourCC code of the data source.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        * @return    The fourCC code identifying the source datatype for the specified aMediaId.
        */
        TFourCC SourceDataTypeCode( TMediaId /*aMediaId*/ );

        /**
        * From MDataSource. Sets the data type as a fourCC code for the data source
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
        * From MDataSource. Method to 'prime' the data source.
        *
        * Datasource is set to primed state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePrimeL();
        
        /**
        * From MDataSource. Method to 'play' the data source.
        *
        * Downlink timeout timer is started and datasource is set to playing state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePlayL( TBool aIgnoreEvents = EFalse, TBool aFileChangeMode = EFalse );
        
        /**
        * From MDataSource. Method to 'pause' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to paused state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePauseL();
        
        /**
        * From MDataSource. Method to 'stop' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to stopped state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourceStopL( TBool aIgnoreEvents = EFalse, TBool aFileChangeMode = EFalse );

        /**
        * From MDataSource. Method called by a MDataSink to request the data source to fill aBuffer with data.
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

		TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
		
		void SourceThreadLogoff();
		
    public:
        void PauseAudioL( TInt aError = KErrNone );
        void PauseVideoL( TInt aError = KErrNone );
	    
        void SetDelayAfterStop( TInt aDelay );
  	  	TInt DelayAfterStop() const;

        void StopEofTimer(); 
        void EofTimerExpiredL(); // from MMccEofTimerObserver
        
        void InvalidVideoFrame( TBool aInvalid = EFalse );
  	  	
    public:
    
    	static TTimeIntervalMicroSeconds MilliToMicroSeconds( TUint32 aMilliSeconds );
		static TUint32 MicroToMilliSeconds( TTimeIntervalMicroSeconds aMicroSeconds );
		
    private:

		void ConstructL( MMccResources* aMccResources );
		
	private:
	    void FileOpenErrorL( MP4Err error );	 
	    
	    void SendStreamEventToClient( TMccEventCategory aEventCategory,
	                                  TMccEventType aEventType, 
	                                  TInt aError = KErrNone,
	                                  TUint32 aEventNumData = 0 );   
		
		void SetFileName( const TFileName& aFileName );
		
	    void OpenFileForStreamingL( TFileName aFileName );
		
		TBool AudioInUse() const;
		
		TBool VideoInUse() const;
		
		void DoCleanupL( TBool aFileChangeMode );
		
        void StartEofTimerL();
    
        void ClearEvent();
        
        TBool StartNeededL( CMccFileSourceTypeBase& aFileSource );
        
	
    private:
   
	    // 3GP library handle
	    MP4Handle   iMP4Handle;
        
		enum TCurrentState 
		    {
		    EConstructed = 0,
		    EPrimed,       
		    EPaused,
		    EWaitingPause,
		    EStopped, // Equals EConstructed
		    EPlaying // Streaming (timers active)
		    };  
    
	    TCurrentState iCurrentState;
		
		TBool iIsAudio;
		TBool iIsVideo;
	    CMccFileAudio* iFileAudio;
    	CMccFileVideo* iFileVideo;
    	
    	MAsyncEventHandler* iAsyncEventHandler;
    	
    	TUint32 iEndpointId;
    	
    	TFileName iFileName;
    	
    	TMccEvent iMccEvent;
    
        TInt iDelayAfterStop;    	
    	
        CMccEofTimer* iEofTimer;
    
	#ifdef EUNIT_TEST
        friend class UT_CMccFileSourceImpl;
        friend class UT_CMccMultiplexer;
    #endif	
	};


#endif  // end of MCCFILESOURCEIMPL_H


// End of file
