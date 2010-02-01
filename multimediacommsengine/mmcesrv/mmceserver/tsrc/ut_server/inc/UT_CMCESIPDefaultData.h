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




#ifndef __UT_CMceSipDEFAULTDATA_H__
#define __UT_CMceSipDEFAULTDATA_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  FORWARD DECLARATIONS
class CMceSipDefaultData;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSipDefaultData
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSipDefaultData* NewL();
        static UT_CMceSipDefaultData* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSipDefaultData();
        
    private:    // Constructors and destructors

        UT_CMceSipDefaultData();
        void ConstructL();

    private:    // New methods

        void SetupL();

        void Teardown();
         
        void UT_CMceSipDefaultData_NewLL();
        
        void UT_CMceSipDefaultData_NewL_1L();
    
        void UT_CMceSipDefaultData_SetHeadersL();
        
        void UT_CMceSipDefaultData_SetHeaders_1L();
    
        void UT_CMceSipDefaultData_DefaultHeadersL();
    
        void UT_CMceSipDefaultData_MatchL();
    
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
        CMceSipDefaultData* iDefaultData;
	    
	    RPointerArray< CMceSipDefaultData > iDataArray;
	    
	    TInt iIndex;
	    
		
    };

#endif      //  __UT_CMceSipDEFAULTDATA_H__

// End of file
