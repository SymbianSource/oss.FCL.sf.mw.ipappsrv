/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Device Management NAT-FW Traversal Adapter
*
*/


// INCLUDE FILES
#include <utf.h>
#include <sysutil.h>
#include <f32file.h>
#include <shareddataclient.h>
#include "nsmldmnatfwtraversaladapter.h"
#include "nsmldmiapmatcher.h"
#include "natfwdomainentry.h"
#include "natfwiapentry.h"
#include "natfwdefaults.h" // Default values.

#ifdef VOIP_TRACE_ENABLED
#include <voip_trace.h>
#endif // VOIP_TRACE_ENABLED

// Central Repository keys for NAT-FW Traversal settings
#include "centralrepository.h"
#include <unsafprotocolscrkeys.h>

// Following lines are for enabling debug prints.
#ifdef _DEBUG
#define DBG_PRINT( p ) RDebug::Print( _L( p ) )
#define DBG_PRINT_2( p,i ) RDebug::Print ( _L( p ), i )
#else
#define DBG_PRINT( p )
#define DBG_PRINT_2( p,i )
#endif // _DEBUG

// LOCAL CONSTANTS

const TInt KNSmlNATFWMaxUriLength       = 256;
const TInt KNSmlNATFWMaxResultLength    = 256;
const TInt KDefaultResultSize           = 64;
const TInt KOneSeg                      = 1;
const TInt KTwoSegs                     = 2;
const TInt KThreeSegs                   = 3;
const TInt KFourSegs                    = 4;
const TInt KFiveSegs                    = 5;
const TInt KSixSegs                     = 6;
const TInt KSevenSegs                   = 7;
const TInt KLUIDMaxLength               = 256;

// Reference: OMA_Device_Management_DDF_for_NAT-FW_v1.0.doc
_LIT8( KNSmlDMNATFWDDFVersion,          "1.0" ); 
_LIT8( KNSmlDMNATFWNodeName,            "NATFW" );

// Domain specific settings
_LIT8( KNSmlDMDomainSpecificNodeName,   "DomainSpecific" );
_LIT8( KNSmlDMDomain,                   "Domain" );
_LIT8( KNSmlDMSTUNSrvAddr,              "STUNSrvAddr" );
_LIT8( KNSmlDMSTUNSrvPort,              "STUNSrvPort" );
_LIT8( KNSmlDMDomainNATRefreshTCP,      "NATRefreshTCP" );
_LIT8( KNSmlDMDomainNATRefreshUDP,      "NATRefreshUDP" );
_LIT8( KNSmlDMDomainEnableCRLFRefresh,  "EnableCRLFRefresh" );

// IAP specific settings
_LIT8( KNSmlDMIAPSpecificNodeName,      "IAPSpecific" );
_LIT8( KNSmlDMNATPrefConRef,            "PrefConRef" );
_LIT8( KNSmlDMNATRefreshTCP,            "NATRefreshTCP" );
_LIT8( KNSmlDMNATRefreshUDP,            "NATRefreshUDP" );
_LIT8( KNSmlDMSTUNRetransmit,           "STUNRetransmit" );

// Domain specific settings.
_LIT8( KNSmlDMSTUNUsername,             "STUNUsername" );
_LIT8( KNSmlDMSTUNPassword,             "STUNPassword" );
_LIT8( KNSmlDMSTUNSharedSecret,         "STUNSharedSecret" );
_LIT8( KNSmlDMPortPoolStart,            "PortPoolStartPort" );
_LIT8( KNSmlDMPortPoolEnd,              "PortPoolEndPort" );
_LIT8( KNSmlDMUsedNatProtocol,          "UsedNATProtocol" );
// Domain / STUN specific settings.
_LIT8( KNSmlDMAdditionalSTUNNodeName,   "AdditionalSTUNServer" );
// Domain / ICE specific settings.
_LIT8( KNSmlDMICESpecificNodeName,      "ICESpecific" );
_LIT8( KNSmlDMNatUtilities,             "NATUtilities" );
_LIT8( KNSmlDMHostPref,                 "HostPreference" );
_LIT8( KNSmlDMServerReflexPref,         "ServerReflexivePreference" );
_LIT8( KNSmlDMRelayPref,                "RelayPreference" );
_LIT8( KNSmlDMPeerReflexPref,           "PeerReflexivePreference" );
_LIT8( KNSmlDMIPv4Pref,                 "IPv4Preference" );
_LIT8( KNSmlDMIPv6Pref,                 "IPv6Preference" );
_LIT8( KNSmlDMVpnPref,                  "VPNPreference" );
_LIT8( KNSmlDMUdpPref,                  "UDPPreference" );
_LIT8( KNSmlDMTcpPref,                  "TCPPreference" );
_LIT8( KNSmlDMTcpActivePref,            "TCPActivePreference" );
_LIT8( KNSmlDMTcpPassivePref,           "TCPPassivePreference" );
_LIT8( KNSmlDMTcpSimultPref,            "TCPSimultaneousPreference" );
// Domain / TURN specific settings.
_LIT8( KNSmlDMTURNSpecificNodeName,     "TURNSpecific" );
_LIT8( KNSmlDMTURNServerNodeName,       "TURNServer" );
_LIT8( KNSmlDMTURNSrvAddr,              "TURNSrvAddr" );
_LIT8( KNSmlDMTURNSrvPort,              "TURNSrvPort" );
_LIT8( KNSmlDMTURNUsername,             "TURNUsername" );
_LIT8( KNSmlDMTURNPassword,             "TURNPassword" );

// Explanations of settings
_LIT8( KNSmlDMNATFWNodeNameExp,                 
    "node for NAT-FW Traversal settings" );
_LIT8( KNSmlDMDomainSpecificNodeNameExp,        
    "Domain Specific settings node" );
_LIT8( KNSmlDMDomainSpecificDynamicNodeExp,     
    "placeholder for all domain specific settings" );
_LIT8( KNSmlDMDomainExp,                        
    "Domain field" );
_LIT8( KNSmlDMSTUNSrvAddrExp,                   
    "STUN server address" );
_LIT8( KNSmlDMSTUNSrvPortExp,                   
    "STUN server port" );
_LIT8( KNSmlDMDomainNATRefreshTCPExp,           
    "NAT refresh interval for TCP in domain settings" );
_LIT8( KNSmlDMDomainNATRefreshUDPExp,              
    "NAT refresh interval for UDP in domain settings" );
_LIT8( KNSmlDMDomainEnableCRLFRefreshExp,       
    "For enabling CRLF-based NAT binding refresh" );
_LIT8( KNSmlDMIAPSpecificNodeNameExp,           
    "Access Point Specific settings node" );
_LIT8( KNSmlDMIAPSpecificDynamicNodeExp,        
    "placeholder for all IAP specific settings" );
_LIT8( KNSmlDMNATPrefConRefExp,                 
    "Link to IAP" );
_LIT8( KNSmlDMNATRefreshTCPExp,                 
    "NAT refresh interval for TCP" );
_LIT8( KNSmlDMNATRefreshUDPExp,                 
    "NAT refresh interval for UDP" );
_LIT8( KNSmlDMSTUNRetransmitExp,                
    "STUN request retransmit timer" );

// Domain
_LIT8( KNSmlDMSTUNUsernameExp,
    "STUN server username" );
_LIT8( KNSmlDMSTUNPasswordExp,
    "STUN server password" );
_LIT8( KNSmlDMSTUNSharedSecretExp,
    "STUN shared secret mechanism used or not" );
_LIT8( KNSmlDMPortPoolStartExp,
    "Start port of port pool" );
_LIT8( KNSmlDMPortPoolEndExp,
    "End port of port pool" );
_LIT8( KNSmlDMUsedNatProtocolExp,
    "Used NAT protocol" );
// AdditionalSTUNServer
_LIT8( KNSmlDMAdditionalSTUNNodeNameExp,
    "Additional STUN server settings" );
_LIT8( KNSmlDMAdditionalSTUNDynamicNodeExp,
    "placeholder for additional STUN server settings" );
// ICE
_LIT8( KNSmlDMICESpecificNodeNameExp,
    "ICE specific settings node" );
_LIT8( KNSmlDMNatUtilitiesExp,
    "NAT utilities to be used by ICE" );
_LIT8( KNSmlDMHostPrefExp,
    "HostPreference" );
_LIT8( KNSmlDMServerReflexPrefExp,
    "ServerReflexivePreference" );
_LIT8( KNSmlDMRelayPrefExp,
    "RelayPreference" );
_LIT8( KNSmlDMPeerReflexPrefExp,
    "PeerReflexivePreference" );
_LIT8( KNSmlDMIPv4PrefExp,
    "IPv4Preference" );
_LIT8( KNSmlDMIPv6PrefExp,
    "IPv6Preference" );
_LIT8( KNSmlDMVpnPrefExp,
    "VPNPreference" );
_LIT8( KNSmlDMUdpPrefExp,
    "UDPPreference" );
_LIT8( KNSmlDMTcpPrefExp,
    "TCPPreference" );
_LIT8( KNSmlDMTcpActivePrefExp,
    "TCPActivePreference" );
_LIT8( KNSmlDMTcpPassivePrefExp,
    "TCPPassivePreference" );
_LIT8( KNSmlDMTcpSimultPrefExp,
    "TCPSimultaneousPreference" );
// TURN
_LIT8( KNSmlDMTURNSpecificNodeNameExp,
    "TURN specific settings node" );
_LIT8( KNSmlDMTURNServerNodeNameExp,
    "TURN server settings node" );
_LIT8( KNSmlDMTURNServerDynamicNodeExp,
    "placeholder for TURN server settings" );
_LIT8( KNSmlDMTURNSrvAddrExp,
    "TURN server address" );
_LIT8( KNSmlDMTURNSrvPortExp,
    "TURN server port" );
_LIT8( KNSmlDMTURNUsernameExp,
    "TURN username" );
_LIT8( KNSmlDMTURNPasswordExp,
    "TURN password" );

// Other
_LIT8( KNSmlDMNATFWValueTrue,   "True" );
_LIT8( KNSmlDMNATFWValueFalse,  "False" );
_LIT8( KNSmlDMNATFWValueNotSet, "NotSet" );
_LIT8( KNSmlDMNATFWTextPlain,   "text/plain" );
_LIT8( KNSmlDMNATFWSeparator,   "/" );
_LIT8( KNSmlDMNATFWUriDotSlash, "./" );
//MACROS 
const TUint8 KDMNATFWSeparator = '/';

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter* CNSmlDmNATFWTraversalAdapter::NewL
//
// ---------------------------------------------------------------------------
//
CNSmlDmNATFWTraversalAdapter* CNSmlDmNATFWTraversalAdapter::NewL( 
    MSmlDmCallback* aDmCallback )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::NewL(): begin");
    CNSmlDmNATFWTraversalAdapter* self = NewLC( aDmCallback ); // CS:1
    CleanupStack::Pop( self ); // CS:0
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::NewL(): end");
    return self;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter* CNSmlDmNATFWTraversalAdapter::NewLC
//
// ---------------------------------------------------------------------------
//
CNSmlDmNATFWTraversalAdapter* CNSmlDmNATFWTraversalAdapter::NewLC( 
    MSmlDmCallback* aDmCallback )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::NewLC(): begin");
    CNSmlDmNATFWTraversalAdapter* self = 
        new ( ELeave ) CNSmlDmNATFWTraversalAdapter( aDmCallback );
    CleanupStack::PushL( self ); // CS:1
    self->iDmCallback = aDmCallback;
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::NewLC(): end");
    return self;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::~CNSmlDmNATFWTraversalAdapter
//
// ---------------------------------------------------------------------------
//
CNSmlDmNATFWTraversalAdapter::~CNSmlDmNATFWTraversalAdapter()
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::~CNSmlDmNATFWTraversalAdapter():\
 begin");
    if ( iNATFWDomainEntries )
        {
        iNATFWDomainEntries->ResetAndDestroy();
        delete iNATFWDomainEntries;    
        }
    if ( iNATFWIAPEntries )
        {
        iNATFWIAPEntries->ResetAndDestroy();
        delete iNATFWIAPEntries;    
        }
    iNATFWCommandBuffer.Close();
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::~CNSmlDmNATFWTraversalAdapter():\
 end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter
// 
// ---------------------------------------------------------------------------
//
CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter(
    TAny* aEcomArguments ) : CSmlDmAdapter( aEcomArguments )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter( \
aEcomArguments ): begin");
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter( \
aEcomArguments ): end"); 
    }
    
// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::DDFVersionL
// Inserts DDF version of the adapter to aDDFVersion
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::DDFVersionL(
    CBufBase& aDDFVersion )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::DDFVersionL(TDes& aDDFVersion):\
 begin");
    aDDFVersion.InsertL( 0, KNSmlDMNATFWDDFVersion );
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::DDFVersionL(TDes& aDDFVersion):\
 end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::DDFStructureL
