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




#ifndef __UT_CMCECSSESSION_H__
#define __UT_CMCECSSESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceCsSession;
class CMceServerCore;
class CMCETls;
class CMceComSession;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceCsSession
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceCsSession* NewL();
        static UT_CMceCsSession* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceCsSession();

    private:    // Constructors and destructors

        UT_CMceCsSession();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMceCsSession_TestOpenSubSessionL();
                
        
         void UT_CMceCsSession_ServiceLL();
                
        
         void UT_CMceCsSession_UidL();
        
        
         void UT_CMceCsSession_TestCancelClientReceiveL();
        
        
         void UT_CMceCsSession_SubSessionsL();
        
         void UT_CMceCsSession_SendToClientLL();
        
         void UT_CMceCsSession_StoreDefaultHeadersLL();

         void UT_CMceCsSession_DefaultHeadersLL();
    
    private: // Helpers
         
         void SetTranscodingItcParams( CMceComSession* aSession );
    
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		CMceServerCore* iServerCore;
    	CMceCsSession* iSession;
    	
    private: //not Owned
    	    
	    CMCETls* iStorage;
    	
		
    };

#endif      //  __UT_CMCECSSESSION_H__

// End of file
