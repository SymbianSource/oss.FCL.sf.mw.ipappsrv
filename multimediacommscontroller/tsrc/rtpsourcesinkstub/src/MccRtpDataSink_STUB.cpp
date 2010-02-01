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
#include "mccrtpdatasink.h"
#include "mmccevents.h"
#include "mccinternaldef.h"
#include "mccrtpdefs.h"
#include "mmccinterfacedef.h"

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
void CMccRtpDataSink::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::ConstructSinkL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::~CMccRtpDataSink
// Destructor
// -----------------------------------------------------------------------------
//
CMccRtpDataSink::~CMccRtpDataSink()
    {
    TRACE_RTP_SINK_PRINT2( "CMccRtpDataSink::~CMccRtpDataSink 0x%x", this )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SetCurrentUser
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::Mute( TBool aMuteOn, TUint8 aExceptionPt )
    {
    TRACE_RTP_SINK_PRINT3( "CMccRtpDataSink::Mute, STATE: %d, PT: %u", 
        aMuteOn, aExceptionPt )
    
    iMuteOn = aMuteOn;
    iExceptionPt = aExceptionPt;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SetCurrentUser
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SetCurrentUser( MAsyncEventHandler* /*aEventHandler*/ )
    {
//    iEventHandler = aEventHandler;
    }
    
// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkThreadLogon
//
// Method to 'logon' the data sink to the same thread that sink will be consuming
// data in. Thread specific initialisation is done here.
// -----------------------------------------------------------------------------
//
TInt CMccRtpDataSink::SinkThreadLogon( MAsyncEventHandler& /*aEventHandler*/ ) 
    {
    TRACE_RTP_SINK_PRINT ( "CMccRtpDataSink::SinkThreadLogon" )
    return KErrNone;
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
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SetSinkDataTypeCode
// Sets the datatype code ( codec )
// ----------------------------------------------------------------------------- 
//
TInt CMccRtpDataSink::SetSinkDataTypeCode( TFourCC /*aCodec*/, TMediaId /*aMedia*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkDataTypeCode
// Returns the datatype code ( codec )
// -----------------------------------------------------------------------------
//
TFourCC CMccRtpDataSink::SinkDataTypeCode( TMediaId aMediaId )
    {
    if ( KUidMediaTypeAudio == aMediaId.iMediaType )
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
                                    TRtpSendHeader& /*aHeaderInfo*/ )
    {
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aSupplier, User::Leave( KErrArgument ) );
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::BufferFilledL
// NOT SUPPORTED. MDataSink pure virtual function must be implemented.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
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
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkPlayL
// Start the playout operation.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkPlayL()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkPlayL IN" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkPauseL
// Pauses the playout operation.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkPauseL()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkPauseL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SinkStopL
// Stop the playout operation. 
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkStopL()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkStopL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SendMediaSignallingL()
// Sends media level signalling
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::SendMediaSignallingL( const TMccEvent& /*aEvent*/ )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SendMediaSignallingL" )
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::DoCreateStreamL
// Creates a transmit stream.
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::DoCreateStreamL()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::DoCreateStreamL" )
    
    iRtpStreamId = 1;
    }


// ---------------------------------------------------------------------------
// CMccRtpDataSink::SinkCustomCommand()
// 
// ---------------------------------------------------------------------------
//
void CMccRtpDataSink::SinkCustomCommand( TMMFMessage& /*aMessage*/ )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SinkCustomCommand" )
    }

// ---------------------------------------------------------------------------
// CMccRtpDataSink::SendRTCPData()
// Sends Non-RTCP data
// ---------------------------------------------------------------------------
//    
void CMccRtpDataSink::SendRTCPDataL( const TDesC8& /*aData*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccRtpDataSink::HandleBySsrc
// -----------------------------------------------------------------------------
//   
TBool CMccRtpDataSink::HandleBySsrc( const TRtpSSRC& /*aSsrc*/ )
    {
    return ETrue;
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
// CMccRtpDataSink::DoCreateSrtpStreamL
// -----------------------------------------------------------------------------
// 
void CMccRtpDataSink::DoCreateSrtpStreamL()
    {
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
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CMccRtpDataSink::AddUserL
// -----------------------------------------------------------------------------
//
void CMccRtpDataSink::AddUserL( TUint32 /*aStreamId*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CMccRtpDataSink::SendStreamEventToClient()
// -----------------------------------------------------------------------------
//	
void CMccRtpDataSink::SendStreamEventToClient( 
    TMccEventType /*aEventType*/,
    TInt /*aError*/ )
    {
    TRACE_RTP_SOURCE_PRINT( "CMccRtpDataSink::SendStreamEventToClient" )
    }
    
// ---------------------------------------------------------------------------
// CMccRtpDataSink::SetMediaClock
// ---------------------------------------------------------------------------
//    
void CMccRtpDataSink::SetMediaClock( CMccRtpMediaClock& /*aRtpMediaClock*/ )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpDataSink::SetMediaClock" )
    
    //iRtpMediaClock = &aRtpMediaClock;   
    }    
    
// ---------------------------------------------------------------------------
// FROM SRTP API
// This function is called by SRTP Stream initiated with 
// MSRTPReKeyingObserver when a master key is stale and needs 
// to be refreshed.  
// ---------------------------------------------------------------------------
//
void CMccRtpDataSink::SRTPMasterKeyStaleEvent( const CSRTPStream& /*aStream*/ )
    {
    }

// ---------------------------------------------------------------------------
// FROM SRTP API
// This function is called by SRTP Stream initiated with 
// CSRTPSession  when a master key is stale and
// needs to be refreshed.  
// ---------------------------------------------------------------------------
void CMccRtpDataSink::SRTPMasterKeyStaleEvent(const CSRTPSession& /*aSession*/ )
    {
    } 
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  

