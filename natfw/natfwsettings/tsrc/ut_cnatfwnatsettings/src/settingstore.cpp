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



#include "settingstore.h"

#include <unsafprotocolscrkeys.h>

#include "datadepository.h"
#include "cnatfwstunsettings.h"
#include "cnatfwturnsettings.h"
#include <mnatfwserversettings.h>
#include "natfwdefaultsettingsvalues.h"

const TUint KDefaultGranularity = 1;

// ---------------------------------------------------------------------------
// CSettingStore::CSettingStore
// ---------------------------------------------------------------------------
// 
CSettingStore::CSettingStore( TUint aIapId ) :
    iIapId( aIapId )
    {
    //domain
    iRefreshIntervalUdp = KNATFWDefaultRefreshIntervalUdp;
    iRefreshIntervalTcp = KNATFWDefaultRefreshIntervalTcp;
    iCrlfRefresEnabled = KNATFWDefaultCRLFRefreshEnabled;
    iUseSharedSecret = KNATFWDefaultUseSharedSecret;
    iStartPort = KNATFWDefaultPortPoolStartPort;
    iEndPort = KNATFWDefaultPortPoolEndPort;
    //ICE
    iHostPref = KNATFWDefaultlHostPref;
    iServerReflexivePref = KNATFWDefaultServerRefPref;
    iRelayPref = KNATFWDefaultRelayPref;
    iPeerReflexivePref = KNATFWDefaultPeerRefPref;
    iIpv4AddrFamilyPref = KNATFWDefaultIPV4Pref;
    iIpv6AddrFamilyPref = KNATFWDefaultIPV6Pref;
    iVpnInterfacePref = KNATFWDefaultVPNPref;
    iUdpTransportPref = KNATFWDefaultUDPPref;
    iTcpTransportPref = KNATFWDefaultTCPPref;
    iTcpDirActivePref = KNATFWDefaultTCPActivePref;
    iTcpDirPassivePref = KNATFWDefaultTCPPassivePref;
    iTcpDirSimultaneousOpenPref = KNATFWDefaultTCPSimultPref;
    // Iap
    iIapRefreshIntervalUdp = KNATFWDefaultIapRefreshIntervalUdp;
    iIapRefreshIntervalTcp = KNATFWDefaultIapRefreshIntervalTcp;
    iIapStunRetransmitTimer = KNATFWDefaultStunRetransmitTimer;
    }


// ---------------------------------------------------------------------------
// CSettingStore::ConstructL
// ---------------------------------------------------------------------------
//  
void CSettingStore::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    iRepository = CRepository::NewL( KCRUidUNSAFProtocols );
    iDepository = CDataDepository::NewL( iRepository );
    iUtilityPlugins = KNATFWDefaultIceUsedPlugins().AllocL();
    iAvailableNatProtocols = KNATFWDefaultPriorityOrder().AllocL();
    iStunSettings = CNATFWStunSettings::NewL();
    iTurnSettings = CNATFWTurnSettings::NewL();
    }


// ---------------------------------------------------------------------------
// CSettingStore::NewL
// ---------------------------------------------------------------------------
// 
CSettingStore* CSettingStore::NewL( const TDesC8& aDomain, TUint aIapId )
    {
    CSettingStore* self = CSettingStore::NewLC( aDomain, aIapId );
    CleanupStack::Pop(self);
    return self;
    }


