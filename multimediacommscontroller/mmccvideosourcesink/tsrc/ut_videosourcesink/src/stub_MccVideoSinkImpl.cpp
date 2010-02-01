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
#include "MccVideoSinkImpl.h"
#include "MmccInterfaceDef.h"
#include "MmccCodech263.h"
#include "MccInternalCodecs.h"
#include "MccVideoSourceSinkLogs.h"
#include "mccinternalevents.h"
#include "mmcccodecamr.h"
#include "mccvideosinkuser.h"

// ================= MEMBER FUNCTIONS =======================

#ifndef __WINS__  // hardware target
    _LIT8( KRtpDll, "\\system\\libs\\RtpWrapper.dll" );
#else             // emulator target
    _LIT8( KRtpDll, "RtpWrapper.dll" );
#endif

_LIT8( KSdpInboundSimpleStart,
"v=0;\r\n\
\r\n\
");

_LIT8( KSdpInboundSimpleMiddle,
"\r\n");

_LIT8( KSdpInboundSimpleAudio,
"m=audio 25001 RTP/AVP %d\r\n\
a=rtpmap:%d %S/8000\r\n\
a=ptime:%d\r\n\
a=maxptime:%d\r\n\
a=fmtp:%d octet-align=%d; mode-set=%S");

_LIT8( KSdpInboundSimpleVideo,
"m=video 25002 RTP/AVP %d\r\n\
a=rtpmap:%d %S/90000\r\n\
a=fmtp:%d profile=0; level=%d");

