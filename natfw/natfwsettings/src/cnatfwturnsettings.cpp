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




#include <badesca.h>
#include <unsafprotocolscrkeys.h>
#include "cnatfwturnsettings.h"
#include "cnatfwserversettings.h" 
#include <mnatfwserversettings.h>
#include "cnatfwcenrephandler.h"
#include "tnatfwsettingsparser.h"
#include "natfwdefaultsettingsvalues.h"
#include "natsettingslogs.h"

const TUint KDefaultGranularity = 2;

// ======== MEMBER FUNCTIONS ========

CNATFWTurnSettings::CNATFWTurnSettings()
    {

    }


void CNATFWTurnSettings::ConstructL( )
    {
    iServerArray = new ( ELeave )
        CArrayPtrFlat<CNATFWServerSettings>( KDefaultGranularity );
    iLatestConnectedServerAddr = KNullDesC8().AllocL();
    }


CNATFWTurnSettings* CNATFWTurnSettings::NewL()
    {
    CNATFWTurnSettings* self = CNATFWTurnSettings::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


CNATFWTurnSettings* CNATFWTurnSettings::NewLC()
    {
    CNATFWTurnSettings* self = new( ELeave ) CNATFWTurnSettings;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CNATFWTurnSettings::~CNATFWTurnSettings()
    {
    if ( iServerArray )
        {
        iServerArray->ResetAndDestroy();
        }
    delete iServerArray;
    delete iLatestConnectedServerAddr;
    }


// ---------------------------------------------------------------------------
// CNATFWTurnSettings::SetTurnServersL
// ---------------------------------------------------------------------------
//
void CNATFWTurnSettings::SetTurnServersL(
    CArrayPtr<CNATFWServerSettings>* aServers )
    {
    __ASSERT_ALWAYS( NULL != aServers, User::Leave( KErrArgument ) );
    iServerArray->ResetAndDestroy();
    delete iServerArray;
    iServerArray = NULL;
    iServerArray = aServers;
    }


// ---------------------------------------------------------------------------
// CNATFWTurnSettings::SetLatestConnectedServerAddrL
// ---------------------------------------------------------------------------
//
void CNATFWTurnSettings::SetLatestConnectedServerAddrL
    ( const TDesC8& aServerAddr )
    {
    delete iLatestConnectedServerAddr;
    iLatestConnectedServerAddr = NULL;
    iLatestConnectedServerAddr = aServerAddr.AllocL();
    }


// ---------------------------------------------------------------------------
// CNATFWTurnSettings::SetLatestConnectedServerPort
// ---------------------------------------------------------------------------
//
void CNATFWTurnSettings::SetLatestConnectedServerPort( TInt aServerPort )
    {
    iLatestConnectedServerPort = aServerPort;
    }


// ---------------------------------------------------------------------------
// CNATFWTurnSettings::SetRetransmissionTimeout
// ---------------------------------------------------------------------------
//
void CNATFWTurnSettings::SetRetransmissionTimeout( TUint aRto )
    {
    iRto = aRto;
    }
    

// ---------------------------------------------------------------------------
// CNATFWTurnSettings::ReadSettingsL
// ---------------------------------------------------------------------------
//  
void CNATFWTurnSettings::ReadSettingsL( TUint32 aDomainKey,
    const CNATFWCenRepHandler& aRepHandler )
    {
    __NATSETTINGS( "CNATFWTurnSettings::ReadSettingsL" )
    
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
        KUNSAFProtocolsTURNServerTableMask | aDomainKey, serverKeys );
        
    TInt serverCount = serverKeys.Count();
    
    if ( KErrNone == err && serverCount > 0 )
        {
        for( TInt i( 0 ); i < serverCount; i++ )
            {
            CNATFWServerSettings* serverSettings = CNATFWServerSettings::NewLC(
                KNullDesC8,
                KNATFWDefaultTURNServerPort,
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
    
    // read LatestConnectedTurnServerAddress and possible port
    TInt port = 0;

    HBufC8* tempBufPointer = aRepHandler.ReadL(
        KUNSAFProtocolsLatestConnectedTURNServerAddressMask | aDomainKey );
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
    if ( KErrNone == aRepHandler.Read( KUNSAFProtocolsLatestConnectedTURNServerPortMask |
        aDomainKey, port ) )
        {
        SetLatestConnectedServerPort( port );
        }

    // read retransmission timeout
    if ( KErrNone != aRepHandler.Read( KUNSAFProtocolsTurnRetransmissionTimeoutMask |
        aDomainKey, iRto ) )
        {
        iRto = KNATFWDefaultRtoValue;
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWTurnSettings.
// CNATFWTurnSettings::GetTurnServerArrayL
// ---------------------------------------------------------------------------
//
void CNATFWTurnSettings::GetTurnServerArrayL(
    RPointerArray<MNATFWServerSettings>& aServers ) const
    {
    TInt count = iServerArray->Count();
    
    for ( TInt i( 0 ); i < count; i++ )
        {
        aServers.AppendL( iServerArray->At( i ) );
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWTurnSettings.
// CNATFWServerSettings::RetransmissionTimeout
// ---------------------------------------------------------------------------
//
TUint CNATFWTurnSettings::RetransmissionTimeout() const
    {
    return iRto;
    }


// ---------------------------------------------------------------------------
// From class MNATFWTurnSettings.
// CNATFWTurnSettings::LatestConnectedServerAddr
// ---------------------------------------------------------------------------
//
 const TDesC8& CNATFWTurnSettings::LatestConnectedServerAddr() const
    {
    return *iLatestConnectedServerAddr;
    }


// ---------------------------------------------------------------------------
// From class MNATFWTurnSettings.
// CNATFWTurnSettings::LatestConnectedServerPort
// ---------------------------------------------------------------------------
//
TInt CNATFWTurnSettings::LatestConnectedServerPort() const
    {
    return iLatestConnectedServerPort;
    }
