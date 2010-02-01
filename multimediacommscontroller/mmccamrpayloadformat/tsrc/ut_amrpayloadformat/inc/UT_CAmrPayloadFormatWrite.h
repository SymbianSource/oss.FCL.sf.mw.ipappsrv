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




#ifndef __UT_CAMRPAYLOADFORMATWRITE_H__
#define __UT_CAMRPAYLOADFORMATWRITE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

#include <mmf/common/mmfcontroller.h>
#include <mmf/server/mmfformat.h>
#include <e32base.h>
#include "rtpapi.h"
#include "mccdef.h"         // MMccEventHandler definition
#include "mccinternaldef.h" // MMccEventHandler definition   
#include "mccinternalcodecs.h" 

//  FORWARD DECLARATIONS
class CAmrPayloadFormatWrite;
class MDataSink;
class CMccRtpDataSink;
class CMccRtpMediaClock;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CAmrPayloadFormatWrite
     : public CEUnitTestSuiteClass, public MAsyncEventHandler, public MMccEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CAmrPayloadFormatWrite* NewL();
        static UT_CAmrPayloadFormatWrite* NewLC();
        /**
         * Destructor
         */
        ~UT_CAmrPayloadFormatWrite();

    private:    // Constructors and destructors

        UT_CAmrPayloadFormatWrite();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods
        
         void SetupAL();
         
         void Setup2AL();
         
         void SetupL();
         
         void SetupWbAL();
         
         void SetupWbL();
         
         void CommonSetupL( TBool aIsNb );
         
         void Teardown();
         
                  
         void UT_CAmrPayloadFormatWrite_NewLL();
        
        
         void UT_CAmrPayloadFormatWrite_CancelUlRequestL();
        
        
         void UT_CAmrPayloadFormatWrite_EmptySourceBufferLL();
        
        
         void UT_CAmrPayloadFormatWrite_SourceBufferEmptiedLL();
        
        
         void UT_CAmrPayloadFormatWrite_FrameTimeIntervalL();
        
        
         void UT_CAmrPayloadFormatWrite_DurationL();
        
        
         void UT_CAmrPayloadFormatWrite_NumChannelsL();
        
        
         void UT_CAmrPayloadFormatWrite_SampleRateL();
        
        
         void UT_CAmrPayloadFormatWrite_SetSampleRateL();
        
         void UT_CAmrPayloadFormatWrite_SetSampleRateWbL();
        

         void UT_CAmrPayloadFormatWrite_CreateSinkBufferLL();
        
        
         void UT_CAmrPayloadFormatWrite_SinkDataTypeCodeL();
        
        
         void UT_CAmrPayloadFormatWrite_SetSinkDataTypeCodeL();
        
        
         void UT_CAmrPayloadFormatWrite_SinkThreadLogonL();
        
        
         void UT_CAmrPayloadFormatWrite_SinkThreadLogoffL();
        
        
         void UT_CAmrPayloadFormatWrite_EmptyBufferLL();
        
        
         void UT_CAmrPayloadFormatWrite_BufferEmptiedLL();
        
        
         void UT_CAmrPayloadFormatWrite_SinkPrimeLL();
        
        
         void UT_CAmrPayloadFormatWrite_SinkPlayLL();
        
        
         void UT_CAmrPayloadFormatWrite_SinkPauseLL();
        
        
         void UT_CAmrPayloadFormatWrite_SinkStopLL();
        
         
         void UT_CAmrPayloadFormatWrite_ConfigurePayloadFormatLL();
         
         void UT_CAmrPayloadFormatWrite_IsBitrateChangeValidL();

         
         // Privates
         
              
         void UT_CAmrPayloadFormatWrite_ProcessFramesLL();
         
         
         void UT_CAmrPayloadFormatWrite_DeliverPacketLL();


         // VIRTUALS from derived classes
   
        
         virtual TInt SendEventToClient(const TMMFEvent& /*aEvent*/) { return KErrNone; }
    
    
         virtual TInt SendMccEventToClient( TMccEvent& /*aEvent*/ ) { return KErrNone; }
	
         virtual void StateChange( TInt /*aState*/, TUint32 /*aLink*/ ) {}

         virtual void ErrorOccured( TInt /*aError*/, TUint32 /*aSession*/, TUint32 /*aLink*/,
                               TUint32 /*aStreamId*/, TUint32 /*aEndpointId*/ ) {}
            
         virtual void EmptySourceBufferL( ) {}
        
         virtual void SourceBufferEmptiedL( ) {}


    private:    // Data
        
         CAmrPayloadFormatWrite* iWrite;
         
        TBool iAlloc;

        MDataSource* iUlSink;
        CMccRtpDataSink* iRtpDataSinkStub;
        CMMFBuffer* iBuffer;
        CMMFBuffer* iBuf;       
        CMMFDataBuffer* iBuff;
        CMMFBuffer* iBuffi;
        TMccCodecInfo iCodec;
        
        CMccRtpMediaClock* iRtpMediaClock;
        TMediaId iMediaId;
        
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CAMRPAYLOADFORMATWRITE_H__

// End of file
