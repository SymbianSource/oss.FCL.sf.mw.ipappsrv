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
#include "TCmdUpdateSDPLines.h"
#include "CTcMCEContext.h"

#include <MceMediaStream.h>
#include <MCESession.h>


void TCmdUpdateSDPLines::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------
	
	// Get session
	//CMceSession Configure SDP Line is not ready and it will have link error
	/*CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, EFalse));*/
	
	// Get stream 

	CMceMediaStream* mediaStream = 
		reinterpret_cast<CMceMediaStream*>(GetObjectForIdL(KStreamId, ETrue));	
	

	CDesC8Array* sdpLines ;
	if(mediaStream)
		{
		sdpLines = ExtractArrayL( KParamSDPLines, ETrue );
		// ---------- Execution ----------------------------------------------------
		mediaStream->SetMediaAttributeLinesL( sdpLines );
		// ---------- Response creation --------------------------------------------
		AddIdResponseL( KStreamId, *mediaStream ); 	
		}
	/*if (session)
		{
		sdpLines = ExtractArrayL( KParamSDPLines, ETrue );	
		// ---------- Execution ----------------------------------------------------
		session->ConfigureL( sdpLines );
		// ---------- Response creation --------------------------------------------
		AddIdResponseL( KSessionId, *session ); 
		}*/

	}
	
TBool TCmdUpdateSDPLines::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("UpdateSDPLines") );
	}

TTcCommandBase* TCmdUpdateSDPLines::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUpdateSDPLines( aContext );
	}
