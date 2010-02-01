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
#include "TCmdEstablish.h"
#include "CTcMCEContext.h"

#include <MCEMicSource.h>
#include <MceRtpSink.h>
#include <MCEAudioStream.h>
#include <MceRtpSource.h>
#include <MCESpeakerSink.h>
#include <MceOutSession.h>
#include <MCEManager.h>

#include "CTcMCEFloorControlObserver.h"

#include "sipprofile.h"

void TCmdEstablish::ExecuteL()
	{
	
	// --- Setup ---------------------------------------------------------------

	// Get session
	CMceOutSession* outSession = 
		reinterpret_cast<CMceOutSession*>(GetObjectForIdL(KSessionId, ETrue));	

	// --- Execute -------------------------------------------------------------
	
	// Get timeout 
	TInt timeout = ExtractIntegerL( KParamTimeout, 0, EFalse );
        
	// Get ContentType
	HBufC8* contentType = HBufCParameterL(ExtractTextL(KParamContentType, 
													   EFalse));
	CleanupStack::PushL( contentType );
	
	// Get Headers
	CDesC8Array* headers = ExtractHeadersL( EFalse );
	if (!headers)
		{
		headers = new (ELeave) CDesC8ArrayFlat(1);	
		}
	headers->AppendL(_L8("event: ttcn"));	
	CleanupStack::PushL( headers );
	
	// Get Body
	HBufC8* body = HBufCParameterL(ExtractTextL(KParamBody, EFalse));
	CleanupStack::PushL( body );

	// Floor control usage
	TBool useFC = ExtractBooleanL( KParamUseFloorControl, EFalse );
	
	if (useFC )
		{
		TInt fcPort = ExtractIntegerL(KParamFCPort, 2000, ETrue);
	 	iContext.FC().InitializeL( *outSession, fcPort, iContext.FCObserver() );
		
		// Floor control preparations
		CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(1);
		CleanupStack::PushL( array );	
		array->AppendL(
			_L8("a=fmtp:TBCP queuing=1; tb_priority=2; timestamp=1\r\n"));
		iContext.FC().UpdateL( *outSession, array );
		CleanupStack::Pop( array );
		}

	// Establish
	if ( !headers && !body && !contentType && timeout == 0 )
		{
		CleanupStack::PopAndDestroy( body ); body = NULL;
		CleanupStack::PopAndDestroy( headers ); headers = NULL;
		CleanupStack::PopAndDestroy( contentType ); contentType = NULL;
		outSession->EstablishL();
		}
	else
		{
		outSession->EstablishL( timeout, headers, contentType, body );
		CleanupStack::Pop( body );
		CleanupStack::Pop( headers );
		CleanupStack::Pop( contentType );	
		}

	CMceSession::TState state = outSession->State();

	// --- Construct response --------------------------------------------------

	AddIdResponseL(KSessionId, *outSession);	

	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );

	}

TBool TCmdEstablish::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("Establish") );
	}

TTcCommandBase* TCmdEstablish::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdEstablish( aContext );
	}

