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
#include "TCmdGetFCMessage.h"
#include "CTcMCEContext.h"
#include "CTcMCEReceived.h"
#include <MCESession.h>


void TCmdGetFCMessage::ExecuteL()
	{
	
	// -- Setup ---------
	
	// Get an item off the receive queue (waits until timeout if none is present)

	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, 
									EFalse );
	CTcMCEReceived& item = iContext.ReceivedFCItemL( timeout );	
			
	if ( !item.Session() ) 
		{		
		User::Leave(KErrNotFound); // If first item doesn't contain a session
		}	

 	CMceSession* expectedSession = reinterpret_cast<CMceSession*>
 		(GetObjectForIdL(KSessionId, ETrue));
	
	if(item.Session() != expectedSession)
		{
		// If the first item's containing session is not the expected
		User::Leave(KErrNotFound); 
		}
			
	// -- Execution ------
		
	// -- Response creation ----------------------------------------------------	

	// Add Session
	AddIdResponseL( KSessionId, *(item.Session()) );
	
	// Add FCMessage
	if (item.FCMessage())
		{
		if (item.FCMessage()->Length() > 0)
			{	
			AddTextResponseL(KResponseFCMessage, *(item.FCMessage()));
			}
		}	
		
	// Add Error
	if ( item.Error() != KErrNone )
		{
		AddIntegerResponseL( KResponseError, item.Error() );
		}
		
	}

TBool TCmdGetFCMessage::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetFCMessage") );
	}

TTcCommandBase* TCmdGetFCMessage::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetFCMessage( aContext );
	}
