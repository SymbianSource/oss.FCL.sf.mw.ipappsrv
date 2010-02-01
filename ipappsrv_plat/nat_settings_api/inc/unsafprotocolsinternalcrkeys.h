/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef UNSAFPROTOCOLSINTERNALCRKEYS_H
#define UNSAFPROTOCOLSINTERNALCRKEYS_H


#include <e32std.h>


/**
 * Defines configuration parameters used by the UNSAF Protocols.
 */
const TUid KCRUidUNSAFProtocols = { 0x102747d2 };


/*
 * Mask for the less significant bits in UNSAF Protocols setting keys.
 * these bits are reserved for keys which indicates
 * variable amount of the domain settings  
 */
const TUint32 KUNSAFProtocolsFieldTypeMask = 0xfffff000;


/* 
 * Mask for the subtable field keys
 * these bits are reserved for keys which indicates variable amount of
 * server and plug-in settings
 */
const TUint32 KUNSAFProtocolsSubTableFieldTypeMask = 0xfff00fff;


/*
 * Mask for Domain or IAP table field keys.
 * These bits are reserved for keys that indicate separate fields under
 * one table.
 */
const TUint32 KUNSAFProtocolsTableMask = 0xf0000fff;


/*
 * Mask for Domain or IAP table field keys.
 * These bits are reserved for the actual key part
 */
const TUint32 KUNSAFProtocolsKeyMask = 0xff000fff;


/*
 * Mask for subtable field keys.
 * These bits are reserved for the actual key part.
 */
const TUint32 KUNSAFProtocolsSubKeyMask = 0xff0fffff;


// Domain Specific settings keys

/** 
 * Domain table
 * Mask for domain table in UNSAF Protocols settings
 */
const TUint32 KUNSAFProtocolsDomainTableMask = 0x10000000;


/** 
 * Mask for domain field in UNSAF Protocols domain settings
 * Field type is TDesC8.
 * Field is used as the key of the table.
 */
const TUint32 KUNSAFProtocolsDomainMask = 0x10001000;


/**
 * Mask for indicating which NAT protocol should be used and whitch
 * priority order.
 * Format is x,x,x,...
 * example: nokia.stun,nokia.turn,nokia.ice
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsUsedNATProtocolMask = 0x10002000;


/**
 * Mask for indicating whether the STUN/TURN server supports TLS and
 * shared secret. Non-zero value indicates that 
 * the STUN server does NOT support TLS and shared secret.
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsDomainSharedSecretNotSupported = 0x10003000;


/**
 * Mask for indicating that the latest binding request attempt failed 
 * Non-zero value indicates fail.
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsDomainBindingRequestFailed = 0x10004000;


/**
 * Mask for indicating start port of the port range 
 * All instances of the NATFW uses ports of this port range  
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsPortPoolStartPortMask  = 0x10005000;


/**
 * Mask for indicating end port of the NATFW port range
 * All instances of the NATFW uses ports of this port range  
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsPortPoolEndPortMask = 0x10006000;


/**
 * Mask for Host type preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsHostPref = 0x10022000;


/**
 * Mask for Server-reflexive type preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsServerRefPref = 0x10023000;


/**
 * Mask for Relay type Preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsRelayPref = 0x10024000;


/**
 * Mask for Peer-reflexive type Preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsPeerRefPref = 0x10025000;


/**
 * Mask for IPV4 local preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsIPV4Pref = 0x10026000;


/**
 * Mask for IPV6 local preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsIPV6Pref = 0x10027000;


/**
 * Mask for VPN local preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsVPNPref = 0x10028000;


/**
 * Mask for UDP transport preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsUDPPref = 0x10029000;


/**
 * Mask for TCP transport preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsTCPPref = 0x1002a000;


/**
 * Mask for TCP-active direction preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsTCPActivePref = 0x1002b000;


/**
 * Mask for TCP-passive direction preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsTCPPassivePref = 0x1002c000;


/**
 * Mask for TCP-simultaneous-open direction preference value in the ICE
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsTCPSimultPref = 0x1002d000;


/**
 * Mask for NAT utilities used by ICE.
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsNATUtilities = 0x1002e000;


/** 
 * Mask for latest connected STUN server field in
 * UNSAF Protocols domain settings
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsLatestConnectedSTUNServerAddressMask = 
    0x10047000;


/** 
 * Mask for latest connected STUN server port field in
 * UNSAF Protocols domain settings
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsLatestConnectedSTUNServerPortMask = 0x10048000;


/** 
 * Mask for latest connected TURN server field in
 * UNSAF Protocols domain settings
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsLatestConnectedTURNServerAddressMask = 
    0x10049000;


/** 
 * Mask for latest connected TURN server port field in
 * UNSAF Protocols domain settings
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsLatestConnectedTURNServerPortMask = 0x1004a000;


/*
* Mask for STUN server field in UNSAF Protocols domain settings
* Field type is TDesC8.
* Field is optional.
*/
const TUint32 KUNSAFProtocolsSTUNServerMask = 0x1004b000;


