/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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




// INCLUDES FILES
#include <w32std.h>
#include <AudioPreference.h> 
#include <mmcccodecinformationfactory.h>
#include <mmcccodecinformation.h>
#include "mccvideosinkimpl.h"
#include "mmcccodech263.h"
#include "mccvideosourcesinklogs.h"
#include "mccinternalevents.h"
#include "CXPSPacketSink.h"
#include "mmcccodecamr.h"
#include "mccvideosinkuser.h"
#include "mccdef.h"
#include "mccinternaldef.h"
#include <videoplayer2.h>

// CONSTANTS


_LIT8( KMccSdpInboundStart,
"v=0\r\n\
o=- 63327476467155000 63327476467155000 IN IP4 0.0.0.0\r\n\
s=-\r\n\
c=IN IP4 0.0.0.0\r\n\
t=0 0\r\n" );

_LIT8( KMccSdpInboundAudio,
"m=audio 25001 RTP/AVP %d\r\n\
a=rtpmap:%d %S/8000\r\n\
a=ptime:%d\r\n\
a=maxptime:%d\r\n\
a=fmtp:%d octet-align=%d; mode-set=%S\r\n" );

_LIT8( KMccSdpInboundVideo,
"m=video 25002 RTP/AVP %d\r\n\
a=rtpmap:%d %S/90000\r\n\
a=fmtp:%d %S\r\n\
a=XpsPyldBufSize:integer;%d\r\n\
a=FrameWidth:integer;%d\r\n\
a=FrameHeight:integer;%d\r\n" );

_LIT8( KMccSdpInboundPreroll, "a=DisableMinPrerollCheck:integer;1\r\n\
a=Preroll:integer;%d\r\n" );

_LIT8( KMccSdpInboundDisableVideoClock, "a=DisableVideoClock:integer;1\r\n" );


_LIT( KMccWindowGroupName,"32MccVideoWindow" );
_LIT( KMccHelixSource,"MccDataSource%d");
// NOTE: filepath separator changed to unix style because of problem at Helix side
_LIT( KMccPacketSource, "file://c:/notinuse.xps?XPSServer=%S" );

// MACROS

#define MCC_BITRATE_IN_MASK( bitratemask, bitrate ) bitrate == ( bitratemask & bitrate )

// ================= MEMBER FUNCTIONS =======================



// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMccVideoSinkImpl::CMccVideoSinkImpl( TUint32 aEndpointId ) :
    iEndpointId( aEndpointId ),
    iVolumeAdaptation( 0 ),
    iPreroll( 0 ), 
    iPrerollSet( EFalse )
    {
    	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::CMccVideoSinkImpl" ) 
    	
    iSettings.iVideoFrameRate = KMccDefaultVideoFrameRate;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::ConstructL( const TMccVideoSinkSetting& aSettings )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::ConstructL" ) 
    
    iSettings = aSettings;
    
    SetVideoFrameRate( aSettings.iVideoFrameRate );

    const TInt KMccVideoSinkMaxSdpLen = 800;
    iSdp = HBufC8::NewL( KMccVideoSinkMaxSdpLen );

    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::ConstructL, init Helix" ) 
	iPacketSink = CXPSPacketSink::New();
	__ASSERT_ALWAYS(iPacketSink, User::Leave( KErrNotReady ) );
	
	const TInt KMaxTUintAsDesLen = 12;
	iServerName  = 
	    HBufC::NewL( KMccHelixSource().Length() + KMaxTUintAsDesLen );
	iServerName->Des().AppendFormat( KMccHelixSource, (TUint32)this );
	
	iPacketSink->Init( *iServerName, this );
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::ConstructL, init Helix complete" ) 
    
    iSearchTermUser = CMccVideoSinkUser::NewL( 
        NULL, KNullUid, 0, iSettings.iVideoFrameRate, *iPacketSink );
  
    User::LeaveIfError( iRwSession.Connect() );
	
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::ConstructL connected to ws" )
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl window pos:", 
                                  iSettings.iLocation.iX, " ", iSettings.iLocation.iY ) 
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl window size (w,h):", 
                                  iSettings.iSize.iWidth, " ", iSettings.iSize.iHeight ) 
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl window ordinal pos:", 
                                  iSettings.iWindowOrdinalPosition )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl window ordinal priority:", 
                                  iSettings.iWindowOrdinalPriority )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl rotation:", 
                                  iSettings.iRotation )
    
    CreateWindowingElementsL(iSettings);
      
    // Videoplayer needs to be created last, otherwise destruction
    // order causes problems
    iVideoPlayer = CVideoPlayerUtility2::NewL( 
            *this,
            iSettings.iPriority.iPriority,
            iSettings.iPriority.iPref );

    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl: created CVideoPlayerUtility2" )
    
    // Wait for MvpuoOpenComplete before adding display window
    
    RegisterForVideoLoadingNotification( *this );
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMccVideoSinkImpl* CMccVideoSinkImpl::NewL( 
    const TMccVideoSinkSetting& aSettings,
    TUint32 aEndpointId )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::NewL" ) 
    CMccVideoSinkImpl* self = new ( ELeave ) CMccVideoSinkImpl( aEndpointId );

    CleanupStack::PushL( self );

    self->ConstructL( aSettings );

    CleanupStack::Pop( self ); // self 

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMccVideoSinkImpl::~CMccVideoSinkImpl()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::~CMccVideoSinkImpl" )
    
    SetCurrentUser( NULL );
    
    delete iSdp;
    
    if ( iPacketSink )
        {
        for ( TInt i = 0; i < iUsers.Count(); i++ )
            {
            iPacketSink->StreamEnd( iUsers[ i ]->iStreamId );
            }
        iPacketSink->Reset();
        }

    	
	iUsers.ResetAndDestroy();
	   
    delete iPacketSink;
    
    if( iVideoPlayer )
		{
		Stop();
		iVideoPlayer->Close();	
		}
		
	delete iVideoPlayer;
	iVideoPlayer = NULL;
	
	DeleteWindowingElements();

	iRwSession.Close();

	delete iServerName;
	
	delete iSearchTermUser;
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::~CMccVideoSinkImpl, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SinkThreadLogon()
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
	{
	// Give zero stream id at beginning, correct id is set later
	TRAPD( err, AddUserL( aEventHandler ) );
	return err;
	}
   
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SinkThreadLogoff()
// ---------------------------------------------------------------------------
//     
TBool CMccVideoSinkImpl::SinkThreadLogoff()
	{
	if ( iCurrentUser )
	    {
	    TUint streamId( 0 );
	    if ( ResolveStreamId( iCurrentUser, streamId ) == KErrNone )
	        {
            RemoveVideoSinkUser( iCurrentUser, streamId );
	        }
	    iCurrentUser = NULL;
	    }	
	return ( iUsers.Count() == 0 );
	}

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetCurrentUser()
// ---------------------------------------------------------------------------
// 	
void CMccVideoSinkImpl::SetCurrentUser( MAsyncEventHandler* aEventHandler )
    {
    iCurrentUser = aEventHandler;
    }
        
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::OpenL()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::OpenL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::OpenL" )
    if ( SetVideoSinkState( EOpening ) )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::OpenL, doing setup" )
        
        RemoteSetupL();
        
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::OpenL, opening" )
        __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
        __ASSERT_ALWAYS( iPacketSink, User::Leave( KErrArgument ) );

        const TUid KUidController = { 0x101F8514 };  //RNAVCONTROLLER.DLL
    
        HBufC* url = 
            HBufC::NewLC( KMccPacketSource().Length() + iServerName->Length() );
        url->Des().AppendFormat( KMccPacketSource, &*iServerName );
        iVideoPlayer->OpenUrlL( *url, 0, KNullDesC8, KUidController );
        CleanupStack::PopAndDestroy( url );
        }
    else if ( iState == EOpened )
        {
        // If already opened, prepare can be called immediately
        Prepare();
        }
    else
        {
        __V_SOURCESINK_CONTROLL( 
            "CMccVideoSinkImpl::OpenL, waiting for opening" )
        }
        
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::OpenL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::Prepare()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::Prepare()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Prepare" )
    
    if ( SetVideoSinkState( EPrepared ) && iVideoPlayer )
        {
	    iVideoPlayer->Prepare();
	    
	    // Wait for async completion	
	    }
    }


// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::RemoteSetupL()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::RemoteSetupL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::RemoteSetup" )
    
    __ASSERT_ALWAYS( iVideoPlayer && iPacketSink, User::Leave( KErrNotReady ) );

    ConstructSdpL();
    
    __V_SOURCESINK_CONTROLL_INT1( "Sending SDP to helix, len= ", iSdp->Length() )

    TUint numStreams( iUsers.Count() );
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkImpl::RemoteSetup, num streams:", numStreams );
                                 
    User::LeaveIfError( iPacketSink->SetSessionDescription( *iSdp, numStreams ) );
    
    for ( TInt i = 0;  i < iUsers.Count(); i++ )
        {
        CMccVideoSinkUser& entry = *iUsers[ i ];
        
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkImpl::RemoteSetup, stream:", entry.iStreamId );
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkImpl::RemoteSetup, queuesize defined:", 
            entry.IsQueueSizeDefined() );
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkImpl::RemoteSetup, queuesize:", entry.QueueSize() );
        User::LeaveIfError( 
            iPacketSink->ConfigStream( entry.iStreamId, entry.QueueSize() ) );
        }

    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::RemoteSetup, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::StartL()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::StartL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::StartL" )
    
    if ( SetVideoSinkState( EPlaying ) )
        { 
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::StartL, playing" )

        CMccVideoSinkUser* userEntry = FindVideoSinkUserEntryForCurrent();
        
        UpdateSettingsL( iSettings, ETrue );

        DoPlay();
        
        // No need to wait any async completion
        if ( IsStartedOnce( userEntry ) )
            {
            SendStreamEventToClient( KMccStreamResumed, iCurrentUser );
            }
        else
            {
            SendStreamEventToClient( KMccStreamStarted, iCurrentUser );
            SetStartedOnce( userEntry, ETrue );
            }   
        }
  
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::StartL, exit" )
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::Stop()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::Stop()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Stop" )
    
    TBool playerOpened( IsPlayerOpened() );
    
    if ( SetVideoSinkState( EStopped ) && iVideoPlayer ) 
	    {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Stop, hiding window disabled" )
        
        // Stopping the player before opening has completed can cause crash
        if ( playerOpened )
            {
            iVideoPlayer->Stop();
            }
	    
	    // No need to wait any async completion
	    SendStreamEventToClient( KMccStreamStopped, iCurrentUser );
	    
	    CMccVideoSinkUser* userEntry = FindVideoSinkUserEntryForCurrent();
	    SetStartedOnce( userEntry, EFalse );
	    
	    DoPauseForUser( userEntry );
	    }
	    
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Stop, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::Pause()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::PauseL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::PauseL" )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    
    if ( SetVideoSinkState( EPaused ) )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::PauseL, pausing" )

        DoPauseL();
        
        DoResetL();
        
        // No need to wait any async completion
        SendStreamEventToClient( KMccStreamPaused, iCurrentUser );
        }
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::PauseL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::Close()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::Close()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Close" )
    if ( iVideoPlayer ) 
	    {
	    iVideoPlayer->Close();
	    }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetDisplayWindowL()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetDisplayWindowL( 
    RWsSession& aWs, 
    CWsScreenDevice& aScreenDevice,
    RWindow& aWindow )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetDisplayWindowL" ) 
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    iVideoPlayer->RemoveDisplayWindow( aWindow );
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetDisplayWindowL, removed old" )
    iVideoPlayer->AddDisplayWindowL( aWs, aScreenDevice, aWindow );
    
    iVideoPlayer->SetAutoScaleL( aWindow, EAutoScaleBestFit );
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetDisplayWindowL, added new" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::Priority()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::Priority( TInt& aPriority, 
                                  TInt& aPreference )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Priority" ) 
    
    aPriority = iSettings.iPriority.iPriority;
    aPreference = iSettings.iPriority.iPref;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetPriority()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetPriority( TInt aPriority, 
                                     TInt aPreference )
    {
    if ( iSettings.iPriority.iPriority != aPriority ||
         iSettings.iPriority.iPref != aPreference )
        {
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkImpl::SetPriority, mmf priority:", aPriority )
        __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkImpl::SetPriority, mmf preference:", aPreference )
    
        iSettings.iPriority.iPriority = aPriority;
        iSettings.iPriority.iPref = aPreference;
            
        if ( IsPlayerOpened() && iVideoPlayer )
            {
            __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetPriority, setting" )
            

            TRAPD( err, iVideoPlayer->SetPriorityL( iSettings.iPriority.iPriority, 
                                                    iSettings.iPriority.iPref ) );
            if ( err )
                {
                __V_SOURCESINK_CONTROLL_INT1( 
                    "CMccVideoSinkImpl::SetPriority, failed with err:", err )
                err++;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::Volume()
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::Volume() const 
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Volume" )
    
    if ( iVideoPlayer )
	    {
	    return iVideoPlayer->Volume();	
	    }
    else
	    {
	    return KErrArgument;	    	
	    }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MaxVolume()
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::MaxVolume() const 
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::MaxVolume" )

    if ( iVideoPlayer )
	    {
	    return iVideoPlayer->MaxVolume();
	    }
    else
	    {
	    return KErrArgument;
	    }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetVolumeL()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetVolumeL( TInt aVolume )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkImpl::SetVolumeL, vol:", aVolume )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    
    iVolume = aVolume;
    
    if ( iVolumeAdaptation == 0 && iMccMaxVolume != 0 && IsPlayerOpened() )
        {
        TInt playerMaxVolume = iVideoPlayer->MaxVolume();
        
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkImpl::SetVolumeL, player max volume:", 
            playerMaxVolume )
        
        iVolumeAdaptation = TReal( playerMaxVolume ) / iMccMaxVolume;
        
        __V_SOURCESINK_CONTROLL_REAL( 
            "CMccVideoSinkImpl::SetVolumeL, vol adapt:", iVolumeAdaptation )
        }
    
    if ( iVolumeAdaptation != 0 )
        {
        aVolume = (TInt) ( iVolume * iVolumeAdaptation );
        
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkImpl::SetVolumeL, after adaptation:", aVolume )
    
        iVideoPlayer->SetVolumeL( aVolume );
        }

    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetVolumeL, exit" )
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetMaxVolume()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetMaxVolume( TInt aMaxVolume )
    {
     __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::SetMaxVolume, max vol:", aMaxVolume )
     
    iMccMaxVolume = aMaxVolume;
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::Balance()
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::Balance() const
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Balance" )
    
    if ( iVideoPlayer ) 
	    {
	   	return iVideoPlayer->Balance();
	    }
	else
		{
		return KErrArgument;
		}
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetBalanceL()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetBalanceL( TInt aBalance )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetBalanceL" )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    iVideoPlayer->SetBalanceL(aBalance);
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::RegisterForVideoLoadingNotification()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::RegisterForVideoLoadingNotification(
                                     MVideoLoadingObserver& aCallback )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::RegisterForVideoLoadingNotification" )
    
    if ( iVideoPlayer )
	    {
	    iVideoPlayer->RegisterForVideoLoadingNotification( aCallback );
	    }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::Version()
// ---------------------------------------------------------------------------
//
TVersion CMccVideoSinkImpl::Version() const
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Version" )
    return TVersion(0, 1, 0);
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::FormatVideoSDPInfoL
// ---------------------------------------------------------------------------
// 	
void CMccVideoSinkImpl::FormatVideoSDPInfoL( const TMccCodecInfo& aVideoCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::FormatVideoSDPInfoL" )
    
    // Update payload type since it might not have been provided at
    // initialization phase
    iSettings.iVideoPayloadType = aVideoCodec.iPayloadType;
    
    CMccVideoSinkUser* userEntry = FindVideoSinkUserEntryForCurrent();
    __ASSERT_ALWAYS( userEntry, User::Leave( KErrNotFound ) );
    userEntry->iMediaType = KUidMediaTypeVideo;
    userEntry->SetCodecInfoL( aVideoCodec );
	}
	
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::FormatAudioSDPInfoL
// ---------------------------------------------------------------------------
// 	
void CMccVideoSinkImpl::FormatAudioSDPInfoL( const TMccCodecInfo& aAudioCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::FormatAudioSDPInfoL" )
    
    // Update payload type since it might not have been provided at
    // initialization phase
    iSettings.iAudioPayloadType = aAudioCodec.iPayloadType;
    
    CMccVideoSinkUser* userEntry = FindVideoSinkUserEntryForCurrent();
    __ASSERT_ALWAYS( userEntry, User::Leave( KErrNotFound ) );
    userEntry->iMediaType = KUidMediaTypeAudio;
    userEntry->SetCodecInfoL( aAudioCodec );
	}

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::EmptyBufferL
// 
// ---------------------------------------------------------------------------
//     
void CMccVideoSinkImpl::EmptyBufferL( CMMFBuffer* aBuffer, 
		                          MDataSource* /*aSupplier*/,
		                          TMediaId aMediaId,
		                          TRtpRecvHeader& aHeaderInfo )	
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::EmptyBufferL" )
    
    if ( iState != EPlaying )
        {
        __V_SOURCESINK_CONTROLL( 
            "CMccVideoSinkImpl::EmptyBufferL, not playing, ignore buffer!" )
        return;
        }
    
    TInt index = FindVideoSinkUserEntryByMediaType( aMediaId.iMediaType );
    __ASSERT_ALWAYS( index != KErrNotFound, User::Leave( KErrNotFound ) );
    
    CMccVideoSinkUser& user = *iUsers[ index ];
	
	__V_SOURCESINK_CONTROLL_INT1( 
	    "CMccVideoSinkImpl::EmptyBufferL, streamId:", user.iStreamId )
	
	CMMFDataBuffer* buf = static_cast<CMMFDataBuffer*>( aBuffer ); 
	
	TInt dataSize = buf->Data().Length();
	__V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::EmptyBufferL, data size:", 
    	                          dataSize )		

	if ( AnalyzeTimeStampL( user, aHeaderInfo ) &&
	     user.PacketOverflowState() != CMccVideoSinkUser::EOccured && 
	     dataSize >= KMccMinPacketSize )
		{
    	__V_SOURCESINK_CONTROLL( 
    	    "CMccVideoSinkImpl::EmptyBufferL, sending packet to helix" )		
      
        CMccVideoJitterBuffer::TMccPacketBufferingStatus bufferingStatus =
            user.EnqueueL( aHeaderInfo, buf->Data() );
        
        if ( bufferingStatus == CMccVideoJitterBuffer::EPlaying )
    	    {
    	    DoMvloLoadingComplete( ETrue );
    	    }
        else
            {
            DoMvloLoadingStarted( ETrue );
            }
    	    
        iNumPacketsEnqueued++;
		}
	else
		{
		__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::EmptyBufferL, packet dropped" )		
		}

	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::EmptyBufferL, exit" )
	}

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::UpdateSettingsL() 
// ---------------------------------------------------------------------------
//	
void CMccVideoSinkImpl::UpdateSettingsL( 
    const TMccVideoSinkSetting& aSettings,
    TBool aForceUpdate )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::UpdateSettingsL" )
    
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrNotReady ) );
    
    if ( aForceUpdate ||
         iSettings.iWindowOrdinalPosition != aSettings.iWindowOrdinalPosition ||
         iSettings.iWindowOrdinalPriority != aSettings.iWindowOrdinalPriority ||
         iSettings.iLocation != aSettings.iLocation ||
         iSettings.iSize != aSettings.iSize ||
         iSettings.iDeviceIndex != aSettings.iDeviceIndex ||
         iSettings.iRotation != aSettings.iRotation )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::UpdateSettingsL, settings changed" )
        
        __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl window pos:", 
                aSettings.iLocation.iX, " ", aSettings.iLocation.iY ) 
        __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl window size (w,h):", 
                aSettings.iSize.iWidth, " ", aSettings.iSize.iHeight ) 
        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl window ordinal pos:", 
                aSettings.iWindowOrdinalPosition )
        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl window ordinal priority:", 
                aSettings.iWindowOrdinalPriority )
        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl rotation:", 
                aSettings.iRotation )
        
        // Need to recreate windowing stuff, otherwise orientation switch does not work
        DeleteWindowingElements();
        CreateWindowingElementsL(aSettings);
        
        UpdateScreenTransparencyL(aSettings);
            
        SetDisplayWindowL( iRwSession, *iDev, *iRw );
        
        SetRotationL( aSettings.iRotation, aForceUpdate );
        
        __V_SOURCESINK_CONTROLL( 
            "CMccVideoSinkImpl::UpdateSettingsL, videoplyer updated" )
        }
    
    // Store new settings
    iSettings = aSettings;
    
    SetVideoFrameRate( aSettings.iVideoFrameRate );
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::UpdateSettingsL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::GetCurrentSettings() 
// ---------------------------------------------------------------------------
//        
void CMccVideoSinkImpl::GetCurrentSettings( TMccVideoSinkSetting& aSettings ) const
    {
    aSettings = iSettings;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::RestorePacketSupply() 
// ---------------------------------------------------------------------------
//	
void CMccVideoSinkImpl::RestorePacketSupply( TUint aStreamId )
	{
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkImpl::RestorePacketSupply, streamId:", aStreamId )
    iSearchTermUser->Set( 0, KNullUid, aStreamId );
    TIdentityRelation<CMccVideoSinkUser> comparison( VideoSinkUserMatch );
    TInt index = iUsers.Find( iSearchTermUser, comparison );
    if ( index != KErrNotFound )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::RestorePacketSupply, found entry")
        
        iUsers[ index ]->SetPacketOverflow( CMccVideoSinkUser::ERecovered );
        }

    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::RestorePacketSupply, exit" )	
	}

	
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MvloLoadingStarted() 
// ---------------------------------------------------------------------------
//     
void CMccVideoSinkImpl::MvloLoadingStarted() 
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::MvloLoadingStarted" )
	
	DoMvloLoadingStarted();
	}

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MvloLoadingComplete()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvloLoadingComplete()
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::MvloLoadingComplete" ) 

    DoMvloLoadingComplete();
	}
	
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MvpuoOpenComplete()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvpuoOpenComplete( TInt aError )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::MvpuoOpenComplete" ) 

    if ( !aError && ( iState != EStopped ) )
        {
        iState = EOpened;
        SetPriority( iSettings.iPriority.iPriority, iSettings.iPriority.iPref );
        TRAP( aError, 
            {
            SetDisplayWindowL( iRwSession, *iDev, *iRw );
            SetVolumeL( iVolume );
            SetRotationL( iSettings.iRotation, ETrue );
            } );
            
        if ( !aError )
            {
            Prepare();
            }
        }
        
    if ( aError )
        {
        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::MvpuoOpenComplete, error:", 
                                      aError ) 
                                      
        SendStreamEventToAllClients( KMccStreamPrepared, aError );
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MvpuoPrepareComplete()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvpuoPrepareComplete( TInt aError )
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::MvpuoPrepareComplete(), \
aError=", aError)
	
    SendStreamEventToAllClients( KMccStreamPrepared, aError );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MvpuoFrameReady()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvpuoFrameReady( CFbsBitmap& /*aFrame*/,
#ifdef _DEBUG
TInt aError
#else
TInt /*aError*/
#endif
 )
    {
#ifdef _DEBUG
    
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::MvpuoFrameReady, \
aError=", aError )
#endif
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MvpuoPlayComplete()
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvpuoPlayComplete( TInt aError )
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::MvpuoPlayComplete(), \
aError=", aError)
	
	if ( aError )
	    {
        SendStreamEventToAllClients( KMccStreamStopped, aError );
	    }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MvpuoEvent()
// ---------------------------------------------------------------------------
//	
void CMccVideoSinkImpl::MvpuoEvent( const TMMFEvent& aEvent )
    {
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl::MvpuoEvent(), \
aEvent.iEventType=", (TInt)aEvent.iEventType.iUid, "aEvent.iErrorCode", 
    aEvent.iErrorCode)

    if ( aEvent.iEventType == KMMFEventCategoryVideoPlayerGeneralError /*&&
         aEvent.iErrorCode == KConnectionDone*/ )
        {
        // NOP
        }
    }

