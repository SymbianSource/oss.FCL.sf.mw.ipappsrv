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
#include "UT_CNATFWIceSettings.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <badesca.h>
#include <unsafprotocolscrkeys.h>

//  INTERNAL INCLUDES
#include "cnatfwicesettings.h"
#include "settingstore.h"
#include "cnatfwcenrephandler.h"
#include "natfwdefaultsettingsvalues.h"
#include "natfwunittestmacros.h"

_LIT8( KNokiaStun, "nokia.stun" );
_LIT8( KNokiaTurn, "nokia.turn" );

_LIT8( KDomain,"www.domain.fi" );
const TUint KIapId = 1979;

_LIT8( KUtilityPlugin, "nokia.stun, nokia.turn" );

const TInt KTestHostPref = 1001;
const TInt KTestServerRefPref = 1002;
const TInt KTestRelayPref = 1003;
const TInt KTestPeerRefPref = 1004;
const TInt KTestIPV4Pref = 1005;
const TInt KTestIPV6Pref = 1006;
const TInt KTestVPNPref = 1007;
const TInt KTestUDPPref = 1008;
const TInt KTestTCPPref = 1009;
const TInt KTestTCPActivePref = 1010; 
const TInt KTestTCPPassivePref = 1011;
const TInt KTestTCPSimultPref = 1012;

const TUint KDefaultGranularity = 1;


// CONSTRUCTION
UT_CNATFWIceSettings* UT_CNATFWIceSettings::NewL()
    {
    UT_CNATFWIceSettings* self = UT_CNATFWIceSettings::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWIceSettings* UT_CNATFWIceSettings::NewLC()
    {
    UT_CNATFWIceSettings* self = new( ELeave ) UT_CNATFWIceSettings();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWIceSettings::~UT_CNATFWIceSettings()
    {
    }

// Default constructor
UT_CNATFWIceSettings::UT_CNATFWIceSettings()
    {
    }

// Second phase construct
void UT_CNATFWIceSettings::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATFWIceSettings::SetupL(  )
    {
    iCNATFWIceSettings = CNATFWIceSettings::NewL();
    }
    

void UT_CNATFWIceSettings::Teardown(  )
    {
    delete iCNATFWIceSettings; 
    iCNATFWIceSettings = NULL; 
    }
    

void UT_CNATFWIceSettings::SetupCenRepL(  )
    {
    SetupL();
    
    iSettingsStore = CSettingStore::NewL( KDomain, KIapId );
    iSettingsStore->EraseL( 0, 0 );
    
    CNATFWCenRepHandler* repHandler =
       CNATFWCenRepHandler::NewL( KCRUidUNSAFProtocols );
    CleanupStack::PushL( repHandler );
    
    //Create Domain
    TUint32 domainKey = iSettingsStore->CreateNewDomainL( KDomain );
    
    // ICE settings  
    delete iSettingsStore->iUtilityPlugins;
    iSettingsStore->iUtilityPlugins = NULL;
    iSettingsStore->iUtilityPlugins= KUtilityPlugin().AllocL();
    
    iSettingsStore->iHostPref = KTestHostPref;
    iSettingsStore->iServerReflexivePref = KTestServerRefPref;
    iSettingsStore->iRelayPref = KTestRelayPref;
    iSettingsStore->iPeerReflexivePref = KTestPeerRefPref;
    iSettingsStore->iIpv4AddrFamilyPref = KTestIPV4Pref;
    iSettingsStore->iIpv6AddrFamilyPref = KTestIPV6Pref;
    iSettingsStore->iVpnInterfacePref = KTestVPNPref;
    iSettingsStore->iUdpTransportPref = KTestUDPPref;
    iSettingsStore->iTcpTransportPref = KTestTCPPref;
    iSettingsStore->iTcpDirActivePref = KTestTCPActivePref;
    iSettingsStore->iTcpDirPassivePref = KTestTCPPassivePref;
    iSettingsStore->iTcpDirSimultaneousOpenPref = KTestTCPSimultPref;
    
    CleanupStack::PushL( iSettingsStore );
    iSettingsStore->StoreSettingsL();
    CleanupStack::Pop( iSettingsStore );
    
    iCNATFWIceSettings->ReadSettingsL( domainKey, *repHandler );
    CleanupStack::PopAndDestroy( repHandler );
    }


void UT_CNATFWIceSettings::TeardownCenRepL(  )
    {
    Teardown();
    iSettingsStore->EraseL( 0, 0 );
    delete iSettingsStore;
    iSettingsStore = NULL;
    }
    

void UT_CNATFWIceSettings::T_CNATFWIceSettings_SetPreferencesL()
    {
    TNATFWIcePreferences preferences;
    TInt prefValue = 0;
    
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefHostCandidate, KTestHostPref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefServerReflexiveCandidate,
        KTestServerRefPref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefRelayCandidate, KTestRelayPref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefPeerReflexiveCandidate, KTestPeerRefPref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefIpv4AddrFamily, KTestIPV4Pref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefIpv6AddrFamily, KTestIPV6Pref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefVpnInterface, KTestVPNPref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefUdpTransport, KTestUDPPref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefTcpTransport, KTestTCPPref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionActive, KTestTCPActivePref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionPassive, KTestTCPPassivePref );
    preferences.SetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen,
        KTestTCPSimultPref );
    
    iCNATFWIceSettings->SetPreferences( preferences );
    
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefHostCandidate, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestHostPref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefServerReflexiveCandidate, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestServerRefPref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefRelayCandidate, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestRelayPref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefPeerReflexiveCandidate, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestPeerRefPref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefIpv4AddrFamily, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestIPV4Pref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefIpv6AddrFamily, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestIPV6Pref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefVpnInterface, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestVPNPref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefUdpTransport, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestUDPPref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefTcpTransport, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestTCPPref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionActive, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestTCPActivePref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionPassive, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestTCPPassivePref );
    iCNATFWIceSettings->Preferences().GetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen, prefValue );
    EUNIT_ASSERT_EQUALS( prefValue, KTestTCPSimultPref );
    }

    
