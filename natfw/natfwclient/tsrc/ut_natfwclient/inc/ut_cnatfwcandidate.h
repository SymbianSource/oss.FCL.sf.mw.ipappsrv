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




#ifndef __UT_CNATFWCANDIDATE_H__
#define __UT_CNATFWCANDIDATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
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
NONSHARABLE_CLASS( UT_CNATFWCandidate )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWCandidate* NewL();
        static UT_CNATFWCandidate* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWCandidate();

    private:    // Constructors and destructors

        UT_CNATFWCandidate();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();

         void Teardown();
         
        
         void UT_CNATFWCandidate_NewLL();
         

         void UT_CNATFWCandidate_SetSessionIdL();

        
         void UT_CNATFWCandidate_SetStreamCollectionIdL();


         void UT_CNATFWCandidate_SetStreamIdL();

        
         void UT_CNATFWCandidate_SetPriorityL();
       
        
         void UT_CNATFWCandidate_SetTransportAddrLL();


         void UT_CNATFWCandidate_SetTransportDomainAddrLL();

        
         void UT_CNATFWCandidate_SetTransportProtocolL();
        
        
         void UT_CNATFWCandidate_BaseL();
        
        
         void UT_CNATFWCandidate_SetBaseL();
        
        
         void UT_CNATFWCandidate_SetFoundationLL();

        
         void UT_CNATFWCandidate_SetCandidateTypeL();

         
         void UT_CNATFWCandidate_SetComponentIdL();
        
        
         void UT_CNATFWCandidate_CompareFoundationsL();
        
        
         void UT_CNATFWCandidate_PriorityOrderL();
         
         
         void UT_CNATFWCandidate_CopyLL();

    private:    // Data

        CNATFWCandidate* iCandidate;

        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_CNATFWCANDIDATE_H__

// End of file
