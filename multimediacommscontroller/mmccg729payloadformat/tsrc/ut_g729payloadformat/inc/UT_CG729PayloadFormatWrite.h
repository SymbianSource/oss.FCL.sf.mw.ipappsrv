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




#ifndef __UT_CG729PAYLOADFORMATWRITE_H__
#define __UT_CG729PAYLOADFORMATWRITE_H__

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
class CG729PayloadFormatWrite;
class MDataSink;
class CMccRtpDataSink;
class CMccRtpMediaClock;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CG729PayloadFormatWrite
     : public CEUnitTestSuiteClass, public MAsyncEventHandler, public MMccEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CG729PayloadFormatWrite* NewL();
        static UT_CG729PayloadFormatWrite* NewLC();
        /**
         * Destructor
         */
        ~UT_CG729PayloadFormatWrite();

    private:    // Constructors and destructors

        UT_CG729PayloadFormatWrite();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods
         
         void SetupAL();
     
         void Setup2AL();
         
         void SetupL();
         
         void Teardown();
         
        
         void UT_CG729PayloadFormatWrite_NewLL();
        
        
         void UT_CG729PayloadFormatWrite_InitializeL();
      
        
         void UT_CG729PayloadFormatWrite_EmptySourceBufferLL();


         void UT_CG729PayloadFormatWrite_SourceBufferEmptiedLL();
        
        
         void UT_CG729PayloadFormatWrite_SinkPrimeLL();
        
        
         void UT_CG729PayloadFormatWrite_SinkPlayLL();
        
        
         void UT_CG729PayloadFormatWrite_SinkPauseLL();
        
        
         void UT_CG729PayloadFormatWrite_SinkStopLL();
         
        
         void UT_CG729PayloadFormatWrite_DoBitPackingL( );
                    
                    
         void UT_CG729PayloadFormatWrite_FrameTimeIntervalL();
        
        
         void UT_CG729PayloadFormatWrite_DurationL();
        
        
         void UT_CG729PayloadFormatWrite_CreateSinkBufferLL();
        
        
         void UT_CG729PayloadFormatWrite_SinkDataTypeCodeL();
        
        
         void UT_CG729PayloadFormatWrite_SetSinkDataTypeCodeL();
        
        
         void UT_CG729PayloadFormatWrite_SinkThreadLogonL();
        
        
         void UT_CG729PayloadFormatWrite_SinkThreadLogoffL();
        
        
         void UT_CG729PayloadFormatWrite_EmptyBufferLL();
        
        
         void UT_CG729PayloadFormatWrite_BufferEmptiedLL();
        
        
         void UT_CG729PayloadFormatWrite_NumChannelsL();
        
        
         void UT_CG729PayloadFormatWrite_SampleRateL();
        
        
         void UT_CG729PayloadFormatWrite_SetSampleRateL();
         
         void UT_CG729PayloadFormatWrite_ConfigurePayloadFormatLL();
         
         void UT_CG729PayloadFormatWrite_UpdateConfigurationLL();

         // privates
         
         void UT_CG729PayloadFormatWrite_DeliverPacketL();
         
        
        virtual TInt SendEventToClient( const TMMFEvent& /*aEvent*/ ) { return KErrNone; }
         
        virtual TInt SendMccEventToClient( TMccEvent& /*aEvent*/ ) { return KErrNone; }
        
        virtual void StateChange( TInt /*aState*/, TUint32 /*aLink*/ ) {}
         
        virtual void ErrorOccured( TInt /*aError*/, 
                                   TUint32 /*aSessionId*/,
                                   TUint32 /*aLinkId*/,
            	                   TUint32 /*aStreamId*/, 
            	                   TUint32 /*aEndpointId*/ ) {}
   
    
    private:    // Data
        
       
        TBool iAlloc;
        CG729PayloadFormatWrite* iWrite;
        MDataSource* iUlSink;
        CMccRtpDataSink* iRtpDataSinkStub;
        CMMFBuffer* iBuffer;
        CMMFBuffer* iBuf;
        
        CMMFDataBuffer* iBuff;
        
        TMccCodecInfo iCodec;
        
        CMccRtpMediaClock* iRtpMediaClock;
        TMediaId iMediaId;
        
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CG729PAYLOADFORMATWRITE_H__

// End of file
