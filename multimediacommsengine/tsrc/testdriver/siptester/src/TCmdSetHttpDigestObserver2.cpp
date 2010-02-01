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
#include "TCmdSetHttpDigestObserver2.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSetHttpDigestObserver2::ExecuteL()
	{
	CTcSIPHttpDigestContainer& digestContainer = iContext.DigestContainerL();
	
	digestContainer.HttpDigest().SetObserver( 
	    static_cast< MSIPHttpDigestChallengeObserver2& > ( digestContainer ) );
	}

TBool TCmdSetHttpDigestObserver2::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SetHttpDigestObserver2") );
	}

TTcCommandBase* TCmdSetHttpDigestObserver2::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetHttpDigestObserver2( aContext );
	}
