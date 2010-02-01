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
* Description:  Implementation
*
*/

#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TCmdDestroyAllProfiles.h"
#include "CTcSIPProfileContainer.h"

#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofile.h>
#include <sipprofiletypeinfo.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			RegistryId
 *
 * OUTPUT:
 *	 Parameters:	-
 *	 IDs:			-
 */
void TCmdDestroyAllProfiles::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------
	
    // Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// -- Execution -----------------------------------------------------------

	// Get default profile from the profile registry
	CSIPManagedProfile* profile;

	do {
		profile = static_cast< CSIPManagedProfile* >
				( container.ManagedProfileRegistry().DefaultProfileL() );

		// See if profile was found
		if( profile )
			{
			// Deleting profile from the persistent storage
			container.ManagedProfileRegistry().DestroyL( *profile );
			iContext.Registry().DestroyObject( profile );
			}
		} while( profile );


	// -- Response creation ---------------------------------------------------
	}

TBool TCmdDestroyAllProfiles::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("DestroyAllProfiles") );
	}

TTcCommandBase* TCmdDestroyAllProfiles::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdDestroyAllProfiles( aContext );
	}


