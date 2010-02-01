/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_TMCESRVSTREAMITERATOR_H__
#define __UT_TMCESRVSTREAMITERATOR_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_TMceSrvStreamIterator : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMceSrvStreamIterator* NewL();
        static UT_TMceSrvStreamIterator* NewLC();
        /**
         * Destructor
         */
        ~UT_TMceSrvStreamIterator();

    private:    // Constructors and destructors

        UT_TMceSrvStreamIterator();
        void ConstructL();



    private:    // New methods

         void SetupL();
        
         void Teardown();
        
        /**
        * Basic
        */
        void UT_TMceSrvStreamIterator_BasicL();
        
        /**
        * Tests
        *
        * CMceSrvStream* Next( TStreamStateMatchType aMatchType = ExactMatch );
        *
        */
        void UT_TMceSrvStreamIterator_Next_1L();


        /**
        * Tests
        *
        * CMceSrvStream* Next( TMceMccComEvent& aRecipient,
        *                     TStreamStateMatchType aMatchType = ExactMatch );
        *
        */
        void UT_TMceSrvStreamIterator_Next_2L();

        /**
        * Tests
        *
        * CMceSrvStream* Next( CMceSrvStream& aClone,
        *                     TStreamStateMatchType aMatchType = ExactMatch );
        *
        */
        void UT_TMceSrvStreamIterator_Next_3L();

        /**
        * Tests
        *
        * CMceSrvStream* Next( CMceSrvStream::TState aState, 
        *                     TStreamStateMatchType aMatchType = ExactMatch );
        *
        */
        void UT_TMceSrvStreamIterator_Next_4L();
        

        /**
        * Tests
        *
        * CMceSrvStream* NextIntersect( TMceSrvStreamIterator& aSet,
        *                              CMceSrvStream*& aIntersectCandidate );
        *
        */
        void UT_TMceSrvStreamIterator_NextIntersectL();

        /**
        * Tests
        *
        * CMceSrvStream* NextComplement( TMceSrvStreamIterator& aSet );
        *
        */
        void UT_TMceSrvStreamIterator_NextComplementL();
                                      
        /**
        * Tests
        *
        * void Reset();
        *
        */
        void UT_TMceSrvStreamIterator_ResetL();
        
        /**
        * Tests
        *
        * TInt Current();
        *
        */
        void UT_TMceSrvStreamIterator_CurrentL();
        

        /**
        * Tests
        *
        * TBool IsEof();
        *
        */
        void UT_TMceSrvStreamIterator_IsEofL();
        

    private:    // Data


		CMceMediaManager* iManager;
		CMceComSession* iSession;
		CMceComSession* iUpdate;
		CMceServerStub* iServer;

		EUNIT_DECLARE_TEST_TABLE; 

		
    };

#endif      //  __UT_TMCESRVSTREAMITERATOR_H__

// End of file
