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
#include "TCmdGetReceiverReport.h"
#include "CTcMCEContext.h"
#include <MceMediaStream.h>
#include <MceRtpSource.h>


void TCmdGetReceiverReport::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------


	// ---------- Execution ----------------------------------------------------
	
	// Get an item off the receive queue (waits until timeout if none is present)
	TInt timeout = ExtractIntegerL( KParamTimeout, KDefaultReceiveTimeout, 
									EFalse );
	CTcMCEReceived& item = iContext.ReceivedRTPItemL( timeout );	

 	if (!item.MediaStream() || !item.RtpSource())
 		{
 		User::Leave( KErrNotFound );
 		}
	// ---------- Response creation --------------------------------------------
 

 	AddIdResponseL( KStreamId, *(item.MediaStream()) );
	AddIdResponseL( KSourceId, *(item.RtpSource()) );
	
	}
	
TBool TCmdGetReceiverReport::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetReceiverReport") );
	}

TTcCommandBase* TCmdGetReceiverReport::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetReceiverReport( aContext );
	}
