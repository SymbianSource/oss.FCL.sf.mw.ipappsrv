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
* Description:    Encapsulates preference settings for ICE prioritization.
*
*/




#include "tnatfwicepreferences.h"
#include "natfwdefaultsettingsvalues.h"

// ======== MEMBER FUNCTIONS ========

TNATFWIcePreferences::TNATFWIcePreferences()
    :
    iHostPref( KNATFWDefaultlHostPref ),
    iServerReflexivePref( KNATFWDefaultServerRefPref ),
    iRelayPref( KNATFWDefaultRelayPref ),
    iPeerReflexivePref( KNATFWDefaultPeerRefPref ),
    iIpv4AddrFamilyPref( KNATFWDefaultIPV4Pref ),
    iIpv6AddrFamilyPref( KNATFWDefaultIPV6Pref ),
    iVpnInterfacePref( KNATFWDefaultVPNPref ),
    iUdpTransportPref( KNATFWDefaultUDPPref ),
    iTcpTransportPref( KNATFWDefaultTCPPref ),
    iTcpDirActivePref( KNATFWDefaultTCPActivePref ),
    iTcpDirPassivePref( KNATFWDefaultTCPPassivePref ),
    iTcpDirSimultaneousOpenPref( KNATFWDefaultTCPSimultPref )
    {
    }


// ---------------------------------------------------------------------------
// TNATFWIcePreferences::SetPrefValue
// ---------------------------------------------------------------------------
//
TInt TNATFWIcePreferences::SetPrefValue( 
        MNATFWIceSettings::TPrefSetting aSetting, TInt aValue )
    {
    switch ( aSetting )
        {
        case MNATFWIceSettings::EPrefHostCandidate:
            iHostPref = aValue;
            break;
        
        case MNATFWIceSettings::EPrefServerReflexiveCandidate:
            iServerReflexivePref = aValue;
            break;
        
        case MNATFWIceSettings::EPrefRelayCandidate:
            iRelayPref = aValue;
            break;
        
        case MNATFWIceSettings::EPrefPeerReflexiveCandidate:
            iPeerReflexivePref = aValue;
            break;
        
        case MNATFWIceSettings::EPrefIpv4AddrFamily:
            iIpv4AddrFamilyPref = aValue;
            break;
        
        case MNATFWIceSettings::EPrefIpv6AddrFamily:
            iIpv6AddrFamilyPref = aValue;
            break;

        case MNATFWIceSettings::EPrefVpnInterface:
            iVpnInterfacePref = aValue;
            break;

        case MNATFWIceSettings::EPrefUdpTransport:
            iUdpTransportPref = aValue;
            break;
        
        case MNATFWIceSettings::EPrefTcpTransport:
            iTcpTransportPref = aValue;
            break;

        case MNATFWIceSettings::EPrefTcpDirectionActive:
            iTcpDirActivePref = aValue;
            break;
        
        case MNATFWIceSettings::EPrefTcpDirectionPassive:
            iTcpDirPassivePref = aValue;
            break;
        
        case MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen:
            iTcpDirSimultaneousOpenPref = aValue;
            break;
        
        default:
            return KErrNotSupported;
        }
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// TNATFWIcePreferences::GetPrefValue
// ---------------------------------------------------------------------------
//
TInt TNATFWIcePreferences::GetPrefValue( 
        MNATFWIceSettings::TPrefSetting aSetting, 
        TInt& aValue ) const
    {
    switch ( aSetting )
        {
        case MNATFWIceSettings::EPrefHostCandidate:
            aValue = iHostPref;
            break;
        
        case MNATFWIceSettings::EPrefServerReflexiveCandidate:
            aValue = iServerReflexivePref;
            break;
        
        case MNATFWIceSettings::EPrefRelayCandidate:
            aValue = iRelayPref;
            break;
        
        case MNATFWIceSettings::EPrefPeerReflexiveCandidate:
            aValue = iPeerReflexivePref;
            break;
        
        case MNATFWIceSettings::EPrefIpv4AddrFamily:
            aValue = iIpv4AddrFamilyPref;
            break;
        
        case MNATFWIceSettings::EPrefIpv6AddrFamily:
            aValue = iIpv6AddrFamilyPref;
            break;

        case MNATFWIceSettings::EPrefVpnInterface:
            aValue = iVpnInterfacePref;
            break;

        case MNATFWIceSettings::EPrefUdpTransport:
            aValue = iUdpTransportPref;
            break;
        
        case MNATFWIceSettings::EPrefTcpTransport:
            aValue = iTcpTransportPref;
            break;

        case MNATFWIceSettings::EPrefTcpDirectionActive:
            aValue = iTcpDirActivePref;
            break;
        
        case MNATFWIceSettings::EPrefTcpDirectionPassive:
            aValue = iTcpDirPassivePref;
            break;
        
        case MNATFWIceSettings::EPrefTcpDirectionSimultaneousOpen:
            aValue = iTcpDirSimultaneousOpenPref;
            break;
        
        default:
            return KErrNotSupported;
        }
    
    return KErrNone;
    }
