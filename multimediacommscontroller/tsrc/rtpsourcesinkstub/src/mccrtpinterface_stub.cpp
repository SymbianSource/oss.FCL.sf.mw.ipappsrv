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
void MMccRtpInterface::SetSessionParamsL( const TMccRtpSessionParams& /*aParams*/ )
    {

    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::CheckBufferSupportL
// Check the buffer support
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::CheckBufferSupportL( CMMFBuffer* /*aBuffer*/ ) const
    {

    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::SetStateL
// Make transition to the new state if acceptable. Otherwise leave.
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::SetStateL( TSourceSinkState /*aState*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// MMccRtpInterface::CloseStreams
// close all RTP streams
// -----------------------------------------------------------------------------
//  
void MMccRtpInterface::CloseStreams()
	{ 

    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::ConfigureL
// Configures the RTP source/sink
// -----------------------------------------------------------------------------
//
void MMccRtpInterface::ConfigureL( const TDesC8& /*aConfig*/, CMccRtpMediaClock* /*aRtpMediaClock*/ )
    {
    if ( KNullId == RtpStreamId() )
        {
        this->DoCreateStreamL();
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
    if ( KNullId != streamId )
        {
        return KErrNone;
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
    if ( KNullId != streamId )
        {
        return KErrNone;
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
TInt MMccRtpInterface::StandBy( TMccStandbyActionType /*aActionType*/, TUint /*aPayloadType*/ )
    {
    return KErrNone;
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
TBool MMccRtpInterface::HandleByStreamId( const TRtpId& /*aStreamId*/ )
    {
    return ETrue;
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
// MMccRtpInterface::SendInternalRtpEventToClient
// -----------------------------------------------------------------------------
//   
void MMccRtpInterface::SendInternalRtpEventToClient( 
    MAsyncEventHandler* /*aEventHandler*/,
    TUid /*aEventOriginator*/,
    TMccInternalEventType /*aInternalEventType*/,
    TMccEventType /*aEventType*/,
    TUint32 /*aEndpointId*/,
    TInt /*aError*/, 
    TUint64 /*aJitterEstimate*/,
    TUint32 /*aPacketsReceived*/,
    TInt64 /*iPrevTransTime*/,
    TUint64 /*iTriggeredJitterLevel*/ )
    {

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
    
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::StartKeepaliveL
// -----------------------------------------------------------------------------
//     
void MMccRtpInterface::StartKeepaliveL( CMccRtpMediaClock& /*aRtpMediaClock*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::StopKeepalive
// -----------------------------------------------------------------------------
//    
void MMccRtpInterface::StopKeepalive()
    {
    
    }

// -----------------------------------------------------------------------------
// MMccRtpInterface::ResetKeepaliveTimer
// -----------------------------------------------------------------------------
//    
void MMccRtpInterface::ResetKeepaliveTimer()
    {

    }    

// -----------------------------------------------------------------------------
// MMccRtpInterface::ApplyCryptoContextL
// -----------------------------------------------------------------------------
// 
void MMccRtpInterface::ApplyCryptoContextL( const TMccCryptoContext& /*aContextParams*/ )
    {
    }
  
// -----------------------------------------------------------------------------
// MMccRtpInterface::CreateContext
// -----------------------------------------------------------------------------
// 
TInt MMccRtpInterface::RemoveCryptoContext()
    {
    return KErrNone;
    } 
        
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
