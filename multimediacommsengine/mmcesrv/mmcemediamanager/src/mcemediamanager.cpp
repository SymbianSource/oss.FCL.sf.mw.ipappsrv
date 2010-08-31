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




// INCLUDE FILES
#include <AudioPreference.h>

#include <mmcccodecinformationfactory.h>
#include <mmcccodecinformation.h>
#include <mmccinterface.h>

#include <sdpdocument.h>
#include <sdpcodecstringpool.h>

#include "mcemediasource.h"
#include "mcemediastream.h"
#include "mceaudiostream.h"
#include "mcemicsource.h"
#include "mcertpsource.h"
#include "mcertpsink.h"
#include "mcecamerasource.h"
#include "mcefilesource.h"
#include "mcedisplaysink.h"
#include "mceaudiocodec.h"
#include "mceamrcodec.h"
#include "mcenatpluginmanager.h"

#include "mcecssessionimplementation.h"
#include "mcemediamanager.h"
#include "mcemediaobserver.h"
#include "mcesdpsession.h"
#include "mcemediadefs.h"

#include "mcecommediastream.h"
#include "mcecomaudiostream.h"
#include "mcecomsession.h"
#include "mcecomaudiocodec.h"
#include "mcecomvideocodec.h"
#include "mcecommediasource.h"
#include "mcecommediasink.h"
#include "mcecomrtpsource.h"
#include "mcecomdisplaysink.h"
#include "mcecomcamerasource.h"
#include "mcecomrtpsink.h"
#include "mcecomspeakersink.h"

#include "mcemmlogs.h"

#include "mcecomfactory.h"
#include "mcesrvstreamiterator.h"
#include "mcesrvsink.h"
#include "mcesrvsource.h"
#include "mceevents.h"
#include "mcemediastate.h"

#include "mceaudiosdpcodec.h"
#include "mcevideosdpcodec.h"
#include "cleanupresetanddestroy.h"
#include "mcedtmfhandler.h"
#include "mcecomendpointproxy.h"

#define MCE_HEAP_CONSTRUCT_LC( type, variable ) \
type* variable = new ( ELeave ) type; \
CleanupStack::PushL( variable );

#define MCE_HEAP_CONSTRUCT_WITH_PARAM_LC( type, variable, param ) \
type* variable = new ( ELeave ) type( param ); \
CleanupStack::PushL( variable );

const TInt KMceMaxFileName8 = KMceMaxFileNameLength * 4;


// ================ MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CMceMediaManager::CMceMediaManager
// ---------------------------------------------------------
CMceMediaManager::CMceMediaManager() 
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::ConstructL
// ---------------------------------------------------------
void CMceMediaManager::ConstructL( MMceMediaGeneralObserver& aObserver )
    {  
    iObserver = &aObserver;
    iMccInterface = CMccInterface::NewL( *this );
    iMccSecureInterface = CMccSecureInterface::NewL( *iMccInterface );
    
    SdpCodecStringPool::OpenL();
    
    iNatPluginManager = CMceNatPluginManager::NewL();
    
    CMceSdpCodec* sdpCodec = NULL;
 
    //audio SDP codec
    sdpCodec = CMceAudioSdpCodec::NewL();
    CleanupStack::PushL( sdpCodec );
    iSdpCodecs.AppendL( sdpCodec );
    CleanupStack::Pop( sdpCodec );
    

    //video SDP codec
    sdpCodec = CMceVideoSdpCodec::NewL();
    CleanupStack::PushL( sdpCodec );
    iSdpCodecs.AppendL( sdpCodec );
    CleanupStack::Pop( sdpCodec );
    
    }


// ---------------------------------------------------------
// CMceMediaManager::SdpCodec
// ---------------------------------------------------------
CMceSdpCodec* CMceMediaManager::SdpCodec( const CSdpMediaField& aMedia )
    {
    CMceSdpCodec* sdpCodec = NULL;
    TInt index = 0;
    while( !sdpCodec && index < iSdpCodecs.Count() )
        {
        sdpCodec = iSdpCodecs[ index++ ];
        sdpCodec = sdpCodec->Decodes( aMedia ) ? sdpCodec : NULL;
        }
    
    return sdpCodec;
    
    }


// ---------------------------------------------------------
// CMceMediaManager::SdpCodec
// ---------------------------------------------------------
CMceSdpCodec* CMceMediaManager::SdpCodec( CMceComMediaStream& aStream )
    {
    CMceSdpCodec* sdpCodec = NULL;
    TInt index = 0;
    while( !sdpCodec && index < iSdpCodecs.Count() )
        {
        sdpCodec = iSdpCodecs[ index++ ];
        sdpCodec = sdpCodec->Encodes( aStream ) ? sdpCodec : NULL;
        }
    
    return sdpCodec;
    
    }


// ---------------------------------------------------------
// CMceMediaManager::NewL
// ---------------------------------------------------------
 CMceMediaManager* CMceMediaManager::NewL( 
                                    MMceMediaGeneralObserver& aObserver )
    {
    CMceMediaManager* self = NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------
// CMceMediaManager::NewLC
// ---------------------------------------------------------
 CMceMediaManager* CMceMediaManager::NewLC( 
                                    MMceMediaGeneralObserver& aObserver )
    {
    CMceMediaManager* self = new ( ELeave ) CMceMediaManager;
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    return self;
    }


// ---------------------------------------------------------
// CMceMediaManager::~CMceMediaManager
// ---------------------------------------------------------    
 CMceMediaManager::~CMceMediaManager()
    {
    MCEMM_DEBUG("CMceMediaManager::~CMceMediaManager(), Entry ");
    
    delete iMccSecureInterface;
    iMccSecureInterface=NULL;
    
    delete iNatPluginManager;
    iNatPluginManager = NULL;

    delete iMccInterface;
    iMccInterface = NULL;
    iMceSdpSessions.ResetAndDestroy();
    iSdpCodecs.ResetAndDestroy();
    iTranscodeSessions.ResetAndDestroy();
    SdpCodecStringPool::Close();
    MCEMM_DEBUG("CMceMediaManager::~CMceMediaManager(), Exit ");
    
    }   
    
    

// ---------------------------------------------------------
// CMceMediaManager::CreateSessionL
// ---------------------------------------------------------    
CMceSdpSession& CMceMediaManager::CreateSessionL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("CMceMediaManager::CreateSessionL(), Entry ");
    
    CMceSdpSession* sdpSession = CMceSdpSession::NewL( aSession, *this );
    CleanupStack::PushL( sdpSession );

    aSession.AttachSDPSessionL( *sdpSession );
    User::LeaveIfError (iMceSdpSessions.Append( sdpSession ));
    CleanupStack::Pop( sdpSession ); 

    MCEMM_DEBUG("CMceMediaManager::CreateSessionL(), Exit ");

    return *sdpSession;
    }

// ---------------------------------------------------------
// CMceMediaManager::NatMediaManager
// ---------------------------------------------------------    
CMceNatPluginManager& CMceMediaManager::NatPluginManager()
    {
    return *iNatPluginManager;
    }

// ---------------------------------------------------------
// CMceMediaManager::EncodeL
// ---------------------------------------------------------    
CSdpDocument* CMceMediaManager::EncodeL( CMceComSession& aSession )
    {

    MCEMM_DEBUG("CMceMediaManager::EncodeL(), Entry ");

    if ( !SdpSession( aSession ) )
        {
        CreateSessionL( aSession );
        }

	TMceMMState negotationStateId = aSession.NegotiationState().Id();
    MCEMM_DEBUG_STATE( "current state", 
                        negotationStateId );
    CSdpDocument* sdp = aSession.NegotiationState().EncodeL();
    
    MCEMM_DEBUG("CMceMediaManager::EncodeL(), Exit ");
    
    return sdp;
    
    }

// ---------------------------------------------------------
// CMceMediaManager::DecodeL
// ---------------------------------------------------------    
TMceSipWarningCode CMceMediaManager::DecodeL( 
                            CMceComSession& aSession,
                            CSdpDocument& aSdpDocument )
    {
    MCEMM_DEBUG("CMceMediaManager::DecodeL(), Entry ");

    if ( !SdpSession( aSession ) )
        {
        CreateSessionL( aSession );
        }
	TMceMMState negotationStateId = aSession.NegotiationState().Id();
    MCEMM_DEBUG_STATE( "CMceMediaManager::DecodeL(): current state", 
                        negotationStateId );
    TMceSipWarningCode result = 
        aSession.NegotiationState().DecodeL( aSdpDocument );

    MCEMM_DEBUG("CMceMediaManager::DecodeL(), Exit ");
        
    return result; 
    
    }

// ---------------------------------------------------------
// CMceMediaManager::UpdateL
// ---------------------------------------------------------    
TMceReturnStatus CMceMediaManager::UpdateL( CMceComSession& aSession )
    {
    MCEMM_DEBUG("CMceMediaManager::UpdateL(), Entry ");

    TMceReturnStatus status = KMceAsync;
    
    if ( !SdpSession( aSession ) )
        {
        CreateSessionL( aSession );
        }
	TMceMMState negotationStateId = aSession.NegotiationState().Id();
    MCEMM_DEBUG_STATE( "CMceMediaManager::UpdateL(): current state", 
                       negotationStateId );
    aSession.NegotiationState().UpdateL();
    status = !aSession.NegotiationState().CallbackAllowed() ? 
             KMceReady : KMceAsync;
    MCEMM_DEBUG("CMceMediaManager::UpdateL(), Exit ");
    return status;
    }
              
// ---------------------------------------------------------
// CMceMediaManager::ReserveL
// ---------------------------------------------------------    

TMceReturnStatus CMceMediaManager::ReserveL( CMceComSession& aSession, 
                                             const TDesC8& /*aToken*/ )
    {
    MCEMM_DEBUG("CMceMediaManager::ReserveL(), Entry ");
    
    TMceReturnStatus status = KMceReady;
    
    CMceSdpSession* sdpSession = SdpSession( aSession );
    __ASSERT_ALWAYS( sdpSession, User::Leave( KErrNotFound ) );
    
    MCEMM_DEBUG_STATE_1( "CMceMediaManager::ReserveL(): current state", 
                        aSession );
    if ( sdpSession )
        {
        status = aSession.NegotiationState().ReserveL();
        }
    
    
    MCEMM_DEBUG("CMceMediaManager::ReserveL(), Exit ");

    return status;
    
    }

// ---------------------------------------------------------
// CMceMediaManager::ReservationNeeded
// ---------------------------------------------------------    
TBool CMceMediaManager::ReservationNeeded( CMceComSession& aSession )
    {
    MCEMM_DEBUG("CMceMediaManager::ReservationNeeded(), Entry ");
    TBool reservationNeeded = EFalse;
    CMceSdpSession* sdpSession = SdpSession( aSession );
    
    MCEMM_DEBUG_STATE_1( "CMceMediaManager::ReservationNeeded(): current state", 
                        aSession );
    if ( sdpSession )
        {
        reservationNeeded = aSession.NegotiationState().ReservationNeeded();
        }
        
    MCEMM_DEBUG("CMceMediaManager::ReservationNeeded(), Exit ");
    return reservationNeeded;
    
    }

// ---------------------------------------------------------
// CMceMediaManager::NeedToNegotiate
// ---------------------------------------------------------    
TBool CMceMediaManager::NeedToNegotiate( CMceComSession& aSession,
                                         CSdpDocument* aSdpDocument )
    {
    MCEMM_DEBUG("CMceMediaManager::NeedToNegotiate(), Entry ");
    TBool needToNegotiate = EFalse;
    CMceSdpSession* sdpSession = SdpSession( aSession );

    MCEMM_DEBUG_STATE_1( "CMceMediaManager::NeedToNegotiate(): current state", 
                       aSession );
    if ( sdpSession )
        {
        needToNegotiate = aSession.NegotiationState().NeedToNegotiate( aSdpDocument );
        }
    
    MCEMM_DEBUG("CMceMediaManager::NeedToNegotiate(), Exit ");
    return needToNegotiate;
    
    }


// ---------------------------------------------------------
// CMceMediaManager::ConfirmationNeeded
// --------------------------------------------------------- 
//   
TBool CMceMediaManager::ConfirmationNeeded( CMceComSession& aSession )
    {
    MCEMM_DEBUG("CMceMediaManager::ConfirmationNeeded(), Entry ");
    TBool needToNegotiate = EFalse;
    CMceSdpSession* sdpSession = SdpSession( aSession );

    MCEMM_DEBUG_STATE_1( "CMceMediaManager::ConfirmationNeeded(): current state", 
                        aSession);
    if ( sdpSession )
        {
        needToNegotiate = aSession.NegotiationState().ConfirmationNeeded();
        }
    
    MCEMM_DEBUG("CMceMediaManager::ConfirmationNeeded(), Exit ");
    return needToNegotiate;
    }
    

// ---------------------------------------------------------
// CMceMediaManager::GetRole
// --------------------------------------------------------- 
//   

TMceNegotiationRole CMceMediaManager::GetRole( CMceComSession& aSession )
	{
	MCEMM_DEBUG("CMceMediaManager::GetRole()");
	
	return aSession.NegotiationState().Role();
	}


// ---------------------------------------------------------
// CMceMediaManager::CreateRejectedAnswer
// --------------------------------------------------------- 
//   
 void CMceMediaManager::CreateRejectedAnswer( CMceComSession& aSession )
    { 
    MCEMM_DEBUG("CMceMediaManager::CreateRejectedAnswer(), Entry ");

    CMceSdpSession* sdpSession = SdpSession( aSession );
    if( !sdpSession )
        {
        return;
        }
    TRAPD( err, sdpSession->CreateAnswerL( aSession, ETrue ) );
    if( err != KErrNone )
        {
        MCEMM_DEBUG_DVALUE( "CMceMediaManager::CreateRejectedAnswer(): \
ERROR: Creating rejected answer", err );
        iObserver->MediaError( err );
        }
    MCEMM_DEBUG("CMceMediaManager::CreateRejectedAnswer(), Exit ");
    }


// ---------------------------------------------------------
// CMceMediaManager::PauseL 
// ---------------------------------------------------------   
// 
 void CMceMediaManager::PauseL( CMceComMediaSink& aSink )
    {
    MCEMM_DEBUG("CMceMediaManager::PauseL() - sink, Entry ");
    CMceComSession* session = aSink.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aSink );
    CMceSrvStream* stream = NULL;
    
    while( streams.Next( stream ) )
        {
        if ( !stream->Sink().DisableL( *stream ) )
            {
            SynchronizeMccStreamL( *stream, 0, stream->Sink().Id() );
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::PauseL() - sink, Exit ");
    }
    
// ---------------------------------------------------------
// CMceMediaManager::PauseL
// ---------------------------------------------------------  
// 
 void CMceMediaManager::PauseL( CMceComMediaSource& aSource )
    { 
    MCEMM_DEBUG("CMceMediaManager::PauseL() - source, Entry ");
    CMceComSession* session = aSource.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aSource );
    CMceSrvStream* stream = NULL;
    
    while( streams.Next( stream ) )
        {
        if ( !stream->Source().DisableL( *stream ) )
            {
            SynchronizeMccStreamL( *stream, 0, stream->Source().Id() );
            }
        }

    MCEMM_DEBUG("CMceMediaManager::PauseL() - source, Exit ");
    }
    
    
// ---------------------------------------------------------
// CMceMediaManager::ResumeL 
// ---------------------------------------------------------    
//
 void CMceMediaManager::ResumeL( CMceComMediaSink& aSink )
    { 
    MCEMM_DEBUG("CMceMediaManager::ResumeL() - sink, Entry ");
    CMceComSession* session = aSink.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aSink );
    CMceSrvStream* stream = NULL;
    
    while( streams.Next( stream ) )
        {
        if ( !stream->Sink().EnableL( *stream ) )
            {
            MCEMM_DEBUG_STREAM( "CMceMediaManager::ResumeL() - sink: \
synchronize stream", *stream );
            SynchronizeMccStreamL( *stream, stream->Sink().Id(), 0 );
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::ResumeL() - sink, Exit ");
    }
    
    
// ---------------------------------------------------------
// CMceMediaManager::ResumeL 
// ---------------------------------------------------------  
//  
 void CMceMediaManager::ResumeL( CMceComMediaSource& aSource )
    { 
    MCEMM_DEBUG("CMceMediaManager::ResumeL() - source, Entry ");
    CMceComSession* session = aSource.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aSource );
    CMceSrvStream* stream = NULL;
    
    while( streams.Next( stream ) )
        {
        if ( !stream->Source().EnableL( *stream ) )
            {
            MCEMM_DEBUG_STREAM( "CMceMediaManager::ResumeL() - source: \
synchronize stream", *stream );
            SynchronizeMccStreamL( *stream, stream->Source().Id(), 0 );
            }
        }

    MCEMM_DEBUG("CMceMediaManager::ResumeL() - source, Exit ");
    }


// ---------------------------------------------------------
// CMceMediaManager::StartInactivityTimerL 
// ---------------------------------------------------------    
//
void CMceMediaManager::StartInactivityTimerL( 
                            const CMceComRtpSource& aRtpSource, 
                            TUint32 aInactivityTimeout )
    { 
    MCEMM_DEBUG("CMceMediaManager::StartInactivityTimerL(), Entry ");
    
    CMceComSession* session = aRtpSource.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aRtpSource );
    CMceSrvStream* stream = NULL;
    
    while( streams.Next( stream ) )
        {
        MCEMM_DEBUG_STREAM( "CMceMediaManager::StartInactivityTimerL(): \
start inactivity timer for stream", *stream );
        User::LeaveIfError( 
            iMccInterface->StartInactivityTimer( 
                                    stream->SessionId(),
                                    stream->LinkId(),
                                    stream->Id(),
                                    aInactivityTimeout ) );
                            
        }

    MCEMM_DEBUG("CMceMediaManager::StartInactivityTimerL(), Exit ");
   
    }
    
// ---------------------------------------------------------
// CMceMediaManager::StopInactivityTimerL 
// ---------------------------------------------------------    
//
void CMceMediaManager::StopInactivityTimerL( 
                    const CMceComRtpSource& aRtpSource )
    { 
    
    MCEMM_DEBUG("CMceMediaManager::StopInactivityTimerL(), Entry ");
    
    CMceComSession* session = aRtpSource.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aRtpSource );
    CMceSrvStream* stream = NULL;
    
    while( streams.Next( stream ) )
        {
        MCEMM_DEBUG_STREAM( "CMceMediaManager::StopInactivityTimerL(): \
stop inactivity timer for stream", *stream );
        User::LeaveIfError( 
            iMccInterface->StopInactivityTimer( 
                                    stream->SessionId(),
                                    stream->LinkId(),
                                    stream->Id() ) );
                            
        }

    MCEMM_DEBUG("CMceMediaManager::StopInactivityTimerL(), Exit ");
   
    }

