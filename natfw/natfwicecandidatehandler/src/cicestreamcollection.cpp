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
* Description:    Represents stream collection.
*
*/




#include "cicestreamcollection.h"
#include "icecandidatehandlerlogs.h"

// ======== MEMBER FUNCTIONS ========

CIceStreamCollection::CIceStreamCollection( TUint aCollectionId )
    :
    iCollectionId( aCollectionId )
    {
    __ICEDP( "CIceStreamCollection::CIceStreamCollection" )
    }
     

CIceStreamCollection* CIceStreamCollection::NewL( TUint aCollectionId )
    {
    __ICEDP( "CIceStreamCollection::NewL" )

    CIceStreamCollection* self 
        = CIceStreamCollection::NewLC( aCollectionId );
    CleanupStack::Pop( self );
    
    return self;
    }


CIceStreamCollection* CIceStreamCollection::NewLC( TUint aCollectionId )
    {
    __ICEDP( "CIceStreamCollection::NewLC" )

    CIceStreamCollection* self 
        = new( ELeave ) CIceStreamCollection( aCollectionId );
    CleanupStack::PushL( self );
    
    return self;
    }
    
  
CIceStreamCollection::~CIceStreamCollection()
    {
    __ICEDP( "CIceStreamCollection::~CIceStreamCollection" )
    
    iMediaComponents.Close();
    }
    
    
// ---------------------------------------------------------------------------
// CIceStreamCollection::StreamCollectionId
// ---------------------------------------------------------------------------
//
TUint CIceStreamCollection::StreamCollectionId() const
    {
    __ICEDP( "CIceStreamCollection::StreamCollectionId" )
    
    return iCollectionId;
    }
    

// ---------------------------------------------------------------------------
// CIceStreamCollection::AddMediaComponentL
// ---------------------------------------------------------------------------
//
void CIceStreamCollection::AddMediaComponentL( TUint aStreamId, 
        TUint aComponentId )
    {
    __ICEDP( "CIceStreamCollection::AddMediaComponentL" )
    
    TIceMediaComponent item = TIceMediaComponent( aStreamId, aComponentId );
    TInt ind( iMediaComponents.Find( item ) );
    __ASSERT_DEBUG( KErrNotFound == ind, User::Leave( KErrAlreadyExists ) );
    
    iMediaComponents.AppendL( item );
    }


// ---------------------------------------------------------------------------
// CIceStreamCollection::RemoveMediaComponent
// ---------------------------------------------------------------------------
//
void CIceStreamCollection::RemoveMediaComponent( TUint aStreamId )
    {
    __ICEDP( "CIceStreamCollection::RemoveMediaComponent" )
    
    for ( TInt i( iMediaComponents.Count() - 1 ); i >= 0; --i )
        {
        if ( iMediaComponents[i].StreamId() == aStreamId )
            {
            iMediaComponents.Remove( i );
            }
        }
    }


// ---------------------------------------------------------------------------
// CIceStreamCollection::HasMediaComponent
// ---------------------------------------------------------------------------
//
TBool CIceStreamCollection::HasMediaComponent( TUint aStreamId ) const
    {
    TInt count( iMediaComponents.Count() );
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( iMediaComponents[i].StreamId() == aStreamId )
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CIceStreamCollection::NumOfMediaComponents
// ---------------------------------------------------------------------------
//
TInt CIceStreamCollection::NumOfMediaComponents() const
    {
    return iMediaComponents.Count();
    }
