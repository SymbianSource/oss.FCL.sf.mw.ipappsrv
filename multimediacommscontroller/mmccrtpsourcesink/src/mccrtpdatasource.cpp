/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP Datasource
*
*/




// INCLUDE FILES
#include <in_sock.h> 
#include <mmf/common/mmfcontroller.h>

#include "rtpapi.h"
#include "mccrtpdatasource.h"
#include "mccinternalevents.h"
#include "formatstatemachine.h"
#include "mccrtpdefs.h"
#include "mmccinterfacedef.h"
#include "mcctimermanager.h"

#include <srtpcryptocontext.h>
#include <srtpstreamin.h>

// MACROS
#define MCC_RTPSOURCE_ENDPOINT_ID reinterpret_cast<TUint32>( static_cast<MDataSource*>( this ) )


// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRtpDataSource::CMccRtpDataSource
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRtpDataSource::CMccRtpDataSource() : 
    CMccDataSource( KMccRtpSourceUid ),
    MMccRtpInterface(),
    iStandByTimerValue( KRtpStandByTimer ),
    iRtpStreamId( KNullId ),
    iInactivityTimerId( KMaxTUint32 )
    {
    iCurRecvPayloadType = KMccPTNotDefined;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::ConstructSourceL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::ConstructSourceL( const TDesC8& aInitData )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::ConstructSourceL" )

    SetStateL( ERtpStateConstructed );
 
    TMccRtpSourceSetting rtpSettings;
    rtpSettings.iStandByTimerValue = 200;
    TMccRtpSourceSettingBuf rtpSettingsBuf( rtpSettings );
    
    if ( aInitData.Length() > 0)
	    {
	   	TMccRtpSourceSettingBuf settingsBuf;
		settingsBuf.Copy( aInitData );
		iStandByTimerValue = settingsBuf().iStandByTimerValue;
	    }
    
    iTimer = CMccTimerManager::NewL();
    
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::ConstructSourceL iStandByTimerValue %d",
        iStandByTimerValue )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::NewSourceL
// Static constructor.
// -----------------------------------------------------------------------------
//
MDataSource* CMccRtpDataSource::NewSourceL( TUid /*aImplementationUid*/, 
                                            const TDesC8& /*aInitData*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::NewSourceL" )

    CMccRtpDataSource* self = new ( ELeave ) CMccRtpDataSource();
    return static_cast<MDataSource*>( self );
    }


// -----------------------------------------------------------------------------
// CMccRtpDataSource::~CMccRtpDataSource()
// Destructor.
// -----------------------------------------------------------------------------
//
CMccRtpDataSource::~CMccRtpDataSource()
    {
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::~CMccRtpDataSource 0x%x", this )

    iBufferToFill = NULL;
    iEventHandler = NULL;
    iFillBufferRequester = NULL;

    CloseStreams();
    iUsers.Close();
    
    delete iTimer;
    delete iJitCalc;
    
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::~CMccRtpDataSource OUT" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::FillBufferL
// FillBufferL works synchronously.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::FillBufferL( CMMFBuffer* aBuffer, 
                                     MDataSink*  aConsumer, 
                                     TMediaId    /*aMediaId*/ )
    {    
    TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::FillBufferL" )
    
    // Sink might want to indicate pause/stop by passing NULL buffer
    if ( aBuffer )
        {
        __ASSERT_ALWAYS ( aConsumer, User::Leave( KErrArgument ) );
        __ASSERT_ALWAYS ( KUidMmfDataBuffer == aBuffer->Type( ), 
        User::Leave( KErrNotSupported ) );
        }
        
    iBufferToFill = static_cast<CMMFDataBuffer*>( aBuffer );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::CanCreateSourceBuffer
// NOT SUPPORTED. MDataSource pure virtual function must to be implemented.
// -----------------------------------------------------------------------------
//
TBool CMccRtpDataSource::CanCreateSourceBuffer()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::CreateSourceBufferL
// NOT SUPPORTED. MDataSource pure virtual function must to be implemented.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccRtpDataSource::CreateSourceBufferL( TMediaId /*aMediaId*/,
                                                    TBool& /*aReference*/ )
    {
    TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::CreateSourceBufferL KErrNotSupported" )
    
    User::Leave( KErrNotSupported );
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::RtpPacketReceived
// RTP stack callback function for received RTP packet.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::RtpPacketReceived( TRtpId aStreamId, 
                                           const TRtpRecvHeader& aHeaderInfo, 
                                           const TDesC8& aPayloadData )
	{
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceived iSecureKeyExpired: %d",
        iSecureKeyExpired )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceived iPayloadType: %u",
        aHeaderInfo.iPayloadType )
        
    // Filter out also bogus payload types here. NB; should something be done
    // for the trapped errorcode and bogus RTP packets. Though they should not
    // reach this point.
	if ( !iSecureKeyExpired &&
	     aHeaderInfo.iPayloadType < KPayloadTypeUndefined )
		{
		TRAP_IGNORE( RtpPacketReceivedL( aStreamId, aHeaderInfo, aPayloadData ) )
		}
	}