// -----------------------------------------------------------------------------
// CMccVideoSinkImpl::SetVideoSinkState
// -----------------------------------------------------------------------------
//    
TBool CMccVideoSinkImpl::SetVideoSinkState( TMccVideoSinkState aState )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkImpl::SetVideoSinkState, state:", aState )

    TBool controlSink( iState != aState );
    
    TMccEventType eventType = KMccEventNone;
    
    switch ( aState )
        {
        case EOpening:
            {
            // No event to client
            if ( AllUsersReady() )
                {
                iState = aState;
                }
            else
                {
                // All users not ready, don't start opening yet
                controlSink = EFalse;
                }
            break;
            }
        case EOpened:
            {
            // No event to client
            iState = aState;
            break;
            }
        case EPrepared:
            {
            eventType = KMccStreamPrepared;
            iState = aState;
            break;
            }
        case EPlaying:
            {
            CMccVideoSinkUser* userEntry = FindVideoSinkUserEntryForCurrent();
            if ( IsStartedOnce( userEntry ) )
                {
                eventType = KMccStreamResumed;
                }
            else
                {
                eventType = KMccStreamStarted;
                
                if ( !controlSink )
                    {
                    SetStartedOnce( userEntry, ETrue );
                    }
                }
            iState = aState;
            break;
            }
        case EPaused:
            {
            eventType = KMccStreamPaused;
            iState = aState;
            break;
            }
        case EStopped:
            {
            eventType = KMccStreamStopped;
            CMccVideoSinkUser* userEntry = FindVideoSinkUserEntryForCurrent();
            SetStartedOnce( userEntry, EFalse );
            iState = aState;
            break;
            }
        default:
            {
            break;
            }
        }
        
    if ( eventType != KMccEventNone && !controlSink )
        {
        SendStreamEventToClient( eventType, iCurrentUser );
        }
        
    return controlSink;
    }

