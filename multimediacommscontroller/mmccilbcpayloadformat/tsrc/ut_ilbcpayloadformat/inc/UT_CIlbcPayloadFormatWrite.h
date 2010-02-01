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




#ifndef __UT_CILBCPAYLOADFORMATWRITE_H__
#define __UT_CILBCPAYLOADFORMATWRITE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <mmf/common/mmfcontroller.h>
#include <mmf/server/mmfformat.h>
#include <e32base.h>
#include "rtpapi.h"
#include "mccdef.h"         // MMccEventHandler definition
#include "mccinternaldef.h" // MMccEventHandler definition   
#include "mccinternalcodecs.h" 

//  FORWARD DECLARATIONS
class CIlbcPayloadFormatWrite;
class MDataSink;
class CMccRtpDataSink;
class CMccRtpMediaClock;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CIlbcPayloadFormatWrite : public CEUnitTestSuiteClass,
    public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIlbcPayloadFormatWrite* NewL();
        static UT_CIlbcPayloadFormatWrite* NewLC();
        /**
         * Destructor
         */
        ~UT_CIlbcPayloadFormatWrite();

    private:    // Constructors and destructors

        UT_CIlbcPayloadFormatWrite();
        void ConstructL();

    private:    // New methods

         
         void SetupAL();
         
         void Setup2AL();
                 
         void SetupL();
         
         void Teardown();
                          
        
         void UT_CIlbcPayloadFormatWrite_NewLL();
        
         void UT_CIlbcPayloadFormatWrite_FrameTimeIntervalL();
        
        
         void UT_CIlbcPayloadFormatWrite_DurationL();
        
        
         void UT_CIlbcPayloadFormatWrite_CreateSinkBufferLL();
        
        
         void UT_CIlbcPayloadFormatWrite_SinkDataTypeCodeL();
        
        
         void UT_CIlbcPayloadFormatWrite_SetSinkDataTypeCodeL();
        
        
         void UT_CIlbcPayloadFormatWrite_SinkThreadLogonL();
        
        
         void UT_CIlbcPayloadFormatWrite_SinkThreadLogoffL();
        
        
         void UT_CIlbcPayloadFormatWrite_EmptyBufferLL();
        
        
         void UT_CIlbcPayloadFormatWrite_BufferEmptiedLL();
        
        
         void UT_CIlbcPayloadFormatWrite_SampleRateL();
        
        
         void UT_CIlbcPayloadFormatWrite_SetSampleRateL();


		 void UT_CIlbcPayloadFormatWrite_DeliverPacketLL();
         
         
         void UT_CIlbcPayloadFormatWrite_EmptySourceBufferLL();
        
        
         void UT_CIlbcPayloadFormatWrite_SourceBufferEmptiedLL();
        
        
         void UT_CIlbcPayloadFormatWrite_SinkPrimeLL();
        
        
         void UT_CIlbcPayloadFormatWrite_SinkPlayLL();
        
        
         void UT_CIlbcPayloadFormatWrite_SinkPauseLL();
        
        
         void UT_CIlbcPayloadFormatWrite_SinkStopLL();
         
         void UT_CIlbcPayloadFormatWrite_ConfigurePayloadFormatLL();
         
         void UT_CIlbcPayloadFormatWrite_UpdateConfigurationLL();
         
         void UT_CIlbcPayloadFormatWrite_NumChannelsL();
                 
    
    // VIRTUALS
    
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
        CIlbcPayloadFormatWrite* iWrite;
        MDataSource* iUlSink;
        MDataSink* iRtpDataSinkStub;
        CMMFBuffer* iBuffer;
        CMMFBuffer* iBuf;
        
        CMMFDataBuffer* iBuff;
        
        TMccCodecInfo iCodec;
        
        CMccRtpMediaClock* iRtpMediaClock;
        TMediaId iMediaId;
        
		EUNIT_DECLARE_TEST_TABLE;
		
    };

#endif      //  __UT_CILBCPAYLOADFORMATWRITE_H__

// End of file
