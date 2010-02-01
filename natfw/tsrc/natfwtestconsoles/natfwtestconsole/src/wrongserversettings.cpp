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




#include "wrongserversettings.h"

#include <e32debug.h>
#include <unsafprotocolscrkeys.h>
#include <cnatfwsettingsapi.h>
#include <centralrepository.h>

//  CONSTANTS

//Settings
//Domain spesific settings
_LIT8( KWrongAddress, "nokia.stun.com:1" );


// -----------------------------------------------------------------------------
// CWrongServerSettings::NewL
// -----------------------------------------------------------------------------
// 
CWrongServerSettings* CWrongServerSettings::NewL(
    const TDesC8& aDomain )
    {
    CWrongServerSettings* self =
        CWrongServerSettings::NewLC( aDomain );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CWrongServerSettings::NewL
// -----------------------------------------------------------------------------
//
CWrongServerSettings* CWrongServerSettings::NewLC(
    const TDesC8& aDomain )
    {   
    CWrongServerSettings* self = new( ELeave ) CWrongServerSettings( );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
// CWrongServerSettings::CWrongServerSettings
// -----------------------------------------------------------------------------
//
CWrongServerSettings::CWrongServerSettings()
    {
    }
    
    
// -----------------------------------------------------------------------------
// CWrongServerSettings::~CWrongServerSettings
// -----------------------------------------------------------------------------
//   
CWrongServerSettings::~CWrongServerSettings( )
    {
    // delete settings
    EraseL(  iStunServerAddressKey, 0xffffffff );
    EraseL(  iTurnServerAddressKey, 0xffffffff );
    
    // Restore old server settings
    
    // STUN
    if ( iStoredStunAddress )
        {
        StoreL( iStunServerAddressKey, *iStoredStunAddress );
        delete iStoredStunAddress;
        }
    //TURN    
    if ( iStoredTurnAddress )
        {
        StoreL( iTurnServerAddressKey, *iStoredTurnAddress );
        delete iStoredTurnAddress;
        }
        
 	delete iDomain;
    }
    
    
// -----------------------------------------------------------------------------
// CWrongServerSettings::ConstructL
// -----------------------------------------------------------------------------
//    
void CWrongServerSettings::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    ConstructBaseL();
    CreateSettingsL();
    }
    
    
// -----------------------------------------------------------------------------
// CWrongServerSettings::CreateSettingsL
// -----------------------------------------------------------------------------
//   
void CWrongServerSettings::CreateSettingsL()
    {
    RDebug::Print( _L( "\nTEST PRINT: CWrongServerSettings::CreateSettingsL - start" ) );
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
    // read server settings keys 
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols );
    RArray<TUint32> array;
    CleanupClosePushL( array );
    
    // stun server  
    if ( !iStunServerAddressKey )
        {
        // readStunserverKey
        err = rep->FindL( iDomainKey | KUNSAFProtocolsSTUNAddressMask,
            KUNSAFProtocolsSubTableFieldTypeMask, array );
        if ( !err )
            {
            iStunServerAddressKey = array[0];
            // store old address
            ReadL( iStunServerAddressKey, &iStoredStunAddress );
            }
        else
            {
            //Create key for turn server settings
            iStunServerAddressKey = CreateNewTableKeyL(
                KUNSAFProtocolsSTUNServerTableMask | iDomainKey,
                KUNSAFProtocolsSubTableFieldTypeMask );
                
            iStunServerAddressKey |=
                KUNSAFProtocolsSTUNServerTableMask | iDomainKey;
            }
        array.Reset();
        }
    
    // turn server
    if ( !iTurnServerAddressKey )
        {
        // readTurnServerKey
        err = rep->FindL( iDomainKey | KUNSAFProtocolsTURNAddressMask,
            KUNSAFProtocolsSubTableFieldTypeMask, array );
        if ( !err )
            {
            iTurnServerAddressKey = array[0];
            // store old address
            ReadL( iTurnServerAddressKey, &iStoredTurnAddress );
            }
        else
            {
            //Create key for turn server settings
            iTurnServerAddressKey = CreateNewTableKeyL(
                KUNSAFProtocolsTURNServerTableMask | iDomainKey,
                KUNSAFProtocolsSubTableFieldTypeMask );
                
            iTurnServerAddressKey |=
                KUNSAFProtocolsTURNServerTableMask | iDomainKey;
            }
        array.Reset();
        }
    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( rep );
    
    // store wrong settings
    // STUN
    StoreL( iStunServerAddressKey, KWrongAddress );
    // TURN
    StoreL( iTurnServerAddressKey, KWrongAddress );
    
    RDebug::Print( _L( "\nTEST PRINT: CWrongServerSettings::CreateSettingsL - end" ) );
    }
