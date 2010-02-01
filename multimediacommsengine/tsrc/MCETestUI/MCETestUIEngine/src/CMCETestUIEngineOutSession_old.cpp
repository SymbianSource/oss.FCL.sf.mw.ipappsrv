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

#include <MCEManager.h>
#include <MCEOutSession.h>
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

const TUint KAudioPriorityPocAudioUplink                  = 95;
const TUint KAudioPriorityPocAudioDownlink                = 95;
const TUint KAudioPrefPocAudioUplink                      = 0x05130001;
const TUint KAudioPrefPocAudioDownlink                    = 0x05120001;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineOutSession::NewL()
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutSession* CMCETestUIEngineOutSession::NewL(
            CMCETestUIEngine& aEngine,
            CMCETestUIEngineProfile& aProfile,
            CMCETestUIEngineOutSession::TDirection aDirection,
            CMCETestUIEngineOutSession::TVideoDirection aVideoDirection,
            CMCETestUIEngineOutSession::TSessionType aSessionType ) 
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
// CMCETestUIEngineOutSession::CMCETestUIEngineOutSession
// -----------------------------------------------------------------------------
//
CMCETestUIEngineOutSession::CMCETestUIEngineOutSession( 
                CMCETestUIEngine& aEngine,
                CMCETestUIEngineProfile& aProfile,
                CMCETestUIEngineOutSession::TDirection aDirection,
                CMCETestUIEngineOutSession::TVideoDirection aVideoDirection,
                CMCETestUIEngineOutSession::TSessionType aSessionType )
    : CMCETestUIEngineSession( aEngine, aDirection,aVideoDirection ),
      iProfile( &aProfile ),
      iId( 0 ),
      iSessionType(aSessionType)
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
    if(SessionParamsL(iSessionParams,iSessionType))
    	{
    	 // Solve recipient
	   CMCETestUIQuestionDataQuery* question = 
		    CMCETestUIQuestionDataQuery::NewLC();
		    
		question->SetCaptionL( KUserQuestionInputRecipient );
		question->SetDefaultValueL(iSessionParams.iRecipentAddress  );
		question->SetAnswerL( iSessionParams.iRecipentAddress  );
		TBufC8<100> recipient( question->Answer8() );
		
		CleanupStack::PopAndDestroy( question );
	  
		//////
		// solve originator
	   
	   if ( iSessionParams.originator.Compare(KNone))
	   	{
		   	HBufC8* orig8 = HBufC8::NewL(iSessionParams.originator.Length());
		   	TPtr8 ptr = orig8->Des();
		    ptr.Copy(iSessionParams.originator);
		    // Create Outsession
		    CMceOutSession* outSession = CMceOutSession::NewL(
	        iEngine.MCEManager(), 
	        iProfile->Profile(),
	        recipient ,orig8);
	    	CleanupStack::PushL(outSession);
	    	
	    	    	
	        if(iSessionType == EPoCSession)
				{
			 	FillPoCSessionData(outSession);
				}
			else if (iSessionType == ECustomSession)
				{
				FillCustomSessionData(outSession);
				}
		  
			CleanupStack::Pop( outSession );
			if(iEngine.IsFCSession()) 
				{
				TRAP_IGNORE(iEngine.FloorControl().InitializeL(
													*outSession,0,iEngine.MMObserver()));
				CDesC8Array* array = NULL;
				if(iSessionParams.iPoCAdhocSessionType)	
					{
					array = new (ELeave) CDesC8ArrayFlat(1);	
				    array->AppendL(_L8("a=fmtp:TBCP queuing=0; tb_priority=1; timestamp=0; tb_granted=0; poc_sess_priority=0; poc_lock=0\r\n"));
				    iEngine.FloorControl().UpdateL(*outSession, array);	
					}
				else
					{
					iEngine.FloorControl().UpdateL(*outSession, array);
					}
				}
			iSession = outSession;
			iOutSession = outSession;     
	   	}
	   else 
	   	{
		    // Create Outsession
		    CMceOutSession* outSession = CMceOutSession::NewL(
	        iEngine.MCEManager(), 
	        iProfile->Profile(),
	        recipient );
	    	CleanupStack::PushL(outSession);
	    
		    if(iSessionType == EPoCSession)
				{
			 	FillPoCSessionData(outSession);
				}
			else if (iSessionType == ECustomSession)
				{
				FillCustomSessionData(outSession);
				}
		  
			CleanupStack::Pop( outSession );
			if(iEngine.IsFCSession()) 
				{
				TRAP_IGNORE(iEngine.FloorControl().InitializeL(
												*outSession,0,iEngine.MMObserver()));
				CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(1);	
			    array->AppendL(_L8("a=fmtp:TBCP queuing=0; tb_priority=1; timestamp=0; tb_granted=0; poc_sess_priority=0; poc_lock=0\r\n"));
			    iEngine.FloorControl().UpdateL(*outSession, array);	
				}
			iSession = outSession;
			iOutSession = outSession; 
	   	}
		
		    // Get identity
	    iId = iEngine.NextOutSessionId();
    	}
   
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
		    break;
		case CMceSession::EOffering: 
		    iCommands.Append( new (ELeave) TMCETestUIEngineCmdCancelSession( 
                                iEngine, *this ) );
            iCommands.Append( new (ELeave) TMCETestUIEngineCmdFCSendSession( 
                                iEngine, *this ) );
			iCommands.Append( new (ELeave) 
		        TMCETestUIEngineCmdDeleteSession( iEngine, *this ) );                              
		    break;
		case CMceSession::EEstablished:
			if(VideoDirection() != CMCETestUIEngineSession::EVideoNotSpesified)    
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
									CMCETestUIEngineOutSession::TSessionType aSessionType)
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
// CMCETestUIEngineOutSession::FillPoCSessionData
// -----------------------------------------------------------------------------
//

