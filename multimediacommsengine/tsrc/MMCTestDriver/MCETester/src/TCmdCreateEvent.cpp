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
#include "TCmdCreateEvent.h"
#include "CTcMCEContext.h"
#include <MceOutEvent.h>


void TCmdCreateEvent::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	
	// Get profile
	CSIPProfile* profile = 
		reinterpret_cast<CSIPProfile*>(GetObjectForIdL(KProfileObj, EFalse));
		
	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, EFalse));	
		
	// Get event
	CMceEvent* event = 
	    reinterpret_cast<CMceEvent*>(GetObjectForIdL(KEventId, EFalse));
	    
	// Get refer
	CMceRefer* refer = 
	    reinterpret_cast<CMceRefer*>(GetObjectForIdL(KReferId, EFalse));	
				
	// Get EventHeader
	TPtrC8 eventHeader = ExtractTextL(KParamEventHeader, ETrue);
	
	// Get refresh interval
	TInt refresh = ExtractIntegerL( KParamRefreshInterval, 0, ETrue );
	
			
	// ---------- Execution ----------------------------------------------------

	CMceOutEvent* outEvent = NULL;

	if ( profile ) 
		{
		
		TPtrC8 recipient = ExtractTextL( KParamRecipient, ETrue );
		
        HBufC8* originator = HBufCParameterL(ExtractTextL( KParamOriginator, 
                                                           EFalse));
	    CleanupStack::PushL( originator );
			
		outEvent = CMceOutEvent::NewL( iContext.MCEManager(), 
		                               *profile,
		                               recipient,
		                               eventHeader,
		                               refresh,
		                               originator );
		                               
		CleanupStack::Pop( originator );
		}
	else if ( session ) 
		{
		outEvent = CMceOutEvent::NewL( *session,
		                               eventHeader,
		                               refresh );
		}
	else if ( event ) 
		{
		outEvent = CMceOutEvent::NewL( *event,
		                               eventHeader,
		                               refresh );
		}
	else if ( refer )
	    {
		outEvent = CMceOutEvent::NewL( *refer,
		                               eventHeader,
		                               refresh );	    
	    }
    else
    	{
		User::Leave( KErrNotFound );
		}
		
	// ---------- Response creation --------------------------------------------

	// Must be added by pointer, ownership is transferred to object registry.
 	AddIdResponseL(KEventId, outEvent);


	}
	
TBool TCmdCreateEvent::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("CreateEvent") );
	}

TTcCommandBase* TCmdCreateEvent::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdCreateEvent( aContext );
	}
