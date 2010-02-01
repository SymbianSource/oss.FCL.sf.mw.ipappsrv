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
* Description:    
*
*/




// MCE API
#include <ecam.h>
#include <MCEManager.h>
#include <Mcedefs.h>
#include <MCEOutSession.h>
#include <mcerefer.h>
#include <MCEAudioStream.h>
#include <MCEAudioStream.h>
#include <mcevideostream.h>
#include <MCEMicSource.h>
#include <MCERtpSink.h>
#include <MCERtpSource.h>
#include <MCESpeakerSink.h>
#include <mcecamerasource.h>
#include <mcedisplaysink.h>
#include <MCEmediaSink.h>
#include <MCEFileSource.h>
#include <Uri8.h>
#include <mcefilesink.h>
#include <MCEAudioCodec.h>
#include <MCEVideoCodec.h>
#include <MCEAMRCodec.h>
#include <MCEFCMsgExchange.h>
#include <mcestreambundle.h>
#include <mcesecureoutsession.h>
#include <mcesecuresession.h>
#include <aknutils.h>
#include <mceavccodec.h>
#include "MCETestUIEngineDefaultValues.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineProfileContainer.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineOutSession.h"
#include "TMCETestUIEngineCmdEstablish.h"
#include "TMCETestUIEngineCmdCancelSession.h"
#include "TMCETestUIEngineCmdDeleteSession.h"
#include "TMCETestUIEngineCmdFloorControlSession.h"
#include "CMCETestUIEngineMultimediaObserver.h"
#include "TMCETestUIEngineCmdCryptoHandling.h"

#include "TMCETestUIEngineCmdSRReqRes.h"



//const TUint KAudioPrefAudioUplink                      = 0x05220001;
//const TUint KAudioPrefAudioDownlink                    = 0x05210001;


const TUint KJitterBufferInactTimeout 					  = 10000;