#define MCC_BITRATE_IN_MASK( bitratemask, bitrate ) bitrate == ( bitratemask & bitrate )

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMccVideoSinkImpl::CMccVideoSinkImpl( TUint32 aEndpointId  )
: iEndpointId ( aEndpointId )
    {
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::ConstructL( const TMccVideoSinkSetting& aSettings )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::ConstructL" ) 
    
    iSettings = aSettings;

    iSdp = HBufC8::NewL( ( KSdpInboundSimpleStart().Length() + 
                           KSdpInboundSimpleMiddle().Length() + 
                           KSdpInboundSimpleVideo().Length() +
                           KSdpInboundSimpleAudio().Length() ) * 2 );
   
    User::LeaveIfError( iRwSession.Connect() );
	
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::ConstructL connected to ws" )
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl window pos:", iSettings.iLocation.iX, " ", iSettings.iLocation.iY ) 
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl window size:", iSettings.iSize.iHeight, " ", iSettings.iSize.iWidth ) 
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl window ordinal pos:", iSettings.iWindowOrdinalPosition )
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl window ordinal priority:", iSettings.iWindowOrdinalPriority )
    
	// Create window group
	TInt groupId = iRwSession.GetFocusWindowGroup();
    iRwGroup = new (ELeave) RWindowGroup( iRwSession ) ;	
	TInt handle = iRwSession.GetWindowGroupHandle( groupId );
    
	User::LeaveIfError( iRwGroup->Construct( groupId, EFalse ) );	
    
    // TBD: use also iWindowOrdinalPriority
    iRwGroup->SetOrdinalPosition( iSettings.iWindowOrdinalPosition );
         
    // Create screen device
	iDev = new (ELeave) CWsScreenDevice( iRwSession );
	User::LeaveIfError( iDev->Construct( iSettings.iDeviceIndex ) ); 
	
	// Create window
	iRw = new (ELeave) RWindow( iRwSession );
    
    User::LeaveIfError( iRw->Construct( *iRwGroup, (TUint32) this + 1 ) );

    iRw->SetPosition( iSettings.iLocation );
	iRw->SetBackgroundColor( KRgbBlack );
    iRw->SetSize( iSettings.iSize );    
    iRw->SetOrdinalPosition( iSettings.iWindowOrdinalPosition );
    iRw->Activate();
    iRwSession.Flush();
    
    TRect area = ResolveVideoPlayerDrawArea( iSettings );

    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl rect x1:", area.iTl.iX, " y1 ", area.iTl.iY ) 
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl rect x2:", area.iBr.iX, " y2 ", area.iBr.iY ) 
      
    iVideoPlayer = CVideoPlayerUtility::NewL( 
        *this,
    	iSettings.iPriority.iPriority,
    	iSettings.iPriority.iPref, 
    	iRwSession, 
    	*iDev,
    	*iRw, 
    	area,
    	area );	
    	
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
    delete iSdp;
    if( iVideoPlayer )
		{
		iVideoPlayer->Stop();	
		iVideoPlayer->Close();	
		}
		
	delete iVideoPlayer;
	delete iDev;

    delete iRw;
    delete iRwGroup;	
	iRwSession.Close();
	
	iUsers.Reset();
	iUsers.Close();
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::~CMccVideoSinkImpl, exit" )
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::SinkThreadLogon()
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
	{
    TMccVideoSinkUser user( &aEventHandler, KNullUid, GenerateStreamId() );
	TInt err = iUsers.Append( user );
	return err;
	}
   
// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::SinkThreadLogoff()
// ---------------------------------------------------------------------------
//     
void CMccVideoSinkImpl::SinkThreadLogoff()
	{
    iCurrentUser = 0;	
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
// void CMccVideoSinkImpl::OpenL()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::OpenL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::OpenL" )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::Prepare()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::Prepare()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Prepare" )

    }

// ---------------------------------------------------------------------------
// TInt CMccVideoSinkImpl::RemoteSetup()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::RemoteSetupL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::RemoteSetup" )
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::StartL()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::StartL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::StartL" )

    if ( iVideoPlayer && iRw ) 
	    {
        iRw->SetOrdinalPosition( 0 );
        iRw->SetVisible( ETrue );
	    }
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::Stop()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::Stop()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Stop" )
    if ( iVideoPlayer ) 
	    {
        __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Stop, hiding window disabled" )
        /*iRwGroup.SetOrdinalPosition( -1 );	    
        iRw.SetOrdinalPosition( -1 );
        iRw.SetVisible( EFalse );
	    iRwSession.Flush();
	    */
	    }  
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Stop, exit" )
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::Pause()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::PauseL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::PauseL" )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::Close()
// 
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
// void CMccVideoSinkImpl::SetDisplayWindowL()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetDisplayWindowL( RWsSession& aWs, 
                                               CWsScreenDevice& aScreenDevice,
                                               RWindowBase& aWindow, 
                                               const TRect& aWindowRect, 
                                               const TRect& aClipRect )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetDisplayWindowL" ) 
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    iVideoPlayer->SetDisplayWindowL( aWs, aScreenDevice, aWindow, aWindowRect,
                                     aClipRect );
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::PriorityL()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::Priority( TInt& /*aPriority*/, 
                                       TMdaPriorityPreference& /*aPreference*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::PriorityL" ) 
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::SetPriority()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetPriority( TInt /*aPriority*/, 
                                          TMdaPriorityPreference /*aPreference*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetPriorityL" )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    }

// ---------------------------------------------------------------------------
// TInt CMccVideoSinkImpl::Volume()
// 
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::Volume() const 
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Volume" )
    return 0;
    }

// ---------------------------------------------------------------------------
// TInt CMccVideoSinkImpl::MaxVolume()
// 
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::MaxVolume() const 
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::MaxVolume" )
    return 0;
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::SetVolumeL()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetVolumeL( TInt /*aVolume*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetVolumeL" )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );

    }

// ---------------------------------------------------------------------------
// TInt CMccVideoSinkImpl::Balance()
// 
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::Balance() const
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::Balance" )
    return  0;
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::SetBalanceL()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::SetBalanceL( TInt /*aBalance*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::SetBalanceL" )
    __ASSERT_ALWAYS( iVideoPlayer, User::Leave( KErrArgument ) );
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::RegisterForVideoLoadingNotification()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::RegisterForVideoLoadingNotification(
                                     MVideoLoadingObserver& /*aCallback*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::\
RegisterForVideoLoadingNotification" )
    }

// ---------------------------------------------------------------------------
// TVersion CMccVideoSinkImpl::Version()
// 
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
    
 	// If sink is already opened/prepared, the sdp cannot be modified
 	__ASSERT_ALWAYS( iState == ENotSet, User::Leave( KErrInUse ) );
 	
    // Update payload type since it might not have been provided at
    // initialization phase
    iSettings.iVideoPayloadType = aVideoCodec.iPayloadType;
 	
    const TInt levelOne = 10;
    const TInt levelTwo = 45;
    const TInt maxBitRateOne = 64000;
    const TInt maxBitRateTwo = 128000;
    TInt level = levelOne;
    
    if ( aVideoCodec.iMaxBitrate <= maxBitRateOne )
	    {
	    level = levelOne;	
	    }
	if ( aVideoCodec.iMaxBitrate > maxBitRateOne && 
		 aVideoCodec.iMaxBitrate < maxBitRateTwo )  
		{
		level = levelTwo; 	
		}
		
	__V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::FormatVideoSDPInfoL, Level:", level )
	}
	
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::FormatAudioSDPInfoL
// ---------------------------------------------------------------------------
// 	
void CMccVideoSinkImpl::FormatAudioSDPInfoL( const TMccCodecInfo& aAudioCodec )
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::FormatAudioSDPInfoL" )
 	__ASSERT_ALWAYS( iSdp, User::Leave( KErrArgument ) );
 	
 	// If sink is already opened/prepared, the sdp cannot be modified
 	__ASSERT_ALWAYS( iState == ENotSet, User::Leave( KErrInUse ) );
  

    iSettings.iAudioPayloadType = aAudioCodec.iPayloadType;

	}
	
// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::MvloLoadingStarted() 
// 
// ---------------------------------------------------------------------------
//     
void CMccVideoSinkImpl::MvloLoadingStarted() 
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::MvloLoadingStarted" )
	}

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::MvloLoadingComplete()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvloLoadingComplete()
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::MvloLoadingComplete" ) 
	}
// void CMccVideoSinkImpl::MvpuoOpenComplete()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvpuoOpenComplete( 
#ifdef _DEBUG
TInt aError
#else
TInt /*aError*/
#endif
)
    {
#ifdef _DEBUG
    
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::MvpuoOpenComplete(), \
aError=", aError) 
#endif
    Prepare();
  }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::MvpuoPrepareComplete()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvpuoPrepareComplete( 
#ifdef _DEBUG
TInt aError
#else
TInt /*aError*/
#endif
)
    {
#ifdef _DEBUG
    
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::MvpuoPrepareComplete(), \
aError=", aError)
#endif
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::MvpuoFrameReady()
// 
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
// void CMccVideoSinkImpl::MvpuoPlayComplete()
// 
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::MvpuoPlayComplete( 
#ifdef _DEBUG
TInt aError
#else
TInt /*aError*/
#endif 
)
    {
#ifdef _DEBUG    
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkImpl::MvpuoPlayComplete(), \
aError=", aError)
#endif
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::MvpuoEvent()
// 
// ---------------------------------------------------------------------------
//	
void CMccVideoSinkImpl::MvpuoEvent( const TMMFEvent& aEvent )
    {
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSinkImpl::MvpuoEvent(), \
aEvent.iEventType=", (TInt)aEvent.iEventType.iUid, "aEvent.iErrorCode", 
    aEvent.iErrorCode)
    }

// ---------------------------------------------------------------------------
// void CMccVideoSinkImpl::EmptyBufferL
// ---------------------------------------------------------------------------
//     
void CMccVideoSinkImpl::EmptyBufferL( CMMFBuffer* /*aBuffer*/, 
		                          MDataSource* /*aSupplier*/,
		                          TMediaId /*aMediaId*/,
		                          TRtpRecvHeader& /*aHeaderInfo*/ )	
	{
	__V_SOURCESINK_CONTROLL( "CMccVideoSink::EmptyBufferL" )
	}

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::RestorePacketSupply() 
// ---------------------------------------------------------------------------
//	
void CMccVideoSinkImpl::RestorePacketSupply( TUint /*aStreamId*/ )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::RestorePacketSupply" )
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::RestorePacketSupply, exit" )	
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSinkImpl::SetVideoSinkState
// -----------------------------------------------------------------------------
//    
TBool CMccVideoSinkImpl::SetVideoSinkState( TMccVideoSinkState aState )
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccMultiplexer::SetVideoSinkState, state:", aState )
    
    // If state of the multiplexed source is already what is tried to be set,
    // multiplexer generates the corresponding completion event
    TBool controlSink( iState != aState );
    
    TMccEventType eventType = KMccEventNone;
    
    switch ( aState )
        {
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
            if ( iState == EPaused )
                {
                eventType = KMccStreamResumed;
                }
            else
                {
                eventType = KMccStreamStarted;
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
                                 iUsers[ i ].iAsyncEventHandler, 
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
    TMccVideoSinkUser entry( 0, aMediaId.iMediaType, 0 );
    
    return FindStreamIdForVideoSinkUser( entry, aStreamId );
    }

// -----------------------------------------------------------------------------
// CMccVideoSinkImpl::ResolveStreamId()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::ResolveStreamId( 
    MAsyncEventHandler* aUser, 
    TUint& aStreamId )
    {
    TMccVideoSinkUser entry( aUser, KNullUid, 0 );
    
    return FindStreamIdForVideoSinkUser( entry, aStreamId );
    }   
    
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::GenerateStreamId
// ---------------------------------------------------------------------------
//
TUint CMccVideoSinkImpl::GenerateStreamId()
    {
    return ++iHelixStreamId;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::FindStreamIdForVideoSinkUser
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkImpl::FindStreamIdForVideoSinkUser( 
    TMccVideoSinkUser& aEntry,
    TUint& aStreamId )
    {
    TInt err( KErrNotFound );

    TIdentityRelation<TMccVideoSinkUser> comparison( VideoSinkUserMatch );
    TInt index = iUsers.Find( aEntry, comparison );
    if ( index != KErrNotFound )
        {
        aStreamId = iUsers[ index ].iStreamId;
        err = KErrNone;
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::FindVideoSinkUserEntryForCurrentL
// ---------------------------------------------------------------------------
//
TMccVideoSinkUser* CMccVideoSinkImpl::FindVideoSinkUserEntryForCurrent()
    {
    TMccVideoSinkUser entry( iCurrentUser, KNullUid, 0 );
    TIdentityRelation<TMccVideoSinkUser> comparison( VideoSinkUserMatch );
    TInt index = iUsers.Find( entry, comparison );
    if ( index != KErrNotFound )
        {
        return &iUsers[ index ];
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::RemoveVideoSinkUser
// ---------------------------------------------------------------------------
//
void CMccVideoSinkImpl::RemoveVideoSinkUser( TUint aStreamId )
    {
    TMccVideoSinkUser entry( 0, KNullUid, aStreamId );
    TIdentityRelation<TMccVideoSinkUser> comparison( VideoSinkUserMatch );
    TInt index = iUsers.Find( entry, comparison );
    if ( index != KErrNotFound )
        {
        iUsers.Remove( index );
        }
    }
        
// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::VideoSinkUserMatch
// ---------------------------------------------------------------------------
//   
TBool CMccVideoSinkImpl::VideoSinkUserMatch( 
    const TMccVideoSinkUser& aUser1, 
    const TMccVideoSinkUser& aUser2 )
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
// CMccVideoSinkImpl::GetCurrentSettings() 
// ---------------------------------------------------------------------------
//        
void CMccVideoSinkImpl::GetCurrentSettings( TMccVideoSinkSetting& aSettings ) const
    {
    aSettings = iSettings;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::UpdateSettingsL() 
// ---------------------------------------------------------------------------
//	
void CMccVideoSinkImpl::UpdateSettingsL( 
    const TMccVideoSinkSetting& aSettings, 
    TBool /*aForceUpdate*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::UpdateSettingsL" )
  
    // Store new settings
    iSettings = aSettings;
    
    __V_SOURCESINK_CONTROLL( "CMccVideoSinkImpl::UpdateSettingsL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkImpl::ResolveVideoPlayerDrawArea
// ---------------------------------------------------------------------------
//    
TRect CMccVideoSinkImpl::ResolveVideoPlayerDrawArea( 
    const TMccVideoSinkSetting& aSettings )
    {
    TPoint location2( aSettings.iLocation.iX + aSettings.iSize.iWidth, 
                      aSettings.iLocation.iY + aSettings.iSize.iHeight );
    return TRect( aSettings.iLocation, location2 );
    }

