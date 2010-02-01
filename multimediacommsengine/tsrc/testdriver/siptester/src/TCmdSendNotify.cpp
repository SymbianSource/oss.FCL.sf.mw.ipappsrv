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
#include <sipnotifydialogassoc.h>
#include <sipdialog.h>
#include <sipregistrationbinding.h>

#include "CTcSIPConnectionContainer.h"
#include "TCmdSendNotify.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			NotifyDialogId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, DialogId
 */
void TCmdSendNotify::ExecuteL()
	{

	// Get SIP objects from registry
	CSIPNotifyDialogAssoc* notifyDialogAssoc =
										GetNotifyDialogAssocL( );

	// -- Execution -----------------------------------------------------------

	// Start SIP Invite transaction.
	CSIPClientTransaction* transaction = notifyDialogAssoc->SendNotifyL();

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	AddIdResponseL( KDialogId, notifyDialogAssoc->Dialog() );
	}

TBool TCmdSendNotify::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendNotify") );
	}

TTcCommandBase* TCmdSendNotify::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendNotify( aContext );
	}
