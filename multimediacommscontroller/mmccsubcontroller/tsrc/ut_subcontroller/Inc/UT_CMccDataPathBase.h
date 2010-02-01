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




#ifndef __UT_CMccDATAPATHBASE_H__
#define __UT_CMccDATAPATHBASE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccDataPathBase;
class CMccUlDataPath;
class MDataSource;
class MDataSink;
class CMccResourcePoolStub;

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
NONSHARABLE_CLASS( UT_CMccDataPathBase )
     : public CEUnitTestSuiteClass,
       public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccDataPathBase* NewL();
        static UT_CMccDataPathBase* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccDataPathBase();

    private:    // Constructors and destructors

        UT_CMccDataPathBase();
        void ConstructL();

    public:     // From observer interface

        TInt SendEventToClient(const TMMFEvent& /*aEvent*/)
	        {
	        return KErrNone;	
	        }

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccDataPathBase_AddDataSinkLL();
        
         void UT_CMccDataPathBase_ChangeDataPathTransferState();
         
         void UT_CMccDataPathBase_ConstructSinkSource();
         
         void UT_CMccDataPathBase_IsBufferSupportedL();
         
         void UT_CMccDataPathBase_CreateCompleteCallBackL();
         
         void UT_CMccDataPathBase_CopyBuffer();
         
         void UT_CMccDataPathBase_AddDataSourceLL();
        
         void UT_CMccDataPathBase_EndOfDataL();
        
        
         void UT_CMccDataPathBase_PrimeLL();
        
        
         void UT_CMccDataPathBase_PlayLL();
        
        
         void UT_CMccDataPathBase_PauseLL();
        
        
         void UT_CMccDataPathBase_StopLL();
        
        
         void UT_CMccDataPathBase_ResumeLL();
        
        
         void UT_CMccDataPathBase_CreateSinkBufferLL();
        
        
         void UT_CMccDataPathBase_CreateSourceBufferLL();
        
        
         void UT_CMccDataPathBase_CanCreateSinkBufferL();
        
        
         void UT_CMccDataPathBase_CanCreateSourceBufferL();
        
        
         void UT_CMccDataPathBase_EmptyBufferLL();
        
        
         void UT_CMccDataPathBase_FillBufferLL();
        
        
         void UT_CMccDataPathBase_SinkDataTypeCodeL();
        
        
         void UT_CMccDataPathBase_SourceDataTypeCodeL();
        
        
         void UT_CMccDataPathBase_SetSinkDataTypeCodeL();
        
        
         void UT_CMccDataPathBase_SetSourceDataTypeCodeL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccUlDataPath* iUlPath;
        MDataSource* iSourceStub;
        MDataSink*   iSinkStub;
        
        CMccResourcePoolStub* iResources;
    };

#endif      //  __UT_CMccDATAPATHBASE_H__

// End of file