// Builds the DDF structure of adapter
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::DDFStructureL( MSmlDmDDFObject& aDDF )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::DDFStructureL(): begin");

    // Default access types.
    TSmlDmAccessTypes accessTypesGet;
    accessTypesGet.SetGet();

    TSmlDmAccessTypes accessTypesGetAdd;
    accessTypesGetAdd.SetGet();
    accessTypesGetAdd.SetAdd();

    TSmlDmAccessTypes accessTypesGetReplaceAdd;
    accessTypesGetReplaceAdd.SetGet();
    accessTypesGetReplaceAdd.SetReplace();
    accessTypesGetReplaceAdd.SetAdd();

    TSmlDmAccessTypes accessTypesAll;
    accessTypesAll.SetGet();
    accessTypesAll.SetDelete();
    accessTypesAll.SetAdd();
    accessTypesAll.SetReplace();

    TSmlDmAccessTypes accessTypesNoDelete;
    accessTypesNoDelete.SetGet();
    accessTypesNoDelete.SetAdd();
    accessTypesNoDelete.SetReplace();

    // NATFWTraversal (./NATFW)
    MSmlDmDDFObject& natFwTraversal = aDDF.AddChildObjectL( 
        KNSmlDMNATFWNodeName );
    natFwTraversal.SetAccessTypesL( accessTypesGetAdd );
    natFwTraversal.SetOccurenceL( MSmlDmDDFObject::EOne );
    natFwTraversal.SetScopeL( MSmlDmDDFObject::EPermanent );
    natFwTraversal.SetDFFormatL( MSmlDmDDFObject::ENode );
    natFwTraversal.SetDescriptionL( KNSmlDMNATFWNodeNameExp );

    // DomainSpecific (./NATFW/DomainSpecific)
    MSmlDmDDFObject& domainSpecific = natFwTraversal.AddChildObjectL( 
        KNSmlDMDomainSpecificNodeName );
    domainSpecific.SetAccessTypesL( accessTypesGetAdd );
    domainSpecific.SetOccurenceL( MSmlDmDDFObject::EOne );
    domainSpecific.SetScopeL( MSmlDmDDFObject::EPermanent );
    domainSpecific.SetDFFormatL( MSmlDmDDFObject::ENode );
    domainSpecific.SetDescriptionL( KNSmlDMDomainSpecificNodeNameExp );

    // dynamic settings ID node ( ./NATFW/DomainSpecific/<x> )
    MSmlDmDDFObject& dynamicDomainNode = 
        domainSpecific.AddChildObjectGroupL();
    dynamicDomainNode.SetAccessTypesL( accessTypesAll );
    dynamicDomainNode.SetOccurenceL( MSmlDmDDFObject::EZeroOrMore );
    dynamicDomainNode.SetScopeL( MSmlDmDDFObject::EDynamic );
    dynamicDomainNode.SetDFFormatL( MSmlDmDDFObject::ENode );
    dynamicDomainNode.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    dynamicDomainNode.SetDescriptionL( KNSmlDMDomainSpecificDynamicNodeExp );

    // DomainSpecific/<x>/Domain
    MSmlDmDDFObject& domain = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMDomain );
    domain.SetAccessTypesL( accessTypesGetReplaceAdd );
    domain.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    domain.SetScopeL( MSmlDmDDFObject::EDynamic );
    domain.SetDFFormatL( MSmlDmDDFObject::EChr );
    domain.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    domain.SetDescriptionL( KNSmlDMDomainExp );

    // DomainSpecific/<x>/STUNSrvAddr
    MSmlDmDDFObject& stunSrvAddr = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMSTUNSrvAddr );
    stunSrvAddr.SetAccessTypesL( accessTypesGetReplaceAdd );
    stunSrvAddr.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    stunSrvAddr.SetScopeL( MSmlDmDDFObject::EDynamic );
    stunSrvAddr.SetDFFormatL( MSmlDmDDFObject::EChr );
    stunSrvAddr.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    stunSrvAddr.SetDescriptionL( KNSmlDMSTUNSrvAddrExp );

    // DomainSpecific/<x>/STUNSrvPort
    MSmlDmDDFObject& stunSrvPort = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMSTUNSrvPort );
    stunSrvPort.SetAccessTypesL( accessTypesGetReplaceAdd );
    stunSrvPort.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    stunSrvPort.SetScopeL( MSmlDmDDFObject::EDynamic );
    stunSrvPort.SetDFFormatL( MSmlDmDDFObject::EInt );
    stunSrvPort.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    stunSrvPort.SetDescriptionL( KNSmlDMSTUNSrvPortExp );

    // DomainSpecific/<x>/NATRefreshTCP
    MSmlDmDDFObject& domainRefreshTCP = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMDomainNATRefreshTCP );
    domainRefreshTCP.SetAccessTypesL( accessTypesGetReplaceAdd );
    domainRefreshTCP.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    domainRefreshTCP.SetScopeL( MSmlDmDDFObject::EDynamic );
    domainRefreshTCP.SetDFFormatL( MSmlDmDDFObject::EInt );
    domainRefreshTCP.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    domainRefreshTCP.SetDescriptionL( KNSmlDMDomainNATRefreshTCPExp );

    // DomainSpecific/<x>/NATRefreshUDP
    MSmlDmDDFObject& domainRefreshUDP = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMDomainNATRefreshUDP );
    domainRefreshUDP.SetAccessTypesL( accessTypesGetReplaceAdd );
    domainRefreshUDP.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    domainRefreshUDP.SetScopeL( MSmlDmDDFObject::EDynamic );
    domainRefreshUDP.SetDFFormatL( MSmlDmDDFObject::EInt );
    domainRefreshUDP.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    domainRefreshUDP.SetDescriptionL( KNSmlDMDomainNATRefreshUDPExp );

    // DomainSpecific/<x>/EnableCRLFRefresh
    MSmlDmDDFObject& enableCRLF = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMDomainEnableCRLFRefresh );
    enableCRLF.SetAccessTypesL( accessTypesGetReplaceAdd );
    enableCRLF.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    enableCRLF.SetScopeL( MSmlDmDDFObject::EDynamic );
    enableCRLF.SetDFFormatL( MSmlDmDDFObject::EInt );
    enableCRLF.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    enableCRLF.SetDescriptionL( KNSmlDMDomainEnableCRLFRefreshExp );

    // DomainSpecific/<x>/STUNUsername
    MSmlDmDDFObject& stunUsername = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMSTUNUsername );
    stunUsername.SetAccessTypesL( accessTypesGetReplaceAdd );
    stunUsername.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    stunUsername.SetScopeL( MSmlDmDDFObject::EDynamic );
    stunUsername.SetDFFormatL( MSmlDmDDFObject::EChr );
    stunUsername.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    stunUsername.SetDescriptionL( KNSmlDMSTUNUsernameExp );

    // DomainSpecific/<x>/STUNPassword
    MSmlDmDDFObject& stunPassword = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMSTUNPassword );
    stunPassword.SetAccessTypesL( accessTypesGetReplaceAdd );
    stunPassword.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    stunPassword.SetScopeL( MSmlDmDDFObject::EDynamic );
    stunPassword.SetDFFormatL( MSmlDmDDFObject::EChr );
    stunPassword.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    stunPassword.SetDescriptionL( KNSmlDMSTUNPasswordExp );

    // DomainSpecific/<x>/AdditionalSTUNServer
    MSmlDmDDFObject& stunSrv = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMAdditionalSTUNNodeName );
    stunSrv.SetAccessTypesL( accessTypesGetAdd );
    stunSrv.SetOccurenceL( MSmlDmDDFObject::EOne );
    stunSrv.SetScopeL( MSmlDmDDFObject::EDynamic );
    stunSrv.SetDFFormatL( MSmlDmDDFObject::ENode );
    stunSrv.SetDescriptionL( KNSmlDMAdditionalSTUNNodeNameExp );
    
    // DomainSpecific/<x>/AdditionalSTUNServer/<x>
    MSmlDmDDFObject& dynStunSrv = stunSrv.AddChildObjectGroupL();
    dynStunSrv.SetAccessTypesL( accessTypesAll );
    dynStunSrv.SetOccurenceL( MSmlDmDDFObject::EZeroOrMore );
    dynStunSrv.SetScopeL( MSmlDmDDFObject::EDynamic );
    dynStunSrv.SetDFFormatL( MSmlDmDDFObject::ENode );
    dynStunSrv.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    dynStunSrv.SetDescriptionL( KNSmlDMAdditionalSTUNDynamicNodeExp );

    // DomainSpecific/<x>/AdditionalSTUNServer/<x>/STUNSrvAddr
    MSmlDmDDFObject& addStunSrvAddr = dynStunSrv.AddChildObjectL( 
        KNSmlDMSTUNSrvAddr );
    addStunSrvAddr.SetAccessTypesL( accessTypesGetReplaceAdd );
    addStunSrvAddr.SetOccurenceL( MSmlDmDDFObject::EOne );
    addStunSrvAddr.SetScopeL( MSmlDmDDFObject::EDynamic );
    addStunSrvAddr.SetDFFormatL( MSmlDmDDFObject::EChr );
    addStunSrvAddr.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    addStunSrvAddr.SetDescriptionL( KNSmlDMSTUNSrvAddrExp );

    // DomainSpecific/<x>/AdditionalSTUNServer/<x>/STUNSrvPort
    MSmlDmDDFObject& addStunSrvPort = dynStunSrv.AddChildObjectL( 
        KNSmlDMSTUNSrvPort );
    addStunSrvPort.SetAccessTypesL( accessTypesGetReplaceAdd );
    addStunSrvPort.SetOccurenceL( MSmlDmDDFObject::EOne );
    addStunSrvPort.SetScopeL( MSmlDmDDFObject::EDynamic );
    addStunSrvPort.SetDFFormatL( MSmlDmDDFObject::EInt );
    addStunSrvPort.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    addStunSrvPort.SetDescriptionL( KNSmlDMSTUNSrvPortExp );

    // DomainSpecific/<x>/AdditionalSTUNServer/<x>/STUNUsername
    MSmlDmDDFObject& addStunUsername = dynStunSrv.AddChildObjectL( 
        KNSmlDMSTUNUsername );
    addStunUsername.SetAccessTypesL( accessTypesGetReplaceAdd );
    addStunUsername.SetOccurenceL( MSmlDmDDFObject::EOne );
    addStunUsername.SetScopeL( MSmlDmDDFObject::EDynamic );
    addStunUsername.SetDFFormatL( MSmlDmDDFObject::EChr );
    addStunUsername.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    addStunUsername.SetDescriptionL( KNSmlDMSTUNUsernameExp );

    // DomainSpecific/<x>/AdditionalSTUNServer/<x>/STUNPassword
    MSmlDmDDFObject& addStunPassword = dynStunSrv.AddChildObjectL( 
        KNSmlDMSTUNPassword );
    addStunPassword.SetAccessTypesL( accessTypesGetReplaceAdd );
    addStunPassword.SetOccurenceL( MSmlDmDDFObject::EOne );
    addStunPassword.SetScopeL( MSmlDmDDFObject::EDynamic );
    addStunPassword.SetDFFormatL( MSmlDmDDFObject::EChr );
    addStunPassword.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    addStunPassword.SetDescriptionL( KNSmlDMSTUNPasswordExp );

    // DomainSpecific/<x>/STUNSharedSecret
    MSmlDmDDFObject& stunSharedSecret = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMSTUNSharedSecret );
    stunSharedSecret.SetAccessTypesL( accessTypesGetReplaceAdd );
    stunSharedSecret.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    stunSharedSecret.SetScopeL( MSmlDmDDFObject::EDynamic );
    stunSharedSecret.SetDFFormatL( MSmlDmDDFObject::EBool );
    stunSharedSecret.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    stunSharedSecret.SetDescriptionL( KNSmlDMSTUNSharedSecretExp );

    // DomainSpecific/<x>/PortPoolStartPort
    MSmlDmDDFObject& startPort = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMPortPoolStart );
    startPort.SetAccessTypesL( accessTypesGetReplaceAdd );
    startPort.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    startPort.SetScopeL( MSmlDmDDFObject::EDynamic );
    startPort.SetDFFormatL( MSmlDmDDFObject::EInt );
    startPort.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    startPort.SetDescriptionL( KNSmlDMPortPoolStartExp );

    // DomainSpecific/<x>/PortPoolEndPort
    MSmlDmDDFObject& endPort = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMPortPoolEnd );
    endPort.SetAccessTypesL( accessTypesGetReplaceAdd );
    endPort.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    endPort.SetScopeL( MSmlDmDDFObject::EDynamic );
    endPort.SetDFFormatL( MSmlDmDDFObject::EInt );
    endPort.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    endPort.SetDescriptionL( KNSmlDMPortPoolEndExp );

    // DomainSpecific/<x>/UsedNATProtocol
    MSmlDmDDFObject& natProtocol = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMUsedNatProtocol );
    natProtocol.SetAccessTypesL( accessTypesGetReplaceAdd );
    natProtocol.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    natProtocol.SetScopeL( MSmlDmDDFObject::EDynamic );
    natProtocol.SetDFFormatL( MSmlDmDDFObject::EChr );
    natProtocol.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    natProtocol.SetDescriptionL( KNSmlDMUsedNatProtocolExp );

    // DomainSpecific/<x>/ICESpecific
    MSmlDmDDFObject& iceSpecific = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMICESpecificNodeName );
    iceSpecific.SetAccessTypesL( accessTypesGetAdd );
    iceSpecific.SetOccurenceL( MSmlDmDDFObject::EOne );
    iceSpecific.SetScopeL( MSmlDmDDFObject::EDynamic );
    iceSpecific.SetDFFormatL( MSmlDmDDFObject::ENode );
    iceSpecific.SetDescriptionL( KNSmlDMICESpecificNodeNameExp );

    // DomainSpecific/<x>/ICESpecific/NATUtilities
    MSmlDmDDFObject& natUtils = iceSpecific.AddChildObjectL( 
        KNSmlDMNatUtilities );
    natUtils.SetAccessTypesL( accessTypesGetReplaceAdd );
    natUtils.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    natUtils.SetScopeL( MSmlDmDDFObject::EDynamic );
    natUtils.SetDFFormatL( MSmlDmDDFObject::EInt );
    natUtils.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    natUtils.SetDescriptionL( KNSmlDMNatUtilitiesExp );

    // DomainSpecific/<x>/ICESpecific/HostPreference
    MSmlDmDDFObject& hostPref = iceSpecific.AddChildObjectL( 
        KNSmlDMHostPref );
    hostPref.SetAccessTypesL( accessTypesGetReplaceAdd );
    hostPref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    hostPref.SetScopeL( MSmlDmDDFObject::EDynamic );
    hostPref.SetDFFormatL( MSmlDmDDFObject::EInt );
    hostPref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    hostPref.SetDescriptionL( KNSmlDMHostPrefExp );

    // DomainSpecific/<x>/ICESpecific/ServerReflexivePreference
    MSmlDmDDFObject& serverReflexPref = iceSpecific.AddChildObjectL( 
        KNSmlDMServerReflexPref );
    serverReflexPref.SetAccessTypesL( accessTypesGetReplaceAdd );
    serverReflexPref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    serverReflexPref.SetScopeL( MSmlDmDDFObject::EDynamic );
    serverReflexPref.SetDFFormatL( MSmlDmDDFObject::EInt );
    serverReflexPref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    serverReflexPref.SetDescriptionL( KNSmlDMServerReflexPrefExp );

    // DomainSpecific/<x>/ICESpecific/RelayPreference
    MSmlDmDDFObject& relayPref = iceSpecific.AddChildObjectL( 
        KNSmlDMRelayPref );
    relayPref.SetAccessTypesL( accessTypesGetReplaceAdd );
    relayPref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    relayPref.SetScopeL( MSmlDmDDFObject::EDynamic );
    relayPref.SetDFFormatL( MSmlDmDDFObject::EInt );
    relayPref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    relayPref.SetDescriptionL( KNSmlDMRelayPrefExp );

    // DomainSpecific/<x>/ICESpecific/PeerReflexivePreference
    MSmlDmDDFObject& peerReflexPref = iceSpecific.AddChildObjectL( 
        KNSmlDMPeerReflexPref );
    peerReflexPref.SetAccessTypesL( accessTypesGetReplaceAdd );
    peerReflexPref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    peerReflexPref.SetScopeL( MSmlDmDDFObject::EDynamic );
    peerReflexPref.SetDFFormatL( MSmlDmDDFObject::EInt );
    peerReflexPref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    peerReflexPref.SetDescriptionL( KNSmlDMPeerReflexPrefExp );

    // DomainSpecific/<x>/ICESpecific/IPv4Preference
    MSmlDmDDFObject& ipv4Pref = iceSpecific.AddChildObjectL( 
        KNSmlDMIPv4Pref );
    ipv4Pref.SetAccessTypesL( accessTypesGetReplaceAdd );
    ipv4Pref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    ipv4Pref.SetScopeL( MSmlDmDDFObject::EDynamic );
    ipv4Pref.SetDFFormatL( MSmlDmDDFObject::EInt );
    ipv4Pref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    ipv4Pref.SetDescriptionL( KNSmlDMIPv4PrefExp );

    // DomainSpecific/<x>/ICESpecific/IPv6Preference
    MSmlDmDDFObject& ipv6Pref = iceSpecific.AddChildObjectL( 
        KNSmlDMIPv6Pref );
    ipv6Pref.SetAccessTypesL( accessTypesGetReplaceAdd );
    ipv6Pref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    ipv6Pref.SetScopeL( MSmlDmDDFObject::EDynamic );
    ipv6Pref.SetDFFormatL( MSmlDmDDFObject::EInt );
    ipv6Pref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    ipv6Pref.SetDescriptionL( KNSmlDMIPv6PrefExp );

    // DomainSpecific/<x>/ICESpecific/VPNPreference
    MSmlDmDDFObject& vpnPref = iceSpecific.AddChildObjectL( 
        KNSmlDMVpnPref );
    vpnPref.SetAccessTypesL( accessTypesGetReplaceAdd );
    vpnPref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    vpnPref.SetScopeL( MSmlDmDDFObject::EDynamic );
    vpnPref.SetDFFormatL( MSmlDmDDFObject::EInt );
    vpnPref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    vpnPref.SetDescriptionL( KNSmlDMVpnPrefExp );

    // DomainSpecific/<x>/ICESpecific/UDPPreference
    MSmlDmDDFObject& updPref = iceSpecific.AddChildObjectL( 
        KNSmlDMUdpPref );
    updPref.SetAccessTypesL( accessTypesGetReplaceAdd );
    updPref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    updPref.SetScopeL( MSmlDmDDFObject::EDynamic );
    updPref.SetDFFormatL( MSmlDmDDFObject::EInt );
    updPref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    updPref.SetDescriptionL( KNSmlDMUdpPrefExp );

    // DomainSpecific/<x>/ICESpecific/TCPPreference
    MSmlDmDDFObject& tcpPref = iceSpecific.AddChildObjectL( 
        KNSmlDMTcpPref );
    tcpPref.SetAccessTypesL( accessTypesGetReplaceAdd );
    tcpPref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    tcpPref.SetScopeL( MSmlDmDDFObject::EDynamic );
    tcpPref.SetDFFormatL( MSmlDmDDFObject::EInt );
    tcpPref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    tcpPref.SetDescriptionL( KNSmlDMTcpPrefExp );

    // DomainSpecific/<x>/ICESpecific/TCPActivePreference
    MSmlDmDDFObject& tcpActivePref = iceSpecific.AddChildObjectL( 
        KNSmlDMTcpActivePref );
    tcpActivePref.SetAccessTypesL( accessTypesGetReplaceAdd );
    tcpActivePref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    tcpActivePref.SetScopeL( MSmlDmDDFObject::EDynamic );
    tcpActivePref.SetDFFormatL( MSmlDmDDFObject::EInt );
    tcpActivePref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    tcpActivePref.SetDescriptionL( KNSmlDMTcpActivePrefExp );

    // DomainSpecific/<x>/ICESpecific/TCPPassivePreference
    MSmlDmDDFObject& tcpPassivePref = iceSpecific.AddChildObjectL( 
        KNSmlDMTcpPassivePref );
    tcpPassivePref.SetAccessTypesL( accessTypesGetReplaceAdd );
    tcpPassivePref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    tcpPassivePref.SetScopeL( MSmlDmDDFObject::EDynamic );
    tcpPassivePref.SetDFFormatL( MSmlDmDDFObject::EInt );
    tcpPassivePref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    tcpPassivePref.SetDescriptionL( KNSmlDMTcpPassivePrefExp );

    // DomainSpecific/<x>/ICESpecific/TCPSimultaneousPreference
    MSmlDmDDFObject& tcpSimultPref = iceSpecific.AddChildObjectL( 
        KNSmlDMTcpSimultPref );
    tcpSimultPref.SetAccessTypesL( accessTypesGetReplaceAdd );
    tcpSimultPref.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    tcpSimultPref.SetScopeL( MSmlDmDDFObject::EDynamic );
    tcpSimultPref.SetDFFormatL( MSmlDmDDFObject::EInt );
    tcpSimultPref.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    tcpSimultPref.SetDescriptionL( KNSmlDMTcpSimultPrefExp );

    // DomainSpecific/<x>/TURNSpecific
    MSmlDmDDFObject& turnSpecific = dynamicDomainNode.AddChildObjectL( 
        KNSmlDMTURNSpecificNodeName );
    turnSpecific.SetAccessTypesL( accessTypesGetAdd );
    turnSpecific.SetOccurenceL( MSmlDmDDFObject::EOne );
    turnSpecific.SetScopeL( MSmlDmDDFObject::EDynamic );
    turnSpecific.SetDFFormatL( MSmlDmDDFObject::ENode );
    turnSpecific.SetDescriptionL( KNSmlDMTURNSpecificNodeNameExp );

    // DomainSpecific/<x>/TURNSpecific/TURNServer
    MSmlDmDDFObject& turnServer = turnSpecific.AddChildObjectL( 
        KNSmlDMTURNServerNodeName );
    turnServer.SetAccessTypesL( accessTypesGetAdd );
    turnServer.SetOccurenceL( MSmlDmDDFObject::EOne );
    turnServer.SetScopeL( MSmlDmDDFObject::EDynamic );
    turnServer.SetDFFormatL( MSmlDmDDFObject::ENode );
    turnServer.SetDescriptionL( KNSmlDMTURNServerNodeNameExp );

    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>
    MSmlDmDDFObject& dynTurnServer = turnServer.AddChildObjectGroupL();
    dynTurnServer.SetAccessTypesL( accessTypesAll );
    dynTurnServer.SetOccurenceL( MSmlDmDDFObject::EZeroOrMore );
    dynTurnServer.SetScopeL( MSmlDmDDFObject::EDynamic );
    dynTurnServer.SetDFFormatL( MSmlDmDDFObject::ENode );
    dynTurnServer.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    dynTurnServer.SetDescriptionL( KNSmlDMTURNServerDynamicNodeExp );
    
    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/TURNSrvAddr
    MSmlDmDDFObject& turnSrvAddr = dynTurnServer.AddChildObjectL( 
        KNSmlDMTURNSrvAddr );
    turnSrvAddr.SetAccessTypesL( accessTypesGetReplaceAdd );
    turnSrvAddr.SetOccurenceL( MSmlDmDDFObject::EOne );
    turnSrvAddr.SetScopeL( MSmlDmDDFObject::EDynamic );
    turnSrvAddr.SetDFFormatL( MSmlDmDDFObject::EChr );
    turnSrvAddr.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    turnSrvAddr.SetDescriptionL( KNSmlDMTURNSrvAddrExp );

    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/TURNSrvPort
    MSmlDmDDFObject& turnSrvPort = dynTurnServer.AddChildObjectL( 
        KNSmlDMTURNSrvPort );
    turnSrvPort.SetAccessTypesL( accessTypesGetReplaceAdd );
    turnSrvPort.SetOccurenceL( MSmlDmDDFObject::EOne );
    turnSrvPort.SetScopeL( MSmlDmDDFObject::EDynamic );
    turnSrvPort.SetDFFormatL( MSmlDmDDFObject::EInt );
    turnSrvPort.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    turnSrvPort.SetDescriptionL( KNSmlDMTURNSrvPortExp );
    
    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/TURNUsername
    MSmlDmDDFObject& turnUsername = dynTurnServer.AddChildObjectL( 
        KNSmlDMTURNUsername );
    turnUsername.SetAccessTypesL( accessTypesGetReplaceAdd );
    turnUsername.SetOccurenceL( MSmlDmDDFObject::EOne );
    turnUsername.SetScopeL( MSmlDmDDFObject::EDynamic );
    turnUsername.SetDFFormatL( MSmlDmDDFObject::EChr );
    turnUsername.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    turnUsername.SetDescriptionL( KNSmlDMTURNUsernameExp );
    
    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/TURNPassword
    MSmlDmDDFObject& turnPassword = dynTurnServer.AddChildObjectL( 
        KNSmlDMTURNPassword );
    turnPassword.SetAccessTypesL( accessTypesGetReplaceAdd );
    turnPassword.SetOccurenceL( MSmlDmDDFObject::EOne );
    turnPassword.SetScopeL( MSmlDmDDFObject::EDynamic );
    turnPassword.SetDFFormatL( MSmlDmDDFObject::EChr );
    turnPassword.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    turnPassword.SetDescriptionL( KNSmlDMTURNPasswordExp );

    // IAPSpecific ( ./NATFW/IAPSpecific )
    MSmlDmDDFObject& iapSpecific = natFwTraversal.AddChildObjectL( 
        KNSmlDMIAPSpecificNodeName );
    iapSpecific.SetAccessTypesL( accessTypesGetAdd );
    iapSpecific.SetOccurenceL( MSmlDmDDFObject::EOne );
    iapSpecific.SetScopeL( MSmlDmDDFObject::EPermanent );
    iapSpecific.SetDFFormatL( MSmlDmDDFObject::ENode );
    iapSpecific.SetDescriptionL( KNSmlDMIAPSpecificNodeNameExp );

    // dynamic IAP specific settings ID node ( ./NATFW/IAPSpecific/<x> )
    MSmlDmDDFObject& dynIAPNode = iapSpecific.AddChildObjectGroupL();
    dynIAPNode.SetAccessTypesL( accessTypesAll );
    dynIAPNode.SetOccurenceL( MSmlDmDDFObject::EZeroOrMore );
    dynIAPNode.SetScopeL( MSmlDmDDFObject::EDynamic );
    dynIAPNode.SetDFFormatL( MSmlDmDDFObject::ENode );
    dynIAPNode.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    dynIAPNode.SetDescriptionL( KNSmlDMIAPSpecificDynamicNodeExp );

    // IAPSpecific/<x>/PrefConRef
    MSmlDmDDFObject& prefConRef = dynIAPNode.AddChildObjectL( 
        KNSmlDMNATPrefConRef );
    prefConRef.SetAccessTypesL( accessTypesGetReplaceAdd );
    prefConRef.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    prefConRef.SetScopeL( MSmlDmDDFObject::EDynamic );
    prefConRef.SetDFFormatL( MSmlDmDDFObject::EChr );
    prefConRef.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    prefConRef.SetDescriptionL( KNSmlDMNATPrefConRefExp );

    // IAPSpecific/<x>/NATRefreshTCP
    MSmlDmDDFObject& natRefreshTCP = dynIAPNode.AddChildObjectL( 
        KNSmlDMNATRefreshTCP );
    natRefreshTCP.SetAccessTypesL( accessTypesGetReplaceAdd );
    natRefreshTCP.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    natRefreshTCP.SetScopeL( MSmlDmDDFObject::EDynamic );
    natRefreshTCP.SetDFFormatL( MSmlDmDDFObject::EInt );
    natRefreshTCP.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    natRefreshTCP.SetDescriptionL( KNSmlDMNATRefreshTCPExp );

    // IAPSpecific/<x>/NATRefreshUDP
    MSmlDmDDFObject& natRefreshUDP = dynIAPNode.AddChildObjectL( 
        KNSmlDMNATRefreshUDP );
    natRefreshUDP.SetAccessTypesL( accessTypesGetReplaceAdd );
    natRefreshUDP.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    natRefreshUDP.SetScopeL( MSmlDmDDFObject::EDynamic );
    natRefreshUDP.SetDFFormatL( MSmlDmDDFObject::EInt );
    natRefreshUDP.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    natRefreshUDP.SetDescriptionL( KNSmlDMNATRefreshUDPExp );

    // IAPSpecific/<x>/STUNRetransmit
    MSmlDmDDFObject& stunRetransmit = dynIAPNode.AddChildObjectL( 
        KNSmlDMSTUNRetransmit );
    stunRetransmit.SetAccessTypesL( accessTypesGetReplaceAdd );
    stunRetransmit.SetOccurenceL( MSmlDmDDFObject::EZeroOrOne );
    stunRetransmit.SetScopeL( MSmlDmDDFObject::EDynamic );
    stunRetransmit.SetDFFormatL( MSmlDmDDFObject::EInt );
    stunRetransmit.AddDFTypeMimeTypeL( KNSmlDMNATFWTextPlain );
    stunRetransmit.SetDescriptionL( KNSmlDMSTUNRetransmitExp );

    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::DDFStructureL(): end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::UpdateLeafObjectL
