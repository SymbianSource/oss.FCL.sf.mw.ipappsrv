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




#include <ecom/ecom.h>
#include "natfwpluginapi.h"
#include "natfwcandidate.h"
#include "cicelocalcandidatefinder.h"
#include "cicesessiondata.h"
#include "cicecandidateprioritizer.h"
#include "ticenatplugincontaineriter.h"
#include "icecandidatehandlerlogs.h"
#include "cicehostresolver.h"

const TInt KMsToUsFactor = 1000;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CIceLocalCandidateFinder::CIceLocalCandidateFinder
// ---------------------------------------------------------------------------
//
CIceLocalCandidateFinder::CIceLocalCandidateFinder(
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer )
    :
    iEventObserver( aEventObserver ),
    iSessionData( aSessionData ),
    iPluginContainer( aPluginContainer )
    {
    __ICEDP( "CIceLocalCandidateFinder::CIceLocalCandidateFinder" )
    }
    
// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CIceLocalCandidateFinder::ConstructL()
    {
    __ICEDP( "CIceLocalCandidateFinder::ConstructL" )
    
    iPrioritizer = CIceCandidatePrioritizer::NewL( iSessionData.Domain() );
    iTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    
    iPluginContainer.RegObserverForEventL( *this, 
        MIceNatPluginEventObs::ELocalCandidateFound );
    iPluginContainer.RegObserverForEventL( *this, 
        MIceNatPluginEventObs::EFetchingCompleted );
    }
    

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CIceLocalCandidateFinder* CIceLocalCandidateFinder::NewL(
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer )
    {
    __ICEDP( "CIceLocalCandidateFinder::NewL" )
    
    CIceLocalCandidateFinder* self = CIceLocalCandidateFinder::NewLC(
        aEventObserver, aSessionData, aPluginContainer );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CIceLocalCandidateFinder* CIceLocalCandidateFinder::NewLC(
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer )
    {
    __ICEDP( "CIceLocalCandidateFinder::NewLC" )

    CIceLocalCandidateFinder* self = new( ELeave ) 
        CIceLocalCandidateFinder( 
        aEventObserver, aSessionData, aPluginContainer );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }


// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CIceLocalCandidateFinder::~CIceLocalCandidateFinder()
    {
    __ICEDP( "CIceLocalCandidateFinder::~CIceLocalCandidateFinder" )
    
    delete iPrioritizer;
    delete iTimer;
    iResolveRequests.Close();
    iResolvingCnts.Close();
    
    iPluginContainer.UnregObserver( *this );
    }


// ---------------------------------------------------------------------------
// CIceLocalCandidateFinder::ResolveNextCandidate
// ---------------------------------------------------------------------------
//
TInt CIceLocalCandidateFinder::ResolveNextCandidate( TAny* aObject )
    {
    if ( aObject )
        {
        TRAP_IGNORE( 
            reinterpret_cast<CIceLocalCandidateFinder*>( aObject )
            ->ResolveNextCandidateL() )
        return 1;
        }
    else
        {
        return 0;
        }
    }


// ---------------------------------------------------------------------------
// CIceLocalCandidateFinder::FetchCandidatesL
// ---------------------------------------------------------------------------
//
void CIceLocalCandidateFinder::FetchCandidatesL( 
        TUint aStreamCollectionId, TUint aStreamId, TUint aComponentId,
        TUint aAddrFamily, TBool aIPv6After )
    {
    __ICEDP( "CIceLocalCandidateFinder::FetchCandidatesL" )
    
    TInt numOfRequests = iPluginContainer.Count();
    TCounter counter( aStreamId, numOfRequests );
    __ASSERT_ALWAYS( KErrNotFound == iResolvingCnts.Find( 
        counter, TCounter::MatchStreamId ), 
        User::Leave( KErrAlreadyExists ) );
    iResolvingCnts.AppendL( counter );
    
    // form requests
    TIceNatPluginContainerIter iterator 
        = TIceNatPluginContainerIter( iPluginContainer );
    while ( !iterator.IsDone() )
        {
        CNATFWPluginApi* item = iterator++;

        ScheduleResolveRequestL( *item, aStreamCollectionId, aStreamId, 
            aComponentId, aAddrFamily, aIPv6After );
        }
    }


// ---------------------------------------------------------------------------
// From class MIceNatPluginEventObs.
// CIceLocalCandidateFinder::PluginEventOccured
// ---------------------------------------------------------------------------
//
void CIceLocalCandidateFinder::PluginEventOccured( 
        const CNATFWPluginApi* aPlugin, TUint aStreamId, 
        MIceNatPluginEventObs::TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId,
        TUint aComponentId, TBool aIPv6After )
    {
    __ICEDP( "CIceLocalCandidateFinder::PluginEventOccured" )
    
    switch ( aEventCode )
        {
        case MIceNatPluginEventObs::ELocalCandidateFound:
            {
            if ( aEventData && NULL != aPlugin )
                {
                CNATFWCandidate* candidate 
                    = reinterpret_cast<CNATFWCandidate*>( aEventData );
                NewLocalCandidateFound( *aPlugin, *candidate );
                }
            else
                {
                __ICEDP( "CIceLocalCandidateFinder::PluginEventOccured, ELSE" )
                ASSERT( EFalse );
                }
            }
            break;
        
        case MIceNatPluginEventObs::EFetchingCompleted:
            {
            if ( KErrNone == aErrorCode )
                {
                iSucceeded = ETrue;
                }

            // At least one candidate must success
            // otherwise pass error code upwards.
            if ( ResolvingCompleted( aStreamId, aStreamConnectionId,
                     aComponentId, aIPv6After ) )
                {
                iSucceeded ? aErrorCode = KErrNone : aErrorCode = KErrNotFound;
                iSucceeded = EFalse;

                iEventObserver.PluginEventOccured( NULL, aStreamId,
                     aEventCode, aErrorCode, aEventData, aStreamConnectionId,
                     aComponentId, aIPv6After );
                }
            }
            break;

        default:
            __ICEDP( "CIceLocalCandidateFinder::PluginEventOccured, DEFAULT" )
            ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CIceLocalCandidateFinder::ScheduleResolveRequestL
// ---------------------------------------------------------------------------
//
void CIceLocalCandidateFinder::ScheduleResolveRequestL( 
        const CNATFWPluginApi& aPlugin,
        TUint aCollectionId, TUint aStreamId, TUint aComponentId,
        TUint aAddrFamily, TBool aIPv6After )
    {
    __ICEDP( "CIceLocalCandidateFinder::ScheduleResolveRequestL" )
    
    TResolveRequest request( aPlugin,
        aCollectionId, aStreamId, aComponentId, 
        TResolveRequest::ERequestNotIssued, aAddrFamily, aIPv6After );
    iResolveRequests.AppendL( request );
    
    if ( !iTimer->IsActive() )
        {
        TCallBack callBack( 
            CIceLocalCandidateFinder::ResolveNextCandidate, this );
        iTimer->Start( 
            0, iSessionData.TaTimerValue() * KMsToUsFactor, callBack );
        }
    }


// ---------------------------------------------------------------------------
// CIceLocalCandidateFinder::ResolveNextCandidateL
// ---------------------------------------------------------------------------
//
void CIceLocalCandidateFinder::ResolveNextCandidateL()
    {
    TResolveRequest searchItem( TResolveRequest::ERequestNotIssued );
    TInt reqIndex = iResolveRequests.Find( 
        searchItem, TResolveRequest::MatchState );
    
    if ( KErrNotFound != reqIndex )
        {
        __ICEDP( "CIceLocalCandidateFinder::ResolveNextCandidateL" )
        
        TResolveRequest& request( iResolveRequests[reqIndex] );
        request.SetState( TResolveRequest::ERequestOngoing );
        
        CNATFWPluginApi* plugin
            = const_cast<CNATFWPluginApi*>( request.Plugin() );
        ASSERT( NULL != plugin );
        
        TInt error( KErrNone );
        if ( KNatPluginIdNokiaHost() == plugin->PluginIdentifier() )
            {
            // Create connection with desired address family
            TUint addrFamily( request.AddrFamily() );
            TRAP( error, plugin->FetchCandidateL( request.StreamId(),
                RetransmissionTimeOut(), addrFamily ) );
            if ( KErrNone != error )
                {
                PluginEventOccured( plugin, request.StreamId(), 
                    MIceNatPluginEventObs::EFetchingCompleted, error, NULL );
                }
            else
                {
                CIceHostResolver* host
                    = reinterpret_cast<CIceHostResolver*>( plugin );
                iLocalAddr = host->LocalAddress( 
                    request.StreamId(), addrFamily );
                ASSERT( !iLocalAddr.IsUnspecified() );
                }
            }
        else
            {
            TRAP( error, plugin->FetchCandidateL( request.StreamId(), 
                RetransmissionTimeOut(), iLocalAddr ) );
            if ( KErrNone != error )
                {
                PluginEventOccured( plugin, request.StreamId(), 
                    MIceNatPluginEventObs::EFetchingCompleted, error, NULL );
                }
            }
        }
    else
        {
        __ICEDP( "CIceLocalCandidateFinder::ResolveNextCandidateL, READY" )
        
        iTimer->Cancel();
        }
    }


// ---------------------------------------------------------------------------
// CIceLocalCandidateFinder::NewLocalCandidateFound
// ---------------------------------------------------------------------------
//
void CIceLocalCandidateFinder::NewLocalCandidateFound( 
        const CNATFWPluginApi& aPlugin,
        CNATFWCandidate& aLocalCandidate )
    {
    __ICEDP("CIceLocalCandidateFinder::NewLocalCandidateFound")

    TResolveRequest searchItem( aPlugin, aLocalCandidate.StreamId(), 
        TResolveRequest::ERequestOngoing );
    TInt index( iResolveRequests.Find( 
        searchItem, TResolveRequest::MatchStreamStatePlugin ) );
    
    if ( KErrNotFound != index )
        {
        aLocalCandidate.SetStreamCollectionId( 
            iResolveRequests[index].CollectionId() );
        aLocalCandidate.SetComponentId( 
            iResolveRequests[index].ComponentId() );
        
        TRAP_IGNORE( iPrioritizer->PrioritizeL( aLocalCandidate ) )
        }
    else
        {
        ASSERT( EFalse );
        }
    
    TBool redundantCandidate( EFalse );
    TInt error( KErrNone );
    TRAP( error, redundantCandidate 
        = iSessionData.AddLocalCandidateL( &aLocalCandidate ) )
    
    if ( ( !redundantCandidate ) && ( KErrNone == error ) )
        {
        iEventObserver.PluginEventOccured( 
            NULL,
            aLocalCandidate.StreamId(),
            MIceNatPluginEventObs::ELocalCandidateFound,
            KErrNone,
            CNATFWCandidate::NewL( aLocalCandidate ) );
        }
    }


// ---------------------------------------------------------------------------
// CIceLocalCandidateFinder::ResolvingCompleted
// ---------------------------------------------------------------------------
//
TBool CIceLocalCandidateFinder::ResolvingCompleted( TUint aStreamId,
        TUint& aStreamCollectionId, TUint& aComponentId, TBool& aIPv6After )
    {
    __ICEDP( "CIceLocalCandidateFinder::ResolvingCompleted" )
    
    TCounter counter( aStreamId );
    TInt streamInd = iResolvingCnts.Find( counter, TCounter::MatchStreamId );
    ASSERT( KErrNotFound != streamInd );
    
    --iResolvingCnts[streamInd];
    
    TBool completed( iResolvingCnts[streamInd] == 0 );
    if ( completed )
        {
        iResolvingCnts.Remove( streamInd );
        
        TResolveRequest request( aStreamId );
        TInt reqIndex = iResolveRequests.Find( request, 
            TResolveRequest::MatchStreamId );

        while ( KErrNotFound != reqIndex )
            {
            aIPv6After = iResolveRequests[reqIndex].IPv6After();
            if ( aIPv6After )
                {
                aStreamCollectionId
                    = iResolveRequests[reqIndex].CollectionId();
                aComponentId = iResolveRequests[reqIndex].ComponentId();
                }
                
            iResolveRequests.Remove( reqIndex );
            reqIndex = iResolveRequests.Find( request, 
                TResolveRequest::MatchStreamId );
            }
        }
    
    return completed;
    }


// ---------------------------------------------------------------------------
// CIceLocalCandidateFinder::RetransmissionTimeOut
// Calculates RTO while being in gathering phase as specified in ICE-17, 16.
// RTO = MAX (100ms, Ta * (number of pairs)), where the number of pairs
// refers to the number of pairs of candidates with STUN or TURN servers.
// ---------------------------------------------------------------------------
//
TUint CIceLocalCandidateFinder::RetransmissionTimeOut() const
    {
    // resolving requests for host plugin excluded in formula
    TInt numOfHostRequests( 0 );
    
    TInt numOfRequests( iResolveRequests.Count() );
    for ( TInt i = 0; i < numOfRequests; i++ )
        {
        if ( KNatPluginIdNokiaHost() == 
            iResolveRequests[i].Plugin()->PluginIdentifier() )
            {
            numOfHostRequests++;
            }
        }
    
    TInt numOfPairs = iResolveRequests.Count() - numOfHostRequests;
    numOfPairs >= 0 ? numOfPairs = numOfPairs : numOfPairs = 0;
    
    const TInt KMinimumRto( 100 );
    return TUint( Max( KMinimumRto, 
        iSessionData.TaTimerValue() * numOfPairs ) );
    }
