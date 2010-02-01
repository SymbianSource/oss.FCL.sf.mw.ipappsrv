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
* Description:    Represents ICE valid list.
*
*/




#include "natfwcredentials.h"
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"
#include "cicevalidlist.h"
#include "icecandidatehandlerlogs.h"

// ======== MEMBER FUNCTIONS ========

CIceValidList::CIceValidList( TUint aCollectionId )
    :
    iCollectionId( aCollectionId )
    {
    __ICEDP( "CIceValidList::CIceValidList" )
    }
     

CIceValidList* CIceValidList::NewL( TUint aCollectionId )
    {
    __ICEDP( "CIceValidList::NewL" )

    CIceValidList* self 
        = CIceValidList::NewLC( aCollectionId );
    CleanupStack::Pop( self );
    
    return self;
    }


CIceValidList* CIceValidList::NewLC( TUint aCollectionId )
    {
    __ICEDP( "CIceValidList::NewLC" )

    CIceValidList* self 
        = new( ELeave ) CIceValidList( aCollectionId );
    CleanupStack::PushL( self );
    
    return self;
    }
    
  
CIceValidList::~CIceValidList()
    {
    __ICEDP( "CIceValidList::~CIceValidList" )
    
    iValidList.ResetAndDestroy();
    }
    
    
// ---------------------------------------------------------------------------
// CIceValidList::StreamCollectionId
// ---------------------------------------------------------------------------
//
TUint CIceValidList::StreamCollectionId() const
    {
    __ICEDP( "CIceValidList::StreamCollectionId" )
    
    return iCollectionId;
    }


// ---------------------------------------------------------------------------
// CIceValidList::AddValidPairL
// ---------------------------------------------------------------------------
//
void CIceValidList::AddValidPairL( const CNATFWCandidatePair& aPair )
    {
    __ICEDP( "CIceValidList::AddValidPairL" )
    
    // check if existing pair needs to be updated
    TBool existingPairFound( EFalse );
    TInt validPairInd( iValidList.Count() - 1 );
    while ( !existingPairFound && 0 <= validPairInd )
        {
        if ( aPair.LocalCandidate().ComponentId() 
                == iValidList[validPairInd]->LocalCandidate().ComponentId()
            && aPair.Priority() == iValidList[validPairInd]->Priority() )
            {
            existingPairFound = ETrue;
            iValidList[ validPairInd ]->CopyL( aPair );
            }
        
        validPairInd--;
        }
    
    if ( !existingPairFound )
        {
        // add new pair to the valid list
        CNATFWCandidatePair* newPair = CNATFWCandidatePair::NewLC( aPair );
        iValidList.InsertInOrderL( 
            newPair, CNATFWCandidatePair::PriorityOrder );
        CleanupStack::Pop( newPair );
        }
    }


// ---------------------------------------------------------------------------
// CIceValidList::HasPair
// ---------------------------------------------------------------------------
//
TBool CIceValidList::HasPair( const CNATFWCandidatePair& aPair ) const
    {
    __ICEDP( "CIceValidList::HasPair" )
    
    TInt ind = iValidList.Find( &aPair, CNATFWCandidatePair::MatchAddresses );
    return ( KErrNotFound != ind );
    }


// ---------------------------------------------------------------------------
// CIceValidList::HasPairForComponents
// ---------------------------------------------------------------------------
//
TBool CIceValidList::HasPairForComponents( 
        RArray<TUint>& aComponentIds ) const
    {
    __ICEDP( "CIceValidList::HasPairForComponents" )
    
    TInt numOfComponents( aComponentIds.Count() );
    ASSERT( numOfComponents );
    TInt numOfValidPairs( iValidList.Count() );
    
    for ( TInt i( 0 ); i < numOfComponents; ++i )
        {
        TUint compId( aComponentIds[i] );
        
        TBool validPairExists( EFalse );
        for ( TInt j( 0 ); j < numOfValidPairs; j++ )
            {
            if ( iValidList[j]->LocalCandidate().ComponentId() == compId )
                {
                validPairExists = ETrue;
                }
            }
        
        if ( !validPairExists )
            {
            __ICEDP( "CIceValidList::HasPairForComponents, EFalse" )
            return EFalse;
            }
        }
    
    __ICEDP( "CIceValidList::HasPairForComponents, ETrue" )
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CIceValidList::SetPairNominatedL
// ---------------------------------------------------------------------------
//
void CIceValidList::SetPairNominatedL( const CNATFWCandidatePair& aPair )
    {
    __ICEDP( "CIceValidList::SetPairNominatedL" )
    
    TInt ind = iValidList.Find( &aPair, CNATFWCandidatePair::MatchAddresses );
    User::LeaveIfError( ind );
    
    iValidList[ind]->SetSelected( ETrue );
    }


// ---------------------------------------------------------------------------
// CIceValidList::NominatedPairsExist
// ---------------------------------------------------------------------------
//
TBool CIceValidList::NominatedPairsExist( 
        RArray<TUint>& aComponentIds ) const
    {
    __ICEDP( "CIceValidList::NominatedPairsExist" )
    
    TInt ind( aComponentIds.Count() );
    ASSERT( aComponentIds.Count() );
    
    while ( ind-- )
        {
        if ( NULL == SelectedPair( aComponentIds[ind] ) )
            {
            return EFalse;
            }
        }
    
    return ETrue;
    }


// ---------------------------------------------------------------------------
// CIceValidList::SelectedPair
// ---------------------------------------------------------------------------
//
const CNATFWCandidatePair* 
    CIceValidList::SelectedPair( TUint aComponentId ) const
    {
    __ICEDP( "CIceValidList::SelectedPair" )
    
    TInt count( iValidList.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        CNATFWCandidatePair* item( iValidList[i] );
        if ( item->Selected() 
            && item->LocalCandidate().ComponentId() == aComponentId )
            {
            return item;
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CIceValidList::HighestPriorityPair
// ---------------------------------------------------------------------------
//
const CNATFWCandidatePair* CIceValidList::HighestPriorityPair( 
        TUint aComponentId ) const
    {
    __ICEDP( "CIceValidList::HighestPriorityPair" )
    
    TInt count( iValidList.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        CNATFWCandidatePair* item( iValidList[i] );
        if ( item->LocalCandidate().ComponentId() == aComponentId )
            {
            return item;
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// CIceValidList::FlushValidList
// ---------------------------------------------------------------------------
//
void CIceValidList::FlushValidList()
    {
    __ICEDP( "CIceValidList::FlushValidList" )
    
    iValidList.ResetAndDestroy();
    }
