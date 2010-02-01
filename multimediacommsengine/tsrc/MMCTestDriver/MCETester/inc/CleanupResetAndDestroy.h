/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Inlined implementation of CleanupResetAndDestroyPushL()
*
*/



#ifndef __CLEANUPRESETANDDESTROY_H__
#define __CLEANUPRESETANDDESTROY_H__

// INCLUDES
#include <e32base.h>

// CLASS DEFINITION
/**
 * Template class for cleaning up arrays that have a ResetAndDestroy() function.
 * To be used with the CleanupStack.
 */
template <class T>
class CleanupResetAndDestroy
	{
	public:	// New functions

		inline static void PushL( T& aRef );

	private: // New functions

		static void ResetAndDestroy( TAny *aPtr );

	};

// INLINE FUNCTIONS
template <class T>
inline void CleanupResetAndDestroy< T >::PushL( T& aRef )
	{
	CleanupStack::PushL( TCleanupItem( &ResetAndDestroy, &aRef ) );
	}

template <class T>
void CleanupResetAndDestroy<T>::ResetAndDestroy( TAny *aPtr )
	{ ( static_cast< T* >( aPtr ) )->ResetAndDestroy(); }

template <class T>
inline void CleanupResetAndDestroyPushL( T& aRef )
	{ CleanupResetAndDestroy< T >::PushL( aRef ); }

#endif // __CLEANUPRESETANDDESTROY_H__