// -----------------------------------------------------------------------------
// CMccVideoSinkImpl::CreateModesetDesL()
// -----------------------------------------------------------------------------
// 
HBufC8* CMccVideoSinkImpl::CreateModesetDesL( TUint aBitrateMask )
    {
    _LIT8( KModesetDelim, "," );
    const TInt KModeValMaxLength = 2;
    const TInt KModesMaxAmount = 8;
    const TInt KModesetDelimLength = KModesetDelim().Length();
    
    HBufC8* modeset = 
        HBufC8::NewL( KModesMaxAmount * 
                    ( KModeValMaxLength + KModesetDelimLength ) );
    
    TPtr8 modesetPtr( modeset->Des() );
    
    TBool allModes( 
        MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrateAll ) );
                 
    if ( allModes || 
         MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrate475 ) )
        {
        modesetPtr.AppendNum( KAMRMode0 );
        modesetPtr.Append( KModesetDelim );
        }
    if ( allModes || 
         MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrate515 ) )
        {
        modesetPtr.AppendNum( KAMRMode1 );
        modesetPtr.Append( KModesetDelim );
        }
    if ( allModes || 
         MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrate590 ) )
        {
        modesetPtr.AppendNum( KAMRMode2 );
        modesetPtr.Append( KModesetDelim );
        }
    if ( allModes || 
         MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrate670 ) )
        {
        modesetPtr.AppendNum( KAMRMode3 );
        modesetPtr.Append( KModesetDelim );
        }
    if ( allModes || 
         MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrate740 ) )
        {
        modesetPtr.AppendNum( KAMRMode4 );
        modesetPtr.Append( KModesetDelim );
        }
    if ( allModes || 
         MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrate795 ) )
        {
        modesetPtr.AppendNum( KAMRMode5 );
        modesetPtr.Append( KModesetDelim );
        }
    if ( allModes || 
         MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrate102 ) )
        {
        modesetPtr.AppendNum( KAMRMode6 );
        modesetPtr.Append( KModesetDelim );
        }
    if ( allModes || 
         MCC_BITRATE_IN_MASK( aBitrateMask, KMccAllowedAmrNbBitrate122 ) )
        {
        modesetPtr.AppendNum( KAMRMode7 );
        modesetPtr.Append( KModesetDelim );
        }
        
    if ( modesetPtr.Length() - KModesetDelimLength >= 0 )
        {
        // Remove last delimiter
        modesetPtr.Delete( modesetPtr.Length() - KModesetDelimLength, 
                           KModesetDelimLength );
        }
        
    return modeset;
    }
   
