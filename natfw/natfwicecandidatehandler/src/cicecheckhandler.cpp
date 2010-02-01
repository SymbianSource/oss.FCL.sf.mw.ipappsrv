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
#include "mnatfwpluginobserver.h"
#include "natfwcredentials.h"
#include "cicecheckhandler.h"
#include "cicesessiondata.h"
#include "cicechecklist.h"
#include "ciceconnectivitycheck.h"
#include "natfwstunsrvclientsession.h"
#include "icecandidatehandlerlogs.h"

const TInt KMsToUsFactor = 1000;

// ======== MEMBER FUNCTIONS ========

CIceCheckHandler::CIceCheckHandler(
        CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer,
        CIceConnectionHandler& aConnHandler,
        MIceNatPluginEventObs& aEventObserver )
    :
    iSessionData( aSessionData ),
    iMultiplexer( aMultiplexer ),
    iConnHandler( aConnHandler ),
    iEventObserver( aEventObserver )
    {
    }


void CIceCheckHandler::ConstructL()
    {
    iStunSrv = CNATFWSTUNSrvClientSession::NewL( *this, iMultiplexer );
    }


CIceCheckHandler* CIceCheckHandler::NewL(
        CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer,
        CIceConnectionHandler& aConnHandler,
        MIceNatPluginEventObs& aEventObserver )
    {
    CIceCheckHandler* self 
        = CIceCheckHandler::NewLC( 
        aSessionData, aMultiplexer, aConnHandler, aEventObserver );
    CleanupStack::Pop( self );
    return self;
    }


