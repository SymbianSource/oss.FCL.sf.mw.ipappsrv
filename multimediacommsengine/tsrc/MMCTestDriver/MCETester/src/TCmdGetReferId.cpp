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
#include "TCmdGetReferId.h"
#include "CTcMCEContext.h"

#include <MCERefer.h>
#include <MceInRefer.h>

void TCmdGetReferId::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------


	// ---------- Execution ----------------------------------------------------

	// Get an item off the receive queue (waits until timeout if none is present)

	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, 
									EFalse );
									
	CTcMCEReceived& item = iContext.ReceivedEventItemL( timeout );	
	
	if ( !item.InRefer() ) 
		{		
		User::Leave(KErrNotFound); // If first item doesn't contain an refer
		}

    const CMceInRefer& inRefer( *item.InRefer() );

	// ---------- Response creation --------------------------------------------
 
    // Add the refer ID
    AddIdResponseL( KReferId, inRefer );
 
    // Add the session ID of the related session if it exists
    if ( inRefer.AssociatedSession() )
        {
        AddIdResponseL( KSessionId, *( inRefer.AssociatedSession() ) );
        }      
 
    // Add referTo
    AddTextResponseL( KResponseReferTo, inRefer.ReferTo() );

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

	// Add refer state	
	AddIntegerResponseL( KResponseReferState, inRefer.State() );
	AddTextualReferStateL( inRefer.State() );	
	}
	
TBool TCmdGetReferId::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetReferId") );
	}

TTcCommandBase* TCmdGetReferId::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetReferId( aContext );
	}
