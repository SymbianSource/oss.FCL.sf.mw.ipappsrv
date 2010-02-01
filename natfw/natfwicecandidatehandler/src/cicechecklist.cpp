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




#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "cicechecklist.h"
#include "cicecheckprioritizer.h"
#include "cicecandidateprioritizer.h"
#include "ciceconnectivitycheck.h"
#include "cicesessiondata.h"
#include "micechecklistobserver.h"
#include "cicevalidlist.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlerlogs.h"

const TUint KComponentIdRtp = 1;

// ======== MEMBER FUNCTIONS ========

CIceCheckList::CIceCheckList(
        MIceChecklistObserver& aClient,
        TUint aStreamCollectionId,
        CIceSessionData& aSessionData,
        CIceConnectionHandler& aConnHandler )
    :
    iStreamCollectionId( aStreamCollectionId ),
    iClient( aClient ),
    iSessionData( aSessionData ),
    iConnHandler( aConnHandler )
    {
    }


void CIceCheckList::ConstructL()
    {
    iCheckPrioritizer = CIceCheckPrioritizer::NewL();
    iCandPrioritizer = CIceCandidatePrioritizer::NewL( 
        iSessionData.Domain() );
    iTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    iValidList = CIceValidList::NewL( iStreamCollectionId );
    GenerateCheckListL( iChecks );
    }


CIceCheckList* CIceCheckList::NewL(
        MIceChecklistObserver& aClient,
        TUint aStreamCollectionId,
        CIceSessionData& aSessionData,
        CIceConnectionHandler& aConnHandler )
    {
    CIceCheckList* self 
        = CIceCheckList::NewLC( aClient, aStreamCollectionId, 
        aSessionData, aConnHandler );
    CleanupStack::Pop( self );
    return self;
    }


