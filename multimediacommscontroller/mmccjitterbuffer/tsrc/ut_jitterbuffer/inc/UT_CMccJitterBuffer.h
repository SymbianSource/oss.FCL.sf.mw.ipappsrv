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




#ifndef __UT_CMccJITTERBUFFER_H__
#define __UT_CMccJITTERBUFFER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfutilities.h>

//  INTERNAL INCLUDES
#include <mmf/common/mmfcontrollerframework.h>
#include "mccinternalcodecs.h"

//  FORWARD DECLARATIONS
class CMccJitterBuffer;
class CMccDataSinkStub;
class CMccDataSourceStub;
class TMccCodecInfo;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccJitterBuffer : public CEUnitTestSuiteClass, 
                            public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccJitterBuffer* NewL();
        
        static UT_CMccJitterBuffer* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMccJitterBuffer();
        

    private:    // Constructors and destructors

        UT_CMccJitterBuffer();
        
        void ConstructL();
        

    private:    // New methods

        void SetupL();
         
         void Setup2L();
         
         void SetupAL();
         
         void SetupA2L();
        
         void Teardown();
        
         
         void UT_CMccJitterBuffer_SetupLL();
        
        
         void UT_CMccJitterBuffer_ResetBufferL();
        
        
         void UT_CMccJitterBuffer_DelayDownLL();
        
        
         void UT_CMccJitterBuffer_DelayUpLL();
        
        
         void UT_CMccJitterBuffer_SetSinkDataTypeCodeL();
        
        
         void UT_CMccJitterBuffer_SetSourceDataTypeCodeL();
        
        
         void UT_CMccJitterBuffer_SinkDataTypeCodeL();
        
        
         void UT_CMccJitterBuffer_SourceDataTypeCodeL();
        
        
         void UT_CMccJitterBuffer_ConstructSinkLL();
        
        
         void UT_CMccJitterBuffer_ConstructSourceLL();
        
        
         void UT_CMccJitterBuffer_SinkThreadLogonL();
        
        
         void UT_CMccJitterBuffer_SourceThreadLogonL();
        
        
         void UT_CMccJitterBuffer_SinkThreadLogoffL();
        
        
         void UT_CMccJitterBuffer_SinkPrimeLL();
        
        
         void UT_CMccJitterBuffer_SinkPlayLL();
        
        
         void UT_CMccJitterBuffer_SinkPauseLL();
        
        
         void UT_CMccJitterBuffer_SinkStopLL();
        
        
         void UT_CMccJitterBuffer_CanCreateSinkBufferL();
        
        
         void UT_CMccJitterBuffer_CanCreateSourceBufferL();
         
        
         void UT_CMccJitterBuffer_CreateSinkBufferLL();


         void UT_CMccJitterBuffer_CreateSourceBufferLL();
        
        
         void UT_CMccJitterBuffer_FillBufferLL();


         void UT_CMccJitterBuffer_EmptyBufferLL();


         void UT_CMccJitterBuffer_EmptyBuffer2LL();


         void UT_CMccJitterBuffer_EmptyBuffer3LL();


         void UT_CMccJitterBuffer_EmptyBuffer4LL();

         
         void UT_CMccJitterBuffer_BufferFilledLL();
        
        
         void UT_CMccJitterBuffer_BufferEmptiedLL();
         
         
         void UT_CMccJitterBuffer_AddDataSinkLL();


         void UT_CMccJitterBuffer_AddDataSourceLL();
        

         void UT_CMccJitterBuffer_SendJitterBufferEventToClientL();
         
        
         void UT_CMccJitterBuffer_GetDataSinkL();
        
         
         void UT_CMccJitterBuffer_NegotiateLL();

// Virtual methods
    
        virtual TInt SendEventToClient(const TMMFEvent& /*aEvent*/)
            {
            return KErrNone;
            };

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
		
        CMccJitterBuffer* iJitter;
		TMccCodecInfo iCInfo;
		TBool iAlloc;
		CMccDataSourceStub* iDataSource;
		CMccDataSinkStub* iDataSink;
		CMMFDescriptorBuffer* iBuffer;
		CMMFDescriptorBuffer* iBuffer2;
		TFourCC i4ccjit;
    };

#endif      //  __UT_CMccJITTERBUFFER_H__

// End of file
