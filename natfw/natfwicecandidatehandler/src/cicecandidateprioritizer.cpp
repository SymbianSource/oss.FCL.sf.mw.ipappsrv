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
* Description:    Handles priorization of local candidates
*
*/




#include <e32base.h>
#include "natfwcandidate.h"
#include <cnatfwsettingsapi.h>
#include <mnatfwicesettings.h>
#include "cicecandidateprioritizer.h"
#include "icecandidatehandlerlogs.h"

const TUint KLocalPrefSingleHomedUdp    = 65535;

/**  powers of 2 */
const TUint K2ToPowerOf24   = 2^24;
const TUint K2ToPowerOf12   = 2^12;
const TUint K2ToPowerOf9    = 2^9;
const TUint K2ToPowerOf8    = 2^8;
const TUint K2ToPowerOf0    = 2^0;

/**  max component Id */
const TUint KMaxComponentId             = 256;

// ======== MEMBER FUNCTIONS ========

CIceCandidatePrioritizer::CIceCandidatePrioritizer()
    {
    __ICEDP( "CIceCandidatePrioritizer::CIceCandidatePrioritizer" )
    }


void CIceCandidatePrioritizer::ConstructL( const TDesC8& aDomain )
    {
    iNatSettings = CNATFWNatSettingsApi::NewL( aDomain );
    
    }


CIceCandidatePrioritizer* CIceCandidatePrioritizer::NewL( 
        const TDesC8& aDomain )
    {
    __ICEDP( "CIceCandidatePrioritizer::NewL" )

    CIceCandidatePrioritizer* self 
        = CIceCandidatePrioritizer::NewLC( aDomain );
    CleanupStack::Pop( self );

    return self;
    }


CIceCandidatePrioritizer* CIceCandidatePrioritizer::NewLC( 
        const TDesC8& aDomain )
    {
    __ICEDP( "CIceCandidatePrioritizer::NewLC" )

    CIceCandidatePrioritizer* self = new( ELeave ) CIceCandidatePrioritizer();
    CleanupStack::PushL( self );
    
    self->ConstructL( aDomain );
    
    return self;
    }


CIceCandidatePrioritizer::~CIceCandidatePrioritizer()
    {
    __ICEDP( "CIceCandidatePrioritizer::~CIceCandidatePrioritizer" )
    
    delete iNatSettings;
    }


// ---------------------------------------------------------------------------
// prioritize candidate
// ---------------------------------------------------------------------------
//
void CIceCandidatePrioritizer::PrioritizeL( CNATFWCandidate& aCandidate ) const
    {
    __ICEDP( "CIceCandidatePrioritizer::PrioritizeL" )
    
    TInt type_preference( 0 );
    TInt direction_pref( 0 );
    const MNATFWIceSettings& settings = iNatSettings->IceSettingsL();
    
    switch( aCandidate.Type() )
        {
// UDP candidate types
        case CNATFWCandidate::EHost:
            {
            settings.GetPrefValue( MNATFWIceSettings::EPrefHostCandidate, 
                type_preference );
            break;
            }        
        case CNATFWCandidate::EServerReflexive:
            {
            settings.GetPrefValue( 
                MNATFWIceSettings::EPrefServerReflexiveCandidate, 
                type_preference );
            break;
            }
        case CNATFWCandidate::EPeerReflexive:
            {
            settings.GetPrefValue( 
                MNATFWIceSettings::EPrefPeerReflexiveCandidate,
                type_preference  );
            
            // safety check for invalid provisioning
            if ( MNATFWIceSettings::KPrefValueDisabled == type_preference )
                {
                type_preference = 0;
                }
            break;
            }
        case CNATFWCandidate::ERelay:
            {
            settings.GetPrefValue( MNATFWIceSettings::EPrefRelayCandidate,
                type_preference );
            break;
            }
            
        default:
            {
            __ICEDP( "CIceCandidatePrioritizer::Prioritize, DEFAULT" )
            ASSERT( EFalse );
            break;
            }
        }
    
    ASSERT( MNATFWIceSettings::KPrefValueDisabled != type_preference );
    ASSERT( MNATFWIceSettings::KPrefValueDisabled != direction_pref );
    
    TInt transport_pref( 0 );
    switch( aCandidate.TransportProtocol() )
        {
        case KProtocolInetUdp:
            settings.GetPrefValue( MNATFWIceSettings::EPrefUdpTransport,
                transport_pref );
            break;
        
        case KProtocolInetTcp:
            settings.GetPrefValue( MNATFWIceSettings::EPrefTcpTransport,
                transport_pref );
            break;
        
        default:
            __ICEDP( "CIceCandidatePrioritizer::Prioritize, DEFAULT" )
            ASSERT( EFalse );
            break;
        }
    
    ASSERT( MNATFWIceSettings::KPrefValueDisabled != transport_pref );
    
    TUint local_preference = KLocalPrefSingleHomedUdp;
    if ( KProtocolInetTcp == aCandidate.TransportProtocol() )
        {
        TUint other_pref = 0;
        local_preference = K2ToPowerOf12 * transport_pref + \
                           K2ToPowerOf9  * direction_pref + \
                           K2ToPowerOf0  * other_pref;
        }
    
    TUint component_ID = aCandidate.ComponentId();
    TUint priority = K2ToPowerOf24 * type_preference + \
                     K2ToPowerOf8  * local_preference + \
                     K2ToPowerOf0  * ( KMaxComponentId - component_ID );
    
    aCandidate.SetPriority( priority );
    }
