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
* Description:    Interface for querying ICE related software settings.
*
*/





#ifndef M_MNATFWICESETTINGS_H
#define M_MNATFWICESETTINGS_H


#include <e32base.h>

class CDesC8Array;


/**
 *  Interface for querying ICE related software settings.
 *  
 *  Interface is not intended for derivation outside NATFW.
 *  
 *  @lib natfwnatsettings.dll
 *  @since S60 v3.2
 */
class MNATFWIceSettings
    {

public:

    /** Special value for the setting indicating that use of that particular
    type of candidate is blocked. */
    static const TInt KPrefValueDisabled = -1;
    
    /** Defines available preference settings. */
    enum TPrefSetting
        {
        /** Type preference of host candidate. */
        EPrefHostCandidate                  = 1,
        
        /** Type preference of server reflexive candidate. */
        EPrefServerReflexiveCandidate       = 2,
        
        /** Type preference of relay candidate. */
        EPrefRelayCandidate                 = 3,
        
        /** Type preference of peer reflexive candidate. */
        EPrefPeerReflexiveCandidate         = 4,
        
        /** IPv4 interface preference. */
        EPrefIpv4AddrFamily                 = 5,
        
        /** IPv6 interface preference. */
        EPrefIpv6AddrFamily                 = 6,
        
        /** VPN interface preference. */
        EPrefVpnInterface                   = 7,
        
        /** UDP transport protocol preference. */
        EPrefUdpTransport                   = 8,
        
        /** TCP transport protocol preference. */
        EPrefTcpTransport                   = 9,
        
        /** TCP active direction preference of TCP-based candidate. */
        EPrefTcpDirectionActive             = 10,
        
        /** TCP passive direction preference of TCP-based candidate. */
        EPrefTcpDirectionPassive            = 11,
        
        /** TCP simultaneous open direction preference of TCP-based
        candidate. */
        EPrefTcpDirectionSimultaneousOpen   = 12
        };

    /**
     * Returns preference value for a specifed setting.
     *
     * @since   S60 v3.2
     * @param   aSetting    Setting for which to return preference value
     * @param   aValue      Preference value if query was valid
     * @return  KErrNotSupported if setting is not known, KErrNone otherwise
     */
    virtual TInt GetPrefValue( 
        TPrefSetting aSetting, TInt& aValue ) const = 0;
    
    /**
     * Returns array of identifiers of utility plugins available for ICE.
     * 
     * @since   S60 v3.2
     * @return  Utilities available for ICE
     */
    virtual const CDesC8Array& NatUtilityPlugins() const = 0;
    
    };


#endif // M_MNATFWICESETTINGS_H
