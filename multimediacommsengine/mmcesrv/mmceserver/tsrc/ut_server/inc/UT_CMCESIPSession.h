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




#ifndef __UT_CMceSipSESSION_H__
#define __UT_CMceSipSESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceSipStateMachine;
class CSIPConnection;
class CSIP;
class CMceServerCore;
class CMceCsSession;
class CMceSipConnection;
class CMceSipManager;
class CSIPProfile;
class CMCETls;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSipSession
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSipSession* NewL();
        static UT_CMceSipSession* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSipSession();

    private:    // Constructors and destructors

        UT_CMceSipSession();
        void ConstructL();


    private:    // New methods

    void SetupL();

    void Teardown();

    void UT_CMceSipSession_NeedToSendL();
    void UT_CMceSipSession_DoesMediaManagerNeedToNegotiateL();
    void UT_CMceSipSession_ServiceLL();
    void UT_CMceSipSession_FCTestsL();
    
    void UT_CMceSipSession_CurrentStateL();
    void UT_CMceSipSession_BodyL();
    void UT_CMceSipSession_ClearRemoteUriL();
    void UT_CMceSipSession_FindRemoteUriL();
    void UT_CMceSipSession_AddRemoteUriL();
    void UT_CMceSipSession_AddRemoteUriFromResponseL();
    void UT_CMceSipSession_CreateDialogFrom3XXResponseL();
    void UT_CMceSipSession_OfferL();
    void UT_CMceSipSession_NextStateL();
    void UT_CMceSipSession_InviteTransactionL();
    
    void UT_CMceSipSession_AcceptMethodL();
    void UT_CMceSipSession_RequestReceivedL();
    void UT_CMceSipSession_RequestReceived2L();
    void UT_CMceSipSession_ForkedResponseReceivedL();
    void UT_CMceSipSession_ResponseReceivedL();
    void UT_CMceSipSession_ProvisionalResponseReceivedL();
    void UT_CMceSipSession_RedirectionResponseReceivedL();
    void UT_CMceSipSession_ErrorResponseReceivedL();
    void UT_CMceSipSession_StandAloneRequestReceivedL();
    void UT_CMceSipSession_StandAloneResponseReceivedL();
    
    void UT_CMceSipSession_DoInitializeIncomingDialogLL();
    void UT_CMceSipSession_ActionsL();
    void UT_CMceSipSession_OnExpiredL();
    void UT_CMceSipSession_CanDisposeL();
    void UT_CMceSipSession_ConnectionStateChangedL();
    void UT_CMceSipSession_CreateOutgoingDialogL();
    void UT_CMceSipSession_ReCreateOutgoingDialogL();

    void UT_CMceSipSession_SessionClosedL();
    void UT_CMceSipSession_EventReceivedL();
    
    void UT_CMceSipSession_MediaErrorL();
    void UT_CMceSipSession_TerminateL();
    void UT_CMceSipSession_UseFCL();
    void UT_CMceSipSession_FCReceivedDataL();
    void UT_CMceSipSession_FCErrorNotifyL();
    void UT_CMceSipSession_InviteForkedLL();
    void UT_CMceSipSession_HandleForkedDialogsL();
    void UT_CMceSipSession_WaitingMediaCallbackL();
    void UT_CMceSipSession_SetWaitingMediaCallbackL();
    
    void UT_CMceSipSession_ReliableFailedL();
    void UT_CMceSipSession_NoPrackReceivedL();

    void UT_CMceSipSession_UpdateMediaLL();
    void UT_CMceSipSession_ActiveBodyL();
        
    void UT_CMceSipSession_CanceledL();
    
    void UT_CMceSipSession_ForceSDPSending();
        
    void UT_CMceSipSession_IsProfileContactSecureLL();
    
    void UT_CMceSipSession_ErrorOccuredL();
    
    void UT_CMceSipSession_DoErrorOccured();
    
    void UT_CMceSipSession_ConsumesL();
    
    void UT_CMceSipSession_MethodCodeL();
    
    void UT_CMceSipSession_IsSipDialogConfirmedL();
    
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
	TMceIds iIds;
    CMceServerCore* iServerCore;
    CMceCsSession* iSession;
    CMceSipSession* iSipSession;
       
    private: //not Owned
    	    
    CMCETls* iStorage;
    
		
    };

#endif      //  __UT_CMceSipSESSION_H__

// End of file
