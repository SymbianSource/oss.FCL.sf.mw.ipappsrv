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
#include "TCmdUpdateMediaSDPLines.h"
#include "CTcMCEContext.h"

#include <MceMediaStream.h>


void TCmdUpdateMediaSDPLines::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get stream 

	CMceMediaStream* mediaStream = 
		reinterpret_cast<CMceMediaStream*>(GetObjectForIdL(KStreamId, ETrue));	

	// ---------- Execution ----------------------------------------------------

	CDesC8Array* mediaLines = ExtractArrayL( KParamMediaSDPLines, ETrue );

    mediaStream->ConfigureL( mediaLines );

	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KStreamId, *mediaStream );  

	}
	
TBool TCmdUpdateMediaSDPLines::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("UpdateMediaSDPLines") );
	}

TTcCommandBase* TCmdUpdateMediaSDPLines::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUpdateMediaSDPLines( aContext );
	}
