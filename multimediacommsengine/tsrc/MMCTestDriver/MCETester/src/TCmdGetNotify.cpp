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
#include "TCmdGetNotify.h"
#include "CTcMCEContext.h"
#include <MCEEvent.h>

void TCmdGetNotify::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get an item off the receive queue (waits until timeout if none is present)
	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, EFalse );
	CTcMCEReceived& item = iContext.ReceivedEventItemL( timeout );
	
	if ( !item.Event() ) 
		{		
		User::Leave(KErrNotFound); // If first item doesn't contain an event
		}
		
 	CMceEvent* expectedEvent = reinterpret_cast<CMceEvent*>
 		(GetObjectForIdL(KEventId, ETrue));
	
	if(item.Event() != expectedEvent)
		{
		// If the first item's containing event is not the expected
		User::Leave(KErrNotFound); 
		}		

	// ---------- Execution ----------------------------------------------------
	
	
		
	// ---------- Response creation --------------------------------------------
 
 	// Add Event
 	AddIdResponseL( KEventId, *(item.Event()) );
		
	// Add Headers
	AddHeadersL( item.Headers() );

	// Add ContentType
	if (item.ContentType())
		{
		if (item.ContentType()->Length() > 0)
			{
			AddTextResponseL(KResponseContentType, *(item.ContentType()));
			}
			
		}
		
	// Add Body
	if (item.Body())
		{
		if (item.Body()->Length() > 0)
			{	
			AddTextResponseL(KResponseBody, *(item.Body()));	
			}
		}		

	}

	
TBool TCmdGetNotify::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetNotify") );
	}

TTcCommandBase* TCmdGetNotify::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetNotify( aContext );
	}
