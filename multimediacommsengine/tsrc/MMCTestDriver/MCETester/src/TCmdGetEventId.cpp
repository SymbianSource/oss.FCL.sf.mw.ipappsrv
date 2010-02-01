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
#include "TCmdGetEventId.h"
#include "CTcMCEContext.h"

#include <MCEEvent.h>
#include <MceInEvent.h>

void TCmdGetEventId::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------



	// ---------- Execution ----------------------------------------------------

	// Get an item off the receive queue (waits until timeout if none is present)

	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, 
									EFalse );
									
	CTcMCEReceived& item = iContext.ReceivedEventItemL( timeout );	
	
	if ( !item.InEvent() ) 
		{		
		User::Leave(KErrNotFound); // If first item doesn't contain an event
		}
		
	CMceEvent::TState eventState = item.InEvent()->State();
	// ---------- Response creation --------------------------------------------
 
 	// Add Event
 	AddIdResponseL( KEventId, *item.InEvent() );
 	
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
	
	// Add event state	
	AddIntegerResponseL( KResponseEventState, eventState );
	AddTextualEventStateL( eventState );
		
	}
	
TBool TCmdGetEventId::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetEventId") );
	}

TTcCommandBase* TCmdGetEventId::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetEventId( aContext );
	}
