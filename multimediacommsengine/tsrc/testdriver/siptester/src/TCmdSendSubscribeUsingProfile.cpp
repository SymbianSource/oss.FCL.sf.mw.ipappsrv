/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include <siprefresh.h>
#include <sipmessageelements.h>
#include <sipsubscribedialogassoc.h>
#include <sipprofile.h>

#include "CTcSIPProfileContainer.h"
#include "CTcSIPConnectionContainer.h"
#include "CTcSIPContext.h"

#include "TCmdSendSubscribeUsingProfile.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		Contact*, From*, To*, Event*, Content-Type*
 *   Parameters:	RemoteUri*, Refresh*
 *   IDs:			ProfileId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, SubscribeDialogId
 */
void TCmdSendSubscribeUsingProfile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Extract required headers
	CSIPEventHeader* eventHeader = ExtractEventHeaderLC(true);
	// Extract remote URI
	CUri8* uri = ExtractRemoteURILC( true );
	
	//  Extract optional headers
	CSIPToHeader* toHeader = ExtractToHeaderLC( EFalse );
	CSIPFromHeader* fromHeader = ExtractFromHeaderLC( EFalse );
    CSIPContactHeader* contactHeader = ExtractContactHeaderLC( EFalse );

	// Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Get profile from the profile registry
	CSIPProfile& profile = GetProfileL( container );
	
	CSIPConnection* sipConnection = container.ProfileRegistry().ConnectionL( profile );
	// Ownership is not really transferred if SIPTester has opened already 
	// CSIPConnection for same IAP as profile is using!!
	if ( sipConnection != &(iContext.Connection().Connection()) )
		{
		// Have to store because needs longer lifetime than just current ExecuteL
		iContext.Registry().AddObjectL( sipConnection );	
		}		

	// Create a new dialog association
	CSIPSubscribeDialogAssoc* newdialogAssoc =
						CSIPSubscribeDialogAssoc::NewL(*sipConnection,
														uri,
														profile,
														eventHeader,
														fromHeader,
														toHeader,
														contactHeader );


	// Purge items from cleanup stack, now they are owned by CSIPInviteDialogAssoc
	// Some items are optional (i.e. NULL), so we've been tracking the number
	// of items in pushed to CleanupStack
	CleanupStack::Pop( iPushed );
	iPushed = 0;
	
	CleanupStack::PushL( newdialogAssoc );

	// Conditionally create refresh object
	CSIPRefresh* refresh = ExtractRefreshLC();

	// Extract both headers (that are still left) and content.
	CSIPMessageElements* elements = ExtractHeadersAndContentLC();

	// -- Execution -----------------------------------------------------------

	// Start SIP Subscribe transaction.
	CSIPClientTransaction* transaction =
							newdialogAssoc->SendSubscribeL( elements, refresh );
	CleanupStack::Pop( elements );
	if( refresh )
		{
		CleanupStack::Pop( refresh );
		}

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	CleanupStack::Pop( newdialogAssoc );
	AddIdResponseL( KSubscribeDialogId, newdialogAssoc );
	}

TBool TCmdSendSubscribeUsingProfile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendSubscribeUsingProfile") );
	}

TTcCommandBase* TCmdSendSubscribeUsingProfile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendSubscribeUsingProfile( aContext );
	}

