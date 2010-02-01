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




#ifndef __UT_CNATFWIDENTIFICATION_H__
#define __UT_CNATFWIDENTIFICATION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNATFWCredentials;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CNATFWIdentification )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWIdentification* NewL();
        static UT_CNATFWIdentification* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWIdentification();

    private:    // Constructors and destructors

        UT_CNATFWIdentification();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Teardown();
        
         void UT_CNATFWIdentification_NewLL();
        
         
         void UT_CNATFWIdentification_SetStreamCollectionIdL();

         
         void UT_CNATFWIdentification_SetStreamIdL();
       
         
         void UT_CNATFWIdentification_SetDirectionL();
        
        
         void UT_CNATFWIdentification_SetUsernameLL();
        
        
         void UT_CNATFWIdentification_SetPasswordLL();
        
        
         void UT_CNATFWIdentification_CopyLL();
        
        
         void UT_CNATFWIdentification_CompareL();
        

         void UT_CNATFWIdentification_operatorsL();
       

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CNATFWCredentials* iIdentifi;
        
    };

#endif      //  __UT_CNATFWIDENTIFICATION_H__

// End of file
