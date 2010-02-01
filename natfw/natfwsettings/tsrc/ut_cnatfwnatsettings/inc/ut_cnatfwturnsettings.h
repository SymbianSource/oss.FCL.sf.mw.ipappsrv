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




#ifndef __UT_CNATFWTURNSETTINGS_H__
#define __UT_CNATFWTURNSETTINGS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitDecorators.h>


//  INTERNAL INCLUDES
#include "cnatfwturnsettings.h"

//  FORWARD DECLARATIONS
class CSettingStore;

//  CLASS DEFINITION
/**
 * TODO Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( UT_CNATFWTurnSettings )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWTurnSettings* NewL();
        static UT_CNATFWTurnSettings* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWTurnSettings();

    private:    // Constructors and destructors

        UT_CNATFWTurnSettings();
        void ConstructL();

    private:    // New methods

        void SetupL();
        
        void Teardown();
        
        void SetupCenRepL();
        
        void TeardownCenRepL();
        
        void T_CNATFWTurnSettings_GetTurnServerArrayLL();
         
        void T_CNATFWStunSettings_GetTurnServerArrayL2L();
        
        void T_CNATFWTurnSettings_RetransmissionTimeoutL();
        
        void T_CNATFWTurnSettings_LatestConnectedServerAddrL();
        
        void T_CNATFWTurnSettings_LatestConnectedServerPortL();
        
        void T_CNATFWTurnSettings_ReadSettingsL();

    private:    // Data
		
	    TUint32 iDomainKey;
		CNATFWCenRepHandler* iRepHandler;
        CNATFWTurnSettings* iCNATFWTurnSettings;
        CSettingStore* iSettingsStore;
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_CNATFWTURNSETTINGS_H__

// End of file
