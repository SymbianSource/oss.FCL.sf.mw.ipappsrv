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
#include "TCmdGetSupportedCodecs.h"
#include "CTcMCEContext.h"

#include <MCEAudioCodec.h>
#include <MCEVideoCodec.h>

void TCmdGetSupportedCodecs::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

	// ---------- Execution ----------------------------------------------------

    // Get supported audio codecs
	// Array for codec names
	CDesC8Array* audioCodecs = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( audioCodecs );

    const RPointerArray<const CMceAudioCodec>& managerAudioCodecs =
                iContext.MCEManager().SupportedAudioCodecs();

    for ( TInt i = 0; i < managerAudioCodecs.Count(); i++ )
        {
        const CMceAudioCodec* managerAudioCodec = managerAudioCodecs[i];
        
        audioCodecs->AppendL( managerAudioCodec->SdpName() );
        }
	
 // Get supported video codecs  
   	CDesC8Array* avideoCodecs = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( avideoCodecs );

    const RPointerArray<const CMceVideoCodec>& managerVideoCodecs =
                iContext.MCEManager().SupportedVideoCodecs();

    for ( TInt i = 0; i < managerVideoCodecs.Count(); i++ )
        {
        const CMceVideoCodec* c_videoCodec = managerVideoCodecs[i];
        
        avideoCodecs->AppendL( c_videoCodec->SdpName() );
        }
	
	// ---------- Response creation --------------------------------------------

	if ( audioCodecs->Count() || avideoCodecs->Count() )
		{	
        AddArrayResponseL( KResponseSupportedAudioCodecs, *audioCodecs );
        AddArrayResponseL( KResponseSupportedVideoCodecs, *avideoCodecs );
		}
		
	CleanupStack::Pop( avideoCodecs );
	CleanupStack::Pop( audioCodecs );	

	}
	
TBool TCmdGetSupportedCodecs::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("GetSupportedCodecs") );
	}

TTcCommandBase* TCmdGetSupportedCodecs::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdGetSupportedCodecs( aContext );
	}
