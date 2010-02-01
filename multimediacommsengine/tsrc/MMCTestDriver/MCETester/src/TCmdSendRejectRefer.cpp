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
* Description:    Implementation
*
*/



#include "MCEConstants.h"
#include "TCmdSendRejectRefer.h"
#include "CTcMCEContext.h"

#include <MceInRefer.h>

void TCmdSendRejectRefer::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get refer
	CMceInRefer* refer = reinterpret_cast<CMceInRefer*>
		(GetObjectForIdL(KReferId, ETrue));	

	// ---------- Execution ----------------------------------------------------

	// Get reasonphrase
	TPtrC8 reason = ExtractTextL(KParamReasonPhrase, EFalse);
		
	// Get statuscode
	TInt status = ExtractIntegerL(KParamStatusCode, 0, EFalse);
	
	// Get Headers
	CDesC8Array* headers = ExtractHeadersL( EFalse );
	CleanupStack::PushL(headers);

	// Reject
	if( reason == KNullDesC8 && status == 0 && !headers )  
		{
		CleanupStack::PopAndDestroy(headers);
		refer->RejectL();
		}
	else
		{
		refer->RespondL( reason, status, headers );
		CleanupStack::Pop(headers);
		}

	CMceRefer::TState state = refer->State();		

	// ---------- Response creation --------------------------------------------
 
 	AddIdResponseL( KReferId, *refer );

	// Add refer state	
	AddIntegerResponseL( KResponseReferState, state );
	AddTextualReferStateL( state );

	}
	
TBool TCmdSendRejectRefer::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendRejectRefer") );
	}

TTcCommandBase* TCmdSendRejectRefer::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRejectRefer( aContext );
	}
