/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a base class for datapaths in Mcc
*
*/




#ifndef MCCDATAPATHBASE_H
#define MCCDATAPATHBASE_H

// INCLUDES
#include <e32std.h>
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfdatasource.h>
#include "mccdatapathdef.h"


// FORWARD DECLARATIONS
class CCompleteCallback;
class MMccResources;

// CLASS DECLARATION

/**
*  Mcc Datapath base class definition
*
*  @lib Mccsubthreads.dll
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccDataPathBase ) : public CActive, public MDataSink,
        public MDataSource
    {
    public:  // Constructors and destructor
        
        /*
        * Destructor
        */
        virtual ~CMccDataPathBase();           

    public: // New functions

        /**
        * Add datasink to datapath
        * @since Series 60 3.0
        * @param [input] aSink Sink to be added
        * @return void
        */
        void AddDataSinkL( MDataSink* aSink );

        /**
        * Add a datasource to datapath
        * @since Series 60 3.0
        * @param [input] aSource Source to be added
        * @return void
        */
        void AddDataSourceL( MDataSource* aSource );
        
        /**
        * Set the associated data sink
        * @since Series 60 3.0
        * @param [input] aAssociatedSink
        * @return void
        */
        void SetAssociatedSink( MDataSink* aAssociatedSink );
        
        /**
        * Set the associated data source
        * @since Series 60 3.0
        * @param [input] aAssociatedSource
        * @return void
        */
        void SetAssociatedSource( MDataSource* aAssociatedSource );
        
        /**
        * Handles the end of data
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        void EndOfData();
        
        /**
        * Primes the datapath
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void PrimeL( TUint32 aEndpointId ) = 0;
        
        /**
        * Starts the audio play/record
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void PlayL( TUint32 aEndpointId ) = 0;
        
        /**
        * Pauses the audio play/record
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void PauseL( TUint32 aEndpointId ) = 0;
        
        /**
        * Stops the audio play/record
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void StopL( TUint32 aEndpointId ) = 0;
        
        /**
        * Resumes the audio play/record from paused state
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void ResumeL( TUint32 aEndpointId ) = 0;

    public: // Functions from base classes

        /**
        * From MDataSink
        */
        virtual CMMFBuffer* CreateSinkBufferL(TMediaId aMediaId, TBool& aReference);

        /**
        * From MDataSource
        */
        virtual CMMFBuffer* CreateSourceBufferL(TMediaId aMediaId, TBool& aReference);

        /**
        * From MDataSink
        */
        virtual TBool CanCreateSinkBuffer();

        /**
        * From MDataSource
        */
        virtual TBool CanCreateSourceBuffer();

        /**
        * From MDataSink
        */
        virtual void EmptyBufferL(CMMFBuffer* aBuffer, MDataSource* aSupplier, TMediaId aMediaId);

        /**
        * From MDataSource
        */
        virtual void FillBufferL(CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId);

        /**
        * From MDataSink
        */
        TFourCC SinkDataTypeCode(TMediaId aMediaId);

        /**
        * From MDataSource
        */
        TFourCC SourceDataTypeCode(TMediaId aMediaId);

        /**
        * From MDataSink
        */
        virtual TInt SetSinkDataTypeCode(TFourCC aSourceFourCC, TMediaId aMediaId);

        /**
        * From MDataSource
        */
        virtual TInt SetSourceDataTypeCode(TFourCC aSourceFourCC, TMediaId aMediaId);

    protected:  // New functions
        
        /**
        * Changes the current datapath transferstate.
        * @since Series 60 3.0
        * @param [input] aTransferState New transfer state
        * @param aFailureFallback, ETrue if state change is done because of some failure
        *        from which can be recovered
        * @return void
        */
        TBool ChangeDataPathTransferState( TTransferState aTransferState, 
                                           TBool aFailureFallback = EFalse );

        /**
        * Returns the current transferstate
        * @since Series 60 3.0
        * @param None
        * @return TTransferState
        */
        TTransferState TransferState() const;

        /**
        * Sets the new datapath state
        * @since Series 60 3.0
        * @param [input] aState New datapath state
        * @return void
        */
        void SetStateL( const TDataPathState& aState );

        /**
        * Checks if the given buffer is supported.
        * Leaves with KErrNotSupported
        * @since Series 60 3.0
        * @param aBuffer Buffer to be checked
        * @return void
        */
        void IsBufferSupportedL( CMMFBuffer* aBuffer );
        
        /**
        * copy CMMFBuffer data and setting from one buffer
        * to another
        * @since Series 60 3.0
        * @param aDesBuffer, destination buffer
        * @param aOrigBuffer, original buffer
        * @return void
        */
        void CopyBuffer( CMMFBuffer* aDesBuffer,
                         CMMFBuffer* aOrigBuffer);
        
        /**
        * a CCompleteCallback object is created
        * @since Series 60 3.0
        * @return void
        */
        void CreateCompleteCallBackL();
        
        /**
        * a signal the datapath complete with a error code
        * @since Series 60 3.0
        * @param aErrorCode, a datapath complete error code.
        * @return void
        */
        void SignalDataPathCompleteL( TInt aErrorCode );
        
        /**
        * Worker function for dataending
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void DoEndOfDataL() = 0;
        
        /**
        * Returns datapath's current state
        * @since Series 60 3.0
        * @param None
        * @return TDataPathState
        */
        TDataPathState State() const;
        
        virtual TBool ControlSource( TUint32 aEndpointId ) const;
        
        virtual TBool ControlSink( TUint32 aEndpointId ) const;
        
        TBool IsMmfEndpoint( MDataSink* aSink ) const;
        
        TBool IsMmfEndpoint( MDataSource* aSource ) const;
        
        /**
        * Fills the source buffer
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void FillSourceBufferL() = 0;
        
        /**
        * Empty the sink buffer
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        virtual void EmptySinkBufferL() = 0;
        
    protected:  // Functions from base classes
        
        /**
        * From MDataSink
        */
        virtual void ConstructSinkL( const TDesC8& aInitData );

        /**
        * From MDataSource
        */
        virtual void ConstructSourceL( const TDesC8& aInitData );

        /**
        * From CActive
        */
        virtual void DoCancel();

        /**
        * From CActive
        */
        void RunL();
        
        /**
        * From CActive
        */
        TInt RunError( TInt aError );

    protected: // Constructors

        /*
        * Constructor
        */
        inline CMccDataPathBase( MAsyncEventHandler* aEventHandler, 
                                 MMccResources* aMccResources,
                                 TMediaId aMediaId ) : 
                CActive( EPriorityStandard ), MDataSink( KUidMmfDataPath ),
                MDataSource( KUidMmfDataPath ), iDataSink( NULL ), 
                iDataSource( NULL ), iSinkBuffer( NULL ), iSourceBuffer( NULL ),
                iMediaId( aMediaId ), iEventHandler( aEventHandler ),
                iMccResources( aMccResources ),
                iCompleteCallback( NULL ), iState( EStopped ) 
                { CActiveScheduler::Add(this); };

    private: // New functions
        
        /**
        * Creates the datapath
        * @since Series 60 3.0
        * @param [input] aSource Datapaths datasource
        * @param [input] aSink Datapaths datasink
        * @return void
        */
        void CreateDataPathL( MDataSource* aSource, MDataSink* aSink );

    protected:  // Data
        
        // Datasink
        MDataSink* iDataSink;

        // Datasource
        MDataSource* iDataSource;

        // Pointers to used buffers
        CMMFBuffer* iSinkBuffer;
        CMMFBuffer* iSourceBuffer;
        
        // Datapath's media id. Should be audio
        TMediaId iMediaId;
        
        // Datapaths completion error code
        TInt iDataPathCompletedErrorCode;

        // Sink & Source added, CreateDataPath() called
        TBool iDataPathCreated;
        
        // Eventhandler
        MAsyncEventHandler* iEventHandler;
        
        MMccResources* iMccResources;
        
        CCompleteCallback* iCompleteCallback;
        
        MDataSink* iAssociatedSink;
        
        MDataSource* iAssociatedSource;

    private:    // Data

        // Current data transfer state
        TTransferState iTransferState;

        // Datasink FourCC code
        TFourCC iSinkFourCC;

        // Datasource FourCC code
        TFourCC iSourceFourCC;

        // Current datapath state
        TDataPathState iState;

    private: //friend class
        #ifdef TEST_EUNIT
        friend class UT_CMccDlDataPath;
        friend class UT_CMccUlDataPath;
        friend class UT_CMccDataPathBase;

        #endif
    };

#endif      // MCCDATAPATHBASE_H
            
// End of File
