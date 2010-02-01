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
* Description:    Provides DL Datapath for DL subthread
*
*/




#ifndef MCCDLDATAPATH_H
#define MCCDLDATAPATH_H

//  INCLUDES
#include "mccdatapathbase.h"

// FORWARD DECLARATIONS
class MMccResources;

// CLASS DECLARATION

/**
*  Mcc DL Datapath
*
*  @lib Mccsubthreads.dll
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccDlDataPath ) : public CMccDataPathBase
    {
    public:  // Constructors and destructor
            
        /**
        * Two-phased constructor.
        */
        static CMccDlDataPath* NewL( MAsyncEventHandler* aEventHandler, 
                                     MMccResources* aMccResources,
                                     TMediaId aMediaId );
        
        /**
        * Destructor.
        */
        virtual ~CMccDlDataPath();

    public: // New functions

        /**
        * Negotiates the source and sink
        * @since Series 60 3.0
        * @param aDataSource Source to be negotiated with
        * @return void
        */
        void NegotiateL( MDataSource& aDataSource );
        
        /**
        * Loads codec.
        */
        void LoadL( MDataSource& aDataSource );

    public: // Functions from baseclasses
 
        /**
        * From MDataSink
        */
        void BufferFilledL(CMMFBuffer* aBuffer);

        /**
        * From MDataSource
        */
        void BufferEmptiedL(CMMFBuffer* aBuffer);
        
        /**
        * From CMccDataPathBase
        */
        void PrimeL( TUint32 aEndpointId );
        
        /**
        * From CMccDataPathBase
        */
        void PlayL( TUint32 aEndpointId );

        /**
        * From CMccDataPathBase
        */
        void PauseL( TUint32 aEndpointId );
        
        /**
        * From CMccDataPathBase
        */
        void StopL( TUint32 aEndpointId );
        
        /**
        * From CMccDataPathBase
        */
        void ResumeL( TUint32 aEndpointId );
        
        /**
        * From CMccDataPathBase
        */
        void FillSourceBufferL();
        
        /**
        * From CMccDataPathBase
        */
        void EmptySinkBufferL();
    
    private:    // Functions from baseclasses
    
        /**
        * From CMccDataPathBase
        */
        void DoEndOfDataL();
        
    private: // Constructors

        /**
        * C++ default constructor.
        */
        CMccDlDataPath( MAsyncEventHandler* aEventHandler, 
                        MMccResources* aMccResources,
                        TMediaId aMediaId );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private:    // New functions
        
        void ActivateSourceBuffer();
        
    private:    // Friend classes
        #ifdef TEST_EUNIT
        friend class UT_CMccDlDataPath;
        #endif
    };

#endif      // MCCDLDATAPATH_H
            
// End of File