CIceCheckHandler* CIceCheckHandler::NewLC(
        CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer,
        CIceConnectionHandler& aConnHandler,
        MIceNatPluginEventObs& aEventObserver )
    {
    CIceCheckHandler* self 
        = new( ELeave ) CIceCheckHandler( 
        aSessionData, aMultiplexer, aConnHandler, aEventObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CIceCheckHandler::~CIceCheckHandler()
    {
    iCheckLists.ResetAndDestroy();
    delete iStunSrv;
    iCurCredentials.ResetAndDestroy();
    iBufferedChecks.Close();
    }


// Non-derived function


// ---------------------------------------------------------------------------
// CIceCheckHandler::SetCredentialsL
// Local credentials are needed for incoming request authentication and
// must be set before starting connectivity checks.
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::SetCredentialsL( 
        const CNATFWCredentials& aCredentials )
    {
    if ( CNATFWCredentials::EInbound == aCredentials.Direction() )
        {
        UpdateStunServerParamsL();
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::PerformConnectivityChecksL
// Start perform connectivity checks.
// Timer value depends on number of active check lists (initially one).
// As an optimization ICE starts with one active check list and continue
// with activation of other lists when certain results are got from first
// check list.
// Order of media streams is concluded from the order in which fetchcandidates
// has been called.
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::PerformConnectivityChecksL()
    {
    __ICEDP( "CIceCheckHandler::PerformConnectivityChecksL" )
    __ASSERT_ALWAYS( iSessionData.Role(), User::Leave( KErrNotReady ) );
    
    if ( !iState )
        {
        FormChecklistsL();
        UpdateStunServerParamsL();
        
        // Activate first check list
        TTimeIntervalMicroSeconds32 interval 
            = iSessionData.TaTimerValue() * KMsToUsFactor
            * ( NumOfActiveCheckLists() + 1 );
        iCheckLists[0]->InitializeCheckListL();
        iCheckLists[0]->StartPerformChecksL( interval );
        
        SetState( EIceRunning );
        ExecuteBufferedTriggeredChecksL();
        }
    else
        {
        // Either new collection is added or ICE is restarted for some stream
        HandleNewCollectionsL();
        HandleIceRestartsL();
        UpdateStunServerParamsL();
        
        SetState( EIceRunning );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidatePair>& aPeerSelectedPairs )
    {
    __ICEDP( "CIceCheckHandler::UpdateIceProcessingL, PEERSELECTEDCANDS" )
    __ASSERT_DEBUG( 0 != aPeerSelectedPairs.Count(), 
        User::Leave( KErrArgument ) );
    
    RPointerArray<CIceCheckList> updatedLists;
    CleanupClosePushL( updatedLists );
    
    // Find and update affected check lists
    TInt selectedPairCount( aPeerSelectedPairs.Count() );
    for ( TInt i( 0 ); i < selectedPairCount; ++i )
        {
        TUint streamCollId( 
            aPeerSelectedPairs[i]->LocalCandidate().StreamCollectionId() );
        CIceCheckList* checkList = ChecklistByCollectionIdL( streamCollId );
        if ( KErrNotFound == updatedLists.Find( checkList )
            && CIceCheckList::EIceCheckListRunning == checkList->State() )
            {
            // Checklist ignores candidates for the other collections
            checkList->UpdateCheckListL( aPeerSelectedPairs );
            updatedLists.AppendL( checkList );
            }
        }
    
    CleanupStack::PopAndDestroy( &updatedLists );
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::UpdateIceProcessingL
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidate>& aRemoteCands )
    {
    __ICEDP( "CIceCheckHandler::UpdateIceProcessingL, REMOTECANDS" )
    __ASSERT_DEBUG( 0 != aRemoteCands.Count(), User::Leave( KErrArgument ) );
    
    RPointerArray<CIceCheckList> updatedLists;
    CleanupClosePushL( updatedLists );
    
    // Find and update affected check lists
    TInt remoteCandsCount( aRemoteCands.Count() );
    for ( TInt i( 0 ); i < remoteCandsCount; ++i )
        {
        TUint streamCollId( aRemoteCands[i]->StreamCollectionId() );
        CIceCheckList* checkList = ChecklistByCollectionIdL( streamCollId );
        if ( KErrNotFound == updatedLists.Find( checkList )
            && CIceCheckList::EIceCheckListRunning == checkList->State() )
            {
            // Checklist ignores candidates for the other collections
            checkList->UpdateCheckListL( aRemoteCands );
            updatedLists.AppendL( checkList );
            }
        }
    
    CleanupStack::PopAndDestroy( &updatedLists );
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::CleanupCollectionData
// Session data must be updated before coming here.
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::CleanupCollectionData( TUint aCollectionId )
    {
    __ICEDP( "CIceCheckHandler::CleanupCollectionData" )
    
    if ( EIceRunning == iState || EIceCompleted == iState )
        {
        CIceCheckList* checkList = ChecklistByCollectionId( aCollectionId );
        if ( checkList )
            {
            TInt ind( iCheckLists.Find( checkList ) );
            delete iCheckLists[ind];
            iCheckLists.Remove( ind );
            }
        
        TRAP_IGNORE( UpdateStunServerParamsL() )
        }
    }

// Derived function

// ---------------------------------------------------------------------------
// From class MNATFWStunSrvObserver.
// Entity which first receives incoming message must set aPeerAddr either to
// the same as aFromAddr or to REMOTE-ADDR from data indication.
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::STUNRequestReceivedL( const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, const TInetAddr& aPeerAddr,
        TUint aPriority, TBool aRemoteFavored )
    {
    if ( EIceRunning == iState || EIceCompleted == iState )
        {
        __ICEDP_INT1( "CIceCheckHandler::STUNRequestReceivedL, FAVORED:",
            aRemoteFavored )
        
        const CNATFWCandidate* hostCand
            = iSessionData.FindLocalCandidate( aLocalAddr );
        __ASSERT_ALWAYS( NULL != hostCand, User::Leave( KErrNotFound ) );
        
        CIceCheckList* checkList 
            = ChecklistByCollectionIdL( hostCand->StreamCollectionId() );
        
        checkList->STUNRequestReceivedL( 
            aLocalAddr, aFromAddr, aPeerAddr, aPriority, aRemoteFavored );
        }
    else
        {
        __ICEDP( "CIceCheckHandler::STUNRequestReceivedL, BUFFERING" )
        
        // Must buffer triggered checks until remote credentials are known
        TIceTriggeredCheckInfo check( aLocalAddr, aFromAddr, aPeerAddr,
            aPriority, aRemoteFavored );
        iBufferedChecks.AppendL( check );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::RoleChangeNeeded
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::RoleChangeNeeded( TNATFWIceRole aDesiredRole )
    {
    __ICEDP( "CIceCheckHandler::RoleChangeNeeded" )
    
    TNATFWIceRole currentRole = iSessionData.Role();
    if ( currentRole != aDesiredRole )
        {
        iSessionData.SetRole( aDesiredRole );
        
        // pair priorities must be recomputed (ICE-17, section 7.2.1.1.)
        TInt numOfCheckLists( iCheckLists.Count() );
        for ( TInt i( 0 ); i < numOfCheckLists; ++i )
            {
            iCheckLists[i]->RecomputePairPriorities();
            }
        }
    else
        {
        __ICEDP( "CIceCheckHandler::RoleChangeNeeded, ALREADY CHANGED" )
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::ChecklistCompletedL
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::ChecklistCompletedL( 
        const CIceCheckList& aCheckList,
        const RPointerArray<CNATFWCandidatePair>& aSelectedPairs )
    {
    __ICEDP( "CIceCheckHandler::ChecklistCompletedL, SUCCESS" )
    
    TInt numOfSelectedPairs( aSelectedPairs.Count() );
    __ASSERT_DEBUG( numOfSelectedPairs, User::Leave( KErrArgument ) );
    
    UpdateICEProcessingStateL( aCheckList );
    for ( TInt i( 0 ); i < numOfSelectedPairs; ++i )
        {
        TUint streamId( aSelectedPairs[i]->LocalCandidate().StreamId() );
        iEventObserver.PluginEventOccured( NULL, streamId,
            MIceNatPluginEventObs::ECandidatePairFound,
            KErrNone, aSelectedPairs[i] );
        
        iEventObserver.PluginEventOccured( NULL, streamId,
            MIceNatPluginEventObs::EConnChecksCompleted,
            KErrNone, NULL );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::ChecklistCompletedL
// Check list has failed, post error events for the components.
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::ChecklistCompletedL( 
        const CIceCheckList& aCheckList,
        const RArray<TUint>& aFailedComps,
        TInt aErrCode )
    {
    __ICEDP( "CIceCheckHandler::ChecklistCompletedL, FAIL" )
    
    TInt numOfFailedComps( aFailedComps.Count() );
    __ASSERT_DEBUG( numOfFailedComps, User::Leave( KErrArgument ) );
    
    UpdateICEProcessingStateL( aCheckList );
    for ( TInt i( 0 ); i < numOfFailedComps; ++i )
        {
        iEventObserver.PluginEventOccured( NULL, aFailedComps[i],
            MIceNatPluginEventObs::EConnChecksCompleted,
            aErrCode, NULL );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::UpdateICEProcessingStateL
// Update ICE processing state.
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::UpdateICEProcessingStateL( 
        const CIceCheckList& aCheckList )
    {
    __ICEDP( "CIceCheckHandler::UpdateICEProcessingStateL" )
    
    TBool processingCompleted( ETrue );
    TInt numOfCheckLists( iCheckLists.Count() );
    
    for ( TInt i( 0 ); i < numOfCheckLists; ++i )
        {
        if ( CIceCheckList::EIceCheckListCompleted != iCheckLists[i]->State()
            && CIceCheckList::EIceCheckListFailed != iCheckLists[i]->State() )
            {
            processingCompleted = EFalse;
            }
        }
    
    if ( processingCompleted )
        {
        SetState( EIceCompleted );
        }
    else
        {
        // ICE-15, 7.1.2.3.  Check List and Timer State Updates
        UnFreezeCheckListsL( aCheckList, KNullDesC8 );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::UnFreezeCheckListsL
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::UnFreezeCheckListsL(
        const CIceCheckList& aExcludedList, const TDesC8& aFoundation )
    {
    __ICEDP( "CIceCheckHandler::UnFreezeCheckListsL" )
    
    TInt count( iCheckLists.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        // Do not update same checklist which reported event
        if ( iCheckLists[i] != &aExcludedList )
            {
            aFoundation.Length()
                ? iCheckLists[i]->InitializeCheckListL( aFoundation )
                : iCheckLists[i]->InitializeCheckListL();
            
            iCheckLists[i]->StartPerformChecksL( 
                TTimeIntervalMicroSeconds32( 
                    iSessionData.TaTimerValue() * KMsToUsFactor
                    * ( NumOfActiveCheckLists() + 1 ) ) );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::ComponentsHaveValidPairsL
// ICE 7.1.2, Change state of first check in other check lists
// to waiting if it is in frozen state and foundation is the same.
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::ComponentsHaveValidPairsL( 
        const CIceCheckList& aCheckList, const TDesC8& aFoundation )
    {
    __ICEDP( "CIceCheckHandler::ComponentsHaveValidPairsL" )
    
    UnFreezeCheckListsL( aCheckList, aFoundation );
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::FormChecklistsL
// Checklists must be in the same order than media lines in SDP. This is 
// guaranteed if client has called fetchcandidates in correct order.
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::FormChecklistsL()
    {
    __ICEDP( "CIceCheckHandler::FormChecklistsL" )
    
    RArray<TUint> streamCollIds;
    CleanupClosePushL( streamCollIds );
    iSessionData.GetStreamCollectionIdsL( streamCollIds );
    __ASSERT_ALWAYS( streamCollIds.Count(), User::Leave( KErrNotReady ) );
    
    TInt numOfChecklists( streamCollIds.Count() );
    for ( TInt i = 0; i < numOfChecklists; ++i )
        {
        CIceCheckList* checklist 
            = CIceCheckList::NewLC( 
            *this, streamCollIds[i],
            iSessionData, iConnHandler );
        iCheckLists.AppendL( checklist );
        CleanupStack::Pop( checklist );
        }
    
    CleanupStack::PopAndDestroy( &streamCollIds );
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::ChecklistByCollectionId
// ---------------------------------------------------------------------------
//
CIceCheckList* CIceCheckHandler::ChecklistByCollectionId( 
        TUint aStreamCollId )
    {
    TInt count( iCheckLists.Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        if ( iCheckLists[i]->StreamCollectionId() == aStreamCollId )
            {
            return iCheckLists[i];
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::ChecklistByCollectionIdL
// ---------------------------------------------------------------------------
//
CIceCheckList* CIceCheckHandler::ChecklistByCollectionIdL( 
        TUint aStreamCollId )
    {
    CIceCheckList* checkList = ChecklistByCollectionId( aStreamCollId );
    if ( !checkList )
        {
        User::Leave( KErrNotFound );
        }
    
    return checkList;
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::SetState
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::SetState( TIceProcessingState aState )
    {
    __ICEDP( "CIceCheckHandler::SetState" )
    
    iState = aState;
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::HandleNewCollectionsL
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::HandleNewCollectionsL()
    {
    __ICEDP( "CIceCheckHandler::HandleNewCollectionsL" )
    
    RArray<TUint> streamCollIds;
    CleanupClosePushL( streamCollIds );
    iSessionData.GetStreamCollectionIdsL( streamCollIds );
    TInt numOfCollections( streamCollIds.Count() );
    __ASSERT_ALWAYS( numOfCollections, User::Leave( KErrNotReady ) );
    
    for ( TInt i( 0 ); i < numOfCollections; ++i )
        {
        if ( NULL == ChecklistByCollectionId( streamCollIds[i] ) )
            {
            CIceCheckList* checklist 
                = CIceCheckList::NewLC( 
                *this, streamCollIds[i],
                iSessionData, iConnHandler );
            iCheckLists.AppendL( checklist );
            CleanupStack::Pop( checklist );
            
            TTimeIntervalMicroSeconds32 interval = 
                iSessionData.TaTimerValue() * KMsToUsFactor
                * ( NumOfActiveCheckLists() + 1 );
            checklist->InitializeCheckListL();
            checklist->StartPerformChecksL( interval );
            }
        }
    
    CleanupStack::PopAndDestroy( &streamCollIds );
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::HandleIceRestartsL
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::HandleIceRestartsL()
    {
    __ICEDP( "CIceCheckHandler::HandleIceRestartsL" )
    
    TInt numOfCurCredentials( iCurCredentials.Count() );
    __ASSERT_DEBUG( numOfCurCredentials, User::Leave( KErrNotReady ) );
    
    RPointerArray<CIceCheckList> updatedLists;
    CleanupClosePushL( updatedLists );
    
    for ( TInt i( 0 ); i < numOfCurCredentials; ++i )
        {
        CNATFWCredentials* entry = iCurCredentials[i];
        const CNATFWCredentials* storedId = iSessionData.Credentials(
            entry->StreamId(), entry->Direction() );
        
        if ( storedId && ( *storedId != *entry ) )
            {
            CIceCheckList* checkList = ChecklistByCollectionIdL( 
                storedId->StreamCollectionId() );

            if ( KErrNotFound == updatedLists.Find( checkList ) )
                {
                TTimeIntervalMicroSeconds32 interval 
                    = iSessionData.TaTimerValue() * KMsToUsFactor
                    * ( NumOfActiveCheckLists() + 1 );
                
                checkList->RestartCheckListL( interval );
                updatedLists.AppendL( checkList );
                }
            }
        }
    
    CleanupStack::PopAndDestroy( &updatedLists );
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::NumOfActiveCheckLists
// ---------------------------------------------------------------------------
//
TInt CIceCheckHandler::NumOfActiveCheckLists()
    {
    TInt count( 0 );
    TInt numOfChecklists( iCheckLists.Count() );
    for ( TInt i( 0 ); i < numOfChecklists; ++i )
        {
        if ( CIceCheckList::EIceCheckListRunning == iCheckLists[i]->State() )
            {
            count++;
            }
        }
    
    return count;
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::UpdateStunServerParamsL
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::UpdateStunServerParamsL()
    {
    const RPointerArray<CNATFWCredentials>& credentials
        = iSessionData.Credentials();
    TInt count( credentials.Count() );
    __ICEDP_INT1( "CIceCheckHandler::UpdateStunServerParamsL, IDENT_COUNT:",
        count )
    
    iStunSrv->RemoveAuthenticationParamsL( iCurCredentials );
    iCurCredentials.ResetAndDestroy();
    for ( TInt i( 0 ); i < count; ++i )
        {
        CNATFWCredentials* credential 
            = CNATFWCredentials::NewLC( *credentials[i] );
        iCurCredentials.AppendL( credential );
        CleanupStack::Pop( credential );
        }
    
    iStunSrv->AddAuthenticationParamsL( iCurCredentials );
    iStunSrv->SetRoleL( iSessionData.Role(), iSessionData.TieBreaker() );
    }


// ---------------------------------------------------------------------------
// CIceCheckHandler::ExecuteBufferedTriggeredChecksL
// ---------------------------------------------------------------------------
//
void CIceCheckHandler::ExecuteBufferedTriggeredChecksL()
    {
    TInt count( iBufferedChecks.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        TIceTriggeredCheckInfo& check( iBufferedChecks[i] );
        STUNRequestReceivedL( check.LocalAddr(), check.FromAddr(), 
            check.PeerAddr(), check.Priority(), check.IsRemoteFavored() );
        }
    
    iBufferedChecks.Reset();
    }