// -----------------------------------------------------------------------------
// CMccRtpDataSource::RtpPacketReceivedL
// RTP stack callback function for received RTP packet.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::RtpPacketReceivedL( TRtpId aStreamId, 
                                           const TRtpRecvHeader& aHeaderInfo, 
                                           const TDesC8& aPayloadData )
    {
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceivedL DATALEN: %d",
        aPayloadData.Length() )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceivedL ASTREAM: %d",
        aStreamId )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceivedL ISTREAM: %d",
        iRtpStreamId )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceivedL SEQ_NUM: %d",
        aHeaderInfo.iSeqNum )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceivedL T_STAMP: %d",
        aHeaderInfo.iTimestamp )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceivedL MARKER: %d",
        aHeaderInfo.iMarker )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RtpPacketReceivedL STATE: %d",
        State() )
    
    if ( ERtpStatePlaying == State() )
        {
        __ASSERT_ALWAYS( iBufferToFill, User::Leave( KErrNotReady ) );
        __ASSERT_ALWAYS( iFillBufferRequester, User::Leave( KErrNotReady ) );
        
        TMccRtpUser* currentUser = ValidatePacketL( aStreamId, aHeaderInfo, aPayloadData );
        TBool isCN = EFalse;
        
        // Change the main receive PT only if it is not CN.
        if ( KCnPayloadType != aHeaderInfo.iPayloadType &&
             KCnPayloadTypeReserved != aHeaderInfo.iPayloadType )
            {
            iCurRecvPayloadType = aHeaderInfo.iPayloadType;
            }
        else
            {
            isCN = ETrue;
            }
        
        if ( InactivityTimerActive() )
            {
            TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::resetting inactivity timer" )
            
            StartInactivityTimerL( iTimeoutTime );
            }
        
        // Note that ValidataPacketL may return a NULL ptr, thus guard against
        // it.
        if ( currentUser )
            {
  		    StartRtpStandByTimerL( currentUser );
  		    DoStandByDecision( currentUser );
            }
        
        // Do jitter calculations
        if ( iJitCalc->IsObserving() )
            {
            iJitCalc->RtpPacketReceived( aHeaderInfo.iTimestamp,
                                         aHeaderInfo.iMarker,
                                         aHeaderInfo.iSeqNum );
            }
            
        // First packet after SourcePlayL, notify event handler by sending
        // the event.
        if ( !iDlStreamStarted )
            {
            iDlStreamStarted = ETrue;
            SendInternalRtpEventToClient( iEventHandler,
                                          KMccRtpSourceUid, 
                                          KMccInternalRtpSrcMmfEvent, 
                                          KMccActivityEvent,
                                          MCC_RTPSOURCE_ENDPOINT_ID );
            }
        
        // CN is not currently sent forward to decoding and playback. It
        // causes problems with jitterbuffering and results into a degraded
        // audio quality.
        if ( !isCN )
            {
            TRAPD( err, PlayoutRtpPacketL( aHeaderInfo, aPayloadData ) )
            
            // Notify the event handler about error cases so appropriate
            // actions can be taken.
            if ( KErrNone != err )
                {
                SendInternalRtpEventToClient( iEventHandler,
                                              KMccRtpSourceUid,
                                              KMccInternalRtpSrcMmfEvent,
                                              KMccStreamError,
                                              MCC_RTPSOURCE_ENDPOINT_ID,
                                              err );
                }
            }
		}
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SetSourceDataTypeCode
// Sets the sources datatype code ( Codec )
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSource::SetSourceDataTypeCode( TFourCC aCodec, TMediaId /*aMedia*/ )
    {
    iCodecInfo.iFourCC = aCodec;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceDataTypeCode()
// Sets the datatype code ( codec )
// -----------------------------------------------------------------------------
//
TFourCC CMccRtpDataSource::SourceDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return iCodecInfo.iFourCC;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::BufferEmptiedL
// CMccRtpDataSource supports only passive mode of operation. 
// Thus, Datapath->EmptyBufferL isn't called.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::BufferEmptiedL KErrNotSupported" )
    
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceThreadLogon
//
// Method to 'logon' the data source to the same thread that source will be 
// consuming data in. Thread specific initialisation is done here.
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSource::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourceThreadLogon" )
    
    TInt err( KErrNone );
    
    iEventHandler = &aEventHandler;
    if ( NULL == iJitCalc )
        {
        TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourceThreadLogon,\
            create jitter calculator" )
        TRAP( err, iJitCalc = CMccJitterCalculator::NewL( *this ) );
        }
    
    return err;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceThreadLogoff
//
// Method to 'logoff' the data source from the same thread that source consumes 
// data in. Thread specific releasing of resources is done here.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourceThreadLogoff()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourceThreadLogoff" )
    StopKeepalive();

    iEventHandler = NULL;

    // Close the streams (if any)
    if ( KNullId != iRtpStreamId )
        {
        iRtpAPI->UnregisterRtpObserver( iSessionID );
    	}
    	
	CloseStreams();
    iRtpAPI = NULL;
    iRtpStreamId = KNullId;
    
    iBufferToFill = NULL;
        
    delete iTimer;
    iTimer = NULL;

    delete iJitCalc;
    iJitCalc = NULL;
    }