void UT_CNATFWIceSettings::T_CNATFWIceSettings_GetPrefValueL(  )
    {
    TInt value( 0 );
    
    CSettingStore* settingsStore =
        CSettingStore::NewL( KDomain, KIapId );
    CleanupStack::PushL( settingsStore );
    
    settingsStore->EraseL( 0, 0 );
    
    //Create Domain
    TUint32 domainKey = settingsStore->CreateNewDomainL( KDomain );
    
    CNATFWCenRepHandler* repHandler =
       CNATFWCenRepHandler::NewL( KCRUidUNSAFProtocols );
    
    CleanupStack::PushL( repHandler );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWIceSettings->ReadSettingsL( domainKey, *repHandler ) );
    CleanupStack::PopAndDestroy( repHandler );
    
    // Test read parameter when no parameters in CR
    // Then default values should be used
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefHostCandidate, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultlHostPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefServerReflexiveCandidate, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultServerRefPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefRelayCandidate, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultRelayPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefPeerReflexiveCandidate, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultPeerRefPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefIpv4AddrFamily, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultIPV4Pref, value);
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefIpv6AddrFamily, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultIPV6Pref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefVpnInterface, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultVPNPref, value );
    
    iCNATFWIceSettings->GetPrefValue(
        MNATFWIceSettings::EPrefUdpTransport, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultUDPPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpTransport, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultTCPPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionActive, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultTCPActivePref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionPassive, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultTCPPassivePref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen, value );
    EUNIT_ASSERT_EQUALS( KNATFWDefaultTCPSimultPref, value );
    
    
    // Write ICE priority parameters
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefHostCandidate, KTestHostPref );
    
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefServerReflexiveCandidate,
        KTestServerRefPref );
    
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefRelayCandidate, KTestRelayPref );
    
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefPeerReflexiveCandidate, KTestPeerRefPref );
    
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefIpv4AddrFamily, KTestIPV4Pref );
    
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefIpv6AddrFamily, KTestIPV6Pref );
    
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefVpnInterface, KTestVPNPref );
        
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefUdpTransport, KTestUDPPref );
    
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefTcpTransport, KTestTCPPref );
        
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionActive, KTestTCPActivePref );
        
    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionPassive, KTestTCPPassivePref );

    iCNATFWIceSettings->SetPrefValue(
        MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen, KTestTCPSimultPref );


    //  Test read parameters
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefHostCandidate, value );
    EUNIT_ASSERT_EQUALS( KTestHostPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefServerReflexiveCandidate, value );
    EUNIT_ASSERT_EQUALS( KTestServerRefPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefRelayCandidate, value );
    EUNIT_ASSERT_EQUALS( KTestRelayPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefPeerReflexiveCandidate, value );
    EUNIT_ASSERT_EQUALS( KTestPeerRefPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefIpv4AddrFamily, value );
    EUNIT_ASSERT_EQUALS( KTestIPV4Pref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefIpv6AddrFamily, value );
    EUNIT_ASSERT_EQUALS( KTestIPV6Pref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefVpnInterface, value );
    EUNIT_ASSERT_EQUALS( KTestVPNPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefUdpTransport, value );
    EUNIT_ASSERT_EQUALS( KTestUDPPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpTransport, value );
    EUNIT_ASSERT_EQUALS( KTestTCPPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionActive, value );
    EUNIT_ASSERT_EQUALS( KTestTCPActivePref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionPassive, value );
    EUNIT_ASSERT_EQUALS( KTestTCPPassivePref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen, value );
    EUNIT_ASSERT_EQUALS( KTestTCPSimultPref, value );
    
    CleanupStack::PopAndDestroy( settingsStore );
    }


