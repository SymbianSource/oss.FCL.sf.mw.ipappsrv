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




#include "additionalicesettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

/******************************************************/
//Settings
//Domain spesific settings
_LIT8( KPriorityOrder, "nokia.ice" );


// ---------------------------------------------------------------------------
// CAdditionalIceSettings::NewL
// ---------------------------------------------------------------------------
// 
CAdditionalIceSettings* CAdditionalIceSettings::NewL( const TDesC8& aDomain )
    {
    CAdditionalIceSettings* self =
        CAdditionalIceSettings::NewLC( aDomain );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CAdditionalIceSettings::NewL
// ---------------------------------------------------------------------------
//
CAdditionalIceSettings* CAdditionalIceSettings::NewLC( const TDesC8& aDomain )
    {   
    CAdditionalIceSettings* self =
        new( ELeave ) CAdditionalIceSettings();
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CAdditionalIceSettings::CAdditionalIceSettings
// ---------------------------------------------------------------------------
//
CAdditionalIceSettings::CAdditionalIceSettings()
    {
    }
    
    
// ---------------------------------------------------------------------------
// CAdditionalIceSettings::~CAdditionalIceSettings
// ---------------------------------------------------------------------------
//   
CAdditionalIceSettings::~CAdditionalIceSettings( )
    {
    /*
    iRepository->Delete( iDomainKey | KUNSAFProtocolsPlugInIdMask );
    iRepository->Delete( iDomainKey | KUNSAFProtocolsPlugInServiceIdMask );
    */
    delete iDomain;
    }
    
    
// ---------------------------------------------------------------------------
// CAdditionalIceSettings::ConstructL
// ---------------------------------------------------------------------------
//    
void CAdditionalIceSettings::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.Alloc();
    ConstructBaseL();
    }
    
    
// ---------------------------------------------------------------------------
// CAdditionalIceSettings::CreateSettingsL
// ---------------------------------------------------------------------------
//   
void CAdditionalIceSettings::CreateSettingsL()
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
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | iDomainKey, 
        KPriorityOrder );
    }