// -----------------------------------------------------------------------------
// CMccVideoSinkImpl::SendStreamEventToAllClients()
// -----------------------------------------------------------------------------
//	
void CMccVideoSinkImpl::SendStreamEventToAllClients( 
    TMccEventType aEventType,
    TInt aError,
    TBool aAllEndpoints )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SendStreamEventToAllClients" )
    
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        SendStreamEventToClient( aEventType, 
                                 iUsers[ i ]->iAsyncEventHandler, 
                                 aError, 
                                 aAllEndpoints );
        }
    }
       
// -----------------------------------------------------------------------------
// CMccVideoSinkImpl::SendStreamEventToClient()
// -----------------------------------------------------------------------------
//	
void CMccVideoSinkImpl::SendStreamEventToClient( 
    TMccEventType aEventType,
    MAsyncEventHandler* aEventHandler,
    TInt aError,
    TBool aAllEndpoints )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SendStreamEventToClient" )
    
    if ( aEventHandler )
	    {
	    aEventType = aError ? KMccStreamError : aEventType;
	    
	    TUint32 endpointId = 
	        aAllEndpoints ? 0 : iEndpointId;
	        
	    TMccEvent event( 0, 
	                     0, 
	                     0, 
	                     endpointId, 
	                     KMccEventCategoryStream, 
	                     aEventType, 
	                     aError, 
	                     KNullDesC8 );

		TMccInternalEvent internalEvent( KMccVideoSinkUid, 
		                                 EMccInternalEventNone,
		                                 event );
		                         
		aEventHandler->SendEventToClient( internalEvent );
	    }
	else
		{
		__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SendStreamEventToClient, \
		aEventHandler=NULL" )
		}
    }

// -----------------------------------------------------------------------------
// CMccVideoSinkImpl::ResolveStreamId()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::ResolveStreamId( TMediaId aMediaId, TUint& aStreamId )
    {
    iSearchTermUser->Set( 0, aMediaId.iMediaType, 0 );
    return FindStreamIdForVideoSinkUser( *iSearchTermUser, aStreamId );
    }

