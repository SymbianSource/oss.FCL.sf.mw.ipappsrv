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
#include "TCmdSetStreamLocalPort.h"
#include "CTcMCEContext.h"

#include <MceMediaStream.h>

void TCmdSetStreamLocalPort::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get stream
	CMceMediaStream* mediaStream = reinterpret_cast<CMceMediaStream*>
		(GetObjectForIdL(KStreamId, ETrue));

	TInt mediaStreamPort = ExtractIntegerL( KParamMediaStreamLocalPort, 0, EFalse );

	// ---------- Execution ----------------------------------------------------

	//SetPort
	mediaStream->SetLocalMediaPortL(mediaStreamPort);
	
    TBool state = mediaStream->IsEnabled();

	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KStreamId, *mediaStream );
    AddBooleanResponseL(KResponseStreamState, state);
    AddTextualStreamStateL( state, EFalse );


	}
	
TBool TCmdSetStreamLocalPort::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SetStreamLocalPort") );
	}

TTcCommandBase* TCmdSetStreamLocalPort::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetStreamLocalPort( aContext );
	}
