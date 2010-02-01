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
* Description:    Performs connectivity check
*
*/



#include "natfwcredentials.h"
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"
#include "ciceconnectivitycheck.h"
#include "miceconnchecklistener.h"
#include "natfwbindingbase.h"
#include "ciceconnectionhandler.h"
#include "cicesessiondata.h"

#include "icecandidatehandlerlogs.h"

// ======== MEMBER FUNCTIONS ========

CIceConnectivityCheck::CIceConnectivityCheck(
        MIceConnCheckListener& aClient,
        CIceConnectionHandler& aConnHandler,
        CIceSessionData& aSessionData )
    :
    iState( EIceCheckFrozen ),
    iClient( aClient ),
    iConnHandler( aConnHandler ),
    iSessionData( aSessionData )
    {
    }


void CIceConnectivityCheck::ConstructL(
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand )
    {
    __ICEDP( "CIceConnectivityCheck::ConstructL" )
    
    __ASSERT_ALWAYS( aLocalCand.ComponentId() == aRemoteCand.ComponentId(),
        User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( 
        aLocalCand.StreamCollectionId() == aRemoteCand.StreamCollectionId(),
        User::Leave( KErrArgument ) );
    
    iCandidatePair = CNATFWCandidatePair::NewL( aLocalCand, aRemoteCand );
    }


CIceConnectivityCheck* CIceConnectivityCheck::NewL(
        MIceConnCheckListener& aClient,
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand,
        CIceConnectionHandler& aConnHandler,
        CIceSessionData& aSessionData )
    {
    CIceConnectivityCheck* self = CIceConnectivityCheck::NewLC(
        aClient, aLocalCand, aRemoteCand, aConnHandler, aSessionData );
    CleanupStack::Pop( self );
    return self;
    }


CIceConnectivityCheck* CIceConnectivityCheck::NewLC(
        MIceConnCheckListener& aClient,
        const CNATFWCandidate& aLocalCand,
        const CNATFWCandidate& aRemoteCand,
        CIceConnectionHandler& aConnHandler,
        CIceSessionData& aSessionData )
    {
    CIceConnectivityCheck* self = new( ELeave ) 
        CIceConnectivityCheck( aClient, aConnHandler, aSessionData );
    CleanupStack::PushL( self );
    self->ConstructL( aLocalCand, aRemoteCand );
    return self;
    }


CIceConnectivityCheck::~CIceConnectivityCheck()
    {
    __ICEDP( "CIceConnectivityCheck::~CIceConnectivityCheck" )
    
    delete iValidatedPair;
    if ( iCandidatePair )
        {
        iConnHandler.RemoveConnection( 
            iCandidatePair->LocalCandidate().StreamId(),
            iConnectionId );
        }
    
    delete iCandidatePair;
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::ComparePriorities
// Implements TLinearOrder (descending order).
// ---------------------------------------------------------------------------
//
TInt CIceConnectivityCheck::ComparePriorities( 
        const CIceConnectivityCheck& aA, 
        const CIceConnectivityCheck& aB )
    {
    return static_cast<TInt>( 
        aB.CandidatePair().Priority() - aA.CandidatePair().Priority() );
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::MatchAddresses
// Implements TIdentityRelation.
// ---------------------------------------------------------------------------
//
TBool CIceConnectivityCheck::MatchAddresses( 
        const CIceConnectivityCheck& aItem1, 
        const CIceConnectivityCheck& aItem2 )
    {
    const TInetAddr& localAddrA( 
        aItem1.CandidatePair().LocalCandidate().TransportAddr() );
    const TInetAddr& localAddrB( 
        aItem2.CandidatePair().LocalCandidate().TransportAddr() );
    const TInetAddr& remoteAddrA( 
        aItem1.CandidatePair().RemoteCandidate().TransportAddr() );
    const TInetAddr& remoteAddrB( 
        aItem2.CandidatePair().RemoteCandidate().TransportAddr() );
    
    return ( ( TIceUtils::MatchAddresses( localAddrA, localAddrB ) ) 
        && ( TIceUtils::MatchAddresses( remoteAddrA, remoteAddrB ) ) );
    }

// Non-derived function

// ---------------------------------------------------------------------------
// CIceConnectivityCheck::Initialize
// ---------------------------------------------------------------------------
//
void CIceConnectivityCheck::Initialize()
    {
    __ICEDP( "CIceConnectivityCheck::Initialize" )
    
    iIsNominated = EFalse;
    iResendInProgress = EFalse;
    
    iState = EIceCheckWaiting;
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::PerformConnCheckL
// USERNAME and MESSAGE-INTEGRITY -attributes are set by connection handler.
// ---------------------------------------------------------------------------
//
void CIceConnectivityCheck::PerformConnCheckL( 
        TIceCheckType aType, 
        TUint aPriority,
        TUint aRtoValue )
    {
    __ICEDP( "CIceConnectivityCheck::PerformConnCheckL" )
    __ICEDP_ADDRLOG( "LOCAL_ADDR", 
        iCandidatePair->LocalCandidate().TransportAddr() )
    __ICEDP_ADDRLOG( "PEER_ADDR", 
        iCandidatePair->RemoteCandidate().TransportAddr() )
    
    __ASSERT_ALWAYS( 
        ( EIceCheckWaiting == iState || EIceCheckFrozen == iState ),
        User::Leave( KErrNotReady ) );
        
    __ASSERT_ALWAYS( ( iCandidatePair->LocalCandidate().Type() ==
        CNATFWCandidate::ERelay || iCandidatePair->LocalCandidate().Type() ==
        CNATFWCandidate::EHost ), User::Leave( KErrArgument ) );
    
    iType = aType;
    iPriority = aPriority;
    
    if ( 0 == iConnectionId )
        {
        iConnectionId = iConnHandler.CreateConnectionL( *iCandidatePair );
        }
    
    iConnHandler.PerformConnCheckL( 
        StreamId(), iConnectionId, iPriority, EFalse, 
        aRtoValue, iCandidatePair->RemoteCandidate().TransportAddr(),
        *this );
    
    iState = EIceCheckInProgress;
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::PerformNominationL
// ---------------------------------------------------------------------------
//
void CIceConnectivityCheck::PerformNominationL( TUint aRtoValue )
    {
    __ICEDP( "CIceConnectivityCheck::PerformNominationL" )
    __ICEDP_ADDRLOG( "LOCAL_ADDR", 
        iCandidatePair->LocalCandidate().TransportAddr() )
    __ICEDP_ADDRLOG( "PEER_ADDR", 
        iCandidatePair->RemoteCandidate().TransportAddr() )
    
    __ASSERT_ALWAYS( 
        EIceCheckSucceed == iState || EIceCheckInProgress == iState, 
        User::Leave( KErrNotReady ) );
    
    iIsNominated = ETrue;
    
    if ( EIceCheckSucceed == iState )
        {
        iConnHandler.PerformConnCheckL( 
            StreamId(), iConnectionId, iPriority, ETrue, aRtoValue,
            iCandidatePair->RemoteCandidate().TransportAddr(), *this );
        iResendInProgress = ETrue;
        iState = EIceCheckInProgress;
        }
    else
        {
        ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::Nominated
// ---------------------------------------------------------------------------
//
TBool CIceConnectivityCheck::Nominated() const
    {
    __ICEDP_INT1( "CIceConnectivityCheck::Nominated:", iIsNominated )
    
    return iIsNominated;
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::SetRemoteCheckInfo
// ---------------------------------------------------------------------------
//
void CIceConnectivityCheck::SetRemoteCheckInfo( 
        TUint aPriority, TBool aFavored )
    {
    __ICEDP( "CIceConnectivityCheck::SetRemoteCheckInfo" )
    
    iRemotePriority = aPriority;
    iRemoteFavored = aFavored;
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::Cancel
// ---------------------------------------------------------------------------
//
void CIceConnectivityCheck::Cancel()
    {
    iConnHandler.CancelCheck( StreamId(), iConnectionId, 
        iCandidatePair->RemoteCandidate().TransportAddr() );
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::StreamId
// ---------------------------------------------------------------------------
//
TUint CIceConnectivityCheck::StreamId() const
    {
    return iCandidatePair->LocalCandidate().StreamId();
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::ComponentId
// ---------------------------------------------------------------------------
//
TUint CIceConnectivityCheck::ComponentId() const
    {
    return iCandidatePair->LocalCandidate().ComponentId();
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::State
// ---------------------------------------------------------------------------
//
CIceConnectivityCheck::TIceCheckState CIceConnectivityCheck::State() const
    {
    return iState;
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::StreamCollectionId
// ---------------------------------------------------------------------------
//
TUint CIceConnectivityCheck::StreamCollectionId() const
    {
    return iCandidatePair->LocalCandidate().StreamCollectionId();
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::ValidatedPair
// ---------------------------------------------------------------------------
//
const CNATFWCandidatePair* CIceConnectivityCheck::ValidatedPair() const
    {
    return iValidatedPair;
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::CandidatePair
// ---------------------------------------------------------------------------
//
const CNATFWCandidatePair& CIceConnectivityCheck::CandidatePair() const
    {
    return *iCandidatePair;
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::ConnCheckCompletedL
// ---------------------------------------------------------------------------
//
void CIceConnectivityCheck::ConnCheckCompletedL(
    TInt aCompletionCode,
#ifdef _DEBUG
    const TInetAddr& aLocalAddr,
#else
    const TInetAddr& /*aLocalAddr*/,
#endif
    const TInetAddr& aPeerAddr,
    const TInetAddr& aMappedAddr )
    {
    __ICEDP_INT1( "CIceConnectivityCheck::ConnCheckCompletedL, COMPLCODE",
        aCompletionCode )
    
    __ICEDP_ADDRLOG( "LOCAL_ADDR", aLocalAddr )
    __ICEDP_ADDRLOG( "PEER_ADDR", aPeerAddr )
    __ICEDP_ADDRLOG( "MAPPED_ADDR", aMappedAddr )
    
    __ASSERT_DEBUG( EIceCheckInProgress == iState, 
        User::Leave( KErrNotReady ) );
    
    TInt completionCode = aCompletionCode;
    if ( !TIceUtils::MatchAddresses( aPeerAddr, 
            iCandidatePair->RemoteCandidate().TransportAddr() ) )
        {
        completionCode = KErrCouldNotConnect;
        }
    
    if ( KErrNone != completionCode )
        {
        iState = EIceCheckFailed;
        if ( iResendInProgress )
            {
            iResendInProgress = EFalse;
            iClient.NominationCompletedL( 
                completionCode, *this, *iValidatedPair );
            }
        else
            {
            iClient.CheckCompletedL( completionCode, *this, NULL );
            }
        
        return;
        }
    
    if ( iResendInProgress )
        {
        iResendInProgress = EFalse;
        if ( iIsNominated )
            {
            iValidatedPair->SetSelected( ETrue );
            iCandidatePair->SetSelected( ETrue );
            }
        
        iClient.NominationCompletedL( completionCode, *this, 
            *iValidatedPair );
        }
    else
        {
        iState = EIceCheckSucceed;
        iValidatedPair = CreateValidatedPairL( aMappedAddr );
        
        CNATFWCandidatePair* validatedPair
            = CNATFWCandidatePair::NewL( *iValidatedPair );
        iClient.CheckCompletedL( completionCode, *this, 
            validatedPair );
        }
    }


// ---------------------------------------------------------------------------
// CIceConnectivityCheck::CreateValidatedPairL
// ICE-17, 7.1.2.2.2, Constructing a Valid Pair
// ---------------------------------------------------------------------------
//
CNATFWCandidatePair* CIceConnectivityCheck::CreateValidatedPairL(
        const TInetAddr& aMappedAddr )
    {
    __ICEDP( "CIceConnectivityCheck::CreateValidatedPairL" )
    
    const CNATFWCandidate* knownLocalCand 
        = iSessionData.FindLocalCandidate( aMappedAddr );
    
    CNATFWCandidate* validatedLocalCand = NULL;
    if ( knownLocalCand )
        {
        validatedLocalCand = CNATFWCandidate::NewLC( *knownLocalCand );
        }
    else
        {
        // Peer reflexive candidate
        validatedLocalCand = CNATFWCandidate::NewLC( 
            iCandidatePair->LocalCandidate() );
        validatedLocalCand->SetTransportAddrL( aMappedAddr );
        validatedLocalCand->SetType( CNATFWCandidate::EPeerReflexive );
        validatedLocalCand->SetPriority( iPriority );
        }
    
    CNATFWCandidate* remoteCand 
        = CNATFWCandidate::NewLC( iCandidatePair->RemoteCandidate() );
    if ( EIceCheckTypeTriggered == iType )
        {
        remoteCand->SetPriority( iRemotePriority );
        }
    
    CNATFWCandidatePair* validatedPair = CNATFWCandidatePair::NewLC(
        *validatedLocalCand, *remoteCand );
    
    if ( iIsNominated )
        {
        // ICE-17, 7.1.2.2.4. Updating the Nominated Flag
        validatedPair->SetSelected( ETrue );
        iCandidatePair->SetSelected( ETrue );
        }
    else
        {
        if ( EIceCheckTypeTriggered == iType && iRemoteFavored )
            {
            // ICE-17, 7.2.1.5. Updating the Nominated Flag
            validatedPair->SetSelected( ETrue );
            iCandidatePair->SetSelected( ETrue );
            }
        }
    
    CleanupStack::Pop( validatedPair );
    CleanupStack::PopAndDestroy( remoteCand );
    CleanupStack::PopAndDestroy( validatedLocalCand );
    return validatedPair;
    }
