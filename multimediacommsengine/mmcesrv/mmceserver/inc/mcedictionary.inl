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



// -----------------------------------------------------------------------------
// CMceKeyTValuePair::CMceKeyTValuePair
// -----------------------------------------------------------------------------
//
template <class T>
CMceKeyTValuePair<T>::CMceKeyTValuePair( TInt aKey, T aValue ):
    CMceDictionaryItem( aKey ), iValue( aValue )
    {
    }

// -----------------------------------------------------------------------------
// CMceKeyTValuePair::Value
// -----------------------------------------------------------------------------
//
template <class T>
T& CMceKeyTValuePair<T>::Value()
    {
    return iValue;
    }

// -----------------------------------------------------------------------------
// CMceKeyTValuePair::SetValue
// -----------------------------------------------------------------------------
//
template <class T>
void CMceKeyTValuePair<T>::SetValue( T aValue )
    {
    iValue = aValue;
    }

// -----------------------------------------------------------------------------
// CMceKeyCValuePair::CMceKeyCValuePair
// -----------------------------------------------------------------------------
//
template <class C>
CMceKeyCValuePair<C>::CMceKeyCValuePair( TInt aKey, C* aValue ):
    CMceDictionaryItem( aKey ), iValue( aValue )
    {
    }

// -----------------------------------------------------------------------------
// CMceKeyCValuePair::Value
// -----------------------------------------------------------------------------
//
template <class C>
C* CMceKeyCValuePair<C>::Value()
    {
    return iValue;
    }

// -----------------------------------------------------------------------------
// CMceKeyCValuePair::SetValue
// -----------------------------------------------------------------------------
//
template <class C>
void CMceKeyCValuePair<C>::SetValue( C* aValue )
    {
    delete iValue;
    iValue = aValue;
    }

// -----------------------------------------------------------------------------
// CMceKeyCValuePair::CMceKeyCValuePair
// -----------------------------------------------------------------------------
//
template <class C>
CMceKeyCValuePair<C>::~CMceKeyCValuePair()
    {
    delete iValue;
    }
    
// End of file
