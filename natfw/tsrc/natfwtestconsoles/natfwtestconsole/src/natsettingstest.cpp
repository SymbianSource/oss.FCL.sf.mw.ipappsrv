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




#include <e32cons.h>
#include <e32debug.h>
#include <badesca.h>

#include "natsettingstest.h"
#include <cnatfwsettingsapi.h>
#include <mnatfwserversettings.h>
#include <mnatfwstunsettings.h>
#include <mnatfwturnsettings.h>
#include "natfwdefaultsettingsvalues.h"


// ---------------------------------------------------------------------------
// CNATSettingsTest::NewL
// ---------------------------------------------------------------------------
//
CNATSettingsTest* CNATSettingsTest::NewL( const TDesC8& aDomain, TUint aIapId )
    {
    CNATSettingsTest* self = new ( ELeave ) CNATSettingsTest( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATSettingsTest::CNATSettingsTest
// ---------------------------------------------------------------------------
//
CNATSettingsTest::CNATSettingsTest( TUint aIapId ) :
    iIapId( aIapId )
    {
    
    }


// ---------------------------------------------------------------------------
// CNATSettingsTest::ConstructL
// ---------------------------------------------------------------------------
//
void CNATSettingsTest::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    iSettings = CNATFWNatSettingsApi::NewL( *iDomain );
    }


// ---------------------------------------------------------------------------
// CNATSettingsTest::~CNATSettingsTest
// ---------------------------------------------------------------------------
//    
CNATSettingsTest::~CNATSettingsTest()
    {
    delete iSettings;
    delete iDomain;
    }


// ---------------------------------------------------------------------------
// CNATSettingsTest::PrintSettings
// ---------------------------------------------------------------------------
//    
void CNATSettingsTest::PrintSettingsL( CConsoleBase& aConsole )
    {
    _LIT8( KProtocolFormat, "%S " );
    TBuf16<160> buffer16;
    TBuf8<160> buf;
    TBuf8<20> protocol;
    
    aConsole.Printf( _L( "DOMAIN SETTINGS \n" ) );
    
    buffer16.Copy( iSettings->Domain() );
    aConsole.Printf( _L( "Domain name: %S\n" ), &buffer16 );
    
    const CDesC8Array& array = iSettings->AvailableNatProtocols();

    for ( TInt i( 0 ); i < array.Count(); i++ )
        {
        protocol.Copy( array.MdcaPoint( i ) );
        buf.AppendFormat( KProtocolFormat, &protocol );
        }
    buffer16.Copy( buf );
    aConsole.Printf( _L( "\nPriorityOrder =  %S\n" ), &buffer16 );

    if ( KNATFWDefaultUseSharedSecret == iSettings->UseSharedSecret() )
        {
        aConsole.Printf( _L( "STUN Sharedsecret: %d - default\n" ),
            iSettings->UseSharedSecret() );
        }
    else
        {
        aConsole.Printf( _L( "STUN Sharedsecret: %d\n" ),
            iSettings->UseSharedSecret() );
        }
    
    aConsole.Printf( _L( " STUN SETTINGS\n" ) );
    
    PrintStunServerSettings( aConsole );
    
    aConsole.Printf( _L( " TURN SETTINGS \n" ) );
    
    PrintTurnServerSettings( aConsole );
/*
    if( KNATFWDefaultTURNLifeTime == iSettings->TURNLifetime() )
        {
        aConsole.Printf( _L( " Life time: %d -default\n" ), iSettings->TURNLifetime() );
        }
    else
        {
        aConsole.Printf( _L( " Life time: %d\n" ), iSettings->TURNLifetime() );
        }

    if( KNATFWDefaultTURNTimerValue == iSettings->TURNTimerValue() )
        {
        aConsole.Printf( _L( " Timervalue: %d -default\n" ), iSettings->TURNTimerValue() );
        }
    else
        {
        aConsole.Printf( _L( " Timervalue: %d\n" ), iSettings->TURNTimerValue() );
        }

    aConsole.Printf( _L( " ICE SETTINGS \n" ) );

    if( KNATFWDefaultICECanditateTimer ==  iSettings->ChooseAnotherCandidateTimer() )
        {
        aConsole.Printf( _L( " anot.candid.timer: %d -default\n" ), iSettings->ChooseAnotherCandidateTimer() );
        }
    else
        {
        aConsole.Printf( _L( " anot.candid.timer: %d\n" ), iSettings->ChooseAnotherCandidateTimer() );
        }
    */
    }


// ---------------------------------------------------------------------------
// CNATSettingsTest::PrintStunServerSettings
// ---------------------------------------------------------------------------
//      
void CNATSettingsTest::PrintStunServerSettings( CConsoleBase& aConsole )
    {
    RPointerArray<MNATFWServerSettings> serverSettings;
    
    MNATFWStunSettings* stunSettings = &iSettings->StunSettingsL();
    
    stunSettings->GetStunServerArrayL( serverSettings );
    
    TUint count = serverSettings.Count();
    TBuf<40> buffer;
    
    aConsole.Printf( _L( " Server settings found %d\n" ), count );
    
    for( TUint i = 0; i < count; ++i )
        {
        aConsole.Printf( _L( "  Server %d settings\n" ), i );
        buffer.Copy( serverSettings[i]->Address() );
        aConsole.Printf( _L( "  Address: %S\n" ), &buffer );
        
        if ( KNATFWDefaultSTUNServerPort == serverSettings[i]->Port() )
            {
            aConsole.Printf( _L( "  Port: %d -default\n" ), serverSettings[i]->Port() );
            }
        else
            {
            aConsole.Printf( _L( "  Port: %d\n" ), serverSettings[i]->Port() );
            }
         /*   
        if ( KNATFWDefaultTCPTimeout == serverSettings[i]->TCPTimeout() )
            {
            aConsole.Printf( _L( "  TCP Refresh: %d -default\n" ), stunSettings->TCPTimeout() );
            }
        else
            {
            aConsole.Printf( _L( "  TCP Refresh: %d\n" ), serverSettings-[i]->TCPTimeout() );
            }
        
        
        if ( KNATFWDefaultUDPTimeout == serverSettings->At( i ).UDPTimeout() )
            {
            aConsole.Printf( _L( "  UDP Refresh: %d -default\n " ), stunSettings->UDPTimeout() );
            }
        else
            {
            aConsole.Printf( _L( "  UDP Refresh: %d\n" ), serverSettings[i]->UDPTimeout() );
            }
            
        if ( KNATFWDefaultConnectivityCheckTimeout == serverSettings[i]->ConnectivityCheckTimeout() )
            {
            aConsole.Printf( _L( "  ConnectivityCheckTimeout: %d -default\n" ), serverSettings[i]->ConnectivityCheckTimeout() );
            }
        else
            {
            aConsole.Printf( _L( "  ConnectivityCheckTimeout: %d\n" ), serverSettings[i]->ConnectivityCheckTimeout() );
            }
       
        
        if ( KNATFWDefaultCRLFRefreshEnabled == serverSettings->At( i ).CRLFRefreshEnabled() )
            {
            aConsole.Printf( _L( "  CRLFRefreshEnabled: %d -default\n" ), serverSettings[i]->CRLFRefreshEnabled() );
            }
        else
            {
            aConsole.Printf( _L( "  CRLFRefreshEnabled: %d\n" ), serverSettings[i]->CRLFRefreshEnabled() );
            }
        */  
            
        buffer.Copy( serverSettings[i]->Username() );
        aConsole.Printf( _L( "  Username: %S\n" ), &buffer );
        
        buffer.Copy( serverSettings[i]->Password() );
        aConsole.Printf( _L( "  Password: %S\n" ), &buffer );
        }
    
    serverSettings.Close();
    }


// ---------------------------------------------------------------------------
// CNATSettingsTest::PrintTurnServerSettings
// ---------------------------------------------------------------------------
//      
void CNATSettingsTest::PrintTurnServerSettings( CConsoleBase& aConsole )
    {
    RPointerArray<MNATFWServerSettings> serverSettings;
    
    MNATFWTurnSettings* turnSettings = &iSettings->TurnSettingsL();
    
    turnSettings->GetTurnServerArrayL( serverSettings );
    
    TUint count = serverSettings.Count();
    TBuf<40> buffer;
    
    aConsole.Printf( _L( " Server settings found %d\n" ), count );
    
    for( TUint i = 0; i < count; ++i )
        {
        aConsole.Printf( _L( "  Server %d settings\n" ), i );
        buffer.Copy( serverSettings[i]->Address() );
        aConsole.Printf( _L( "  Address: %S\n" ), &buffer );
        
        if ( KNATFWDefaultTURNServerPort == serverSettings[i]->Port() )
            {
            aConsole.Printf( _L( "  Port: %d -default\n" ), serverSettings[i]->Port() );
            }
        else
            {
            aConsole.Printf( _L( "  Port: %d\n" ), serverSettings[i]->Port() );
            }
        /*    
        if ( KNATFWDefaultTCPTimeout == serverSettings->At( i ).TCPTimeout() )
            {
            aConsole.Printf( _L( "  TCP Refresh: %d -default\n" ), serverSettings->At( i ).TCPTimeout() );
            }
        else
            {
            aConsole.Printf( _L( "  TCP Refresh: %d\n" ), serverSettings->At( i ).TCPTimeout() );
            }
        
        
        if ( KNATFWDefaultUDPTimeout == serverSettings->At( i ).UDPTimeout() )
            {
            aConsole.Printf( _L( "  UDP Refresh: %d -default\n " ), serverSettings->At( i ).UDPTimeout() );
            }
        else
            {
            aConsole.Printf( _L( "  UDP Refresh: %d\n" ), serverSettings->At( i ).UDPTimeout() );
            }
       
       
        if ( KNATFWDefaultConnectivityCheckTimeout == serverSettings->At( i ).ConnectivityCheckTimeout() )
            {
            aConsole.Printf( _L( "  ConnectivityCheckTimeout: %d -default\n" ), serverSettings->At( i ).ConnectivityCheckTimeout() );
            }
        else
            {
            aConsole.Printf( _L( "  ConnectivityCheckTimeout: %d\n" ), serverSettings->At( i ).ConnectivityCheckTimeout() );
            }
       
        
        
        if ( KNATFWDefaultCRLFRefreshEnabled == serverSettings->At( i ).CRLFRefreshEnabled() )
            {
            aConsole.Printf( _L( "  CRLFRefreshEnabled: %d -default\n" ), serverSettings->At( i ).CRLFRefreshEnabled() );
            }
        else
            {
            aConsole.Printf( _L( "  CRLFRefreshEnabled: %d\n" ), serverSettings->At( i ).CRLFRefreshEnabled() );
            }
        */
        }
    
    serverSettings.Close();
    }