// ---------------------------------------------------------------------------
// CSettingStore::NewL
// ---------------------------------------------------------------------------
//     
CSettingStore* CSettingStore::NewLC( const TDesC8& aDomain, TUint aIapId )
    {
    CSettingStore* self = new( ELeave ) CSettingStore( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }


// ---------------------------------------------------------------------------
// CSettingStore::~CSettingStore
// ---------------------------------------------------------------------------
//
CSettingStore::~CSettingStore( )
    {
    delete iDepository;
    delete iRepository;
    delete iDomain;
    delete iAvailableNatProtocols;
    delete iUtilityPlugins;
    delete iStunSettings;
    delete iTurnSettings;
    }


// ---------------------------------------------------------------------------
// CSettingStore::ReadDomainSettingsKeyL
// ---------------------------------------------------------------------------
//
TInt CSettingStore::ReadDomainSettingsKeyL( const TDesC8& aDomain, 
    TUint32& aDomainKey ) const
    {
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    
    TInt err = iRepository->FindEqL( KUNSAFProtocolsDomainMask,
        KUNSAFProtocolsFieldTypeMask, aDomain, keys );
    TInt count = keys.Count();
    
    if ( KErrNone == err && 1 == count )
        {
        aDomainKey = KUNSAFProtocolsDomainMask ^ keys[0];
        }
    if ( count > 1 )
        {
        err = KErrCorrupt;
        }
    CleanupStack::PopAndDestroy( &keys );
    return err;
    }


// ---------------------------------------------------------------------------
// CSettingStore::EraseL
// ---------------------------------------------------------------------------
//
TInt CSettingStore::EraseL( TUint32 aTableMask, TUint32 aFieldTypeMask )
    {
    return iDepository->EraseL( aTableMask, aFieldTypeMask );
    }


// ---------------------------------------------------------------------------
// CSettingStore::GetDomainKey
// ---------------------------------------------------------------------------
//
TUint32 CSettingStore::GetDomainKeyL()
    {
    TUint32 domainKey; 
    TInt error = ReadDomainSettingsKeyL( *iDomain, domainKey );
    
    if( KErrNotFound == error )
        {
        domainKey = CreateNewDomainL( *iDomain );
        }
    else
        {
        User::LeaveIfError( error );
        }
    return domainKey;
    }


// ---------------------------------------------------------------------------
// CSettingStore::CreateNewDomainL
// ---------------------------------------------------------------------------
//
TUint32 CSettingStore::CreateNewDomainL( const TDesC8& aDomain )
    {
    TUint32 domainKey = iDepository->CreateNewTableKeyL(
        KUNSAFProtocolsDomainMask, KUNSAFProtocolsFieldTypeMask );
    
    iDepository->StoreL( KUNSAFProtocolsDomainMask | domainKey, aDomain );
    return domainKey;
    }


// ---------------------------------------------------------------------------
// CSettingStore::CreateNewIapL
// ---------------------------------------------------------------------------
//
TUint32 CSettingStore::CreateNewIapL( const TInt aIapId )
    {
    TUint32 iapKey = iDepository->CreateNewTableKeyL(
        KUNSAFProtocolsIAPIdMask, KUNSAFProtocolsFieldTypeMask );
        
    iDepository->StoreL( KUNSAFProtocolsIAPIdMask | iapKey, aIapId );
    return iapKey;
    }


// ---------------------------------------------------------------------------
// CSettingStore::CreateServerKeyL
// ---------------------------------------------------------------------------
//
TUint32 CSettingStore::CreateServerKeyL( TUint32 aDomainKey,
    TUint32 aServerMask )
    {
    const TUint32 KMaskForAllServerSettingsIds = 0xff000fff;
    
    return iDepository->CreateNewServerKeyL( aServerMask |
        aDomainKey, KMaskForAllServerSettingsIds );
    }


// ---------------------------------------------------------------------------
// CSettingStore::StoreSettingsL
// ---------------------------------------------------------------------------
//
void CSettingStore::StoreSettingsL()
    {
    TUint32 domainKey;

    domainKey = GetDomainKeyL();
    
    RPointerArray<MNATFWServerSettings> stunServers;
    CleanupClosePushL( stunServers );
    
    iStunSettings->GetStunServerArrayL( stunServers );

    RPointerArray<MNATFWServerSettings> turnServers;
    CleanupClosePushL( turnServers ); 
       
    iTurnSettings->GetTurnServerArrayL( turnServers );

    TUint32 IapKey = CreateNewIapL( iIapId );
    
    //Settings
    /************************************************************************/
    //Domain spesific settings
    
    iDepository->StoreL( KUNSAFProtocolsDomainSharedSecretNotSupported |
        domainKey, !iUseSharedSecret ); 
           
    iDepository->StoreL( KUNSAFProtocolsPortPoolStartPortMask | domainKey,
        iStartPort );
        
    iDepository->StoreL( KUNSAFProtocolsPortPoolEndPortMask | domainKey, 
        iEndPort );
        
    iDepository->StoreL( KUNSAFProtocolsUsedNATProtocolMask | domainKey, 
        *iAvailableNatProtocols );
        
    iDepository->StoreL( KUNSAFProtocolsDomainEnableCRLFRefresh | domainKey, 
        iCrlfRefresEnabled );
        
    iDepository->StoreL( KUNSAFProtocolsDomainIntervalUDPMask | domainKey,
        iRefreshIntervalUdp );
        
    iDepository->StoreL( KUNSAFProtocolsDomainIntervalTCPMask | domainKey, 
        iRefreshIntervalTcp );
       
    /************************************************************************/
    //STUN settings in the domain
    
    //STUN Servers
    for ( TInt i( 0 ); i < stunServers.Count(); i++ )
        {
        TUint32 stunServerkey = CreateServerKeyL(
            domainKey, KUNSAFProtocolsSTUNServerTableMask );
        
        if ( stunServers[i]->Address().Compare( KNullDesC8 ) )
            {
            iDepository->StoreL( KUNSAFProtocolsSTUNAddressMask | domainKey |
                stunServerkey, stunServers[i]->Address() );
            }
        if ( stunServers[i]->Port() != 0 )
            {
            iDepository->StoreL( KUNSAFProtocolsSTUNPortMask | domainKey |
                stunServerkey, stunServers[i]->Port() );
            }
        if ( stunServers[i]->Username().Compare( KNullDesC8 ) )
            {    
            iDepository->StoreL( KUNSAFProtocolsSTUNUsernameMask | domainKey |
                stunServerkey, stunServers[i]->Username() );
            }
        if ( stunServers[i]->Password().Compare( KNullDesC8 ) )
            {
            iDepository->StoreL( KUNSAFProtocolsSTUNPasswordMask | domainKey |
                stunServerkey, stunServers[i]->Password() );
            }
        /*    
        iDepository->StoreL( KUNSAFProtocolsSTUNConnectivityCheckTimeoutMask |
            domainKey | stunServerkey, iStunSettings->RetransmissionTimeout() );
        */
        }

    iDepository->StoreL( KUNSAFProtocolsLatestConnectedSTUNServerAddressMask |
        domainKey, iStunSettings->LatestConnectedServerAddr() );
    
    iDepository->StoreL( KUNSAFProtocolsLatestConnectedSTUNServerPortMask |
        domainKey, iStunSettings->LatestConnectedServerPort() );
        
    iDepository->StoreL( KUNSAFProtocolsStunRetransmissionTimeoutMask |
        domainKey, iStunSettings->RetransmissionTimeout() );

    /************************************************************************/
    //TURN settings in the domain
   
    iDepository->StoreL( KUNSAFProtocolsLatestConnectedTURNServerAddressMask |
        domainKey, iTurnSettings->LatestConnectedServerAddr() );
    
    iDepository->StoreL( KUNSAFProtocolsLatestConnectedTURNServerPortMask |
        domainKey, iTurnSettings->LatestConnectedServerPort() );
    
    iDepository->StoreL( KUNSAFProtocolsTurnRetransmissionTimeoutMask |
        domainKey, iTurnSettings->RetransmissionTimeout() );
      
    //TURN Servers
    for ( TInt i( 0 ); i < turnServers.Count(); i++ )
        {
        TUint32 turnServerkey = CreateServerKeyL(
            domainKey, KUNSAFProtocolsTURNServerTableMask );
        if ( turnServers[i]->Address().Compare( KNullDesC8 ) )
            {    
            iDepository->StoreL( KUNSAFProtocolsTURNAddressMask | domainKey |
                turnServerkey, turnServers[i]->Address() );
            }
        if ( turnServers[i]->Port() != 0 )
            {
            iDepository->StoreL( KUNSAFProtocolsTURNPortMask | domainKey |
                turnServerkey, turnServers[i]->Port() );
            }
        if ( turnServers[i]->Username().Compare( KNullDesC8 ) )
            {    
            iDepository->StoreL( KUNSAFProtocolsTURNUsernameMask | domainKey |
                turnServerkey, turnServers[i]->Username() );
            }
        if ( turnServers[i]->Password().Compare( KNullDesC8 ) )
            {
            iDepository->StoreL( KUNSAFProtocolsTURNPasswordMask | domainKey |
                turnServerkey, turnServers[i]->Password() );
            }
        /* 
        iDepository->StoreL( KUNSAFProtocolsTURNConnectivityCheckTimeoutMask |
            domainKey | turnServerkey, iTurnSettings->RetransmissionTimeout() );
        */
        }
        
    /************************************************************************/
    //ICE spesific in the Domain settings
    
    iDepository->StoreL( KUNSAFProtocolsNATUtilities | domainKey,
        *iUtilityPlugins );
    
    iDepository->StoreL( KUNSAFProtocolsHostPref | domainKey,
       iHostPref );

    iDepository->StoreL( KUNSAFProtocolsServerRefPref | domainKey,
        iServerReflexivePref );    
 
    iDepository->StoreL( KUNSAFProtocolsRelayPref | domainKey,
        iRelayPref );

    iDepository->StoreL( KUNSAFProtocolsPeerRefPref | domainKey,
        iPeerReflexivePref );

    iDepository->StoreL( KUNSAFProtocolsIPV4Pref | domainKey,
        iIpv4AddrFamilyPref );

    iDepository->StoreL( KUNSAFProtocolsIPV6Pref | domainKey,
        iIpv6AddrFamilyPref );

    iDepository->StoreL( KUNSAFProtocolsVPNPref | domainKey,
        iVpnInterfacePref );
    
    iDepository->StoreL( KUNSAFProtocolsUDPPref | domainKey,
        iUdpTransportPref );
    
    iDepository->StoreL( KUNSAFProtocolsTCPPref | domainKey,
        iTcpTransportPref );

    iDepository->StoreL( KUNSAFProtocolsTCPActivePref | domainKey,
        iTcpDirActivePref );

    iDepository->StoreL( KUNSAFProtocolsTCPPassivePref | domainKey,
        iTcpDirPassivePref );

    iDepository->StoreL( KUNSAFProtocolsTCPSimultPref | domainKey,
        iTcpDirSimultaneousOpenPref );

    /************************************************************************/
    //IAP Spesific settings

    iDepository->StoreL( KUNSAFProtocolsIntervalTCPMask | IapKey, 
        iIapRefreshIntervalTcp );
        
    iDepository->StoreL( KUNSAFProtocolsIntervalUDPMask | IapKey, 
        iIapRefreshIntervalUdp );
        
    iDepository->StoreL( KUNSAFProtocolsSTUNRetransmitTimerMask | IapKey, 
        iIapStunRetransmitTimer );
    
    CleanupStack::PopAndDestroy( &turnServers );
    CleanupStack::PopAndDestroy( &stunServers );
    }
