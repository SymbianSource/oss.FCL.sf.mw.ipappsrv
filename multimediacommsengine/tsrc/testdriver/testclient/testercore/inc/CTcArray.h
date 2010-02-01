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
* Description:  See class definition below.
*
*/

#ifndef __CTCARRAY_H__
#define __CTCARRAY_H__

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "MTcTyped.h"
#include "TesterConstants.h"

// CLASS DEFINITION
/**
 * CTcArray implements a container for textual items.
 * However, items can also be inserted and fetched as integer values.
 */
class CTcArray
	: public CBase,
	  public MTcTyped
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor. Leaves pointer to cleanup stack.
		 *
		 * @return An initialized instance of this class.
		 */
		IMPORT_C static CTcArray* NewLC();

		/// Destructor
		IMPORT_C ~CTcArray();

	private:	// Constructors and destructor

		/// Default constructor.
		CTcArray();

	public: // from MTcTyped

		MTcTyped::TParameterType ParameterType() const;
		TPtrC8 Name() const;

	public: // New methods

		/// @return Number of contained items
		IMPORT_C TInt Count() const;

		/**
		 * Sets the array name. Content of aName is copied.
		 *
		 * @param aName Name as string value.
		 */
		IMPORT_C void SetName( const TDesC8& aName );

		/**
		 * Returns an item as string value.
		 *
		 * @param aIndex Item position (0..Count()-1)
		 * @return Pointer descriptor to the item
		 */
		IMPORT_C TPtrC8 Item( TInt aIndex ) const;

		/**
		 * Returns an item as integer value. Returns 0 in case string-to-int
		 * conversion fails for some reason (e.g. try to convert "abba" to int)
		 *
		 * @param aIndex Item position (0..Count()-1)
		 * @return Value of item as integer
		 */
		IMPORT_C TInt ItemAsInt( TInt aIndex ) const;

		/**
		 * Adds a new item to the array
		 * Content of aItem is copied.
		 *
		 * @param aItem Reference to a string value of the item.
		 */
		IMPORT_C void AddItemL( const TDesC8& aItem );

		/**
		 * Adds a new item to the array.
		 * Value of aItem is converted to a string which is then copied.
		 *
		 * @param aItem An integer value of the item.
		 */
		IMPORT_C void AddItemL( TInt aItem );

		/**
		 * Adds new items to the array. Items are copied from the
		 * provided MDesC8Array.
		 *
		 * @param aItem Reference to a string array
		 */
		IMPORT_C void AddItemsL( const MDesC8Array& aItems );

		/**
		 * Removes an item from the array.
		 *
		 * @param aIndex Item position (0..Count()-1)
		 */
		IMPORT_C void RemoveItem( TInt aIndex );

		/// @return Reference to the content of this arrray as a MDesC8Array.
		IMPORT_C const MDesC8Array& AsMDesCArray() const;

	private: // Data

		/// Item array. Owned.
		CDesC8ArraySeg iArray;

		// Array name. Owned.
		TBuf8< KTcMaxTypeName > iName;

	};

#endif // __CTCARRAY_H__
