/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#include "mcedictionary.h"

// -----------------------------------------------------------------------------
// CMceDictionaryItem::CMceDictionaryItem
// -----------------------------------------------------------------------------
//
CMceDictionaryItem::CMceDictionaryItem( TInt aKey ):iKey( aKey )
    {
    }

// -----------------------------------------------------------------------------
// CMceDictionaryItem::Key
// -----------------------------------------------------------------------------
//
TInt CMceDictionaryItem::Key()
    {
    return iKey;
    }

// -----------------------------------------------------------------------------
// CMceDictionary::~CMceDictionary
// -----------------------------------------------------------------------------
//
CMceDictionary::~CMceDictionary()
 	{
 	iPairs.ResetAndDestroy();
	}

// -----------------------------------------------------------------------------
// CMceDictionary::Pair
// -----------------------------------------------------------------------------
//
CMceDictionaryItem* CMceDictionary::Pair( TInt aKey ) const
    {
    CMceDictionaryItem* pair = NULL;
    for ( TInt i = 0; !pair && i < iPairs.Count(); i++  )
        {
        pair = iPairs[ i ];
        if ( pair->Key() == aKey )
            {
            // NOP
            }
        else
            {
            pair = NULL;
            }
        }
    return pair;
    }

// -----------------------------------------------------------------------------
// CMceDictionary::AddL
// -----------------------------------------------------------------------------
//
void CMceDictionary::AddL( CMceDictionaryItem* aPair )
    {
    iPairs.AppendL( aPair );
    }

// -----------------------------------------------------------------------------
// CMceDictionary::Delete
// -----------------------------------------------------------------------------
//
void CMceDictionary::Delete( TInt aKey )
    {
    TBool found = EFalse;
    for ( TInt i = 0; !found && i < iPairs.Count(); i++  )
        {
        CMceDictionaryItem* pair = iPairs[ i ];
        if ( pair->Key() == aKey )
            {
            iPairs.Remove( i );
            delete pair;
            found = ETrue;
            }
        }
    }
    
// End of file
