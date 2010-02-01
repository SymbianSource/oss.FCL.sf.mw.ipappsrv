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
#include "TCmdSendRR.h"
#include "CTcMCEContext.h"

#include <MceMediaSource.h>
#include <MCERTPSource.h>

void TCmdSendRR::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	/*CMceMediaSink* mediaSink = reinterpret_cast<CMceMediaSink*>
		(GetObjectForIdL(KSinkId, ETrue));    */
	CMceMediaSource* mediaSource = reinterpret_cast<CMceMediaSource*>
		(GetObjectForIdL(KSourceId, ETrue));    

    if ( mediaSource->Type() != KMceRTPSource )
        {
        User::Leave( KErrArgument );
        }
   
    CMceRtpSource* rtp = static_cast<CMceRtpSource*>(mediaSource);
        
	// ---------- Execution ----------------------------------------------------

    rtp->SendRRL();

	// ---------- Response creation --------------------------------------------
 
    AddIdResponseL( KSinkId, *rtp );

	}
	
TBool TCmdSendRR::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendRR") );
	}

TTcCommandBase* TCmdSendRR::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendRR( aContext );
	}
