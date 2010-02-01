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
#include "TCmdSendSubscribe.h"
#include "CTcMCEContext.h"
#include <MceOutRefer.h>


void TCmdSendSubscribe::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get event
	CMceOutEvent* event = reinterpret_cast<CMceOutEvent*>
		(GetObjectForIdL(KEventId, ETrue));			

	// Get Headers
	CDesC8Array* headers = ExtractHeadersL( EFalse );
	CleanupStack::PushL(headers);
		
	// Get ContentType
	HBufC8* contentType = HBufCParameterL(ExtractTextL(KParamContentType, 
													   EFalse));
	CleanupStack::PushL(contentType);
		
	// Get Body
	HBufC8* body = HBufCParameterL(ExtractTextL(KParamBody, EFalse));
	CleanupStack::PushL(body);
		
	// ---------- Execution ----------------------------------------------------

	event->SubscribeL( headers, contentType, body);

	CleanupStack::Pop(body);
	CleanupStack::Pop(contentType);
	CleanupStack::Pop(headers);

	// ---------- Response creation --------------------------------------------
 
 	AddIdResponseL(KEventId, *event);
 	
	}
	
TBool TCmdSendSubscribe::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendSubscribe") );
	}

TTcCommandBase* TCmdSendSubscribe::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendSubscribe( aContext );
	}
