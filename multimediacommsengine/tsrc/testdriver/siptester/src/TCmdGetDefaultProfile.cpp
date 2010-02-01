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
#include "TCmdGetDefaultProfile.h"
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
 *	 Parameters:	Type, IapId, AORs, PrivateIdentity, Name, SigComp, 
 *					AutoRegistration, SecurityNegotiation, Server,
 *					ServerParameter, ServerExtensionParameter, 
 *					ExtensionParameter, Default, Enabled
 *	 IDs:			ProfileId
 */
void TCmdGetDefaultProfile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Get profile from the profile registry
	CSIPManagedProfile& profile = container.GetDefaultProfileL();

	
	// -- Execution -----------------------------------------------------------


	// -- Response creation ---------------------------------------------------

	GetProfileParamsL( container, &profile );

	// Add ProfileId to response
	TUint32 profileId( 0 );
	User::LeaveIfError( profile.GetParameter( KSIPProfileId, profileId ) );
	AddIntegerResponseL( KParamProfileId, profileId );
	}


TBool TCmdGetDefaultProfile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetDefaultProfile") );
	}

TTcCommandBase* TCmdGetDefaultProfile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetDefaultProfile( aContext );
	}


