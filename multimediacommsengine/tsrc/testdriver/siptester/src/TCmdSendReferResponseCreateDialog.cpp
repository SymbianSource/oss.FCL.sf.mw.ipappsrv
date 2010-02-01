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

#include <sipinvitedialogassoc.h>
#include <sipnotifydialogassoc.h>
#include <sipservertransaction.h>
#include <sipdialog.h>
#include <sipprofile.h>

#include "TCmdSendReferResponseCreateDialog.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, Content-Type*, Content-Encoding*
 *   Parameters:	StatusCode, Reason, Content*
 *   IDs:			ServerTransactionId, ProfileId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			NotifyId, DialogId
 */
void TCmdSendReferResponseCreateDialog::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPServerTransaction* serverTransaction = GetServerTransactionL();

	CSIPProfile* profile = GetProfileL();

	// Extract required headers
	CSIPEventHeader* eventHeader = ExtractEventHeaderLC();
	CSIPSubscriptionStateHeader* stateHeader = ExtractSubStateHeaderLC();

	// -- Execution -----------------------------------------------------------

	// Create an notify dialog association
	CSIPNotifyDialogAssoc* dialogAssoc = NULL;
	
	if ( profile )
	    {
	    dialogAssoc = CSIPNotifyDialogAssoc::NewL( 
	        *serverTransaction, *profile, eventHeader, stateHeader );
	    }
	else
        {
	    dialogAssoc = CSIPNotifyDialogAssoc::NewL( 
	        *serverTransaction, eventHeader, stateHeader );    
        }
													
	CleanupStack::Pop( 2 );	//eventHeader, stateHeader
	CleanupStack::PushL( dialogAssoc );

	// Use base class to do the hard work
	TCmdSendResponse::ExecuteL();

	// -- Response creation ---------------------------------------------------

	CleanupStack::Pop( dialogAssoc );
	AddIdResponseL( KNotifyDialogId, dialogAssoc );
	AddIdResponseL( KDialogId, dialogAssoc->Dialog() );
	}

TBool TCmdSendReferResponseCreateDialog::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendReferResponseCreateDialog") );
	}

TTcCommandBase* TCmdSendReferResponseCreateDialog::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendReferResponseCreateDialog( aContext );
	}

