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
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfbuffer.h>
#include "rtpheader.h"
#include "mccsinkitem.h"
#include "formatstatemachine.h"
#include "mccmultiplexerlogs.h"
#include "mccinternaldef.h"
#include "mccinternalevents.h"
#include "mccuids.hrh"

// MACROS

#define MCC_PAYLOAD_READ( a ) static_cast<CPayloadFormatRead*>( a )

// LOCAL FUNCTION PROTOTYPES

// LOCAL CONSTANTS

const TInt KMccBadDataCount = 20;

const TInt KMccMaxStoredBuffers = 10;

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccSinkItem::NewLC
// -----------------------------------------------------------------------------
//
CMccSinkItem* CMccSinkItem::NewLC( 
    MDataSink* aSink, 
    TBool aSinkIsDecoder,
    TUid aMediaType,
    TBool aPassAllBuffersSink )
    {
    CMccSinkItem* self = 
        new ( ELeave ) CMccSinkItem( aMediaType, aPassAllBuffersSink );
    CleanupStack::PushL( self );
    self->ConstructL( aSink, aSinkIsDecoder );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::~CMccSinkItem
// -----------------------------------------------------------------------------
//    
CMccSinkItem::~CMccSinkItem()
    {
    iStoredBuffers.ResetAndDestroy();
    iPayloadsToAccept.Close();
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::BufferFilledL
// -----------------------------------------------------------------------------
//    
void CMccSinkItem::BufferFilledL( 
    CMMFBuffer* aBuffer, 
    const TRtpRecvHeader* aHeaderInfo,
    TBool aPrimary,
    TUid aMediaType )
    {
    __MULTIPLEXER_INT1( "CMccSinkItem::BufferFilledL, sinkitem:", 
                        reinterpret_cast<TUint32>( this ) )

    CMMFBuffer* completedBuffer = NULL;
    
    CMMFDataBuffer* dataBuffer = static_cast<CMMFDataBuffer*>( aBuffer );
    
    if ( aMediaType != KNullUid && MediaType() != aMediaType )
        {
        __MULTIPLEXER( "CMccSinkItem::BufferFilledL, wrong media type" )
        return;
        }
    
    if ( HandleBadDataL( dataBuffer ) )
        {
        __MULTIPLEXER( "CMccSinkItem::BufferFilledL, stopping buffer forwarding" )
        return;
        }
    
    if ( aHeaderInfo
        && ( KErrNotFound != iPayloadsToAccept.Find( aHeaderInfo->iPayloadType ) ) )
        {
        __MULTIPLEXER( "CMccSinkItem::BufferFilledL, payload type demultiplexing" )
        
        if ( iRequestedBuffer )
            {
            __MULTIPLEXER( "CMccSinkItem::BufferFilledL, fillbuffer request exists" )
            
            CopyBufferL( iRequestedBuffer, aBuffer );
            completedBuffer = iRequestedBuffer;
            iRequestedBuffer = NULL;
            }
        }
    else if ( iPassAllBuffersSink )
        {
        __MULTIPLEXER( "CMccSinkItem::BufferFilledL, in passall buffers mode" )
        
        // Do not zero the request, sink will eat all buffers
        // once it has requested for one
        if ( iRequestedBuffer )
            {
            completedBuffer = aBuffer;
            }
        }
    else if ( iPassAllRequestsSource )
        {
        __MULTIPLEXER( "CMccSinkItem::BufferFilledL, in passall requests mode" )
        
        // If in passall mode, buffer received from source must match with the
        // one the sink gave when requesting the buffer fill
        if ( iRequestedBuffer &&
             iRequestedBuffer == aBuffer )
            {
            __MULTIPLEXER( "CMccSinkItem::BufferFilledL, existing fillbuffer request" )
            
            completedBuffer = aBuffer;
            iRequestedBuffer = NULL;
            }
        }
    else if ( iRequestedBuffer && aPrimary )
        {
        __MULTIPLEXER( "CMccSinkItem::BufferFilledL, existing fillbuffer request for primary" )
        
        completedBuffer = aBuffer;
        iRequestedBuffer = NULL;
        }
    else
        {
        if ( iRequestedBuffer )
            {
            __MULTIPLEXER( "CMccSinkItem::BufferFilledL, existing fillbuffer request" )
            
            // If secondary sink has already requested buffer fill, the request
            // can be completed immediately
            CopyBufferL( iRequestedBuffer, aBuffer );
            completedBuffer = iRequestedBuffer;
            iRequestedBuffer = NULL;
            }
        else
            {
            // Otherwise the buffer has to be "stored"
            StoreBufferL( aBuffer );
            }
        }
    
    if ( completedBuffer )
        {    
        if ( iDecoder && aHeaderInfo )
            {
            iDecoder->DataBufferFilledL( completedBuffer, *aHeaderInfo );
            }
        else
            {
            Sink()->BufferFilledL( completedBuffer );
            }
        }
        
    __MULTIPLEXER( "CMccSinkItem::BufferFilledL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::FillBufferRequestL
// -----------------------------------------------------------------------------
//    
void CMccSinkItem::FillBufferRequestL( CMMFBuffer* aBuffer )
    {
    __MULTIPLEXER_INT1( "CMccSinkItem::FillBufferRequestL, sinkitem:", 
                        reinterpret_cast<TUint32>( this ) )
    
    CMMFBuffer* storedBuffer = GetStoredBuffer();
    if ( storedBuffer )
        {
        __MULTIPLEXER( "CMccSinkItem::FillBufferRequestL, stored buffer exists" )
        
        CleanupStack::PushL( storedBuffer );
        CopyBufferL( aBuffer, storedBuffer );
        CleanupStack::PopAndDestroy( storedBuffer );
        
        Sink()->BufferFilledL( aBuffer );
        }
    else
        {
        __MULTIPLEXER( "CMccSinkItem::FillBufferRequestL, store fillbuffer request" )
        
        iRequestedBuffer = aBuffer;
        }
        
    __MULTIPLEXER( "CMccSinkItem::FillBufferRequestL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::Match
// -----------------------------------------------------------------------------
//                            
TBool CMccSinkItem::Match( MDataSink* aSink )
    {
    return ( Sink() == aSink );
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::SourceThreadLogon
// -----------------------------------------------------------------------------
//
TInt CMccSinkItem::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
    {
    iEventHandler = &aEventHandler;
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccSinkItem::SendEventToClient
// -----------------------------------------------------------------------------
//    
TInt CMccSinkItem::SendEventToClient( const TMMFEvent& aEvent )
    {
    __MULTIPLEXER( "CMccSinkItem::SendEventToClient" )
    
    // Filter duplicate stream state change events. Errors are always reported.
    // Check also whether the event is targeted for specific payload type.
    //
    
    TInt ret( KErrNotReady );
    TBool sendEvent( ETrue );

  	if ( IS_MCC_EVENT( aEvent ) )
	    {
	    TMccEvent* mccEventPtr = 
	        reinterpret_cast<const TMccInternalEvent&>( aEvent ).iMccEvent;  
	        
	    if ( mccEventPtr && MCC_STREAM_STATE_CHANGE_EVENT( mccEventPtr ) &&
	         !mccEventPtr->iErrorCode )
	        {
	        sendEvent = HandleStreamStateChangeEvent( *mccEventPtr );
	        ret = KErrNone;
	        }
	    else if ( mccEventPtr && 
	              mccEventPtr->iEventNumData == KMccPayloadSpecificEvent &&
	              mccEventPtr->iReserved != KMccPTNotDefined )
	        {
	        __MULTIPLEXER_INT1( "CMccSinkItem::SendEventToClient, event for payload type:",
	                            mccEventPtr->iReserved )
	                            
	        sendEvent = 
	            ( iPayloadsToAccept.Find( mccEventPtr->iReserved ) != KErrNotFound );
	        }
	    else
	        {
	        // NOP
	        }
	    }
    
    if ( iEventHandler && sendEvent )
        {
        ret = iEventHandler->SendEventToClient( aEvent );
        }
    else
        {
        __MULTIPLEXER( "CMccSinkItem::SendEventToClient, dropping event" )
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::MediaType
// -----------------------------------------------------------------------------
//     
TUid CMccSinkItem::MediaType() const
    {
    return iMediaType;
    }
    
// -----------------------------------------------------------------------------
// CMccSinkItem::RegisterPayloadTypesL
// -----------------------------------------------------------------------------
//     
void CMccSinkItem::RegisterPayloadTypesL( const RArray<TUint>& aPayloadTypes )
    {
    __MULTIPLEXER( "CMccSinkItem::RegisterPayloadTypesL" )
    
    TInt ind = aPayloadTypes.Count();
    while ( ind-- )
        {
        if ( KErrNotFound == iPayloadsToAccept.Find( aPayloadTypes[ind]) )
            {
            iPayloadsToAccept.AppendL( aPayloadTypes[ind] );
            }
        else
            {
            User::Leave( KErrAlreadyExists );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::RegisteredPayloadTypes
// -----------------------------------------------------------------------------
//
const RArray<TUint>& CMccSinkItem::RegisteredPayloadTypes() const
    {
    return iPayloadsToAccept;
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::RequestedBuffer
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccSinkItem::RequestedBuffer()
    {
    return iRequestedBuffer;
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::EventHandler
// -----------------------------------------------------------------------------
//
MAsyncEventHandler* CMccSinkItem::EventHandler()
    {
    return iEventHandler;
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::SetMultiplexerState
// -----------------------------------------------------------------------------
//
void CMccSinkItem::SetMultiplexerState( CMccMultiplexer::TMccMultiplexerState aState )
    {
    __MULTIPLEXER_INT2( "CMccSinkItem::SetMultiplexerState, sinkitem:", 
                        reinterpret_cast<TUint32>( this ),
                        " state:", aState )
    iState = aState;
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::MultiplexerState
// -----------------------------------------------------------------------------
//
CMccMultiplexer::TMccMultiplexerState CMccSinkItem::MultiplexerState() const
    {
    return iState;
    }
    
// -----------------------------------------------------------------------------
// CMccSinkItem::SetSourceMode
// -----------------------------------------------------------------------------
//    
void CMccSinkItem::SetSourceMode( TBool aPassAllRequestsSource )
    {
    __MULTIPLEXER_INT1( "CMccSinkItem::SetSourceMode, sinkitem:", 
                        reinterpret_cast<TUint32>( this ) )
    __MULTIPLEXER_INT1( "CMccSinkItem::SetSourceMode, mediatype:", 
                        iMediaType.iUid )
    __MULTIPLEXER_INT1( "CMccSinkItem::SetSourceMode, mode:", 
                        aPassAllRequestsSource )
    
    iPassAllRequestsSource = aPassAllRequestsSource;
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::PassAllRequestsToSource
// -----------------------------------------------------------------------------
//     
TBool CMccSinkItem::PassAllRequestsToSource() const
    {
    return iPassAllRequestsSource;
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::CMccSinkItem
// -----------------------------------------------------------------------------
//     
TUid CMccSinkItem::ResolveMediaType( CMMFBuffer* aBuffer ) const
    {
    if ( aBuffer && iPassAllRequestsSource && iRequestedBuffer == aBuffer )
        {
        return MediaType();
        }
    return KNullUid;
    }
    
// -----------------------------------------------------------------------------
// CMccSinkItem::CMccSinkItem
// -----------------------------------------------------------------------------
//   
CMccSinkItem::CMccSinkItem( TUid aMediaType, TBool aPassAllBuffersSink ) :
    iMediaType( aMediaType ),
    iPassAllBuffersSink( aPassAllBuffersSink ),
    iReportedEvent( KMccEventNone ), iBadDataCount( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::Sink
// -----------------------------------------------------------------------------
//       
void CMccSinkItem::ConstructL( MDataSink* aSink, TBool aSinkIsDecoder )
    {
    __MULTIPLEXER( "CMccSinkItem::ConstructL" )
    
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    
    if ( aSinkIsDecoder )
        {
        __MULTIPLEXER( "CMccSinkItem::ConstructL, sink is decoder" )
        
        CMMFFormatDecode* decoder = static_cast<CMMFFormatDecode*>( aSink ); 
        
        iDecoder = MCC_PAYLOAD_READ( decoder );
        }
    else
        {
        iSink = aSink;
        }
        
    __MULTIPLEXER( "CMccSinkItem::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSinkItem::Sink
// -----------------------------------------------------------------------------
//    
MDataSink* CMccSinkItem::Sink()
    {
    if ( iDecoder )
        {
        return iDecoder;
        }
    
    return iSink;
    }
    
// ---------------------------------------------------------------------------
// CMccSinkItem::CopyBufferL
// ---------------------------------------------------------------------------
//  
void CMccSinkItem::CopyBufferL( 
    CMMFBuffer* aDesBuffer,
    CMMFBuffer* aOrigBuffer )
	{
	__ASSERT_ALWAYS( aDesBuffer && aOrigBuffer, User::Leave( KErrArgument ) );
	
	// Copy buffer setting
    aDesBuffer->SetFrameNumber( aOrigBuffer->FrameNumber() );
    aDesBuffer->SetLastBuffer( aOrigBuffer->LastBuffer() );
    aDesBuffer->SetPosition( aOrigBuffer->Position() );
     // Note: setting the status to available clears the buffer data
    aDesBuffer->SetStatus( aOrigBuffer->Status() );
    aDesBuffer->SetTimeToPlay( aOrigBuffer->TimeToPlay() );
    	
	// Copy buffer data
	CMMFDataBuffer* desBuffer = static_cast<CMMFDataBuffer*>( aDesBuffer );
	CMMFDataBuffer* origBuffer = static_cast<CMMFDataBuffer*>( aOrigBuffer );
	
	__ASSERT_ALWAYS( desBuffer->Data().MaxLength() >= (TInt) origBuffer->BufferSize(), 
	                 User::Leave( KErrOverflow ) );
	
    desBuffer->Data().Copy( origBuffer->Data() );
	}

// ---------------------------------------------------------------------------
// CMccSinkItem::IsInternalDecoder
// ---------------------------------------------------------------------------
//  
TBool CMccSinkItem::IsInternalDecoder( TUid aDecoderUid ) const
    {
    TUint32 decoderUid = aDecoderUid.iUid;
    return ( decoderUid == KImplUidAmrPayloadFormatDecode ||
             decoderUid == KImplUidAnyPayloadFormatDecode ||
             decoderUid == KImplUidDTMFPayloadFormatDecode ||
             decoderUid == KImplUidRedPayloadFormatDecode );
    
    }

// ---------------------------------------------------------------------------
// CMccSinkItem::StoreBufferL
// ---------------------------------------------------------------------------
//     
void CMccSinkItem::StoreBufferL( CMMFBuffer* aBuffer )
    {
    __MULTIPLEXER( "CMccSinkItem::StoreBufferL" )
    
    // If secondary sink has not yet requested buffer fill, the buffer
    // will be stored
    if ( iStoredBuffers.Count() >= KMccMaxStoredBuffers )
        {
        __MULTIPLEXER( "CMccSinkItem::BufferFilledL, dropping oldest stored buffer!" )
        delete iStoredBuffers[ 0 ];
        iStoredBuffers.Remove( 0 );
        }
    
    CMMFDataBuffer* storedBuffer = CMMFDataBuffer::NewL( aBuffer->BufferSize() );
    
    CleanupStack::PushL( storedBuffer );
    CopyBufferL( storedBuffer, aBuffer ); 
    iStoredBuffers.AppendL( storedBuffer );
    CleanupStack::Pop( storedBuffer );
    
    __MULTIPLEXER( "CMccSinkItem::StoreBufferL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccSinkItem::GetStoredBuffer
// ---------------------------------------------------------------------------
//   
CMMFBuffer* CMccSinkItem::GetStoredBuffer()
    {
    CMMFBuffer* buffer = NULL;
    
    if ( iStoredBuffers.Count() > 0 )
        {
        buffer = iStoredBuffers[ 0 ];
        iStoredBuffers.Remove( 0 );
        }
        
    return buffer;
    }

// ---------------------------------------------------------------------------
// CMccSinkItem::HandleBadData
// ---------------------------------------------------------------------------
// 
TBool CMccSinkItem::HandleBadDataL( CMMFDataBuffer* aBuffer )
    { 
    TBool stopBufferForwarding( EFalse ); 
    if ( aBuffer == NULL || !aBuffer->BufferSize() )
        {
        __MULTIPLEXER( "CMccSinkItem::HandleBadDataL, bad data received" )
        iBadDataCount++;
        if ( iBadDataCount >= KMccBadDataCount && iEventHandler )
            {
            __MULTIPLEXER( "CMccSinkItem::HandleBadDataL, pausing stream" )
            iBadDataCount = 0;
            
    	    TMccEvent* event = new ( ELeave ) TMccEvent;
	        CleanupStack::PushL( event );
    	    event->iEndpointId = 0;
    	    event->iEventCategory = KMccEventCategoryStreamControl;
    	    event->iEventType = KMccStreamPaused;
    	    event->iErrorCode = KErrNone;

    		TMccInternalEvent internalEvent( KMccMultiplexerUid, EMccInternalEventNone,
    		                                 *event );
		                         
		    User::LeaveIfError( iEventHandler->SendEventToClient( internalEvent ) );
		    
		    CleanupStack::PopAndDestroy( event );
            __MULTIPLEXER( "CMccSinkItem::HandleBadDataL, pausing event sent" )
            
            stopBufferForwarding = ETrue;
            }
        }
    else    
        {
        iBadDataCount = 0;
        }
        
    return stopBufferForwarding;
    }

// ---------------------------------------------------------------------------
// CMccSinkItem::HandleStreamStateChangeEvent
// ---------------------------------------------------------------------------
//
TBool CMccSinkItem::HandleStreamStateChangeEvent( TMccEvent& aEvent )
    {
    TBool sendEvent( ETrue );
    
    // Modify started event to resumed event if previously paused
    if ( aEvent.iEventType == KMccStreamStarted && 
         iReportedEvent == KMccStreamPaused )
        {
        __MULTIPLEXER( "CMccSinkItem::SendEventToClient, modify event type" )
        aEvent.iEventType = KMccStreamResumed;
        }
    
    // Automatic started/resumed event is not forwarded if this user hasn't yet
    // started the source
    if ( aEvent.iEventNumData == KMccAutomaticEvent &&
       ( aEvent.iEventType == KMccStreamStarted ||
         aEvent.iEventType == KMccStreamResumed ) )
        {
        sendEvent = ( iState >= CMccMultiplexer::EPlaying );
        
        __MULTIPLEXER_INT1( 
        "CMccSinkItem::SendEventToClient, automatic event send:", sendEvent )
        }
    
    if ( sendEvent )
        {
        sendEvent = ( aEvent.iEventType != iReportedEvent );
        
        iReportedEvent = aEvent.iEventType;
        }
    
    return sendEvent;
    }
    
//  End of File 

