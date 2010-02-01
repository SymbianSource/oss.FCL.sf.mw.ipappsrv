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




#ifndef __UT_CICECALLBACKEXECUTER_H__
#define __UT_CICECALLBACKEXECUTER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CIceCallBackExecuter;

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
NONSHARABLE_CLASS( UT_CIceCallBackExecuter )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceCallBackExecuter* NewL();
        static UT_CIceCallBackExecuter* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceCallBackExecuter();

        static void HandleCallBack( CBase& aObject, TInt aCallBackType );
        
        void DoHandleCallBack( TInt aCallBackType );

    private:    // Constructors and destructors

        UT_CIceCallBackExecuter();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CICECallBackExecuter_AddCallBackLL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CIceCallBackExecuter* iExecuter;
    };

#endif      //  __UT_CICECALLBACKEXECUTER_H__

// End of file
