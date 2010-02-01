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




#ifndef C_CICECHECKLIST_H
#define C_CICECHECKLIST_H

#include <e32base.h>
#include <badesca.h>
#include "miceconnchecklistener.h"
#include "ticecheckqueitem.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class CIceConnectivityCheck;
class CIceCheckPrioritizer;
class CIceSessionData;
class CNATFWCandidate;
class CNATFWCandidatePair;
class CIceCandidatePrioritizer;
class MIceChecklistObserver;
class CIceConnectionHandler;
class TInetAddr;
class CIceValidList;

/**
 *  ICE check list for a stream collection (e.g. SDP media stream).
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceCheckList : public CBase, public MIceConnCheckListener
    {

    UNIT_TEST( UT_CIceCheckList )
    UNIT_TEST( UT_CIceCheckHandler )
    
public:

    /**  ICE check list states */
    enum TIceCheckListState
        {
        EIceCheckListRunning        = 1,
        EIceCheckListCompleted      = 2,
        EIceCheckListFailed         = 3
        };

    /**
     * Two-phased constructor.
     * @param   aClient                 The client
     * @param   aStreamCollectionId     The stream collection
     * @param   aSessionData            The session data
     * @param   aConnHandler            The connection handler
     */
    static CIceCheckList* NewL( 
        MIceChecklistObserver& aClient,
        TUint aStreamCollectionId,
        CIceSessionData& aSessionData,
        CIceConnectionHandler& aConnHandler );

    /**
     * Two-phased constructor.
     * @param   aClient                 The client
     * @param   aStreamCollectionId     The stream collection
     * @param   aSessionData            The session data
     * @param   aConnHandler            The connection handler
     */
    static CIceCheckList* NewLC( 
        MIceChecklistObserver& aClient,
        TUint aStreamCollectionId,
        CIceSessionData& aSessionData,
        CIceConnectionHandler& aConnHandler );

    /**
     * Destructor.
     */
    virtual ~CIceCheckList();

    /**
     * For usage with CPeriodic.
     */
    static TInt TimerFired( TAny* aObject );

    /**
     * Return state of the checklist.
     * 
     * @since   S60 v3.2
     * @return  Sate of the checklist
     */
    TIceCheckListState State() const;

    /**
     * Returns stream collection id for the check list.
     * 
     * @since   S60 v3.2
     * @return  Stream collection identifier
     */
    TUint StreamCollectionId() const;

    /**
     * Prepares check list before candidate pair checks are started.
     * Used with check list corresponding first SDP media line (ICE 5.7 )
     *
     * @since   S60 v3.2
     */
    void InitializeCheckListL();

    /**
     * Prepares check list before candidate pair checks are started.
     * Used for activation after check list for first media line has
     * completed first check for the last media component.
     *
     * @since   S60 v3.2
     * @param   aFoundation     The foundation to match
     */
    void InitializeCheckListL( const TDesC8& aFoundation );

    /**
     * Starts perform periodic connectivity checks.
     *
     * @since   S60 v3.2
     * @param   anInterval      The time interval for pacing checks
     */
    void StartPerformChecksL( 
        const TTimeIntervalMicroSeconds32& anInterval );
    
    /**
     * Restarts check list (due to ICE restart).
     *
     * @since   S60 v3.2
     * @param   anInterval      The time interval for pacing checks     
     */
    void RestartCheckListL( const TTimeIntervalMicroSeconds32& anInterval );
     
    /**
     * Updates check list with peer selected candidate pairs. ICE processing
     * is completed with help of new information.
     * 
     * @since   S60 v3.2
     * @param   aPeerSelectedPairs  Peer selected candidate pairs
     */
    void UpdateCheckListL( 
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs );
    
    /**
     * Updates check list with updated set of remote candidates. 
     * ICE processing is completed with help of new information.
     * 
     * @since   S60 v3.2
     * @param   aRemoteCands        All remote candidates known currently
     */
    void UpdateCheckListL( 
        RPointerArray<CNATFWCandidate>& aRemoteCands );
    
    /**
     * Called when incoming STUN request has come.
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
     * Indicates whether relay is used for some selected pair.
     *
     * @since   S60 v3.2
     * @return  ETrue if relay is used
     */
    TBool RelayUsedForSelectedPairL() const;
    
    /**
     * Recomputes pair priorities according to the new role.
     *
     * @since   S60 v3.2
     */
    void RecomputePairPriorities();

