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
#include "TCmdSendRefer.h"
#include "CTcMCEContext.h"

#include <MceOutRefer.h>
#include <MceOutRefer.h>
#include <MCEEvent.h>

void TCmdSendRefer::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get refer
	CMceOutRefer* refer = 
	    reinterpret_cast<CMceOutRefer*>(GetObjectForIdL(KReferId, ETrue));			
	
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

	CMceOutEvent* outEventRefer = NULL;
	
	// ---------- Execution ----------------------------------------------------

    outEventRefer = refer->ReferL( headers, contentType, body );
    
	TPtrC8 outEventName;
	if (outEventRefer)
		{
		iContext.Registry().AddObjectL(	outEventRefer );
		outEventName.Set(iContext.Registry().ObjectNameL( outEventRefer));	
		}
    CleanupStack::Pop( body );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( headers );

	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KReferId, *refer );
  
    if(outEventRefer)
    	{
    	AddTextResponseL(KParamNotifyReceiverEventId, outEventName);		
    	}
    }
	
TBool TCmdSendRefer::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendRefer") );
	}

TTcCommandBase* TCmdSendRefer::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRefer( aContext );
	}