CIceCheckList* CIceCheckList::NewLC(
        MIceChecklistObserver& aClient,
        TUint aStreamCollectionId,
        CIceSessionData& aSessionData,
        CIceConnectionHandler& aConnHandler )
    {
    CIceCheckList* self 
        = new( ELeave ) CIceCheckList( 
        aClient, aStreamCollectionId, aSessionData, aConnHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CIceCheckList::~CIceCheckList()
    {
    __ICEDP( "CIceCheckList::~CIceCheckList" )
    
    iChecks.ResetAndDestroy();
    iCheckQue.Close();
    delete iCheckPrioritizer;
    delete iCandPrioritizer;
    delete iTimer;
    delete iValidList;
    iComponentIds.Close();
    iSelectedPairs.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CIceCheckList::TimerFired
// ---------------------------------------------------------------------------
//
TInt CIceCheckList::TimerFired( TAny* aObject )
    {
    if ( aObject )
        {
        CIceCheckList* list = reinterpret_cast<CIceCheckList*>( aObject );
        TRAPD( error, list->OnTimerFiredL() )
        if ( error )
            {
            __ICEDP_INT1( "CIceCheckList::TimerFired, ERR:", error )
            if ( KErrNoMemory == error )
                {
                TRAP_IGNORE( list->SetStateL( EIceCheckListFailed ) )
                }
            else
                {
                ASSERT( EFalse );
                }
            }
        
        return 1;
        }
    else
        {
        __ICEDP( "CIceCheckList::TimerFired, ELSE" )
        ASSERT( EFalse );
        return 0;   // lint #527
        }
    }


// Non-derived function

// ---------------------------------------------------------------------------
// CIceCheckList::State
// ---------------------------------------------------------------------------
//
CIceCheckList::TIceCheckListState CIceCheckList::State() const
    {
    return iState;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::StreamCollectionId()
// ---------------------------------------------------------------------------
//
TUint CIceCheckList::StreamCollectionId() const
    {
    return iStreamCollectionId;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::InitializeCheckListL()
// Used to initialize check list corresponding first SDP mediastream.
// ICE-17, section 5.7.4.
// ---------------------------------------------------------------------------
//
void CIceCheckList::InitializeCheckListL()
    {
    __ICEDP( "CIceCheckList::InitializeCheckListL" )
    __ASSERT_DEBUG( iChecks.Count(), User::Leave( KErrNotReady ) );
    
    TUint lowestComponentId = LowestComponentId( iComponentIds );
    
    CDesC8ArrayFlat* foundations 
        = FoundationsForCollectionL( iStreamCollectionId );
    
    TInt foundationCount( foundations->Count() );
    for ( TInt i(0); i < foundationCount; ++i )
        {
        // inside foundation group set lowest component id waiting
        TInt checkIndex( iChecks.Count() );
        TBool checkInitialized( EFalse );
        
        while ( ( checkIndex-- ) && ( !checkInitialized ) )
            {
            CIceConnectivityCheck& check( *iChecks[checkIndex] );
            if ( check.CandidatePair().Foundation() 
                    == foundations->MdcaPoint(i)
                && check.ComponentId() == lowestComponentId )
                {
                check.Initialize();
                checkInitialized = ETrue;
                }
            }
        }
    
    delete foundations;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::InitializeCheckListL
// ICE-17, 7.1.2.2.3, Updating Pair States.
// ---------------------------------------------------------------------------
//
void CIceCheckList::InitializeCheckListL( const TDesC8& aFoundation )
    {
    __ICEDP( "CIceCheckList::InitializeCheckListL" )
    __ASSERT_DEBUG( iChecks.Count(), User::Leave( KErrNotReady ) );
    
    TBool initComplete( EFalse );
    TInt count( iChecks.Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        if ( CIceConnectivityCheck::EIceCheckFrozen == iChecks[i]->State()
            && iChecks[i]->CandidatePair().Foundation() == aFoundation )
            {
            iChecks[i]->Initialize();
            initComplete = ETrue;
            }
        }
    
    if ( !initComplete && CheckListFrozen() )
        {
        InitializeCheckListL();
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::StartPerformChecksL()
// ---------------------------------------------------------------------------
//
void CIceCheckList::StartPerformChecksL( 
        const TTimeIntervalMicroSeconds32& anInterval )
    {
    __ICEDP( "CIceCheckList::StartPerformChecksL" )
    
    iInterval = anInterval;
    SetStateL( EIceCheckListRunning );
    }


// ---------------------------------------------------------------------------
// CIceCheckList::RestartCheckListL()
// ---------------------------------------------------------------------------
//
void CIceCheckList::RestartCheckListL(
        const TTimeIntervalMicroSeconds32& anInterval )
    {
    __ICEDP( "CIceCheckList::RestartCheckListL" )
    __ASSERT_ALWAYS( EIceCheckListCompleted == State(), 
        User::Leave( KErrNotReady ) );
    
    iValidList->FlushValidList();
    iChecks.ResetAndDestroy();
    GenerateCheckListL( iChecks );
    
    iInterval = anInterval;
    SetStateL( EIceCheckListRunning );
    }


// ---------------------------------------------------------------------------
// CIceCheckList::UpdateCheckListL()
// Controlling peer has selected candidate pairs to be used.
// ICE-17, 9.2.2.3. Existing Media Streams and remote-candidates.
// ---------------------------------------------------------------------------
//
void CIceCheckList::UpdateCheckListL( 
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs )
    {
    __ICEDP( "CIceCheckList::UpdateIceProcessingL" )
    __ASSERT_DEBUG( EIceCheckListRunning == State(), 
        User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( !iSelectedPairs.Count(), 
        User::Leave( KErrAlreadyExists ) );
    
    TInt numOfSelectedComps( aPeerSelectedPairs.Count() );
    for ( TInt i( 0 ); i < numOfSelectedComps; ++i )
        {
        CNATFWCandidatePair* pair 
            = CNATFWCandidatePair::NewLC( *aPeerSelectedPairs[i] );
        iSelectedPairs.AppendL( pair );
        CleanupStack::Pop( pair );
        }
    
    DoLosingPairProcessingL( iSelectedPairs );
    UpdateCheckListStateL();
    }


// ---------------------------------------------------------------------------
// CIceCheckList::UpdateCheckListL()
// Peer has added new remote candidates to the session negotiation.
// ICE-17, 9.3.1.4. ICE Continuing for Existing Media Stream.
// ---------------------------------------------------------------------------
//
void CIceCheckList::UpdateCheckListL( 
        RPointerArray<CNATFWCandidate>& /*aRemoteCands*/ )
    {
    __ICEDP( "CIceCheckList::UpdateIceProcessingL" )
    __ASSERT_DEBUG( EIceCheckListRunning == State(), 
        User::Leave( KErrNotReady ) );
    
    RPointerArray<CIceConnectivityCheck> newCheckList;
    CleanupResetAndDestroyPushL( newCheckList );
    GenerateCheckListL( newCheckList );
    
    // Add new checks to the checklist
    for ( TInt i( newCheckList.Count() - 1 ); i >= 0; --i )
        {
        CIceConnectivityCheck* newCheck( newCheckList[i] );
        
        TInt ind = iChecks.Find( 
            newCheck, CIceConnectivityCheck::MatchAddresses );
        
        // All new checks are in frozen state initially
        if ( KErrNotFound == ind )
            {
            iChecks.InsertInOrderL( newCheck,
                CIceConnectivityCheck::ComparePriorities );
            newCheckList.Remove( i );
            }
        }
    
    TBool rtpComponentFound( EFalse );
    TInt ind( iChecks.Count() - 1 );
    while( ( !rtpComponentFound ) && ( 0 <= ind ) )
        {
        if ( KComponentIdRtp == iChecks[ind]->ComponentId()
            && CIceConnectivityCheck::EIceCheckSucceed 
                == iChecks[ind]->State() )
            {
            rtpComponentFound = ETrue;
            InitializeCheckList( 
                iChecks[ind]->CandidatePair().Foundation(), 
                KComponentIdRtp );
            
            if ( iValidList->HasPairForComponents( iComponentIds ) )
                {
                // Start checks in other check lists for checks of same type
                iClient.ComponentsHaveValidPairsL( 
                    *this, iChecks[ind]->CandidatePair().Foundation() );
                }
            }
        
        ind--;
        }
    
    CleanupStack::PopAndDestroy( &newCheckList );
    }


// ---------------------------------------------------------------------------
// CIceCheckList::GenerateCheckListL()
// ---------------------------------------------------------------------------
//
void CIceCheckList::GenerateCheckListL( 
        RPointerArray<CIceConnectivityCheck>& aCheckList )
    {
    __ICEDP( "CIceCheckList::GenerateCheckListL" )
    
    RPointerArray<CNATFWCandidate> localCandidates;
    RPointerArray<CNATFWCandidate> remoteCandidates;
    CleanupClosePushL( localCandidates );
    CleanupClosePushL( remoteCandidates );
    
    iSessionData.GetLocalCandidates(
        iStreamCollectionId,
        localCandidates );
    
    iSessionData.GetRemoteCandidatesL(
        iStreamCollectionId,
        remoteCandidates );
    
    __ASSERT_ALWAYS( localCandidates.Count() && remoteCandidates.Count(),
        User::Leave( KErrNotReady ) );
    
    PairCandidatesL( aCheckList, localCandidates, remoteCandidates );
    CleanupStack::PopAndDestroy( &remoteCandidates );
    CleanupStack::PopAndDestroy( &localCandidates );
    
    iCheckPrioritizer->PrioritizeChecks( aCheckList, iSessionData.Role() );
    aCheckList.Sort( CIceConnectivityCheck::ComparePriorities );
    PruneChecksL( aCheckList );
    
#ifdef _DEBUG
    __ICEDP( "CIceCheckList::GenerateCheckListL, LIST AFTER PRUNING" )
    TInt count( aCheckList.Count() );
    for ( TInt i(0); i < count; ++i )
        {
        __ICEDP_ADDRLOG( "LOCAL_ADDR", 
            aCheckList[i]->CandidatePair().LocalCandidate().TransportAddr() )
        
        __ICEDP_ADDRLOG( "PEER_ADDR", 
            aCheckList[i]->CandidatePair().RemoteCandidate().TransportAddr() )
        }
#endif
    
    GetComponentIdsL( iComponentIds );
    }


// ---------------------------------------------------------------------------
// CIceCheckList::PairCandidatesL
// ---------------------------------------------------------------------------
//
void CIceCheckList::PairCandidatesL(
    RPointerArray<CIceConnectivityCheck>& aCheckList,
    RPointerArray<CNATFWCandidate>& aLocalCands,
    RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    __ICEDP( "CIceCheckList::PairCandidatesL" )
    
    CNATFWCandidate* lCand( NULL );
    CNATFWCandidate* rCand( NULL );
    
    TInt lInd = aLocalCands.Count();
    while ( lInd-- )
        {
        lCand = aLocalCands[lInd];
        
        TInt rInd = aRemoteCands.Count();
        while ( rInd-- )
            {
            rCand = aRemoteCands[rInd];
            
            if ( CompatibleCandidates( *lCand, *rCand ) )
                {
                CIceConnectivityCheck* check
                    = CIceConnectivityCheck::NewLC( *this, *lCand, *rCand, 
                    iConnHandler, iSessionData );
                
                aCheckList.AppendL( check );
                CleanupStack::Pop( check );
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::CompatibleCandidates
// According to ICE-17, 5.7.1. and ICE-TCP-03, 4.1.
// ---------------------------------------------------------------------------
//
TBool CIceCheckList::CompatibleCandidates( 
        const CNATFWCandidate& aCand1, const CNATFWCandidate& aCand2 ) const
    {
    __ICEDP( "CIceCheckList::CompatibleCandidates" )
    
    TUint protocol( aCand1.TransportProtocol() );
    if ( protocol != aCand2.TransportProtocol()
        || aCand1.TransportAddr().Family() != aCand2.TransportAddr().Family()
        || aCand1.ComponentId() != aCand2.ComponentId() )
        {
        return EFalse;
        }
    
    if ( KProtocolInetUdp == protocol )
        {
        return ETrue;
        }
    else
        {
        // we have not TCP support yet
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::PruneChecksL
// ICE-17, 5.7.3 Pruning the pairs.
// Procedures of ICE-TCP-03, 4.1. to be added.
// ---------------------------------------------------------------------------
//
void CIceCheckList::PruneChecksL( 
        RPointerArray<CIceConnectivityCheck>& aChecks )
    {
    __ICEDP_INT1( "CIceCheckList::PruneChecksL start, COUNT:", aChecks.Count() )
    
    // Replace server reflexive local candidate of the check by corresponding
    // base candidate.
    ReplaceReflexiveCandidatesWithBaseL( aChecks );
    
    // Remove redundant checks.
    for ( TInt i( aChecks.Count() - 1 ); 0 <= i; --i )
        {
        TInt matchInd = aChecks.Find( aChecks[i],
            CIceConnectivityCheck::MatchAddresses );
        
        if ( KErrNotFound != matchInd && matchInd != i )
            {
            delete aChecks[i];
            aChecks.Remove( i );
            }
        }
    
    __ICEDP_INT1( "CIceCheckList::PruneChecksL end, COUNT:", aChecks.Count() )
    }


// ---------------------------------------------------------------------------
// CIceCheckList::ReplaceReflexiveCandidatesWithBaseL
// ---------------------------------------------------------------------------
//
void CIceCheckList::ReplaceReflexiveCandidatesWithBaseL( 
        RPointerArray<CIceConnectivityCheck>& aChecks )
    {
    for ( TInt i( aChecks.Count() - 1 ); i >= 0; --i )
        {
        CNATFWCandidate& localCand = const_cast<CNATFWCandidate&>
            ( aChecks[ i ]->CandidatePair().LocalCandidate() );
        
        if ( CNATFWCandidate::EServerReflexive == localCand.Type() )
            {
            TInt numOfChecks( aChecks.Count() );
            for ( TInt j( 0 ); j < numOfChecks; ++j )
                {
                const CNATFWCandidate& baseCand
                     = aChecks[ j ]->CandidatePair().LocalCandidate();
                if ( CNATFWCandidate::EHost == baseCand.Type()
                    && TIceUtils::MatchAddresses( 
                        localCand.Base(), baseCand.Base() ) )
                    {
                    localCand.SetTransportAddrL( baseCand.Base() );
                    localCand.SetType( CNATFWCandidate::EHost );
                    localCand.SetFoundationL( baseCand.Foundation() );
                    localCand.SetPriority( baseCand.Priority() );
                    }
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::OnTimerFiredL
// If triggered check queue contains items, top-most triggered check is sent.
// Otherwise an ordinary check is sent. ICE-17, 5.8. Scheduling checks.
// ---------------------------------------------------------------------------
//
void CIceCheckList::OnTimerFiredL()
    {
    __ICEDP( "CIceCheckList::OnTimerFiredL" )
    
    TBool found = DequeueCheckL();
    if ( found )
        {
        return;
        }
    
    // Find highest priority check in waiting state
    TInt ind = FindCheckInState( CIceConnectivityCheck::EIceCheckWaiting );
    if ( KErrNotFound != ind )
        {
        TUint priority = PriorityForPeerReflCandL( 
            iChecks[ind]->CandidatePair().LocalCandidate() );
        iChecks[ind]->PerformConnCheckL( 
            CIceConnectivityCheck::EIceCheckTypePeriodic, priority,
            RetransmissionTimeOut() );
        
        return;
        }
    
    ind = FindCheckInState( CIceConnectivityCheck::EIceCheckFrozen );
    if ( KErrNotFound != ind )
        {
        iChecks[ind]->Initialize();
        TUint priority = PriorityForPeerReflCandL( 
            iChecks[ind]->CandidatePair().LocalCandidate() );
        iChecks[ind]->PerformConnCheckL( 
            CIceConnectivityCheck::EIceCheckTypePeriodic, priority,
            RetransmissionTimeOut() );
        
        return;
        }
    
    iTimer->Cancel();
    }


// ---------------------------------------------------------------------------
// CIceCheckList::PriorityForPeerReflCandL
// ---------------------------------------------------------------------------
//
TUint CIceCheckList::PriorityForPeerReflCandL( 
        const CNATFWCandidate& aLocalCandidate ) const
    {
    __ICEDP( "CIceCheckList::PriorityForTentativePeerReflCandL" )
    
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC( aLocalCandidate );
    candidate->SetType( CNATFWCandidate::EPeerReflexive );
    iCandPrioritizer->PrioritizeL( *candidate );
    TUint priority( candidate->Priority() );
    CleanupStack::PopAndDestroy( candidate );
    
    return priority;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::STUNRequestReceivedL
// Resolve local and remote candidates and set up them for actual handler.
// ---------------------------------------------------------------------------
//
void CIceCheckList::STUNRequestReceivedL( const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, const TInetAddr& aPeerAddr,
        TUint aPriority, TBool aRemoteFavored )
    {
    __ICEDP( "CIceCheckList::STUNRequestReceivedL" )
    if ( ( State() != EIceCheckListRunning ) 
        && ( State() != EIceCheckListCompleted ) )
        {
        SetStateL( EIceCheckListRunning );
        }
    
    // Find real local candidate where request was sent to
    const CNATFWCandidate* localCand = NULL;
    TBool relayUsed = !TIceUtils::MatchAddresses( aFromAddr, aPeerAddr );
    if ( relayUsed )
        {
        __ICEDP( "CIceCheckList::STUNRequestReceivedL, RELAY USED" )
        
        // Find out media component type (RTP/RTCP)
        localCand = iSessionData.FindLocalCandidate( aLocalAddr );
        TUint componentId = localCand->ComponentId();
        localCand = iSessionData.FindLocalCandidate( 
            iStreamCollectionId, componentId, CNATFWCandidate::ERelay );
        }
    else
        {
        localCand = iSessionData.FindLocalCandidate( aLocalAddr );
        }
    
    __ASSERT_ALWAYS( NULL != localCand, User::Leave( KErrNotFound ) );
    
    const CNATFWCandidate* remoteCand 
        = iSessionData.FindRemoteCandidate( aPeerAddr );
    CNATFWCandidate* newRemoteCand = NULL;
    if ( remoteCand )
        {
        newRemoteCand = CNATFWCandidate::NewLC( *remoteCand );
        }
    else
        {
        // ICE-17, 7.2.1.3.  Learning Peer Reflexive (remote) Candidates
        newRemoteCand = CNATFWCandidate::NewLC();
        newRemoteCand->SetTransportAddrL( aPeerAddr );
        newRemoteCand->SetType( CNATFWCandidate::EPeerReflexive );
        newRemoteCand->SetStreamCollectionId( localCand->StreamCollectionId() );
        newRemoteCand->SetStreamId( localCand->StreamId() );
        newRemoteCand->SetComponentId( localCand->ComponentId() );
        newRemoteCand->SetPriority( aPriority );
        
        CNATFWCandidate* peerReflCand 
            = CNATFWCandidate::NewLC( *newRemoteCand );
        // Session data generates arbitrary foundation
        iSessionData.AddPeerReflexiveCandidateL( peerReflCand, ETrue );
        CleanupStack::Pop( peerReflCand );
        }
    
    CNATFWCandidatePair* pair = CNATFWCandidatePair::NewLC( 
        *localCand, *newRemoteCand );
    pair->SetSelected( aRemoteFavored );
    HandleTriggeredCheckL( *pair );
    
    CleanupStack::PopAndDestroy( pair );
    CleanupStack::PopAndDestroy( newRemoteCand );
    }


// ---------------------------------------------------------------------------
// CIceCheckList::RelayUsedForSelectedPairL
// ---------------------------------------------------------------------------
//
TBool CIceCheckList::RelayUsedForSelectedPairL() const
    {
    __ICEDP( "CIceCheckList::RelayUsedForSelectedPairL" )

    TInt numOfComponents( iComponentIds.Count() );
    __ASSERT_ALWAYS( numOfComponents, User::Leave( KErrNotReady ) );
    
    TBool relayUseDetected( EFalse );
    for ( TInt i( 0 ); i < numOfComponents; ++i )
        {
        const CNATFWCandidatePair* pair 
            = iValidList->SelectedPair( iComponentIds[i] );
        
        if ( !relayUseDetected && pair
            && CNATFWCandidate::ERelay == pair->LocalCandidate().Type() )
            {
            relayUseDetected = ETrue;
            }
        }
    
    return relayUseDetected;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::RecomputePairPriorities
// ---------------------------------------------------------------------------
//
void CIceCheckList::RecomputePairPriorities()
    {
    __ICEDP( "CIceCheckList::RecomputePairPriorities" )
    
    iCheckPrioritizer->PrioritizeChecks( iChecks, iSessionData.Role() );
    
    iChecks.Sort( CIceConnectivityCheck::ComparePriorities );
    }


// Derived function

// ---------------------------------------------------------------------------
// CIceCheckList::CheckCompletedL
// From class MIceConnCheckListener.
// Once a successful check has completed for the first component, the other
// components of the same type and local preference will get performed.
// ---------------------------------------------------------------------------
//
void CIceCheckList::CheckCompletedL( 
        TInt aCompletionCode,
        const CIceConnectivityCheck& aCheck,
        CNATFWCandidatePair* aValidatedPair )
    {
    __ICEDP_INT1( "CIceCheckList::CheckCompletedL, COMPLETION_CODE:", 
        aCompletionCode )
    
    CleanupStack::PushL( aValidatedPair );
    
    if ( KErrNone == aCompletionCode )
        {
        __ASSERT_DEBUG( aValidatedPair, User::Leave( KErrArgument ) );
        
        // Priorities of local and remote candidates of validated pair must
        // be calculated before coming here.
        iCheckPrioritizer->PrioritizePair( 
            *aValidatedPair, iSessionData.Role() );
        iValidList->AddValidPairL( *aValidatedPair );
        
        if ( CNATFWCandidate::EPeerReflexive 
                == aValidatedPair->LocalCandidate().Type() )
            {
            iSessionData.AddPeerReflexiveCandidateL( 
                CNATFWCandidate::NewL( aValidatedPair->LocalCandidate() ),
                EFalse );
            }
        
        // Unfreeze other checks having same foundation ICE-17, 7.1.2.2.3
        InitializeCheckListL( aCheck.CandidatePair().Foundation() );
        
        TBool validPairsExistForComponents( 
            iValidList->HasPairForComponents( iComponentIds ) );
        if ( validPairsExistForComponents )
            {
            // Start checks in other check lists for candidates of same type
            iClient.ComponentsHaveValidPairsL( *this, 
                aCheck.CandidatePair().Foundation() );
            }
        
        if ( ( validPairsExistForComponents ) &&
             ( EIceRoleControlling == iSessionData.Role() ) &&
             ( !NominationExecuted() ) )
            {
            // stopping criteria is minimal latency; nominate pairs when we
            // have some valid pair for each component
            NominateCandidatePairsL();
            }
        }
    else if ( KErrRoleConflict == aCompletionCode )
        {
        // ICE-17, section 7.1.2.1. Failure cases.
        // Candidate pair whose check has generated 487 role conflict is
        // enqueued to the triggered check queue.
        TUint priority = PriorityForPeerReflCandL( 
            aCheck.CandidatePair().LocalCandidate() );
        
        EnqueueCheckL( const_cast<CIceConnectivityCheck&>( aCheck ), 
            CIceConnectivityCheck::EIceCheckTypeTriggered, 
            priority );
        
        TNATFWIceRole currentRole = iSessionData.Role();
        TNATFWIceRole desiredRole = ( EIceRoleControlling == currentRole )
            ? EIceRoleControlled : EIceRoleControlling;
        iClient.RoleChangeNeeded( desiredRole );
        }
    else
        {
        // unrecoverable failure
        }
    
    CleanupStack::PopAndDestroy( aValidatedPair );
    
    DoLosingPairProcessingL( iSelectedPairs );
    
    // ICE-17, 7.1.2.3.  Check List and Timer State Updates
    UpdateCheckListStateL();
    }


// ---------------------------------------------------------------------------
// CIceCheckList::NominationCompletedL
// From class MIceConnCheckListener.
// ICE-15, 7.1.2.2.4.  Updating the Nominated Flag
// ---------------------------------------------------------------------------
//
void CIceCheckList::NominationCompletedL( 
        TInt aCompletionCode,
        const CIceConnectivityCheck& /*aCheck*/,
        const CNATFWCandidatePair& aValidatedPair )
    {
    __ICEDP( "CIceCheckList::NominationCompletedL" )
    
    if ( KErrNone == aCompletionCode )
        {
        iValidList->SetPairNominatedL( aValidatedPair );
        }
    
    // ICE-17, 7.1.2.3.  Check List and Timer State Updates
    UpdateCheckListStateL();
    }


// ---------------------------------------------------------------------------
// CIceCheckList::HandleTriggeredCheckL
// ICE-17 7.2.1.4.  Triggered Checks
// STUN server has set PRIORITY & USE-CANDIDATE -attributes to the remote
// candidate.
// ---------------------------------------------------------------------------
//
void CIceCheckList::HandleTriggeredCheckL( 
        const CNATFWCandidatePair& aPair )
    {
    __ICEDP( "CIceCheckList::HandleTriggeredCheckL" )
    
    TUint trCheckPriority 
        = PriorityForPeerReflCandL( aPair.LocalCandidate() );
    
    TInt ind = FindCheck( aPair );
    
    if ( KErrNotFound != ind )
        {
        // We have corresponding check already in checklist
        CIceConnectivityCheck& check = *iChecks[ind];
        check.SetRemoteCheckInfo( 
            aPair.RemoteCandidate().Priority(), 
            aPair.Selected() );
        
        CIceConnectivityCheck::TIceCheckState state = check.State();
        if ( CIceConnectivityCheck::EIceCheckWaiting & state 
            || CIceConnectivityCheck::EIceCheckFrozen & state )
            {
            EnqueueCheckL( check, 
                CIceConnectivityCheck::EIceCheckTypeTriggered, 
                trCheckPriority );
            }
        else if ( CIceConnectivityCheck::EIceCheckInProgress & state )
            {
            __ICEDP( "CIceCheckList::HandleTriggeredCheckL, INPROGRESS" )
            // Nomination check should always succeed, so do not cancel it.
            // With ordinary checks ICE specification is followed.
            if ( !check.Nominated() )
                {
                check.Cancel();
                EnqueueCheckL( check, 
                    CIceConnectivityCheck::EIceCheckTypeTriggered,
                    trCheckPriority );
                }
            }
        else if ( CIceConnectivityCheck::EIceCheckFailed & state )
            {
            EnqueueCheckL( check, 
                CIceConnectivityCheck::EIceCheckTypeTriggered,
                trCheckPriority );
            }
        else if ( CIceConnectivityCheck::EIceCheckSucceed & state )
            {
            if ( aPair.Selected() )
                {
                // ICE-17, 7.2.1.5, Updating the Nominated Flag
                const_cast<CNATFWCandidatePair&>
                    ( check.CandidatePair() ).SetSelected( ETrue );
                // Set validated pair learned from this check as selected
                const CNATFWCandidatePair* validatedPair 
                    = check.ValidatedPair();
                __ASSERT_DEBUG( NULL != validatedPair, 
                    User::Leave( KErrNotReady ) );
                
                iValidList->SetPairNominatedL( *validatedPair );
                
                UpdateCheckListStateL();
                }
            }
        else
            {
            __ICEDP( "CIceCheckList::HandleTriggeredCheckL, ASSERT" )
            ASSERT( EFalse );
            }
        }
    else
        {
        // We have learned peer reflexive remote candidate
        CIceConnectivityCheck* trCheck = CIceConnectivityCheck::NewLC(
            *this, aPair.LocalCandidate(), aPair.RemoteCandidate(), 
            iConnHandler, iSessionData );
        iCheckPrioritizer->PrioritizeCheck( *trCheck,
            iSessionData.Role() );
        iChecks.InsertInOrderL( trCheck, 
            CIceConnectivityCheck::ComparePriorities );
        CleanupStack::Pop( trCheck );
        
        // remote priority used due to ICE-17, 7.1.2.2.2
        trCheck->SetRemoteCheckInfo( 
            aPair.RemoteCandidate().Priority(), aPair.Selected() );
        
        EnqueueCheckL( *trCheck, 
            CIceConnectivityCheck::EIceCheckTypeTriggered,
            trCheckPriority );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::UpdateCheckListStateL
// Check list state is updated due to check completion or pair nomination.
// Refer to ICE-17 7.1.2.3.  Check List and Timer State Updates and
// ICE-15 8.2.  Updating States.
// Change state to completed if controlling client has selected candidate
// pair for every component of the media stream. If all checks for any
// media component has failed, ICE processing is stopped for a media stream.
// ---------------------------------------------------------------------------
//
void CIceCheckList::UpdateCheckListStateL()
    {
    __ICEDP( "CIceCheckList::UpdateCheckListStateL" )
    
    if ( EIceCheckListRunning == State() )
        {
        TInt numOfComponents( iComponentIds.Count() );
        __ASSERT_DEBUG( numOfComponents, User::Leave( KErrNotReady ) );
        
        for ( TInt i( 0 ); i < numOfComponents; ++i )
            {
            // ICE-15, 8.2. If some component has completed, remove pending
            // checks for that component.
            RemovePendingChecks( iComponentIds[i] );
            }
        
        if ( iValidList->NominatedPairsExist( iComponentIds ) )
            {
            // when check list completes, frozen check lists are unfreezed
            SetStateL( EIceCheckListCompleted );
            }
        else
            {
            // If all checks are completed and there is not valid pair for
            // each media component, check list has failed. ICE-17, 7.1.2.3.
            TInt activeStates = (
                CIceConnectivityCheck::EIceCheckWaiting |
                CIceConnectivityCheck::EIceCheckInProgress |
                CIceConnectivityCheck::EIceCheckFrozen );
            
            if ( KErrNotFound == FindCheckInState( activeStates )
                    && !iValidList->HasPairForComponents( iComponentIds ) )
                {
                // when check list completes, frozen check lists are unfreezed
                SetStateL( EIceCheckListFailed );
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::RemovePendingChecks
// ICE-17, 8.1.2 Updating States
// ---------------------------------------------------------------------------
//      
void CIceCheckList::RemovePendingChecks( TUint aComponentId )
    {
    const CNATFWCandidatePair* nominatedPair 
        = iValidList->SelectedPair( aComponentId );
    if ( nominatedPair )
        {
        TInt stateMask = (
            CIceConnectivityCheck::EIceCheckWaiting |
            CIceConnectivityCheck::EIceCheckFrozen );
        
        TInt ind = FindCheckInState( stateMask, aComponentId );
        while ( KErrNotFound != ind )
            {
            iChecks.Remove( ind );
            ind = FindCheckInState( stateMask, aComponentId );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::GetComponentIdsL
// ---------------------------------------------------------------------------
//      
void CIceCheckList::GetComponentIdsL( RArray<TUint>& aComponentIds ) const
    {
    __ICEDP( "CIceCheckList::GetComponentIdsL" )

    aComponentIds.Reset();
    TInt count( iChecks.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        TUint curCompId 
            = iChecks[i]->CandidatePair().LocalCandidate().ComponentId();
        if ( KErrNotFound == aComponentIds.Find( curCompId ) )
            {
            aComponentIds.AppendL( curCompId );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::FindCheckInState
// ---------------------------------------------------------------------------
//
TInt CIceCheckList::FindCheckInState( TInt aStateMask )
    {
    TInt count( iChecks.Count() );
    __ICEDP_INT1( "CIceCheckList::FindCheckInState1:", count )
    
    for ( TInt i( 0 ); i < count; ++i )
        {
        CIceConnectivityCheck& check( *iChecks[i] );
        if ( aStateMask & check.State() )
            {
            return i;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::FindCheckInState
// ---------------------------------------------------------------------------
//
TInt CIceCheckList::FindCheckInState( TInt aStateMask, TUint aComponentId )
    {
    TInt count( iChecks.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        CIceConnectivityCheck& check( *iChecks[i] );
        if ( ( aStateMask & check.State() ) 
            && check.ComponentId() == aComponentId )
            {
            return i;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::FindCheckInState
// ---------------------------------------------------------------------------
//
TInt CIceCheckList::FindCheckInState( TInt aStateMask, 
        const TInetAddr& aRemoteAddr )
    {
    TInt count( iChecks.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        CIceConnectivityCheck& check( *iChecks[i] );
        const TInetAddr& remoteAddr 
            = check.CandidatePair().RemoteCandidate().TransportAddr();
        if ( ( aStateMask & check.State() ) 
            && TIceUtils::MatchAddresses( remoteAddr, aRemoteAddr ) )
            {
            return i;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::FindCheck
// ---------------------------------------------------------------------------
//
TInt CIceCheckList::FindCheck( const CNATFWCandidatePair& aPair )
    {
    for ( TInt i( iChecks.Count() - 1 ); 0 <= i; --i )
        {
        TBool match = CNATFWCandidatePair::MatchAddresses( 
            iChecks[i]->CandidatePair(), aPair );
        if ( match )
            {
            return i;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::SetStateL
// ---------------------------------------------------------------------------
//
void CIceCheckList::SetStateL( TIceCheckListState aNewState )
    {
    __ICEDP( "CIceCheckList::SetStateL" )
    
    __ASSERT_ALWAYS( iState != aNewState, User::Leave( KErrArgument ) );
    iState = aNewState;
    iCheckQue.Reset();
    
    if ( EIceCheckListCompleted == aNewState )
        {
        iTimer->Cancel();
        iSelectedPairs.ResetAndDestroy();
        
        TInt numOfComponents( iComponentIds.Count() );
        RPointerArray<CNATFWCandidatePair> selectedPairs;
        CleanupResetAndDestroyPushL( selectedPairs );
        for ( TInt i( 0 ); i < numOfComponents; ++i )
            {
            CNATFWCandidatePair* newPair = CNATFWCandidatePair::NewLC(
                *iValidList->SelectedPair( iComponentIds[i] ) );
            selectedPairs.AppendL( newPair );
            CleanupStack::Pop( newPair );
            }
        
        iClient.ChecklistCompletedL( *this, selectedPairs );
        
        selectedPairs.Close();
        CleanupStack::Pop( &selectedPairs );
        }
    else if ( EIceCheckListRunning == aNewState )
        {
        if ( !iTimer->IsActive() )
            {
            TCallBack callBack( CIceCheckList::TimerFired, this );
            iTimer->Start( 0, iInterval, callBack );
            }
        }
    else if ( EIceCheckListFailed == aNewState )
        {
        iTimer->Cancel();
        iSelectedPairs.ResetAndDestroy();
        
        RArray<TUint> failedStreams;
        CleanupClosePushL( failedStreams );
        TInt stateMask = (
            CIceConnectivityCheck::EIceCheckWaiting |
            CIceConnectivityCheck::EIceCheckInProgress |
            CIceConnectivityCheck::EIceCheckSucceed | 
            CIceConnectivityCheck::EIceCheckFailed |
            CIceConnectivityCheck::EIceCheckFrozen );
        
        TInt numOfComponents( iComponentIds.Count() );
        for ( TInt i( 0 ); i < numOfComponents; ++i )
            {
            if ( KErrNotFound != FindCheckInState( 
                    stateMask, iComponentIds[i] ) )
                {
                failedStreams.AppendL( iChecks[i]->StreamId() );
                }
            }
        
        iClient.ChecklistCompletedL( 
            *this, failedStreams, KErrCouldNotConnect );
        CleanupStack::PopAndDestroy( &failedStreams );
        }
    else
        {
        __ICEDP( "CIceCheckList::SetStateL, ELSE" )
        User::Leave( KErrArgument );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::DoLosingPairProcessingL
// ICE-17, 9.2.2.3.
// ICE SHOULD be restarted if some peer selected pair has failed.
// ---------------------------------------------------------------------------
//
void CIceCheckList::DoLosingPairProcessingL( 
        const RPointerArray<CNATFWCandidatePair>& aSelectedPairs )
    {
    __ICEDP( "CIceCheckList::DoLosingPairProcessingL" )
    
    // find out losing pairs
    RPointerArray<CNATFWCandidatePair> losingPairs;
    CleanupResetAndDestroyPushL( losingPairs );
    for ( TInt i( aSelectedPairs.Count() - 1 ); 0 <= i ; --i )
        {
        const CNATFWCandidatePair& pair( *aSelectedPairs[i] );
        if ( iValidList->HasPair( pair ) )
            {
            iValidList->SetPairNominatedL( pair );
            }
        else
            {
            CNATFWCandidatePair* losingPair 
                = CNATFWCandidatePair::NewLC( pair );
            losingPairs.AppendL( losingPair );
            CleanupStack::Pop( losingPair );
            }
        }
    
    /* If none of peer selected pairs is in-progress and at least one
       is failed, check list is completed unsuccessfully. If some selected
       pair is in-progress, we wait for check completion and do this
       processing again. */
    TBool inProgressCheckFound( EFalse );
    TBool failedCheckFound( EFalse );
    TInt losingPairInd( losingPairs.Count() - 1 );
    while ( ( 0 <= losingPairInd ) && ( !inProgressCheckFound ) )
        {
        TInetAddr remoteAddr( 
            losingPairs[losingPairInd]->RemoteCandidate().TransportAddr() );
        
        TInt inProgressCheckInd = FindCheckInState( 
            CIceConnectivityCheck::EIceCheckInProgress, remoteAddr );
        if ( KErrNotFound == inProgressCheckInd )
            {
            TInt failedCheckInd = FindCheckInState( 
                CIceConnectivityCheck::EIceCheckFailed, remoteAddr );
            if ( KErrNotFound != failedCheckInd )
                {
                failedCheckFound = ETrue;
                }
            }
        else
            {
            inProgressCheckFound = ETrue;
            }
        
        losingPairInd--;
        }
    
    if ( failedCheckFound )
        {
        SetStateL( EIceCheckListFailed );
        }
    
    CleanupStack::PopAndDestroy( &losingPairs );
    }


// ---------------------------------------------------------------------------
// CIceCheckList::NominateCandidatePairsL
// Resends connectivity checks to select pair for each media component.
// ICE-17, section 8.1.1.1, regular nomination is used.
// ---------------------------------------------------------------------------
//
void CIceCheckList::NominateCandidatePairsL()
    {
    __ICEDP( "CIceCheckList::NominateCandidatePairsL" )
    
    TInt numOfComponents( iComponentIds.Count() );
    for ( TInt i( 0 ); i < numOfComponents; ++i )
        {
        const CNATFWCandidatePair* pair 
            = iValidList->HighestPriorityPair( iComponentIds[i] );
        ASSERT( NULL != pair );
        const TInetAddr& lAddr( pair->LocalCandidate().TransportAddr() );
        const TInetAddr& rAddr( pair->RemoteCandidate().TransportAddr() );
        
        TInt index( iChecks.Count() );
        TBool checkFound( EFalse );
        while ( !checkFound && --index >= 0 )
            {
            const CNATFWCandidatePair* validatedPair 
                = iChecks[index]->ValidatedPair();
            if ( validatedPair )
                {
                const TInetAddr& lAddrOfCheck 
                    = validatedPair->LocalCandidate().TransportAddr();
                const TInetAddr& rAddrOfCheck 
                    = validatedPair->RemoteCandidate().TransportAddr();
                
                checkFound = 
                    ( TIceUtils::MatchAddresses( lAddrOfCheck, lAddr ) &&
                      TIceUtils::MatchAddresses( rAddrOfCheck, rAddr ) );
                }
            }
        
        User::LeaveIfError( index );
        EnqueueCheckL( *iChecks[index],
            CIceConnectivityCheck::EIceCheckTypeNomination, 0 );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::CheckListFrozen
// ---------------------------------------------------------------------------
//
TBool CIceCheckList::CheckListFrozen()
    {
    __ICEDP( "CIceCheckList::CheckListFrozen" )
    
    TInt ind( iChecks.Count() );
    while ( ind-- )
        {
        if ( CIceConnectivityCheck::EIceCheckFrozen != iChecks[ind]->State() )
            {
            return EFalse;
            }
        }
    
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::EnqueueCheckL
// ---------------------------------------------------------------------------
//
void CIceCheckList::EnqueueCheckL( CIceConnectivityCheck& aCheck,
        CIceConnectivityCheck::TIceCheckType aType,
        TUint aLocalPriority )
    {
    TIceCheckQueItem item( aCheck, aType, aLocalPriority );
    
    TInt index = iCheckQue.Find( item, TIceCheckQueItem::MatchCheck );
    if ( CIceConnectivityCheck::EIceCheckTypeNomination == aType 
            && KErrNotFound != index )
        {
        // nomination check is privileged over triggered one
        __ICEDP( "CIceCheckList::EnqueueCheckL, REMOVE TRIGGERED CHECK" )
        iCheckQue.Remove( index );
        }
    
    // do not allow duplicate entries
    if ( KErrNotFound == iCheckQue.Find( 
            item, TIceCheckQueItem::MatchCheck ) )
        {
        __ICEDP( "CIceCheckList::EnqueueCheckL, APPENDING" )
        iCheckQue.AppendL( item );
        
        if ( !iTimer->IsActive() )
            {
            TCallBack callBack( CIceCheckList::TimerFired, this );
            iTimer->Start( 0, iInterval, callBack );
            }
        }
    else
        {
        __ICEDP( "CIceCheckList::EnqueueCheckL, DUPLICATE ENTRY" )
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::DequeueCheckL
// ---------------------------------------------------------------------------
//
TBool CIceCheckList::DequeueCheckL()
    {
    __ICEDP_INT1( "CIceCheckList::DequeueCheckL, COUNT:", iCheckQue.Count() )
    
    if ( 0 != iCheckQue.Count() )
        {
        TIceCheckQueItem item = iCheckQue[0];
        iCheckQue.Remove( 0 );
        
        CIceConnectivityCheck& check = item.Check();
        if ( CIceConnectivityCheck::EIceCheckTypeNomination == item.Type() )
            {
            check.PerformNominationL( RetransmissionTimeOut() );
            }
        else
            {
            CIceConnectivityCheck::TIceCheckState state = check.State();
            // save bandwidth if previous transaction already succeeded
            if ( CIceConnectivityCheck::EIceCheckSucceed != state )
                {
                check.Initialize();
                check.PerformConnCheckL(
                    CIceConnectivityCheck::EIceCheckTypeTriggered,
                    item.Priority(), RetransmissionTimeOut() );
                }
            }
        
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckList::RetransmissionTimeOut
// ICE-17, section 16. RTO = MAX (100ms, Ta*N * (Num-Waiting))
// ---------------------------------------------------------------------------
//
TUint CIceCheckList::RetransmissionTimeOut() const
    {
    TInt numOfWaitingPairs( 0 );
    
    TInt numOfChecks( iChecks.Count() );
    for ( TInt i( 0 ); i < numOfChecks; ++i )
        {
        CIceConnectivityCheck& check( *iChecks[i] );
        if ( CIceConnectivityCheck::EIceCheckWaiting == check.State() )
            {
            numOfWaitingPairs++;
            }
        }
    
    const TInt KMinimumRto( 100 );
    return TUint( Max( KMinimumRto, iInterval.Int() * numOfWaitingPairs ) );
    }


// ---------------------------------------------------------------------------
// CIceCheckList::FoundationsForCollectionL
// ---------------------------------------------------------------------------
//
CDesC8ArrayFlat* CIceCheckList::FoundationsForCollectionL( 
        TUint aStreamCollectionId ) const
    {
    const TInt KGranularity(4);
    CDesC8ArrayFlat* descArray = new (ELeave) CDesC8ArrayFlat( KGranularity );
    CleanupStack::PushL( descArray );
    
    TInt numOfChecks( iChecks.Count() );
    for ( TInt i(0); i < numOfChecks; ++i )
        {
        CIceConnectivityCheck& check( *iChecks[i] );
        if ( check.StreamCollectionId() == aStreamCollectionId )
            {
            TInt matchItemInd(0);
            const TDesC8& foundation( 
                iChecks[i]->CandidatePair().Foundation() );
            if ( 0 != descArray->Find( foundation, matchItemInd ) )
                {
                descArray->AppendL( foundation );
                }
            }
        }
    
    CleanupStack::Pop( descArray );
    return descArray;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::LowestComponentId
// ---------------------------------------------------------------------------
//
TUint CIceCheckList::LowestComponentId( RArray<TUint>& aComponentIds ) const
    {
    TInt componentIdCount( aComponentIds.Count() );
    ASSERT( 0 != componentIdCount );
    
    TUint lowestComponentId( aComponentIds[0] );
    for ( TInt i(1); i < componentIdCount; ++i )
        {
        if ( aComponentIds[i] < lowestComponentId )
            {
            lowestComponentId = aComponentIds[i];
            }
        }
    
    return lowestComponentId;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::NominationExecuted
// ---------------------------------------------------------------------------
//
TBool CIceCheckList::NominationExecuted() const
    {
    TInt numOfChecks( iChecks.Count() );
    for ( TInt i(0); i < numOfChecks; ++i )
        {
        CIceConnectivityCheck& check( *iChecks[i] );
        if ( check.Nominated() )
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CIceCheckList::InitializeCheckList
// Special initialization due to ICE-17, 9.3.1.4 ICE Continuing for Existing
// Media Stream.
// ---------------------------------------------------------------------------
//
void CIceCheckList::InitializeCheckList( const TDesC8& aFoundation, 
        TInt aExcludedComponentId )
    {
    __ICEDP( "CIceCheckList::InitializeCheckList" )
    ASSERT( 0 != iChecks.Count() );
    
    for ( TInt i( iChecks.Count() - 1 ); i >= 0; --i )
        {
        if ( CIceConnectivityCheck::EIceCheckFrozen == iChecks[i]->State()
                && iChecks[i]->CandidatePair().Foundation() == aFoundation
                && iChecks[i]->ComponentId() != aExcludedComponentId )
            {
            iChecks[i]->Initialize();
            }
        }
    }
