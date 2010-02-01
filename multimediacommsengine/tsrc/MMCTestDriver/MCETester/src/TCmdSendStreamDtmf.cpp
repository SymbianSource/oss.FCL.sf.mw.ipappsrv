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
#include "TCmdSendStreamDtmf.h"
#include "CTcMCEContext.h"

#include <MCESession.h>
#include <MceMediaStream.h>
#include <MceMediaSource.h>
#include <MceRtpSource.h>
#include <MceAudioStream.h>

#include <MCEAudioCodec.h>
#include <MceDtmfCodec.h>

const TUint KDtmfAudioPriority					= 45;
const TUint KDtmfAudioPref						= 0x00150001;

_LIT( KTestSequence, "0123456789abcd*#");
				
void TCmdSendStreamDtmf::ExecuteL()
	{	
	// ---------- Setup --------------------------------------------------------

// DTMF out-of-band means DTMF tones are sent as the numerical value in a packet
// (RTP type telephone-event, refer RFC2833).

	const TInt KDefaultDuration = 0;

		// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));
		
	TPtrC8 dtmfTones = ExtractTextL( KParamDtmfTones, EFalse );
	TInt dtmfDuration = ExtractIntegerL( KParamDtmfDuration, KDefaultDuration, EFalse );
	TBool isSingleTone = ExtractBooleanL( KParamDtmfIsSingleTone, EFalse );

	const RPointerArray<CMceMediaStream>& streams = session->Streams();	
	
	for ( TInt i = 0; i < streams.Count(); ++i )
        {                
        CMceMediaStream* mediaStream;
        mediaStream = streams[i];
        
        if ( mediaStream->Type() == KMceAudio && mediaStream->Source()->Type() == KMceRTPSource )
        	{
            CMceAudioStream* audioStream = dynamic_cast<CMceAudioStream*>(mediaStream);
            
            const RPointerArray<CMceAudioCodec>& codecsMain =  audioStream->Codecs();
            
            TBool dtmfFound = EFalse;
            for (TInt i = codecsMain.Count()-1; dtmfFound == EFalse && i >= 0;  i --)
				{
				if ( codecsMain[i]->SdpName().Compare(KValueCodecDtmf()))
					{
					//TODO modify dtmf codec 
					codecsMain[i]->SetMMFPriorityL( KDtmfAudioPriority );
	   			 	codecsMain[i]->SetMMFPriorityPreferenceL( KDtmfAudioPref );	
		   			codecsMain[i]->SetPreferenceL(0);
	   			 	dtmfFound = ETrue;
		   			 	   			 
					}
				}
            //getting the media source pointer from the audio stream;
            CMceMediaSource* dtmfSource = audioStream->Source();
			//checking if dtmf is available
			if(dtmfSource->DtmfAvailable() && (!dtmfSource->DtmfActive()) )
				{
				//sending dtmf tones
				if( dtmfTones.Compare( KNullDesC8 ) != 0 )
					{
					HBufC* buf = HBufC::NewLC(dtmfTones.Length());
                    buf->Des().Copy(dtmfTones);
					
					if( isSingleTone && dtmfDuration > 0)
						{
						TLex lexer( *buf );
						TChar tone = lexer.Get();
						//starts sending the tone
						dtmfSource->StartDtmfToneL( tone );
						
						iDtmfTimer = NULL;
						//create instance of timer to stop sending the tone after given duration
						iDtmfTimer = CDelayedProcess::NewL(dtmfSource);
							
						if(iDtmfTimer)
							{
							TTimeIntervalMicroSeconds32 delay = dtmfDuration * 1000;
							iDtmfTimer->StartProcess(delay);
							}
						}					
					else if( !isSingleTone )
						{
						iDtmfTimer = NULL;
						dtmfSource->SendDtmfToneSequenceL(*buf);	
						}
														
					iaudioStream = audioStream;
					CleanupStack::PopAndDestroy(buf);
					buf = NULL;					
					}
				else
					{
					//send default sequence if not present in the params
					dtmfSource->SendDtmfToneSequenceL(KTestSequence);
					iaudioStream = audioStream;
					}
				
				}
        	//stop sending dtmf        	
        	if( dtmfSource->DtmfActive() && ( !isSingleTone ) )
	        	{
	        	dtmfSource->CancelDtmfToneSequenceL();
	        	}
        	}
        }


		
	// ---------- Response creation --------------------------------------------
    if(iaudioStream)
	    {
	    AddIdResponseL( KStreamId, *iaudioStream );
		AddTextResponseL( KParamDtmfTones, dtmfTones );
		AddIntegerResponseL( KResponseLocalMediaPort, 
						 iaudioStream->LocalMediaPort() );		
	    }
    else
	    {
	    AddIntegerResponseL(KResponseActionReturnCode, KErrArgument);
	    }
	}
	
TBool TCmdSendStreamDtmf::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("SendStreamDtmf") );
	}

TTcCommandBase* TCmdSendStreamDtmf::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSendStreamDtmf( aContext );
	}
