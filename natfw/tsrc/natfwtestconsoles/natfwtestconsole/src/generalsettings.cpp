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




#include "generalsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

/******************************************************/
//Settings
//Domain spesific settings

_LIT8( KSTUNServAddr, "193.65.183.3:3478" );
//const TInt KNATRefreshTCP = 500;
//const TInt KNATRefreshUDP = 500;
//const TInt KEnableCRLFRefresh = 0;
_LIT8( KSTUNServUsername, "username" );
_LIT8( KSTUNServPassword, "password" );
// none additional STUN server settings
_LIT8( KNatProtocol, "nokia.stun" );
const TInt KSharedsecretNotSupported = 1; // 0 = supported
const TInt KPortPoolStartPort = 5000;
const TInt KPortPoolEndPort = 5100;
//ICE spesific in the Domain settings 
//TURN specific in the Domain settings 
//const TInt KLifeTime = 500;
//const TInt KTimerValue = 500;
//IAP Spesific settings
const TInt KIAPNATRefreshTCP = 100;
const TInt KIAPNATRefreshUDP = 100;
const TInt KIAPSTUNRetransmit = 20;


// -----------------------------------------------------------------------------
// CGeneralSettings::NewL
// -----------------------------------------------------------------------------
// 
CGeneralSettings* CGeneralSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CGeneralSettings* self = CGeneralSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CGeneralSettings::NewL
// -----------------------------------------------------------------------------
//
CGeneralSettings* CGeneralSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CGeneralSettings* self = new( ELeave ) CGeneralSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CGeneralSettings::CGeneralSettings
// -----------------------------------------------------------------------------
//
CGeneralSettings::CGeneralSettings( TUint aIapId )
    : iIapId( aIapId )
    {
    }
    
    
// -----------------------------------------------------------------------------
// CGeneralSettings::~CGeneralSettings
// -----------------------------------------------------------------------------
//   
CGeneralSettings::~CGeneralSettings( )
    {
    EraseL( KUNSAFProtocolsDomainTableMask, 0xf0000000 );
 	EraseL( KUNSAFProtocolsIAPTableMask, 0xf0000000 );
 	delete iDomain;
    }
    
    
// -----------------------------------------------------------------------------
// CGeneralSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CGeneralSettings::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    ConstructBaseL();
    }


// -----------------------------------------------------------------------------
// CGeneralSettings::GetDomainKey
// -----------------------------------------------------------------------------
//      
TInt CGeneralSettings::GetDomainKey( TUint32& aDomainKey )
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
// CGeneralSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CGeneralSettings::CreateSettingsL()
    {
    TUint32 domainKey;
    User::LeaveIfError( GetDomainKey( domainKey ) );

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
    StoreL( KUNSAFProtocolsDomainSharedSecretNotSupported |
        domainKey, KSharedsecretNotSupported );    
    StoreL( KUNSAFProtocolsPortPoolStartPortMask | domainKey,
        KPortPoolStartPort );
    StoreL( KUNSAFProtocolsPortPoolEndPortMask | domainKey, 
        KPortPoolEndPort );
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
    StoreL( KUNSAFProtocolsSTUNUsernameMask | domainKey | STUNServerKey,
        KSTUNServUsername );
    StoreL( KUNSAFProtocolsSTUNPasswordMask | domainKey | STUNServerKey,
        KSTUNServPassword );
    
    /************************************************************************/
    //ICE spesific in the Domain settings
    /*
    StoreL( KUNSAFProtocolsICECandidateTimerMask | domainKey,
        KAnotherCanditateTimer );
    */
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | domainKey, 
        KNatProtocol );
    /************************************************************************/
    //TURN specific in the Domain settings
    /*
    StoreL( KUNSAFProtocolsTURNLifeTimeMask | domainKey, KLifeTime );
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

