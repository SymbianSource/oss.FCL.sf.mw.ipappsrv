/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "CTcSIPHttpDigestContainer.h"
#include "TCmdIgnoreChallenge.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	DigestRealm
 *   IDs:			TransactionId*, RefreshId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdIgnoreChallenge::ExecuteL()
	{
	// Realm is mandatory
	TPtrC8 realm = ExtractTextL( KParamDigestRealm );
	CSIPClientTransaction* clientTransaction = GetClientTransactionL( EFalse );
	CSIPRefresh* refresh = GetRefreshL( EFalse );	
	
	if ( clientTransaction )
	    {
        iContext.DigestContainerL().HttpDigest().IgnoreChallenge( 
            *clientTransaction, realm );
	    }
	else
	    {
	    if ( refresh )
	        {
	        iContext.DigestContainerL().HttpDigest().IgnoreChallenge( 
	            *refresh, realm );
	        }
	    else
	        {
	        iContext.DigestContainerL().HttpDigest().IgnoreChallenge( realm );
	        }
	    }
	}

TBool TCmdIgnoreChallenge::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("IgnoreChallenge") );
	}

TTcCommandBase* TCmdIgnoreChallenge::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdIgnoreChallenge( aContext );
	}