// Updates value of a leaf object
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::UpdateLeafObjectL( 
    const TDesC8& aURI, 
    const TDesC8& aLUID, 
    const TDesC8& aObject, 
    const TDesC8& /*aType*/, 
    const TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::UpdateLeafObjectL(): begin");

    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );

    CSmlDmAdapter::TError status = EOk;

    if ( aLUID.Length() <= 0 )
        {
        status = CSmlDmAdapter::ENotFound;
        iDmCallback->SetStatusL( aStatusRef, status );
        return;
        }

    // Load profile information if not loaded yet.
    if ( !iNATFWDomainEntries || !iNATFWIAPEntries )
        {
        LoadProfilesL();
        }

    TInt uriSegs = NumOfURISegs( aURI );
   
    // ==============================
    // DomainSpecific node
    // ==============================
    //
    if ( KErrNotFound != aURI.Find( KNSmlDMDomainSpecificNodeName() )
        && ( KFourSegs == uriSegs || KFiveSegs == uriSegs
        ) )
        {
        status = UpdateDomainSpecificObjectL( aURI, aLUID, aObject,
            aStatusRef );
        }

    // ==============================
    // IAPSpecific/<x>/
    // ==============================
    //
    else if ( KErrNotFound != aURI.Find( KNSmlDMIAPSpecificNodeName() )
        && KFourSegs == NumOfURISegs( aURI ) )
        {
        status = UpdateIAPSpecificObjectL( aURI, aLUID, aObject, aStatusRef );
        }

    // ===========================================
    // DomainSpecific/<x>/AdditionalSTUNServer/<x>
    // ===========================================
    //
    else if ( KErrNotFound != aURI.Find( KNSmlDMAdditionalSTUNNodeName() )
        && KSixSegs == uriSegs && KFourSegs == NumOfURISegs( aLUID ) )
        {
        status = UpdateAdditionalSTUNServerObjectL( aURI, aLUID, aObject,
            aStatusRef );
        }

    // ==============================================
    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>
    // ==============================================
    //
    else if ( KErrNotFound != aURI.Find( KNSmlDMTURNServerNodeName() )
        && KErrNotFound != aURI.Find( KNSmlDMTURNSpecificNodeName() )
        && KSevenSegs == uriSegs && KFiveSegs == NumOfURISegs( aLUID ) )
        {
        status = UpdateTURNServerObjectL( aURI, aLUID, aObject, aStatusRef );
        }

    else
        {
        // No node found under NATFW.
        status = CSmlDmAdapter::ENotFound;
        }  
        
    // We can return status right away if something went wrong.       
    if ( EOk != status )
        {
        iDmCallback->SetStatusL( aStatusRef, status ); 
        }
    
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::UpdateLeafObjectL(): end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::DeleteObjectL
// Deletes NAT-FW settings.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::DeleteObjectL( 
    const TDesC8& aURI, 
    const TDesC8& aLUID, 
    const TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::DeleteObjectL(): begin");

    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );

    // Load profile information if not loaded yet
    if ( !iNATFWDomainEntries || ! iNATFWIAPEntries )
        {
        LoadProfilesL();
        }
    CSmlDmAdapter::TError status = EOk;
    TPtrC8 uriTmp = RemoveLastURISeg( aURI );
    TPtrC8 secondLastUriSeg = LastURISeg( uriTmp );

    // ==============================
    // IAPSpecific/<x>/
    // ==============================
    //
    if ( KNSmlDMIAPSpecificNodeName() == secondLastUriSeg
      && aLUID.Find( KNSmlDMIAPSpecificNodeName ) != KErrNotFound )
        {
        TInt iapLoc( 0 );
        iapLoc = FindIAPLocation( aLUID );

        // Set status info for buffered delete command.
        TDMNATFWStatusInfo statusInfo;
        statusInfo.iKeyId = iNATFWIAPEntries->At( iapLoc )->GetIAPKey();
        statusInfo.iStatusRef = aStatusRef;
        statusInfo.iCommandType = TDMNATFWStatusInfo::EDMDeleteIAP;

        // Internal IPA entry can be deleted already.
        delete iNATFWIAPEntries->At( iapLoc );
        iNATFWIAPEntries->At( iapLoc ) = NULL;
        iNATFWIAPEntries->Delete( iapLoc );
        iNATFWIAPEntries->Compress();    
        iNATFWCommandBuffer.AppendL( statusInfo );
        }

    // ==============================
    //  DomainSpecific/<x>/
    // ==============================
    //
    else if ( KNSmlDMDomainSpecificNodeName() == secondLastUriSeg
      && aLUID.Find( KNSmlDMDomainSpecificNodeName ) != KErrNotFound )
        {

        TInt domainLoc ( 0 );
        domainLoc = FindDomainLocation( aLUID );

        // Set status info for buffered delete command.
        TDMNATFWStatusInfo statusInfo;
        statusInfo.iKeyId = iNATFWDomainEntries->At( 
            domainLoc )->GetDomainKey();
        statusInfo.iStatusRef = aStatusRef;
        statusInfo.iCommandType = TDMNATFWStatusInfo::EDMDeleteDomain;

        // Internal Domain entry can be deleted already.
        delete iNATFWDomainEntries->At( domainLoc );
        iNATFWDomainEntries->At( domainLoc ) = NULL;
        iNATFWDomainEntries->Delete( domainLoc );
        iNATFWDomainEntries->Compress();    
        iNATFWCommandBuffer.AppendL( statusInfo );
        }

    // ===========================================
    // DomainSpecific/<x>/AdditionalSTUNServer/<x>
    // ===========================================
    //
    else if ( KNSmlDMAdditionalSTUNNodeName() == secondLastUriSeg
        && aLUID.Find( KNSmlDMAdditionalSTUNNodeName ) != KErrNotFound )
        {
        // STUNSrvAddrKey is used as LUID.
        TUint32 stunKey = DesToInt( LastURISeg( aLUID ) );

        // STUN key is zero, i.e. false LUID
        if ( KErrNone == stunKey )
            {
            status = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, status );
            return;
            }

        TPtrC8 tmpLuid = RemoveLastURISeg( aLUID );
        TPtrC8 domainLuid = RemoveLastURISeg( tmpLuid );
        TInt domainLoc = FindDomainLocation( domainLuid );

        if ( KErrNotFound == domainLoc )
            {
            status = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, status );
            return;
            }

        TUint32 domainKey = iNATFWDomainEntries->At( domainLoc )->
            GetDomainKey();
        domainKey |= KUNSAFProtocolsFieldTypeMask;
        domainKey ^= KUNSAFProtocolsFieldTypeMask;
        RArray<TUint32> stunKeys;
        CleanupClosePushL( stunKeys ); // CS:1
        CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:2
        rep->FindL( domainKey|KUNSAFProtocolsSTUNAddressMask, 
            KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );
        CleanupStack::PopAndDestroy( rep ); // CS:1
        TInt stunLoc( KErrNotFound );
        const TInt stunCount( stunKeys.Count() );
        for ( TInt counter = 0; counter < stunCount; counter++ )
            {
            if ( stunKey == stunKeys[counter] )
                {
                stunLoc = counter;
                }
            }
        stunKeys.Close();
        CleanupStack::PopAndDestroy( &stunKeys ); // CS:0

        // STUN key was not in CentRep.
        if ( KErrNotFound == stunLoc )
            {
            status = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, status );
            return;
            }
        // STUN key was in CentRep index 0 which is reserved for STUN
        // settings directly under DomainSpecific node.
        else if ( KErrNone == stunLoc )
            {
            status = CSmlDmAdapter::EError;
            iDmCallback->SetStatusL( aStatusRef, status );
            return;
            }
        else
            {
            // Remove the deleted settings from internal arrays.
            iNATFWDomainEntries->At( domainLoc )->RemoveStunSrvAddr( 
                stunLoc );
            iNATFWDomainEntries->At( domainLoc )->RemoveStunSrvPort( 
                stunLoc );
            iNATFWDomainEntries->At( domainLoc )->RemoveStunUsername( 
                stunLoc );
            iNATFWDomainEntries->At( domainLoc )->RemoveStunPassword( 
                stunLoc );

            // Set status info for buffered delete command.
            TDMNATFWStatusInfo statusInfo;
            statusInfo.iKeyId = stunKey;
            statusInfo.iStatusRef = aStatusRef;
            statusInfo.iCommandType = TDMNATFWStatusInfo::EDMDeleteSTUN;
            iNATFWCommandBuffer.AppendL( statusInfo );
            }
        }

    // ==============================================
    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>
    // ==============================================
    //
    else if ( KNSmlDMTURNServerNodeName() == secondLastUriSeg
        && KErrNotFound != aURI.Find( KNSmlDMTURNSpecificNodeName() )
        && KErrNotFound != aLUID.Find( KNSmlDMTURNSpecificNodeName ) )
        {
        // TURNSpecific key is used as LUID.
        TUint32 turnKey = DesToInt( LastURISeg( aLUID ) );

        // key is zero, i.e. false LUID
        if ( KErrNone == turnKey )
            {
            status = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, status );
            return;
            }

        TPtrC8 tmpLuid = RemoveLastURISeg( aLUID );
        TPtrC8 domainLuid = RemoveLastURISeg( RemoveLastURISeg( tmpLuid ) );
        TInt domainLoc = FindDomainLocation( domainLuid );

        if ( KErrNotFound == domainLoc )
            {
            status = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, status );
            return;
            }

        TUint32 domainKey = iNATFWDomainEntries->At( domainLoc )->
            GetDomainKey();
        domainKey |= KUNSAFProtocolsFieldTypeMask;
        domainKey ^= KUNSAFProtocolsFieldTypeMask;
        RArray<TUint32> turnKeys;
        CleanupClosePushL( turnKeys ); // CS:1
        CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:2
        rep->FindL( domainKey|KUNSAFProtocolsTURNAddressMask, 
            KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );
        CleanupStack::PopAndDestroy( rep ); // CS:1
        TInt turnLoc( KErrNotFound );
        const TInt turnCount( turnKeys.Count() );
        for ( TInt counter = 0; counter < turnCount; counter++ )
            {
            if ( turnKey == turnKeys[counter] )
                {
                turnLoc = counter;
                }
            }
        turnKeys.Close();
        CleanupStack::PopAndDestroy( &turnKeys ); // CS:0

        // TURN key was not in CentRep.
        if ( KErrNotFound == turnLoc )
            {
            status = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, status );
            return;
            }
        else
            {
            // Remove the deleted settings from internal arrays.
            iNATFWDomainEntries->At( domainLoc )->RemoveTurnSrvAddr(
                turnLoc );
            iNATFWDomainEntries->At( domainLoc )->RemoveTurnSrvPort(
                turnLoc );
            iNATFWDomainEntries->At( domainLoc )->RemoveTurnUsername(
                turnLoc );
            iNATFWDomainEntries->At( domainLoc )->RemoveTurnPassword(
                turnLoc );

            // Set status info for buffered delete command.
            TDMNATFWStatusInfo statusInfo;
            statusInfo.iKeyId = turnKey;
            statusInfo.iStatusRef = aStatusRef;
            statusInfo.iCommandType = TDMNATFWStatusInfo::EDMDeleteTURN;
            iNATFWCommandBuffer.AppendL( statusInfo );
            }
        }

    else
        {
        status = EInvalidObject;
        iDmCallback->SetStatusL( aStatusRef, status ); 
        }
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::DeleteObjectL(): end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FetchLeafObjectL
// Fetches the values of leaf objects.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::FetchLeafObjectL( 
    const TDesC8& aURI, 
    const TDesC8& aLUID, 
    const TDesC8& aType, 
    const TInt aResultsRef, 
    const TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::FetchLeafObjectL(): begin");

    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );

    CBufBase *result = CBufFlat::NewL( KDefaultResultSize );
    CleanupStack::PushL( result ); // CS:1
    CSmlDmAdapter::TError status = FetchObjectL( aURI, aLUID, *result );

    if ( status == CSmlDmAdapter::EOk )
        {
        iDmCallback->SetResultsL( aResultsRef, *result, aType );
        }
    iDmCallback->SetStatusL( aStatusRef, status );

    CleanupStack::PopAndDestroy( result ); // CS:0
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::FetchLeafObjectL(): end");
    }
    
// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FetchLeafObjectSizeL
// Fetches size of a leaf object
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::FetchLeafObjectSizeL( 
    const TDesC8& aURI, 
    const TDesC8& aLUID, 
    const TDesC8& aType, 
    const TInt aResultsRef, 
    const TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::FetchLeafObjectSizeL(): begin");

    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );

    CBufBase *object = CBufFlat::NewL( 1 );
    CleanupStack::PushL( object ); // CS:1
    CSmlDmAdapter::TError retValue = FetchObjectL( aURI, aLUID, *object );

    TInt objSizeInBytes = object->Size();
    TBuf8<16> stringObjSizeInBytes;
    stringObjSizeInBytes.Num( objSizeInBytes );
    object->Reset();
    object->InsertL( 0, stringObjSizeInBytes );

    iDmCallback->SetStatusL( aStatusRef, retValue );
    iDmCallback->SetResultsL( aResultsRef, *object, aType );
    CleanupStack::PopAndDestroy( object ); // CS:0
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::FetchLeafObjectSizeL(): end");
    }
    
// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::ChildURIListL
// Asks for the list of children objects of the node
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::ChildURIListL( 
    const TDesC8& aURI,
    const TDesC8& aLUID,
    const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList,
    const TInt aResultsRef,
    const TInt aStatusRef )  
    {
    DBG_PRINT( "CNSmlDmNATFWTraversalAdapter::ChildURIListL(): begin" );

    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );

    CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;

    // load profile information in not loaded yet
    if ( !iNATFWDomainEntries || ! iNATFWIAPEntries )
        {
        LoadProfilesL();
        }

    CBufBase *currentURISegmentList = CBufFlat::NewL( KDefaultResultSize );
    CleanupStack::PushL( currentURISegmentList ); // CS:1
    TBuf8<KNSmlNATFWMaxUriLength> mappingInfo( KNullDesC8 );

    // get last URI segment. 
    TBuf8<KSmlMaxURISegLen> segment = LastURISeg( aURI );
    TBuf8<KSmlMaxURISegLen> segmentName;

    // ==============================
    // NATFW/
    // ==============================
    //
    if ( KOneSeg == NumOfURISegs( aURI ) &&
        KNSmlDMNATFWNodeName() == segment )  
        {
        segmentName.Copy( KNSmlDMDomainSpecificNodeName );
        currentURISegmentList->InsertL( currentURISegmentList->
            Size(), segmentName );
        currentURISegmentList->InsertL( currentURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMIAPSpecificNodeName );
        currentURISegmentList->InsertL( currentURISegmentList->
            Size(), segmentName );
        currentURISegmentList->InsertL( currentURISegmentList->
            Size(), KNSmlDMNATFWSeparator );
        }

    // ==============================
    // DomainSpecific/ etc
    // ==============================
    //
    if ( aURI.Find( KNSmlDMDomainSpecificNodeName ) != KErrNotFound )
        {
        retValue = DomainChildURIListL( aURI, aLUID, currentURISegmentList,
            aPreviousURISegmentList );
        }
        
    // ==================================
    // IAPSpecific/ and IAPSpecific/<x>/
    // ==================================
    //
    else if ( aURI.Find( KNSmlDMIAPSpecificNodeName ) != KErrNotFound &&
        KThreeSegs == NumOfURISegs( aURI ) || 
        KTwoSegs == NumOfURISegs( aURI ) )
        {
        retValue = IAPChildURIListL( aURI, aLUID, currentURISegmentList,
            aPreviousURISegmentList );
        }
    else 
        {
        // If none of asked nodes were found, return error.
        retValue = CSmlDmAdapter::ENotFound;
        }

    iDmCallback->SetStatusL( aStatusRef, retValue );
    if ( EOk == retValue )
        {
        iDmCallback->SetResultsL( aResultsRef,
            *currentURISegmentList, KNullDesC8 );
        }        
    CleanupStack::PopAndDestroy( currentURISegmentList ); // CS:0

    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::ChildURIListL(): end");
    }
    
// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::AddNodeObjectL
// Adds a new NAT-FW Traversal setting
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::AddNodeObjectL( 
    const TDesC8& aURI, 
    const TDesC8& aParentLUID, 
    const TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::AddNodeObjectL(): begin");
    CSmlDmAdapter::TError retValue = EOk;

    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );

    // Load profile information if not loaded yet
    if ( !iNATFWDomainEntries || ! iNATFWIAPEntries )
        {
        LoadProfilesL();
        }

    // Find profile from array, profile deleted from elsewhere if not found.
    if ( aParentLUID.Length() > 0 )
        {
        TInt iapLoc    = FindIAPLocation( aParentLUID );
        TInt domainLoc = FindDomainLocation( aParentLUID );
        if ( KErrNotFound == iapLoc && KErrNotFound == domainLoc )
            {
            retValue = CSmlDmAdapter::ENotFound;
            }
        }

    // Get last URI segment.
    TBuf8<KNSmlNATFWMaxUriLength> uri = aURI;
    TBuf8<KNSmlNATFWMaxUriLength> mappingInfo( KNullDesC8 );
    TPtrC8 lastUriSeg = LastURISeg( aURI );
    TPtrC8 uriTmp = RemoveLastURISeg( aURI );
    TPtrC8 secondLastUriSeg = LastURISeg( uriTmp );

    // Profile already exists if LUID exists, profile is not deleted from
    // elsewhere and node to be set is not a new AdditionalSTUNServer node or
    // a new STUNServer node.
    if ( EOk == retValue && 0 < aParentLUID.Length()
        && KNSmlDMAdditionalSTUNNodeName() != secondLastUriSeg
        && KNSmlDMTURNServerNodeName() != secondLastUriSeg )
        {
        uri.Copy( aParentLUID );
        retValue = CSmlDmAdapter::EAlreadyExists;
        iDmCallback->SetStatusL( aStatusRef, retValue );  
        DBG_PRINT("CNSmlDmNATFWTraversalAdapter::AddNodeObjectL(): end \
            (profile already exists)");
        return;
        }

    // ==============================
    // IAPSpecific/x/
    // ==============================
    //
    if ( KNSmlDMIAPSpecificNodeName() == secondLastUriSeg
        && aParentLUID.Find( KNSmlDMIAPSpecificNodeName ) == KErrNotFound )
        {
        // Central Repository for NAT-FW Traversal settings
        CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
        RArray<TUint32> keys;
        CleanupClosePushL( keys ); // CS:2
        rep->FindL( KUNSAFProtocolsIAPIdMask, 
            KUNSAFProtocolsFieldTypeMask, keys );
        TInt keyCount = keys.Count();   

        CNATFWIAPEntry* entry = CNATFWIAPEntry::NewLC(); // CS:3
        TUint32 tmp ( 0 );
        if ( keyCount > 0 )
            {
            tmp = keys[keys.Count() - 1] + 1;
            }
        else
            {
            tmp = KUNSAFProtocolsIAPTableMask;
            }
        entry->SetIAPKey( tmp|KUNSAFProtocolsIAPIdMask );
        iNATFWIAPEntries->AppendL( entry );
        CleanupStack::Pop( entry ); // CS:2

        // Set status info for buffered command.
        TDMNATFWStatusInfo statusInfo;
        statusInfo.iKeyId = tmp|KUNSAFProtocolsIAPIdMask;
        statusInfo.iStatusRef = aStatusRef;
        statusInfo.iCommandType = TDMNATFWStatusInfo::EDMAddIAP;
        iNATFWCommandBuffer.AppendL( statusInfo );

        // Set mapping info.
        mappingInfo.Copy( KNSmlDMIAPSpecificNodeName );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.AppendNum( entry->GetIAPKey() );
        iDmCallback->SetMappingL( aURI, mappingInfo );
        keys.Close();
        CleanupStack::PopAndDestroy( &keys ); // CS:1
        CleanupStack::PopAndDestroy( rep ); // CS:0
        SaveProfilesL();
        retValue = CSmlDmAdapter::EOk;
        }

    // ==============================
    // DomainSpecific/x/
    // ==============================
    //
    if ( KNSmlDMDomainSpecificNodeName() == secondLastUriSeg
      && aParentLUID.Find( KNSmlDMDomainSpecificNodeName ) == KErrNotFound )
        {
        // Central Repository for NAT-FW Traversal settings
        CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
        RArray<TUint32> keys;
        CleanupClosePushL( keys ); // CS:2
        rep->FindL( KUNSAFProtocolsDomainMask, 
            KUNSAFProtocolsFieldTypeMask, keys );
        TInt keyCount = keys.Count();

        // Add settings to domain entry.
        CNATFWDomainEntry* entry = CNATFWDomainEntry::NewLC(); // CS:3

        // Add new key ID and domain to entry.
        TUint32 tmp ( 0 );
        if ( keyCount > 0 )
            {
            tmp = keys[keyCount - 1] + 1;
            }
        else
            {
            tmp = KUNSAFProtocolsDomainTableMask;
            }
        entry->SetDomainKey( tmp|KUNSAFProtocolsDomainMask );
        entry->SetDomainL( lastUriSeg );

        // Add STUN server settings with values "not set" / "not found" to
        // internal array. These values will not be used when saving settings.
        entry->AddStunSrvAddrL( KNSmlDMNATFWValueNotSet );
        entry->AddStunSrvPortL( KErrNotFound );
        entry->AddStunUsernameL( KNSmlDMNATFWValueNotSet );
        entry->AddStunPasswordL( KNSmlDMNATFWValueNotSet );

        // Append new domain settings to buffered entries.
        iNATFWDomainEntries->AppendL( entry );
        CleanupStack::Pop( entry ); // CS:2

        // Set status info for buffered command.
        TDMNATFWStatusInfo statusInfo;

        // Set temporary buffer for save profiles.
        statusInfo.iKeyId = tmp|KUNSAFProtocolsDomainMask;
        statusInfo.iStatusRef = aStatusRef;
        statusInfo.iCommandType = TDMNATFWStatusInfo::EDMAddDomain;
        iNATFWCommandBuffer.AppendL( statusInfo );

        // Set mapping information to newly added settings.
        mappingInfo.Copy( KNSmlDMDomainSpecificNodeName );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.AppendNum( tmp|KUNSAFProtocolsDomainMask );
        iDmCallback->SetMappingL( aURI, mappingInfo );   
        keys.Close();
        CleanupStack::PopAndDestroy( &keys ); // CS:1
        CleanupStack::PopAndDestroy( rep ); // CS:0
        SaveProfilesL();
        retValue = CSmlDmAdapter::EOk;
        }

    // ============================================
    // DomainSpecific/<x>/AdditionalSTUNServer/<x>/
    // ============================================
    //
    if ( KNSmlDMAdditionalSTUNNodeName() == secondLastUriSeg
        && aParentLUID.Find( KNSmlDMAdditionalSTUNNodeName ) == KErrNotFound )
        {
        // If no parent LUID, no DomainSpecific profile and no
        // AdditionalSTUNServer nodes.
        if ( !( aParentLUID.Length() ) )
            {
            retValue = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, retValue );
            DBG_PRINT("CNSmlDmNATFWTraversalAdapter::AddNodeObjectL(): end \
                (no parent LUID)");
            return;
            }

        TInt domainLoc( FindDomainLocation( aParentLUID ) );
        // Domain settings to be updated are deleted from somewhere else.
        if ( KErrNotFound == domainLoc )
            {
            retValue = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, retValue );
            DBG_PRINT("CNSmlDmNATFWTraversalAdapter::AddNodeObjectL(): end \
                (domain settings deleted from somewhere else)");
            return;
            }
        TUint32 domainKey( iNATFWDomainEntries->At( domainLoc )->
            GetDomainKey() );
        CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
        RArray<TUint32> stunKeys;
        CleanupClosePushL( stunKeys ); // CS:2
        domainKey |= KUNSAFProtocolsFieldTypeMask;
        domainKey ^= KUNSAFProtocolsFieldTypeMask;

        rep->FindL( domainKey|KUNSAFProtocolsSTUNAddressMask, 
                    KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );
        TInt stunCount = stunKeys.Count();

        TUint32 tmp( 0 );
        if ( stunCount > 0 )
            {
            tmp = stunKeys[stunCount - 1];
            tmp = tmp|KUNSAFProtocolsSubTableFieldTypeMask;
            tmp++;
            tmp |= KUNSAFProtocolsSubTableFieldTypeMask;
            tmp = tmp^KUNSAFProtocolsSubTableFieldTypeMask|domainKey;
            // STUNAddressMask is used as LUID so let's add it to tmp now.
            tmp |= KUNSAFProtocolsSTUNAddressMask;
            }
        else
            {
            // If there are no STUN keys in CentRep, an AdditionalSTUNServer
            // node cannot be added since the first CentRep STUN key is
            // reserved to STUN settings directly under DomainSpecific node.
            stunKeys.Close();
            CleanupStack::PopAndDestroy( &stunKeys ); // CS:1
            CleanupStack::PopAndDestroy( rep ); // CS:0
            retValue = CSmlDmAdapter::EError;
            iDmCallback->SetStatusL( aStatusRef, retValue );
            DBG_PRINT("CNSmlDmNATFWTraversalAdapter::AddNodeObjectL(): end \
                (no STUN keys in Centrep)");
            return;
            }
        stunKeys.Close();
        CleanupStack::PopAndDestroy( &stunKeys ); // CS:1

        // Add AdditionalSTUNServer settings with values "not set" or 
        // "not found" to internal arrays. These values will not be used when
        // saving settings.
        iNATFWDomainEntries->At( domainLoc )->AddStunSrvAddrL( 
            KNSmlDMNATFWValueNotSet );
        iNATFWDomainEntries->At( domainLoc )->AddStunSrvPortL( 
            KErrNotFound );
        iNATFWDomainEntries->At( domainLoc )->AddStunUsernameL( 
            KNSmlDMNATFWValueNotSet );
        iNATFWDomainEntries->At( domainLoc )->AddStunPasswordL( 
            KNSmlDMNATFWValueNotSet );

        // Set status info for buffered command.
        TDMNATFWStatusInfo statusInfo;
        statusInfo.iKeyId = tmp;
        statusInfo.iStatusRef = aStatusRef;
        statusInfo.iCommandType = TDMNATFWStatusInfo::EDMAddSTUN;
        iNATFWCommandBuffer.AppendL( statusInfo );

        // Set mapping info. 
        mappingInfo.Copy( KNSmlDMDomainSpecificNodeName );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.AppendNum( iNATFWDomainEntries->At( domainLoc )->
            GetDomainKey() );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.Append( KNSmlDMAdditionalSTUNNodeName );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.AppendNum( tmp );
        iDmCallback->SetMappingL( aURI, mappingInfo );
        CleanupStack::PopAndDestroy( rep ); // CS:0
        SaveProfilesL();
        retValue = CSmlDmAdapter::EOk; 
        }

    // ================================================
    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/
    // ================================================
    //
    if ( KNSmlDMTURNServerNodeName() == secondLastUriSeg &&
        KErrNotFound != aURI.Find( KNSmlDMTURNSpecificNodeName() ) &&
        aParentLUID.Find( KNSmlDMTURNServerNodeName ) == KErrNotFound )
        {
        // If no parent LUID, no DomainSpecific profile and no
        // TURNSpecific/TURNServer nodes.
        if ( !( aParentLUID.Length() ) )
            {
            retValue = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, retValue );
            DBG_PRINT("CNSmlDmNATFWTraversalAdapter::AddNodeObjectL(): end \
                (no parent LUID)");
            return;
            }

        TInt domainLoc( FindDomainLocation( aParentLUID ) );
        // Domain settings to be updated are deleted from somewhere else.
        if ( KErrNotFound == domainLoc )
            {
            retValue = CSmlDmAdapter::ENotFound;
            iDmCallback->SetStatusL( aStatusRef, retValue );
            DBG_PRINT("CNSmlDmNATFWTraversalAdapter::AddNodeObjectL(): end \
                (domain settings deleted from somewhere else)");
            return;
            }
        TUint32 domainKey( iNATFWDomainEntries->At( domainLoc )->
            GetDomainKey() );
        CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
        RArray<TUint32> turnKeys;
        CleanupClosePushL( turnKeys ); // CS:2
        domainKey |= KUNSAFProtocolsFieldTypeMask;
        domainKey ^= KUNSAFProtocolsFieldTypeMask;

        rep->FindL( domainKey|KUNSAFProtocolsTURNAddressMask, 
                    KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );
        TInt turnCount = turnKeys.Count();

        TUint32 tmp( 0 );
        if ( turnCount > 0 )
            {
            tmp = turnKeys[turnCount - 1];
            tmp = tmp|KUNSAFProtocolsSubTableFieldTypeMask;
            tmp++;
            tmp |= KUNSAFProtocolsSubTableFieldTypeMask;
            tmp = tmp^KUNSAFProtocolsSubTableFieldTypeMask|domainKey;
            }
        else
            {
            TUint32 currentNewKey = domainKey;
            currentNewKey |= KUNSAFProtocolsFieldTypeMask;
            currentNewKey ^= KUNSAFProtocolsFieldTypeMask;

            tmp = KUNSAFProtocolsSubTableFieldTypeMask + 1;
            tmp |= KUNSAFProtocolsSubTableFieldTypeMask;
            tmp = tmp^KUNSAFProtocolsSubTableFieldTypeMask|currentNewKey;
            }

        // TURNAddressMask is used as LUID so let's add it to tmp now.
        tmp |= KUNSAFProtocolsTURNAddressMask;

        turnKeys.Close();
        CleanupStack::PopAndDestroy( &turnKeys ); // CS:1

        // Add TURNSpecific/TURNServer settings with values "not set" or 
        // "not found" to internal arrays. These values will not be used when
        // saving settings.
        iNATFWDomainEntries->At( domainLoc )->AddTurnSrvAddrL( 
            KNSmlDMNATFWValueNotSet );
        iNATFWDomainEntries->At( domainLoc )->AddTurnSrvPortL( 
            KErrNotFound );
        iNATFWDomainEntries->At( domainLoc )->AddTurnUsernameL(
            KNSmlDMNATFWValueNotSet );
        iNATFWDomainEntries->At( domainLoc )->AddTurnPasswordL(
            KNSmlDMNATFWValueNotSet );

        // Set status info for buffered command.
        TDMNATFWStatusInfo statusInfo;
        statusInfo.iKeyId = tmp;
        statusInfo.iStatusRef = aStatusRef;
        statusInfo.iCommandType = TDMNATFWStatusInfo::EDMAddTURN;
        iNATFWCommandBuffer.AppendL( statusInfo );

        // Set mapping info. 
        mappingInfo.Copy( KNSmlDMDomainSpecificNodeName );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.AppendNum( iNATFWDomainEntries->At( domainLoc )->
            GetDomainKey() );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.Append( KNSmlDMTURNSpecificNodeName );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.Append( KNSmlDMTURNServerNodeName );
        mappingInfo.Append( KNSmlDMNATFWSeparator );
        mappingInfo.AppendNum( tmp );
        iDmCallback->SetMappingL( aURI, mappingInfo );
        CleanupStack::PopAndDestroy( rep ); // CS:0
        SaveProfilesL();
        retValue = CSmlDmAdapter::EOk; 
        }

    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::AddNodeObjectL(): end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::ExecuteCommandL
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::ExecuteCommandL( 
    const TDesC8& /*aURI*/, 
    const TDesC8& /*aLUID*/, 
    const TDesC8& /*aArgument*/, 
    const TDesC8& /*aType*/, 
    const TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::ExecuteCommandL(): begin");
    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );
    iDmCallback->SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::ExecuteCommandL(): end");
    }

