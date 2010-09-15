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




#ifndef __UT_CMCCFILESINK_H__
#define __UT_CMCCFILESINK_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccFileSink;
class CMMFDataBuffer;
class CMccDlDataPath;
class CMccTestEventHandler;
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
NONSHARABLE_CLASS( UT_CMccFileSink )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccFileSink* NewL();
        static UT_CMccFileSink* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccFileSink();

    private:    // Constructors and destructors

        UT_CMccFileSink();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccFileSink_NewSinkLL();
        
        
         void UT_CMccFileSink_ConstructSinkLL();
        
        
         void UT_CMccFileSink_CMccFileSinkL();
        
        
         void UT_CMccFileSink_SetFileNameLL();
        
        
         void UT_CMccFileSink_RecordTimeAvailableLL();
        
        
         void UT_CMccFileSink_SinkDataTypeCodeL();
        
        
         void UT_CMccFileSink_SetSinkDataTypeCodeL();
        
        
         void UT_CMccFileSink_BufferEmptiedLL();
        
        
         void UT_CMccFileSink_CanCreateSinkBufferL();
        
        
         void UT_CMccFileSink_CreateSinkBufferLL();
        
        
         void UT_CMccFileSink_SinkThreadLogonL();
        
        
         void UT_CMccFileSink_SinkThreadLogoffL();
        
        
         void UT_CMccFileSink_SinkPrimeLL();
        
        
         void UT_CMccFileSink_SinkPlayLL();
        
        
         void UT_CMccFileSink_SinkPauseLL();
        
        
         void UT_CMccFileSink_SinkStopLL();
        
        
         void UT_CMccFileSink_EmptyBufferLL();
         
         void UT_CMccFileSink_EmptyBufferL2L();
        
         void UT_CMccFileSink_BufferFilledLL();
                
        
         void UT_CMccFileSink_MfcoDiskFullLL();
        
        
         void UT_CMccFileSink_MfcoSizeLimitReachedLL();
        
         void UT_CMccFileSink_TimeToPlayL();
         
         void UT_CMccFileSink_SetPausedDurationL();
         
         void UT_CMccFileSink_UpdateActiveUserL();
         
         void UT_CMccFileSink_CalculateAverageTimestampDifferenceL();
         
    private: // Helpers
    
         CMccTestEventHandler* AddUserL( TBool aIsAvc = EFalse );
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccFileSink* iFileSink;
		CMMFDataBuffer* iBuffer;
		CMccDlDataPath* iDLDataPathStub;
		CMccTestEventHandler* iEventHandler;
		CMccTestEventHandler* iEventHandler2;
		CMccResourcePoolStub* iResourceStub;
    };

#endif      //  __UT_CMCCFILESINK_H__

// End of file
