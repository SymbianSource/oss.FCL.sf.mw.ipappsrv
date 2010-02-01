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

#ifndef __TCMDCREATEPROFILE_H__
#define __TCMDCREATEPROFILE_H__

// INCLUDES
#include "TTcSIPCommandBase.h"
#include <sipprofile.h>

// CLASS DEFINITION
/**
 * Command class responsible for "CreateProfile" functionality.
 */
class TCmdCreateProfile
	: public TTcSIPCommandBase
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP Test case context
		 */
		TCmdCreateProfile( MTcTestContext& aContext )
						   : TTcSIPCommandBase( aContext ) {};

	public: // From TTcSIPCommandBase

		void ExecuteL();

	public: // New methods

		/**
		 * Static function for matching the name of this command to
		 * a function identifier.
		 *
		 * @param aId An initialized identifier containing a function name.
		 * @return ETrue if this command matches the specified name.
		 */
		static TBool Match( const TTcIdentifier& aId );

		/**
		 * static function for creating an instance of this class
		 *
		 * @param aContext SIP Test case context
		 * @return An initialized instance of this class.
		 */
		static TTcCommandBase* CreateL( MTcTestContext& aContext );
		
		/**
		 * Function for inquiring profile type info
		 *
		 * @param aStruct Incoming SIP profile structure
		 * @return type info
		 */
		 static TSIPProfileTypeInfo ProfileTypeInfoL( CTcStructure* aStruct );
		 
		 /**
		 * Function for inquiring profile type info class
		 *
		 * @param aStruct Incoming SIP profile structure
		 * @return type info
		 */
		 static TSIPProfileTypeInfo::TSIPProfileClass ProfileTypeClassL( CTcStructure* aStruct );

	protected: // New methods
		/**
		 * Function for setting profile parameters
		 *
		 * @param aProfile Incoming profile
		 */
		void SetProfileParamsL( CSIPManagedProfile* aProfile );

		/**
		 * Function for inquiring server type
		 *
		 * @param aStruct Incoming SIP profile structure
		 * @return server type
		 */
		TUint32 ServerL( CTcStructure* aStruct );

		/**
		 * Function for inquiring server type
		 *
		 * @param aStruct Incoming SIP profile structure
		 * @return server parameter
		 */
		TUint32 ServerParameterL( CTcStructure* aStruct );

	};

#endif // __TCMDCREATEPROFILE_H__
