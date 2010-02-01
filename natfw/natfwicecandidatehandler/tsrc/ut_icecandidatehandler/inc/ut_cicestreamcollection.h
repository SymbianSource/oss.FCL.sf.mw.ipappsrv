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




#ifndef __UT_CNATFWSTREAMCOLLECTION_H__
#define __UT_CNATFWSTREAMCOLLECTION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CIceStreamCollection;

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
NONSHARABLE_CLASS( UT_CIceStreamCollection )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceStreamCollection* NewL();
        static UT_CIceStreamCollection* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceStreamCollection();

    private:    // Constructors and destructors

        UT_CIceStreamCollection();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Teardown();
         
         void UT_CNATFWStreamCollection_StreamCollectionIdL();
         
         void UT_CNATFWStreamCollection_AddMediaComponentLL();
         
         void UT_CNATFWStreamCollection_RemoveMediaComponentL();
         
         void UT_CNATFWStreamCollection_HasMediaComponentL();
         
         void UT_CNATFWStreamCollection_NumOfMediaComponentsL();
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CIceStreamCollection* iCollection;

    };

#endif      //  __UT_CNATFWSTREAMCOLLECTION_H__

// End of file
