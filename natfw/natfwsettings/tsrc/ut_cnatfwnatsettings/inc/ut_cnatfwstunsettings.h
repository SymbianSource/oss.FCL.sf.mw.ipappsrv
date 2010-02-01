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




#ifndef __UT_CNATFWSTUNSETTINGS_H__
#define __UT_CNATFWSTUNSETTINGS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitDecorators.h>
#include <unsafprotocolscrkeys.h>

//  INTERNAL INCLUDES
#include "cnatfwstunsettings.h"

//  FORWARD DECLARATIONS
class CSettingStore;

//  CLASS DEFINITION
/**
 * TODO Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( UT_CNATFWStunSettings )
	: public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWStunSettings* NewL();
        static UT_CNATFWStunSettings* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWStunSettings();

    private:    // Constructors and destructors

        UT_CNATFWStunSettings();
        void ConstructL();

    private:    // New methods

        void SetupL();
        
        void Teardown();
        
        void SetupCenRepL();
        
        void TeardownCenRepL();
        
        void T_CNATFWStunSettings_GetStunServerArrayLL();
        
        void T_CNATFWStunSettings_GetStunServerArrayL2L(); 
         
        void T_CNATFWStunSettings_RetransmissionTimeoutL();
       
        void T_CNATFWStunSettings_LatestConnectedServerAddrL();
        
        void T_CNATFWStunSettings_LatestConnectedServerPortL();
        
        void T_CNATFWStunSettings_ReadSettingsL();
        
    private:    // Data
		
		TUint32 iDomainKey;
		CNATFWCenRepHandler* iRepHandler;
        CNATFWStunSettings* iCNATFWStunSettings;
        CSettingStore* iSettingsStore;
        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CNATFWSTUNSETTINGS_H__

// End of file
