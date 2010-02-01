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




#include "testsettings.h"

#include <unsafprotocolscrkeys.h>
#include "natfwdefaultsettingsvalues.h"

// -----------------------------------------------------------------------------
// CTestSettings::NewL
// -----------------------------------------------------------------------------
// 
CTestSettings* CTestSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CTestSettings* self = CTestSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CTestSettings::NewL
// -----------------------------------------------------------------------------
//
CTestSettings* CTestSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CTestSettings* self = new( ELeave ) CTestSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CTestSettings::CTestSettings
// -----------------------------------------------------------------------------
//
CTestSettings::CTestSettings( TUint aIapId )
    : iIapId( aIapId )
    {
    //iUdpTimeOut = KNATFWDefaultUDPTimeout;
    //iTcpTimeOut = KNATFWDefaultTCPTimeout;
    iUdpRefreshInterval = KNATFWDefaultRefreshIntervalUdp;
    iTcpRefreshInterval = KNATFWDefaultRefreshIntervalTcp;
    iStunRetransmitTimer = KNATFWDefaultStunRetransmitTimer;
    iUseSharedSecret = KNATFWDefaultUseSharedSecret;
    iTurnTimerValue = KNATFWDefaultTURNTimerValue;
    iPortPoolStartPort = KNATFWDefaultPortPoolStartPort;
    iPortPoolEndPort = KNATFWDefaultPortPoolEndPort;
    iEnableCrlfRefresh = KNATFWDefaultCRLFRefreshEnabled;
    //iConnectivityCheckTimeout = KNATFWDefaultConnectivityCheckTimeout;
    iStunServerPort = 0;
 	iTurnServerPort = 0;
    }
    
    
// -----------------------------------------------------------------------------
// CTestSettings::~CTestSettings
// -----------------------------------------------------------------------------
//   
CTestSettings::~CTestSettings( )
    {
    TRAP_IGNORE( EraseL( KUNSAFProtocolsDomainTableMask, 0xf0000000 ) )
    TRAP_IGNORE( EraseL( KUNSAFProtocolsIAPTableMask, 0xf0000000 ) )
 	delete iLatestConnectedStunServerAddress;
    delete iLatestConnectedTurnServerAddress;
    delete iEcomPlugInPriorityOrder;
    delete iIceUsedPlugIns;
    delete iServerUsername; 
    delete iServerPassword;
    delete iStunServerAddr;
    delete iTurnServerAddr;
    delete iRequestedAddress;
    delete iDomain;
    }
    
    
// -----------------------------------------------------------------------------
// CTestSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CTestSettings::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();

    iLatestConnectedStunServerAddress = KNullDesC8().AllocL();
    iLatestConnectedTurnServerAddress = KNullDesC8().AllocL();
    iEcomPlugInPriorityOrder = KNATFWDefaultPriorityOrder().AllocL();
    iIceUsedPlugIns = KNATFWDefaultIceUsedPlugins().AllocL();
    iServerUsername = KNullDesC8().AllocL(); 
    iServerPassword = KNullDesC8().AllocL();
    iStunServerAddr = KNullDesC8().AllocL();
    iTurnServerAddr = KNullDesC8().AllocL();
    iRequestedAddress = KNullDesC8().AllocL();
    
    ConstructBaseL();
    }


// -----------------------------------------------------------------------------
// CTestSettings::GetDomainKey
// -----------------------------------------------------------------------------
//      
TInt CTestSettings::GetDomainKey( TUint32& aDomainKey )
    {
    TInt error = ReadDomainSettingsKeyL( *iDomain, aDomainKey );
    
    if( KErrNotFound == error )
        {
        aDomainKey = CreateNewTableKeyL(
            KUNSAFProtocolsDomainTableMask, KUNSAFProtocolsFieldTypeMask );
        StoreL( KUNSAFProtocolsDomainMask | aDomainKey, *iDomain );
        return KErrNone;
        }
    else
        {
        return error;
        }
    }
    
    
