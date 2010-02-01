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
#include "TCmdSendTerminateEvent.h"
#include "CTcMCEContext.h"
#include "MCEEvent.h"

void TCmdSendTerminateEvent::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	// Get event
	CMceEvent* event = 
		reinterpret_cast<CMceEvent*>(GetObjectForIdL(KEventId, ETrue));	
	
	// ---------- Execution ----------------------------------------------------
	
	// Get Headers
	CDesC8Array* headers = ExtractHeadersL( EFalse );
	if (!headers)
	{
		headers = new (ELeave) CDesC8ArrayFlat(1);	
	}
	//headers->AppendL(_L8("event: ttcn"));
	CleanupStack::PushL(headers);
	
	// Get ContentType
	HBufC8* contentType = HBufCParameterL(ExtractTextL(KParamContentType, 
													   EFalse));		
	CleanupStack::PushL(contentType);
	
	// Get Body
	HBufC8* body = HBufCParameterL(ExtractTextL(KParamBody, EFalse));
	CleanupStack::PushL(body);
	
	event->TerminateL(headers, contentType, body);
	
	CMceEvent::TState eventState = event->State();
	
	// ---------- Response creation --------------------------------------------
 
	AddIdResponseL(KEventId, *event);
 
	AddIntegerResponseL( KResponseEventState, eventState );
	AddTextualEventStateL( eventState );
	
	CleanupStack::Pop( body );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( headers );

	}
	
TBool TCmdSendTerminateEvent::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendTerminateEvent") );
	}

TTcCommandBase* TCmdSendTerminateEvent::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendTerminateEvent( aContext );
	}
