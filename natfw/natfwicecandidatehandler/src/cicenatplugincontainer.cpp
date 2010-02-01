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




#include "natfwpluginapi.h"
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"
#include <cnatfwsettingsapi.h>
#include <mnatfwicesettings.h>
#include <badesca.h>
#include "cicenatplugincontainer.h"
#include "mncmconnectionmultiplexer.h"
#include "cicesessiondata.h"
#include "cicehostresolver.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlerlogs.h"

const TInt KMaxNumberOfUtilityPlugins   = 2;

// ======== MEMBER FUNCTIONS ========

CIceNatPluginContainer::CIceNatPluginContainer( 
        const CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer )
        :
        iInitParams( *this, aMultiplexer, 
            aSessionData.Domain(), aSessionData.IapId() )
    {
    }


void CIceNatPluginContainer::ConstructL()
    {
    __ICEDP( "CIceNatPluginContainer::ConstructL" )
    
    TInt startInd = MIceNatPluginEventObs::EFirstPluginEventInd;
    TInt stopInd = MIceNatPluginEventObs::ELastPluginEventInd;
    for ( TInt i = startInd; i <= stopInd; ++i )
        {
        RPointerArray<MIceNatPluginEventObs>* arrayForEvent 
            = new ( ELeave ) RPointerArray<MIceNatPluginEventObs>;
        CleanupClosePushL( *arrayForEvent );
        iObserversForEvent.AppendL( arrayForEvent );
        CleanupStack::Pop( arrayForEvent ); // lint #429
        }
    
    ResolvePluginsL();
    }


CIceNatPluginContainer* CIceNatPluginContainer::NewL( 
        const CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer )
    {
    CIceNatPluginContainer* self 
        = CIceNatPluginContainer::NewLC( aSessionData, aMultiplexer );
    CleanupStack::Pop( self );
    return self;
    }