// -----------------------------------------------------------------------------
// CTestSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CTestSettings::CreateSettingsL()
    {
    TUint32 domainKey;
    User::LeaveIfError( GetDomainKey( domainKey ) );

    //create key for the stun server settings in the domain settings
    TUint32 StunServerKey = CreateNewTableKeyL(
        KUNSAFProtocolsSTUNServerTableMask | domainKey,
        KUNSAFProtocolsSubTableFieldTypeMask );
    //create key for the turn server settings in the domain settings
    TUint32 TurnServerKey = CreateNewTableKeyL(
        KUNSAFProtocolsTURNServerTableMask | domainKey,
        KUNSAFProtocolsSubTableFieldTypeMask );
    //create key for the Iap settings
    TUint32 IapKey = CreateNewTableKeyL(
        KUNSAFProtocolsIAPTableMask, KUNSAFProtocolsFieldTypeMask );
    
    //Settings
    /************************************************************************/
    //Domain spesific settings
    StoreL( KUNSAFProtocolsDomainSharedSecretNotSupported |
        domainKey, !iUseSharedSecret );    
    StoreL( KUNSAFProtocolsPortPoolStartPortMask | domainKey,
        iPortPoolStartPort );
    StoreL( KUNSAFProtocolsPortPoolEndPortMask | domainKey, 
        iPortPoolEndPort );
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | domainKey, 
        *iEcomPlugInPriorityOrder );
    StoreL( KUNSAFProtocolsLatestConnectedSTUNServerAddressMask | domainKey, 
        *iLatestConnectedStunServerAddress );
    StoreL( KUNSAFProtocolsLatestConnectedTURNServerAddressMask | domainKey, 
        *iLatestConnectedTurnServerAddress );
        
    /************************************************************************/
    //STUN-server  settings in the domain
    StoreL( KUNSAFProtocolsSTUNAddressMask | domainKey | StunServerKey,
        *iStunServerAddr );
   /*     
    StoreL( KUNSAFProtocolsSTUNTCPTimeoutMask | domainKey | 
        StunServerKey, iTcpTimeOut );
    StoreL( KUNSAFProtocolsSTUNUDPTimeoutMask | domainKey | 
        StunServerKey, iUdpTimeOut );
    StoreL( KUNSAFProtocolsSTUNEnableCRLFRefreshMask | domainKey | 
        StunServerKey, iEnableCrlfRefresh );
    StoreL( KUNSAFProtocolsSTUNConnectivityCheckTimeoutMask | domainKey |
        StunServerKey, iConnectivityCheckTimeout );
    */
    StoreL( KUNSAFProtocolsSTUNUsernameMask | domainKey | StunServerKey,
        *iServerUsername );
    StoreL( KUNSAFProtocolsSTUNPasswordMask | domainKey | StunServerKey,
        *iServerPassword );
    
    /************************************************************************/
    //ICE spesific in the Domain settings
    StoreL( KUNSAFProtocolsNATUtilities | domainKey,
        *iIceUsedPlugIns );
        
    /************************************************************************/
    //TURN specific in the Domain settings
    /*
    StoreL( KUNSAFProtocolsTURNLifeTimeMask | domainKey, iTurnLifeTime );
    StoreL( KUNSAFProtocolsTURNBandwidthMask | domainKey, iTurnBandwidth );
    StoreL( KUNSAFProtocolsRequestedPortPropsMask | domainKey, 
        iRequestedPortProps );
    StoreL( KUNSAFProtocolsRequestedTransportMask | domainKey, 
        iRequestedTransport );
    StoreL( KUNSAFProtocolsRequestedAdressMask | domainKey, 
        *iRequestedAddress );
    StoreL( KUNSAFProtocolsTURNTimerValueMask | domainKey, 
        iTurnTimerValue );
    */
    /************************************************************************/
    //TURN Server Settings
    StoreL( KUNSAFProtocolsTURNAddressMask | domainKey | TurnServerKey,
        *iTurnServerAddr );
    /*
    StoreL( KUNSAFProtocolsTURNTCPTimeoutMask | domainKey | 
        TurnServerKey, iTcpTimeOut );
    StoreL( KUNSAFProtocolsTURNUDPTimeoutMask | domainKey | 
        TurnServerKey, iUdpTimeOut );
    StoreL( KUNSAFProtocolsTURNEnableCRLFRefreshMask | domainKey | 
        TurnServerKey, iEnableCrlfRefresh );
    StoreL( KUNSAFProtocolsTURNConnectivityCheckTimeoutMask | domainKey |
        TurnServerKey, iConnectivityCheckTimeout );
    */
    StoreL( KUNSAFProtocolsTURNUsernameMask | domainKey | TurnServerKey,
        *iServerUsername );
    StoreL( KUNSAFProtocolsTURNPasswordMask | domainKey | TurnServerKey,
        *iServerPassword );
        
    /************************************************************************/
    //IAP Spesific settings
    StoreL( KUNSAFProtocolsIAPIdMask | IapKey, iIapId );
    StoreL( KUNSAFProtocolsIntervalTCPMask | IapKey, 
        iTcpRefreshInterval );
    StoreL( KUNSAFProtocolsIntervalUDPMask | IapKey, 
        iUdpRefreshInterval );
    StoreL( KUNSAFProtocolsSTUNRetransmitTimerMask | IapKey, 
        iStunRetransmitTimer );
    }
    
