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
#include "TCmdSetStreamActivity.h"
#include "CTcMCEContext.h"

#include <MceMediaStream.h>

void TCmdSetStreamActivity::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get stream
	CMceMediaStream* mediaStream = reinterpret_cast<CMceMediaStream*>
		(GetObjectForIdL(KStreamId, ETrue));

	TBool enable = ExtractBooleanL( KParamActivity, ETrue );

	// ---------- Execution ----------------------------------------------------

	// Enable or disable stream
	if ( enable )
		{	
		mediaStream->EnableL();	
		}
	else
		{
		mediaStream->DisableL();		
		}

    TBool state = mediaStream->IsEnabled();

	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KStreamId, *mediaStream );
	AddBooleanResponseL( KResponseStreamState, state);
	AddTextualStreamStateL( state, EFalse );


	}
	
TBool TCmdSetStreamActivity::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SetStreamActivity") );
	}

TTcCommandBase* TCmdSetStreamActivity::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetStreamActivity( aContext );
	}
