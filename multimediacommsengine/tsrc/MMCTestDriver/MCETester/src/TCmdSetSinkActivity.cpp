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
#include "TCmdSetSinkActivity.h"
#include "CTcMCEContext.h"

#include <MceMediaSink.h>

void TCmdSetSinkActivity::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get sink
	CMceMediaSink* sink = reinterpret_cast<CMceMediaSink*>
		(GetObjectForIdL(KSinkId, ETrue));	

	TBool enable = ExtractBooleanL( KParamActivity, ETrue );
	
	// ---------- Execution ----------------------------------------------------

	if ( enable )
		{
		if ( !sink->IsEnabled() )
			{
			sink->EnableL();
			}
		}
	else
		{
		if ( sink->IsEnabled() )
			{
			sink->DisableL();
			} 	
		}   

	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KSinkId, *sink );
    AddBooleanResponseL( KResponseActivity, sink->IsEnabled() );
    
	}
	
TBool TCmdSetSinkActivity::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SetSinkActivity") );
	}

TTcCommandBase* TCmdSetSinkActivity::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetSinkActivity( aContext );
	}
