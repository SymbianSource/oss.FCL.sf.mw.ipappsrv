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
#include <sipinvitedialogassoc.h>
#include <sipmessageelements.h>
#include <sipprofile.h>

#include "CTcSIPProfileContainer.h"
#include "CTcSIPConnectionContainer.h"
#include "CTcSIPContext.h"

#include "TCmdSendInviteUsingProfile.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		To*, From*, Contact*
 *   Parameters:	RemoteURI
 *   IDs:			ProfileId, RegistryId
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId, InviteDialogId
 */
void TCmdSendInviteUsingProfile::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Extract required headers (From may be either mandatory or optional)
	CSIPToHeader* toHeader = ExtractToHeaderLC( EFalse );
	CSIPFromHeader* fromHeader = ExtractFromHeaderLC( EFalse );
    CSIPContactHeader* contactHeader = ExtractContactHeaderLC( EFalse );

	// Extract remote URI
	CUri8* uri = ExtractRemoteURILC( ETrue );

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

    CSIPInviteDialogAssoc* inviteDialogAssoc = 
        CSIPInviteDialogAssoc::NewL( *sipConnection,
                                     uri,
                                     profile,
                                     fromHeader,
                                     toHeader,
                                     contactHeader );
                                                                    
    CleanupStack::Pop( iPushed ); // toHeader; fromheader, uri
    iPushed = 0;

    CleanupStack::PushL( inviteDialogAssoc );
    
    // Extract rest of headers (if provided)
    CSIPMessageElements* elements = ExtractHeadersAndContentLC();
    
     // -- Execution -----------------------------------------------------------
     
    // Start SIP Invite transaction.
	CSIPClientTransaction* transaction = inviteDialogAssoc->SendInviteL( elements );
    CleanupStack::Pop( elements );

	// -- Response creation ---------------------------------------------------

    AddIdResponseL( KTransactionId, transaction );
	AddIdResponseL( KInviteDialogId, inviteDialogAssoc );

    CleanupStack::Pop( inviteDialogAssoc );
	}

TBool TCmdSendInviteUsingProfile::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendInviteUsingProfile") );
	}

TTcCommandBase* TCmdSendInviteUsingProfile::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendInviteUsingProfile( aContext );
	}

