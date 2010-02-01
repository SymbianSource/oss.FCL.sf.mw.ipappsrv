/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcemediastream.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcestreamobserver.h"
#include "mcertpobserver.h"
#include "mcecommediastream.h"
#include "mcefactory.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mceclilogs.h"
#include "mcesessionobserver.h"

#define _FLAT_DATA static_cast<CMceComMediaStream*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMediaStream::~CMceMediaStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaStream::~CMceMediaStream()
    {
    
    MCECLI_DEBUG("CMceMediaStream::~CMceMediaStream, Entry");
    MCECLI_DEBUG_DVALUE("id", iFlatData ? Id().iId : KMceNotAssigned );
    
    if ( iSource )
        {
        DeleteSource();
        }
    
    if ( _FLAT_DATA && FLAT_DATA( iLinkOwner ) )
	    {
    	delete iLinkedStream;
	    }

	while( iSinks.Count() > 0 )
	    {
	    DeleteSink( 0 );
	    }
	    
    iSinks.Reset();
    iSinks.Close();
    
    delete iFlatData;
    MCECLI_DEBUG("CMceMediaStream::~CMceMediaStream, Exit");
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::Type
// -----------------------------------------------------------------------------
//
EXPORT_C TMceMediaType CMceMediaStream::Type() const
    {
    return FLAT_DATA( iType );
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::SetMediaAttributeLinesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::SetMediaAttributeLinesL(
    CDesC8Array* aMediaSDPLines )
    {
    MCECLI_DEBUG("CMceMediaStream::SetMediaAttributeLinesL, Entry");
    User::LeaveIfError( !iSession || 
                      ( iSession->State() == CMceSession::EIdle ||
		                iSession->State() == CMceSession::EIncoming ||
		                iSession->State() == CMceSession::EEstablished ) ? 
		                KErrNone : KErrNotReady );
    
    CDesC8Array* lines = NULL;
    TBool linesPushed = EFalse;
    
    if ( aMediaSDPLines )
        {
        lines = aMediaSDPLines;
        }
    else
        {
        lines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( lines );
        linesPushed = ETrue;
        }
        
    if ( iLinkedStream )
        {
        CDesC8Array* copyLines = 
            new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( copyLines );
        for ( TInt i = 0; i < lines->MdcaCount(); i++ )
            {
            copyLines->AppendL( lines->MdcaPoint( i ) );
            }
        delete iLinkedStream->iFlatData->iLocalMediaSDPLines;
        iLinkedStream->iFlatData->iLocalMediaSDPLines = copyLines;
        CleanupStack::Pop( copyLines );
        }
        
    if ( linesPushed )
        {
        CleanupStack::Pop( lines );
        }
        
    delete FLAT_DATA( iLocalMediaSDPLines );
    FLAT_DATA( iLocalMediaSDPLines ) = lines;
    MCECLI_DEBUG("CMceMediaStream::SetMediaAttributeLinesL, Exit");
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::MediaAttributeLinesL
// -----------------------------------------------------------------------------
//
EXPORT_C MDesC8Array* CMceMediaStream::MediaAttributeLinesL()
    {
    CDesC8Array*  lines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( lines );
    for ( TInt i=0; i < FLAT_DATA( iRemoteMediaSDPLines )->Count(); i++ )
        {
        lines->AppendL( FLAT_DATA( iRemoteMediaSDPLines )->MdcaPoint( i ) );
        }
    CleanupStack::Pop( lines );
    return lines;
    }
        
// -----------------------------------------------------------------------------
// CMceMediaStream::SetStartMediaPort
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::SetLocalMediaPortL( TUint aLocalMediaPort )
    {
    MCECLI_DEBUG("CMceMediaStream::SetLocalMediaPortL, Entry");
    MCECLI_DEBUG_DVALUE("port", aLocalMediaPort );
    
    FLAT_DATA( iLocalMediaPort ) = aLocalMediaPort;

    MCECLI_DEBUG("CMceMediaStream::SetLocalMediaPortL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::StartMediaPort
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceMediaStream::LocalMediaPort() const        
    {
    return FLAT_DATA( iLocalMediaPort );
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::Session
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSession* CMceMediaStream::Session() const
    {
    return iSession;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::EnableL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::EnableL() 
    {
    MCECLI_DEBUG("CMceMediaStream::EnableL, Entry");
    
    if ( iSource )
        {
    	iSource->Enabled( ETrue );
        }
        
    for ( TInt i = 0; i < iSinks.Count(); i++ )
        {
        iSinks[ i ]->Enabled( ETrue );
        }
        
    FLAT_DATA( iIsEnabled ) = ETrue;
    MCECLI_DEBUG("CMceMediaStream::EnableL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::DisableL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::DisableL() 
    {
    MCECLI_DEBUG("CMceMediaStream::DisableL, Entry");
    
    if ( iSource )
        {
    	iSource->Enabled( EFalse );
        }
        
    for ( TInt i = 0; i < iSinks.Count(); i++ )
        {
        iSinks[ i ]->Enabled( EFalse );
        }
    
    FLAT_DATA( iIsEnabled ) = EFalse;

    MCECLI_DEBUG("CMceMediaStream::DisableL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::IsEnabled
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceMediaStream::IsEnabled() const
    {
    return FLAT_DATA( iIsEnabled );
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::SetSourceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::SetSourceL( CMceMediaSource* aSource )
    {
    if ( aSource )
        {
        aSource->ReferenceCount()++;
        }
    
    if ( iSource )
        {
        DeleteSource();
        }
    iSource = aSource;
    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::AddSinkL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::AddSinkL( CMceMediaSink* aSink )
    {
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iSinks.Count() == 0, User::Leave( KErrNotSupported ) );
    iSinks.AppendL( aSink );
    aSink->ReferenceCount()++;    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::Source
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSource* CMceMediaStream::Source() const
    {
    return iSource;
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::Sinks
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceMediaSink>& CMceMediaStream::Sinks() const
    {
    return iSinks;
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::RemoveSinkL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::RemoveSinkL( CMceMediaSink& aSink )
    {
	for ( TInt i = 0; i < iSinks.Count(); i++ )
	    {
	    if ( iSinks[i] == &aSink )
	        {
	        DeleteSink( i );
	        iSinks.Compress();
	        return;
	        }
	    }
	User::Leave( KErrNotFound );
    }



// -----------------------------------------------------------------------------
// CMceMediaStream::BindL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::BindL( CMceMediaStream* aStream )
    {
    __ASSERT_ALWAYS( !(! FLAT_DATA( iLinkOwner ) &&  
                         FLAT_DATA( iLinkedStream ) ) , 
                         User::Leave( KErrArgument ) );
    
    if ( aStream )
        {
        FLAT_DATA(iLinkOwner) = ETrue;
        aStream->DoBindL( this );
        DoBindL( aStream );
        }
    else
        {
        FLAT_DATA(iLinkOwner) = EFalse;
    	delete iLinkedStream;
    	iLinkedStream = NULL;
        }
    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::BoundStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaStream& CMceMediaStream::BoundStreamL() const
    {
    __ASSERT_ALWAYS( iLinkedStream , User::Leave( KErrNotFound ) );
    
    return *iLinkedStream;
    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::BoundStream
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceMediaStream::BoundStream() const
    {
    return iLinkedStream != NULL;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::State
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaStream::TState CMceMediaStream::State() const
    {
    return FLAT_DATA( iState );
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::InternalizeFlatL( RReadStream& aReadStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->InternalizeFlatL( aReadStream );
            
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->ExternalizeFlatL( aWriteStream );
    
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceMediaStream::SerializationId() const
    {
    return FLAT_DATA( SerializationId() );
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    TMceMediaStreamSerializer<CMceMediaStream> serial( *this, _FLAT_DATA );
    serial.InternalizeL( aSerCtx );
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    
    TMceMediaStreamSerializer<CMceMediaStream> serial( *this, _FLAT_DATA );
    serial.ExternalizeL( aSerCtx );
    
    }

    
// -----------------------------------------------------------------------------
// CMceMediaStream::SetSourceL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::SetSourceL( MMceComSerializationContext& aSerCtx )
    {

    RReadStream& readStream = aSerCtx.ReadStream();

    TBool externalizedReference = readStream.ReadUint8L();
    
    if ( externalizedReference )
        {        
        TMceMediaId id;
        MceSerial::DecodeL( id, readStream );
        CMceMediaSource* source = NULL;
        TInt index = 0;
        MCECLI_DEBUG_DVALUE("CMceMediaStream::SetSourceL, using cached source. id", id.iId );
        
        while( !source && index < aSerCtx.SourceCache().Count() )
            {
            source = static_cast<CMceMediaSource*>( aSerCtx.SourceCache()[ index++ ] );
            source = source->SerializationId() == id.Uint64() ? source : NULL;
            }
        __ASSERT_ALWAYS( source, User::Leave( KErrNotFound ) );
        SetSourceL( source );
        }
    else
        {
        CMceMediaSource* source = 
            BaseFactory().SourceFactory().CreateLC( aSerCtx );
        SetSourceL( source );
        CleanupStack::Pop( source );
        aSerCtx.SourceCache().AppendL( source );
        
        }
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::AddSinkL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::AddSinkL( MMceComSerializationContext& aSerCtx )
    {

    RReadStream& readStream = aSerCtx.ReadStream();

    TBool externalizedReference = readStream.ReadUint8L();
    
    if ( externalizedReference )
        {
        TMceMediaId id;
        MceSerial::DecodeL( id, readStream );
        CMceMediaSink* sink = NULL;
        TInt index = 0;
        MCECLI_DEBUG_DVALUE("CMceMediaStream::AddSinkL, using cached sink. id", id.iId );
        while( !sink && index < aSerCtx.SinkCache().Count() )
            {
            sink = static_cast<CMceMediaSink*>( aSerCtx.SinkCache()[ index++ ] );
            sink = sink->SerializationId() == id.Uint64() ? sink : NULL;
            }
        __ASSERT_ALWAYS( sink, User::Leave( KErrNotFound ) );
        AddSinkL( sink );
        }
    else
        {
        CMceMediaSink* sink = 
            BaseFactory().SinkFactory().CreateLC( aSerCtx );
        AddSinkL( sink );
        CleanupStack::Pop( sink );
        aSerCtx.SinkCache().AppendL( sink );
        
        }
    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::BaseFactory
// -----------------------------------------------------------------------------
//
TMceFactory CMceMediaStream::BaseFactory() const
    {
    return TMceFactory();
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::Factory
// -----------------------------------------------------------------------------
//
TMceMediaStreamFactory CMceMediaStream::Factory()
    {
    return TMceMediaStreamFactory();
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::Binder
// -----------------------------------------------------------------------------
//
TBool CMceMediaStream::Binder() const
    {
    return _FLAT_DATA->Binder();
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::Id
// -----------------------------------------------------------------------------
//
TMceMediaId CMceMediaStream::Id() const
    {
    return FLAT_DATA( iID );
    }
    

// -----------------------------------------------------------------------------
// CMceMediaStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::InitializeL( 
    CMceSession& aParent, 
    TBool aDiscardUnusedCodecs )
    {
    iSession = &aParent;

    if ( !Id().IsAssigned() )
        {
        FLAT_DATA( iID ) = iSession->Manager().NextMediaId();
        }
    
    ReorderCodecsByPreferenceL( aDiscardUnusedCodecs ); 
    
    if ( Binder() && BoundStream() )
        {
        iLinkedStream->InitializeL( aParent, aDiscardUnusedCodecs );
        }
        
    if ( iSource )
        {
        iSource->InitializeL( *this );
        }
        
    for( TInt i = 0; i < iSinks.Count();i++)
        {
        iSinks[i]->InitializeL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::InitializeL( CMceManager* aManager, CMceSession& aSession )
    {
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );
    iSession = &aSession;
    
    if ( Binder() && BoundStream() )
        {
        iLinkedStream->InitializeL( aManager, aSession );
        }
        
    if ( iSource )
        {
        iSource->InitializeL( aManager );
        }
        
    for( TInt i = 0; i < iSinks.Count();i++)
        {
        iSinks[i]->InitializeL( aManager );
        }
    
    if ( State() == EUninitialized )
        {
        SetState( EInitialized );
        }
              
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::AddedL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::AddedL()
    {
    
    if ( Binder() && BoundStream() )
        {
        iLinkedStream->AddedL();
        }
        
    if ( iSource )
        {
        iSource->StreamAddedL( *this );
        }
        
    for( TInt i = 0; i < iSinks.Count();i++)
        {
        iSinks[i]->StreamAddedL( *this );
        }

    }
    
    
// -----------------------------------------------------------------------------
// CMceMediaStream::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceMediaStream::EventReceivedL( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceMediaStream::EventReceivedL, Entry");
    MCECLI_DEBUG_DVALUES2("this: type", Type(), "id", Id().iId, 
                          "main", ( !BoundStream() || Binder() ) );
    
    TInt status = KMceEventNotOwner;
    TBool idMatch = aEvent.Id().iMediaID == Id();
    if ( idMatch )
        {
        if ( aEvent.Id().IsMediaId() )
            {
            status = HandleEvent( aEvent );
            }
        else
            {
            status = KMceEventNotConsumed;
            }
        }
    else
        {
        //NOP
        //might be for linked stream
        }

    if ( status == KMceEventConsumed || status == KMceEventUpdate )
        {
        MCECLI_DEBUG_DVALUE("CMceMediaStream::EventReceivedL, before exit. enabled", IsEnabled() );
        TState state = State();
        MCECLI_DEBUG_MEDIASTATE("CMceMediaStream::EventReceivedL, before exit. state", state );
        MCECLI_DEBUG_DVALUE("CMceMediaStream::EventReceivedL, Exit. status", status );
        return status;
        }

    //try binded
    if ( status == KMceEventNotOwner )
        {
        if ( Binder() && BoundStream() )
            {
            status = iLinkedStream->EventReceivedL( aEvent );
            if ( status == KMceEventUpdate )
                {
                delete iLinkedStream;
                iLinkedStream = NULL;
                CMceMsgObject<CMceMediaStream>* updateMsg = 
                    static_cast<CMceMsgObject<CMceMediaStream>*>( aEvent.Message() );
                CMceMediaStream* updateStream = updateMsg->Object();
                CleanupStack::PushL( updateStream );
                BindL( updateStream );
                CleanupStack::Pop( updateStream );
                InitializeL( *(Session()), EFalse );
                updateStream->Updated();
                status = KMceEventConsumed;
                }
            }
        MCECLI_DEBUG_DVALUE("CMceMediaStream::EventReceivedL, before exit. enabled", IsEnabled() );
        TState state = State();
        MCECLI_DEBUG_MEDIASTATE("CMceMediaStream::EventReceivedL, before exit. state", state );
        MCECLI_DEBUG_DVALUE("CMceMediaStream::EventReceivedL, Entry. status", status );
        return status;
        }
        
    //try source
    if ( aEvent.Id().IsSourceId() )
        {
        if ( iSource )
            {
            TInt sourceStatus = iSource->EventReceivedL( aEvent );
            if ( sourceStatus == KMceEventUpdate )
                {
                CMceMsgObject<CMceMediaSource>* updateMsg = 
                    static_cast<CMceMsgObject<CMceMediaSource>*>( aEvent.Message() );
                CMceMediaSource* updateSource = updateMsg->iObject;
                iSource->UpdateL( *updateSource );
                iSource->Updated();
                sourceStatus = KMceEventConsumed;
                }
            status = sourceStatus == KMceEventNotOwner ? 
                KMceEventNotConsumed : sourceStatus;
            }
        return status;
        }
        
    //try sinks
    if ( aEvent.Id().IsSinkId() )
        {
        TInt sinkStatus = status;
        TInt j = 0;
        while ( sinkStatus != KMceEventConsumed && j < iSinks.Count() )
            {
            CMceMediaSink* sink = iSinks[j]; 
            sinkStatus = sink->EventReceivedL( aEvent );
            if ( sinkStatus == KMceEventUpdate )
                {
                CMceMsgObject<CMceMediaSink>* updateMsg = 
                    static_cast<CMceMsgObject<CMceMediaSink>*>( aEvent.Message() );
                CMceMediaSink* updateSink = updateMsg->iObject;
                sink->UpdateL( *updateSink );
                sink->Updated();
                sinkStatus = KMceEventConsumed;
                }
            j++;                
            }
        status = sinkStatus == KMceEventNotOwner ? 
            KMceEventNotConsumed : sinkStatus;        
        }

    MCECLI_DEBUG_DVALUE("CMceMediaStream::EventReceivedL, before exit. enabled", IsEnabled() );
    TState state = State();
    MCECLI_DEBUG_MEDIASTATE("CMceMediaStream::EventReceivedL, before exit. state", state );
    MCECLI_DEBUG_DVALUE("CMceMediaStream::EventReceivedL, Exit. status", status );
    return status;    
    
    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::Updated
// -----------------------------------------------------------------------------
//
void CMceMediaStream::Updated()
    {
    MMceStreamObserver* observer = iSession->Manager().MediaObserver();
    
    if ( observer )
        {
        observer->StreamStateChanged( *this );
        }
    
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::HandleMediaError
// -----------------------------------------------------------------------------
//
TBool CMceMediaStream::HandleMediaError( TState aState, TBool aIsEnabled, TInt aError )
    {
    TBool mediaErrorHandled( EFalse );
    if ( aState == EDisabled && !aIsEnabled && aError != KErrNone )
        {
        MCECLI_DEBUG_DVALUE( "CMceMediaStream::HandleMediaError, fatal error:", aError )
        
        CMceManager& manager = iSession->Manager();
        
        TRAPD( err, iSession->TerminateL() )
        if ( err )
            {
            CMceSession::TState state = iSession->State();
            MCECLI_DEBUG_DVALUES( "CMceMediaStream::HandleMediaError, terminating failed:", err, 
                                  " at state:", state )
            
            // Nothing to do if already terminating                      
            if ( state != CMceSession::ETerminating && 
                 state != CMceSession::ETerminated )
                {
                TRAP_IGNORE( iSession->ErrorOccuredL( aError ) )
                }
            }
        else if ( manager.SessionObserver() )
            {
            // Inform client that we are terminating
            manager.SessionObserver()->SessionStateChanged( 
                *iSession, manager.TransactionContainer() );
            }
        else
            {
            // NOP
            }
            
        mediaErrorHandled = ETrue;
            
        MCECLI_DEBUG( "CMceMediaStream::HandleMediaError, exit" )
        }
    return mediaErrorHandled;
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::CMceMediaStream
// -----------------------------------------------------------------------------
//
CMceMediaStream::CMceMediaStream()
    :iLinkedStream( NULL ),
     iSource( NULL )
    {
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::ConstructL( CMceComMediaStream* aFlatData )
    {
    __ASSERT_ALWAYS( aFlatData , User::Leave( KErrArgument ) );
    iFlatData = aFlatData;
    FLAT_DATA( iID ) = TMceMediaId();//as not assigned
    
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::DeleteSink
// -----------------------------------------------------------------------------
//
void CMceMediaStream::DeleteSink( TInt aIndex )
    {
    MCECLI_DEBUG("CMceMediaStream::DeleteSink, Entry");
    MCECLI_DEBUG_DVALUE("sink id", iSinks[ aIndex ]->Id().iId );
    
    iSinks[ aIndex ]->ReferenceCount()--;
    if ( iSinks[ aIndex ]->ReferenceCount() == 0 )
        {
        delete iSinks[ aIndex ];
        }
    else
        {
        MCECLI_DEBUG("CMceMediaStream::DeleteSink, \
there are references to sink. Not deleted");
        iSinks[ aIndex ]->UnInitialize( *this );
        }
    iSinks.Remove( aIndex );
    MCECLI_DEBUG("CMceMediaStream::DeleteSink, Exit");
    
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::SetState
// -----------------------------------------------------------------------------
//
TBool CMceMediaStream::SetState( CMceMediaStream::TState aState )
    {
    // If in transcoding state, do not allow certain state changes
    if ( ( FLAT_DATA( iState ) == CMceMediaStream::ETranscodingRequired || 
           FLAT_DATA( iState ) == CMceMediaStream::ETranscoding ) &&
         ( aState == EBuffering ||
	       aState == EIdle ||
	       aState == EStreaming ||
	       aState == EDisabled ) )
        {
        return EFalse;
        }
    FLAT_DATA( iState ) = aState;
    
    MCECLI_DEBUG_DVALUE("CMceMediaStream::SetState, id", Id().iId );
    TState state = State();
    MCECLI_DEBUG_MEDIASTATE("CMceMediaStream::SetState, current state", state );
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::DeleteSource
// -----------------------------------------------------------------------------
//
void CMceMediaStream::DeleteSource()
    {
    MCECLI_DEBUG("CMceMediaStream::DeleteSource, Entry");
    MCECLI_DEBUG_DVALUE("source id", iSource->Id().iId );
    
    iSource->ReferenceCount()--;
    if ( iSource->ReferenceCount() == 0 )
        {
        delete iSource;
        }
    else
        {
        MCECLI_DEBUG("CMceMediaStream::DeleteSource, \
there are references to source. Not deleted");
        iSource->UnInitialize( *this );
        }
    iSource = NULL;
    MCECLI_DEBUG("CMceMediaStream::DeleteSource, Exit");
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::HandleEvent
// -----------------------------------------------------------------------------
//
TInt CMceMediaStream::HandleEvent( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceMediaStream::HandleEvent, Entry");
    MCECLI_DEBUG_DVALUES2("this: type", Type(), "id", Id().iId, 
                          "main", ( !BoundStream() || Binder() ) );
    
    TInt status = KMceEventNotConsumed;

    if ( aEvent.MessageType() == EMceItcMsgTypeStream )
        {
        MCECLI_DEBUG("CMceMediaStream::HandleEvent, content of stream changed");
        status = KMceEventUpdate;
        }
    else if ( aEvent.Action() == EMceItcStateChanged ) 
        {
        MCECLI_DEBUG("CMceMediaStream::HandleEvent, state changed");
        FLAT_DATA( iIsEnabled ) = static_cast<TBool>( aEvent.ActionData() );
        Updated();
        status = KMceEventConsumed;
        }
    else if ( aEvent.Action() == EMceItcInProgress ) 
        {
        MCECLI_DEBUG("CMceMediaStream::HandleEvent, media state changed");
        SetState( static_cast<CMceMediaStream::TState>( aEvent.ActionData() ) );
        
        if ( !HandleMediaError( FLAT_DATA( iState ), 
                                FLAT_DATA( iIsEnabled ), 
                                aEvent.Id().iStatus ) )
            {
            Updated();
            }

        status = KMceEventConsumed;
        }
    else if ( aEvent.Action() == EMceItcRRReceived ) 
        {
        MCECLI_DEBUG("CMceMediaStream::HandleEvent, RR received");
        MMceRtpObserver* observer = iSession->Manager().RtpObserver();
        if ( observer )
            {
            observer->RRReceived( *iSession, *this );
            }
        status = KMceEventConsumed;
        }
    else if ( aEvent.Action() == EMceItcLinkCreated )
        {
        MCECLI_DEBUG("CMceMediaStream::HandleEvent, link created");
        _FLAT_DATA->SetLocalMediaPort( aEvent.ActionData() );
        status = KMceEventConsumed;
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceMediaStream::HandleEvent, not consumed");
        }    
        
    MCECLI_DEBUG_DVALUE("CMceMediaStream::HandleEvent, Exit. status", status );
    
    return status;
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::DoBindL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::DoBindL( CMceMediaStream* aStream )
    {
    if ( Session() )
        {
        aStream->InitializeL( *Session(), EFalse );
        }
    delete iLinkedStream;    
    iLinkedStream = aStream;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::Bound
// -----------------------------------------------------------------------------
//
CMceMediaStream* CMceMediaStream::Bound() const
    {
    return iLinkedStream;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::FlatData
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceMediaStream::FlatData()
    {
    return iFlatData;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::FindCodec
// -----------------------------------------------------------------------------
//
CMceCodec* CMceMediaStream::FindCodec( 
    const RPointerArray<CMceCodec>& aCodecs, 
    CMceCodec& aCodec )
    {
    CMceCodec* foundCodec = NULL;
    TInt index = aCodecs.Find( &aCodec );
    if ( index == KErrNotFound )
        {
        TInt boundCodecIndex( KErrNotFound );
        if ( iLinkedStream != NULL )
            {
            // If multiple matches based exist based on SDP, index based match
            // will be done. Find out index of the codec in bound stream.
            const RPointerArray<CMceCodec>& boundCodecs = iLinkedStream->BaseCodecs();
            boundCodecIndex = boundCodecs.Find( &aCodec );
            }
            
        for ( TInt i = 0; i < aCodecs.Count(); i++ )
            {
            if ( aCodecs[ i ]->SdpName().CompareF( aCodec.SdpName() ) == 0 )
                {
                if ( foundCodec )
                    {
                    if ( boundCodecIndex != KErrNotFound &&  i == boundCodecIndex )
                        { 
                        // This codec is selected as it has the same index as
                        // search term codec has in bound stream
                        foundCodec = aCodecs[ i ];
                        }
                    }
                else
                    {
                    foundCodec = aCodecs[ i ];
                    }
                }     
            }
        }
    else
        {
        foundCodec = aCodecs[ index ];
        }
        
    return foundCodec;
    }
    
// End of file
