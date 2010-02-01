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
#include "TCmdSetSourceActivity.h"
#include "CTcMCEContext.h"

#include <MceMediaSource.h>

void TCmdSetSourceActivity::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get source
	CMceMediaSource* source = reinterpret_cast<CMceMediaSource*>
		(GetObjectForIdL(KSourceId, ETrue));	

	TBool enable = ExtractBooleanL( KParamActivity, ETrue );

	// ---------- Execution ----------------------------------------------------

	if ( enable )
		{
		if ( !source->IsEnabled() )
			{
			source->EnableL();
			}
		}
	else
		{
		if ( source->IsEnabled() )
			{
			source->DisableL();
			} 	
		}


	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KSourceId, *source );
    AddBooleanResponseL( KResponseActivity, source->IsEnabled() );


	}
	
TBool TCmdSetSourceActivity::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SetSourceActivity") );
	}

TTcCommandBase* TCmdSetSourceActivity::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetSourceActivity( aContext );
	}
