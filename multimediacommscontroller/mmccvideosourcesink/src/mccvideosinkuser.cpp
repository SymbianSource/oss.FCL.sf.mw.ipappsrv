/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mccvideosinkuser.h"
#include "mccvideosourcesinklogs.h"
#include "mccdef.h"
#include "mccinternaldef.h"
#include <CXPSPacketSink.h>

// CONSTANTS
const TUint32 KMccModifyMonitoringShortPeriodMicrosecs = 2000000;
const TUint32 KMccModifyMonitoringLongPeriodMicrosecs = 10000000;
const TUint32 KMccMaxDenyPeriodMicrosecs = 1000000;
const TReal KMccMonitoringShortPeriodTimestampIncreaseFactorDown = 0.6;
const TReal KMccMonitoringShortPeriodTimestampIncreaseFactorUp = 1.4;
const TReal KMccMonitoringLongPeriodTimestampIncreaseFactorDown = 0.8;
const TReal KMccMonitoringLongPeriodTimestampIncreaseFactorUp = 1.2;

const TInt KMccJitterBufferThresholdPacketTime = 20;

// Minimum times for preroll
const TInt KMccOneMediaMinPreroll = 1000;
const TInt KMccSeveralMediaMinPreroll = 2000;

// MACROS
#define MCC_CONVERT_TO_90KHZ_CLOCK( valInMicroSecs ) ( valInMicroSecs / 100 * 9 )

// ---------------------------------------------------------------------------
// TMccTimeStampEntry::TMccTimeStampEntry
// ---------------------------------------------------------------------------
//
TMccTimeStampEntry::TMccTimeStampEntry() : 
    iTime( 0 ), iSeq( 0 )
    {
    }

