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
#include "TCmdDestroyProfile.h"

#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofile.h>
#include <sipprofiletypeinfo.h>

#include "CTcSIPProfileContainer.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:
 *   IDs:			ProfileId, RegistryId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdDestroyProfile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Get profile from the profile registry
	CSIPManagedProfile& profile = GetManagedProfileL( container );

	// -- Execution -----------------------------------------------------------

	// Deleting profile from the persistent storage
	container.ManagedProfileRegistry().DestroyL( profile );
	
	// -- Response creation ---------------------------------------------------

	}


TBool TCmdDestroyProfile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("DestroyProfile") );
	}

TTcCommandBase* TCmdDestroyProfile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdDestroyProfile( aContext );
	}


