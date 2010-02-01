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




#ifndef __UT_CMccSYMULSTREAM_H__
#define __UT_CMccSYMULSTREAM_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "Mccinternalcodecs.h" 

//  FORWARD DECLARATIONS
class CMccRtpManager;
class CMccTestEventHandler;
class CMccSymUlStream;
class MDataSource;
class MDataSink;
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
NONSHARABLE_CLASS( UT_CMccSymUlStream )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSymUlStream* NewL();
        static UT_CMccSymUlStream* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccSymUlStream();

    private:    // Constructors and destructors

        UT_CMccSymUlStream();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Setup2L();
        
         void Teardown();
        
         void UT_CMccSymUlStream_SetPrioritySettingsLL();
        
        
         void UT_CMccSymUlStream_AddSinkAndSourceLL();
        
        
         void UT_CMccSymUlStream_LoadCodecLL();
        
        
         void UT_CMccSymUlStream_UpdateCodecInformationLL();
        
        
         void UT_CMccSymUlStream_PrimeLL();
        
        
         void UT_CMccSymUlStream_PlayLL();
        
        
         void UT_CMccSymUlStream_PauseLL();
        
        
         void UT_CMccSymUlStream_ResumeLL();
        
        
         void UT_CMccSymUlStream_StopLL();
        
        
         void UT_CMccSymUlStream_SetBalanceLL();
        
        
         void UT_CMccSymUlStream_GetBalanceLL();
        
        
         void UT_CMccSymUlStream_SendMediaSignalLL();
         
         void UT_CMccSymUlStream_DoChangeBitrateLL();
        
        
         void UT_CMccSymUlStream_SendRTCPReceiverReportLL();
        
        
         void UT_CMccSymUlStream_ResetCountersLL();
        
        
         void UT_CMccSymUlStream_SendRTCPSenderReportLL();
        
        
         void UT_CMccSymUlStream_SendRTCPDataLL();
        
        
         void UT_CMccSymUlStream_GetSSRCLL();
        
    public: // base class test cases
         
         void UT_CMccSymUlStreamBase_SetGainL();
         void UT_CMccSymUlStreamBase_SetVolumeL();
         void UT_CMccSymUlStreamBase_MaxVolumeL();
         void UT_CMccSymUlStreamBase_MaxGainL();
         void UT_CMccSymUlStreamBase_VolumeL();
         void UT_CMccSymUlStreamBase_GainL();
         void UT_CMccSymUlStreamBase_SendMediaSignalL();
         void UT_CMccSymUlStreamBase_SendRTCPDataL();
         void UT_CMccSymUlStreamBase_StartInactivityTimerL();
         void UT_CMccSymUlStreamBase_StopInactivityTimerL();
         void UT_CMccSymUlStreamBase_GetFmtpL();
         void UT_CMccSymUlStreamBase_GetCodecL();
         void UT_CMccSymUlStreamBase_ParsePayloadTypesLL();
         void UT_CMccSymUlStreamBase_ChangeBitrateL();
         void UT_CMccSymUlStreamBase_DoChangeBitrateLL();
         void UT_CMccSymUlStreamBase_BindContextIntoStreamLL();
         void UT_CMccSymUlStreamBase_RemoveContextL();
         void UT_CMccSymUlStreamBase_InformStreamStateChange();
         void UT_CMccSymUlStreamBase_HandleStandby();
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CMccTestEventHandler* iHandler;
        CMccRtpManager* iRtpMan;
        CMccSymUlStream* iStream;
        TUint32 iStreamId;
        MDataSource* iSourceStub;
        MDataSink*   iSinkStub;
        CMccRtpMediaClock* iRtpMediaClock;
        
        CMccResourcePoolStub* iResources;
        TMccCodecInfo iCInfo;
        TUint32 iMccSessionId;
        
    };

#endif      //  __UT_CMccSYMULSTREAM_H__

// End of file
