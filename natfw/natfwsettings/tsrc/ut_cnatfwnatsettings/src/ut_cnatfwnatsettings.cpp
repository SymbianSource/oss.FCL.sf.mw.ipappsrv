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




//  CLASS HEADER
#include "UT_CNATFWNatSettings.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <badesca.h>
#include <mnatfwicesettings.h>

//  INTERNAL INCLUDES
#include <cnatfwsettingsapi.h>
#include "cnatfwsettingsimpl.h"
#include "cnatfwserversettings.h"
#include "cnatfwstunsettings.h"
#include "cnatfwturnsettings.h"
#include <unsafprotocolscrkeys.h>
#include "natfwdefaultsettingsvalues.h"
#include "settingstore.h"
#include "natfwunittestmacros.h"

const TBool KCRLFEnabled = ETrue;
const TUint KUDPRefreshInterval = 200;
const TUint KTCPRefreshInterval = 300;

const TInt KStartPort = 4000;
const TInt KEndPort = 4100;

_LIT8( KNATProtocol, "nokia.ice,nokia.stun,nokia.turn" );
_LIT8( KNokiaStun, "nokia.stun" );
_LIT8( KNokiaTurn, "nokia.turn" );
_LIT8( KNokiaIce, "nokia.ice" );


const TBool KUseSharedSecret = ETrue; 

_LIT8(KDomain,"www.domain.fi");
const TUint KIapId = 1979;

const TUint KDefaultGranularity = 1;

