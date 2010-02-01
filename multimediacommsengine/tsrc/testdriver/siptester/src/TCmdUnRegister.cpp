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

#include <sipclienttransaction.h>
#include <sipregistrationbinding.h>

#include "TCmdUnregister.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		*
 *   Parameters:	-
 *   IDs:			RegistrationId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdUnregister::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPRegistrationBinding* registration = GetRegistrationL();	

	// -- Execution -----------------------------------------------------------

    CSIPMessageElements* elements = ExtractHeadersAndContentLC();
	CSIPClientTransaction* transaction = registration->DeregisterL( elements );
 	CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdUnregister::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("Unregister") );
	}

TTcCommandBase* TCmdUnregister::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUnregister( aContext );
	}

