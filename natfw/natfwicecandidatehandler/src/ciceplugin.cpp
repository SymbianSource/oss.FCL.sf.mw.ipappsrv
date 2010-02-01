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




#include <e32math.h>
#include "natfwcandidate.h"
#include "ciceplugin.h"
#include "cicesessiondata.h"
#include "cicelocalcandidatefinder.h"
#include "ciceconnectionhandler.h"
#include "cicecheckhandler.h"
#include "cicestreamcollection.h"
#include "cicenatplugincontainer.h"
#include "icecandidatehandlerlogs.h"

// ======== MEMBER FUNCTIONS ========

CIcePlugin::CIcePlugin()
    {
    __ICEDP( "CIcePlugin::CIcePlugin" )
    }


void CIcePlugin::ConstructL( TAny* aInitParams )
    {
    __ICEDP( "CIcePlugin::ConstructL" )
    __ASSERT_ALWAYS( aInitParams, User::Leave( KErrArgument ) );
    
    iPluginId = KNatPluginIdNokiaIce().AllocL();
    
    CNATFWPluginApi::TNATFWPluginInitParams* initParams = 
        static_cast<CNATFWPluginApi::TNATFWPluginInitParams*>
        ( aInitParams );
   
    iPluginObserver = &(initParams->iObserver);
    
    iSessionData = CIceSessionData::NewL(
        initParams->iDomain, initParams->iIapId );
    
    iPluginContainer = CIceNatPluginContainer::NewL( 
        *iSessionData, initParams->iMultiplexer );
    
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EGeneralError );
    iPluginContainer->RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EServerConnected );
    
    iCandidateFinder = CIceLocalCandidateFinder::NewL( 
        *this, *iSessionData, *iPluginContainer );
    
    iConnectionHandler = CIceConnectionHandler::NewL(
        initParams->iMultiplexer, *this, *iSessionData, 
        *iPluginContainer, *this );
    
    iCheckHandler = CIceCheckHandler::NewL( *iSessionData,
        initParams->iMultiplexer, *iConnectionHandler, *this );
    }


CIcePlugin* CIcePlugin::NewL( TAny* aInitParams )
    {
    __ICEDP( "CIcePlugin::NewL" )
    
    CIcePlugin* self = new ( ELeave ) CIcePlugin();  
    CleanupStack::PushL(self);
    self->ConstructL( aInitParams );
    CleanupStack::Pop(self);
    
    return self;
    }


CIcePlugin::~CIcePlugin()
    {
    __ICEDP( "CIcePlugin::~CIcePlugin" )
    
    delete iPluginId;
    delete iCheckHandler;
    delete iSessionData;
    
    if ( iPluginContainer )
        {
        iPluginContainer->UnregObserver( *this );
        delete iPluginContainer;
        }
    
    delete iCandidateFinder;
    delete iConnectionHandler;
    
    iPluginObserver = NULL;
    }


// From CNATFWPluginApi

// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//    
void CIcePlugin::ConnectServerL( const RSocketServ& aSocketServ,
        const TName& aConnectionName )
    {
    __ICEDP( "CIcePlugin::ConnectServerL start" )
    iPluginContainer->ConnectPluginsL( aSocketServ, aConnectionName );
    __ICEDP( "CIcePlugin::ConnectServerL end" )
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//
void CIcePlugin::FetchCandidatesL( TUint aStreamCollectionId,
    TUint aStreamId, TUint aComponentId, TUint aAddrFamily )
    {
    __ICEDP( "CIcePlugin::FetchCandidatesL" )

    // Save collection identifiers
    iSessionData->CreateCollectionL( aStreamCollectionId );
    iSessionData->CollectionL( aStreamCollectionId )
        ->AddMediaComponentL( aStreamId, aComponentId );

    const TInt KTaTimerValue( 20 );
    iSessionData->SetTaTimerValue( KTaTimerValue );

    // Both ipv4 and ipv6 candidates will be fetched. Ipv6 candidates will be
    // fetched after ipv4 candidate fetching has completed
    if ( KAFUnspec == aAddrFamily )
        {
        TRAPD( err, iCandidateFinder->FetchCandidatesL(
            aStreamCollectionId, aStreamId, aComponentId, KAfInet, ETrue ) );
        if ( KErrNone != err )
            {
            iCandidateFinder->FetchCandidatesL( aStreamCollectionId,
                aStreamId, aComponentId, KAfInet6, EFalse );
            }
        }

    // Desired address family only
    else
        {
        iCandidateFinder->FetchCandidatesL(
            aStreamCollectionId, aStreamId, aComponentId, aAddrFamily, EFalse );
        }
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//
void CIcePlugin::SetRoleL( TNATFWIceRole aRole )
    {
    __ICEDP( "CIcePlugin::SetRoleL" )
    __ASSERT_ALWAYS( 
        EIceRoleControlling == aRole || EIceRoleControlled == aRole,
        User::Leave( KErrArgument ) );
    
    iSessionData->SetRole( aRole );
    iSessionData->SetTieBreaker(
        TUint64( Math::Random() ) + TUint64( Math::Random() ) );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// Set credentials for message signing and authentication.
// This plugin supports only stream specific credentials.
// ---------------------------------------------------------------------------
//
void CIcePlugin::SetCredentialsL( const CNATFWCandidate& aCandidate, 
        const CNATFWCredentials& aCredentials )
    {
    __ICEDP( "CIcePlugin::SetCredentialsL" )
    
    const_cast<CNATFWCredentials&>( aCredentials )
        .SetStreamId( aCandidate.StreamId() );
    
    iSessionData->AddCredentialsL( aCredentials );
    iCheckHandler->SetCredentialsL( aCredentials );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// Credentials must be set before starting connectivity checks.
// ---------------------------------------------------------------------------
//
void CIcePlugin::PerformConnectivityChecksL( 
        RPointerArray<CNATFWCandidate>& aRemoteCandidates )
    {
    __ICEDP( "CIcePlugin::PerformConnectivityChecksL" )
    
    iSessionData->RemoveRemoteCandidates();
    iSessionData->AddRemoteCandidatesL( aRemoteCandidates );
    
    const TInt KTaTimerValue( 20 );
    iSessionData->SetTaTimerValue( KTaTimerValue );
    
    iCheckHandler->PerformConnectivityChecksL();
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// Peer selected pairs should be known by local agent. Peer reflexive remote
// candidate is added to the known remote candidate list when receiving check
// from peer. Peer MUST not include new candidates when it has completed
// ICE processing.
// ---------------------------------------------------------------------------
//
void CIcePlugin::UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs )
    {
    __ICEDP( "CIcePlugin::UpdateIceProcessingL, PEERSELECTEDPAIRS" )
    __ASSERT_ALWAYS( 0 != aPeerSelectedPairs.Count(),
        User::Leave( KErrArgument ) );
    
    iCheckHandler->UpdateIceProcessingL( aPeerSelectedPairs );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CIcePlugin::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CIcePlugin::UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    __ICEDP( "CIcePlugin::UpdateIceProcessingL, REMOTECANDS" )
    __ASSERT_ALWAYS( 0 != aRemoteCands.Count(),
        User::Leave( KErrArgument ) );
    
    iSessionData->AddRemoteCandidatesL( aRemoteCands );
    iCheckHandler->UpdateIceProcessingL( aRemoteCands );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CIcePlugin::SetReceivingStateL
// ---------------------------------------------------------------------------
//
void CIcePlugin::SetReceivingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState )
    {
    iConnectionHandler->SetReceivingStateL( aLocalCandidate, aState );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CIcePlugin::SetSendingStateL
// ---------------------------------------------------------------------------
//
void CIcePlugin::SetSendingStateL( 
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    iConnectionHandler->SetSendingStateL( aLocalCandidate, aState, aDestAddr );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CIcePlugin::GetConnectionIdL
// ---------------------------------------------------------------------------
//
void CIcePlugin::GetConnectionIdL( 
        const CNATFWCandidate& aLocalCandidate,
        TUint& aConnectionId )
    {
    iConnectionHandler->GetConnectionIdL( aLocalCandidate, aConnectionId );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// CIcePlugin::PluginIdentifier
// ---------------------------------------------------------------------------
//
const TDesC8& CIcePlugin::PluginIdentifier() const
    {
    return *iPluginId;
    }


// ---------------------------------------------------------------------------
// CIcePlugin::StreamClosed
// Stream collection can be deleted right away, when one media component has
// been removed.
// ---------------------------------------------------------------------------
//
void CIcePlugin::StreamClosed( TUint aStreamId )
    {
    TUint collectionId( 0 );
    TInt err = iSessionData->GetCollectionIdForComponent( 
        aStreamId, collectionId );
    
    if ( KErrNone == err )
        {
        iSessionData->CleanupStreamData( aStreamId );
        iCheckHandler->CleanupCollectionData( collectionId );
        iSessionData->DeleteCollection( collectionId );
        }
    }

// ---------------------------------------------------------------------------
// From class MIceNatPluginEventObs.
// Dispatches events from specialized event listeners to the ICE-plugin user.
//
// Listens to general errors on real NAT-plugins. If unexpected error within
// a stream in some plugin occurs, we cannot do anything but close the stream.
// ---------------------------------------------------------------------------
//
void CIcePlugin::PluginEventOccured( 
        #ifdef _DEBUG
        const CNATFWPluginApi* aPlugin,
        #else
        const CNATFWPluginApi* /*aPlugin*/,
        #endif
        TUint aStreamId, 
        MIceNatPluginEventObs::TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId,
        TUint aComponentId, TBool aIPv6After )
    {
    __ICEDP( "CIcePlugin::PluginEventOccured" )
    
    switch ( aEventCode )
        {
        case MIceNatPluginEventObs::EServerConnected:
            {
            ASSERT( NULL != aPlugin );
            
            iConnectedServerCount++;
            if ( iConnectedServerCount == iPluginContainer->Count() )
                {
                iPluginObserver->Notify( *this, aStreamId, 
                    MNATFWPluginObserver::EServerConnected, KErrNone );
                }
            }
            break;
                    
        case MIceNatPluginEventObs::ELocalCandidateFound:
            {
            ASSERT( NULL == aPlugin && NULL != aEventData );
            
            iPluginObserver->NewLocalCandidateFound( *this, 
                reinterpret_cast<CNATFWCandidate*>( aEventData ) );
            }
            break;
        
        case MIceNatPluginEventObs::EFetchingCompleted:
            {
            ASSERT( NULL == aPlugin );

            if ( aIPv6After )
                {
                if ( KErrNone == aErrorCode )
                    {
                    iIpv4Succeeded = ETrue;
                    }
                    
                TRAPD( err, iCandidateFinder->FetchCandidatesL(
                    aStreamConnectionId, aStreamId, aComponentId, 
                    KAfInet6, EFalse ) );
                    
                if ( KErrNone != err )
                    {
                    iPluginObserver->Notify( *this, aStreamId, 
                        MNATFWPluginObserver::EFetchingCompleted, aErrorCode );
                    }
                }
                
            else
                {
                if ( iIpv4Succeeded )
                    {
                    aErrorCode = KErrNone;
                    iIpv4Succeeded = EFalse;
                    }

                iPluginObserver->Notify( *this, aStreamId,
                    MNATFWPluginObserver::EFetchingCompleted, aErrorCode );
                }
            }
            break;
            
        case MIceNatPluginEventObs::EReceivingActivated:
            {
            ASSERT( NULL == aPlugin );
            
            iPluginObserver->Notify( *this, aStreamId, 
                MNATFWPluginObserver::EReceivingActivated, aErrorCode );
            }
            break;
            
        case MIceNatPluginEventObs::EReceivingDeactivated:
            {
            ASSERT( NULL == aPlugin );
            
            iPluginObserver->Notify( *this, aStreamId, 
                MNATFWPluginObserver::EReceivingDeactivated, aErrorCode );
            }
            break;            

        case MIceNatPluginEventObs::ESendingActivated:
            {
            ASSERT( NULL == aPlugin );
            
            iPluginObserver->Notify( *this, aStreamId, 
                MNATFWPluginObserver::ESendingActivated, aErrorCode );
            }
            break;
        
        case MIceNatPluginEventObs::ESendingDeactivated:
            {
            ASSERT( NULL == aPlugin );
            
            iPluginObserver->Notify( *this, aStreamId, 
                MNATFWPluginObserver::ESendingDeactivated, aErrorCode );
            }
            break;
        
        case MIceNatPluginEventObs::ECandidatePairFound:
            {
            ASSERT( NULL == aPlugin && NULL != aEventData );
            
            iPluginObserver->NewCandidatePairFound( *this, 
                reinterpret_cast<CNATFWCandidatePair*>( aEventData ) );
            }
            break;

        case MIceNatPluginEventObs::EConnChecksCompleted:
            {
            ASSERT( NULL == aPlugin );
            
            iPluginObserver->Notify( *this, aStreamId, 
                MNATFWPluginObserver::EConnChecksCompleted, aErrorCode );
            }
            break;

        case MIceNatPluginEventObs::EGeneralError:
            {
            // this is only event from real NAT-plugin
            ASSERT( NULL != aPlugin );
            ASSERT( KErrNone != aErrorCode );
            
            iPluginObserver->Error( *this, aStreamId, aErrorCode );
            }
            break;
        
        default:
            
            __ICEDP( "CIcePlugin::PluginEventOccured, DEFAULT" )
            ASSERT( EFalse );
        }
    }