// ---------------------------------------------------------------------------
// CMccRtpDataSource::SourcePrimeL
// Source must be primed before playing.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourcePrimeL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourcePrimeL" )

    SetStateL( ERtpStatePrimed );
    
    SendStreamEventToClient( KMccStreamPrepared ); 
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourcePlayL
// Start receiving RTP packets.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourcePlayL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourcePlayL" )
    if ( iSecureKeyExpired )
    	{
    	TRACE_RTP_SINK_PRINT( "Leave becuase the secure key expired" )
    	User::Leave( KErrGeneral );
    	}
	//Leave when is the secure evnet key expired and try to restart
    iPacketsReceived = 0;
    iDlStreamStarted = EFalse;
    
    TSourceSinkState oldState = State();
    
    __ASSERT_ALWAYS( iJitCalc, User::Leave( KErrNotFound ) );
    iJitCalc->ResetCounters();
    
    ResetStandBy();
    
    SetStateL( ERtpStatePlaying );
    
    // Enable keepalive while receiving (depending on codec settings)
    // Keepalive is not supported in recvonly secure sessions
    if ( !iSecSession )
        {
        StartKeepaliveL( *iRtpMediaClock );
        }
    
    if ( oldState == ERtpStatePaused )
        {
        SendStreamEventToClient( KMccStreamResumed ); 
        }
    else
        {
        SendStreamEventToClient( KMccStreamStarted ); 
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourcePauseL
// Pause RTP packet receiving.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourcePauseL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourcePauseL" )
    
    iPacketsReceived = 0;
    __ASSERT_ALWAYS( iJitCalc, User::Leave( KErrNotFound ) );
    iJitCalc->ResetCounters();

    SetStateL( ERtpStatePaused );
    
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourcePauseL\
    stop standby timers" )
    
    StopRtpStandByTimers();
    
    SendStreamEventToClient( KMccStreamPaused ); 
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceStopL
// Stop RTP packet receiving.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourceStopL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourceStopL" )
    
    iDlStreamStarted = EFalse;
    SetStateL( ERtpStateStopped );
    
    StopKeepalive();
    
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SourceStopL,\
    Stop standby timers" )
    
    StopRtpStandByTimers();
    
    SendStreamEventToClient( KMccStreamStopped ); 
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::PlayoutRtpPacketL
// Pass filled buffer to the data sink of RTP data source.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::PlayoutRtpPacketL( const TRtpRecvHeader& aHeaderInfo, 
        const TDesC8& aPayloadData )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::PlayoutRtpPacketL" )
    __ASSERT_ALWAYS( iBufferToFill, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iFillBufferRequester, User::Leave( KErrNotReady ) );
    
    iBufferToFill->Data().Copy( aPayloadData );
    
    CPayloadFormatRead* sink = static_cast<CPayloadFormatRead*>( iFillBufferRequester );
    User::LeaveIfNull( sink );
    sink->DataBufferFilledL( iBufferToFill, aHeaderInfo );
    }


