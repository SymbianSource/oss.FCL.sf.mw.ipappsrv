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
#include "TCmdRemoveCodec.h"
#include "CTcMCEContext.h"

#include <badesca.h>
//#include <MceInSession.h>
#include <MCEAudioStream.h>
//#include <MCEMicSource.h>
//#include <MceRtpSink.h>
//#include <MceRtpSource.h>
//#include <MCESpeakerSink.h>
#include <MCEAudioCodec.h>
//#include <MCEAMRCodec.h>

void TCmdRemoveCodec::ExecuteL()
	{	

	// ---------- Setup --------------------------------------------------------

	// Get existing stream
	CMceAudioStream* stream = reinterpret_cast<CMceAudioStream*>
		(GetObjectForIdL(KStreamId, EFalse));	

	// ---------- Execution ----------------------------------------------------

		
	// Modify codecs
	CDesC8Array* codecs = ExtractArrayL( KParamCodecs, EFalse );
	
	if ( codecs )
		{
		CleanupStack::PushL( codecs );
		if ( codecs->Count() > 0)
			{
			const RPointerArray<CMceAudioCodec>& streamCodecs = stream->Codecs();
			
			if ( (*codecs)[0] == KValueCodecAll )
				{
			    // Remove all codecs from stream
                for (TInt i = streamCodecs.Count(); i > 0;  i-- )
                    {
                    stream->RemoveCodecL( *(streamCodecs[i-1]) );
                    }
				}
			else 
				{
			    // Loop all codecs from stream
                for (TInt i = streamCodecs.Count(); i > 0;  i-- )
                    {
                    for (TInt ii = 0; ii < codecs->Count(); ii++) 
                    	{
                        if ( streamCodecs[i-1]->SdpName() == (*codecs)[ii] )
                    		{
                    		stream->RemoveCodecL( *(streamCodecs[i-1]) );
                    		}
                    
                    	}	
                    }
				
				}
			}
		CleanupStack::PopAndDestroy( codecs );
		}
	
	// ---------- Response creation --------------------------------------------
 
	AddIdResponseL(KStreamId, *stream); 
	
	}
	
TBool TCmdRemoveCodec::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("RemoveCodec") );
	}

TTcCommandBase* TCmdRemoveCodec::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdRemoveCodec( aContext );
	}
	
	
CMceAudioCodec* TCmdRemoveCodec::GetAudioCodecCloneL( const TDesC8& aParamCodecName )
    {
    
    const RPointerArray<const CMceAudioCodec>& managerAudioCodecs =
        iContext.MCEManager().SupportedAudioCodecs();
    for ( TInt i = 0; i < managerAudioCodecs.Count(); i++ )
        {
        const CMceAudioCodec* managerAudioCodec = managerAudioCodecs[i];
        
        if ( managerAudioCodec->SdpName() == aParamCodecName )
            {
            return managerAudioCodec->CloneL();
            }
        
        }
    
    // If the requested codec is not supported
    User::Leave( KErrNotSupported );
    return NULL; // To keep compiler happy
    }
