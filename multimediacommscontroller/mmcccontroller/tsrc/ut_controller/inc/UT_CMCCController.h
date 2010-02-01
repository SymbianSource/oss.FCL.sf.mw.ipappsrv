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




#ifndef __UT_CMccCONTROLLER_H__
#define __UT_CMccCONTROLLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <mmf/common/mmffourcc.h>
#include <mmf/common/mmfstandardcustomcommands.h>

//  FORWARD DECLARATIONS
class CMccController;
class CMccRtpDataSource;
class CMccRtpDataSink;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccController
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccController* NewL();
        static UT_CMccController* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccController();

    private:    // Constructors and destructors

        UT_CMccController();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
         void Setup2L();
         
         void Teardown();
         void Teardown2();
         
         void UT_CMccController_NewLL();
        
        
         void UT_CMccController_CMccControllerL();
        
        
         void UT_CMccController_ConstructLL();
        
        
         void UT_CMccController_CreateSessionLL();
        
        
         void UT_CMccController_CloseSessionLL();
        
        
         void UT_CMccController_CreateLinkLL();
        
        
         void UT_CMccController_CloseLinkLL();
        
        
         void UT_CMccController_StartStreamLL();
        
        
         void UT_CMccController_StopStreamLL();
        
        
         void UT_CMccController_StartInactivityTimerLL();
        
        
         void UT_CMccController_StopInactivityTimerLL();
        
        
         void UT_CMccController_RequestEventNotificationLL();
        
        
         void UT_CMccController_SendEventL();
        
        
         void UT_CMccController_SendEventLL();
        
        
         void UT_CMccController_CreateStreamLL();
        
        
         void UT_CMccController_DeleteStreamLL();
        
        
         void UT_CMccController_PrepareStreamLL();
        
        
         void UT_CMccController_PauseStreamLL();
        
        
         void UT_CMccController_ResumeStreamLL();
        
        
         void UT_CMccController_GetSupportedCodecsLL();
        
        
         void UT_CMccController_GetCodecLL();
        
        
         void UT_CMccController_GetFmtpAttrLL();
        
        
         void UT_CMccController_SetCodecLL();
        
        
         void UT_CMccController_GetSupportedBitratesLL();
        
        
         void UT_CMccController_SetPriorityLL();
        
        
         void UT_CMccController_StreamSettingsLL();
        
        
         void UT_CMccController_SendRTCPReceiverReportLL();
        
        
         void UT_CMccController_SendRTCPSenderReportLL();
        

         void UT_CMccController_SendRTCPDataLL();

        
         void UT_CMccController_SetRemoteAddressLL();
        
        
         void UT_CMccController_GetPayloadTypeL();
        
        
         void UT_CMccController_FindSessionLL();
        
        
         void UT_CMccController_CustomCommandLL();
        
        
         void UT_CMccController_SendMediaSignalLL();
        
        
         void UT_CMccController_SendMccEventToClientL();
        
        
         void UT_CMccController_SendMediaSignallingToClientL();
        
        
         void UT_CMccController_CustomCommandL();
        
        
         void UT_CMccController_MapdGetVolumeLL();
        
        
         void UT_CMccController_MapdGetMaxVolumeLL();
        
        
         void UT_CMccController_MapdSetVolumeLL();
        
        
         void UT_CMccController_MapdGetBalanceLL();
        
        
         void UT_CMccController_MapdSetBalanceLL();
        
        
         void UT_CMccController_MapdSetVolumeRampLL();
        
        
         void UT_CMccController_MardSetGainLL();
        
        
         void UT_CMccController_MardGetMaxGainLL();
        
        
         void UT_CMccController_MardGetGainLL();
        
        
         void UT_CMccController_MardSetBalanceLL();
        
        
         void UT_CMccController_MardGetBalanceLL();
         
         void UT_CMccController_AddDataSinkL();
         
         void UT_CMccController_AddDataSourceL();
         
         void UT_CMccController_PlayL();
        
         void UT_CMccController_ResetL();
        
         void UT_CMccController_StopL();
        
         void UT_CMccController_PrimeL();
        
         void UT_CMccController_PauseL();
         
         void UT_CMccController_PositionL();
         
         void UT_CMccController_SetPositionL();
         
         void UT_CMccController_DurationL();

         void UT_CMccController_RestartEventMonitorL();
         
         void UT_CMccController_StreamExistsL();
         
         void UT_CMccController_GetSSRCL();
         
         void UT_CMccController_EnableViewFinderL();
         
         void UT_CMccController_DisableViewFinderL();
         
         void UT_CMccController_GetViewFinderSettingsL();
         
         void UT_CMccController_GetCamInfoL();
         
         void UT_CMccController_GetNumOfCamsL();
         
         void UT_CMccController_GetCamValue1L();
         
         void UT_CMccController_SetCamValue1L();
         
         void UT_CMccController_GetCamValue2L();
         
         void UT_CMccController_SetCamValue2L();

         void UT_CMccController_SetAudioRouteL();
         
         void UT_CMccController_GetAudioRouteL();

         void UT_CMccController_SetEndpointParameterL();

         void UT_CMccController_GetEndpointParameterL();

         void UT_CMccController_UpdateEndpointL();
         
         void UT_CMccController_ReferenceCountL();
         
         void UT_CMccController_ReuseL();
         
         void UT_CMccController_SetRemoteRtcpAddrLL();
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
		CMccController* iControllerPtr;
		//Controller
		RMMFController iController;
		
		//Destination of the packets
		TMMFMessageDestinationPckg iMessageDest;
		
		// For easier access to Audio Play Device functionality
        RMMFAudioPlayDeviceCustomCommands iAudioPlayDeviceCustomCommands;

        // For easier access to Audio Record Device functionality
        RMMFAudioRecordDeviceCustomCommands iAudioRecordDeviceCustomCommands;
    
		TUint32 iSessionId, iLinkId, iStreamId;
		
		TRequestStatus iStatus;
		CMccRtpDataSource* iSourceStub;
		CMccRtpDataSink*   iSinkStub;
		
		TUint32 iSinkEndpointId;
		TUint32 iSourceEndpointId;
    };

#endif      //  __UT_CMccCONTROLLER_H__

// End of file
