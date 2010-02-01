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




#ifndef __UT_CAMRPAYLOADFORMATREAD_H__
#define __UT_CAMRPAYLOADFORMATREAD_H__




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
class CAmrPayloadFormatRead;
class MDataSource;
class CMccDlDataPath;
class CMccRtpDataSource;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CAmrPayloadFormatRead 
    : public CEUnitTestSuiteClass, public MAsyncEventHandler, public MMccEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CAmrPayloadFormatRead* NewL();
        static UT_CAmrPayloadFormatRead* NewLC();
        /**
         * Destructor
         */
        ~UT_CAmrPayloadFormatRead();

    private:    // Constructors and destructors

        UT_CAmrPayloadFormatRead();
        void ConstructL();
        CAmrPayloadFormatRead* iRead;

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupAL();
         
         void SetupL();
       
         void SetupWbAL();
         
         void SetupWbL();
       
         void Teardown();
         
         void CreateFramesL( TInt aNumFrames, TBool aOkData = EFalse );

         void UT_CAmrPayloadFormatRead_CancelDlRequestL();
        
        
         void UT_CAmrPayloadFormatRead_SendDataToSinkLL();
        
        
         void UT_CAmrPayloadFormatRead_FillSinkBufferL();
        
        
         void UT_CAmrPayloadFormatRead_FillSourceBufferLL();
        
        
         void UT_CAmrPayloadFormatRead_StreamsL();
        
        
         void UT_CAmrPayloadFormatRead_FrameTimeIntervalL();
        
        
         void UT_CAmrPayloadFormatRead_DurationL();
        
        
         void UT_CAmrPayloadFormatRead_NumChannelsL();
        
        
         void UT_CAmrPayloadFormatRead_SampleRateL();
        
        
         void UT_CAmrPayloadFormatRead_SetSampleRateL();
        
         void UT_CAmrPayloadFormatRead_SetSampleRateWbL();
        
        
         void UT_CAmrPayloadFormatRead_SinkDataTypeCodeL();
        
        
         void UT_CAmrPayloadFormatRead_FillBufferLL();
        
        
         void UT_CAmrPayloadFormatRead_SourcePrimeLL();
        
        
         void UT_CAmrPayloadFormatRead_SourcePlayLL();
        
        
         void UT_CAmrPayloadFormatRead_SourcePauseLL();
        
        
         void UT_CAmrPayloadFormatRead_SourceStopLL();
        
        
         void UT_CAmrPayloadFormatRead_CreateSourceBufferLL();
        
        
         void UT_CAmrPayloadFormatRead_CreateSourceBufferL_1L();
        
        
         void UT_CAmrPayloadFormatRead_SourceDataTypeCodeL();
        
        
         void UT_CAmrPayloadFormatRead_SetSourceDataTypeCodeL();
        
        
         void UT_CAmrPayloadFormatRead_SourceThreadLogonL();
        
        
         void UT_CAmrPayloadFormatRead_SourceThreadLogoffL();
        
        
         void UT_CAmrPayloadFormatRead_NegotiateSourceLL();
        
        
         void UT_CAmrPayloadFormatRead_BufferFilledLL();
         
         
         void UT_CAmrPayloadFormatRead_ConfigurePayloadFormatLL();
         
         void UT_CAmrPayloadFormatRead_ConfigurePayloadFormatLWbL();

         void UT_CAmrPayloadFormatRead_ConvertModeToBitrate();

         void UT_CAmrPayloadFormatRead_ConvertModeToBitrateWb();
         
         void UT_CAmrPayloadFormatRead_SendAmrEventToClient();

         // PRIVATE METHODS
    
         void UT_CAmrPayloadFormatRead_DoReadL();
         
         
    
    // VIRTUAL METHODS
    
    virtual TInt SendEventToClient(const TMMFEvent& /*aEvent*/) { return KErrNone; }
    
    
    virtual TInt SendMccEventToClient( TMccEvent& /*aEvent*/ ) { return KErrNone; }
	
    virtual void StateChange( TInt /*aState*/, TUint32 /*aLink*/ ) {}

    virtual void ErrorOccured( TInt /*aError*/, TUint32 /*aSession*/, TUint32 /*aLink*/,
                               TUint32 /*aStreamId*/, TUint32 /*aEndpointId*/ ) {}
	
    private:    // Data
    

		EUNIT_DECLARE_TEST_TABLE; 
		
		TBool iAlloc;
		
    	MDataSource*            iRtpDataSourceStub;
	    CMccDlDataPath*         iDLDataPathStub;
		CMMFDataBuffer*   iBuffi;
        CMMFDescriptorBuffer* iBuf;
		MDataSink* iConsumer;
        
        TMediaId iMediaId;
		TFourCC iFourCC;
		MAsyncEventHandler* aEventH;
		MDataSink* iSink;		
		TMccCodecInfo iCodec;
		
		HBufC8* iFrameData;
		
    };

#endif      //  __UT_CAMRPAYLOADFORMATREAD_H__

// End of file
