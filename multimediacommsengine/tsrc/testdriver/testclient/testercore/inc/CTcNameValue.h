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

#ifndef __CTCNAMEVALUE_H__
#define __CTCNAMEVALUE_H__

// INCLUDES
#include <e32base.h>
#include "MTcTyped.h"

// CLASS DEFINITION
/**
 * CTcNameValue implements a name-value pair container. Both Name and Value
 * are stored as strings. The Value field can also be accessed as integer.
 */
class CTcNameValue
	: public CBase,
	  public MTcTyped
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor. Leaves pointer to cleanup stack.
		 *
		 * @return An initialized instance of this class.
		 */
		IMPORT_C static CTcNameValue* NewLC();

		/// Destructor
		IMPORT_C ~CTcNameValue();

	private:	// Constructors and destructor

		/// Default constructor.
		CTcNameValue();

		/// 2nd phase constructor.
		void ConstructL();

	public: // From MTcTyped

		MTcTyped::TParameterType ParameterType() const;
		TPtrC8 Name() const;

	public: // New methods

		/**
		 * Sets both the Name and Value fields of this object.
		 *
		 * @param aName String value to be copied.
		 * @param aValue String value to be copied.
		 * @exceptions Leaves with KErrNoMemory if either of the new values
		 *			   could not be assigned. Contents of Type and Value are
		 *			   not guaranteed to be either old or new in that case.
		 */
		IMPORT_C void SetL( const TDesC8& aName, const TDesC8& aValue );

		/**
		 * Sets both the Name and Value fields of this object.
		 *
		 * @param aName String value to be copied.
		 * @param aValue Integer value to be converted
		 * @exceptions Leaves with KErrNoMemory if either of the new values
		 *			   could not be assigned. Contents of Type and Value are
		 *			   not guaranteed to be either old or new in that case.
		 */
		IMPORT_C void SetL( const TDesC8& aName, TInt aValue );
		
		/**
		 * Sets both the Name and Value fields of this object.
		 *
		 * @param aName String value to be copied.
		 * @param aValue Boolean value to be converted
		 * @exceptions Leaves with KErrNoMemory if either of the new values
		 *			   could not be assigned. Contents of Type and Value are
		 *			   not guaranteed to be either old or new in that case.
		 */
		IMPORT_C void SetBooleanL( const TDesC8& aName, TBool aValue );

		/**
		 * Sets the Name field of this object to that of aName.
		 *
		 * @param aType String value to be copied.
		 * @exceptions Leaves with KErrNoMemory if a the new type could not
		 *			   be assigned. In that case the original Type is unchanged
		 */
		IMPORT_C void SetNameL( const TDesC8& aName );

		/**
		 * Sets the Value field of this object to that of aValue.
		 *
		 * @param aValue String value to be copied.
		 * @exceptions Leaves with KErrNoMemory if a the new value could not
		 *			   be assigned. In that case the original Value is unchanged
		 */
		IMPORT_C void SetValueL( const TDesC8& aValue );

		/**
		 * Sets the Value field of this object to a textual representation of
		 * the specified integer value.
		 *
		 * @param aValue Integer value to be converted
		 * @exceptions Leaves with KErrNoMemory if a the new value could not
		 *			   be assigned. In that case the original Value is unchaned
		 */
		IMPORT_C void SetValueL( TInt aValue );
		
	    /**
		 * Sets the Value field of this object to a textual representation of
		 * the specified boolean value.
		 *
		 * @param aValue Boolean value to be converted
		 * @exceptions Leaves with KErrNoMemory if a the new value could not
		 *			   be assigned. In that case the original Value is unchaned
		 */
		IMPORT_C void SetBooleanValueL( TBool aValue );

		/// @return Value string as pointer descriptor.
		IMPORT_C TPtrC8 Value() const;
        
		/// @return Convert value string to integer and return it, ignores conversion errors.
		IMPORT_C TInt ValueAsInt() const;
		
		/** 
        * 
        * Converts value to integer
        * @return value string as integer
        * @exceptions Leaves if value cannot be converted (all characters have to
        *               be numbers e.g. "14")
        */
        IMPORT_C TInt ValueAsIntL() const;
        
        /** 
        * 
        * Converts value to boolean
        * @return value string as boolean
        * @exceptions Leaves if value cannot be converted (has to be "true" of "false")
        */
        IMPORT_C TBool ValueAsBooleanL() const;
        
		/** 
        * 
        * Converts name to integer
        * @return name string as integer
        * @exceptions Leaves if name cannot be converted (all characters have to
        *               be numbers e.g. "10001")
        */
        IMPORT_C TInt NameAsIntL() const;

	private: // Data

		/// Name string. Owned
		HBufC8* iName;

		/// Value string. Owned
		HBufC8* iValue;

	};

#endif // __CTCNAMEVALUE_H__