CIceNatPluginContainer* CIceNatPluginContainer::NewLC( 
        const CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer )
    {
    CIceNatPluginContainer* self 
        = new( ELeave ) CIceNatPluginContainer( aSessionData, aMultiplexer );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CIceNatPluginContainer::~CIceNatPluginContainer()
    {
    __ICEDP( "CIceNatPluginContainer::~CIceNatPluginContainer" )
    
    TInt count( iObserversForEvent.Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        iObserversForEvent[i]->Close();
        }
    
    iObserversForEvent.ResetAndDestroy();
    
    iPlugins.ResetAndDestroy();
    
    REComSession::FinalClose();
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::ConnectPluginsL
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::ConnectPluginsL( const RSocketServ& aSocketServ,
        const TName& aConnectionName )
    {
    __ICEDP( "CIceNatPluginContainer::ConnectPluginsL" )
    
    for ( TInt i( iPlugins.Count() - 1 ); i >= 0; --i )
        {
        TRAPD( error,
            iPlugins[i]->ConnectServerL( aSocketServ, aConnectionName ) );
        
        if ( KErrNone != error )
            {
            delete iPlugins[i];
            iPlugins.Remove( i );
            }
        }
    
    __ASSERT_ALWAYS( iPlugins.Count(), User::Leave( KErrCouldNotConnect ) );
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::Count
// ---------------------------------------------------------------------------
//
TInt CIceNatPluginContainer::Count() const
    {
    return iPlugins.Count();
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::RegObserverForEventL
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::RegObserverForEventL( 
        const MIceNatPluginEventObs& aObserver,
        MIceNatPluginEventObs::TNatPluginEvent aEvent )
    {
    __ICEDP( "CIceNatPluginContainer::RegObserverForEventL" )
    __ASSERT_DEBUG( aEvent < iObserversForEvent.Count(), 
        User::Leave( KErrArgument ) );
    
    iObserversForEvent[aEvent]->InsertInAddressOrderL( &aObserver );
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::UnregObserverForEvent
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::UnregObserverForEvent( 
        const MIceNatPluginEventObs& aObserver,
        MIceNatPluginEventObs::TNatPluginEvent aEvent )
    {
    __ICEDP( "CIceNatPluginContainer::UnregObserverForEvent" )
    ASSERT( aEvent < iObserversForEvent.Count() );
    
    TInt ind = iObserversForEvent[aEvent]->Find( &aObserver );
    if ( KErrNotFound != ind )
        {
        iObserversForEvent[aEvent]->Remove( ind );
        }
    else
        {
        ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::UnregObserver
// Unregisters observer for all events.
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::UnregObserver( 
        const MIceNatPluginEventObs& aObserver )
    {
    __ICEDP( "CIceNatPluginContainer::UnregObserver" )
    
    TInt numOfEvents = iObserversForEvent.Count();
    for ( TInt i( numOfEvents - 1 ); i >= 0; --i )
        {
        TInt ind = iObserversForEvent[i]->Find( &aObserver );
        if ( KErrNotFound != ind )
            {
            iObserversForEvent[i]->Remove( ind );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::ReleasePlugins
// Host plugin is always preserved.
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::ReleasePlugins( TBool aRetainRelay )
    {
    __ICEDP_INT1( "CIceNatPluginContainer::ReleasePlugins, RETAINRELAY:",
        aRetainRelay )
    
    for ( TInt i( iPlugins.Count() - 1 ); i >= 0; --i )
        {
        TPtrC8 type = ParsePluginType( iPlugins[i]->PluginIdentifier() );
        
        if ( !( aRetainRelay && KNatPluginTypeTurn() == type )
            && ( KNatPluginTypeHost() != type ) )
            {
            delete iPlugins[i];
            iPlugins.Remove( i );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::PluginByIdentifier
// ---------------------------------------------------------------------------
//
CNATFWPluginApi* CIceNatPluginContainer::PluginByIdentifier( 
        const TDesC8& aIdentifier )
    {
    __ICEDP_STR( "CIceNatPluginContainer::PluginByIdentifier", aIdentifier )
    
    for ( TInt i( 0 ); i < iPlugins.Count(); ++i )
        {
        CNATFWPluginApi* item = iPlugins[i];
        if ( item->PluginIdentifier() == aIdentifier )
            {
            return item;
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::PluginByType
// ---------------------------------------------------------------------------
//
CNATFWPluginApi* CIceNatPluginContainer::PluginByType( 
        const TDesC8& aType )
    {
    __ICEDP_STR( "CIceNatPluginContainer::PluginByType", aType )
    
    TInt numOfPlugins = iPlugins.Count();
    for ( TInt i( 0 ); i < numOfPlugins; ++i )
        {
        CNATFWPluginApi* item = iPlugins[i];
        
        TPtrC8 type = ParsePluginType( item->PluginIdentifier() );
        if ( type == aType )
            {
            return item;
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// From class MNATFWPluginObserver.
// CIceNatPluginContainer::Error
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::Error( const CNATFWPluginApi& aPlugin,
        TUint aStreamId, TInt aErrorCode )
    {
    __ICEDP( "CIceNatPluginContainer::Error" )
    
    RPointerArray<MIceNatPluginEventObs>& observers = 
        *iObserversForEvent[MIceNatPluginEventObs::EGeneralError];
    
    TInt numOfObservers( observers.Count() );
    for ( TInt i = 0; i < numOfObservers; ++i )
        {
        observers[i]->PluginEventOccured( &aPlugin, aStreamId,
            MIceNatPluginEventObs::EGeneralError, 
            aErrorCode, NULL );
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWPluginObserver.
// CIceNatPluginContainer::Notify
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::Notify( const CNATFWPluginApi& aPlugin,
        TUint aStreamId, TNATFWPluginEvent aEvent, TInt aErrCode )
    {
    __ICEDP( "CIceNatPluginContainer::Notify" )
    
    MIceNatPluginEventObs::TNatPluginEvent convertedEvent;
    TInt error = GetConvertedEvent( aEvent, convertedEvent );
    if ( KErrNone == error )
        {
        RPointerArray<MIceNatPluginEventObs>& observers = 
            *iObserversForEvent[convertedEvent];
        
        TInt numOfObservers = observers.Count();
        for ( TInt i = 0; i < numOfObservers; ++i )
            {
            observers[i]->PluginEventOccured( &aPlugin,
                aStreamId, convertedEvent, aErrCode, NULL );
            }
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWPluginObserver.
// CIceNatPluginContainer::NewCandidatePairFound
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::NewCandidatePairFound( 
        const CNATFWPluginApi& aPlugin,
        CNATFWCandidatePair* aPair )
    {
    __ICEDP( "CIceNatPluginContainer::NewCandidatePairFound" )
    
    if ( aPair )
        {
        RPointerArray<MIceNatPluginEventObs>& observers = 
            *iObserversForEvent[MIceNatPluginEventObs::ECandidatePairFound];
        
        TInt numOfObservers = observers.Count();
        for ( TInt i = 0; i < numOfObservers; ++i )
            {
            CNATFWCandidatePair* pair( NULL );
            TRAP_IGNORE( ( pair = CNATFWCandidatePair::NewL( *aPair ) ) )
            if ( pair )
                {
                observers[i]->PluginEventOccured( 
                    &aPlugin,
                    aPair->LocalCandidate().StreamId(),
                    MIceNatPluginEventObs::ECandidatePairFound, 
                    KErrNone, pair );
                }
            }
        
        delete aPair;
        }
    else
        {
        __ICEDP( "CIceNatPluginContainer::NewCandidatePairFound, NULL" )
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWPluginObserver.
// CIceNatPluginContainer::NewLocalCandidateFound
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::NewLocalCandidateFound( 
        const CNATFWPluginApi& aPlugin,
        CNATFWCandidate* aCandidate )
    {
    __ICEDP( "CIceNatPluginContainer::NewLocalCandidateFound" )
    
    if ( aCandidate )
        {
        RPointerArray<MIceNatPluginEventObs>& observers = 
            *iObserversForEvent[MIceNatPluginEventObs::ELocalCandidateFound];
        
        TInt numOfObservers = observers.Count();
        for ( TInt i = 0; i < numOfObservers; ++i )
            {
            CNATFWCandidate* candidate( NULL );
            TRAP_IGNORE( ( candidate = CNATFWCandidate::NewL( *aCandidate ) ) )
            
            if ( candidate )
                {
                observers[i]->PluginEventOccured( 
                    &aPlugin,
                    candidate->StreamId(),
                    MIceNatPluginEventObs::ELocalCandidateFound, 
                    KErrNone, candidate );
                }
            }
        
        delete aCandidate;
        }
    else
        {
        __ICEDP( "CIceNatPluginContainer::NewLocalCandidateFound, NULL" )
        }
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::ResolvePluginsL
// ---------------------------------------------------------------------------
//
void CIceNatPluginContainer::ResolvePluginsL()
    {
    __ICEDP( "CIceNatPluginContainer::ResolvePluginsL" )
    
    CNATFWNatSettingsApi* natSettings = CNATFWNatSettingsApi::NewL( 
        iInitParams.iDomain );
    CleanupStack::PushL( natSettings );
    const MNATFWIceSettings& settings = natSettings->IceSettingsL();
    
    
    TInt hostPreference( MNATFWIceSettings::KPrefValueDisabled );
    settings.GetPrefValue( MNATFWIceSettings::EPrefHostCandidate,
        hostPreference );
    if ( MNATFWIceSettings::KPrefValueDisabled != hostPreference )
        {
        // Create host plugin
        CNATFWPluginApi* hostPlugin 
            = CIceHostResolver::NewL( &iInitParams );
        CleanupStack::PushL( hostPlugin );
        iPlugins.AppendL( hostPlugin );
        CleanupStack::Pop( hostPlugin );
        }
    
    // instantiate utility plugins
    const CDesC8Array& utilityPluginIds = settings.NatUtilityPlugins();
    TInt idCount( utilityPluginIds.Count() );
    __ASSERT_ALWAYS( idCount <= KMaxNumberOfUtilityPlugins, 
        User::Leave( KErrCorrupt ) );
    
    TInt stunPreference( MNATFWIceSettings::KPrefValueDisabled );
    settings.GetPrefValue( 
        MNATFWIceSettings::EPrefServerReflexiveCandidate, stunPreference );
    TInt relayPreference( MNATFWIceSettings::KPrefValueDisabled );
    settings.GetPrefValue( 
        MNATFWIceSettings::EPrefRelayCandidate, relayPreference );
    
    for ( TInt i( 0 ); i < idCount; ++i )
        {
        TPtrC8 type = ParsePluginType( utilityPluginIds[i] );
        if ( ( type.CompareF( KNatPluginTypeStun ) == 0 
                && MNATFWIceSettings::KPrefValueDisabled != stunPreference )
            || ( type.CompareF( KNatPluginTypeTurn ) == 0 
                && MNATFWIceSettings::KPrefValueDisabled != relayPreference ) )
            {
            CNATFWPluginApi* plugin = CNATFWPluginApi::NewL( 
                utilityPluginIds[i], iInitParams );
            CleanupStack::PushL( plugin );
            iPlugins.AppendL( plugin );
            CleanupStack::Pop( plugin );
            }        
        }
    
    CleanupStack::PopAndDestroy( natSettings );
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::GetConvertedEvent
// ---------------------------------------------------------------------------
//
TInt CIceNatPluginContainer::GetConvertedEvent( TNATFWPluginEvent aEvent, 
        MIceNatPluginEventObs::TNatPluginEvent& aConvertedEvent ) const
    {
    switch ( aEvent )
        {
        case MNATFWPluginObserver::EServerConnected:
            aConvertedEvent = MIceNatPluginEventObs::EServerConnected;
            break;
        case MNATFWPluginObserver::EFetchingCompleted:
            aConvertedEvent = MIceNatPluginEventObs::EFetchingCompleted;
            break;
        case MNATFWPluginObserver::ETcpConnSetupCompleted:
            aConvertedEvent = MIceNatPluginEventObs::ETcpConnSetupCompleted;
            break;
        case MNATFWPluginObserver::EReceivingActivated:
            aConvertedEvent = MIceNatPluginEventObs::EReceivingActivated;
            break;
        case MNATFWPluginObserver::EReceivingDeactivated:
            aConvertedEvent = MIceNatPluginEventObs::EReceivingDeactivated;
            break;
        case MNATFWPluginObserver::ESendingActivated:
            aConvertedEvent = MIceNatPluginEventObs::ESendingActivated;
            break;
        case MNATFWPluginObserver::ESendingDeactivated:
            aConvertedEvent = MIceNatPluginEventObs::ESendingDeactivated;
            break;
        case MNATFWPluginObserver::EConnChecksCompleted:
            aConvertedEvent = MIceNatPluginEventObs::EConnChecksCompleted;
            break;
        default:
            __ICEDP_INT1( "CIceNatPluginContainer::GetConvertedEvent DEF:",
                aEvent )
            
            return KErrArgument;
        }
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CIceNatPluginContainer::ParsePluginType
// ---------------------------------------------------------------------------
//
TPtrC8 CIceNatPluginContainer::ParsePluginType( 
        const TDesC8& aIdentifier ) const
    {
    TLex8 lex( aIdentifier );
    while( !lex.Eos() )
        {
        if ( lex.Get() == '.' )
            {
            lex.Mark();
            }
        }
    
    return lex.RemainderFromMark();
    }
