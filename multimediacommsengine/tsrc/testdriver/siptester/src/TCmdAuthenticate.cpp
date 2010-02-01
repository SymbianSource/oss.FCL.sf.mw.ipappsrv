/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TCmdAuthenticate.h"
#include "SIPConstants.h"
#include "CTcSIPIMSAuthenticator.h"
#include "CTcSIPContext.h"

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	AuthenticationNonce
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	AuthenticationRes, AuthenticationIk, AuthenticationCk, 
 *                  AuthenticationAuts
 *   IDs:			-
 */

void TCmdAuthenticate::ExecuteL()
	{
	HBufC8* nonce = ExtractHBufLC( KParamAuthenticationNonce );
	__ASSERT_ALWAYS( nonce, User::Leave( KErrNotFound ) );
			
	CTcSIPIMSAuthenticationParams& params = 
	    iContext.AuthenticateL( *nonce );
	
    AddTextResponseL( KParamAuthenticationRes, params.Res() );
    AddTextResponseL( KParamAuthenticationIk, params.Ik() );
    AddTextResponseL( KParamAuthenticationCk, params.Ck() );
    AddTextResponseL( KParamAuthenticationAuts, params.Auts() );
    
    CleanupStack::PopAndDestroy( nonce );
	}

TBool TCmdAuthenticate::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("Authenticate") );
	}

TTcCommandBase* TCmdAuthenticate::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdAuthenticate( aContext );
	}
