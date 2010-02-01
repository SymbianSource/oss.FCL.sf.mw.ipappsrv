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
#include "TCmdGetMessage.h"
#include "CTcMCEContext.h"
#include <MCESession.h>
#include <MCEEvent.h>


void TCmdGetMessage::ExecuteL()
	{
	
	// -- Setup ---------
	

	// -- Execution ------
	
	// Get an item off the receive queue (waits until timeout if none is present)
	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, EFalse );
	CTcMCEReceived& item = iContext.ReceivedSessionItemL( timeout );
		
	// -- Response creation --------	
	
	// Add Session
	if ( item.Session() )
		{
		AddIdResponseL( KSessionId, *(item.Session()) );
		}

	if ( item.InSession() )
		{
		AddIdResponseL( KSessionId, *(item.Session()) );
		}

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
		
	// Add StatusCode
	if ( item.StatusCode() != 0 )
		{
		AddIntegerResponseL(KResponseStatusCode, item.StatusCode());		
		}
		
	// Add ReasonPhrase				
	if (item.ReasonPhrase())
		{
		if (item.ReasonPhrase()->Length() > 0)
			{	
			AddTextResponseL(KResponseReasonPhrase, 
				 			 *(item.ReasonPhrase()));	
			}
		}

	// Add FCMessage
	if (item.FCMessage())
		{
		if (item.FCMessage()->Length() > 0)
			{	
			AddTextResponseL(KResponseFCMessage, *(item.FCMessage()));	
			}
		}		
	
	// Add event Id
	if ( item.Event() )
		{
		AddIdResponseL( KEventId, item.Event() );
		}

	if ( item.InEvent() )
		{
		AddIdResponseL( KEventId, item.Event() );		
		}

	// Add ReferTo-header
	if ( item.ReferTo() != KNullDesC8 )
		{
		AddTextResponseL( KResponseReferTo, item.ReferTo() );
		}
		
	// Add Error
	if ( item.Error() != KErrNone )
		{
		AddIntegerResponseL( KResponseError, item.Error() );
		}		

	// Add Request method
	if ( item.Method() )
		{
		AddTextResponseL( KResponseMethod, *item.Method() );
		}

	// Add transaction id
	if ( item.TransactionId() )
		{
		AddIntegerResponseL( KResponseTransactionId, item.TransactionId() );
		}
	}

TBool TCmdGetMessage::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetMessage") );
	}

TTcCommandBase* TCmdGetMessage::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetMessage( aContext );
	}
