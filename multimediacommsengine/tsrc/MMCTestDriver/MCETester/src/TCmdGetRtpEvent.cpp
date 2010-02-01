/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "TCmdGetRtpEvent.h"
#include "CTcMCEContext.h"
#include <MCEMediaStream.h>
#include <MCERtpSource.h>

/**
* INPUT:
*   Parameters: (int:CallbackType)
*               (int:Timeout=5)             
*
*   IDs:        -
*
* OUTPUT:
*   Parameters: int:CallbackType
*
*   IDs:        StreamId 
*               SourceId
*/

void TCmdGetRtpEvent::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------


	// ---------- Execution ----------------------------------------------------
	
	// It is not necessary to define what event is expected
	TTcMceCallbackType type = ExtractIntegerL( KParamCallbackType, KTcMceUnknown, EFalse );
									
	// Get an item off the receive queue (waits until timeout if none is present)
	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, EFalse );

	// Get an item off the receive queue (waits until timeout if none is present)
	TPtrC8 sourceId = ExtractIdL( KParamSourceId, EFalse );
	//Get RTP Source object from registry
	CMceRtpSource *rtpSource = NULL;
	rtpSource =reinterpret_cast<CMceRtpSource*> (iContext.Registry().ObjectPtrL(sourceId));
	//time as millisecs
	rtpSource->EnableInactivityTimerL(timeout*1000);

	//time as seconds
	CTcMCEReceived& item = iContext.ReceivedRTPItemL( timeout + 5);

 	if (!item.MediaStream() || 
 	    !item.RtpSource() ||
 	    ( type != KTcMceUnknown && item.Type() != type ) )
 		{
 		User::Leave( KErrNotFound );
 		}
	// ---------- Response creation --------------------------------------------
 

 	AddIdResponseL( KStreamId, *(item.MediaStream()) );
	AddIdResponseL( KSourceId, *(item.RtpSource()) );
	AddIntegerResponseL( KParamCallbackType, item.Type() );
	}
	
TBool TCmdGetRtpEvent::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetRtpEvent") );
	}

TTcCommandBase* TCmdGetRtpEvent::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetRtpEvent( aContext );
	}
