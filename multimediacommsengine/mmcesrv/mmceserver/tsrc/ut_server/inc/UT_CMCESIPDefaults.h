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




#ifndef __UT_CMceSipDEFAULTS_H__
#define __UT_CMceSipDEFAULTS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipDefaults;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSipDefaults
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceSipDefaults* NewL();
        static UT_CMceSipDefaults* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceSipDefaults();
        
    private:    // Constructors and destructors

        UT_CMceSipDefaults();
        void ConstructL();

    private:    // New methods

        void SetupL();

        void Teardown();
        
        void UT_CMceSipDefaults_StoreDefaultHeadersLL();
        
        void UT_CMceSipDefaults_StoreDefaultHeadersL2L();
        
        void UT_CMceSipDefaults_DefaultHeadersL();
         
    private:    // Data

        CMceSipDefaults* iDefaults;
        
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMceSipDEFAULTS_H__

// End of file
