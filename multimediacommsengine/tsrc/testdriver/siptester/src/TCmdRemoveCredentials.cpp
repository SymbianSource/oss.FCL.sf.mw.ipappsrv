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
#include "CTcSIPHttpDigestContainer.h"
#include "TCmdRemoveCredentials.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	DigestRealm*
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdRemoveCredentials::ExecuteL()
	{
    // Realm is optional
	TPtrC8 realm = ExtractTextL( KParamDigestRealm, EFalse );
	
	if( realm != KNullDesC8 )
	    { 
	    iContext.DigestContainerL().HttpDigest().RemoveCredentials( realm );
	    }
	else
	    {
	    iContext.DigestContainerL().HttpDigest().RemoveCredentials();
	    }
	}

TBool TCmdRemoveCredentials::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("RemoveCredentials") );
	}

TTcCommandBase* TCmdRemoveCredentials::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdRemoveCredentials( aContext );
	}
