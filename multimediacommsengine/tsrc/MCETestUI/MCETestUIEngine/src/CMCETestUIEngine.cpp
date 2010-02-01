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

#include <sip.h>
#include <MceManager.h>
#include <MceFCMsgExchange.h>
#include <Mcedefs.h>
#include <commdbconnpref.h>
#include <f32file.h>
#include <BADESCA.H>
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineMultimediaObserver.h"
#include "CMCETestUIEngineProfileContainer.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineRefer.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMCETestUIEngineSipObserver.h"
#include "MMCETestUIUserInterface.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIEngineOutEvent.h"
#include "CMCETestUIEngineOutRefer.h"
#include "CMCETestUIEngineCodec.h"
#include "CMCETestUIEngineVideoCodec.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineVideoStream.h"
#include "CMCETestUIEngineVideoSink.h"
#include "CMCETestUIEngineVideoSource.h"
#include "TMCETestUIEngineCmdUpdateSession.h"
#include "TMCETestUIEngineCmdAcceptSession.h"
#include "CMCETestUIEngineInSession.h"
#include "MCETestUIDebug.h"
#include "CMCETestUIEngineRefer.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMCETestUIEngineInSession.h"
#include "CMCETestUIEngineOutEvent.h"
#include "CMCETestUIEngineInEvent.h"
#include "CMCETestUIEngineOutRefer.h"
#include "CMCETestUIEngineInRefer.h"

// MCE Interface
#include <MceOutSession.h>
#include <MceInSession.h>
#include <MceAudioStream.h>
#include <MceVideoStream.h>
#include <MceOutEvent.h>
#include <MceOutRefer.h>
#include <MceAudioCodec.h>
#include <MCEDisplaySink.h>
#include <MCECameraSource.h>
#include <MCERtpSource.h>
#include <MCEAudioCodec.h>
#include <MCEVideoCodec.h>
#include <MCEAMRCodec.h>
#include <MCEMicSource.h>
#include <MCERtpSink.h>
#include <MCERtpSource.h>
#include <MCESpeakerSink.h>
#include <MCEStreamBundle.h>
#include <mcedefs.h>
#include <mcerefer.h>
// Applicaple commands
#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdCreateSession.h"
#include "TMCETestUIEngineCmdSelectProfile.h"
#include "TMCETestUIEngineCmdCreateEvent.h"


// PoC priority
const TUint KAudioPriorityPocAudioUplink                  = 95;
const TUint KAudioPriorityPocAudioDownlink                = 95;
const TUint KAudioPrefPocAudioUplink                      = 0x05130001;
const TUint KAudioPrefPocAudioDownlink                    = 0x05120001;

// VoIP priority
const TUint KSVPAudioPriorityAudioUplink                  = 100;
const TUint KSVPAudioPriorityAudioDownlink                = 100;
const TUint KSVPAudioPrefAudioUplink                      = 0x05220001; 
const TUint KSVPAudioPrefAudioDownlink                    = 0x05210001; 


//SWIS Playback
const TUint KAudioPrioritySwisPlayback                    = 74;
const TUint KAudioPrefSwisPlayback                        = 0x05330001;


const TUint KJitterBufferInactTimeout 					  = 10000;
const TUint KJitterBufferSize							  = 200;
const TUint KJitterBufferTreshold						  = 100;
const TUint KJitterBufferTresholdRealTimeVideoVideo	      = 20;
const TUint KJitterBufferTresholdRealTimeVideoAudio	      = 2;
const TUint KJitterBufferSizeRealTimeVideoAudio	          = 30;
//const TUint KJitterBufferSize							  = 1000;
//const TUint KJitterBufferTreshold						  = 500;
//const TUint KAudioPrefAudioUplink                      = 0x05220001;
//const TUint KAudioPrefAudioDownlink                    = 0x05210001;

// DTMF
const TUint KDtmfAudioPriority                           = 45;
const TUint KDtmfAudioPref                               = 0x00150001; 


#define IS_DOWNLINK( a ) ( a->Source()->Type() == KMceRTPSource )

// CONSTANTS

// Experimental Uid for UI engine, reserve own if needed
const TUid KUIEngineUid = { 0x10281F9B };  
const TUid KUIOptionsUid = { 0x10123F9E };

