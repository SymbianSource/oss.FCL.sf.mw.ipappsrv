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
#include "mccvideojitterbuffer.h"
#include "mccvideosourcesinklogs.h"
#include "mccdef.h"
#include <CXPSPacketSink.h>

// CONSTANTS
const TUint KMccMilliSecondInMicroSecs = 1000;
const TUint KMccMinInactivityTimeoutInMicroSecs = 1000000; // 1 sec

#define MCC_CONVERT_TO_90KHZ_CLOCK( valInMicroSecs ) ( valInMicroSecs / 100 * 9 )

// ---------------------------------------------------------------------------
// CMccVideoPacket::NewLC
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer::CMccVideoPacket* CMccVideoJitterBuffer::CMccVideoPacket::NewLC( 
    TUint aStreamId, 
    const TRtpRecvHeader& aHeaderInfo, 
    const TDesC8& aPayloadData,
    TBool aImportantData )
    {
    CMccVideoPacket* self = 
        new ( ELeave ) CMccVideoPacket( aStreamId, aHeaderInfo, aImportantData );
    CleanupStack::PushL( self );
    self->ConstructL( aPayloadData );
    return self;
    }

// ---------------------------------------------------------------------------
// CMccVideoPacket::~CMccVideoPacket
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer::CMccVideoPacket::~CMccVideoPacket()
    {
    delete iPayloadData;
    }

// ---------------------------------------------------------------------------
// CMccVideoPacket::StreamId
// ---------------------------------------------------------------------------
//
TUint CMccVideoJitterBuffer::CMccVideoPacket::StreamId()
    {
    return iStreamId;
    }

// ---------------------------------------------------------------------------
// CMccVideoPacket::HeaderInfo
// ---------------------------------------------------------------------------
//
TRtpRecvHeader& CMccVideoJitterBuffer::CMccVideoPacket::HeaderInfo()
    {
    return iHeaderInfo;
    }

// ---------------------------------------------------------------------------
// CMccVideoPacket::PayloadData
// ---------------------------------------------------------------------------
//    
const TDesC8& CMccVideoJitterBuffer::CMccVideoPacket::PayloadData()
    {
    return *iPayloadData;
    }

// ---------------------------------------------------------------------------
// CMccVideoPacket::CMccVideoPacket
// ---------------------------------------------------------------------------
//    
CMccVideoJitterBuffer::CMccVideoPacket::CMccVideoPacket( 
    TUint aStreamId, 
    const TRtpRecvHeader& aHeaderInfo,
    TBool aImportantData ) :
    iStreamId( aStreamId ),
    iHeaderInfo( aHeaderInfo ),
    iImportantData( aImportantData )
    {
    }
 
// ---------------------------------------------------------------------------
// CMccVideoPacket::ConstructL
// ---------------------------------------------------------------------------
//           
void CMccVideoJitterBuffer::CMccVideoPacket::ConstructL( const TDesC8& aPayloadData )
    {
    iPayloadData = aPayloadData.AllocL();
    }


// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::InsertInSeqNumOrder
// ---------------------------------------------------------------------------
//
TInt CMccVideoJitterBuffer::CMccVideoPacket::InsertInSeqNumOrder( 
    const CMccVideoPacket& aPacket1, const CMccVideoPacket& aPacket2 )
    {
    if ( aPacket1.iHeaderInfo.iSeqNum == aPacket2.iHeaderInfo.iSeqNum )
        {
        return 0;
        }
    return ( aPacket1.iHeaderInfo.iSeqNum < aPacket2.iHeaderInfo.iSeqNum ) ? -1 : 1;
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::DroppingAllowed
// ---------------------------------------------------------------------------
//    
TBool CMccVideoJitterBuffer::CMccVideoPacket::DroppingAllowed() const
    {
    return ( !iImportantData );
    }
    
// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::CMccVideoJitterBuffer
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer::CMccVideoJitterBuffer( 
    MMccVideoJitterBufferObserver& aObserver, 
    CXPSPacketSink& aPacketSink,
    TUint aInactivityTimeoutInMs ) :
    CTimer( CActive::EPriorityStandard ),
    iObserver( aObserver ),
    iPacketSink( aPacketSink ),
    iInactivityTimeoutInMicroSecs( aInactivityTimeoutInMs * KMccMilliSecondInMicroSecs ),
    iPreviousTime( 0 )
    {
    CActiveScheduler::Add( this );
    
    iInactivityTimeoutInMicroSecs = aInactivityTimeoutInMs * KMccMilliSecondInMicroSecs;
    
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoJitterBuffer::CMccVideoJitterBuffer, inactivity timeout:",
        iInactivityTimeoutInMicroSecs ) 
    
    if ( iInactivityTimeoutInMicroSecs < KMccMinInactivityTimeoutInMicroSecs )
        {
        iInactivityTimeoutInMicroSecs = KMccMinInactivityTimeoutInMicroSecs;
        __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoJitterBuffer::CMccVideoJitterBuffer, inactivity timeout adjusted:",
        iInactivityTimeoutInMicroSecs ) 
        }
    }


// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::ConstructL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::ConstructL" ) 
    
    CTimer::ConstructL();
    
    ConfigureL( KMccJitterBufferDefaultLowLimit, 
                KMccJitterBufferDefaultHighLimit, 
                KMccJitterBufferDefaultPlayThreshold,
                KMccJitterBufferDefaultMaxSize,
                KMccDefaultVideoFrameRate );
    
    __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::ConstructL, exit" ) 
    }


// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer* CMccVideoJitterBuffer::NewL( 
    MMccVideoJitterBufferObserver& aObserver, 
    CXPSPacketSink& aPacketSink,
    TUint aInactivityTimeoutInMs )
    {
    CMccVideoJitterBuffer* self = CMccVideoJitterBuffer::NewLC( 
        aObserver, aPacketSink, aInactivityTimeoutInMs );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer* CMccVideoJitterBuffer::NewLC( 
    MMccVideoJitterBufferObserver& aObserver, 
    CXPSPacketSink& aPacketSink,
    TUint aInactivityTimeoutInMs )
    {
    CMccVideoJitterBuffer* self = new( ELeave ) CMccVideoJitterBuffer( 
        aObserver, aPacketSink, aInactivityTimeoutInMs );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::~CMccVideoJitterBuffer
// Destructor.
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer::~CMccVideoJitterBuffer()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::~CMccVideoJitterBuffer" ) 
    
    iQueue.ResetAndDestroy();
    
    __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::~CMccVideoJitterBuffer, exit" ) 
    }


// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::ConfigureL
// Jitterbuffer gathers data until defined play threshold is exceeded.
// After that, packets are posted with certain interval towards Helix. In case
// buffer is close to running out of data (low limit reached), posting interval
// is increased in order to increase data amount in buffer. Ideal situation
// is that buffer has all the time some data so that playback can be kept
// smooth. If high limit is reached, posting interval is tightened to avoid
// max limit reach. If max limit is anyhow reached, jitterbuffer makes room
// by just pushing some amount of oldest data towards Helix. 
//
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::ConfigureL( 
    TUint aLowLimitMs, 
    TUint aHighLimitMs, 
    TUint aPlayThresholdMs, 
    TUint aMaxSizeMs, 
    TUint aFrameRateFps )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoJitterBuffer::ConfigureL, framerate:", aFrameRateFps ) 
    __V_SOURCESINK_CONTROLL_INT1( "low limit:", aLowLimitMs )  
    __V_SOURCESINK_CONTROLL_INT1( "high limit:", aHighLimitMs ) 
    __V_SOURCESINK_CONTROLL_INT1( "play threshold:", aPlayThresholdMs ) 
    __V_SOURCESINK_CONTROLL_INT1( "max size:", aMaxSizeMs ) 
    
    __ASSERT_ALWAYS( aFrameRateFps > 0, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aLowLimitMs < aPlayThresholdMs, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aPlayThresholdMs < aHighLimitMs, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aHighLimitMs < aMaxSizeMs, User::Leave( KErrArgument ) );
         
    iLowLimitMicroSecs = aLowLimitMs * KMccMilliSecondInMicroSecs;
    iHighLimitMicroSecs = aHighLimitMs * KMccMilliSecondInMicroSecs;
    iPlayThresholdMicroSecs = aPlayThresholdMs * KMccMilliSecondInMicroSecs;
    iMaxSizeMicroSecs = aMaxSizeMs * KMccMilliSecondInMicroSecs;
    iFrameRate = aFrameRateFps;
    
    const TInt KMccSecondInMicroSecs = 1000000;
    
    const TReal KMccLowLimitInterval = 1.4;
    const TReal KMccHighLimitInterval = 0.7;
    iNormalInterval = KMccSecondInMicroSecs / iFrameRate;
    iLowLimitInterval = (TInt)( iNormalInterval.Int() * KMccLowLimitInterval );
    iHighLimitInterval = (TInt)( iNormalInterval.Int() * KMccHighLimitInterval );
    
    iCurrentInterval = iNormalInterval;
    
    __V_SOURCESINK_CONTROLL_INT1( "normal interval:", iNormalInterval.Int() )  
    __V_SOURCESINK_CONTROLL_INT1( "low limit interval:", iLowLimitInterval.Int() ) 
    __V_SOURCESINK_CONTROLL_INT1( "high limit interval:", iHighLimitInterval.Int() ) 
    }
    
// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::EnqueueL
// Packets are posted towards Helix in receiving order, queuing could be
// seq num based but out of order packets are rare.
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer::TMccPacketBufferingStatus CMccVideoJitterBuffer::EnqueueL( 
    TUint aStreamId, 
    const TRtpRecvHeader& aHeaderInfo, 
    const TDesC8& aPayloadData,
    TBool aImportantData )
    { 
    CMccVideoPacket* packet = 
        CMccVideoPacket::NewLC( aStreamId, aHeaderInfo, aPayloadData, aImportantData );
    TLinearOrder<CMccVideoPacket> ordering( CMccVideoPacket::InsertInSeqNumOrder );
    iQueue.InsertInOrderAllowRepeatsL( packet, ordering );
    CleanupStack::Pop( packet );
    
    UpdateFrameCount( *packet, ETrue );
    
    if ( PlayThresholdExceeded() )
        {
        if ( MaxSizeExceeded() )
            {
            MakeRoom();
            }

        Start();
        }
        
    return PlayingStatus();
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::Play
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::Play()
    {
    if ( PlayThresholdExceeded() )
        {
        Start();
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::Pause
// ---------------------------------------------------------------------------
//    
void CMccVideoJitterBuffer::Pause()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::PlayThresholdInMs
// ---------------------------------------------------------------------------
//
TInt CMccVideoJitterBuffer::PlayThresholdInMs()
    {
    const TInt KMccMilliSecondInMicroSecs = 1000;
    return ( iPlayThresholdMicroSecs / KMccMilliSecondInMicroSecs );
    }
    
// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::Start
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::Start()
    {
    TTimeIntervalMicroSeconds32 newCurrentInterval = CheckCurrentInterval();
    if ( newCurrentInterval != iCurrentInterval )
        {
        __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoJitterBuffer::Start, new interval", newCurrentInterval.Int() ) 
        
        Cancel();
        }
            
    if ( !IsActive() )
        {
        iCurrentInterval = newCurrentInterval;
	    CTimer::HighRes( iCurrentInterval );
        }
    }


// ---------------------------------------------------------------------------
// From class CActive.
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::RunL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::RunL" ) 
    
    if ( PostFirstFrame() )
        {
        Start();
        }
        
    __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::RunL, exit" ) 
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::PlayThresholdExceeded
// ---------------------------------------------------------------------------
//    
TBool CMccVideoJitterBuffer::PlayThresholdExceeded()
    {
    if ( !iPlayThresholdExeeced && 
       ( iFrameCount * iNormalInterval.Int() >= iPlayThresholdMicroSecs ) )
        {
        iPlayThresholdExeeced = ETrue;
        }
    return iPlayThresholdExeeced;
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::CheckCurrentInterval
// ---------------------------------------------------------------------------
// 
TTimeIntervalMicroSeconds32 CMccVideoJitterBuffer::CheckCurrentInterval()
    {
    TTimeIntervalMicroSeconds32 interval( iNormalInterval );
    TUint32 dataAmountInMicroSecs( iFrameCount * iNormalInterval.Int() );
    if ( iQueue.Count() == 0 || dataAmountInMicroSecs < iLowLimitMicroSecs )
        {
        interval = iLowLimitInterval;
        }
    else if ( dataAmountInMicroSecs > iHighLimitMicroSecs )
        {
        interval = iHighLimitInterval;
        }
    else if ( iCurrentInterval == iLowLimitInterval && 
              dataAmountInMicroSecs < iPlayThresholdMicroSecs )
        {
        // If on low interval, normalize only once at normal level, otherwise
        // low limit might be reached shortly again.
        interval = iLowLimitInterval;
        }
    else
        {
        }
    return interval;
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::UpdateFrameCount
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::UpdateFrameCount( CMccVideoPacket& aPacket, TBool aIsAdded )
    {
    if ( aPacket.HeaderInfo().iMarker != 0 )
        {
        if ( aIsAdded )
            {
            iFrameCount++;
            }
        else if ( iFrameCount > 0 )
            {
            iFrameCount--;
            }
        else
            {
            // NOP
            }
            
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoJitterBuffer::UpdateFrameCount, count", iFrameCount ) 
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::PostFirstFrame
// ---------------------------------------------------------------------------
//
TBool CMccVideoJitterBuffer::PostFirstFrame()
    {
    TBool posted( EFalse );
    TInt frameEndIndex( KErrNotFound );
    TUint32 frameTimeStamp( 0 );
    for ( TInt i = 0; i < iQueue.Count() && !posted; i++ )
        {
        CMccVideoPacket* packet = iQueue[ i ]; 
        
        if ( frameEndIndex != KErrNotFound )
            {
            TRtpRecvHeader& recvHeader = packet->HeaderInfo();
            if ( recvHeader.iTimestamp != frameTimeStamp && packet->DroppingAllowed() )
                {
                // Timestamp mismatch, packet does not actually belong
                // to this frame, drop it silently
                __V_SOURCESINK_CONTROLL( 
                "CMccVideoJitterBuffer::PostBuffer, packet dropped due incorrect ts" ) 
                }
            else
                {
                // Always modify timestamp to zero which causes that Helix
                // lets packet go straight away to decoder.
                DoRtpHeaderModify( recvHeader );
         
                TInt ret = iPacketSink.Enqueue( 
                    packet->StreamId(), recvHeader, packet->PayloadData() );
                
                if ( ret != KErrNone )
                    {
                    iObserver.ErrorOccured( ret );
                    }
                else
                    {
                    posted = ( frameEndIndex == i );
                    }
                }
            }
        else if ( packet->HeaderInfo().iMarker != 0 )
            {
            // Full frame in queue, post all packets belonging to the frame
            frameEndIndex = i;
            frameTimeStamp = packet->HeaderInfo().iTimestamp;
            i = -1; // Start again from beginning
            }
        else
            {
            }
        }
    
    // Delete all posted packets
    for ( TInt j = frameEndIndex; j >= 0; j-- )
        {
        CMccVideoPacket* packet = iQueue[ j ]; 
        UpdateFrameCount( *packet, EFalse );
        delete packet;
        iQueue.Remove( j );
        }
        
    return posted;
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::MakeRoom
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::MakeRoom()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::MakeRoom" ) 
    
    // Make room by pushing some amount from beginning of the queue
    // towards helix.
    TBool post = PostFirstFrame();
    while ( post )
        {
        __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::MakeRoom, post" ) 
        
        TTimeIntervalMicroSeconds32 interval = CheckCurrentInterval();
        if ( interval <= iHighLimitInterval )
            {
            // Data amount still exceeds high limit
            post = PostFirstFrame(); 
            }
        else
            {
            post = EFalse;
            }
        }
        
    __V_SOURCESINK_CONTROLL( "CMccVideoJitterBuffer::MakeRoom, exit" ) 
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::MaxSizeExceeded
// ---------------------------------------------------------------------------
//
TBool CMccVideoJitterBuffer::MaxSizeExceeded()
    {
    TUint32 dataAmountInMicroSecs( iFrameCount * iNormalInterval.Int() );
    return ( dataAmountInMicroSecs >= iMaxSizeMicroSecs );
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::CheckPostingInactivity
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::CheckPostingInactivity( 
    TInt64 aTimeFromPreviousFrame )
    {
    if ( aTimeFromPreviousFrame > (TInt64)iInactivityTimeoutInMicroSecs )
        {
        // If frames haven't been posted within inactivity time, next packet
        // will cause again "playing started" status report
        iPlaybackStarted = EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::PlayingStatus
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer::TMccPacketBufferingStatus CMccVideoJitterBuffer::PlayingStatus()
    {
    TMccPacketBufferingStatus bufferingStatus( EBuffering );
    if ( PlayThresholdExceeded() )
        {
        if ( iPlaybackStarted )
            {
            bufferingStatus = EPlaying;
            }
        else
            {
            bufferingStatus = EPlayingStarted;
            iPlaybackStarted = ETrue;
            }
        }
    return bufferingStatus;
    }

// ---------------------------------------------------------------------------
// CMccVideoJitterBuffer::DoRtpHeaderModify
// Always overwrite timestamps by using own real time clock. There's some 
// implementations which do not use RTP timestamps correctly so this
// approach is for better interoperability. Also hide seqnum gaps from Helix. 
// ---------------------------------------------------------------------------
//
void CMccVideoJitterBuffer::DoRtpHeaderModify( TRtpRecvHeader& aRecvHeader )
    {
    if ( iPreviousTime.Int64() == 0 )
        {
        iPreviousTime.HomeTime();
        iCurrentRealtimeTimestamp = aRecvHeader.iTimestamp;
        }
    else if ( aRecvHeader.iTimestamp != iPreviousReceivedTimestamp )
        {
        TTime currentTime;
        currentTime.HomeTime();
        TTimeIntervalMicroSeconds timeFromPreviousFrame = 
            currentTime.MicroSecondsFrom( iPreviousTime );
        iCurrentRealtimeTimestamp += 
            MCC_CONVERT_TO_90KHZ_CLOCK( timeFromPreviousFrame.Int64() );
        
        CheckPostingInactivity( timeFromPreviousFrame.Int64() );
        
        iPreviousTime = currentTime;
        }
    else
        {
        // NOP
        }
    
    __V_SOURCESINK_CONTROLL_INT2( 
        "Timestamp mod, orig:", aRecvHeader.iTimestamp, " mod:", iCurrentRealtimeTimestamp ) 
        
    iPreviousReceivedTimestamp = aRecvHeader.iTimestamp;    
    aRecvHeader.iTimestamp = iCurrentRealtimeTimestamp;
    
    if ( !iSeqNum )
        {
        iSeqNum = aRecvHeader.iSeqNum;
        }
    else
        {
        aRecvHeader.iSeqNum = ++iSeqNum;
        }
    }
                        
// End of file

