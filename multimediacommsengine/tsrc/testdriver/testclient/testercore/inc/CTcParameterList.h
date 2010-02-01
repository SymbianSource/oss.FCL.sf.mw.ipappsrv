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

#ifndef __CTCPARAMETERLIST_H__
#define __CTCPARAMETERLIST_H__

// INCLUDES
#include "CTcArray.h"
#include "CTcStructure.h"
#include "CTcNameValue.h"

// CLASS DEFINITION
/**
 * CTcParameterList implements a container for objects of type
 * CTcArray, CTcStructure and CTcNameValue (all derived from MTcTyped)
 */
class CTcParameterList
	: public CBase
	{
	public:	// Constructors and destructor

		/// Default constructor.
		IMPORT_C CTcParameterList();

		/// Destructor
		IMPORT_C ~CTcParameterList();

	public: // New methods

		/// @return Number of items in the list
		IMPORT_C TInt Count() const;

		/**
		 * Return type of specified parameter (at position aIndex)
		 *
		 * @param aIndex Parameter position (0..Count()-1)
		 */
		IMPORT_C MTcTyped::TParameterType ParameterType( TInt aIndex ) const;

		/**
		 * Return the specified parameter (at position aIndex) as an array.
		 * This function only performs array lookup and typecast.
		 *
		 * @param aIndex Parameter position (0..Count()-1)
		 * @exceptions Leaves with KTcErrRequestCorrupt if the object at
		 *			   aIndex is of unexpected type
		 */
		IMPORT_C CTcArray& AsArrayL( TInt aIndex ) const;

		/**
		 * Return the specified parameter (at position aIndex) as an array.
		 * This function only performs array lookup and typecast.
		 *
		 * @param aIndex Parameter position (0..Count()-1)
		 */
		IMPORT_C CTcArray& AsArray( TInt aIndex ) const;

		/**
		 * Return the specified parameter (at position aIndex) as a structure.
		 * This function only performs array lookup and typecast.
		 *
		 * @param aIndex Parameter position (0..Count()-1)
		 * @exceptions Leaves with KTcErrRequestCorrupt if the object at
		 *			   aIndex is of unexpected type
		 */
		IMPORT_C CTcStructure& AsStructureL( TInt aIndex ) const;

		/**
		 * Return the specified parameter (at position aIndex) as a structure.
		 * This function only performs array lookup and typecast.
		 *
		 * @param aIndex Parameter position (0..Count()-1)
		 */
		IMPORT_C CTcStructure& AsStructure( TInt aIndex ) const;

		/**
		 * Return the specified parameter (at position aIndex) as a name-value
		 * pair. This function only performs array lookup and typecast.
		 *
		 * @param aIndex Parameter position (0..Count()-1)
		 * @exceptions Leaves with KTcErrRequestCorrupt if the object at
		 *			   aIndex is of unexpected type
		 */
		IMPORT_C CTcNameValue& AsNameValueL( TInt aIndex ) const;

		/**
		 * Return the specified parameter (at position aIndex) as a name-value
		 * pair. This function only performs array lookup and typecast.
		 *
		 * @param aIndex Parameter position (0..Count()-1)
		 */
		IMPORT_C CTcNameValue& AsNameValue( TInt aIndex ) const;

		/**
		 * Adds a new parameter item to the list. Ownership is transferred.
		 *
		 * @param aItem Pointer to a typed parameter object.
		 */
		IMPORT_C void AddParameterL( MTcTyped* aItem );

		/**
		 * Insert a new parameter item to the list. Ownership is transferred.
		 *
		 * @param aItem Pointer to a typed parameter object.
		 * @param aIndex Position in the list (0 is the first one)
		 */
		IMPORT_C void InsertParameterL( MTcTyped* aItem, TInt aIndex );

		/// Delete any and all contained parameters.
		IMPORT_C void Reset();

		/**
		 * Find a parameter by name from the list.
		 * Allows searchig for parameters of different type but with
		 * the same name.
		 *
		 * @param aName Parameter name
		 * @param aType Parameter type - specifying EUnknown accepts
		 *				parameters of any type (that's the default).
		 * @return Parameter index (0...Count()-1) or KErrNotFound
		 */
		IMPORT_C TInt FindParameter(
				   const TDesC8& aName,
				   MTcTyped::TParameterType aType = MTcTyped::EUnknown ) const;

		/**
		 * Delete a specific parameter. Deletes the first occurrence, if
		 * there are multiple parameters with the same name.
		 * Allows searching for parameters of different type but with
		 * the same name.
		 *
		 * @param aName Parameter name
		 * @param aType Parameter type - specifying EUnknown accepts
		 *				parameters of any type (that's the default).
		 */
		IMPORT_C void RemoveParameter(
				   const TDesC8& aName,
				   MTcTyped::TParameterType aType = MTcTyped::EUnknown );

	private: // Data

		/// Parameter array. Owned.
		CArrayPtrSeg< MTcTyped > iParameters;

	};

#endif // __CTCPARAMETERLIST_H__