void UT_CNATFWIceSettings::T_CNATFWIceSettings_GetPrefValueCenRepL(  )
    {
    TInt value( 0 );

    // Test read parameter in CR
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefHostCandidate, value );
    EUNIT_ASSERT_EQUALS( KTestHostPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefServerReflexiveCandidate, value );
    EUNIT_ASSERT_EQUALS( KTestServerRefPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefRelayCandidate, value );
    EUNIT_ASSERT_EQUALS( KTestRelayPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefPeerReflexiveCandidate, value );
    EUNIT_ASSERT_EQUALS( KTestPeerRefPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefIpv4AddrFamily, value );
    EUNIT_ASSERT_EQUALS( KTestIPV4Pref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefIpv6AddrFamily, value );
    EUNIT_ASSERT_EQUALS( KTestIPV6Pref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefVpnInterface, value );
    EUNIT_ASSERT_EQUALS( KTestVPNPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefUdpTransport, value );
    EUNIT_ASSERT_EQUALS( KTestUDPPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpTransport, value );
    EUNIT_ASSERT_EQUALS( KTestTCPPref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionActive, value );
    EUNIT_ASSERT_EQUALS( KTestTCPActivePref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionPassive, value );
    EUNIT_ASSERT_EQUALS( KTestTCPPassivePref, value );
    
    iCNATFWIceSettings->GetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen, value );
    EUNIT_ASSERT_EQUALS( KTestTCPSimultPref, value );
    }


void UT_CNATFWIceSettings::T_CNATFWIceSettings_NatUtilityPluginsLL(  )
    {
    CDesC8Array* desArray  = NULL;
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( iCNATFWIceSettings->SetUtilityPluginsL(
        *desArray ), KErrArgument );
    
    desArray = new( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    CleanupStack::PushL( desArray );
    
    desArray->AppendL( KNokiaStun );
    desArray->AppendL( KNokiaTurn ); 
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNATFWIceSettings->SetUtilityPluginsL(
        *desArray ) );
    CleanupStack::PopAndDestroy( desArray );
    
    EUNIT_ASSERT_EQUALS( iCNATFWIceSettings->NatUtilityPlugins().Count(), 2 );
    
    EUNIT_ASSERT_EQUALS( iCNATFWIceSettings->NatUtilityPlugins()[0],
        KNokiaStun() );
    
    EUNIT_ASSERT_EQUALS( iCNATFWIceSettings->NatUtilityPlugins()[1],
        KNokiaTurn() );
    }
    

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWIceSettings,
    "Add test suite description here.",
    "UNIT" )
  
EUNIT_TEST(
    "SetPreferences - test1",
    "CNATFWIceSettings",
    "SetPreferences / Preferences - test1",
    "FUNCTIONALITY",
    SetupL, T_CNATFWIceSettings_SetPreferencesL, Teardown )

EUNIT_TEST(
    "GetPrefValue - test2",
    "CNATFWIceSettings",
    "GetPrefValue - test2",
    "FUNCTIONALITY",
    SetupL, T_CNATFWIceSettings_GetPrefValueL, Teardown )
    
EUNIT_TEST(
    "GetPrefValueCenrep - test3",
    "CNATFWIceSettings",
    "GetPrefValue - test3",
    "FUNCTIONALITY",
    SetupCenRepL, T_CNATFWIceSettings_GetPrefValueCenRepL, TeardownCenRepL )
    
EUNIT_TEST(
    "NatUtilityPlugins - test4",
    "CNATFWIceSettings",
    "NatUtilityPlugins - test4",
    "FUNCTIONALITY",
    SetupL, T_CNATFWIceSettings_NatUtilityPluginsLL, Teardown )
    

EUNIT_END_TEST_TABLE

//  END OF FILE
