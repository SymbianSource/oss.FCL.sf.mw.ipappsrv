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




#ifndef T_TNATFWICEPREFERENCES_H
#define T_TNATFWICEPREFERENCES_H

#include <mnatfwicesettings.h>

/**
 *  Encapsulates preference settings for ICE prioritization.
 *
 *  @lib natfwnatsettings.dll
 *  @since S60 v3.2
 */
class TNATFWIcePreferences
    {

public:

    TNATFWIcePreferences();

    /**
     * Sets preference value for a specified setting.
     *
     * @since   S60 v3.2
     * @param   aSetting    Setting for which to set value
     * @param   aValue      Preference value
     * @return  KErrNotSupported if setting is not known, KErrNone otherwise
     */
    TInt SetPrefValue( MNATFWIceSettings::TPrefSetting aSetting, 
        TInt aValue );

    /**
     * Returns preference value for a specifed setting.
     *
     * @since   S60 v3.2
     * @param   aSetting    Setting for which to return preference value
     * @param   aValue      Preference value if query was valid
     * @return  KErrNotSupported if setting is not known, KErrNone otherwise
     */
    TInt GetPrefValue( MNATFWIceSettings::TPrefSetting aSetting, 
        TInt& aValue ) const;

private: // data

    /**
     * Type preference of host candidate.
     */
    TInt iHostPref;

    /**
     * Type preference of server-reflexive candidate.
     */
    TInt iServerReflexivePref;

    /**
     * Type preference of relay candidate.
     */
    TInt iRelayPref;

    /**
     * Type preference of peer-reflexive candidate.
     */
    TInt iPeerReflexivePref;

    /**
     * Local preference for candidates from IPv4 'interface'.
     */
    TInt iIpv4AddrFamilyPref;

    /**
     * Local preference for candidates from IPv6 'interface'.
     */
    TInt iIpv6AddrFamilyPref;

    /**
     * Local preference for candidates from VPN interface.
     */
    TInt iVpnInterfacePref;

    /**
     * Transport preference for UDP-based candidates.
     */
    TInt iUdpTransportPref;

    /**
     * Transport preference for TCP-based candidates.
     */
    TInt iTcpTransportPref;

    /**
     * TCP-active direction preference.
     */
    TInt iTcpDirActivePref;

    /**
     * TCP-passive direction preference.
     */
    TInt iTcpDirPassivePref;

    /**
     * TCP-simultaneous-open direction preference.
     */
    TInt iTcpDirSimultaneousOpenPref;
    };

#endif // T_TNATFWICEPREFERENCES_H
