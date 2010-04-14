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




#include "mcesrvstream.h"
#include "mcesrvsource.h"
#include "mcesrvsink.h"
#include "mceevents.h"

#include <sdpcodecstringconstants.h>

#include "mcecomsession.h"
#include "mcecomaudiostream.h"
#include "mcecomaudiocodec.h"
#include "mcecommediasource.h"
#include "mcecommediasink.h"
#include "mcecomvideostream.h"
#include "mcecomvideocodec.h"
#include "mcecomdisplaysink.h"
#include "mcecomcamerasource.h"
#include "mcedisplaysink.h"
#include "mcecamerasource.h"

#include "mceaudiostream.h"
#include "mcevideostream.h"
#include "mcesrvstreamiterator.h"
#include "mcemediamanager.h"
#include "mcedtmfcodec.h"
#include "mcemmlogs.h"
#include "mcedtmfhandler.h"

#define MCE_MCC_STREAM_STATE_CHANGE_EVENT( event )\
( aEvent.iEvent == KMccStreamPrepared || \
  aEvent.iEvent == KMccStreamStarted || \
  aEvent.iEvent == KMccStreamPaused || \
  aEvent.iEvent == KMccStreamResumed || \
  aEvent.iEvent == KMccStreamStopped )



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceSrvStream::DecodeL
// -----------------------------------------------------------------------------
//
void CMceSrvStream::DecodeL( RPointerArray<CMceSrvStream>& aStreams,
                             CMceComMediaStream& aStream,
                             CMceMediaManager& aManager )
    {
    MCEMM_DEBUG("CMceSrvStream::DecodeL(), Entry ");
        
    switch( aStream.iType )
        {
        case KMceAudio:
            {
            CMceComAudioStream& audio = 
                static_cast<CMceComAudioStream&>( aStream );
            DecodeAudioL( aStreams, audio, aManager );
            break;
            }
        case KMceVideo:
            {
            CMceComVideoStream& video = 
                static_cast<CMceComVideoStream&>( aStream );
            DecodeVideoL( aStreams, video, aManager );
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
    MCEMM_DEBUG("CMceSrvStream::DecodeL(), Exit ");
    }



// -----------------------------------------------------------------------------
// CMceSrvStream::DecodeAudioL
// -----------------------------------------------------------------------------
//
void CMceSrvStream::DecodeAudioL( RPointerArray<CMceSrvStream>& aStreams,
                                  CMceComAudioStream& aAudio,
                                  CMceMediaManager& aManager )
    {
    MCEMM_DEBUG("CMceSrvStream::DecodeAudioL(), Entry ");
    
    // First clear all enabled states, those are affected inside codec
    // specific decoding
    for(  TInt i = 0; i < aAudio.CodecCount(); ++i )
        {
        aAudio.CodecL( i )->SetEnabled( EFalse );
        }
    
    for ( TInt j = 0; j < aAudio.CodecCount(); ++j )
        {
        aAudio.CodecL( j )->DoDecodeAudioL( j, 
                                              aStreams, 
                                              aAudio, 
                                              aManager );
        }
        
    if ( aAudio.BoundStream() && aAudio.Binder() )
        {
        DecodeL( aStreams, aAudio.BoundStreamL(), aManager );
        }
    MCEMM_DEBUG("CMceSrvStream::DecodeAudioL(), Exit ");
    }



// -----------------------------------------------------------------------------
// CMceSrvStream::DecodeVideoL
// -----------------------------------------------------------------------------
//

void CMceSrvStream::DecodeVideoL( RPointerArray<CMceSrvStream>& aStreams,
                                  CMceComVideoStream& aVideo,
                                  CMceMediaManager& aManager )
    {
    MCEMM_DEBUG("CMceSrvStream::DecodeVideoL(), Entry ");
    
    for( TInt codecNdx = 0;codecNdx < aVideo.CodecCount();codecNdx++ )
        {
        aVideo.CodecL( codecNdx )->SetEnabled( 
            codecNdx == 0 || IS_RECEIVESTREAM( &aVideo ) );
            
        for( TInt sinkNdx = 0 ; sinkNdx < aVideo.Sinks().Count() ; sinkNdx++ )
            {
            CMceSrvStream* srvStream = NewL( aManager, aVideo, 
                                             *aVideo.Source(),
                                             *aVideo.Sinks()[ sinkNdx ],
                                             *aVideo.CodecL( codecNdx ) );
                CleanupStack::PushL( srvStream );
                MCEMM_DEBUG_STREAM( "CMceSrvStream::DecodeVideoL(): decoded video", *srvStream );
                aStreams.AppendL( srvStream );
                CleanupStack::Pop( srvStream );
            }
        }
    if ( aVideo.BoundStream() && aVideo.Binder() )
        {
        DecodeL( aStreams, aVideo.BoundStreamL(), aManager );
        }
    MCEMM_DEBUG("CMceSrvStream::DecodeVideoL(), Exit ");
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::NewL
// -----------------------------------------------------------------------------
//
CMceSrvStream* CMceSrvStream::NewL( CMceMediaManager& aManager,
                          CMceComMediaStream& aData,
                          CMceComMediaSource& aSource,
                          CMceComMediaSink& aSink,
                          CMceComCodec& aCodec )
    {
    CMceSrvStream* self = new (ELeave) CMceSrvStream( &aManager, &aData, &aCodec );
    CleanupStack::PushL( self );
    self->ConstructL( aSource, aSink );
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::CMceSrvStream
// -----------------------------------------------------------------------------
//
CMceSrvStream::CMceSrvStream()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceSrvStream::CMceSrvStream
// -----------------------------------------------------------------------------
//
CMceSrvStream::CMceSrvStream( CMceMediaManager* aManager,
                              CMceComMediaStream* aData,
                              CMceComCodec* aCodec )
    : iManager( aManager ),
      iData( aData ),
      iCodec( aCodec ),
      iMerged( EFalse ),
      iID( KMceNotAssigned )
    {
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::CreateLinkL
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::CreateLinkL()
    {
    __ASSERT_ALWAYS( State() == ECreated, User::Leave( KErrArgument ) );
    TBool sequence = KMceSrvStreamDefaultSequence;
    
    if ( !UseDefaultStartupSequence() )
        {
        sequence = KMceSrvStreamAlternativeSequence;
        }
        
    return sequence;
    
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::PrepareL
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::PrepareL()
    {
    __ASSERT_ALWAYS( State() == ELinkCreated, User::Leave( KErrArgument ) );
    TBool sequence = KMceSrvStreamDefaultSequence;
    
    if ( !UseDefaultStartupSequence() )
        {
        MCEMM_DEBUG("CMceSrvStream::PrepareL(): alternative prepare");
        
        sequence = KMceSrvStreamAlternativeSequence;
        TMceMccComEvent prepare( KMccStreamPrepared );
                
        iSource->EventReceived( prepare );
        iSink->EventReceived( prepare );
        
        MCEMM_DEBUG_STREAM( "CMceSrvStream::PrepareL(): after prepare", *this );
        }
    else if ( IsMccPrepared() )
        {
        iSource->Data().PrepareL();
        iSink->Data().PrepareL();
        }        
    else
        {
        // NOP
        }

    return sequence;
    
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::StartL
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::StartL()
    {
        
    if ( IsMccPrepared() )
        {
        return KMceSrvStreamDefaultSequence;
        }

    MCEMM_DEBUG("CMceSrvStream::StartL(): alternative start");
        
    TMceMccComEvent started( KMccStreamStarted );
    TMceMccComEvent paused( KMccStreamPaused );
    
    if ( iSource->Data().IsEnabled() )
        {
        iSource->EventReceived( started );
        iSource->EnableL( *this, KMceSrvStreamSync );
        }
    else
        {
        iSource->EventReceived( paused );
        }
        
    if ( iSink->Data().IsEnabled() )
        {
        iSink->EventReceived( started );
        iSink->EnableL( *this, KMceSrvStreamSync );
        }
    else
        {
        iSink->EventReceived( paused );
        }
        
    MCEMM_DEBUG_STREAM( "CMceSrvStream::StartL(): after start", *this );
    
    return KMceSrvStreamAlternativeSequence;
    
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::SynchronizeL
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::SynchronizeL()
    {    
        
    if ( IsMccPrepared() )
        {
        return KMceSrvStreamDefaultSequence;
        }

    MCEMM_DEBUG("CMceSrvStream::SynchronizeL(): alternative synchronize");
    
    if ( iSource->Data().IsEnabled() )
        {
        iSource->EnableL( *this, KMceSrvStreamSync );
        }
    else
        {
        iSource->DisableL( *this, KMceSrvStreamSync );
        }
        
    if ( iSink->Data().IsEnabled() )
        {
        iSink->EnableL( *this, KMceSrvStreamSync );
        }
    else
        {
        iSink->DisableL( *this, KMceSrvStreamSync );
        }

    MCEMM_DEBUG_STREAM( "CMceSrvStream::SynchronizeL(): after synchronize", *this );
        
    return KMceSrvStreamAlternativeSequence;
    
    }

    
// -----------------------------------------------------------------------------
// CMceSrvStream::Stop
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::Stop()
    {
        
    if ( IsMccPrepared() )
        {
        return KMceSrvStreamDefaultSequence;
        }
        
    MCEMM_DEBUG("CMceSrvStream::Stop(): alternative stop, stopping source");

    DoEndpointDisable( *iSource );
  
    MCEMM_DEBUG("CMceSrvStream::Stop(): alternative stop, stopping sink");
    
    DoEndpointDisable( *iSink );
    
    // Generating stopped event must be done after disabling
    TMceMccComEvent stopped( KMccStreamStopped );
    iSink->EventReceived( stopped );
    iSource->EventReceived( stopped );
        
    MCEMM_DEBUG_STREAM( "CMceSrvStream::Stop(): after stop", *this );
    return KMceSrvStreamAlternativeSequence;
    
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::Validate
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::Validate()
    {
    return StreamType() == CMceComMediaStream::ELocalStream;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::UseDefaultStartupSequence
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::UseDefaultStartupSequence()
    {
    TBool defaultSequence = ETrue;
    
    //special case: local stream, which has camera source and
    //              diaplay sink
    if ( StreamType() == CMceComMediaStream::ELocalStream &&
         iSource->Data().iType == KMceCameraSource &&
         iSink->Data().iType == KMceDisplaySink )
        {
        //try to find stream, which is send stream and it has the
        //same camera source
        TMceSrvStreamIterator streams( 
                Data().Session()->MccStreams(), iSource->Data() );
                
        CMceSrvStream* pairedStream = NULL;
        CMceSrvStream* stream = NULL;
        while( !pairedStream && streams.Next( stream ) )
            {
            pairedStream = 
                ( stream->StreamType() == CMceComMediaStream::ESendStream ||
                  stream->StreamType() == CMceComMediaStream::ESendOnlyStream ) ?
                  stream : NULL;
            }
        defaultSequence = MCE_IS_NULL_PTR( pairedStream );
        if ( !defaultSequence )
            {
            static_cast<CMceComDisplaySink&>( iSink->Data() ).SetViewFinder( ETrue );
            }
        
        }
    
    return defaultSequence;
    
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSrvStream::ConstructL( CMceComMediaSource& aSource,
                               CMceComMediaSink& aSink )
    {
    iSource = new (ELeave) CMceSrvSource( *iManager, aSource );
    iSink = new (ELeave) CMceSrvSink( *iManager, aSink );
    }
                   

// -----------------------------------------------------------------------------
// CMceSrvStream::IsMerged
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::IsMerged() const
    {
    return iMerged;
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::IsMccPrepared
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::IsMccPrepared() const
    {
    return iID != KMceNotAssigned;
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::Merge
// -----------------------------------------------------------------------------
//
void CMceSrvStream::Merge( CMceSrvStream& aMergeWith, TBool aDeepMerge )
    {
    aMergeWith.iMerged = ETrue;
    
    iMerged = aMergeWith.iMerged;
    iID = aMergeWith.iID;
    iSource->Merge( aMergeWith.Source() );
    iSink->Merge( aMergeWith.Sink() );
    Data().SetLinkId( aMergeWith.LinkId() );
    
    if ( aDeepMerge )
        {
        Data().Merge( aMergeWith.Data() );
        iCodec->Merge( aMergeWith.Codec() );
        iSource->Data().Merge( aMergeWith.Source().Data() );
        iSink->Data().Merge( aMergeWith.Sink().Data() );
        }
    
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::IsAdopted
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::IsAdopted() const
    {
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::UnMerge
// -----------------------------------------------------------------------------
//
void CMceSrvStream::UnMerge()
    {
    iMerged = EFalse;
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::UnMergeL
// -----------------------------------------------------------------------------
//
void CMceSrvStream::UnMergeL( CMceSrvStream& aUnMergeFrom )
    {
    UnMerge();
    
    if ( !IsAdopted() && !aUnMergeFrom.IsAdopted() )
        {
        iSource->Merge( aUnMergeFrom.Source() );
        iSink->Merge( aUnMergeFrom.Sink() );
        iManager->SynchronizeMccStreamL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::Cleanup
// -----------------------------------------------------------------------------
//
void CMceSrvStream::Cleanup()
    {
    MCEMM_DEBUG("CMceSrvStream::Cleanup(), Entry ");

    if ( iManager->CanReleaseEndpoint( *this, Source().Id() ) )
        {
        iManager->ReleaseSource( *this, Source().Id() );
        }
        
    if ( iManager->CanReleaseEndpoint( *this, Sink().Id() ) )
        {
        iManager->ReleaseSink( *this, Sink().Id() );
        }

    MCEMM_DEBUG("CMceSrvStream::Cleanup(), Exit ");        
    }



    
// -----------------------------------------------------------------------------
// CMceSrvStream::IsEqual
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::IsEqual( const CMceSrvStream& aStream ) const
    {
    TBool equal = EFalse;
    if ( &aStream )
        {
        equal = Codec().Id() == aStream.Codec().Id() &&
               Source().Data().Id() == aStream.Source().Data().Id() &&
               Sink().Data().Id() == aStream.Sink().Data().Id();
        }
    
    return equal;
            
    }
    
// -----------------------------------------------------------------------------
// CMceSrvStream::Consumes
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::Consumes( const TMceMccComEvent& aEvent ) const
    {
    TBool match = EFalse;
    
    if ( State() != EAdopted )
        {
        if ( aEvent.iStreamId )
            {
            match = iID == aEvent.iStreamId;
            }
        else if ( aEvent.iEndpointId )
            {
            match = LinkId() == aEvent.iLinkId &&
                    ( Sink().Id() == aEvent.iEndpointId ||
                      Source().Id() == aEvent.iEndpointId );
            }
        else 
            {
            match = LinkId() == aEvent.iLinkId;
            }
        }
    
    return match;
    }
 
 // -----------------------------------------------------------------------------
// CMceSrvStream::DtmfHandlerL
// -----------------------------------------------------------------------------
//   
CMceDtmfHandler& CMceSrvStream::DtmfHandlerL( CMccInterface& aMccInterface )
    {
    __ASSERT_ALWAYS( MccStreamType() == KMccDtmfStream, 
                     User::Leave( KErrNotFound ) );
    
    if ( !iDtmfHandler )
        {
        iDtmfHandler = CMceDtmfHandler::NewL( *this, aMccInterface );
        }
        
    return *iDtmfHandler;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::UpdateEndpointStates
// -----------------------------------------------------------------------------
//    
void CMceSrvStream::UpdateEndpointStates( 
    TBool aSinkAffected, 
    TBool aSourceAffected, 
    TBool aIsEnabled )
    {
    MCEMM_DEBUG_DVALUES( "CMceSrvStream::UpdateEndpointStates, sink affected:", 
                         aSinkAffected,
                         " source affected:",
                         aSourceAffected )
    MCEMM_DEBUG_DVALUE( "CMceSrvStream::UpdateEndpointStates, enabled:", 
                        aIsEnabled )
    
    if ( aSinkAffected )
        {
        Sink().Data().Enabled( aIsEnabled );
        }
    if ( aSourceAffected )
        {
        Source().Data().Enabled( aIsEnabled );
        }
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::EndpointMatch
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::EndpointMatch( 
    const CMceComEndpoint& aEndpoint, 
    TBool aUseProxyMatch )
    {
    CMceSrvEndpoint* srvEndpoint = NULL;
    if ( aEndpoint.Category() == KMceCategorySource )
        {
        srvEndpoint = &Source();
        }
    else
        {
        srvEndpoint = &Sink();
        }
    return srvEndpoint->EndpointMatch( aEndpoint, aUseProxyMatch );
    }
       
// -----------------------------------------------------------------------------
// CMceSrvStream::~CMceSrvStream
// -----------------------------------------------------------------------------
//
CMceSrvStream::~CMceSrvStream()
    {
    delete iDtmfHandler;
    delete iSource;
    delete iSink;
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::SessionId
// -----------------------------------------------------------------------------
//
TUint32 CMceSrvStream::SessionId() const
    {
    return Data().Session()->iMccID;
    }

    
// -----------------------------------------------------------------------------
// CMceSrvStream::LinkId
// -----------------------------------------------------------------------------
//
TUint32 CMceSrvStream::LinkId() const
    {
    TUint32 linkId = Data().iLinkId;
    return linkId == 0 ? KMceNotAssigned : linkId;
    }
    
// -----------------------------------------------------------------------------
// CMceSrvStream::SetLinkId
// -----------------------------------------------------------------------------
//
void CMceSrvStream::SetLinkId( TUint32 aLinkId )
    {
    Data()().SetLinkId( aLinkId );
    }
    
    
// -----------------------------------------------------------------------------
// CMceSrvStream::IapId
// -----------------------------------------------------------------------------
//
TUint32 CMceSrvStream::IapId() const
    {
    return Data().Session()->iIapId;
    }
    
// -----------------------------------------------------------------------------
// CMceSrvStream::LocalMediaPort
// -----------------------------------------------------------------------------
//
TUint CMceSrvStream::LocalMediaPort() const
    {
    return Data().LocalMediaPort();
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::SetLocalMediaPort
// -----------------------------------------------------------------------------
//
void CMceSrvStream::SetLocalMediaPort( TUint aPort )
    {
    return Data().SetLocalMediaPort( aPort );
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::RemoteIpAddress
// -----------------------------------------------------------------------------
//
TInetAddr CMceSrvStream::RemoteIpAddress() const
    {
    TInetAddr remoteAddress = Data().Session()->iRemoteIpAddress;
    remoteAddress.SetPort( Data().RemoteMediaPort() );
    return remoteAddress;
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::Id
// -----------------------------------------------------------------------------
//
TUint32& CMceSrvStream::Id()
    {
    return iID;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::Data
// -----------------------------------------------------------------------------
//
CMceComMediaStream& CMceSrvStream::Data() const
    {
    return *iData;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::Source
// -----------------------------------------------------------------------------
//
CMceSrvSource& CMceSrvStream::Source() const
    {
    return *iSource;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::Sink
// -----------------------------------------------------------------------------
//
CMceSrvSink& CMceSrvStream::Sink() const
    {
    return *iSink;
    }
    
// -----------------------------------------------------------------------------
// CMceSrvStream::Codec
// -----------------------------------------------------------------------------
//
CMceComCodec& CMceSrvStream::Codec() const
    {
    return *iCodec;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::Invalidate
// -----------------------------------------------------------------------------
//
void CMceSrvStream::Invalidate()
    {
    MCEMM_DEBUG("CMceSrvStream::Invalidate(), Entry ");

    TMceMccComEvent invalidate( KMccStreamStopped, 
                                Data().iLinkId, iID, 0 );
                                
    EventReceived( invalidate, KMceSrvStreamNoCallback );
    MCEMM_DEBUG("CMceSrvStream::Invalidate(), Exit ");
    
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::EventReceived
// -----------------------------------------------------------------------------
//
TInt CMceSrvStream::EventReceived( TMceMccComEvent& aEvent, TBool aCallback )
    {

    MCEMM_DEBUG("CMceSrvStream::EventReceived(), Entry ");
    MCEMM_DEBUG_DVALUE("event", aEvent.iEvent );
    MCEMM_DEBUG_DVALUE("callback", aCallback );
    
    TInt status = KMceEventConsumed;
        
    if ( aEvent.iEvent == KMccLinkCreated )
        {
        if ( State() == ECreatingLink ) //might be inactive
            {
            iSource->EventReceived( aEvent );
            iSink->EventReceived( aEvent );
            TPckgBuf<TMccNetSettings> netSettingsBuf;
            netSettingsBuf.Copy( *aEvent.iItcDataDesC );
            TMccNetSettings netSettings = netSettingsBuf();
            SetLocalMediaPort( netSettings.iLocalAddress.Port() );
            Data().Session()->iServiceType = netSettings.iMediaQosValue;
            TMceSrvStreamIterator streams( Data().Session()->MccStreams(), 
                                           Data() );
            CMceSrvStream* stream = NULL;
            if ( !streams.Next( stream, ECreatingLink ) )
                {
                // If all created, update client side
                Data().EventReceived( aEvent );
                }
            }
        }
    else if ( IsMccPrepared() ||
              aEvent.iLinkId == Data().iLinkId )
        {
        aEvent.iCodec = iCodec;
        aEvent.iSource = aEvent.iEndpointId > 0 && iSource->Id() == aEvent.iEndpointId ? 
                         &iSource->Data() : NULL;
        aEvent.iSink = aEvent.iEndpointId > 0 && iSink->Id() == aEvent.iEndpointId ? 
                       &iSink->Data() : NULL;

        if ( aEvent.iSource )
            {
            iSource->EventReceived( aEvent );
            }
        else if ( aEvent.iSink )
            {
            iSink->EventReceived( aEvent );
            }
        else
            {
            iSource->EventReceived( aEvent );
            iSink->EventReceived( aEvent );
            }
        
        TBool allowEvent( ETrue );
        if ( MCE_MCC_STREAM_STATE_CHANGE_EVENT( aEvent ) )
            {
            allowEvent = StreamStateChangeEventReceived( aEvent );
            }
        
        if ( Data().iState == CMceMediaStream::EDisabled &&
             aEvent.iEvent == KMccStreamError &&
             aEvent.iError == KErrHostUnreach )
            {
            allowEvent = EFalse;
            }
        
        if ( allowEvent )
            {
            Data().SetState( aEvent );
            if ( aCallback )
                {
                Data().EventReceived( aEvent );
                }
            }
        }
    else //if 'local' stream
        {
        iSource->EventReceived( aEvent );
        iSink->EventReceived( aEvent );
        }

    MCEMM_DEBUG_STREAM( "CMceSrvStream::EventReceived(): after received", *this );
    MCEMM_DEBUG("CMceSrvStream::EventReceived(), Exit ");

    return status;
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::RequireSignalling
// -----------------------------------------------------------------------------
//
TInt CMceSrvStream::RequireSignalling( CMceSrvStream& aCurrent,
                                        CMccCodecInformation& aMccCurentCodec, 
                                        CMccCodecInformation& aMccUpdateCodec ) const
    {
    MCEMM_DEBUG("CMceSrvStream::RequireSignalling(), Entry ");
    
    TInt action = KMceNoSignalling;
    
    if ( Codec().MccRequireSignalling( *this, aMccCurentCodec, aMccUpdateCodec ) )
        {
        MCEMM_DEBUG("CMceSrvStream::RequireSignalling(): codec requires signalling");
        action = KMceRequiresSignalling;
        }
    else if ( LocalMediaPort() != aCurrent.LocalMediaPort() )
    //local port has changed => needs new media session & signaling
        {
        MCEMM_DEBUG("CMceSrvStream::RequireSignalling(): local port changed. requires signalling");
        action = KMceRequiresSignalling;
        }
    else if ( Data().RemoteMediaPortChanged( aCurrent.Data().RemoteMediaPort() ) )
    	{
    	MCEMM_DEBUG("CMceSrvStream::RequireSignalling(): Remote port changed. requires signalling");
    	action = KMceRequiresSipSignallingOnly;
    	}
        
    else if ( Data().RemoteRTCPAddressChanged(aCurrent.Data().iRemoteRtcpPort, 
    		 	aCurrent.Data().iRemoteRtcpAddress))
    		  
    	{
    	MCEMM_DEBUG("CMceSrvStream::RequireSignalling(): Remote Rtcp Port changed, requires signalling");
        action = KMceRequiresSipSignallingOnly;
    	}
        
    else if ( Data().iIsEnabled != aCurrent.Data().iIsEnabled )
    //hold or resume => just SIP signaling
        {
        MCEMM_DEBUG("CMceSrvStream::RequireSignalling(): hold/resume detected. requires signalling");
        action = KMceRequiresSipSignallingOnly;
        }
    else
        {
        MCEMM_DEBUG("CMceSrvStream::RequireSignalling(): requires no signalling");
        //NOP
        }
        
    MCEMM_DEBUG("CMceSrvStream::RequireSignalling(), Exit ");
    return action;
    
    }


// -----------------------------------------------------------------------------
// CMceSrvStream::RequireSignalling
// -----------------------------------------------------------------------------
//
TInt CMceSrvStream::RequireSignalling( 
    RPointerArray<CMceSrvStream>& /*aUpdateStreams*/ ) const
    {
    TInt action = KMceRequiresSignalling;
    
    //local stream added or removed => no signalling
    if ( StreamType() == CMceComMediaStream::ELocalStream )
        {
        action = KMceNoSignalling;
        }
    //enabled stream
    else if ( !IsMccPrepared() )
        {
        action = KMceRequiresSipSignallingOnly;
        }
    return action;
    
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::State
// -----------------------------------------------------------------------------
//
CMceSrvStream::TState CMceSrvStream::State() const
    {
    
    CMceSrvStream::TState state = EInactive;
        
    if ( iSource->State() == ECreated &&
         iSink->State() == ECreated &&
         Data().IsEnabled() )
        {
        state = ECreated;
        }
    else if ( iSource->State() == EStopped &&
              iSink->State() == EStopped  )
        {
        state = EStopped;
        }
    else if ( iSource->State() == EPending && 
              iSink->State() == EPending )
        {
        state = EPending;
        }
    else if ( iSource->State() == ECreatingLink || 
              iSink->State() == ECreatingLink )
        {
        state = ECreatingLink;
        }
    else if ( iSource->State() == EPreparing || 
              iSink->State() == EPreparing )
        {
        state = EPreparing;
        }
    else if ( iSource->State() == EStarting || 
              iSink->State() == EStarting )
        {
        state = EStarting;
        }
    else if ( iSource->State() == ELinkCreated ||
              iSink->State() == ELinkCreated  )
        {
        state = ELinkCreated;
        }
    else if ( iSource->State() == EPrepared ||
              iSink->State() == EPrepared  )
        {
        state = EPrepared;
        }
    else if ( iSource->State() >= EStarted ||
              iSink->State() >= EStarted  )
        {
        state = EStarted;
        }
    else
        {
        state = EInactive;
        }
        
    return state;
            
    }




// -----------------------------------------------------------------------------
// CMceSrvStream::MccPrepareCalledL
// -----------------------------------------------------------------------------
//
void CMceSrvStream::MccPrepareCalledL()
    {
    iSource->MccPrepareCalledL( *this );
    iSink->MccPrepareCalledL( *this );
    }



// -----------------------------------------------------------------------------
// CMceSrvStream::MccStreamType
// -----------------------------------------------------------------------------
//
TInt CMceSrvStream::MccStreamType() const
    {
    if ( iCodec->iSdpName.CompareF( KMceSDPNameDtmf ) == 0 )
        {
        return KMccDtmfStream;
        }
    return iData->MccStreamType();
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::StreamType
// -----------------------------------------------------------------------------
//
CMceComMediaStream::TStreamType CMceSrvStream::StreamType() const
    {
    return iData->iStreamType;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::Direction
// -----------------------------------------------------------------------------
//
CMceComMediaStream::TStreamType CMceSrvStream::Direction() const
    {
    CMceComMediaStream::TStreamType type = StreamType();
    TInt direction = iData->Direction();
    if ( direction == SdpCodecStringConstants::EAttributeRecvonly )
        {
        type = type == CMceComMediaStream::EReceiveStream ? 
                       CMceComMediaStream::EReceiveOnlyStream : type;
        }
    else if ( direction == SdpCodecStringConstants::EAttributeSendonly )
        {
        type = type == CMceComMediaStream::ESendStream ? 
                       CMceComMediaStream::ESendOnlyStream : type;
        }
    else
        {
        }
        
    return type;
    }
        

// -----------------------------------------------------------------------------
// CMceSrvStream::LinkType
// -----------------------------------------------------------------------------
//
TInt CMceSrvStream::LinkType() const
    {
    return iData->MccLinkType();
    }



// -----------------------------------------------------------------------------
// CMceSrvStream::EventReceived
// -----------------------------------------------------------------------------
//
CMceSrvStream* CMceSrvStream::EventReceived( RPointerArray<CMceSrvStream>& aStreams,
                                             TMceMccComEvent& aEvent,
                                             TBool aConsumeOnlyOnce )
    {
    TMceSrvStreamIterator iterator( aStreams );
    CMceSrvStream* stream = NULL;
    CMceSrvStream* firstMatch = NULL;
    
    TBool propagate( ETrue );
    while( iterator.Next( stream, aEvent ) && propagate )
        {
        propagate = ( stream->EventReceived( aEvent ) == KMceEventConsumed ) ? 
            ( !aConsumeOnlyOnce ) : ETrue;
        firstMatch = !firstMatch ? stream : firstMatch;
        }
    
    return firstMatch;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::StreamStateChangeEventReceived
// -----------------------------------------------------------------------------
//
TBool CMceSrvStream::StreamStateChangeEventReceived( TMceMccComEvent& aEvent )
    {
    TBool allowEvent( ETrue );

    switch ( aEvent.iEvent )
        {
        case KMccStreamStarted:
        case KMccStreamPaused:
        case KMccStreamResumed:
            {
            // Client side is not affected until all mcc streams are on same
            // level
            if ( aEvent.iSource )
                {
                allowEvent = iSource->StreamStateChangeEventReceived( aEvent );
                }
            else if ( aEvent.iSink )
                {
                allowEvent = iSink->StreamStateChangeEventReceived( aEvent );
                }
            else
                {
                // It might be that only another endpoint can be informed,
                // in that case modify the event
                TBool allowSourceEvent = iSource->StreamStateChangeEventReceived( aEvent );
                aEvent.iSource = allowSourceEvent ? aEvent.iSource : 0;
                TBool allowSinkEvent = iSink->StreamStateChangeEventReceived( aEvent );
                aEvent.iSink = allowSinkEvent ? aEvent.iSink : 0;
                allowEvent = ( allowSourceEvent || allowSinkEvent );
                }
            break;
            }
        case KMccStreamStopped:
            {
            // Special handling for stopped events, all streams associated with
            // the source and sink has to be stopped before state can be updated
            // 
            TMceSrvStreamIterator streams( Data().Session()->MccStreams(), 
                                           iSource->Data() );
            CMceSrvStream* stream = NULL;
            if ( streams.Next( 
                        stream,
                        CMceSrvStream::EStopped, 
                        TMceSrvStreamIterator::ExactReverseMatch ) )
                {
                allowEvent = EFalse;
                }
                
            TMceSrvStreamIterator streams2( Data().Session()->MccStreams(), 
                                            iSink->Data() );
            stream = NULL;
            if ( allowEvent && streams2.Next( 
                        stream,
                        CMceSrvStream::EStopped, 
                        TMceSrvStreamIterator::ExactReverseMatch ) )
                {
                allowEvent = EFalse;
                }
            break;
            }
        default:
            {
            break;
            }
        }
        
    return allowEvent;
    }

// -----------------------------------------------------------------------------
// CMceSrvStream::DoEndpointDisable
// -----------------------------------------------------------------------------
//
void CMceSrvStream::DoEndpointDisable( CMceSrvEndpoint& aEndpoint )
    {
    // Disable only if all corresponding endpoints are stopped 
    //
    TMceSrvStreamIterator streams( Data().Session()->MccStreams(), 
                                   aEndpoint.Data(),
                                   KMceComNoProxyMatch,
                                   TMceSrvStreamIterator::ELocal );
    
    CMceSrvStream* stream = NULL;
    TBool allowDisable( ETrue );
    while ( streams.Next( stream ) && allowDisable )
        {
        if ( stream != this && 
             aEndpoint.Endpoint( *stream ).State() != CMceSrvStream::EStopped )
            {
            MCEMM_DEBUG("CMceSrvStream::DoEndpointDisable(): stopping not yet allowed");
            allowDisable = EFalse;
            }   
        }    
       
    if ( allowDisable )
        {
        TRAP_IGNORE( 
            aEndpoint.DisableL( *this, KMceSrvStreamNoSync, KMceSrvStreamForce ) );
        }
    }
    
// End of file
