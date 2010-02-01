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

#ifndef __CTCREGISTRY_H__
#define __CTCREGISTRY_H__

// INCLUDES
#include <e32base.h>
#include "TesterConstants.h"

// CLASS DEFINITION
/**
 * CTcRegistry implements a generic object registry. It usually owns
 * contained objects but it also supports externally owned objects.
 * All objects in the registry are associated with a textual name which
 * is automatically generated. Names are based on the object memory
 * address and can be converted to integer values.
 */
class CTcRegistry
	: public CBase
	{
	private: // Nested classes

		// CLASS DEFINITION
		/**
		 * Simple container class for objects in registry
		 */
		class TObjectRef
			{
			public: // enumerations

				/// Object type
				enum TType
					{
					/// Any object, used for T- and M-classes
					EAny,
					/// CBase-derived objcet
					ECBase,
					/// External CBase-derived object, NOT owned
					ECBaseExternal
					};

			public: // Constructors and destructor

				/// Default constructor
				TObjectRef() : iType( EAny ), iObject( NULL ) {};

			public: // Data

				/// Objec type
				TType iType;
				/// Pointer to object. Owned.
				TAny* iObject;
				/// Object name. Owned.
				TBuf8< KTcMaxObjectName > iName;

			};

	public:	// Constructors and destructor

		/// Default constructor.
		IMPORT_C CTcRegistry();

		/// Destructor
		IMPORT_C ~CTcRegistry();

	public: // New methods

		/**
		 * Adds an object to the registry.
		 * Ownership is transferred to registry.
		 * Automaticaly creates a name (e.g. "42005534") for the object.
		 * Function does nothing if the specified object already exists
		 * in the registry.
		 *
		 * NOTE!! This function uses CleanupStack for safe array insertion.
		 * You must not call CleanupStack::PushL()/Pop() when calling this!!
		 *
		 * @param aObject Object pointer.
		 */
		IMPORT_C void AddObjectL( TAny* aObject );

		/**
		 * Adds a CBase-derived object to the registry.
		 * Ownership is transferred to registry.
		 * Automaticaly creates a name (e.g. "42005534") for the object.
		 * Function does nothing if the specified object already exists
		 * in the registry.
		 *
		 * NOTE!! This function uses CleanupStack for safe array insertion.
		 * You must not call CleanupStack::PushL()/Pop() when calling this!!
		 *
		 * @param aObject Object pointer
		 */
		IMPORT_C void AddObjectL( CBase* aObject );

		/**
		 * Adds a CBase-derived object to the registry.
		 * Ownership is NOT transferred to registry.
		 * Automaticaly creates a name (e.g. "42005534") for the object.
		 * Function does nothing if the specified object already exists
		 * in the registry.
		 *
		 * @param aObject Object pointer
		 */
		IMPORT_C void AddObjectL( const CBase& aObject );

		/**
		 * Queries if the specified object is registered.
		 *
		 * @param aObject Object pointer
		 * @return ETrue if object was found, EFalse otherwise
		 */
		IMPORT_C TBool IsRegistered( const TAny* aObject );

		/**
		 * Finds an object by name. Ownership is not transferred.
		 *
		 * @param aObjectName The registry is searched for an object with
		 *					  this name
		 * @return Pointer to the object mathing the name.
		 * @exceptions Leave with KErrNotFound if the object was not found.
		 */
		IMPORT_C TAny* ObjectPtrL( const TDesC8& aObjectName );

		/**
		 * Looks up the name of a contained object.
		 *
		 * @param aObject Object pointer
		 * @return A descriptor pointing to the object's name
		 * @exceptions Leave with KErrNotFound if the object was not found.
		 */
		IMPORT_C TPtrC8 ObjectNameL( const TAny* aObject ) const;

		/**
		 * Removes an object from the registry. The object is deleted if
		 * it is owned by the array.
		 * The function does nothing if the specified object is not found
		 *
		 * @param aObjecName Object name to be searched for
		 */
		IMPORT_C void DestroyObject( const TDesC8& aObjectName );

		/**
		 * Removes an object from the registry. The object is deleted if
		 * it is owned by the array.
		 * The function does nothing if the specified object is not found
		 *
		 * @param aObject Object pointer to be searched for or NULL
		 */
		IMPORT_C void DestroyObject( TAny* aObject );

		/// Empties the registry, deleting all owned objects. External
		/// object are just removed.
		IMPORT_C void DestroyAll();

		/// @return The number of objects in the registry.
		IMPORT_C TInt Count() const;

	private: // New methods

		/**
		 * Adds an object to the registry. Ownership is transferred to registry
		 * if aType != TObjectRef::CBaseExternal
		 *
		 * @param aObjecName Object name
		 * @param aObject Object pointer
		 * @param aType Object type
		 */
		void AddObjectL( const TDesC8& aObjectName, TAny* aObject,
						 TObjectRef::TType aType );

		/**
		 * Finds an object by name
		 *
		 * @param aObjectName The registry is searched for an object with
		 *					  this name
		 * @return Object index
		 * @exceptions Leaves with KErrNotFund if object was not found
		 */
		TInt FindL( const TDesC8& aObjectName ) const;

		/**
		 * Finds an object by pointer
		 *
		 * @param aObject The registry is searched for an object with
		 *				  this pointer
		 * @return Object index
		 * @exceptions Leaves with KErrNotFund if object was not found
		 */
		TInt FindL( const TAny* aObject ) const;

		/**
		 * Finds an object by pointer
		 *
		 * @param aObject The registry is searched for an object with
		 *				  this pointer
		 * @return Object index
		 */
		TInt Find( const TAny* aObject ) const;

		/**
		 * Deletes the specified object. Does not remove the object reference
		 * from the registry, however.
		 *
		 * @param aObjRef Object reference
		 */
		void Delete( TObjectRef& aObjRef );

	private: // Data

		/// Registry array. Owned.
		CArrayFixSeg< TObjectRef > iRegistry;

	};

#endif // __CTCREGISTRY_H__