// -----------------------------------------------------------------------------
// CMccRtpDataSource::DoCreateStreamL
// Creates a receive stream.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::DoCreateStreamL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::DoCreateStreamL IN !!!!" )
    __ASSERT_ALWAYS( KNullId == iRtpStreamId, User::Leave( KErrAlreadyExists ) );
    __ASSERT_ALWAYS( NULL != iRtpAPI, User::Leave( KErrNotReady ) );
    
    TRcvStreamParams rcvParams;
    rcvParams.iPayloadType = iCodecInfo.iPayloadType;

    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::DoCreateStreamL CONFIGURED_PT: %u",
        iCodecInfo.iPayloadType )
    
    iRtpStreamId = iRtpAPI->CreateReceiveStreamL( iSessionID, rcvParams );
    if ( KNullId == iRtpStreamId )
        {
        TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::DoCreateStreamL KErrCouldNotConnect 1" )
        
        User::Leave( KErrCouldNotConnect );
        }
    
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::DoCreateStreamL SetSamplingRate 1" )
        
    User::LeaveIfError( 
        iRtpAPI->SetSamplingRate( iCodecInfo.iPayloadType, KDefSampleRate ) );

    if ( KMccPayloadTypeMax != iCodecInfo.iRedundantPayload )
        {
        User::LeaveIfError( iRtpAPI->SetSamplingRate( 
            iCodecInfo.iRedundantPayload, KDefSampleRate ) );
        }
    
    User::LeaveIfError( iRtpAPI->RegisterRtpObserver( iSessionID, *this ) );

    DoCreateSrtpStreamL();
    
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSource::DoCreateStreamL OUT" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::DoCreateSrtpStreamL
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::DoCreateSrtpStreamL()
	{
	TRACE_RTP_SINK_PRINT( "CMccRtpDataSource::DoCreateStreamL IN" )
	if ( !iSrtpStream && iContext && iSecSession && KNullId != iRtpStreamId )
        {
        TRACE_RTP_SINK_PRINT( "CMccRtpDataSource::DoCreateStreamL, creating" )
        iSrtpStream = CSRTPStreamIn::NewL( *iSecSession, iContext, *this );
        }
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSource::DoCreateStreamL OUT" )
	}
	
