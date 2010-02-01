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




#ifndef __UT_CG711PAYLOADFORMATREAD_H__
#define __UT_CG711PAYLOADFORMATREAD_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <mmf/common/mmfcontroller.h>
#include <mmf/server/mmfformat.h>
#include <e32base.h>
#include "rtpapi.h"
#include "mccdef.h"
#include "mccinternaldef.h" // MMccEventHandler definition
#include "mccinternalcodecs.h" 

//  FORWARD DECLARATIONS
class CG711PayloadFormatRead;
class MDataSource;
class CMccDlDataPath;
class CMccRtpDataSource;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CG711PayloadFormatRead
     : public CEUnitTestSuiteClass, public MAsyncEventHandler, public MMccEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CG711PayloadFormatRead* NewL();
        static UT_CG711PayloadFormatRead* NewLC();
        /**
         * Destructor
         */
        ~UT_CG711PayloadFormatRead();

    private:    // Constructors and destructors

        UT_CG711PayloadFormatRead();
        void ConstructL();
        CG711PayloadFormatRead* iRead;

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupAL();
         
         void SetupL();
         
         void Teardown();
      
        
         void UT_CG711PayloadFormatRead_NewLL();
        
        
         void UT_CG711PayloadFormatRead_InitializeL();

     
         void UT_CG711PayloadFormatRead_SendDataToSinkLL();
        
        
         void UT_CG711PayloadFormatRead_FillSinkBufferL();
        
        
         void UT_CG711PayloadFormatRead_FillSourceBufferLL();
        
        
         void UT_CG711PayloadFormatRead_StreamsL();
        
        
         void UT_CG711PayloadFormatRead_FrameTimeIntervalL();
        
        
         void UT_CG711PayloadFormatRead_DurationL();
        
        
         void UT_CG711PayloadFormatRead_NumChannelsL();
        
        
         void UT_CG711PayloadFormatRead_SampleRateL();
        
        
         void UT_CG711PayloadFormatRead_SetSampleRateL();
        
        
         void UT_CG711PayloadFormatRead_SourceDataTypeCodeL();
        
        
         void UT_CG711PayloadFormatRead_SetSourceDataTypeCodeL();
        
        
         void UT_CG711PayloadFormatRead_CreateSourceBufferLL();
        
        
         void UT_CG711PayloadFormatRead_CreateSourceBufferL_1L();
        
        
         void UT_CG711PayloadFormatRead_FillBufferLL();
        
        
         void UT_CG711PayloadFormatRead_SourceThreadLogonL();
        
        
         void UT_CG711PayloadFormatRead_SourceThreadLogoffL();
        
        
         void UT_CG711PayloadFormatRead_NegotiateSourceLL();
        
        
         void UT_CG711PayloadFormatRead_SourcePrimeLL();
        
        
         void UT_CG711PayloadFormatRead_SourcePlayLL();
        
        
         void UT_CG711PayloadFormatRead_SourcePauseLL();
        
        
         void UT_CG711PayloadFormatRead_SourceStopLL();
        
        
         void UT_CG711PayloadFormatRead_BufferFilledLL();
        
        
         void UT_CG711PayloadFormatRead_SinkDataTypeCodeL();
         
         
         void UT_CG711PayloadFormatRead_ConfigurePayloadFormatLL();
         
         
         void UT_CG711PayloadFormatRead::UpdateConfigurationLL();
         
    // privates     
         
         void UT_CG711PayloadFormatRead_GetNextFrameL(); 

    
    // VIRTUAALIT
    
        virtual TInt SendEventToClient( const TMMFEvent& /*aEvent*/ ) { return KErrNone; }
         
        virtual TInt SendMccEventToClient( TMccEvent& /*aEvent*/ ) { return KErrNone; }
         
        virtual void StateChange( TInt /*aState*/, TUint32 /*aLink*/ ) {}
         
        virtual void ErrorOccured( TInt /*aError*/, 
                                   TUint32 /*aSessionId*/,
                                   TUint32 /*aLinkId*/,
            	                   TUint32 /*aStreamId*/, 
            	                   TUint32 /*aEndpointId*/ ) {}
	    
	       
    
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
		TBool iAlloc;
		
    	MDataSource*            iRtpDataSourceStub;
	    CMccDlDataPath*         iDLDataPathStub;
		CMMFDataBuffer*   iBuffi;

        TMediaId iMediaId;
		TFourCC iFourCC;
		MAsyncEventHandler* aEventH;
		MDataSink* iSink;		
		TMccCodecInfo iCodec;
		MDataSink* iConsumer;
		
    };

#endif      //  __UT_CG711PAYLOADFORMATREAD_H__

// End of file
