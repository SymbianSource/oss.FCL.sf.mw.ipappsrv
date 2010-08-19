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




#ifndef __UT_CMccSYMSIMPLEDLSTREAM_H__
#define __UT_CMccSYMSIMPLEDLSTREAM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>

//  INTERNAL INCLUDES
#include "MccInternalDef.h"

//  FORWARD DECLARATIONS
class CMccSymSimpleDlStream;
class CMccRtpManager;
class MDataSource;
class MDataSink;
class CMccTestEventHandler;
class CMccResourcePoolStub;
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
NONSHARABLE_CLASS( UT_CMccSymSimpleDlStream )
     : public CEUnitTestSuiteClass,
       public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSymSimpleDlStream* NewL();
        static UT_CMccSymSimpleDlStream* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccSymSimpleDlStream();

    private:    // Constructors and destructors

        UT_CMccSymSimpleDlStream();
        void ConstructL();

    public:     // From observer interface MAsyncEventHandler
        TInt SendEventToClient(const TMMFEvent& /*aEvent*/)
	        {
	        return KErrNone;	
	        }
	
    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccSymSimpleDlStream_CreatePayloadFormatDecoderLL();
        
        
         void UT_CMccSymSimpleDlStream_SetPrioritySettingsLL();
        
        
         void UT_CMccSymSimpleDlStream_AddSinkAndSourceLL();
        
        
         void UT_CMccSymSimpleDlStream_LoadCodecLL();
        
        
         void UT_CMccSymSimpleDlStream_UpdateCodecInformationLL();
        
        
         void UT_CMccSymSimpleDlStream_PrimeLL();
        
        
         void UT_CMccSymSimpleDlStream_PlayLL();
        
        
         void UT_CMccSymSimpleDlStream_PauseLL();
        
        
         void UT_CMccSymSimpleDlStream_ResumeLL();
        
        
         void UT_CMccSymSimpleDlStream_StopLL();
        
        
         void UT_CMccSymSimpleDlStream_SetBalanceLL();
        
        
         void UT_CMccSymSimpleDlStream_GetBalanceLL();
        
        
         void UT_CMccSymSimpleDlStream_GetSSRCLL();
        
        
         void UT_CMccSymSimpleDlStream_ResetCountersLL();
        
        
         void UT_CMccSymSimpleDlStream_SendRTCPReceiverReportLL();
        
        
         void UT_CMccSymSimpleDlStream_SendRTCPSenderReportLL();
        
        
         void UT_CMccSymSimpleDlStream_SendRTCPReceiverReportL();
        
        
         void UT_CMccSymSimpleDlStream_StartInactivityTimerLL();
        
        
         void UT_CMccSymSimpleDlStream_StopInactivityTimerLL();
        
        
         void UT_CMccSymSimpleDlStream_IsUplinkL();
        
        
         void UT_CMccSymSimpleDlStream_StandByLL();
        
        
         void UT_CMccSymSimpleDlStream_JitterBufferErrorL();
         
         void UT_CMccSymSimpleDlStream_SendEventToClientL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccSymSimpleDlStream* iSimpleDlStream;
        CMccRtpManager*        iRtpManager;
        TUint32                iStreamId;
        MDataSource* iSourceStub;
        MDataSink*   iSinkStub;
        CMccTestEventHandler* iEventHandler;
        
        CMccResourcePoolStub* iResources;
        
        CMccRtpMediaClock* iRtpMediaClock;
		
        TUint32 iMccSessionId;

    };

#endif      //  __UT_CMccSYMSIMPLEDLSTREAM_H__

// End of file
