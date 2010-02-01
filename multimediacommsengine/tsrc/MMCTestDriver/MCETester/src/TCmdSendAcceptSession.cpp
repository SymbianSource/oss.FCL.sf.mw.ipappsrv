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
#include "TCmdSendAcceptSession.h"
#include "CTcMCEContext.h"
#include <MceInSession.h>
#include "CTcMCEFloorControlObserver.h"

void TCmdSendAcceptSession::ExecuteL()
	{	
	// ---------- Setup ------------------------------------
	
	// Get session
	CMceInSession* session = reinterpret_cast<CMceInSession*>
		(GetObjectForIdL(KSessionId, ETrue));	
	
	// ---------- Execution --------------------------------

	// Get reasonphrase
	TPtrC8 reason = ExtractTextL(KParamReasonPhrase, EFalse);
		
	// Get statuscode
	TInt status = ExtractIntegerL(KParamStatusCode, 0, EFalse);
	
	// Get Headers
	//CDesC8Array* headers = ExtractHeadersL( EFalse );
	//CleanupStack::PushL(headers);
	
	// Floor control usage
	TBool useFC = ExtractBooleanL( KParamUseFloorControl, EFalse );
	


	if ( useFC )
		{
		TInt fcPort = ExtractIntegerL(KParamFCPort,2000, EFalse);
	 	iContext.FC().InitializeL( *session, fcPort, iContext.FCObserver() );
		// Floor control preparations
		MDesC8Array* mediaAttrLines = iContext.FC().MediaAttributeLinesL(*session);
		CDesC8Array* tmpArray = new (ELeave) CDesC8ArrayFlat(1);
		CleanupStack::PushL(tmpArray);
		if (mediaAttrLines->MdcaCount()>0)
			{
			for (TInt i = 0; i < mediaAttrLines->MdcaCount(); i++)
				{
				tmpArray->AppendL(mediaAttrLines->MdcaPoint(i));
				}
			}
		iContext.FC().UpdateL( *session, tmpArray );
		CleanupStack::Pop();
		delete mediaAttrLines;
		}

	// Accept
	if ( reason == KNullDesC8 && status == 0 )
		{
		session->AcceptL();
		}
	else
		{
		session->AcceptL(reason, status);
		}

	CMceSession::TState state = session->State();			

	// ---------- Response creation -----------------------
 
	AddIdResponseL( KSessionId, *session );

	// Add Session state	
	AddIntegerResponseL( KResponseState, state );
	AddTextualSessionStateL( state );		
	}
	
TBool TCmdSendAcceptSession::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendAnswer") );
	}

TTcCommandBase* TCmdSendAcceptSession::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendAcceptSession( aContext );
	}
