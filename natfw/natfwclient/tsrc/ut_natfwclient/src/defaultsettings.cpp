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




#include "defaultsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

/******************************************************/
//Settings
//Domain spesific settings

_LIT8( KSTUNServAddr, "isp1v.wipsl.com:3478" );
_LIT8( KPriorityOrder, "nokia.test" );
const TInt KNATRefreshTCP = 500;
const TInt KNATRefreshUDP = 500;
const TInt KEnableCRLFRefresh = 0;
// none additional STUN server settings
const TInt KSTUNUsed = 1;
const TInt KSharedsecretNotSupported = 1;   
const TInt KPortPoolStartPort = 49152;
const TInt KPortPoolEndPort = 49252;
const TInt KPortFieldRange = 20;
//const TInt KPlugInId = 0x10275446; // STUN plug-in implementation id
const TInt KPlugInId = 0x10275401; // Test plug-in implementation id
const TInt KPlugInServiceId = 1;
//ICE spesific in the Domain settings 
const TInt KOfferICE = 0;
const TInt KMode = 1;
const TInt KAnotherCanditateTimer = 500;
//TURN specific in the Domain settings 
const TInt KLifeTime = 500;
const TInt KBandwith = 28800;
_LIT8( KRemoteAddr, "40.0.0.4:404" );
_LIT8( KData, "Hello World" );
_LIT8( KRelayAddr, "20.0.0.2:202" );
const TInt KRequestedPortProps = 0x00000000;
const TInt KRequestedTransport = 1; // 0 = UDP, 1 = TCP
_LIT8( KRequestedAddr, "30.0.0.3:303" );
const TInt KTimerValue = 500;
//IAP Spesific settings

const TInt KIAPNATRefreshTCP = 100;
const TInt KIAPNATRefreshUDP = 100;
const TInt KIAPSTUNRetransmit = 400;


// -----------------------------------------------------------------------------
// CDefaultSettings::NewL
// -----------------------------------------------------------------------------
// 
EXPORT_C CDefaultSettings* CDefaultSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CDefaultSettings* self = CDefaultSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CDefaultSettings::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CDefaultSettings* CDefaultSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CDefaultSettings* self = new( ELeave ) CDefaultSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    self->ConstructBaseL( KCRUidUNSAFProtocols );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CDefaultSettings::CDefaultSettings
// -----------------------------------------------------------------------------
//
CDefaultSettings::CDefaultSettings( TUint aIapId )
    : iIapId( aIapId )
    {
    }
    
    
// -----------------------------------------------------------------------------
// CDefaultSettings::~CDefaultSettings
// -----------------------------------------------------------------------------
//   
CDefaultSettings::~CDefaultSettings( )
    {
    EraseL( KUNSAFProtocolsDomainTableMask, 0xf0000000 );
 	EraseL( KUNSAFProtocolsIAPTableMask, 0xf0000000 );
 	delete iDomain;
    }
    
    
// -----------------------------------------------------------------------------
// CDefaultSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CDefaultSettings::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    }
    
    
// -----------------------------------------------------------------------------
// CDefaultSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
EXPORT_C void CDefaultSettings::CreateSettingsL()
    {
    //Create key for domain settings
    TUint32 domainKey = CreateNewTableKeyL(
        KUNSAFProtocolsDomainTableMask, KUNSAFProtocolsFieldTypeMask );
    //create key for the stun server settings in the domain settings
    TUint32 STUNServerKey = CreateNewTableKeyL(
        KUNSAFProtocolsSTUNServerTableMask | domainKey,
        KUNSAFProtocolsSubTableFieldTypeMask );
    //create key for the Iap settings
    TUint32 IapKey = CreateNewTableKeyL(
        KUNSAFProtocolsIAPTableMask, KUNSAFProtocolsFieldTypeMask );
    
    //Settings
    /************************************************************************/
    //Domain spesific settings
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | domainKey, 
        KPriorityOrder );
    StoreL( KUNSAFProtocolsDomainMask | domainKey, *iDomain );
    //StoreL( KUNSAFProtocolsSTUNUsedMask | domainKey, KSTUNUsed );
    StoreL( KUNSAFProtocolsDomainSharedSecretNotSupported |
        domainKey, KSharedsecretNotSupported );    
    StoreL( KUNSAFProtocolsPortPoolStartPortMask | domainKey,
        KPortPoolStartPort );
    StoreL( KUNSAFProtocolsPortPoolEndPortMask | domainKey, 
        KPortPoolEndPort );
    /*StoreL( KUNSAFProtocolsPortFieldLengthMask | domainKey, 
        KPortFieldRange ); */
    /*StoreL( KUNSAFProtocolsPlugInIdMask | domainKey, KPlugInId );
    StoreL( KUNSAFProtocolsPlugInServiceIdMask | domainKey, 
        KPlugInServiceId );*/
    /************************************************************************/
    //STUN-server  settings in the domain
    StoreL( KUNSAFProtocolsSTUNAddressMask | domainKey | STUNServerKey,
        KSTUNServAddr );
    /*    
    StoreL( KUNSAFProtocolsSTUNTCPTimeoutMask | domainKey | 
        STUNServerKey, KNATRefreshTCP );
    StoreL( KUNSAFProtocolsSTUNUDPTimeoutMask | domainKey | 
        STUNServerKey, KNATRefreshUDP );
    StoreL( KUNSAFProtocolsSTUNEnableCRLFRefreshMask | domainKey | 
        STUNServerKey, KEnableCRLFRefresh );
    */
    /************************************************************************/
    //ICE spesific in the Domain settings
    /*
    StoreL( KUNSAFProtocolsOfferICEMask | domainKey, KOfferICE );
    StoreL( KUNSAFProtocolsICECandidateTimerMask | domainKey,
        KAnotherCanditateTimer );
    */
    /************************************************************************/
    //TURN specific in the Domain settings
    /*
    StoreL( KUNSAFProtocolsTURNLifeTimeMask | domainKey, KLifeTime );
    StoreL( KUNSAFProtocolsTURNBandwidthMask | domainKey, KBandwith );
    StoreL( KUNSAFProtocolsRequestedPortPropsMask | domainKey, 
        KRequestedPortProps );
    StoreL( KUNSAFProtocolsRequestedTransportMask | domainKey, 
        KRequestedTransport );
    StoreL( KUNSAFProtocolsRequestedAdressMask | domainKey, 
        KRequestedAddr );
    StoreL( KUNSAFProtocolsTURNTimerValueMask | domainKey,
        KTimerValue );
    */
    /************************************************************************/
    //IAP Spesific settings
    StoreL( KUNSAFProtocolsIAPIdMask | IapKey, iIapId );
    StoreL( KUNSAFProtocolsIntervalTCPMask | IapKey, 
        KIAPNATRefreshTCP );
    StoreL( KUNSAFProtocolsIntervalUDPMask | IapKey, 
        KIAPNATRefreshUDP );
    StoreL( KUNSAFProtocolsSTUNRetransmitTimerMask | IapKey, 
        KIAPSTUNRetransmit );
    }
    
    