// ---------------------------------------------------------------------------
//  CNSmlDmNATFWTraversalAdapter::ExecuteCommandL
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::ExecuteCommandL( 
    const TDesC8& /*aURI*/, 
    const TDesC8& /*aParentLUID*/, 
    RWriteStream*& /*aStream*/, 
    const TDesC8& /*aType*/, 
    const TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::ExecuteCommandL(): stream: begin");
    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );
    iDmCallback->SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::ExecuteCommandL(): stream: end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::CopyCommandL
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::CopyCommandL( 
    const TDesC8& /*aTargetURI*/, 
    const TDesC8& /*aTargetLUID*/, 
    const TDesC8& /*aSourceURI*/, 
    const TDesC8& /*aSourceLUID*/, 
    const TDesC8& /*aType*/, 
    TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::CopyCommandL(): begin");
    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );
    iDmCallback->SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::CopyCommandL(): end");
    }

// ---------------------------------------------------------------------------
//  CNSmlDmNATFWTraversalAdapter::StartAtomicL
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::StartAtomicL()
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::StartAtomicL(): begin");
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::StartAtomicL(): end");
    }
    
// ---------------------------------------------------------------------------
//  CNSmlDmNATFWTraversalAdapter::CommitAtomicL
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::CommitAtomicL()
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::CommitAtomicL(): begin");
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::CommitAtomicL(): end");
    }

// ---------------------------------------------------------------------------
//  CNSmlDmNATFWTraversalAdapter::RollbackAtomicL
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::RollbackAtomicL()
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::RollbackAtomicL(): begin");
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::RollbackAtomicL(): end");
    }

// ---------------------------------------------------------------------------
//  CNSmlDmNATFWTraversalAdapter::StreamingSupport
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
TBool CNSmlDmNATFWTraversalAdapter::StreamingSupport( TInt& /*aItemSize*/ )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::StreamingSupport(): begin");
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::StreamingSupport(): end");
    return EFalse;
    }

// ---------------------------------------------------------------------------
//  CNSmlDmNATFWTraversalAdapter::StreamCommittedL
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::StreamCommittedL()
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::StreamCommittedL(): begin");
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::StreamCommittedL(): end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::CompleteOutstandingCmdsL
// Saves modified profiles back to permanent store
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::CompleteOutstandingCmdsL()
    {
    SaveProfilesL();
    }
    
// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::UpdateLeafObjectL
// (Not supported, see smldmadapter.h)
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::UpdateLeafObjectL( 
    const TDesC8& /*aURI*/, 
    const TDesC8& /*aLUID*/, 
    RWriteStream*& /*aStream*/, 
    const TDesC8& /*aType*/, 
    const TInt aStatusRef )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::UpdateLeafObjectL(): stream: begin");
    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );
    iDmCallback->SetStatusL( aStatusRef, CSmlDmAdapter::EError );
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::UpdateLeafObjectL(): stream: end");
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::UpdateDomainSpecificObjectL
// 
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError 
    CNSmlDmNATFWTraversalAdapter::UpdateDomainSpecificObjectL(
    const TDesC8& aURI,
    const TDesC8& aLUID, 
    const TDesC8& aObject, 
    const TInt aStatusRef )
    {
    CSmlDmAdapter::TError status = EOk;
    
    TInt domainLoc = FindDomainLocation( aLUID );
    if ( KErrNotFound == domainLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }
    TDMNATFWStatusInfo statusInfo;
    statusInfo.iKeyId = iNATFWDomainEntries->
        At( domainLoc )->GetDomainKey();
    statusInfo.iStatusRef = aStatusRef;
    statusInfo.iCommandType = TDMNATFWStatusInfo::EDMUpdateDomain;
    
    TInt uriSegs = NumOfURISegs( aURI );

    TBool iceSpecific( EFalse );
    if ( KErrNotFound != aURI.Find( KNSmlDMICESpecificNodeName() )
        && KFiveSegs == uriSegs )
        {
        iceSpecific = ETrue;
        }
    TInt err( KErrNone );
    TPtrC8 lastURISeg = LastURISeg( aURI );

    // DomainSpecific/<x>/Domain
    if ( KNSmlDMDomain() == lastURISeg )    
        {
        iNATFWDomainEntries->At( domainLoc )->SetDomainL( aObject );
        }
    // DomainSpecific/<x>/STUNSrvAddr
    else if ( KNSmlDMSTUNSrvAddr() == lastURISeg && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetSTUNServerAddrL( aObject );

        // STUN server settings directly under DomainSpecific node go to
        // array index 0.
        err = iNATFWDomainEntries->At( domainLoc )->
            UpdateStunSrvAddrL( 0, aObject );
        }
    // DomainSpecific/<x>/STUNSrvPort
    else if ( KNSmlDMSTUNSrvPort() == lastURISeg && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetSTUNServerPort( DesToInt( aObject ) );

        err = iNATFWDomainEntries->At( domainLoc )->
            UpdateStunSrvPort( 0, DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/STUNUsername
    else if ( KNSmlDMSTUNUsername() == lastURISeg && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetSTUNUsernameL( aObject );
        err = iNATFWDomainEntries->At( domainLoc )->
            UpdateStunUsernameL( 0, aObject );
        }
    // DomainSpecific/<x>/STUNPassword
    else if ( KNSmlDMSTUNPassword() == lastURISeg && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetSTUNPasswordL( aObject );
        err = iNATFWDomainEntries->At( domainLoc )->
            UpdateStunPasswordL( 0, aObject );
        }

    // DomainSpecific/<x>/NATRefreshTCP
    else if ( KNSmlDMDomainNATRefreshTCP() == lastURISeg 
        && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetNATRefreshTCP( DesToInt( aObject ) );
        }
    // DomainSpecific/<x>/NATRefreshUDP
    else if ( KNSmlDMDomainNATRefreshUDP() == lastURISeg 
        && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetNATRefreshUDP( DesToInt( aObject ) );
        }
    // DomainSpecific/<x>/EnableCRLFRefresh
    else if ( KNSmlDMDomainEnableCRLFRefresh() == lastURISeg 
        && KFourSegs == uriSegs )
        {
        if ( KNSmlDMNATFWValueTrue() == aObject )
            {
            iNATFWDomainEntries->At( domainLoc )->
                SetEnableCRLFRefresh( ETrue );    
            }
        else if ( KNSmlDMNATFWValueFalse() == aObject )
            {
            iNATFWDomainEntries->At( domainLoc )->
                SetEnableCRLFRefresh( EFalse );        
            }
        else 
            {
            status = CSmlDmAdapter::EInvalidObject;
            }
        }

    // DomainSpecific/<x>/STUNSharedSecret
    else if ( KNSmlDMSTUNSharedSecret() == lastURISeg
        && KFourSegs == uriSegs )
        {
        if ( KNSmlDMNATFWValueTrue() == aObject )
            {
            // The CentRep key is for not using STUN shared secret
            // so we must change the value from true to false.
            iNATFWDomainEntries->At( domainLoc )->
                SetDisableStunSharedSecret( EFalse );
            }
        else if ( KNSmlDMNATFWValueFalse() == aObject )
            {
            iNATFWDomainEntries->At( domainLoc )->
                SetDisableStunSharedSecret( ETrue );
            }
        else
            {
            status = CSmlDmAdapter::EInvalidObject;
            }
        }
    // DomainSpecific/<x>/PortPoolStartPort
    else if ( KNSmlDMPortPoolStart() == lastURISeg
        && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetStartPortRange( DesToInt( aObject ) );
        }
    // DomainSpecific/<x>/PortPoolEndPort
    else if ( KNSmlDMPortPoolEnd() == lastURISeg
        && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetEndPortRange( DesToInt( aObject ) );
        }
    // DomainSpecific/<x>/UsedNATProtocol
    else if ( KNSmlDMUsedNatProtocol() == lastURISeg
        && KFourSegs == uriSegs )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetNatProtocolL( aObject );
        }

    // DomainSpecific/<x>/ICESpecific/NATUtilities
    else if ( KNSmlDMNatUtilities() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetNatUtilitiesL( aObject );
        }
    // DomainSpecific/<x>/ICESpecific/HostPreference
    else if ( KNSmlDMHostPref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetHostPref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/ServerReflexivePreference
    else if ( KNSmlDMServerReflexPref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetServerReflexPref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/RelayPreference
    else if ( KNSmlDMRelayPref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetRelayPref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/PeerReflexivePreference
    else if ( KNSmlDMPeerReflexPref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetPeerReflexPref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/IPv4Preference
    else if ( KNSmlDMIPv4Pref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetIPv4Pref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/IPv6Preference
    else if ( KNSmlDMIPv6Pref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetIPv6Pref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/VPNPreference
    else if ( KNSmlDMVpnPref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetVpnPref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/UDPPreference
    else if ( KNSmlDMUdpPref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetUdpPref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/TCPPreference
    else if ( KNSmlDMTcpPref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetTcpPref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/TCPActivePreference
    else if ( KNSmlDMTcpActivePref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetTcpActivePref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/TCPPassivePreference
    else if ( KNSmlDMTcpPassivePref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetTcpPassivePref( DesToInt( aObject ) );
        }

    // DomainSpecific/<x>/ICESpecific/TCPSimultaneousPreference
    else if ( KNSmlDMTcpSimultPref() == lastURISeg 
        && iceSpecific )
        {
        iNATFWDomainEntries->At( domainLoc )->
            SetTcpSimultPref( DesToInt( aObject ) );
        }

    else
        {
        status = CSmlDmAdapter::ENotFound;
        }

    if ( KErrNone != err )
        {
        status = CSmlDmAdapter::EError;
        }

    // append command to buffered commands if it was made ok    
    if ( EOk == status )    
        {
        iNATFWCommandBuffer.AppendL( statusInfo );
        }
    
    return status;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::UpdateIAPSpecificObjectL
// 
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError CNSmlDmNATFWTraversalAdapter::UpdateIAPSpecificObjectL(
    const TDesC8& aURI,
    const TDesC8& aLUID, 
    const TDesC8& aObject, 
    const TInt aStatusRef )
    {
    CSmlDmAdapter::TError status = EOk;
    
    TInt iapLoc = FindIAPLocation( aLUID );
    if ( KErrNotFound == iapLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    TDMNATFWStatusInfo statusInfo;
    statusInfo.iKeyId = iNATFWIAPEntries->At( iapLoc )->GetIAPKey();
    statusInfo.iStatusRef = aStatusRef;
    statusInfo.iCommandType = TDMNATFWStatusInfo::EDMUpdateIAP;
    
    TPtrC8 lastURISeg = LastURISeg( aURI );

    // PrefConRef
    if ( KNSmlDMNATPrefConRef() == lastURISeg ) 
        {

        // IAP-nbr from CommsDB
        TInt lIAPId = SetConRefL( aObject, iapLoc ); 
        if ( KErrNotFound != lIAPId )
            {
            status = CSmlDmAdapter::EOk;       
            }
        else
            {
            status = CSmlDmAdapter::EInvalidObject;
            }
        }
    else if ( KNSmlDMNATRefreshTCP() == lastURISeg )
        {
        iNATFWIAPEntries->At( iapLoc )->
            SetIntervalTCP( DesToInt( aObject ) );
        }
    else if ( KNSmlDMNATRefreshUDP() == lastURISeg )
        {
        iNATFWIAPEntries->At( iapLoc )->
            SetIntervalUDP( DesToInt( aObject ) );
        }
    else if ( KNSmlDMSTUNRetransmit() == lastURISeg )
        {
        iNATFWIAPEntries->At( iapLoc )->
            SetSTUNRetransmitTimer( DesToInt( aObject ) );
        }
    else
        {
        // none of iap specific settings found
        status = CSmlDmAdapter::ENotFound;
        }

    // append command to buffered commands if it was made ok    
    if ( EOk == status )    
        {
        iNATFWCommandBuffer.AppendL( statusInfo );
        }
        
    return status;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::UpdateAdditionalSTUNServerObjectL
// 
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError 
    CNSmlDmNATFWTraversalAdapter::UpdateAdditionalSTUNServerObjectL(
    const TDesC8& aURI,
    const TDesC8& aLUID, 
    const TDesC8& aObject, 
    const TInt aStatusRef )
    {
    CSmlDmAdapter::TError status = EOk;
    
    TUint32 stunKey = DesToInt( LastURISeg( aLUID ) );

    // STUN key is zero, i.e. false LUID
    if ( KErrNone == stunKey )
        {
        status = CSmlDmAdapter::ENotFound;
        iDmCallback->SetStatusL( aStatusRef, status );
        return status;
        }

    TBuf8<KLUIDMaxLength> luid( aLUID );
    TPtrC8 tmpLuid = RemoveLastURISeg( luid );
    TPtrC8 domainLuid = RemoveLastURISeg( tmpLuid );
    TInt domainLoc = FindDomainLocation( domainLuid );

    if ( KErrNotFound == domainLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    TDMNATFWStatusInfo statusInfo;
    statusInfo.iKeyId = stunKey;
    statusInfo.iStatusRef = aStatusRef;
    statusInfo.iCommandType = TDMNATFWStatusInfo::EDMUpdateSTUN;
    statusInfo.iLocation = domainLoc;

    TUint32 domainKey = iNATFWDomainEntries->At( domainLoc )->
        GetDomainKey();
    domainKey |= KUNSAFProtocolsFieldTypeMask;
    domainKey ^= KUNSAFProtocolsFieldTypeMask;
    RArray<TUint32> stunKeys;
    CleanupClosePushL( stunKeys ); // CS:1
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:2
    rep->FindL( domainKey|KUNSAFProtocolsSTUNAddressMask, 
        KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );
    CleanupStack::PopAndDestroy( rep ); // CS:1
    TInt stunLoc( KErrNotFound );
    TInt stunCount( stunKeys.Count() );
    for ( TInt counter = 0; counter < stunCount; counter++ )
        {
        if ( stunKey == stunKeys[counter] )
            {
            stunLoc = counter;
            }
        }
    stunKeys.Close();
    CleanupStack::PopAndDestroy( &stunKeys ); // CS:0
    
    TPtrC8 lastURISeg = LastURISeg( aURI );

    // STUN key was not in CentRep.
    if ( KErrNotFound == stunLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        }
    // STUN key was in CentRep index 0 which is reserved for STUN
    // settings directly under DomainSpecific node.
    else if ( KErrNone == stunLoc )
        {
        status = CSmlDmAdapter::EError;
        }
    // Update values.
    else
        {
        TInt err( KErrNone );

        // DomainSpecific/<x>/AdditionalSTUNServer/<x>/STUNSrvAddr
        if ( KNSmlDMSTUNSrvAddr() == lastURISeg )
            {
            err = iNATFWDomainEntries->At( domainLoc )->
                UpdateStunSrvAddrL( stunLoc, aObject );
            }
        // DomainSpecific/<x>/AdditionalSTUNServer/<x>/STUNSrvPort
        else if ( KNSmlDMSTUNSrvPort() == lastURISeg )
            {
            err = iNATFWDomainEntries->At( domainLoc )->
                UpdateStunSrvPort( stunLoc, DesToInt( aObject ) );
            }
        // DomainSpecific/<x>/AdditionalSTUNServer/<x>/STUNUsername
        else if ( KNSmlDMSTUNUsername() == lastURISeg )
            {
            err = iNATFWDomainEntries->At( domainLoc )->
                UpdateStunUsernameL( stunLoc, aObject );
            }
        // DomainSpecific/<x>/AdditionalSTUNServer/<x>/STUNPassword
        else if ( KNSmlDMSTUNPassword() == lastURISeg )
            {
            err = iNATFWDomainEntries->At( domainLoc )->
                UpdateStunPasswordL( stunLoc, aObject );
            }
        else
            {
            // None of AdditionalSTUNServer settings found.
            status = CSmlDmAdapter::ENotFound;
            }

        if ( KErrNone != err )
            {
            status = CSmlDmAdapter::EError;
            }
        }

    // Append command to buffered commands if it was made ok.
    if ( EOk == status )    
        {
        iNATFWCommandBuffer.AppendL( statusInfo );
        }
    
    return status;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::UpdateTURNServerObjectL
// 
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError 
    CNSmlDmNATFWTraversalAdapter::UpdateTURNServerObjectL(
    const TDesC8& aURI,
    const TDesC8& aLUID, 
    const TDesC8& aObject, 
    const TInt aStatusRef )
    {
    CSmlDmAdapter::TError status = EOk;
    
    TUint32 turnKey = DesToInt( LastURISeg( aLUID ) );

    // TURN key is zero, i.e. false LUID
    if ( KErrNone == turnKey )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    TBuf8<KLUIDMaxLength> luid( aLUID );
    TPtrC8 tmpLuid = RemoveLastURISeg( luid );
    TPtrC8 domainLuid = RemoveLastURISeg( RemoveLastURISeg( tmpLuid ) );
    TInt domainLoc = FindDomainLocation( domainLuid );

    if ( KErrNotFound == domainLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    TDMNATFWStatusInfo statusInfo;
    statusInfo.iKeyId = turnKey;
    statusInfo.iStatusRef = aStatusRef;
    statusInfo.iCommandType = TDMNATFWStatusInfo::EDMUpdateTURN;
    statusInfo.iLocation = domainLoc;

    TUint32 domainKey = iNATFWDomainEntries->At( domainLoc )->
        GetDomainKey();
    domainKey |= KUNSAFProtocolsFieldTypeMask;
    domainKey ^= KUNSAFProtocolsFieldTypeMask;
    RArray<TUint32> turnKeys;
    CleanupClosePushL( turnKeys ); // CS:1
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:2
    rep->FindL( domainKey|KUNSAFProtocolsTURNAddressMask, 
        KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );
    CleanupStack::PopAndDestroy( rep ); // CS:1
    TInt turnLoc( KErrNotFound );
    TInt turnCount( turnKeys.Count() );
    for ( TInt counter = 0; counter < turnCount; counter++ )
        {
        if ( turnKey == turnKeys[counter] )
            {
            turnLoc = counter;
            }
        }
    turnKeys.Close();
    CleanupStack::PopAndDestroy( &turnKeys ); // CS:0
    
    TPtrC8 lastURISeg = LastURISeg( aURI );

    // TURN key was not in CentRep.
    if ( KErrNotFound == turnLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        }
    // Update values.
    else
        {
        TInt err( KErrNone );

        // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/TURNSrvAddr
        if ( KNSmlDMTURNSrvAddr() == lastURISeg )
            {
            err = iNATFWDomainEntries->At( domainLoc )->
                UpdateTurnSrvAddrL( turnLoc, aObject );
            }
        // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/TURNSrvPort
        else if ( KNSmlDMTURNSrvPort() == lastURISeg )
            {
            err = iNATFWDomainEntries->At( domainLoc )->
                UpdateTurnSrvPort( turnLoc, DesToInt( aObject ) );
            }
        // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/TURNUsername
        else if ( KNSmlDMTURNUsername() == lastURISeg )
            {
            err = iNATFWDomainEntries->At( domainLoc )->
                UpdateTurnUsernameL( turnLoc, aObject );
            }
        // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/TURNPassword
        else if ( KNSmlDMTURNPassword() == lastURISeg )
            {
            err = iNATFWDomainEntries->At( domainLoc )->
                UpdateTurnPasswordL( turnLoc, aObject );
            }
        else
            {
            // None of TURNServer settings found.
            status = CSmlDmAdapter::ENotFound;
            }

        if ( KErrNone != err )
            {
            status = CSmlDmAdapter::EError;
            }
        }

    // Append command to buffered commands if it was made ok.
    if ( EOk == status )    
        {
        iNATFWCommandBuffer.AppendL( statusInfo );
        }
    
    return status;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::DomainChildURIListL
// Lists the Domain specific children of the node.
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError CNSmlDmNATFWTraversalAdapter::DomainChildURIListL(
    const TDesC8& aURI,
    const TDesC8& aLUID, 
    CBufBase* const aURISegmentList,
    const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList )
    {
    __ASSERT_ALWAYS( aURISegmentList != NULL, User::Leave( KErrArgument ) );
    
    // get needed URI segments.
    TBuf8<KSmlMaxURISegLen> segment = LastURISeg( aURI );
    TBuf8<KSmlMaxURISegLen> segmentName;
    TPtrC8 uriTmp = RemoveLastURISeg( aURI );
    TPtrC8 secondLastUriSeg = LastURISeg( uriTmp );
    
    TBuf8<KNSmlNATFWMaxUriLength> mappingInfo( KNullDesC8 );
    TBuf8<KNSmlNATFWMaxUriLength> uri = aURI;
    
    // Central Repository for NAT-FW Traversal settings
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1

    if ( KNSmlDMDomainSpecificNodeName() == segment )   
        {
        TInt domainCount( 0 );
        if ( iNATFWDomainEntries )
            {
            domainCount = iNATFWDomainEntries->Count();
            }
        for ( TInt counter = 0; counter < domainCount ; counter++ )
            { 
            TBool inList = EFalse;
            TInt currentline = 0;
            TUint32 ddId ( iNATFWDomainEntries->At( 
                counter )->GetDomainKey() );
            while ( !inList && currentline < aPreviousURISegmentList.Count() )
                {
                TInt profileLocation = FindDomainLocation ( DesToInt( 
                    LastURISeg( aPreviousURISegmentList.
                        At( currentline ).iURISegLUID ) ) );
                if ( profileLocation != KErrNotFound )
                    {
                    TUint32 profileID ( iNATFWDomainEntries->
                        At( profileLocation )->GetDomainKey() );
                    if ( profileID == ddId ) 
                        {
                        inList = ETrue;
                        break;
                        }
                    }
                currentline++;
                }       
            if ( !inList )
                {

                // Use domain key as run-time node.
                segmentName.Num( iNATFWDomainEntries->
                        At( counter )->GetDomainKey() );
                aURISegmentList->InsertL( aURISegmentList->
                    Size(), segmentName );
                aURISegmentList->InsertL( aURISegmentList->
                    Size(), KNSmlDMNATFWSeparator );

                // Set LUID mapping to DM module.
                mappingInfo.Copy( KNSmlDMDomainSpecificNodeName );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                mappingInfo.AppendNum( iNATFWDomainEntries->
                    At( counter )->GetDomainKey() );
                uri.Copy( KNSmlDMNATFWNodeName );
                uri.Append( KNSmlDMNATFWSeparator );
                uri.Append( KNSmlDMDomainSpecificNodeName );
                uri.Append( KNSmlDMNATFWSeparator );
                uri.AppendNum( iNATFWDomainEntries->
                    At( counter )->GetDomainKey() );
                iDmCallback->SetMappingL( uri, mappingInfo ); 
                }
            else
                { // Add those in PreviousList which are on database.
                aURISegmentList->InsertL( aURISegmentList->Size(),
                    aPreviousURISegmentList.At( currentline ).iURISeg );
                aURISegmentList->InsertL( aURISegmentList->Size(),
                     KNSmlDMNATFWSeparator );
                }
            }
        }

    // ==============================
    // DomainSpecific/<x>/
    // ==============================
    //
    else if ( KThreeSegs == NumOfURISegs( aURI ) &&
        KNSmlDMDomainSpecificNodeName() == secondLastUriSeg 
        && aLUID.Length() )  
        {
        segmentName.Copy( KNSmlDMDomain );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );    

        segmentName.Copy( KNSmlDMSTUNSrvAddr );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMSTUNSrvPort );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMSTUNUsername );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMSTUNPassword );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMDomainNATRefreshTCP );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMDomainNATRefreshUDP );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMDomainEnableCRLFRefresh );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMAdditionalSTUNNodeName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMSTUNSharedSecret );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMPortPoolStart );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMPortPoolEnd );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMUsedNatProtocol );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMICESpecificNodeName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMTURNSpecificNodeName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );
        }

    // ==============================
    // AdditionalSTUNServer
    // ==============================
    //
    else if ( KFourSegs == NumOfURISegs( aURI ) &&
        KNSmlDMAdditionalSTUNNodeName() == segment
        && aLUID.Length() )
        {
        TInt domainLoc( KErrNotFound );

        if ( KFourSegs == NumOfURISegs( aLUID ) )
            {
            TPtrC8 tmpLuid = RemoveLastURISeg( aLUID );
            TPtrC8 domainLuid = RemoveLastURISeg( tmpLuid );
            domainLoc = FindDomainLocation( domainLuid );
            }
        else if ( KTwoSegs == NumOfURISegs( aLUID ) )
            {
            domainLoc = FindDomainLocation( aLUID );
            }

        if ( KErrNotFound == domainLoc )
            {
            CleanupStack::PopAndDestroy( rep ); // CS:0
            return ENotFound;
            }

        RArray<TUint32> stunKeys;
        CleanupClosePushL( stunKeys ); // CS:2

        TUint32 domainKey = iNATFWDomainEntries->At( 
            domainLoc )->GetDomainKey();
        domainKey |= KUNSAFProtocolsFieldTypeMask;
        domainKey ^= KUNSAFProtocolsFieldTypeMask;
        rep->FindL( KUNSAFProtocolsSTUNAddressMask|domainKey,
            KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );

        const TInt stunServerCount( stunKeys.Count() );

        // First CentRep key is reserved for STUN settings directly under
        // DomainSpecific node so counter starts from 1.
        for ( TInt counter = 1; counter < stunServerCount; counter++ )
            { 
            TInt prevUriSegListCount( aPreviousURISegmentList.Count() );
            TBool inList( EFalse );
            TInt currentLine( 0 );
            while ( !inList && currentLine < prevUriSegListCount )
                {
                TUint32 lastSeg = DesToInt( LastURISeg( 
                    aPreviousURISegmentList.At( currentLine ).iURISegLUID ) );
                if ( lastSeg == stunKeys[counter] )
                    {
                    inList = ETrue;
                    break;
                    }
                currentLine++;
                }
            if ( !inList )
                {
                // Use AdditionalSTUNServer/STUNSrvAddr key as run-time node.
                segmentName.Num( stunKeys[counter] );
                aURISegmentList->InsertL( aURISegmentList->Size(),
                    segmentName );
                aURISegmentList->InsertL( aURISegmentList->
                    Size(), KNSmlDMNATFWSeparator );

                // Set LUID mapping to DM module.
                mappingInfo.Copy( KNSmlDMDomainSpecificNodeName );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                mappingInfo.AppendNum( domainKey|KUNSAFProtocolsDomainMask );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                mappingInfo.Append( KNSmlDMAdditionalSTUNNodeName );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                // STUNSrvAddr key is used as LUID.
                mappingInfo.AppendNum( stunKeys[counter] );
                uri.Copy( aURI );
                uri.Append( KNSmlDMNATFWSeparator );
                uri.AppendNum ( stunKeys[counter] );
                iDmCallback->SetMappingL( uri, mappingInfo ); 
                }
            else
                {
                // Add the one in aPreviousURISegmentList into
                // aURISegmentList.
                aURISegmentList->InsertL( aURISegmentList->Size(),
                    aPreviousURISegmentList.At( currentLine ).iURISeg );
                aURISegmentList->InsertL( aURISegmentList->Size(),
                     KNSmlDMNATFWSeparator );
                }
            }
        stunKeys.Close();
        CleanupStack::PopAndDestroy( &stunKeys ); // CS:1
        }

    // ==============================
    // AdditionalSTUNServer/<x>
    // ==============================
    //
    else if ( KFiveSegs == NumOfURISegs( aURI ) &&
        KNSmlDMAdditionalSTUNNodeName() == secondLastUriSeg 
        && aLUID.Length() )
        {
        segmentName.Copy( KNSmlDMSTUNSrvAddr );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMSTUNSrvPort );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMSTUNUsername );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMSTUNPassword );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        }

    // ==============================
    // ICESpecific
    // ==============================
    //   
    else if ( KFourSegs == NumOfURISegs( aURI ) &&
        KNSmlDMICESpecificNodeName() == segment 
        && aLUID.Length() )  
        {
        segmentName.Copy( KNSmlDMNatUtilities );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMHostPref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMServerReflexPref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMRelayPref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMPeerReflexPref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMIPv4Pref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMIPv6Pref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMVpnPref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMUdpPref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMTcpPref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMTcpActivePref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMTcpPassivePref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMTcpSimultPref );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );
        }

    // ==============================
    // TURNSpecific
    // ==============================
    //   
    else if ( KFourSegs == NumOfURISegs( aURI ) &&
        KNSmlDMTURNSpecificNodeName() == segment 
        && aLUID.Length() )  
        {
        segmentName.Copy( KNSmlDMTURNServerNodeName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );
        }
    
    // ==============================
    // TURNSpecific/TURNServer
    // ==============================
    //
    else if ( KFiveSegs == NumOfURISegs( aURI ) &&
        KNSmlDMTURNServerNodeName() == segment
        && aLUID.Length() )
        {
        TInt domainLoc( KErrNotFound );

        if ( KFourSegs == NumOfURISegs( aLUID ) )
            {
            TPtrC8 tmpLuid = RemoveLastURISeg( aLUID );
            TPtrC8 domainLuid = RemoveLastURISeg( tmpLuid );
            domainLoc = FindDomainLocation( domainLuid );
            }
        else if ( KTwoSegs == NumOfURISegs( aLUID ) )
            {
            domainLoc = FindDomainLocation( aLUID );
            }

        if ( KErrNotFound == domainLoc )
            {
            CleanupStack::PopAndDestroy( rep ); // CS:0
            return ENotFound;
            }

        RArray<TUint32> turnKeys;
        CleanupClosePushL( turnKeys ); // CS:2

        TUint32 domainKey = iNATFWDomainEntries->At( 
            domainLoc )->GetDomainKey();
        domainKey |= KUNSAFProtocolsFieldTypeMask;
        domainKey ^= KUNSAFProtocolsFieldTypeMask;
        rep->FindL( KUNSAFProtocolsTURNAddressMask|domainKey,
            KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );

        const TInt turnServerCount( turnKeys.Count() );

        for ( TInt counter = 0; counter < turnServerCount; counter++ )
            { 
            TInt prevUriSegListCount( aPreviousURISegmentList.Count() );
            TBool inList( EFalse );
            TInt currentLine( 0 );
            while ( !inList && currentLine < prevUriSegListCount )
                {
                TUint32 lastSeg = DesToInt( LastURISeg( 
                    aPreviousURISegmentList.At( currentLine ).iURISegLUID ) );
                if ( lastSeg == turnKeys[counter] )
                    {
                    inList = ETrue;
                    break;
                    }
                currentLine++;
                }
            if ( !inList )
                {
                // Use TURNSpecific/TURNServer/TURNSrvAddr key as run-time node.
                segmentName.Num( turnKeys[counter] );
                aURISegmentList->InsertL( aURISegmentList->Size(),
                    segmentName );
                aURISegmentList->InsertL( aURISegmentList->
                    Size(), KNSmlDMNATFWSeparator );

                // Set LUID mapping to DM module.
                mappingInfo.Copy( KNSmlDMDomainSpecificNodeName );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                mappingInfo.AppendNum( domainKey|KUNSAFProtocolsDomainMask );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                mappingInfo.Append( KNSmlDMTURNSpecificNodeName );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                mappingInfo.Append( KNSmlDMTURNServerNodeName );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                // TURNSrvAddr key is used as LUID.
                mappingInfo.AppendNum( turnKeys[counter] );
                uri.Copy( aURI );
                uri.Append( KNSmlDMNATFWSeparator );
                uri.AppendNum ( turnKeys[counter] );
                iDmCallback->SetMappingL( uri, mappingInfo ); 
                }
            else
                {
                // Add the one in aPreviousURISegmentList into
                // aURISegmentList.
                aURISegmentList->InsertL( aURISegmentList->Size(),
                    aPreviousURISegmentList.At( currentLine ).iURISeg );
                aURISegmentList->InsertL( aURISegmentList->Size(),
                     KNSmlDMNATFWSeparator );
                }
            }
        turnKeys.Close();
        CleanupStack::PopAndDestroy( &turnKeys ); // CS:1
        }

    // ==============================
    // TURNSpecific/TURNServer/<x>
    // ==============================
    //
    else if ( KSixSegs == NumOfURISegs( aURI ) &&
        KNSmlDMTURNServerNodeName() == secondLastUriSeg &&
        KErrNotFound != aURI.Find( KNSmlDMTURNSpecificNodeName() ) &&
        aLUID.Length() )
        {
        segmentName.Copy( KNSmlDMTURNSrvAddr );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMTURNSrvPort );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMTURNUsername );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMTURNPassword );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );
        }

    CleanupStack::PopAndDestroy( rep ); // CS:0
    
    // no errors
    return EOk;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::IAPChildURIListL
