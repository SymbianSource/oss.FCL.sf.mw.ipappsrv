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
#include "TCmdSetCredentials.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	DigestRealm, DigestUserName, DigestPassword, Proxy*
 *   IDs:			TransactionId*, RefreshId* 
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSetCredentials::ExecuteL()
	{
	// Extract mandatory params
	TPtrC8 realm = ExtractTextL( KParamDigestRealm );
	TPtrC8 username = ExtractTextL( KParamDigestUsername );
	TPtrC8 password = ExtractTextL( KParamDigestPassword );
	
	// Extract optional params
	CSIPClientTransaction* clientTransaction = GetClientTransactionL( EFalse );
	CSIPRefresh* refresh = GetRefreshL( EFalse );
	TPtrC8 proxy = ExtractTextL( KParamProxy, EFalse );
	
	if ( clientTransaction )
	    {
        iContext.DigestContainerL().HttpDigest().SetCredentialsL( 
            *clientTransaction, proxy, realm, username, password );	    
	    }
	else
	    {
	    if ( refresh )
	        {
            iContext.DigestContainerL().HttpDigest().SetCredentialsL( 
                *refresh, proxy, realm, username, password );
	        }
	    else
	        {
	        if( proxy != KNullDesC8 )
	            {
	            iContext.DigestContainerL().HttpDigest().SetCredentialsL( 
	                proxy, realm, username, password );
	            }
	        else
	            {
	            iContext.DigestContainerL().HttpDigest().SetCredentialsL( 
	                realm, username, password );
	            }
	        }
	    }
	

	}

TBool TCmdSetCredentials::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SetCredentials") );
	}

TTcCommandBase* TCmdSetCredentials::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetCredentials( aContext );
	}
