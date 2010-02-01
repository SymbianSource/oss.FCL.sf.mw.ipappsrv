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
* Description:    Reads RTP payload format containing G729 data.
*
*/



#ifndef G729PAYLOADFORMATREAD_H
#define G729PAYLOADFORMATREAD_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfformat.h>
#include "rtpheader.h"
#include "formatstatemachine.h"
#include "mccinternalcodecs.h"

// CLASS DECLARATION
/**
*  Depacketizes RTP payload containing G729 data.
*
*  @lib ?library
*  @since Series 60 3.0
*/
class CG729PayloadFormatRead : public CPayloadFormatRead, 
                               public MAsyncEventHandler, 
                               public MPayloadFormatRead
    {
    public:
        /**
        * Two-phased constructor.
        * @param aSource    Data source configuration parameters
        */
        static   CG729PayloadFormatRead* NewL (MDataSource* aSource);

        /**
        * Destructor.
        */
        virtual  ~CG729PayloadFormatRead ();

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
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return Number of audio streams
        */
        virtual TUint Streams(TUid aMediaType) const;

        /**
        * Return the frame time interval for the given media
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           Time interval in micro seconds
        */
        virtual TTimeIntervalMicroSeconds FrameTimeInterval(
                                            TMediaId aMediaType) const;

        /**  *** NOT SUPPORTED ***
        * Return the clip duration for the given media
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           Clip duration in micro seconds
        */
        virtual  TTimeIntervalMicroSeconds Duration(
                                            TMediaId /*aMediaType*/) const;

        /**
        * Accessor funtion to return the number of channels
        *
        * @param
        *
        * @return   Number of channels
        */
        virtual TUint NumChannels();

        /**
        * Accessor funtion to return the sampling rate
        *
        * @param
        *
        * @return   Sampling rate
        */
        virtual TUint SampleRate();
        
        /**
        * Set the samplerate to the given value.
        *
        * @param aSampleRate
        *
        * @return error code KErrNotSupported if invalid samplerate, else 
        *         return KErrNone
        */
        virtual TInt SetSampleRate (TUint aSampleRate);

    public: // From MDataSource

        /**
        * Return the four CC code for the given media
        *
        * @param aMediaId    contains the media type KUidMediaTypeAudio or 
        *                    KUidMediaTypeVideo
        *
        * @return            FourCC code
        */
        virtual TFourCC SourceDataTypeCode(TMediaId aMediaId);

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
        virtual TInt SetSourceDataTypeCode(
                                           TFourCC aSourceFourCC, 
                                           TMediaId aMediaId
                                           ); 

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
        virtual CMMFBuffer* CreateSourceBufferL(
                                                TMediaId aMediaId, 
                                                TBool &aReference
                                                );

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
        virtual CMMFBuffer* CreateSourceBufferL(
                                                TMediaId aMediaId, 
                                                CMMFBuffer& aSinkBuffer, 
                                                TBool &aReference
                                                );

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
        virtual void FillBufferL(
                                 CMMFBuffer* aBuffer, 
                                 MDataSink* aConsumer, 
                                 TMediaId aMediaId
                                 );

        /**
        * Log in to the source thread - this funtion merely passes the 
        * command to its source clip data source object. The source clip 
        * object will handle the thread log on procedures.
        *
        * @param aEventHandler    address of event handler
        *
        * @return                 error code returned by source clip
        */
        virtual TInt SourceThreadLogon(MAsyncEventHandler& aEventHandler);

        /**
        * Log out of the source thread - this funtion merely passes the 
        * command to its source clip data source object. The source clip 
        * object will handle the thread log off procedures.
        *
        * @param
        * @return
        */
        virtual void SourceThreadLogoff();

        /**
        * Negotiate source settings (buffer size) to match 
        * given source object
        *
        * @param aDataSink          address of sink object
        *
        * @return
        */
        virtual void NegotiateSourceL(MDataSink& aDataSink);
        

        /**
        * From MDataSource Primes the source.
        * @since 
        * @param None
        */    
        virtual void SourcePrimeL();

        /**
        * From MDataSource Plays the source.
        * @since 
        * @param None
        */    
        virtual void SourcePlayL();

        /**
        * From MDataSource Pauses the source.
        * @since 
        * @param None
        */
        virtual void SourcePauseL();

        /**
        * From MDataSource Stops the source.
        * @since 
        * @param None
        */    
        virtual void SourceStopL();        
        
            
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
        * @since 
        * @param None
        */    
        void SendDataToSinkL();

        /**
        * Fill the sink buffer by decoding the RTP payload into iLBC frames
        * Sink buffer is decided on one of the two frame buffers
        * Called by the state machine.
        * @since 
        * @param None
        */    
        void FillSinkBufferL();

        /**
        * Request the RTPSource to fill the source buffer with RTP payload
        * Source buffer is decided by the RTPSource.
        * Called by the state machine.
        * @since 
        * @param None
        */    
        void FillSourceBufferL();
    
    private: // New functions
    
        /**
        * Decode audio frames from RTP payload to the audio frame array. 
        * Next decoded frame may be requested by GetNextFrameL().
        *
        * @since    Series 60 3.0
        * @param    aSourceBuffer      RTP payload to be decoded
        * @return   void
        */
        void DecodePayload( const TDesC8& aSourceBuffer );
        
        /**
        * Return next decoded audio frame.
        * @since    Series 60 3.0
        * @param    aToBuffer   - [output] Buffer to place decoded frame
        * @return   void
        */
        void GetNextFrame( TDes8& aToBuffer );
        
        /**
        * Does unpacking for bit-packed G729 RTP payload.
        * @since    Series 60 3.0
        * @param    aSourceBuf             Source buffer where to read from
        * @param    aDestBuf               Destination buffer where to place unpacked data
        * @param    aIsCNoise              Indicates does source buf contain comfort noise
        * @return   System wide error code.
        */
        TInt DoBitUnPacking( const TDesC8& aSourceBuf,
                             TDes8& aDestBuf,
                             TBool aIsCNoise ) const;
                             
        /**
         * Update payload format with format spesific parameters.
         * Leaves with KErrArgument if parameters are invalid.
         *
         * @since    Series 60 3.0 
         * @param    aCodecInfo       Configuration parameters
         * @return   void
         */
        void UpdateConfigurationL( TMccCodecInfo& aCodecInfo );                              

    private:
    
        /**
        * C++ default constructor.
        */
        CG729PayloadFormatRead ();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aConf    Decoder Configuration params
        */
        void ConstructL (MDataSource* aSource);
        
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

    private: // Data
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

        // Frame time interval in micro seconds
        TTimeIntervalMicroSeconds iFrameTimeInterval;

        // Buffer to store depacketized G729 frames 
        CMMFDataBuffer* iFrameBufferOne;

        // Alternative buffer to store  depacketized G729 frames
        CMMFDataBuffer* iFrameBufferTwo;

        // Buffer to hold payload got from data source;
        CMMFDataBuffer* iSourceBuffer;
        
        // Indicates whether payload read has ownership of source buffer
        TBool iSourceBufOwnership;

        // Flag to indicate which buffer is in use
        TCurrentBuffer iCurrentBuffer;

        TMediaId iMediaId;
        
        // Current index in set of audio frames decoded from last payload
        TInt iFrameIndex;        

        // Contains pointers to audio frames in received payload
        RArray<TPtr8> iFrameArray;
        
        // RTP Header
        TRtpRecvHeader iRecvHeader;
        
        // Parameters relating to RTP data encoding
        TMccCodecInfo iCInfo;
        
        // Friend class
        #ifdef TEST_EUNIT
            friend class UT_CG729PayloadFormatRead;
        #endif
    };


#endif /* G729PAYLOADFORMATREAD_H */

// End of File