/* 
* Mask for STUN server port field in UNSAF Protocols domain settings
* Field type is TUint.
* Field is optional.
*/
const TUint32 KUNSAFProtocolsSTUNServerPortMask = 0x1004c000;


/*
* Mask for NAT refresh interval for UDP in UNSAF Protocols domain settings
* Field type is TInt. The unit of measure is seconds.
* Field is optional.
*/
const TUint32 KUNSAFProtocolsDomainIntervalUDPMask = 0x1004d000;


/*
* Mask for NAT refresh interval for TCP in UNSAF Protocols domain settings
* Field type is TInt. The unit of measure is seconds.
* Field is optional.
*/
const TUint32 KUNSAFProtocolsDomainIntervalTCPMask = 0x1004e000;


/*
* Mask for enabling CRLF-based NAT binding refresh over UDP 
* to SIP proxy when no STUN server is available or
* the STUN server returns the local IP address of the terminal.
* Field type is TInt. Non-zero value indicates that the refresh is enabled.
* Field is optional.
*/
const TUint32 KUNSAFProtocolsDomainEnableCRLFRefresh = 0x1004f000;


/**
 * Mask for Retransmission Timeout setting for Stun in domain settings
 * The unit of measure is mseconds.
 * Field type is TInt. 
 */
const TUint32 KUNSAFProtocolsStunRetransmissionTimeoutMask = 0x10050000;


/**
 * Mask for Retransmission Timeout setting for Turn in domain settings
 * The unit of measure is mseconds.
 * Field type is TInt. 
 */
const TUint32 KUNSAFProtocolsTurnRetransmissionTimeoutMask = 0x10051000;



// STUN Server Specific settings in the domain

/**
 * STUN Server table
 */
const TUint32 KUNSAFProtocolsSTUNServerTableMask = 0x11000000;


/**
 * Mask for server address field in server settings
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsSTUNAddressMask = 0x11100000;


/**
 * Mask for server port field in server settings
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsSTUNPortMask = 0x11200000;


/**
 * Mask for Connectivity Check Timeout setting in server settings
 * The unit of measure is seconds.
 * Field type is TInt. 
 */
const TUint32 KUNSAFProtocolsSTUNConnectivityCheckTimeoutMask = 0x11500000;


/**
 * Mask for server username field in server settings
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsSTUNUsernameMask = 0x11700000;


/**
 * Mask for server password field in server settings
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsSTUNPasswordMask = 0x11800000;


// TURN Server specific settings keys in the domain

/**
 * TURN Server table
 */
const TUint32 KUNSAFProtocolsTURNServerTableMask = 0x12000000;


/** 
 * Mask for server address field in server settings
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsTURNAddressMask = 0x12100000;


/**
 * Mask for server port field in server settings
 * Field type is TInt.
 */
const TUint32 KUNSAFProtocolsTURNPortMask = 0x12200000;


/**
 * Mask for Connectivity Check Timeout setting in server settings
 * The unit of measure is seconds.
 * Field type is TInt. 
 */
const TUint32 KUNSAFProtocolsTURNConnectivityCheckTimeoutMask = 0x12500000;


/**
 * Mask for server username field in server settings
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsTURNUsernameMask= 0x12700000;


/**
 * Mask for server password field in server settings
 * Field type is TDesC8.
 */
const TUint32 KUNSAFProtocolsTURNPasswordMask = 0x12800000;


// IAP specific settings keys

/**
 * IAP table
 * { IAP ID, IntervalUDP, IntervalTCP, RetransmitInterval }
 * Mask for IAP table in UNSAF Protocols settings
 */
const TUint32 KUNSAFProtocolsIAPTableMask = 0x20000000;


/**
 * Mask for IAP ID field in UNSAF Protocols IAP settings
 * Field type is TUint32.
 * Field is used as the key of the table.
 */
const TUint32 KUNSAFProtocolsIAPIdMask = 0x20001000;


/**
 * Mask for NAT refresh interval for UDP in UNSAF Protocols IAP settings
 * Field type is TInt. The unit of measure is seconds.
 */
const TUint32 KUNSAFProtocolsIntervalUDPMask = 0x20002000;


/**
 * Mask for NAT refresh interval for TCP in UNSAF Protocols IAP settings
 * Field type is TInt. The unit of measure is seconds.
 */
const TUint32 KUNSAFProtocolsIntervalTCPMask = 0x20003000;


/**
 * Mask for initial STUN request retransmit timer in UNSAF Protocols
 * IAP settings
 * Field type is TInt. The unit of measure is milliseconds.
 */
const TUint32 KUNSAFProtocolsSTUNRetransmitTimerMask = 0x20004000;


#endif // UNSAFPROTOCOLSINTERNALCRKEYS_H
