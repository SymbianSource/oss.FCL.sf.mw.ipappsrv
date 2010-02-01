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

#ifndef __TCMDGETPROFILE_H__
#define __TCMDGETPROFILE_H__

// INCLUDES
#include "TCmdCreateProfile.h"
#include <sipprofile.h>

// FORWARD DECLARATIONS
class CSIPManagedProfile;
class TSIPProfileTypeInfo;

// CLASS DEFINITION
/**
 * Command class responsible for "GetProfile" functionality.
 */
class TCmdGetProfile
	: public TCmdCreateProfile
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP Test case context
		 */
		TCmdGetProfile( MTcTestContext& aContext )
						: TCmdCreateProfile( aContext ) {};

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

	protected: // New methods
		
		/**
		 * Function for getting profile parameters
		 *
		 * @param aContainer Profile container of the profile
		 * @param aProfile Incoming profile
		 */
		void GetProfileParamsL( CTcSIPProfileContainer& aContainer,
		                        CSIPManagedProfile* aProfile );

		/**
		 * Function for inquiring profile type
		 *
		 * @param aServer Incoming server type
		 * @param aServerParameter Incoming server paramater
		 * @param aProfile Incoming profile
		 * @return type info
		 */
		void ServerParameterL( TUint32 aServer,
							   TUint32 aServerParameter,
							   const CSIPProfile* aSipProfile );
		
		/**
		 * Function for converting server type to string
		 *
		 * @param aServer Incoming server type
		 * @return server type as string
		 */		
		TPtrC8 ServerToString( TUint32 aServer );

	};

#endif // __TCMDGETPROFILE_H__
