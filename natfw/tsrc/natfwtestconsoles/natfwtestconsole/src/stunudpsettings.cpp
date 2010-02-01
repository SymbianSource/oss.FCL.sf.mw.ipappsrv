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




#include "stunudpsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

//Domain spesific settings
_LIT8( KNatProtocol, "nokia.stun" );


// -----------------------------------------------------------------------------
// CStunUdpSettings::NewL
// -----------------------------------------------------------------------------
// 
CStunUdpSettings* CStunUdpSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CStunUdpSettings* self = CStunUdpSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CStunUdpSettings::NewL
// -----------------------------------------------------------------------------
//
CStunUdpSettings* CStunUdpSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CStunUdpSettings* self = new( ELeave ) CStunUdpSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CStunUdpSettings::CStunUdpSettings
// -----------------------------------------------------------------------------
//
CStunUdpSettings::CStunUdpSettings( TUint aIapId )
    : CGeneralSettings( aIapId )
    {
    }
    
    
// -----------------------------------------------------------------------------
// CStunUdpSettings::~CStunUdpSettings
// -----------------------------------------------------------------------------
//   
CStunUdpSettings::~CStunUdpSettings( )
    {
    /*
    EraseL( KUNSAFProtocolsDomainTableMask, 0xf0000000 );
 	EraseL( KUNSAFProtocolsIAPTableMask, 0xf0000000 );
    */
    }
    
    
// -----------------------------------------------------------------------------
// CStunUdpSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CStunUdpSettings::ConstructL( const TDesC8& aDomain )
    {
    CGeneralSettings::ConstructL( aDomain );
    }
    
    
// -----------------------------------------------------------------------------
// CStunUdpSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CStunUdpSettings::CreateSettingsL()
    {
    CGeneralSettings::CreateSettingsL();
    
    User::LeaveIfError( GetDomainKey( iDomainKey ) );
    
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | iDomainKey, 
        KNatProtocol );
    }
