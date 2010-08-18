/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Method implementations common to RTP data source and -sink.
*
*/





// INCLUDE FILES
#include "rtpapi.h"
#include "mccrtpinterface.h"
#include "mccrtpdefs.h"
#include "mccinternalevents.h"
#include "mccrtpkeepalivecontainer.h"

#include "mmcccryptocontext.h"
#include "mmcccryptoadapter.h"

#include <srtpcryptocontext.h>
#include <srtpstream.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// MMccRtpInterface::SetSessionParamsL
// Set session level parameters.
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::SetSessionParamsL( const TMccRtpSessionParams& aParams )
    {
    TRACE_RTP_INTERFACE_PRINT( "MMccRtpInterface::SetSessionParamsL IN" )
    
    User::LeaveIfNull( aParams.iRtpAPI );
    User::LeaveIfNull( aParams.iRtpKeepalive );
    
    iRtpAPI = aParams.iRtpAPI;
    iEnableRtcp = aParams.iEnableRTCP;
    iSessionID = aParams.iSessionId;
    iRtpKeepalive = aParams.iRtpKeepalive;
    iSecSession = aParams.iSecSession;
    
    TRACE_RTP_INTERFACE_PRINT( "MMccRtpInterface::SetSessionParamsL OUT" )
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::CheckBufferSupportL
// Check the buffer support
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::CheckBufferSupportL( CMMFBuffer* aBuffer ) const
    {
    User::LeaveIfNull( aBuffer );
    TUid type = aBuffer->Type();
    
    __ASSERT_ALWAYS( type == KUidMmfDataBuffer ||
                     type == KUidMmfTransferBuffer ||
                     type == KUidMmfAudioBuffer ||
                     type == KUidMmfPtrBuffer,
                     User::Leave( KErrNotSupported ) );
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::SetStateL
// Make transition to the new state if acceptable. Otherwise leave.
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::SetStateL( TSourceSinkState aState )
    {
    TRACE_RTP_INTERFACE_PRINT3( "MMccRtpInterface::SetStateL - OLD STATE: %d, NEW STATE: %d",
        iState, aState )
    
    TBool transitionOk( EFalse );
    if ( aState == iState )
        {
        TRACE_RTP_INTERFACE_PRINT( "MMccRtpInterface::SetStateL - WARNING STATES ARE SAME" )
        transitionOk = ETrue;
        }
        
    switch ( aState )
        {
        case ERtpStateConstructed:
            transitionOk = ETrue;
            break;

        case ERtpStateStopped:
            transitionOk = ETrue;
            break;

        case ERtpStatePrimed:
            if ( KNullId != RtpStreamId() && iRtpAPI )
                {
                if ( ERtpStateStopped == iState || ERtpStateConstructed == iState )
                    {
                    transitionOk = ETrue;
                    }
                else if ( ERtpStatePlaying == iState || ERtpStatePaused == iState )
                    {
                    // Rtp component might be already started by some other user,
                    // and preparing is not therefore anymore needed
                    TRACE_RTP_INTERFACE_PRINT( "MMccRtpInterface::SetStateL - PREPARED BY OTHER USER" )
                    aState = iState;
                    transitionOk = ETrue;
                    }
                else
                    {
                    // NOP
                    }
                }
            break;

        case ERtpStatePlaying:
            if ( ( ERtpStatePrimed == iState || ERtpStatePaused == iState )
            	 && KNullId != RtpStreamId() && iRtpAPI )
                {
                transitionOk = ETrue;
                }
            break;

        case ERtpStatePaused:
            if ( (ERtpStatePlaying == iState )
            	 && KNullId != RtpStreamId() && iRtpAPI )
                {
                transitionOk = ETrue;
                }
            break;
          
        default:
            TRACE_RTP_INTERFACE_PRINT( "MMccRtpInterface::SetStateL - DEFAULT SWITCH CASE" )
            break;            
        }

    if ( transitionOk )
        {
        iState = aState;
        }
    else
        {
        #ifdef TRACE_RTP_INTERFACE
            TRACE_RTP_INTERFACE_PRINT( "MMccRtpInterface::SetStateL - LEAVE KErrNotReady" )
          	TRACE_RTP_INTERFACE_PRINT2( "MMccRtpInterface::SetStateL - LEAVE iStreamID: %d", RtpStreamId() )
            TRACE_RTP_INTERFACE_PRINT2( "MMccRtpInterface::SetStateL - LEAVE iRtpAPI: 0x%x", iRtpAPI )
        #endif
        				
       	__ASSERT_ALWAYS (transitionOk, User::Leave (KErrNotReady));
        }
        
    TRACE_RTP_INTERFACE_PRINT2( "MMccRtpInterface::SetStateL - EXIT WITH STATE: %d", iState )
    }
    
// -----------------------------------------------------------------------------
// MMccRtpInterface::CloseStreams
// close all RTP streams
// -----------------------------------------------------------------------------
//  
void MMccRtpInterface::CloseStreams()
	{ 
   	if ( iRtpAPI )
   	    {
   	    iRtpAPI->CloseStream( RtpStreamId() );
   	    }
    if ( !iSrtpStream )
        {
        delete iContext;
        }
    iContext = NULL;
    
    delete iSrtpStream;
    iSrtpStream = NULL;
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::ConfigureL
// Configures the RTP source/sink
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::ConfigureL( const TDesC8& aConfig, 
									CMccRtpMediaClock* aRtpMediaClock )
    {
    TRACE_RTP_INTERFACE_PRINT( "MMccRtpInterface::ConfigureL, Entry" )
    __ASSERT_ALWAYS( NULL != iRtpKeepalive, User::Leave( KErrNotReady ) );
    
    if ( !iRtpMediaClock )
        {
        iRtpMediaClock = aRtpMediaClock;
        }
    
    if ( aConfig.Size() == sizeof( TMccCodecInfo ) )
        {
        TMccCodecInfoBuffer infoBuffer;
        infoBuffer.Copy( aConfig );
        const TMccCodecInfo& codecInfo( infoBuffer() );
        
        if ( KNullId == RtpStreamId() )
            {
            iCodecInfo = codecInfo;
            DoCreateStreamL();
            }
        else
            {
            // Keep latest valid keep-alive configuration. Keep-alive can be
            // kept disabled by not configuring interval at any phase.
            TBool updateNeeded = 
                ( iCodecInfo.iKeepalivePT != codecInfo.iKeepalivePT ||
                iCodecInfo.iKeepaliveInterval != codecInfo.iKeepaliveInterval ||
                iCodecInfo.iKeepaliveData != codecInfo.iKeepaliveData ) &&
                0 != codecInfo.iKeepaliveInterval ||
				iRtpKeepaliveUpdatePending;
            
            if ( updateNeeded )
                {
				if ( iRtpMediaClock )
					{
	                TRACE_RTP_INTERFACE_PRINT( 
	                    "MMccRtpInterface::ConfigureL - Update" )
	                
	                iCodecInfo.iKeepalivePT = codecInfo.iKeepalivePT;
	                iCodecInfo.iKeepaliveInterval = codecInfo.iKeepaliveInterval;
	                iCodecInfo.iKeepaliveData = codecInfo.iKeepaliveData;
	                __ASSERT_ALWAYS( NULL != iRtpMediaClock, User::Leave( KErrNotReady ) );
	                iRtpKeepalive->UpdateParamsL( *this, codecInfo, *iRtpMediaClock );
					
					iRtpKeepaliveUpdatePending = EFalse;
					}
				else
					{
					iRtpKeepaliveUpdatePending = ETrue;
					}
                }
            }
        }
    else
        {
        TRACE_RTP_INTERFACE_PRINT( "MMccRtpInterface::ConfigureL KErrArgument" )
        User::Leave( KErrArgument );
        }
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::SendRTCPReceiverReport
// Sends RTCP receiver report to uplink if possible.
// -----------------------------------------------------------------------------
//
TInt MMccRtpInterface::SendRTCPReceiverReport()
    {
    TRtpId streamId = RtpStreamId();
    if ( iRtpAPI && KNullId != streamId )
        {
        return iRtpAPI->SendRtcpRrPacket( streamId );
        }
    else
        {
        return KErrNotReady;
        }
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::SendRTCPSenderReport
// Sends RTCP sender report to uplink if possible.
// -----------------------------------------------------------------------------
//
TInt MMccRtpInterface::SendRTCPSenderReport()
    {
    TRtpId streamId = RtpStreamId();
    if ( iRtpAPI && KNullId != streamId )
        {
        return iRtpAPI->SendRtcpSrPacket( streamId );
        }
    else
        {
        return KErrNotReady;
        }
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::SendRTCPDataL
// Sends RTCP sender report to uplink if possible.
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::SendRTCPDataL( const TDesC8& /*aData*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::StartInactivityTimerL
// Starts inactivity timer for a stream
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::StartInactivityTimerL( TUint32 /*aTimeoutTime*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// MMccRtpInterface::StopInactivityTimerL
// Stops inactivity timer for a stream
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::StopInactivityTimerL( )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::StandBy
// -----------------------------------------------------------------------------
//
TInt MMccRtpInterface::StandBy( 
    TMccStandbyActionType /*aActionType*/, TUint /*aPayloadType*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::HandleBySsrc
// -----------------------------------------------------------------------------
//   
TBool MMccRtpInterface::HandleBySsrc( const TRtpSSRC& /*aSsrc*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::HandleByStreamId
// -----------------------------------------------------------------------------
//        
TBool MMccRtpInterface::HandleByStreamId( const TRtpId& aStreamId )
    {
    return ( RtpStreamId() == aStreamId );
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::IsSink
// -----------------------------------------------------------------------------
//
TBool MMccRtpInterface::IsSink() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::IsSending
// -----------------------------------------------------------------------------
//        
TBool MMccRtpInterface::IsSending() const
    {
    return EFalse;
    }
        
// -----------------------------------------------------------------------------
// MMccRtpInterface::SendJitterEventToClient
// -----------------------------------------------------------------------------
// 
void MMccRtpInterface::SendJitterEventToClient( MAsyncEventHandler* aEventHandler,
				                              	TUid aEventOriginator,
				                            	TMccInternalEventType aInternalEventType,
				                            	TMccEventType aEventType,
				                            	TUint32 aEndpointId,
				                            	TInt aError, 
				                            	TUint64 aJitterEstimate,
				                           	 	TUint32 aPacketsReceived,
				                            	TInt64 aPrevTransTime,
				                            	TUint64 aTriggeredJitterLevel,
												TInt aPacketLoss,
												TInt aTriggeredPacketLoss )
	{
	
	TRACE_RTP_SOURCE_PRINT( "MMccRtpInterface::SendJitterEventToClient" )
    
    if ( aEventHandler )
        {
        ClearMccEvent();
        
        iMccEvent.iEventCategory = KMccEventCategoryRtp;
        iMccEvent.iStreamId = iMccStreamId;
        iMccEvent.iEndpointId = aEndpointId;
        iMccEvent.iEventType = aEventType;
        iMccEvent.iErrorCode = aError;
        
        // Save some stack memory
        {
        TMccRtpEventDataExtended eventData;
        eventData.iJitterEstimate = aJitterEstimate;
        eventData.iPacketsReceived = aPacketsReceived;
        eventData.iPrevTransTime = aPrevTransTime;
        eventData.iTriggeredJitterLevel = aTriggeredJitterLevel;
        eventData.iPacketLoss = aPacketLoss;
        eventData.iTriggeredPacketLoss = aTriggeredPacketLoss;
        
        iMccEvent.iEventData.Copy( TMccRtpEventDataExtendedPackage( eventData ) );
        }
        
        TMccInternalEvent internalEvent( aEventOriginator, 
                                         aInternalEventType,
                                         iMccEvent );
        
        aEventHandler->SendEventToClient( internalEvent );
        }
    else
        {
	    TRACE_RTP_INTERFACE_PRINT("MMccRtpInterface::SendInternalRtpEventToClient, \
aEventHandler=NULL")
        }  
	}        
        
// -----------------------------------------------------------------------------
// MMccRtpInterface::SendInternalRtpEventToClient
// -----------------------------------------------------------------------------
//   
void MMccRtpInterface::SendInternalRtpEventToClient( 
    MAsyncEventHandler* aEventHandler,
    TUid aEventOriginator,
    TMccInternalEventType aInternalEventType,
    TMccEventType aEventType,
    TUint32 aEndpointId,
    TInt aError, 
    TUint64 aJitterEstimate,
    TUint32 aPacketsReceived,
    TInt64 aPrevTransTime,
    TUint64 aTriggeredJitterLevel )
    {
    TRACE_RTP_SOURCE_PRINT( "MMccRtpInterface::SendInternalRtpEventToClient" )
    
    if ( aEventHandler )
        {
        ClearMccEvent();
        
        iMccEvent.iEventCategory = KMccEventCategoryRtp;
        iMccEvent.iStreamId = iMccStreamId;
        iMccEvent.iEndpointId = aEndpointId;
        iMccEvent.iEventType = aEventType;
        iMccEvent.iErrorCode = aError;
        
        // Save some stack memory
        {
        TMccRtpEventData eventData;
        eventData.iJitterEstimate = aJitterEstimate;
        eventData.iPacketsReceived = aPacketsReceived;
        eventData.iPrevTransTime = aPrevTransTime;
        eventData.iTriggeredJitterLevel = aTriggeredJitterLevel;
        
        iMccEvent.iEventData.Copy( TMccRtpEventDataPackage( eventData ) );
        }
        
        TMccInternalEvent internalEvent( aEventOriginator, 
                                         aInternalEventType,
                                         iMccEvent );
        
        aEventHandler->SendEventToClient( internalEvent );
        }
    else
        {
	    TRACE_RTP_INTERFACE_PRINT("MMccRtpInterface::SendInternalRtpEventToClient, \
aEventHandler=NULL")
        }
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::SendInternalRtpEventToAllClients
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::SendInternalRtpEventToAllClients( 
    RArray<TMccRtpUser>& aUsers,
    TUid aEventOriginator,
    TMccInternalEventType aInternalEventType,
    TMccEventType aEventType,
    TUint32 aEndpointId,
    TInt aError,
    TUint64 aJitterEstimate,
    TUint32 aPacketsReceived,
    TInt64 aPrevTransTime,
    TUint64 aTriggeredJitterLevel )
    {
    TRACE_RTP_SOURCE_PRINT( "MMccRtpInterface::SendInternalRtpEventToAllClients" )
    
    TInt index( aUsers.Count() );
    while ( index-- )
        {
        SendInternalRtpEventToClient( aUsers[index].iEventHandler, aEventOriginator, 
            aInternalEventType, aEventType, aEndpointId, aError, aJitterEstimate,
            aPacketsReceived, aPrevTransTime, aTriggeredJitterLevel );
        }
    }
    
// -----------------------------------------------------------------------------
// MMccRtpInterface::ClearMccEvent
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::ClearMccEvent()
    {
    iMccEvent = TMccEvent();
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::StartKeepaliveL
// -----------------------------------------------------------------------------
//     
void MMccRtpInterface::StartKeepaliveL( CMccRtpMediaClock& aRtpMediaClock )
    {
    if ( iRtpKeepalive )
        {
        iRtpKeepalive->StartKeepaliveL( *this, iCodecInfo, aRtpMediaClock );
        }
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::StopKeepalive
// -----------------------------------------------------------------------------
//    
void MMccRtpInterface::StopKeepalive()
    {
    if ( iRtpKeepalive )
        {
        iRtpKeepalive->StopKeepalive( *this, iCodecInfo );
        }
    }
    
// -----------------------------------------------------------------------------
// MMccRtpInterface::ResetKeepaliveTimer
// -----------------------------------------------------------------------------
//    
void MMccRtpInterface::ResetKeepaliveTimer()
    {
    if ( iRtpKeepalive )
        {
        iRtpKeepalive->ResetKeepaliveTimer();
        }
    }    
    
// -----------------------------------------------------------------------------
// MMccRtpInterface::ApplyCryptoContextL
// -----------------------------------------------------------------------------
// 
void MMccRtpInterface::ApplyCryptoContextL( const TMccCryptoContext& aContextParams )
    {
    if ( iSrtpStream )
        {
        TMccCryptoAdapter::UpdateCryptoContextL( aContextParams, *iContext );
        }
    else
        {
        delete iContext;
        iContext = NULL;
        iContext = TMccCryptoAdapter::CreateCryptoContextL( aContextParams );
        iContextId = aContextParams.ContextId();
        
        DoCreateSrtpStreamL();
        }
    }
  
// -----------------------------------------------------------------------------
// MMccRtpInterface::CreateContext
// -----------------------------------------------------------------------------
// 
TInt MMccRtpInterface::RemoveCryptoContext()
    {
    if ( !iSrtpStream )
        {
        delete iContext;
        } 
        
    delete iSrtpStream;
    iSrtpStream = NULL;
    iContext = NULL;
    
    return KErrNone;
    }    

// -----------------------------------------------------------------------------
// MMccRtpInterface::SendSecureRtpEventToClient
// -----------------------------------------------------------------------------
//   
void MMccRtpInterface::SendSecureRtpEventToClient( 
    MAsyncEventHandler* aEventHandler,
    TUid aEventOriginator,
    TMccInternalEventType aInternalEventType,
    TMccEventType aEventType,
    TUint32 aEndpointId )
    {
    TRACE_RTP_SOURCE_PRINT( "MMccRtpInterface::SendSecureRtpEventToClient" )
    
    if ( aEventHandler )
        {
        ClearMccEvent();
        
        iMccEvent.iEventCategory = KMccEventCategoryRtp;
        iMccEvent.iStreamId = iMccStreamId;
        iMccEvent.iEndpointId = aEndpointId;
        iMccEvent.iEventType = aEventType;
        iMccEvent.iEventNumData = iContextId;
        
        
        TMccInternalEvent internalEvent( aEventOriginator, 
                                         aInternalEventType,
                                         iMccEvent );
        
        aEventHandler->SendEventToClient( internalEvent );
        }
    else
        {
	    TRACE_RTP_INTERFACE_PRINT("MMccRtpInterface::SendSecureRtpEventToClient, \
        aEventHandler=NULL")
        }
    }  
                 
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
