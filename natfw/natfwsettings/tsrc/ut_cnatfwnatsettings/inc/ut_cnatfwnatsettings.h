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




#ifndef __UT_CNATFWNATSETTINGS_H__
#define __UT_CNATFWNATSETTINGS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CNATFWNatSettingsApi;
class CRepository;
class CSettingStore;

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
NONSHARABLE_CLASS( UT_CNATFWNatSettings )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWNatSettings* NewL();
        static UT_CNATFWNatSettings* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWNatSettings();

    private:    // Constructors and destructors

        UT_CNATFWNatSettings();
        void ConstructL();

    public:     // From observer interface

        

    private:    // Test methods

         void SetupL();
   
   
         void Teardown();
         
         
         void CRSetupL();
         
         
         void SetupAL();
         

         void CRSetupAL();

         
         void CRTeardown();
        
        
         void UT_CNATFWNatSettings_NewLL();
        
        
         void UT_CNATFWNatSettings_NewLCL();

         
         void UT_CNATFWNatSettings_RetrieveIapSettingsLL();
        
        
         void UT_CNATFWNatSettings_DomainL();
        

         void UT_CNATFWNatSettings_UseSharedSecretL();
        
        
         void UT_CNATFWNatSettings_UDPRefreshIntervalL();
        
        
         void UT_CNATFWNatSettings_TCPRefreshIntervalL();
         
         
         void UT_CNATFWNatSettings_CrlfRefresEnabledL();
         
         
         void UT_CNATFWNatSettings_GetPortAreaL();
         
         
         void UT_CNATFWNatSettings_IceSettingsLL();
         
         
         void UT_CNATFWNatSettings_StunSettingsLL();
         
         
         void UT_CNATFWNatSettings_TurnSettingsLL();


         void UT_CNATFWNatSettings_AvailableNatProtocolsL();    
        
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CNATFWNatSettingsApi* iSettings;
        CRepository* iRepository;
        TUint32 iDomainKey;
        CSettingStore* iSettingsStore;
    };

#endif      //  __UT_CNATFWNATSETTINGS_H__

// End of file
