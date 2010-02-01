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




#ifndef __UT_CFCPLUGINENGINE_H__
#define __UT_CFCPLUGINENGINE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "fcobserver.h"
#include "fcsession.h"
//  FORWARD DECLARATIONS
class CFCPlugInEngine;
class MFCObserver;
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CFCPlugInEngine
     : public CEUnitTestSuiteClass,
     public MFCObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CFCPlugInEngine* NewL();
        static UT_CFCPlugInEngine* NewLC();
        /**
         * Destructor
         */
        ~UT_CFCPlugInEngine();

    private:    // Constructors and destructors

        UT_CFCPlugInEngine();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         
         void Teardown();
        
         
         void UT_CFCPlugInEngine_NewLL();
        
        
         void UT_CFCPlugInEngine_NewLCL();
        
        
         void UT_CFCPlugInEngine_FCPlugInIterL();
        
        
         void UT_CFCPlugInEngine_ListMechanismImplementationsLL();
        
        
         void UT_CFCPlugInEngine_SupportedProtocolLL();
        
        
         void UT_CFCPlugInEngine_PlugInByNameLL();
        
        
         void UT_CFCPlugInEngine_CreateNewSessionLL();
        
        
         void UT_CFCPlugInEngine_ReleaseSessionL();
        
        
         void UT_CFCPlugInEngine_FormatAttributeFieldsL();
        
        
         void UT_CFCPlugInEngine_SetFormatAttributeFieldsLL();
        
        
         void UT_CFCPlugInEngine_PeekDocumentLL();
        
        
         void UT_CFCPlugInEngine_UpdateOfferLL();
        
        
         void UT_CFCPlugInEngine_UpdateAnswerLL();
        
        
         void UT_CFCPlugInEngine_StartConnectionLL();
        
         
         void UT_CFCPlugInEngine_IsReceiverLL();
        
         
         void UT_CFCPlugInEngine_SendToNetLL();
         
         
         void UT_CFCPlugInEngine_UpdateSessionLL();
     
         
         void UT_CFCPlugInEngine_SetSessionPortLL();
         
          
         void UT_CFCPlugInEngine_ReceivedDataL();
        
        
         void UT_CFCPlugInEngine_ErrorNotifyL();
         
         
         void UT_CFCPlugInEngine_ArrayCleanupL();
         
         
         void FCReceivedData(HBufC8* aData, const CFCSession* aFCSession);
         
       	 
       	 void FCErrorNotify(TInt aErrCode, const CFCSession* aFCSession) ;


    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		TInt iErr;
		HBufC8* iData;
		CFCSession* iSession;
		CFCPlugInEngine*	iEngine;
    };

#endif      //  __UT_CFCPLUGINENGINE_H__

// End of file
