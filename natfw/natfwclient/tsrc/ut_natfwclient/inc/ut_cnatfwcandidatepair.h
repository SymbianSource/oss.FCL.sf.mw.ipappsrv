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




#ifndef __UT_CNATFWCANDIDATEPAIR_H__
#define __UT_CNATFWCANDIDATEPAIR_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CNATFWCandidatePair;
class CNATFWCandidate;

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
NONSHARABLE_CLASS( UT_CNATFWCandidatePair )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWCandidatePair* NewL();
        static UT_CNATFWCandidatePair* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CNATFWCandidatePair();

    private:    // Constructors and destructors

        UT_CNATFWCandidatePair();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void Setup2L();
 
         void Teardown();
        
         void UT_CNATFWCandidatePair_NewLL();
        
        
         void UT_CNATFWCandidatePair_NewL_1L();

        
         void UT_CNATFWCandidatePair_SetLocalCandidateLL();


         void UT_CNATFWCandidatePair_SetRemoteCandidateLL();

        
         void UT_CNATFWCandidatePair_SetPriorityL();

        
         void UT_CNATFWCandidatePair_CopyLL();
        
        
         void UT_CNATFWCandidatePair_SetSelectedL();
        
        
         void UT_CNATFWCandidatePair_SelectedL();
        
        
         void UT_CNATFWCandidatePair_PriorityOrderL();
        
        
         void UT_CNATFWCandidatePair_CompareL();
        
         
         void UT_CNATFWCandidatePair_MatchAddressesL();
         
         
         void UT_CNATFWCandidatePair_operatorsL();


    private:    // Data

        CNATFWCandidatePair* iCandPair;
        CNATFWCandidate* iCandidate;
        
        EUNIT_DECLARE_TEST_TABLE; 
    };

#endif      //  __UT_CNATFWCANDIDATEPAIR_H__

// End of file