// CONSTRUCTION
UT_CNATFWNatSettings* UT_CNATFWNatSettings::NewL()
    {
    UT_CNATFWNatSettings* self = UT_CNATFWNatSettings::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWNatSettings* UT_CNATFWNatSettings::NewLC()
    {
    UT_CNATFWNatSettings* self = new( ELeave ) UT_CNATFWNatSettings();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWNatSettings::~UT_CNATFWNatSettings()
    {
    }

// Default constructor
UT_CNATFWNatSettings::UT_CNATFWNatSettings()
    {
    }

// Second phase construct
void UT_CNATFWNatSettings::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWNatSettings::SetupL( )
    {
    iSettingsStore = CSettingStore::NewL( KDomain, KIapId );
    //clean all repository contets
    iSettingsStore->EraseL( 0, 0 );

    iSettings = CNATFWNatSettingsApi::NewL( KDomain );
    }

void UT_CNATFWNatSettings::CRSetupL()
    {
  	SetupL();
  	
  	//Create Domain
  	iSettingsStore->CreateNewDomainL( KDomain );
  	
  	// General settings
  	iSettingsStore->iRefreshIntervalUdp = KUDPRefreshInterval;
    iSettingsStore->iRefreshIntervalTcp = KTCPRefreshInterval;
  	iSettingsStore->iStartPort = KStartPort;
    iSettingsStore->iEndPort = KEndPort;
    iSettingsStore->iCrlfRefresEnabled = KCRLFEnabled;
    iSettingsStore->iUseSharedSecret = KUseSharedSecret;

   	CleanupStack::PushL( iSettingsStore );
  	iSettingsStore->StoreSettingsL();
  	CleanupStack::Pop( iSettingsStore );

	delete iSettings;
	iSettings = NULL;
	iSettings = CNATFWNatSettingsApi::NewL( KDomain );
    }

void UT_CNATFWNatSettings::CRTeardown()
    {
    iSettingsStore->EraseL( 0, 0 );
	Teardown();
    }
    
void UT_CNATFWNatSettings::Teardown()
    {
    delete iSettings;
    iSettings = NULL;
    iSettingsStore->EraseL( 0, 0 );
    delete iSettingsStore;
    iSettingsStore = NULL;
    }

/***********************************************************************************/

void UT_CNATFWNatSettings::UT_CNATFWNatSettings_NewLL()
    {
    CNATFWNatSettingsApi* settings = NULL;

    settings = CNATFWNatSettingsApi::NewL( KDomain );
            
    delete settings;
    settings = NULL;
    
    // create settings with 2 same domain name
    iSettingsStore->CreateNewDomainL( KDomain );
    iSettingsStore->CreateNewDomainL( KDomain );

    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE(
        settings = CNATFWNatSettingsApi::NewL( KDomain ), KErrCorrupt );
    
    delete settings;
    settings = NULL;
    
    // domain is Null pointer
    HBufC8* nullBuf = NULL;

    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE(
        settings = CNATFWNatSettingsApi::NewL( *nullBuf ), KErrArgument );
    
    delete settings;
    settings = NULL;
    
    // domain is empty descriptor
    nullBuf = HBufC8::NewLC( 0 );

    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE(
        settings = CNATFWNatSettingsApi::NewL( *nullBuf ), KErrArgument );
   
    delete settings;
    settings = NULL;
    
    CleanupStack::PopAndDestroy( nullBuf );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_NewLCL(  )
    {
    CNATFWNatSettingsApi* settings = CNATFWNatSettingsApi::NewLC( KDomain );
    CleanupStack::PopAndDestroy( settings );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_RetrieveIapSettingsLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iSettings->RetrieveIapSettingsL( KIapId ) );
    
    // new setings with the same iapid
    NATFW_EUNIT_ASSERT_NO_LEAVE( iSettingsStore->CreateNewIapL( KIapId ) );

    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iSettings->RetrieveIapSettingsL( KIapId ), KErrCorrupt );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_DomainL(  )
    {
    EUNIT_ASSERT_EQUALS( iSettings->Domain(), KDomain );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_UseSharedSecretL(  )
    {
    iSettingsStore->iUseSharedSecret = ETrue;
    iSettingsStore->StoreSettingsL();    

    delete iSettings;
    iSettings = NULL;
    iSettings = CNATFWNatSettingsApi::NewL( KDomain );
    
    EUNIT_ASSERT( iSettings->UseSharedSecret() );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_UDPRefreshIntervalL(  )
    {
    EUNIT_ASSERT_EQUALS( iSettings->RefreshIntervalUdp(),
        KUDPRefreshInterval );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_TCPRefreshIntervalL(  )
    {
    EUNIT_ASSERT_EQUALS( iSettings->RefreshIntervalTcp(),
        KTCPRefreshInterval );
    }
    

void UT_CNATFWNatSettings::UT_CNATFWNatSettings_AvailableNatProtocolsL()
    {
    EUNIT_ASSERT( iSettings->AvailableNatProtocols().Count() == 3 );
    EUNIT_ASSERT_EQUALS( iSettings->AvailableNatProtocols().MdcaPoint( 0 ),
        KNokiaStun() );
    EUNIT_ASSERT_EQUALS( iSettings->AvailableNatProtocols().MdcaPoint( 1 ),
        KNokiaTurn() );
    EUNIT_ASSERT_EQUALS( iSettings->AvailableNatProtocols().MdcaPoint( 2 ),
        KNokiaIce() );
        
    //test with cr contents
    delete iSettingsStore->iAvailableNatProtocols;
    iSettingsStore->iAvailableNatProtocols = NULL;
    iSettingsStore->iAvailableNatProtocols = KNATProtocol().AllocL();
    iSettingsStore->StoreSettingsL();
    
    delete iSettings;
    iSettings = NULL;
    iSettings = CNATFWNatSettingsApi::NewL( KDomain );

    EUNIT_ASSERT( iSettings->AvailableNatProtocols().Count() == 3 );
    EUNIT_ASSERT_EQUALS( iSettings->AvailableNatProtocols().MdcaPoint( 0 ),
        KNokiaIce() );
    EUNIT_ASSERT_EQUALS( iSettings->AvailableNatProtocols().MdcaPoint( 1 ),
        KNokiaStun() );
    EUNIT_ASSERT_EQUALS( iSettings->AvailableNatProtocols().MdcaPoint( 2 ),
        KNokiaTurn() );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_GetPortAreaL()
    {
    const TInt KPortPoolStartPort = 45000;
    const TInt KPortPoolEndPort = 50000;
    
    TUint startPort = 0;
    TUint endPort = 0;
    
    // write portarea settings to cr
    iSettingsStore->iStartPort = KPortPoolStartPort;
    iSettingsStore->iEndPort = KPortPoolEndPort;
    iSettingsStore->StoreSettingsL();
    
    delete iSettings;
    iSettings = NULL;
    iSettings = CNATFWNatSettingsApi::NewL( KDomain );

    iSettings->GetPortArea( startPort, endPort );
    
    EUNIT_ASSERT_EQUALS( startPort, KPortPoolStartPort );
    EUNIT_ASSERT_EQUALS( endPort, KPortPoolEndPort );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_CrlfRefresEnabledL()
    {
    EUNIT_ASSERT( iSettings->CrlfRefreshEnabled() );
    
    iSettingsStore->iCrlfRefresEnabled = EFalse;
    iSettingsStore->StoreSettingsL();
    
    delete iSettings;
    iSettings = NULL;
    iSettings = CNATFWNatSettingsApi::NewL( KDomain );
    
    EUNIT_ASSERT( !iSettings->CrlfRefreshEnabled() );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_IceSettingsLL()
    {
    MNATFWIceSettings* iceSettings;
    MNATFWIceSettings* iceSettings2;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iceSettings = &iSettings->IceSettingsL() );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iceSettings2 = &iSettings->IceSettingsL() );
    
    EUNIT_ASSERT_EQUALS( iceSettings, iceSettings2 );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_StunSettingsLL()
    {
    MNATFWStunSettings* stunSettings;
    MNATFWStunSettings* stunSettings2;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( stunSettings = &iSettings->StunSettingsL() );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        stunSettings2 = &iSettings->StunSettingsL() );
    
    EUNIT_ASSERT_EQUALS( stunSettings, stunSettings2 );
    }


void UT_CNATFWNatSettings::UT_CNATFWNatSettings_TurnSettingsLL()
    {
    MNATFWTurnSettings* turnSettings;
    MNATFWTurnSettings* turnSettings2;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( turnSettings = &iSettings->TurnSettingsL() );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        turnSettings2 = &iSettings->TurnSettingsL() );
    
    EUNIT_ASSERT_EQUALS( turnSettings, turnSettings2 );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWNatSettings,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CNATFWNATSettings",
    "NewL",
    "FUNCTIONALITY",
    CRSetupL, UT_CNATFWNatSettings_NewLL, CRTeardown)

EUNIT_TEST(
    "NewLC - test ",
    "CNATFWNATSettings",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWNatSettings_NewLCL, Teardown)

EUNIT_TEST(
    "RetrieveIapSettingsL - test ",
    "CNATFWNATSettings",
    "RetrieveIapSettingsL",
    "FUNCTIONALITY",
    CRSetupL, UT_CNATFWNatSettings_RetrieveIapSettingsLL, CRTeardown)

EUNIT_TEST(
    "Domain - test ",
    "CNATFWNATSettings",
    "Domain",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWNatSettings_DomainL, Teardown)

EUNIT_TEST(
    "UseSharedSecret - test ",
    "CNATFWNATSettings",
    "UseSharedSecret",
    "FUNCTIONALITY",
    CRSetupL, UT_CNATFWNatSettings_UseSharedSecretL, CRTeardown)

EUNIT_TEST(
    "UDPRefreshInterval - test ",
    "CNATFWNATSettings",
    "UDPRefreshInterval",
    "FUNCTIONALITY",
    CRSetupL, UT_CNATFWNatSettings_UDPRefreshIntervalL, CRTeardown)

EUNIT_TEST(
    "TCPRefreshInterval - test ",
    "CNATFWNATSettings",
    "TCPRefreshInterval",
    "FUNCTIONALITY",
    CRSetupL, UT_CNATFWNatSettings_TCPRefreshIntervalL, CRTeardown)

EUNIT_TEST(    
    "GetPortArea - test",
    "CNATFWNATSettings",
    "GetPortArea",
    "FUNCTIONALITY",    
    CRSetupL, UT_CNATFWNatSettings_GetPortAreaL, CRTeardown)

EUNIT_TEST(
    "IceSettingsL - test",
    "CNATFWNATSettings",
    "IceSettingsL",
    "FUNCTIONALITY",    
    SetupL, UT_CNATFWNatSettings_IceSettingsLL, Teardown )
    
EUNIT_TEST(
    "StunSettingsL - test",
    "CNATFWNATSettings",
    "StunSettingsL",
    "FUNCTIONALITY",    
    SetupL, UT_CNATFWNatSettings_StunSettingsLL, Teardown )
    
EUNIT_TEST(
    "TurnSettingsL - test",
    "CNATFWNATSettings",
    "TurnSettingsL",
    "FUNCTIONALITY",    
    SetupL, UT_CNATFWNatSettings_TurnSettingsLL, Teardown )

EUNIT_TEST(    
    "AvailableNatProtocols - test",
    "CNATFWNATSettings",
    "AvailableNatProtocols",
    "FUNCTIONALITY",    
    CRSetupL, UT_CNATFWNatSettings_AvailableNatProtocolsL, CRTeardown)

EUNIT_TEST(    
    "CrlfRefresEnabled - test",
    "CNATFWNATSettings",
    "CrlfRefresEnabled",
    "FUNCTIONALITY",    
    CRSetupL, UT_CNATFWNatSettings_CrlfRefresEnabledL, CRTeardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
