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
* Description:    Fetches available local candidates for a NATFW stream.
*
*/





#ifndef CNATFWLOCALCANDIDATEFINDER_H
#define CNATFWLOCALCANDIDATEFINDER_H

#include <e32base.h>
#include "micenatplugineventobs.h"
#include "icecandidatehandlerdefs.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class CIceSessionData;
class CIceCandidatePrioritizer;
class CPeriodic;
class CIceNatPluginContainer;
class CNATFWCandidate;

/**
 *  Fetches available local candidates for a NATFW stream.
 *
 *  @lib icecandidatehandler.lib
 *  @since S60 v3.2
 */
class CIceLocalCandidateFinder : public CBase, 
                                 public MIceNatPluginEventObs
    {
    
    UNIT_TEST( UT_CIceLocalCandidateFinder )

public:

    /**
     * Two-phased constructor.
     * @param aEventObserver        Observer for plugin events
     * @param aSessionData          Session data
     * @param aPluginContainer      Plugin container
     */
    static CIceLocalCandidateFinder* NewL(
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer );

    /**
     * Two-phased constructor.
     * @param aEventObserver        Observer for plugin events
     * @param aSessionData          Session data
     * @param aPluginContainer      Plugin container
     */
    static CIceLocalCandidateFinder* NewLC(
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer );
    
    /**
     * Destructor.
     */
    virtual ~CIceLocalCandidateFinder();
    
    /**
     * For usage with CPeriodic.
     */
    static TInt ResolveNextCandidate( TAny* aObject );
    
    /**
     * Starts fetching local(host, server reflexive, relay) candidates.
     *
     * @since S60 3.2
     * @param aStreamCollectionId   Stream collection identifier
     * @param aStreamId             Stream identifier
     * @param aComponentId          Stream collection identifier
     * @param aIPv6After            Fetch IPv6 candidates after completion
     */
    void FetchCandidatesL( TUint aStreamCollectionId, TUint aStreamId, 
        TUint aComponentId, TUint aAddrFamily, TBool aIPv6After );
    
// from base class MIceNatPluginEventObs

    void PluginEventOccured( const CNATFWPluginApi* aPlugin, TUint aStreamId,
        TNatPluginEvent aEventCode, TInt aErrorCode, TAny* aEventData,
        TUint aStreamConnectionId = 0, TUint aComponentId = 0, 
        TBool aIPv6After = EFalse );

private:

    CIceLocalCandidateFinder(
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer );

    void ConstructL();
    
    void ScheduleResolveRequestL( const CNATFWPluginApi& aPlugin,
        TUint aCollectionId, TUint aStreamId, TUint aComponentId,
        TUint aAddrFamily, TBool aIPv6After );
    
    void ResolveNextCandidateL();
    
    void NewLocalCandidateFound( const CNATFWPluginApi& aPlugin,
        CNATFWCandidate& aLocalCandidate );
    
    TBool ResolvingCompleted( TUint aStreamId,
        TUint& aStreamCollectionId, TUint& aComponentId, TBool& aIPv6After );
    
    TUint RetransmissionTimeOut() const;
    
    class TResolveRequest
        {
    public:
        
        enum TRequestState
            {
            ERequestNotIssued   = 0,
            ERequestOngoing     = 1,
            ERequestCompleted   = 2
            };
        
        TResolveRequest( const CNATFWPluginApi& aPlugin,
                TUint aCollectionId, TUint aStreamId, 
                TUint aComponentId, const TRequestState& aState,
                TUint aAddrFamily, TBool aIPv6After )
            : 
            iPlugin( &aPlugin ),
            iCollectionId( aCollectionId ), 
            iStreamId( aStreamId ),
            iComponentId( aComponentId ),
            iState( aState ),
            iAddrFamily( aAddrFamily ),
            iIPv6After( aIPv6After ) {}
        
        TResolveRequest( TUint aStreamId )
            :
            iStreamId( aStreamId ) { }

        TResolveRequest( const TRequestState& aState )
            :
            iState( aState ) { }
        
        TResolveRequest( const CNATFWPluginApi& aPlugin, 
                TUint aStreamId, const TRequestState& aState )
            :
            iPlugin( &aPlugin ),
            iStreamId( aStreamId ),
            iState( aState ) { }
        
        static TBool MatchStreamId( 
            const TResolveRequest& aReq1,
            const TResolveRequest& aReq2 )
            {
            return ( aReq1.StreamId() == aReq2.StreamId() );
            }
        
        static TBool MatchState( 
            const TResolveRequest& aReq1,
            const TResolveRequest& aReq2 )
            {
            return ( aReq1.State() == aReq2.State() );
            }
        
        static TBool MatchStreamStatePlugin( 
            const TResolveRequest& aReq1,
            const TResolveRequest& aReq2 )
            {
            return ( 
                ( aReq1.StreamId() == aReq2.StreamId() )
                && ( aReq1.State() == aReq2.State() )
                && ( aReq1.Plugin() == aReq2.Plugin() ) );
            }
        
        const CNATFWPluginApi* Plugin() const { return iPlugin; }
        TUint CollectionId() const { return iCollectionId; }
        TUint StreamId() const { return iStreamId; }
        TUint ComponentId() const { return iComponentId; }
        TRequestState State() const { return iState; }
        void SetState( const TRequestState& aState )  { iState = aState; }
        TUint AddrFamily() const { return iAddrFamily; }
        TBool IPv6After() const { return iIPv6After; }
        
    private:

        TResolveRequest();
        const CNATFWPluginApi* iPlugin;
        TUint iCollectionId;
        TUint iStreamId;
        TUint iComponentId;
        TRequestState iState;
        TUint iAddrFamily;
        TBool iIPv6After;
        };
    
    
private: // data

    /**
     * ICE observer to inform about found candidates.
     * Not own.
     */
    MIceNatPluginEventObs& iEventObserver;
    
    /**
     * The session data.
     * Not own.
     */
    CIceSessionData& iSessionData;

    /**
     * NAT protocol plugin container.
     * Not own.
     */
    CIceNatPluginContainer& iPluginContainer;

    /**
     * Candidate prioritizer.
     * Own.
     */
    CIceCandidatePrioritizer* iPrioritizer;

    /**
     * Timer for pacing plugin resolving.
     * Own.
     */
    CPeriodic* iTimer;
    
    /**
     * Queue for resolve requests.
     * Own.
     */
    RArray<TResolveRequest> iResolveRequests;

    /**
     * Used to keep track ongoing resolving requests on stream basis.
     * Own.
     */
    RArray<TCounter> iResolvingCnts;
    
    /**
     * Local address from multiplexer.
     */
    TInetAddr iLocalAddr;

    /**
     * True if candidate resolving success at least for one type.
     */
    TBool iSucceeded;

    };

#endif // CNATFWLOCALCANDIDATEFINDER_H
