/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include "TCmdRemoveStream.h"
#include "CTcMCEContext.h"

#include <badesca.h>
#include <MCESession.h>
#include <MCEVideoStream.h>
#include <MCEMediaStream.h>

void TCmdRemoveStream::ExecuteL()
	{	

	// ---------- Setup --------------------------------------------------------

	// Get session
//	CMceSession* session = reinterpret_cast<CMceSession*>
//		(GetObjectForIdL(KSessionId, ETrue));	

	// Get existing stream
	CMceMediaStream* mainStream = reinterpret_cast<CMceMediaStream*>
		(GetObjectForIdL(KStreamId, EFalse));	

	// ---------- Execution ----------------------------------------------------
	
	mainStream->Session()->RemoveStreamL( *mainStream );	
 
	AddIdResponseL(KStreamId, *mainStream); 
	
	}
	
TBool TCmdRemoveStream::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("RemoveStream") );
	}

TTcCommandBase* TCmdRemoveStream::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdRemoveStream( aContext );
	}
