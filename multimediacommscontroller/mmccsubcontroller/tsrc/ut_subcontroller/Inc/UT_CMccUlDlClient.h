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




#ifndef __UT_CMccULDLCLIENT_H__
#define __UT_CMccULDLCLIENT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

#include "Mcctesteventhandler.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccUlDlClient;
class MDataSource;
class MDataSink;
class CMccResourcePoolStub;

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
NONSHARABLE_CLASS( UT_CMccUlDlClient )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccUlDlClient* NewL();
        static UT_CMccUlDlClient* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccUlDlClient();

    private:    // Constructors and destructors

        UT_CMccUlDlClient();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccUlDlClient_NewLL();
        
        
         void UT_CMccUlDlClient_GetCodecDefaultsLL();
        
        
         void UT_CMccUlDlClient_GetGainLL();
        
        
         void UT_CMccUlDlClient_SetGainLL();
        
        
         void UT_CMccUlDlClient_MaxGainLL();
        
        
         void UT_CMccUlDlClient_SetVolumeLL();
        
        
         void UT_CMccUlDlClient_MaxVolumeLL();
        
        
         void UT_CMccUlDlClient_GetCodecLL();
        
        
         void UT_CMccUlDlClient_GetFmtpAttrLL();
        
        
         void UT_CMccUlDlClient_SetPriorityLL();
        
        
         void UT_CMccUlDlClient_OpenLL();
        
        
         void UT_CMccUlDlClient_PrepareLL();
        
        
         void UT_CMccUlDlClient_PlayLL();
        
        
         void UT_CMccUlDlClient_PauseLL();
        
        
         void UT_CMccUlDlClient_StopLL();
        
        
         void UT_CMccUlDlClient_ResumeLL();
        
        
         void UT_CMccUlDlClient_StreamsExistsLL();
        
        
         void UT_CMccUlDlClient_GetPlayBalanceLL();
        
        
         void UT_CMccUlDlClient_GetRecordBalanceLL();
        
        
         void UT_CMccUlDlClient_SetRecordBalanceLL();
        
        
         void UT_CMccUlDlClient_SetPlayBalanceLL();
        
        
         void UT_CMccUlDlClient_CloseLL();
        
        
         void UT_CMccUlDlClient_GetSessionIdL();
        
        
         void UT_CMccUlDlClient_SetRemoteAddressLL();
         
         void UT_CMccUlDlClient_SetRemoteRtcpAddrLL(  );
        
        
         void UT_CMccUlDlClient_CreateLinkLL();
        
        
         void UT_CMccUlDlClient_InitializeLinkLL();
        
        
         void UT_CMccUlDlClient_CreateRtpSessionLL();
        
        
         void UT_CMccUlDlClient_CloseLinkLL();
        
        
         void UT_CMccUlDlClient_SendMediaSignalLL();
        
        
         void UT_CMccUlDlClient_SetCodecInformationLL();
        
        
         void UT_CMccUlDlClient_GetSupportedBitratesLL();
        
        
         void UT_CMccUlDlClient_GetSSRCLL();
        
        
         void UT_CMccUlDlClient_SendRTCPReceiverReportLL();
        
        
         void UT_CMccUlDlClient_SendRTCPSenderReportLL();
        
        
         void UT_CMccUlDlClient_SendRTCPDataLL();
        
        
         void UT_CMccUlDlClient_StartInactivityTimerLL();
        
        
         void UT_CMccUlDlClient_StopInactivityTimerLL();
         
         void UT_CMccUlDlClient_AudioRoutingLL();
        
        
         void UT_CMccUlDlClient_SetParameterLL();
         void UT_CMccUlDlClient_GenerateStreamIdL();
        
        
         void UT_CMccUlDlClient_UnuseLL();
        
        
         void UT_CMccUlDlClient_ResolveIpAddressesLL();
        
        
         void UT_CMccUlDlClient_StunBindingKeepAliveLL();
        
        
         void UT_CMccUlDlClient_SetResolveStateL();
        
        
         void UT_CMccUlDlClient_BindContextIntoStreamLL();
        
        
         void UT_CMccUlDlClient_RemoveContextLL();
        
        
         void UT_CMccUlDlClient_SendMccEventToClientL();
        
        
         void UT_CMccUlDlClient_StateChangeL();
        
        
         void UT_CMccUlDlClient_ErrorOccuredL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccTestEventHandler* iHandler;
        CMccUlDlClient*       iClient;
        TUint32               iLinkId;
        TUint32               iStreamId;
        MDataSource*          iSourceStub;
        MDataSink*            iSinkStub;
        
        CMccResourcePoolStub* iResources;

    };

#endif      //  __UT_CMCCULDLCLIENT_H__

// End of file
