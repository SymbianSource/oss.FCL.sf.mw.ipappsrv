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




#include "additionalturnudpsettings.h"

#include <unsafprotocolscrkeys.h>


//  CONSTANTS

/******************************************************/
//Settings
//Domain spesific settings
_LIT8( KPriorityOrder, "nokia.turn" );


// ---------------------------------------------------------------------------
// CAdditionalTurnUdpSettings::NewL
// ---------------------------------------------------------------------------
// 
CAdditionalTurnUdpSettings* CAdditionalTurnUdpSettings::NewL(
    const TDesC8& aDomain )
    {
    CAdditionalTurnUdpSettings* self =
        CAdditionalTurnUdpSettings::NewLC( aDomain );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CAdditionalTurnUdpSettings::NewL
// ---------------------------------------------------------------------------
//
CAdditionalTurnUdpSettings* CAdditionalTurnUdpSettings::NewLC(
    const TDesC8& aDomain )
    {   
    CAdditionalTurnUdpSettings* self = new( ELeave )
        CAdditionalTurnUdpSettings();
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CAdditionalTurnUdpSettings::CAdditionalTurnUdpSettings
// ---------------------------------------------------------------------------
//
CAdditionalTurnUdpSettings::CAdditionalTurnUdpSettings()
    {
    }
    
    
// ---------------------------------------------------------------------------
// CAdditionalTurnUdpSettings::~CAdditionalTurnUdpSettings
// ---------------------------------------------------------------------------
//   
CAdditionalTurnUdpSettings::~CAdditionalTurnUdpSettings( )
    {
    /*
    iRepository->Delete( iDomainKey | KUNSAFProtocolsPlugInIdMask );
    iRepository->Delete( iDomainKey | KUNSAFProtocolsPlugInServiceIdMask );
    */
    delete iDomain; 
    }
    
    
// ---------------------------------------------------------------------------
// CAdditionalTurnUdpSettings::ConstructL
// ---------------------------------------------------------------------------
//    
void CAdditionalTurnUdpSettings::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    ConstructBaseL();
    }
    
    
// ---------------------------------------------------------------------------
// CAdditionalTurnUdpSettings::CreateSettingsL
// ---------------------------------------------------------------------------
//   
void CAdditionalTurnUdpSettings::CreateSettingsL()
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
    StoreL( KUNSAFProtocolsUsedNATProtocolMask | iDomainKey, KPriorityOrder );
    }