_LIT8( KFCTypeTBCP, "TBCP" ); 
_LIT8( KMCERtpSinkIdentity, "RtpSink@multimedia.engine.com" ); 
_LIT8( KMCERtpSourceIdentity, "RtpSource@multimedia.engine.com" ); 
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngine::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngine* CMCETestUIEngine::NewL() 
    {

    CMCETestUIEngine* self = new (ELeave) CMCETestUIEngine();
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CMCETestUIEngine
// -----------------------------------------------------------------------------
//
CMCETestUIEngine::CMCETestUIEngine()
    : iNextOutSessionId( 0 ),iNextInSessionId( 0 ),
      iNextOutEventId(0),iNextInEventId(0),
      iNextOutReferId(0),iNextInReferId(0),
      iFCSession(EFalse), iRtcpFlag(EFalse),iKoppaFlag(ETrue), 
      iBundle(EFalse), iSdp(EFalse),i180PreCondition(EFalse),iSecureSessionAVP(EFalse),
	  iSecPreconditions(EFalse),iAutoResponse(EFalse),
      iVoIPFlag(EFalse), iHoldWithAddress( ETrue ),iPriorityType( KVoIPPriority ),
      iJitterBufTreshold( KJitterBufferTreshold ), iJitterBufSize( KJitterBufferSize ),
      iJitterBufTimeout(KJitterBufferInactTimeout), iMultiCodecSupport(ETrue)
     {
     }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::ConstructL()
    {
    iSipObserver = CMCETestUIEngineSipObserver::NewL();
  //  iSip = CSIP::NewL( KUIEngineUid, *iSipObserver );
  //	  iSip = CSIP::NewL( KUIOptionsUid, *iSipObserver );
	iSip = iSipObserver->GetSipInstance();
    iProfiles = CMCETestUIEngineProfileContainer::NewL( *this, *iSip );

    iManager = CMceManager::NewL( KUIEngineUid, &iDataContainer );

    iMMObserver = CMCETestUIEngineMultimediaObserver::NewL( *this );
	iManager->SetSessionObserver( iMMObserver );
	iManager->SetTransactionObserver( iMMObserver ); 
	iManager->SetInTransactionObserver( iMMObserver ); 
	iManager->SetInSessionObserver( iMMObserver );
	iManager->SetMediaObserver( iMMObserver );
	iManager->SetRtpObserver( iMMObserver );	
	iManager->SetEventObserver( iMMObserver );	
	iManager->SetInEventObserver( iMMObserver );
	iManager->SetReferObserver(iMMObserver);
	iManager->SetInReferObserver(iMMObserver);
	iManager->SetDtmfObserver( iMMObserver );
		
	iCallWatcher = CMCEPoCOmaCSCallWatcher::NewL( iMMObserver );
	iCallWatcher->Subscribe();	

    iFC = CMceFcMsgExchange::NewL( *iManager, KFCTypeTBCP );
    
    iOutSessionWithProfile = EFalse;        
    iOutSessionWithAPList = EFalse;
    iOutSessionWithAPNumber = EFalse;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::~CMCETestUIEngine
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngine::~CMCETestUIEngine()
    {
    iCommands.ResetAndDestroy();
    iOutSessions.ResetAndDestroy();
    iInSessions.ResetAndDestroy();
    iOutEvents.ResetAndDestroy();
    iInEvents.ResetAndDestroy();
    iOutRefers.ResetAndDestroy();
    iInRefers.ResetAndDestroy();
    iTransactionIds.Close();
    delete iCallWatcher;
    delete iFC;
    delete iManager;
    delete iMMObserver;
    delete iProfiles;
    delete iSip;
    delete iSipObserver;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::GetCommandsL
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<TMCETestUIEngineCmdBase>& 
        CMCETestUIEngine::GetCommandsL()
    {
    iCommands.ResetAndDestroy();

    iCommands.Append( new (ELeave) TMCETestUIEngineCmdSelectProfile( *this ) );

    TBool canCreateOutSession = EFalse;
    
    const RPointerArray<CMCETestUIEngineProfile> profiles = UsedProfiles();
    
    // OutSession can be created with profile
    for ( TInt i = 0; i < profiles.Count(); ++i )
        {
        if ( profiles[i]->IsRegistered() )
            {
            canCreateOutSession = ETrue;
            }
        }
    
      // TODO: OutSession can be created also with event or refer.
    //       Add check for those.    

    
    // Provide caller with ability to create an outsession.
    if ( canCreateOutSession )
        {    
     iCommands.Append( 
                    new (ELeave) TMCETestUIEngineCmdCreateSession( *this ) );
     
        }
    iCommands.Append( 
                    new (ELeave) TMCETestUIEngineCmdCreateSessionUsingAPList( *this ) );
    iCommands.Append( 
                    new (ELeave) TMCETestUIEngineCmdCreateSessionUsingAPNumber( *this ) );
    if ( canCreateOutSession )
        {    
        iCommands.Append( 
                    new (ELeave) TMCETestUIEngineCmdCreateEvent( *this ) );
        iCommands.Append( 
                    new (ELeave) TMCETestUIEngineCmdCreateRefer( *this ) );
        
        }
    
     
        
    return iCommands;    
    }
            

// -----------------------------------------------------------------------------
// CMCETestUIEngine::SetUserInterface
// -----------------------------------------------------------------------------
//
EXPORT_C void CMCETestUIEngine::SetUserInterface(
        MMCETestUIUserInterface* aUserInterface )
    {
    iUserInterface = aUserInterface;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::AvailableProfiles
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineProfile>& 
        CMCETestUIEngine::AvailableProfiles( )
    {
    return iProfiles->AvailableProfiles();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::UsedProfiles
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineProfile>& 
        CMCETestUIEngine::UsedProfiles( )
    {
    return iProfiles->UsedProfiles();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::DeleteSession
// -----------------------------------------------------------------------------
//
EXPORT_C void CMCETestUIEngine::DeleteSession( 
        CMCETestUIEngineSession& aSession )
    {
    for ( TInt i = 0; i < iOutSessions.Count(); ++i )
        {
        if ( iOutSessions[i] == &aSession )
            {
            delete iOutSessions[i];
            iOutSessions.Remove( i );
            iOutSessions.Compress();
            EngineStateChangedL();
            return;
            }
        }
    for ( TInt i = 0; i < iInSessions.Count(); ++i )
        {
        if ( iInSessions[i] == &aSession )
            {
         	delete iInSessions[i];
            iInSessions.Remove( i );
            iInSessions.Compress();
            EngineStateChangedL();
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateOutSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutSession& CMCETestUIEngine::CreateOutSessionL( 
        CMCETestUIEngineProfile& aProfile,
        TDirection aDirection ,
        TVideoDirection aVideoDirection ,
        TSessionType aSessionType)
    {
    SetPriorities();
    
    CMCETestUIEngineOutSession* outSession = CMCETestUIEngineOutSession::NewL( 
            *this, aProfile, aDirection,aVideoDirection,aSessionType );
    CleanupStack::PushL ( outSession );
    iOutSessions.Append( outSession );
	NotifyUser( KUserNotificationSessionCreated );
	EngineStateChangedL();	    
    CleanupStack::Pop( outSession );
    
    return *outSession;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateOutSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutSession& CMCETestUIEngine::CreateOutSessionL( 
        TUint32 aIAPId,
        TDirection aDirection ,
        TVideoDirection aVideoDirection ,
        TSessionType aSessionType)
    {
    SetPriorities();
    
    CMCETestUIEngineOutSession* outSession = CMCETestUIEngineOutSession::NewL( 
            *this, aIAPId, aDirection,aVideoDirection,aSessionType );
    CleanupStack::PushL ( outSession );
    iOutSessions.Append( outSession );
	NotifyUser( KUserNotificationSessionCreated );
	EngineStateChangedL();	    
    CleanupStack::Pop( outSession );
    
    return *outSession;
    }
 // -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateOutSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutSession& CMCETestUIEngine::CreateOutSessionL( CMceRefer& aRefer)
    {
    SetPriorities();
    TSessionType sessionType = ECustomSession;	
	        
	TDirection direction = ENotSpesified;
	        
	TVideoDirection videoDirection = EVideoNotSpesified;
    CMCETestUIEngineOutSession* outSession = CMCETestUIEngineOutSession::NewL( 
            *this, aRefer ,direction,videoDirection,sessionType);
    CleanupStack::PushL ( outSession );
    iOutSessions.Append( outSession );
	NotifyUser( KUserNotificationSessionCreated );
	EngineStateChangedL();	    
    CleanupStack::Pop( outSession );
    
    return *outSession;
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateInSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineInSession& CMCETestUIEngine::CreateInSessionL(
        CMceInSession* aSession )
    {
    if ( aSession->Streams().Count()  == 0)
    	{
    	AddSendRecvDefaultAudioStreamsL(aSession);
    	}
    
    UpdateWithPriorityCodecValues(aSession);
    if( i180PreCondition )
    	{
    	aSession->SetModifierL( KMce100Rel, KMce100RelRequired );
    	}
   	
   	if( iHoldWithAddress )
    	{
    	aSession->SetModifierL( KMceMediaDirection, KMceMediaDirectionWithAddress );
    	} 
    if (iSecureSessionAVP )
		{
		aSession->SetModifierL( KMceSecureSession, KMceSecurePlainAVP );
		} 	

    if(IsFCSession())
    	{
    	UpdatePoCValues( aSession );
    	}
    else 
    	{
    	UpdateInactivitytimeValues(aSession);
    	}
    if( iBundle )
        {
        BundleStreamsL( aSession );
        }
    UpdateRtpSink(aSession);
    
    CMCETestUIEngineInSession* inSession = CMCETestUIEngineInSession::NewL(
            *this, aSession );
    CleanupStack::PushL( inSession );
    iInSessions.Append( inSession );
	NotifyUser( KMiscIncomingSession );
	
	EngineStateChangedL();	
	CleanupStack::Pop( inSession );
	
	return *inSession;   
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngine::AddSendRecvDefaultAudioStreamsL
// -----------------------------------------------------------------------------
//

void CMCETestUIEngine::AddSendRecvDefaultAudioStreamsL(CMceInSession* aSession)
	{
	
	//create stream, boundstream with micsource - rtpsink and rtpsource and speaker sink
	    CMceAudioStream* audioOutStream = NULL;
	    CMceAudioStream* audioOutStreamLocal = NULL;
	 
	    // Create outstream (== uplink)
       audioOutStream = CMceAudioStream::NewLC();
	   CMceMicSource* mic = NULL;
	   mic = CMceMicSource::NewLC();
	   audioOutStream->SetSourceL( mic );	
	   CleanupStack::Pop( mic );
			
	   CMceRtpSink* rtpSink;
	  
	   if(iRtcpFlag)
			{
			rtpSink = CMceRtpSink::NewL(ETrue);	
			CleanupStack::PushL(rtpSink);
			}
		else
			{
			rtpSink = CMceRtpSink::NewLC();	
			}
		audioOutStream->AddSinkL( rtpSink );
		CleanupStack::Pop( rtpSink );
			
	

	    // Create instream (== downlink)
	    CMceAudioStream* audioInStream = NULL;
	    audioInStream = CMceAudioStream::NewLC();
		CMceRtpSource* rtpSource = CMceRtpSource::NewLC( iJitterBufSize, 
                                	                         iJitterBufTreshold, 
                                	                         iJitterBufTimeout, 0);
		audioInStream->SetSourceL( rtpSource );
		CleanupStack::Pop( rtpSource );
			
		CMceSpeakerSink* speaker = CMceSpeakerSink::NewLC();
		audioInStream->AddSinkL( speaker );
		CleanupStack::Pop( speaker );
		
	   
		audioOutStream->BindL( audioInStream ); 
	    CleanupStack::Pop( audioInStream );     
	
	   // Add streams to OutSession    
	    aSession->AddStreamL( audioOutStream );
	    CleanupStack::Pop( audioOutStream );
	
 	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::UpdateIncomingSessionVoIPData
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::UpdateIncomingSessionVoIPDataL(CMceInSession* aSession)
	{
	
	// modify codecs and codec settings if streams found
    if ( aSession->Streams().Count() )
        {
       	const RPointerArray< CMceMediaStream >& streams = aSession->Streams();
       for ( TInt i = 0; i < streams.Count(); i++ )
            {
            CMceMediaStream* stream1 = ( CMceMediaStream* ) streams[i];
            if ( KMceVideo == stream1->Type() )
                {
                aSession->RemoveStreamL( *streams[ i ] );
                }
            
            if ( stream1->Source() && stream1->Source()->Type() == KMceRTPSource )
                {
               	CMceRtpSource* rtpSource = ( CMceRtpSource* ) stream1->Source();
               	rtpSource->UpdateL( 16, 11, 1100 ); // magic numbers
               	rtpSource->DisableL();
                }
                
            if ( stream1->Sinks().Count()  )
                {
                const RPointerArray< CMceMediaSink >& sinks = stream1->Sinks();
                
                 for ( TInt z = 0; z < sinks.Count(); z++ )
                    {
                    if ( sinks[ z ]->Type() == KMceSpeakerSink )
                        {
                        CMceSpeakerSink* speakerSink = ( CMceSpeakerSink* )  sinks[ z ];
                        speakerSink->DisableL();                       
                        }
                    } 
                }
            
            if ( stream1->Source() && stream1->Source()->Type() == KMceMicSource )
                {
               	CMceMicSource* micSource = ( CMceMicSource* ) stream1->Source();
               	micSource->DisableL();
                }
            }
        }
  //  CheckStreamsL( *aSession );   
   const RPointerArray< CMceMediaStream >& streamArray = aSession->Streams();
   for ( TInt i = 0; i < streamArray.Count(); i++ )
        {
        CMceAudioStream* audioStream = ( CMceAudioStream* ) streamArray[ i ];
        
        TUint priority = IS_DOWNLINK( audioStream ) ? 
                KSVPAudioPriorityAudioDownlink : 
                KSVPAudioPriorityAudioUplink;
                
        TUint preference = IS_DOWNLINK( audioStream ) ?
                KSVPAudioPrefAudioDownlink :
                KSVPAudioPrefAudioUplink;
            
        SetMMFPriorityForCodecsL( audioStream->Codecs(),
                                  priority, 
                                  preference );
        
        if ( audioStream->BoundStream() )
            {
            CMceAudioStream* audioStream2 = ( CMceAudioStream* ) &audioStream->BoundStreamL();

            priority = IS_DOWNLINK( audioStream2 ) ? 
                KSVPAudioPriorityAudioDownlink : 
                KSVPAudioPriorityAudioUplink;
                
            preference = IS_DOWNLINK( audioStream2 ) ?
                KSVPAudioPrefAudioDownlink :
                KSVPAudioPrefAudioUplink;
                
            SetMMFPriorityForCodecsL( audioStream2->Codecs(),
                                      priority, 
                                      preference );
            }
        }
        
	aSession->SetModifierL( KMcePreconditions, KMcePreconditionsNotUsed  );
    aSession->UpdateL();
 //   aSession->RingL();
 //   aSession->AcceptL();
    
    
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::SetMMFPriorityForCodecsL
// -----------------------------------------------------------------------------
//	
void CMCETestUIEngine::SetMMFPriorityForCodecsL( 
    const RPointerArray<CMceAudioCodec>& aCodecs, 
    TUint aAudioPriority, 
    TUint aAudioPreference )
    {
    for ( TInt z = 0; z < aCodecs.Count(); z++ )
        {
        if ( !aCodecs[ z ]->SdpName().Compare( KMceSDPNameDtmf ) )
            {
            aCodecs[ z ]->SetMMFPriorityL( KDtmfAudioPriority );
            aCodecs[ z ]->SetMMFPriorityPreferenceL( KDtmfAudioPref );    
            }
        else
            {
            aCodecs[ z ]->SetMMFPriorityL( aAudioPriority );
            aCodecs[ z ]->SetMMFPriorityPreferenceL( aAudioPreference );    
            }
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::SetMMFPriorityForCodecsL
// -----------------------------------------------------------------------------
//	
void CMCETestUIEngine::SetMMFPriorityForCodecsL( 
    const RPointerArray<CMceVideoCodec>& aCodecs, 
    TUint aPriority, 
    TUint aPreference )
    {
    for ( TInt z = 0; z < aCodecs.Count(); z++ )
        {
        aCodecs[ z ]->SetMMFPriorityL( aPriority );
        aCodecs[ z ]->SetMMFPriorityPreferenceL( aPreference );    
        }
    }
        
// -----------------------------------------------------------------------------
// CMCETestUIEngine::UpdateIncomingSessionVoIPDataAfterAnswerL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::UpdateIncomingSessionVoIPDataAfterAnswerL(CMceInSession& aSession)
	{
	const RPointerArray<CMceMediaStream>& streams = aSession.Streams();
    
    for ( TInt i = 0; i < streams.Count(); i++ )
        {
        CMceMediaStream* stream1 = ( CMceMediaStream* ) streams[i];
        
        if ( stream1->Source() && stream1->Source()->Type() == KMceRTPSource )
            {
            CMceRtpSource* rtpSource = ( CMceRtpSource* ) stream1->Source();
           	rtpSource->EnableL();
            }
            
        if ( stream1->Sinks().Count()  )
            {
            const RPointerArray< CMceMediaSink >& sinks = stream1->Sinks();
            
             for ( TInt z = 0; z < sinks.Count(); z++ )
                {
                if ( sinks[ z ]->Type() == KMceSpeakerSink )
                    {
                    CMceSpeakerSink* speakerSink = 
                        ( CMceSpeakerSink* )  sinks[ z ];
                    speakerSink->EnableL();
                    }
                } 
            }
        
        if ( stream1->Source() && stream1->Source()->Type() == KMceMicSource )
            {
           	CMceMicSource* micSource = ( CMceMicSource* ) stream1->Source();
           	micSource->EnableL();
            }  
        } 	
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::IncomingUpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMCETestUIEngine::IncomingUpdateL(CMceSession& aOrigSession, 
					CMceInSession* aUpdatedSession)
	{
	 for ( TInt i = 0; i < iOutSessions.Count(); ++i )
        {
        CMceOutSession& wrappedSession = iOutSessions[i]->OutSession();
        if ( &wrappedSession == &aOrigSession )
            {
         	delete iOutSessions[i];
            iOutSessions.Remove( i );
            iOutSessions.Compress();
            CMCETestUIEngineInSession& inSession = CreateInSessionL(aUpdatedSession);
            return;
            }
        }

	 for ( TInt i = 0; i < iInSessions.Count(); ++i )
        {
        CMceInSession& wrappedSession = iInSessions[i]->InSession();
        if ( &wrappedSession == &aOrigSession )
            {
         	delete iInSessions[i];
            iInSessions.Remove( i );
            iInSessions.Compress();
            CMCETestUIEngineInSession& inSession = CreateInSessionL(aUpdatedSession);
            return;
            }
        }
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::OutSessions
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineOutSession>& 
        CMCETestUIEngine::OutSessions( )
    {
    return iOutSessions;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::InSessions
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineInSession>& 
        CMCETestUIEngine::InSessions( )
    {
    return iInSessions;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::ProfileContainer
// -----------------------------------------------------------------------------
//
CMCETestUIEngineProfileContainer& CMCETestUIEngine::ProfileContainer()
    {
    return *iProfiles;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::MCEManager
// -----------------------------------------------------------------------------
//
CMceManager& CMCETestUIEngine::MCEManager()
    {
    return *iManager;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::MMObserver
// -----------------------------------------------------------------------------
//  
    
CMCETestUIEngineMultimediaObserver& CMCETestUIEngine::MMObserver()
	{
	return *iMMObserver;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::FloorControl
// -----------------------------------------------------------------------------
//
CMceFcMsgExchange& CMCETestUIEngine::FloorControl()
	{
	return *iFC;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::EngineStateChanged
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::EngineStateChangedL()
    {
    if ( iUserInterface )
        {
        iUserInterface->EngineStateChangedL();
        }
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::NotifyUser
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::NotifyUser( const TDesC& aMessage )
    {
    if ( iUserInterface )
        {
        iUserInterface->NotifyUser( aMessage );
        }
    }
    
void CMCETestUIEngine::NotifyUserWaitL ( const TDesC& aMessage )
  {
  if ( iUserInterface )
     {
     iUserInterface->NotifyUserWaitL ( aMessage );
     }
  }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::AskQuestion
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEngine::AskQuestionL( 
        const CMCETestUIQuestionSingleSelection& aQuestion )
    {
    if ( iUserInterface )
        {
        return iUserInterface->AskQuestionL( aQuestion );
        }    
    return KErrNotReady;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::AskQuestion
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::AskQuestionL( CMCETestUIQuestionDataQuery& aQuestion )
    {
    if ( iUserInterface )
        {
        iUserInterface->AskQuestionL( aQuestion );
        }     
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::SessionParams
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngine::SessionParamsL(TSessionParams &aSessionParams, TSessionType aSessionType)
{
	if ( iUserInterface )
        {
        return iUserInterface->SessionParamsL( aSessionParams,aSessionType );
        }     
    return EFalse;    
}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::SessionEstablishValues
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngine::SessionEstablishValues(TSessionEstablishValues &sessionValues)
	{
	if ( iUserInterface )
        {
        return iUserInterface->SessionEstablishValues( sessionValues);
        }     
    return EFalse;
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::SessionResponseValues
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngine::SessionResponseValues(TSessionResponseValues &responseValues)
	{
	if ( iUserInterface )
        {
        return iUserInterface->SessionResponseValues( responseValues);
        }     
    return EFalse;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::SessionEstablishValues
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngine::NotifyValues(TNotifyValues &notifyValues)
	{
	if ( iUserInterface )
        {
        return iUserInterface->NotifyValues( notifyValues);
        }     
    return EFalse;
	}		
// -----------------------------------------------------------------------------
// CMCETestUIEngine::NextOutSessionId
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEngine::NextOutSessionId()
    {
    return iNextOutSessionId++;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::NextInSessionId
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEngine::NextInSessionId()
    {
    return iNextInSessionId++;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::UpdateWithPoCValues
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::UpdatePoCValues(CMceInSession* aSession)
	{
	RDebug::Print( _L( "*MCE Test UI* UpdatePoCValues-Entry" ));
	const RPointerArray<CMceMediaStream>& streams = aSession->Streams();

    if ( streams.Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
    
    CMceMediaStream* inStream = (CMceAudioStream*) streams[0]; 
    CMceAudioStream* audioIn = NULL;
    if( inStream->Type() == KMceAudio )
        {
        audioIn = (CMceAudioStream*) inStream;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    
    CMceMediaStream& outStream = inStream->BoundStreamL();
    CMceAudioStream* audioOut = (CMceAudioStream*) &outStream;
	
	const RPointerArray<CMceAudioCodec>& codecs = audioIn->Codecs();
	
	for( int i = 0; i < codecs.Count(); i++ )
	    {
	    CMceAudioCodec* codec = codecs[i];
	    // Search for AMR codec and delete others
	    if( codec->SdpName().Find( KMceSDPNameAMR ) == KErrNotFound )
	        {
	        audioIn->RemoveCodecL( *codec );
	        --i;
	        }
	    }
	    
	if( ( codecs.Count() != 1 ) || 
	    ( codecs[0]->SdpName().Find( KMceSDPNameAMR ) == KErrNotFound ))
	    {
	    // AMR codec not found
	    User::Leave( KErrArgument );
	    }
	
	CMceRtpSource* rtpSource = (CMceRtpSource*) inStream->Source();
	HBufC8* rtpSourceIdentity = KMCERtpSourceIdentity().AllocLC();
	rtpSource->UpdateL( iJitterBufSize, 
					    iJitterBufTreshold, 
					    iJitterBufTimeout,
					    rtpSourceIdentity );	
	CleanupStack::PopAndDestroy(rtpSourceIdentity);
	CMceMicSource* micSource = (CMceMicSource*)audioOut->Source();
	micSource->DisableL();
	CMceMediaSink* rtpOutSink = (CMceMediaSink*) audioOut->Sinks()[0];
//	rtpOutSink->DisableL();  
	// disabling RTCP sending
	HBufC8* rtpSinkIdentity = KMCERtpSinkIdentity().AllocLC();
	((CMceRtpSink*)rtpOutSink)->UpdateL( ETrue,rtpSinkIdentity ); 
	RDebug::Print( _L( "*MCE Test UI* UpdatePoCValues-Exit" )); 
	CleanupStack::PopAndDestroy(rtpSinkIdentity);

	}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::UpdateInactivitytimeValues
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::UpdateInactivitytimeValues(CMceInSession* aSession)
	{	
	const RPointerArray<CMceMediaStream>& streams = aSession->Streams();

    if ( streams.Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
        
    TBool sendRecvVideo = IsSendRecvVideo( aSession );
	for ( TInt i = 0; i < streams.Count(); ++i )
		{
		TInt jitbufSize( iJitterBufSize );
    	TInt jitbufThreshold( iJitterBufTreshold );
    	TInt jitbufTimeout( iJitterBufTimeout );
	
		CMceMediaStream* mediaStream = 
                                    static_cast<CMceMediaStream*>(streams[i]);

    	GetJitterBufferValues( 
    	    mediaStream, jitbufSize, jitbufThreshold, jitbufTimeout, sendRecvVideo );

        HBufC8* rtpSourceIdentity = KMCERtpSourceIdentity().AllocLC();
        if ( mediaStream->Source()->Type() == KMceRTPSource )
        	{
        
        	CMceRtpSource* rtpSource = (CMceRtpSource*) mediaStream->Source();
			rtpSource->UpdateL( jitbufSize, 
					            jitbufThreshold, 
					            jitbufTimeout,
					            rtpSourceIdentity);
        	}
        if( mediaStream->BoundStream() )
            {
            CMceMediaSource* boundsource = mediaStream->BoundStreamL().Source();
            if( boundsource->Type() == KMceRTPSource )
                {
                CMceRtpSource* boundRtpSource = (CMceRtpSource*) boundsource;
                boundRtpSource->UpdateL( jitbufSize, 
					                  jitbufThreshold, 
					                  jitbufTimeout,
					                  rtpSourceIdentity );
                }
            }
         CleanupStack::PopAndDestroy(rtpSourceIdentity);
		}
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::UpdateRtpSink
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::UpdateRtpSink(CMceInSession* aSession)
	{
	if ( IsSendRecvVideo( aSession ) )
	    {
	    // Don't use rtcp in sendrecv video session
	    iRtcpFlag = ETrue;
	    }
	
	const RPointerArray<CMceMediaStream>& streams = aSession->Streams();

    if ( streams.Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
	for ( TInt i = 0; i < streams.Count(); ++i )
		{
		CMceMediaStream* mediaStream = 
                                    static_cast<CMceMediaStream*>(streams[i]);
        const RPointerArray<CMceMediaSink>& sinks = mediaStream->Sinks();
        for ( TInt j = 0; j < sinks.Count(); ++j )
        	{
        	if(sinks[j]->Type() == KMceRTPSink)
        		{
        		CMceRtpSink* rtpSink = (CMceRtpSink*)(sinks[j]);
        		HBufC8* rtpSinkIdentity = KMCERtpSinkIdentity().AllocLC();
        		rtpSink->UpdateL( iRtcpFlag, rtpSinkIdentity );
        		CleanupStack::PopAndDestroy(rtpSinkIdentity);
        		}
        	}
        if( mediaStream->BoundStream() )
            {
            const RPointerArray<CMceMediaSink>& boundsinks = mediaStream->BoundStreamL().Sinks();
            for ( TInt j = 0; j < boundsinks.Count(); ++j )
            	{
            	if(boundsinks[j]->Type() == KMceRTPSink)
            		{
            		CMceRtpSink* rtpSink = (CMceRtpSink*)(boundsinks[j]);
            		HBufC8* rtpSinkIdentity = KMCERtpSinkIdentity().AllocLC();
        			rtpSink->UpdateL( iRtcpFlag, rtpSinkIdentity );
        			CleanupStack::PopAndDestroy(rtpSinkIdentity);
            		}
            	}
            }
      	}
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngine::BundleStreamsL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::BundleStreamsL( CMceInSession* aSession )
	{
	const RPointerArray<CMceMediaStream>& streams = aSession->Streams();

    if ( streams.Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
        
    CMceMediaStream* audioStream = NULL;
    CMceMediaStream* videoStream = NULL;
        
	for ( TInt i = 0; i < streams.Count(); ++i )
		{
		CMceMediaStream* mediaStream = 
                                    static_cast<CMceMediaStream*>(streams[i]);
        
        CMceMediaSource* source = mediaStream->Source();
        User::LeaveIfNull( source );
        if( mediaStream->Type() == KMceAudio && source->Type() == KMceRTPSource )
            {
            audioStream = mediaStream;
            }
        else if( mediaStream->Type() == KMceVideo && source->Type() == KMceRTPSource )
            {
            videoStream = mediaStream;
            }      
        if( mediaStream->BoundStream() )
            {
            CMceMediaStream* boundStream = &mediaStream->BoundStreamL();
            
            CMceMediaSource* boundsource = boundStream->Source();
            User::LeaveIfNull( boundsource );
            
            if( boundStream->Type() == KMceAudio && boundsource->Type() == KMceRTPSource )
                {
                audioStream = mediaStream;
                }
             else if( boundStream->Type() == KMceVideo && boundsource->Type() == KMceRTPSource )
                {
                videoStream = mediaStream;
                }     
            }
      	}
    if( audioStream && videoStream )
        {
        CMceStreamBundle* bundle = CMceStreamBundle::NewLC( CMceStreamBundle::ELS );
        bundle->AddStreamL( *audioStream );
        bundle->AddStreamL( *videoStream );
        CleanupStack::Pop( bundle );
        aSession->AddBundleL( bundle );
        }
    }
    
      
// -----------------------------------------------------------------------------
// CMCETestUIEngine::UpdateWithPocPriorityCodecValues
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// CMCETestUIEngine::SetPriorities
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::SetPriorities()
	{
    if( iPriorityType == KPoCPriority )
        {
        iPriorityUplink = KAudioPriorityPocAudioUplink;
        iPriorityDownlink = KAudioPriorityPocAudioDownlink;
        iPrefUplink = KAudioPrefPocAudioUplink;
        iPrefDownlink = KAudioPrefPocAudioDownlink;
        }
    else if( iPriorityType == KVoIPPriority )
        {
        iPriorityUplink = KSVPAudioPriorityAudioUplink;
        iPriorityDownlink = KSVPAudioPriorityAudioDownlink;
        iPrefUplink = KSVPAudioPrefAudioUplink;
        iPrefDownlink = KSVPAudioPrefAudioDownlink;
        }
    else
        {
        iPriorityUplink = KAudioPrioritySwisPlayback;
        iPriorityDownlink = KAudioPrioritySwisPlayback;
        iPrefUplink = KAudioPrefSwisPlayback;
        iPrefDownlink = KAudioPrefSwisPlayback;
        }
	}


// -----------------------------------------------------------------------------
// CMCETestUIEngine::UpdateWithPriorityCodecValues
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::UpdateWithPriorityCodecValues(CMceInSession* aSession)
	{
	
	SetPriorities();
	
	const RPointerArray<CMceMediaStream>& streams = aSession->Streams();
	
	for ( TInt i = 0; i < streams.Count(); ++i )
        {        
        if ( streams[i]->Type() == KMceAudio )
            {
            CMceAudioStream* audioStream = ( CMceAudioStream* ) streams[ i ];
            
            TUint priority = IS_DOWNLINK( audioStream ) ? 
                    iPriorityDownlink : 
                    iPriorityUplink;
                    
            TUint preference = IS_DOWNLINK( audioStream ) ?
                    iPrefDownlink :
                    iPrefUplink;
                
            SetMMFPriorityForCodecsL( audioStream->Codecs(),
                                      priority, 
                                      preference );
            
            if ( audioStream->BoundStream() )
                {
                CMceAudioStream* audioStream2 = ( CMceAudioStream* ) &audioStream->BoundStreamL();

                priority = IS_DOWNLINK( audioStream2 ) ? 
                    iPriorityDownlink : 
                    iPriorityUplink;
                    
                preference = IS_DOWNLINK( audioStream2 ) ?
                    iPrefDownlink :
                    iPrefUplink;
                    
                SetMMFPriorityForCodecsL( audioStream2->Codecs(),
                                          priority, 
                                          preference );
                }
            }
        else if ( streams[i]->Type() == KMceVideo )
            {
            // Direction does not matter in video case
            CMceVideoStream* videoStream = ( CMceVideoStream* ) streams[ i ];
            SetMMFPriorityForCodecsL( videoStream->Codecs(),
                                      iPriorityDownlink, 
                                      iPrefDownlink );
            if ( videoStream->BoundStream() )
                {
                CMceVideoStream* videoStream2 = ( CMceVideoStream* ) &videoStream->BoundStreamL();
                SetMMFPriorityForCodecsL( videoStream2->Codecs(),
                                          iPriorityDownlink, 
                                          iPrefDownlink );
                }
            }
        else
            {
            // NOP
            }
        }
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::DeleteEvent
// -----------------------------------------------------------------------------
//
EXPORT_C void CMCETestUIEngine::DeleteEvent( 
        CMCETestUIEngineEvent& aEvent )
    {
    for ( TInt i = 0; i < iOutEvents.Count(); ++i )
        {
        if ( iOutEvents[i] == &aEvent )
            {
            delete iOutEvents[i];
            iOutEvents.Remove( i );
            iOutEvents.Compress();
            EngineStateChangedL();
            return;
            }
        }
    for ( TInt i = 0; i < iInEvents.Count(); ++i )
        {
        if ( iInEvents[i] == &aEvent )
            {
         	delete iInEvents[i];
            iInEvents.Remove( i );
            iInEvents.Compress();
            EngineStateChangedL();
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateOutEvent
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutEvent& CMCETestUIEngine::CreateOutEventL( 
        CMCETestUIEngineProfile& aProfile)
    {
    
    CMCETestUIEngineOutEvent* outEvent = CMCETestUIEngineOutEvent::NewL( 
            *this, aProfile);
    CleanupStack::PushL ( outEvent);
    iOutEvents.Append( outEvent );
	NotifyUser( KUserNotificationEventCreated );
	EngineStateChangedL();	    
    CleanupStack::Pop( outEvent );
    
    return *outEvent;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateOutEvent
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutEvent& CMCETestUIEngine::CreateOutEventL( 
        CMceOutEvent* aEvent)
    {
    
    CMCETestUIEngineOutEvent* outEvent = CMCETestUIEngineOutEvent::NewL( 
            *this, aEvent);
    CleanupStack::PushL ( outEvent);
    iOutEvents.Append( outEvent );
	NotifyUser( KUserNotificationEventCreated );
	EngineStateChangedL();	    
    CleanupStack::Pop( outEvent );
    
    return *outEvent;
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateInEvent
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineInEvent& CMCETestUIEngine::CreateInEventL(
        CMceInEvent* aEvent )
    {
    CMCETestUIEngineInEvent* inEvent = CMCETestUIEngineInEvent::NewL(
            *this, aEvent );
    CleanupStack::PushL( inEvent );
    iInEvents.Append( inEvent );
	NotifyUser( KMiscIncomingEvent );
	EngineStateChangedL();	
	CleanupStack::Pop( inEvent );
	
	return *inEvent;   
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::OutEvents
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineOutEvent>& 
        CMCETestUIEngine::OutEvents( )
    {
    return iOutEvents;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::InEvents
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineInEvent>& 
        CMCETestUIEngine::InEvents( )
    {
    return iInEvents;
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::EventParams
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngine::EventParamsL(TEventParams &aEventParams)
{
	if ( iUserInterface )
        {
        return iUserInterface->EventParamsL( aEventParams);
        }     
    return EFalse;    
}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::NextOutEventId
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEngine::NextOutEventId()
    {
    return iNextOutEventId++;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::NextInEventId
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEngine::NextInEventId()
    {
    return iNextInEventId++;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::DeleteRefer
// -----------------------------------------------------------------------------
//
EXPORT_C void CMCETestUIEngine::DeleteRefer( 
        CMCETestUIEngineRefer& aRefer )
    {
    for ( TInt i = 0; i < iOutRefers.Count(); ++i )
        {
        if ( iOutRefers[i] == &aRefer )
            {
            delete iOutRefers[i];
            iOutRefers.Remove( i );
            iOutRefers.Compress();
            EngineStateChangedL();
            return;
            }
        }
    for ( TInt i = 0; i < iInRefers.Count(); ++i )
        {
        if ( iInRefers[i] == &aRefer )
            {
         	delete iInRefers[i];
            iInRefers.Remove( i );
            iInRefers.Compress();
            EngineStateChangedL();
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateOutReferL
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutRefer& CMCETestUIEngine::CreateOutReferL( 
        CMCETestUIEngineProfile& aProfile)
    {
    
    CMCETestUIEngineOutRefer* outRefer = CMCETestUIEngineOutRefer::NewL( 
            *this, aProfile);
    CleanupStack::PushL ( outRefer);
    iOutRefers.Append( outRefer );
	NotifyUser( KUserNotificationReferCreated );
	EngineStateChangedL();	    
    CleanupStack::Pop( outRefer );
    
    return *outRefer;
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateOutReferL
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutRefer& CMCETestUIEngine::CreateOutReferL( 
        CMCETestUIEngineSession& aSession)
    {
    
    CMCETestUIEngineOutRefer* outRefer = CMCETestUIEngineOutRefer::NewL( 
            *this, aSession);
    CleanupStack::PushL ( outRefer);
    iOutRefers.Append( outRefer );
	NotifyUser( KUserNotificationReferCreated );
	EngineStateChangedL();	    
    CleanupStack::Pop( outRefer );
    
    return *outRefer;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateOutReferL
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineOutRefer& CMCETestUIEngine::CreateOutReferL( 
        CMCETestUIEngineEvent& aEvent)
    {
    
    CMCETestUIEngineOutRefer* outRefer = CMCETestUIEngineOutRefer::NewL( 
            *this, aEvent);
    CleanupStack::PushL ( outRefer);
    iOutRefers.Append( outRefer );
	NotifyUser( KUserNotificationReferCreated );
	EngineStateChangedL();	    
    CleanupStack::Pop( outRefer );
    
    return *outRefer;
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateInReferL
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIEngineInRefer& CMCETestUIEngine::CreateInReferL(
        CMceInRefer* aRefer )
    {
    CMCETestUIEngineInRefer* inRefer = CMCETestUIEngineInRefer::NewL(
            *this, aRefer);
    CleanupStack::PushL( inRefer);
    iInRefers.Append( inRefer );
	NotifyUser( KMiscIncomingRefer );
	EngineStateChangedL();	
	CleanupStack::Pop( inRefer );
	
	return *inRefer;   
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::OutRefers
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineOutRefer>& 
        CMCETestUIEngine::OutRefers( )
    {
    return iOutRefers;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::InRefers
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMCETestUIEngineInRefer>& 
        CMCETestUIEngine::InRefers( )
    {
    return iInRefers;
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::ReferParams
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngine::ReferParamsL(TReferParams &aReferParams)
{
	if ( iUserInterface )
        {
        return iUserInterface->ReferParamsL( aReferParams);
        }     
    return EFalse;    
}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::ReferToParams
// -----------------------------------------------------------------------------
//

TBool CMCETestUIEngine::ReferToParamsL(TReferParams &aReferParams)
{
	if ( iUserInterface )
        {
        return iUserInterface->ReferToParamsL( aReferParams);
        }     
    return EFalse;    
}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::NextOutReferId
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEngine::NextOutReferId()
    {
    return iNextOutReferId++;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::NextInReferId
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEngine::NextInReferId()
    {
    return iNextInReferId++;
    }


// -----------------------------------------------------------------------------
// CMCETestUIEngine::DeleteAudioCodec
// -----------------------------------------------------------------------------
//

EXPORT_C void CMCETestUIEngine::DeleteAudioCodec( CMCETestUIEngineCodec& aAudioCodec)
	{

	 for ( TInt k = 0; k < iOutSessions.Count(); ++k )
        {
         const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iOutSessions[k]->CurrentAudioStreams();
            
         for ( TInt i = 0; i < audioStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineCodec>& codecs = 
            	audioStreams[i]->CurrentCodecs();
            	
         	for ( TInt j = 0; j < codecs.Count(); ++j )
				{
					if ( codecs[j] == &aAudioCodec )
		            {
		            audioStreams[i]->AudioStream().RemoveCodecL(codecs[j]->Codec());
		            delete codecs[j];
		            codecs.Remove( j );
		            codecs.Compress();
		            EngineStateChangedL();
		            return;
		            }
				}
        	}
         
            
        }

	 for ( TInt k = 0; k < iInSessions.Count(); ++k )
        {
         const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iInSessions[k]->CurrentAudioStreams();
            
        for ( TInt i = 0; i < audioStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineCodec>& codecs = 
            	audioStreams[i]->CurrentCodecs();
            
			for ( TInt j = 0; j < codecs.Count(); ++j )
				{
					if ( codecs[j] == &aAudioCodec )
		            {
		            audioStreams[i]->AudioStream().RemoveCodecL(codecs[j]->Codec());
		            delete codecs[j];
		            codecs.Remove( j );
		            codecs.Compress();
		            EngineStateChangedL();
		            return;
		            }
				}
        	}
         
            
        }
		
	}
 
// -----------------------------------------------------------------------------
// CMCETestUIEngine::DeleteVideoCodec
// -----------------------------------------------------------------------------
//

EXPORT_C void CMCETestUIEngine::DeleteVideoCodec( CMCETestUIEngineVideoCodec& aVideoCodec)
	{

	 for ( TInt k = 0; k < iOutSessions.Count(); ++k )
        {
         const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iOutSessions[k]->CurrentVideoStreams();
            
         for ( TInt i = 0; i < videoStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            	videoStreams[i]->CurrentCodecs();
            	
         	for ( TInt j = 0; j < codecs.Count(); ++j )
				{
					if ( codecs[j] == &aVideoCodec )
		            {
		            videoStreams[i]->VideoStream().RemoveCodecL(codecs[j]->Codec());
		            delete codecs[j];
		            codecs.Remove( j );
		            codecs.Compress();
		            EngineStateChangedL();
		            return;
		            }
				}
        	}
         
            
        }

	 for ( TInt k = 0; k < iInSessions.Count(); ++k )
        {
         const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iInSessions[k]->CurrentVideoStreams();
            
        for ( TInt i = 0; i < videoStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            	videoStreams[i]->CurrentCodecs();
            
			for ( TInt j = 0; j < codecs.Count(); ++j )
				{
					if ( codecs[j] == &aVideoCodec )
		            {
		            videoStreams[i]->VideoStream().RemoveCodecL(codecs[j]->Codec());
		            delete codecs[j];
		            codecs.Remove( j );
		            codecs.Compress();
		            EngineStateChangedL();
		            return;
		            }
				}
        	}
         
            
        }
		
	}
       
// -----------------------------------------------------------------------------
// CMCETestUIEngine::IsFCSession
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMCETestUIEngine::IsFCSession()
	{
	return iFCSession;
	}


// -----------------------------------------------------------------------------
// CMCETestUIEngine::SetFCSession
// -----------------------------------------------------------------------------
//
EXPORT_C void CMCETestUIEngine::SetFCSession(TBool aFCSession)
	{
	iFCSession = aFCSession;
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateMceSettingsL
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::CreateMceSettingsL()
	{
	if ( iUserInterface )
        {
        return iUserInterface->CreateMceSettingsL();
        }
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateMceValueSettingsL
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::CreateMceValueSettingsL()
	{
	if ( iUserInterface )
        {
        return iUserInterface->CreateMceValueSettingsL();
        }
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateMceSettingsL
// -----------------------------------------------------------------------------
//	

EXPORT_C TBool CMCETestUIEngine::StartDTMFToneL( TChar& aChar )
	{
	if ( iUserInterface )
        {
        return iUserInterface->StartDTMFToneL( aChar );
        }
    return EFalse;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateMceSettingsL
// -----------------------------------------------------------------------------
//	

EXPORT_C TBool CMCETestUIEngine::SendDTMFToneL( TChar& aChar, TInt& aInterval )
	{
	if ( iUserInterface )
        {
        return iUserInterface->SendDTMFToneL( aChar, aInterval );
        }
    return EFalse;
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::GetCodecTypeL
// -----------------------------------------------------------------------------
//	

EXPORT_C TBool CMCETestUIEngine::GetCodecTypeL(TInt& aCodecIndex )
	{
	if ( iUserInterface )
        {
        return iUserInterface->GetCodecTypeL( aCodecIndex );
        }
    return EFalse;
	}	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::GetIapIdL
// -----------------------------------------------------------------------------
//	

EXPORT_C TBool CMCETestUIEngine::GetIapIdL(TInt& aIapid )
	{
	if ( iUserInterface )
        {
        return iUserInterface->GetIapIdL( aIapid );
        }
    return EFalse;
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::GetIapIdL
// -----------------------------------------------------------------------------
//	

void CMCETestUIEngine::GetIapIdFromAPList()
	{
  
    iIapId = -1;
    TCommDbConnPref prefs;
  
    TInt error( PrepareConnection( prefs, iIapId ) );
 	 if ( error != KErrNone )
        {
        return ;
        }
    error = iConn.Start( prefs );
    if ( error != KErrNone )
        {
        iConn.Close();
        iSocketServ.Close();
   
        }
    TUint32 aValue;
    
    error = iConn.GetIntSetting(_L("IAP\\Id"), aValue);
	
	if ( error != KErrNone )
	    {
	    iConn.Close();
	    iSocketServ.Close();
	    }
   	else
       {
       iIapId = aValue;
       }
    }
	
// ---------------------------------------------------------------------------
// TInt CMCETestUIEngine::PrepareConnection()
// ---------------------------------------------------------------------------
//
TInt CMCETestUIEngine::PrepareConnection( TCommDbConnPref& aPrefs,
										  TInt /*aIapId*/ )
    {
    TInt err( KErrCouldNotConnect );

    aPrefs.SetDirection( ECommDbConnectionDirectionOutgoing );

    err = iSocketServ.Connect();

    if ( err != KErrNone )
        {
        iSocketServ.Close();
        return err;
        }

    err = iConn.Open( iSocketServ );

    if ( err != KErrNone )
        {
        iConn.Close(); 
        iSocketServ.Close();
        return err;
        }

    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngine::GetRect
// -----------------------------------------------------------------------------
//	

EXPORT_C TRect CMCETestUIEngine::GetRect()
	{
	if ( iUserInterface )
    	{
    	return iUserInterface->GetRect();
    	}
    return TRect();
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::ShowCurrentCryptoContexts
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::ShowCurrentCryptoContextsL(TBool aCrypto32,TBool aCrypto80)
	{
	if ( iUserInterface )
    	{
    	return iUserInterface->ShowCurrentCryptoContextsL(aCrypto32, aCrypto80 );
    	}
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::ShowCurrentCryptoContexts
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::ShowSupportedCryptoContextsL(TBool aCrypto32,TBool aCrypto80)
	{
	if ( iUserInterface )
    	{
    	return iUserInterface->ShowSupportedCryptoContextsL(aCrypto32, aCrypto80 );
    	}
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::ShowCurrentCryptoContexts
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::GetCryptoContextsL(TBool& aCrypto32,TBool& aCrypto80)
	{
	if ( iUserInterface )
    	{
    	return iUserInterface->GetCryptoContextsL(aCrypto32, aCrypto80 );
    	}
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::CreateMceSettingsL
// -----------------------------------------------------------------------------
//	

EXPORT_C TBool CMCETestUIEngine::SendDTMFToneSequenceL( TDes& aSequence  )
	{
	if ( iUserInterface )
        {
        return iUserInterface->SendDTMFToneSequenceL( aSequence );
        }
    return EFalse;
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::IsVideoEnabled
// -----------------------------------------------------------------------------
//	

EXPORT_C TBool CMCETestUIEngine::IsVideoEnabled()
	{
	if ( iUserInterface )
        {
        return iUserInterface->IsVideoEnabled();
        }
    return EFalse;
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::EnableVideo
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::EnableVideo(CMCETestUIEngineSession& aSession)
	{
	const RPointerArray<CMCETestUIEngineVideoStream>& streams = aSession.VideoStreamsL();

    for (TInt i = 0; i < streams.Count(); i ++)
        {
	 //   CMceMediaStream* stream = (CMceMediaStream*) streams[i];  
	     CMCETestUIEngineVideoStream* stream = (CMCETestUIEngineVideoStream*) streams[i];  
	//	const RPointerArray<CMceMediaSink>& sinks = stream->Sinks();
	    const RPointerArray<CMCETestUIEngineVideoSink>& sinks = stream->SinksL();
	    const RPointerArray<CMCETestUIEngineVideoSource>& sources = stream->SourcesL();

		if (sinks[0]->Type() == KMceDisplaySink &&
		    sources[0]->Type() == KMceCameraSource )
    		{
		    sinks[0]->EnableL();
		  	}
	    }

	if ( iUserInterface )
        {
        return iUserInterface->EnableVideo();
        }
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::DisableVideo
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::DisableVideo(CMCETestUIEngineSession& aSession)
	{
	
	const RPointerArray<CMCETestUIEngineVideoStream>& streams = aSession.VideoStreamsL();

    for (TInt i = 0; i < streams.Count(); i ++)
        {
	  //  CMceMediaStream* stream = (CMceMediaStream*) streams[i];  
	    CMCETestUIEngineVideoStream* stream = (CMCETestUIEngineVideoStream*) streams[i];  
	//	const RPointerArray<CMceMediaSink>& sinks = stream->Sinks();

		 const RPointerArray<CMCETestUIEngineVideoSink>& sinks = stream->SinksL();
		 const RPointerArray<CMCETestUIEngineVideoSource>& sources = stream->SourcesL();
		if (sinks[0]->Type() == KMceDisplaySink &&
		    sources[0]->Type() == KMceCameraSource )
    		{
		    sinks[0]->DisableL();
		  	}
	       
		  }
	if ( iUserInterface )
        {
        return iUserInterface->DisableVideo();
        }
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::EnableReceivingVideo
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::EnableReceivingVideo(CMCETestUIEngineSession& aSession)
	{
	const RPointerArray<CMCETestUIEngineVideoStream>& streams = aSession.VideoStreamsL();

    for (TInt i = 0; i < streams.Count(); i ++)
        {
	 //   CMceMediaStream* stream = (CMceMediaStream*) streams[i];  
	     CMCETestUIEngineVideoStream* stream = (CMCETestUIEngineVideoStream*) streams[i];  
	//	const RPointerArray<CMceMediaSink>& sinks = stream->Sinks();
	    const RPointerArray<CMCETestUIEngineVideoSink>& sinks = stream->SinksL();
	    const RPointerArray<CMCETestUIEngineVideoSource>& sources = stream->SourcesL();

		if (sinks[0]->Type() == KMceDisplaySink &&
		    sources[0]->Type() == KMceRTPSource )
    		{
		    sinks[0]->EnableL();
		  	}
	    }

	if ( iUserInterface )
        {
        return iUserInterface->EnableVideo();
        }
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::DisableReceivingVideo
// -----------------------------------------------------------------------------
//	

EXPORT_C void CMCETestUIEngine::DisableReceivingVideo(CMCETestUIEngineSession& aSession)
	{
	
	const RPointerArray<CMCETestUIEngineVideoStream>& streams = aSession.VideoStreamsL();

    for (TInt i = 0; i < streams.Count(); i ++)
        {
	  //  CMceMediaStream* stream = (CMceMediaStream*) streams[i];  
	    CMCETestUIEngineVideoStream* stream = (CMCETestUIEngineVideoStream*) streams[i];  
	//	const RPointerArray<CMceMediaSink>& sinks = stream->Sinks();

		 const RPointerArray<CMCETestUIEngineVideoSink>& sinks = stream->SinksL();
		 const RPointerArray<CMCETestUIEngineVideoSource>& sources = stream->SourcesL();
		if (sinks[0]->Type() == KMceDisplaySink &&
		    sources[0]->Type() == KMceRTPSource )
    		{
		    sinks[0]->DisableL();
		  	}
	       
		  }
	if ( iUserInterface )
        {
        return iUserInterface->DisableVideo();
        }
	}
// -----------------------------------------------------------------------------
// CMCETestUIEngine::CsStatusChangedL
// -----------------------------------------------------------------------------
//	

void CMCETestUIEngine::CsStatusChangedL( TPoCOmaCsState aStatus )
    {   
    MCE_TESTUI_DEBUG3( "CsStatusChangedL, Status: ", aStatus );
    switch ( aStatus )
        {
        case EPoCOmaCallStateDialling:
        case EPoCOmaCallStateRinging:
        case EPoCOmaCallStateAnswering:
            // do hold
            //NotifyUser( KCallStateActivated );
            //EngineStateChangedL();
            TRAPD( err, DoHoldUnholdL( ETrue ) );
            if( err != KErrNone )
                {
                MCE_TESTUI_DEBUG3( "Error in doing hold", err );
                }
            break;
        case EPoCOmaCallStateDisconnecting:
        case EPoCOmaCallStateNone:
            // do unhold
            //NotifyUser( KCallStateDeActivated );
            //EngineStateChangedL();
            TRAPD( err2, DoHoldUnholdL( EFalse ));
            if( err2 != KErrNone )
                {
                MCE_TESTUI_DEBUG3( "Error in doing unhold", err2 );
                }
            break;
        default:
            break;
        
        }
    
    }
// -----------------------------------------------------------------------------
// CMCETestUIEngine::DoHoldUnholdL
// -----------------------------------------------------------------------------
//	
	
void CMCETestUIEngine::DoHoldUnholdL( TBool aHold )
    {
    MCE_TESTUI_DEBUG3( "DoHoldUnholdL, Hold or Unhold: ", aHold );

    for ( TInt k = 0; k < iOutSessions.Count(); ++k )
        {
        DoEnableDisableL( iOutSessions[k]->OutSession(), aHold );
        }
	for ( TInt k = 0; k < iInSessions.Count(); ++k )
        {
        DoEnableDisableL( iInSessions[k]->InSession(), aHold );
        }
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngine::DoHoldUnholdL
// -----------------------------------------------------------------------------
//	   
void CMCETestUIEngine::DoEnableDisableL( CMceSession& aSession, TBool aHold ) 
    {
    const RPointerArray<CMceMediaStream>& streams = aSession.Streams();
     
    for ( TInt i = 0; i < streams.Count(); ++i )
		{
		CMceMediaStream* mediaStream = 
                                    static_cast<CMceMediaStream*>(streams[i]);
        
        CMceMediaSource* source = mediaStream->Source();
        User::LeaveIfNull( source );
        
        if( mediaStream->Type() == KMceAudio && source->Type() == KMceRTPSource )
            {
            if( aHold )
                {
                MCE_TESTUI_DEBUG1(" Disabling downlink " );
                mediaStream->DisableL();
                }
            else
                {
                MCE_TESTUI_DEBUG1(" Enabling downlink " );
                mediaStream->EnableL();
                }
            }
            
        if( mediaStream->BoundStream() )
            {
            CMceMediaStream* boundStream = &mediaStream->BoundStreamL();
            
            CMceMediaSource* boundsource = boundStream->Source();
            User::LeaveIfNull( boundsource );
            
            if( boundStream->Type() == KMceAudio && boundsource->Type() == KMceRTPSource )
                {
                if( aHold )
                    {
                    MCE_TESTUI_DEBUG1(" Disabling downlink " );
                    boundStream->DisableL();
                    }
                else
                    {
                    MCE_TESTUI_DEBUG1(" Enabling downlink " );
                    boundStream->EnableL();
                    }
                }
            }
		}   
    aSession.UpdateL();
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngine::NotifyStreamStateChangeL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::NotifyStreamStateChangeL( CMceMediaStream& aStream ) 
    {
    TInt state = aStream.State();

	TBuf16<100> buf16;
	buf16.Append(KUserNotificationStreamStateChanged );
	buf16.Append(KLeftParenthesis);
		
	switch( state ) 
		{
		case CMceMediaStream::EUninitialized: 
		   	buf16.Append(KStreamUninitialized); 
		    break;
		case CMceMediaStream::EInitialized: 
		    buf16.Append(KStreamInitialized); 
		    break;
		case CMceMediaStream::EBuffering: 
		    buf16.Append(KStreamBuffering); 
		    break;
		case CMceMediaStream::EIdle: 
		    buf16.Append(KStreamIdle); 
		    break;
		case CMceMediaStream::EStreaming: 
		    buf16.Append(KStreamStreaming); 
		    break;
		case CMceMediaStream::EDisabled: 
		    buf16.Append(KStreamDisabled); 
		    break;
		case CMceMediaStream::ENoResources: 
		    buf16.Append(KStreamNoResources); 
		    break;
		case CMceMediaStream::ETranscodingRequired: 
		    buf16.Append(KStreamTranscodingRequired); 
		    break;
		case CMceMediaStream::ETranscoding: 
		    buf16.Append(KStreamTranscoding); 
		    break;
		default: 
		    buf16.Append(KSessionStateUnknown); 
		}
	buf16.Append(KRightParenthesis);
	NotifyUser(buf16);
    }



// -----------------------------------------------------------------------------
// CMCETestUIEngine::AddTransactionIdL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::AddTransactionIdL( TMceTransactionId aId )
	{
	if ( iTransactionIds.Find( aId ) == KErrNotFound )
		{
		iTransactionIds.AppendL( aId );
		}
	}
	
// -----------------------------------------------------------------------------
// CMCETestUIEngine::TransactionIds
// -----------------------------------------------------------------------------
//
const RArray< TMceTransactionId >& CMCETestUIEngine::TransactionIds() const
	{
	return iTransactionIds;
	}


// -----------------------------------------------------------------------------
// CMCETestUIEngine::RemoveTransactionId
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::RemoveTransactionId( TMceTransactionId aId )
	{
	TInt index = iTransactionIds.Find( aId );
	if ( index != KErrNotFound )
		{
		iTransactionIds.Remove( index );
		}
	}

// -----------------------------------------------------------------------------
// CMCETestUIEngine::GetJitterBufferValues
// Default values are converted to more suitable in case of video session
// -----------------------------------------------------------------------------
//
void CMCETestUIEngine::GetJitterBufferValues( 
    CMceMediaStream* aMediaStream, 
    TInt& aJitbufSize, 
    TInt& aJitbufThreshold, 
    TInt& aJitbufTimeout, 
    TBool aSessionTypeKnownToBeVideo )
    {
    TBool defaultValues( ETrue );
    TBool videoSession( aSessionTypeKnownToBeVideo );
    if ( aMediaStream && !videoSession )
        {
        videoSession = ( aMediaStream->Type() == KMceVideo );
        }
    
    // If two way video session, default value for jitterbuffer is altered    
    if ( videoSession && aJitbufThreshold == KJitterBufferTreshold )
        {
        if ( aMediaStream && aMediaStream->Type() == KMceVideo )
            {
            aJitbufThreshold = KJitterBufferTresholdRealTimeVideoVideo;
            aJitbufSize = iJitterBufSize;  
            }
        else
            {
            aJitbufThreshold = KJitterBufferTresholdRealTimeVideoAudio;
            aJitbufSize = KJitterBufferSizeRealTimeVideoAudio;  
            }     
        aJitbufTimeout = iJitterBufTimeout; 
        defaultValues = EFalse;
        }
            
    if ( defaultValues )
        {
        aJitbufSize = iJitterBufSize;
	    aJitbufThreshold = iJitterBufTreshold;
	    aJitbufTimeout = iJitterBufTimeout;
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngine::IsSendRecvVideo
// -----------------------------------------------------------------------------
//
TBool CMCETestUIEngine::IsSendRecvVideo( CMceSession* aSession )
    {
    TBool sendRecvVideo( EFalse );
    if ( aSession )
        {
        const RPointerArray<CMceMediaStream>& streams = aSession->Streams();
        for ( TInt i = 0; i < streams.Count() && !sendRecvVideo; i++ )
            {
            if ( streams[ i ]->Type() == KMceVideo )
                {
                sendRecvVideo = streams[ i ]->BoundStream();
                }
            }
        }
    return sendRecvVideo;
    }
    
// End of file
