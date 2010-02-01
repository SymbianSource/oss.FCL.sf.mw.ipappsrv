/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    MMF Format Decode interface for redundancy payload plugin.
*
*/





#ifndef MCCREDPAYLOADREAD_H
#define MCCREDPAYLOADREAD_H

//  INCLUDES
#include <mmf/server/mmfformat.h>
#include "mccredencdecbase.h"
#include "formatstatemachine.h"

// FORWARD DECLARATIONS
class CMccRedDecoder;

// CLASS DECLARATION
/**
*  Defines configuration parameters to be used with configuration method.
*
*  @lib mmccredplformat.dll
*  @since Series 60 3.0
*/
class TMccRedPayloadReadConfig
    {
    public:
        TMccRedPayloadReadConfig() :
            iRedPayloadType(0), iRedBlockCount( 0 ), iMaxPayloadSize( 0 ),
            iNumOfEncodings( 0 ), iEncPayloadTypes()
                { }

        TMccRedPayloadReadConfig( TUint8 aRedPayloadType, TUint8 aRedBlockCount,
            TUint8 aMaxPayloadSize, TUint8 aNumOfEncodings ) :
            iRedPayloadType( aRedPayloadType ), iRedBlockCount( aRedBlockCount ),
            iMaxPayloadSize( aMaxPayloadSize ), iNumOfEncodings( aNumOfEncodings ),
            iEncPayloadTypes()
                { }                    

        void InitPayloadTypes()
            {
            TInt ind( iEncPayloadTypes.Count() );
            while ( --ind )
                {
                iEncPayloadTypes[ind] = KPayloadNotDefined;
                }
            }
            
        // RTP header payload type indicating redundancy format
        TUint8 iRedPayloadType;
        
        // Number of redundant blocks included in one RTP packet
        TUint8 iRedBlockCount;
        
        // Size of maximum packetization interval of codec data
        TUint iMaxPayloadSize;
        
        // Number of encodings combined in RTP packet
        TUint8 iNumOfEncodings;
        
        // Payload types for encodings to use
        TFixedArray<TUint8, KPayloadNotDefined> iEncPayloadTypes;
    };
    
typedef TPckgBuf<TMccRedPayloadReadConfig> TMccRedPayloadReadPckg;

