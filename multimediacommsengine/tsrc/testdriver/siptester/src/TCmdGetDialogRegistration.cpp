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
#include "TCmdGetDialogRegistration.h"

#include <sipdialog.h>
#include <sipdialogassocbase.h>
#include <sipregistrationbinding.h>
#include <sipregistrationcontext.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			InviteDialogId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			RegistrationId*
 */
void TCmdGetDialogRegistration::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPDialogAssocBase* dialogAssoc = GetAnyDialogAssocL();

	// -- Execution -----------------------------------------------------------

	// Get registration context for the diaog, if any
	const CSIPDialog& dialog = dialogAssoc->Dialog();
	MSIPRegistrationContext* context =
		const_cast< MSIPRegistrationContext* >( dialog.RegistrationContext() );
	CSIPRegistrationBinding* registration =
								static_cast< CSIPRegistrationBinding* >( context );
	if( !registration )
		{
		return;
		}

	// If the object is not in registry, add it.
	if( !iContext.Registry().IsRegistered( registration ) )
		{
		iContext.Registry().AddObjectL( registration );
		}

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KRegistrationId, registration );
	}

TBool TCmdGetDialogRegistration::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetDialogRegistration") );
	}

TTcCommandBase* TCmdGetDialogRegistration::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetDialogRegistration( aContext );
	}
