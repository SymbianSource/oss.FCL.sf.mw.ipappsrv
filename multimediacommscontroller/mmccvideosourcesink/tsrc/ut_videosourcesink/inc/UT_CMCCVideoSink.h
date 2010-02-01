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




#ifndef __UT_CMccVIDEOSINK_H__
#define __UT_CMccVIDEOSINK_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccVideoSink;
class CMMFDataBuffer;
class CMccDlDataPath;

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
NONSHARABLE_CLASS( UT_CMccVideoSink )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccVideoSink* NewL();
        static UT_CMccVideoSink* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccVideoSink();

    private:    // Constructors and destructors

        UT_CMccVideoSink();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccVideoSink_NewSinkLL();
        
        
         void UT_CMccVideoSink_EmptyBufferLL();
        
        
         void UT_CMccVideoSink_EmptyBufferL_1L();
        
        
         void UT_CMccVideoSink_SinkThreadLogonL();
        
        
         void UT_CMccVideoSink_SinkThreadLogoffL();
        
        
         void UT_CMccVideoSink_SinkDataTypeCodeL();
        
        
         void UT_CMccVideoSink_SetSinkDataTypeCodeL();
        
        
         void UT_CMccVideoSink_BufferFilledLL();
        
        
         void UT_CMccVideoSink_CanCreateSinkBufferL();
        
        
         void UT_CMccVideoSink_CreateSinkBufferLL();
        
        
         void UT_CMccVideoSink_SinkPrimeLL();
        
        
         void UT_CMccVideoSink_SinkPlayLL();
        
        
         void UT_CMccVideoSink_SinkPauseLL();
        
        
         void UT_CMccVideoSink_SinkStopLL();
        
        
         void UT_CMccVideoSink_SinkCustomCommandL();
         
         void UT_CMccVideoSink_SetVideoCodecL();
        
         void UT_CMccVideoSink_SetAudioCodecL();
         
         void UT_CMccVideoSink_SetCurrentUserL();
         
         void UT_CMccVideoSink_GetParameterL();
         
         void UT_CMccVideoSink_SetParameterL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccVideoSink*  iVideoSink;
        CMMFDataBuffer* iBuffer;
        CMccDlDataPath* iDLDataPathStub;

    };

#endif      //  __UT_CMccVIDEOSINK_H__

// End of file
