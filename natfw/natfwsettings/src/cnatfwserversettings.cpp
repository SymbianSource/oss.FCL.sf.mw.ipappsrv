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




#include <unsafprotocolscrkeys.h>
#include "cnatfwserversettings.h"
#include "natsettingslogs.h"
#include "cnatfwcenrephandler.h"
#include "tnatfwsettingsparser.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWServerSettings::CNATFWServerSettings
// ---------------------------------------------------------------------------
//
CNATFWServerSettings::CNATFWServerSettings( TUint aPort,
    const TBool aSharedSecretEnabled ) :
    iPort( aPort), iSharedSecretEnabled( aSharedSecretEnabled )
    {
    }


// ---------------------------------------------------------------------------
// CNATFWServerSettings::ConstructL
// ---------------------------------------------------------------------------
//    
void CNATFWServerSettings::ConstructL( const TDesC8& aAddress,
    const TDesC8& aUsername, const TDesC8& aPassword )
    {
    __NATSETTINGS( "CNATFWServerSettings::ConstructL" )
    iAddress = aAddress.AllocL();
    iUsername = aUsername.AllocL();
    iPassword = aPassword.AllocL();
    }


// ---------------------------------------------------------------------------
// CNATFWServerSettings::NewL
// ---------------------------------------------------------------------------
//
CNATFWServerSettings* CNATFWServerSettings::NewL(
    const TDesC8& aAddress,
    TUint aPort,
    const TDesC8& aUsername,
    const TDesC8& aPassword,
    const TBool aSharedSecretEnabled )
    {
    __NATSETTINGS( "CNATFWServerSettings::NewL" )
    CNATFWServerSettings* self = CNATFWServerSettings::NewLC( aAddress, aPort,
        aUsername, aPassword, aSharedSecretEnabled );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWServerSettings::NewLC
// ---------------------------------------------------------------------------
//
CNATFWServerSettings* CNATFWServerSettings::NewLC(
    const TDesC8& aAddress,
    TUint aPort, 
    const TDesC8& aUsername,
    const TDesC8& aPassword,
    const TBool aSharedSecretEnabled )
    {
    __NATSETTINGS( "CNATFWServerSettings::NewLC" )
    CNATFWServerSettings* self = new( ELeave )CNATFWServerSettings(
        aPort, aSharedSecretEnabled );
    CleanupStack::PushL( self );
    self->ConstructL( aAddress, aUsername, aPassword );
    return self;    
    }


// ---------------------------------------------------------------------------
// CNATFWServerSettings::~CNATFWServerSettings
// ---------------------------------------------------------------------------
//
CNATFWServerSettings::~CNATFWServerSettings()
    {
    delete iAddress;
    delete iUsername;
    delete iPassword;
    }


// ---------------------------------------------------------------------------
// CNATFWServerSettings::ReadSettingsL
// ---------------------------------------------------------------------------
//
void CNATFWServerSettings::ReadSettingsL( TUint32 aServerSettingsKey,
    const CNATFWCenRepHandler& aRepHandler )
    {
    __NATSETTINGS( "CNATFWServerSettings::ReadSettingsL - start" )
    
    const TUint32 KServerAddressMask = 
        KUNSAFProtocolsSTUNAddressMask & ( ~KUNSAFProtocolsSubKeyMask );

    const TUint32 KServerUsernameMask = 
        KUNSAFProtocolsSTUNUsernameMask & ( ~KUNSAFProtocolsSubKeyMask );
        
    const TUint32 KServerPasswordMask = 
        KUNSAFProtocolsSTUNPasswordMask & ( ~KUNSAFProtocolsSubKeyMask );
        
    const TUint32 KServerPortMask = 
        KUNSAFProtocolsSTUNPortMask & ( ~KUNSAFProtocolsSubKeyMask );

    HBufC8* tempBuffer = NULL;
    
    // read port
    aRepHandler.Read( KServerPortMask | aServerSettingsKey, iPort );
    
    // read address and possible port, port is replaced if found
    tempBuffer = aRepHandler.ReadL(
        KServerAddressMask | aServerSettingsKey );
    if ( tempBuffer )
        {
        TInt port( 0 );
        CleanupStack::PushL( tempBuffer );
        delete iAddress;
        iAddress = NULL;
        iAddress = TNATFWSettingsParser::ParseAddressL( *tempBuffer, port );
        if ( port )
            {
            iPort = port;
            }
        CleanupStack::PopAndDestroy( tempBuffer );
        tempBuffer = NULL;
        }
    
    // read username
    tempBuffer = aRepHandler.ReadL( KServerUsernameMask |
        aServerSettingsKey );
    if ( tempBuffer )
        {
        delete iUsername;
        iUsername = tempBuffer;
        tempBuffer = NULL;
        }
    
    // read password
    tempBuffer = aRepHandler.ReadL( KServerPasswordMask |
        aServerSettingsKey );
    if ( tempBuffer )
        {
        delete iPassword;
        iPassword = tempBuffer;
        tempBuffer = NULL;
        }

    __NATSETTINGS( "CNATFWServerSettings::ReadSettingsL - end" )
    }

    
// ---------------------------------------------------------------------------
// From MNATFWServerSettings
// CNATFWServerSettings::Address
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWServerSettings::Address() const
    {
    return *iAddress;
    }


// ---------------------------------------------------------------------------
// From MNATFWServerSettings
// CNATFWServerSettings::Port
// ---------------------------------------------------------------------------
//  
TUint CNATFWServerSettings::Port() const
    {
    return iPort;
    }


// ---------------------------------------------------------------------------
// From MNATFWServerSettings
// CNATFWServerSettings::Username
// ---------------------------------------------------------------------------
//  
const TDesC8& CNATFWServerSettings::Username() const
    {
    return *iUsername;
    }


// ---------------------------------------------------------------------------
// From MNATFWServerSettings
// CNATFWServerSettings::Password
// ---------------------------------------------------------------------------
//  
const TDesC8& CNATFWServerSettings::Password() const
    {
    return *iPassword;
    }


// ---------------------------------------------------------------------------
// From MNATFWServerSettings
// CNATFWServerSettings::SharedSecretEnabled
// ---------------------------------------------------------------------------
//      
TBool CNATFWServerSettings::SharedSecretEnabled() const
    {
    return iSharedSecretEnabled;
    }
