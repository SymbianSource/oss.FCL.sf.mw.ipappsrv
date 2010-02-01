/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    JitterBuffer component capable to audioframe buffering.
*
*/




// INCLUDE FILES
#include <mmf/server/mmfaudiooutput.h>
#include "mccjitterbuffer.h"
#include "mccjitterbufferimpl.h"
#include "mccinternaldef.h"
#include "mccinternalevents.h"
#include "mccjitterbufferlogs.h"
#include "mccjitterbufferobserver.h"

// LOCAL CONSTANTS
const TUint KAdaptUpdateIval = 250;

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccJitterBuffer::CMccJitterBuffer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccJitterBuffer::CMccJitterBuffer( MJitterBufferObserver* aObserver ) :
    MDataSink( KMccJitterBufferUid ), MDataSource( KMccJitterBufferUid ),
    iState( EJBufStopped ),
    iObserver ( aObserver )
    {
    
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMccJitterBuffer* CMccJitterBuffer::NewL( MJitterBufferObserver* aObserver )
    {
    CMccJitterBuffer* self = new ( ELeave ) CMccJitterBuffer( aObserver );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::~CMccJitterBuffer
// Destructor deallocate memory.
// -----------------------------------------------------------------------------
//
CMccJitterBuffer::~CMccJitterBuffer()
    {
    __JITTER_BUFFER_INT1( "CMccJitterBuffer::~CMccJitterBuffer:", 
                          reinterpret_cast<TInt>( this ) )
    __JITTER_BUFFER_INT1( "CMccJitterBuffer::~CMccJitterBuffer iDataSink:", 
                          reinterpret_cast<TInt>( iDataSink ) )
    __JITTER_BUFFER_INT1( "CMccJitterBuffer::~CMccJitterBuffer iDataSource:", 
                          reinterpret_cast<TInt>( iDataSource ) )
    __JITTER_BUFFER_INT1( "CMccJitterBuffer::~CMccJitterBuffer iSinkBuffer:", 
                          reinterpret_cast<TInt>( iSinkBuffer ) )
    __JITTER_BUFFER_INT1( "CMccJitterBuffer::~CMccJitterBuffer iBufferImpl:",
                          reinterpret_cast<TInt>( iBufferImpl ) )
    
    if ( iDataSink )
        {
        iDataSink->SinkThreadLogoff();
        }
    
    if ( !iSnkBufRef )
        {
        delete iSinkBuffer;
        }
    
    delete iBufferImpl;
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::SetupL
// Setup Jitterbuffer
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccJitterBuffer::SetupL( TInt /*aBufferSize*/, TInt aPlayThreshold, 
        const TMccCodecInfo& aCInfo )
    {
    __JITTER_BUFFER( "CMccJitterBuffer::SetupL" )
    
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrNotReady ) );
    
    if( !iBufferImpl )
        {
        iBufferImpl = CMccJitterBufferImpl::NewL( iObserver );
        }
        
    MMMFAudioOutput* output = static_cast<MMMFAudioOutput*>( iDataSink );
    iBufferImpl->SetupL( aPlayThreshold,
                         aCInfo,
                         output->SoundDevice(),
                         iEventHandler,
                         MCC_ENDPOINT_ID( iDataSink ) );
    } 
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::ResetBuffer
// Reset Jitterbuffer
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccJitterBuffer::ResetBuffer( TBool aPlayTone )
    {
    if ( iBufferImpl )
        {
        iBufferImpl->ResetBuffer( aPlayTone );
        }
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::DelayDownL
// Delay Down
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccJitterBuffer::DelayDownL()
    {
    User::LeaveIfNull( iBufferImpl );
    iBufferImpl->DelayDownL();
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::DelayUpL
// Delay Up
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccJitterBuffer::DelayUpL()
    {
    User::LeaveIfNull( iBufferImpl );
    iBufferImpl->DelayUpL();
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::SetSinkDataTypeCode
// From MDataSink
// -----------------------------------------------------------------------------
//
TInt CMccJitterBuffer::SetSinkDataTypeCode( TFourCC aSinkFourCC, TMediaId aMediaId )
    {
    if( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        __JITTER_BUFFER( "CMccJitterBuffer::SetSinkDataTypeCode To iDataSink" )
        
        return iDataSink->SetSinkDataTypeCode( aSinkFourCC, aMediaId );
        }
    else
        {
        __JITTER_BUFFER( "CMccJitterBuffer::SetSinkDataTypeCode NOT SUPPORTED" )

        return KErrNotSupported;
        }
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::SetSourceDataTypeCode
// 
// -----------------------------------------------------------------------------
//
TInt CMccJitterBuffer::SetSourceDataTypeCode( TFourCC aSourceFourCC, TMediaId aMediaId )
    {
    if( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        __JITTER_BUFFER( "CMccJitterBuffer::SetSourceDataTypeCode To iDataSource" )
        
        return iDataSource->SetSourceDataTypeCode( aSourceFourCC, aMediaId );
        }
    else
        {
        __JITTER_BUFFER( "CMccJitterBuffer::SetSourceDataTypeCode NOT SUPPORTED" )
        
        return KErrNotSupported;
        }
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::SinkDataTypeCode
// From MDataSink
// -----------------------------------------------------------------------------
//
TFourCC CMccJitterBuffer::SinkDataTypeCode( TMediaId aMediaId )
    {
    if( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        __JITTER_BUFFER( "CMccJitterBuffer::SinkDataTypeCode From iDataSink" )

        return iDataSink->SinkDataTypeCode( aMediaId );
        }
    else
        {    
        // Default to NULL FourCC
        __JITTER_BUFFER( "CMccJitterBuffer::SinkDataTypeCode DEFAULT" )

        return TFourCC();
        }
    }    
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::SourceDataTypeCode
// From MDataSource
// -----------------------------------------------------------------------------
//
TFourCC CMccJitterBuffer::SourceDataTypeCode( TMediaId aMediaId )
    {
    if( KUidMediaTypeAudio == aMediaId.iMediaType )
        {
        __JITTER_BUFFER( "CMccJitterBuffer::SourceDataTypeCode From iDataSource" )

        return iDataSource->SourceDataTypeCode( aMediaId );
        }
    else
        {    
        // Default to NULL FourCC
        __JITTER_BUFFER( "CMccJitterBuffer::SourceDataTypeCode DEFAULT" )

        return TFourCC();
        }
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::ConstructSinkL
// From MDataSink
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    User::Leave( KErrNotSupported );
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::ConstructSourceL
// From MDataSource
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::ConstructSourceL( const TDesC8& /*aInitData*/ )
    {    
    User::Leave( KErrNotSupported );
    }        
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::SinkThreadLogon
// From MDataSink
// -----------------------------------------------------------------------------
//
TInt CMccJitterBuffer::SinkThreadLogon( MAsyncEventHandler& aEventHandler )
    {
    if( !iEventHandler )
        {
        if( iDataSink )
            {
            __JITTER_BUFFER( "CMccJitterBuffer::SinkThreadLogon NORMAL" )
            
            iEventHandler = &aEventHandler;
            return iDataSink->SinkThreadLogon( aEventHandler );
            }
        else
            {
            __JITTER_BUFFER( "CMccJitterBuffer::SinkThreadLogon KErrNotReady" )

            return KErrNotReady;
            }
        }
    else
        {
        __JITTER_BUFFER( "CMccJitterBuffer::SinkThreadLogon KErrAlreadyExists" )

        return KErrAlreadyExists;
        }
    }    

// -----------------------------------------------------------------------------
// CMccJitterBuffer::SourceThreadLogon
// From MDataSource
// -----------------------------------------------------------------------------
//
TInt CMccJitterBuffer::SourceThreadLogon( MAsyncEventHandler& /*aEventHandler*/ )
    {
    __JITTER_BUFFER( "CMccJitterBuffer::SourceThreadLogon" )

    // Jitter buffer is used always as a datasink, so we do not support any
    // source threads logging on.
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::SinkThreadLogoff
// From MDataSink
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::SinkThreadLogoff()
    {
    __JITTER_BUFFER( "CMccJitterBuffer::SinkThreadLogoff in" )

    iEventHandler = NULL;
        
    if( iBufferImpl )
        {
        delete iBufferImpl;
        iBufferImpl = NULL;
        }
    
    if( iDataSink )
        {
        iDataSink->SinkThreadLogoff();
        iDataSink = NULL;
        }
    
    if ( !iSnkBufRef )
        {
        delete iSinkBuffer;
        }
    
    iSinkBuffer = NULL;
    
    __JITTER_BUFFER( "CMccJitterBuffer::SinkThreadLogoff out" )
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::SinkPrimeL
// 
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::SinkPrimeL()
    {
    __JITTER_BUFFER( "CMccJitterBuffer::SinkPrimeL" )

    iState = EJBufPrimed;
    this->ResetBuffer();
    iDataSink->SinkPrimeL();
    }    
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::SinkPlayL
// 
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::SinkPlayL()
    {
    __JITTER_BUFFER("CMccJitterBuffer::SinkPlayL iDataSink->SinkPlayL" )
    
    iState = EJBufPlaying;
    iDataSink->SinkPlayL();
    iSnkBufRef = ETrue; // Sinkbuffer owned by MMF
    iRequestSize = 0;
    
    __JITTER_BUFFER( "CMccJitterBuffer::SinkPlayL CreateSinkBufferL" )

    iSinkBuffer = iDataSink->CreateSinkBufferL( KUidMediaTypeAudio, iSnkBufRef );
    
    // If buffer has not been supplied via CreateSinkBufferL, must use 
    // asynchronous buffer creation
    if ( !iSinkBuffer )
        {
        __JITTER_BUFFER( "CMccJitterBuffer::SinkPlayL EmptyBufferL ASYNC CREATION" )

        iDataSink->EmptyBufferL( iSinkBuffer, this, KUidMediaTypeAudio );
        }
    else
        {
        //we have a sink buffer from CreateSinkBufferL
        __JITTER_BUFFER( "CMccJitterBuffer::SinkPlayL EmptyBufferL SYNC CREATION" )

        iSinkBuffer->SetStatus( EAvailable );
        }
    
    iPlayedFrames = 0;        
    }
        
// -----------------------------------------------------------------------------
// CMccJitterBuffer::SinkPauseL
// 
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::SinkPauseL()
    {
    __JITTER_BUFFER( "CMccJitterBuffer::SinkPauseL" )

    iState = EJBufPaused;
    ResetBuffer();
    iDataSink->SinkPauseL();
    iPlayedFrames = 0;
    
    // If sinkbuffer is not a reference then we need to delete it here so that
    // play - pause - play works. Also devsound creates new buffers when
    // resuming from Pause through Play.
    if ( !iSnkBufRef )
        {
        delete iSinkBuffer;
        }
    
    iSinkBuffer = NULL;
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::SinkStopL
// 
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::SinkStopL()
    {
    __JITTER_BUFFER( "CMccJitterBuffer::SinkStopL" )
    
    iState = EJBufStopped;
    ResetBuffer();
    iDataSink->SinkStopL();
    iPlayedFrames = 0;
    if ( !iSnkBufRef )
        {
        delete iSinkBuffer;
        }
    
    iSinkBuffer = NULL;
    }
    

// -----------------------------------------------------------------------------
// CMccJitterBuffer::CanCreateSinkBuffer
// From MDataSink
// -----------------------------------------------------------------------------
//
TBool CMccJitterBuffer::CanCreateSinkBuffer()
    {
    // CMccJitterBuffer can't create buffers
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::CanCreateSourceBuffer
// From MDataSource
// -----------------------------------------------------------------------------
//
TBool CMccJitterBuffer::CanCreateSourceBuffer()
    {
    // CMccJitterBuffer can't create buffers
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::CreateSinkBufferL
// From MDataSink
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccJitterBuffer::CreateSinkBufferL( TMediaId /*aMediaId*/, 
        TBool& /*aReference*/ )
    {
    // CMccJitterBuffer can't create buffers
    User::Leave( KErrNotSupported );
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::CreateSourceBufferL
// From MDataSource
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccJitterBuffer::CreateSourceBufferL( TMediaId /*aMediaId*/, 
        TBool& /*aReference*/ )
    {
    // CMccJitterBuffer can't create buffers
    User::Leave( KErrNotSupported );
    return NULL;
    }              
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::FillBufferL
// From MDataSource
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::FillBufferL( CMMFBuffer* /*aBuffer*/,
    MDataSink* /*aConsumer*/, 
    TMediaId /*aMediaId*/ )
    {
    __JITTER_BUFFER( "CMccJitterBuffer::FillBufferL KErrNotSupported" )
    
    User::Leave( KErrNotSupported );
    }
        
// -----------------------------------------------------------------------------
// CMccJitterBuffer::EmptyBufferL
// From MDataSink
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::EmptyBufferL( CMMFBuffer* aBuffer,
    MDataSource* aSupplier,
    TMediaId /*aMediaId*/ )
    {
    if( iBufferImpl && ( iState == EJBufPlaying ) )
        {
        __ASSERT_ALWAYS( iBufferImpl->BufferLength(), User::Leave( KErrNotReady ) );
        
        __JITTER_BUFFER_INT1( "CMccJitterBuffer::EmptyBufferL BUF_SZ:", aBuffer->BufferSize() )
        __JITTER_BUFFER_INT1( "CMccJitterBuffer::EmptyBufferL REQ_SZ:", aBuffer->RequestSize() )
        
        // Adaptation control will be done based on played frames
        iBufferImpl->AddDataFrameL( aBuffer );
        aBuffer->SetStatus( EAvailable );
        }
        
    // Inform the source immediately so that packets keep on flowing in
    if ( aSupplier )
        {
        aSupplier->BufferEmptiedL( aBuffer );
        }
    }    
    
// -----------------------------------------------------------------------------
// CMccJitterBuffer::BufferFilledL
// 
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
    __JITTER_BUFFER( "CMccJitterBuffer::BufferFilledL KErrNotSupported" )

    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::BufferEmptiedL
// 
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::BufferEmptiedL( CMMFBuffer* aBuffer )
    {
    __JITTER_BUFFER( "CMccJitterBuffer::BufferEmptiedL" )
    
    User::LeaveIfNull( aBuffer );
    iSinkBuffer = aBuffer;
    iSinkBuffer->SetStatus( EAvailable );
    
    if( iBufferImpl && iBufferImpl->BufferLength() )
        {
        __JITTER_BUFFER( "CMccJitterBuffer::BufferEmptiedL NORMAL" )
        
        // Check for request size changes if current codec happens to support
        // dynamic codec frame length changes. Currently only needed for
        // supporting 10/20ms variants of G.711 codec. Also one frame has two
        // bytes extra for the frameheader, so we remove it here.
        const TInt reqSize( iSinkBuffer->RequestSize() - KVoIPHeaderLength );
        if ( reqSize != iRequestSize &&
             iBufferImpl->CurrentCodec() == KMccFourCCIdG711 )
            {
            __JITTER_BUFFER_INT1( "CMccJitterBuffer::BufferEmptiedL reqSize: ", reqSize )
            __JITTER_BUFFER_INT1( "CMccJitterBuffer::BufferEmptiedL iRequestSize: ", iRequestSize )
            iRequestSize = reqSize;
            
            if ( iObserver )
                {
                // We don't do this on emulator as it will kick us in the teeth
                // later when re-calculating jitterbuffer size.
                #ifndef __WINSCW__
                iObserver->DynamicBufferChangeRequest( iRequestSize );
                #endif
                }
            }
        
        iBufferImpl->GetDataFrameL( iSinkBuffer );
        iPlayedFrames++;
        
        if( KAdaptUpdateIval == iPlayedFrames )
            {
            // Don't care about the error if frames haven't been received yet
            // because it's sensible to do the adaptation only when there is 
            // actual data in the buffer
            iPlayedFrames = 0;
            SendJitterBufferEventToClient();
            }
        
        iSinkBuffer->SetStatus( EFull );
        iDataSink->EmptyBufferL( iSinkBuffer, this, KUidMediaTypeAudio );
        }
    else
        {
        __JITTER_BUFFER( "CMccJitterBuffer::BufferEmptiedL KErrNotReady" )
        
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::AddDataSinkL
// From MDataSource 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccJitterBuffer::AddDataSinkL( MDataSink* aSink )
    {
    if( iDataSink )
        {
        __JITTER_BUFFER( "CMccJitterBuffer::AddDataSinkL KErrAlreadyExists" )

        User::Leave( KErrAlreadyExists );
        }
    else
        {
        User::LeaveIfNull( aSink );
        if( KUidMmfAudioOutput == aSink->DataSinkType() )
            {
            __JITTER_BUFFER( "CMccJitterBuffer::AddDataSinkL AUDIO SINK" )

            iDataSink = aSink;
            }
        else
            {
            __JITTER_BUFFER( "CMccJitterBuffer::AddDataSinkL KErrArgument" )

            User::Leave( KErrArgument );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::AddDataSourceL
// From MDataSource 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccJitterBuffer::AddDataSourceL( MDataSource* aSource )
    {
    __JITTER_BUFFER( "CMccJitterBuffer::AddDataSourceL" )

    User::LeaveIfNull( aSource );
    iDataSource = aSource;
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::SendJitterBufferEventToClient
// 
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::SendJitterBufferEventToClient()
    {  
    TMccJitterBufferEventData eventData;
    iBufferImpl->GenerateStatistics( eventData );
    
    if( iEventHandler )
	    {
	    TMccEvent event( 0, 
	                     0, 
	                     0, 
	                     MCC_ENDPOINT_ID( iDataSink ), 
	                     KMccEventCategoryStream, 
	                     KMccEventNone, 
	                     KErrNone, 
	                     KNullDesC8 );
	                     
	    event.iEventData.Copy( TMccJitterBufferEventDataPackage( eventData ) );
	    
	    TMccInternalEvent internalEvent( KMccJitterBufferUid, 
	                                     EMccInternalJitterEventStatusReport,
	                                     event );
	                                     
	    iEventHandler->SendEventToClient( internalEvent );	
	    }
	else
		{
		__JITTER_BUFFER( "CMccJitterBuffer::RunError, iEventHandler=NULL" )	
		}
    }
            
// -----------------------------------------------------------------------------
// CMccJitterBuffer::GetDataSink
// Get DataSink
// -----------------------------------------------------------------------------
//
EXPORT_C MDataSink* CMccJitterBuffer::GetDataSink()
    {
    return iDataSink;
    }

// -----------------------------------------------------------------------------
// CMccJitterBuffer::NegotiateL
// From MDataSink
// -----------------------------------------------------------------------------
//
void CMccJitterBuffer::NegotiateL( MDataSource& aDataSource )
    {
    if( iDataSink )
        {
        __JITTER_BUFFER( "CMccJitterBuffer::NegotiateL" )

        iDataSink->NegotiateL( aDataSource );
        }
    else
        {
        __JITTER_BUFFER( "CMccJitterBuffer::NegotiateL KErrNotReady" )

        User::Leave( KErrNotReady );
        }
    }

//  End of File 

