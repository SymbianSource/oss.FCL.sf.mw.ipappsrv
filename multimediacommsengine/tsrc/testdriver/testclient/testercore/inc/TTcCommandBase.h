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

#ifndef __TTCCOMMANDBASE_H__
#define __TTCCOMMANDBASE_H__

// INCLUDES
#include "CTcContextBase.h"

// FORWARD DECLARATIONS
class TTcIdentifier;

// CLASS DEFINITION
/**
 * TTcCommandBase implements the base class for all tester commands.
 * It provides common functionality for accessing request and creating responses.
 */
class TTcCommandBase
	{
	public:	// Constructors and destructor

		/// Virtual destructor
		IMPORT_C virtual ~TTcCommandBase();

	protected:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext Test case context
		 */
		IMPORT_C TTcCommandBase( CTcContextBase& aContext );

	private: // Constructors and destructor

		/// Default constructor. Not implemented.
		TTcCommandBase();

	public: // Abstract methods

		/// Executes a sequence of test operations.
		virtual void ExecuteL() = 0;

	public: // New methods, for name matching

		/**
		 * Matches the specified CTRL identifier to the function name.
		 *
		 * @param aId CTRL request identifier
		 * @param aFunction Function name
		 * @return ETrue if the specified name matches the identifier.
		 */
		IMPORT_C static TBool Match( const TTcIdentifier& aId,
									 const TDesC8& aFunction );

		/**
		 * Matches the specified CTRL identifier to the container/function pair
		 *
		 * @param aId CTRL request identifier
		 * @param aContainer Container name
		 * @param aFunction Function name
		 * @return ETrue if the specified names match the identifier.
		 */
		IMPORT_C static TBool Match( const TTcIdentifier& aId,
									 const TDesC8& aContainer,
									 const TDesC8& aFunction );

	protected: // New methods

		/**
		 * Adds a Name-Value pair parameter to the CTRL response with
		 * the specified name and integer value translated to string.
		 *
		 * @param aValue Integer parameter value.
		 */
		IMPORT_C void AddIntegerResponseL( const TDesC8& aName, TInt aValue );

		/**
		 * Adds a Name-Value pair parameter to the CTRL response with
		 * name the specified name and boolean value translated to stirng.
		 *
		 * @param aValue Boolean parameter value translated to "true" or "false"
		 */
		IMPORT_C void AddBooleanResponseL( const TDesC8& aName, TBool aValue );

		/**
		 * Adds a Name-Value pair parameter to the CTRL response.
		 *
		 * @param aName Parameter name (e.g. "Address")
		 * @param aValue Parameter value (e.g. "<sip:myaddress@dot.net>")
		 */
		IMPORT_C void AddTextResponseL( const TDesC8& aName,
										const TDesC8& aValue );

		/**
		 * Adds an Array parameter to the CTRL response.
		 *
		 * @param aName Parameter name (e.g. "IAPNames")
		 * @param aValues Parameter values as an array
		 */
		IMPORT_C void AddArrayResponseL( const TDesC8& aName,
										 const MDesC8Array& aValues );

		/**
		 * Adds a Structure parameter to the CTRL response.
		 * Ownership of returned object is not transferred.		 
		 *
		 * @param aName Parameter name (e.g. "IAPNames")
		 * @return Pointer to the initialized structure, ready for
		 *		   adding items.
		 */
		IMPORT_C CTcStructure* AddStructureResponseL( const TDesC8& aName );

		/**
		 * Find a TestClientId item from the CTRL request.
		 * Return the matching object from the object registry.
		 * Ownership of returned object is not transferred.
		 *
		 * @param aName Item name (e.g. "RequestId")
		 * @param aIsMandatory ETrue if the Id is required.
		 * @return Pointer to the object that matches the id retrieved
		 *		   from the Item value or NULL.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if a required
		 *			   Id cannot be found.
		 */
		IMPORT_C TAny* GetObjectForIdL( const TDesC8& aName,
										TBool aMandatory = ETrue ) const;

		/**
		 * Find a Name-Value parameter from the CTRL request.
		 * Convert the found item's value an integer.
		 *
		 * @param aName Parameter name (e.g. "Timeout")
		 * @param aIsMandatory ETrue if the parameter is required.
		 * @return Parameter's value as integer if a parameter with name aName
		 *		   is found. aDefaultValue otherwise.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if
		 *			   a required parameter cannot be found.
		 */
		IMPORT_C TInt ExtractIntegerL( const TDesC8& aName,
									   TInt aDefaultValue = 0,
									   TBool aIsMandatory = ETrue ) const;

		/**
		 * Find a Name-Value parameter from the CTRL request.
		 * Convert the found item's value a TBool.
		 *
		 * @param aName Parameter name (e.g. "Boolean")
		 * @param aIsMandatory ETrue if the parameter is required.
		 * @return ETrue if a parameter with name aName and value KParamTrue
		 *		   is found. EFalse otherwise.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if
		 *			   a required parameter cannot be found.
		 */
		IMPORT_C TBool ExtractBooleanL( const TDesC8& aName,
										TBool aIsMandatory = ETrue ) const;

		/**
		 * Find a Name-Value parameter from the CTRL request.
		 * Return its value as a pointer descriptor.
		 *
		 * @param aName Parameter name (e.g. "IAPName")
		 * @param aIsMandatory ETrue if the parameter is required.
		 * @return Pointer descriptor assigned to parameter's value if
		 *		   a parameter with name aName was found.
		 *		   TPtrC8() (equals KNullDesC8) otherwise.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if
		 *			   a required parameter cannot be found.
		 */
		IMPORT_C TPtrC8 ExtractTextL( const TDesC8& aName,
									  TBool aIsMandatory = ETrue ) const;

		/**
		 * Find a TestClientId item from the CTRL request.
		 *
		 * @param aName Item name (e.g. "RequestId")
		 * @param aIsMandatory ETrue if the Id is required.
		 * @return Pointer descriptor to the item value, or to KNullDesC8
		 *		   if an item with the specified name could not be found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if a required
		 *			   Id cannot be found.
		 */
		IMPORT_C TPtrC8 ExtractIdL( const TDesC8& aName,
									TBool aIsMandatory = ETrue ) const;

		/**
		 * Find a Structure parameter from the CTRL request.
		 * Ownership of returned object is not transferred.
		 *
		 * @param aName Parameter name (e.g. "SecurityParams")
		 * @param aIsMandatory ETrue if the parameter is required.
		 * @return Pointer to a structure object, or NULL
		 *		   if an item with the specified name could not be found.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if
		 *			   a required parameter cannot be found.
		 */
		IMPORT_C CTcStructure* FindStructureL( const TDesC8& aName,
											   TBool aMandatory = ETrue ) const;

		/**
		 * Find an Array parameter from the CTRL request.
		 * Ownership of returned object is not transferred.
		 *
		 * @param aName Parameter name (e.g. "Headers")
		 * @param aIsMandatory ETrue if the parameter is required.
		 * @return Pointer to an array object, or NULL
		 *		   if an item with the specified name could not be found.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if
		 *			   a required parameter cannot be found.
		 */
		IMPORT_C CTcArray* FindArrayL( const TDesC8& aName,
									   TBool aMandatory = ETrue ) const;

		/**
		 * Find a Name-Value parameter from the CTRL request.
		 * Ownership of returned object is not transferred.
		 *
		 * @param aName Parameter name (e.g. "IAPName")
		 * @param aIsMandatory ETrue if the parameter is required.
		 * @return Pointer to a name-value object, or NULL
		 *		   if an item with the specified name could not be found.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if
		 *			   a required parameter cannot be found.
		 */
		IMPORT_C CTcNameValue* FindNameValueL( const TDesC8& aName,
											   TBool aIsMandatory = ETrue ) const;

		/**
		 * Find a Name-Value parameter from the specified
		 * structure object.
		 * Ownership of returned object is not transferred.
		 *
		 * @param aStructure Pointer to a structure or NULL.
		 * @param aName Parameter name (e.g. "IAPName")
		 * @param aIsMandatory ETrue if the parameter is required.
		 * @return Pointer to a name-value object, or NULL
		 *		   if an item with the specified name could not be found.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if
		 *			   a required parameter cannot be found.
		 */
		IMPORT_C CTcNameValue* FindNameValueL( CTcStructure* aStructure,
											   const TDesC8& aName,
											   TBool aIsMandatory ) const;
		
		/**
		 * Checks whether boolean parameter exists
		 * NOTE! return value doesn't represent actual value of the
		 * parameter
		 *
		 * @param aName Parameter name (e.g. "AutoRegistration")
		 * @return true if parameter is found, false otherwise
		 */									   
		IMPORT_C TBool HasBooleanL( const TDesC8& aName ) const;

	protected:	// Data

		/// Reference to the test context. Not owned.
		CTcContextBase& iContextBase;

	};

#endif // __TTCCOMMANDBASE_H__
