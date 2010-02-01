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
* Description:    MMF Format Encode interface for redundancy payload plugin.
*
*/





#ifndef MCCREDPAYLOADWRITE_H
#define MCCREDPAYLOADWRITE_H

//  INCLUDES
#include <mmf/server/mmfformat.h>
#include "rtpdatasink.h"
#include "mccredencdecbase.h"
#include "formatstatemachine.h"


// FORWARD DECLARATIONS
class CMccRedEncoder;
class TRtpSendHeader;

// CLASS DECLARATION
/**
*  Defines configuration parameters to be used with configuration method.
*
*  @lib mmccredplformat.dll
*  @since Series 60 3.0
*/
class TMccRedPayloadWriteConfig
    {
    public:
        TMccRedPayloadWriteConfig() :
            iRedPayloadType(0), iRedBlockCount( 0 ), iMaxPayloadSize( 0 ),
            iNumOfEncodings( 0 ), iEncPayloadTypes()
                { }
                
        TMccRedPayloadWriteConfig( TUint8 aRedPayloadType, TUint8 aRedBlockCount,
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
        TFixedArray<TUint8, KMaxNumOfEncodings> iEncPayloadTypes;
    };
    
typedef TPckgBuf<TMccRedPayloadWriteConfig> TMccRedPayloadWritePckg;

/**
*  CMMFFormatEncode interface for redundancy payload plugin.
*
*  @lib mmccredplformat.dll
*  @since Series 60 3.0
*/
class CMccRedPayloadWrite : public CPayloadFormatWrite,
                            public MMccRtpDataSink
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccRedPayloadWrite* NewL( MDataSink* aSink );
        
        /**
        * Destructor.
        */
        virtual ~CMccRedPayloadWrite();

    public: // New functions
        
        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        *
        * @since    Series 60 3.0 
        * @param    aConfigParams       Configuration parameters
        * @param    aClock              RTP media clock
        * @return   void
        */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams,
            CMccRtpMediaClock& aClock );
        
    public: // From CMMFFormatEncode

        /**
        * From CMMFFormatEncode. Return the frame time interval for the given
        * media.
        *
        * @since    Series 60 3.0
        * @param    aMediaType  KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   Time interval in micro seconds
        */
        TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType) const;

        /**
        * NOT SUPPORTED. Return the clip duration for the given media.
        *
        * @since    Series 60 3.0        
        * @param    aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   Clip duration in micro seconds
        */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;          


    public:     // From MDataSink

        /**
        * From MDataSink. Create a sink buffer for the given media
        *
        * @since    Series 60 3.0
        * @param    aMediaId    media type KUidMediaTypeAudio or KUidMediaTypeVideo
        * @param    aReference  False is returned if sink buffer is created
        * @return   Pointer to sink buffer created
        */
        CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId,
                                       TBool &aReference ); 

        /**
        * From MDataSink. Returns the UID identifying the type of data sink.
        *
        * @since    Series 60 3.0
        * @return   The UID identifying the type of data sink
        */
        TUid DataSinkType() const;
        
        /**
        * From MDataSink. Return the sink four CC code for the given media
        *
        * @since    Series 60 3.0
        * @param    aMediaId    media type KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   FourCC code for the mediaId
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );

        /**
        * From MDataSink. Set the sink data type to the given four CC code for
        * the given media.
        *
        * @since    Series 60 3.0
        * @param    aSinkFourCC fourCC code
        * @param    aMediaId    media type KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return   KErrNotSupported if invalid media ID, else KErrNone
        */
        TInt SetSinkDataTypeCode( TFourCC aSinkFourCC,
                                  TMediaId aMediaId ); 

        /**
        * From MDataSink. Log on to the sink thread - this funtion merely
        * passes the command to its sink clip data source object. The sink
        * clip object will handle the thread log on procedures.
        *
        * @since    Series 60 3.0
        * @param    aEventHandler   address of event handler
        * @return   Error code returned by source clip
        */
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * From MDataSink. Log out of the sink thread - this funtion merely
        * passes the command to its sink clip data source object. The sink
        * clip object will handle the thread log off procedures.
        *
        * @since    Series 60 3.0
        * @return   void
        */
        void SinkThreadLogoff();
        
        /**
        * From MDataSink Primes the source.
        *
        * @since    Series 60 3.0
        * @return   void
        */
        void SinkPrimeL();

        /**
        * From MDataSink Plays the source.
        *        
        * @since    Series 60 3.0
        * @return   void
        */
        void SinkPlayL();

        /**
        * From MDataSink Pauses the source.
        *
        * @since    Series 60 3.0
        * @return   void
        */
        void SinkPauseL();

        /**
        * From MDataSink Stops the source.
        *
        * @since    Series 60 3.0
        * @return   void
        */
        void SinkStopL();

        /**
        * From MDataSink. Empty the given source buffer.
        *
        * @param    aBuffer    data buffer to empty
        * @param    aSupplier  data source pointer
        * @param    aMediaId   media type KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return   void
        */
        void EmptyBufferL( CMMFBuffer* aBuffer, MDataSource* aSupplier,
            TMediaId aMediaId );
        

        /**
        * From MDataSink. Called after the data buffer is written.
        * Update the number of bytes written and the current write position for
        * the next write operation. 
        *
        * @since    Series 60 3.0
        * @param    aBuffer       data buffer emptied
        * @return   void
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
    public: // From MMccRtpDataSink
    
        /**
        * Empty source buffer to the sink of redundancy plugin.
        *
        * @since    Series 60 3.0
        * @param    aBuffer     Buffer to empty
        * @param    aSupplier   Data source to notify after buffer is emptied
        * @param    aMediaId    Identifies the media type of particular stream
        * @param    aSendHeader Carries RTP packet header fields
        * @return   void
        */
        void EmptyBufferL( CMMFBuffer* aBuffer,
                           MDataSource* aSupplier, 
                           TMediaId aMediaId,
                           TRtpSendHeader& aSendHeader );

    private:

        /**
        * C++ default constructor.
        */
        CMccRedPayloadWrite();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( MDataSink* aSink );
        
        /**
        * Set payload types to accept as encodings.
        *
        * @since    Series 60 3.0
        * @param    aPayloadTypes       Payload types
        * @return   System wide error code
        */
        TInt SetPayloadTypes( RArray<TUint>& aPayloadTypes );

    private:    // Data
        
        // Redundancy encoder used to construct redundancy RTP format packet
        CMccRedEncoder* iRedEncoder;        // Has
        
        // Data source for redundancy plugin
        MDataSource* iDataSource;           // Uses
        
        // Sink buffer holding constructed redundancy RTP payload
        CMMFDataBuffer* iSinkBuffer;        // Has
        
        // Pointer to data buffer containing source data
        CMMFDataBuffer* iBufToEmptyPtr;     // Uses
        
        // Encapsulates payload format encoding parameters
        TMccRedPayloadWriteConfig iPLConfig;

    private:    // Friend classes
         // Friend class for testing
        #ifdef TEST_EUNIT
            friend class UT_CMccRedPayloadWrite;
        #endif
    };

#endif      // MCCREDPAYLOADWRITE_H   
            
// End of File
