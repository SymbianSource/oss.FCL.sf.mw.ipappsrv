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




#ifndef __UT_CFCSENDER_H__
#define __UT_CFCSENDER_H__

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

//  FORWARD DECLARATIONS
class CFCSender;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CFCSender
     : public CEUnitTestSuiteClass,
       public MFCNotifier,
       public MFCConnectionNotifier
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CFCSender* NewL();
        static UT_CFCSender* NewLC();
        /**
         * Destructor
         */
        ~UT_CFCSender();

    private:    // Constructors and destructors

        UT_CFCSender();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CFCSender_NewLL();
        
        
         void UT_CFCSender_SendLL();
         void UT_CFCSender_SendL2L();
         void UT_CFCSender_SendL3L();
         void UT_CFCSender_SendL4L();
         void UT_CFCSender_RunErrorL();
         void UT_CFCSender_DoCancelL();
  		 void UT_CFCSender_TestRunLL();
  		 void UT_CFCSender_TestRunL2L();
  		 void ReceivedData(HBufC8* aData);
		 void ErrorNotify(TInt aErrCode);
		 void ConnectionStarted( TInt aError );
		 void ConnectionStartedL( TInt aError );
		 static TInt StopScheduler( TAny* aThis );
		 
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		RSocketServ iSocketServ;
		RSocket iSocket;	
		CFCSender* iSender;
		TInt iErr;
		
		CFCConnectionContainer* iConnection;
		HBufC8* iData;
		CActiveSchedulerWait iWait;
		TInt iEventCount;
		TBool iCallBackCalled;
		CPeriodic* iTimer;
		TBool iSimplifiedConnect;
		
    };

#endif      //  __UT_CFCSENDER_H__

// End of file
