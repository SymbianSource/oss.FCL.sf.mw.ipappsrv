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
#include <sipconnection.h>
#include <sipfromheader.h>
#include <siprefresh.h>
#include <sipregistrationbinding.h>
#include <siprequestelements.h>
#include <siptoheader.h>
#include <sipstrings.h>
#include <stringpool.h>
#include <_sipcodecdefs.h>
#include <sipprofile.h>

#include "CTcSIPConnectionContainer.h"
#include "TCmdSendRequest.h"
#include "SIPConstants.h"

/**
 * INPUT:
 *   Headers:		To, Contact, From*, Content-Type*, Content-Encoding*,
 *					Expires*, Security-Client*
 *   Parameters:	Content*, RemoteURI*, Refresh*
 *   IDs:			ConnectionId*, RegistrationId*, RegistryId*, ProfileId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			TransactionId
 */
void TCmdSendRequest::ExecuteL()
	{
	// Select connection; either default or user specified (and existing)
	CSIPConnection& connection = SelectConnectionL().Connection();

	// Get SIP objects from registry
	CSIPRegistrationBinding* registration = GetRegistrationL( EFalse );
	
	 // Get selected registry
    CTcSIPProfileContainer& container = SelectProfileL();

	// Get profile from the profile registry, not mandatory
	CSIPProfile* profile = GetProfileL( container, EFalse );
	
	MSIPRegistrationContext* registrationContext = 0;
	
	TBool fromHeaderMandatory( ETrue );
	
	if ( profile )
	    {
	    fromHeaderMandatory = EFalse;
	    registrationContext = profile;
	    }
    
    // Defining registration binding will override possibly defined profile	    
	if( registration )
		{
		fromHeaderMandatory = EFalse;
		registrationContext = registration;
		}		  
	
	// Extract remote URI
	CUri8* uri = ExtractRemoteURILC();

	// Create SIP request elements, giving toHeader away
	CSIPRequestElements* reqElements = CSIPRequestElements::NewL( uri );
	CleanupStack::Pop( uri );
	CleanupStack::PushL( reqElements );
	
	// Extract To header (may be either mandatory or optional)
	CSIPToHeader* toHeader = ExtractToHeaderLC();
	if( toHeader )
	    {
	    reqElements->SetToHeaderL( toHeader );
	    CleanupStack::Pop( toHeader );
	    }

	// Set request method
	TPtrC8 methodPtr = ExtractTextL( KParamMethod );
	RStringF methodStr = SIPStrings::Pool().OpenFStringL(methodPtr);
	CleanupClosePushL(methodStr);	
    reqElements->SetMethodL( methodStr );
	
	// Extract From header (may be either mandatory or optional)
	CSIPFromHeader* fromHeader = ExtractFromHeaderLC( fromHeaderMandatory );
	if( fromHeader )
		{
		reqElements->SetFromHeaderL( fromHeader );
		CleanupStack::Pop( fromHeader );
		}

	// Conditionally create refresh object
	CSIPRefresh* refresh = ExtractRefreshLC();

	// Extract both headers (that are still left) and content.
	ExtractHeadersAndContentL( reqElements->MessageElements() );
 		
	// -- Execution -----------------------------------------------------------

	// Start SIP Request transaction.
	CSIPClientTransaction* transaction;
	if( registrationContext )
		{
		if( refresh )
			{
			transaction = connection.SendRequestL( reqElements, *registrationContext,
												   *refresh );
			}
		else
			{
			transaction = connection.SendRequestL( reqElements, *registrationContext );
			}
		}
	else
		{
		if( refresh )
			{
			transaction = connection.SendRequestL( reqElements, *refresh );
			}
		else
			{
			transaction = connection.SendRequestL( reqElements );
			}
		}

	if( refresh )
		{
		CleanupStack::Pop( refresh );
		}
	CleanupStack::PopAndDestroy(); // methodStr
	CleanupStack::Pop( reqElements );

	// -- Response creation ---------------------------------------------------

	AddIdResponseL( KTransactionId, transaction );
	}

TBool TCmdSendRequest::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SendRequest") );
	}

TTcCommandBase* TCmdSendRequest::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRequest( aContext );
	}

