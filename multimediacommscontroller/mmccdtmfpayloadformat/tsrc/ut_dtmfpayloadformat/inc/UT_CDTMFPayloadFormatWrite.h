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




#ifndef __UT_CDTMFPAYLOADFORMATWRITE_H__
#define __UT_CDTMFPAYLOADFORMATWRITE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <mmf/common/mmfcontroller.h>
#include <mmf/server/mmfformat.h>
#include <mmf/server/sounddevice.h>
#include <mmf/common/mmfcontrollerframework.h>
#include <e32base.h>
#include "rtpapi.h"
#include "mccdef.h"
#include "dtmfpayloadformatdefs.h"
#include "mmccevents.h"


//  FORWARD DECLARATIONS
class CDTMFPayloadFormatWrite;
class MDataSink;
class CMccRtpDataSink;
class CMccRtpManager;
class MMMFAudioOutput;
class CMccRtpMediaClock;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CDTMFPayloadFormatWrite : public CEUnitTestSuiteClass, 
                                   public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CDTMFPayloadFormatWrite* NewL();
        static UT_CDTMFPayloadFormatWrite* NewLC();
        /**
         * Destructor
         */
        ~UT_CDTMFPayloadFormatWrite();

    private:    // Constructors and destructors

        UT_CDTMFPayloadFormatWrite();
        CDTMFPayloadFormatWrite* iWrite;
        void ConstructL();
        

	public: 	// From observer interface
		
		

    private:    // New methods
         void SetupL();

         void Teardown();

         void UT_CDTMFPayloadFormatWrite_NewLL();
         
         void UT_CDTMFPayloadFormatWrite_HandleTimerEventL();
        
         void UT_CDTMFPayloadFormatWrite_HandleControlChar();
         
         void UT_CDTMFPayloadFormatWrite_DeliverPacketL();
         
         void UT_CDTMFPayloadFormatWrite_StartDTMFToneLL();

         void UT_CDTMFPayloadFormatWrite_StopDTMFToneLL();

         void UT_CDTMFPayloadFormatWrite_SendDTMFTonesLL();
         
         void UT_CDTMFPayloadFormatWrite_CancelDTMFStringSending();

         void UT_CDTMFPayloadFormatWrite_NotifyStopInDTMFStringL();

         void UT_CDTMFPayloadFormatWrite_ContinueDTMFStringSendingL();
        
         void UT_CDTMFPayloadFormatWrite_SetFrameTimeIntervalL();
        
         void UT_CDTMFPayloadFormatWrite_PayloadFormatL();
        
         void UT_CDTMFPayloadFormatWrite_SetPayloadFormatL();
        
         void UT_CDTMFPayloadFormatWrite_FrameTimeIntervalL();
        
         void UT_CDTMFPayloadFormatWrite_DurationL();
        
         void UT_CDTMFPayloadFormatWrite_EmptyBufferLL();
        
         void UT_CDTMFPayloadFormatWrite_BufferEmptiedLL();
        
         void UT_CDTMFPayloadFormatWrite_CanCreateSinkBufferL();
        
         void UT_CDTMFPayloadFormatWrite_CreateSinkBufferLL();
        
         void UT_CDTMFPayloadFormatWrite_SinkPrimeLL();
        
         void UT_CDTMFPayloadFormatWrite_SinkPlayLL();
        
         void UT_CDTMFPayloadFormatWrite_SinkPauseLL();
        
         void UT_CDTMFPayloadFormatWrite_SinkStopLL();
        
         void UT_CDTMFPayloadFormatWrite_SinkThreadLogonL();
        
         void UT_CDTMFPayloadFormatWrite_SinkThreadLogoffL();
        
         void UT_CDTMFPayloadFormatWrite_SinkDataTypeCodeL();
        
         void UT_CDTMFPayloadFormatWrite_SetSinkDataTypeCodeL();
         
         void UT_CDTMFPayloadFormatWrite_MatoPrepareCompleteL();
    
         void UT_CDTMFPayloadFormatWrite_MatoPlayCompleteL();
    
         void UT_CDTMFPayloadFormatWrite_SetSinkPrioritySettingsL();
         
         void UT_CDTMFPayloadFormatWrite_PlayDTMFTonesL();
         
         void UT_CDTMFPayloadFormatWrite_ConfigurePayloadFormatLL();
         
         void UT_CDTMFPayloadFormatWrite_EncodeNextDigitLL();
         
         void UT_CDTMFPayloadFormatWrite_DoToneOnActionsLL();
         
         void UT_CDTMFPayloadFormatWrite_DoToneOffActionsLL();

        
         //VIRTUALS
         TInt SendEventToClient( const TMMFEvent& aEvent );

    private:    // Data
    
        MDataSink* iRtpDataSinkStub;
        
        CMccRtpMediaClock* iRtpMediaClock;
        
        CMMFDescriptorBuffer* iBuffer;
        
        MDataSource* iDataSource;
        
        TBuf8<9> iTones;
        
        TMccDtmfEventData iEventData;
        
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CDTMFPAYLOADFORMATWRITE_H__

// End of file