/**
*  Depacketizes RTP payload in redundancy format.
*
*  @lib mmccredplformat.dll
*  @since Series 60 3.0
*/
class CMccRedPayloadRead : public CPayloadFormatRead
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccRedPayloadRead* NewL( MDataSource* aSource );
        
        /**
        * Destructor.
        */
        virtual ~CMccRedPayloadRead();

    public: // From CPayloadFormatRead
        
        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        *
        * @since    Series 60 3.0 
        * @param    aConfigParams       Configuration parameters
        * @return   void
        */
        void ConfigurePayloadFormatL( const TDesC8& aConfigParams );

        /**
        * Called after the data buffer is filled. Update the number of bytes
        * read and the current read position for the next read operation. 
        *
        * @since    Series 60 3.0
        * @param    aBuffer         data buffer filled
        * @param    aRecvHeader     RTP header
        * @return   void
        */
        void DataBufferFilledL( CMMFBuffer* aBuffer, const TRtpRecvHeader& aRecvHeader );
    
    public: // From CMMFFormatDecode

        /**
        * From CMMFFormatDecode. Return number of audio streams for the given
        * media. Raw audio  files can only have 1 audio stream.
        *
        * @since    Series 60 3.0
        * @param    aMediaType  KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   Number of audio streams
        */
        TUint Streams( TUid aMediaType ) const;
        
        /**
        * From CMMFFormatDecode. Return the frame time interval for the given
        * media.
        *
        * @since    Series 60 3.0
        * @param    aMediaType  KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   Time interval in micro seconds
        */
        TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType) const;
        
        /**
        * From CMMFFormatDeoce. NOT SUPPORTED. Return the clip duration for 
        * the given media.
        *
        * @since    Series 60 3.0
        * @param    aMediaType  KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   Clip duration in micro seconds
        */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;
        
    public: // From MDataSource

        /**
        * From MDataSource. Return the four CC code for the given media.
        *
        * @since    Series 60 3.0
        * @param    aMediaId    KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   FourCC code
        */
        TFourCC SourceDataTypeCode( TMediaId aMediaId );
        
        /**
        * From MDataSource. Set the source data type to the given four CC code
        * for the given media.
        *
        * @since    Series 60 3.0
        * @param    aSourceFourCC    fourCC code
        * @param    aMediaId         contains the media type KUidMediaTypeAudio
        *                            or KUidMediaTypeVideo
        *
        * @return   KErrNotSupported if invalid media ID, else return KErrvoid
        */
        TInt SetSourceDataTypeCode( TFourCC aSourceFourCC,
                                    TMediaId aMediaId );

        /**
        * From MDataSource. Tests whether a source buffer can be created.
        *
        * @since    Series 60 3.0
        * @return   ETrue if a source buffer can be created.
        */
        TBool CanCreateSourceBuffer();
        
        /**
        * From MDataSource. Create a source buffer for the given media
        *
        * @since    Series 60 3.0
        * @param    aMediaId   contains the media type KUidMediaTypeAudio or 
        *                      KUidMediaTypeVideo
        * @param    aReference value of False is returned to caller if source 
        *                      buffer is created
        *
        * @return   Pointer to source buffer created
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, 
                                         TBool &aReference );

        /**
        * From MDataSource. Read the next block of data from file into the
        * given buffer.
        *
        * @since    Series 60 3.0        
        * @param    aBuffer    source data buffer for the AMR-NB data read
        * @param    aConsumer  data sink pointer
        * @param    aMediaId   contains the media type KUidMediaTypeAudio or 
        *                      KUidMediaTypeVideo
        *
        * @return   void
        */
        void FillBufferL( CMMFBuffer* aBuffer, 
                          MDataSink* aConsumer, 
                          TMediaId aMediaId );
                                 
        /**
        * From MDataSource. Log in to the source thread - this funtion merely
        * passes the command to its source clip data source object. The source
        * clip object will handle the thread log on procedures.
        *
        * @since    Series 60 3.0        
        * @param    aEventHandler    address of event handler
        *
        * @return   error code returned by source clip
        */
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * From MDataSource. Log out of the source thread - this funtion merely
        * passes the command to its source clip data source object. The source
        * clip object will handle the thread log off procedures.
        *
        * @since    Series 60 3.0
        * @return   void
        */
        void SourceThreadLogoff();  
        
        /**
        * From MDataSource. Primes the source.
        *
        * @since    Series 60 3.0
        * @return   void
        */    
        void SourcePrimeL();

        /**
        * From MDataSource. Plays the source.
        *
        * @since    Series 60 3.0
        * @return   void
        */    
        void SourcePlayL();

        /**
        * From MDataSource. Pauses the source.
        *
        * @since    Series 60 3.0
         * @return   void
        */
        void SourcePauseL();

        /**
        * From MDataSource. Stops the source.
        *
        * @since    Series 60 3.0
        * @return   void
        */    
        void SourceStopL();
        
        /**
        * From MDataSource. Negotiates source.
        *
        * @since    Series 60 3.0
        * @param    aDataSink       Data sink interested to receive data
        * @return   void
        */    
        void NegotiateSourceL( MDataSink& aDataSink );
        
        /**
        * From MDataSource. Used by the sink to suggest a source buffer size.
        *
        * @since    Series 60 3.0
        * @param    aSuggestedBufferSize A recommended buffer size
        * @return   void
        */
        void SuggestSourceBufferSize( TUint aSuggestedBufferSize );
    
    public: // From MDataSink

        /**
        * From MDataSink. Gets the data type code for the sink specified by the
        * media ID. Format decode may act in role of MDataSink.
        *
        * @since    Series 60 3.0
        * @param    aMediaId       Media ID
        * @return   The data sink type code
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );

    private:

        /**
        * C++ default constructor.
        */
        CMccRedPayloadRead();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( MDataSource* aSource );
        
        /**
        * Set payload types to accept as encodings.
        *
        * @since    Series 60 3.0
        * @param    aPayloadTypes       Payload types
        * @return   System wide error code
        */
        TInt SetPayloadTypes( RArray<TUint>& aPayloadTypes );

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
        
    private:    // Data
        
        // Decoder for redundancy payload
        CMccRedDecoder* iRedDecoder;
        
        // Data sink for redundancy plugin
        MDataSink* iDataSink;
        
        // The fourCC indicating data type used as redundant data
        TFourCC iFourCC;        

        // Buffer holding redundancy data got from far end
        CMMFDataBuffer* iSourceBuffer;
        
        // Indicates whether plugin owns it's source buffer
        TBool iSourceBufOwnership;
        
        // Pointer to the buffer passed with FillBufferL
        CMMFDataBuffer* iSinkBuffer;        // Uses
        
        // Encapsulates payload decoding parameters
        TMccRedPayloadReadConfig iPLConfig;
        
        // Suggeste source buffer size
        TUint iSuggestedBufSize;
        
    private:    // Friend classes

        // Friend class for testing
        #ifdef TEST_EUNIT
            friend class UT_CMccRedPayloadRead;
        #endif

    };

#endif      // MCCREDPAYLOADREAD_H   
            
// End of File
