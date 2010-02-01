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




#ifndef __UT_CMCCVIDEOSOURCEKEYRETRIEVER_H__
#define __UT_CMCCVIDEOSOURCEKEYRETRIEVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccVideoSourceConfigKeyRetriever;
class CMccCameraHandlerStub;

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
NONSHARABLE_CLASS( UT_CMccVideoSourceKeyRetriever )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccVideoSourceKeyRetriever* NewL();
        static UT_CMccVideoSourceKeyRetriever* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccVideoSourceKeyRetriever();

    private:    // Constructors and destructors

        UT_CMccVideoSourceKeyRetriever();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccVideoSourceKeyRetriever_RetrieveL();
         
         void UT_CMccVideoSourceKeyRetriever_SendEventToClientTestL();
         
         void UT_CMccVideoSourceKeyRetriever_DummyTestL();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccVideoSourceConfigKeyRetriever* iRetriever;

        CMccCameraHandlerStub* iCameraHandler;


    };

#endif      //  __UT_CMCCVIDEOSOURCEKEYRETRIEVER_H__

// End of file