// Lists the IAP specific children of the node.
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError CNSmlDmNATFWTraversalAdapter::IAPChildURIListL(
    const TDesC8& aURI,
    const TDesC8& aLUID, 
    CBufBase* const aURISegmentList,
    const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList )
    {
    __ASSERT_ALWAYS( aURISegmentList != NULL, User::Leave( KErrArgument ) );
    
    CSmlDmAdapter::TError retValue = EOk;
    
    // get needed URI segments.
    TBuf8<KSmlMaxURISegLen> segment = LastURISeg( aURI );
    TBuf8<KSmlMaxURISegLen> segmentName;
    TPtrC8 uriTmp = RemoveLastURISeg( aURI );
    TPtrC8 secondLastUriSeg = LastURISeg( uriTmp );
    
    TBuf8<KNSmlNATFWMaxUriLength> mappingInfo( KNullDesC8 );
    TBuf8<KNSmlNATFWMaxUriLength> uri = aURI;

    // IAPSpecific/
    if ( KNSmlDMIAPSpecificNodeName() == segment )
        {
        TInt iapCount( 0 );
        if ( iNATFWIAPEntries )
            {
            iapCount = iNATFWIAPEntries->Count();
            }            
            
        for ( TInt counter = 0; counter < iapCount ; counter++ )
            { 
            TBool inList = EFalse;
            TInt currentline = 0;
            TUint32 ddId ( iNATFWIAPEntries->At( counter )->GetIAPKey() );
            while ( !inList && currentline < aPreviousURISegmentList.Count() )
                {
                TInt profileLocation = FindIAPLocation ( 
                    aPreviousURISegmentList.At( currentline ).iURISegLUID );
                if ( profileLocation != KErrNotFound )
                    {
                    TUint32 profileID ( iNATFWIAPEntries->
                        At( profileLocation )->GetIAPKey() );
                    if ( profileID == ddId ) 
                        {
                        inList = ETrue;
                        break;
                        }
                    }
                currentline++;
                }       
            if ( !inList )
                {
                segmentName.Num( iNATFWIAPEntries->At( counter )->
                    GetIAPKey() );
                aURISegmentList->InsertL( aURISegmentList->
                    Size(), segmentName );
                aURISegmentList->InsertL( aURISegmentList->
                   Size(), KNSmlDMNATFWSeparator );

                // set luid mapping to dm-module
                mappingInfo.Copy( KNSmlDMIAPSpecificNodeName );
                mappingInfo.Append( KNSmlDMNATFWSeparator );
                mappingInfo.AppendNum( iNATFWIAPEntries->
                    At( counter )->GetIAPKey() );
                uri.Copy( KNSmlDMNATFWNodeName );
                uri.Append( KNSmlDMNATFWSeparator );
                uri.Append( KNSmlDMIAPSpecificNodeName );
                uri.Append( KNSmlDMNATFWSeparator );
                uri.AppendNum( iNATFWIAPEntries->At( counter )->GetIAPKey() );
                iDmCallback->SetMappingL( uri, mappingInfo ); 
                }
            else
                { // Add those in PreviousList which are on database
                aURISegmentList->InsertL( aURISegmentList->Size(),
                    aPreviousURISegmentList.At( currentline ).iURISeg );
                aURISegmentList->InsertL( aURISegmentList->Size(),
                     KNSmlDMNATFWSeparator );
                }
            }
        }

    // IAPSpecific/<x>/
    else if ( KThreeSegs == NumOfURISegs( aURI ) &&
        KNSmlDMIAPSpecificNodeName() == secondLastUriSeg 
        && aLUID.Length() )  
        {
        segmentName.Copy( KNSmlDMNATPrefConRef );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMNATRefreshTCP );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMNATRefreshUDP );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );

        segmentName.Copy( KNSmlDMSTUNRetransmit );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), segmentName );
        aURISegmentList->InsertL( aURISegmentList->
            Size(), KNSmlDMNATFWSeparator );
        }
    else
        {
        retValue = ENotFound;
        }
        
    return retValue;
    }
    

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::LoadProfilesL
// Loads all nat-fw settings information to memory.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::LoadProfilesL()
    {
    // Central Repository for NAT-FW Traversal settings
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1

    // Load domain specific data
    RArray<TUint32> keys;
    CleanupClosePushL( keys ); // CS:2
    rep->FindL( KUNSAFProtocolsDomainMask, 
        KUNSAFProtocolsFieldTypeMask, keys ); 
    iNATFWDomainEntries = new (ELeave) CArrayPtrFlat<CNATFWDomainEntry>( 
        keys.Count() + 1 );
    for ( TInt counter = 0; counter < keys.Count(); counter++ )
        {
        CNATFWDomainEntry* domainEntry = CNATFWDomainEntry::NewLC();
            // CS:3
        GetDomainSettingsL( keys[counter], *domainEntry );
        iNATFWDomainEntries->AppendL( domainEntry );
        CleanupStack::Pop( domainEntry ); // CS:2
        }
    keys.Reset();

    // Load IAP specific data
    rep->FindL( KUNSAFProtocolsIAPIdMask, 
        KUNSAFProtocolsFieldTypeMask, keys ); 
    iNATFWIAPEntries = new (ELeave) CArrayPtrFlat<CNATFWIAPEntry>( 
        keys.Count() + 1 );

    for ( TInt counter = 0; counter < keys.Count(); counter++ )
        {
        CNATFWIAPEntry* iapEntry = CNATFWIAPEntry::NewLC(); // CS:3
        GetIAPSettingsL( keys[counter], *iapEntry );
        iNATFWIAPEntries->AppendL( iapEntry );
        CleanupStack::Pop( iapEntry ); // CS:2
        }

    keys.Close();
    CleanupStack::PopAndDestroy( &keys ); // CS:1
    CleanupStack::PopAndDestroy( rep ); // CS:0
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::SaveProfilesL
// Saves all nat-fw settings information back to persistent memory.
// Buffered commands are executed here.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::SaveProfilesL()
    {
    __ASSERT_ALWAYS( iDmCallback != NULL, User::Leave( KErrArgument ) );

    // Central Repository for NAT-FW Traversal settings
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1

    if ( iNATFWDomainEntries && iNATFWIAPEntries )
        {
        // ==============================
        // Add commands first
        // ==============================
        //
        AddSettingsL( rep );

        // ==============================
        // Delete commands
        // ==============================
        //
        DeleteSettingsL( rep );

        // ==============================
        // Update commands, whole entry.
        // ==============================
        //
        UpdateSettingsL( rep );
        }

    iNATFWCommandBuffer.Reset();    
    CleanupStack::PopAndDestroy( rep );  // CS:0
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::AddSettingsL
// Adds NAT-FW settings.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::AddSettingsL( CRepository* aRep )
    {
    __ASSERT_ALWAYS( aRep != NULL, User::Leave( KErrArgument ) );
    
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMAddDomain )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            RArray<TUint32> keys;
            CleanupClosePushL( keys ); // CS:2
            aRep->FindL( KUNSAFProtocolsDomainMask, 
                KUNSAFProtocolsFieldTypeMask, keys );
            TUint32 currentKey = iNATFWCommandBuffer[counter].iKeyId;
            TInt domainLocation = FindDomainLocation( currentKey );
            TInt err( KErrNone );

            currentKey &= KUNSAFProtocolsKeyMask;
            // create keys
            err = aRep->Create( currentKey|KUNSAFProtocolsDomainMask,
                iNATFWDomainEntries->At( domainLocation )->GetDomain() );

            if ( KErrNone == err )
                {
                err = aRep->Create( 
                    KUNSAFProtocolsSTUNServerMask|currentKey,
                    KDefaultSTUNSrvAddr );
                }
            if ( KErrNone == err )
                {
                err = aRep->Create( 
                    KUNSAFProtocolsSTUNServerPortMask|currentKey,
                    KDefaultSTUNPort );
                }
            if ( KErrNone == err )
                {
                err = aRep->Create( 
                    KUNSAFProtocolsDomainIntervalUDPMask|currentKey, 
                    KDefaultUDPRefreshInterval );
                }
            if ( KErrNone == err )
                {
                err = aRep->Create( 
                    KUNSAFProtocolsDomainIntervalTCPMask|currentKey, 
                    KDefaultTCPRefreshInterval );
                }
            if ( KErrNone == err )
                {
                err = aRep->Create( 
                    KUNSAFProtocolsDomainEnableCRLFRefresh|
                    currentKey, KDefaultSTUNRetransmitTimer );
                }

            TUint32 currentNewKey = currentKey;
            currentNewKey |= KUNSAFProtocolsFieldTypeMask;
            currentNewKey ^= KUNSAFProtocolsFieldTypeMask;

            TUint32 stunKey;
            stunKey = KUNSAFProtocolsSubTableFieldTypeMask + 1;
            stunKey |= KUNSAFProtocolsSubTableFieldTypeMask;
            stunKey = stunKey^KUNSAFProtocolsSubTableFieldTypeMask|
                currentNewKey;

            // DomainSpecific/<x>/STUNSrvAddr
            if ( KErrNone == err )
                {
                err = aRep->Create(
                    KUNSAFProtocolsSTUNAddressMask|stunKey,
                    KDefaultSTUNSrvAddr );
                }
            // DomainSpecific/<x>/STUNSrvPort
            if ( KErrNone == err )
                {
                err = aRep->Create( 
                    KUNSAFProtocolsSTUNPortMask|stunKey,
                    KDefaultSTUNPort );
                }
            // DomainSpecific/<x>/STUNSharedSecret
            if ( KErrNone == err )
                {
                err = aRep->Create( currentNewKey|
                   KUNSAFProtocolsDomainSharedSecretNotSupported,
                   KDefaultDisableStunSharedSecret );
                }
            // DomainSpecific/<x>/PortPoolStartPort
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsPortPoolStartPortMask|currentNewKey,
                   KDefaultStartPort );
                }
            // DomainSpecific/<x>/PortPoolEndPort
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsPortPoolEndPortMask|currentNewKey,
                   KDefaultEndPort );
                }
            // DomainSpecific/<x>/UsedNATProtocol
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsUsedNATProtocolMask|currentNewKey,
                   KDefaultNatProtocol );
                }

            // DomainSpecific/<x>/ICESpecific/NATUtilities
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsNATUtilities|currentNewKey,
                   KDefaultNatUtilities );
                }
            // DomainSpecific/<x>/ICESpecific/HostPreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsHostPref|currentNewKey,
                   KDefaultHostPref );
                }
            // DomainSpecific/<x>/ICESpecific/ServerReflexivePreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsServerRefPref|currentNewKey,
                   KDefaultServerReflexivePref );
                }
            // DomainSpecific/<x>/ICESpecific/RelayPreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsRelayPref|currentNewKey,
                   KDefaultRelayPref );
                }
            // DomainSpecific/<x>/ICESpecific/PeerReflexivePreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsPeerRefPref|currentNewKey,
                   KDefaultPeerReflexivePref );
                }
            // DomainSpecific/<x>/ICESpecific/IPv4Preference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsIPV4Pref|currentNewKey,
                   KDefaultIPv4Pref );
                }
            // DomainSpecific/<x>/ICESpecific/IPv6Preference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsIPV6Pref|currentNewKey,
                   KDefaultIPv6Pref );
                }
            // DomainSpecific/<x>/ICESpecific/VPNPreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsVPNPref|currentNewKey,
                   KDefaultVPNPref );
                }
            // DomainSpecific/<x>/ICESpecific/UDPPreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsUDPPref|currentNewKey,
                   KDefaultUDPPref );
                }
            // DomainSpecific/<x>/ICESpecific/TCPPreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsTCPPref|currentNewKey,
                   KDefaultTCPPref );
                }
            // DomainSpecific/<x>/ICESpecific/TCPActivePreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsTCPActivePref|currentNewKey,
                   KDefaultTCPActivePref );
                }
            // DomainSpecific/<x>/ICESpecific/TCPPassivePreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsTCPPassivePref|currentNewKey,
                   KDefaultTCPPassivePref );
                }
            // DomainSpecific/<x>/ICESpecific/TCPSimultaneousPreference
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsTCPSimultPref|currentNewKey,
                   KDefaultTCPSimultaneousPref );
                }

            if ( KErrNone == err )    
                {
                retValue = EOk;
                }
            else 
                {
                retValue = EError;
                } 
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );     
            keys.Close();
            CleanupStack::PopAndDestroy( &keys ); // CS:1
            }
        }
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        // IAPSpecific/x/  add commands
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMAddIAP )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            RArray<TUint32> keys;
            CleanupClosePushL( keys ); // CS:2
            TInt err = aRep->FindL( KUNSAFProtocolsIAPIdMask, 
                KUNSAFProtocolsFieldTypeMask, keys );
            TUint32 currentKey = iNATFWCommandBuffer[counter].iKeyId;
            TInt iapLocation = FindIAPLocation( currentKey );
            currentKey &= KUNSAFProtocolsKeyMask;

            // Create keys.
            err = aRep->Create( currentKey|KUNSAFProtocolsIAPIdMask, 
                (TInt)iNATFWIAPEntries->At( iapLocation )->GetIAPId() );
            if ( KErrNone == err )
                {
                err = aRep->Create( 
                    KUNSAFProtocolsIntervalUDPMask|currentKey, 
                    KDefaultUDPRefreshInterval );

                if ( KErrNone == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsIntervalTCPMask|currentKey, 
                        KDefaultTCPRefreshInterval );
                    }

                if ( KErrNone == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsSTUNRetransmitTimerMask|
                        currentKey, KDefaultSTUNRetransmitTimer );
                    }
                if ( KErrNone == err )
                    {
                    retValue = EOk;
                    }
                else 
                    {
                    retValue = EError;
                    }
                iDmCallback->SetStatusL( 
                    iNATFWCommandBuffer[counter].iStatusRef, retValue );
                }
            keys.Close();
            CleanupStack::PopAndDestroy( &keys ); // CS:1
            }
        }

    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        // DomainSpecific/<x>/AdditionalSTUNServer/<x>/ add commands
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMAddSTUN )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            TUint32 stunKey = iNATFWCommandBuffer[counter].iKeyId;
            // Stored key is for STUNSrvAddr so we must remove
            // STUNSrvAddr mask.
            stunKey ^= KUNSAFProtocolsSTUNAddressMask;
            TInt err = aRep->Create(
                    KUNSAFProtocolsSTUNAddressMask|stunKey,
                    KDefaultSTUNSrvAddr );
            if ( KErrNone == err )
                {
                err = aRep->Create( 
                    KUNSAFProtocolsSTUNPortMask|stunKey,
                    KDefaultSTUNPort );
                }
            // STUN username and password won't be created.
            if ( KErrNone == err )    
                {
                retValue = EOk;
                }
            else 
                {
                retValue = EError;
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );
            }
        }
        
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); counter++ )
        {
        // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/ add commands
        if ( iNATFWCommandBuffer[counter].iCommandType ==
            TDMNATFWStatusInfo::EDMAddTURN )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            TUint32 turnKey = iNATFWCommandBuffer[counter].iKeyId;
            
            TInt err = aRep->Create(
                KUNSAFProtocolsTURNAddressMask|turnKey,
                KDefaultTURNSrvAddr );
            if ( KErrNone == err )
                {
                err = aRep->Create(
                   KUNSAFProtocolsTURNPortMask|turnKey,
                   KDefaultTURNSrvPort );
                }
            // TURN username and password won't be created.
            
            if ( KErrNone == err )
                {
                retValue = EOk;
                }
            else
                {
                retValue = EError;
                }
            iDmCallback->SetStatusL(
                iNATFWCommandBuffer[counter].iStatusRef, retValue );
            }
        }
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::UpdateSettings
// Updates NAT-FW settings.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::UpdateSettingsL( CRepository* aRep )
    {
    __ASSERT_ALWAYS( aRep != NULL, User::Leave( KErrArgument ) );
    
    // Update Domain entry.
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMUpdateDomain )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            RArray<TUint32> keys;
            CleanupClosePushL( keys ); // CS:2
            aRep->FindL( KUNSAFProtocolsDomainMask, 
                KUNSAFProtocolsFieldTypeMask, keys );
            TInt32 currentKey = iNATFWCommandBuffer[counter].iKeyId;
            TInt domainLocation = FindDomainLocation( currentKey );
            TInt err( KErrNone );
            currentKey &= KUNSAFProtocolsKeyMask;

            // Update keys. Set domain.

            // DomainSpecific/<x>/Domain
            if ( iNATFWDomainEntries->At( domainLocation )->
                GetDomain() != KNSmlDMNATFWValueNotSet )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsDomainMask|currentKey, 
                    iNATFWDomainEntries->At( domainLocation )->
                    GetDomain() );
                }

            // DomainSpecific/<x>/StunSrvAddr
            if ( KErrNone == err && iNATFWDomainEntries->
                At( domainLocation )->GetSTUNAddr() != 
                KNSmlDMNATFWValueNotSet )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsSTUNServerMask|currentKey, 
                    iNATFWDomainEntries->At( domainLocation )->
                    GetSTUNAddr() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsSTUNServerMask|currentKey, 
                        iNATFWDomainEntries->At( domainLocation )->
                        GetSTUNAddr() );
                    }
                }
            // DomainSpecific/<x>/StunSrvPort
            if ( KErrNone == err && iNATFWDomainEntries->
                At( domainLocation )->GetSTUNPort() != KErrNotFound )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsSTUNServerPortMask|currentKey, 
                    ( TInt ) iNATFWDomainEntries->At( domainLocation )->
                    GetSTUNPort() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsSTUNServerPortMask|currentKey, 
                        ( TInt ) iNATFWDomainEntries->At( domainLocation )->
                        GetSTUNPort() );
                    }
                }

            // DomainSpecific/<x>/StunUsername
            if ( KErrNone == err && iNATFWDomainEntries->
                At( domainLocation )->GetSTUNUsername() != 
                KNSmlDMNATFWValueNotSet )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsSTUNUsernameMask|currentKey, 
                    iNATFWDomainEntries->At( domainLocation )->
                    GetSTUNUsername() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsSTUNUsernameMask|currentKey, 
                        iNATFWDomainEntries->At( domainLocation )->
                        GetSTUNUsername() );
                    }
                }
            // DomainSpecific/<x>/StunPassword
            if ( KErrNone == err && iNATFWDomainEntries->
                At( domainLocation )->GetSTUNPassword() != 
                KNSmlDMNATFWValueNotSet )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsSTUNPasswordMask|currentKey, 
                    iNATFWDomainEntries->At( domainLocation )->
                    GetSTUNPassword() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsSTUNPasswordMask|currentKey, 
                        iNATFWDomainEntries->At( domainLocation )->
                        GetSTUNPassword() );
                    }
                }

            // DomainSpecific/<x>/NATRefreshUDP
            if ( KErrNone == err && iNATFWDomainEntries->
                At( domainLocation )->GetNATRefreshUDP() != KErrNotFound )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsDomainIntervalUDPMask|currentKey, 
                    iNATFWDomainEntries->At( domainLocation )->
                    GetNATRefreshUDP() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsDomainIntervalUDPMask|currentKey, 
                        iNATFWDomainEntries->At( domainLocation )->
                        GetNATRefreshUDP() );
                    }
                }
            // DomainSpecific/<x>/NATRefreshTCP
            if ( KErrNone == err && iNATFWDomainEntries->
                At( domainLocation )->GetNATRefreshTCP() != KErrNotFound )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsDomainIntervalTCPMask|currentKey, 
                    iNATFWDomainEntries->At( domainLocation )->
                    GetNATRefreshTCP() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsDomainIntervalTCPMask|currentKey, 
                        iNATFWDomainEntries->At( domainLocation )->
                        GetNATRefreshTCP() );
                    }
                }
            // DomainSpecific/<x>/EnableCRLFRefresh
            if ( KErrNone == err && iNATFWDomainEntries->
                At( domainLocation )->GetEnableCRLFRefresh() != KErrNotFound )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsDomainEnableCRLFRefresh|currentKey, 
                    iNATFWDomainEntries->At( domainLocation )->
                    GetEnableCRLFRefresh() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsDomainEnableCRLFRefresh|currentKey, 
                        iNATFWDomainEntries->At( domainLocation )->
                        GetEnableCRLFRefresh() );
                    }
                }

            TUint32 currentNewKey = currentKey;
            currentNewKey |= KUNSAFProtocolsFieldTypeMask;
            currentNewKey ^= KUNSAFProtocolsFieldTypeMask;

            RArray<TUint32> stunKeys;
            CleanupClosePushL( stunKeys ); // CS:3
            err = aRep->FindL( 
                currentNewKey|KUNSAFProtocolsSTUNAddressMask,
                KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );

            // STUN server keys directly under DomainSpecific node are in
            // CentRep table index 0, i.e. stunKeys[0]. We must remove
            // STUN server address mask from the key.
            TUint32 stunKey( 0 );
            if ( KErrNone == err )
                {
                stunKey = KUNSAFProtocolsSTUNAddressMask^stunKeys[0];
                }
            stunKeys.Close();
            CleanupStack::PopAndDestroy( &stunKeys ); // CS:2

            // DomainSpecific/<x>/STUNSrvAddr
            if ( KErrNone == err && iNATFWDomainEntries->At( 
                domainLocation )->StunSrvAddrL( 0 ) != 
                KNSmlDMNATFWValueNotSet )
                {
                err = aRep->Set( stunKey|KUNSAFProtocolsSTUNAddressMask,
                    iNATFWDomainEntries->At( domainLocation )->
                    StunSrvAddrL( 0 ) );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        stunKey|KUNSAFProtocolsSTUNAddressMask,
                        iNATFWDomainEntries->At( domainLocation )->
                        StunSrvAddrL( 0 ) );
                    }
                }

            // DomainSpecific/<x>/STUNSrvPort
            if ( KErrNone == err && iNATFWDomainEntries->At( 
                domainLocation )->StunSrvPort( 0 ) != KErrNotFound )
                {
                err = aRep->Set( stunKey|KUNSAFProtocolsSTUNPortMask,
                    iNATFWDomainEntries->At( domainLocation )->
                    StunSrvPort( 0 ) );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( stunKey|KUNSAFProtocolsSTUNPortMask,
                        iNATFWDomainEntries->At( domainLocation )->
                        StunSrvPort( 0 ) );
                    }
                }

            // DomainSpecific/<x>/STUNUsername
            if ( KErrNone == err && iNATFWDomainEntries->At( 
                domainLocation )->StunUsernameL( 0 ) != 
                KNSmlDMNATFWValueNotSet )
                {
                err = aRep->Set( stunKey|KUNSAFProtocolsSTUNUsernameMask,
                    iNATFWDomainEntries->At( domainLocation )->
                    StunUsernameL( 0 ) );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        stunKey|KUNSAFProtocolsSTUNUsernameMask,
                        iNATFWDomainEntries->At( domainLocation )->
                        StunUsernameL( 0 ) );
                    }
                }

            // DomainSpecific/<x>/STUNPassword
            if ( KErrNone == err && iNATFWDomainEntries->At( 
                domainLocation )->StunPasswordL( 0 ) != 
                KNSmlDMNATFWValueNotSet )
                {
                err = aRep->Set( stunKey|KUNSAFProtocolsSTUNPasswordMask,
                    iNATFWDomainEntries->At( domainLocation )->
                    StunPasswordL( 0 ) );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        stunKey|KUNSAFProtocolsSTUNPasswordMask,
                        iNATFWDomainEntries->At( domainLocation )->
                        StunPasswordL( 0 ) );
                    }
                }

            // DomainSpecific/<x>/STUNSharedSecret
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->DisableStunSharedSecret() )
                {
                err = aRep->Set( currentNewKey|
                    KUNSAFProtocolsDomainSharedSecretNotSupported,
                    iNATFWDomainEntries->At( domainLocation )->
                    DisableStunSharedSecret() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( currentNewKey|
                        KUNSAFProtocolsDomainSharedSecretNotSupported,
                        iNATFWDomainEntries->At( domainLocation )->
                        DisableStunSharedSecret() );
                    }
                }

            // DomainSpecific/<x>/PortPoolStartPort
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->StartPortRange() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsPortPoolStartPortMask|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    StartPortRange() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsPortPoolStartPortMask|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        StartPortRange() );
                    }
                }

            // DomainSpecific/<x>/PortPoolEndPort
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->EndPortRange() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsPortPoolEndPortMask|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    EndPortRange() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsPortPoolEndPortMask|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        EndPortRange() );
                    }
                }

            // DomainSpecific/<x>/UsedNATProtocol
            if ( KErrNone == err && KNSmlDMNATFWValueNotSet() != 
                iNATFWDomainEntries->At( domainLocation )->UsedNatProtocol() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsUsedNATProtocolMask|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    UsedNatProtocol() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsUsedNATProtocolMask|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        UsedNatProtocol() );
                    }
                }

            // DomainSpecific/<x>/ICESpecific/NATUtilities
            if ( KErrNone == err && KNSmlDMNATFWValueNotSet() != 
                iNATFWDomainEntries->At( domainLocation )->NatUtilities() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsNATUtilities|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    NatUtilities() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsNATUtilities|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        NatUtilities() );
                    }
                }

            // DomainSpecific/<x>/ICESpecific/HostPreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->HostPref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsHostPref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    HostPref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsHostPref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        HostPref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/ServerReflexivePreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->ServerReflexPref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsServerRefPref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    ServerReflexPref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsServerRefPref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        ServerReflexPref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/RelayPreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->RelayPref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsRelayPref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    RelayPref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsRelayPref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        RelayPref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/PeerReflexivePreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->PeerReflexPref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsPeerRefPref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    PeerReflexPref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsPeerRefPref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        PeerReflexPref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/IPv4Preference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->IPv4Pref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsIPV4Pref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    IPv4Pref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsIPV4Pref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        IPv4Pref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/IPv6Preference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->IPv6Pref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsIPV6Pref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    IPv6Pref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsIPV6Pref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        IPv6Pref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/VPNPreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->VpnPref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsVPNPref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    VpnPref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsVPNPref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        VpnPref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/UDPPreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->UdpPref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsUDPPref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    UdpPref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsUDPPref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        UdpPref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/TCPPreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->TcpPref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsTCPPref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    TcpPref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsTCPPref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        TcpPref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/TCPActivePreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->TcpActivePref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsTCPActivePref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    TcpActivePref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsTCPActivePref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        TcpActivePref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/TCPPassivePreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->TcpPassivePref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsTCPPassivePref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    TcpPassivePref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsTCPPassivePref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        TcpPassivePref() );
                    }
                }
            // DomainSpecific/<x>/ICESpecific/TCPSimultaneousPreference
            if ( KErrNone == err && KErrNotFound != iNATFWDomainEntries->
                At( domainLocation )->TcpSimultPref() )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsTCPSimultPref|currentNewKey,
                    iNATFWDomainEntries->At( domainLocation )->
                    TcpSimultPref() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( 
                        KUNSAFProtocolsTCPSimultPref|currentNewKey,
                        iNATFWDomainEntries->At( domainLocation )->
                        TcpSimultPref() );
                    }
                }

            if ( KErrNone == err )    
                {
                retValue = EOk;
                }
            else 
                {
                retValue = EError;
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );     
            keys.Close();
            CleanupStack::PopAndDestroy( &keys ); // CS:1
            }
        }

    // Update IAP specific entry.
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMUpdateIAP )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            RArray<TUint32> keys;
            CleanupClosePushL( keys ); // CS:2
            aRep->FindL( KUNSAFProtocolsIAPIdMask, 
                KUNSAFProtocolsFieldTypeMask, keys );
            TInt32 currentKey = iNATFWCommandBuffer[counter].iKeyId;
            TInt iapLocation = FindIAPLocation( currentKey );
            currentKey &= KUNSAFProtocolsKeyMask;

            // Update keys. Only keys with changed values are created/updated.
            TInt err ( 0 );
            if ( iNATFWIAPEntries->At( iapLocation )->GetIAPId() 
                != KErrNone )
                {
                err = aRep->Set( currentKey|KUNSAFProtocolsIAPIdMask, 
                    (TInt)iNATFWIAPEntries->At( 
                    iapLocation )->GetIAPId() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create( currentKey|KUNSAFProtocolsIAPIdMask, 
                        (TInt)iNATFWIAPEntries->At( 
                        iapLocation )->GetIAPId() );
                    }
                }
            if ( KErrNone == err && iNATFWIAPEntries->At( iapLocation )->
                GetIntervalUDP() != KErrNotFound )
                {
                err = aRep->Set( KUNSAFProtocolsIntervalUDPMask|currentKey, 
                    iNATFWIAPEntries->At( 
                    iapLocation )->GetIntervalUDP() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create(
                        KUNSAFProtocolsIntervalUDPMask|currentKey,
                        iNATFWIAPEntries->At( 
                        iapLocation )->GetIntervalUDP() );
                    }
                }
            if ( KErrNone == err && iNATFWIAPEntries->At( iapLocation )->
                GetIntervalTCP() != KErrNotFound )
                {
                err = aRep->Set( KUNSAFProtocolsIntervalTCPMask|currentKey,
                    iNATFWIAPEntries->At( 
                    iapLocation )->GetIntervalTCP() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create(
                        KUNSAFProtocolsIntervalTCPMask|currentKey,
                        iNATFWIAPEntries->At( 
                        iapLocation )->GetIntervalTCP() );
                    }
                }
            if ( KErrNone == err && iNATFWIAPEntries->At( iapLocation )->
                GetSTUNRetransmitTimer() != KErrNotFound )
                {
                err = aRep->Set( 
                    KUNSAFProtocolsSTUNRetransmitTimerMask|currentKey,
                    iNATFWIAPEntries->At( 
                    iapLocation )->GetSTUNRetransmitTimer() );
                if ( KErrNotFound == err )
                    {
                    err = aRep->Create(
                        KUNSAFProtocolsSTUNRetransmitTimerMask|currentKey,
                        iNATFWIAPEntries->At( 
                        iapLocation )->GetSTUNRetransmitTimer() );
                    }
                }
            if ( KErrNone == err )    
                {
                retValue = EOk;
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );
            keys.Close();
            CleanupStack::PopAndDestroy( &keys ); // CS:1
            }
        }

    // Update AdditionalSTUNServer node.
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMUpdateSTUN )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EError;
            TInt32 stunKey = iNATFWCommandBuffer[counter].iKeyId;
            TInt domainLoc = iNATFWCommandBuffer[counter].iLocation;
            TUint32 domainKey = iNATFWDomainEntries->At( domainLoc )->
                GetDomainKey();
            domainKey |= KUNSAFProtocolsFieldTypeMask;
            domainKey ^= KUNSAFProtocolsFieldTypeMask;
            RArray<TUint32> stunKeys;
            CleanupClosePushL( stunKeys ); // CS:2
            TInt err = aRep->FindL( 
                domainKey|KUNSAFProtocolsSTUNAddressMask,
                KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );
            TInt stunCount( stunKeys.Count() );
            TInt stunLoc( KErrNotFound );

            for ( TInt stunCounter = 0; stunCounter < stunCount; 
                stunCounter++ )
                {
                if ( stunKey == stunKeys[stunCounter] )
                    {
                    stunLoc = stunCounter;
                    }
                }
            stunKeys.Close();
            CleanupStack::PopAndDestroy( &stunKeys ); // CS:1

            // Matching key not found, set status to not found.
            if ( KErrNotFound == stunLoc )
                {
                retValue = ENotFound;
                }
            // STUN settings location starts from index 1 for 
            // AdditionalSTUNServer node.
            else if ( KErrNone == stunLoc )
                {
                retValue = EError;
                }
            // Update keys.
            else
                {
                // Remove STUN server address mask from the key.
                stunKey ^= KUNSAFProtocolsSTUNAddressMask;

                // AdditionalSTUNServer/<x>/STUNSrvAddr
                if ( KErrNone == err && iNATFWDomainEntries->At( 
                    domainLoc )->StunSrvAddrL( stunLoc ) != 
                    KNSmlDMNATFWValueNotSet )
                    {
                    err = aRep->Set( 
                        stunKey|KUNSAFProtocolsSTUNAddressMask,
                        iNATFWDomainEntries->At( domainLoc )->
                        StunSrvAddrL( stunLoc ) );
                    if ( KErrNotFound == err )
                        {
                        err = aRep->Create( 
                            stunKey|KUNSAFProtocolsSTUNAddressMask,
                            iNATFWDomainEntries->At( domainLoc )->
                            StunSrvAddrL( stunLoc ) );
                        }
                    }

                // AdditionalSTUNServer/<x>/STUNSrvPort
                if ( KErrNone == err && KErrNotFound != 
                    iNATFWDomainEntries->At( domainLoc )->
                    StunSrvPort( stunLoc ) )
                    {
                    err = aRep->Set( stunKey|KUNSAFProtocolsSTUNPortMask,
                        iNATFWDomainEntries->At( domainLoc )->
                        StunSrvPort( stunLoc ) );
                    if ( KErrNotFound == err )
                        {
                        err = aRep->Create( 
                            stunKey|KUNSAFProtocolsSTUNPortMask,
                            iNATFWDomainEntries->At( domainLoc )->
                            StunSrvPort( stunLoc ) );
                        }
                    }

                // AdditionalSTUNServer/<x>/STUNUsername
                if ( KErrNone == err && iNATFWDomainEntries->At( 
                    domainLoc )->StunUsernameL( stunLoc ) != 
                    KNSmlDMNATFWValueNotSet )
                    {
                    err = aRep->Set( 
                        stunKey|KUNSAFProtocolsSTUNUsernameMask,
                        iNATFWDomainEntries->At( domainLoc )->
                        StunUsernameL( stunLoc ) );
                    if ( KErrNotFound == err )
                        {
                        err = aRep->Create( 
                            stunKey|KUNSAFProtocolsSTUNUsernameMask,
                            iNATFWDomainEntries->At( domainLoc )->
                            StunUsernameL( stunLoc ) );
                        }
                    }

                // AdditionalSTUNServer/<x>/STUNPassword
                if ( KErrNone == err && iNATFWDomainEntries->At( 
                    domainLoc )->StunPasswordL( stunLoc ) != 
                    KNSmlDMNATFWValueNotSet )
                    {
                    err = aRep->Set( 
                        stunKey|KUNSAFProtocolsSTUNPasswordMask,
                        iNATFWDomainEntries->At( domainLoc )->
                        StunPasswordL( stunLoc ) );
                    if ( KErrNotFound == err )
                        {
                        err = aRep->Create( 
                            stunKey|KUNSAFProtocolsSTUNPasswordMask,
                            iNATFWDomainEntries->At( domainLoc )->
                            StunPasswordL( stunLoc ) );
                        }
                    }

                if ( KErrNone == err )
                    {
                    retValue = EOk;
                    }
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );
            }
        }

    // Update TURNSpecific/TURNServer node.
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMUpdateTURN )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EError;
            TInt32 turnKey = iNATFWCommandBuffer[counter].iKeyId;
            TInt domainLoc = iNATFWCommandBuffer[counter].iLocation;
            TUint32 domainKey = iNATFWDomainEntries->At( domainLoc )->
                GetDomainKey();
            domainKey |= KUNSAFProtocolsFieldTypeMask;
            domainKey ^= KUNSAFProtocolsFieldTypeMask;
            RArray<TUint32> turnKeys;
            CleanupClosePushL( turnKeys ); // CS:2
            TInt err = aRep->FindL( 
                domainKey|KUNSAFProtocolsTURNAddressMask,
                KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );
            TInt turnCount( turnKeys.Count() );
            TInt turnLoc( KErrNotFound );

            for ( TInt turnCounter = 0; turnCounter < turnCount; 
                turnCounter++ )
                {
                if ( turnKey == turnKeys[turnCounter] )
                    {
                    turnLoc = turnCounter;
                    }
                }
            turnKeys.Close();
            CleanupStack::PopAndDestroy( &turnKeys ); // CS:1

            // Matching key not found, set status to not found.
            if ( KErrNotFound == turnLoc )
                {
                retValue = ENotFound;
                }
            // Update keys.
            else
                {
                // Remove TURN server address mask from the key.
                turnKey ^= KUNSAFProtocolsTURNAddressMask;

                // TURNSpecific/TURNServer/<x>/TURNSrvAddr
                if ( KErrNone == err && iNATFWDomainEntries->At( 
                    domainLoc )->TurnSrvAddrL( turnLoc ) != 
                    KNSmlDMNATFWValueNotSet )
                    {
                    err = aRep->Set( 
                        turnKey|KUNSAFProtocolsTURNAddressMask,
                        iNATFWDomainEntries->At( domainLoc )->
                        TurnSrvAddrL( turnLoc ) );
                    if ( KErrNotFound == err )
                        {
                        err = aRep->Create( 
                            turnKey|KUNSAFProtocolsTURNAddressMask,
                            iNATFWDomainEntries->At( domainLoc )->
                            TurnSrvAddrL( turnLoc ) );
                        }
                    }

                // TURNSpecific/TURNServer/<x>/TURNSrvPort
                if ( KErrNone == err && KErrNotFound != 
                    iNATFWDomainEntries->At( domainLoc )->
                    TurnSrvPort( turnLoc ) )
                    {
                    err = aRep->Set( turnKey|KUNSAFProtocolsTURNPortMask,
                        iNATFWDomainEntries->At( domainLoc )->
                        TurnSrvPort( turnLoc ) );
                    if ( KErrNotFound == err )
                        {
                        err = aRep->Create(
                            turnKey|KUNSAFProtocolsTURNPortMask,
                            iNATFWDomainEntries->At( domainLoc )->
                            TurnSrvPort( turnLoc ) );
                        }
                    }
                    
                // TURNSpecific/TURNServer/<x>/TURNUsername
                if ( KErrNone == err && iNATFWDomainEntries->At( 
                    domainLoc )->TurnUsernameL( turnLoc ) != 
                    KNSmlDMNATFWValueNotSet )
                    {
                    err = aRep->Set( 
                        turnKey|KUNSAFProtocolsTURNUsernameMask,
                        iNATFWDomainEntries->At( domainLoc )->
                        TurnUsernameL( turnLoc ) );
                    if ( KErrNotFound == err )
                        {
                        err = aRep->Create( 
                            turnKey|KUNSAFProtocolsTURNUsernameMask,
                            iNATFWDomainEntries->At( domainLoc )->
                            TurnUsernameL( turnLoc ) );
                        }
                    }

                // TURNSpecific/TURNServer/<x>/TURNPassword
                if ( KErrNone == err && iNATFWDomainEntries->At( 
                    domainLoc )->TurnPasswordL( turnLoc ) != 
                    KNSmlDMNATFWValueNotSet )
                    {
                    err = aRep->Set( 
                        turnKey|KUNSAFProtocolsTURNPasswordMask,
                        iNATFWDomainEntries->At( domainLoc )->
                        TurnPasswordL( turnLoc ) );
                    if ( KErrNotFound == err )
                        {
                        err = aRep->Create( 
                            turnKey|KUNSAFProtocolsTURNPasswordMask,
                            iNATFWDomainEntries->At( domainLoc )->
                            TurnPasswordL( turnLoc ) );
                        }
                    }

                if ( KErrNone == err )
                    {
                    retValue = EOk;
                    }
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );
            }
        }
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::DeleteSettings
// Deletes NAT-FW settings.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::DeleteSettingsL( CRepository* aRep )
    {
    __ASSERT_ALWAYS( aRep != NULL, User::Leave( KErrArgument ) );
    
    // DomainSpecific/<x>/  delete commands.
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMDeleteDomain )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            RArray<TUint32> keys;
            CleanupClosePushL( keys ); // CS:2
            aRep->FindL( KUNSAFProtocolsDomainMask, 
                KUNSAFProtocolsFieldTypeMask, keys );
            TInt32 currentKey = iNATFWCommandBuffer[counter].iKeyId;
            TInt err( KErrNone );
            currentKey &= KUNSAFProtocolsKeyMask;

            // DomainSpecific/<x>/Domain
            err = aRep->Delete( 
                KUNSAFProtocolsDomainMask|currentKey );

            // DomainSpecific/<x>/STUNSrvAddr
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsSTUNServerMask|currentKey );
                }
            // DomainSpecific/<x>/STUNSrvPort
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsSTUNServerPortMask|currentKey );
                }
            // DomainSpecific/<x>/STUNUsername
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsSTUNUsernameMask|currentKey );
                }
            // DomainSpecific/<x>/StunPassword
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsSTUNPasswordMask|currentKey );
                }
            // DomainSpecific/<x>/NATRefreshUDP
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsDomainIntervalUDPMask|currentKey );
                }
            // DomainSpecific/<x>/NATRefreshTCP
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsDomainIntervalTCPMask|currentKey );
                }
            // DomainSpecific/<x>/EnableCRLFRefresh
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsDomainEnableCRLFRefresh|currentKey );
                }

            TUint32 currentNewKey = currentKey;
            currentNewKey |= KUNSAFProtocolsFieldTypeMask;
            currentNewKey ^= KUNSAFProtocolsFieldTypeMask;

            // When deleting a DomainSpecific node, we must also delete
            // all STUN server settings under it.
            RArray<TUint32> stunKeys;
            CleanupClosePushL( stunKeys ); // CS:3
            aRep->FindL( 
                currentNewKey|KUNSAFProtocolsSTUNAddressMask, 
                KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );
            TInt stunKeyCount( stunKeys.Count() );

            for ( TInt stunCounter = 0; stunCounter < stunKeyCount; 
                stunCounter++ )
                {
                // Remove STUN address mask.
                TUint32 key = KUNSAFProtocolsSTUNAddressMask^
                    stunKeys[stunCounter];

                // STUNSrvAddr
                err = aRep->Delete( key|KUNSAFProtocolsSTUNAddressMask );

                // STUNSrvPort
                if ( KErrNone == err || KErrNotFound == err )
                    {
                    err = aRep->Delete( key|KUNSAFProtocolsSTUNPortMask );
                    }

                // STUNUsername
                if ( KErrNone == err || KErrNotFound == err )
                    {
                    err = aRep->Delete( key|KUNSAFProtocolsSTUNUsernameMask );
                    }

                // STUNPassword
                if ( KErrNone == err || KErrNotFound == err )
                    {
                    err = aRep->Delete( key|KUNSAFProtocolsSTUNPasswordMask );
                    }
                }
            stunKeys.Close();
            CleanupStack::PopAndDestroy( &stunKeys ); // CS:2

            // DomainSpecific/<x>/STUNSharedSecret
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete( currentNewKey|
                   KUNSAFProtocolsDomainSharedSecretNotSupported );
                }

            // DomainSpecific/<x>/PortPoolStartPort
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsPortPoolStartPortMask|currentNewKey );
                }

            // DomainSpecific/<x>/PortPoolEndPort
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsPortPoolEndPortMask|currentNewKey );
                }

            // DomainSpecific/<x>/UsedNATProtocol
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsUsedNATProtocolMask|currentNewKey );
                }

            // DomainSpecific/<x>/ICESpecific/NATUtilities
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsNATUtilities|currentNewKey );
                }

            // DomainSpecific/<x>/ICESpecific/HostPreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsHostPref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/ServerReflexivePreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsServerRefPref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/RelayPreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsRelayPref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/PeerReflexivePreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsPeerRefPref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/IPv4Preference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsIPV4Pref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/IPv6Preference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsIPV6Pref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/VPNPreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsVPNPref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/UDPPreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsUDPPref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/TCPPreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsTCPPref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/TCPActivePreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsTCPActivePref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/TCPPassivePreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsTCPPassivePref|currentNewKey );
                }
            // DomainSpecific/<x>/ICESpecific/TCPSimultaneousPreference
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = aRep->Delete(
                   KUNSAFProtocolsTCPSimultPref|currentNewKey );
                }
            
            if ( KErrNone == err || KErrNotFound == err )    
                {
                retValue = EOk;
                }
            else 
                {
                retValue = EError;
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );
            keys.Close();
            CleanupStack::PopAndDestroy( &keys ); // CS:1
            }
        }

    // IAPSpecific/x/  delete commands
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMDeleteIAP )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            RArray<TUint32> keys;
            CleanupClosePushL( keys ); // CS:2
            aRep->FindL( KUNSAFProtocolsIAPIdMask, 
                KUNSAFProtocolsFieldTypeMask, keys );
            TInt32 currentKey = iNATFWCommandBuffer[counter].iKeyId;
            currentKey &= KUNSAFProtocolsKeyMask;

            // Delete keys.
            TInt err = aRep->Delete( 
                KUNSAFProtocolsIAPIdMask|currentKey );
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsIntervalUDPMask|currentKey );
                }
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsIntervalTCPMask|currentKey );
                }
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsSTUNRetransmitTimerMask|currentKey );
                }
            if ( KErrNone == err )    
                {
                retValue = EOk;
                }
            else 
                {
                retValue = EError;
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );     
            keys.Close();
            CleanupStack::PopAndDestroy( &keys ); // CS:1
            }
        }

    // DomainSpecific/<x>/AdditionalSTUNServer/<x>/  delete commands
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMDeleteSTUN )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            
            TUint32 stunKey = iNATFWCommandBuffer[counter].iKeyId;
            // Remove STUN server address mask from key.
            stunKey ^= KUNSAFProtocolsSTUNAddressMask;

            TInt err = aRep->Delete(
                    KUNSAFProtocolsSTUNAddressMask|stunKey );
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsSTUNPortMask|stunKey );
                }
            if ( KErrNone == err )    
                {
                retValue = EOk;
                }
            else 
                {
                retValue = EError;
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );   
            }
        }

    // DomainSpecific/<x>/TURNSpecific/TURNServer/<x>/ delete commands
    for ( TInt counter = 0; counter < iNATFWCommandBuffer.Count(); 
        counter++ )
        {
        if ( iNATFWCommandBuffer[counter].iCommandType == 
            TDMNATFWStatusInfo::EDMDeleteTURN )
            {
            CSmlDmAdapter::TError retValue = CSmlDmAdapter::EOk;
            
            TUint32 turnKey = iNATFWCommandBuffer[counter].iKeyId;
            // Remove TURN server address mask from key.
            turnKey ^= KUNSAFProtocolsTURNAddressMask;

            TInt err = aRep->Delete(
                    KUNSAFProtocolsTURNAddressMask|turnKey );
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsTURNPortMask|turnKey );
                }
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsTURNUsernameMask|turnKey );
                }
            if ( KErrNone == err )
                {
                err = aRep->Delete( 
                    KUNSAFProtocolsTURNPasswordMask|turnKey );
                }
                
            if ( KErrNone == err )    
                {
                retValue = EOk;
                }
            else 
                {
                retValue = EError;
                }
            iDmCallback->SetStatusL( 
                iNATFWCommandBuffer[counter].iStatusRef, retValue );   
            }
        }
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::GetDomainSettingsL
// Gets one domain settings entry.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::GetDomainSettingsL( TUint32 aDomainKeyId, 
    CNATFWDomainEntry& aEntry )
    {
    // Central Repository for NAT-FW Traversal settings.
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1

    // Select domain.
    TUint32 key = 0;
    key = ( KUNSAFProtocolsDomainMask^( aDomainKeyId ) );
    key |= KUNSAFProtocolsDomainTableMask;

    // Set key ID to entry.
    aEntry.SetDomainKey( aDomainKeyId );

    // Set Domain to entry.
    TInt actualLength = 0;
    TBuf8<1> tmp;
    rep->Get( KUNSAFProtocolsDomainMask|key, tmp, actualLength );

    if ( actualLength > 0 )
        {
        HBufC8* domainName = HBufC8::NewLC( actualLength ); // CS:2
        TPtr8 ptr( domainName->Des() );
        TInt err = rep->Get( KUNSAFProtocolsDomainMask|key, ptr ); 
        if ( KErrNone == err )
            {
            aEntry.SetDomainL( domainName->Des() );
            }
        else
            {
            aEntry.SetDomainL( KNSmlDMNATFWValueNotSet );
            }
        CleanupStack::PopAndDestroy( domainName ); // CS:1
        actualLength = 0;
        }
    else
        {
        aEntry.SetDomainL( KNSmlDMNATFWValueNotSet );
        }

    // Set STUN server address to entry.
    rep->Get( KUNSAFProtocolsSTUNServerMask|key, tmp, actualLength );

    if ( actualLength > 0 )
        {
        HBufC8* stunServerAddr = HBufC8::NewLC( actualLength ); // CS:2
        TPtr8 ptr ( stunServerAddr->Des() );
        TInt err = rep->Get( KUNSAFProtocolsSTUNServerMask|key, ptr );
        if ( err == KErrNone ) 
            {
            aEntry.SetSTUNServerAddrL( stunServerAddr->Des() );
            }
        else
            {
            aEntry.SetSTUNServerAddrL( KNSmlDMNATFWValueNotSet );
            }
        CleanupStack::PopAndDestroy( stunServerAddr ); // CS:1
        }
    else
        {
        aEntry.SetSTUNServerAddrL( KNSmlDMNATFWValueNotSet );
        }

    // STUN server port.
    TInt tmpValue( KErrNotFound );
    rep->Get( KUNSAFProtocolsSTUNServerPortMask|key, tmpValue );
    aEntry.SetSTUNServerPort( tmpValue );

    // STUN server username.
    rep->Get( KUNSAFProtocolsSTUNUsernameMask|key, tmp, actualLength );

    if ( actualLength > 0 )
        {
        HBufC8* stunUsername = HBufC8::NewLC( actualLength ); // CS:2
        TPtr8 ptr ( stunUsername->Des() );
        TInt err = rep->Get( KUNSAFProtocolsSTUNUsernameMask|key, ptr );
        if ( err == KErrNone ) 
            {
            aEntry.SetSTUNUsernameL( stunUsername->Des() );
            }
        else
            {
            aEntry.SetSTUNUsernameL( KNSmlDMNATFWValueNotSet );
            }
        CleanupStack::PopAndDestroy( stunUsername ); // CS:1
        }
    else
        {
        aEntry.SetSTUNUsernameL( KNSmlDMNATFWValueNotSet );
        }

    // STUN server password.
    rep->Get( KUNSAFProtocolsSTUNPasswordMask|key, tmp, actualLength );

    if ( actualLength > 0 )
        {
        HBufC8* stunPassword = HBufC8::NewLC( actualLength ); // CS:2
        TPtr8 ptr ( stunPassword->Des() );
        TInt err = rep->Get( KUNSAFProtocolsSTUNPasswordMask|key, ptr );
        if ( err == KErrNone ) 
            {
            aEntry.SetSTUNPasswordL( stunPassword->Des() );
            }
        else
            {
            aEntry.SetSTUNPasswordL( KNSmlDMNATFWValueNotSet );
            }
        CleanupStack::PopAndDestroy( stunPassword ); // CS:1
        }
    else
        {
        aEntry.SetSTUNPasswordL( KNSmlDMNATFWValueNotSet );
        }

    // NAT refresh interval for UDP
    tmpValue = KErrNotFound;
    rep->Get( KUNSAFProtocolsDomainIntervalUDPMask|key, tmpValue );
    aEntry.SetNATRefreshUDP( tmpValue );

    // NAT refresh interval for TCP
    tmpValue = KErrNotFound;
    rep->Get( KUNSAFProtocolsDomainIntervalTCPMask|key, tmpValue );
    aEntry.SetNATRefreshTCP( tmpValue );

    // CRLF-based NAT binding refresh 
    tmpValue = KErrNotFound;
    rep->Get( KUNSAFProtocolsDomainEnableCRLFRefresh|key, tmpValue );
    aEntry.SetEnableCRLFRefresh( tmpValue );

    RArray<TUint32> stunKeys;
    CleanupClosePushL( stunKeys ); // CS:2
    rep->FindL( key|KUNSAFProtocolsSTUNAddressMask, 
        KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );
    TInt stunKeyCount = stunKeys.Count();
    for ( TInt counter = 0; counter < stunKeyCount; counter++ )
        {
        // Remove STUN server address mask from key.
        TUint32 tmpKey = KUNSAFProtocolsSTUNAddressMask^stunKeys[counter];

        // STUNSrvAddr
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsSTUNAddressMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* stunSrvAddr = HBufC8::NewLC( actualLength ); // CS:3
            TPtr8 ptr( stunSrvAddr->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsSTUNAddressMask, 
                ptr );
            if ( KErrNone == err )
                {
                aEntry.AddStunSrvAddrL( stunSrvAddr->Des() );
                }
            else
                {
                aEntry.AddStunSrvAddrL( KNSmlDMNATFWValueNotSet );
                }
            CleanupStack::PopAndDestroy( stunSrvAddr ); // CS:2
            }
        else
            {
            aEntry.AddStunSrvAddrL( KNSmlDMNATFWValueNotSet );
            }

        // STUNSrvPort
        tmpValue = KErrNotFound;
        rep->Get( tmpKey|KUNSAFProtocolsSTUNPortMask, tmpValue );
        aEntry.AddStunSrvPortL( tmpValue );

        // STUNUsername
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsSTUNUsernameMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* stunUsername = HBufC8::NewLC( actualLength ); // CS:3
            TPtr8 ptr( stunUsername->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsSTUNUsernameMask, 
                ptr );
            if ( KErrNone == err )
                {
                aEntry.AddStunUsernameL( stunUsername->Des() );
                }
            else
                {
                aEntry.AddStunUsernameL( KNSmlDMNATFWValueNotSet );
                }
            CleanupStack::PopAndDestroy( stunUsername ); // CS:2
            }
        else
            {
            aEntry.AddStunUsernameL( KNSmlDMNATFWValueNotSet );
            }

        // STUNPassword
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsSTUNPasswordMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* stunPassword = HBufC8::NewLC( actualLength ); // CS:3
            TPtr8 ptr( stunPassword->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsSTUNPasswordMask, 
                ptr );
            if ( KErrNone == err )
                {
                aEntry.AddStunPasswordL( stunPassword->Des() );
                }
            else
                {
                aEntry.AddStunPasswordL( KNSmlDMNATFWValueNotSet );
                }
            CleanupStack::PopAndDestroy( stunPassword ); // CS:2
            }
        else
            {
            aEntry.AddStunSrvAddrL( KNSmlDMNATFWValueNotSet );
            }
        }
    stunKeys.Close();
    CleanupStack::PopAndDestroy( &stunKeys ); // CS:1

    // STUNSharedSecret
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsDomainSharedSecretNotSupported, 
        tmpValue );
    aEntry.SetDisableStunSharedSecret( tmpValue );

    // PortPoolStartPort
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsPortPoolStartPortMask, tmpValue );
    aEntry.SetStartPortRange( tmpValue );

    // PortPoolEndPort
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsPortPoolEndPortMask, tmpValue );
    aEntry.SetEndPortRange( tmpValue );

    // UsedNATProtocol
    rep->Get( KUNSAFProtocolsUsedNATProtocolMask|key, tmp, actualLength );
    if ( actualLength > 0 )
        {
        HBufC8* natProtocol = HBufC8::NewLC( actualLength ); // CS:2
        TPtr8 ptr ( natProtocol->Des() );
        TInt err = rep->Get( KUNSAFProtocolsUsedNATProtocolMask|key, ptr );
        if ( err == KErrNone ) 
            {
            aEntry.SetNatProtocolL( natProtocol->Des() );
            }
        else
            {
            aEntry.SetNatProtocolL( KNSmlDMNATFWValueNotSet );
            }
        CleanupStack::PopAndDestroy( natProtocol ); // CS:1
        }
    else
        {
        aEntry.SetNatProtocolL( KNSmlDMNATFWValueNotSet );
        }

    // ICESpecific/NATUtilities
    rep->Get( KUNSAFProtocolsNATUtilities|key, tmp, actualLength );
    if ( actualLength > 0 )
        {
        HBufC8* natUtils = HBufC8::NewLC( actualLength ); // CS:2
        TPtr8 ptr ( natUtils->Des() );
        TInt err = rep->Get( KUNSAFProtocolsNATUtilities|key, ptr );
        if ( err == KErrNone ) 
            {
            aEntry.SetNatUtilitiesL( natUtils->Des() );
            }
        else
            {
            aEntry.SetNatUtilitiesL( KNSmlDMNATFWValueNotSet );
            }
        CleanupStack::PopAndDestroy( natUtils ); // CS:1
        }
    else
        {
        aEntry.SetNatProtocolL( KNSmlDMNATFWValueNotSet );
        }

    // ICESpecific/HostPreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsHostPref, tmpValue );
    aEntry.SetHostPref( tmpValue );

    // ICESpecific/ServerReflexivePreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsServerRefPref, tmpValue );
    aEntry.SetServerReflexPref( tmpValue );

    // ICESpecific/RelayPreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsRelayPref, tmpValue );
    aEntry.SetRelayPref( tmpValue );

    // ICESpecific/PeerReflexivePreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsPeerRefPref, tmpValue );
    aEntry.SetPeerReflexPref( tmpValue );

    // ICESpecific/IPv4Preference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsIPV4Pref, tmpValue );
    aEntry.SetIPv4Pref( tmpValue );

    // ICESpecific/IPv6Preference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsIPV6Pref, tmpValue );
    aEntry.SetIPv6Pref( tmpValue );

    // ICESpecific/VPNPreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsVPNPref, tmpValue );
    aEntry.SetVpnPref( tmpValue );

    // ICESpecific/UDPPreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsUDPPref, tmpValue );
    aEntry.SetUdpPref( tmpValue );

    // ICESpecific/TCPPreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsTCPPref, tmpValue );
    aEntry.SetTcpPref( tmpValue );

    // ICESpecific/TCPActivePreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsTCPActivePref, tmpValue );
    aEntry.SetTcpActivePref( tmpValue );

    // ICESpecific/TCPPassivePreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsTCPPassivePref, tmpValue );
    aEntry.SetTcpPassivePref( tmpValue );

    // ICESpecific/TCPSimultaneousPreference
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsTCPSimultPref, tmpValue );
    aEntry.SetTcpSimultPref( tmpValue );

    // TURNSpecific/TURNServer/<x>/
    RArray<TUint32> turnKeys;
    CleanupClosePushL( turnKeys ); // CS:2
    rep->FindL( key|KUNSAFProtocolsTURNAddressMask, 
        KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );
    TInt turnKeyCount = turnKeys.Count();
    for ( TInt counter = 0; counter < turnKeyCount; counter++ )
        {
        // Remove TURN server address mask from key.
        TUint32 tmpKey = KUNSAFProtocolsTURNAddressMask^turnKeys[counter];

        // TURNSrvAddr
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsTURNAddressMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* turnSrvAddr = HBufC8::NewLC( actualLength ); // CS:3
            TPtr8 ptr( turnSrvAddr->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsTURNAddressMask, 
                ptr );
            if ( KErrNone == err )
                {
                aEntry.AddTurnSrvAddrL( turnSrvAddr->Des() );
                }
            else
                {
                aEntry.AddTurnSrvAddrL( KNSmlDMNATFWValueNotSet );
                }
            CleanupStack::PopAndDestroy( turnSrvAddr ); // CS:2
            }
        else
            {
            aEntry.AddTurnSrvAddrL( KNSmlDMNATFWValueNotSet );
            }

        // TURNSrvPort
        tmpValue = KErrNotFound;
        rep->Get( tmpKey|KUNSAFProtocolsTURNPortMask, tmpValue );
        aEntry.AddTurnSrvPortL( tmpValue );
        
        // TURNUsername
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsTURNUsernameMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* turnUsername = HBufC8::NewLC( actualLength ); // CS:3
            TPtr8 ptr( turnUsername->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsTURNUsernameMask, 
                ptr );
            if ( KErrNone == err )
                {
                aEntry.AddTurnUsernameL( turnUsername->Des() );
                }
            else
                {
                aEntry.AddTurnUsernameL( KNSmlDMNATFWValueNotSet );
                }
            CleanupStack::PopAndDestroy( turnUsername ); // CS:2
            }
        else
            {
            aEntry.AddTurnUsernameL( KNSmlDMNATFWValueNotSet );
            }

        // TURNPassword
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsTURNPasswordMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* turnPassword = HBufC8::NewLC( actualLength ); // CS:3
            TPtr8 ptr( turnPassword->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsTURNPasswordMask, 
                ptr );
            if ( KErrNone == err )
                {
                aEntry.AddTurnPasswordL( turnPassword->Des() );
                }
            else
                {
                aEntry.AddTurnPasswordL( KNSmlDMNATFWValueNotSet );
                }
            CleanupStack::PopAndDestroy( turnPassword ); // CS:2
            }
        else
            {
            aEntry.AddTurnPasswordL( KNSmlDMNATFWValueNotSet );
            }
        }
    turnKeys.Close();
    CleanupStack::PopAndDestroy( &turnKeys ); // CS:1
    CleanupStack::PopAndDestroy( rep ); // CS:0
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::GetIAPSettingsL
// Gets one IAP settings entry.
// ---------------------------------------------------------------------------
//
void CNSmlDmNATFWTraversalAdapter::GetIAPSettingsL( TUint32 aIAPKey, 
    CNATFWIAPEntry& aEntry )
    {
    // Central Repository for NAT-FW Traversal settings.
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1

    TUint32 key( KErrNone );
    // Select IAP.
    key = ( KUNSAFProtocolsIAPIdMask^( aIAPKey ) );
    key |= KUNSAFProtocolsIAPTableMask;

    // Set key ID.
    aEntry.SetIAPKey( aIAPKey );
    TInt tmp( KErrNotFound );
    rep->Get( KUNSAFProtocolsIAPIdMask|key, tmp );
    aEntry.SetIAPId( tmp );
    tmp = KErrNotFound;
    rep->Get( KUNSAFProtocolsIntervalUDPMask|key, tmp );
    aEntry.SetIntervalUDP( tmp );
    tmp = KErrNotFound;
    rep->Get( KUNSAFProtocolsIntervalTCPMask|key, tmp );
    aEntry.SetIntervalTCP( tmp );
    tmp = KErrNotFound;
    rep->Get( KUNSAFProtocolsSTUNRetransmitTimerMask|key, tmp );
    aEntry.SetSTUNRetransmitTimer( tmp );
    CleanupStack::PopAndDestroy( rep ); // CS:0
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FindDomainLocation
//
// ---------------------------------------------------------------------------
//
TInt CNSmlDmNATFWTraversalAdapter::FindDomainLocation( const TDesC8& aLUID )
    {
    for ( TInt counter = 0; counter < iNATFWDomainEntries->Count(); counter++ )
        {
        if ( iNATFWDomainEntries->At( counter )->GetDomainKey() == 
            DesToInt( LastURISeg( aLUID ) ) )
            {
            return counter;
            }
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FindDomainLocation
//
// ---------------------------------------------------------------------------
//
TInt CNSmlDmNATFWTraversalAdapter::FindDomainLocation( 
    const TUint32 aDomainKey )
    {
    for ( TInt counter = 0; counter < iNATFWDomainEntries->Count(); counter++ )
        {
        if ( iNATFWDomainEntries->At( counter )->GetDomainKey() 
            == aDomainKey )
            {
            return counter;
            }
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FindIAPLocation
//
// ---------------------------------------------------------------------------
//
TInt CNSmlDmNATFWTraversalAdapter::FindIAPLocation( const TDesC8& aLUID )
    {
    for ( TInt counter = 0; counter < iNATFWIAPEntries->Count(); counter++ )
        {
        if ( iNATFWIAPEntries->At( counter )->GetIAPKey() == DesToTUint( 
            LastURISeg( aLUID ) ) )
            {
            return counter;
            }
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FindIAPLocation
//
// ---------------------------------------------------------------------------
//
TInt CNSmlDmNATFWTraversalAdapter::FindIAPLocation( const TUint32 aIAPKey )
    {
    for ( TInt counter = 0; counter < iNATFWIAPEntries->Count(); counter++ )
        {
        if ( iNATFWIAPEntries->At( counter )->GetIAPKey() == aIAPKey )
            {
            return counter;
            }
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::SetConRefL
// Set conref value.
// ---------------------------------------------------------------------------
//
TInt CNSmlDmNATFWTraversalAdapter::SetConRefL( const TDesC8& aObject, 
    TInt aProfileLoc )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::SetConRefL() : begin");
    CNSmlDMIAPMatcher* iapmatch = CNSmlDMIAPMatcher::NewL( iDmCallback );
    CleanupStack::PushL( iapmatch ); // CS:1
    TInt lIAPid = iapmatch->IAPIdFromURIL( aObject );
    if ( KErrNotFound != lIAPid )
        {
        iNATFWIAPEntries->At( aProfileLoc )->SetIAPId( ( TUint32 )lIAPid );
        }
    CleanupStack::PopAndDestroy( iapmatch ); // CS:0
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::SetConRefL() : end");
    return lIAPid;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::GetConRefL
// Get URI for given accesspoint ID.
// Returns: TBool: True if connection reference found
// ---------------------------------------------------------------------------
//
TBool CNSmlDmNATFWTraversalAdapter::GetConRefL( CBufBase& aObject, 
    TInt aProfileLoc )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::GetConRefL() : begin");
    TBool found = EFalse;

    CNSmlDMIAPMatcher* iapmatch = CNSmlDMIAPMatcher::NewL( iDmCallback );
    CleanupStack::PushL( iapmatch ); // CS:1

    // Get current access point ID.
    TUint32 profIAPid;
    profIAPid = iNATFWIAPEntries->At( aProfileLoc )->GetIAPId();

    HBufC8* uri8 = iapmatch->URIFromIAPIdL( profIAPid );
    CleanupStack::PushL( uri8 ); // CS:2
    if ( uri8 )
        {
        aObject.InsertL( aObject.Size(), uri8->Des() );
        found = ETrue;
        }
    CleanupStack::PopAndDestroy( uri8 ); // CS:1
    CleanupStack::PopAndDestroy( iapmatch ); // CS:0
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::GetConRefL() : end");
    return found;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::DesToTUint
// Converts a 8bit descriptor to TUint.
// Returns: TUint: 
// ---------------------------------------------------------------------------
TUint CNSmlDmNATFWTraversalAdapter::DesToTUint( const TDesC8& aDes )
    {
    TLex8 lex( aDes );
    TUint value = 0;
    lex.Val( value );
    return value;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FetchObjectL
// Fetches the values of leaf objects.
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError CNSmlDmNATFWTraversalAdapter::FetchObjectL( 
    const TDesC8& aURI, 
    const TDesC8& aLUID,
    CBufBase& aObject )
    {
    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::FetchLeafObjectL(): begin");
    CSmlDmAdapter::TError status = CSmlDmAdapter::EOk;
    TBuf8< KNSmlNATFWMaxResultLength > segmentResult;

    // Load profile information if not loaded yet.
    if ( !iNATFWDomainEntries || ! iNATFWIAPEntries )
        {
        LoadProfilesL();
        }   

    // ==============================
    // DomainSpecific node
    // ==============================
    //
    if ( KErrNotFound != aURI.Find( KNSmlDMDomainSpecificNodeName() )
        && ( KFourSegs == NumOfURISegs( aURI ) 
        || KFiveSegs == NumOfURISegs( aURI )
        ) )
        {
        status = FetchDomainObjectL( aURI, aLUID, segmentResult );
        }

    // ==============================
    // IAPSpecific/x/
    // ==============================
    //
    else if ( KErrNotFound != aURI.Find( KNSmlDMIAPSpecificNodeName() )
        && KFourSegs == NumOfURISegs( aURI ) )
        {
        status = FetchIAPObjectL( aURI, aLUID, segmentResult, aObject );
        }

    // ==============================
    // AdditionalSTUNServer/<x>/
    // ==============================
    //
    else if ( KErrNotFound != aURI.Find( KNSmlDMAdditionalSTUNNodeName() )
        && KSixSegs == NumOfURISegs( aURI ) 
        && KFourSegs == NumOfURISegs( aLUID ) )
        {
        status = FetchAdditionalSTUNServerObjectL( aURI, aLUID,
            segmentResult );
        }

    // ==============================
    // TURNSpecific/TURNServer/<x>/
    // ==============================
    //
    else if ( KErrNotFound != aURI.Find( KNSmlDMTURNServerNodeName() )
        && KErrNotFound != aURI.Find( KNSmlDMTURNSpecificNodeName() )
        && KSevenSegs == NumOfURISegs( aURI ) 
        && KFiveSegs == NumOfURISegs( aLUID ) )
        {
        status = FetchTURNServerObjectL( aURI, aLUID,
            segmentResult );
        }

    else
        {
        // No node under NATFW found.
        status = CSmlDmAdapter::ENotFound;
        }  

    if ( CSmlDmAdapter::EOk == status )    
        {
        aObject.InsertL( aObject.Size(), segmentResult );
        }

    DBG_PRINT("CNSmlDmNATFWTraversalAdapter::FetchLeafObjectL(): end");
    return status;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FetchDomainObjectL
// Fetches the values of Domain specific leaf objects.
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError CNSmlDmNATFWTraversalAdapter::FetchDomainObjectL( 
    const TDesC8& aURI, 
    const TDesC8& aLUID,
    TDes8& aSegmentResult )
    {
    CSmlDmAdapter::TError status = EOk;
    TBuf8<KSmlMaxURISegLen> lastURISeg = LastURISeg( aURI );
    
    TInt domainLoc = FindDomainLocation( aLUID );
    if ( KErrNotFound == domainLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    TBool iceSpecific( EFalse );
    TInt uriSegs = NumOfURISegs( aURI );
    if ( KErrNotFound != aURI.Find( KNSmlDMICESpecificNodeName() )
        && KFiveSegs == uriSegs )
        {
        iceSpecific = ETrue;
        }

    // Domain
    if ( KNSmlDMDomain() == lastURISeg )    
        {
        aSegmentResult.Copy( iNATFWDomainEntries->
            At( domainLoc )->GetDomain() );
        }  
    // STUNSrvAddr
    else if ( KNSmlDMSTUNSrvAddr() == lastURISeg )    
        {
        aSegmentResult.Copy( iNATFWDomainEntries->
            At( domainLoc )->GetSTUNAddr() );
        }       
    // STUNSrvPort
    else if ( KNSmlDMSTUNSrvPort() == lastURISeg )
        {
        aSegmentResult.Num( iNATFWDomainEntries->
            At( domainLoc )->GetSTUNPort() );
        }

    // STUNUsername
    else if ( KNSmlDMSTUNUsername() == lastURISeg )    
        {
        aSegmentResult.Copy( iNATFWDomainEntries->
            At( domainLoc )->GetSTUNUsername() );
        }       
    // STUNPassword
    else if ( KNSmlDMSTUNPassword() == lastURISeg )    
        {
        aSegmentResult.Copy( iNATFWDomainEntries->
            At( domainLoc )->GetSTUNPassword() );
        }       

    // NATRefreshTCP
    else if ( KNSmlDMDomainNATRefreshTCP() == lastURISeg )
        {
        aSegmentResult.Num( iNATFWDomainEntries->
            At( domainLoc )->GetNATRefreshTCP() );
        }
    // NATRefreshUDP
    else if ( KNSmlDMDomainNATRefreshUDP() == lastURISeg )
        {
        aSegmentResult.Num( iNATFWDomainEntries->
            At( domainLoc )->GetNATRefreshUDP() );
        }
    // CRLFRefresh
    else if ( KNSmlDMDomainEnableCRLFRefresh() == lastURISeg )
        {
        if ( iNATFWDomainEntries->At( domainLoc )->
            GetEnableCRLFRefresh() == 1 )
            {
            aSegmentResult.Copy( KNSmlDMNATFWValueTrue );
            }
        else if ( iNATFWDomainEntries->At( domainLoc )-> 
            GetEnableCRLFRefresh() == 0 )
            {
            aSegmentResult.Copy( KNSmlDMNATFWValueFalse );   
            }
        else 
            {
            status = CSmlDmAdapter::EInvalidObject;     
            }
        }

    // STUNSharedSecret
    else if ( KNSmlDMSTUNSharedSecret() == lastURISeg )
        {
        // CentRep key is for "STUN shared secret not supported" so
        // we must reverse its value.
        if ( TInt( ETrue ) == iNATFWDomainEntries->At( domainLoc )->
            DisableStunSharedSecret() )
            {
            aSegmentResult.Copy( KNSmlDMNATFWValueFalse );
            }
        else if ( TInt( EFalse ) == iNATFWDomainEntries->At( domainLoc )->
            DisableStunSharedSecret() )
            {
            aSegmentResult.Copy( KNSmlDMNATFWValueTrue );
            }
        else
            {
            status = CSmlDmAdapter::EInvalidObject;
            }
        }
    // PortPoolStartPort
    else if ( KNSmlDMPortPoolStart() == lastURISeg )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            StartPortRange() );
        }
    // PortPoolEndPort
    else if ( KNSmlDMPortPoolEnd() == lastURISeg )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            EndPortRange() );
        }
    // UsedNATProtocol
    else if ( KNSmlDMUsedNatProtocol() == lastURISeg )
        {
        aSegmentResult.Copy( iNATFWDomainEntries->At( domainLoc )->
            UsedNatProtocol() );
        }

    // ICESpecific/NATUtilities
    else if ( KNSmlDMNatUtilities() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Copy( iNATFWDomainEntries->At( domainLoc )->
            NatUtilities() );
        }
    // ICESpecific/HostPreference
    else if ( KNSmlDMHostPref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            HostPref() );
        }
    // ICESpecific/ServerReflexivePreference
    else if ( KNSmlDMServerReflexPref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            ServerReflexPref() );
        }
    // ICESpecific/RelayPreference
    else if ( KNSmlDMRelayPref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            RelayPref() );
        }
    // ICESpecific/PeerReflexivePreference
    else if ( KNSmlDMPeerReflexPref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            PeerReflexPref() );
        }
    // ICESpecific/IPv4Preference
    else if ( KNSmlDMIPv4Pref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            IPv4Pref() );
        }
    // ICESpecific/IPv6Preference
    else if ( KNSmlDMIPv6Pref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            IPv6Pref() );
        }
    // ICESpecific/VPNPreference
    else if ( KNSmlDMVpnPref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->VpnPref() );
        }
    // ICESpecific/UDPPreference
    else if ( KNSmlDMUdpPref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->UdpPref() );
        }
    // ICESpecific/TCPPreference
    else if ( KNSmlDMTcpPref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->TcpPref() );
        }
    // ICESpecific/TCPActivePreference
    else if ( KNSmlDMTcpActivePref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            TcpActivePref() );
        }
    // ICESpecific/TCPPassivePreference
    else if ( KNSmlDMTcpPassivePref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            TcpPassivePref() );
        }
    // ICESpecific/TCPSimultaneousPreference
    else if ( KNSmlDMTcpSimultPref() == lastURISeg 
        && iceSpecific )
        {
        aSegmentResult.Num( iNATFWDomainEntries->At( domainLoc )->
            TcpSimultPref() );
        }
    else
        {
        status = CSmlDmAdapter::ENotFound;
        }
        
    return status;
    }


// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FetchIAPObjectL
// Fetches the values of IAP specific leaf objects.
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError CNSmlDmNATFWTraversalAdapter::FetchIAPObjectL( 
    const TDesC8& aURI, 
    const TDesC8& aLUID,
    TDes8& aSegmentResult,
    CBufBase& aResult )
    {
    CSmlDmAdapter::TError status = EOk;
    TBuf8< KSmlMaxURISegLen> lastURISeg = LastURISeg( aURI );
    
    TInt iapLoc = FindIAPLocation ( aLUID );
    if ( KErrNotFound == iapLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    // PrefConRef
    if ( KNSmlDMNATPrefConRef() == lastURISeg ) 
        {
        TBool conref = GetConRefL( aResult, iapLoc );
        if ( !conref )
            {
            aResult.Reset();
            status = CSmlDmAdapter::ENotFound;
            DBG_PRINT("CNSmlDmNATFWTraversalAdapter::FetchLeafObjectL(): PrefConRef not found end");
            return status; 
            }
        }
    else if ( KNSmlDMNATRefreshTCP() == lastURISeg )
        {
        aSegmentResult.Num( iNATFWIAPEntries->
            At( iapLoc )->GetIntervalTCP() );
        }
    else if ( KNSmlDMNATRefreshUDP() == lastURISeg )
        {
        aSegmentResult.Num( iNATFWIAPEntries->
            At( iapLoc )->GetIntervalUDP() );
        }
    else if ( KNSmlDMSTUNRetransmit() == lastURISeg )
        {
        aSegmentResult.Num( iNATFWIAPEntries->
            At( iapLoc )->GetSTUNRetransmitTimer() );
        }
    else
        {
        // None of IAP specific settings found.
        status = CSmlDmAdapter::ENotFound;
        }
        
    return status;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FetchAdditionalSTUNServerObjectL
// Fetches the values of AdditionalSTUNServer/<x>/ leaf objects.
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError 
    CNSmlDmNATFWTraversalAdapter::FetchAdditionalSTUNServerObjectL(
    const TDesC8& aURI, 
    const TDesC8& aLUID,
    TDes8& aSegmentResult )
    {
    CSmlDmAdapter::TError status = EOk;
    TBuf8<KSmlMaxURISegLen> lastURISeg = LastURISeg( aURI );
    
    TUint32 stunKey = DesToInt( LastURISeg( aLUID ) );
    if ( KErrNone == stunKey )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    TPtrC8 tmpLuid = RemoveLastURISeg( aLUID );
    TPtrC8 domainLuid = RemoveLastURISeg( tmpLuid );
    TInt domainLoc = FindDomainLocation( domainLuid );

    if ( KErrNotFound == domainLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
    RArray<TUint32> stunKeys;
    CleanupClosePushL( stunKeys ); // CS:2
    TUint32 domainKey = iNATFWDomainEntries->At( domainLoc )->
        GetDomainKey();
    domainKey |= KUNSAFProtocolsFieldTypeMask;
    domainKey ^= KUNSAFProtocolsFieldTypeMask;
   rep->FindL( domainKey|KUNSAFProtocolsSTUNAddressMask,
        KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );
    TInt stunCount( stunKeys.Count() );
    TInt stunLoc( KErrNotFound );

    for ( TInt counter = 0; counter < stunCount; counter++ )
        {
        if ( stunKey == stunKeys[counter] )
            {
            stunLoc = counter;
            }
        }
    stunKeys.Close();
    CleanupStack::PopAndDestroy( &stunKeys ); // CS:1
    CleanupStack::PopAndDestroy( rep ); // CS:0
    // STUN key was not in CentRep.
    if ( KErrNotFound == stunLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }
    // STUN key was in CentRep index 0 which is reserved for STUN
    // settings directly under DomainSpecific node.
    else if ( KErrNone == stunLoc )
        {
        status = CSmlDmAdapter::EError;
        return status;
        }
    // Fetch values.
    else
        {
        // STUNSrvAddr
        if ( KNSmlDMSTUNSrvAddr() == lastURISeg )    
            {
            aSegmentResult.Copy( iNATFWDomainEntries->
                At( domainLoc )->StunSrvAddrL( stunLoc ) );
            }       
        // STUNSrvPort
        else if ( KNSmlDMSTUNSrvPort() == lastURISeg )
            {
            aSegmentResult.Num( iNATFWDomainEntries->
                At( domainLoc )->StunSrvPort( stunLoc ) );
            }
        // STUNUsername
        else if ( KNSmlDMSTUNUsername() == lastURISeg )    
            {
            aSegmentResult.Copy( iNATFWDomainEntries->
                At( domainLoc )->StunUsernameL( stunLoc ) );
            }       
        // STUNPassword
        else if ( KNSmlDMSTUNPassword() == lastURISeg )    
            {
            aSegmentResult.Copy( iNATFWDomainEntries->
                At( domainLoc )->StunPasswordL( stunLoc ) );
            }       
        else
            {
            // None of STUN specific settings found.
            status = CSmlDmAdapter::ENotFound;
            }
        }
        
    return status;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::FetchTURNServerObjectL
// Fetches the values of TURNSpecific/TURNServer/<x>/ leaf objects.
// ---------------------------------------------------------------------------
//
CSmlDmAdapter::TError 
    CNSmlDmNATFWTraversalAdapter::FetchTURNServerObjectL(
    const TDesC8& aURI, 
    const TDesC8& aLUID,
    TDes8& aSegmentResult )
    {
    CSmlDmAdapter::TError status = EOk;
    TBuf8<KSmlMaxURISegLen> lastURISeg = LastURISeg( aURI );
    
    TUint32 turnKey = DesToInt( LastURISeg( aLUID ) );
    if ( KErrNone == turnKey )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    TPtrC8 tmpLuid = RemoveLastURISeg( aLUID );
    TPtrC8 domainLuid = RemoveLastURISeg( RemoveLastURISeg( tmpLuid ) );
    TInt domainLoc = FindDomainLocation( domainLuid );

    if ( KErrNotFound == domainLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }

    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
    RArray<TUint32> turnKeys;
    CleanupClosePushL( turnKeys ); // CS:2
    TUint32 domainKey = iNATFWDomainEntries->At( domainLoc )->
        GetDomainKey();
    domainKey |= KUNSAFProtocolsFieldTypeMask;
    domainKey ^= KUNSAFProtocolsFieldTypeMask;
    rep->FindL( domainKey|KUNSAFProtocolsTURNAddressMask,
        KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );
    TInt turnCount( turnKeys.Count() );
    TInt turnLoc( KErrNotFound );

    for ( TInt counter = 0; counter < turnCount; counter++ )
        {
        if ( turnKey == turnKeys[counter] )
            {
            turnLoc = counter;
            }
        }
    turnKeys.Close();
    CleanupStack::PopAndDestroy( &turnKeys ); // CS:1
    CleanupStack::PopAndDestroy( rep ); // CS:0
    // TURN key was not in CentRep.
    if ( KErrNotFound == turnLoc )
        {
        status = CSmlDmAdapter::ENotFound;
        return status;
        }
    // Fetch values.
    else
        {
        // TURNSrvAddr
        if ( KNSmlDMTURNSrvAddr() == lastURISeg )    
            {
            aSegmentResult.Copy( iNATFWDomainEntries->
                At( domainLoc )->TurnSrvAddrL( turnLoc ) );
            }       
        // TURNSrvPort
        else if ( KNSmlDMTURNSrvPort() == lastURISeg )
            {
            aSegmentResult.Num( iNATFWDomainEntries->
                At( domainLoc )->TurnSrvPort( turnLoc ) );
            }
        // TURNUsername
        else if ( KNSmlDMTURNUsername() == lastURISeg )    
            {
            aSegmentResult.Copy( iNATFWDomainEntries->
                At( domainLoc )->TurnUsernameL( turnLoc ) );
            }       
        // TURNPassword
        else if ( KNSmlDMTURNPassword() == lastURISeg )    
            {
            aSegmentResult.Copy( iNATFWDomainEntries->
                At( domainLoc )->TurnPasswordL( turnLoc ) );
            }       
        else
            {
            // None of TURN server settings found.
            status = CSmlDmAdapter::ENotFound;
            }
        }
        
    return status;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::DesToInt
// Converts a 8 bit descriptor to int.
// ---------------------------------------------------------------------------
//
TInt CNSmlDmNATFWTraversalAdapter::DesToInt( 
    const TDesC8& aDes )
    {
    TLex8 lex( aDes );
    TInt value = 0;
    lex.Val( value );
    return value;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::RemoveLastURISeg
// Returns parent URI, i.e. removes last uri segment.
// ---------------------------------------------------------------------------
//
TPtrC8 CNSmlDmNATFWTraversalAdapter::RemoveLastURISeg( 
    const TDesC8& aURI )
    {
    TInt counter ( 0 );
    
    // Check that aURI exists
    if ( 0 < aURI.Length() )
        {
        for ( counter = aURI.Length() - 1; counter >= 0; counter-- )
            {
            if ( KDMNATFWSeparator == aURI[counter] )
                {
                break;
                }
            }
        }
    return aURI.Left( counter );
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::NumOfURISegs
// For getting the number of URI segs.
// ---------------------------------------------------------------------------
//
TInt CNSmlDmNATFWTraversalAdapter::NumOfURISegs( const TDesC8& aURI )
    {
    TInt numOfURISegs ( KOneSeg );
    for ( TInt counter = 0; counter < aURI.Length(); counter++ )
        {
        if ( KDMNATFWSeparator == aURI[counter] )
            {
            numOfURISegs++;
            }
        }
    return numOfURISegs;
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::LastURISeg
// Returns only the last URI segment.
// ---------------------------------------------------------------------------
//
TPtrC8 CNSmlDmNATFWTraversalAdapter::LastURISeg( const TDesC8& aURI )
    {
    TInt counter ( 0 );
    
    // Check that aURI exists
    if ( 0 < aURI.Length() )
        {
        for ( counter = aURI.Length() - 1; counter >= 0; counter-- )
            {
            if ( KDMNATFWSeparator == aURI[counter] )
                {
                break;
                }
            }
        }
    if ( 0 == counter  )
        {
        return aURI;
        }
    else
        {
        return aURI.Mid( counter + 1 );
        }
    }

// ---------------------------------------------------------------------------
// CNSmlDmNATFWTraversalAdapter::RemoveDotSlash
// Return URI without dot and slash in start.
// ---------------------------------------------------------------------------
//
TPtrC8 CNSmlDmNATFWTraversalAdapter::RemoveDotSlash( const TDesC8& aURI )
    {
    if ( 0 == aURI.Find( KNSmlDMNATFWUriDotSlash ) )
        {
        return aURI.Right( aURI.Length()-KNSmlDMNATFWUriDotSlash().Length() );
        }
    else
        {
        return aURI;
        }
    }
