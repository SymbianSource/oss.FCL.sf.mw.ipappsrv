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




#include "icetcpsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

/******************************************************/
//Settings
//Domain spesific settings
_LIT8( KNatProtocol, "nokia.ice" );

// -----------------------------------------------------------------------------
// CICETCPSettings::NewL
// -----------------------------------------------------------------------------
// 
CICETCPSettings* CICETCPSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CICETCPSettings* self = CICETCPSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CICETCPSettings::NewL
// -----------------------------------------------------------------------------
//
CICETCPSettings* CICETCPSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CICETCPSettings* self = new( ELeave ) CICETCPSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    self->ConstructBaseL();
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CICETCPSettings::CICETCPSettings
// -----------------------------------------------------------------------------
//
CICETCPSettings::CICETCPSettings( TUint aIapId )
    : CGeneralSettings( aIapId )
    {
    }
    
    
// -----------------------------------------------------------------------------
// CICETCPSettings::~CICETCPSettings
// -----------------------------------------------------------------------------
//   
CICETCPSettings::~CICETCPSettings( )
    {
 	iRepository->Delete( iDomainKey | KUNSAFProtocolsUsedNATProtocolMask );
 	//iRepository->Delete( iDomainKey | KUNSAFProtocolsRequestedTransportMask );
    }
    
    
// -----------------------------------------------------------------------------
// CICETCPSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CICETCPSettings::ConstructL( const TDesC8& aDomain )
    {
    CGeneralSettings::ConstructL( aDomain );
    }
    
    
// -----------------------------------------------------------------------------
// CICETCPSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CICETCPSettings::CreateSettingsL()
    {
    User::LeaveIfError( GetDomainKey( iDomainKey ) );
   
    //Settings
    /************************************************************************/
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | iDomainKey, 
        KNatProtocol );
    /*    
    StoreL( KUNSAFProtocolsRequestedTransportMask | iDomainKey, 
        KRequestedTransport );
    */
    }
