/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef ANYPAYLOADFORMATREAD_H
#define ANYPAYLOADFORMATREAD_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <mmf/server/mmfformat.h>
#include "rtpheader.h"
#include "formatstatemachine.h"
#include "mccinternalcodecs.h"


// CLASS DECLARATION
class CMMFBuffer;

/**
*
*/ 
class CAnyPayloadFormatRead : public CPayloadFormatRead,
                              public MPayloadFormatRead
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aSource    Source of the data for the payload format plugin
        */
        static   CAnyPayloadFormatRead* NewL ( MDataSource* aSource );

        /**
        * Destructor.
        */
        virtual  ~CAnyPayloadFormatRead ( );

    public:  // New functions

        /**
        * Get number of audio packets received on downlink in last talk spurt.
        * @since Series-60 2.1
        * @return Number of audio packets received
        */
        virtual TInt LastDlPacketCount( );

        /**
        * Cancel any requests from DataPath and Stop state machine also
        * @since Series-60 2.1
        * @return None
        */
        virtual void CancelDlRequest( );

        /**
        * Send full sink buffer with AMR frames to data path.
        * Sink buffer is decided on one of the two frame buffers
        * Called by the state machine.
        * @since Series-60 2.1
        * @return None
        */
        void SendDataToSinkL( );

        /*
        * Fill the sink buffer by decoding the RTP payload into AMR frames
        * Sink buffer is decided on one of the two frame buffers
        * Called by the state machine.
        * @since Series-60 2.1
        * @return None
        */
        void FillSinkBufferL( );

        /**
        * Request the RTPSource to fill the source buffer with RTP payload
        * Source buffer is decided by the RTPSource.
        * Called by the state machine.
        * @since Series-60 2.1
        * @return None
        */
        void FillSourceBufferL( );
        
    public: // From CMMFFormatDecode

        /**
        * From CMMFFormatDecode. Return number of audio streams for the given
        * media Raw audio files can only have 1 audio stream.
        *
        * @param    aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   Number of audio streams
        */
        TUint Streams( TUid aMediaType ) const;

        /**
        * Return the frame time interval for the given media
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           Time interval in micro seconds
        */
        TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType ) const;

        /**  *** NOT SUPPORTED ***
        * Return the clip duration for the given media
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           Clip duration in micro seconds
        */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;

        /**
        * From CMMFFormatDecode
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );
        
    public: // From MDataSource.  CAmrPayloadFormatRead is data source to the data path and AMR codec.

        /**
        * From MDataSource. Read the next block of data from file into the
        * given buffer.
        *
        * @param aBuffer    source data buffer for the AMR-NB data read
        * @param aConsumer  data sink pointer
        * @param aMediaId   contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return
        */
        void FillBufferL( CMMFBuffer* aBuffer, 
                          MDataSink* aConsumer,
                          TMediaId aMediaId );

        /**
        * From MDataSource. Primes the source.
        * @since Series 60 3.0
        * @return None
        */
        void SourcePrimeL( );

        /**
        * From MDataSource. Plays the source.
        * @since Series 60 3.0
        * @return None
        */
        void SourcePlayL( );
        
        /**
        * From MDataSource. Pauses the source.
        * @since Series 60 3.0
        * @return None
        */
        void SourcePauseL( );
        
        /**
        * From MDataSource. Stops the source.
        * @since Series 60 3.0
        * @return None
        */
        void SourceStopL( );

        /**
        * Create a source buffer for the given media
        *
        * @param aMediaId   contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        * @param aReference value of False is returned to caller if source buffer is created
        *
        * @return           Pointer to source buffer created
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, 
                                         TBool &aReference );

        /**
        * Create a source buffer for the given media, setting frame size to match 
        * the given sink buffer
        *
        * @param aMediaId    contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        * @param aSinkBuffer address of sink buffer
        * @param aReference  value of False is returned to caller if source buffer is created
        *
        * @return            Pointer to source buffer created
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, 
                                         CMMFBuffer& aSinkBuffer, 
                                         TBool &aReference );

        /**
        * Return the four CC code for the given media
        *
        * @param aMediaId    contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return            FourCC code
        */
        TFourCC SourceDataTypeCode( TMediaId aMediaId );

        /**
        * Set the source data type to the given four CC code for the given media
        *
        * @param aSourceFourCC    fourCC code
        * @param aMediaId         contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return                 error code KErrNotSupported if invalid media ID, else return KErrNone
        */
        TInt SetSourceDataTypeCode( TFourCC aSourceFourCC, 
                                    TMediaId aMediaId ); 

        /**
        * Log in to the source thread - this funtion merely passes the command to 
        * its source clip data source object. The source clip object will handle 
        * the thread log on procedures.
        *
        * @param aEventHandler    address of event handler
        *
        * @return                 error code returned by source clip
        */
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * Log out of the source thread - this funtion merely passes the command to 
        * its source clip data source object. The source clip object will handle 
        * the thread log off procedures.
        *
        * @return None
        */
        void SourceThreadLogoff( );

        /**
        * Negotiate source settings ( buffer size ) to match 
        * given source object
        *
        * @param aDataSink          address of sink object
        *
        * @return None
        */
        void NegotiateSourceL( MDataSink& aDataSink );


    public: // From MDataSink. CAmrPayloadFormatRead is data sink to the RTP data source.

        /**
        * Called after the data buffer is filled. Update the number of bytes read
        * and the current read position for the next read operation. 
        *
        * @param    aBuffer         data buffer filled
        * @param    aHeaderInfo     RTP packet header information
        * @return None
        */
        virtual void DataBufferFilledL( CMMFBuffer* aBuffer, 
                                    const TRtpRecvHeader& aHeaderInfo );

    public: // only for videosource-anyformatter-filesink
       void BufferFilledL( CMMFBuffer* aBuffer );
    public:
    
        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        *
        * @since    Series 60 3.0 
        * @param    aConfigParams       Configuration parameters
        * @return   void
        */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams );
        

    protected:  // Functions from base classes
    private:
        /**
        * C++ default constructor.
        */
        CAnyPayloadFormatRead();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aSource    Source of the data for the payload format plugin
        */
        void ConstructL( MDataSource* aSource );

        /**
         * Creates a buffer used in data transfer between format read and
         * its datasource.
         *
         * @since    Series 60 3.2 
         * @param    aSize              Suggested buffer size
         * @param    aIsOwnBuffer       Indicates ownership of the buffer
         * @return   Created buffer
         */
        CMMFDataBuffer* CreateClipBufferL( TUint aSize, TBool& aIsOwnBuffer );
    
    private:    // data

        // Flag used to determine which buffer is currently used as a sink buffer
        enum TCurrentBuffer
            {
            EBufferOne = 0,
            EBufferTwo,
            EBufferNone
            };
            
        // Data source providing RTP data
        MDataSource* iRtpDataSource;

        TFourCC iFourCC;

        TMediaId iMediaId;

		TBool iFillRequested;
		
		TBool iBufferToReadExists;
                
        // Format decoding state machine
        CFormatDecodeStateMachine* iStateMachine;
        
        // Buffer one to store data 
        CMMFDataBuffer* iFrameBufferOne;
        
        // Buffer two to store data 
        CMMFDataBuffer* iFrameBufferTwo;
        
        // Current buffer in use
        CMMFDataBuffer* iCurrentBuffer;
        
        // Buffer to hold payload got from RtpSourceSink
        CMMFDataBuffer* iPayloadBuffer;
        
        // Indicates whether payload read has ownership of source buffer
        TBool iSourceBufOwnership;
        		
		// Parameters relating to RTP data decoding
        TMccCodecInfo iCInfo;
        
        // MMF AsyncEventHandler
        MAsyncEventHandler* iEventHandler;
        
        TInt iPacketCount;

        TInt iDroppedPacketCount;
        
   private:    // Friend classes
    
   #ifdef EUNIT_TEST
        friend class UT_CAnyPayloadFormatRead;
   #endif
    };

#endif 
