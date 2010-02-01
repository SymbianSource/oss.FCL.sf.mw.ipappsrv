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




#ifndef __UT_CMccRTPMANAGER_H__
#define __UT_CMccRTPMANAGER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccRtpManager;
class CMccTestEventHandler;
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
NONSHARABLE_CLASS( UT_CMccRtpManager )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRtpManager* NewL();
        static UT_CMccRtpManager* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccRtpManager();

    private:    // Constructors and destructors

        UT_CMccRtpManager();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccRtpManager_RemoveStreamFromArray();
        
         void UT_CMccRtpManager_IsOpen();
         
         void UT_CMccRtpManager_CheckStreamExists();
         
         void UT_CMccRtpManager_CreateSessionLL();
        
        
         void UT_CMccRtpManager_CloseSessionL();
        
        
         void UT_CMccRtpManager_GetRtpReferenceLL();
        
        
         void UT_CMccRtpManager_SetRemoteAddressL();
        
         void UT_CMccRtpManager_SetRemoteRtcpAddrL();
        
         void UT_CMccRtpManager_SetIpTOSL();
        
        
         void UT_CMccRtpManager_StartSessionLL();
        
        
         void UT_CMccRtpManager_InitializeLL();
        
        
         void UT_CMccRtpManager_SessionIdL();
        
        
         void UT_CMccRtpManager_CreateTransmitStreamLL();
        
        
         void UT_CMccRtpManager_CreateReceiveStreamLL();
        
        
         void UT_CMccRtpManager_RefreshDownlinkLL();
        
        
         void UT_CMccRtpManager_SetRTCPSendL();
        
        
         void UT_CMccRtpManager_ErrorNotifyL();
        
         void UT_CMccRtpManager_GetLocalIpAddressesL();
         
         void UT_CMccRtpManager_SetCNameL();
         
         void UT_CMccRtpManager_QoSParamEvent();
         
         void UT_CMccRtpManager_QoSConnEvent();
         
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        CMccTestEventHandler* iHandler;
        CMccRtpManager* iRtpMan;
        CMccResourcePoolStub* iResources;
        TUint32 iMccSessionId;
    };

#endif      //  __UT_CMccRTPMANAGER_H__

// End of file
