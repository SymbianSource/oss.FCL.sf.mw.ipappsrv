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




#include <MceInSession.h>
#include <MceMediaStream.h>
#include <MCEAudioStream.h>
#include <MCEAudioCodec.h>

#include "MCEConstants.h"
#include "TCmdGetSessionId.h"
#include "CTcMCEContext.h"
#include "CTcMCEReceived.h"

void TCmdGetSessionId::ExecuteL()
	{
	
	// -- Setup ---------
	
	// Connection selection
	
	// -- Execution ------
	
	// Get an item off the receive queue (waits until timeout if none is present)

	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, 
									EFalse );
	TBool update = ExtractBooleanL( KParamSessionUpdate, EFalse );	
		
	CTcMCEReceived* item;
								
	if (update)
	{
		item = &(iContext.ReceivedUpdatedSessionItemL( timeout ));
	}
	else 
	{
		item = &(iContext.ReceivedSessionItemL( timeout ));	
	}
	
		
	if ( !item->InSession() ) 
		{		
		if ( item->Error() != KErrNone ) 
			{
			AddIdResponseL( KSessionId, item->Session() );
			AddIntegerResponseL( KResponseError, item->Error() );
			}
		else 
			{
			User::Leave(KErrNotFound);
			}
		}

		
	CMceSession::TState state = item->InSession()->State();		
	CTcStructure* structure;
	do
		{
		structure = FindStructureL( KParamSessionModifier, EFalse );
		if( structure )
			{
            //structure contains a list of name-value pairs
			for (int i=0; i < structure->Count(); i++)
				{
				CTcNameValue* itemVal = structure->ItemL( i );
				//sessionMod->SetModifierL( itemVal->NameAsIntL(), itemVal->ValueAsIntL() );
				item->InSession()->SetModifierL( itemVal->NameAsIntL(), itemVal->ValueAsIntL() );
				
				//checking
				TMceSessionModifier nameInt = itemVal->NameAsIntL();
				TUint valueInt = itemVal->ValueAsIntL();
				item->InSession()->GetModifierL( nameInt, valueInt );
			
				//sending response output
				
				AddIntegerResponseL( KParamSessionModifier, valueInt );
			
				}
			iContext.List().RemoveParameter( KParamSessionModifier );
		
			}
	} while( structure );
	// -- Response creation --------	

	// Add Session	
	AddIdResponseL( KSessionId, *(item->InSession()) );
	
	// Add Headers
	AddHeadersL( item->Headers() );
	
	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );
	
	// Add Originator and Recipient
	AddTextResponseL( KResponseOriginator, item->InSession()->Originator() );
	AddTextResponseL( KResponseRecipient, item->InSession()->Recipient() );
		
	// Add Body
	if (item->Body())
		{
		if (item->Body()->Length() > 0)
			{	
			AddTextResponseL(KResponseBody, *(item->Body()));	
			}
		}
		
	// Add ContentType
	if (item->ContentType())
		{
		if (item->ContentType()->Length() > 0)
			{
			AddTextResponseL(KResponseContentType, *(item->ContentType()));
			}
			
		}


	// Find and add streams
	
	CDesC8Array* streams = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( streams );

	const RPointerArray<CMceMediaStream>& streamPtrs = 
				item->InSession()->Streams();

	for (TInt streamIndex=0; streamIndex < streamPtrs.Count(); streamIndex++)
		{	
		// Add to registry by reference and fetch name
		iContext.Registry().AddObjectL( *(streamPtrs[streamIndex]) );	
		TBuf8< KTcMaxObjectName > streamId = 
					iContext.Registry().ObjectNameL( streamPtrs[streamIndex] );	
		streams->AppendL( streamId );
		}

	if ( streams->Count() > 0 )
		{
		AddArrayResponseL( KResponseStreams, *streams );
		}
		
	CleanupStack::PopAndDestroy( streams );	
	
	}

TBool TCmdGetSessionId::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetSessionId") );
	}

TTcCommandBase* TCmdGetSessionId::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetSessionId( aContext );
	}
	
