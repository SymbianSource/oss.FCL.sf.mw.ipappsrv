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
#include "TCmdUpdateProfile.h"
#include "CTcSIPProfileContainer.h"

#include <sipprofile.h>
#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofiletypeinfo.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	Type*, IapId*, AORs*, PrivateIdentity*, Name*, SigComp*, 
 *					AutoRegistration*, SecurityNegotiation*, Server*,
 *					ServerParameter*, ServerExtensionParameter*, 
 *					ExtensionParameter*, Default*
 *   IDs:			ProfileId, RegistryId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdUpdateProfile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------
	// Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Get profile from the profile registry
	CSIPManagedProfile& profile = GetManagedProfileL( container );

	// -- Execution -----------------------------------------------------------
	SetProfileParamsL( &profile );

	// Saving the profile
	container.ManagedProfileRegistry().SaveL( profile );

	// -- Response creation ---------------------------------------------------
    
	AddProfileIdResponseL( profile );	
	}


TBool TCmdUpdateProfile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("UpdateProfile") );
	}

TTcCommandBase* TCmdUpdateProfile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUpdateProfile( aContext );
	}
