/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include <badesca.h>
#include <unsafprotocolscrkeys.h>
#include "cnatfwicesettings.h"
#include "natfwdefaultsettingsvalues.h"
#include "cnatfwcenrephandler.h"
#include "tnatfwsettingsparser.h"
#include "natsettingslogs.h"

const TUint KDefaultGranularity = 2;


// ======== MEMBER FUNCTIONS ========

CNATFWIceSettings::CNATFWIceSettings()
    {

    }


void CNATFWIceSettings::ConstructL( )
    {
    iUtilityPlugins = new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    }


CNATFWIceSettings* CNATFWIceSettings::NewL()
    {
    CNATFWIceSettings* self = CNATFWIceSettings::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


CNATFWIceSettings* CNATFWIceSettings::NewLC()
    {
    CNATFWIceSettings* self = new( ELeave ) CNATFWIceSettings;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CNATFWIceSettings::~CNATFWIceSettings()
    {
    delete iUtilityPlugins;
    }


// ---------------------------------------------------------------------------
// CNATFWIceSettings::SetPreferences
// ---------------------------------------------------------------------------
//
void CNATFWIceSettings::SetPreferences( 
        const TNATFWIcePreferences& aPreferences )
    {
    iPreferences = aPreferences;
    }


// ---------------------------------------------------------------------------
// CNATFWIceSettings::SetPrefValue
// ---------------------------------------------------------------------------
//
TInt CNATFWIceSettings::SetPrefValue( TPrefSetting aSetting, TInt aValue )
    {
    return iPreferences.SetPrefValue( aSetting, aValue );
    }


// ---------------------------------------------------------------------------
// CNATFWIceSettings::Preferences
// ---------------------------------------------------------------------------
//
const TNATFWIcePreferences& CNATFWIceSettings::Preferences() const
    {
    return iPreferences;
    }


// ---------------------------------------------------------------------------
// CNATFWIceSettings::SetUtilityPluginsL
// ---------------------------------------------------------------------------
//
void CNATFWIceSettings::SetUtilityPluginsL(
    const CDesC8Array& aUtilityPlugins )
    {
    __ASSERT_ALWAYS( &aUtilityPlugins, User::Leave( KErrArgument ) );
    iUtilityPlugins->Reset();
    TInt pluginIdCount = aUtilityPlugins.Count();
    for ( TInt i( 0 ); i < pluginIdCount; ++i )
        {
        iUtilityPlugins->AppendL( aUtilityPlugins[ i ] );
        }
    }


// ---------------------------------------------------------------------------
// CNATFWIceSettings::ReadSettingsL
// ---------------------------------------------------------------------------
//
void CNATFWIceSettings::ReadSettingsL( TUint32 aDomainKey,
    const CNATFWCenRepHandler& aRepHandler )
    {
    __NATSETTINGS( "CNATFWIceSettings::ReadSettingsL - start" )
    
    //ICE used plug-ins
    CDesC8Array* utilityPlugindIds( NULL );
    HBufC8* tempBuffer = aRepHandler.ReadL(
        KUNSAFProtocolsNATUtilities | aDomainKey );
    
    if ( tempBuffer )
        {
        CleanupStack::PushL( tempBuffer );
        utilityPlugindIds =
            TNATFWSettingsParser::ParseDataToArrayL( *tempBuffer );
        CleanupStack::PopAndDestroy( tempBuffer );
        }
    else
        {
        utilityPlugindIds = TNATFWSettingsParser::ParseDataToArrayL(
            KNATFWDefaultIceUsedPlugins );
        }
    CleanupStack::PushL( utilityPlugindIds );
    SetUtilityPluginsL( *utilityPlugindIds );
    CleanupStack::PopAndDestroy( utilityPlugindIds );
    
    TInt prefValue( 0 );
    
    // Host type preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsHostPref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultlHostPref;
        }
    SetPrefValue( 
        MNATFWIceSettings::EPrefHostCandidate, prefValue );
    
    // Server-reflexive type preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsServerRefPref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultServerRefPref;
        }
    SetPrefValue( 
        MNATFWIceSettings::EPrefServerReflexiveCandidate, prefValue );
    
    // Relay type Preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsRelayPref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultRelayPref;
        }
    SetPrefValue( 
        MNATFWIceSettings::EPrefRelayCandidate, prefValue );
    
    // Peer-reflexive type Preference value.
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsPeerRefPref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultPeerRefPref;
        }
    SetPrefValue(
        MNATFWIceSettings::EPrefPeerReflexiveCandidate, prefValue );
    
    // IPV4 local preference value
    if ( KErrNone != aRepHandler.Read(KUNSAFProtocolsIPV4Pref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultIPV4Pref;
        }
    SetPrefValue(
        MNATFWIceSettings::EPrefIpv4AddrFamily, prefValue );
    
    // IPV6 local preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsIPV6Pref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultIPV6Pref;
        }
    SetPrefValue(
        MNATFWIceSettings::EPrefIpv6AddrFamily, prefValue );
    
    // VPN local preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsVPNPref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultVPNPref;
        }
    SetPrefValue(
        MNATFWIceSettings::EPrefVpnInterface, prefValue );
    
    // UDP transport preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsUDPPref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultUDPPref;
        }
    SetPrefValue(
        MNATFWIceSettings::EPrefUdpTransport, prefValue );
    
    // TCP transport preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsTCPPref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultTCPPref;
        }
    SetPrefValue(
        MNATFWIceSettings::EPrefTcpTransport, prefValue );
    
    // TCP-active direction preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsTCPActivePref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultTCPActivePref;
        }
    SetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionActive, prefValue );
    
    // TCP-passive direction preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsTCPPassivePref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultTCPPassivePref;
        }
    SetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionPassive, prefValue );
    
    // TCP-simultaneous-open direction preference value
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsTCPSimultPref |
        aDomainKey, prefValue ) )
        {
        prefValue = KNATFWDefaultTCPSimultPref;
        }
    SetPrefValue( 
        MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen, prefValue );
        
    __NATSETTINGS( "CNATFWIceSettings::ReadSettingsL - end" )
    }
    
    
// ---------------------------------------------------------------------------
// From class MNATFWIceSettings.
// Returns preference value for a specified setting.
// ---------------------------------------------------------------------------
//
TInt CNATFWIceSettings::GetPrefValue( 
        TPrefSetting aSetting, TInt& aValue ) const
    {
    return iPreferences.GetPrefValue( aSetting, aValue );
    }


// ---------------------------------------------------------------------------
// From class MNATFWIceSettings.
// Returns array of identifiers of utility plugins available for ICE.
// ---------------------------------------------------------------------------
//
const CDesC8Array& CNATFWIceSettings::NatUtilityPlugins() const
    {
    return *iUtilityPlugins;
    }
