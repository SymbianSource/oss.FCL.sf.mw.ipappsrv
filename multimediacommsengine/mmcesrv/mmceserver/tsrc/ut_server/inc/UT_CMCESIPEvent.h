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




#ifndef __UT_CMceSipEVENT_H__
#define __UT_CMceSipEVENT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipEvent;
class CMceServerCore;
class CMceCsSession;
class CMCETls;
class CMceSipSession;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSipEvent
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSipEvent* NewL();
        static UT_CMceSipEvent* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSipEvent();

    private:    // Constructors and destructors

        UT_CMceSipEvent();
        void ConstructL();


    private:    // New methods

    void SetupL();
    void SetupMoSubscribeL();
    void SetupMtSubscribeL();
    void SetupMoReferL();
    void SetupMtReferL();
    void Teardown();

    void UT_CMceSipEvent_NewLL();
    void UT_CMceSipEvent_DoServiceLL();
    void UT_CMceSipEvent_DoServiceL2L();
    void UT_CMceSipEvent_DoServiceL3L();
    void UT_CMceSipEvent_DoServiceL4L();
    void UT_CMceSipEvent_DoServiceL5L();
    void UT_CMceSipEvent_DoServiceL6L();
    void UT_CMceSipEvent_AcceptMethodL();
    void UT_CMceSipEvent_RequestReceivedL();
    void UT_CMceSipEvent_ResponseReceivedL();
    void UT_CMceSipEvent_ProvisionalResponseReceivedL();
    void UT_CMceSipEvent_RedirectionResponseReceivedL();
    void UT_CMceSipEvent_ErrorResponseReceivedL();
    void UT_CMceSipEvent_StandAloneRequestReceivedL();
    void UT_CMceSipEvent_StandAloneResponseReceivedL();
    void UT_CMceSipEvent_DoInitializeIncomingDialogLL();
    void UT_CMceSipEvent_CanDisposeL();
    void UT_CMceSipEvent_ConnectionStateChangedL();
    void UT_CMceSipEvent_CreateOutgoingDialogLL();
    void UT_CMceSipEvent_CreateOutgoingDialogL2L();
    void UT_CMceSipEvent_CreateIncomingDialogLL();
    void UT_CMceSipEvent_AcceptDialogTransactionL();
    void UT_CMceSipEvent_AcceptDialogTransaction2L();
    void UT_CMceSipEvent_AcceptDialogTransaction3L();
    void UT_CMceSipEvent_CanceledL();
    void UT_CMceSipEvent_IncomingReferLL();
    void UT_CMceSipEvent_IncomingSubscribeLL();
    void UT_CMceSipEvent_NotifyReceivedLL();
    void UT_CMceSipEvent_ClientStateChangedLL();
    void UT_CMceSipEvent_IdleL();

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
	TMceIds iIds;
    CMceServerCore* iServerCore;
    CMceCsSession* iSession;
    CMceSipEvent* iSipEvent;
    CMceSipSession* iSipSession;
    
    private: //not Owned
    	    
    CMCETls* iStorage;
    
		
    };

#endif      //  __UT_CMceSipEVENT_H__

// End of file
