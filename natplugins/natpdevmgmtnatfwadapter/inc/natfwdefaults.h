/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Holds default values for DM NAT/FW Traversal adapter.
*
*/


#include <e32base.h>

// Common defaults.
const TInt KDefaultUDPRefreshInterval      = 28;   // seconds
const TInt KDefaultTCPRefreshInterval      = 1200; // seconds (20 minutes)

// IAP specific defaults.
const TInt KDefaultSTUNRetransmitTimer     = 250; // milliseconds

// Domain specific defaults.
_LIT8( KDefaultSTUNSrvAddr,                 "0.0.0.0" );
const TInt KDefaultCRLFRefreshInterval     = 0;
const TInt KDefaultSTUNPort                = 3478;
const TInt KDefaultDisableStunSharedSecret = 0;
const TInt KDefaultStartPort               = 49152;
const TInt KDefaultEndPort                 = 49200;
_LIT8( KDefaultNatProtocol,                 "nokia.stun" );
// ICE specific defaults.
_LIT8( KDefaultNatUtilities,                "nokia.stun,nokia.turn" );
const TInt KDefaultHostPref                = 126;
const TInt KDefaultServerReflexivePref     = 100;
const TInt KDefaultRelayPref               = 0;
const TInt KDefaultPeerReflexivePref       = 110;
const TInt KDefaultIPv4Pref                = 60;
const TInt KDefaultIPv6Pref                = 40;
const TInt KDefaultVPNPref                 = 20;
const TInt KDefaultUDPPref                 = 15;
const TInt KDefaultTCPPref                 = 6;
const TInt KDefaultTCPActivePref           = 5;
const TInt KDefaultTCPPassivePref          = 2;
const TInt KDefaultTCPSimultaneousPref     = 7;
// TURN specific defaults.
_LIT8( KDefaultTURNSrvAddr,                 "0.0.0.0" );
const TInt KDefaultTURNSrvPort             = 3478;