// ---------------------------------------------------------
// CMceMediaManager::SendRTCPAnyDataL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SendRTCPAnyDataL( const CMceComMediaStream& aMediaStream,
                                         const TDesC8& aData )
    { 
    MCEMM_DEBUG("CMceMediaManager::SendRTCPAnyDataL(), Entry ");
    
    CMceComMediaSink* rtpSink = NULL;
    TInt index = 0;
    while( !rtpSink && index < aMediaStream.iSinks.Count() )
        {
        rtpSink = aMediaStream.iSinks[ index++ ];
        rtpSink = rtpSink->iType != KMceRTPSink ? NULL : rtpSink;
        }

    if ( !rtpSink )
        {
        MCEMM_DEBUG("No target!");
        return;
        }
        
    CMceComSession* session = aMediaStream.Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), *rtpSink );
    CMceSrvStream* stream = NULL;
    
    // Taking some precautions to avoid stack memory overuse
    //
    TMccEvent* event = new ( ELeave ) TMccEvent;
    CleanupStack::PushL( event );
    
    event->iEventCategory = KMccEventCategoryRtcp;
    event->iEventType = KMccRtcpControl;
    
    {       
    TMccRtcpEventData eventData;
    eventData.iRtcpPacketType = KRtcpAnyPacket;
    eventData.iRtcpPacketData.Copy( aData );
    TMccRtcpEventDataPackage eventDataBuf( eventData );
    event->iEventData.Copy( eventDataBuf );
    }
    
    while( streams.Next( stream ) )
        {

        MCEMM_DEBUG_STREAM( "CMceMediaManager::SendRTCPAnyDataL(): \
send rtcp any data to stream", *stream );
        event->iSessionId = stream->SessionId();
        event->iLinkId = stream->LinkId();
        event->iStreamId = stream->Id();
        event->iEndpointId = stream->Sink().Id();
        
        iMccInterface->SendMediaSignalL( *event );
        
        }

    CleanupStack::PopAndDestroy( event );
    
    MCEMM_DEBUG("CMceMediaManager::SendRTCPAnyDataL(), Exit ");
   
    }

    

// ---------------------------------------------------------
// CMceMediaManager::SendRTCPReceiverReportL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SendRTCPReceiverReportL( 
                            const CMceComRtpSource& aRtpSource )
    { 
    MCEMM_DEBUG("CMceMediaManager::SendRTCPReceiverReportL(), Entry ");
    
    CMceComSession* session = aRtpSource.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aRtpSource );
    CMceSrvStream* stream = NULL;
    
    // Taking some precautions to avoid stack memory overuse
    //
    TMccEvent* event = new ( ELeave ) TMccEvent;
    CleanupStack::PushL( event );
    
    event->iEventCategory = KMccEventCategoryRtcp;
    event->iEventType = KMccRtcpControl;
    
    {  
    TMccRtcpEventData eventData;
    eventData.iRtcpPacketType = KRtcpRrPacket;
    TMccRtcpEventDataPackage eventDataBuf( eventData );
    event->iEventData.Copy( eventDataBuf );
    }
    
    while( streams.Next( stream ) )
        {
        
        MCEMM_DEBUG_STREAM( "CMceMediaManager::SendRTCPReceiverReportL(): \
send rtcp receiver report to stream", *stream );
        event->iSessionId = stream->SessionId();
        event->iLinkId = stream->LinkId();
        event->iStreamId = stream->Id();
        event->iEndpointId = stream->Source().Id();

        iMccInterface->SendMediaSignalL( *event );
        
        }

    CleanupStack::PopAndDestroy( event );
    
    MCEMM_DEBUG("CMceMediaManager::SendRTCPReceiverReportL(), Exit ");
    
    }
    
// ---------------------------------------------------------
// CMceMediaManager::SendRTCPSenderReportL 
// ---------------------------------------------------------   
// 
void CMceMediaManager::SendRTCPSenderReportL( 
                            const CMceComRtpSink& aRtpSink )
    { 
    MCEMM_DEBUG("CMceMediaManager::SendRTCPSenderReportL(), Entry ");
    
    CMceComSession* session = aRtpSink.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aRtpSink );
    CMceSrvStream* stream = NULL;
    
    // Taking some precautions to avoid stack memory overuse
    //
    TMccEvent* event = new ( ELeave ) TMccEvent;
    CleanupStack::PushL( event );
    
    event->iEventCategory = KMccEventCategoryRtcp;
    event->iEventType = KMccRtcpControl;
     
    {  
    TMccRtcpEventData eventData;
    eventData.iRtcpPacketType = KRtcpSrPacket;
    TMccRtcpEventDataPackage eventDataBuf( eventData );
    event->iEventData.Copy( eventDataBuf );
    }
    
    while( streams.Next( stream ) )
        {

        MCEMM_DEBUG_STREAM( "CMceMediaManager::SendRTCPSenderReportL(): \
send rtcp sender report to stream", *stream );
        event->iSessionId = stream->SessionId();
        event->iLinkId = stream->LinkId();
        event->iStreamId = stream->Id();
        event->iEndpointId = stream->Sink().Id();

        iMccInterface->SendMediaSignalL( *event );
        
        }

    CleanupStack::PopAndDestroy( event );
    
    MCEMM_DEBUG("CMceMediaManager::SendRTCPSenderReportL(), Exit ");
   
    }   


// ---------------------------------------------------------
// CMceMediaManager::GetMaxVolumeL
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetMaxVolumeL( TInt& aMaxVolume )
    {
    MCEMM_DEBUG("CMceMediaManager::GetMaxVolumeL(), Entry ");
    
    TPckgBuf<TInt> maxVolume( aMaxVolume );
    
    // Max volume can be asked always after MCC creation
    User::LeaveIfError( iMccInterface->GetParameter( 
        0, 0, 0, 0, KMccSpeakerMaxVolume, maxVolume ) );
    
    aMaxVolume = maxVolume();
    
    MCEMM_DEBUG("CMceMediaManager::GetMaxVolumeL(), Exit ");
      
    }   

// ---------------------------------------------------------
// CMceMediaManager::GetVolumeL
// ---------------------------------------------------------    
//
void CMceMediaManager::GetVolumeL( const CMceComMediaSink& aSink, 
                                   TInt& aVolume )
    { 
    MCEMM_DEBUG("CMceMediaManager::GetVolumeL(), Entry ");

    TPckgBuf<TInt> volume( aVolume );
      
    GetMccParameterL( aSink, KMccSpeakerVolume, volume );
    aVolume = volume();
    
    MCEMM_DEBUG("CMceMediaManager::GetVolumeL(), Exit ");
    
    }     

// ---------------------------------------------------------
// CMceMediaManager::SetVolumeL 
// ---------------------------------------------------------  
//  
void CMceMediaManager::SetVolumeL( const CMceComMediaSink& aSink, 
                                  TInt aVolume )
    { 
    MCEMM_DEBUG("CMceMediaManager::SetVolumeL(), Entry ");

    TPckgBuf<TInt> volume( aVolume );
    
    if ( aSink.EndpointProxy() )
        {
        aSink.EndpointProxy()->SetParameterL( aSink, KMccSpeakerVolume, volume );
        }
    else
        {
        SetMccParameterL( aSink, KMccSpeakerVolume, volume );
        }
    
    MCEMM_DEBUG("CMceMediaManager::SetVolumeL(), Exit ");
    
    }   


// ---------------------------------------------------------
// CMceMediaManager::GetAudioRoutingL 
// ---------------------------------------------------------    
//
void CMceMediaManager::GetAudioRoutingL( 
                       const CMceComSpeakerSink& aSpeaker, 
                       TInt& aRoutingDestination )
    { 
    MCEMM_DEBUG("CMceMediaManager::GetAudioRoutingL(), Entry ");

    TPckgBuf<TInt> routingDestination( aRoutingDestination );
    
    GetMccParameterL( aSpeaker, KMccSpeakerRouting, 
                      routingDestination );
                                   
    aRoutingDestination = routingDestination();
    
    MCEMM_DEBUG("CMceMediaManager::GetAudioRoutingL(), Exit ");
    
    }     

// ---------------------------------------------------------
// CMceMediaManager::SetAudioRoutingL
// ---------------------------------------------------------  
//  
 void CMceMediaManager::SetAudioRoutingL( 
                            const CMceComSpeakerSink& aSpeaker, 
                            TInt aRoutingDestination )
    { 
    MCEMM_DEBUG("CMceMediaManager::SetAudioRoutingL(), Entry ");

    TPckgBuf<TInt> routingDestination( aRoutingDestination );
    
    SetMccParameterL( aSpeaker, KMccSpeakerRouting, 
                      routingDestination );
                                   
    MCEMM_DEBUG("CMceMediaManager::SetAudioRoutingL(), Exit ");
    
    }   

// ---------------------------------------------------------
// CMceMediaManager::SetBalanceL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SetBalanceL( 
                        const CMceComMediaSink& aSink, 
                        TInt aBalance )
    { 
    MCEMM_DEBUG("CMceMediaManager::SetBalanceL(), Entry ");

    TPckgBuf<TInt> balance( aBalance );
    
    SetMccParameterL( aSink, KMccSpeakerBalance, balance );
    
    MCEMM_DEBUG("CMceMediaManager::SetBalanceL(), Exit ");
    
    }  


// ---------------------------------------------------------
// CMceMediaManager::SetBalanceL
// ---------------------------------------------------------    
//
void CMceMediaManager::SetBalanceL( 
                        const CMceComMediaSource& aSource, 
                        TInt aBalance )
    { 
    MCEMM_DEBUG("CMceMediaManager::SetBalanceL(), Entry ");

    TPckgBuf<TInt> balance( aBalance );
    
    SetMccParameterL( aSource, KMccMicBalance, balance );
                                   
    MCEMM_DEBUG("CMceMediaManager::SetBalanceL(), Exit ");
    
    }  

// ---------------------------------------------------------
// CMceMediaManager::GetGainL 
// ---------------------------------------------------------    
//
void CMceMediaManager::GetGainL( 
                        const CMceComMediaSource& aSource, 
                        TInt& aGain )
    {
    MCEMM_DEBUG("CMceMediaManager::GetGainL(), Entry ");

    TPckgBuf<TInt> gain( aGain );
    
    GetMccParameterL( aSource, KMccMicGain, gain );
                                   
    aGain = gain();
    
    MCEMM_DEBUG("CMceMediaManager::GetGainL(), Exit ");
    
    }  

// ---------------------------------------------------------
// CMceMediaManager::GetMaxGainL
// ---------------------------------------------------------    
//
void CMceMediaManager::GetMaxGainL( TInt& aMaxGain )
    { 
    MCEMM_DEBUG("CMceMediaManager::GetMaxGainL(), Entry ");
    
    TPckgBuf<TInt> maxGain( aMaxGain );
    
    // Max gain can be asked always after MCC creation
    User::LeaveIfError( iMccInterface->GetParameter( 
        0, 0, 0, 0, KMccMicMaxGain, maxGain ) );
    
    aMaxGain = maxGain();
    
    MCEMM_DEBUG("CMceMediaManager::GetMaxGainL(), Exit ");
    }      


// ---------------------------------------------------------
// CMceMediaManager::SetGainL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetGainL( 
                const CMceComMediaSource& aSource,
                TInt aGain )
    { 
    MCEMM_DEBUG("CMceMediaManager::SetGainL(), Entry ");

    TPckgBuf<TInt> gain( aGain );
    
    SetMccParameterL( aSource, KMccMicGain, gain );
    
    MCEMM_DEBUG("CMceMediaManager::SetGainL(), Exit ");
    
    }   


// ---------------------------------------------------------
// CMceMediaManager::GetDisplaysAvailableL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetDisplaysAvailableL( 
                        TInt& aDisplaysAvailable )
    {
    MCEMM_DEBUG("CMceMediaManager::GetDisplaysAvailableL(), Entry ");

    const TInt KMceDefDisplaysAvailable = 1;

    aDisplaysAvailable = KMceDefDisplaysAvailable;
    
    MCEMM_DEBUG_DVALUE("Default value", aDisplaysAvailable );
    
    MCEMM_DEBUG("CMceMediaManager::GetDisplaysAvailableL(), Exit ");
    
    
    }

