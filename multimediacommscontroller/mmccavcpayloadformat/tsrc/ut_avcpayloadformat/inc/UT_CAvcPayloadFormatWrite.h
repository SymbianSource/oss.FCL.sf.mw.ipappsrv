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




#ifndef __UT_CAVCPAYLOADFORMATWRITE_H__
#define __UT_CAVCPAYLOADFORMATWRITE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <mmf/server/mmfdatasink.h>

//  FORWARD DECLARATIONS
class CAvcPayloadFormatWrite;

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
NONSHARABLE_CLASS( UT_CAvcPayloadFormatWrite )
     : public CEUnitTestSuiteClass,
       public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CAvcPayloadFormatWrite* NewL();
        static UT_CAvcPayloadFormatWrite* NewLC();
        /**
         * Destructor
         */
        ~UT_CAvcPayloadFormatWrite();

    private:    // Constructors and destructors

        UT_CAvcPayloadFormatWrite();
        void ConstructL();

    public:     // From observer interface

	public: // From MAsyncEventHandler
    
        TInt SendEventToClient( const TMMFEvent& /*aEvent*/ )
        {
            return KErrNone;
        }

	        

    private:    // New methods

         void SetupL();
         
         void EmptySetupL();
        
         void Teardown();
         
         void Teardown2();
        
         void UT_CAvcPayloadFormatWrite_NewLL();
         
         void UT_CAvcPayloadFormatWrite_NewLErrL();
         
         void UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_0_L();
        
         void UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_1_L();
         
         void UT_CAvcPayloadFormatWrite_ConfigurePayloadFormatL_2_L(TInt mode);
        
         void UT_CAvcPayloadFormatWrite_InitializeLL();
        
        
         void UT_CAvcPayloadFormatWrite_EmptySourceBufferLL();
        
        
         void UT_CAvcPayloadFormatWrite_SourceBufferEmptiedLL();
        
        
         void UT_CAvcPayloadFormatWrite_SetPayloadTypeL();
        
        
         void UT_CAvcPayloadFormatWrite_CancelUlRequestL();
        
        
         void UT_CAvcPayloadFormatWrite_FrameTimeIntervalL();
        
        
         void UT_CAvcPayloadFormatWrite_DurationL();
        
        
         void UT_CAvcPayloadFormatWrite_CreateSinkBufferLL();
         
         void UT_CAvcPayloadFormatWrite_CreateSinkBufferLErrL();
        
        
         void UT_CAvcPayloadFormatWrite_SinkDataTypeCodeTestsL();
        
        
         void UT_CAvcPayloadFormatWrite_SinkThreadLogonL();
        
        
         void UT_CAvcPayloadFormatWrite_SinkThreadLogoffL();
        
        
         void UT_CAvcPayloadFormatWrite_EmptyBufferLL();
        
        
         void UT_CAvcPayloadFormatWrite_BufferEmptiedLL();
        
        
         void UT_CAvcPayloadFormatWrite_SinkPrimeLL();
        
        
         void UT_CAvcPayloadFormatWrite_SinkPlayLL();
        
        
         void UT_CAvcPayloadFormatWrite_SinkPauseLL();
        
        
         void UT_CAvcPayloadFormatWrite_SinkStopLL();
         
         
         void UT_CAvcPayloadFormatWrite_StreamVideoL();
         
         
         void UT_CAvcPayloadFormatWrite_TestPayloadization_0_L();
         
         
         void UT_CAvcPayloadFormatWrite_TestPayloadization_1_L();
         
         
         void UT_CAvcPayloadFormatWrite_TestPayloadization_2_L();
         
         
         void UT_CAvcPayloadFormatWrite_TestPayloadization_3_L();
         
         void UT_CAvcPayloadFormatWrite_TestPayloadization_Of_SpropParameters_L();
        
    private: // Helpers
    
        void DoEmptySourceBufferL();
        
    private:    // Data
    
    		CAvcPayloadFormatWrite * iAvcPayloadFormatter;
    		CMMFBuffer* iSinkBuff;
    		MDataSink *iSink;
    		MDataSource * iSource;
    		HBufC8* iRefData;

        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_CAVCPAYLOADFORMATWRITE_H__

// End of file