// -----------------------------------------------------------------------------
// CMccVideoSinkImpl::ResolveStreamId()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::ResolveStreamId( 
    MAsyncEventHandler* aUser, 
    TUint& aStreamId )
    {
    iSearchTermUser->Set( aUser, KNullUid, 0 );
    return FindStreamIdForVideoSinkUser( *iSearchTermUser, aStreamId );
    }   
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::GenerateStreamId
// ---------------------------------------------------------------------------
//
TUint CMccVideoSinkImpl::GenerateStreamId()
    {
    return iHelixStreamId++;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::FindVideoSinkUserEntryByMediaType
// ---------------------------------------------------------------------------
//   
TInt CMccVideoSinkImpl::FindVideoSinkUserEntryByMediaType( TUid aMediaType )
    {
    iSearchTermUser->Set( NULL, aMediaType, 0 );
    TIdentityRelation<CMccVideoSinkUser> comparison( VideoSinkUserMatch );
    return iUsers.Find( iSearchTermUser, comparison );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::FindStreamIdForVideoSinkUser
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::FindStreamIdForVideoSinkUser( 
    CMccVideoSinkUser& aEntry,
    TUint& aStreamId )
    {
    TInt err( KErrNotFound );

    TIdentityRelation<CMccVideoSinkUser> comparison( VideoSinkUserMatch );
    TInt index = iUsers.Find( &aEntry, comparison );
    if ( index != KErrNotFound )
        {
        aStreamId = iUsers[ index ]->iStreamId;
        err = KErrNone;
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::FindVideoSinkUserEntryForCurrent
// ---------------------------------------------------------------------------
//
CMccVideoSinkUser* CMccVideoSinkImpl::FindVideoSinkUserEntryForCurrent()
    {
    iSearchTermUser->Set( iCurrentUser, KNullUid, 0 );
    TIdentityRelation<CMccVideoSinkUser> comparison( VideoSinkUserMatch );
    TInt index = iUsers.Find( iSearchTermUser, comparison );
    if ( index != KErrNotFound )
        {
        return iUsers[ index ];
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::RemoveVideoSinkUser
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::RemoveVideoSinkUser( 
    MAsyncEventHandler* aUser, TUint aStreamId )
    {
    iSearchTermUser->Set( aUser, KNullUid, aStreamId );
    TIdentityRelation<CMccVideoSinkUser> comparison( VideoSinkUserMatch );
    TInt index = iUsers.Find( iSearchTermUser, comparison );
    if ( index != KErrNotFound )
        {
        delete iUsers[ index ];
        iUsers.Remove( index );
        }
    
    // If there's no more users for this stream id, packet sink is notified
    // about stream ending
    iSearchTermUser->Set( NULL, KNullUid, aStreamId );
    index = iUsers.Find( iSearchTermUser, comparison );
    if ( iPacketSink && index == KErrNotFound )
        {
        iPacketSink->StreamEnd( aStreamId );
        }
    }
        
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::VideoSinkUserMatch
// ---------------------------------------------------------------------------
//   
TBool CMccVideoSinkImpl::VideoSinkUserMatch( 
    const CMccVideoSinkUser& aUser1, 
    const CMccVideoSinkUser& aUser2 )
    {
    // First argument is always the search term
    
    TBool match( EFalse );
    if ( aUser1.iAsyncEventHandler )
        {
        match = ( aUser1.iAsyncEventHandler == aUser2.iAsyncEventHandler );
        }
    else if ( aUser1.iMediaType != KNullUid )
        {
        match = ( aUser1.iMediaType == aUser2.iMediaType );
        }
    else
        {
        match = ( aUser1.iStreamId == aUser2.iStreamId );
        }
    return match;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::ConstructSdpL
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::ConstructSdpL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::ConstructSdpL" )
    
    __ASSERT_ALWAYS( iSdp, User::Leave( KErrNotReady ) );

    iSdp->Des().Copy( KMccSdpInboundStart );
    
    
    // Check amount of mediatypes before constructing sdp as 
    // sdp contents are affected on that information    
    TBool multipleMediaTypes( MultipleMediaTypes() );
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        iUsers[ i ]->SetMultipleMediaTypesL( multipleMediaTypes );
        }
        
    HandleAudioSdpL();
    
    HandleVideoSdpL();
    
    __V_SOURCESINK_CONTROLL_STR8( "SDP:", *iSdp )
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::ConstructSdpL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::HandleAudioSdpL
// ---------------------------------------------------------------------------
//    
void CMccVideoSinkImpl::HandleAudioSdpL()
    {
    TInt audioEntry = FindVideoSinkUserEntryByMediaType( KUidMediaTypeAudio );
    if ( audioEntry == KErrNotFound )
 	    {
 	    __V_SOURCESINK_CONTROLL( 
 	        "CMccVideoSinkImpl::HandleAudioSdpL, no audio codec defined" )
 	    return;
 	    }	   
 	
 	// Set correct stream id   
 	iUsers[ audioEntry ]->iStreamId = GenerateStreamId();

    // Form sdp from codec information
    TMccCodecInfo& audioCodec = iUsers[ audioEntry ]->CodecInfo();
    	
    HBufC8* modeset = CreateModesetDesL( audioCodec.iBitrateMask );

    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::HandleAudioSdpL, AudioPayloadType:", 
                                  iSettings.iAudioPayloadType )
	
    iSdp->Des().AppendFormat( KMccSdpInboundAudio, 
	                          iSettings.iAudioPayloadType,
                    	      iSettings.iAudioPayloadType, 
                              &audioCodec.iSdpName, 
                              audioCodec.iPtime,
                              audioCodec.iMaxPtime,
    	                      iSettings.iAudioPayloadType,
    	                      ( audioCodec.iCodecMode == KAmrCodecModeOctetAlign ),
    	                      &*modeset );
    
    
    delete modeset;
    
    iSdp->Des().AppendFormat( 
        KMccSdpInboundPreroll, iUsers[ audioEntry ]->GetPreroll() );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::HandleVideoSdpL
// ---------------------------------------------------------------------------
//     
void CMccVideoSinkImpl::HandleVideoSdpL()
    {
    TUint streamId( 0 );
    TBool firstUserHandled( EFalse );
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        CMccVideoSinkUser& user = *iUsers[ i ];
        if ( user.iMediaType == KUidMediaTypeVideo )
            {
            if ( !firstUserHandled )
                {
                streamId = GenerateStreamId();
                
                // Form sdp from codec information
            	TMccCodecInfo& videoCodec = user.CodecInfo();
         	
                 // Helix uses width and height info for loading appropriate
                 // video decoder
                TInt frameWidth = KH263FrameWidth;
                TInt frameHeight = KH263FrameHeight;
            		
            	__V_SOURCESINK_CONTROLL_INT1( 
            	    "CMccVideoSinkImpl::HandleVideoSdpL, VideoPayloadType:", 
            	    iSettings.iVideoPayloadType )

                HBufC8* fmtp = GetFmtpLC( videoCodec );
            	iSdp->Des().AppendFormat( KMccSdpInboundVideo, 
            	                          iSettings.iVideoPayloadType,
                                	      iSettings.iVideoPayloadType, 
                                          &videoCodec.iSdpName, 
                	                      iSettings.iVideoPayloadType,
                	                      &*fmtp,
                	                      user.GetPayloadSize(),
                	                      frameWidth,
                	                      frameHeight );
                CleanupStack::PopAndDestroy( fmtp );
                
                
                iPreroll = user.GetPreroll();
                
                if ( RealTimeMode() )
                    {
                    __V_SOURCESINK_CONTROLL( 
                        "CMccVideoSinkImpl::HandleVideoSdpL, disable clock" )
                    iSdp->Des().Append( KMccSdpInboundDisableVideoClock );
                    
                    // Currently, preroll needs to be set to some value
                    // until disable video clock feature is working fully
                    // at Helix side, once supported fully, preroll value
                    // will not have any effect.
                    const TInt KMccVideoSinkRealTimeTransitionPreroll = 500;
                    iPreroll = KMccVideoSinkRealTimeTransitionPreroll;
                    }
                
                 __V_SOURCESINK_CONTROLL_INT1( 
                    "CMccVideoSinkImpl::HandleVideoSdpL, iPreroll ", iPreroll )
                iPrerollSet = ETrue;
                iSdp->Des().AppendFormat( KMccSdpInboundPreroll, iPreroll );
                
                firstUserHandled = ETrue;
                }
                
            // Set correct stream id (all video users will have the same id)
            user.iStreamId = streamId;
            }
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::AnalyzeTimeStampL
// Checks whether rtp packet belongs to currently played stream. Check is done
// based on timestamp monitoring. If this is new stream, helix needs to be 
// resetted as it cannot cope with stream change. Helix is resetted
// immediately if one of media types is detected to be from new stream.
// Packets of other media types need to be dropped until also new stream
// in those begins.
// ---------------------------------------------------------------------------
//
TBool CMccVideoSinkImpl::AnalyzeTimeStampL( 
    CMccVideoSinkUser& aUser, 
    TRtpRecvHeader& aHeaderInfo )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::AnalyzeTimeStampL" )
        
    aUser.CalculateAverageTimeStampDifference();

    if ( aUser.IsResetNeeded( aHeaderInfo ) )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::AnalyzeTimeStampL, reset needed" )
            
        if ( ResetOngoing() )
            {
            // XPS was resetted already earlier, allow queueing for this user
            aUser.Reset( ETrue );
            }
        else
            {
            DoResetL( ETrue, &aUser );
            
            // Other media types cannot pass their data until new stream
            // in them begin
            for ( TInt i = 0; i < iUsers.Count(); i++ )
                {
                if ( iUsers[ i ] != &aUser )
                    {
                    iUsers[ i ]->SetAllowFrame( EFalse );
                    }
                }
            }     
        
        }
    
    aUser.AddTimeStamp( (TInt64)aHeaderInfo.iTimestamp, aHeaderInfo.iSeqNum );
        
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::AnalyzeTimeStampL, exit" )
    
    return aUser.AllowFrame();
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::DoResetL
// ---------------------------------------------------------------------------
//     
void CMccVideoSinkImpl::DoResetL( TBool aFullReset, CMccVideoSinkUser* aUser )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::DoResetL" )
    
    // Start from scratch.
    //
    
    if ( iVideoPlayer )
        {
        // If playing, have to pause, otherwise set position will not work
        if ( iState == EPlaying )
            {
            __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::DoResetL, mode 1" )
            DoPauseL( aUser );
            ResetPacketSink();
            iVideoPlayer->SetPositionL( TTimeIntervalMicroSeconds( 0 ) );
            DoPlay( aUser );
            }
        else
            { 
            __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::DoResetL, mode 2" )
            ResetPacketSink();
            iVideoPlayer->SetPositionL( TTimeIntervalMicroSeconds( 0 ) );
            }
        }
    
    if ( aUser )
        {
        aUser->Reset( aFullReset );
        }
    else
        {
        for ( TInt i = 0; i < iUsers.Count(); i++ )
            {
            iUsers[ i ]->Reset( aFullReset );
            }
        }
        
    iNumPacketsEnqueued = 0;
        
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::DoResetL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::IsPlayerOpened
// ---------------------------------------------------------------------------
//    
TBool CMccVideoSinkImpl::IsPlayerOpened() const
    {
    return ( iState > EOpening );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::ResetPacketSink
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::ResetPacketSink()
    {
    if ( iPacketSink )
        {
        iPacketSink->Reset(); 
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::IsStartedOnce
// ---------------------------------------------------------------------------
//   
TBool CMccVideoSinkImpl::IsStartedOnce( const CMccVideoSinkUser* aUser )
    {
    return ( aUser && aUser->iStartedOnce );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetStartedOnce
// ---------------------------------------------------------------------------
//   
void CMccVideoSinkImpl::SetStartedOnce( 
    CMccVideoSinkUser* aUser, 
    TBool aIsStartedOnce )
    {
    if ( aUser )
        {
        aUser->iStartedOnce = aIsStartedOnce;
        }
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetRotationL
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetRotationL( 
    TMccVideoRotation aRotation, 
    TBool aForceUpdate )
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::SetRotationL, rotation:", 
                                  aRotation )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    
    if ( aForceUpdate || ( aRotation != iSettings.iRotation ) )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetRotationL, setting" )
        
        // Enums for videoplayer rotation and mcc rotation are identical
        iVideoPlayer->SetRotationL( *iRw, static_cast<TVideoRotation>( aRotation ) );
        }
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetRotationL, exit" )
    }
    

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::GetActualPrerollL
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::GetActualPrerollL( TInt& aPreroll )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkImpl::GetActualPrerollL, iPrerollSet:", iPrerollSet )
    TInt videoEntry = FindVideoSinkUserEntryByMediaType( KUidMediaTypeVideo );
    if ( videoEntry == KErrNotFound )
 	    {
 	    __V_SOURCESINK_CONTROLL( 
 	        "CMccVideoSinkImpl::GetActualPrerollL, no video codec defined" )
 	    return;
 	    }
 	
    aPreroll = iUsers[ videoEntry ]->GetActualPreroll();
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::GetVideoFrameRate
// ---------------------------------------------------------------------------
//
TReal CMccVideoSinkImpl::GetVideoFrameRate()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::GetVideoFrameRate" )
    return iSettings.iVideoFrameRate;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::SetVideoFrameRate
// ---------------------------------------------------------------------------
//        
void CMccVideoSinkImpl::SetVideoFrameRate( TReal aFrameRate )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkImpl::SetVideoFrameRate, framerate:", (TUint32) aFrameRate )
    
    if ( aFrameRate > 0 )
        {   
        iSettings.iVideoFrameRate = aFrameRate;
        
        for ( TInt i = 0; i < iUsers.Count(); i++ )
            {
            TRAP_IGNORE( iUsers[ i ]->SetFrameRateL( iSettings.iVideoFrameRate ) )
            }
        }
    else
        {
        iSettings.iVideoFrameRate = KMccDefaultVideoFrameRate;
        }
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetVideoFrameRate, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::ResetOngoing
// ---------------------------------------------------------------------------
//
TBool CMccVideoSinkImpl::ResetOngoing() const
    {
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        if ( !iUsers[ i ]->AllowFrame() )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::GetFmtpLC
// ---------------------------------------------------------------------------
//
HBufC8* CMccVideoSinkImpl::GetFmtpLC( const TMccCodecInfo& aCodecInfo )    
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::GetFmtpLC" )
    	
    CMccCodecInformationFactory* factory = CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( factory );
    CMccCodecInformation* codec = 
        factory->CreateCodecInformationL( aCodecInfo.iSdpName );
    CleanupStack::PushL( codec );
    codec->SetValues( aCodecInfo );
    HBufC8* fmtp = codec->GetFmtpL().AllocL();
    CleanupStack::PopAndDestroy( codec );
    CleanupStack::PopAndDestroy( factory );
    CleanupStack::PushL( fmtp );
    return fmtp;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::DoPauseL
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::DoPauseL( const CMccVideoSinkUser* aUser )
    {
    if ( !RealTimeMode() )
        {
        iVideoPlayer->PauseL();
        }
    
    DoPauseForUser( aUser );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::DoPauseForUser
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::DoPauseForUser( const CMccVideoSinkUser* aUser )
    {
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        if ( !aUser || aUser == iUsers[ i ] )
            {
            iUsers[ i ]->Pause();
            }
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::DoPlay
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::DoPlay( const CMccVideoSinkUser* aUser )
    {
    for ( TInt i = 0; i < iUsers.Count(); i++ )
        {
        if ( !aUser || aUser == iUsers[ i ] )
            {
            iUsers[ i ]->Play();
            }
        }
        
    iVideoPlayer->Play();
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MultipleMediaTypes
// ---------------------------------------------------------------------------
//
TBool CMccVideoSinkImpl::MultipleMediaTypes()
    {
    TInt audioEntry = FindVideoSinkUserEntryByMediaType( KUidMediaTypeAudio );
    TInt videoEntry = FindVideoSinkUserEntryByMediaType( KUidMediaTypeVideo );
    return ( audioEntry != KErrNotFound && videoEntry != KErrNotFound );	   
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::RealTimeMode
// ---------------------------------------------------------------------------
//
TBool CMccVideoSinkImpl::RealTimeMode()
    {
    TBool realTimeMode( EFalse );
    if ( !MultipleMediaTypes() )
        {
        for ( TInt i = 0; i < iUsers.Count() && !realTimeMode; i++ )
            {
            // If preroll is zero, real time mode is used
            if ( iUsers[ i ]->GetPreroll() == 0 )
                {
                realTimeMode = ETrue;
                }
            }
        }
    
    return realTimeMode;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::AddUserL
// ---------------------------------------------------------------------------
//    
void CMccVideoSinkImpl::AddUserL( MAsyncEventHandler& aEventHandler )
    {
    __ASSERT_ALWAYS( iPacketSink, User::Leave( KErrNotReady ) );
    CMccVideoSinkUser* user = CMccVideoSinkUser::NewLC( 
        &aEventHandler, KNullUid, 0, iSettings.iVideoFrameRate, *iPacketSink );
    iUsers.AppendL( user );
    CleanupStack::Pop( user );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::DoMvloLoadingComplete
// If in realtime mode, helix gives loading completion istantly after starting
// whichis not desired as buffering is done at MCC side. In that case
// loading completion is not notified until MCC jitter buffer completes
// loading (i.e. play threshold is reached).
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::DoMvloLoadingComplete( TBool aSimulated )
	{
	if ( RealTimeMode() )
        {
        if ( aSimulated && !iSimulatedStreamingEventSent )
            {
            __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::DoMvloLoadingComplete, simulate" ) 
            
            SendStreamEventToAllClients( KMccStreamPlaying, KErrNone, ETrue );
            
            iSimulatedBufferingEventSent = EFalse;
            iSimulatedStreamingEventSent = ETrue;
            }
        }
	else if ( !aSimulated )
        {
        SendStreamEventToAllClients( KMccStreamPlaying, KErrNone, ETrue );
        }
    else
        {
        // NOP
        }
	}

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::DoMvloLoadingComplete
// If in realtime mode, helix gives loading started instantly after starting
// it (even before passing in any packets). Loading starting is not notified
// until first packet has been passed to MCC video jitterbuffer.
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::DoMvloLoadingStarted( TBool aSimulated )
    {
    if ( RealTimeMode() )
        {
        if ( aSimulated && !iSimulatedBufferingEventSent )
            {
            __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::DoMvloLoadingStarted, simulate" ) 
            
            SendStreamEventToAllClients( KMccStreamBuffering, KErrNone, ETrue );
            
            iSimulatedBufferingEventSent = ETrue;
            iSimulatedStreamingEventSent = EFalse;
            }
        }
    else if ( !aSimulated )
        {
        SendStreamEventToAllClients( KMccStreamBuffering, KErrNone, ETrue );
        }
    else
        {
        // NOP
        }
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::AllUsersReady
// Check if some user hasn't yet configured its settings
// ---------------------------------------------------------------------------
// 
TBool CMccVideoSinkImpl::AllUsersReady()
    {
    TBool allUsersReady( ETrue );
    for ( TInt i = 0; i <  iUsers.Count(); i++ )
        {
        if ( iUsers[ i ]->iMediaType == KNullUid )
            {
            allUsersReady = EFalse;
            }
        }
    return allUsersReady;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
// 
void CMccVideoSinkImpl::UpdateScreenTransparencyL(const TMccVideoSinkSetting& aSettings)
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::UpdateScreenTransparencyL" )
    __ASSERT_ALWAYS( iGc && iRw, User::Leave( KErrNotReady ) );
    TRect vfRect( TPoint( 0, 0 ), aSettings.iSize );
    iGc->Activate( *iRw ); 
    iRw->Invalidate( vfRect );
    iRw->BeginRedraw( vfRect );
    iGc->CancelClippingRect();
    iGc->SetDrawMode( CGraphicsContext::EDrawModeWriteAlpha );
    iGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iGc->SetBrushColor( TRgb( 0, 0, 0, 0 ) );
    iGc->DrawRect( vfRect );
    iRw->EndRedraw();
    iGc->Deactivate();
    iRwSession.Flush();
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::UpdateScreenTransparencyL, exit" )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::DeleteWindowingElements()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::DeleteWindowingElements" )
    if ( iVideoPlayer && iRw )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl: removing window from player" )
        iVideoPlayer->RemoveDisplayWindow( *iRw );
        }
    delete iGc;
    iGc = NULL;
    delete iDev;
    iDev = NULL;
    delete iRw;
    iRw = NULL;
    delete iRwGroup;    
    iRwGroup = NULL;
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::DeleteWindowingElements, exit" )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::CreateWindowingElementsL(const TMccVideoSinkSetting& aSettings)
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::CreateWindowingElementsL" )
        
    __ASSERT_ALWAYS( !iGc && !iDev && !iRw && !iRwGroup, User::Leave( KErrAlreadyExists ) );
        
    TInt groupId = iRwSession.GetFocusWindowGroup();
    iRwGroup = new (ELeave) RWindowGroup( iRwSession ); 
    
    User::LeaveIfError( iRwGroup->Construct( groupId, EFalse ) );   
    iRwGroup->SetName( KMccWindowGroupName );
    
    // TBD: use also iWindowOrdinalPriority
    iRwGroup->SetOrdinalPosition( aSettings.iWindowOrdinalPosition );
    iRwGroup->AutoForeground( EFalse );
       
    // Create screen device
    iDev = new (ELeave) CWsScreenDevice( iRwSession );
    User::LeaveIfError( iDev->Construct( aSettings.iDeviceIndex ) ); 
    
    // Create window
    iRw = new (ELeave) RWindow( iRwSession );

    User::LeaveIfError( iRw->Construct( *iRwGroup, (TUint32)iRw ) );
    
    iRw->SetPosition( aSettings.iLocation );
    iRw->SetSize( aSettings.iSize );
    
    iRw->SetOrdinalPosition( aSettings.iWindowOrdinalPosition );
      
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl: creating graphics context ..." )
    User::LeaveIfError( iDev->CreateContext( iGc ) );
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl: graphics context created!" )   
    
    iRw->Activate();
    
    iRwSession.Flush();
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::CreateWindowingElementsL, exit" )
    }
    
// End of file

