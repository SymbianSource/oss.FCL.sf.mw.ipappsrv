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




#include "iceudpsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

/******************************************************/
//Settings
//Domain spesific settings
_LIT8( KNatProtocol, "nokia.ice" );

// -----------------------------------------------------------------------------
// CICEUDPSettings::NewL
// -----------------------------------------------------------------------------
// 
CICEUDPSettings* CICEUDPSettings::NewL( const TDesC8& aDomain,
                                                 TUint aIapId )
    {
    CICEUDPSettings* self = CICEUDPSettings::NewLC(aDomain, aIapId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CICEUDPSettings::NewL
// -----------------------------------------------------------------------------
//
CICEUDPSettings* CICEUDPSettings::NewLC( const TDesC8& aDomain,
                                                  TUint aIapId )
    {   
    CICEUDPSettings* self = new( ELeave ) CICEUDPSettings( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CICEUDPSettings::CICEUDPSettings
// -----------------------------------------------------------------------------
//
CICEUDPSettings::CICEUDPSettings( TUint aIapId )
    : CGeneralSettings( aIapId )
    {
    }
    
    
// -----------------------------------------------------------------------------
// CICEUDPSettings::~CICEUDPSettings
// -----------------------------------------------------------------------------
//   
CICEUDPSettings::~CICEUDPSettings( )
    {
 	iRepository->Delete( iDomainKey | KUNSAFProtocolsUsedNATProtocolMask );
 	//iRepository->Delete( iDomainKey | KUNSAFProtocolsRequestedTransportMask );
    }
    
    
// -----------------------------------------------------------------------------
// CICEUDPSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CICEUDPSettings::ConstructL( const TDesC8& aDomain )
    {
    CGeneralSettings::ConstructL( aDomain );
    }
    
    
// -----------------------------------------------------------------------------
// CICEUDPSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CICEUDPSettings::CreateSettingsL()
    {
    CGeneralSettings::CreateSettingsL();
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
