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
* Description:    Provides a UL datapath
*
*/




#ifndef MCCULDATAPATH_H
#define MCCULDATAPATH_H

//  INCLUDES
#include "mccdatapathbase.h"

// FORWARD DECLARATIONS
class MMccResources;

// CLASS DECLARATION

/**
*  Mcc UL Datapath declaration
*
*  @lib Mccsubthreads.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccUlDataPath ) : public CMccDataPathBase
    {
    public:  // Constructors and destructor
            
        /**
        * Two-phased constructor.
        */
        static CMccUlDataPath* NewL( MAsyncEventHandler* aEventHandler, 
                                     MMccResources* aMccResources,
                                     TMediaId aMediaId );
        
        /**
        * Destructor.
        */
		virtual ~CMccUlDataPath();

    public: // New functions

        /**
        * Negotiates the source and sink
        * @since Series 60 3.0
        * @param aDataSink Sink to be negotiated with
        * @return void
        */
        // Silencing PC-lint warning due to CMccUlDataPath::NegotiateL
        // hiding MDataSink::NegotiateL.
        /*lint -e1411 */
        void NegotiateL( MDataSink& aDataSink );
        /*lint +e1411 */
        
        /**
        * Loads codec
        */
        void LoadL( MDataSink& aDataSink );

    public: // Functions from baseclasses
        /**
        * From MDataSource
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );

        /**
        * From MDataSink
        */
        void BufferFilledL( CMMFBuffer* aBuffer );
        
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
        CMccUlDataPath( MAsyncEventHandler* aEventHandler, 
                        MMccResources* aMccResources,
                        TMediaId aMediaId  );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // New functions
        
        void ActivateSinkBuffer();
        
    private:    // Friend classes
        #ifdef TEST_EUNIT
        friend class UT_CMccUlDataPath;
        #endif
        
    };

#endif      // MCCULDATAPATH_H
            
// End of File
