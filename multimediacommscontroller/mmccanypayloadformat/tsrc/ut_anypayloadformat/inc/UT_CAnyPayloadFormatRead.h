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




#ifndef __UT_CANYPAYLOADFORMATREAD_H__
#define __UT_CANYPAYLOADFORMATREAD_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CAnyPayloadFormatRead;
class MDataSource;
//class CMccDataPath_Stub;
class CMccDlDataPath;
class CMccRtpDataSource;
//class CMMFDescriptorBuffer;
class CMMFDataBuffer;
//class CMccRtpDataSource_Stub;

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
NONSHARABLE_CLASS( UT_CAnyPayloadFormatRead )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CAnyPayloadFormatRead* NewL();
        static UT_CAnyPayloadFormatRead* NewLC();
        /**
         * Destructor
         */
        ~UT_CAnyPayloadFormatRead();

    private:    // Constructors and destructors

        UT_CAnyPayloadFormatRead();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
                
         void UT_CAnyPayloadFormatRead_NewL();
         
         void UT_CAnyPayloadFormatRead_CreateClipBufferL();

         void UT_CAnyPayloadFormatRead_LastDlPacketCountL();
         
         void UT_CAnyPayloadFormatRead_Duration();
        
        
         void UT_CAnyPayloadFormatRead_CancelDlRequestL();
        
        
         void UT_CAnyPayloadFormatRead_SendDataToSinkLL();
        
        
         void UT_CAnyPayloadFormatRead_FillSinkBufferL();
        
        
         void UT_CAnyPayloadFormatRead_FillSourceBufferLL();
        
        
         void UT_CAnyPayloadFormatRead_StreamsL();
        
        
         void UT_CAnyPayloadFormatRead_FrameTimeIntervalL();
        
        
         void UT_CAnyPayloadFormatRead_DurationL();
        
        
         void UT_CAnyPayloadFormatRead_SinkDataTypeCodeL();
        
        
         void UT_CAnyPayloadFormatRead_FillBufferLL();
        
        
         void UT_CAnyPayloadFormatRead_SourcePrimeLL();
        
        
         void UT_CAnyPayloadFormatRead_SourcePlayLL();
        
        
         void UT_CAnyPayloadFormatRead_SourcePauseLL();
        
        
         void UT_CAnyPayloadFormatRead_SourceStopLL();
        
        
         void UT_CAnyPayloadFormatRead_CreateSourceBufferLL();
       
        
         void UT_CAnyPayloadFormatRead_SourceDataTypeCodeL();
        
        
         void UT_CAnyPayloadFormatRead_SetSourceDataTypeCodeL();
        
        
         void UT_CAnyPayloadFormatRead_SourceThreadLogonL();
        
        
         void UT_CAnyPayloadFormatRead_SourceThreadLogoffL();
        
        
         void UT_CAnyPayloadFormatRead_NegotiateSourceLL();
        
        
         void UT_CAnyPayloadFormatRead_BufferFilledLL();
         
         void UT_CAnyPayloadFormatRead_BufferFilledL2L();
         
         void UT_CAnyPayloadFormatRead_FillBufferL_BufferFilledLL();

         void UT_CAnyPayloadFormatRead_ConfigurePayloadFormatLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CAnyPayloadFormatRead*  iFormatRead;
    	MDataSource*            iRtpDataSourceStub;
	    CMccDlDataPath*         iDLDataPathStub;
		CMMFDataBuffer*   iBuffer;
      
    };

#endif      //  __UT_CANYPAYLOADFORMATREAD_H__

// End of file
