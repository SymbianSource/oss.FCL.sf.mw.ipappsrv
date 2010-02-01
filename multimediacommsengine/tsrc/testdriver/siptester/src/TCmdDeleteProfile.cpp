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
#include "TCmdDeleteProfile.h"
#include "CTcSIPProfileContainer.h"

#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofile.h>
#include <sipprofiletypeinfo.h>

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
void TCmdDeleteProfile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// -- Execution -----------------------------------------------------------

    container.DeleteManagedProfileById( ExtractProfileIdL() );
    
	// -- Response creation ---------------------------------------------------

	}


TBool TCmdDeleteProfile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("DeleteProfile") );
	}

TTcCommandBase* TCmdDeleteProfile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdDeleteProfile( aContext );
	}


