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




#ifndef __UT_CMCEDICTIONARY_H__
#define __UT_CMCEDICTIONARY_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CMceDictionary;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceDictionary
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceDictionary* NewL();
        static UT_CMceDictionary* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceDictionary();
        
    private:    // Constructors and destructors

        UT_CMceDictionary();
        void ConstructL();

    private:    // New methods

        void SetupL();

        void Teardown();
        
        void UT_CMceDictionaryItem_KeyL();
        
        void UT_CMceKeyTValuePair_ValueL();
        
        void UT_CMceKeyTValuePair_SetValueL();
        
        void UT_CMceKeyCValuePair_ValueL();
        
        void UT_CMceKeyCValuePair_SetValueL();
        
        void UT_CMceDictionary_PairL();
        
        void UT_CMceDictionary_AddLL();
       
        void UT_CMceDictionary_DeleteL();
         
         
    private:    // Data

        CMceDictionary* iDictionary;
        
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMCEDICTIONARY_H__

// End of file
