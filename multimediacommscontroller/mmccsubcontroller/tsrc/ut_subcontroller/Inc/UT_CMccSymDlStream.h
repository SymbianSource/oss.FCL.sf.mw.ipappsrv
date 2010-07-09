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




#ifndef __UT_CMccSYMDLSTREAM_H__
#define __UT_CMccSYMDLSTREAM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccRtpManager;
class CMccTestEventHandler;
class CMccSymDlStream;
class MDataSource;
class MDataSink;
class CMccResourcePoolStub;
class CMccMultiplexer;
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
NONSHARABLE_CLASS( UT_CMccSymDlStream )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSymDlStream* NewL();
        static UT_CMccSymDlStream* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccSymDlStream();

    private:    // Constructors and destructors

        UT_CMccSymDlStream();
        void ConstructL();

    private:    // New methods

        void SetupL();
        void Setup2L();
        void SetupIlbcL();
        void Teardown();
        
        void UT_CMccSymDlStream_SetPrioritySettingsLL();
        
        void UT_CMccSymDlStream_CreatePayloadFormatDecoderL();
         
        void UT_CMccSymDlStream_DoDynamicFrameSizeChangeL();
         
        void UT_CMccSymDlStream_MultiplexerSink();
         
        void UT_CMccSymDlStream_AddSinkAndSourceLL();
        
        void UT_CMccSymDlStream_LoadCodecLL();
        
        void UT_CMccSymDlStream_UpdateCodecInformationLL();
        void IlbcModeChangeTestL();
        
        void UT_CMccSymDlStream_PrimeLL();
        
        void UT_CMccSymDlStream_PlayLL();
        
        void UT_CMccSymDlStream_PauseLL();
        
        void UT_CMccSymDlStream_ResumeLL();
        
        void UT_CMccSymDlStream_StopLL();
        
        void UT_CMccSymDlStream_SetBalanceLL();
        
        void UT_CMccSymDlStream_GetBalanceLL();
        
        void UT_CMccSymDlStream_GetSSRCLL();
        
        void UT_CMccSymDlStream_ResetCountersLL();
        
        void UT_CMccSymDlStream_SendMediaSignalL();
        
        void UT_CMccSymDlStream_SendRTCPReceiverReportLL();
        
        void UT_CMccSymDlStream_SendRTCPSenderReportLL();
        
        void UT_CMccSymDlStream_StartInactivityTimerLL();
        
        void UT_CMccSymDlStream_StopInactivityTimerLL();
         
        void UT_CMccSymDlStream_AudioRoutingLL();
        
        void UT_CMccSymDlStream_IsUplink();
         
        void UT_CMccSymDlStream_StandByL();
         
        void UT_CMccSymDlStream_JitterBufferError();
         
        void UT_CMccSymDlStream_BindContextIntoStreamL();
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        CMccTestEventHandler* iHandler;
        CMccRtpManager* iRtpMan;
        CMccSymDlStream* iStream;
        TUint32 iStreamId;
        
        MDataSource* iSourceStub;
        MDataSink* iSinkStub;
        
        CMccResourcePoolStub* iResources;
        CMccMultiplexer* iMultiPlexer;
        CMccRtpMediaClock* iRtpMediaClock;
		
        TUint32 iMccSessionId;

    };

#endif      //  __UT_CMccSYMDLSTREAM_H__

// End of file
