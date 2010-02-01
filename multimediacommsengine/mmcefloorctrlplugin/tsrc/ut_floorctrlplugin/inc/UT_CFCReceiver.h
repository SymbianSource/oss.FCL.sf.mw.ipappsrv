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




#ifndef __UT_CFCRECEIVER_H__
#define __UT_CFCRECEIVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <e32std.h>
#include <e32base.h>
#include <in_sock.h>
#include <CommDbConnPref.h>
//  INTERNAL INCLUDES
#include "fcnotifier.h"
#include "fcconnectionnotifier.h"
#include "fcconnectioncontainer.h"
#include "fcreceiver.h"
#include "fcsender.h"
//  FORWARD DECLARATIONS
class CFCReceiver;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CFCReceiver
     : public CEUnitTestSuiteClass,
       public MFCNotifier,
       public MFCConnectionNotifier
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CFCReceiver* NewL();
        static UT_CFCReceiver* NewLC();
        /**
         * Destructor
         */
        ~UT_CFCReceiver();

    private:    // Constructors and destructors

        UT_CFCReceiver();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CFCReceiver_NewLL();
        
        
         void UT_CFCReceiver_NewLCL();
        
        
         void UT_CFCReceiver_ListenL();
        
        
         void UT_CFCReceiver_ReceivedDataLL();
         void UT_CFCReceiver_RunErrorL(  );
         void UT_CFCReceiver_TestRunLL();
         void UT_CFCReceiver_TestRunL2L(  );
         void UT_CFCReceiver_TestRunL3L(  );
         void UT_CFCReceiver_TestRunL4L(  );
         void ReceivedData(HBufC8* aData);
         void ErrorNotify(TInt aErrCode);
         void ConnectionStarted( TInt aError );
         void ConnectionStartedL( TInt aError );
		 static TInt StopScheduler( TAny* aThis );

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		RSocketServ iSocketServ;
		RSocket iSocket;	
		CFCReceiver* iReceiver;
		TInt iErr;
		TInt iConnectErr;
		CFCConnectionContainer* iConnection;
		HBufC8* iData;
		CActiveSchedulerWait iWait;
		TInt iEventCount;
		TBool iCallBackCalled;
		CPeriodic* iTimer;
		CFCSender* iSender;
    };

#endif      //  __UT_CFCRECEIVER_H__

// End of file
