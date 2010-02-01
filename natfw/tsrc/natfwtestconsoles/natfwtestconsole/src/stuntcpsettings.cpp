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




#include "stuntcpsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

//Domain spesific settings
_LIT8( KNatProtocol, "nokia.stun" );


// -----------------------------------------------------------------------------
// CStunTcpSettings::NewL
// -----------------------------------------------------------------------------
// 
CStunTcpSettings* CStunTcpSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CStunTcpSettings* self = CStunTcpSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CStunTcpSettings::NewL
// -----------------------------------------------------------------------------
//
CStunTcpSettings* CStunTcpSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CStunTcpSettings* self = new( ELeave ) CStunTcpSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CStunTcpSettings::CStunTcpSettings
// -----------------------------------------------------------------------------
//
CStunTcpSettings::CStunTcpSettings( TUint aIapId )
    : CGeneralSettings( aIapId )
    {
    }
    
    
// -----------------------------------------------------------------------------
// CStunTcpSettings::~CStunTcpSettings
// -----------------------------------------------------------------------------
//   
CStunTcpSettings::~CStunTcpSettings( )
    {
    /*
    EraseL( KUNSAFProtocolsDomainTableMask, 0xf0000000 );
 	EraseL( KUNSAFProtocolsIAPTableMask, 0xf0000000 );
    */
    }
    
    
// -----------------------------------------------------------------------------
// CStunTcpSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CStunTcpSettings::ConstructL( const TDesC8& aDomain )
    {
    CGeneralSettings::ConstructL( aDomain );
    }
    
    
// -----------------------------------------------------------------------------
// CStunTcpSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CStunTcpSettings::CreateSettingsL()
    {
    CGeneralSettings::CreateSettingsL();
    
    User::LeaveIfError( GetDomainKey( iDomainKey ) );
    
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | iDomainKey, 
        KNatProtocol );
    }
    
