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
* Description:    RTP Datasink
*
*/




// INCLUDE FILES
#include <mmf/common/mmfcontroller.h> 

#include "rtpheader.h"
#include <srtpcryptocontext.h>
#include <srtpstreamout.h>

#include "mccrtpdatasink.h"
#include "mmccevents.h"
#include "mccinternaldef.h"
#include "mccrtpdefs.h"
#include "mmccinterfacedef.h"
#include "mccrtpmediaclock.h"

// MACROS

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS
#define MCC_RTPSINK_ENDPOINT_ID MCC_ENDPOINT_ID( static_cast<MDataSink*>( this ) )

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRtpDataSink::CMccRtpDataSink
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRtpDataSink::CMccRtpDataSink() :
    CMccDataSink( KMccRtpSinkUid ),
    MMccRtpInterface(), iRtpStreamId( KNullId )
    {
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::NewSinkL
// Static constructor.
// -----------------------------------------------------------------------------
//
MDataSink* CMccRtpDataSink::NewSinkL( TUid /*aImplementationUid*/, 
                                      const TDesC8& /*aInitData*/ )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::NewSinkL()" )
       
    CMccRtpDataSink* self = new ( ELeave ) CMccRtpDataSink();
    return static_cast<MDataSink*>( self ); 
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::ConstructSinkL
// class MDataSink inherited 2nd phase construction
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::ConstructSinkL( const TDesC8& aInitData )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::ConstructSinkL" )
    
    SetStateL( ERtpStateConstructed );
    
    TInt res = aInitData.Compare( KNullDesC8 );
    if ( 0 != res )
        {
        TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSink::ConstructSinkL KErrArgument" )
    
        User::Leave( KErrArgument );
        }
        
    #ifdef FTD_ENABLED
    TInt err = iStreamStatsQueue.OpenGlobal( KMccStreamStats, EOwnerProcess );
    if ( KErrNone != err )
        {
        User::Leave( err );
        }
    #endif
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::~CMccRtpDataSink
// Destructor
// -----------------------------------------------------------------------------
//
CMccRtpDataSink::~CMccRtpDataSink()
    {
    TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::~CMccRtpDataSink 0x%x", this )
    
    // We don't own this
    iEventHandler = NULL;
        
    CloseStreams();
        
    iDataSource = NULL;
    iBufferToBeEmptied = NULL;
    
    iUsers.Close();
        
    delete iSender;
    
    #ifdef FTD_ENABLED
    iStreamStatsQueue.Close();
    #endif
    }


// -----------------------------------------------------------------------------
// CMccRtpDataSink::SetCurrentUser
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SetCurrentUser( MAsyncEventHandler* aEventHandler )
    {
    iEventHandler = aEventHandler;
    }


// -----------------------------------------------------------------------------
// CMccRtpDataSink::Mute
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::Mute( TBool aMuteOn, TUint8 aExceptionPt )
    {
    TRACE_RTP_SINK_PRINT3( "CMccRtpDataSink::Mute, ENABLED: %d, PT: %u", 
        aMuteOn, aExceptionPt )
    
    iMuteOn = aMuteOn;
    iExceptionPt = aExceptionPt;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkThreadLogon
//
// Method to 'logon' the data sink to the same thread that sink will be consuming
// data in. Thread specific initialisation is done here.
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSink::SinkThreadLogon( MAsyncEventHandler& aEventHandler ) 
    {
    TRACE_RTP_SINK_PRINT ( "CMccRtpDataSink::SinkThreadLogon" )
    
    if ( FindRtpUserEntryForCurrent( aEventHandler ) )
        {
        TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::SinkThreadLogon, ERR: %d", KErrAlreadyExists )
        return KErrAlreadyExists;
        }

    TMccRtpUser user( &aEventHandler );
    TInt err = iUsers.Append( user );
    
    TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::SinkThreadLogon, exit with err: %d", err )
    
    return err;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkThreadLogoff
//
// Method to 'logoff' the data sink from the same thread that sink consumes 
// data in. Thread specific releasing of resources is done here.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkThreadLogoff() 
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkThreadLogoff" )
    
    RemoveCurrentRtpUser();
    
    SetCurrentUser( NULL );
    
    if ( 0 == iUsers.Count() )
        {
        // SinkThreadLogoff may be called while not stopped, so don't
        // bother checking any states here. SinkThreadLogoff means that the
        // thread is going down, so we must cleanup here...
    
        StopKeepalive();
    	CloseStreams();
        iRtpAPI = NULL;
        iRtpStreamId = KNullId;
        iUsers.Close();
        
    	delete iSender;
	    iSender = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SetSinkDataTypeCode
// Sets the datatype code ( codec )
// ----------------------------------------------------------------------------- 
//
TInt CMccRtpDataSink::SetSinkDataTypeCode( TFourCC aCodec, TMediaId aMedia )
    {
    if ( KUidMediaTypeAudio == aMedia.iMediaType || 
         KUidMediaTypeVideo == aMedia.iMediaType )
        {
        TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::SetSinkDataTypeCode, mediatype:%d", 
                               aMedia.iMediaType.iUid )
        iMedia = aMedia;
        iCodecInfo.iFourCC = aCodec;
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkDataTypeCode
// Returns the datatype code ( codec )
// -----------------------------------------------------------------------------
//
TFourCC CMccRtpDataSink::SinkDataTypeCode( TMediaId aMediaId )
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType ||
         KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iCodecInfo.iFourCC;
        }
    else
        {
        // Initializes to KMMFFourCCCodeNULL so the caller cannot see
        // if this sink really has some media type currently registered
        return TFourCC();
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::EmptyBufferL
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::EmptyBufferL( CMMFBuffer* /*aBuffer*/, 
                                    MDataSource* /*aSupplier*/, 
                                    TMediaId /*aMediaId*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::EmptyBufferL
// Overload to class MDataSink pure virtual function implementation.
// Take RTP packet in the buffer and send it.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::EmptyBufferL( CMMFBuffer* aBuffer,
                                    MDataSource* aSupplier,
                                    TMediaId /*aMediaId*/,
                                    TRtpSendHeader& aHeaderInfo )
    {
    __ASSERT_ALWAYS( KNullId != iRtpStreamId, User::Leave( KErrNotReady ) );
    this->CheckBufferSupportL( aBuffer );
    User::LeaveIfNull( aSupplier );
    
    if ( SendingAllowed( aHeaderInfo ) )
        {
        __ASSERT_ALWAYS( iSender, User::Leave( KErrNotReady ) );

        TRACE_RTP_SINK_PRINT3( 
            "CMccRtpDataSink::EmptyBufferL, timestamp: %d, datalen: %d", 
            aHeaderInfo.iTimestamp, aBuffer->BufferSize() )
                               
        iBufferToBeEmptied = static_cast<CMMFDataBuffer*>( aBuffer );

        iSender->SendRtpPacketL( iRtpStreamId,
                                 aHeaderInfo, 
                                 iBufferToBeEmptied->Data() ); 
        ResetKeepaliveTimer();
        }
    else
        {
        TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::EmptyBufferL, PACKET DROPPED" )
        }
    
    // Buffer is emptied, notify the caller so it can reuse the buffer.
    // We risk a dead lock if we don't do this if the supplier relies on this
    // callback.
    aSupplier->BufferEmptiedL( aBuffer ); 
        
    #ifdef FTD_ENABLED
    TMccStreamStats stats;
    stats.iPacketsSent = ++iPacketsSent;
    stats.SetFieldUpdatedFlag( EPacketsSent );
    iStreamStatsQueue.Send( stats );
    #endif
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::BufferFilledL
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
    // DataSource calls synchronously CMccRtpDataSink->EmptyBuffer(),
    // so DataSource->FillBuffer, CMccRtpDataSink->BufferFilledL chain is not used at the moment
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::CanCreateSinkBuffer
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
TBool CMccRtpDataSink::CanCreateSinkBuffer()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::CreateSinkBufferL
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccRtpDataSink::CreateSinkBufferL( TMediaId /*aMediaId*/,
                                                TBool& /*aReference*/ )
    {
    User::Leave( KErrNotSupported ); 
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkPrimeL
// Prime the sink. 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkPrimeL()
    { 
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkPrimeL" )
    
    SetStateL( ERtpStatePrimed );
    
    // Start sending keep alive packets. Secure session has to wait until
    // secure stream is created 
    if ( !iSecSession )
        {
        StartKeepaliveL( *iRtpMediaClock ); 
        }
        
    SendStreamEventToClient( KMccStreamPrepared );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkPlayL
// Start the playout operation.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkPlayL()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkPlayL IN" )
    if ( iSecureKeyExpired )
    	{
    	TRACE_RTP_SINK_PRINT( "Leave becuase the secure key expired" )
    	User::Leave( KErrGeneral );
    	}
    TMccRtpUser* user = FindRtpUserEntryForCurrent( *iEventHandler );
    __ASSERT_ALWAYS( user, User::Leave( KErrNotReady ) );
    
    SetStateL( ERtpStatePlaying );
    
    if ( user->iStartedOnce )
        {
        SendStreamEventToClient( KMccStreamResumed ); 
        }
    else
        {
        SendStreamEventToClient( KMccStreamStarted ); 
        user->iStartedOnce = ETrue;
        }

    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkPlayL OUT" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkPauseL
// Pauses the playout operation.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkPauseL()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkPauseL" )
    
    SetStateL( ERtpStatePaused );
    
    
    iSender->Clear();
    
    SendStreamEventToClient( KMccStreamPaused ); 
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkStopL
// Stop the playout operation. 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkStopL()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkStopL" )
    
    SetStateL( ERtpStateStopped );
    
    StopKeepalive();
    
    iSender->Clear();
    
    SendStreamEventToClient( KMccStreamStopped ); 
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SendMediaSignallingL()
// Sends media level signalling
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SendMediaSignallingL( const TMccEvent& aEvent )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SendMediaSignallingL" )
    
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
        case KRtcpSdesPacket:
            {
            TPckgBuf<TRtpSdesParams> package;
            package.Copy( rtcpEvent.iRtcpPacketData );
            iRtpAPI->SetLocalSdes( package() );
            break;
            }   
        case KRtcpByePacket:
            {
			__ASSERT_ALWAYS( KNullId != iRtpStreamId, User::Leave( KErrNotReady ) );
            User::LeaveIfError( 
                iRtpAPI->SendRtcpByePacket( iRtpStreamId, 
                    rtcpEvent.iRtcpPacketData ) );
            break;
            }
        case KRtcpAppPacket:
            {
			__ASSERT_ALWAYS( KNullId != iRtpStreamId, User::Leave( KErrNotReady ) );
			
            TPckgBuf<TRtcpApp> package;
            package.Copy( rtcpEvent.iRtcpPacketData );
			
            User::LeaveIfError( 
                iRtpAPI->SendRtcpAppPacket( iRtpStreamId, package() ) );
            
            break;
            }
        case KRtcpSrPacket:
            {
            TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SendMediaSignallingL, SR" )
            	
            __ASSERT_ALWAYS( KNullId != iRtpStreamId, User::Leave( KErrNotReady ) ); 
            User::LeaveIfError( iRtpAPI->SendRtcpSrPacket( iRtpStreamId ) );
            break;
            }
        case KRtcpAnyPacket:
            {
            TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SendMediaSignallingL, FC" )
            TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::SendMediaSignallingL, sessionid:%d", 
                                   iSessionID )
            TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::SendMediaSignallingL, datalen: %d", 
                                   rtcpEvent.iRtcpPacketData.Length() )
            
            __ASSERT_ALWAYS( iSender, User::Leave( KErrNotReady ) );
            TBool useRTPSocket( EFalse );
            iSender->SendDataL( iSessionID,
                                useRTPSocket,
                                rtcpEvent.iRtcpPacketData );
            break;
            }
        default:
            {
            TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SendMediaSignallingL, default" )
            User::Leave( KErrArgument );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::DoCreateStreamL
// Creates a transmit stream.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::DoCreateStreamL()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::DoCreateStreamL IN" )
    __ASSERT_ALWAYS( KNullId == iRtpStreamId, User::Leave( KErrAlreadyExists ) );
    __ASSERT_ALWAYS( NULL != iRtpAPI, User::Leave( KErrNotReady ) );
    
    if ( !iSender )
        {
        // Can do marker based cleanup only with video stream
        TBool doMarkerBasedCleanup( iMedia.iMediaType == KUidMediaTypeVideo );
        iSender = CMccRtpSender::NewL( 
                        *this, *iRtpAPI, iSessionID, doMarkerBasedCleanup );
        }
    
    TTranStreamParams transParams;
    transParams.iPayloadType = iCodecInfo.iPayloadType;
    iRtpStreamId =
    	iRtpAPI->CreateTransmitStreamL( iSessionID, transParams, iSSRC ); 
    
    if ( KNullId == iRtpStreamId )
        {
        TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::DoCreateStreamL KErrCouldNotConnect 1" )
        
        User::Leave( KErrCouldNotConnect );
        }
        
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::DoCreateStreamL SetSamplingRate 1" ) 
        
    User::LeaveIfError( iRtpAPI->SetSamplingRate( 
        iCodecInfo.iPayloadType, KDefSampleRate ) );

    if ( KMccPayloadTypeMax != iCodecInfo.iRedundantPayload )
        {
        User::LeaveIfError( iRtpAPI->SetSamplingRate( 
            iCodecInfo.iRedundantPayload, KDefSampleRate ) );
        }
   
    DoCreateSrtpStreamL();
            
  	TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::DoCreateStreamL OUT" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SendErrorOccured
// Asynchronous sending error occured
// -----------------------------------------------------------------------------
//    
void CMccRtpDataSink::SendErrorOccured( TInt aError )
    {
    TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::SendErrorOccured, err: %d", aError )
    
    SendInternalRtpEventToAllClients( iUsers,
                                      KMccRtpSinkUid, 
                                      EMccInternalRtpSinkError, 
                                      KMccStreamError,
                                      MCC_RTPSINK_ENDPOINT_ID,
                                      aError );
    
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SendErrorOccured, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::DoCreateSrtpStreamL
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::DoCreateSrtpStreamL()
	{
	TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::DoCreateSrtpStreamL IN" )
    if ( !iSrtpStream && iContext && iSecSession && KNullId != iRtpStreamId )
        {
        TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::DoCreateSrtpStreamL, creating" )
        iSrtpStream = CSRTPStreamOut::NewL( 
            *iSecSession, static_cast<TUint>( iSSRC ), iContext, *this );
        
        // Now it's possible to send keepalive
        StartKeepaliveL( *iRtpMediaClock );
        }
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::DoCreateSrtpStreamL OUT" )
	}
	
// ---------------------------------------------------------------------------
// CMccRtpDataSink::SinkCustomCommand()
// 
// ---------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkCustomCommand( TMMFMessage& aMessage )
    {
    TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::SinkCustomCommand - FUNCTION: %d",
            aMessage.Function() )

    TInt result( KErrNone );
    aMessage.Complete( result );
    }

// ---------------------------------------------------------------------------
// CMccRtpDataSink::SendRTCPData()
// Sends Non-RTCP data
// ---------------------------------------------------------------------------
//    
void CMccRtpDataSink::SendRTCPDataL( const TDesC8& aData )
    {
    if ( iRtpAPI && iSender )
        {
        TBool useRTPSocket( EFalse );
        iSender->SendDataL( iSessionID, useRTPSocket, aData );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::HandleBySsrc
// -----------------------------------------------------------------------------
//   
TBool CMccRtpDataSink::HandleBySsrc( const TRtpSSRC& aSsrc )
    {
    return ( iSSRC == aSsrc );
    }
    
// -----------------------------------------------------------------------------
// CMccRtpDataSink::RtpStreamId
// -----------------------------------------------------------------------------
//    
TRtpId CMccRtpDataSink::RtpStreamId()
    {
    return iRtpStreamId;
    }
    
// -----------------------------------------------------------------------------
// CMccRtpDataSink::IsSink
// -----------------------------------------------------------------------------
//
TBool CMccRtpDataSink::IsSink() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::IsSending
// -----------------------------------------------------------------------------
//        
TBool CMccRtpDataSink::IsSending() const
    {
    return ( ERtpStatePlaying == State() );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SendStreamEventToClient()
// -----------------------------------------------------------------------------
//	
void CMccRtpDataSink::SendStreamEventToClient( 
    TMccEventType aEventType,
    TInt aError )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSink::SendStreamEventToClient" )
    
    if ( iEventHandler )
	    {
	    TMccEvent event( 0, 
	                     0, 
	                     0, 
	                     MCC_ENDPOINT_ID( static_cast<MDataSink*>( this ) ), 
	                     KMccEventCategoryStream, 
	                     aEventType, 
	                     aError, 
	                     KNullDesC8 );

		TMccInternalEvent internalEvent( KMccRtpSinkUid, 
		                                 EMccInternalEventNone,
		                                 event );
		                         
		iEventHandler->SendEventToClient( internalEvent );
	    }
	else
		{
		TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSink::SendStreamEventToClient, \
		iEventHandler=NULL" )
		}
    }
 
// ---------------------------------------------------------------------------
// CMccRtpDataSink::FindRtpUserEntryForCurrent
// ---------------------------------------------------------------------------
//
TMccRtpUser* CMccRtpDataSink::FindRtpUserEntryForCurrent( 
    MAsyncEventHandler& aEventHandler )
    {
    TMccRtpUser entry( &aEventHandler );
    TIdentityRelation<TMccRtpUser> comparison( RtpUserMatch );
    TInt index = iUsers.Find( entry, comparison );
    if ( index != KErrNotFound )
        {
        return &iUsers[ index ];
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// CMccRtpDataSink::RemoveCurrentRtpUser
// ---------------------------------------------------------------------------
//
void CMccRtpDataSink::RemoveCurrentRtpUser()
    {
    TMccRtpUser entry( iEventHandler );
    TIdentityRelation<TMccRtpUser> comparison( RtpUserMatch );
    TInt index = iUsers.Find( entry, comparison );
    if ( index != KErrNotFound )
        {
        iUsers.Remove( index );
        }
    }
        
// ---------------------------------------------------------------------------
// CMccRtpDataSink::RtpUserMatch
// ---------------------------------------------------------------------------
//   
TBool CMccRtpDataSink::RtpUserMatch( 
    const TMccRtpUser& aUser1, 
    const TMccRtpUser& aUser2 )
    {
    // First argument is always the search term
    
    TBool match( EFalse );
    if ( aUser1.iEventHandler )
        {
        match = ( aUser1.iEventHandler == aUser2.iEventHandler );
        }
    return match;
    }

// ---------------------------------------------------------------------------
// CMccRtpDataSink::SendingAllowed
// ---------------------------------------------------------------------------
//   
TBool CMccRtpDataSink::SendingAllowed( 
        const TRtpSendHeader& aHeaderInfo ) const
    {
    // If secure session, srtp stream has to exist
    TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::Sending allowed iSecureKeyExpired = ",
        iSecureKeyExpired )
    
    TBool isPayloadMuted 
        = iMuteOn ? aHeaderInfo.iPayloadType != iExceptionPt : EFalse;
    
    return ( ERtpStatePlaying == State() && 
             ( !iSecSession || iSrtpStream ) &&
    		 !iSecureKeyExpired && 
    		 !isPayloadMuted );
    }

// ---------------------------------------------------------------------------
// FROM SRTP API
// This function is called by SRTP Stream initiated with 
// MSRTPReKeyingObserver when a master key is stale and needs 
// to be refreshed.  
// ---------------------------------------------------------------------------
//
void CMccRtpDataSink::SRTPMasterKeyStaleEvent( const CSRTPStream& aStream )
    {
    TRACE_RTP_INTERFACE_PRINT( "CMccRtpDataSink::SRTPMasterKeyStaleEvent" )
    
    if ( iSrtpStream == &aStream )
        {
        iSecureKeyExpired = ETrue;
        SendSecureRtpEventToClient( iEventHandler,
                                    KMccRtpSinkUid,
                                    EMccInternalEventNone,
                                    KMccMasterKeyStaled,
                                    MCC_RTPSINK_ENDPOINT_ID );   
        }
    else
        {
        TRACE_RTP_INTERFACE_PRINT( "CMccRtpDataSink::SRTPMasterKeyStaleEvent - Wrong stream" )
        }
    }

// ---------------------------------------------------------------------------
// FROM SRTP API
// This function is called by SRTP Stream initiated with 
// CSRTPSession  when a master key is stale and
// needs to be refreshed.  
// ---------------------------------------------------------------------------
void CMccRtpDataSink::SRTPMasterKeyStaleEvent(const CSRTPSession& aSession )
    {
    TRACE_RTP_INTERFACE_PRINT( "CMccRtpStream::SRTPMasterKeyStaleEvent" )
    
    if ( iSecSession == &aSession )
        {
        iSecureKeyExpired = ETrue;
        SendSecureRtpEventToClient( iEventHandler,
                                    KMccRtpSinkUid,
                                    EMccInternalEventNone,
                                    KMccMasterKeyStaled,
                                    MCC_RTPSINK_ENDPOINT_ID );    
        }
    else
        {
         TRACE_RTP_INTERFACE_PRINT( "MccRtpStream::SRTPMasterKeyStaleEvent - Wrong session" )
        }
    } 
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  

