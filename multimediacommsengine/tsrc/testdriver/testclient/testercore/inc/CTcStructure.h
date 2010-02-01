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

#ifndef __CTCSTRUCTURE_H__
#define __CTCSTRUCTURE_H__

// INCLUDES
#include <badesca.h>
#include "MTcTyped.h"
#include "CTcNameValue.h"
#include "TesterConstants.h"

// CLASS DEFINITION
/**
 * CTcStructure implements a container for a set of name-value
 * (CTcNameValue) items.
 */
class CTcStructure
	: public CBase,
	  public MTcTyped
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor. Leaves pointer to cleanup stack.
		 *
		 * @return An initialized instance of this class.
		 */
		IMPORT_C static CTcStructure* NewLC();

		/// Destructor
		IMPORT_C ~CTcStructure();

	private:	// Constructors and destructor

		/// Default constructor.
		CTcStructure();

	public: // from MTcTyped

		MTcTyped::TParameterType ParameterType() const;
		TPtrC8 Name() const;

	public: // New methods

		/// @return Number of contained items
		IMPORT_C TInt Count() const;

		/**
		 * Sets the name of the structure.
		 *
		 * @param aName Name as string value. Copied.
		 */
		IMPORT_C void SetName( const TDesC8& aName );

		/**
		 * Returns an item as a name-value pair reference
		 *
		 * @param aIndex Item position (0..Count()-1)
		 * @return Reference to the specified object
		 */
		IMPORT_C const CTcNameValue& Item( TInt aIndex ) const;

		/**
		 * Returns an item as a name-value pair pointer.
		 * Ownership of returned item is not transferred.
		 *
		 * @param aIndex Item position (0..Count()-1)
		 * @return Pointer to the found object or NULL.
		 * @exceptions Leaves with KTcErrMandatoryStructureItemNotFound if
		 *			   aMandatory = ETrue
		 */
		IMPORT_C CTcNameValue* ItemL( TInt aIndex, 
									  TBool aMandatory = ETrue );

		/**
		 * Returns an item as a name-value pair pointer.
		 * Searches the structure for the named item.
		 * Ownership of returned item is not transferred.
		 *
		 * @param aName Item name
		 * @return Pointer to the found object or NULL.
		 */
		IMPORT_C CTcNameValue* Item( const TDesC8& aName );

		/**
		 * Returns an item as a name-value pair pointer
		 * Ownership of returned item is not transferred.
		 *
		 * @param aName Item name
		 * @return Pointer to the found object or NULL.
		 * @exceptions Leaves with KTcErrMandatoryStructureItemNotFound if
		 *			   aMandatory = ETrue
		 */
		IMPORT_C CTcNameValue* ItemL( const TDesC8& aName,
									  TBool aMandatory = ETrue );
		/**
		 * Adds a new item to the array. Ownership is transferred
		 *
		 * @param aItem Pointer to a name-value pair item.
		 * @exceptions Leaves with KErrArgument if aItem = NULL
		 */
		IMPORT_C void AddItemL( CTcNameValue* aItem );

		/**
		 * Creates and adds a new item to the array.
		 *
		 * @param aName String value to be copied.
		 * @param aValue String value to be copied.
		 */
		IMPORT_C void AddItemL( const TDesC8& aName, const TDesC8& aValue );
		
		/**
		 * Creates and adds a new item to the array.
		 *
		 * @param aName String value to be copied.
		 * @param aValue Integer value to be converted
		 */
		IMPORT_C void AddItemL( const TDesC8& aName, TInt aValue );
		
		/**
		 * Creates and adds a new item to the array.
		 *
		 * @param aName String value to be copied.
		 * @param aValue Boolean value to be converted
		 */
		IMPORT_C void AddBooleanItemL( const TDesC8& aName, TBool aValue );

	private: // Data

		/// Item array. Owned.
		CArrayPtrSeg< CTcNameValue > iArray;

		/// Structure name. Owned.
		TBuf8< KTcMaxTypeName > iName;

	};

#endif // __CTCSTRUCTURE_H__
