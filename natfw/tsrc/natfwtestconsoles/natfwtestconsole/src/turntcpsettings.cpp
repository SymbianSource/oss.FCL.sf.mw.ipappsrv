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




#include "turntcpsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

//Settings
//Domain spesific settings
_LIT8( KTURNServAddr, "193.65.183.11:3478" );
_LIT8( KTURNUser, "markosaa" );
_LIT8( KTURNPassword, "markosaaeb" );
//const TInt KNATRefreshTCP = 1140;
//const TInt KNATRefreshUDP = 30;
//const TInt KEnableCRLFRefresh = 0;
// none additional STUN server settings
_LIT8( KNatProtocol, "nokia.turn" );
//TURN specific in the Domain settings 
//const TInt KLifeTime = 500;
//const TInt KTimerValue = 1600;


// -----------------------------------------------------------------------------
// CTurnTcpSettings::NewL
// -----------------------------------------------------------------------------
// 
CTurnTcpSettings* CTurnTcpSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CTurnTcpSettings* self = CTurnTcpSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CTurnTcpSettings::NewL
// -----------------------------------------------------------------------------
//
CTurnTcpSettings* CTurnTcpSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CTurnTcpSettings* self = new( ELeave ) CTurnTcpSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CTurnTcpSettings::CTurnTcpSettings
// -----------------------------------------------------------------------------
//
CTurnTcpSettings::CTurnTcpSettings( TUint aIapId )
    : CGeneralSettings( aIapId )
    {
    }
    
    
// -----------------------------------------------------------------------------
// CTurnTcpSettings::~CTurnTcpSettings
// -----------------------------------------------------------------------------
//   
CTurnTcpSettings::~CTurnTcpSettings( )
    {
    EraseL( KUNSAFProtocolsDomainTableMask, 0xf0000000 );
 	EraseL( KUNSAFProtocolsIAPTableMask, 0xf0000000 );
    }
    
    
// -----------------------------------------------------------------------------
// CTurnTcpSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CTurnTcpSettings::ConstructL( const TDesC8& aDomain )
    {
    CGeneralSettings::ConstructL( aDomain );
    }
    
    
// -----------------------------------------------------------------------------
// CTurnTcpSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CTurnTcpSettings::CreateSettingsL()
    {
    CGeneralSettings::CreateSettingsL();
    
    User::LeaveIfError( GetDomainKey( iDomainKey ) );
    
    //create key for the turn server settings in the domain settings
    TUint32 TURNServerKey = CreateNewTableKeyL(
        KUNSAFProtocolsTURNServerTableMask | iDomainKey,
        KUNSAFProtocolsSubTableFieldTypeMask );
   
    //Settings
    /************************************************************************/
    //domain specific
     StoreL( KUNSAFProtocolsUsedNATProtocolMask | iDomainKey, 
        KNatProtocol );
        
    /************************************************************************/
    //TURN-server settings in the domain
    StoreL( KUNSAFProtocolsTURNAddressMask | iDomainKey | TURNServerKey,
        KTURNServAddr );
    /*    
    StoreL( KUNSAFProtocolsTURNTCPTimeoutMask | iDomainKey | TURNServerKey,
        KNATRefreshTCP );
    StoreL( KUNSAFProtocolsTURNUDPTimeoutMask | iDomainKey | TURNServerKey,
        KNATRefreshUDP );
    StoreL( KUNSAFProtocolsTURNEnableCRLFRefreshMask | iDomainKey | TURNServerKey,
        KEnableCRLFRefresh );
    */
    StoreL( KUNSAFProtocolsTURNUsernameMask | iDomainKey | TURNServerKey,
        KTURNUser );
    StoreL( KUNSAFProtocolsTURNPasswordMask | iDomainKey | TURNServerKey,
        KTURNPassword );
    /************************************************************************/
    //TURN specific in the Domain settings
    StoreL( KUNSAFProtocolsTURNAddressMask | iDomainKey, KTURNServAddr );
    /*
    StoreL( KUNSAFProtocolsTURNLifeTimeMask | iDomainKey, KLifeTime );
    StoreL( KUNSAFProtocolsTURNTimerValueMask | iDomainKey, 
        KTimerValue );
    */
    }
