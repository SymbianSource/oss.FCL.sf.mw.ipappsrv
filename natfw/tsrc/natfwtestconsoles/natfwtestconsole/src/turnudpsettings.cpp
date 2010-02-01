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




#include "turnudpsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

//Settings
//Domain spesific settings
_LIT8( KTURNServAddr, "193.65.183.11:5070" );
_LIT8( KTURNUser, "eballusr" );
_LIT8( KTURNPassword, "ebpassw" );
//const TInt KNATRefreshTCP = 500;
//const TInt KNATRefreshUDP = 500;
//const TInt KEnableCRLFRefresh = 0;
// none additional STUN server settings
_LIT8( KNatProtocol, "nokia.turn" );
//TURN specific in the Domain settings 
//const TInt KLifeTime = 500;
//const TInt KTimerValue = 500;


// -----------------------------------------------------------------------------
// CTurnUdpSettings::NewL
// -----------------------------------------------------------------------------
// 
CTurnUdpSettings* CTurnUdpSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CTurnUdpSettings* self = CTurnUdpSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CTurnUdpSettings::NewL
// -----------------------------------------------------------------------------
//
CTurnUdpSettings* CTurnUdpSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CTurnUdpSettings* self = new( ELeave ) CTurnUdpSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CTurnUdpSettings::CTurnUdpSettings
// -----------------------------------------------------------------------------
//
CTurnUdpSettings::CTurnUdpSettings( TUint aIapId )
    : CGeneralSettings( aIapId )
    {
    }
    
    
// -----------------------------------------------------------------------------
// CTurnUdpSettings::~CTurnUdpSettings
// -----------------------------------------------------------------------------
//   
CTurnUdpSettings::~CTurnUdpSettings( )
    {
    EraseL( KUNSAFProtocolsDomainTableMask, 0xf0000000 );
 	EraseL( KUNSAFProtocolsIAPTableMask, 0xf0000000 );
    }
    
    
// -----------------------------------------------------------------------------
// CTurnUdpSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CTurnUdpSettings::ConstructL( const TDesC8& aDomain )
    {
    CGeneralSettings::ConstructL( aDomain );
    }
    
    
// -----------------------------------------------------------------------------
// CTurnUdpSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CTurnUdpSettings::CreateSettingsL()
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
