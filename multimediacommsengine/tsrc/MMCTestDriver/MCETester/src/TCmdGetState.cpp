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
#include "TCmdGetState.h"
#include "CTcMCEContext.h"
#include <MCESession.h>
#include "CTcMCEReceived.h"


void TCmdGetState::ExecuteL()
	{

	// -- Execution ------
	
	// Get an item off the receive queue (waits until timeout if none is present)

	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, 
									EFalse );
	CTcMCEReceived& item = iContext.ReceivedSessionItemL( timeout );	
	
	const CMceSession* session = NULL;
	
	if ( item.Session() )
		{
		session = item.Session();
		}
	
	if ( !session ) 
		{	
		// If first item doesn't contain a session
		User::Leave(KErrNotFound); 	
		}	

 	CMceSession* expectedSession = reinterpret_cast<CMceSession*>
 		(GetObjectForIdL(KSessionId, ETrue));
	
	/*if ( session != expectedSession && session != expectedSession )
		{
		// If the first item's containing session is not the expected
		User::Leave(KErrNotFound); 
		}
	*/		
	CMceSession::TState state = item.Session()->State();
	//SessionState();
	
	TBool connectionActive = session->ConnectionActive();
	
	// -- Response creation ----------------------------------------------------	

	// Add Session
	AddIdResponseL( KSessionId, *session );
	
	// Add state
	AddIntegerResponseL(KResponseState, state);
	AddTextualSessionStateL(state);
	
	// Add connection state
	AddBooleanResponseL(KResponseConnectionActive, connectionActive);	
	
	// Add Headers
	AddHeadersL( item.Headers() ); 
	
	// Add Body
	if (item.Body())
		{
		if (item.Body()->Length() > 0)
			{	
			AddTextResponseL(KResponseBody, *(item.Body()));	
			}
		}
	
	// Add StatusCode
	AddIntegerResponseL(KResponseStatusCode, item.StatusCode());

	// Add ReasonPhrase
	if (item.ReasonPhrase())
		{
		if (item.ReasonPhrase()->Length() > 0)
			{	
			AddTextResponseL(KResponseReasonPhrase, 
				 			 *(item.ReasonPhrase()));	
			}
		}
	
	// Add Error
	if ( item.Error() != KErrNone )
		{
		AddIntegerResponseL( KResponseError, item.Error() );
		}	
	}

TBool TCmdGetState::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetState") );
	}

TTcCommandBase* TCmdGetState::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetState( aContext );
	}
