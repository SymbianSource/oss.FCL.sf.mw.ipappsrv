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




#ifndef __UT_CFCSESSION_H__
#define __UT_CFCSESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
//  INTERNAL INCLUDES
#include <e32base.h>
#include <badesca.h>

#include <ecom/ecom.h>
#include <in_sock.h>

#include "fcmessageobserver.h"
#include "fcplugin.h"
#include "fcsession.h"
#include "TestPlugInEngine.h"
//  FORWARD DECLARATIONS
class CFCSession;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CFCSession
     : public CEUnitTestSuiteClass,
     public MFCMessageObserver
     
     
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CFCSession* NewL();
        static UT_CFCSession* NewLC();
        /**
         * Destructor
         */
        ~UT_CFCSession();

    private:    // Constructors and destructors

        UT_CFCSession();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         
         void Teardown();
        
         
         void UT_CFCSession_EmptyTestL();
         
         
         void UT_CFCSession_DestructorL();
         
         
         void UT_CFCSession_NewLL();
        
        
         void UT_CFCSession_NewLCL();
        
         
         void UT_CFCSession_PortL();
        
         
         void UT_CFCSession_IsReceiverL();
         
         
         void UT_CFCSession_NegotiatedFCPlugInL();
        
        
         void UT_CFCSession_FormatAttributeFieldsLL();
        
        
         void UT_CFCSession_SetFormatAttributeFieldsL();
        
        
         void UT_CFCSession_StoreInfoLL();
         
         
         void UT_CFCSession_StoreInfoL2L();
        
        
         void UT_CFCSession_DecodeSDPMediaPortL();
        
        
         void UT_CFCSession_DecodeConnectionAddrL();
        
        
         void UT_CFCSession_UpdateSDPLL();
        
        
         void UT_CFCSession_UpdateSDPL2L();
         
         
         void UT_CFCSession_ReleaseSessionL();
        
        
         void UT_CFCSession_ConnectionReadyLL();
        
        
         void UT_CFCSession_SendToNetLL();
      	 
      	 
		 void UT_CFCSession_IsFmtAttributeValidL();
		 
		 void UT_CFCSession_IsFmtAttributeValid2L();
		 
		 void UT_CFCSession_IsFmtAttributeValid3();
		 
		 void UT_CFCSession_ArrayCleanupL();

    public: //from observer
    
        void ReceivedData(HBufC8* aData, MFCPlugIn* aSession );
      	 
      	void ErrorNotify(TInt aErrCode, MFCPlugIn* aSession );
      	 
		 
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		CFCSession* iFCSession;
		CTestPlugInEngine* iTestPlugInEngine;
		HBufC8* iData;
		TInt iErr;
		
    };

#endif      //  __UT_CFCSESSION_H__

// End of file
