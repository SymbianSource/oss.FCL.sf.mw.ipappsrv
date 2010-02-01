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
* Description:    Reads RTP payload format containing ILBC data.
*
*/



#ifndef ILBCPAYLOADFORMATREAD_H
#define ILBCPAYLOADFORMATREAD_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfformat.h>
#include "rtpheader.h"
#include "formatstatemachine.h"
#include "mccinternalcodecs.h"

// CLASS DECLARATION
/**
*  Depacketizes RTP payload containing ILBC data.
*
*  @lib MMCCIlbcPlFormat.dll
*  @since Series 60 3.0
*/
class CIlbcPayloadFormatRead : public CPayloadFormatRead, 
                               public MAsyncEventHandler, 
                               public MPayloadFormatRead
    {
    public:
        /**
        * Two-phased constructor.
        * @param aSource    Data source configuration parameters
        */
        static CIlbcPayloadFormatRead* NewL ( MDataSource* aSource );

        /**
        * Destructor.
        */
        virtual ~CIlbcPayloadFormatRead();

    public: // New functions

        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        *
        * @since    Series 60 3.0 
        * @param    aConfigParams       Configuration parameters
        * @return   void
        */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams );
        
    public: // From CMMFFormatDecode

        /**
        * Return number of audio streams for the given media
        * Raw audio  files can only have 1 audio stream
        * @since Series 60 3.0
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return Number of audio streams
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
        * @param
        *
        * @return   Number of channels
        */
        TUint NumChannels();

        /**
        * Accessor funtion to return the sampling rate
        *
        * @param
        *
        * @return   Sampling rate
        */
        TUint SampleRate();
        
        /**
        * Set the samplerate to the given value.
        *
        * @param aSampleRate
        *
        * @return error code KErrNotSupported if invalid samplerate, else 
        *         return KErrNone
        */
        TInt SetSampleRate( TUint aSampleRate );

    public: // From MDataSource

        /**
        * Return the four CC code for the given media
        *
        * @param aMediaId    contains the media type KUidMediaTypeAudio or 
        *                    KUidMediaTypeVideo
        *
        * @return            FourCC code
        */
        TFourCC SourceDataTypeCode( TMediaId aMediaId );

        /**
        * Set the source data type to the given four CC code for the given 
        * media
        *
        * @param aSourceFourCC    fourCC code
        * @param aMediaId         contains the media type KUidMediaTypeAudio 
        *                         or KUidMediaTypeVideo
        *
        * @return                 error code KErrNotSupported if invalid 
        *                         media ID, else return KErrNone
        */
        TInt SetSourceDataTypeCode( TFourCC aSourceFourCC, 
                                    TMediaId aMediaId ); 

        /**
        * Create a source buffer for the given media
        *
        * @param aMediaId   contains the media type KUidMediaTypeAudio or 
        *                   KUidMediaTypeVideo
        * @param aReference value of False is returned to caller if source 
        *                   buffer is created
        *
        * @return           Pointer to source buffer created
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, 
                                         TBool &aReference );

        /**
        * Create a source buffer for the given media, setting frame size 
        * to match the given sink buffer
        *
        * @param aMediaId    contains the media type KUidMediaTypeAudio or 
        *                    KUidMediaTypeVideo
        * @param aSinkBuffer address of sink buffer
        * @param aReference  value of False is returned to caller if source 
        *                    buffer is created
        *
        * @return            Pointer to source buffer created
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, 
                                         CMMFBuffer& aSinkBuffer, 
                                         TBool &aReference );

        /**
        * Read the next block of data from file into the given buffer
        *
        * @param aBuffer    source data buffer for the AMR-NB data read
        * @param aConsumer  data sink pointer
        * @param aMediaId   contains the media type KUidMediaTypeAudio or 
        *                   KUidMediaTypeVideo
        *
        * @return
        */
        void FillBufferL( CMMFBuffer* aBuffer, 
                          MDataSink* aConsumer, 
                          TMediaId aMediaId );

        /**
        * Log in to the source thread - this funtion merely passes the 
        * command to its source clip data source object. The source clip 
        * object will handle the thread log on procedures.
        *
        * @param aEventHandler    address of event handler
        *
        * @return                 error code returned by source clip
        */
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * Log out of the source thread - this funtion merely passes the 
        * command to its source clip data source object. The source clip 
        * object will handle the thread log off procedures.
        *
        * @param
        * @return
        */
        void SourceThreadLogoff();

        /**
        * Negotiate source settings (buffer size) to match 
        * given source object
        *
        * @param aDataSink          address of sink object
        *
        * @return
        */
        void NegotiateSourceL( MDataSink& aDataSink );
    
        /**
        * From MDataSource Primes the source.
        * @since 
        * @param None
        */    
        void SourcePrimeL();

        /**
        * From MDataSource Plays the source.
        * @since 
        * @param None
        */    
        void SourcePlayL();

        /**
        * From MDataSource Pauses the source.
        * @since 
        * @param None
        */
        void SourcePauseL();

        /**
        * From MDataSource Stops the source.
        * @since 
        * @param None
        */    
        void SourceStopL();
        
    public: // From MDataSink

        /**
        * Called after the data buffer is filled. Update the number of bytes
        * read and the current read position for the next read operation. 
        *
        * @param aBuffer     data buffer filled
        *
        * @return
        */
        virtual void DataBufferFilledL( CMMFBuffer* aBuffer, 
                                    const TRtpRecvHeader& aRecvHeader );

        /**
        * Gets the data type code for the sink specified by the media ID.
        * Format decode may act in role of MDataSink.
        *
        * @param    aMediaId       Media ID
        * @return   The data sink type code
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );                                    

    public: // From MAsyncEventHandler
        virtual TInt SendEventToClient(const TMMFEvent& /*aEvent*/) 
            {return KErrNone;}
            
    public: // From MPayloadFormatRead
    
        /**
        * Send full sink buffer with iLBC frames to data path.
        * Sink buffer is decided on one of the two frame buffers
        * Called by the state machine.
        * @since Series 60 3.0
        * @param None
        */    
        void SendDataToSinkL();

        /**
        * Fill the sink buffer by decoding the RTP payload into iLBC frames
        * Sink buffer is decided on one of the two frame buffers
        * Called by the state machine.
        * @since Series 60 3.0
        * @param None
        */    
        void FillSinkBufferL();

        /**
        * Request the RTPSource to fill the source buffer with RTP payload
        * Source buffer is decided by the RTPSource.
        * Called by the state machine.
        * @since Series 60 3.0
        * @param None
        */    
        void FillSourceBufferL();
            
    protected:  // New functions
    
    protected:  // Functions from base classes
    
    private: // New functions
    
        /**
        * Decode audio frames from RTP payload to the audio frame array. 
        * Next decoded frame may be requested by GetNextFrameL().
        *
        * @since    Series 60 3.0
        * @param    aSourceBuffer      RTP payload to be decoded
        * @return   Number of decoded frames
        */
        TInt DecodePayload( const TDesC8& aSourceBuffer );
        
        /**
        * Return next decoded audio frame.
        * @since    Series 60 3.0
        * @param    aToBuffer   - [output] Buffer to place decoded frame
        * @return   Boolean indicating whether decoded frames are remaining.
        */
        TBool GetNextFrame( TDes8& aToBuffer );
        
        /**
         * Update payload format with format spesific parameters.
         *
         * @since    Series 60 3.0 
         * @param    aCodecInfo       Codec info
         * @return   void
         */
        void UpdateConfigurationL( const TMccCodecInfo& aCodecInfo );
        
        /**
         * Configures the formatter based on input parameters.
         *
         * @since    Series 60 3.0 
         * @param    aCodecInfo       Codec info
         * @return   void
         */
        void DoConfigurePayloadFormatL( const TMccCodecInfo& aCodecInfo );

    private:
    
        /**
        * C++ default constructor.
        */
        CIlbcPayloadFormatRead ();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aConf    Decoder Configuration params
        */
        void ConstructL ( MDataSource* aSource );
        
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

    private:     // Data

        enum TCurrentBuffer
            {
            EBufferOne = 0,
            EBufferTwo
            };
        
        // the fourCC
        TFourCC iFourCC;             

        // Format decoding state machine
        CFormatDecodeStateMachine* iStateMachine;
        
        // ETrue if decoded frames are remaining
        TBool iBufferToReadExists;

        // Number of frames decoded from received RTP payload
        TInt iNumOfFrames;
        // Data sink that requested "FillBuffer".
        MDataSink* iSink;               

        // Frame time interval in micro seconds
        TTimeIntervalMicroSeconds iFrameTimeInterval;

        // Buffer to store depacketized Ilbc frames 
        CMMFDataBuffer* iFrameBufferOne;

        // Alternative buffer to store  depacketized Ilbc frames
        CMMFDataBuffer* iFrameBufferTwo;

        // Flag to indicate which buffer is in use
        TCurrentBuffer iCurrentBuffer;
        
        // Buffer to hold payload got from data source;
        CMMFDataBuffer* iSourceBuffer;
        
        // Indicates whether payload read has ownership of source buffer
        TBool iSourceBufOwnership;

        TMediaId iMediaId;
        
        // Flag to indicate that received frame was comfort noise
        TBool iCnFrame;

        // Current index in set of audio frames decoded from last payload
        TInt iFrameIndex;

        // Contains pointers to audio frames in received payload
        RArray<TPtr8> iFrameArray;
        
        // RTP Header
        TRtpRecvHeader iRecvHeader;        
        
        // Parameters relating to RTP data encoding
        TMccCodecInfo iCInfo; 
                
        // Friend class for testing
        #ifdef TEST_EUNIT
            friend class UT_CIlbcPayloadFormatRead;
        #endif
    };

#endif /* ILBCPAYLOADFORMATREAD_H */

// End of File