// ---------------------------------------------------------
// CMceMediaManager::GetDisplayRectL
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetDisplayRectL( CMceComDisplaySink& aSink, 
                                       TRect& aDisplayRect )
    {
    MCEMM_DEBUG("CMceMediaManager::GetDisplayRectL(), Entry ");

    TMccVideoSinkSetting display;
    TMccVideoSinkSettingBuf displayBuf( display );
    
    GetMccParameterL( aSink, aSink.MccParameterName(), 
                      displayBuf );
                                   
    display = displayBuf();
    aDisplayRect = TRect( display.iLocation, display.iSize );

    aSink.SetDisplayRect( aDisplayRect );

    MCEMM_DEBUG("CMceMediaManager::GetDisplayRectL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetDisplayRectL
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetDisplayRectL( CMceComDisplaySink& aSink, 
                                       TRect aDisplayRect )
    {
    MCEMM_DEBUG("CMceMediaManager::SetDisplayRectL(), Entry ");

    aSink.SetDisplayRect( aDisplayRect );

    TMccVideoSinkSetting display;
    TMccVideoSinkSettingBuf displayBuf( display );
    GetMccParameterL( aSink, aSink.MccParameterName(), 
                      displayBuf );
                      
    display = displayBuf();
    CMceComDisplaySink::ToPointAndSize( aDisplayRect,
                                        display.iLocation,
                                        display.iSize );
    TMccVideoSinkSettingBuf updateBuf( display );
    SetMccParameterL( aSink, aSink.MccParameterName(), 
                      updateBuf );
                               

    MCEMM_DEBUG("CMceMediaManager::SetDisplayRectL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::GetDisplayPriorityL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetDisplayPriorityL( CMceComDisplaySink& aSink, 
                         TInt& aDisplayPriority )
    {
    
    MCEMM_DEBUG("CMceMediaManager::GetDisplayPriorityL(), Entry ");

    TMccVideoSinkSetting display;
    TMccVideoSinkSettingBuf displayBuf( display );
    
    GetMccParameterL( aSink, aSink.MccParameterName(), 
                      displayBuf );
                                   
    display = displayBuf();
    aDisplayPriority = display.iPriority.iPriority;
    
    aSink.iPriority = aDisplayPriority;

    MCEMM_DEBUG("CMceMediaManager::GetDisplayPriorityL(), Exit ");
    
    }


// ---------------------------------------------------------
// CMceMediaManager::SetDisplayPriorityL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetDisplayPriorityL( CMceComDisplaySink& aSink, 
                                           TInt aDisplayPriority )
    {
    MCEMM_DEBUG("CMceMediaManager::SetDisplayPriorityL(), Entry ");

    aSink.iPriority = aDisplayPriority;

    TMccVideoSinkSetting display;
    TMccVideoSinkSettingBuf displayBuf( display );
    GetMccParameterL( aSink, aSink.MccParameterName(), 
                             displayBuf );
                             
    display = displayBuf();
    display.iPriority.iPriority = aSink.iPriority;
                           
    SetMccParameterL( aSink, aSink.MccParameterName(), 
                      displayBuf );
                               
                               
    MCEMM_DEBUG("CMceMediaManager::SetDisplayPriorityL(), Exit ");
    }


// ---------------------------------------------------------
// CMceMediaManager::GetDisplayRotationL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetDisplayRotationL( CMceComDisplaySink& aSink, 
                                            TInt& aDisplayRotation )
    {
    MCEMM_DEBUG("CMceMediaManager::GetDisplayRotationL(), Entry ");

    User::LeaveIfError( aSink.MccParameterName() == KMccCamLocalVideo 
                        ? KErrNotSupported : KErrNone );

    TMccVideoSinkSetting display;
    TMccVideoSinkSettingBuf displayBuf( display );
    
    GetMccParameterL( aSink, aSink.MccParameterName(), 
                      displayBuf );
                                   
    display = displayBuf();
    aDisplayRotation = display.iRotation;
    
    aSink.iRotation = 
        static_cast<CMceDisplaySink::TRotation>( aDisplayRotation );

    MCEMM_DEBUG("CMceMediaManager::GetDisplayRotationL(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::SetDisplayRotationL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetDisplayRotationL( CMceComDisplaySink& aSink, 
                                            TInt aDisplayRotation )
    {
    MCEMM_DEBUG("CMceMediaManager::SetDisplayRotationL(), Entry ");

    User::LeaveIfError( aSink.MccParameterName() == KMccCamLocalVideo 
                        ? KErrNotSupported : KErrNone );
    
    aSink.iRotation = 
        static_cast<CMceDisplaySink::TRotation>( aDisplayRotation );

    TMccVideoSinkSetting display;
    TMccVideoSinkSettingBuf displayBuf( display );
    GetMccParameterL( aSink, aSink.MccParameterName(), 
                             displayBuf );
                             
    display = displayBuf();
    display.iRotation = static_cast<TMccVideoRotation>( aSink.iRotation );
    displayBuf = display;
                           
    SetMccParameterL( aSink, aSink.MccParameterName(), 
                      displayBuf );
    
    MCEMM_DEBUG("CMceMediaManager::SetDisplayRotationL(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::SetDisplayIndexL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetDisplayIndexL( CMceComDisplaySink& aSink, 
                                        TInt aDisplayIndex )
    {
    
    MCEMM_DEBUG("CMceMediaManager::SetDisplayIndexL(), Entry ");

    aSink.iDisplayIndex = aDisplayIndex;

    TMccVideoSinkSetting display;
    TMccVideoSinkSettingBuf displayBuf( display );
    GetMccParameterL( aSink, aSink.MccParameterName(), 
                      displayBuf );
                      
    display = displayBuf();
    display.iDeviceIndex = static_cast<TUint>( aSink.iDisplayIndex );
                           
    SetMccParameterL( aSink, aSink.MccParameterName(), 
                      displayBuf );
                               

    MCEMM_DEBUG("CMceMediaManager::SetDisplayIndexL(), Exit ");
    
    }
 

// ---------------------------------------------------------
// CMceMediaManager::GetCameraInfoL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetCameraInfoL( TInt aCameraIndex, 
                                       TCameraInfo& aCameraInfo )
    {    
    MCEMM_DEBUG("CMceMediaManager::GetCameraInfoL(), Entry ");

    TMccVideoInfo2 info;
    info.iCameraIndex = aCameraIndex;
    
    TMccVideoInfoBuf2 videoInfo( info );
    GetMccParameterL( KMccCamInfo, videoInfo );
                                   
    aCameraInfo = videoInfo().iInfo;

    MCEMM_DEBUG("CMceMediaManager::GetCameraInfoL(), Exit ");
    
    
    }

// ---------------------------------------------------------
// CMceMediaManager::GetCamerasAvailableL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetCamerasAvailableL( TInt& aCamerasAvailable )
    {
    MCEMM_DEBUG("CMceMediaManager::GetCamerasAvailableL(), Entry ");

    TPckgBuf<TInt> camerasAvailable( aCamerasAvailable );

    GetMccParameterL( KMccCamCount, camerasAvailable );

    aCamerasAvailable = camerasAvailable();

    MCEMM_DEBUG("CMceMediaManager::GetCamerasAvailableL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::GetZoomFactorL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetZoomFactorL( CMceComCameraSource& aSource, 
                                      TInt& aZoomFactor )
    {
    
    MCEMM_DEBUG("CMceMediaManager::GetZoomFactorL(), Entry ");

    TPckgBuf<TInt> zoomFactor( aZoomFactor );

    GetMccParameterL( aSource, KMccCamZoomFactor, zoomFactor );
                                   
    aZoomFactor = zoomFactor();
    aSource.iZoomFactor = aZoomFactor;

    MCEMM_DEBUG("CMceMediaManager::GetZoomFactorL(), Exit ");
    
    
    }

// ---------------------------------------------------------
// CMceMediaManager::GetDigitalZoomFactorL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetDigitalZoomFactorL( CMceComCameraSource& aSource, 
                                             TInt& aDigitalZoomFactor )
    {
    MCEMM_DEBUG("CMceMediaManager::GetDigitalZoomFactorL(), Entry ");

    TPckgBuf<TInt> digitalZoomFactor( aDigitalZoomFactor );

    GetMccParameterL( aSource, KMccCamDigitZoomFactor, 
                      digitalZoomFactor );
                                   
    aDigitalZoomFactor = digitalZoomFactor();
    aSource.iDigitalZoomFactor = aDigitalZoomFactor;

    MCEMM_DEBUG("CMceMediaManager::GetDigitalZoomFactorL(), Exit ");
    
    }



// ---------------------------------------------------------
// CMceMediaManager::GetContrastL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetContrastL( CMceComCameraSource& aSource, 
                                    TInt& aContrast )
    {
    MCEMM_DEBUG("CMceMediaManager::GetContrastL(), Entry ");

    TPckgBuf<TInt> contrast( aContrast );

    GetMccParameterL( aSource, KMccCamContrast, contrast );
                                   
    aContrast = contrast();
    aSource.iContrast = aContrast;

    MCEMM_DEBUG("CMceMediaManager::GetContrastL(), Exit ");
    
    }


// ---------------------------------------------------------
// CMceMediaManager::GetBrightnessL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetBrightnessL( CMceComCameraSource& aSource, 
                                      TInt& aBrightness )
    {
    MCEMM_DEBUG("CMceMediaManager::GetBrightnessL(), Entry ");

    TPckgBuf<TInt> brightness( aBrightness );

    GetMccParameterL( aSource, KMccCamBrightness, 
                      brightness );
                                   
    aBrightness = brightness();
    aSource.iBrightness = aBrightness;

    MCEMM_DEBUG("CMceMediaManager::GetBrightnessL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::GetExposureL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetExposureL( 
    CMceComCameraSource& aSource, 
    TInt& aExposure )
    {
    MCEMM_DEBUG("CMceMediaManager::GetExposureL(), Entry ");

    TPckgBuf<TInt> exposure( aExposure );

    GetMccParameterL( aSource, KMccCamExposure, 
                      exposure );
                                   
    aExposure = exposure();
    aSource.iExposure = aExposure;

    MCEMM_DEBUG("CMceMediaManager::GetExposureL(), Exit ");   
    }
    

// ---------------------------------------------------------
// CMceMediaManager::GetWhiteBalanceL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetWhiteBalanceL( CMceComCameraSource& aSource, 
                                        TInt& aWhiteBalance )
    {
    MCEMM_DEBUG("CMceMediaManager::GetWhiteBalanceL(), Entry ");

    TPckgBuf<TInt> whiteBalance( aWhiteBalance );

    GetMccParameterL( aSource, KMccCamWhiteBalance, 
                      whiteBalance );
                                   
    aWhiteBalance = whiteBalance();
    aSource.iWhiteBalance = aWhiteBalance;

    MCEMM_DEBUG("CMceMediaManager::GetWhiteBalanceL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetCameraIndexL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetCameraIndexL( CMceComCameraSource& aSource, 
                                        TInt aCameraIndex,
                                        TCameraInfo& aCameraInfo )
    {
    MCEMM_DEBUG("CMceMediaManager::SetCameraIndexL(), Entry ");

    aSource.iCameraIndex = aCameraIndex;
                      
    UpdateMccSourceL( aSource );
    
    GetCameraInfoL( aCameraIndex, aCameraInfo );

    MCEMM_DEBUG("CMceMediaManager::SetCameraIndexL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetZoomFactorL
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetZoomFactorL( CMceComCameraSource& aSource, 
                                      TInt aZoomFactor )
    {
    MCEMM_DEBUG("CMceMediaManager::SetZoomFactorL(), Entry ");

    aSource.iZoomFactor = aZoomFactor;
    TPckgBuf<TInt> zoomFactor( aSource.iZoomFactor );

    SetMccParameterL( aSource, KMccCamZoomFactor, 
                      zoomFactor );
                                   
    MCEMM_DEBUG("CMceMediaManager::SetZoomFactorL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetDigitalZoomFactorL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetDigitalZoomFactorL( CMceComCameraSource& aSource, 
                                             TInt aDigitalZoomFactor )
    {
    MCEMM_DEBUG("CMceMediaManager::SetDigitalZoomFactorL(), Entry ");

    aSource.iDigitalZoomFactor = aDigitalZoomFactor;
    TPckgBuf<TInt> digitalZoomFactor( aSource.iDigitalZoomFactor );

    SetMccParameterL( aSource, KMccCamDigitZoomFactor, 
                      digitalZoomFactor );
                                   

    MCEMM_DEBUG("CMceMediaManager::SetDigitalZoomFactorL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetContrastL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetContrastL( CMceComCameraSource& aSource, 
                                    TInt aContrast )
    {
    MCEMM_DEBUG("CMceMediaManager::SetContrastL(), Entry ");

    aSource.iContrast = aContrast;
    TPckgBuf<TInt> contrast( aSource.iContrast );

    SetMccParameterL( aSource, KMccCamContrast, 
                      contrast );

    MCEMM_DEBUG("CMceMediaManager::SetContrastL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetBrightnessL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetBrightnessL( CMceComCameraSource& aSource, 
                                      TInt aBrightness )
    {
    MCEMM_DEBUG("CMceMediaManager::SetBrightnessL(), Entry ");

    aSource.iBrightness = aBrightness;
    TPckgBuf<TInt> brightness( aSource.iBrightness );

    SetMccParameterL( aSource, KMccCamBrightness, 
                     brightness );
                                   

    MCEMM_DEBUG("CMceMediaManager::SetBrightnessL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetExposureL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetExposureL( 
    CMceComCameraSource& aSource, 
    TInt aExposure)
    {
    MCEMM_DEBUG("CMceMediaManager::SetExposureL(), Entry ");

    aSource.iExposure = aExposure;
    TPckgBuf<TInt> exposure( aSource.iExposure );

    SetMccParameterL( aSource, KMccCamExposure, 
                      exposure );
                                   
    MCEMM_DEBUG("CMceMediaManager::SetExposureL(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::SetWhiteBalanceL
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetWhiteBalanceL( CMceComCameraSource& aSource, 
                                        TInt aWhiteBalance )
    {
    MCEMM_DEBUG("CMceMediaManager::SetWhiteBalanceL(), Entry ");

    aSource.iWhiteBalance = aWhiteBalance;
    TPckgBuf<TInt> whiteBalance( aSource.iWhiteBalance );

    SetMccParameterL( aSource, KMccCamWhiteBalance, 
                      whiteBalance );
                                   
    MCEMM_DEBUG("CMceMediaManager::SetWhiteBalanceL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::GetFileInfoL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetFileInfoL( CMceComFileSource& aSource, 
                                     TMceFileInfo& aFileInfo )
    {

    MCEMM_DEBUG("CMceMediaManager::GetFileInfoL(), Entry ");

    if ( aSource.MediaStream() )
        {
        MCE_HEAP_CONSTRUCT_LC( TMccFileSourceSetting, fileInfo );
        MCE_HEAP_CONSTRUCT_WITH_PARAM_LC( TMccFileSourceSettingBuf, fileInfoBuf, *fileInfo );
        
        GetMccParameterL( aSource, KMccFileInfo, *fileInfoBuf );
        
        FillMceFileInfoL( (*fileInfoBuf)(), aFileInfo );
        
        aSource.iFileInfo = aFileInfo;
        
        CleanupStack::PopAndDestroy( fileInfoBuf );
        CleanupStack::PopAndDestroy( fileInfo );
        }
    else
        {
        GetFileInfoL( aFileInfo );
        }

    MCEMM_DEBUG("CMceMediaManager::GetFileInfoL(), Exit ");
    
    }


// ---------------------------------------------------------
// CMceMediaManager::SetFileInfoL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetFileInfoL( CMceComFileSource& aSource, 
                                     TMceFileInfo& aFileInfo )
    {

    MCEMM_DEBUG("CMceMediaManager::SetFileInfoL(), Entry ");

    MCE_HEAP_CONSTRUCT_LC( TMccFileSourceSetting, fileInfo );
    
    fileInfo->iFileName = aFileInfo.iFileName;
    
    MCE_HEAP_CONSTRUCT_WITH_PARAM_LC( TMccFileSourceSettingBuf, fileInfoBuf, *fileInfo );

    SetMccParameterL( aSource, KMccFileName, 
                      *fileInfoBuf );
                      
    CleanupStack::PopAndDestroy( fileInfoBuf );
    CleanupStack::PopAndDestroy( fileInfo );
    
    MCEMM_DEBUG("CMceMediaManager::SetFileInfoL(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::GetFileInfoL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetFileInfoL( TMceFileInfo& aFileInfo )
    {
    MCEMM_DEBUG("CMceMediaManager::GetFileInfoL(), Entry ");
    
    MCE_HEAP_CONSTRUCT_LC( TMccFileSourceSetting, fileInfo );
    
    fileInfo->iFileName = aFileInfo.iFileName;
    
    MCE_HEAP_CONSTRUCT_WITH_PARAM_LC( TMccFileSourceSettingBuf, fileInfoBuf, *fileInfo );
    
    GetMccParameterL( KMccFileInfo, *fileInfoBuf );
    
    FillMceFileInfoL( (*fileInfoBuf)(), aFileInfo );
    
    CleanupStack::PopAndDestroy( fileInfoBuf );
    CleanupStack::PopAndDestroy( fileInfo );
    
    MCEMM_DEBUG("CMceMediaManager::GetFileInfoL(), Exit ");
    
    }
    

// ---------------------------------------------------------
// CMceMediaManager::GetFilePositionL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetFilePositionL( CMceComFileSource& aSource, 
                                         TTimeIntervalMicroSeconds& aFilePosition )
    {
    
    MCEMM_DEBUG("CMceMediaManager::GetFilePositionL(), Entry ");

    MCE_HEAP_CONSTRUCT_LC( TMccFileSourceSetting, fileInfo );
    MCE_HEAP_CONSTRUCT_WITH_PARAM_LC( TMccFileSourceSettingBuf, fileInfoBuf, *fileInfo );
    
    GetMccParameterL( aSource, KMccFilePosition, 
                      *fileInfoBuf );
    
    aFilePosition = (*fileInfoBuf)().iPosition;
    
    CleanupStack::PopAndDestroy( fileInfoBuf );
    CleanupStack::PopAndDestroy( fileInfo );
    
    MCEMM_DEBUG("CMceMediaManager::GetFilePositionL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetFilePositionL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetFilePositionL( CMceComFileSource& aSource, 
                                         TTimeIntervalMicroSeconds& aFilePosition )
    {

    MCEMM_DEBUG("CMceMediaManager::SetFilePositionL(), Entry ");

    MCE_HEAP_CONSTRUCT_LC( TMccFileSourceSetting, fileInfo );
    
    fileInfo->iPosition = aFilePosition;
    
    MCE_HEAP_CONSTRUCT_WITH_PARAM_LC( TMccFileSourceSettingBuf, fileInfoBuf, *fileInfo );
    
    SetMccParameterL( aSource, KMccFilePosition, 
                      *fileInfoBuf );
    
    CleanupStack::PopAndDestroy( fileInfoBuf );
    CleanupStack::PopAndDestroy( fileInfo );                  

    MCEMM_DEBUG("CMceMediaManager::SetFilePositionL(), Exit ");
    
    }

// ---------------------------------------------------------
// CMceMediaManager::FastForwardL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::FastForwardL( CMceComFileSource& /*aSource*/, 
                                    TBool& /*aOnOff*/ )
    {
    MCEMM_DEBUG("CMceMediaManager::FastForwardL(), Entry ");
/*    
    TMceFileInfo fileInfo;
    TTimeIntervalMicroSeconds position;

    GetFileInfoL( aSource, fileInfo );
    GetFilePositionL( aSource, position );
    
    TTimeIntervalMicroSeconds newPosition(
            position.Int64() + 
             (TInt64)( KMceOneSecondAsMicros * 
                       KMceFastForwardMultiplier ) );
                                   
    if ( newPosition < fileInfo.iDuration )
        {
        SetFilePositionL( aSource, newPosition );
        aSource.iPosition = newPosition;
        }
    else
        {
        MCEMM_DEBUG("At the end. No forward");
        }
*/
    MCEMM_DEBUG("CMceMediaManager::FastForwardL(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::FastRewindL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::FastRewindL( CMceComFileSource& /*aSource*/, 
                                   TBool& /*aOnOff*/ )
    {
    MCEMM_DEBUG("CMceMediaManager::FastRewindL(), Entry ");
/*    
    TMceFileInfo fileInfo;
    TTimeIntervalMicroSeconds position;

    GetFileInfoL( aSource, fileInfo );
    GetFilePositionL( aSource, position );
    
    TTimeIntervalMicroSeconds newPosition(
            position.Int64() - 
             (TInt64)( KMceOneSecondAsMicros * 
                       KMceFastForwardMultiplier ) );
                                   
    if ( newPosition > 0 )
        {
        SetFilePositionL( aSource, newPosition );
        aSource.iPosition = newPosition;
        }
    else
        {
        MCEMM_DEBUG("At the beginning. No rewind");
        }
*/    
    MCEMM_DEBUG("CMceMediaManager::FastRewindL(), Exit ");
    }


// ---------------------------------------------------------
// CMceMediaManager::TranscodeFileL 
// --------------------------------------------------------- 
//   
TUint32 CMceMediaManager::TranscodeFileL( CMceComFileSource* aFile )
    {
    MCEMM_DEBUG("CMceMediaManager::TranscodeFileL(), Entry ");
    __ASSERT_ALWAYS( aFile, User::Leave( KErrArgument ) );
    
    MCEMM_DEBUG_SVALUE(" from", aFile->iFileInfo.iFileName );
    MCEMM_DEBUG_SVALUE(" to", aFile->iTranscodeInfo.iFileName );

     HBufC8* sourceFileBuf = HBufC8::NewLC( KMceMaxFileName8 );
     TPtr8 sourceFile = sourceFileBuf->Des();
     HBufC8* destFileBuf = HBufC8::NewLC( KMceMaxFileName8 );
     TPtr8 destFile = destFileBuf->Des();
     
    TInt cnvRet = 
        CnvUtfConverter::ConvertFromUnicodeToUtf8( sourceFile,
                                                   aFile->iFileInfo.iFileName );

    MCEMM_DEBUG_DVALUE( "CMceMediaManager::TranscodeFileL(), source name converted: ", 
                        cnvRet )
    __ASSERT_ALWAYS( !cnvRet, User::Leave( KErrGeneral ) );
    
    cnvRet = CnvUtfConverter::ConvertFromUnicodeToUtf8( destFile,
                                                     aFile->iTranscodeInfo.iFileName );
     
    MCEMM_DEBUG_DVALUE( "CMceMediaManager::TranscodeFileL(), dest name converted: ", 
                        cnvRet )
    __ASSERT_ALWAYS( !cnvRet, User::Leave( KErrGeneral ) );
     
    CMccCodecInformation* audioCodec = NULL;
    CMccCodecInformation* videoCodec = NULL;
    if ( aFile->MediaStream() && aFile->MediaStream()->Session() )
    	{
    	CreateTranscodingOutputCodecsL( &audioCodec,&videoCodec,aFile );
    	}
    if ( audioCodec )
    	{
    	CleanupStack::PushL( audioCodec );
    	}
    if ( videoCodec )
    	{
    	CleanupStack::PushL( videoCodec );
    	}
    	
     User::LeaveIfError ( iMccInterface->TranscodeFile( 
                          sourceFile,
                          destFile,
                          aFile->iTranscodeInfo.iQuality,
                          aFile->iTranscodeInfo.iTranscodeSessionId,
                          videoCodec,
                          audioCodec ) );
    if ( videoCodec )
    	{
    	CleanupStack::PopAndDestroy();
    	}
    if ( audioCodec )
    	{
    	CleanupStack::PopAndDestroy();
    	}  
                          
    CleanupStack::PopAndDestroy( destFileBuf );
    CleanupStack::PopAndDestroy( sourceFileBuf );
 	   
    iTranscodeSessions.AppendL( aFile );

    MCEMM_DEBUG_DVALUE(" sessionid", aFile->iTranscodeInfo.iTranscodeSessionId );
    MCEMM_DEBUG("CMceMediaManager::TranscodeFileL(), Exit ");
    
    return aFile->iTranscodeInfo.iTranscodeSessionId;                     
    }
    
    
// ---------------------------------------------------------
// CMceMediaManager::CreateTranscodingOutputCodecsL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::CreateTranscodingOutputCodecsL( 
										   CMccCodecInformation** aAudioCodec ,
        								   CMccCodecInformation** aVideoCodec ,
        								   CMceComFileSource* aFile )
    {  
    MCEMM_DEBUG("CMceMediaManager::CreateTranscodingOutputCodecsL(),entry ");
    CMceComSession* tempSession = aFile->MediaStream()->Session();
    RPointerArray<CMceComMediaStream>& streams = tempSession->Streams();
    CMceSrvStream* audioSrvStream = NULL;
    CMceSrvStream* videoSrvStream = NULL;
    TInt cleanitems = 0;
    CMceComAudioStream* audioStream = NULL;
    CMceComVideoStream* videoStream	= NULL;
   
    for ( int i = 0; i < streams.Count();i++ )
    	{
    	if ( streams[i]->iType == KMceAudio &&  !audioSrvStream )
    		{
    		audioStream = static_cast<CMceComAudioStream*>( 
                  								tempSession->Streams()[ i ] );
    		if ( audioStream->Codecs().Count() )
    			{
    			audioSrvStream = CMceSrvStream::NewL( *this, *aFile->MediaStream(),
    												 *aFile->MediaStream()->Source(),
    												 *aFile->MediaStream()->Sinks()[0],
    												 *(audioStream->Codecs()[0])	);
    			CleanupStack::PushL( audioSrvStream );
    			cleanitems++;
    			MCEMM_DEBUG("CMceMediaManager::CreateTranscodingOutputCodecsL(), AudioCodec  found ");
    			}
    		}
    			
    	if ( streams[i]->iType == KMceVideo &&  !videoSrvStream )
    		{
    		videoStream = static_cast<CMceComVideoStream*>( 
                    							tempSession->Streams()[ i ] );
    		if ( videoStream->Codecs().Count() )
    			{
    			videoSrvStream = CMceSrvStream::NewL( *this, *aFile->MediaStream(),
    												 *aFile->MediaStream()->Source(),
    												 *aFile->MediaStream()->Sinks()[0],
    												 *(videoStream->Codecs()[0])	);
    			CleanupStack::PushL( videoSrvStream );
    			cleanitems++;
    			MCEMM_DEBUG("CMceMediaManager::CreateTranscodingOutputCodecsL(), VideoCodec found ");
    			}
    		}
    	
    
    	}
    if ( audioSrvStream )
    	{
    	*aAudioCodec = CreateMccCodecLC( *audioSrvStream, EMceRoleOfferer );
    	cleanitems++;
    	}
    		
    if ( videoSrvStream )
    	{
    	*aVideoCodec = CreateMccCodecLC( *videoSrvStream, EMceRoleOfferer );
    	cleanitems++;
    	}	
    CleanupStack::Pop( cleanitems );
    delete audioSrvStream;
    audioSrvStream = NULL;
    delete videoSrvStream;
    videoSrvStream = NULL;
    MCEMM_DEBUG("CMceMediaManager::CreateTranscodingOutputCodecsL(),exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::CancelTranscodeFileL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::CancelTranscodeFileL( TUint32 aSessionId )
    {
    
    MCEMM_DEBUG("CMceMediaManager::CancelTranscodeFileL(), Entry ");
    MCEMM_DEBUG_DVALUE(" sessionid", aSessionId );
    
    CMceComFileSource* session = TranscodingSession( aSessionId );
    
    if ( session )
        {
        MCEMM_DEBUG("CMceMediaManager::CancelTranscodeFileL(), cancelling ");
        
        User::LeaveIfError ( 
            iMccInterface->CancelTranscodeFile( 
                session->iTranscodeInfo.iTranscodeSessionId ) );
        
        TInt index = iTranscodeSessions.Find( session );
        
        iTranscodeSessions.Remove( index );
        delete session;
        }
    
    MCEMM_DEBUG("CMceMediaManager::CancelTranscodeFileL(), Exit ");
    }


// ---------------------------------------------------------
// CMceMediaManager::TranscodingSession 
// ---------------------------------------------------------    
//
CMceComFileSource* CMceMediaManager::TranscodingSession( 
                        const CMceComFileSource& aFile ) const
    {
    CMceComFileSource* session = NULL;
    TInt index = 0;
    while( !session && index < iTranscodeSessions.Count() )
        {
        session = iTranscodeSessions[ index++ ];
        session = session->Id() == aFile.Id() ? session : NULL;
        
        }

    return session;
    
    }
    
// ---------------------------------------------------------
// CMceMediaManager::TranscodingSession 
// ---------------------------------------------------------    
//
CMceComFileSource* CMceMediaManager::TranscodingSession( 
                                TUint32 aTcSessionId ) const
    {
    CMceComFileSource* session = NULL;
    TInt index = 0;
    while( !session && index < iTranscodeSessions.Count() )
        {
        session = iTranscodeSessions[ index++ ];
        session = 
            session->iTranscodeInfo.iTranscodeSessionId == 
                                aTcSessionId ? session : NULL;
        
        }

    return session;
    
    }
    
  
// ---------------------------------------------------------
// CMceMediaManager::GetSupportedCodecs 
// ---------------------------------------------------------    
//
 TInt CMceMediaManager::GetSupportedCodecs( RArray<TFourCC>& /*aCodecs*/  )
    { 
    MCEMM_DEBUG("CMceMediaManager::GetSupportedCodecs(), Entry ");
    /*
    TInt err = iMccInterface->GetSupportedCodecs( aCodecs );
    */
    MCEMM_DEBUG("NO IMPLEMENTATION");
    TInt err = KErrNone;
    MCEMM_DEBUG("CMceMediaManager::GetSupportedCodecs(), Exit ");
    return err;
    }     

// ---------------------------------------------------------
// CMceMediaManager::GetCapabilities 
// ---------------------------------------------------------  
//  
void CMceMediaManager::GetCapabilitiesL( 
                    RPointerArray<CMceComAudioCodec>& aCapabilities  )
    { 
    MCEMM_DEBUG("CMceMediaManager::GetCapabilitiesL() - audio , Entry ");
    RPointerArray<CMccCodecInformation> mccCapabilities;
    
    TMceComAudioCodecFactory codecFactory;
    RStringF audio = MCE_SDP_STRING_AUDIOL();
    
    TInt index = 0;
    CMceSdpCodec* audioSdpCodec = NULL;
    while( !audioSdpCodec && index < iSdpCodecs.Count() )
        {
        audioSdpCodec = iSdpCodecs[ index++ ];
        audioSdpCodec = audioSdpCodec->Media() == audio ? audioSdpCodec : NULL;
        }

    User::LeaveIfNull ( audioSdpCodec );
    
    User::LeaveIfError ( iMccInterface->GetCapabilities( mccCapabilities ) );

    MceCleanupResetAndDestroyPushL( mccCapabilities );
    
    for( int i = 0; i < mccCapabilities.Count(); i++ )
        {
        CMccCodecInformation* mccCodec = mccCapabilities[i];
        if ( mccCodec->Type() == KUidMediaTypeAudio )
            {
            CMceComAudioCodec* codec = codecFactory.CreateCodecLC( mccCodec->SdpName() );
            if ( codec )
                {
                *codec = *mccCodec;
                
                aCapabilities.AppendL( codec );
                CleanupStack::Pop( codec );
            
                MCEMM_DEBUG("audio capabilities");
                MCEMM_DEBUG_SVALUE("codec", codec->iSdpName );
                MCEMM_DEBUG_DVALUE("sampling freq", codec->iSamplingFreq )
                MCEMM_DEBUG_DVALUE("payload type", codec->iPayloadType )
                MCEMM_DEBUG_DVALUE("bitrate", codec->iBitrate )
                MCEMM_DEBUG_DVALUE("max ptime", codec->iMaxPTime )
                MCEMM_DEBUG_DVALUE("ptime", codec->iPTime )
                MCEMM_DEBUG_DVALUE("VAD", codec->iEnableVAD )
                MCEMM_DEBUG_DVALUE("codec mode",codec->iCodecMode )
                MCEMM_DEBUG_DVALUE("fourCC", codec->iFourCC )
                MCEMM_DEBUG_SVALUE("fmtp", *codec->iFmtpAttr )

                audioSdpCodec->AddCapabilityL( mccCodec->SdpName() );
                audioSdpCodec->AddCapabilityPayloadTypeL( mccCodec->PayloadType() );
                }
            else
                {
                MCEMM_DEBUG_SVALUE("Not supported codec",mccCodec->SdpName() );
                }
            }
        }
    CleanupStack::PopAndDestroy();//mccCapabilities
    
    MCEMM_DEBUG("CMceMediaManager::GetCapabilitiesL() - audio , Exit ");
         
    }


// ---------------------------------------------------------
// CMceMediaManager::GetCapabilities 
// ---------------------------------------------------------  
//  
void CMceMediaManager::GetCapabilitiesL( 
                    RPointerArray<CMceComVideoCodec>& aCapabilities  )
    { 
    MCEMM_DEBUG("CMceMediaManager::GetCapabilitiesL() - video, Entry ");
    
    
    RPointerArray<CMccCodecInformation> mccCapabilities;
    TMceComVideoCodecFactory codecFactory;
    RStringF video = MCE_SDP_STRING_VIDEOL();
    
    TInt index = 0;
    CMceSdpCodec* videoSdpCodec = NULL;
    while( !videoSdpCodec && index < iSdpCodecs.Count() )
        {
        videoSdpCodec = iSdpCodecs[ index++ ];
        videoSdpCodec = videoSdpCodec->Media() == video ? videoSdpCodec : NULL;
        }

    User::LeaveIfNull ( videoSdpCodec );
    User::LeaveIfError ( iMccInterface->GetCapabilities( mccCapabilities ) );

    MceCleanupResetAndDestroyPushL( mccCapabilities );
    
    for( int i = 0; i < mccCapabilities.Count(); i++ )
        {
        CMccCodecInformation* mccCodec = mccCapabilities[i];
        if ( mccCodec->Type() == KUidMediaTypeVideo )
            {
            CMceComVideoCodec* codec = codecFactory.CreateCodecLC( mccCodec->SdpName() );
            
            if ( codec )
                {
                *codec = *mccCodec;
                
                aCapabilities.AppendL( codec );
                CleanupStack::Pop( codec );

                MCEMM_DEBUG("video capabilities");
                MCEMM_DEBUG_SVALUE("codec", codec->iSdpName );
                MCEMM_DEBUG_DVALUE("clock rate", codec->iClockRate )
                MCEMM_DEBUG_DVALUE("payload type", codec->iPayloadType )
                MCEMM_DEBUG_DVALUE("bitrate", codec->iBitrate )
                MCEMM_DEBUG_DVALUE("framerate", (TInt)codec->iFrameRate )
                MCEMM_DEBUG_DVALUE("max bitrate", codec->iMaxBitRate )
                MCEMM_DEBUG_DVALUE("resolution width", codec->iResolutionWidth )
                MCEMM_DEBUG_DVALUE("resolution height", codec->iResolutionHeight )
                MCEMM_DEBUG_DVALUE("codec mode",codec->iCodecMode )
                MCEMM_DEBUG_DVALUE("fourCC", codec->iFourCC )
                MCEMM_DEBUG_SVALUE("fmtp", *codec->iFmtpAttr )
                
                
                videoSdpCodec->AddCapabilityL( mccCodec->SdpName() );
                videoSdpCodec->AddCapabilityPayloadTypeL(mccCodec->PayloadType());
                }
            else 
                {
                MCEMM_DEBUG_SVALUE("Not supported codec",mccCodec->SdpName() );    
                }
            }
        }
    CleanupStack::PopAndDestroy();//mccCapabilities
    
        
    MCEMM_DEBUG("CMceMediaManager::GetCapabilitiesL() - video, Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::DtmfL
// ---------------------------------------------------------    
//
void CMceMediaManager::DtmfL( 
    CMceComMediaSource& aSource, 
    TMceComEvent& aEvent,
    const TDesC8& aSequence )
    {
    MCEMM_DEBUG("CMceMediaManager::DtmfL(), Entry ");

    
    TBool handled = DoDtmfL( aSource, aEvent, aSequence );
    if ( !handled && aSource.MediaStream()->BoundStream() )
        {
        handled = DoDtmfL( aSource, aEvent, aSequence, ETrue );
        }
    
    __ASSERT_ALWAYS( handled, User::Leave( KErrNotSupported ) );
    
    MCEMM_DEBUG("CMceMediaManager::DtmfL(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::SetCNameL
// ---------------------------------------------------------    
//
void CMceMediaManager::SetCNameL( 
    const CMceComEndpoint& aEndpoint, 
    const TDesC8& aCName )
    {
    MCEMM_DEBUG_SVALUE("CMceMediaManager::SetCNameL(), cname:", aCName );
    
    CMceComMediaStream* endpointMediaStream = aEndpoint.MediaStream();
    if ( IS_RECEIVESTREAM( endpointMediaStream ) &&
         aEndpoint.iType == KMceRTPSource )
        {
        SetMccParameterL( static_cast<const CMceComMediaSource&>( aEndpoint ), 
                          KMccRtpCName,
                          aCName,
                          ETrue );
        }
    else if ( IS_SENDSTREAM( endpointMediaStream ) &&
              aEndpoint.iType == KMceRTPSink )
        {
        SetMccParameterL( static_cast<const CMceComMediaSink&>( aEndpoint ), 
                          KMccRtpCName,
                          aCName,
                          ETrue );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
        
    MCEMM_DEBUG("CMceMediaManager::SetCNameL(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::UpdateConfigKeyL
// ---------------------------------------------------------    
//
void CMceMediaManager::UpdateConfigKeyL( const CMceComMediaSource& aSource )
    {
    MCEMM_DEBUG("CMceMediaManager::UpdateConfigKeyL(), Entry ");
    
    if ( aSource.MediaStream() &&
         aSource.MediaStream()->iType == KMceVideo &&
       ( aSource.Type() == KMceCameraSource || aSource.Type() == KMceFileSource ) )
        {
        TInt err( KErrNone );
        HBufC8* configKey = HBufC8::NewLC( KMaxConfigKeyLen );
        TPtr8 configKeyPtr( configKey->Des() );

        TMceSrvStreamIterator streams( 
                        aSource.MediaStream()->Session()->MccStreams(), aSource );
        
        CMceComMediaStream* mediaStream = NULL;
        CMceSrvStream* stream = NULL;
        while ( streams.Next( stream ) && !err )
            {
            mediaStream = &stream->Data();
            if ( mediaStream->iType != KMceVideo )
                {
                // NOP
                }
            else if ( CMceComVideoCodec::IsConfigKeyValid( configKey ) )
                {
                DoConfigKeyUpdateL( *mediaStream, configKey );
                }
            else if ( stream->IsMccPrepared() )
                {
                err = iMccInterface->GetParameter( stream->SessionId(),
                                                   stream->LinkId(),
                                                   stream->Id(),
                                                   stream->Source().Id(),
                                                   KMccConfigKey,
                                                   configKeyPtr );                                 
                if ( !err && CMceComVideoCodec::IsConfigKeyValid( configKey ) )
                    {
                    streams.Reset();
                    }
                }
            else
                {
                // NOP
                }
            }

        CleanupStack::PopAndDestroy( configKey );
        }
    
    
    MCEMM_DEBUG("CMceMediaManager::UpdateConfigKeyL(), Exit ");
    }
    
// ---------------------------------------------------------
// CMceMediaManager::GetSSRCL
// ---------------------------------------------------------    
//
TUint32 CMceMediaManager::GetSSRCL( CMceSrvStream& aStream )
    {
    MCEMM_DEBUG("CMceMediaManager::GetSSRCL(), Entry ");
    
    TUint32 ssrc = 0;
    
    User::LeaveIfError( iMccInterface->GetSSRC( 
                                        aStream.SessionId(),
                                        aStream.LinkId(),
                                        aStream.Id(),
                                        0,
                                        ssrc ) );
                                        
    MCEMM_DEBUG("CMceMediaManager::GetSSRCL(), Exit ");
    return ssrc;
    
    }



// ---------------------------------------------------------------------------
// CMceMediaManager::EnableViewFinderL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::EnableViewFinderL( const CMceComMediaSink& /*aMediaSink*/, 
                                          const TDesC8& aParam )
    {
    MCEMM_DEBUG("CMceMediaManager::EnableViewFinderL(), Entry ");
    
    SetMccParameterL( KMccCamLocalVideo, aParam );

    MCEMM_DEBUG("CMceMediaManager::EnableViewFinderL(), Exit ");
    
    }


// ---------------------------------------------------------------------------
// CMceMediaManager::DisableViewFinderL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::DisableViewFinderL( const CMceComMediaSink& /*aMediaSink*/ )
    {
    MCEMM_DEBUG("CMceMediaManager::DisableViewFinderL(), Entry ");

    SetMccParameterL( KMccCamLocalVideo, KNullDesC8 );
    
    MCEMM_DEBUG("CMceMediaManager::DisableViewFinderL(), Exit ");
    
    }


// ---------------------------------------------------------
// CMceMediaManager::CreateMccSessionL
// ---------------------------------------------------------    
//
void CMceMediaManager::CreateMccSessionL( CMceComSession& aSession )
    {
    User::LeaveIfError( iMccInterface->CreateSession( aSession.iMccID ) );
    }


// ---------------------------------------------------------
// CMceMediaManager::CreateMccLinkL
// ---------------------------------------------------------    
//
void CMceMediaManager::CreateMccLinkL( CMceSrvStream& aStream )
    {      
    MCEMM_DEBUG("CMceMediaManager::CreateMccLinkL(), Entry ");
    
    __ASSERT_ALWAYS( aStream.State() != CMceSrvStream::EAdopted,
                     User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aStream.State() == CMceSrvStream::ECreated,
                     User::Leave( KErrArgument ) );
    
    TMccNetSettings netSettings;
    netSettings.iIapId = aStream.IapId();
    netSettings.iLocalAddress.SetPort( aStream.LocalMediaPort() );
    netSettings.iMediaQosValue = aStream.Data().Session()->iServiceType;
    TUint32 linkId = aStream.LinkId();
    
    if ( linkId == KMceNotAssigned && 
         !ReuseLink( aStream ) )
        {
        MCEMM_DEBUG("CMceMediaManager::CreateMccLinkL(): no link. creating link");
        User::LeaveIfError( 
            iMccInterface->CreateLink( aStream.SessionId(),
                                       aStream.LinkType(), 
                                       linkId,
                                       netSettings ) );
        
        aStream.SetLinkId( linkId );
        
        SetPendingState( aStream, 0, CMceSrvStream::ECreatingLink );
        
        if ( aStream.Data().Session()->SecureSession() )
        	{
        	aStream.Data().Session()->SecureSession()->iCryptoContextUpdate = ETrue;		
        	}
        }
    else
        {
        TMceSrvStreamIterator creatingLink( aStream.Data().Session()->MccStreams() );
        TMceMccComEvent event( KMccEventNone,
                               aStream.LinkId(), 
                               0, 
                               0 );
        
        CMceSrvStream* stream = NULL;
        TBool creatingLinkCurrently( EFalse );
        while ( creatingLink.Next( stream, event ) && !creatingLinkCurrently )
            {
            if ( stream != &aStream && stream->State() == CMceSrvStream::ECreatingLink )
                {
                MCEMM_DEBUG("CMceMediaManager::CreateMccLinkL(): link not ready. waiting");
                SetPendingState( aStream, 0, CMceSrvStream::ECreatingLink );
                creatingLinkCurrently = ETrue;
                }
            }
        
        if ( !creatingLinkCurrently )
            {
            MCEMM_DEBUG("CMceMediaManager::CreateMccLinkL(): link exists");
            SetPendingState( aStream, 0, CMceSrvStream::ELinkCreated );
            }
        }
   
        
    MCEMM_DEBUG("CMceMediaManager::CreateMccLinkL(), Exit ");
        
    }
    


// ---------------------------------------------------------
// CMceMediaManager::InitializeMccStreamL
// ---------------------------------------------------------    
//
void CMceMediaManager::InitializeMccStreamL( CMceSrvStream& aStream )
    {

    MCEMM_DEBUG("CMceMediaManager::InitializeMccStreamL(), Entry ");

    __ASSERT_ALWAYS( aStream.State() != CMceSrvStream::EAdopted,
                     User::Leave( KErrArgument ) );
    
    //create codec information
    CMccCodecInformationFactory* codecFactory = 
        CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( codecFactory );
    
    CMccCodecInformation* codec = 
        codecFactory->CreateCodecInformationL( aStream.Codec().iSdpName );
            
    CleanupStack::PushL( codec );
    aStream.Codec().MccDecodeL( *codec );

    CleanupStack::PopAndDestroy( codec );
    CleanupStack::PopAndDestroy( codecFactory );
    
    MCEMM_DEBUG("CMceMediaManager::InitializeMccStreamL(), Exit ");
    }
    

// ---------------------------------------------------------
// CMceMediaManager::CreateMccCodecLC
// ---------------------------------------------------------    
//
CMccCodecInformation* CMceMediaManager::CreateMccCodecLC( 
                                        CMceSrvStream& aStream,
                                        TMceNegotiationRole aRole )
    {

    //create codec information
    CMccCodecInformationFactory* codecFactory = 
        CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( codecFactory );
    
    CMccCodecInformation* codec = 
        codecFactory->CreateCodecInformationL( aStream.Codec().iSdpName );

    CleanupStack::PopAndDestroy( codecFactory );
            
    CleanupStack::PushL( codec );
    aStream.Codec().MccPopulateL( *codec, aStream, aRole );

    return codec;
        
    }
    
// ---------------------------------------------------------
// CMceMediaManager::CreateMccStreamL
// ---------------------------------------------------------    
//
void CMceMediaManager::CreateMccStreamL( 
            CMceSrvStream& aStream,
            TMceNegotiationRole aRole )
    {

    MCEMM_DEBUG("CMceMediaManager::CreateMccStreamL(), Entry ");

    __ASSERT_ALWAYS( aStream.State() != CMceSrvStream::EAdopted,
                     User::Leave( KErrArgument ) );
    
    // Create mcc codec even if mcc prepare is not needed in order to
    // construct fmtp info also for local codecs. That information might
    // be needed in some stream matching cases later on.
    CMccCodecInformation* mccCodec = CreateMccCodecLC( aStream, aRole );
        
    if ( !aStream.PrepareL() )
        {
        CMceComCodec& codec = aStream.Codec();

        if ( ReuseSource( aStream ) )
            {
            MCEMM_DEBUG("CMceMediaManager::CreateMccStreamL(): resuing source ");
            
            //reuse source
            iMccInterface->Reuse( aStream.SessionId(),
                                  aStream.Id(),
                                  aStream.LinkId(),
                                  aStream.Source().Id() );
            }
        else
            {
            MCEMM_DEBUG("CMceMediaManager::CreateMccStreamL(): adding source ");
            //add source
            iMccInterface->AddDataSource( aStream.Source().Type(),
                                          aStream.Source().Data().InitParamL( codec ),
                                          aStream.Source().Id() );
            }
            
        if ( ReuseSink( aStream ) )
            {
            MCEMM_DEBUG("CMceMediaManager::CreateMccStreamL(): resuing sink ");
            //reuse sink
            iMccInterface->Reuse( aStream.SessionId(),
                                  aStream.Id(),
                                  aStream.LinkId(),
                                  aStream.Sink().Id() );
            }
        else
            {
            MCEMM_DEBUG("CMceMediaManager::CreateMccStreamL(): adding sink ");
            //add sink
            iMccInterface->AddDataSink( aStream.Sink().Type(),
                                        aStream.Sink().Data().InitParamL( codec ),
                                        aStream.Sink().Id() );
            }

        MCEMM_DEBUG_STREAM( "CMceMediaManager::CreateMccStreamL(): creating stream", aStream );
        //create stream    
        User::LeaveIfError( iMccInterface->CreateStream( 
                                    aStream.SessionId(),
                                    aStream.LinkId(),
                                    aStream.Id(),
                                    aStream.MccStreamType(),
                                    *mccCodec ) );
        
        aStream.Source().Data().InitializedL();
        aStream.Sink().Data().InitializedL();

        }

    CleanupStack::PopAndDestroy( mccCodec );
    
    MCEMM_DEBUG("CMceMediaManager::CreateMccStreamL(), Exit ");
    }

    
// ---------------------------------------------------------
// CMceMediaManager::PrepareMccStreamL
// ---------------------------------------------------------    
//
void CMceMediaManager::PrepareMccStreamL( 
            CMceSrvStream& aStream,
            TMceNegotiationRole /*aRole*/ )
    {

    MCEMM_DEBUG("CMceMediaManager::PrepareMccStreamL(), Entry ");

    __ASSERT_ALWAYS( aStream.State() != CMceSrvStream::EAdopted,
                     User::Leave( KErrArgument ) );
    
    if ( !aStream.PrepareL() )
        {
        //prepare stream
        TUint32 endpointId = 0;
        User::LeaveIfError( iMccInterface->PrepareStream( 
                                    aStream.SessionId(),
                                    aStream.LinkId(),
                                    aStream.Id(),
                                    endpointId ) );

        SetPendingState( aStream, endpointId, CMceSrvStream::EPreparing );
        
        aStream.MccPrepareCalledL();
        
        }
    MCEMM_DEBUG_STREAM( "CMceMediaManager::PrepareMccStreamL(): after prepare", aStream );

    MCEMM_DEBUG("CMceMediaManager::PrepareMccStreamL(), Exit ");
    }


// ---------------------------------------------------------
// CMceMediaManager::SetRemoteAddressL
// ---------------------------------------------------------   
//
void CMceMediaManager::SetRemoteAddressL( CMceSrvStream& aStream )
    {
    TInetAddr remoteIpAddress = aStream.RemoteIpAddress();
    
    MCEMM_DEBUG_DVALUE( "CMceMediaManager::SetRemoteAddressL address",
        remoteIpAddress.Address() )
    MCEMM_DEBUG_DVALUE( "CMceMediaManager::SetRemoteAddressL port",
        remoteIpAddress.Port() ) 
    
    //for rtp
    if ( !remoteIpAddress.IsUnspecified() &&
    	 remoteIpAddress.Port() != 0 &&
         aStream.IsMccPrepared() )
        {
        User::LeaveIfError( 
            iMccInterface->SetRemoteAddress( 
                    aStream.SessionId(),
                    aStream.LinkId(),
                    remoteIpAddress ) );
        
         // for rtcp
    	if ( aStream.Data().RemoteMediaPort() != 0 &&
    	     aStream.Data().iRemoteRtcpPort != 0 )
		    {
		    if ( !aStream.Data().iRemoteRtcpAddress.IsUnspecified() )
		    	{
		    	//copy address
		    	remoteIpAddress = aStream.Data().iRemoteRtcpAddress;
		    	}
		    	
		    remoteIpAddress.SetPort( 
		    			aStream.Data().iRemoteRtcpPort );
		    
		    User::LeaveIfError( 
		        iMccInterface->SetRemoteAddress( 
		                aStream.SessionId(),
		                aStream.LinkId(),
		                remoteIpAddress,
		                aStream.Data().iRemoteRtcpPort ) );
		    }
        }
    }


// ---------------------------------------------------------
// CMceMediaManager::StartMccStreamL
// ---------------------------------------------------------   
//
void CMceMediaManager::StartMccStreamL( CMceSrvStream& aStream )
    {
    
    MCEMM_DEBUG("CMceMediaManager::StartMccStreamL(), Entry ");
    
    __ASSERT_ALWAYS( aStream.State() != CMceSrvStream::EAdopted,
                     User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aStream.State() == CMceSrvStream::EPrepared, 
                     User::Leave( KErrArgument ) );

    if ( !aStream.StartL() )
        {
        //set remote address for link
        SetRemoteAddressL( aStream );
        
        if ( !DoStartEndpointsL( aStream, 0, 0 ) &&
             aStream.Sink().State() == CMceSrvStream::EPrepared &&
             aStream.Source().State() == CMceSrvStream::EPrepared )
             
        	{
            SetPendingState( aStream, 0, CMceSrvStream::EPending );
        	}
        }

    MCEMM_DEBUG_STREAM( "CMceMediaManager::StartMccStreamL(): after start", aStream );
        
    MCEMM_DEBUG("CMceMediaManager::StartMccStreamL(), Exit ");
    }


// ---------------------------------------------------------
// CMceMediaManager::StopMccStream
// ---------------------------------------------------------   
//
void CMceMediaManager::StopMccStream( CMceSrvStream& aStream )
    {
    MCEMM_DEBUG("CMceMediaManager::StopMccStream(), Entry ");
    
    if ( !aStream.Stop() )
        {

        TMccNetSettings netSettings;
        netSettings.iIapId = aStream.IapId();
        netSettings.iLocalAddress.SetPort( aStream.LocalMediaPort() );
        TUint32 linkId = aStream.LinkId();
        
        iMccInterface->StopStream( aStream.SessionId(), 
                                   linkId, 
                                   aStream.Id() );
        
        aStream.Cleanup();
        
        iMccInterface->DeleteStream( aStream.SessionId(), 
                                     linkId, 
                                     aStream.Id() );
                                     
        if ( !iMccInterface->StreamsExists( aStream.SessionId(), 
                                            linkId ) )
            {
            MCEMM_DEBUG("CMceMediaManager::StopMccStream(): closing link ");
            iMccInterface->CloseLink( aStream.SessionId(),
                                      linkId );
            aStream.SetLinkId( KMceNotAssigned );
            }
            
        }
        
    aStream.Invalidate();
    
    MCEMM_DEBUG_STREAM( "CMceMediaManager::StopMccStream(): after stop", aStream );
    MCEMM_DEBUG("CMceMediaManager::StopMccStream(), Exit ");
    
    }


// ---------------------------------------------------------
// CMceMediaManager::CanReleaseEndpoint
// ---------------------------------------------------------   
//
TBool CMceMediaManager::CanReleaseEndpoint( CMceSrvStream& aStream, 
                                            TUint32 aEndpointId )
    {
    TBool remove = EFalse;
    TUint referenceCount = 0;
    if ( iMccInterface->ReferenceCount( aStream.SessionId(), 
                                        aStream.LinkId(), 
                                        aStream.Id(),
                                        aEndpointId,
                                        referenceCount ) == KErrNone )
        {
        MCEMM_DEBUG_DVALUE("CMceMediaManager::CanReleaseEndpoint(): \
reference count", referenceCount );
        remove = referenceCount == 1;
        }

    return remove;
    
    }


// ---------------------------------------------------------
// CMceMediaManager::ReleaseSink
// ---------------------------------------------------------   
//
TInt CMceMediaManager::ReleaseSink( CMceSrvStream& aStream,
                                    TUint32 aEndpointId )
    {
    MCEMM_DEBUG("CMceMediaManager::ReleaseSink(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aStream.SessionId() );
    MCEMM_DEBUG_DVALUE("  LinkID", aStream.LinkId() );
    MCEMM_DEBUG_DVALUE("  StreamID", aStream.Id() );
    MCEMM_DEBUG_DVALUE("  EndpointID", aEndpointId );

    TInt err = 
        iMccInterface->RemoveDataSink( aStream.SessionId(), 
                                       aStream.LinkId(),
                                       aStream.Id(),
                                       aEndpointId );
                                       
    MCEMM_DEBUG_DVALUE("CMceMediaManager::ReleaseSink(), Exit. ", err );

    return err;    
    }

// ---------------------------------------------------------
// CMceMediaManager::ReleaseSource
// ---------------------------------------------------------   
//
TInt CMceMediaManager::ReleaseSource( CMceSrvStream& aStream,
                                      TUint32 aEndpointId )
    {
    MCEMM_DEBUG("CMceMediaManager::ReleaseSource(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aStream.SessionId() );
    MCEMM_DEBUG_DVALUE("  LinkID", aStream.LinkId() );
    MCEMM_DEBUG_DVALUE("  StreamID", aStream.Id() );
    MCEMM_DEBUG_DVALUE("  EndpointID", aEndpointId );

    TInt err = 
        iMccInterface->RemoveDataSource( aStream.SessionId(), 
                                       aStream.LinkId(),
                                       aStream.Id(),
                                       aEndpointId );
                                       
    MCEMM_DEBUG_DVALUE("CMceMediaManager::ReleaseSource(), Exit. ", err );
    
    return err;    
    }
    

// ---------------------------------------------------------
// CMceMediaManager::CleanupL
// ---------------------------------------------------------   
//
void CMceMediaManager::CleanupL( CMceComSession& aSession )
    {

    MCEMM_DEBUG("CMceMediaManager::CleanupL(), Entry ");
    CMceSrvStream* stream = NULL;

    if ( aSession.AnswerType() == KMceNegotiationAnswerTypeFinal )
        {
        // check if non-local media was rejected, if yes then stop the stream

        TMceSrvStreamIterator rejects( aSession.MccStreams(), 
                                       TMceSrvStreamIterator::EReceive );

        //stop all started downgraded receive streams, which are downgraded by the answer
        while( rejects.Next( stream, CMceSrvStream::EStarted, 
                                     TMceSrvStreamIterator::GreaterOrEqualMatch ) )
            {
            if ( !stream->Data().IsEnabled() )
                {
                MCEMM_DEBUG_STREAM( "CMceMediaManager::CleanupL(): \
    synchronizing downgraded stream", *stream );
                SynchronizeMccStreamL( *stream, aSession.NegotiationState().Role() );
                }
            }
                                       
        
        TMceSrvStreamIterator iterator( aSession.MccStreams(), 
                                        TMceSrvStreamIterator::ELocal );
        
        //cleanup rejected media
        while( iterator.Next( stream, TMceSrvStreamIterator::ExactReverseMatch ) )
            {
            if ( stream->Data().RemoteMediaPort() == 0 )
                {
                MCEMM_DEBUG_STREAM( "CMceMediaManager::CleanupL(): \
    stream rejected. Disposing stream", *stream );
                CMceComMediaStream* media = &stream->Data();
                DisposeMediaStreamL( media );
                iterator.Reset();
                }
            }

        iterator.Reset();
        
        //cleanup rejected codecs
        while( iterator.Next( stream, TMceSrvStreamIterator::ExactReverseMatch ) )
            {
            if ( !stream->Codec().iIsNegotiated )
                {
                MCEMM_DEBUG_STREAM( "CMceMediaManager::CleanupL(): \
    codec rejected. Disposing codec", *stream );
                CMceComCodec* codec = &stream->Codec();
                DisposeCodecL( codec );
                iterator.Reset();
                }
            }
        }
     
    for ( TInt i = 0; i < aSession.Streams().Count(); i++ )
        {    
        // First codec of send streams is always enabled automatically,
        // if such codec is removed/changed because of negotiation, first
        // one of the selected codecs is enabled.
        //
        MCEMM_DEBUG("CMceMediaManager::CleanupL(), updating default codecs ");
        
        CMceComMediaStream* sendStream = aSession.Streams()[ i ]->SendStream();
        if ( sendStream )
            {
            sendStream->UpdateDefaultCodecL();
            }
        }

    
    MCEMM_DEBUG("CMceMediaManager::CleanupL(), Exit ");

    }


// ---------------------------------------------------------
// CMceMediaManager::DisposeMediaStreamL
// ---------------------------------------------------------   
//
void CMceMediaManager::DisposeMediaStreamL( CMceComMediaStream* aStream )
    {
    __ASSERT_ALWAYS( aStream, User::Leave( KErrArgument ) );
    
    CMceSrvStream* stream = NULL;
    CMceComSession& session = *aStream->Session();

    //find all mcc streams related to stream and
    //and remove mcc streams
    TMceSrvStreamIterator removed( session.MccStreams(), 
                                   *aStream );
    while( removed.Next( stream ) )
        {
        MCEMM_DEBUG_DVALUE("CMceMediaManager::DisposeMediaStreamL(): \
stopping and removing stream", stream->Id() );
        StopMccStream( *stream );
        removed.Remove( session.MccStreams() );
        delete stream;
        stream = NULL;
        }
    //make it inactive        
    aStream->Enable( EFalse, ETrue );

    //if not found strem is bounded stream        
    TInt streamIndex = session.Streams().Find( aStream );
    
    if ( streamIndex >= 0 && !aStream->iLinkedStream )
        {
        session.Streams().Remove( streamIndex );
        delete aStream;
        }
        
    }


// ---------------------------------------------------------
// CMceMediaManager::DisposeCodecL
// ---------------------------------------------------------   
//
void CMceMediaManager::DisposeCodecL( CMceComCodec* aCodec )
    {
    if ( aCodec )
        {
        CMceSrvStream* stream = NULL;
        CMceComSession& session = *aCodec->Stream()->Session();

        MCEMM_DEBUG_SVALUE("CMceMediaManager::DisposeCodecL(): \
    about to remove codec", aCodec->iSdpName );
        
        //find all mcc streams related to stream and
        //and remove mcc streams
        TMceSrvStreamIterator removed( session.MccStreams(), 
                                       *aCodec );
        while( removed.Next( stream ) )
            {
            MCEMM_DEBUG_STREAM( "CMceMediaManager::DisposeCodecL(): \
    stopping and removing stream", *stream );

            StopMccStream( *stream );
            removed.Remove( session.MccStreams() );
            
            // dispose also associated local streams                                 
            TMceSrvStreamIterator associatedLocalStreams( 
                session.MccStreams(), stream->Source().Data(), 
                ETrue, TMceSrvStreamIterator::ELocal, aCodec );
            CMceSrvStream* associatedLocalStream = NULL;
            while( associatedLocalStreams.Next( associatedLocalStream ) )
                {
                MCEMM_DEBUG_STREAM( "CMceMediaManager::DisposeCodecL(): \
    stopping and removing associated local stream", *associatedLocalStream );
    
                StopMccStream( *associatedLocalStream );
                associatedLocalStreams.Remove( session.MccStreams() );
                delete associatedLocalStream;
                associatedLocalStream = NULL;
                }
            delete stream;
            stream = NULL;
            }
        MCEMM_DEBUG_SVALUE("CMceMediaManager::DisposeCodecL(): \
    removing codec", aCodec->iSdpName );
    
        CMceComCodec* boundCodec = NULL;
        if ( aCodec->Stream()->BoundStream() )
            {
            boundCodec = aCodec->Stream()->BoundStreamL().FindCodecL( *aCodec );
            }
            
        aCodec->Stream()->RemoveCodecL( aCodec );
        
        DisposeCodecL( boundCodec );
        
        MCEMM_DEBUG("CMceMediaManager::DisposeCodecL(), done ");
        }
    }


// ---------------------------------------------------------
// CMceMediaManager::AdjustMccStreamL
// ---------------------------------------------------------   
//
void CMceMediaManager::AdjustMccStreamL( CMceSrvStream& aStream )
    {

    MCEMM_DEBUG("CMceMediaManager::AdjustMccStreamL(), Entry ");
    
    CMceComCodec* bidirectional = DoAdjustMccStreamL( aStream );
    
    if ( bidirectional )
        {
        CMceSrvStream* stream = NULL;
        TMceSrvStreamIterator streams( aStream.Codec().Stream()->Session()->MccStreams(), 
                                       *bidirectional );
        while( streams.Next( stream ) )
            {
            DoAdjustMccStreamL( *stream );
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::AdjustMccStreamL(), Exit ");
    
    }



// ---------------------------------------------------------
// CMceMediaManager::ValidateMccStreamL
// ---------------------------------------------------------   
//
void CMceMediaManager::ValidateMccStreamL( CMceSrvStream& aStream )
    {
    
    MCEMM_DEBUG("CMceMediaManager::ValidateMccStreamL(), Entry ");
    
    __ASSERT_ALWAYS( aStream.State() != CMceSrvStream::EAdopted,
                     User::Leave( KErrArgument ) );
    
    //if valid
    if ( aStream.Validate() )
        {
        return;
        }
         
    TMceNegotiationRole role = EMceRoleOfferer;
            
    CMccCodecInformation* codec = NULL;
    
    if ( !aStream.IsMccPrepared() )
        {
        role = EMceRoleAnswerer;
        //create codec information
        CMccCodecInformationFactory* codecFactory = 
            CMccCodecInformationFactory::NewL();
        CleanupStack::PushL( codecFactory );
        
        codec = 
            codecFactory->CreateCodecInformationL( aStream.Codec().iSdpName );

        CleanupStack::PopAndDestroy( codecFactory );
            
        }
    else
        {
        //get codec
        codec = 
            iMccInterface->CodecL(         
                    aStream.SessionId(),
                    aStream.LinkId(),
                    aStream.Id() );
        }
        
    CleanupStack::PushL( codec );
        
    //validate codec
    aStream.Codec().MccValidateL( *codec, aStream, role );

    CleanupStack::PopAndDestroy( codec );
    
    MCEMM_DEBUG("CMceMediaManager::ValidateMccStreamL(), Exit ");
    }



// ---------------------------------------------------------
// CMceMediaManager::PrepareUpdateL 
// ---------------------------------------------------------    
//
void CMceMediaManager::PrepareUpdateL( CMceSdpSession& aSession )
    {
    MCEMM_DEBUG("CMceMediaManager::PrepareUpdateL(), Entry ");

    CMceComSession* session = aSession.MediaSession();
    CMceComSession* backup = aSession.Backup();
    
    TInt action = RequireSignallingL( *session, *backup );
    
    //do local update
    if ( action != KMceRequiresSignalling )
        {
        MCEMM_DEBUG("CMceMediaManager::PrepareUpdateL(): \
no need for new mcc session. Doing merge"); 
        session->Merge( *backup );
        }
     else if( session )
     	{
	     if( session->iCodecSelection == KMceCodecSelectionMerge )
	     	{
	     	session->Merge( *backup );
	     	}
     	}
     	

    aSession.SignallingRequired() = action;
    
    MCEMM_DEBUG("CMceMediaManager::PrepareUpdateL(), Exit ");
    
    }


// ---------------------------------------------------------
// CMceMediaManager::RequireSignallingL 
// ---------------------------------------------------------    
//
TInt CMceMediaManager::RequireSignallingL( 
                                  CMceComSession& aSession,
                                  CMceComSession& aBackup )
 
    {
    MCEMM_DEBUG("CMceMediaManager::RequireSignallingL(), Entry ");

    TInt action = KMceNoSignalling;

    TMceSrvStreamIterator streams( 
                        aBackup.MccStreams(), 
                        TMceSrvStreamIterator::ESendOrReceive );
                        
    TMceSrvStreamIterator updates( 
                        aSession.MccStreams(),
                        TMceSrvStreamIterator::ESendOrReceive );
    
    CMceSrvStream* update = NULL;
    CMceSrvStream* stream = NULL;
    //intersection i.e. streams both in update and original
    while( !action && 
            updates.NextIntersect( update, streams, stream ) )
        {
        //current codec
        if ( stream->IsMccPrepared() )
            {
            
            CMccCodecInformation* codec = 
                iMccInterface->CodecL( stream->SessionId(),
                                       stream->LinkId(),
                                       stream->Id() );
            
            CleanupStack::PushL( codec );
            
            //updated codec
            CMccCodecInformation* codecUpdate = 
                CreateMccCodecLC( *update, EMceRoleOfferer );
                                          
            action = update->RequireSignalling( *stream,
                                                *codec,
                                                *codecUpdate );
                                                   
            CleanupStack::PopAndDestroy( codecUpdate );
            CleanupStack::PopAndDestroy( codec );
            }
        else 
            {
            action = stream->RequireSignalling( aSession.MccStreams() );
            }
        }

    updates.Reset();
    //added to update
    while( !action && 
            updates.NextComplement( update, streams ) )
        {
        action = update->RequireSignalling( aSession.MccStreams() );
        }

    streams.Reset();
    //removed from update
    while( !action && 
            streams.NextComplement( stream, updates ) )
                
        {
        action = stream->RequireSignalling( aSession.MccStreams() );
        }

        
    MCEMM_DEBUG("CMceMediaManager::RequireSignallingL(), Exit ");
    return action;
            
    }


// ---------------------------------------------------------
// CMceMediaManager::SynchronizeMccStreamL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SynchronizeMccStreamL( 
    CMceSrvStream& aStream, 
    TMceNegotiationRole aRole,
    TBool aSyncCodecOnly )
    {
    MCEMM_DEBUG("CMceMediaManager::SynchronizeMccStreamL(), Entry ");
    
    if ( !aStream.SynchronizeL() )
        {
    
        CMccCodecInformation* codec = 
            iMccInterface->CodecL( aStream.SessionId(),
                                   aStream.LinkId(),
                                   aStream.Id() );
        CleanupStack::PushL( codec );
        //re-populate codec with updated values
        aStream.Codec().MccPopulateL( *codec, aStream, aRole );
        
        MCEMM_DEBUG("CMceMediaManager::SynchronizeMccStreamL(): \
re-setting codec");
        
        User::LeaveIfError( 
            iMccInterface->SetCodec( 
                    aStream.SessionId(),
                    aStream.LinkId(),
                    aStream.Id(),
                    *codec ) );
                    
        CleanupStack::PopAndDestroy( codec );

        if ( !aSyncCodecOnly )
            {
            SynchronizeMccStreamL( aStream );
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::SynchronizeMccStreamL(), Exit ");
    }
    
// ---------------------------------------------------------
// CMceMediaManager::SynchronizeMccStreamL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SynchronizeMccStreamL( 
    CMceSrvStream& aStream, 
    TUint32 aForceEnableEndpoint, 
    TUint32 aForceDisableEndpoint )
    {
    MCEMM_DEBUG("CMceMediaManager::SynchronizeMccStreamL() two, Entry ");
    
    if ( !DoStartEndpointsL( aStream, aForceEnableEndpoint, aForceDisableEndpoint ) )
        {
        if ( !DoResumeEndpointsL( aStream, aForceEnableEndpoint ) )
            {
            DoPauseEndpointsL( aStream, aForceDisableEndpoint );
            }
        }
        
    MCEMM_DEBUG_STREAM( "CMceMediaManager::SynchronizeMccStreamL() two: \
syncronized stream", aStream );

    MCEMM_DEBUG("CMceMediaManager::SynchronizeMccStreamL() two, Exit ");
    }
    
// ---------------------------------------------------------
// CMceMediaManager::ReuseLink
// ---------------------------------------------------------    
//
TBool CMceMediaManager::ReuseLink( CMceSrvStream& aStream )
    {
    TBool inUse = EFalse;

    CMceComSession* session = aStream.Data().Session();
    CMceSrvStream* stream = NULL;
    
    TMceSrvStreamIterator 
        sourceMatchStreams( session->MccStreams(), 
                            aStream.Source().Data(),
                            KMceComUseProxyMatch );

    TMceSrvStreamIterator 
        sinkMatchStreams( session->MccStreams(), 
                          aStream.Sink().Data(),
                          KMceComUseProxyMatch );
                                   
    while( !inUse && sourceMatchStreams.Next( stream ) )
        {
        if ( ( aStream.Data().iStreamType == CMceComMediaStream::ELocalStream || 
               aStream.Source().Data().Type() == KMceRTPSource ) &&
               stream->Data().iLinkId != KMceNotAssigned )
            {
            inUse = ETrue;
            aStream.Data().iLinkId = stream->Data().iLinkId;
            }
        }

    while( !inUse && sinkMatchStreams.Next( stream ) )
        {
        if ( ( aStream.Data().iStreamType == CMceComMediaStream::ELocalStream || 
               aStream.Sink().Data().Type() == KMceRTPSink ) &&
               stream->Data().iLinkId != KMceNotAssigned )
            {
            inUse = ETrue;
            aStream.Data().iLinkId = stream->Data().iLinkId;
            }
        }
    
    return inUse;
    
    }
    
// ---------------------------------------------------------
// CMceMediaManager::ReuseSource
// ---------------------------------------------------------    
//
TBool CMceMediaManager::ReuseSource( CMceSrvStream& aStream )
    {
    TBool inUse = EFalse;
    
    if ( aStream.Source().Reusable( aStream ) )
        {
        CMceComSession* session = aStream.Data().Session();
        CMceSrvStream* stream = NULL;
        
        TMceSrvStreamIterator streams( session->MccStreams(), 
                                       aStream.Source().Data(),
                                       KMceComUseProxyMatch );
                                       
        while( !inUse && streams.Next( stream ) )
            {
            if ( stream->Source().Id() != KMceNotAssigned )
                {
                inUse = ETrue;
                aStream.Id() = stream->Id();
                aStream.Source().Id() = stream->Source().Id();
                }
            }
        }
        
    return inUse;
    
    }

// ---------------------------------------------------------
// CMceMediaManager::ReuseSink
// ---------------------------------------------------------    
//
TBool CMceMediaManager::ReuseSink( CMceSrvStream& aStream )
    {
    TBool inUse = EFalse;

    if ( aStream.Sink().Reusable( aStream ) )
        {
        CMceComSession* session = aStream.Data().Session();
        CMceSrvStream* stream = NULL;
        
        TMceSrvStreamIterator streams( session->MccStreams(), 
                                       aStream.Sink().Data(),
                                       KMceComUseProxyMatch );
                                       
        while( !inUse && streams.Next( stream ) )
            {
            if ( stream->Sink().Id() != KMceNotAssigned )
                {
                inUse = ETrue;
                aStream.Id() = stream->Id();
                aStream.Sink().Id() = stream->Sink().Id();
                }
            }
        }
        
    return inUse;
    
    }

// ---------------------------------------------------------
// CMceMediaManager::SetMccParameterL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SetMccParameterL( TUint32 aParam,
                                         const TDesC8& aValue ) const
    {
    
    User::LeaveIfError( 
        iMccInterface->SetParameter( 0, 0, 0, 0, aParam, aValue ) );
    
    }

// ---------------------------------------------------------
// CMceMediaManager::GetMccParameterL 
// ---------------------------------------------------------    
//
void CMceMediaManager::GetMccParameterL( TUint32 aParam,
                                         TDes8& aValue ) const
    {
    User::LeaveIfError( 
        iMccInterface->GetParameter( 0, 0, 0, 0, aParam, aValue ) );
    }
    

// ---------------------------------------------------------
// CMceMediaManager::SetMccParameterL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SetMccParameterL( const CMceComMediaSink& aMediaSink,
                                        TUint32 aParam,
                                        const TDesC8& aValue,
                                        TBool aAllowNonPrepared ) const
                                                
    { 
    CMceComSession* session = aMediaSink.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aMediaSink );
    CMceSrvStream* stream = NULL;
    TBool done = EFalse;
    
    while( !done && streams.Next( stream ) )
        {
        if ( stream->IsMccPrepared() || aAllowNonPrepared )
            {
            MCEMM_DEBUG_STREAM( "CMceMediaManager::SetMccParameterL(): \
setting parameter of stream", *stream );
            User::LeaveIfError( 
                iMccInterface->SetParameter( stream->SessionId(),
                                              stream->LinkId(),
                                              stream->Id(),
                                              stream->Sink().Id(),
                                              aParam,
                                              aValue ) );
            }
        else
            {
            SetMccParameterL( aParam, aValue );
            }
        done = ETrue;
        }
        
    }

// ---------------------------------------------------------
// CMceMediaManager::SetMccParameterL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SetMccParameterL( const CMceComMediaSource& aMediaSource,
                                         TUint32 aParam,
                                         const TDesC8& aValue,
                                         TBool aAllowNonPrepared ) const
                                                
    { 
    
    CMceComSession* session = aMediaSource.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aMediaSource );
    CMceSrvStream* stream = NULL;
    TBool done = EFalse;
    
    while( !done && streams.Next( stream ) )
        {
        if ( stream->IsMccPrepared() || aAllowNonPrepared )
            {
            MCEMM_DEBUG_STREAM( "CMceMediaManager::SetMccParameterL(): \
setting parameter of stream", *stream );
            User::LeaveIfError( 
                iMccInterface->SetParameter( stream->SessionId(),
                                              stream->LinkId(),
                                              stream->Id(),
                                              stream->Source().Id(),
                                              aParam,
                                              aValue ) );
            }
        else
            {
            SetMccParameterL( aParam, aValue );
            }
        done = ETrue;
        }
    
    }

    


// ---------------------------------------------------------
// CMceMediaManager::GetMccParameterL 
// ---------------------------------------------------------    
//
void CMceMediaManager::GetMccParameterL( const CMceComMediaSink& aMediaSink,
                                         TUint32 aParam,
                                         TDes8& aValue,
                                         TBool aAllowNonPrepared ) const
                                                
    { 
    
    CMceComSession* session = aMediaSink.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aMediaSink );
    CMceSrvStream* stream = NULL;
    TBool done = EFalse;
    
    while( !done && streams.Next( stream ) )
        {
        if ( stream->IsMccPrepared() || aAllowNonPrepared )
            {
            MCEMM_DEBUG_STREAM( "CMceMediaManager::GetMccParameterL(): \
getting parameter of stream", *stream );
            User::LeaveIfError( 
                iMccInterface->GetParameter( stream->SessionId(),
                                              stream->LinkId(),
                                              stream->Id(),
                                              stream->Sink().Id(),
                                              aParam,
                                              aValue ) );
            }
        else
            {
            GetMccParameterL( aParam, aValue );
            }
        done = ETrue;
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::GetMccParameterL 
// ---------------------------------------------------------    
//
void CMceMediaManager::GetMccParameterL( const CMceComMediaSource& aMediaSource,
                                         TUint32 aParam,
                                         TDes8& aValue,
                                         TBool aAllowNonPrepared ) const
                                                
    { 
    
    CMceComSession* session = aMediaSource.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aMediaSource );
    CMceSrvStream* stream = NULL;
    TBool done = EFalse;
    
    while( !done && streams.Next( stream ) )
        {
        if ( stream->IsMccPrepared() || aAllowNonPrepared )
            {
            MCEMM_DEBUG_STREAM( "CMceMediaManager::GetMccParameterL(): \
getting parameter of stream", *stream );
            User::LeaveIfError( 
                iMccInterface->GetParameter( stream->SessionId(),
                                              stream->LinkId(),
                                              stream->Id(),
                                              stream->Source().Id(),
                                              aParam,
                                              aValue ) );
            
            }
        else
            {
            GetMccParameterL( aParam, aValue );
            }
        done = ETrue;
        }
    
    }


// ---------------------------------------------------------
// CMceMediaManager::UpdateMccSourceL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::UpdateMccSourceL( CMceComCameraSource& aMediaSource )
    {
    CMceComSession* session = aMediaSource.MediaStream()->Session();
    
    TMceSrvStreamIterator streams( session->MccStreams(), aMediaSource );
    CMceSrvStream* stream = NULL;
    TBool done = EFalse;
    
    while( !done && streams.Next( stream ) )
        {
        CMceComCodec& codec = stream->Codec();
        
        if ( stream->IsMccPrepared() )
            {
            MCEMM_DEBUG_STREAM( "CMceMediaManager::UpdateMccSourceL(): \
updating data source of stream", *stream );
            User::LeaveIfError( 
                iMccInterface->UpdateDataSource( stream->SessionId(),
                                                 stream->LinkId(),
                                                 stream->Id(),
                                                 stream->Source().Id(),
                                                 aMediaSource.InitParamL( codec ) ) );
            done = ETrue;
            }
        }
    }

/*
** From MMccCtrlObserver ***
*/

// ---------------------------------------------------------
// CMceMediaManager::MccMediaLinkCreated 
// ---------------------------------------------------------    
//
void CMceMediaManager::MccMediaLinkCreated( TUint32 aSessionId,
                                            TUint32 aLinkId,
                                            const TDesC8& aNetSettings )

    {
    MCEMM_DEBUG("CMceMediaManager::MccMediaLinkCreated(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );

    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccLinkCreated,
                               aLinkId, 
                               0, 
                               0 );

        event.SetDataDesC( aNetSettings );
        
        TRAPD( err, 
            session->NegotiationState().MccLinkCreatedL( event ) );
            
        if( err != KErrNone )
            {
            MCEMM_DEBUG_DVALUE("Error occured!, error", err );
            session->NegotiationState().MccError( err, event );
            }
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
        
    MCEMM_DEBUG("CMceMediaManager::MccMediaLinkCreated(), Exit ");
        
    }

    
// ---------------------------------------------------------
// CMceMediaManager::MccMediaPrepared 
// ---------------------------------------------------------    
//
void CMceMediaManager::MccMediaPrepared( TUint32 aSessionId,
                                         TUint32 aLinkId,
                                         TUint32 aStreamId,
                                         TUint32 aSinkSourceId )
    {
    MCEMM_DEBUG("CMceMediaManager::MccMediaPrepared(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aSinkSourceId );

    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccStreamPrepared,
                               aLinkId, 
                               aStreamId, 
                               aSinkSourceId );
        
        TRAPD( err, 
            session->NegotiationState().MccMediaPreparedL( event ) );
            
        if( err != KErrNone )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::MccMediaPrepared(): \
Error occured!, error", err );
            session->NegotiationState().MccError( err, event );
            }
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
        
    MCEMM_DEBUG("CMceMediaManager::MccMediaPrepared(), Exit ");
        
    }

// ---------------------------------------------------------
// CMceMediaManager::MccMediaStarted
// --------------------------------------------------------- 
//   
void CMceMediaManager::MccMediaStarted( TUint32 aSessionId,
                                        TUint32 aLinkId,     
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    MCEMM_DEBUG("CMceMediaManager::MccMediaStarted(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aSinkSourceId );

    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccStreamStarted,
                               aLinkId, 
                               aStreamId, 
                               aSinkSourceId );

        TRAPD( err, 
            session->NegotiationState().MccMediaStartedL( event ) );
        if( err != KErrNone )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::MccMediaStarted(): \
Error occured!, error", err );
            session->NegotiationState().MccError( err, event );
            }
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
    MCEMM_DEBUG("CMceMediaManager::MccMediaStarted(), Exit ");
    }
        
    
// ---------------------------------------------------------
// CMceMediaManager::MccMediaStopped
// --------------------------------------------------------- 
//      
void CMceMediaManager::MccMediaStopped( TUint32 aSessionId,
                                        TUint32 aLinkId,     
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    MCEMM_DEBUG("CMceMediaManager::MccMediaStopped(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aSinkSourceId );
    
    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccStreamStopped,
                               aLinkId, 
                               aStreamId, 
                               aSinkSourceId );
    
        CMceSrvStream::EventReceived( session->MccStreams(), event );
    
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
    MCEMM_DEBUG("CMceMediaManager::MccMediaStopped(), Exit ");
        
    }

    
// ---------------------------------------------------------
// CMceMediaManager::MccMediaPaused
// ---------------------------------------------------------  
//      
void CMceMediaManager::MccMediaPaused( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    MCEMM_DEBUG("CMceMediaManager::MccMediaPaused(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aSinkSourceId );
    
    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccStreamPaused,
                               aLinkId, 
                               aStreamId, 
                               aSinkSourceId );
                               
        TRAPD( err, 
            session->NegotiationState().MccMediaPausedL( event ) );
        if( err != KErrNone )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::MccMediaPaused(): \
Error occured!, error", err );
            session->NegotiationState().MccError( err, event );
            }
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
    MCEMM_DEBUG("CMceMediaManager::MccMediaPaused(), Exit ");
    }


                           
// ---------------------------------------------------------
// CMceMediaManager::MccMediaResumed
// --------------------------------------------------------- 
//      
void CMceMediaManager::MccMediaResumed( TUint32 aSessionId,
                                        TUint32 aLinkId,
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    MCEMM_DEBUG("CMceMediaManager::MccMediaResumed(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aSinkSourceId );
    
    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccStreamResumed,
                               aLinkId, 
                               aStreamId, 
                               aSinkSourceId );
    
        TRAPD( err, 
            session->NegotiationState().MccMediaResumedL( event ) );
        if( err != KErrNone )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::MccMediaResumed(): \
Error occured!, error", err );
            session->NegotiationState().MccError( err, event );
            }
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
    MCEMM_DEBUG("CMceMediaManager::MccMediaResumed(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::MccMediaInactive
// ---------------------------------------------------------    
//  

void CMceMediaManager::MccMediaInactive( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    MCEMM_DEBUG("CMceMediaManager::MccMediaInactive(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aSinkSourceId );
    
    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccInactivityEvent, 
                               aLinkId, 
                               aStreamId, 
                               aSinkSourceId );
    
        CMceSrvStream::EventReceived( session->MccStreams(), event );
    
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
    MCEMM_DEBUG("CMceMediaManager::MccMediaInactive(), Exit ");
    }
    

// ---------------------------------------------------------
// CMceMediaManager::MccMediaActive
// ---------------------------------------------------------    
//  

void CMceMediaManager::MccMediaActive( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    MCEMM_DEBUG("CMceMediaManager::MccMediaActive(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aSinkSourceId );
    
    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccActivityEvent, 
                               aLinkId, 
                               aStreamId, 
                               aSinkSourceId );
    
        CMceSrvStream* stream = 
            CMceSrvStream::EventReceived( session->MccStreams(), event );
    
        CMceSrvStream* adopted = session->RemoveAdoptedStream( *stream );
        if ( adopted )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::MccMediaActive(): \
stopping adopted stream. id", adopted->Id() );
            StopMccStream( *adopted );
            if ( !session->IsMerged() && session->AdoptedMccStreams().Count() == 0 )
                {
                MCEMM_DEBUG_DVALUE("Closing session. id", adopted->SessionId() );
                iMccInterface->CloseSession( adopted->SessionId() );
                }
            delete adopted;
            }
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
    MCEMM_DEBUG("CMceMediaManager::MccMediaActive(), Exit ");
    }
                             
// ---------------------------------------------------------
// CMceMediaManager::MccCtrlError
// ---------------------------------------------------------    
//  
/*void CMceMediaManager::MccCtrlError( TInt aError )
    {
    MCEMM_DEBUG("CMceMediaManager::MccCtrlError(), Entry ");
    MCEMM_DEBUG_DVALUE("  aError", aError );
    
    iObserver->MediaError( aError );
    MCEMM_DEBUG("CMceMediaManager::MccCtrlError(), Exit ");
    }*/

// ---------------------------------------------------------
// CMceMediaManager::MccCtrlError
// ---------------------------------------------------------   
//  
void CMceMediaManager::MccCtrlError( TInt aError,
                                       TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {   
    MCEMM_DEBUG("CMceMediaManager::MccCtrlError(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aSinkSourceId );
    MCEMM_DEBUG_DVALUE("  error", aError );

    CMceComSession* session = Session( aSessionId );
    if ( session )
        {
        TMceMccComEvent event( KMccStreamError, 
                               aLinkId, 
                               aStreamId, 
                               aSinkSourceId );
        
        session->NegotiationState().MccError( aError, event );
            
        }
    else
        {
        MCEMM_DEBUG("No Session!");
        iObserver->MediaError( KErrGeneral );
        }
    MCEMM_DEBUG("CMceMediaManager::MccCtrlError(), Exit ");
        
    }

// ---------------------------------------------------------
// CMceMediaManager::MccEventReceived
// --------------------------------------------------------- 
//      
void CMceMediaManager::MccEventReceived( const TMccEvent& aEvent )
    {
    MCEMM_DEBUG("CMceMediaManager::MccEventReceived(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aEvent.iSessionId );
    MCEMM_DEBUG_DVALUE("  LinkID", aEvent.iLinkId );
    MCEMM_DEBUG_DVALUE("  StreamID", aEvent.iStreamId );
    MCEMM_DEBUG_DVALUE("  EndpointID", aEvent.iEndpointId );
    
    MCEMM_DEBUG_DVALUE("  Event category", aEvent.iEventCategory );
    MCEMM_DEBUG_DVALUE("  Event type", aEvent.iEventType );
    
    if ( aEvent.iEventCategory == KMccEventCategoryTranscode )
        {
        MccTranscodeEventReceived( aEvent );
        }
    else if ( aEvent.iEventType == KMccLinkCreated ||
              aEvent.iEventType == KMccLinkSecure ||
    		  aEvent.iEventType == KMccLinkSecureVideo )
        {
        MccMediaLinkCreated( aEvent.iSessionId, aEvent.iLinkId,
                             aEvent.iEventData );
        }
    else
        {
                    
        CMceComSession* session = Session( aEvent.iSessionId );
        if ( !session )
            {
            MCEMM_DEBUG("no session");
            MCEMM_DEBUG("CMceMediaManager::MccEventReceived(), Exit ");
            return;
            }

        TMceMccComEvent event( aEvent.iEventType,
                               aEvent.iLinkId, 
                               aEvent.iStreamId, 
                               aEvent.iEndpointId );
        event.SetDataDesC( aEvent.iEventData );
        
        TMceSrvStreamIterator iterator( session->MccStreams() );
        CMceSrvStream* stream = NULL;
        if ( iterator.Next( stream, event ) )
            {
            if ( aEvent.iEventCategory == KMccEventCategoryDtmf )
                {
                // CodeScanner can't see properly inside to TRAP
                // and in some cases it report Problem: 
                // Leaving functions called in non-leaving functions.
                TRAP_IGNORE( \
                	stream->DtmfHandlerL( *iMccInterface ).DtmfReceivedL( \
                	aEvent, event ) );
                
                // Event destination might have been modified
                }
            else if( aEvent.iEventType == KMccMasterKeyStaled )
            	{
            	if ( aEvent.iEventNumData > 0 && 
            	     session->SecureSession() )
            		{
	            	// set up in the session
	            	session->SecureSession()->ContextNeedUpdated( aEvent.iEventNumData );
            		}
            	}
            else if ( aEvent.iEventCategory == KMccEventCategoryRtcp )
                {
                const TMccRtcpEventData& rtcpEvent = 
                    (*reinterpret_cast<const TMccRtcpEventDataPackage*>( 
                        &aEvent.iEventData ))();
                        
                event.iEventSubType = rtcpEvent.iRtcpPacketType;
                event.SetDataDesC( rtcpEvent.iRtcpPacketData );
                }
            else
                {
                // NOP
                }
            
            // Event might contain only link id (e.g. rtcp rr) but
            // we do not want to propagate it to all streams of that link.                               
            CMceSrvStream::EventReceived( session->MccStreams(), event, ETrue );
            }
        else
            {
            MCEMM_DEBUG("CMceMediaManager::MccEventReceived(): \
ERROR: Unknown stream" );
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::MccEventReceived(), Exit ");
    
    }



// ---------------------------------------------------------
// CMceMediaManager::MccTranscodeEventReceived
// ---------------------------------------------------------    
//
void CMceMediaManager::MccTranscodeEventReceived( const TMccEvent& aEvent )
    {
    MCEMM_DEBUG("CMceMediaManager::MccTranscodeEventReceived(), Entry ");
    MCEMM_DEBUG_DVALUE("  SessionID", aEvent.iSessionId );
    
    CMceComFileSource* session = TranscodingSession( aEvent.iSessionId );
    if ( !session )
        {
        MCEMM_DEBUG("no session");
        MCEMM_DEBUG("CMceMediaManager::MccTranscodeEventReceived(), Exit ");
        return;
        }

    TMceMccComEvent event( aEvent.iEventType, 0, 0, 0 );
    event.iSource = session;
    event.iItcEvent = aEvent.iEventType == KMccTranscodeCompleted ?
                      EMceItcTranscodingCompleted : EMceItcTranscodingInProgress;
                      
    event.iItcData = aEvent.iEventNumData;
    event.iError = aEvent.iErrorCode;
    
    session->iClient->EventReceived( event );
    
    if ( aEvent.iEventType == KMccTranscodeCompleted )
        {
        TInt index = iTranscodeSessions.Find( session );
        if ( index >= 0 )
            {
            iTranscodeSessions.Remove( index );
            delete session;
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::MccTranscodeEventReceived(), Exit ");
    
    }
    
    
/*
* Private functions for internal use
*/




// ---------------------------------------------------------
// CMceMediaManager::Session
// ---------------------------------------------------------    
//
CMceComSession* CMceMediaManager::Session( TUint32 aSessionId )
    {
    MCEMM_DEBUG("CMceMediaManager::Session(), Entry ");
    
    CMceComSession* session = NULL;
    
    for ( TInt i=0; i < iMceSdpSessions.Count() && !session; i++ )
        {
        CMceSdpSession* sdpSession = iMceSdpSessions[ i ];
        if ( sdpSession->IsMaster() )
            {
            session = sdpSession->Consumes( aSessionId );
            }
        }
        
    if ( !session )
        {
        MCEMM_DEBUG_DVALUE("Session not found! id", aSessionId )
        iObserver->MediaError( KErrNotFound );
        }
        
    MCEMM_DEBUG("CMceMediaManager::Session(), Exit ");
    return session;
    }

// ---------------------------------------------------------
// CMceMediaManager::SdpSession
// ---------------------------------------------------------    
//
CMceSdpSession* CMceMediaManager::SdpSession( CMceComSession& aSession )
    {

    MCEMM_DEBUG("CMceMediaManager::SdpSession(), Entry ");
    
    TInt error = KErrNone;
    
    CMceSdpSession* sdpSession = &aSession.SdpSession();
    if ( !sdpSession ) //possible clone
        {
        sdpSession = aSession.iCloneOrigin ? &aSession.iCloneOrigin->SdpSession() : NULL;
        
        if ( sdpSession )
        	{
        	sdpSession->SetMediaSession( aSession.iCloneOrigin );
        	}
        }
        
    if ( sdpSession )
        {
        
        if ( &aSession != sdpSession->MediaSession() &&
             &aSession != sdpSession->Backup() )
            {
            TRAP( error, aSession.AttachSDPSessionL( *sdpSession ) );
            }
            
        if ( MCE_IS_NULL_PTR( sdpSession->Backup() ) && //is not update
            !sdpSession->IsMaster() && //is not master
            sdpSession->Context() ) // has context
            {
            sdpSession->ContextSwitch();
            }
        }

        
    if ( error != KErrNone )
        {
        sdpSession = NULL;
        }
        
    MCEMM_DEBUG("CMceMediaManager::SdpSession(), Exit ");
    return sdpSession;
    }
    

// ---------------------------------------------------------
// CMceMediaManager::CloseSession
// Closes MCC session and removes the internal sdp session
// Status : Draft
// ---------------------------------------------------------    
TInt CMceMediaManager::CloseSession( CMceComSession& aSession )
    { 
    MCEMM_DEBUG("CMceMediaManager::CloseSession(), Entry ");

    TInt error = KErrNone;

    CMceSdpSession* sdpSession = SdpSession( aSession );
    if ( !sdpSession )
        {
        MCEMM_DEBUG("No session!");
        MCEMM_DEBUG("CMceMediaManager::CloseSession(), Exit ");
        return KErrNotFound;
        }
    
    CMceComSession* backup = aSession.Backup();
    
    if ( !backup )
    //no backup => close session
        {
        if ( sdpSession->IsMaster() )
            {
            TRAP( error, CleanupSessionL( aSession ) );
            iMccInterface->CloseSession( aSession.iMccID );
            }
        RemoveSession( sdpSession );
        }
    else if ( &aSession == backup )
    //update accepted => close backup
        {
        TRAP( error, UpdateAcceptedL( *sdpSession ) );
        }
    else
    //update rejected => rollback
        {
        TRAP( error, UpdateRejectedL( *sdpSession ) );
        }
            
    MCEMM_DEBUG("CMceMediaManager::CloseSession(), Exit ");
    
    return error;

    }
    

// ---------------------------------------------------------
// CMceMediaManager::UpdateAcceptedL
// ---------------------------------------------------------    
void CMceMediaManager::UpdateAcceptedL( CMceSdpSession& aSdpSession )
    { 
    MCEMM_DEBUG("CMceMediaManager::UpdateAcceptedL(), Entry ");
    
    CMceComSession* session = aSdpSession.MediaSession();
    CMceComSession* backup = aSdpSession.Backup();
    __ASSERT_ALWAYS( session && backup, User::Leave( KErrNotFound ) );
    
    if ( !session->IsMerged() )
    //if not merged session
        {
        MCEMM_DEBUG("CMceMediaManager::UpdateAcceptedL(): \
session is not merged");
        //stop backup's all send streams. 
        //No local streams or receive streams
        TMceSrvStreamIterator 
            sendStreams( backup->MccStreams(), 
                         TMceSrvStreamIterator::ESend );
        CMceSrvStream* sendStream = NULL;
        while( sendStreams.Next( 
                    sendStream,
                    CMceSrvStream::EPrepared, 
                    TMceSrvStreamIterator::GreaterOrEqualMatch ) )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::UpdateAcceptedL(): \
Stopping send stream. id", sendStream->Id() );
            StopMccStream( *sendStream );
            }
        
        //adopt backup's all active receive streams to session
        TMceSrvStreamIterator 
            receiveStreams( backup->MccStreams(), 
                            TMceSrvStreamIterator::EReceive );
        CMceSrvStream* receiveStream = NULL;
        TBool canClose = ETrue;
        while( receiveStreams.Next( 
                        receiveStream,
                        CMceSrvStream::EPrepared, 
                        TMceSrvStreamIterator::GreaterOrEqualMatch ) )
            {
            if ( receiveStream->Data().IsEnabled() )
                {
                MCEMM_DEBUG_DVALUE("CMceMediaManager::UpdateAcceptedL(): \
adopting receive stream. id", receiveStream->Id() );
                session->AdoptStreamL( *receiveStream );
                canClose = EFalse;
                }
            else
                {
                MCEMM_DEBUG_DVALUE("CMceMediaManager::UpdateAcceptedL(): \
stopping receive stream. Id", receiveStream->Id() );
                StopMccStream( *receiveStream );
                }
            }
        //if no receive streams were adopted, backup can be closed
        if ( canClose )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::UpdateAcceptedL(): \
closing session. Id", backup->iMccID );
            iMccInterface->CloseSession( backup->iMccID );
            }
        }
    else
        {
        MCEMM_DEBUG("CMceMediaManager::UpdateAcceptedL(): \
session is merged");
        CleanupSessionL( *backup );
        session->UnMerge();
        }
    
    backup->DetachSDPSession();
    aSdpSession.SetBackup( NULL );
    }


// ---------------------------------------------------------
// CMceMediaManager::UpdateRejectedL
// ---------------------------------------------------------    
void CMceMediaManager::UpdateRejectedL( CMceSdpSession& aSdpSession )
    { 
    MCEMM_DEBUG("CMceMediaManager::UpdateRejected(), Entry ");
    
    CMceComSession* session = aSdpSession.MediaSession();
    CMceComSession* backup = aSdpSession.Backup();
    __ASSERT_ALWAYS( session && backup, User::Leave( KErrNotFound ) );
    
    CleanupSessionL( *session );
    if ( !session->IsMerged() )
    //if not merged session
        {
        MCEMM_DEBUG_DVALUE("Closing session. Id", session->iMccID );
        iMccInterface->CloseSession( session->iMccID );
        }
    else
        {
        MCEMM_DEBUG_DVALUE("Unmerging session. Id", backup->iMccID );
        backup->UnMergeL( *session );
        }
    
    session->DetachSDPSession();
    aSdpSession.SetMediaSession( backup );
    aSdpSession.SetBackup( NULL );
    
    MCEMM_DEBUG("CMceMediaManager::UpdateRejected(), Exit ");
    }

// ---------------------------------------------------------
// CMceMediaManager::CleanupSessionL
// ---------------------------------------------------------    
void CMceMediaManager::CleanupSessionL( CMceComSession& aSession )
    { 
    MCEMM_DEBUG("CMceMediaManager::CleanupSessionL(), Entry ");

    CMceComSession* session = aSession.SdpSession().MediaSession();
    CMceComSession* backup = aSession.SdpSession().Backup();
    
    //close all started unmerged streams
    TMceSrvStreamIterator streams( aSession.MccStreams() );
    CMceSrvStream* stream= NULL;
    while( streams.Next( 
                 stream, 
                 CMceSrvStream::EPrepared, 
                 TMceSrvStreamIterator::GreaterOrEqualMatch ) )
        {
        if ( !stream->IsMerged() )//added or removed
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::CleanupSessionL(): \
stopping unmerged stream. Id", stream->Id() );
            StopMccStream( *stream );
            }
        }

    //close all adopted streams
    TUint32 sessionId = KMceNotAssigned;
    TMceSrvStreamIterator adopts( aSession.AdoptedMccStreams() );
    CMceSrvStream* adopted = NULL;
    while( adopts.Next( adopted ) )
        {
        if ( backup && &aSession == backup )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::CleanupSessionL(): \
re-adopting adopted stream. Id", adopted->Id() );
            adopted = aSession.RemoveAdoptedStream( *adopted );
            CleanupStack::PushL( adopted );
            session->AdoptedMccStreams().AppendL( adopted );
            CleanupStack::Pop( adopted );
            }
        else
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::CleanupSessionL(): \
stopping adopted stream. Id", adopted->Id() );
            sessionId = adopted->SessionId();
            StopMccStream( *adopted );
            }
        
        if ( sessionId != KMceNotAssigned )
            {
            MCEMM_DEBUG_DVALUE("CMceMediaManager::CleanupSessionL(): \
closing session. Id", sessionId );
            iMccInterface->CloseSession( sessionId );
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::CleanupSessionL(), Exit ");
    }
    

// ---------------------------------------------------------
// CMceMediaManager::RemoveSession
// ---------------------------------------------------------    
//
void CMceMediaManager::RemoveSession( CMceSdpSession* aSdpSession )
  
    {
    MCEMM_DEBUG("CMceMediaManager::RemoveSession(), Entry ");
    
    TUint32 sessionId = aSdpSession->MediaSession()->iMccID;
    TBool isMaster = aSdpSession->IsMaster();
    CMceSdpSession* sdpSession = NULL;
    CMceSdpSession::TIterator sdpSessions( iMceSdpSessions );
    TBool loop = ETrue;
    
    // remove all sessions, which does not have media session
    while( loop && sdpSessions.Next( sdpSession ) )
        {
        if ( sdpSession == aSdpSession ||
             !sdpSession->MediaSession() ||
             sdpSession->Consumes( sessionId ) )
              
            {
            if ( isMaster || sdpSession == aSdpSession )
                {
                sdpSessions.Remove();
                delete sdpSession;
                MCEMM_DEBUG("SDP session removed");
                }
            loop = isMaster;
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::RemoveSession(), Exit ");
    }



// ---------------------------------------------------------
// CMceMediaManager::UseRtcp
// ---------------------------------------------------------    
//
TBool CMceMediaManager::UseRtcp( CMceSrvStream& aStream )
    {
    MCEMM_DEBUG("CMceMediaManager::UseRtcp(), Entry ")
    
    TBool useRtcp( ETrue );
    TMceSrvStreamIterator streams( aStream.Data().Session()->MccStreams() );
    CMceSrvStream* stream = NULL;
    while( streams.Next( stream ) && useRtcp )
        {
        if ( aStream.LinkId() == stream->LinkId() )
            {
            useRtcp = useRtcp && aStream.Data().Session()->UseRtcp();
            }
        }

    MCEMM_DEBUG_DVALUE( "CMceMediaManager::UseRtcp() stream id:", aStream.Id() )
    MCEMM_DEBUG_DVALUE( "CMceMediaManager::UseRtcp() use rtcp:", useRtcp )
                    
    return useRtcp;
    }

// ---------------------------------------------------------
// CMceMediaManager::FillMceFileInfoL
// ---------------------------------------------------------    
//    
void CMceMediaManager::FillMceFileInfoL( 
    TMccFileSourceSetting& aMccFileInfo,
    TMceFileInfo& aMceFileInfo ) const
    {
    CMccCodecInformationFactory* codecFactory = 
            CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( codecFactory );
    
    aMceFileInfo.iAudioCodec = KNullDesC8;   
    aMceFileInfo.iAudioElementCount = 0;
     
    if ( aMccFileInfo.iAudioFourCC != KMMFFourCCCodeNULL )
        {
        // File might contain unsupported audio, ignore leave
        CMccCodecInformation* codec = NULL;
        TRAP_IGNORE( 
            (codec = codecFactory->CreateCodecInformationL( aMccFileInfo.iAudioFourCC ) ))
        if ( codec )
            {
            aMceFileInfo.iAudioCodec = codec->SdpName();
            aMceFileInfo.iAudioBitRate = aMccFileInfo.iAudioBitRate;  
            }
        
        delete codec;
        aMceFileInfo.iAudioElementCount = 1;
        }
     
    aMceFileInfo.iVideoCodec = KNullDesC8;
    aMceFileInfo.iVideoElementCount = 0;
           
    if ( aMccFileInfo.iVideoFourCC != KMMFFourCCCodeNULL )
        {
        // File might contain unsupported video, ignore leave
        CMccCodecInformation* codec = NULL;
        TRAP_IGNORE( 
            (codec = codecFactory->CreateCodecInformationL( aMccFileInfo.iVideoFourCC ) ))
        if ( codec )
            {
            aMceFileInfo.iVideoCodec = codec->SdpName();
            aMceFileInfo.iVideoFrameRate = aMccFileInfo.iVideoFrameRate;
            aMceFileInfo.iVideoBitRate = aMccFileInfo.iVideoBitRate;
            aMceFileInfo.iVideoFrameSize = aMccFileInfo.iVideoFrameSize;
            }
        
        delete codec;
        aMceFileInfo.iVideoElementCount = 1;
        }
    
    aMceFileInfo.iDuration = aMccFileInfo.iDuration;
    
    CleanupStack::PopAndDestroy( codecFactory );
    }

// ---------------------------------------------------------
// CMceMediaManager::DoStartEndpointsL
// ---------------------------------------------------------    
//
TBool CMceMediaManager::DoStartEndpointsL( 
    CMceSrvStream& aStream,
    TUint32 aForceEnableEndpoint,
    TUint32 aForceDisableEndpoint )
    {
    MCEMM_DEBUG("CMceMediaManager::DoStartEndpointsL(), Entry ");
    
    if ( aForceDisableEndpoint )
        {
        // No need to start if explicitly disabling
        MCEMM_DEBUG("CMceMediaManager::DoStartEndpointsL(), Exit ");
        return EFalse;
        }
    
    TBool sinkForced( aStream.Sink().Id() == aForceEnableEndpoint );
    TBool sourceForced( aStream.Source().Id() == aForceEnableEndpoint );
    
    TBool sinkNeedsStart( 
          aStream.Codec().iIsEnabled &&
        ( aStream.Sink().State() < CMceSrvStream::EStarted && 
          aStream.Sink().State() != CMceSrvStream::EStarting ) &&
        ( aStream.Sink().Data().IsEnabled( KMceEnabledE2e ) || 
          sinkForced ) &&
        ( aStream.Sink().State() == CMceSrvStream::EPrepared ||
          aStream.Sink().State() == CMceSrvStream::EPending ) );
                          
    TBool sourceNeedsStart( 
          aStream.Codec().iIsEnabled &&
        ( aStream.Source().State() < CMceSrvStream::EStarted && 
          aStream.Source().State() != CMceSrvStream::EStarting ) &&
        ( aStream.Source().Data().IsEnabled( KMceEnabledE2e ) || 
          sourceForced ) &&
        ( aStream.Source().State() == CMceSrvStream::EPrepared ||
          aStream.Source().State() == CMceSrvStream::EPending ) );
    
    MCEMM_DEBUG_DVALUE(" forced (0 means no) endpoint ", aForceEnableEndpoint );
    MCEMM_DEBUG_DVALUE(" start sink", sinkNeedsStart );
    MCEMM_DEBUG_DVALUE(" start source", sourceNeedsStart );
    
    TBool startNeeded( sinkNeedsStart || sourceNeedsStart );
    TUint32 startEndpointId = 0;

    // Resolve resumed endpoints
    //
    if ( sinkNeedsStart ^ sourceNeedsStart )
        {
        startEndpointId = sinkNeedsStart ? aStream.Sink().Id() : 
                                           aStream.Source().Id();
        }
        
    if ( startNeeded )
        {
        MCEMM_DEBUG_DVALUE("starting (0 means all) endpoint(s) ", startEndpointId );
        User::LeaveIfError( 
                iMccInterface->StartStream( 
                            aStream.SessionId(),
                            aStream.LinkId(),
                            aStream.Id(),
                            startEndpointId,
                            EFalse,
                            UseRtcp( aStream ) ) );
        
        SetPendingState( aStream, startEndpointId, CMceSrvStream::EStarting );
        }
    else 
        {
        MCEMM_DEBUG("no need to start");
        }
        
    aStream.UpdateEndpointStates( sinkForced, sourceForced, ETrue );
       
    MCEMM_DEBUG("CMceMediaManager::DoStartEndpointsL(), Exit ");
    
    return startNeeded;
    }
    
// ---------------------------------------------------------
// CMceMediaManager::DoResumeEndpointsL
// ---------------------------------------------------------    
//
TBool CMceMediaManager::DoResumeEndpointsL( 
    CMceSrvStream& aStream,
    TUint32 aForceEnableEndpoint )
    {
    MCEMM_DEBUG("CMceMediaManager::DoResumeEndpointsL(), Entry ");
    
    TBool sinkForced( aStream.Sink().Id() == aForceEnableEndpoint );
    TBool sourceForced( aStream.Source().Id() == aForceEnableEndpoint );
    
    // Do not allow resuming streams which have disabled codec
    //
    TBool sinkNeedsResume( 
          aStream.Codec().iIsEnabled &&
        ( aStream.Sink().Data().IsEnabled( KMceEnabledE2e ) || sinkForced ) &&
        ( aStream.Sink().State() == CMceSrvStream::EPaused ||
          aStream.Sink().State() == CMceSrvStream::EPausing ) );
                          
    TBool sourceNeedsResume( 
          aStream.Codec().iIsEnabled &&
        ( aStream.Source().Data().IsEnabled( KMceEnabledE2e ) || sourceForced ) &&
        ( aStream.Source().State() == CMceSrvStream::EPaused ||
          aStream.Source().State() == CMceSrvStream::EPausing ) );
    
    MCEMM_DEBUG_DVALUE(" forced (0 means no) endpoint ", aForceEnableEndpoint );
    MCEMM_DEBUG_DVALUE(" resume sink", sinkNeedsResume );
    MCEMM_DEBUG_DVALUE(" resume source", sourceNeedsResume );
    
    TBool resumeNeeded( sinkNeedsResume || sourceNeedsResume );
    TUint32 resumeEndpointId = 0;

    // Resolve resumed endpoints
    //
    if ( sinkNeedsResume ^ sourceNeedsResume )
        {
        resumeEndpointId = sinkNeedsResume ? aStream.Sink().Id() : 
                                             aStream.Source().Id();
        }
        
    if ( resumeNeeded )
        {
        MCEMM_DEBUG_DVALUE("resuming (0 means all) endpoint(s) ", resumeEndpointId );
        User::LeaveIfError( 
            iMccInterface->ResumeStream( 
                        aStream.SessionId(),
                        aStream.LinkId(),
                        aStream.Id(),
                        resumeEndpointId,
                        UseRtcp( aStream ) ) );
            
        SetPendingState( aStream, resumeEndpointId, CMceSrvStream::EStarting );
        }
    else 
        {
        MCEMM_DEBUG("no need to resume");
        }
        
    aStream.UpdateEndpointStates( sinkForced, sourceForced, ETrue );
        
    MCEMM_DEBUG("CMceMediaManager::DoResumeEndpointsL(), Exit ");
    
    return resumeNeeded;
    }
    
// ---------------------------------------------------------
// CMceMediaManager::DoPauseEndpointsL
// ---------------------------------------------------------    
//
TBool CMceMediaManager::DoPauseEndpointsL( 
    CMceSrvStream& aStream, 
    TUint32 aForceDisableEndpoint )
    {
    MCEMM_DEBUG("CMceMediaManager::DoPauseEndpointsL(), Entry ");
    
    TBool sinkForced( aStream.Sink().Id() == aForceDisableEndpoint );
    TBool sourceForced( aStream.Source().Id() == aForceDisableEndpoint );
    
    // Do not allow streams with disabled codec to run
    //
    TBool sinkNeedsPause( 
        ( !aStream.Codec().iIsEnabled || 
          !aStream.Sink().Data().IsEnabled( KMceEnabledE2e ) || 
          sinkForced ) &&
        ( aStream.Sink().State() == CMceSrvStream::EStarted ||
          aStream.Sink().State() == CMceSrvStream::EStarting ) );
                          
    TBool sourceNeedsPause( 
        ( !aStream.Codec().iIsEnabled ||
          !aStream.Source().Data().IsEnabled( KMceEnabledE2e ) ||
          sourceForced ) &&
        ( aStream.Source().State() == CMceSrvStream::EStarted ||
          aStream.Source().State() == CMceSrvStream::EStarting ) );
    
    MCEMM_DEBUG_DVALUE(" forced (0 means no) endpoint ", aForceDisableEndpoint );
    MCEMM_DEBUG_DVALUE(" pause sink", sinkNeedsPause );
    MCEMM_DEBUG_DVALUE(" pasue source", sourceNeedsPause );
    
    TBool pauseNeeded( sinkNeedsPause || sourceNeedsPause );
    TUint32 pauseEndpointId = 0;

    // Resolve paused endpoints
    //
    if ( sinkNeedsPause ^ sourceNeedsPause )
        {
        pauseEndpointId = sinkNeedsPause ? aStream.Sink().Id() : 
                                           aStream.Source().Id();
        }
        
    if ( pauseNeeded )
        {
        MCEMM_DEBUG_DVALUE("pausing (0 means all) endpoint(s) ", pauseEndpointId );
        User::LeaveIfError( 
            iMccInterface->PauseStream( 
                        aStream.SessionId(),
                        aStream.LinkId(),
                        aStream.Id(),
                        pauseEndpointId,
                        UseRtcp( aStream ) ) );
                            
        SetPendingState( aStream, pauseEndpointId, CMceSrvStream::EPausing );
        }
    else 
        {
        MCEMM_DEBUG("no need to pause");
        }
        
    aStream.UpdateEndpointStates( sinkForced, sourceForced, EFalse );
        
    MCEMM_DEBUG("CMceMediaManager::DoPauseEndpointsL(), Exit ");
    
    return pauseNeeded;
    }

// ---------------------------------------------------------
// CMceMediaManager::SetPendingState
// ---------------------------------------------------------    
//
void CMceMediaManager::SetPendingState( 
    CMceSrvStream& aStream, 
    TUint32 aEndpointId, 
    CMceSrvStream::TState aState )
    {
    if ( aEndpointId == 0 || aStream.Source().Id() == aEndpointId )
        {
        aStream.Source().SetState( aState );
        }
        
    if ( aEndpointId == 0 || aStream.Sink().Id() == aEndpointId )
        {
        aStream.Sink().SetState( aState );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::DoDtmfL
// ---------------------------------------------------------    
//
TBool CMceMediaManager::DoDtmfL(
    CMceComMediaSource& aSource, 
    TMceComEvent& aEvent,
    const TDesC8& aSequence,
    TBool aUseBoundStream )
    {
    CMceComSession* session = aSource.MediaStream()->Session();
    __ASSERT_ALWAYS( session, User::Leave( KErrNotReady ) );
    
    CMceComMediaStream* comStream = aSource.MediaStream();
    if ( aUseBoundStream )
        {
        comStream = &( comStream->BoundStreamL() );
        __ASSERT_ALWAYS( comStream, User::Leave( KErrNotSupported ) );
        }
    // Sending dtmf signals is only possible through dtmf uplink
    TBool handled( EFalse );    
    if ( IS_RECEIVESTREAM( comStream ) )
        {
        return handled;
        }

    TMceSrvStreamIterator streams( session->MccStreams(), *comStream );
    CMceSrvStream* stream = NULL;
    
    
    while( streams.Next( stream ) && !handled )
        {
        if ( stream->MccStreamType() == KMccDtmfStream )
            {
            stream->DtmfHandlerL( *iMccInterface ).DtmfL( aSource,
                                                          aEvent,
                                                          aSequence );
            handled = ETrue;
            }
        }
        
    return handled;
    }

// ---------------------------------------------------------
// CMceMediaManager::DoAdjustMccStreamL
// ---------------------------------------------------------    
//
CMceComCodec* CMceMediaManager::DoAdjustMccStreamL( CMceSrvStream& aStream )
    {  
    MCEMM_DEBUG("CMceMediaManager::DoAdjustMccStreamL(), Entry ")
    
    __ASSERT_ALWAYS( aStream.State() != CMceSrvStream::EAdopted,
                     User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aStream.State() >= CMceSrvStream::EPrepared,
                     User::Leave( KErrArgument ) );

    //get codec
    CMccCodecInformation* codec = 
        iMccInterface->CodecL(         
                aStream.SessionId(),
                aStream.LinkId(),
                aStream.Id() );
                
    CleanupStack::PushL( codec );
    
    //adjust codec
    CMceComCodec* bidirectional = 
        aStream.Codec().MccValidateL( *codec, aStream, EMceRoleOfferer );
    aStream.Codec().MccAdjustL( *codec, aStream );

    User::LeaveIfError( 
        iMccInterface->SetCodec( 
                aStream.SessionId(),
                aStream.LinkId(),
                aStream.Id(),
                *codec ) );
                
    CleanupStack::PopAndDestroy( codec );
    
    MCEMM_DEBUG("CMceMediaManager::DoAdjustMccStreamL(), Exit ")
    
    return bidirectional;
    }

// ---------------------------------------------------------
// CMceMediaManager::DoConfigKeyUpdateL
// ---------------------------------------------------------    
//
void CMceMediaManager::DoConfigKeyUpdateL( 
    CMceComMediaStream& aStream,
    HBufC8* aConfigKey )
    {
    MCEMM_DEBUG("CMceMediaManager::DoConfigKeyUpdateL(), Entry ");
    
    __ASSERT_ALWAYS( aStream.iType == KMceVideo, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aConfigKey, User::Leave( KErrArgument ) );
    
    RPointerArray<CMceComMediaSink>& sinks = aStream.Sinks();
    for ( TInt i = 0; i < sinks.Count(); i++ )
        {
        TMceSrvStreamIterator streams( 
            aStream.Session()->MccStreams(), aStream );
        CMceSrvStream* stream = NULL;
        while( streams.Next( stream ) )
            {
            CMceComVideoCodec& videoCodec = 
                reinterpret_cast<CMceComVideoCodec&>( stream->Codec() );
            
            MCEMM_DEBUG_STREAM( "updating config key", *stream );

             if ( videoCodec.SetConfigKeyL( aConfigKey ) )
                {
                // Synchronize codec only
                SynchronizeMccStreamL( *stream, 
                                       aStream.Session()->NegotiationState().Role(),
                                       ETrue );
                }
            }
        }
        
    MCEMM_DEBUG("CMceMediaManager::DoConfigKeyUpdateL(), Exit ");
    }
// ---------------------------------------------------------
// CMceMediaManager::UpDateStreamStateL
// ---------------------------------------------------------    
//
void CMceMediaManager::UpDateStreamStateL(CMceComSession& currentSession,
		                    CMceComSession& forkSession )
	{
	MCEMM_DEBUG("CMceMediaManager::UpDateStreamState, Entry ");
	CMceSrvStream* stream = NULL;
	RPointerArray<CMceSrvStream> currentStreams;
	TMceSrvStreamIterator sendStreams( currentSession.MccStreams(), 
			                            TMceSrvStreamIterator::ESend );
	while( sendStreams.Next( stream, CMceSrvStream::EStopped, 
		                           TMceSrvStreamIterator::ExactReverseMatch ) )	                                     
		{
		if ( stream->State() != CMceSrvStream::EInactive &&
				 stream->Codec().iIsNegotiated )
			{ 
			currentStreams.Append( stream );
			}
		}
	TMceSrvStreamIterator forkSendStreams( forkSession.MccStreams(), 
			                                 TMceSrvStreamIterator::ESend );
	while( forkSendStreams.Next( stream, CMceSrvStream::EStopped, 
	                             TMceSrvStreamIterator::ExactReverseMatch ) )                                     
		{
		if ( stream->State() != CMceSrvStream::EInactive &&
				stream->Codec().iIsNegotiated )
			{ 
			CMccCodecInformation* codec = 
			                      iMccInterface->CodecL( stream->SessionId(),
	                    		                         stream->LinkId(),
	                    		                         stream->Id() );
			CleanupStack::PushL( codec );
			stream->Codec().MccPopulateL( *codec, *stream, EMceRoleOfferer );
			User::LeaveIfError( iMccInterface->SetCodec( stream->SessionId(),
	                    		  stream->LinkId(), stream->Id(), *codec ) );
			CleanupStack::PopAndDestroy( codec );
			TBool IsMatch = EFalse;
			for( TInt count = 0; count < currentStreams.Count() && !IsMatch; count++ )
				{
				if( currentStreams[count]->Codec().iSdpName ==  
				                           stream->Codec().iSdpName )
					{
					IsMatch = ETrue;
					}
				}
			if( !IsMatch )
				{
				SetPendingState( *stream, 0, CMceSrvStream::EPaused );
				} 	
			}
		}
	MCEMM_DEBUG("CMceMediaManager::UpDateStreamState, Exit ");
	}
// ---------------------------------------------------------
// CMceMediaManager::ForceEnableSinkState
// ---------------------------------------------------------    
//
void CMceMediaManager::ForceEnableSinkState(CMceComSession& aSession )
	{
	CMceSrvStream* stream = NULL;
	TBool IsSet = EFalse;
	TMceSrvStreamIterator sendStreams( aSession.MccStreams(), TMceSrvStreamIterator::ESend );
	while( !IsSet && sendStreams.Next( stream, CMceSrvStream::EStopped, 
			                             TMceSrvStreamIterator::ExactReverseMatch ) )	                                     
		{
		if ( stream->State() != CMceSrvStream::EInactive &&
			             stream->Codec().iIsNegotiated )
			{ 
			SetPendingState( *stream, stream->Sink().Id(), CMceSrvStream::EPaused );
			IsSet = ETrue;
			}
		}
	}
        
// End of File
