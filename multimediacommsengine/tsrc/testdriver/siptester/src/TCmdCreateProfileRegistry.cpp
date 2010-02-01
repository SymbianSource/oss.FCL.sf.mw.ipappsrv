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
#include "TCmdCreateProfileRegistry.h"
#include "CTcSIPProfileContainer.h"

#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include "tclog.h"

#include "CTcSIPProfileContainer.h"
/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:           -
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			RegistryId
 */
void TCmdCreateProfileRegistry::ExecuteL()
	{
	// -- Execution -----------------------------------------------------------

	// Create a new profile container (+profile) and store to registry
	CTcSIPProfileContainer* registry =
						CTcSIPProfileContainer::NewL( iContext, iContext.SIP() );

	// -- Response creation ---------------------------------------------------

	// Add RegistryId to response
    AddIdResponseL( KRegistryId, registry );
	}

TBool TCmdCreateProfileRegistry::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("CreateProfileRegistry") );
	}

TTcCommandBase* TCmdCreateProfileRegistry::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateProfileRegistry( aContext );
	}

