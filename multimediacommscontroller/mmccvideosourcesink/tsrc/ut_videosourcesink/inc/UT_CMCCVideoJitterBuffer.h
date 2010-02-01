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
*
*/




#ifndef __UT_CMccVideoJitterBuffer_H__
#define __UT_CMccVideoJitterBuffer_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mccvideojitterbuffer.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CXPSPacketSink;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMccVideoJitterBuffer )
     : public CEUnitTestSuiteClass, public MMccVideoJitterBufferObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccVideoJitterBuffer* NewL();
        static UT_CMccVideoJitterBuffer* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccVideoJitterBuffer();

    public: // From MMccVideoJitterBufferObserver
    
        void ErrorOccured( TInt aError );
        
    private:    // Constructors and destructors

        UT_CMccVideoJitterBuffer();
        void ConstructL();

    private: // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();
        
    private: // Tests
    
        void UT_CMccVideoJitterBuffer_EnqueueLL();
        
        void UT_CMccVideoJitterBuffer_ConfigureLL();
        
        void UT_CMccVideoJitterBuffer_PausePlayL();
        
        void UT_CMccVideoJitterBuffer_InactivityTimeoutL();
        
    private: // Data
    
        CMccVideoJitterBuffer* iJitBuf;
        CXPSPacketSink* iPacketSink;

        EUNIT_DECLARE_TEST_TABLE; 
    };

#endif      //  __UT_CMccVideoJitterBuffer_H__

// End of file
