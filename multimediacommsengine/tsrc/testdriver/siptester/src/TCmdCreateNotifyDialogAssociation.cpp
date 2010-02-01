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

#include <sipnotifydialogassoc.h>
#include <sipinvitedialogassoc.h>
#include <sipservertransaction.h>
#include <sipsubscriptionstateheader.h>
#include <sipeventheader.h>
#include <sipprofile.h>

#include "TCmdCreateNotifyDialogAssociation.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Event, Subscription-State
 *   Parameters:	-
 *   IDs:			ServerTransactionId, ProfileId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			NotifyDialogId
 */
void TCmdCreateNotifyDialogAssociation::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get SIP objects from registry
	CSIPServerTransaction* serverTransaction = GetServerTransactionL();
    CSIPProfile* profile = GetProfileL();

	// Extract required headers
	CSIPEventHeader* eventHeader = ExtractEventHeaderLC();
	CSIPSubscriptionStateHeader* stateHeader = ExtractSubStateHeaderLC();

	// -- Execution -----------------------------------------------------------

	// Create a notify dialog association
	CSIPNotifyDialogAssoc* dialogAssoc = NULL;
	
	if ( profile )
	    {
	    dialogAssoc = CSIPNotifyDialogAssoc::NewLC( 
	        *serverTransaction, *profile, eventHeader, stateHeader );
	    }
    else
        {
	    dialogAssoc = CSIPNotifyDialogAssoc::NewLC( 
	        *serverTransaction, eventHeader, stateHeader );        
        }

	// -- Response creation ---------------------------------------------------

	CleanupStack::Pop( 3 ); //eventHeader, stateHeader,dialogAssoc
	AddIdResponseL( KNotifyDialogId, dialogAssoc );
	}

TBool TCmdCreateNotifyDialogAssociation::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("CreateNotifyDialogAssociation") );
	}

TTcCommandBase* TCmdCreateNotifyDialogAssociation::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave )TCmdCreateNotifyDialogAssociation( aContext );
	}