void CMCETestUIEngineOutSession::FillPoCSessionData(CMceOutSession* outSession)
	{
	iDirection = CMCETestUIEngineOutSession::ESendRecv;
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
	CMceRtpSource* rtpSource = CMceRtpSource::NewLC( 10, 5, 1000 );
	audioStreamIn->SetSourceL(rtpSource);
	CleanupStack::Pop();	
	
	CMceAudioStream* audioStreamOut = CMceAudioStream::NewLC();
	
	CMceRtpSink* rtpSink = CMceRtpSink::NewL( ETrue );
	audioStreamOut->AddSinkL(rtpSink);
	
	CMceMicSource* micSource = CMceMicSource::NewLC();
	audioStreamOut->SetSourceL(micSource);
	CleanupStack::Pop();	
	
	audioStreamIn->BindL(audioStreamOut);
	CleanupStack::Pop(audioStreamOut);
	// floor control

	outSession->AddStreamL( audioStreamIn );
	CleanupStack::Pop(audioStreamIn);
	// 
	// TODO: Currently MMComms supports only the first codec of the stream
	//
	const RPointerArray<CMceAudioCodec>& coddecs =  audioStreamIn->Codecs();
	CMceAmrCodec* amrcodec = (CMceAmrCodec*)coddecs[0];
	
	const RPointerArray<CMceAudioCodec>& coddecsOut =  audioStreamOut->Codecs();
	CMceAmrCodec* amrcodecOut = (CMceAmrCodec*)coddecsOut[0];
	
	
	// set prioirity for uplink
	amrcodecOut->SetMMFPriorityL( KAudioPriorityPocAudioUplink );
    amrcodecOut->SetMMFPriorityPreferenceL( KAudioPrefPocAudioUplink );
    
	// set prioirity for downlink
	amrcodec->SetMMFPriorityL( KAudioPriorityPocAudioDownlink );
    amrcodec->SetMMFPriorityPreferenceL( KAudioPrefPocAudioDownlink );
	
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
	
	// Default packet ratio is 8 AMR packets per one RTP packet
	// -> ptime = 8 * 20 = 160. Lower value is not good with GPRS bearer
	amrcodec->SetPTime(AMRDefOfferPTime );
	
	// Allowable maximum ptime, 400 should be OK
	amrcodec->SetMaxPTime( AMRDefOfferMaxPTime );
	
	// 106 seems to be common payload type
	amrcodec->SetPayloadType( 106 );
	

	// Disabled here due to audio server crash
	CMceRtpSink* rtpOutSink = (CMceRtpSink*) audioStreamOut->Sinks()[0];
	rtpOutSink->DisableL();
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
		outSession->SetModifierL(0,0);
		}
	
	switch (iSessionParams.index )
	    {
	    case 0: 
	        iDirection = CMCETestUIEngineOutSession::ESendRecv;
	        break;
	    case 1: 
	        iDirection = CMCETestUIEngineOutSession::ESendOnly;
	        break;
	    case 2: 
	        iDirection = CMCETestUIEngineOutSession::ERecvOnly;
	        break;
	    default:
	        iDirection = CMCETestUIEngineOutSession::ENotSpesified;
	        break;
	    }
	    
	    CMceFileSource* fileSource = NULL;
	 	if ( iDirection != CMCETestUIEngineOutSession::ENotSpesified )
	    {
	    // Create outstream (== uplink)
	    CMceAudioStream* audioOutStream = NULL;
	    
	    if ( iDirection == CMCETestUIEngineOutSession::ESendOnly ||  
	         iDirection == CMCETestUIEngineOutSession::ESendRecv )
	        {
	        audioOutStream = CMceAudioStream::NewLC();
	        CMceMicSource* mic = NULL;
	        
			if(iSessionParams.sourceType)
				{
				fileSource = CMceFileSource::NewL(iEngine.MCEManager(), iSessionParams.sourceFile );
            	CleanupStack::PushL( fileSource );
            	audioOutStream->SetSourceL(fileSource);
            	CleanupStack::Pop();
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
				CleanupStack::Pop( speakerSink );
				audioOutStream->AddSinkL( speakerSink );
				}
			else
				{
				if(iSessionParams.iSuppressRTCP)
					{
					rtpSink = CMceRtpSink::NewL(ETrue);	
					CleanupStack::PushL(rtpSink);
					}
				else
					{
					rtpSink = CMceRtpSink::NewLC();	
					}
				CleanupStack::Pop( rtpSink );
				audioOutStream->AddSinkL( rtpSink );
				 
				}
		
			
			       
	        }

	    // Create instream (== downlink)
	    CMceAudioStream* audioInStream = NULL;
	    
	    if ( iDirection == CMCETestUIEngineOutSession::ERecvOnly ||
	         iDirection == CMCETestUIEngineOutSession::ESendRecv )
	        {
			audioInStream = CMceAudioStream::NewLC();
			
			CMceRtpSource* rtpSource = CMceRtpSource::NewLC(KMceJitterBufferLength,KMceJitterBufferTreshold,
															20000,0);
			audioInStream->SetSourceL( rtpSource );
			CleanupStack::Pop( rtpSource );
			
			CMceSpeakerSink* speaker = CMceSpeakerSink::NewLC();	
			audioInStream->AddSinkL( speaker );
			CleanupStack::Pop( speaker );
	        }
            
	    // Bound streams     
	    if ( iDirection == CMCETestUIEngineOutSession::ESendRecv )
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
	        outSession->AddStreamL( audioOutStream );
	        CleanupStack::Pop( audioOutStream );
	        
	        const RPointerArray<CMceAudioCodec>& codecs =  audioOutStream->Codecs();
	        CMceAmrCodec* amrcodec = (CMceAmrCodec*)codecs[0];
	     
	        // set prioirity for uplink
	        amrcodec->SetMMFPriorityL( KAudioPriorityPocAudioUplink );
            amrcodec->SetMMFPriorityPreferenceL( KAudioPrefPocAudioUplink );
            
            if( audioInStream )
                {
                const RPointerArray<CMceAudioCodec>& codecs =  audioInStream->Codecs();
    	        CMceAmrCodec* amrcodec = (CMceAmrCodec*)codecs[0];  
    	        
    	       	// set prioirity for downlink
    	        amrcodec->SetMMFPriorityL( KAudioPriorityPocAudioDownlink );
                amrcodec->SetMMFPriorityPreferenceL( KAudioPrefPocAudioDownlink );    
                }
	        }
	    else
	        {
	        outSession->AddStreamL( audioInStream );
	        CleanupStack::Pop( audioInStream );
	        
	        const RPointerArray<CMceAudioCodec>& codecs =  audioInStream->Codecs();
	        CMceAmrCodec* amrcodec = (CMceAmrCodec*)codecs[0];  
	        
	       	// set prioirity for downlink
	        amrcodec->SetMMFPriorityL( KAudioPriorityPocAudioDownlink );
            amrcodec->SetMMFPriorityPreferenceL( KAudioPrefPocAudioDownlink ); 
	        }    
	    
	    } 
	  switch (iSessionParams.videoIndex )
	    {
	    case 0: 
	        iVideoDirection = CMCETestUIEngineOutSession::EVideoSendRecv;
	        break;
	    case 1: 
	        iVideoDirection = CMCETestUIEngineOutSession::EVideoSendOnly;
	        break;
	    case 2: 
	        iVideoDirection = CMCETestUIEngineOutSession::EVideoRecvOnly;
	        break;
	    default:
	        iVideoDirection = CMCETestUIEngineOutSession::EVideoNotSpesified;
	        break;
	    }
	 	if ( iVideoDirection != CMCETestUIEngineOutSession::EVideoNotSpesified )
	    {
	    // Create outstream (== uplink)
	    CMceVideoStream* videoOutStreamRTP = NULL;
	    CMceVideoStream* videoOutStreamDisplay = NULL;
	    CMceVideoStream* videoOutStreamFile = NULL;
	    CMceVideoStream* vidStreamLocal = NULL;
	    CMceCameraSource* cam =NULL;
	    CMceFileSource* videoFileSource = NULL;
	    if ( iVideoDirection == CMCETestUIEngineOutSession::EVideoSendOnly ||  
	         iVideoDirection == CMCETestUIEngineOutSession::EVideoSendRecv )
	        {
			CMceRtpSink* rtpSink = NULL;
			CMceDisplaySink* dispSink = NULL;
			CMceFileSink* fileSink = NULL;
			
			if(iSessionParams.videoSinkDisplay)
				{
				dispSink = CMceDisplaySink::NewLC( iEngine.MCEManager());
				videoOutStreamDisplay = CMceVideoStream::NewL();
				CleanupStack::Pop( dispSink );
				CleanupStack::PushL( videoOutStreamDisplay );
				videoOutStreamDisplay->AddSinkL( dispSink );
				if(iSessionParams.sourceType)
					{
					videoOutStreamDisplay->SetSourceL( fileSource );
					}
				else
					{
					cam = CMceCameraSource::NewL(iEngine.MCEManager());
					videoOutStreamDisplay->SetSourceL( cam );	
					}
				CleanupStack::Pop( videoOutStreamDisplay );
				outSession->AddStreamL( videoOutStreamDisplay );
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
					videoOutStreamFile->SetSourceL( fileSource );
					}
				else
					{
					cam = CMceCameraSource::NewL(iEngine.MCEManager());
					videoOutStreamFile->SetSourceL( cam );	
					}
				CleanupStack::Pop( videoOutStreamFile );
				outSession->AddStreamL( videoOutStreamFile );
				}
			if( iSessionParams.videoSinkRtp ||
			    iVideoDirection == CMCETestUIEngineOutSession::EVideoSendRecv )
				{
				if(iSessionParams.iSuppressRTCP)
					{
					rtpSink = CMceRtpSink::NewLC(ETrue);	
					}
				else
					{
					rtpSink = CMceRtpSink::NewLC();	
					}
				videoOutStreamRTP = CMceVideoStream::NewL();
				CleanupStack::Pop( rtpSink );
				CleanupStack::PushL( videoOutStreamRTP );
				videoOutStreamRTP->AddSinkL( rtpSink );
				if(iSessionParams.sourceType)
					{
					videoOutStreamRTP->SetSourceL( fileSource );
					}
				else
					{
					cam = CMceCameraSource::NewL(iEngine.MCEManager());
					videoOutStreamRTP->SetSourceL( cam );	
					}
				}
	
		
			
			if(iSessionParams.videoFileSourceType)
				{
	/*			vidStreamLocal = CMceVideoStream::NewLC();
	        	CMceDisplaySink* dispSink = CMceDisplaySink::NewL( iEngine.MCEManager());
	        	CleanupStack::PushL( dispSink );
	        	
	   
	        	TRect viewFinder(0, 83, 352, 378);
	        	dispSink->SetDisplayRectL( viewFinder );
	   //  		dispSink->DisableL();
	        	    
	        	vidStreamLocal->AddSinkL(dispSink);
	        	CleanupStack::Pop();
				// this does not work now , so commented 				
	       // 	vidStreamLocal->SetSourceL(videoFileSource);
	  */      	}
			else
				{
				vidStreamLocal = CMceVideoStream::NewLC();
	        //	CMceDisplaySink* dispSink = CMceDisplaySink::NewL( iEngine.MCEManager());
	        //	CleanupStack::PushL( dispSink );
	        	
	   
	        	TRect viewFinder(0, 83, 352, 378);
	        	dispSink->SetDisplayRectL( viewFinder );
	   //  		dispSink->DisableL();
	        	    
	        	vidStreamLocal->AddSinkL(dispSink);
	        	CleanupStack::Pop();
	        	vidStreamLocal->SetSourceL(cam);
	        
				}
		        
	        }

	    // Create instream (== downlink)
	    CMceVideoStream* videoInStreamRTP = NULL;
	    
	    if ( iVideoDirection == CMCETestUIEngineOutSession::EVideoRecvOnly ||
	         iVideoDirection == CMCETestUIEngineOutSession::EVideoSendRecv )
	        {
			videoInStreamRTP = CMceVideoStream::NewLC();
			
			CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
			videoInStreamRTP->SetSourceL( rtpSource );
			CleanupStack::Pop( rtpSource );
			
			CMceDisplaySink* display = CMceDisplaySink::NewLC(iEngine.MCEManager());	
			videoInStreamRTP->AddSinkL( display );
			TRect disp(0, 83, 352, 378);
        	display->SetDisplayRectL( disp );
        //	display->DisableL();
			CleanupStack::Pop( display );
	        }
            
	    // Bound streams     
	    if ( iVideoDirection == CMCETestUIEngineOutSession::EVideoSendRecv )
	        {
	        CleanupStack::Pop( videoInStreamRTP );
			videoOutStreamRTP->BindL( videoInStreamRTP ); 
			outSession->AddStreamL(videoOutStreamRTP);
			CleanupStack::Pop( videoOutStreamRTP );
	        }
	    else if( iVideoDirection == CMCETestUIEngineOutSession::EVideoSendOnly &&
	    		 iSessionParams.videoSinkRtp )
	    	{
	    	outSession->AddStreamL(videoOutStreamRTP);
			CleanupStack::Pop( videoOutStreamRTP );
	    	}
		else
			{
			outSession->AddStreamL(videoInStreamRTP);
			CleanupStack::Pop( videoInStreamRTP );
			}
	 
	 
	 
	    // Add streams to OutSession    
	    if ( iVideoDirection != EVideoRecvOnly )
	        {
	        outSession->AddStreamL( videoOutStream );
	        if( !iSessionParams.videoFileSourceType )
	        	{
	        	outSession->AddStreamL(vidStreamLocal);
				CleanupStack::Pop( vidStreamLocal );
	        	}
	       	
			if(iSessionParams.videoFileSourceType)
				{
				CleanupStack::Pop( videoFileSource );
				}
			else
				{
				CleanupStack::Pop( cam );
				}
	       
	        CleanupStack::Pop( videoOutStream );
	        
	        }
	    else
	        {
	        outSession->AddStreamL( videoInStream );
	        CleanupStack::Pop( videoInStream );     
	        }    
	    
	    }   	    	
	}
