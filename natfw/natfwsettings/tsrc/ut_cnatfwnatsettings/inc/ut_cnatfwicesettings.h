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




#ifndef __UT_CNATFWICESETTINGS_H__
#define __UT_CNATFWICESETTINGS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitDecorators.h>


//  INTERNAL INCLUDES
#include "cnatfwicesettings.h"

//  FORWARD DECLARATIONS
class CSettingStore;

//  CLASS DEFINITION
/**
 * TODO Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( UT_CNATFWIceSettings )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWIceSettings* NewL();
        static UT_CNATFWIceSettings* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWIceSettings();

    private:    // Constructors and destructors

        UT_CNATFWIceSettings();
        void ConstructL();

    private:    // New methods

         void SetupL();
        
         void Teardown();
         
         void SetupCenRepL();
         
         void TeardownCenRepL();
         
         void T_CNATFWIceSettings_SetPreferencesL();
        
         void T_CNATFWIceSettings_GetPrefValueL();
         
         void T_CNATFWIceSettings_GetPrefValueCenRepL();
                 
         void T_CNATFWIceSettings_NatUtilityPluginsLL();
        

    private:    // Data
		
        CNATFWIceSettings* iCNATFWIceSettings;
        CSettingStore* iSettingsStore;
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_CNATFWICESETTINGS_H__

// End of file
