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




#ifndef __UT_CTBCPFLOORCONTROLIMPL_H__
#define __UT_CTBCPFLOORCONTROLIMPL_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "fcmessageobserver.h"
#include "fcinterfaceinitparams.h"
//  FORWARD DECLARATIONS
class CTBCPFloorControlImpl;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CTBCPFloorControlImpl
     : public CEUnitTestSuiteClass,
     public MFCMessageObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CTBCPFloorControlImpl* NewL();
        static UT_CTBCPFloorControlImpl* NewLC();
        /**
         * Destructor
         */
        ~UT_CTBCPFloorControlImpl();

   	public: 	// From observer interface
		
		
		 void ReceivedData(HBufC8* aData, MFCPlugIn* aPlugIn) ;
		 void ErrorNotify(TInt aErrCode, MFCPlugIn* aPlugIn);


    private:    // Constructors and destructors

        UT_CTBCPFloorControlImpl();
        void ConstructL();

		 
    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CTBCPFloorControlImpl_NewLL();
         
         
         void UT_CTBCPFloorControlImpl_DestructorL();
        
        
         void UT_CTBCPFloorControlImpl_NameL();
        
        
         void UT_CTBCPFloorControlImpl_SendToNetLL();
         
         void UT_CTBCPFloorControlImpl_SendToNetL2L( );
        
         void UT_CTBCPFloorControlImpl_ConnectLL();
        
        
         void UT_CTBCPFloorControlImpl_SetIapIdL();
        
         void UT_CTBCPFloorControlImpl_SetAddressLL();
        
        
         void UT_CTBCPFloorControlImpl_SetLocalPortL();
        
        
         void UT_CTBCPFloorControlImpl_StopL();
         
         
         void UT_CTBCPFloorControlImpl_Stop2L();
        
        
         void UT_CTBCPFloorControlImpl_StartListenL();
        
        
         void UT_CTBCPFloorControlImpl_ResetSocketLL();
        
        
         void UT_CTBCPFloorControlImpl_CancelSendL();
        
         void UT_CTBCPFloorControlImpl_ConnectionStartedL();
        
         void UT_CTBCPFloorControlImpl_ConnectionStartedLL();
         
         void UT_CTBCPFloorControlImpl_ConnectionStarted2LL( );
        
         void UT_CTBCPFloorControlImpl_ConnectionStartedL3L( );
         void UT_CTBCPFloorControlImpl_ErrorNotifyL();
        
        
         void UT_CTBCPFloorControlImpl_ReceivedDataL();
        
        
         void UT_CTBCPFloorControlImpl_SenderL();
        
        
         void UT_CTBCPFloorControlImpl_GetSocketL();
        
		 static TInt StopScheduler( TAny* aThis );

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		CTBCPFloorControlImpl* iPlugIn;
		MFCMessageObserver* iObserver;
		TInt iError;
		HBufC8* iData;
		CActiveSchedulerWait iWait;
		TInt iEventCount;
		TBool iCallBackCalled;
		CPeriodic* iTimer;
    };

#endif      //  __UT_CTBCPFLOORCONTROLIMPL_H__

// End of file
