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




#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "cicecheckprioritizer.h"
#include "ciceconnectivitycheck.h"
#include "icecandidatehandlerlogs.h"

/** variables used in priority calculation */
const TUint64 K2ToPowerOf32 = 2^32;
const TUint64 KNumValTwo = 2;

// ======== MEMBER FUNCTIONS ========


CIceCheckPrioritizer::CIceCheckPrioritizer()
    {
    __ICEDP( "CIceCheckPrioritizer::CIceCheckPrioritizer" )
    }


CIceCheckPrioritizer* CIceCheckPrioritizer::NewL()
    {
    __ICEDP( "CIceCheckPrioritizer::NewL" )

    CIceCheckPrioritizer* self = CIceCheckPrioritizer::NewLC();
    CleanupStack::Pop( self );

    return self;
    }


CIceCheckPrioritizer* CIceCheckPrioritizer::NewLC()
    {
    __ICEDP( "CIceCheckPrioritizer::NewLC" )

    CIceCheckPrioritizer* self = new( ELeave ) CIceCheckPrioritizer();
    CleanupStack::PushL( self );
    
    return self;
    }


CIceCheckPrioritizer::~CIceCheckPrioritizer()
    {  
    __ICEDP( "CIceCheckPrioritizer::~CIceCheckPrioritizer" )
    }   


// ---------------------------------------------------------------------------
// CIceCheckPrioritizer::PrioritizePair
// ---------------------------------------------------------------------------
//  
void CIceCheckPrioritizer::PrioritizePair( CNATFWCandidatePair& aPair,
        TNATFWIceRole aCurrentRole ) const
    {
    __ICEDP( "CIceCheckPrioritizer::PrioritizePair" )
    
    /*
    ICE-17, section 5.7.2.
    Let G be the priority for the candidate provided by the controlling
    agent. Let D be the priority for the candidate provided by the
    controlled agent. The priority for a pair is computed as:

      pair priority = 2^32*MIN(G,D) + 2*MAX(G,D) + (G>D?1:0)

    Where G>D?1:0 is an expression whose value is 1 if G is greater than
    D, and 0 otherwise.  This formula ensures a unique priority for each
    pair.
    */
    TUint64 controllingPri( 0 );
    TUint64 controlledPri( 0 );
    if ( EIceRoleControlling == aCurrentRole )
        {
        controllingPri = aPair.LocalCandidate().Priority();
        controlledPri = aPair.RemoteCandidate().Priority();
        }
    else
        {
        controlledPri = aPair.LocalCandidate().Priority();
        controllingPri = aPair.RemoteCandidate().Priority();
        }
    
    TUint64 pairPri = K2ToPowerOf32 * Min( controllingPri, controlledPri )
        + KNumValTwo * Max( controllingPri, controlledPri )
        + ( 
            ( controllingPri > controlledPri ) ? TUint64( 1 ) : TUint64( 0 )
          );
    
    aPair.SetPriority( pairPri );
    }


// ---------------------------------------------------------------------------
// CIceCheckPrioritizer::PrioritizePairs
// ---------------------------------------------------------------------------
//  
void CIceCheckPrioritizer::PrioritizePairs( 
        RPointerArray<CNATFWCandidatePair>& aPairs,
        TNATFWIceRole aCurrentRole ) const
    {
    __ICEDP( "CIceCheckPrioritizer::PrioritizePairs" )
    
    TInt ind( aPairs.Count() );
    while ( ind-- )
        {
        PrioritizePair( *aPairs[ind], aCurrentRole );
        }
    }


// ---------------------------------------------------------------------------
// CIceCheckPrioritizer::PrioritizeCheck
// ---------------------------------------------------------------------------
//  
void CIceCheckPrioritizer::PrioritizeCheck( 
        CIceConnectivityCheck& aCheck, TNATFWIceRole aCurrentRole ) const
    {
    __ICEDP( "CIceCheckPrioritizer::PrioritizeCheck" )
    
    PrioritizePair( 
        const_cast<CNATFWCandidatePair&>( aCheck.CandidatePair() ), 
        aCurrentRole );
    }

   
// ---------------------------------------------------------------------------
// CIceCheckPrioritizer::PrioritizeChecks
// ---------------------------------------------------------------------------
//  
void CIceCheckPrioritizer::PrioritizeChecks( 
        RPointerArray<CIceConnectivityCheck>& aChecks,
        TNATFWIceRole aCurrentRole ) const
    {
    __ICEDP( "CIceCheckPrioritizer::PrioritizeChecks" )
    
    TInt ind( aChecks.Count() );
    while ( ind-- )
        {
        PrioritizeCheck( *aChecks[ind], aCurrentRole );
        }
    }