_LIT8( KMCERtpSinkIdentity, "RtpSink@multimedia.engine.com" ); 
_LIT8( KMCERtpSourceIdentity, "RtpSource@multimedia.engine.com" ); 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutSession* CMCETestUIEngineOutSession::NewL(
            CMCETestUIEngine& aEngine,
            CMCETestUIEngineProfile& aProfile,
            TDirection aDirection,
            TVideoDirection aVideoDirection,
            TSessionType aSessionType ) 
    {

    CMCETestUIEngineOutSession* self = 
        new (ELeave) CMCETestUIEngineOutSession( aEngine, 
                                                 aProfile,
                                                 aDirection,
                                                 aVideoDirection,
                                                 aSessionType );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutSession* CMCETestUIEngineOutSession::NewL(
            CMCETestUIEngine& aEngine,
            TUint32 aIAPId,
            TDirection aDirection,
            TVideoDirection aVideoDirection,
            TSessionType aSessionType ) 
    {

    CMCETestUIEngineOutSession* self = 
        new (ELeave) CMCETestUIEngineOutSession( aEngine, 
                                                 aIAPId,
                                                 aDirection,
                                                 aVideoDirection,
                                                 aSessionType );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutSession* CMCETestUIEngineOutSession::NewL(
            CMCETestUIEngine& aEngine,
            CMceRefer &aRefer,
            TDirection aDirection,
            TVideoDirection aVideoDirection,
            TSessionType aSessionType ) 
    {

    CMCETestUIEngineOutSession* self = 
        new (ELeave) CMCETestUIEngineOutSession( aEngine, 
                                                 aRefer,
                                                 aDirection,
                                                 aVideoDirection,
                                                 aSessionType );
    CleanupStack::PushL( self );
    self->ConstructL(aRefer);
    CleanupStack::Pop( self );
    return self;  
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::CMCETestUIEngineOutSession
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutSession::CMCETestUIEngineOutSession( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineProfile& aProfile,
                TDirection aDirection,
                TVideoDirection aVideoDirection,
                TSessionType aSessionType )
    : CMCETestUIEngineSession( aEngine, aDirection,aVideoDirection ),
      iProfile( &aProfile ),
      iId( 0 ),
      iSessionType(aSessionType),
      iIapId ( 0xFF )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::CMCETestUIEngineOutSession
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutSession::CMCETestUIEngineOutSession( 
                CMCETestUIEngine& aEngine,
                TUint32 aIAPId,
                TDirection aDirection,
                TVideoDirection aVideoDirection,
                TSessionType aSessionType )
    : CMCETestUIEngineSession( aEngine, aDirection,aVideoDirection ),
      iIapId( aIAPId ),
      iId( 0 ),
      iSessionType(aSessionType),
      iProfile ( NULL )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::CMCETestUIEngineOutSession
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutSession::CMCETestUIEngineOutSession( 
                CMCETestUIEngine& aEngine,
                CMceRefer &aRefer,
                TDirection aDirection,
                TVideoDirection aVideoDirection,
                TSessionType /*aSessionType*/ )
    : CMCETestUIEngineSession( aEngine, aDirection,aVideoDirection ),
      iId( 0 ),
      iRefer(&aRefer)
      
    {
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutSession::ConstructL()
    {
 
     /////////////////////
    //
    // TODO: After implementing wrappers for streams, sinks and sources 
    //       this function should be refactored to make use of them. That will 
    //       also simplify the function to some extent.
    //
    //////////////////////
    BaseConstructL();
    FillProfileRegistrar();   
    FillPoCOrigAddress();
    if( SessionParamsL(iSessionParams,iSessionType) )
        {
    	// set rtcp flag
    	 if(iEngine.iRtcpFlag == EFalse)
    	 	{
    	 	iEngine.iSRCount = 0;
    		iEngine.iRRCount = 0;	
    	 	}
       		
     
    	 // Solve recipient
	    CMCETestUIQuestionDataQuery* question = 
		    CMCETestUIQuestionDataQuery::NewLC();
		    
        question->SetCaptionL( KUserQuestionInputRecipient );
    	question->SetDefaultValueL(iSessionParams.iRecipentAddress  );
    	question->SetAnswerL( iSessionParams.iRecipentAddress  );
    	TBufC8<300> recipient( question->Answer8() );
    	
    	CleanupStack::PopAndDestroy( question );
	  
		//////
		// solve originator
	    CMceOutSession* outSession = NULL;
	    
	    if( iSessionParams.originator.Compare(KNone) )//if there is originator
	   	    {
		   	HBufC8* orig8 = HBufC8::NewL(iSessionParams.originator.Length());
		   	TPtr8 ptr = orig8->Des();
		    ptr.Copy(iSessionParams.originator);
		    // Create Outsession
		    if(iEngine.iOutSessionWithProfile)// thru profile
		    	{
		    	
		    	if( iSessionParams.iSecureSession )
		    		{
		    		iOutSession = 
		    				CMceSecureOutSession::NewL( iEngine.MCEManager(),
		                                                iProfile->Profile(),
		                                                recipient ,orig8);
		            if( iEngine.iSecureSessionAVP )
			    		{
			    		iOutSession->SetModifierL( KMceSecureSession, 
			    								   KMceSecurePlainAVP );
			    		} 
			    		
			        if ( iEngine.iSecPreconditions )
			    		{			    		
					    iOutSession->SetModifierL( KMceSecPreconditions, 
											   KMcePreconditionsE2ESupported );
			    		}	

		    		}
		    	else
		    		{
		    		iOutSession = CMceOutSession::NewL( iEngine.MCEManager(),
		                                            iProfile->Profile(),
		                                            recipient ,orig8);	
		    		}
		    	
		    	}
		    else ///thru accesspoint , it can be  done only with originator
		    	{
		    	iOutSession = CMceOutSession::NewL( iEngine.MCEManager(),
		                                           iIapId,ptr,recipient);
		    			    	
		    	}
			    
	   	    }
	   	else  // if there is only recipent and no originator 
	   	    {
	   	    if ( iSessionParams.iSecureSession )
		    	{
		    	iOutSession = 
		    		CMceSecureOutSession::NewL( iEngine.MCEManager(), 
	                                            iProfile->Profile(),
	                                            recipient );
	            if ( iEngine.iSecureSessionAVP )
			    		{
			    		iOutSession->SetModifierL( KMceSecureSession, 
			    								   KMceSecurePlainAVP );
			    		} 
		    	if ( iEngine.iSecPreconditions )
			    		{			    		
						iOutSession->SetModifierL( KMceSecPreconditions, 
										KMcePreconditionsE2ESupported );
			    		}
	
		    	}
		    else
		    	{
		    	iOutSession = CMceOutSession::NewL( iEngine.MCEManager(), 
	                                            iProfile->Profile(),
	                                            recipient );
		    	}
	   	    
	   	    }
	    
	    iSession = iOutSession;

        if ( iSessionType == EPoCSession)
			{
		 	FillPoCSessionData(iOutSession);
			}
		else if (iSessionType == EVoIPSession)
			{
			FillVoIPSessionData(iOutSession);
			}
	    else if (iSessionType == E2WayVideoSession)
	        {
	        Fill2WayVideoSessionDataL(iOutSession);
	        }
		else if (iSessionType == ECustomSession)
			{
			FillCustomSessionData(iOutSession);
			}
		if ( iEngine.iHoldWithAddress )
    		{
    		iOutSession->SetModifierL( KMceMediaDirection, KMceMediaDirectionWithAddress );
    		} 
		if ( iEngine.IsFCSession() ) 
			{
			TRAP_IGNORE(iEngine.FloorControl().InitializeL(
												*iOutSession,0,iEngine.MMObserver()));
			CDesC8Array* array = NULL;
		
			array = new (ELeave) CDesC8ArrayFlat(1);	
		    array->AppendL(_L8("a=fmtp:TBCP queuing=1; tb_priority=0; timestamp=1; tb_granted=1; poc_sess_priority=1; poc_lock=1\r\n"));
			iEngine.FloorControl().UpdateL(*iOutSession, array);
				
			}
		
		if ( iEngine.iSdp )
			    {
			    CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(3);
			    array->AppendL(_L8("a=OutgoingSession1:20\r\n"));
                array->AppendL(_L8("a=OutgoingSession2:40\r\n"));
                array->AppendL(_L8("b=AS:64\r\n"));
			    iOutSession->SetSessionSDPLinesL( array );
			    }
        }
		
		    // Get identity
	    iId = iEngine.NextOutSessionId();
    	iType = KMceOutSession;
   
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::ConstructSessionFromReferL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutSession::ConstructL(CMceRefer& /*aRefer*/)
    {
 
     /////////////////////
    //
    // TODO: After implementing wrappers for streams, sinks and sources 
    //       this function should be refactored to make use of them. That will 
    //       also simplify the function to some extent.
    //
    //////////////////////
    BaseConstructL();
    
	    CMceOutSession* outSession = NULL;
	    iOutSession = CMceOutSession::NewL( *iRefer );
		   
	    iSession = iOutSession;

        if ( iSessionType == ECustomSession)
			{
		//	FillCustomSessionData(iOutSession);
			FillVoIPSessionData(iOutSession);
			}
		if( iEngine.iHoldWithAddress )
    		{
    		iOutSession->SetModifierL( KMceMediaDirection, KMceMediaDirectionWithAddress );
    		} 
		if( iEngine.IsFCSession() ) 
			{
			TRAP_IGNORE(iEngine.FloorControl().InitializeL(
												*iOutSession,0,iEngine.MMObserver()));
			CDesC8Array* array = NULL;
		
			array = new (ELeave) CDesC8ArrayFlat(1);	
		    array->AppendL(_L8("a=fmtp:TBCP queuing=1; tb_priority=0; timestamp=1; tb_granted=1; poc_sess_priority=1; poc_lock=1\r\n"));
			iEngine.FloorControl().UpdateL(*iOutSession, array);
				
			}
		if( iEngine.iSdp )
		    {
		    CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(2);
		    array->AppendL(_L8("a=OutgoingSession1:20\r\n"));
            array->AppendL(_L8("a=OutgoingSession2:40\r\n"));
		    iOutSession->SetSessionSDPLinesL( array );
		    }
	
		
		    // Get identity
	    iId = iEngine.NextOutSessionId();
    	iType = KMceOutSession;
   
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::~CMCETestUIEngineOutSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutSession::~CMCETestUIEngineOutSession()
    {
    delete iOutSession;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineOutSession::GetCommandsL()
    {
    iCommands.ResetAndDestroy();

    // Get base class commands
    CMCETestUIEngineSession::GetCommandsL();
        
    // Get own commands       
    TInt state = iOutSession->State();

	switch( state ) 
		{
		case CMceSession::EIdle: 
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdEstablish( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) TMCETestUIEngineCmdEstablishWithValues( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) TMCETestUIEngineCmdFCSendSession( 
                                iEngine, *this ) );
               
            iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) );
		    if(iSession->Type() == KMceSessionSecure) 
		    	{
		    	iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdCurrentCryptoContext( iEngine, *this ) );
		        iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdSupportedCryptoContext( iEngine, *this ) );
		        iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdSetCryptoContext( iEngine, *this ) );
		       	}
		    break;
		case CMceSession::EOffering: 
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdCancelSession( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) TMCETestUIEngineCmdFCSendSession( 
                                iEngine, *this ) );
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) );     
			
		    iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdSendInfo( iEngine, *this ) );
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDTMFInfo( iEngine, *this ) );
		    iCommands.Append( new ( ELeave )
		    	TMCETestUIEngineCmdSendInfoResponse( iEngine, *this ) );

		        
		        
		    if(iSession->Type() == KMceSessionSecure) 
		    	{
		    	iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdCurrentCryptoContext( iEngine, *this ) );
		        iCommands.Append( new (ELeave) 
		        	TMCETestUIEngineCmdSupportedCryptoContext( iEngine, *this ) );
		        }
		                              
		    break;
		case CMceSession::EEstablished:
			if(VideoDirection() != EVideoNotSpesified)    
		    	{
		    	if(iEngine.IsVideoEnabled())
		    	{
		    	iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDisableVideoSession( iEngine, *this ) );
		    	}
		    	else
		    	{
		    	iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdEnableVideoSession( iEngine, *this ) );     		        
		    	}
		    	}
		    break;		
		default: 
		    break;
		}  
		  
    return iCommands; 
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::Id
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMCETestUIEngineOutSession::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::OutSession
// -----------------------------------------------------------------------------
//
CMceOutSession& CMCETestUIEngineOutSession::OutSession()
    {
    return *iOutSession;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::SessionParams
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngineOutSession::SessionParamsL(TSessionParams &aSessionParams,
									TSessionType aSessionType)
	{
  	return iEngine.SessionParamsL( aSessionParams,aSessionType );
 	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::FillPoCSessionData
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineOutSession::FillProfileRegistrar()
	{
	// trying to get profile registrar 
    if(iEngine.iOutSessionWithProfile)
    	{
    	TUriParser8 parser;
	    const TDesC8& aor = iProfile->Aor();
	    parser.Parse( aor ) ; 
	    CUri8* uri8 = CUri8::NewLC( parser );
	    const TDesC8& registrar =  uri8->Uri().Extract( EUriHost ) ;
	    
	    HBufC*registrar16 = HBufC16::NewLC( KRecipent().Length() + registrar.Length() );
	    TPtr registrarPtr = registrar16->Des();
	    registrarPtr.Copy( registrar );
	    
	    iSessionParams.iRecipentAddress.Append(KRecipent) ;
	    iSessionParams.iRecipentAddress.Append(*registrar16)  ;
	    
	    CleanupStack::PopAndDestroy( registrar16 );
	    registrar16 = NULL;
	    
	    CleanupStack::PopAndDestroy( uri8 );
	    uri8 = NULL; 
    	}
       else
       	{
       	iSessionParams.iRecipentAddress.Append(KRecipent) ;
       	}
    
    // profile registrar end .....
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::FillPoCorigAddress
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineOutSession::FillPoCOrigAddress()
	{
 
    if(iEngine.IsFCSession())
    	{
    	const TDesC8& aor = iProfile->Aor();
    	HBufC*aor16 = HBufC16::NewLC( aor.Length() );
    	TPtr aorPtr = aor16->Des();
    	aorPtr.Copy(aor);
    	iSessionParams.originator.Append(*aor16)  ;
	    CleanupStack::PopAndDestroy( aor16 );
	    aor16 = NULL;
	   	}
       
   	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::FillVoIPSessionData
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineOutSession::FillVoIPSessionData(CMceOutSession* outSession)
	{
   
    // OUT STREAM
    // create "audio out" stream first, puts audioOutStream to cleanupstack
    CMceAudioStream* audioOutStream = CMceAudioStream::NewLC();
   
    // create mic source
    CMceMicSource* mic = CMceMicSource::NewLC();
   
    // set source for "audio out" stream, in this case it's microphone
    // there can be only one source at a time.
    audioOutStream->SetSourceL( mic );
    
    // pop mic from cleanupstack
    CleanupStack::Pop( mic );
    
    // create rtp sink for mic
    CMceRtpSink* rtpSink = CMceRtpSink::NewL();
	CleanupStack::PushL( rtpSink );
	audioOutStream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );
   
    // IN STREAM
    // create "audio in" stream
    CMceAudioStream* audioInStream = CMceAudioStream::NewLC();
      
    // create rtp source for "audio in" stream
	CMceRtpSource* rtpSource = CMceRtpSource::NewLC( iEngine.iJitterBufSize, 
                                	                 iEngine.iJitterBufTreshold, 0 );
	audioInStream->SetSourceL( rtpSource );
	// pop rtp source from cleanupstack
	CleanupStack::Pop( rtpSource );
	
	// create speaker sink for "audio in" stream
	CMceSpeakerSink* speaker = CMceSpeakerSink::NewLC();	
	audioInStream->AddSinkL( speaker );
	// pop speaker from cleanupstack
	CleanupStack::Pop( speaker );
	
	// bind "audio out" stream to "audio in" stream
	// audioInStream->BindL( audioOutStream );
	audioOutStream->BindL( audioInStream );
    // iSession->AddStreamL( audioInStream );
    outSession->AddStreamL( audioOutStream );
    
    // modify QoS preconditions off
    outSession->SetModifierL( KMcePreconditions, KMcePreconditionsNotUsed  );

        
    // add codecs to audiostream 
    // some default voip values to be still added
    
    // set MMF priorities and preferences to codecs
    // set MMF codec priorities  for all codecs in both streams
    
    iEngine.SetMMFPriorityForCodecsL( audioInStream->Codecs(),
                              iEngine.iPriorityDownlink, 
                              iEngine.iPrefDownlink );
    
    iEngine.SetMMFPriorityForCodecsL( audioOutStream->Codecs(),
                              iEngine.iPriorityUplink, 
                              iEngine.iPrefUplink );                                
    
    // pop audioInStream from cleanupstack
    CleanupStack::Pop( audioInStream );
    
    // pop audioOutStream from cleanupstack
	CleanupStack::Pop( audioOutStream );
   
    
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::FillPoCSessionData
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineOutSession::FillPoCSessionData(CMceOutSession* outSession)
	{
	iDirection = ESendRecv;
	const TInt KStartLocalPort = 6688;
    const TInt AMRDefOfferPTime = 160;
    
    // Maximum ptime
    const TInt AMRDefOfferMaxPTime = 400;
    
    // Allowed bitrates
    const TInt AMRDefOfferAllowedBitrates = KMceAllowedAmrNbBitrate475 | 
								  KMceAllowedAmrNbBitrate515 | 
								  KMceAllowedAmrNbBitrate590;
								  
    // Bitrate
    const TInt AMRDefOfferBitrate = KMceAmrNbBitrate515;
    
    // Codec mode
    const TInt AMRDefOfferCodingMode = EMceOctetAligned;
    
    
	 CMceAudioStream* audioOutStream = NULL;
	 	// audio in stream
	CMceAudioStream* audioStreamIn = CMceAudioStream::NewLC();

	TInt locPort( KStartLocalPort );
	locPort = locPort + ( 128 * 2 );
	
	audioStreamIn->SetLocalMediaPortL( locPort );
		
	CMceSpeakerSink* speakerSink = CMceSpeakerSink::NewLC();
	audioStreamIn->AddSinkL(speakerSink);
	CleanupStack::Pop();
	
	// TODO:
	HBufC8* rtpSourceIdentity = KMCERtpSourceIdentity().AllocLC();
	CMceRtpSource* rtpSource = CMceRtpSource::NewLC( iEngine.iJitterBufSize, 
                                	                 iEngine.iJitterBufTreshold, 
                                	                 iEngine.iJitterBufTimeout,
                                	                 rtpSourceIdentity);
	audioStreamIn->SetSourceL(rtpSource);
	//rtpSource->EnableInactivityTimerL(10000);
	CleanupStack::Pop();	
	CleanupStack::PopAndDestroy(rtpSourceIdentity);
	
	CMceAudioStream* audioStreamOut = CMceAudioStream::NewLC();
	HBufC8* rtpSinkIdentity = KMCERtpSinkIdentity().AllocLC();
	CMceRtpSink* rtpSink = CMceRtpSink::NewL( ETrue, rtpSinkIdentity);
	audioStreamOut->AddSinkL(rtpSink);
	CleanupStack::PopAndDestroy(rtpSinkIdentity);
	
	CMceMicSource* micSource = CMceMicSource::NewLC();
	audioStreamOut->SetSourceL(micSource);
	micSource->DisableL();
	CleanupStack::Pop();	
	
	audioStreamIn->BindL(audioStreamOut);
	CleanupStack::Pop(audioStreamOut);
	// floor control

	outSession->AddStreamL( audioStreamIn );
	CleanupStack::Pop(audioStreamIn);
	// 
	// TODO: Currently MMComms supports only the first codec of the stream
	//
	const RPointerArray<CMceAudioCodec>& codecs =  audioStreamIn->Codecs();
	
	for( int i = 0; i < codecs.Count(); i++ )
	    {
	    CMceAudioCodec* codec = codecs[i];
	    
	    // Search for AMR codec and delete others
		if( codec->SdpName() != KMceSDPNameAMR )
	        {
	    	audioStreamIn->RemoveCodecL( *codec );
	    	--i;
	    	}
	    }

    if( codecs.Count() != 1 )
        {
        User::Leave( KErrArgument );
        }
        
	CMceAmrCodec* amrcodec = ( CMceAmrCodec* ) codecs[0];
	
	const RPointerArray<CMceAudioCodec>& codecsOut =  audioStreamOut->Codecs();
	CMceAmrCodec* amrcodecOut = (CMceAmrCodec*)codecsOut[0];
	
	
	// set prioirity for uplink
	amrcodecOut->SetMMFPriorityL( iEngine.iPriorityUplink );
    amrcodecOut->SetMMFPriorityPreferenceL( iEngine.iPrefUplink );
    
	// set prioirity for downlink
	amrcodec->SetMMFPriorityL( iEngine.iPriorityDownlink );
    amrcodec->SetMMFPriorityPreferenceL( iEngine.iPrefDownlink );
	
	//
	// Set initial parameters for codec
	//
	// Allowed bitrates
	amrcodec->SetAllowedBitrates( AMRDefOfferAllowedBitrates );
	
	// Bitrate should be 5150
	amrcodec->SetBitrate( AMRDefOfferBitrate );

    // AMR narrow band, sampling rate 8000
	amrcodec->SetSamplingFreq( KMceAmrNbSamplingFreq );
	
	// Use bandwith effient coding
	amrcodec->SetCodecMode( AMRDefOfferCodingMode );
	amrcodecOut->SetCodecMode( AMRDefOfferCodingMode );
	
	// Default packet ratio is 8 AMR packets per one RTP packet
	// -> ptime = 8 * 20 = 160. Lower value is not good with GPRS bearer
	amrcodec->SetPTime(AMRDefOfferPTime );
	
	// Allowable maximum ptime, 400 should be OK
	amrcodec->SetMaxPTime( AMRDefOfferMaxPTime );
	
	// 106 seems to be common payload type
	amrcodec->SetPayloadType( 106 );
	

	// Disabled here due to audio server crash
//	CMceRtpSink* rtpOutSink = (CMceRtpSink*) audioStreamOut->Sinks()[0];
//	rtpOutSink->DisableL();
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::FillCustomSessionData	
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutSession::FillCustomSessionData(CMceOutSession* outSession)
	{
	
	// setmodifier for the session, by default preconditions are present, if no preconditions then setthis
	if( iSessionParams.iNoPrecondtion) 
		{
		outSession->SetModifierL(KMcePreconditions,KMcePreconditionsNotUsed);
		}
	outSession->SetServiceTypeL(iSessionParams.iServiceType);
	switch (iSessionParams.index )
	    {
	    case 0: 
	        iDirection = ESendRecv;
	        break;
	    case 1: 
	        iDirection = ESendOnly;
	        break;
	    case 2: 
	        iDirection = ERecvOnly;
	        break;
	    default:
	        iDirection = ENotSpesified;
	        break;
	    }
	    
	    CMceFileSource* fileSource = NULL;
	    CMceAudioStream* audioOutStream = NULL;
	    CMceAudioStream* audioOutStreamLocal = NULL;
	 	if ( iDirection != ENotSpesified )
	    {
	    // Create outstream (== uplink)

	    if ( iDirection == ESendOnly ||  
	         iDirection == ESendRecv )
	        {
	        audioOutStream = CMceAudioStream::NewLC();
	        CMceMicSource* mic = NULL;
	        
			if(iSessionParams.sourceType)
				{
				fileSource = CMceFileSource::NewL(iEngine.MCEManager(), iSessionParams.sourceFile );
            	CleanupStack::PushL( fileSource );
            	audioOutStream->SetSourceL(fileSource);
            	CleanupStack::Pop( fileSource );
				}
			else
				{
				mic = CMceMicSource::NewLC();
				audioOutStream->SetSourceL( mic );	
				CleanupStack::Pop( mic );
				}
			
			
			CMceRtpSink* rtpSink;
			CMceSpeakerSink* speakerSink;
			
			if(iSessionParams.audioSinkType)			
				{
				speakerSink = CMceSpeakerSink::NewLC();
				audioOutStream->AddSinkL( speakerSink );
				speakerSink->SetRoutingL( KMceAudioRoutePrivate );
				CleanupStack::Pop( speakerSink );
				}
			else
				{
				HBufC8* rtpSinkIdentity = KMCERtpSinkIdentity().AllocLC();
				if(iEngine.iRtcpFlag)
					{
					rtpSink = CMceRtpSink::NewL(ETrue, rtpSinkIdentity);	
					CleanupStack::PushL(rtpSink);
					}
				else
					{
					rtpSink = CMceRtpSink::NewL( EFalse, rtpSinkIdentity );
					CleanupStack::PushL(rtpSink);	
					}
				audioOutStream->AddSinkL( rtpSink );
				CleanupStack::Pop( rtpSink );
				CleanupStack::PopAndDestroy(rtpSinkIdentity);
				}
				
		    // Create stream for local playing of the audio
		    if ( fileSource && iEngine.iBundle )
				{
				audioOutStreamLocal = CMceAudioStream::NewLC();
            	audioOutStreamLocal->SetSourceL(fileSource);
            	speakerSink = 0;
            	speakerSink = CMceSpeakerSink::NewLC();	
				audioOutStreamLocal->AddSinkL( speakerSink );
				CleanupStack::Pop( speakerSink );
				}

			if( iEngine.iSdp )
			    {
			    CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(3);
			    array->AppendL(_L8("a=OutgoingStream1UL:20\r\n"));
                array->AppendL(_L8("a=OutgoingStream2UL:40\r\n"));
                array->AppendL(_L8("b=AS:64\r\n"));
			    audioOutStream->SetMediaAttributeLinesL( array );
			    
			    }    
	        }

	    // Create instream (== downlink)
	    CMceAudioStream* audioInStream = NULL;
	    
	    if ( iDirection == ERecvOnly ||
	         iDirection == ESendRecv )
	        {
			audioInStream = CMceAudioStream::NewLC();
			HBufC8* rtpSourceIdentity = KMCERtpSourceIdentity().AllocLC();
			CMceRtpSource* rtpSource = CMceRtpSource::NewL( iEngine.iJitterBufSize, 
                                	                         iEngine.iJitterBufTreshold, 
                                	                         iEngine.iJitterBufTimeout,
                                	                         rtpSourceIdentity);
			CleanupStack::PushL( rtpSource );
			audioInStream->SetSourceL( rtpSource );
			CleanupStack::Pop( rtpSource );
			CleanupStack::PopAndDestroy(rtpSourceIdentity);
			
			CMceSpeakerSink* speaker = CMceSpeakerSink::NewLC();
			speaker->SetRoutingL( KMceAudioRoutePrivate );	
			audioInStream->AddSinkL( speaker );
			CleanupStack::Pop( speaker );
			
			if( iEngine.iSdp )
			    {
			    CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(3);
			    array->AppendL(_L8("a=OutgoingStream1DL:20\r\n"));
                array->AppendL(_L8("a=BoundStream:40\r\n"));
                array->AppendL(_L8("b=AS:64\r\n"));	
			    audioInStream->SetMediaAttributeLinesL( array );
			    }    
			    
	        }
            
	    // Bound streams     
	    if ( iDirection == ESendRecv )
	        {
			audioOutStream->BindL( audioInStream ); 
			
			if ( audioInStream )
			    {
			    CleanupStack::Pop( audioInStream );     
			    }
	        }

	    // Add streams to OutSession    
	    if ( iDirection != ERecvOnly )
	        {
	        if ( audioOutStreamLocal )
	            {
	            outSession->AddStreamL( audioOutStreamLocal );
	            CleanupStack::Pop( audioOutStreamLocal );
	            
	            iEngine.SetMMFPriorityForCodecsL( audioOutStreamLocal->Codecs(),
                                      iEngine.iPriorityUplink, 
                                      iEngine.iPrefUplink );
	            }

	        outSession->AddStreamL( audioOutStream );
	        CleanupStack::Pop( audioOutStream );

                
            iEngine.SetMMFPriorityForCodecsL( audioOutStream->Codecs(),
                                      iEngine.iPriorityUplink, 
                                      iEngine.iPrefUplink );
            
            if ( audioInStream )
                {  
                iEngine.SetMMFPriorityForCodecsL( audioInStream->Codecs(),
                                          iEngine.iPriorityDownlink, 
                                          iEngine.iPrefDownlink );
                }
	        }
	    else
	        {
	        outSession->AddStreamL( audioInStream );
	        CleanupStack::Pop( audioInStream );
	        
	        iEngine.SetMMFPriorityForCodecsL( audioInStream->Codecs(),
                                      iEngine.iPriorityDownlink, 
                                      iEngine.iPrefDownlink );
	        }    
	    
	    }
	  switch (iSessionParams.videoIndex )
	    {
	    case 0: 
	        iVideoDirection = EVideoSendRecv;
	        break;
	    case 1: 
	        iVideoDirection = EVideoSendOnly;
	        break;
	    case 2: 
	        iVideoDirection = EVideoRecvOnly;
	        break;
	    default:
	        iVideoDirection = EVideoNotSpesified;
	        break;
	    }
	    
	    CMceVideoStream* videoOutStreamRTP = NULL;
	    CMceVideoStream* videoOutStreamDisplay = NULL;
	    CMceVideoStream* videoOutStreamFile = NULL;
	    CMceVideoStream* vidStreamLocal = NULL;
	    CMceCameraSource* cam =NULL;
	    CMceFileSource* videoFileSource = NULL;
	    
	 	if ( iVideoDirection != EVideoNotSpesified )
	    {
	    // Create outstream (== uplink)
	    

	    if ( iVideoDirection == EVideoSendOnly ||  
	         iVideoDirection == EVideoSendRecv )
	        {
	        // UL stream present
			CMceRtpSink* rtpSink = NULL;
			CMceDisplaySink* dispSink = NULL;
			CMceFileSink* fileSink = NULL;
			
			if(iSessionParams.videoSinkDisplay)
				{
				dispSink = CMceDisplaySink::NewLC( iEngine.MCEManager());
			//	TRect viewFinder(0, 83, 352, 378);
				TRect viewFinder(32, 88, 208, 232);
			//    TRect viewFinder(iEngine.GetRect());
	        	dispSink->SetDisplayRectL( viewFinder );
				videoOutStreamDisplay = CMceVideoStream::NewL();
				CleanupStack::Pop( dispSink );
				CleanupStack::PushL( videoOutStreamDisplay );
				videoOutStreamDisplay->AddSinkL( dispSink );
				if(iSessionParams.sourceType)
					{
					//check for filesource if it is already there --- start
					if(fileSource)
						{
						videoOutStreamDisplay->SetSourceL( fileSource );	
						}
					else
						{
						fileSource = CMceFileSource::NewL(iEngine.MCEManager(), iSessionParams.sourceFile );
		            	CleanupStack::PushL( fileSource );
		            	videoOutStreamDisplay->SetSourceL(fileSource);
		            	CleanupStack::Pop( fileSource );
						}
					}
				else
					{
					cam = CMceCameraSource::NewL(iEngine.MCEManager());
					CleanupStack::PushL( cam  );
					videoOutStreamDisplay->SetSourceL( cam );
					CleanupStack::Pop( cam );	
					}
			
				outSession->AddStreamL( videoOutStreamDisplay );
				CleanupStack::Pop( videoOutStreamDisplay );
				
				iEngine.SetMMFPriorityForCodecsL( videoOutStreamDisplay->Codecs(),
                                                  iEngine.iPriorityUplink, 
                                                  iEngine.iPrefUplink );
				}
			
			if(iSessionParams.videoSinkFile)
				{
				fileSink = CMceFileSink::NewLC( iSessionParams.sinkFile);
				videoOutStreamFile = CMceVideoStream::NewL();
				CleanupStack::Pop( fileSink );
				CleanupStack::PushL( videoOutStreamFile );
				videoOutStreamFile->AddSinkL( fileSink );
				if(iSessionParams.sourceType)
					{
					//check for filesource if it is already there --- start
					if(fileSource)
						{
						videoOutStreamFile->SetSourceL( fileSource );	
						}
					else
						{
						fileSource = CMceFileSource::NewL(iEngine.MCEManager(), iSessionParams.sourceFile );
		            	CleanupStack::PushL( fileSource );
		            	videoOutStreamFile->SetSourceL(fileSource);
		            	CleanupStack::Pop( fileSource );
						}
					}
				else
					{
					// need to check for camera ?????
					if(cam)
						{
						videoOutStreamFile->SetSourceL( cam );	
						}
					else
						{
						cam = CMceCameraSource::NewL(iEngine.MCEManager());
						CleanupStack::PushL( cam  );
						videoOutStreamFile->SetSourceL( cam );
						CleanupStack::Pop( cam );
						}
					
					}
				CleanupStack::Pop( videoOutStreamFile );
				outSession->AddStreamL( videoOutStreamFile );
				
                AdjustVideoCodecsL( videoOutStreamFile->Codecs() );
				}
			if( iSessionParams.videoSinkRtp )
				{
				
				videoOutStreamRTP = CMceVideoStream::NewL();
				CleanupStack::PushL( videoOutStreamRTP );
				
				HBufC8* rtpSinkIdentity = KMCERtpSinkIdentity().AllocLC();
				if(iEngine.iRtcpFlag)
					{
					rtpSink = CMceRtpSink::NewL(ETrue,rtpSinkIdentity );	
					CleanupStack::PushL( rtpSink );
					}
				else
					{
					rtpSink = CMceRtpSink::NewL(EFalse, rtpSinkIdentity);
					CleanupStack::PushL( rtpSink );	
					}
				
				videoOutStreamRTP->AddSinkL( rtpSink );
				CleanupStack::Pop( rtpSink );
				CleanupStack::PopAndDestroy(rtpSinkIdentity);
				if(iSessionParams.sourceType)
					{
					//check for filesource if it is already there --- start
					if(fileSource)
						{
						videoOutStreamRTP->SetSourceL( fileSource );	
						}
					else
						{
						fileSource = CMceFileSource::NewL(iEngine.MCEManager(), iSessionParams.sourceFile );
		            	CleanupStack::PushL( fileSource );
		            	videoOutStreamRTP->SetSourceL(fileSource);
		            	CleanupStack::Pop( fileSource );
						}
					}
				else
					{
					// need to check for camera ?????
					if(cam)
						{
						videoOutStreamRTP->SetSourceL( cam );	
						}
					else
						{
						cam = CMceCameraSource::NewL(iEngine.MCEManager());
						CleanupStack::PushL( cam  );
						videoOutStreamRTP->SetSourceL( cam );
						CleanupStack::Pop( cam );
						}
					
					}
				}
			
		     }   // uplink over

	    // Create instream (== downlink)
	    CMceVideoStream* videoInStreamRTP = NULL;
	    
	    if ( iVideoDirection == EVideoRecvOnly ||
	         iVideoDirection == EVideoSendRecv )
	        {
			videoInStreamRTP = CMceVideoStream::NewLC();
			HBufC8* rtpSourceIdentity = KMCERtpSourceIdentity().AllocLC();
			
			TInt jitbufSize( iEngine.iJitterBufSize );
            TInt jitbufThreshold( iEngine.iJitterBufTreshold );
            TInt jitbufTimeout( iEngine.iJitterBufTimeout );
            iEngine.GetJitterBufferValues( 
        	    videoInStreamRTP, jitbufSize, jitbufThreshold, jitbufTimeout, ETrue );
    	    
			CMceRtpSource* rtpSource = CMceRtpSource::NewLC( jitbufSize, 
                                	                         jitbufThreshold, 
                                	                         jitbufTimeout,
                                	                         rtpSourceIdentity);
			videoInStreamRTP->SetSourceL( rtpSource );
			CleanupStack::Pop( rtpSource );
			CleanupStack::PopAndDestroy(rtpSourceIdentity);
			CMceDisplaySink* display = CMceDisplaySink::NewLC(iEngine.MCEManager());	
			videoInStreamRTP->AddSinkL( display );
			//	
			if( iEngine.iSdp )
			    {
			    CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(1);
                array->AppendL(_L8("b=AS:64\r\n"));
			    videoInStreamRTP->SetMediaAttributeLinesL( array );
			    }  	
					
			//	
			// TRect disp(32, 88, 208, 232);
			TRect disp(iEngine.GetRect());
		   	display->SetDisplayRectL( disp );
        //	display->DisableL();
			CleanupStack::Pop( display );
	        }  
	        
	     // Bound streams     
	    if ( iVideoDirection == ESendRecv && 
	         videoOutStreamRTP  )
	        {
			videoOutStreamRTP->BindL( videoInStreamRTP ); 
			
			if ( videoInStreamRTP )
			    {
			    CleanupStack::Pop( videoInStreamRTP );     
			    }
			CleanupStack::Pop( videoOutStreamRTP );
			outSession->AddStreamL( videoOutStreamRTP );
			
			AdjustVideoCodecsL( videoOutStreamRTP->Codecs() );
	        }
	    
	    else // no bound streams present, either UL or DL 
	        {
	        if( iVideoDirection == EVideoRecvOnly ) // DL present
	            {
	            CleanupStack::Pop( videoInStreamRTP );
	            outSession->AddStreamL( videoInStreamRTP );
	            }
	        else if ( videoOutStreamRTP ) // RTP UL if present
	            {
	            CleanupStack::Pop( videoOutStreamRTP );
			    outSession->AddStreamL( videoOutStreamRTP );
			    
			    AdjustVideoCodecsL( videoOutStreamRTP->Codecs() );	 
			    
			    iEngine.SetMMFPriorityForCodecsL( videoOutStreamRTP->Codecs(),
                                                  iEngine.iPriorityUplink, 
                                                  iEngine.iPrefUplink );
	            }
	        }
	    }
	if( iEngine.iBundle /*iSessionParams.iBundle*/ ) 
	    {
	    if( audioOutStream && videoOutStreamDisplay && 
	        iSessionParams.sourceType  )
	        {
	        // audio and video from file present, bundle
	        CMceStreamBundle* bundle = CMceStreamBundle::NewLC( CMceStreamBundle::ELS );
	       // bundle->AddStreamL( *audioOutStream );
	        bundle->AddStreamL( *videoOutStreamDisplay );
	        if ( audioOutStreamLocal )
	            {
	            bundle->AddStreamL( *audioOutStreamLocal );
	            }
	        CleanupStack::Pop( bundle );
	        outSession->AddBundleL( bundle );
	        }
	    }
	     	    	
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::Fill2WayVideoSessionDataL	
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineOutSession::Fill2WayVideoSessionDataL(CMceOutSession* aOutSession)
    {
    // Video
    //
    
    aOutSession->SetModifierL( KMcePreconditions, KMcePreconditionsNotUsed );
    
    // Create video uplink
    //
    CMceVideoStream* videoOutStream = CMceVideoStream::NewL();
    CleanupStack::PushL( videoOutStream );
    
    CMceRtpSink* rtpSink = CMceRtpSink::NewL( ETrue );
    CleanupStack::PushL( rtpSink );	
	videoOutStream->AddSinkL( rtpSink );
	CleanupStack::Pop( rtpSink );
	
    CMceCameraSource* cam = CMceCameraSource::NewL( iEngine.MCEManager() );
	CleanupStack::PushL( cam  );
	
	if ( iSessionParams.iCameraSetting == TSessionParams::EMceCameraSecondary )
        {
        if ( cam->CamerasAvailable() > 1 )
            {
            cam->SetCameraIndexL( 1 );
            }
        }
	
	videoOutStream->SetSourceL( cam );
	CleanupStack::Pop( cam );	

    CMceVideoStream* videoLocalStream = NULL;
    if ( iSessionParams.videoSinkDisplay == TSessionParams::EMceViewFinderEnabled )
        {
        videoLocalStream = CMceVideoStream::NewL();
        CleanupStack::PushL( videoLocalStream );
        CMceDisplaySink* localDispSink = CMceDisplaySink::NewLC( iEngine.MCEManager() );
    
        TRect localDisplayRect(0, 0, 36, 44);
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EStatusPane, localDisplayRect );
        localDispSink->SetDisplayRectL( localDisplayRect );
        localDispSink->SetPriorityL( ECoeWinPriorityAlwaysAtFront );
    	videoLocalStream->AddSinkL( localDispSink );
    	CleanupStack::Pop( localDispSink );
    	
    	videoLocalStream->SetSourceL( cam );
    	
    	aOutSession->AddStreamL( videoLocalStream );
    	CleanupStack::Pop( videoLocalStream );
        }
        
    // Create video downlink
    //
    CMceVideoStream* videoInStream = CMceVideoStream::NewL();
    CleanupStack::PushL( videoInStream );
    
    CMceDisplaySink* dispSink = CMceDisplaySink::NewLC( iEngine.MCEManager());
    
    TRect displayRect(32, 88, 208, 232);
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, displayRect );
    dispSink->SetDisplayRectL( displayRect );
	videoInStream->AddSinkL( dispSink );
	CleanupStack::Pop( dispSink );
	
	TInt jitbufSize( iEngine.iJitterBufSize );
    TInt jitbufThreshold( iEngine.iJitterBufTreshold );
    TInt jitbufTimeout( iEngine.iJitterBufTimeout );
	
	iEngine.GetJitterBufferValues( 
    	    videoInStream, jitbufSize, jitbufThreshold, jitbufTimeout, ETrue );
    	    
   	CMceRtpSource* rtpSource = CMceRtpSource::NewLC( jitbufSize, 
   	                                                 jitbufThreshold, 
   	                                                 jitbufTimeout );
   	videoInStream->SetSourceL( rtpSource );
	CleanupStack::Pop( rtpSource );	
                                	                         
    // Modify codecs and make sendrecv session
    //
    videoOutStream->BindL( videoInStream ); 
    CleanupStack::Pop( videoInStream );
    
    aOutSession->AddStreamL( videoOutStream );
    CleanupStack::Pop( videoOutStream );
    
    if ( iSessionParams.iVideoCodec == TSessionParams::EMceCodecSingle ||
         iSessionParams.iVideoCodec == TSessionParams::EMceCodecMultipleModes )
        {
        // Remove all other codecs than first one
        //
        const RPointerArray<CMceVideoCodec>& videoCodecs = 
            videoOutStream->Codecs();
        
        for ( TInt i = videoCodecs.Count() - 1; i > 0; i-- )
            {
            videoOutStream->RemoveCodecL( *videoCodecs[ i ] );
            }
            
        if ( videoLocalStream )
            {
            const RPointerArray<CMceVideoCodec>& videoCodecs2 = 
                videoLocalStream->Codecs();
        
            for ( TInt i = videoCodecs2.Count() - 1; i > 0; i-- )
                {
                videoLocalStream->RemoveCodecL( *videoCodecs2[ i ] );
                }
            }
        } 
    
    if ( iSessionParams.iVideoCodec == TSessionParams::EMceCodecMultipleModes )
        {
        // Currently make only multiple modes offer for AVC
        CMceVideoCodec* defaultAvcCodec = NULL;
        const RPointerArray<CMceVideoCodec>& videoCodecs = 
            videoOutStream->Codecs();
        for ( TInt i = 0; i < videoCodecs.Count(); i++ )
            {
            if ( videoCodecs[ i ]->SdpName().CompareF( KMceSDPNameH264 ) == 0 )
                {
                defaultAvcCodec = videoCodecs[ i ];
                }
            }
        if ( defaultAvcCodec )
            {
            CMceVideoCodec* otherModeAvcCodec = defaultAvcCodec->CloneL();
            CleanupStack::PushL( otherModeAvcCodec );
            TInt codecMode = defaultAvcCodec->CodecMode();
            TInt payloadType = defaultAvcCodec->PayloadType();
            payloadType++;
            if ( codecMode == KMceAvcModeSingleNal )
                {
                codecMode = KMceAvcModeNonInterleaved;
                }
            else
                {
                codecMode = KMceAvcModeSingleNal;
                }
            User::LeaveIfError( otherModeAvcCodec->SetCodecMode( codecMode ) );
            User::LeaveIfError( otherModeAvcCodec->SetPayloadType( payloadType ) );
            
            videoOutStream->AddCodecL( otherModeAvcCodec );
            CleanupStack::Pop( otherModeAvcCodec );
            
            CMceVideoCodec* otherModeAvcCodec2 = otherModeAvcCodec->CloneL();
            CleanupStack::PushL( otherModeAvcCodec2 );
            videoInStream->AddCodecL( otherModeAvcCodec2 );
            CleanupStack::Pop( otherModeAvcCodec2 );
            
            if ( videoLocalStream )
                {
                CMceVideoCodec* otherModeAvcCodec3 = otherModeAvcCodec->CloneL();
                CleanupStack::PushL( otherModeAvcCodec3 );
                videoLocalStream->AddCodecL( otherModeAvcCodec3 );
                CleanupStack::Pop( otherModeAvcCodec3 ); 
                }
            }
        }
        
    AdjustVideoCodecsL( videoOutStream->Codecs() );   
    
    // Audio
    //
    if ( iSessionParams.iAudioCodec != TSessionParams::EMceCodecNone )
        {
        // Create audio uplink
        //
      	CMceAudioStream* audioStreamOut = CMceAudioStream::NewLC();
    	CMceRtpSink* rtpSink = CMceRtpSink::NewL( ETrue );
    	audioStreamOut->AddSinkL( rtpSink );
    	
    	CMceMicSource* micSource = CMceMicSource::NewLC();
    	audioStreamOut->SetSourceL( micSource );
    	CleanupStack::Pop();	
        
        // Create audio downlink
        //
        CMceAudioStream* audioStreamIn = CMceAudioStream::NewLC();
        
        TInt jitbufSize2( iEngine.iJitterBufSize );
        TInt jitbufThreshold2( iEngine.iJitterBufTreshold );
        TInt jitbufTimeout2( iEngine.iJitterBufTimeout );
        iEngine.GetJitterBufferValues( 
    	    audioStreamIn, jitbufSize2, jitbufThreshold2, jitbufTimeout2, ETrue );
    	    
    	CMceRtpSource* rtpSource = CMceRtpSource::NewLC( jitbufSize2, 
   	                                                     jitbufThreshold2, 
   	                                                     jitbufTimeout2 );
    	audioStreamIn->SetSourceL( rtpSource );
    	CleanupStack::Pop();
    	
    	CMceSpeakerSink* speakerSink = CMceSpeakerSink::NewLC();
    	speakerSink->SetRoutingL( KMceAudioRoutePrivate );	
    	audioStreamIn->AddSinkL( speakerSink );
    	CleanupStack::Pop();	
        
        // Make them sendrecv
        //
        audioStreamOut->BindL( audioStreamIn ); 
        CleanupStack::Pop( audioStreamIn );
        
        aOutSession->AddStreamL( audioStreamOut );
        CleanupStack::Pop( audioStreamOut );
        
        iEngine.SetMMFPriorityForCodecsL( audioStreamOut->Codecs(),
                                          iEngine.iPriorityUplink, 
                                          iEngine.iPrefUplink );
        
        iEngine.SetMMFPriorityForCodecsL( audioStreamIn->Codecs(),
                                          iEngine.iPriorityDownlink, 
                                          iEngine.iPrefDownlink );
                                                                        
        if ( iSessionParams.iAudioCodec == TSessionParams::EMceCodecSingle )
            {
            const TDesC8& singleCodec = KMceSDPNamePCMU();
            const RPointerArray<CMceAudioCodec>& audioCodecs = audioStreamOut->Codecs();
            for( int i = 0; i < audioCodecs.Count(); i++ )
        	    {
        	    CMceAudioCodec* codec = audioCodecs[i];
        	    
        	    // Search for AMR codec and delete others
        	    if ( codec->SdpName().CompareF( singleCodec ) != 0 )
        	        {
        	    	audioStreamOut->RemoveCodecL( *codec );
        	    	--i;
        	    	}
        	    }
        	    
            __ASSERT_ALWAYS( audioCodecs.Count() == 1, User::Leave( KErrArgument ) );
            }
        }
    
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::GetVideoCodec	
// -----------------------------------------------------------------------------
//	
CMceVideoCodec* CMCETestUIEngineOutSession::GetVideoCodec( 
    CMceVideoStream& aStream, const TDesC8& aSdpName )
    {
    const RPointerArray<CMceVideoCodec>& videoCodecs = aStream.Codecs();
    for ( TInt i = 0; i < videoCodecs.Count(); i++ )
        {
        if ( videoCodecs[ i ]->SdpName().CompareF( aSdpName ) == 0 )
            {
            return videoCodecs[ i ];
            }
        }
    return NULL;
    }