// -----------------------------------------------------------------------------
// CMccRtpDataSource::NegotiateSourceL
// Derived from MDataSource
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::NegotiateSourceL( MDataSink& aDataSink )
    {
    iFillBufferRequester = &aDataSink;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SendMediaSignallingL
// Derived from CRtpInterface
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SendMediaSignallingL( const TMccEvent& aEvent )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SendMediaSignallingL" )
    	
    __ASSERT_ALWAYS( aEvent.iEventCategory == KMccEventCategoryRtcp &&
                     aEvent.iEventType == KMccRtcpControl &&
                     iEnableRtcp,
                     User::Leave( KErrNotSupported ) );
                     
    __ASSERT_ALWAYS( iRtpAPI, User::Leave( KErrNotReady ) );
    
    const TMccRtcpEventData& rtcpEvent = 
        (*reinterpret_cast<const TMccRtcpEventDataPackage*>( 
            &aEvent.iEventData ))(); 
    
    switch ( rtcpEvent.iRtcpPacketType )
        {
        case KRtcpRrPacket:
            {
            TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SendMediaSignallingL, RR" )
            
            __ASSERT_ALWAYS( KNullId != iRtpStreamId, User::Leave( KErrNotReady ) );
            User::LeaveIfError( iRtpAPI->SendRtcpRrPacket( iRtpStreamId ) );
            break;
            } 
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::StartInactivityTimer
// Starts inactivity timer for a stream 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StartInactivityTimerL( TUint32 aTimeoutTime )
    {
    TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::StartInactivityTimerL" )
    
    if( aTimeoutTime == 0 )
        {
        User::Leave( KErrArgument );
        }
        
    iTimeoutTime = aTimeoutTime;
    
    StopInactivityTimerL();
    
    iInactivityTimerId = iTimer->StartL( this, aTimeoutTime );
    
    TRACE_RTP_SOURCE_PRINT2 ( "CMccRtpDataSource::StartInactivityTimerL, timer id: %d", 
                              iInactivityTimerId )
    
    TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::StartInactivityTimerL, Exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::StopInactivityTimer
// Stops inactivity timer for a stream
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StopInactivityTimerL()
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::StopInactivityTimerL" )

    // It does not matter whether timer is active or not
    iTimer->Stop( iInactivityTimerId );
    }
    
    
// -----------------------------------------------------------------------------
// CMccRtpDataSource::StandBy
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSource::StandBy( 
    TMccStandbyActionType aActionType, 
    TUint aPayloadType )
    {
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::StandBy, action type:%d", aActionType )
    
    TRAPD( err, StandByL( aActionType, aPayloadType ) );
    if ( err != KErrNone )
        {
        TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::StandByL, error=%d", err )
        }
        
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::StandBy, Exit" )   
    return err;
    }
	
// -----------------------------------------------------------------------------
// CMccRtpDataSource::RtpStreamId
// -----------------------------------------------------------------------------
//    
TRtpId CMccRtpDataSource::RtpStreamId()
    {
    return iRtpStreamId;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::RegisterPayloadTypesL
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::RegisterPayloadTypesL( const RArray<TUint>& aPayloadTypes )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::RegisterPayloadTypesL" )
    
#ifdef _DEBUG

    for( TInt k = 0; k < aPayloadTypes.Count(); k++ )
        {
        TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::RegisterPayloadTypesL PT: %u",
            aPayloadTypes[k] )
        }
        
#endif
    
    
    TInt ind = aPayloadTypes.Count();
    while ( ind-- )
        {
        if ( !FindUserEntryByPayloadType( aPayloadTypes[ind] ) )
            {
            TMccRtpUser entry( iEventHandler );
            entry.iPayloadType = aPayloadTypes[ind];
            iUsers.AppendL( entry );
            }
        }
    }
            
// -----------------------------------------------------------------------------
// CMccRtpDataSource::UnRegisterPayloadTypes
// Unregisters payload types to accept.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::UnRegisterPayloadTypes( const RArray<TUint>& aPayloadTypes )
    {
#ifdef _DEBUG

    for( TInt k = 0; k < aPayloadTypes.Count(); k++ )
        {
        TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::UnRegisterPayloadTypes PT: %u",
            aPayloadTypes[k] )
        }
        
#endif

    TInt ind = aPayloadTypes.Count();
    while ( ind-- )
        {
        TMccRtpUser* userEntry = FindUserEntryByPayloadType( aPayloadTypes[ind] );
        TInt index = FindUserEntryIndex( userEntry );
        if ( index != KErrNotFound )
            {
            iUsers.Remove( index );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::ValidatePacketL
// Validates the received RTP packet and it's header
// -----------------------------------------------------------------------------
//
TMccRtpUser* CMccRtpDataSource::ValidatePacketL( const TRtpId aStreamId, 
        const TRtpRecvHeader& aHeader, const TDesC8& aData )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::ValidatePacketL" )
    
    __ASSERT_ALWAYS( iBufferToFill, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iBufferToFill->Data().MaxSize() >= aData.Size(),
        User::Leave( KErrOverflow ) );
    
    TMccRtpUser* userEntry = FindUserEntryByPayloadType( aHeader.iPayloadType );
    TBool isCN = EFalse;
    
    if ( KCnPayloadType == aHeader.iPayloadType ||
         KCnPayloadTypeReserved == aHeader.iPayloadType )
        {
        isCN = ETrue;
        }
    
    if ( !userEntry )
        {
        if ( iCurRecvPayloadType != aHeader.iPayloadType && !isCN )
            {
            TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::ValidatePacketL Unknown media" )
            
            iCurRecvPayloadType = aHeader.iPayloadType;
            
            // Support for unsymmetric speech codecs, payload type is reported as
            // an error code in this case.
            SendStreamEventToClient( KMccUnknownMediaReceived,
                aHeader.iPayloadType );
            }         
        
        if ( !isCN )
            {
            TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::ValidatePacketL PT NOK" )
            
            User::Leave( KErrNotSupported );
            }
        
        // Try to find user on current PT so the inactivity could be reset.
        userEntry = FindUserEntryByPayloadType( iCurRecvPayloadType );
        }
    else if ( userEntry &&
              KMaxTUint == userEntry->iTimerId &&
              isCN )
        {
        // This is because timers are used against the main payloadtype in CN
        // cases.
        userEntry = FindUserEntryByPayloadType( iCurRecvPayloadType );
        }
    
    if ( iRtpAPI && ( iRtpStreamId != aStreamId ) )
        {
        // Adapt to SSRC change
        iRtpAPI->CloseStream( iRtpStreamId );
        iRtpStreamId = aStreamId;
        }
    
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::ValidatePacketL userEntry: 0x%x",
        userEntry )
    
    return userEntry;
    }
    
// -----------------------------------------------------------------------------
// CMccRtpDataSource::SendStreamEventToClient()
// -----------------------------------------------------------------------------
//	
void CMccRtpDataSource::SendStreamEventToClient( 
    TMccEventType aEventType,
    TInt aError,
    TUint32 aTargetPayloadType )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SendStreamEventToClient" )
    
    if ( iEventHandler )
	    {
	    ClearMccEvent();
	    
	    iMccEvent.iEndpointId = MCC_RTPSOURCE_ENDPOINT_ID;
	    iMccEvent.iEventCategory = KMccEventCategoryStream;
	    iMccEvent.iEventType = aEventType;
	    iMccEvent.iErrorCode = aError;
	    
	    if ( aTargetPayloadType != KMccPTNotDefined )
	        {
    	    iMccEvent.iEventNumData = KMccPayloadSpecificEvent;
    	    iMccEvent.iReserved = aTargetPayloadType;
	        }

		TMccInternalEvent internalEvent( KMccRtpSourceUid, 
		                                 EMccInternalEventNone,
		                                 iMccEvent );
		                         
		iEventHandler->SendEventToClient( internalEvent );
	    }
	else
		{
		TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SendStreamEventToClient, \
		iEventHandler=NULL" )
		}
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SendJitterEvent()
// -----------------------------------------------------------------------------
//	
void CMccRtpDataSource::SendJitterEvent( TMccRtpEventDataExtended aEvent, TInt aError )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::SendJitterEvent" )

	SendJitterEventToClient( iEventHandler,
	                         KMccRtpSourceUid, 
                             EMccInternalJitterEventStatusReport, 
                             KMccMediaQualityStatus,
                             MCC_RTPSOURCE_ENDPOINT_ID,
                             aError,
                             aEvent.iJitterEstimate,
                             aEvent.iPacketsReceived,
                             aEvent.iPrevTransTime,
                             aEvent.iTriggeredJitterLevel,
                             aEvent.iPacketLoss,
                             aEvent.iTriggeredPacketLoss );
    }  

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceCustomCommand()
// 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::SourceCustomCommand( TMMFMessage& aMessage )
    {
    TRAPD( err, SourceCustomCommandL( aMessage ) );
    
    #ifdef TRACE_RTP_SOURCE
        RDebug::Print( _L("CMcpRtpDataSource::SourceCustomCommand ERR: %d"), err );
    #endif
    
    aMessage.Complete( err );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::SourceCustomCommandL()
// Worker function for TRAP'ping possible leaves that custom commands may have
// -----------------------------------------------------------------------------
//    
void CMccRtpDataSource::SourceCustomCommandL( TMMFMessage& aMessage )
    {
    #ifdef TRACE_RTP_SOURCE
        RDebug::Print( _L("CMcpRtpDataSource::SourceCustomCommandL FUNC: %d"), aMessage.Function() );
    #endif
    
    switch( aMessage.Function() )
        {
        case EStartMediaQualityObserving:
            // Send message data to jitter calculator, SetMediaConfigsL will
            // leave with KErrAlreadyExists if there is already a pending
            // client request, which will propagate to SourceCustomCommand()
            iJitCalc->SetMediaConfigsL( aMessage );
            User::LeaveIfError( iJitCalc->StartObserving() );
            break;
            
        case ECancelMediaQualityObserving:
            if ( iJitCalc->IsObserving() )
                {
                iJitCalc->CancelObserving();               
                }
            else
                {             
                User::Leave( KErrNotReady );
                }
            break;

        default:
            // Not supported CustomCommand => Leave 
            User::Leave( KErrNotSupported );
            break;
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::InactivityTimerActive
// -----------------------------------------------------------------------------
//
TBool CMccRtpDataSource::InactivityTimerActive() const
    {
    return ( iInactivityTimerId != KMaxTUint32 && 
             iTimer && 
             iTimer->IsRunning( iInactivityTimerId ) );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::StandbyEnabled
// -----------------------------------------------------------------------------
//   
TBool CMccRtpDataSource::StandbyEnabled( TMccRtpUser* aUser ) const
    {
    return ( iCodecInfo.iType == KUidMediaTypeAudio && 
             iStandByTimerValue > 0 &&
             aUser &&
             aUser->iStandbyState != ETurnedOff );
    }
    	
// -----------------------------------------------------------------------------
// CMccRtpDataSource::StandByL
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StandByL( 
    TMccStandbyActionType aActionType, 
    TUint aPayloadType )
    {
    TMccRtpUser* userEntry = FindUserEntryByPayloadType( aPayloadType );
    __ASSERT_ALWAYS( userEntry, User::Leave( KErrNotFound ) );
    
    switch ( aActionType )
        {
        case EForceStandby:
            {
            if ( ERtpStatePlaying == State() )
                {
                TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::StandByL, force" )
                iTimer->Stop( userEntry->iTimerId );
                HandleStandByL( userEntry );
                }
            break;
            }
        case EActivateStandby:
            {
            TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::StandByL, activate, \
standby state before: %d", userEntry->iStandbyState )

            if ( userEntry->iStandbyState == EActivating )
                {
                // If currently activating, activation was not possible,
                // stop timer and wait that mechanism is activated again
                iTimer->Stop( userEntry->iTimerId );
                }
            else if ( userEntry->iStandbyState == EActive )
                {
                // If was active, re-activation can
                // be tried when data for this user is received.
                iTimer->Stop( userEntry->iTimerId );
                userEntry->iStandbyState = EInactive;
                }
            else 
                {
                // NOP
                }
                
            TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::StandByL, activate, \
standby state after: %d", userEntry->iStandbyState )

            break;
            }
        case EDeactivateStandby:
            {
            TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSource::StandByL, deactivate" )
            iTimer->Stop( userEntry->iTimerId );
            userEntry->iStandbyState = ETurnedOff;
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSource::StartRtpStandByTimerL
// Starts rtp standby timer 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StartRtpStandByTimerL( TMccRtpUser* aUser )
	{
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::StartRtpStandByTimer iStartedOnce: %d",
        aUser->iStartedOnce )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::StartRtpStandByTimer iPayloadType: %d",
        aUser->iPayloadType )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::StartRtpStandByTimer iTimerId: %d",
        aUser->iTimerId )
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::StartRtpStandByTimer iStandbyState: %d",
        aUser->iStandbyState )
    
    if ( StandbyEnabled( aUser ) ) 
        {
        // It does not matter whether timer is active or not
        iTimer->Stop( aUser->iTimerId );
        aUser->iTimerId = iTimer->StartL( this, iStandByTimerValue );
        
        TRACE_RTP_SOURCE_PRINT ( "CMCCRtpDataSource::StartRtpStandByTimer standby enabled" )
        }
	}
	
// -----------------------------------------------------------------------------
// CMccRtpDataSource::StopRtpStandByTimers
// Stop rtp standby timers
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::StopRtpStandByTimers()
	{
	TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::StopRtpStandByTimer" )
	
	// Stop all rtp standby timers
	for ( TInt i = 0; i < iUsers.Count(); i++ )
	    {
	    iTimer->Stop( iUsers[ i ].iTimerId );
	    }
	}

// -----------------------------------------------------------------------------
// CMccRtpDataSource::ResetStandBy
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::ResetStandBy()
	{
	TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::ResetStandBy" )
	
	// Stop all rtp standby timers
	for ( TInt i = 0; i < iUsers.Count(); i++ )
	    {
	    iUsers[ i ].iStandbyState = EInactive;
	    }
	}
	
// -----------------------------------------------------------------------------
// CMccRtpDataSource::HandleStandByL
// -----------------------------------------------------------------------------
//	
void CMccRtpDataSource::HandleStandByL( TMccRtpUser* aUser )    
	{
	__ASSERT_ALWAYS( StandbyEnabled( aUser ), User::Leave( KErrNotSupported ) );

     TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::HandleStandByL, deactivate, \
standby state before: %d", aUser->iStandbyState )

    aUser->iStandbyState = EDeactivating;
    
    SendStreamEventToClient( KMccStandbyInactivityEvent, 
       	                     KErrNone, 
       	                     aUser->iPayloadType );
	
	aUser->iStandbyState = EInactive;
	
     TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::HandleStandByL, deactivate, \
standby state after: %d", aUser->iStandbyState )
	}

// ---------------------------------------------------------------------------
// CMccRtpDataSource::DoStandByDecision
// If standby activation has been done for some other payload type than
// currently received payload type, stream for old payload type is paused
// and stream for the new payload type is started/resumed.
// ---------------------------------------------------------------------------
//    
void CMccRtpDataSource::DoStandByDecision( TMccRtpUser* aUser )
    {


    if ( StandbyEnabled( aUser ) && 
       ( aUser->iStandbyState == EInactive || aUser->iStandbyState == EDeactivating ) )
        {
        TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::DoStandByDecision, \
standby state before: %d", aUser->iStandbyState )

        aUser->iStandbyState = EActivating;
        
        SendStreamEventToClient( KMccStandbyActivityEvent, 
	  	                         KErrNone, 
	  	                         iCurRecvPayloadType );
	  	
	  	if ( aUser->iStandbyState == EActivating )
	  	    {
            aUser->iStandbyState = EActive;
	  	    }
	  	    
        TRACE_RTP_SOURCE_PRINT2( "CMccRtpDataSource::DoStandByDecision, \
standby state after: %d", aUser->iStandbyState )
        }
    }
		
// ---------------------------------------------------------------------------
// CMccRtpDataSource::FindUserEntryByPayloadType
// ---------------------------------------------------------------------------
//
TMccRtpUser* CMccRtpDataSource::FindUserEntryByPayloadType( TUint aPayloadType )
    {
    TMccRtpUser entry( iEventHandler );
    entry.iPayloadType = aPayloadType;
    TIdentityRelation<TMccRtpUser> comparison( RtpUserMatch );
    const TInt index = iUsers.Find( entry, comparison );
    if ( index != KErrNotFound )
        {
        return &iUsers[ index ];
        }
    
    return NULL;
    }

// ---------------------------------------------------------------------------
// CMccRtpDataSource::FindRtpUserEntryByTimerId
// ---------------------------------------------------------------------------
//
TMccRtpUser* CMccRtpDataSource::FindUserEntryByTimerId( TMccTimerId aTimerId )
    {
    TMccRtpUser entry( iEventHandler );
    entry.iTimerId = aTimerId;
    TIdentityRelation<TMccRtpUser> comparison( RtpUserMatch );
    TInt index = iUsers.Find( entry, comparison );
    if ( index != KErrNotFound )
        {
        return &iUsers[ index ];
        }
    return NULL;
    }
    
// ---------------------------------------------------------------------------
// CMccRtpDataSource::FindUserEntryIndex
// ---------------------------------------------------------------------------
//
TInt CMccRtpDataSource::FindUserEntryIndex( TMccRtpUser* aUser )
    {
    TInt index( KErrNotFound );
    if ( aUser )
        {
        TIdentityRelation<TMccRtpUser> comparison( RtpUserMatch );
        index = iUsers.Find( *aUser, comparison );
        }
    return index;
    }
        
// ---------------------------------------------------------------------------
// CMccRtpDataSource::RtpUserMatch
// ---------------------------------------------------------------------------
//   
TBool CMccRtpDataSource::RtpUserMatch( 
    const TMccRtpUser& aUser1, 
    const TMccRtpUser& aUser2 )
    {
    // First argument is always the search term
    
    TBool match( EFalse );
    if ( aUser1.iPayloadType != KMccPTNotDefined )
        {
        match = ( aUser1.iPayloadType == aUser2.iPayloadType );
        }
    else if ( aUser1.iTimerId != KMaxTUint32 )
        {
        match = ( aUser1.iTimerId == aUser2.iTimerId );
        }
    else
        {
        match = ( &aUser1 == &aUser2 );
        }
    return match;
    }
        
// ---------------------------------------------------------------------------
// FROM SRTP API
// This function is called by SRTP Stream initiated with 
// MSRTPReKeyingObserver when a master key is stale and needs 
// to be refreshed.  
// ---------------------------------------------------------------------------
//
void CMccRtpDataSource::SRTPMasterKeyStaleEvent( const CSRTPStream& aStream )
    {
    TRACE_RTP_INTERFACE_PRINT( "CMccRtpDataSource::SRTPMasterKeyStaleEvent" )
    
    if ( iSrtpStream == &aStream )
        {                                    
        iSecureKeyExpired = ETrue;
        SendSecureRtpEventToClient( iEventHandler,
                                    KMccRtpSourceUid,
                                    EMccInternalEventNone,
                                    KMccMasterKeyStaled,
                                    MCC_RTPSOURCE_ENDPOINT_ID );
        }
    else
        {
        TRACE_RTP_INTERFACE_PRINT( "CMccRtpDataSource::SRTPMasterKeyStaleEvent - Wrong stream" )
        }
    }

// ---------------------------------------------------------------------------
// FROM SRTP API
// This function is called by SRTP Stream initiated with 
// CSRTPSession  when a master key is stale and
// needs to be refreshed.  
// ---------------------------------------------------------------------------
//
void CMccRtpDataSource::SRTPMasterKeyStaleEvent(const CSRTPSession& aSession )
    {
    TRACE_RTP_INTERFACE_PRINT( "CMccRtpStream::SRTPMasterKeyStaleEvent" )
    
    if ( iSecSession == &aSession )
        {
        iSecureKeyExpired = ETrue;
        SendSecureRtpEventToClient( iEventHandler,
                                    KMccRtpSourceUid,
                                    EMccInternalEventNone,
                                    KMccMasterKeyStaled,
                                    MCC_RTPSOURCE_ENDPOINT_ID );

        }
    else
        {
        TRACE_RTP_INTERFACE_PRINT( "MccRtpStream::SRTPMasterKeyStaleEvent - Wrong session" )
        }
    } 

// -----------------------------------------------------------------------------
// CMccRtpDataSource::TimerExpiredL
// From MMccExpirationHandler
// -----------------------------------------------------------------------------
//
void CMccRtpDataSource::TimerExpiredL( TMccTimerId aTimerId, TAny* /*aTimerParam*/ )
    {
    TRACE_RTP_SOURCE_PRINT2( "CMccRtpStream::TimerExpiredL, timer id: %d", aTimerId )
    if ( aTimerId == iInactivityTimerId )
        {
        TRACE_RTP_SOURCE_PRINT ( "CMccRtpDataSource::TimerExpiredL, inactivity timer expired" )
    
        SendInternalRtpEventToClient( iEventHandler,
                                      KMccRtpSourceUid, 
                                      KMccInternalRtpSrcMmfEvent, 
                                      KMccInactivityEvent,
                                      MCC_RTPSOURCE_ENDPOINT_ID );
        }
    else
        {
        // Standby timeout
        TMccRtpUser* userEntry = FindUserEntryByTimerId( aTimerId );
        HandleStandByL( userEntry );
        }
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
