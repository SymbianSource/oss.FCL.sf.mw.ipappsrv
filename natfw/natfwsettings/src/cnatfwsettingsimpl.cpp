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




#include <e32def.h>
#include <badesca.h>
#include <unsafprotocolscrkeys.h>
#include "cnatfwsettingsimpl.h"
#include "natsettingslogs.h"
#include "natfwdefaultsettingsvalues.h"
#include "cnatfwcenrephandler.h"
#include "tnatfwsettingsparser.h"
#include "cnatfwicesettings.h"
#include "cnatfwstunsettings.h"
#include "cnatfwturnsettings.h"

const TUint KDefaultGranularity = 1;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::CNATFWNatSettingsImpl
// ---------------------------------------------------------------------------
//
CNATFWNatSettingsImpl::CNATFWNatSettingsImpl()
    {
    iRefreshIntervalUdp = KNATFWDefaultRefreshIntervalUdp;
    iRefreshIntervalTcp = KNATFWDefaultRefreshIntervalTcp;
    iUseSharedSecret = KNATFWDefaultUseSharedSecret;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CNATFWNatSettingsImpl::ConstructL( const TDesC8& aDomain )
    {
    __NATSETTINGS( "CNATFWNatSettingsImpl::ConstructL - start" )
    
    __ASSERT_ALWAYS( TUint( &aDomain ), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aDomain.Length(), User::Leave( KErrArgument ) );
    
    iRepHandler = CNATFWCenRepHandler::NewL( KCRUidUNSAFProtocols );
   
    iEcomPlugInPriorityOrderArray =
        new ( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    
    iIceSettings = CNATFWIceSettings::NewL();
    iStunSettings = CNATFWStunSettings::NewL();
    iTurnSettings = CNATFWTurnSettings::NewL();
    
    TInt err = iRepHandler->ReadDomainSettingsKey( aDomain, iDomainKey );
    
    if ( KErrNotFound == err  )
        {
        __NATSETTINGS( "specific domain not found in the CR" )
        iDomainKey = iRepHandler->CreateNewKeyL( KUNSAFProtocolsDomainMask,
            KUNSAFProtocolsFieldTypeMask );
        }

    if ( KErrNone != err && KErrNotFound != err )
        {
        User::Leave( err );
        }
        
    ReadGeneralSettingsL(); 
    iDomain = aDomain.AllocL();
    
    __NATSETTINGS( "CNATFWNatSettingsImpl::ConstructL - end" )
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::NewL
// ---------------------------------------------------------------------------
//
CNATFWNatSettingsImpl* CNATFWNatSettingsImpl::NewL(
    const TDesC8& aDomain )
    {
    __NATSETTINGS( "CNATFWNatSettingsImpl::NewL" )
    
    CNATFWNatSettingsImpl* self = CNATFWNatSettingsImpl::NewLC(
        aDomain );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::NewLC
// ---------------------------------------------------------------------------
//
CNATFWNatSettingsImpl* CNATFWNatSettingsImpl::NewLC(
    const TDesC8& aDomain )
    {
    __NATSETTINGS( "CNATFWNatSettingsImpl::NewLC" )
    
    CNATFWNatSettingsImpl* self =
        new( ELeave ) CNATFWNatSettingsImpl();
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;    
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::~CNATFWNatSettingsImpl
//----------------------------------------------------------------------------
//
CNATFWNatSettingsImpl::~CNATFWNatSettingsImpl()
    {
    __NATSETTINGS( "CNATFWNatSettingsImpl::~CNATFWNatSettingsImpl" )    
    
    delete iDomain;
    delete iEcomPlugInPriorityOrderArray;
    delete iRepHandler;
    delete iIceSettings;
    delete iStunSettings;
    delete iTurnSettings;
    }


// --------------------------------------------------------------------------- 
// CNATFWNatSettingsImpl::RetrieveIapSettings
// ---------------------------------------------------------------------------
//
void CNATFWNatSettingsImpl::RetrieveIapSettingsL( const TUint aIapId )
    {
    __NATSETTINGS( "CNATFWNatSettingsImpl::RetrieveIapSettingsL" )
    
    ReadIapSettingsL( aIapId );
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::Domain
// ---------------------------------------------------------------------------
//    
const TDesC8& CNATFWNatSettingsImpl::Domain() const
    {
    return *iDomain;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::RefreshIntervalUdp
// ---------------------------------------------------------------------------
//    
TInt CNATFWNatSettingsImpl::RefreshIntervalUdp() const
    {
    return iRefreshIntervalUdp;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::RefreshIntervalTcp
// ---------------------------------------------------------------------------
//    
TInt CNATFWNatSettingsImpl::RefreshIntervalTcp() const
    {
    return iRefreshIntervalTcp;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::CrlfRefreshEnabled
// ---------------------------------------------------------------------------
//
TBool CNATFWNatSettingsImpl::CrlfRefreshEnabled() const
    {
    return iCrlfRefreshEnabled;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::SharedSecret
// ---------------------------------------------------------------------------
//
TBool CNATFWNatSettingsImpl::UseSharedSecret() const
    {
    return iUseSharedSecret;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::IceSettingsL
// ---------------------------------------------------------------------------
// 
MNATFWIceSettings& CNATFWNatSettingsImpl::IceSettingsL()
    {
    iIceSettings->ReadSettingsL( iDomainKey, *iRepHandler );
    return *iIceSettings;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::StunSettingsL
// ---------------------------------------------------------------------------
// 
MNATFWStunSettings& CNATFWNatSettingsImpl::StunSettingsL()
    {
    iStunSettings->ReadSettingsL( iDomainKey, *iRepHandler );
    return *iStunSettings;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::TurnSettingsL
// ---------------------------------------------------------------------------
// 
MNATFWTurnSettings& CNATFWNatSettingsImpl::TurnSettingsL()
    {
    iTurnSettings->ReadSettingsL( iDomainKey, *iRepHandler );
    return *iTurnSettings;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::AvailableNatProtocols
// ---------------------------------------------------------------------------
//
const CDesC8Array& CNATFWNatSettingsImpl::AvailableNatProtocols() const
    {
    return *iEcomPlugInPriorityOrderArray;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::GetPortAreaL
// ---------------------------------------------------------------------------
//
void CNATFWNatSettingsImpl::GetPortArea( TUint& aStartPort,
    TUint& aEndPort ) const
    {
    aStartPort = iPortPoolStartPort;
    aEndPort = iPortPoolEndPort;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::ReadGeneralSettingsL
// ---------------------------------------------------------------------------
//
void CNATFWNatSettingsImpl::ReadGeneralSettingsL( )
    {
    __NATSETTINGS( "CNATFWNatSettingsImpl::ReadGeneralSettingsL - start" )
    
    //Protocol priority order
    delete iEcomPlugInPriorityOrderArray;
    iEcomPlugInPriorityOrderArray = NULL;
    HBufC8* tempBuffer = iRepHandler->ReadL(
        KUNSAFProtocolsUsedNATProtocolMask | iDomainKey );
    if ( tempBuffer )
        {
        CleanupStack::PushL( tempBuffer );
        iEcomPlugInPriorityOrderArray =
            TNATFWSettingsParser::ParseDataToArrayL( *tempBuffer );
        CleanupStack::PopAndDestroy( tempBuffer );
        }
    else
        {
        iEcomPlugInPriorityOrderArray =
            TNATFWSettingsParser::ParseDataToArrayL( KNATFWDefaultPriorityOrder );
        }
    
    // Use Shared secret
    if ( KErrNone != iRepHandler->ReadBoolValue(
        KUNSAFProtocolsDomainSharedSecretNotSupported | iDomainKey,
        iUseSharedSecret ) )
        {
        iUseSharedSecret = KNATFWDefaultUseSharedSecret;
        }
    else
        {
        if ( iUseSharedSecret )
            {
            iUseSharedSecret = EFalse;
            }
        else
            {
            iUseSharedSecret = ETrue;
            }
        }
    
    // Read refresh interval UDP
    if ( KErrNone != iRepHandler->Read( KUNSAFProtocolsDomainIntervalUDPMask |
        iDomainKey, iRefreshIntervalUdp ) )
        {
        iRefreshIntervalUdp = KNATFWDefaultRefreshIntervalUdp;
        }
    
    // Read refresh interval TCP
    if ( KErrNone != iRepHandler->Read( KUNSAFProtocolsDomainIntervalTCPMask |
        iDomainKey, iRefreshIntervalTcp ) )
        {
        iRefreshIntervalTcp = KNATFWDefaultRefreshIntervalTcp;
        }
    
    // Start port of the port range
    if ( KErrNone != iRepHandler->Read ( KUNSAFProtocolsPortPoolStartPortMask |
        iDomainKey, iPortPoolStartPort ) )
        {
        iPortPoolStartPort = KNATFWDefaultPortPoolStartPort;
        }
    
    // Start port of the port range
    if ( KErrNone != iRepHandler->Read( KUNSAFProtocolsPortPoolEndPortMask |
        iDomainKey, iPortPoolEndPort ) )
        {
        iPortPoolEndPort = KNATFWDefaultPortPoolEndPort;
        }
    
    // Enable CRLF- based Refresh
    if ( KErrNone != iRepHandler->Read( KUNSAFProtocolsDomainEnableCRLFRefresh |
            iDomainKey, iCrlfRefreshEnabled ) )
        {
        iCrlfRefreshEnabled = KNATFWDefaultCRLFRefreshEnabled;
        }
    
    __NATSETTINGS( "CNATFWNatSettingsImpl::ReadGeneralSettingsL - end" )
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsImpl::ReadIapSettingsL
// ---------------------------------------------------------------------------
//    
void CNATFWNatSettingsImpl::ReadIapSettingsL( TUint32 aIapId )
    {
    __NATSETTINGS_INT1(
        "CNATFWNatSettingsImpl::ReadIapSettingsL IAPId: - start", aIapId )
    
    TUint32 iapKey;
    TInt err = iRepHandler->ReadIapSettingsKey( aIapId, iapKey );
    
    if ( KErrNone == err )
        {
        TUint stunRetransmitTimer( 0 );
        // read UDP refresh interval
        iRepHandler->Read( KUNSAFProtocolsIntervalUDPMask | iapKey,
            iRefreshIntervalUdp );
        // read TCP refresh interval
        iRepHandler->Read( KUNSAFProtocolsIntervalTCPMask | iapKey,
            iRefreshIntervalTcp );
        // read Retransmit interval
        if ( KErrNone == iRepHandler->Read( KUNSAFProtocolsSTUNRetransmitTimerMask | iapKey,
            stunRetransmitTimer ) )
            {
            iStunSettings->SetRetransmissionTimeout( stunRetransmitTimer );
            iTurnSettings->SetRetransmissionTimeout( stunRetransmitTimer );
            }
        }
    else if ( KErrNotFound == err )
        {
        __NATSETTINGS(
            "Specific IAP settings not found, Use default or current values" );
        }
    else
        {
        User::Leave( err );
        }
    __NATSETTINGS( "CNATFWNatSettingsImpl::ReadIapSettingsL - end" )
    }

//End of file
