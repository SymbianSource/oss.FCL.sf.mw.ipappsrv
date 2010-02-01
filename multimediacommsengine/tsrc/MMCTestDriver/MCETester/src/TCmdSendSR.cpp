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
#include "TCmdSendSR.h"
#include "CTcMCEContext.h"

#include <MceMediaSink.h>
#include <MCERTPSink.h>

void TCmdSendSR::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	CMceMediaSink* mediaSink = reinterpret_cast<CMceMediaSink*>
		(GetObjectForIdL(KSinkId, ETrue));    

    if ( mediaSink->Type() != KMceRTPSink )
        {
        User::Leave( KErrArgument );
        }
   
    CMceRtpSink* rtp = static_cast<CMceRtpSink*>(mediaSink);
        
	// ---------- Execution ----------------------------------------------------

    rtp->SendSRL();

	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KSinkId, *rtp );


	}
	
TBool TCmdSendSR::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendSR") );
	}

TTcCommandBase* TCmdSendSR::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendSR( aContext );
	}
