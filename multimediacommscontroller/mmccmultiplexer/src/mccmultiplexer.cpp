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
#include <mmf/server/mmfdatabuffer.h>
#include "mccmultiplexer.h"
#include "mccsinkitem.h"
#include "mccinternaldef.h"
#include "mccinternalevents.h"
#include "mccmultiplexerlogs.h"
#include "mccinternalevents.h"
#include "mccrtpdatasource.h"

// MACROS

// LOCAL FUNCTION PROTOTYPES

// LOCAL CONSTANTS
const TUint KMultiplexerDefBufSize( 160 );

// ============================= LOCAL FUNCTIONS ===============================
        
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccMultiplexer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMccMultiplexer* CMccMultiplexer::NewL( MDataSource* aSource )
    {
	CMccMultiplexer* self = new ( ELeave ) CMccMultiplexer;
	CleanupStack::PushL( self );
    self->ConstructL( aSource );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::~CMccMultiplexer
// -----------------------------------------------------------------------------
//
EXPORT_C CMccMultiplexer::~CMccMultiplexer()
    {
    __MULTIPLEXER( "CMccMultiplexer::~CMccMultiplexer" )
    
    iSinkItems.ResetAndDestroy();
    delete iDataBuf;
    
    __MULTIPLEXER( "CMccMultiplexer::~CMccMultiplexer, exit" )
    }
  
// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceThreadLogon
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccMultiplexer::MultiplexerSourceThreadLogon( 
    MAsyncEventHandler& aEventHandler, 
    MDataSink& aSink, 
    TBool aSinkIsDecoder, 
    TUid aMediaType,
    TBool aPassAllBuffersSink )
    {
    __MULTIPLEXER_INT1( "CMccMultiplexer::MultiplexerSourceThreadLogon, endpoint:",
                        MCC_ENDPOINT_ID( &aSink ) )
    
    TInt err( KErrNone );
    
    TRAP( err, 
          AddSinkL( &aSink, aSinkIsDecoder, aMediaType, aPassAllBuffersSink ) );
    
    // Gather events from source and propagate them to all sink owners
    
    if ( !err && !iSourceLogonDone )
        {
        __MULTIPLEXER( "CMccMultiplexer::MultiplexerSourceThreadLogon, source logon" )
        
        err = iSource->SourceThreadLogon( *this );
        iSourceLogonDone = ETrue;
        }
        
    if ( !err )
        {
        err = FindSinkItemIndex( &aSink );
        if ( err != KErrNotFound )
            {
            err = iSinkItems[ err ]->SourceThreadLogon( aEventHandler );
            }
        if ( MCC_INTERNAL_ENDPOINT( iSource->DataSourceType().iUid ) )
            {
            static_cast<CMccDataSource*>( iSource )->AddUser( &aEventHandler );
            }
        }
        
    __MULTIPLEXER( "CMccMultiplexer::SourceThreadLogon, exit" )
    
    return err;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceThreadLogoff
// -----------------------------------------------------------------------------
//       
EXPORT_C TInt CMccMultiplexer::MultiplexerSourceThreadLogoff( MDataSink* aSink )
    {
    __MULTIPLEXER_INT1( "CMccMultiplexer::MultiplexerSourceThreadLogoff, endpoint:",
                        MCC_ENDPOINT_ID( aSink ) )
    
    if ( aSink )
        {
        TInt index = FindSinkItemIndex( aSink );
        if ( index != KErrNotFound )
            {
            CMccSinkItem* item = iSinkItems[ index ];
            if ( KMccRtpSourceUid == iSource->DataSourceType() )
                {
                const RArray<TUint>& payloads = item->RegisteredPayloadTypes();
                static_cast<CMccRtpDataSource*>( iSource )
                    ->UnRegisterPayloadTypes( payloads );
                }
            
            // Clear current user
            if ( item == iCurrentUser )
                {
                iCurrentUser = NULL;
                }
                
            TBool passedAllRequests = PassAllRequestsToSource( *item );
                
            iSinkItems.Remove( index );

            UpdateSinkModes();
            
            // Update primary buffer or logoff source
            HandleCleanup( item, passedAllRequests );

            return KErrNone;
            }
        return KErrNotFound;
        }
    return KErrArgument;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SetCurrentUserL
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMccMultiplexer::SetCurrentUserL( MDataSink& aSink )
    {
    __MULTIPLEXER_INT1( "CMccMultiplexer::SetCurrentUserL, endpoint:", 
                        MCC_ENDPOINT_ID( &aSink ) )
    
    iCurrentUser = FindSinkItemL( &aSink );
    
    __MULTIPLEXER( "CMccMultiplexer::SetCurrentUserL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::RegClientForPayloadsL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMccMultiplexer::RegClientForPayloadsL( MDataSink& aConsumer, 
        const RArray<TUint>& aPayloadTypes )
    {
    __MULTIPLEXER( "CMccMultiplexer::RegClientForPayloadsL" )
    __ASSERT_ALWAYS( iSource, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( KMccRtpSourceUid == iSource->DataSourceType(),
        User::Leave( KErrNotSupported ) );
    
    FindSinkItemL( &aConsumer )->RegisterPayloadTypesL( aPayloadTypes );
    static_cast<CMccRtpDataSource*>( iSource )
        ->RegisterPayloadTypesL( aPayloadTypes );
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::Streams
// -----------------------------------------------------------------------------
//
TUint CMccMultiplexer::Streams( TUid /*aMediaType*/ ) const
    {
    return 1;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::FrameTimeInterval
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccMultiplexer::FrameTimeInterval( 
    TMediaId /*aMediaId*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }  

// -----------------------------------------------------------------------------
// CMccMultiplexer::DataSourceType
// -----------------------------------------------------------------------------
//
TUid CMccMultiplexer::DataSourceType() const
    {
    return KMccMultiplexerUid;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::FillBufferL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::FillBufferL( 
    CMMFBuffer* aBuffer, 
    MDataSink* aConsumer, 
    TMediaId aMediaId )
    {
    __MULTIPLEXER_INT1( "CMccMultiplexer::FillBufferL, endpoint:",
                        MCC_ENDPOINT_ID( aConsumer ) )
    __MULTIPLEXER_INT1( "CMccMultiplexer::FillBufferL, mediaId uid:",
                        aMediaId.iMediaType.iUid )
    
    __ASSERT_ALWAYS( aBuffer, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aBuffer->Type() == KUidMmfDataBuffer, 
                     User::Leave( KErrArgument ) ); 
    __ASSERT_ALWAYS( aConsumer, User::Leave( KErrArgument ) );
    
    // Store request
    CMccSinkItem* item = FindSinkItemL( aConsumer );
    item->FillBufferRequestL( aBuffer );
                                                        
    // If not in pass all mode, only one sink at the time can control
    // the source
    if ( iState == EPlaying && 
       ( item->PassAllRequestsToSource() || CanCommandSource( aConsumer ) ) )
        {
        __MULTIPLEXER( "CMccMultiplexer::FillBufferL, command source" )
        
        if ( KMccRtpSourceUid == iSource->DataSourceType() )
            {
            // Multiplexer buffer needed for payload type multiplexing
            __ASSERT_ALWAYS( 
                static_cast<CMMFDataBuffer*>( aBuffer )
                ->Data().MaxSize() <= iDataBuf->Data().MaxSize(),
                User::Leave( KErrTooBig ) );
            
            __MULTIPLEXER( "CMccMultiplexer::FillBufferL, using own buffer" )
            iDataBuf->SetStatus( EBeingFilled );
            iDataBuf->SetLastBuffer( EFalse );
            iSource->FillBufferL( iDataBuf, this, aMediaId );
            }
        else
            {
            iSource->FillBufferL( aBuffer, this, aMediaId );
            }
        }
        
    __MULTIPLEXER( "CMccMultiplexer::FillBufferL, exit" )
    }  
        
// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceDataTypeCode
// -----------------------------------------------------------------------------
//
TFourCC CMccMultiplexer::SourceDataTypeCode( 
	TMediaId aMediaId )
    {
    return iSource->SourceDataTypeCode( aMediaId );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SetSourceDataTypeCode
// -----------------------------------------------------------------------------
//
TInt CMccMultiplexer::SetSourceDataTypeCode( 
    TFourCC aSourceFourCC, 
    TMediaId aMediaId )
    {
    return iSource->SetSourceDataTypeCode( aSourceFourCC, aMediaId );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceThreadLogon
// -----------------------------------------------------------------------------
//
TInt CMccMultiplexer::SourceThreadLogon( 
    MAsyncEventHandler& /*aEventHandler*/ )
    {
    return KErrNotSupported;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::NegotiateSourceL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::NegotiateSourceL( MDataSink& aDataSink )
    {
    __MULTIPLEXER( "CMccMultiplexer::NegotiateSourceL" )
    
    if ( !iSourceNegotiationDone )
        {
        if ( KUidMmfAudioInput == iSource->DataSourceType() )
            {
            // Audio input need settings from real sink.
            // This is just quick workaround, better way would be to route
            // those requests through multiplexer to the sink
            
            __MULTIPLEXER( "CMccMultiplexer::NegotiateSourceL, negotiate (use sink)" )
            
            iSource->NegotiateSourceL( aDataSink );
            }
        else
            {
            __MULTIPLEXER( "CMccMultiplexer::NegotiateSourceL, negotiate (normal)" )
            
            iSource->NegotiateSourceL( *this );
            }
            
        iSourceNegotiationDone = ETrue;
        }
    __MULTIPLEXER( "CMccMultiplexer::NegotiateSourceL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceThreadLogoff
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourceThreadLogoff()
    {
    // NOP
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::SourcePrimeL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourcePrimeL()
    {
    __MULTIPLEXER( "CMccMultiplexer::SourcePrimeL" )
    
    if ( SetMultiplexerState( EPrepared ) )
        {
        iSource->SourcePrimeL();
        }
        
    __MULTIPLEXER( "CMccMultiplexer::SourcePrimeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourcePlayL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourcePlayL()
    {
    __MULTIPLEXER( "CMccMultiplexer::SourcePlayL" )
    
    if ( SetMultiplexerState( EPlaying ) )
        {
        iSource->SourcePlayL();
        }
        
    __MULTIPLEXER( "CMccMultiplexer::SourcePlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourcePauseL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourcePauseL()
    {
    __MULTIPLEXER( "CMccMultiplexer::SourcePauseL" )
    
    if ( SetMultiplexerState( EResourcePaused ) )
        {
        iSource->SourcePauseL();
        }
        
    __MULTIPLEXER( "CMccMultiplexer::SourcePauseL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SourceStopL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SourceStopL()
    {
    __MULTIPLEXER( "CMccMultiplexer::SourceStopL" )
    
    if ( SetMultiplexerState( EResourceStopped ) )
        {
        iSource->SourceStopL();
        }
        
    __MULTIPLEXER( "CMccMultiplexer::SourceStopL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::DataBufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::DataBufferFilledL( 
    CMMFBuffer* aBuffer, 
    const TRtpRecvHeader& aRtpHeader )
    {
    __MULTIPLEXER( "CMccMultiplexer::DataBufferFilledL" )
    __ASSERT_ALWAYS( NULL != aBuffer, User::Leave( KErrArgument ) );
    
    DoBufferFilledL( aBuffer, &aRtpHeader );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::ConfigurePayloadFormatL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::ConfigurePayloadFormatL( const TDesC8& /*aConfigParams*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::BufferFilledL( CMMFBuffer* aBuffer )
	{
    __MULTIPLEXER( "CMccMultiplexer::BufferFilledL" )
    __ASSERT_ALWAYS( NULL != aBuffer, User::Leave( KErrArgument ) );
    
    DoBufferFilledL( aBuffer, 0 );
	}
	
// -----------------------------------------------------------------------------
// CMccMultiplexer::Duration
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccMultiplexer::Duration( 
	TMediaId /*aMediaType*/ ) const
    {
    return TTimeIntervalMicroSeconds( TInt64( 0 ) );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::CanCreateSourceBuffer
// -----------------------------------------------------------------------------
//    
TBool CMccMultiplexer::CanCreateSourceBuffer()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::CreateSourceBufferL
// Created buffer is not intended for sink use.
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccMultiplexer::CreateSourceBufferL( 
    TMediaId /*aMediaId*/, 
    TBool& aReference )
    {
    __MULTIPLEXER( "CMccMultiplexer::CreateSourceBufferL" )
    __ASSERT_ALWAYS( iSuggestedBufSize, User::Leave( KErrNotReady ) );
    
    aReference = EFalse;
    
    if ( iDataBuf->Data().MaxSize() < iSuggestedBufSize )
        {
        static_cast<CMMFDescriptorBuffer*>( iDataBuf )
            ->ReAllocBufferL( iSuggestedBufSize );
        }
    
    CMMFDataBuffer* buf = CMMFDataBuffer::NewL( iSuggestedBufSize );
    iSuggestedBufSize = 0;
    return buf;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SuggestSourceBufferSize
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::SuggestSourceBufferSize( TUint aSuggestedBufferSize )
    {
    __MULTIPLEXER_INT1( "CMccMultiplexer::SuggestSourceBufferSize, SIZE: ",
         aSuggestedBufferSize )
    
    iSuggestedBufSize = aSuggestedBufferSize;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SendEventToClient
// -----------------------------------------------------------------------------
//
TInt CMccMultiplexer::SendEventToClient( const TMMFEvent& aEvent )
    {
    __MULTIPLEXER( "CMccMultiplexer::SendEventToClient" )
    
    // Stream control event has to be stored at this level since control
    // request can go back to source synchronously causing stream state change
    // event, which then overwrites the control event.
    //
    if ( IS_MCC_EVENT( aEvent ) )
	    {
	    TMccEvent* mccEventPtr = 
	        reinterpret_cast<const TMccInternalEvent&>( aEvent ).iMccEvent;
	        
	    if ( mccEventPtr && 
	         MCC_STREAM_STATE_CHANGE_EVENT( mccEventPtr ) &&
	         mccEventPtr->iEventNumData != KMccAutomaticEvent )
            {
            if ( iCurrentUser )
                {
                return iCurrentUser->SendEventToClient( aEvent );
                }
            else
                {
                __MULTIPLEXER( "CMccMultiplexer::SendEventToClient, KErrNotReady" )
                return KErrNotReady;
                }
            }
        
	    if ( mccEventPtr && 
	         mccEventPtr->iEventCategory == KMccEventCategoryStreamControl )
	        {
	        iStreamControlEvent = *mccEventPtr;
	        TMccInternalEvent internalEvent( KMccMultiplexerUid, EMccInternalEventNone,
    		                                 iStreamControlEvent );
            
            SendEventToAllClients( internalEvent );
            
            return KErrNone;
	        }
	    }
	
	SendEventToAllClients( aEvent );        

    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::CMccMultiplexer
// -----------------------------------------------------------------------------
//
CMccMultiplexer::CMccMultiplexer()
    {
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::ConstructL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::ConstructL( MDataSource* aSource )
    {
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    
    iSource = aSource;
    iDataBuf = CMMFDataBuffer::NewL( KMultiplexerDefBufSize );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::FindSinkItemL
// -----------------------------------------------------------------------------
//
CMccSinkItem* CMccMultiplexer::FindSinkItemL( MDataSink* aSink )
    {
    TInt index = FindSinkItemIndex( aSink );
    __ASSERT_ALWAYS( index != KErrNotFound, User::Leave( KErrNotFound ) );
    
    return iSinkItems[ index ];
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::FindSinkItemIndex
// -----------------------------------------------------------------------------
//
TInt CMccMultiplexer::FindSinkItemIndex( MDataSink* aSink )
    {
    for ( TInt i = 0; i < iSinkItems.Count(); i++ )
        {
        if ( iSinkItems[ i ]->Match( aSink ) )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::CanCommandSource
// Primary sink has always preferred access to source, other sinks can use
// source only if primary is not using it
// -----------------------------------------------------------------------------
// 
TBool CMccMultiplexer::CanCommandSource( MDataSink* aSink )
    {
    TBool canCommandSource( EFalse );
    if ( IsPrimarySink( aSink ) )
        {
        canCommandSource = ETrue;
        }
    else if ( iSinkItems.Count() > 0 && 
              !IsActiveState( iSinkItems[ 0 ]->MultiplexerState() ) )
        {
        canCommandSource = ETrue;
        }
    else
        {
        // NOP
        }
    return canCommandSource;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::IsPrimarySink
// -----------------------------------------------------------------------------
//    
TBool CMccMultiplexer::IsPrimarySink( MDataSink* aSink )
    {
    // Primary sink is always the first one
    return ( iSinkItems.Count() > 0 && iSinkItems[ 0 ]->Match( aSink ) );
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::DoBufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::DoBufferFilledL( 
    CMMFBuffer* aBuffer, 
    const TRtpRecvHeader* aRtpHeader )
    {
    TUid mediaType = KNullUid;
    iDataBuf->SetStatus( EFull );
    
    if ( iResolveFilledBufferMediaType )
        {
        for ( TInt i = 0; i < iSinkItems.Count() && mediaType == KNullUid; i++ )
            {
            mediaType = iSinkItems[ i ]->ResolveMediaType( aBuffer );
            }
        }
        
    for ( TInt i = 0; i < iSinkItems.Count(); i++ )
        {
        // First item is always the primary sink, but multiplexer can be
        // also set into mode where all fillbuffer requests are passed to the
        // source (i.e. all sinks are kind of primary ones). In such case,
        // incoming buffer must match the one which was given when fillbuffer
        // request was issued (i.e. source cannot return different buffer).
        TBool isPrimary( !i );
        
        iSinkItems[ i ]->BufferFilledL( aBuffer, 
                                        aRtpHeader, 
                                        isPrimary,
                                        mediaType );
        }
    }   

// -----------------------------------------------------------------------------
// CMccMultiplexer::AddSinkL
// -----------------------------------------------------------------------------
//  
void CMccMultiplexer::AddSinkL( 
    MDataSink* aSink, 
    TBool aSinkIsDecoder,
    TUid aMediaType,
    TBool aPassAllBuffersSink )
    {
    __ASSERT_ALWAYS( FindSinkItemIndex( aSink ) == KErrNotFound, 
                     User::Leave( KErrAlreadyExists ) );
    
    CMccSinkItem* item = CMccSinkItem::NewLC( aSink, 
                                              aSinkIsDecoder, 
                                              aMediaType, 
                                              aPassAllBuffersSink );
    iSinkItems.AppendL( item );
    CleanupStack::Pop( item );
    
    // Update sinks
    UpdateSinkModes();
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SetMultiplexerState
// -----------------------------------------------------------------------------
//    
TBool CMccMultiplexer::SetMultiplexerState( TMccMultiplexerState aState )
    {
    __MULTIPLEXER_INT1( "CMccMultiplexer::SetMultiplexerState, state:", aState )
    
    // If state of the multiplexed source is already what is tried to be set,
    // multiplexer generates the corresponding completion event
    TBool controlSource( iState != aState );
    
    TMccEventType eventType = KMccEventNone;
    
    switch ( aState )
        {
        case EPrepared:
            {
            eventType = KMccStreamPrepared;
            
            if ( iState == EPlaying || iState == EResourcePaused )
                {
                controlSource = EFalse;
                }
            else
                {
                iState = aState;
                }
            break;
            }
        case EPlaying:
            {
            eventType = KMccStreamStarted;
            iState = aState;
            break;
            }
        case EResourcePaused:
            {
            eventType = KMccStreamPaused;
            iState = aState;
            break;
            }
        case EResourceStopped:
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
    
    if ( iCurrentUser )
        {
        iCurrentUser->SetMultiplexerState( iState );
        }
        
    if ( eventType != KMccEventNone && !controlSource )
        {
        SendStreamEventToClient( eventType );
        }
        
    return controlSource;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::SendStreamEventToClient
// -----------------------------------------------------------------------------
//     
void CMccMultiplexer::SendStreamEventToClient( TMccEventType& aEventType )
    {
    __MULTIPLEXER( "CMccMultiplexer::SendStreamEventToClient" )
    
   	TMccEvent event( 0, 
                     0, 
                     0, 
                     MCC_ENDPOINT_ID( iSource ), 
                     KMccEventCategoryStream, 
                     aEventType, 
                     KErrNone, 
                     KNullDesC8 );
   
    TMccInternalEvent internalEvent( KMccMultiplexerUid, 
                                     EMccInternalEventNone,
                                     event );
                                     
    if ( iCurrentUser )
        {
        __MULTIPLEXER( "CMccMultiplexer::SendStreamEventToClient, sending to current user" )
        
        iCurrentUser->SendEventToClient( internalEvent );
        }
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::UpdateSinkModes
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::UpdateSinkModes()
    {
    __MULTIPLEXER( "CMccMultiplexer::UpdateSinkModes" )
    
    // Check if mediatype has to be resolved for each filled buffer
    iResolveFilledBufferMediaType = EFalse;
    
    for ( TInt i = 0; i < iSinkItems.Count(); i++ )
        {
        TBool passAllRequestsToSource = PassAllRequestsToSource( *iSinkItems[ i ] );
        iSinkItems[ i ]->SetSourceMode( passAllRequestsToSource );
        
        iResolveFilledBufferMediaType |= passAllRequestsToSource;
        }
    
    __MULTIPLEXER_INT1( "CMccMultiplexer::UpdateSinkModes, resolve buffers mode:", 
                        iResolveFilledBufferMediaType  )     
    __MULTIPLEXER( "CMccMultiplexer::UpdateSinkModes, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::PassAllRequestsToSource
// -----------------------------------------------------------------------------
//   
TBool CMccMultiplexer::PassAllRequestsToSource( CMccSinkItem& aItem ) const
    {
    // If source is filesource, it can support two primary sinks; one for audio
    // and one for video.
    //

    if ( iSource->DataSourceType() == KMccFileSourceUid )
        {
        TUid mediaType = aItem.MediaType();
        for ( TInt i = 0; i < iSinkItems.Count(); i++ )
            {
            // If sinkitem has highest preference from group of sinkitems of same
            // media type, all its requests can be passed to the source
            if ( mediaType == iSinkItems[ i ]->MediaType() )
                {
                return ( iSinkItems[ i ] == &aItem );
                }
            }
        }
    else if ( iSource->DataSourceType() == KMccRtpSourceUid )
        {
        // Payload type multiplexing is used with network resource. Multiple
        // clients are allowed to request fillbuffer from RTP-source.
        return ETrue;
        }
    else
        {
        // Make PC-LINT happy
        }
    
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::IsSupportedMediaType
// -----------------------------------------------------------------------------
//    
TBool CMccMultiplexer::IsSupportedMediaType( TUid aMediaType ) const
    {
    return ( aMediaType == KUidMediaTypeVideo || 
             aMediaType == KUidMediaTypeAudio );
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::SendEventToAllClients
// -----------------------------------------------------------------------------
// 
void CMccMultiplexer::SendEventToAllClients( const TMMFEvent& aEvent )
    {
    for ( TInt i = 0; i < iSinkItems.Count(); i++ )
        {
        iSinkItems[ i ]->SendEventToClient( aEvent );
        }
    }

// -----------------------------------------------------------------------------
// CMccMultiplexer::HandleCleanup
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::HandleCleanup( 
    CMccSinkItem* aSinkItem, 
    TBool aPassedAllRequests )
    {
    TRAP_IGNORE( HandleCleanupL( aSinkItem, aPassedAllRequests ) )
    delete aSinkItem;
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::HandleCleanupL
// -----------------------------------------------------------------------------
//
void CMccMultiplexer::HandleCleanupL( 
    CMccSinkItem* aSinkItem,
    TBool aPassedAllRequests )
    {
    __MULTIPLEXER( "CMccMultiplexer::HandleCleanupL" )
    
     if ( iSinkItems.Count() == 0 )
        {
        // If no more sinks, source can be logged off
        iSource->SourceThreadLogoff();
        iSourceLogonDone = EFalse;
        }
     else
        {      
        // Update source's sinkbuffer (may be also NULL) and user info
        
        if ( aSinkItem )
            {
            if ( aPassedAllRequests )
                {
                __MULTIPLEXER( "CMccMultiplexer::HandleCleanupL, clear old" )
                iSource->FillBufferL( NULL, this, TMediaId( aSinkItem->MediaType() ) );
                }
            
            if ( MCC_INTERNAL_ENDPOINT( iSource->DataSourceType().iUid ) )
                {
                static_cast<CMccDataSource*>( 
                    iSource )->RemoveUser( aSinkItem->EventHandler() );
                }
            }
            
        CMMFBuffer* newPrimaryBuf = iSinkItems[ 0 ]->RequestedBuffer();
        
        if ( newPrimaryBuf && KMccRtpSourceUid == iSource->DataSourceType() )
            {
            // When payload type multiplexing, multiplexer's own buffer is used
            newPrimaryBuf = iDataBuf;
            }
        
        __MULTIPLEXER( "CMccMultiplexer::HandleCleanupL, set new" )    
        iSource->FillBufferL( newPrimaryBuf, 
                              this, 
                              TMediaId( iSinkItems[ 0 ]->MediaType() ) );
        }
    
    __MULTIPLEXER( "CMccMultiplexer::HandleCleanupL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccMultiplexer::IsActiveState
// -----------------------------------------------------------------------------
//
TBool CMccMultiplexer::IsActiveState( TMccMultiplexerState aState ) const
    {
    return ( aState == EPlaying );
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// E32Dll.
// DLL Entry point
// Returns: KErrNone
// -----------------------------------------------------------------------------
//
#ifndef EKA2
EXPORT_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif


//  End of File 