// from base class MIceConnCheckListener

    /**
     * From MIceConnCheckListener.
     * Notifies result of a check.
     *
     * @since   S60 v3.2
     * @param   aCompletionCode     The completion code
     * @param   aCheck              The check which was completed
     * @param   aValidatedPair      Candidate pair validated by the check
     */
    void CheckCompletedL( 
        TInt aCompletionCode,
        const CIceConnectivityCheck& aCheck,
        CNATFWCandidatePair* aValidatedPair );

    /**
     * From MIceConnCheckListener.
     * Notifies result of a nomination check.
     *
     * @since   S60 v3.2
     * @param   aCompletionCode     The completion code
     * @param   aCheck              The check
     * @param   aValidatedPair      Candidate pair validated by the check
     */
    void NominationCompletedL( 
        TInt aCompletionCode,
        const CIceConnectivityCheck& aCheck,
        const CNATFWCandidatePair& aValidatedPair );
    
private:

    CIceCheckList(
        MIceChecklistObserver& aClient,
        TUint aStreamCollectionId,
        CIceSessionData& aSessionData,
        CIceConnectionHandler& aConnHandler );

    void ConstructL();

    void GenerateCheckListL( 
        RPointerArray<CIceConnectivityCheck>& aCheckList );
    
    void PairCandidatesL(
        RPointerArray<CIceConnectivityCheck>& aCheckList,
        RPointerArray<CNATFWCandidate>& aLocalCandidates,
        RPointerArray<CNATFWCandidate>& aRemoteCandidates );
    
    TBool CompatibleCandidates( const CNATFWCandidate& aCand1, 
        const CNATFWCandidate& aCand2 ) const;
    
    void PruneChecksL( RPointerArray<CIceConnectivityCheck>& aChecks );
    
    void ReplaceReflexiveCandidatesWithBaseL( 
        RPointerArray<CIceConnectivityCheck>& aChecks );
    
    void OnTimerFiredL();
    
    TUint PriorityForPeerReflCandL( 
        const CNATFWCandidate& aLocalCandidate ) const;
        
    void HandleTriggeredCheckL( const CNATFWCandidatePair& aPair );
    
    void UpdateCheckListStateL();
    
    void RemovePendingChecks( TUint aComponentId );
    
    void GetComponentIdsL( RArray<TUint>& aComponentIds ) const;
    
    TInt FindCheckInState( TInt aStateMask );
    
    TInt FindCheckInState( TInt aStateMask, TUint aComponentId );
    
    TInt FindCheckInState( TInt aStateMask, const TInetAddr& aRemoteAddr );
    
    TInt FindCheck( const CNATFWCandidatePair& aPair );
    
    void SetStateL( TIceCheckListState aState );
    
    void DoLosingPairProcessingL( 
        const RPointerArray<CNATFWCandidatePair>& aSelectedPairs );
    
    void NominateCandidatePairsL();
    
    TBool CheckListFrozen();
    
    void EnqueueCheckL( CIceConnectivityCheck& aCheck,
        CIceConnectivityCheck::TIceCheckType aType, TUint aLocalPriority );
    
    TBool DequeueCheckL();
    
    TUint RetransmissionTimeOut() const;

    CDesC8ArrayFlat* FoundationsForCollectionL( TUint aStreamCollectionId ) const;
    
    TUint LowestComponentId( RArray<TUint>& aComponentIds ) const;
    
    TBool NominationExecuted() const;
    
    void InitializeCheckList( const TDesC8& aFoundation, 
        TInt aExcludedComponentId );
        
private: // data

    /**
     * ICE check list state.
     */
    TIceCheckListState iState;

    /**
     * Stream collection for which this check list is.
     */
    TUint iStreamCollectionId;
    
    /**
     * Timer interval for pacing checks.
     */
    TTimeIntervalMicroSeconds32 iInterval;
    
    /**
     * Client notified about a result of the check.
     * Not own.
     */
    MIceChecklistObserver& iClient;
    
    /**
     * The session data.
     * Not own.
     */
    CIceSessionData& iSessionData;
    
    /**
     * Connection handler.
     * Not own.
     */
    CIceConnectionHandler& iConnHandler;

    /**
     * Ordered list of checks to be executed.
     * Own.
     */
    RPointerArray<CIceConnectivityCheck> iChecks;
    
    /**
     * Check queue for congestion control purposes.
     * Own.
     */
    RArray<TIceCheckQueItem> iCheckQue;
    
    /**
     * Prioritizer for the checks.
     * Own.
     */
    CIceCheckPrioritizer* iCheckPrioritizer;
    
    /**
     * Prioritizer for the local candidates.
     * Own.
     */
    CIceCandidatePrioritizer* iCandPrioritizer;

    /**
     * Timer for connectivity check triggering.
     * Own.
     */
    CPeriodic* iTimer;
    
    /**
     * Valid list.
     * Own.
     */
    CIceValidList* iValidList;

    /**
     * Component identifiers.
     * Own.
     */
    RArray<TUint> iComponentIds;

    /**
     * Remote selected candidate pairs.
     * Own.
     */
    RPointerArray<CNATFWCandidatePair> iSelectedPairs;
    };


#endif // C_CICECHECKLIST_H
