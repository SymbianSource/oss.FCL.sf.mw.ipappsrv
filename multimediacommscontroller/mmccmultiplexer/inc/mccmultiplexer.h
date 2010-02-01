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
*  Interface   : 
*
*/




#ifndef MCCMULTIPLEXER_H
#define MCCMULTIPLEXER_H

// INCLUDES
#include <mmf/common/mmfcontrollerframework.h>
#include "formatstatemachine.h"
#include "mmccevents.h"

// CLASS DECLARATION
class CMMFBuffer;
class CMccSinkItem;
class CMMFDataBuffer;


/**
*
*/ 
class CMccMultiplexer : public CPayloadFormatRead, public MAsyncEventHandler
    {
    public:
    
        enum TMccMultiplexerState
            {
            ENotSet,
            EPrepared,
            EPlaying,
            EResourcePaused,
            EResourceStopped
            };
            
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aSource Source of the data for the multiplexer
        */
        IMPORT_C static CMccMultiplexer* NewL( MDataSource* aSource );

        /**
        * Destructor.
        */
        IMPORT_C ~CMccMultiplexer();

    public:  // New functions

        IMPORT_C TInt MultiplexerSourceThreadLogon( MAsyncEventHandler& aEventHandler, 
                                         MDataSink& aSink, 
                                         TBool aSinkIsDecoder,
                                         TUid aMediaType,
                                         TBool aPassAllBuffersSink = EFalse );
        
        IMPORT_C TInt MultiplexerSourceThreadLogoff( MDataSink* aSink );
        
        IMPORT_C void SetCurrentUserL( MDataSink& aSink );
        
        IMPORT_C void RegClientForPayloadsL( MDataSink& aClient, 
            const RArray<TUint>& aPayloadTypes );
        
    public: // From CMMFFormatDecode

        /**
        * From CMMFFormatDecode. Return number of audio streams for the given
        * media
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

        /**
        * Return the clip duration for the given media
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           Clip duration in micro seconds
        */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;
        
        TBool CanCreateSourceBuffer();

        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, TBool &aReference );
        
        void SuggestSourceBufferSize( TUint aSuggestedBufferSize );
        
    public: // From MDataSource.

        TUid DataSourceType() const;
        
        /**
        * From MDataSource. Read the next block of data into the
        * given buffer.
        *
        * @param aBuffer    source data buffer for the data read
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
        void SourcePrimeL();

        /**
        * From MDataSource. Plays the source.
        * @since Series 60 3.0
        * @return None
        */
        void SourcePlayL();
        
        /**
        * From MDataSource. Pauses the source.
        * @since Series 60 3.0
        * @return None
        */
        void SourcePauseL();
        
        /**
        * From MDataSource. Stops the source.
        * @since Series 60 3.0
        * @return None
        */
        void SourceStopL();

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
        void SourceThreadLogoff();

        /**
        * Negotiate source settings ( buffer size ) to match 
        * given source object
        *
        * @param aDataSink          address of sink object
        *
        * @return None
        */
        void NegotiateSourceL( MDataSink& aDataSink );


    public: // From CPayloadFormatRead

        /**
        * Called after the data buffer is filled. Update the number of bytes read
        * and the current read position for the next read operation. 
        *
        * @param    aBuffer         data buffer filled
        * @param    aHeaderInfo     RTP packet header information
        * @return None
        */
        void DataBufferFilledL( CMMFBuffer* aBuffer, 
                            const TRtpRecvHeader& aHeaderInfo );
        
        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        * @param    aConfigParams       Configuration parameters
        * @return   void
        */                    
        void ConfigurePayloadFormatL( const TDesC8& aConfigParams );
        
    public: // MDataSink
    
        void BufferFilledL( CMMFBuffer* aBuffer );
    
    public: // From MAsyncEventHandler
    
        TInt SendEventToClient( const TMMFEvent& aEvent );
    
    private:
        /**
        * C++ default constructor.
        */
        CMccMultiplexer();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aSource    Source of the data for the payload format plugin
        */
        void ConstructL( MDataSource* aSource );
        
    private:
    
        CMccSinkItem* FindSinkItemL( MDataSink* aSink );
    
        TInt FindSinkItemIndex( MDataSink* aSink );
        
        TBool CanCommandSource( MDataSink* aSink );
        
        TBool IsPrimarySink( MDataSink* aSink );
        
        void DoBufferFilledL( CMMFBuffer* aBuffer, 
                              const TRtpRecvHeader* aHeaderInfo );
                                      
        void AddSinkL( MDataSink* aSink, 
                       TBool aSinkIsDecoder, 
                       TUid aMediaType,
                       TBool aPassAllBuffersSink );
        
        TBool SetMultiplexerState( TMccMultiplexerState aState );
        
        void SendStreamEventToClient( TMccEventType& aEventType );
        
        void UpdateSinkModes();
        
        TBool PassAllRequestsToSource( CMccSinkItem& aItem ) const;
        
        TBool IsSupportedMediaType( TUid aMediaType ) const;
        
        void SendEventToAllClients( const TMMFEvent& aEvent );
        
        void HandleCleanup( CMccSinkItem* aSinkItem, 
                            TBool aPassedAllRequests );
    
        void HandleCleanupL( CMccSinkItem* aSinkItem, 
                             TBool aPassedAllRequests );
                             
        TBool IsActiveState( TMccMultiplexerState aState ) const;

    private: // data
        
        MAsyncEventHandler* iEventHandler;
        
        MDataSource* iSource;
        
        // First item is always the primary sink
        RPointerArray<CMccSinkItem> iSinkItems;
        
        TBool iSourceLogonDone;
        TBool iSourceNegotiationDone;
        
        TMccMultiplexerState iState;
        
        CMccSinkItem* iCurrentUser;
        
        TBool iPassAllRequestsToClient;
        
        TBool iResolveFilledBufferMediaType;

        CMMFDataBuffer* iDataBuf;
        
        // Client suggestion for needed source buffer size
        TInt iSuggestedBufSize;

        TMccEvent iStreamControlEvent;
        
   private:    // Friend classes
    
   #ifdef EUNIT_TEST
        friend class UT_CMccMultiplexer;
   #endif
    };

#endif  //MCCMULTIPLEXER_H

// End of File
