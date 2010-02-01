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
#include "TCmdTerminate.h"

#include <sipclienttransaction.h>
#include <sipdialogassocbase.h>
#include <siprefresh.h>
#include <sipregistrationbinding.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			InviteDialogId*, SubscribeDialogId*, RefreshId*,
 *					RegistrationId*, TransactionId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId*
 */
void TCmdTerminate::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	CSIPRegistration* registration = GetRegistrationL( EFalse );
	CSIPClientTransaction* transaction = GetClientTransactionL( EFalse );
	CSIPDialogAssocBase* dialogAssoc= GetAnyDialogAssocL( EFalse );

	// Conditionally create refresh object
	CSIPRefresh* refresh = ExtractRefreshLC();

	// -- Execution -----------------------------------------------------------

	// Destroying NULL objects is safe..
	iContext.Registry().DestroyObject( registration );
	iContext.Registry().DestroyObject( transaction );
	iContext.Registry().DestroyObject( dialogAssoc );

	CSIPClientTransaction* newTransaction;
	if( refresh )
		{
		newTransaction = refresh->TerminateL();
		}
	else
		{
		return;
		}

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, newTransaction );
	}

TBool TCmdTerminate::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("Terminate") );
	}

TTcCommandBase* TCmdTerminate::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdTerminate( aContext );
	}

