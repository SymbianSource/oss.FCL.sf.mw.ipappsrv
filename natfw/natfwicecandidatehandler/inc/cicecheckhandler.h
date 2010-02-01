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




#ifndef C_CICECHECKHANDLER_H
#define C_CICECHECKHANDLER_H

#include <e32base.h>
#include "natfwconnectivityapidefs.h"
#include <in_sock.h>
#include "natfwstunsrvobserver.h"
#include "micechecklistobserver.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class CIceSessionData;
class CIceCheckList;
class CNATFWSTUNSrvClientSession;
class MNcmConnectionMultiplexer;
class CIceConnectionHandler;
class CIceCandidatePrioritizer;
class MIceNatPluginEventObs;
class CNATFWCredentials;
class CNATFWCandidate;
class CNATFWCandidatePair;

/**
 *  Manages check lists and listens to incoming checks.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceCheckHandler
        : 
        public CBase, 
        public MNATFWStunSrvObserver,
        public MIceChecklistObserver
    {

    UNIT_TEST( UT_CIceCheckHandler )
   
public:

    /**  ICE processing state across all media streams */
    enum TIceProcessingState
        {
        EIceRunning     = 1,    // There is checks in progress
        EIceCompleted   = 2     // All check lists has completed
        };

    /**
     * Two-phased constructor.
     * @param   aSessionData        Session data
     * @param   aMultiplexer        Multiplexer
     * @param   aConnHandler        Connection handler
     * @param   aEventObserver      Observer for events
     */
    static CIceCheckHandler* NewL( 
        CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer,
        CIceConnectionHandler& aConnHandler,
        MIceNatPluginEventObs& aEventObserver );

    /**
     * Two-phased constructor.
     * @param   aSessionData        Session data
     * @param   aMultiplexer        Multiplexer
     * @param   aConnHandler        Connection handler
     * @param   aEventObserver      Observer for events
     */
    static CIceCheckHandler* NewLC(
        CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer,
        CIceConnectionHandler& aConnHandler,
        MIceNatPluginEventObs& aEventObserver );

    /**
     * Destructor.
     */
    virtual ~CIceCheckHandler();

    /** 
     * Sets local or remote credentials. Local credentials are needed for
     * incoming STUN-request authentication.
     *
     * @since   S60 v3.2
     * @param   aCredentials        The credentials
     */
    void SetCredentialsL( const CNATFWCredentials& aCredentials );
    
    /**
     * Start perform connectivity checks. Local & remote candidates and
     * credentials for them must be stored before starting.
     *
     * @since   S60 v3.2
     */
    void PerformConnectivityChecksL();

    /**
     * Updates ICE processing with peer selected candidate pairs.
     * 
     * @since   S60 v3.2
     * @param   aPeerSelectedPairs  Peer selected candidate pairs
     */
    void UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs );
    
    /**
     * Updates ICE processing with updated set of remote candidates.
     * 
     * @since   S60 v3.2
     * @param   aRemoteCands        All remote candidates known currently
     */
    void UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidate>& aRemoteCands );
    
    /**
     * Cleanups all data related to the specified stream collection.
     *
     * @since   S60 v3.2
     * @param   aCollectionId       The collection identifier
     */
    void CleanupCollectionData( TUint aCollectionId );

// from base class MNATFWStunSrvObserver

    /**
     * From MNATFWStunSrvObserver.
     * Called by NAT FW STUN server when incoming STUN request has come.
     * If aFromAddr == aPeerAddr relay isn't used.
     *
     * @since   S60 v3.2
     * @param   aLocalAddr          To which local address request came
     * @param   aFromAddr           Where request came from as seen by socket
     * @param   aPeerAddr           Real remote address where request came from
     * @param   aPriority           Priority from request
     * @param   aRemoteFavored      Whether candidate pair checked is favored
                                    by remote peer
     */
    void STUNRequestReceivedL( const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, const TInetAddr& aPeerAddr,
        TUint aPriority, TBool aRemoteFavored );
        
    /**
     * From MNATFWStunSrvObserver.
     * Called by NATFW STUN Server if role conflict is present between agents
     * and local agent needs to change its role.
     *
     * @since   S60 v3.2
     * @param   aDesiredRole        Desired role
     */
    void RoleChangeNeeded( TNATFWIceRole aDesiredRole );

