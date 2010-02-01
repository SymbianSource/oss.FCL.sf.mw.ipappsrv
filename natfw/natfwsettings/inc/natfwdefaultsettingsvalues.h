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
* Description:    Default settings values
*
*/





#ifndef NATFWDEFAULTSETTINGSVALUES_H
#define NATFWDEFAULTSETTINGSVALUES_H

#include <e32std.h>

// Domain / TURN Default
const TInt KNATFWDefaultTURNTimerValue      = 0;

// Domain / STUN & TURN Server default
const TInt KNATFWDefaultSTUNServerPort      = 3478;
const TInt KNATFWDefaultTURNServerPort      = 0;

// Domain / ICE Default
_LIT8( KNATFWDefaultIceUsedPlugins, "nokia.stun,nokia.turn" );
const TInt KNATFWDefaultICECanditateTimer   = 20;
const TInt KNATFWDefaultlHostPref           = 100;
const TInt KNATFWDefaultServerRefPref       = 110;
const TInt KNATFWDefaultRelayPref           = 0;
const TInt KNATFWDefaultPeerRefPref         = 126;
const TInt KNATFWDefaultIPV4Pref            = 0;
const TInt KNATFWDefaultIPV6Pref            = 0;
const TInt KNATFWDefaultVPNPref             = 0;
const TInt KNATFWDefaultUDPPref             = 15;
const TInt KNATFWDefaultTCPPref             = 6;
const TInt KNATFWDefaultTCPActivePref       = 5;
const TInt KNATFWDefaultTCPPassivePref      = 2;
const TInt KNATFWDefaultTCPSimultPref       = 7;

// Domain default
const TInt KNATFWDefaultRtoValue            = 20;
const TInt KNATFWDefaultRefreshIntervalTcp  = 1200;
const TInt KNATFWDefaultRefreshIntervalUdp  = 28;
const TBool KNATFWDefaultCRLFRefreshEnabled = EFalse;
const TInt KNATFWDefaultPortFieldLength     = 10;
const TInt KNATFWDefaultPortPoolStartPort    = 49252;
const TInt KNATFWDefaultPortPoolEndPort     = 65535;
const TBool KNATFWDefaultUseSharedSecret    = EFalse;
_LIT8( KNATFWDefaultPriorityOrder, "nokia.stun,nokia.turn,nokia.ice" );

// IAP Default
const TInt KNATFWDefaultStunRetransmitTimer = 250;
const TInt KNATFWDefaultIapRefreshIntervalUdp  = 28;
const TInt KNATFWDefaultIapRefreshIntervalTcp  = 1200;


#endif // NATFWDEFAULTSETTINGSVALUES_H
