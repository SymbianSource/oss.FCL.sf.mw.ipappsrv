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




#ifndef __UT_CMCCCRYPTOCONTEXTCONTAINER_H__
#define __UT_CMCCCRYPTOCONTEXTCONTAINER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccCryptoContextContainer;

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
NONSHARABLE_CLASS( UT_CMccCryptoContextContainer )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCryptoContextContainer* NewL();
        static UT_CMccCryptoContextContainer* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCryptoContextContainer();

    private:    // Constructors and destructors

        UT_CMccCryptoContextContainer();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCryptoContextContainer_NewLL();
        
        
         void UT_CMccCryptoContextContainer_CreateContextL();
        
        
         void UT_CMccCryptoContextContainer_RemoveContextL();
        
        
         void UT_CMccCryptoContextContainer_UpdateContextL();
        
        
         void UT_CMccCryptoContextContainer_GetContextL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccCryptoContextContainer* iContextContainer;
    };

#endif      //  __UT_CMCCCRYPTOCONTEXTCONTAINER_H__

// End of file
