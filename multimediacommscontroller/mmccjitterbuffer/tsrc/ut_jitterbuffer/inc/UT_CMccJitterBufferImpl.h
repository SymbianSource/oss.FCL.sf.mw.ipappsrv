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




#ifndef __UT_CMccJITTERBUFFERIMPL_H__
#define __UT_CMccJITTERBUFFERIMPL_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/server/sounddevice.h>
#include <mmf/common/mmfcontrollerframework.h>

//  INTERNAL INCLUDES
#include "mccinternalcodecs.h"
#include "mccjitterbuffer.h"
#include "mccjitterbufferimpl.h"

//  FORWARD DECLARATIONS
class TMccEvent;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccJitterBufferImpl : public CEUnitTestSuiteClass,
                                public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccJitterBufferImpl* NewL();
        
        static UT_CMccJitterBufferImpl* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMccJitterBufferImpl();

    private:    // Constructors and destructors

        UT_CMccJitterBufferImpl();
        
        void ConstructL();
		
    public:     // Helpers
    
        void CreateJitterBufferEvent( TMccEvent& aEvent );

    private:    // New methods

        void SetupL();

        void Setup2L();
        
        void Teardown();
        
        
        void UT_CMccJitterBufferImpl_SetupLL();
        
        void UT_CMccJitterBufferImpl_SetupL2L();
        
        void UT_CMccJitterBufferImpl_SetupL3L();
        
        void UT_CMccJitterBufferImpl_SetupL4L();
        
        void UT_CMccJitterBufferImpl_SetupL5L();
        
        void UT_CMccJitterBufferImpl_SetupL6L();
        
        void UT_CMccJitterBufferImpl_ResetBufferL();
        
        void UT_CMccJitterBufferImpl_CurrentCodecL();
        
        void UT_CMccJitterBufferImpl_DelayUpLL();
        
        void UT_CMccJitterBufferImpl_DelayDownLL();
        
        void UT_CMccJitterBufferImpl_CalculateDelayL();
        
        void UT_CMccJitterBufferImpl_AddDataFrameLL();
        
        void UT_CMccJitterBufferImpl_GetDataFrameLL();
        
        void UT_CMccJitterBufferImpl_BufferLengthL();
        
        void UT_CMccJitterBufferImpl_GenerateStatisticsL();
    

// Virtual methods

        virtual TInt SendEventToClient(const TMMFEvent& /*aEvent*/)
            {
            return KErrNone;
            };         

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE;
		
	    CMccJitterBufferImpl* iJBufImpl;
        TMccCodecInfo iCInfo;
        CMMFDevSound* iDevSoundDummy;
    };

#endif      //  __UT_CMccJITTERBUFFERIMPL_H__

// End of file
