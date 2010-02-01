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
#include "TCmdCreateManagedProfileRegistry.h"
#include "TCmdCreateProfile.h"

#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include "tclog.h"

#include "CTcSIPProfileContainer.h"
/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	ProfileType*
 *   IDs:           -
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			RegistryId
 */
void TCmdCreateManagedProfileRegistry::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

    // Create a new profile, if type parameter is not found use default 
	// Internet, IETF
	CSIPManagedProfile* registry = NULL;
	CTcStructure* structure = FindStructureL( KParamProfileType, EFalse );
	if( structure )
	    {
	    registry = iContext.Profile().ManagedProfileRegistry().CreateL( 
	                    TCmdCreateProfile::ProfileTypeInfoL( structure ) );
	    }
	else
	    {
	    TSIPProfileTypeInfo typeInfo;
	    typeInfo.iSIPProfileClass = TSIPProfileTypeInfo::EInternet;
	    typeInfo.iSIPProfileName = KIETF;
	    registry = iContext.Profile().ManagedProfileRegistry().CreateL( typeInfo );
	    }
	CleanupStack::PushL( registry );
	
	// -- Execution -----------------------------------------------------------
	
	// Saving the profile
	iContext.Profile().ManagedProfileRegistry().SaveL( *registry );

	CleanupStack::Pop( registry );

	// -- Response creation ---------------------------------------------------

	// Add RegistryId to response
    AddIdResponseL( KRegistryId, registry );
	}

TBool TCmdCreateManagedProfileRegistry::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("CreateManagedProfileRegistry") );
	}

TTcCommandBase* TCmdCreateManagedProfileRegistry::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateManagedProfileRegistry( aContext );
	}

