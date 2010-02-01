/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class definition for the AMR/RTP payload format plugin
*
*/




#ifndef AMRPAYLOADFORMATREAD_H
#define AMRPAYLOADFORMATREAD_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <mmf/server/mmfformat.h>
#include "rtpheader.h"
#include "formatstatemachine.h"
#include "amrpayloadformatter.h"
#include "streamformatter.h"
#include "mccinternalcodecs.h"
#include "mccinternalevents.h"

#ifdef FTD_ENABLED
#include <e32msgqueue.h>
#include "MccJBufferStats.h"
#endif


// FORWARD DECLARATIONS
class CAmrPayloadEncoder;
class CAmrPayloadDecoder;

// CLASS DECLARATION

/**
*  The CAmrPayloadFormatRead class implements the MMF format plugin for decoding 
*  of RTP payloadformat for AMR codec. Refer to IETF RFC3267.
*  This class receives AMR payload data from an RTP packet and converts it into
*  one or more AMR frames.
*
*  @lib MMccamrplformat.dll
*  @since Series 60 3.0
*/ 
class CAmrPayloadFormatRead : public CPayloadFormatRead, public MPayloadFormatRead
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aSource    Source of the data for the payload format plugin
        */
        static   CAmrPayloadFormatRead* NewL ( MDataSource* aSource );

        /**
        * Destructor.
        */
        virtual  ~CAmrPayloadFormatRead ( );

    public:  // New functions

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
        * Accessor funtion to return the number of channels
        *
        * @return   Number of channels
        */
        TUint NumChannels( );

        /**
        * Accessor funtion to return the sampling rate
        *
        *
        * @return   Sampling rate
        */
        TUint SampleRate( );

        /**
        * Set the packetizationrate to the given value.
        *
        * @param aPacketizationRate  - [input] Number of AMR frame blocks per RTP packet
        *
        * @return  error code KErrNotSupported if invalid packetizationrate, else return KErrNone
        */
        TInt SetSampleRate( TUint aSampleRate );
        
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
        
        /**
         * Update payload format with format spesific parameters.
         * Leaves with KErrArgument if parameters are invalid.
         *
         * @since    Series 60 3.0 
         * @param    aCodecInfo       Configuration parameters
         * @return   void
         */
        virtual void UpdateConfigurationL( const TMccCodecInfo& aCodecInfo  );

    private:
        /**
        * C++ default constructor.
        */
        CAmrPayloadFormatRead( );

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aSource    Source of the data for the payload format plugin
        */
        void ConstructL( MDataSource* aSource );

        /**
        * Reads data starting from the given position into the source buffer 
        *
        * @param   aBuffer     RTP payload buffer
        *
        * @return  TBool    indication of finish decoding RTP payload
        */
        TBool DoRead( CMMFBuffer* aBuffer );

        /**
        * Send Amr event to the client .
        *
        * @param aEventType
        */
        void SendAmrEventToClient( TMccInternalEventType aEventType );
        
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
        
        /**
         * Gives corresponding bitrate for mode request
         * @param aModeRequest
         * @return bitrate
         */
        TInt ConvertModeToBitrate( TAmrModeRequest aModeRequest );
        
    private:    // data

		// True if this AMR narrow band read, False if wideband
		TBool iIsNb;

        // Data source providing RTP data
        MDataSource* iRtpDataSource;

        // The fourCC for AMR-NB
        TFourCC iFourCC;

        // Flag used to determine which buffer is currently used as a sink buffer
        enum TCurrentBuffer
            {
            EBufferOne = 0,
            EBufferTwo
            };

        // Sampling rate from clip header
        TInt iSamplingRate; 

        // Number of channels from clip header
        TUint iChannels;

        // Mode from clip header
        TInt iMode;  

        // Payload decoder
        CAmrPayloadDecoder* iPayloadDecoder;

        TMediaId iMediaId;
        
        // Data buffer passed from RtpSource
        CMMFBuffer* iBufferToRead;

        // Indicates are there frames not decoded remaining
        TBool iBufferToReadExists;
        
        // Number of frames decoded from RTP packet
        TInt iNumOfFrames;

        // Frame index indicating are all frames decoded passed to datapath
        TInt iCurrentFrame;
        
        // Maximum number of frames expected to receive in one RTP packet
        TInt iFramesPerPacket;
        
        // AMR mode request, used to construct RTP AMR payload header
        TAmrModeRequest iCmr;

        // Format decoding state machine
        CFormatDecodeStateMachine* iStateMachine;
        
        // Buffer to store depacketized AMR frames 
        CMMFDataBuffer* iFrameBufferOne;
        
        // Alternate buffer to store  depacketized AMR frames
        CMMFDataBuffer* iFrameBufferTwo;
        
        // Flag to indicate which buffer is in use
        TCurrentBuffer iCurrentBuffer;
        
        // Buffer to hold payload got from data source
        CMMFDataBuffer* iSourceBuffer;

        // Indicates whether payload read has ownership of source buffer
        TBool iSourceBufOwnership;
        
        // Flag to indicate the outstanding request from data path
        TBool iFillRequested;
        
        // Encoder
        TStreamEncoder iStreamEncoder;

		// RTP Header
		TRtpRecvHeader iRecvHeader;
        
        // Parameters relating to RTP data decoding
        TMccCodecInfo iCInfo;
        
        // MMF AsyncEventHandler
        MAsyncEventHandler* iEventHandler;
                
        #ifdef FTD_ENABLED
        // Message queue carrying jitter buffer statistics
        RMsgQueue<TMccJBufferStats> iJBufStatsQueue;
        
        // For RTP packet loss detection
        TUint32 iLatestSeqNum;
        
        // Number of frames decoded after starting
        TUint32 iTotalFrames;
        
        // Number of frames lost due to packet loss
        TUint32 iLostFrames;
        #endif

    private:    // Friend classes
    
    #ifdef TEST_EUNIT
        friend class UT_CAmrPayloadFormatRead;
    #endif
    
    };

#endif //AMRPAYLOADFORMATREAD_H
