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
#include "TCmdGetReferState.h"
#include "CTcMCEContext.h"
#include <MCERefer.h>

void TCmdGetReferState::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get an item off the receive queue (waits until timeout if none is present)

	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, 
									EFalse );
									
	CTcMCEReceived& item = iContext.ReceivedEventItemL( timeout );	

	if ( !item.Refer() ) 
		{		
		User::Leave(KErrNotFound); // If first item doesn't contain a refer
		}

	CMceRefer* expectedRefer = reinterpret_cast<CMceRefer*>
 		(GetObjectForIdL(KReferId, ETrue));
	
	if( item.Refer() != expectedRefer )
		{
		// If the first item's containing refer is not the expected
		User::Leave(KErrNotFound); 
		}		

	// ---------- Execution ----------------------------------------------------
		
	CMceRefer::TState referState = item.ReferState();

	// ---------- Response creation --------------------------------------------
 
    // Add Refer
    AddIdResponseL( KReferId, *(item.Refer()) );

	// Add state
	AddIntegerResponseL( KResponseReferState, referState );
	AddTextualReferStateL( referState );    

	// Add StatusCode
	AddIntegerResponseL(KResponseStatusCode, item.StatusCode());

	// Add ReasonPhrase
	if (item.ReasonPhrase())
		{
		if (item.ReasonPhrase()->Length() > 0)
			{	
			AddTextResponseL(KResponseReasonPhrase, *(item.ReasonPhrase()));	
			}
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
	
	// Add Error
	if ( item.Error() != KErrNone )
		{
		AddIntegerResponseL( KResponseError, item.Error() );
		}	

	}
	
TBool TCmdGetReferState::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetReferState") );
	}

TTcCommandBase* TCmdGetReferState::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetReferState( aContext );
	}
