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




#ifndef __UT_CFCCONNECTIONCONTAINER_H__
#define __UT_CFCCONNECTIONCONTAINER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <in_sock.h>
#include "fcconnectionnotifier.h"
//  FORWARD DECLARATIONS
class CFCConnectionContainer;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CFCConnectionContainer
     : public CEUnitTestSuiteClass,
     public MFCConnectionNotifier
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CFCConnectionContainer* NewL();
        static UT_CFCConnectionContainer* NewLC();
        /**
         * Destructor
         */
        ~UT_CFCConnectionContainer();

    private:    // Constructors and destructors

        UT_CFCConnectionContainer();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CFCConnectionContainer_NewLL();
        
        
         void UT_CFCConnectionContainer_ConnectionL();
        
        
         void UT_CFCConnectionContainer_OpenConnectionL();
         
         void UT_CFCConnectionContainer_CloseConnectionL();
         
         void UT_CFCConnectionContainer_TestRunLL(  );
         void UT_CFCConnectionContainer_TestDoCancelL();
        
		void ConnectionStarted( TInt aError );
         

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		RSocketServ iSocketServ;
		RSocket iSocket;
		CFCConnectionContainer* iConnection;
		TInt iErr;
		
    };

#endif      //  __UT_CFCCONNECTIONCONTAINER_H__

// End of file
