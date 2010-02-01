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
#include "CTcSIPHttpDigestContainer.h"
#include "SIPConstants.h"
#include "TCmdGetChallenge.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	Timeout*
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	DigestRealm
 *   IDs:			-
 */
void TCmdGetChallenge::ExecuteL()
	{
	// Get an item off the receive queue (waits until timeout if none is present)
	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, EFalse );	
	HBufC8* realm = iContext.DigestContainerL().ReceivedChallengeL( timeout );
	if( realm )
	    {
	    CleanupStack::PushL( realm );
	    AddTextResponseL( KParamDigestRealm, *realm ); //makes a copy of realm
	    CleanupStack::PopAndDestroy(); // realm
	    }
	
	}

TBool TCmdGetChallenge::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("GetChallenge") );
	}

TTcCommandBase* TCmdGetChallenge::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetChallenge( aContext );
	}
