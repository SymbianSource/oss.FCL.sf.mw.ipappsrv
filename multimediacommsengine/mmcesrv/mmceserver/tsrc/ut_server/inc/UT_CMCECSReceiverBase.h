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




#ifndef __UT_CMCECSRECEIVERBASE_H__
#define __UT_CMCECSRECEIVERBASE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceCsReceiverBase;
class CMceCsServerITC;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceCsReceiverBase
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceCsReceiverBase* NewL();
        static UT_CMceCsReceiverBase* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceCsReceiverBase();

    private:    // Constructors and destructors

        UT_CMceCsReceiverBase();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMceCsReceiverBase_SendToClientLL();
        
        
         void UT_CMceCsReceiverBase_SendToClientL_1L();
        
        
         void UT_CMceCsReceiverBase_SendToClientL_2L();
        
        
         void UT_CMceCsReceiverBase_ClientReadyToReceiveLL();
        
        
         void UT_CMceCsReceiverBase_CancelClientReceiveLL();
        
        
         void UT_CMceCsReceiverBase_IdsL();
        
        
         void UT_CMceCsReceiverBase_ReceiveLL();
        

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		 CMceCsReceiverBase* iReceiver;
    	CMceCsServerITC*    iITC;
		
    };

#endif      //  __UT_CMCECSRECEIVERBASE_H__

// End of file