// from base class MIceChecklistObserver

    /**
     * From MIceChecklistObserver.
     * Indicates that check list has completed, either succesfully or not.
     *
     * @since   S60 v3.2
     * @param   aChecklist      The checklist which was completed
     * @param   aSelectedPairs  The selected pairs for components
     */
    void ChecklistCompletedL( 
        const CIceCheckList& aChecklist, 
        const RPointerArray<CNATFWCandidatePair>& aSelectedPairs );

    /**
     * From MIceChecklistObserver.
     * Indicates that check list has completed, either succesfully or not.
     *
     * @since   S60 v3.2
     * @param   aChecklist      The checklist which was completed
     * @param   aFailedComps    The failed components
     * @param   aErrCode        The completion code
     */
    void ChecklistCompletedL( 
        const CIceCheckList& aChecklist,
        const RArray<TUint>& aFailedComps,
        TInt aErrCode );
    
    /**
     * From MIceChecklistObserver.
     * Indicates that those checks in other checklists, which have same
     * foundation could be now unfrozen as an optimization.
     *
     * @since   S60 v3.2
     * @param   aChecklist      The checklist reporting event
     * @param   aFoundation     The foundation
     */
    void ComponentsHaveValidPairsL( 
        const CIceCheckList& aChecklist, const TDesC8& aFoundation );

private:

    CIceCheckHandler( 
        CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer,
        CIceConnectionHandler& aConnHandler,
        MIceNatPluginEventObs& aEventObserver );

    void ConstructL();
    
    void FormChecklistsL();
    
    CIceCheckList* ChecklistByCollectionId( TUint aStreamCollId );

    CIceCheckList* ChecklistByCollectionIdL( TUint aStreamCollId );
    
    void SetState( TIceProcessingState aState );
    
    void HandleNewCollectionsL();
    
    void HandleIceRestartsL();
    
    TInt NumOfActiveCheckLists();
    
    void UpdateStunServerParamsL();
    
    void ExecuteBufferedTriggeredChecksL();
    
    void UnFreezeCheckListsL(
        const CIceCheckList& aExcludedList, const TDesC8& aFoundation );
    
    void UpdateICEProcessingStateL( const CIceCheckList& aCheckList );
    
    class TIceTriggeredCheckInfo
        {
        public:
            TIceTriggeredCheckInfo( const TInetAddr& aLocalAddr,
                const TInetAddr& aFromAddr, const TInetAddr& aPeerAddr,
                TUint aPriority, TBool aRemoteFavored )
                :
                iLocalAddr( aLocalAddr ),
                iFromAddr( aFromAddr ),
                iPeerAddr( aPeerAddr ),
                iPriority( aPriority ),
                iRemoteFavored( aRemoteFavored ) { }
        
            const TInetAddr& LocalAddr() const  { return iLocalAddr; }
            const TInetAddr& FromAddr() const { return iFromAddr; }
            const TInetAddr& PeerAddr() const { return iPeerAddr; }
            TUint Priority() const { return iPriority; }
            TBool IsRemoteFavored() const { return iRemoteFavored; }
        
        private:
        
            TInetAddr iLocalAddr;
            TInetAddr iFromAddr;
            TInetAddr iPeerAddr;
            TUint iPriority;
            TBool iRemoteFavored;
        };
    
private: // data

    /**
     * ICE processing state across all media streams.
     */
    TIceProcessingState iState;

    /**
     * The session data.
     * Not own.
     */
    CIceSessionData& iSessionData;

    /**
     * The multiplexer.
     * Not own.
     */
    MNcmConnectionMultiplexer& iMultiplexer;

    /**
     * The connection handler.
     * Not own.
     */
    CIceConnectionHandler& iConnHandler;

    /**
     * Event observer for NAT-plugin events.
     * Not own.
     */
    MIceNatPluginEventObs& iEventObserver;

    /**
     * The checklists.
     * Own.
     */
    RPointerArray<CIceCheckList> iCheckLists;

    /**
     * STUN server instance used for STUN request receiving.
     * Own.  
     */
    CNATFWSTUNSrvClientSession* iStunSrv;

    /**
     * Current credentials for ICE restart detection.
     * Own.  
     */
    RPointerArray<CNATFWCredentials> iCurCredentials;

    /**
     * Buffered triggered checks.
     * Own.  
     */
    RArray<TIceTriggeredCheckInfo> iBufferedChecks;
    };

#endif // C_CICECHECKHANDLER_H
