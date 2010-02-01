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




#include "additionalstunudpsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

//Settings
//Domain spesific settings
_LIT8( KPriorityOrder, "nokia.stun" );

// -----------------------------------------------------------------------------
// CAdditionalStunUdpSettings::NewL
// -----------------------------------------------------------------------------
// 
CAdditionalStunUdpSettings* CAdditionalStunUdpSettings::NewL(
    const TDesC8& aDomain )
    {
    CAdditionalStunUdpSettings* self =
        CAdditionalStunUdpSettings::NewLC( aDomain );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CAdditionalStunUdpSettings::NewL
// -----------------------------------------------------------------------------
//
CAdditionalStunUdpSettings* CAdditionalStunUdpSettings::NewLC(
    const TDesC8& aDomain )
    {   
    CAdditionalStunUdpSettings* self = new( ELeave ) CAdditionalStunUdpSettings( );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CAdditionalStunUdpSettings::CAdditionalStunUdpSettings
// -----------------------------------------------------------------------------
//
CAdditionalStunUdpSettings::CAdditionalStunUdpSettings()
    {
    }
    
    
// -----------------------------------------------------------------------------
// CAdditionalStunUdpSettings::~CAdditionalStunUdpSettings
// -----------------------------------------------------------------------------
//   
CAdditionalStunUdpSettings::~CAdditionalStunUdpSettings( )
    {
    /*
    iRepository->Delete( iDomainKey | KUNSAFProtocolsPlugInIdMask );
    iRepository->Delete( iDomainKey | KUNSAFProtocolsPlugInServiceIdMask );
    */
 	delete iDomain;
    }
    
    
// -----------------------------------------------------------------------------
// CAdditionalStunUdpSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CAdditionalStunUdpSettings::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    ConstructBaseL();
    }
    
    
// -----------------------------------------------------------------------------
// CAdditionalStunUdpSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CAdditionalStunUdpSettings::CreateSettingsL()
    {
    TInt err =  ReadDomainSettingsKeyL( *iDomain, iDomainKey );
    if ( KErrNotFound == err ) 
        {
        //Create key for domain settings
        RDebug::Print( _L("TEST PRINT: CAdditionalStunSettings::CreateSettingsL: Create new domain key\n") );
        iDomainKey = CreateNewTableKeyL(
        KUNSAFProtocolsDomainTableMask, KUNSAFProtocolsFieldTypeMask );
        StoreL( KUNSAFProtocolsDomainMask | iDomainKey, *iDomain );
        }
    else if ( KErrNone != err )
        {
        User::Leave( err );
        }
    else
        {            
        }
    
    //Domain spesific settings
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | iDomainKey, 
        KPriorityOrder );
    }
