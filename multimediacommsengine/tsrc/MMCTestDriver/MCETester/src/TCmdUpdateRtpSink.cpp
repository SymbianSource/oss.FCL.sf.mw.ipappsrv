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
#include "TCmdUpdateRtpSink.h"
#include "CTcMCEContext.h"

#include <MceRtpSink.h>

void TCmdUpdateRtpSink::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// Get sink
	CMceRtpSink* sink = reinterpret_cast<CMceRtpSink*>
		(GetObjectForIdL(KSinkId, ETrue));	

	TBool suppress = ExtractBooleanL( KParamSuppressRtcp, ETrue );
	HBufC8* identity = HBufCParameterL(ExtractTextL( KParamOriginator,EFalse));
	// ---------- Execution ----------------------------------------------------

	sink->UpdateL(suppress, identity);


	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KSinkId, *sink );
	}
	
TBool TCmdUpdateRtpSink::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("UpdateRtpSink") );
	}

TTcCommandBase* TCmdUpdateRtpSink::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdUpdateRtpSink( aContext );
	}
