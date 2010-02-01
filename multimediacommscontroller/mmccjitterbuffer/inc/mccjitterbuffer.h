/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Jitter buffer for audio frames.
*  Interface   : 
*
*/




#ifndef MCCJITTERBUFFER_H
#define MCCJITTERBUFFER_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>

// FORWARD DECLARATIONS
class CMccJitterBufferImpl;
class TMccCodecInfo;
class MJitterBufferObserver;

// CLASS DECLARATION

/**
*  Jitter buffer.
*
*  @lib MMccJitterBuffer.lib
*  @since Series 60 3.0
*/
class CMccJitterBuffer : public CBase, public MDataSink, public MDataSource
    {
    private:
        /**
        * Jitterbuffer states
        */
        enum TJitterBufferState 
    	    {
            EJBufStopped,
            EJBufPrimed,
            EJBufPlaying,
            EJBufPaused
            };
                
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMccJitterBuffer* NewL( MJitterBufferObserver* aObserver = NULL );

        /**
        * Destructor.
        */
        virtual ~CMccJitterBuffer();

    public: // Methods

        /**
        * Setup Jitterbuffer
        * @since Series 60 3.0
        * @param aPayloadSize Initial size of the jitterbuffer in milliseconds
        * @param aPlayoutThreshold Initial playout threshold of the jitterbuffer
        * @param aCInfo Codec information class to pass info about used codec
        * @return void
        */    
        IMPORT_C void SetupL( TInt aBufferSize, TInt aPlayoutThreshold, 
                const TMccCodecInfo& aCInfo );

        /**
        * Reset Jitterbuffer
        * @since Series 60 3.0
        * @param aPlayTone ETrue, if tone is to played
        * @return void
        */    
        IMPORT_C void ResetBuffer( TBool aPlayTone = ETrue );

        /**
        * Delay Down
        * @since Series 60 3.0
        * @param none
        * @return void
        */    
    	IMPORT_C void DelayDownL();

        /**
        * Delay Up
        * @since Series 60 3.0
        * @param none
        * @return void
        */    
    	IMPORT_C void DelayUpL();
    	
    	/**
        * Add datasink. This should be the MMF Audio Output MDataSink plug-in 
        * @since Series 60 3.0
        * @param [input] aSink Sink to be added
        * @return void
        */
        IMPORT_C void AddDataSinkL( MDataSink* aSink );

    	/**
        * Add a datasource. This should be the Mcc DL Datapath.
        * @since Series 60 3.0
        * @param [input] aSource Source to be added
        * @return void
        */
        IMPORT_C void AddDataSourceL( MDataSource* aSource );

    	/**
        * Returns the current datasink pointer
        * @since Series 60 3.0
        * @param none
        * @return MDataSink
        */
    	IMPORT_C MDataSink* GetDataSink();

    public: // Functions from base classes    
           
        /**
        * From MDataSink
        */
    	TInt SetSinkDataTypeCode( TFourCC aCodec, TMediaId aMedia );	     
            
        /**
        * From MDataSource
        */
    	TInt SetSourceDataTypeCode( TFourCC aCodec, TMediaId aMedia );        
            
        /**
        * From MDataSink
        */
    	TFourCC SinkDataTypeCode( TMediaId aMediaId ); 
    	
        /**
        * From MDataSource
        */
    	TFourCC SourceDataTypeCode( TMediaId aMediaId ); 
    	
        /**
        * From MDataSink
        */
    	void ConstructSinkL( const TDesC8& aInitData );	
    	
        /**
        * From MDataSource
        */
    	void ConstructSourceL( const TDesC8& aInitData );	
    	
        /**
        * From MDataSink
        */
    	TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );
    	
        /**
         * From MDataSource
        */	
    	TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
    	
        /**
        * From MDataSink
        */	
    	void SinkThreadLogoff();
    	
        /**
        * From MDataSink
        */
    	void SinkPrimeL(); 
    	
        /**
        * From MDataSink
        */                       
    	void SinkPlayL();
    	
        /**
        * From MDataSink
        */
    	void SinkPauseL();	 
    	
        /**
        * From MDataSink
        */
    	void SinkStopL(); 		 

        /**
        * From MDataSink
        */
    	TBool CanCreateSinkBuffer();
    	
        /**
        * From MDataSource
        */
    	TBool CanCreateSourceBuffer();	
    	
        /**
        * From MDataSource
        */
    	CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, TBool &aReference );		
    	
        /**
        * From MDataSink
        */
    	CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, TBool &aReference );
    	
        /**
        * From MDataSource
        */	
        void FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer,
                TMediaId aMediaId );
                          	
        /**
        * From MDataSink
        */	
    	void EmptyBufferL( CMMFBuffer* aBuffer, MDataSource* aSupplier,
                TMediaId aMediaId ); 	                    		
     
        /**
        * From MDataSink
        */
        void BufferFilledL( CMMFBuffer* aBuffer );

        /**
        * From MDataSource
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        /**
        * From MDataSink
        */
        void NegotiateL( MDataSource& aDataSource );

    private:	

        /**
        * C++ default constructor.
        */
        CMccJitterBuffer( MJitterBufferObserver* aObserver );	

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // New functions
    	
    	/**
        * Send jitter buffer event.
        * @return void
        */
    	void SendJitterBufferEventToClient();
        
    private: // Data

        // Datasink
        MDataSink* iDataSink;
        
    	// Datasource
        MDataSource* iDataSource;
        
    	// Jitter buffer Implementation
    	CMccJitterBufferImpl* iBufferImpl;

        // Pointer to databuffer
        CMMFBuffer* iSinkBuffer;

    	// Jitterbuffer state
    	TJitterBufferState iState;

        // ETrue if MMF owns the databuffer
    	TBool iSnkBufRef;
        
        // Asynchronous event handler
        MAsyncEventHandler* iEventHandler;
        
        // Frames played
        TUint iPlayedFrames;
        
        MJitterBufferObserver* iObserver;

        // Current request size of the buffer
        TInt iRequestSize;
        
        // Friend class for EUNIT tests
        #ifdef TEST_EUNIT
            friend class UT_CMccJitterBuffer;
        #endif
    };

#endif  //MCCJITTERBUFFER_H

// End of File
