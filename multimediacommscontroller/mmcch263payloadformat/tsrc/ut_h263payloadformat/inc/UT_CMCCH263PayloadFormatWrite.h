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




#ifndef __UT_CMccH263PAYLOADFORMATWRITE_H__
#define __UT_CMccH263PAYLOADFORMATWRITE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccH263PayloadFormatWrite;

class CMccUlDataPath;
class MDataSink;
class CMccRtpMediaClock;

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
NONSHARABLE_CLASS( UT_CMccH263PayloadFormatWrite )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccH263PayloadFormatWrite* NewL();
        static UT_CMccH263PayloadFormatWrite* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccH263PayloadFormatWrite();

    private:    // Constructors and destructors

        UT_CMccH263PayloadFormatWrite();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccH263PayloadFormatWrite_NewLL();
                 
        
         void UT_CMccH263PayloadFormatWrite_EmptySourceBufferLL();
         
         void UT_CMccH263PayloadFormatWrite_EmptySourceBufferL2L();
        
         void UT_CMccH263PayloadFormatWrite_SourceBufferEmptiedLL();
        
        
         void UT_CMccH263PayloadFormatWrite_FrameTimeIntervalL();
        
        
         void UT_CMccH263PayloadFormatWrite_DurationL();
        
        
         void UT_CMccH263PayloadFormatWrite_EmptyBufferLL();
        
         void UT_CMccH263PayloadFormatWrite_EmptyBufferL2L();
        
         void UT_CMccH263PayloadFormatWrite_BufferEmptiedLL();
        
        
         void UT_CMccH263PayloadFormatWrite_CanCreateSinkBufferL();
        
        
         void UT_CMccH263PayloadFormatWrite_CreateSinkBufferLL();
        
        
         void UT_CMccH263PayloadFormatWrite_SinkPrimeLL();
        
        
         void UT_CMccH263PayloadFormatWrite_SinkPlayLL();
        
        
         void UT_CMccH263PayloadFormatWrite_SinkPauseLL();
        
        
         void UT_CMccH263PayloadFormatWrite_SinkStopLL();
        
        
         void UT_CMccH263PayloadFormatWrite_SinkThreadLogonL();
        
        
         void UT_CMccH263PayloadFormatWrite_SinkThreadLogoffL();
        
        
         void UT_CMccH263PayloadFormatWrite_SinkDataTypeCodeL();
        
        
         void UT_CMccH263PayloadFormatWrite_SetSinkDataTypeCodeL();
               
        
         void UT_CMccH263PayloadFormatWrite_ConfigurePayloadFormatLL();
        
         void UT_CMccH263PayloadFormatWrite_ResetAndDestroy();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        MDataSink*                  iRtpSinkStub;
        CMccH263PayloadFormatWrite* iWrite;
        CMccUlDataPath*             iULDataPathStub;
        CMccRtpMediaClock*          iRtpMediaClock;

    };

#endif      //  __UT_CMccH263PAYLOADFORMATWRITE_H__

// End of file
