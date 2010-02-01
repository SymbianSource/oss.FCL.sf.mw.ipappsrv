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




#ifndef __UT_CICEValidList_H__
#define __UT_CICEValidList_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CIceValidList;

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
NONSHARABLE_CLASS( UT_CIceValidList )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CIceValidList* NewL();
        static UT_CIceValidList* NewLC();
        /**
         * Destructor
         */
        ~UT_CIceValidList();

    private:    // Constructors and destructors

        UT_CIceValidList();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CICEValidList_StreamCollectionIdL();
        
         void UT_CICEValidList_AddValidPairLL();
         
         void UT_CICEValidList_HasPairL();
         
         void UT_CICEValidList_HasPairForComponentsL();
         
         void UT_CICEValidList_SetPairNominatedLL();
         
         void UT_CICEValidList_NominatedPairsExistL();

         void UT_CICEValidList_SelectedPairL();
              
         void UT_CICEValidList_HighestPriorityPairL();
         
         void UT_CICEValidList_ValidListL();
        
         void UT_CICEValidList_FlushValidListL();
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        CIceValidList* iValidList;

    };

#endif      //  __UT_CICEValidList_H__

// End of file
