/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __UT_MCESIP_H__
#define __UT_MCESIP_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceCsSession;
class CMceServerCore;
class CMCETls;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_MCESIP
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_MCESIP* NewL();
        static UT_MCESIP* NewLC();
        /**
         * Destructor
         */
        ~UT_MCESIP();

    private:    // Constructors and destructors

        UT_MCESIP();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_MCESIP_ReasonPhraseLL();
         void UT_MCESIP_TrxCompletedL();
         void UT_MCESIP_WarningPhraseLL();
         void UT_MCESIP_IsSIPStackErrorL();
         void UT_MCESIP_MethodL();
         void UT_MCESIP_ToContentTypeHeaderLL();
         void UT_MCESIP_UserHeadersToTextArrayLL();
         void UT_MCESIP_RandomL();
         void UT_MCESIP_DiscardInviteL();
         void UT_MCESIP_CompareUriL();
         void UT_MCESIP_RSeq();
           
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
    	
    private: //not Owned
    	    
        CMCETls* iStorage;
    	
    };

#endif      //  __UT_MCESIP_H__

// End of file