// ---------------------------------------------------------------------------
// TMccTimeStampEntry::TMccTimeStampEntry
// ---------------------------------------------------------------------------
//    
TMccTimeStampEntry::TMccTimeStampEntry( TInt64 aTime, TUint16 aSeq ) :
    iTime( aTime ), iSeq( aSeq )
    {
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::NewL
// ---------------------------------------------------------------------------
//
CMccVideoSinkUser* CMccVideoSinkUser::NewL( 
    MAsyncEventHandler* aAsyncEventHandler, 
    TUid aMediaType,
    TUint aStreamId,
    TReal aFrameRate,
    CXPSPacketSink& aPacketSink )
    {
    CMccVideoSinkUser* self = 
        CMccVideoSinkUser::NewLC( aAsyncEventHandler,
                                  aMediaType,
                                  aStreamId,
                                  aFrameRate,
                                  aPacketSink );
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::NewLC
// ---------------------------------------------------------------------------
//
CMccVideoSinkUser* CMccVideoSinkUser::NewLC( 
    MAsyncEventHandler* aAsyncEventHandler, 
    TUid aMediaType,
    TUint aStreamId,
    TReal aFrameRate,
    CXPSPacketSink& aPacketSink )
    {
    CMccVideoSinkUser* self = 
        new ( ELeave ) CMccVideoSinkUser( 
            aAsyncEventHandler, aMediaType, aStreamId, aFrameRate, aPacketSink );
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::~CMccVideoSinkUser
// ---------------------------------------------------------------------------
//        
CMccVideoSinkUser::~CMccVideoSinkUser()
    {
    delete iJitterBuf;
    }
        
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::CMccVideoSinkUser
// ---------------------------------------------------------------------------
//
CMccVideoSinkUser::CMccVideoSinkUser( 
    MAsyncEventHandler* aAsyncEventHandler, 
    TUid aMediaType,
    TUint aStreamId,
    TReal aFrameRate,
    CXPSPacketSink& aPacketSink ) :
    iAsyncEventHandler( aAsyncEventHandler ),
    iMediaType( aMediaType ),
    iStreamId( aStreamId ),
    iStartedOnce( EFalse ),
    iFrameRate( aFrameRate ),
    iPacketSink( aPacketSink ),
    iPacketOverflowState( ENormal ),
    iNumTimeStamps( 0 ),
    iAverageTimeStampDifference( 0 ),
    iResetNeeded( EFalse ),
    iModifyMode( EModifyInit ),
    iFirstModifyCheck( 0 ),
    iFirstModifyCheckTimestamp( 0 ),
    iFirstTimestamp( 0 ),
    iAllowFrame( ETrue ),
    iDenyFramesStarted( 0 ),
    iMultipleMediaTypes( EFalse ),
    iPreviousPacketOriginalTimeStamp( KMaxTUint32 ),
    iPreviousPacketModifiedTimeStamp( 0 ),
    iPreviousPacketTime( 0 ),
    iCurrentMonitoringPeriod( KMccModifyMonitoringShortPeriodMicrosecs ),
    iTimestampIncreaseFactorDown( KMccMonitoringShortPeriodTimestampIncreaseFactorDown ),
    iTimestampIncreaseFactorUp( KMccMonitoringShortPeriodTimestampIncreaseFactorUp )
    {
    } 

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::EnqueueL
// ---------------------------------------------------------------------------
//
CMccVideoJitterBuffer::TMccPacketBufferingStatus CMccVideoSinkUser::EnqueueL( 
    const TRtpRecvHeader& aHeaderInfo, const TDesC8& aPayloadData )
    {
    CMccVideoJitterBuffer::TMccPacketBufferingStatus 
        bufferingStatus( CMccVideoJitterBuffer::EPlaying );

    if ( aPayloadData.Length() > GetPayloadSize() )
        {
        // Too big buffer causes playback to stop at Helix, drop the packet
        // silently
        __V_SOURCESINK_CONTROLL( 
            "CMccVideoSinkUser::EnqueueL, Too big packet dropped" ) 
        }
    else if ( iJitterBuf )
        {
        TBool importantPacket = CodecSpecificDataHandling( aPayloadData );
        bufferingStatus = iJitterBuf->EnqueueL( iStreamId, 
                                                aHeaderInfo, 
                                                aPayloadData, 
                                                importantPacket );
        }
    else
        {
        TInt err = iPacketSink.Enqueue( iStreamId, aHeaderInfo, aPayloadData );
        if ( err )
            {
            CheckErrorL( err );
            }
        }
    return bufferingStatus;
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::Set
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::Set( 
    MAsyncEventHandler* aAsyncEventHandler, 
    TUid aMediaType,
    TUint aStreamId )
    {
    iAsyncEventHandler = aAsyncEventHandler;
    iMediaType = aMediaType;
    iStreamId = aStreamId;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::SetFrameRateL
// ---------------------------------------------------------------------------
//    
void CMccVideoSinkUser::SetFrameRateL( TReal aFrameRate )
    {
    if ( aFrameRate != iFrameRate )
        {
        if ( iJitterBuf )
            {
            __V_SOURCESINK_CONTROLL( 
            "CMccVideoSinkUser::SetFrameRateL, configure jitterbuffer" ) 
            
            iJitterBuf->ConfigureL( KMccJitterBufferDefaultLowLimit, 
                                    KMccJitterBufferDefaultHighLimit, 
                                    KMccJitterBufferDefaultPlayThreshold,
                                    KMccJitterBufferDefaultMaxSize,
                                    aFrameRate );
            }
        
        iFrameRate = aFrameRate;
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::GetPreroll
// Helix does not restrict min preroll (buffering time) but it is safer to
// have some  restriction as otherwise continuous buffering may occur if
// Helix side buffering is used. If we are on real time mode, buffering
// is handled at MCC side (this is indicated with preroll value zero).
// Especially when two or more media types are used, data may not be received
// in sync and longer buffering guards against running out of data of certain
// media type.
// ---------------------------------------------------------------------------
//    
TInt CMccVideoSinkUser::GetPreroll()
    {
    TInt preroll = iCodecInfo.iJitterBufThreshold * KMccJitterBufferThresholdPacketTime;
    
    __V_SOURCESINK_CONTROLL_INT1( 
                "CMccVideoSinkUser::GetPreroll, preroll before adjust:", preroll )
                	
    if ( iMultipleMediaTypes || preroll > KMccOneMediaMinPreroll )
        {    
        TInt minPreroll = iMultipleMediaTypes ? KMccSeveralMediaMinPreroll : 
                                                KMccOneMediaMinPreroll;           
        if ( preroll < minPreroll )
            {
            preroll = minPreroll;
            }
        }
    else
        {
        // Real time mode enabled as client set jitter buffer threshold
        // value to small enough and only one media type is used.
        preroll = 0;
        }
        
    __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkUserImpl::GetPreroll, preroll:", preroll )   
            
    return preroll;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::GetActualPreroll
// In case of real-time mode, actual preroll value is defined by jitterbuffer
// ---------------------------------------------------------------------------
// 
TInt CMccVideoSinkUser::GetActualPreroll()
    {      
    TInt preroll( 0 );
    if ( iJitterBuf )
        {
        preroll = iJitterBuf->PlayThresholdInMs();
        }
    else
        {
        preroll = GetPreroll();
        }
        
     __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkUser::GetActualPreroll, preroll:", 
                                   preroll )
                
    return preroll;
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::Play
// ---------------------------------------------------------------------------
// 
void CMccVideoSinkUser::Play()
    {
    if ( iJitterBuf )
        {
        iJitterBuf->Play();
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::Pause
// ---------------------------------------------------------------------------
//         
void CMccVideoSinkUser::Pause()
    {
    if ( iJitterBuf )
        {
        iJitterBuf->Pause();
        }
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::IsQueueSizeDefined
// ---------------------------------------------------------------------------
//                           
TBool CMccVideoSinkUser::IsQueueSizeDefined() const
    {
    return ( iCodecInfo.iJitterBufBufferLength > 0 );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::QueueSize
// ---------------------------------------------------------------------------
//    
TUint CMccVideoSinkUser::QueueSize() const
    {
    TInt queueSize = 
        ( iCodecInfo.iJitterBufBufferLength < KMccMinXPSQueueSize ) ?
            KMccMinXPSQueueSize : iCodecInfo.iJitterBufBufferLength;

    return queueSize;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::PacketOverflowState
// ---------------------------------------------------------------------------
//
CMccVideoSinkUser::TMccPacketOverflowState 
    CMccVideoSinkUser::PacketOverflowState() const
    {
    return iPacketOverflowState;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::SetPacketOverflow
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::SetPacketOverflow( TMccPacketOverflowState aState )
    {
    iPacketOverflowState = aState;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::AddTimeStamp
// ---------------------------------------------------------------------------
//   
void CMccVideoSinkUser::AddTimeStamp( TInt64 aTimeStamp, TUint16 aSeq )
    {
    if ( iResetNeeded )
        {
        return;
        }
    
    UpdateFirstTimestamp( aTimeStamp );
                
    TInt lastTimeStamp = iNumTimeStamps - 1;
    if ( lastTimeStamp >= 0 && iTimeStamps[ lastTimeStamp ].iTime == aTimeStamp )
        {
        // Do not allow subsequent same timestamps, update sequence number
        iTimeStamps[ lastTimeStamp ].iSeq = aSeq;
        return;
        }
        
    if ( iNumTimeStamps >= KMccTimeStampArraySize )
        {
        RemoveFirstTimeStamp();
        }
    
    iNumTimeStamps++;
    TInt lastIndex = iNumTimeStamps - 1;
    iTimeStamps[ lastIndex ].iTime = aTimeStamp;
    iTimeStamps[ lastIndex ].iSeq = aSeq;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::RemoveFirstTimeStamp
// ---------------------------------------------------------------------------
//   
void CMccVideoSinkUser::RemoveFirstTimeStamp()
    {
    for ( TInt i = 1; i < iNumTimeStamps; i++ )
        {
        iTimeStamps[ i - 1 ] = iTimeStamps[ i ];
        }
    if ( iNumTimeStamps > 0 )
        {
        iNumTimeStamps--;
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::RemoveAllTimeStamps
// ---------------------------------------------------------------------------
//    
void CMccVideoSinkUser::RemoveAllTimeStamps()
    {
    iNumTimeStamps = 0;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::NumTimeStamps
// ---------------------------------------------------------------------------
//   
TInt CMccVideoSinkUser::NumTimeStamps() const
    {
    return iNumTimeStamps;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::Reset
// ---------------------------------------------------------------------------
//     
void CMccVideoSinkUser::Reset( TBool aFullReset )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkUser::Reset, fullreset:", aFullReset )
        
    if ( aFullReset )
        {
        SetPacketOverflow( ENormal );
        RemoveAllTimeStamps();
        iAverageTimeStampDifference = 0;
        iResetNeeded = EFalse;
    
        SetModifyingState( EModifyInit );
        iFirstModifyCheck = 0;
        iFirstModifyCheckTimestamp = 0;
        iFirstTimestamp = 0;
        
        iAllowFrame = ETrue;
        iDenyFramesStarted = TTime( 0 );
        
        iPreviousPacketOriginalTimeStamp = KMaxTUint32;
        iPreviousPacketModifiedTimeStamp = 0;
        iPreviousPacketTime = TTime( 0 );
        
        iCurrentMonitoringPeriod = KMccModifyMonitoringShortPeriodMicrosecs;
        
        iTimestampIncreaseFactorDown = 
            KMccMonitoringShortPeriodTimestampIncreaseFactorDown;
        iTimestampIncreaseFactorUp = 
            KMccMonitoringShortPeriodTimestampIncreaseFactorUp;
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::CalculateAverageTimeStampDifference
// ---------------------------------------------------------------------------
//   
void CMccVideoSinkUser::CalculateAverageTimeStampDifference()
    {
    CheckDenyFramesMaxPeriod();
    
    // Calculate difference only if there's enough timestamps and average
    // difference has not been yet calculated
    if ( !iResetNeeded &&
         !iAverageTimeStampDifference &&
         iNumTimeStamps == KMccTimeStampArraySize )
        {
        TInt lastTimeStamp( iNumTimeStamps - 1 );
        
        // Calculate first the average timestamp difference between last N packets
        TInt seqNumDif( 0 );
        for ( TInt i = lastTimeStamp; i > 0; i-- )
            {
            seqNumDif = iTimeStamps[ i ].iSeq - iTimeStamps[ i - 1 ].iSeq;
            seqNumDif = Abs( seqNumDif );
            seqNumDif = ( seqNumDif == 0 ) ? 1 : seqNumDif;
                
            iAverageTimeStampDifference += (TUint32)(
                ( iTimeStamps[ i ].iTime - iTimeStamps[ i - 1 ].iTime ) / seqNumDif );
            }
            
        iAverageTimeStampDifference = 
            iAverageTimeStampDifference / ( KMccTimeStampArraySize - 1 ); 

        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkUser::CalculateAverageTimeStampDifference, average dif:", 
            iAverageTimeStampDifference )
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::IsResetNeeded
// If there's big gap in timestamps, videoplayer needs to be resetted to
// succesfully continue playback. If modifying is in use, gaps are ignored.
// ---------------------------------------------------------------------------
//    
TBool CMccVideoSinkUser::IsResetNeeded( TRtpRecvHeader& aHeaderInfo )
    {
    if ( !iResetNeeded )
        {
        if ( RealTimeUser() )
            {
            iResetNeeded = IsResetNeededRealTimeMode( aHeaderInfo );
            }
        else
            {
            iResetNeeded = IsResetNeededNormalMode( aHeaderInfo );
            }
        }    
    return iResetNeeded;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::IsResetNeeded
// ---------------------------------------------------------------------------
// 
TBool CMccVideoSinkUser::IsResetNeeded() const
    {
    return iResetNeeded;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::IsModifyNeeded
// Check if other end is using video timestamps incorrectly and modify
// them to be as correct as possible as Helix cannot deal with incorrect
// timestamps. Modify check is not used in audio+video scenario as packet
// flow is not as stable in that case.
// ---------------------------------------------------------------------------
//
TBool CMccVideoSinkUser::IsModifyNeeded( TRtpRecvHeader& aHeaderInfo )
    {
    if ( iMultipleMediaTypes )
        {
        return EFalse;
        }
    
    if ( iModifyMode == EModifyInit )
        {
        DoModifyCheckInit( aHeaderInfo );
        }
    
    if ( MonitoringModify() )
        {
        DoModifyCheckMonitoring( aHeaderInfo );
        }
        
    return DoModify( aHeaderInfo );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::SetAllowFrame
// If some other user has resetted XPS because of timestamp skew, this
// user cannot pass media until it has also detected timestamp skew. 
// However, there's guard "timer" which stops denial if it takes too long
// (i.e. timestamp skew didn't occur/was not detected for some reason).
// This kind of error situtation can cause medias to go out of sync but
// it is anyway better than not being able to continue playback at all.
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::SetAllowFrame( TBool aAllowFrame )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkUser::SetAllowFrame, allow:", aAllowFrame )
            
    iAllowFrame = aAllowFrame;
    
    if ( !iAllowFrame )
        {
        iDenyFramesStarted.HomeTime();
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::AllowFrame
// ---------------------------------------------------------------------------
//            
TBool CMccVideoSinkUser::AllowFrame() const
    {
    return iAllowFrame;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::SetMultipleMediaTypesL
// ---------------------------------------------------------------------------
// 
void CMccVideoSinkUser::SetMultipleMediaTypesL( TBool aMultipleMediaTypes )
    {
    iMultipleMediaTypes = aMultipleMediaTypes;
    if ( RealTimeUser() && !iJitterBuf )
        {
        __V_SOURCESINK_CONTROLL( 
        "CMccVideoSinkUser::SetMultipleMediaTypesL, creating jitterbuffer" )
        
        iJitterBuf = CMccVideoJitterBuffer::NewL( 
            *this, iPacketSink, iCodecInfo.iJitterBufInactivityTimeOut );
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::SetCodecInfoL
// ---------------------------------------------------------------------------
// 
void CMccVideoSinkUser::SetCodecInfoL( const TMccCodecInfo& aCodecInfo )
    {
    iCodecInfo = aCodecInfo;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::CodecInfo
// ---------------------------------------------------------------------------
//         
TMccCodecInfo& CMccVideoSinkUser::CodecInfo()
    {
    return iCodecInfo;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::GetPayloadSize
// ---------------------------------------------------------------------------
//
TInt CMccVideoSinkUser::GetPayloadSize()
    {
    if ( !iPayloadSize )
        {
        TInt payloadSize( iCodecInfo.iFrameSize );
        __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkUser::GetPayloadSize, size:", payloadSize )
            
        if ( payloadSize < KMccVideoSinkMinPayloadBufSize )
            {
            payloadSize = KMccVideoSinkMinPayloadBufSize;
            __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkUser::GetPayloadSize, size modified:", payloadSize )
            }
        iPayloadSize = payloadSize;
        }
    return iPayloadSize;
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::ErrorOccured
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::ErrorOccured( TInt aError )
    {
    TRAP_IGNORE( CheckErrorL( aError ) )
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::DoModify
// ---------------------------------------------------------------------------
// 
TBool CMccVideoSinkUser::DoModify( TRtpRecvHeader& aHeaderInfo )
    {
    TBool modifyRequired( EFalse );
    
    if ( ModifyRequired() )
        {
        TUint32 timeStamp = aHeaderInfo.iTimestamp;
        
        TTime currentTime;
        currentTime.HomeTime();
            
        if ( timeStamp == iPreviousPacketOriginalTimeStamp )
            {
            // Frame is divided into several packets, packet time of
            // first packet in the frame is used until frame changes.
            timeStamp = iPreviousPacketModifiedTimeStamp;
            currentTime = iPreviousPacketTime;
            }
        else if ( iPreviousPacketTime.Int64() != 0 )
            {
            TTimeIntervalMicroSeconds interval = 
                currentTime.MicroSecondsFrom( iPreviousPacketTime );
            
            __V_SOURCESINK_CONTROLL_INT1( 
            "CMccVideoSinkUser diffr:", interval.Int64() )
            
            timeStamp = iPreviousPacketModifiedTimeStamp + 
                MCC_CONVERT_TO_90KHZ_CLOCK( interval.Int64() );
            }
         else
            {
            // NOP
            }
        
        __V_SOURCESINK_CONTROLL_INT2( 
            "CMccVideoSinkUser orig:", aHeaderInfo.iTimestamp, 
            " new:", timeStamp ) 
            
        iPreviousPacketOriginalTimeStamp = aHeaderInfo.iTimestamp;   
        iPreviousPacketModifiedTimeStamp = timeStamp;
        aHeaderInfo.iTimestamp = timeStamp; 
        iPreviousPacketTime = currentTime;
            
        modifyRequired = ETrue;        
        }
        
    return modifyRequired;
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::SeqNumDifToLastSeqNum
// ---------------------------------------------------------------------------
// 
TInt CMccVideoSinkUser::SeqNumDifToLastSeqNum( TUint16 aSeq )
    {
    TInt seqNumDif( 1 );
    TInt lastTimeStampIndex( iNumTimeStamps - 1 );
    if ( lastTimeStampIndex >= 0 )
        {
        TUint16 lastSeqNum = iTimeStamps[ lastTimeStampIndex ].iSeq;
        seqNumDif = aSeq - lastSeqNum;
        }
    return seqNumDif;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::SetModifyingState
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::SetModifyingState( TMccTimeStampModifyMode aModifyMode )
    {
    __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkUser::SetModifyingState, state:", aModifyMode )
    iModifyMode = aModifyMode;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::MonitoringModify
// ---------------------------------------------------------------------------
//
TBool CMccVideoSinkUser::MonitoringModify()
    {
    return ( iModifyMode == EModifyRequiredMonitoring ||
             iModifyMode == EModifyNotRequiredMonitoring );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::ModifyRequired
// ---------------------------------------------------------------------------
//
TBool CMccVideoSinkUser::ModifyRequired()
    {
    return ( iModifyMode == EModifyRequired || 
             iModifyMode == EModifyRequiredMonitoring );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::DoModifyCheckInit
// Check if other end does not use timestamps correctly.
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::DoModifyCheckInit( 
    const TRtpRecvHeader& aHeaderInfo )
    {
    TUint32 timeStamp = aHeaderInfo.iTimestamp;

    TUint32 timeStampDifference = GetTimestampDifferenceToPrevious( timeStamp );
    
    if ( timeStampDifference > 0 )
        {
        const TReal KMccPreferredTimeStampStep = 
                TReal( KMccDefaultVideoFrequency / iFrameRate );
        
        // If timestamp increased less than half of preferred step,
        // there is something wrong and timestamps need to be modified       
        const TInt KMccTimeStampIncreaseMin = KMccPreferredTimeStampStep / 2;
        
        if ( timeStampDifference < KMccTimeStampIncreaseMin )
            {
            __V_SOURCESINK_CONTROLL_INT1( 
                "CMccVideoSinkUser::DoModifyCheckInit, inc, difference:",
                timeStampDifference )
            
            SetModifyingState( EModifyRequiredMonitoring );
            }
        else
            {
            SetModifyingState( EModifyNotRequiredMonitoring );
            }
        }        
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::DoModifyCheckMonitoring
// Monitor certain period of time at the beginning of streaming whether
// other end is using timestamp incorrectly. Or stop modifying if it looks 
// that initial decision to modify timestamps was incorrect (other end is 
// after all using them correctly).
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::DoModifyCheckMonitoring( 
    const TRtpRecvHeader& aHeaderInfo )
    {
    TUint32 timeStamp = aHeaderInfo.iTimestamp;
    
    TTime currentTime;
    currentTime.HomeTime(); 
        
    if ( iFirstModifyCheck == 0 )
        {
        iFirstModifyCheck.HomeTime();
        iFirstModifyCheckTimestamp = timeStamp;
        return;
        }
        
    TTimeIntervalMicroSeconds timeMonitored = 
        currentTime.MicroSecondsFrom( iFirstModifyCheck );
    
    if ( timeMonitored > iCurrentMonitoringPeriod )
        {
        const TInt KMicrosecsToSecs = 1000000;

        TInt timestampIncreaseWithinMonitoringPeriod = 
                timeStamp - iFirstModifyCheckTimestamp;
                
        TReal idealTimestampIncrease = 
            KMccDefaultVideoFrequency * timeMonitored.Int64() / KMicrosecsToSecs;
        
         __V_SOURCESINK_CONTROLL_INT2( 
                "CMccVideoSinkUser::DoModifyCheckMonitoring, period:",
                timeMonitored.Int64(),
                "inc:", timestampIncreaseWithinMonitoringPeriod )
            
        __V_SOURCESINK_CONTROLL_REAL( 
                "CMccVideoSinkUser::DoModifyCheckMonitoring, ideal inc:", 
                idealTimestampIncrease )
                    
        if ( timestampIncreaseWithinMonitoringPeriod < 
                ( idealTimestampIncrease * iTimestampIncreaseFactorDown ) ||
             timestampIncreaseWithinMonitoringPeriod >
                ( idealTimestampIncrease * iTimestampIncreaseFactorUp ) )
            {        
            // Need to calculate average difference again
            iAverageTimeStampDifference = 0;
            iNumTimeStamps = 0;
            
            DecideMonitoringContinuation( EModifyRequired );
            }
        else
            {
            DecideMonitoringContinuation( EModifyNotRequired );
            }
        }      
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::GetTimestampDifferenceToPrevious
// ---------------------------------------------------------------------------
//
TUint32 CMccVideoSinkUser::GetTimestampDifferenceToPrevious( 
    TUint32 aCurrentTimestamp )
    {
    TUint32 timeStampDifference( 0 );
    TInt lastTimeStampIndex( iNumTimeStamps - 1 ); 
    if ( lastTimeStampIndex >= 0 && 
         aCurrentTimestamp != iTimeStamps[ lastTimeStampIndex ].iTime )
        {
        TUint32 lastTimeStamp = iTimeStamps[ lastTimeStampIndex ].iTime;

        timeStampDifference = 
            aCurrentTimestamp > lastTimeStamp ? 
                ( aCurrentTimestamp - lastTimeStamp ) : 
                ( lastTimeStamp - aCurrentTimestamp );
        }
    return timeStampDifference;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::UpdateFirstTimestamp
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::UpdateFirstTimestamp( TUint32 aTimestamp )
    {      
    if ( iFirstTimestamp == 0 )
        {
        iFirstTimestamp = aTimestamp;
        
        __V_SOURCESINK_CONTROLL_INT1( 
        "CMccVideoSinkUser::UpdateFirstTimestamp, first timestamp:", 
        iFirstTimestamp )
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::DifferenceThreshold
// If there's some gap in sequence numbers which is not anyhow too big, it
// might be that some frames are just dropped by network or sender. 
// In such case take seq num dif in count for timestamp difference threshold.
// With negative seq num, threshold is always more strict.
// ---------------------------------------------------------------------------
//    
TUint32 CMccVideoSinkUser::DifferenceThreshold( TInt aSeqNumDif ) const
    {
    if ( aSeqNumDif > KMccSeqNumDifferenceThresholdMin &&
         aSeqNumDif < KMccSeqNumDifferenceThresholdMax )
        {
        return iAverageTimeStampDifference * 
            ( aSeqNumDif + KMccTimeStampDifferenceTreshold );
        }
    return iAverageTimeStampDifference * KMccTimeStampDifferenceTreshold;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::CheckDenyFramesMaxPeriod
// ---------------------------------------------------------------------------
//    
void CMccVideoSinkUser::CheckDenyFramesMaxPeriod()
    {
    if ( iDenyFramesStarted.Int64() > 0 )
        {
        TTime currentTime;
        currentTime.HomeTime();
        
        TTimeIntervalMicroSeconds denyPeriod = 
            currentTime.MicroSecondsFrom( iDenyFramesStarted );
            
        if ( denyPeriod > KMccMaxDenyPeriodMicrosecs )
            {
            __V_SOURCESINK_CONTROLL( 
                "CMccVideoSinkUser::CheckDenyFramesMaxPeriod, end denial" )
        
            Reset( ETrue );
            }
        }
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::DecideMonitoringContinuation
// ---------------------------------------------------------------------------
// 
void CMccVideoSinkUser::DecideMonitoringContinuation( 
    TMccTimeStampModifyMode aNextState )
    {
    TMccTimeStampModifyMode decidedNextState( aNextState );
    if ( iCurrentMonitoringPeriod == KMccModifyMonitoringShortPeriodMicrosecs )
        {
        // Monitor still for longer period, after longer period of sampling
        // more strict rules can be used to detect incorrect behavior
        iCurrentMonitoringPeriod = KMccModifyMonitoringLongPeriodMicrosecs;
        iTimestampIncreaseFactorDown = 
            KMccMonitoringLongPeriodTimestampIncreaseFactorDown;
        iTimestampIncreaseFactorUp = 
            KMccMonitoringLongPeriodTimestampIncreaseFactorUp;
        if ( aNextState == EModifyRequired )
            {
            decidedNextState = EModifyRequiredMonitoring;
            }
        else if ( aNextState == EModifyNotRequired )
            {
            decidedNextState = EModifyNotRequiredMonitoring;
            }
        else
            {
            }
        }
    SetModifyingState( decidedNextState );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::IsResetNeededNormalMode
// ---------------------------------------------------------------------------
// 
TBool CMccVideoSinkUser::IsResetNeededNormalMode( 
    TRtpRecvHeader& aHeaderInfo )
    {
    TBool isResetNeeded( EFalse );
    TUint32 oldTimeStamp = aHeaderInfo.iTimestamp;
        
    if ( !ModifyRequired() && iAverageTimeStampDifference )
        {
        TUint32 timeStamp = aHeaderInfo.iTimestamp;
        
        // Check if new timestamp is totally off the charts.
        
        TInt lastTimeStamp( iNumTimeStamps - 1 );
        
        TInt seqNumDif = SeqNumDifToLastSeqNum( aHeaderInfo.iSeqNum );

        if ( timeStamp > iTimeStamps[ lastTimeStamp ].iTime )
            {
            isResetNeeded = ( timeStamp - iTimeStamps[ lastTimeStamp ].iTime ) > 
                DifferenceThreshold( seqNumDif );
            }
        else
            {
            isResetNeeded = ( timeStamp + DifferenceThreshold( seqNumDif ) ) < 
                    iTimeStamps[ lastTimeStamp ].iTime;
            }
        }
        
    if ( isResetNeeded )
        {
        // Rollback changes if resetted
        aHeaderInfo.iTimestamp = oldTimeStamp; 
        }
    else
        {
        // Return value is not interesting
        IsModifyNeeded( aHeaderInfo );
        }
        
    return isResetNeeded;
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::IsResetNeededRealTimeMode
// ---------------------------------------------------------------------------
// 
TBool CMccVideoSinkUser::IsResetNeededRealTimeMode( 
    TRtpRecvHeader& /*aHeaderInfo*/ )
    {
    // Reset never needed    
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CMccVideoSinkUser::RealTimeUser
// ---------------------------------------------------------------------------
// 
TBool CMccVideoSinkUser::RealTimeUser()
    {
    return ( GetPreroll() == 0 );
    }

// ---------------------------------------------------------------------------
// CMccVideoSinkUser::CheckErrorL
// ---------------------------------------------------------------------------
//
void CMccVideoSinkUser::CheckErrorL( TInt aError )
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSinkUser::CheckErrorL, error", aError )
    if ( aError == KErrOverflow )
		{		
	    SetPacketOverflow( CMccVideoSinkUser::EOccured );	
	    
	    // Overflow is not cosidered as a fatal error as it can be recovered
	    aError = KErrNone;	
		}
    User::LeaveIfError( aError );
    }


// ---------------------------------------------------------------------------
// CMccVideoSinkUser::CodecSpecificDataHandling
// ---------------------------------------------------------------------------
//  
TBool CMccVideoSinkUser::CodecSpecificDataHandling( const TDesC8& aPayloadData )
    {
    TBool importantData( EFalse );
    if ( MCC_IS_AVC_USER_ENTRY( this ) )
        {
        // Avc sprop-parameter-set values are dropped due helix seems to have
        // currently some issue with those.
        if ( TMccCodecInfo::IsAvcPpsOrSpsData( aPayloadData, ETrue ) )
            {
            __V_SOURCESINK_CONTROLL_STR8( 
            "CMccVideoSinkUser::CodecSpecificDataHandling, data:", aPayloadData )
            importantData = ETrue;
            }
        }
    return importantData;
    }
        
// End of file

