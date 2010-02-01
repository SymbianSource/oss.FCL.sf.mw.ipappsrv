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
#include "natsettingslogs.h"
#include "cnatfwstunsettings.h"
#include "cnatfwserversettings.h" 
#include "cnatfwcenrephandler.h"
#include "tnatfwsettingsparser.h"
#include "natfwdefaultsettingsvalues.h"

const TUint KDefaultGranularity = 2;


// ======== MEMBER FUNCTIONS ========

CNATFWStunSettings::CNATFWStunSettings()
    {

    }


void CNATFWStunSettings::ConstructL( )
    {
    iServerArray = new ( ELeave ) CArrayPtrFlat<CNATFWServerSettings>(
        KDefaultGranularity );
    iLatestConnectedServerAddr = KNullDesC8().AllocL();
    }


CNATFWStunSettings* CNATFWStunSettings::NewL()
    {
    CNATFWStunSettings* self = CNATFWStunSettings::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


CNATFWStunSettings* CNATFWStunSettings::NewLC()
    {
    CNATFWStunSettings* self = new( ELeave ) CNATFWStunSettings;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CNATFWStunSettings::~CNATFWStunSettings()
    {
    if ( iServerArray )
        {
        iServerArray->ResetAndDestroy();
        }
    delete iServerArray;
    delete iLatestConnectedServerAddr;
    }


// ---------------------------------------------------------------------------
// CNATFWStunSettings::SetStunServersL
// ---------------------------------------------------------------------------
//
void CNATFWStunSettings::SetStunServersL(
    CArrayPtr<CNATFWServerSettings>* aServers )
    {
    __ASSERT_ALWAYS( NULL != aServers, User::Leave( KErrArgument ) );
    
    iServerArray->ResetAndDestroy();
    delete iServerArray;
    iServerArray = NULL;
    iServerArray = aServers;
    }


// ---------------------------------------------------------------------------
// CNATFWStunSettings::SetLatestConnectedServerAddrL
// ---------------------------------------------------------------------------
//
void CNATFWStunSettings::SetLatestConnectedServerAddrL(
    const TDesC8& aServerAddr )
    {
    delete iLatestConnectedServerAddr;
    iLatestConnectedServerAddr = NULL;
    iLatestConnectedServerAddr = aServerAddr.AllocL();
    }
    

// ---------------------------------------------------------------------------
// CNATFWStunSettings::SetLatestConnectedServerPort
// ---------------------------------------------------------------------------
//
void CNATFWStunSettings::SetLatestConnectedServerPort( TInt aServerPort )
    {
    iLatestConnectedServerPort = aServerPort;
    }


// ---------------------------------------------------------------------------
// CNATFWStunSettings::SetRetransmissionTimeout
// ---------------------------------------------------------------------------
//
void CNATFWStunSettings::SetRetransmissionTimeout( TUint aRto )
    {
    iRto = aRto;
    }


// ---------------------------------------------------------------------------
// CNATFWStunSettings::ReadSettingsL
// ---------------------------------------------------------------------------
//  
void CNATFWStunSettings::ReadSettingsL( TUint32 aDomainKey,
    const CNATFWCenRepHandler& aRepHandler )
    {
    __NATSETTINGS( "CNATFWStunSettings::ReadSettingsL" )
     
    RArray<TUint32> serverKeys;
    CleanupClosePushL( serverKeys );
    
    TBool sharedSecretNotEnabled;
    TBool sharedSecretEnabled;
    
    // read sharedsecret enabled
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsDomainSharedSecretNotSupported |
        aDomainKey, sharedSecretNotEnabled ) )
        {
        sharedSecretEnabled = KNATFWDefaultUseSharedSecret;
        }
    else
        {
        sharedSecretEnabled = !sharedSecretNotEnabled;
        }
        
    // read server settings
    TInt err = aRepHandler.FindServerKeys(
        KUNSAFProtocolsSTUNServerTableMask | aDomainKey, serverKeys );
        
    TInt serverCount = serverKeys.Count();
    
    if ( KErrNone == err && serverCount > 0 )
        {
        for( TInt i( 0 ); i < serverCount; i++ )
            {
            CNATFWServerSettings* serverSettings = CNATFWServerSettings::NewLC(
                KNullDesC8,
                KNATFWDefaultSTUNServerPort,
                KNullDesC8,
                KNullDesC8,
                sharedSecretEnabled );

            TUint32 key = serverKeys[i] | aDomainKey;
            serverSettings->ReadSettingsL( key, aRepHandler );
            iServerArray->AppendL( serverSettings );
            CleanupStack::Pop( serverSettings );
            }
        }
    else
        {
        if ( KErrNotFound == err )
            {
             __NATSETTINGS( "Server settings not found" )
            }
        else
            {
            User::Leave( err );
            }
        }
    CleanupStack::PopAndDestroy( &serverKeys );
    
    // read LatestConnectedStunServerAddress and possible port
    TInt port = 0;
    HBufC8* tempBufPointer = NULL;
    
    tempBufPointer = aRepHandler.ReadL(
        KUNSAFProtocolsLatestConnectedSTUNServerAddressMask | aDomainKey );
    if ( tempBufPointer ) 
        {
        CleanupStack::PushL( tempBufPointer );
        HBufC8* addr = TNATFWSettingsParser::ParseAddressLC(
            *tempBufPointer, port );
        SetLatestConnectedServerAddrL( *addr );
        SetLatestConnectedServerPort( port );
        CleanupStack::PopAndDestroy( addr );
        CleanupStack::PopAndDestroy( tempBufPointer );
        }
    else
        {
        SetLatestConnectedServerAddrL( KNullDesC8 );
        SetLatestConnectedServerPort( port );
        }
    
    // Read LatestConnectedStunServerPort, replaces previous data if found
    if ( KErrNone == aRepHandler.Read( KUNSAFProtocolsLatestConnectedSTUNServerPortMask |
        aDomainKey, port ) )
        {
        SetLatestConnectedServerPort( port );
        }
    
    // read retransmission timeout
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsStunRetransmissionTimeoutMask |
        aDomainKey, iRto ) )
        {
        iRto = KNATFWDefaultRtoValue;
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWStunSettings.
// CNATFWStunSettings::GetStunServerArrayL
// ---------------------------------------------------------------------------
//
void CNATFWStunSettings::GetStunServerArrayL(
    RPointerArray<MNATFWServerSettings>& aServers ) const
    {
    TInt count = iServerArray->Count();
    
    for ( TInt i( 0 ); i < count; i++ )
        {
        aServers.AppendL( iServerArray->At( i ) );
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWStunSettings.
// CNATFWStunSettings::RetransmissionTimeout
// ---------------------------------------------------------------------------
//
TUint CNATFWStunSettings::RetransmissionTimeout() const
    {
    return iRto;
    }
 
    
// ---------------------------------------------------------------------------
// From class MNATFWStunSettings.
// CNATFWStunSettings::LatestConnectedServerAddr
// ---------------------------------------------------------------------------
//
 const TDesC8& CNATFWStunSettings::LatestConnectedServerAddr() const
    {
    return *iLatestConnectedServerAddr;
    }


// ---------------------------------------------------------------------------
// From class MNATFWStunSettings.
// CNATFWStunSettings::LatestConnectedServerPort
// ---------------------------------------------------------------------------
//
TInt CNATFWStunSettings::LatestConnectedServerPort() const
    {
    return iLatestConnectedServerPort;
    }
