/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
//#include <badesca.h>
#include <mnatfwicesettings.h>
#include <mnatfwstunsettings.h>
#include <mnatfwturnsettings.h>
#include <cnatfwsettingsapi.h>
#include "cnatfwsettingsimpl.h"
#include "natsettingslogs.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::CNATFWNatSettingsApi
// ---------------------------------------------------------------------------
//
CNATFWNatSettingsApi::CNATFWNatSettingsApi()
    {
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::ConstructL
// ---------------------------------------------------------------------------
//
void CNATFWNatSettingsApi::ConstructL( const TDesC8& aDomain )
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::ConstructL" )

    iSettingsImpl = CNATFWNatSettingsImpl::NewL( aDomain );
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWNatSettingsApi* CNATFWNatSettingsApi::NewL(
    const TDesC8& aDomain )
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::NewL" )
    
    CNATFWNatSettingsApi* self = CNATFWNatSettingsApi::NewLC(
        aDomain );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::NewLC
// ---------------------------------------------------------------------------
//
EXPORT_C CNATFWNatSettingsApi* CNATFWNatSettingsApi::NewLC(
    const TDesC8& aDomain )
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::NewLC" )
    
    CNATFWNatSettingsApi* self = new ( ELeave ) CNATFWNatSettingsApi();
    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::~CNATFWNatSettingsApi
//----------------------------------------------------------------------------
//
CNATFWNatSettingsApi::~CNATFWNatSettingsApi()
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::~CNATFWNatSettingsApi" )    
    
    delete iSettingsImpl;
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::RetrieveIapSettings
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWNatSettingsApi::RetrieveIapSettingsL( const TUint aIapId )
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::RetrieveIapSettingsL" )
    
    iSettingsImpl->RetrieveIapSettingsL( aIapId );
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::Domain
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CNATFWNatSettingsApi::Domain() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::Domain" )
    
    return iSettingsImpl->Domain();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::RefreshIntervalUdp
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CNATFWNatSettingsApi::RefreshIntervalUdp() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::RefreshIntervalUdp" )
    
    return iSettingsImpl->RefreshIntervalUdp();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::RefreshIntervalTcp
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CNATFWNatSettingsApi::RefreshIntervalTcp() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::RefreshIntervalTcp" )
     
    return iSettingsImpl->RefreshIntervalTcp();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::IceSettingsL
// ---------------------------------------------------------------------------
//
EXPORT_C MNATFWIceSettings& CNATFWNatSettingsApi::IceSettingsL() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::IceSettingsL" )
    
    return iSettingsImpl->IceSettingsL();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::StunSettingsL
// ---------------------------------------------------------------------------
//
EXPORT_C MNATFWStunSettings& CNATFWNatSettingsApi::StunSettingsL() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::StunSettingsL" )
    
    return iSettingsImpl->StunSettingsL();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::TurnSettingsL
// ---------------------------------------------------------------------------
//
EXPORT_C MNATFWTurnSettings& CNATFWNatSettingsApi::TurnSettingsL() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::TurnSettingsL" )
    
    return iSettingsImpl->TurnSettingsL();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::CrlfRefreshEnabled
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWNatSettingsApi::CrlfRefreshEnabled() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::CrlfRefreshEnabled" )
    
    return iSettingsImpl->CrlfRefreshEnabled();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::UseSharedSecret
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CNATFWNatSettingsApi::UseSharedSecret() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::UseSharedSecret" )
    
    return iSettingsImpl->UseSharedSecret();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::AvailableNatProtocols
// ---------------------------------------------------------------------------
//
EXPORT_C const CDesC8Array&
    CNATFWNatSettingsApi::AvailableNatProtocols() const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::AvailableNatProtocols" )
    
    return iSettingsImpl->AvailableNatProtocols();
    }


// ---------------------------------------------------------------------------
// CNATFWNatSettingsApi::GetPortAreaL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNATFWNatSettingsApi::GetPortArea( TUint& aStartPort,
    TUint& aEndPort ) const
    {
    __NATSETTINGS( "CNATFWNatSettingsApi::GetPortArea" )
    
    iSettingsImpl->GetPortArea( aStartPort, aEndPort );
    }

//End of file
