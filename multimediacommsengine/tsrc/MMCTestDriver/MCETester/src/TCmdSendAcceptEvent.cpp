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
#include "TCmdSendAcceptEvent.h"
#include "CTcMCEContext.h"
#include <MceInEvent.h>

void TCmdSendAcceptEvent::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get event
	CMceInEvent* event = reinterpret_cast<CMceInEvent*>
		(GetObjectForIdL(KEventId, ETrue));	

	// ---------- Execution ----------------------------------------------------

	// Get reasonphrase
	TPtrC8 reason = ExtractTextL(KParamReasonPhrase, EFalse);
		
	// Get statuscode
	TInt status = ExtractIntegerL(KParamStatusCode, 0, EFalse);
	
	// Get Headers
	CDesC8Array* headers = ExtractHeadersL( EFalse );
	CleanupStack::PushL(headers);

	// Accept
	if( reason == KNullDesC8 && status == 0 && !headers )  
		{
		CleanupStack::PopAndDestroy(headers);
		event->AcceptL();
		}
	else
		{
		event->RespondL( reason, status, headers );
		CleanupStack::Pop(headers);
		}

	CMceEvent::TState state = event->State();		
	
	// ---------- Response creation --------------------------------------------
 
 	AddIdResponseL( KEventId, *event );

	// Add Event state	
	AddIntegerResponseL( KResponseEventState, state );
	AddTextualEventStateL( state );
	
	}
	
TBool TCmdSendAcceptEvent::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendAcceptEvent") );
	}

TTcCommandBase* TCmdSendAcceptEvent::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendAcceptEvent( aContext );
	}
