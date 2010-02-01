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




#include "mcecommediastream.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mcecommediasource.h"
#include "mcecommediasink.h"
#include "mcertpsource.h"
#include "mcertpsink.h"
#include "mcecomcodec.h"
#include "cleanupresetanddestroy.h"
#include "mceclient.pan"

#ifdef MCE_COMMON_SERVER_SIDE

#include <sdpcodecstringconstants.h>
#include "mcesrvstream.h"
#include "mcemediaobserver.h"
#include "mcepreconditions.h"
#include "mcesdpsession.h"

#endif



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComMediaStream::~CMceMediaStream
// -----------------------------------------------------------------------------
//
CMceComMediaStream::~CMceComMediaStream()
    {
    
    if ( iSource )
        {
        DeleteSource();
        }
    
    if ( iLinkOwner )
	    {
    	delete iLinkedStream;
    	iLinkedStream = NULL;
	    }

	while( iSinks.Count() > 0 )
	    {
	    DeleteSink( 0 );
	    }
	    
    iSinks.Reset();
    iSinks.Close();
    
    if ( iEmptySink )
    	{
    	delete iEmptySink;
    	iEmptySink = NULL;
    	}
    if ( iEmptySource )
    	{
    	delete iEmptySource;
    	iEmptySource = NULL;
    	}
    if	( iLocalMediaSDPLines )
    	{
    	delete iLocalMediaSDPLines;
    	iLocalMediaSDPLines = NULL;
    	}
    if ( iRemoteMediaSDPLines )
    	{
    	delete iRemoteMediaSDPLines;
    	iRemoteMediaSDPLines = NULL;
    	}
    
#ifdef MCE_COMMON_SERVER_SIDE
    
    iPreconditions.ResetAndDestroy();
    
#endif// MCE_COMMON_SERVER_SIDE
    
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::SetSourceL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetSourceL( CMceComMediaSource* aSource )
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
// CMceComMediaStream::AddSinkL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::AddSinkL( CMceComMediaSink* aSink )
    {
    iSinks.AppendL( aSink );
    aSink->ReferenceCount()++;
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::DeleteSink
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::DeleteSink( TInt aIndex )
    {
    
    iSinks[ aIndex ]->ReferenceCount()--;
    if ( iSinks[ aIndex ]->ReferenceCount() == 0 )
        {
        delete iSinks[ aIndex ];
        }
    else
        {
        iSinks[ aIndex ]->UnInitialize( *this );
        }
        
    iSinks.Remove( aIndex );
    
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaStream::DeleteSource
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::DeleteSource()
    {
    iSource->ReferenceCount()--;
    if ( iSource->ReferenceCount() == 0 )
        {
        delete iSource;
        }
    else
        {
        iSource->UnInitialize( *this );
        }
        
    iSource = NULL;
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::Source
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComMediaStream::Source()
    {
    return iSource;
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::Sinks
// -----------------------------------------------------------------------------
//
RPointerArray<CMceComMediaSink>& CMceComMediaStream::Sinks()
    {
    return iSinks;
    }    


// -----------------------------------------------------------------------------
// CMceComMediaStream::BindL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::BindL( CMceComMediaStream* aStream )
    {
    __ASSERT_ALWAYS( !(!iLinkOwner && iLinkedStream ) , User::Leave( KErrArgument ) );
    
    if ( aStream )
        {
        iLinkOwner = ETrue;
        aStream->DoBind( this );
        DoBind( aStream );
        }
    else
        {
        iLinkOwner = EFalse;
    	delete iLinkedStream;
    	iLinkedStream = NULL;
    	if ( iSession )
    		{
			InitializeL( *iSession );
    		}
    	
        }
        
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::DoBind
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::DoBind( CMceComMediaStream* aStream )
    {
    if ( iLinkedStream )
        {
        delete iLinkedStream;
        iLinkedStream = NULL;
        }
        
    iLinkedStream = aStream;
    
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::BoundStreamL
// -----------------------------------------------------------------------------
//
CMceComMediaStream& CMceComMediaStream::BoundStreamL()
    {
    return *iLinkedStream;
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::BoundStream
// -----------------------------------------------------------------------------
//
TBool CMceComMediaStream::BoundStream() const
    {
    return iLinkedStream != NULL;
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::Binder
// -----------------------------------------------------------------------------
//
TBool CMceComMediaStream::Binder() const
    {
    return iLinkOwner;
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::operator()
// -----------------------------------------------------------------------------
//
CMceComMediaStream& CMceComMediaStream::operator()()
    {
    CMceComMediaStream* owner = this;
    
    if ( BoundStream() && !Binder() )
        {
        owner = iLinkedStream;
        }

    return *owner;
    
    }
       
// -----------------------------------------------------------------------------
// CMceComMediaStream::operator()
// -----------------------------------------------------------------------------
//
const CMceComMediaStream& CMceComMediaStream::operator()() const
    {
    const CMceComMediaStream* owner = this;
    
    if ( BoundStream() && !Binder() )
        {
        owner = iLinkedStream;
        }

    return *owner;
    
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceComMediaStream::SerializationId() const
    {
    return iID.Uint64();
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaStream::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::InternalizeFlatL( RReadStream& aReadStream )
    {
    
    //iType
    iType = static_cast<TMceMediaType>( aReadStream.ReadUint8L() );
    //iID
    MceSerial::DecodeL( iID, aReadStream );
    //iState
    iIsEnabled = static_cast< TBool >( aReadStream.ReadUint8L() );
    //iLocalMediaPort
    iLocalMediaPort = aReadStream.ReadUint16L();
    //iRemoteMediaPort
	iRemoteMediaPort = aReadStream.ReadUint16L();
    //iLocalMediaSDPLines
    MceSerial::DecodeL( iLocalMediaSDPLines, aReadStream ); 
    //iRemoteMediaSDPLines
    MceSerial::DecodeL( iRemoteMediaSDPLines, aReadStream ); 
    //iLinkOwner
    iLinkOwner = static_cast<TBool>( aReadStream.ReadUint8L() );
    //iState
    iState = static_cast<CMceMediaStream::TState>( aReadStream.ReadUint8L() );
    //iIgnoreRemotePort
    iIgnoreRemotePort = static_cast<TBool>( aReadStream.ReadUint8L() );    
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaStream::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    
	aWriteStream.WriteUint8L( iType );
    MceSerial::EncodeL( iID, aWriteStream );
	aWriteStream.WriteUint8L( iIsEnabled );
	aWriteStream.WriteUint16L( iLocalMediaPort );
    aWriteStream.WriteUint16L( iRemoteMediaPort );
	MceSerial::EncodeL( iLocalMediaSDPLines, aWriteStream );
	MceSerial::EncodeL( iRemoteMediaSDPLines, aWriteStream );
	aWriteStream.WriteUint8L( iLinkOwner );
	aWriteStream.WriteUint8L( iState );
	aWriteStream.WriteUint8L( iIgnoreRemotePort );
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    
    TMceMediaStreamSerializer<CMceComMediaStream> serial( *this );
    serial.InternalizeL( aSerCtx );
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    
    TMceMediaStreamSerializer<CMceComMediaStream> serial( *this );
    serial.ExternalizeL( aSerCtx );
    
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::CMceComMediaStream
// -----------------------------------------------------------------------------
//
CMceComMediaStream::CMceComMediaStream( TMceMediaType aType )
  : iID( TMceMediaId( (TUint32)&iID ) ),
    iType( aType ),
    iIsEnabled( ETrue ),
    iLocalMediaPort (5000),
    iRemoteRtcpAddress ( KInetAddrAny ),
    iRemoteRtcpPort( 0 ),
    iStreamType( ELocalStream ),
    iState( CMceMediaStream::EUninitialized ),
    iLinkOwner( EFalse ),
    iLinkedStream( NULL ),
    iSource( NULL ),
    iSdpIndex( KErrNotFound ),
    iLinkId( KMceNotAssigned ),
    iDowngradedEnpoints( EFalse )
    {
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::ConstructL()
    {
    
    iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    iRemoteMediaSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::ConstructL( CMceComMediaStream& aStream )
    {
    
    iID = aStream.iID;
    iType = aStream.iType;
    iIsEnabled = aStream.iIsEnabled;
    iState = aStream.iState;
    iLocalMediaPort = aStream.iLocalMediaPort;
    iRemoteMediaPort = aStream.iRemoteMediaPort;
    iIgnoreRemotePort = aStream.iIgnoreRemotePort;
    iRemoteRtcpAddress = aStream.iRemoteRtcpAddress;
    iRemoteRtcpPort = aStream.iRemoteRtcpPort;
    iLocalMediaSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    iRemoteMediaSDPLines = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    
    iSdpIndex = aStream.iSdpIndex;
    iDowngradedEnpoints = aStream.iDowngradedEnpoints;
    
    TInt i = 0;
    for( i = 0;i<aStream.iLocalMediaSDPLines->MdcaCount();i++ )
        {
        iLocalMediaSDPLines->AppendL( aStream.iLocalMediaSDPLines->MdcaPoint( i ) );
        }
        
    for( i = 0;i<aStream.iRemoteMediaSDPLines->MdcaCount();i++ )
        {
        iRemoteMediaSDPLines->AppendL( aStream.iRemoteMediaSDPLines->MdcaPoint( i ) );
        }
        
    iLinkOwner = aStream.iLinkOwner;
    
    if ( aStream.Binder() && aStream.BoundStream() )
        {
        iLinkedStream = static_cast<CMceComMediaStream*>( aStream.iLinkedStream->CloneL() );
        iLinkedStream->iLinkedStream = this;
        }
        
    if ( aStream.iSource )
        {
        CMceComMediaSource* source = aStream.iSource->CloneL();
        CleanupStack::PushL( source );
        SetSourceL( source );
        CleanupStack::Pop( source );
        }
        
    for( i = 0; i < aStream.iSinks.Count();i++)
        {
        CMceComMediaSink* sink = aStream.iSinks[ i ]->CloneL();
        CleanupStack::PushL( sink );
        AddSinkL( sink );
        CleanupStack::Pop( sink );
        }
        
    if ( aStream.iEmptySource )
        {
        iEmptySource = aStream.iEmptySource->CloneL();
        }
        
    if ( aStream.iEmptySink )
        {
        iEmptySink = aStream.iEmptySink->CloneL();
        }
        
#ifdef MCE_COMMON_SERVER_SIDE
    
    if ( aStream.iPreconditions.Count() )
        {
        for ( int j = 0; j < aStream.iPreconditions.Count(); j++ )
        	{
        	TMcePreconditions* precondition = 
        				static_cast < TMcePreconditions* > ( aStream.iPreconditions[j] );
        	TMcePreconditions* clone = precondition->CloneL( *this );
        	CleanupStack::PushL( clone );
        	iPreconditions.AppendL( clone );
        	CleanupStack::Pop( clone );
        	}
        }
    else	
        {
        iPreconditions.Reset();
        }
    
#endif// MCE_COMMON_SERVER_SIDE
        
    }



// -----------------------------------------------------------------------------
// CMceComMediaStream::BaseFactory
// -----------------------------------------------------------------------------
//
TMceComFactory CMceComMediaStream::BaseFactory()
    {
    return TMceComFactory();
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::Factory
// -----------------------------------------------------------------------------
//
TMceComMediaStreamFactory CMceComMediaStream::Factory()
    {
    return TMceComMediaStreamFactory();
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::SetSourceL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetSourceL( MMceComSerializationContext& aSerCtx )
    {

    RReadStream& readStream = aSerCtx.ReadStream();

    TBool externalizedReference = readStream.ReadUint8L();
    
    if ( externalizedReference )
        {
        TMceMediaId id;
        MceSerial::DecodeL( id, readStream );
        CMceComMediaSource* source = NULL;
        TInt index = 0;
        while( !source && index < aSerCtx.SourceCache().Count() )
            {
            source = static_cast<CMceComMediaSource*>( 
                                            aSerCtx.SourceCache()[ index++ ] );
            source = source->SerializationId() == id.Uint64() ? source : NULL;
            }
        __ASSERT_ALWAYS( source, User::Leave( KErrNotFound ) );
        SetSourceL( source );
        }
    else
        {
        CMceComMediaSource* source = 
            BaseFactory().SourceFactory().CreateLC( aSerCtx );
        SetSourceL( source );
        CleanupStack::Pop( source );
        aSerCtx.SourceCache().AppendL( source );
        
        }
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::AddSinkL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::AddSinkL( MMceComSerializationContext& aSerCtx )
    {

    RReadStream& readStream = aSerCtx.ReadStream();

    TBool externalizedReference = readStream.ReadUint8L();
    
    if ( externalizedReference )
        {
        TMceMediaId id;
        MceSerial::DecodeL( id, readStream );
        CMceComMediaSink* sink = NULL;
        TInt index = 0;
        while( !sink && index < aSerCtx.SinkCache().Count() )
            {
            sink = static_cast<CMceComMediaSink*>( aSerCtx.SinkCache()[ index++ ] );
            sink = sink->SerializationId() == id.Uint64() ? sink : NULL;
            }
        __ASSERT_ALWAYS( sink, User::Leave( KErrNotFound ) );
        AddSinkL( sink );
        }
    else
        {
        CMceComMediaSink* sink = 
            BaseFactory().SinkFactory().CreateLC( aSerCtx );
        AddSinkL( sink );
        CleanupStack::Pop( sink );
        aSerCtx.SinkCache().AppendL( sink );
        
        }
    
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::Id
// -----------------------------------------------------------------------------
//
TMceMediaId CMceComMediaStream::Id() const
    {
    return iID;
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::InitializeL( CMceComSession& aParent )
    {
    iSession = &aParent;
    
    iStreamType = ELocalStream;
    
    if ( Binder() && BoundStream() )
        {
        iLinkedStream->InitializeL( aParent );
        }
        
    if ( iSource )
        {
        iSource->InitializeL( *this );
        if ( iSource->iType == KMceRTPSource )
            {
            iStreamType = BoundStream() ? EReceiveStream : EReceiveOnlyStream;
            }
        }
        
        
    for( int i = 0; i < iSinks.Count();i++)
        {
        iSinks[i]->InitializeL( *this );
        if ( iStreamType == ELocalStream && 
             iSinks[i]->iType == KMceRTPSink )
            {
            iStreamType = BoundStream() ? ESendStream : ESendOnlyStream;
            }
        }
        
    if ( iEmptySource )
        {
        iEmptySource->InitializeL( *this );
        }
        
    if ( iEmptySink )
        {
        iEmptySink->InitializeL( *this );
        }
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::Session
// -----------------------------------------------------------------------------
//
CMceComSession* CMceComMediaStream::Session() const
    {
    return iSession;
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::SetRemoteMediaPort
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetRemoteMediaPort( TUint aPort )
    {
    
    iRemoteMediaPort = aPort;
    
    if ( BoundStream() )
        {
        iLinkedStream->iRemoteMediaPort = aPort;
        }
        
    }    

// -----------------------------------------------------------------------------
// CMceComMediaStream::SetRemoteRtcpMediaPort
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetRemoteRtcpMediaAddrL( TDesC16& aAddr )
    {
    User::LeaveIfError ( 
                    iRemoteRtcpAddress.Input( aAddr ) );
    
    if ( BoundStream() )
        {
        iLinkedStream->iRemoteRtcpAddress = iRemoteRtcpAddress;
        }
        
    }
// -----------------------------------------------------------------------------
// CMceComMediaStream::SetRemoteRtcpMediaPort
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetRemoteRtcpMediaPort( TUint aPort )
    {
    
    iRemoteRtcpPort = aPort;
    
    if ( BoundStream() )
        {
        iLinkedStream->iRemoteRtcpPort = aPort;
        }
        
    }    

// -----------------------------------------------------------------------------
// CMceComMediaStream::SetLocalMediaPort
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetLocalMediaPort( TUint aPort )
    {
    
    iLocalMediaPort = aPort;
    
    if ( BoundStream() )
        {
        iLinkedStream->iLocalMediaPort = aPort;
        }
        
    }    
	
	
// -----------------------------------------------------------------------------
// CMceComMediaStream::RemoteMediaPort
// -----------------------------------------------------------------------------
//
TUint CMceComMediaStream::RemoteMediaPort() const
	{
	return iRemoteMediaPort;
	}

// -----------------------------------------------------------------------------
// CMceComMediaStream::LocalMediaPort
// -----------------------------------------------------------------------------
//
TUint CMceComMediaStream::LocalMediaPort() const
	{
	return iLocalMediaPort;
	}

// -----------------------------------------------------------------------------
// CMceComMediaStream::RemoteMediaPortChanged
// -----------------------------------------------------------------------------
//
TBool CMceComMediaStream::RemoteMediaPortChanged( TUint aNewPort )
    {
    return ( aNewPort != iRemoteMediaPort && !iIgnoreRemotePort );
    }
    
    
// -----------------------------------------------------------------------------
// CMceComMediaStream::RemoteRTCPAddressChanged
// -----------------------------------------------------------------------------
//
TBool CMceComMediaStream::RemoteRTCPAddressChanged( TUint aNewPort, TInetAddr aAddr )
    {
    return ( (aNewPort != iRemoteRtcpPort && !iIgnoreRemotePort) ||
    		(aAddr != iRemoteRtcpAddress && !iIgnoreRemotePort ));
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::SetIgnoreRemotePort
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetIgnoreRemotePort( TBool aIgnore )
    {
    // Remote port comparison can be turned on/off. For example nat session might
    // change remote port to be different than defined in sdp (remote port is
    // actually in own loopback address) and in such case comparison is not 
    // valid.
    iIgnoreRemotePort = aIgnore;
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaStream::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComMediaStream::EventReceivedL( TMceComEvent& aEvent )
    {
    TInt status = aEvent.Id().iMediaID == iID ? KMceEventNotConsumed : KMceEventNotOwner;

    //try binded
    if ( status == KMceEventNotOwner )
        {
        if ( Binder() && BoundStream() )
            {
            status = iLinkedStream->EventReceivedL( aEvent );
            }
        return status;
        }
        
    //try source
    if ( aEvent.Id().IsSourceId() )
        {
        if ( iSource )
            {
            status = iSource->EventReceivedL( aEvent );
            status = status == KMceEventNotOwner ? KMceEventNotConsumed : status;
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
            CMceComMediaSink* sink = iSinks[j]; 
            sinkStatus = sink->EventReceivedL( aEvent );
            j++;                
            }
        status = sinkStatus == KMceEventNotOwner ? KMceEventNotConsumed : sinkStatus;    
        return status;            
        }

    return status;
    }




// -----------------------------------------------------------------------------
// CMceComMediaStream::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::UpdateL( CMceComMediaStream& aStream )
    {
    
    iType = aStream.iType;
    iID = aStream.iID;
    iIsEnabled = aStream.iIsEnabled;
    iLocalMediaPort = aStream.iLocalMediaPort;
    // If stream is in ignoring mode, remote port is not updated
    if ( !iIgnoreRemotePort )
        {
        iRemoteMediaPort = aStream.iRemoteMediaPort;
        }
    iIgnoreRemotePort = aStream.iIgnoreRemotePort;
    iState = aStream.iState;
    
    delete iLocalMediaSDPLines;
    iLocalMediaSDPLines = aStream.iLocalMediaSDPLines;
    aStream.iLocalMediaSDPLines = NULL;
    
    delete iRemoteMediaSDPLines;
    iRemoteMediaSDPLines = aStream.iRemoteMediaSDPLines;
    aStream.iRemoteMediaSDPLines = NULL;
    
    
    if ( aStream.BoundStream() )
        {
        if ( BoundStream() ) 
            {
            if ( Binder() )
                {
                iLinkedStream->UpdateL( *aStream.iLinkedStream );
                }
            }
        else 
            {
            Session()->IsStructureChanged() = ETrue;
            CMceComMediaStream* newStream = aStream.iLinkedStream;
            newStream->iLinkedStream = NULL;
            newStream->iLinkOwner = EFalse;
            aStream.iLinkedStream = NULL;
            aStream.iLinkOwner = EFalse;
            CleanupStack::PushL( newStream );
            BindL( newStream );
            CleanupStack::Pop( newStream );
            }
        }
    else if ( !aStream.BoundStream() && BoundStream() )
        {
        Session()->IsStructureChanged() = ETrue;
        BindL( NULL );
        }
    else
        {
        //NOP
        }
        
    if ( Source() && aStream.Source() )
        {
        if ( Source()->Id() == aStream.Source()->Id() )
            {
            iSource->UpdateL( *aStream.Source() );
            }
        else
            {
            SetSourceL( aStream.Source() );
            }
        }
    else if ( !Source() && aStream.Source() )
        {
        SetSourceL( aStream.Source() );
        }
    else 
        {
        Session()->IsStructureChanged() = ETrue;
        SetSourceL( NULL );
        }
     
    
    RPointerArray<CMceComMediaSink> updated;
    CleanupClosePushL( updated );
    
    //updated sinks
    TInt i = 0;
    TInt j = 0;
    CMceComMediaSink* sink = NULL;
    CMceComMediaSink* update = NULL;
    for( i = 0; i < aStream.Sinks().Count(); i++ )
        {
        update = aStream.Sinks()[i];
        TBool isUpdated = EFalse;
        TInt k = 0;
        while( !isUpdated && k < Sinks().Count() )
            {
            sink = Sinks()[k];
            if ( sink->Id() == update->Id() )
                {
                sink->UpdateL( *update );
                updated.AppendL( sink );
                isUpdated = ETrue;
                }
            k++;                
            }
        }

    //removed sinks
    sink = NULL;
    update = NULL;
    for( i = 0; i < Sinks().Count(); i++ )
        {
        sink = Sinks()[ i ];
        update = NULL;
        j = 0;
        while( !update && j < updated.Count() )
            {
            update = updated[ j++ ];
            update = update->Id() == sink->Id() ? update : NULL;
            }
        if ( !update )
            {
            Session()->IsStructureChanged() = ETrue;
            DeleteSink( i );
            i--;
            }
        }

    //added sinks    
    sink = NULL;
    update = NULL;
    for( i = 0; i < aStream.Sinks().Count(); i++ )
        {
        sink = aStream.Sinks()[ i ];
        update = NULL;
        j = 0;
        while( !update && j < updated.Count() )
            {
            update = updated[ j++ ];
            update = update->Id() == sink->Id() ? update : NULL;
            }
        if ( !update )
            {
            Session()->IsStructureChanged() = ETrue;
            AddSinkL( sink );//'reference counted'
            }
        }
    
    CleanupStack::PopAndDestroy();//updated
    
    }
    
    
// -----------------------------------------------------------------------------
// CMceComMediaStream::IsEnabled
// -----------------------------------------------------------------------------
//
TBool CMceComMediaStream::IsEnabled()
    {
    return iIsEnabled;
    }
    

// -----------------------------------------------------------------------------
// CMceComMediaStream::SendStream
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComMediaStream::SendStream()
    {
    CMceComMediaStream* sendStream = NULL;
    
    if ( IS_SENDSTREAM( this ) )
        {
        sendStream = this;
        }
    else if ( BoundStream() )
        {
        if ( IS_SENDSTREAM( iLinkedStream ) )
            {
            //The bound stream is uplink
            sendStream = iLinkedStream;
            }
        }
    else
        {
        //NOP
        }
        
    return sendStream;
    
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::ReceiveStream
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComMediaStream::ReceiveStream()
    {
    CMceComMediaStream* receiveStream = NULL;
    
    if ( IS_RECEIVESTREAM( this ) )
        {
        receiveStream = this;
        }
    else if ( BoundStream() )
        {
        if ( IS_RECEIVESTREAM( iLinkedStream ) )
            {
            //The bound stream is uplink
            receiveStream = iLinkedStream;
            }
        }
    else
        {
        //NOP
        }
        
    return receiveStream;
    
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::OfferStream
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComMediaStream::OfferStream()
    {
    CMceComMediaStream* offerStream = NULL;
    
    offerStream = ReceiveStream();
    if ( !offerStream )
        {
        //try send only
        offerStream = SendStream();
        __ASSERT_ALWAYS( !offerStream ||
                         offerStream->iStreamType == ESendOnlyStream,
                         User::Panic( KMceServerPanic, KErrGeneral ) );
        }

    return offerStream;
    
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::AnswerStream
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceComMediaStream::AnswerStream()
    {
    CMceComMediaStream* answerStream = NULL;
    
    answerStream = SendStream();
    if ( !answerStream )
        {
        //try receive only
        answerStream = ReceiveStream();
        __ASSERT_ALWAYS( !answerStream ||
                         answerStream->iStreamType == EReceiveOnlyStream,
                         User::Panic( KMceServerPanic, KErrGeneral ) );
        }

    return answerStream;
    
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::SdpIndex
// -----------------------------------------------------------------------------
//
TInt& CMceComMediaStream::SdpIndex()
    {
    CMceComMediaStream& main = (*this)();

    return main.iSdpIndex;
    
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaStream::UseRtcp
// -----------------------------------------------------------------------------
//
TBool CMceComMediaStream::UseRtcp() const
    {
    TBool useRtcp = DoUseRtcp();
    if ( useRtcp && BoundStream() )
	    {
	    useRtcp = iLinkedStream->DoUseRtcp();	
	    }
    return useRtcp;
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::DoUseRtcp
// -----------------------------------------------------------------------------
//
TBool CMceComMediaStream::DoUseRtcp() const
    {
    TBool useRtcp( ETrue );
    if ( iSource )
        {
        useRtcp = iSource->UseRTCP();
        }
    for( TInt i=0; i < iSinks.Count() && useRtcp; i++ )
        {
        useRtcp = iSinks[i]->UseRTCP();
        }
    return useRtcp;
    }    
    
#ifdef MCE_COMMON_SERVER_SIDE      

// -----------------------------------------------------------------------------
// CMceComMediaStream::DecodedL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::DecodedL( TBool aRoleAnswerer )
    {    
    SynchronizeL( aRoleAnswerer );
    
    ReorderCodecs();
        
    if ( BoundStream() )
        {
        BoundStreamL().ReorderCodecs();
        }
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::EmptySinkL
// -----------------------------------------------------------------------------
//
CMceComMediaSink& CMceComMediaStream::EmptySinkL()
    {
    if ( !iEmptySink )
        {
        iEmptySink = CMceComMediaSink::NewL();
        iEmptySink->InitializeL( *this );
        }
    return *iEmptySink;
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::EmptySourceL
// -----------------------------------------------------------------------------
//    
CMceComMediaSource& CMceComMediaStream::EmptySourceL()
    {
    if ( !iEmptySource )
        {
        iEmptySource = CMceComMediaSource::NewL();
        iEmptySource->InitializeL( *this );
        }
    return *iEmptySource;
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::EventReceived
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::EventReceived( TMceMccComEvent& aEvent )
    {
    
    //TBD: handle situation where there are multiple codecs and sinks
//    if ( Codecs().Count() == 1 && Sinks().Count() == 1 )
//        {

        aEvent.iStream = this;
                    
        if ( aEvent.iSource )
            {
            aEvent.iSource->EventReceived( aEvent );
            }
        else if ( aEvent.iSink )
            {
            aEvent.iSink->EventReceived( aEvent );
            }
        //state event with endpointid 0
        else 
            {
            if ( aEvent.iEvent == KMccStreamPaused ||
                 aEvent.iEvent == KMccStreamResumed )
                {
                Source()->EventReceived( aEvent );
                for( TInt i = 0;i<Sinks().Count();i++)
                    {
                    Sinks()[i]->EventReceived( aEvent );
                    }
                }
            else
                {
                
                CMceComSession* session = Session();
                
                switch( aEvent.iEvent )
                    {
                    case KMccInactivityEvent:
                        {
                        aEvent.iItcEvent = EMceItcInactivityTimeout;
                        aEvent.iItcData = iState;
                        session->MediaObserver().EventReceived( aEvent );
                        break;
                        }
                    case KMccStreamError:
                        {
                        aEvent.iItcEvent = EMceItcInProgress;
                        aEvent.iItcData = iState;
                        session->MediaObserver().MediaError( aEvent );
                        break;
                        }
                    case KMccRtcpReceived:
                        {
                        aEvent.iItcEvent = EMceItcNotAssigned;
                        switch( aEvent.iEventSubType )
                            {
                            case KRtcpSrPacket:
                                {
                                aEvent.iItcEvent = EMceItcSRReceived;
                                break;
                                }
                            case KRtcpRrPacket:
                                {
                                aEvent.iItcEvent = EMceItcRRReceived;
                                break;
                                }
                            case KRtcpPacketUndefined:
                                {
                                aEvent.iItcEvent = EMceItcFCMsgReceived;
                                break;
                                }
                            default:
                                {
                                //NOP
                                break;
                                }
                            
                            }
                        if ( aEvent.iItcEvent != EMceItcNotAssigned )
                            {
                            aEvent.iItcData = iState;
                            session->MediaObserver().EventReceived( aEvent );
                            }
                        break;
                        }
                    case KMccStreamIdle:
                    case KMccStreamPlaying:
                    case KMccStreamStarted:
                    case KMccStreamBuffering:
                    case KMccStreamClosed:
                    case KMccStreamStopped:
                    case KMccResourceNotAvailable:
                    case KMccActivityEvent:
                    case KMccResourceReserved:
                    case KMccResourceReleased:
                        {
                        aEvent.iItcEvent = EMceItcInProgress;
                        aEvent.iItcData = iState;
                        session->MediaObserver().EventReceived( aEvent );
                        break;
                        }
                    case KMccLinkCreated:
                        {
                        aEvent.iItcEvent = EMceItcLinkCreated;
                        aEvent.iItcData = LocalMediaPort();
                        session->MediaObserver().EventReceived( aEvent );
                        break;
                        }
                    default:
                        {
                        //NOP
                        break;
                        }
                    }
                }
            }
            
         //}
        
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::Merge
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::Merge( CMceComMediaStream& aMergeWith )
    {
    iState = aMergeWith.iState;
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::SetState
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetState( TMceMccComEvent& aEvent )
    {
    //update stream state
    switch( aEvent.iEvent )
        {
        case KMccStreamResumed:
        case KMccStreamPlaying:
        case KMccStreamStarted:
            {
            iState = CMceMediaStream::EStreaming;
            break;
            }
        case KMccStreamBuffering:
            {
            iState = CMceMediaStream::EBuffering;
            break;
            }
        case KMccStreamIdle:
            {
            iState = CMceMediaStream::EIdle;
            break;
            }
        case KMccStreamPaused:
        case KMccStreamStopped:
        case KMccStreamClosed:
        case KMccStreamError:
            {
            iState = CMceMediaStream::EDisabled;
            break;
            }
        case KMccResourceNotAvailable:
            {
            iState = CMceMediaStream::ENoResources;
            break;
            }
        case KMccActivityEvent:
            {
            if ( iState == CMceMediaStream::EInitialized )
                {
                iState = CMceMediaStream::EIdle;
                }
            break;
            }
        case KMccInactivityEvent:
        case KMccResourceReserved:
        case KMccResourceReleased:
        case KMccRtcpReceived:
        default:
            {
            //NOP
            break;
            }
        }
        
    if ( aEvent.iSource )
        {
        aEvent.iSource->SetState( aEvent );
        }
    else if ( aEvent.iSink )
        {
        aEvent.iSink->SetState( aEvent );
        }
    else
        {
        // Codec state changes not used at the moment (enabled flag
        // should not be switched by events).
        //aEvent.iCodec->SetState( aEvent );
        Source()->SetState( aEvent );
        for( TInt i = 0;i<Sinks().Count();i++)
            {
            Sinks()[i]->SetState( aEvent );
            }
        }
    }
    

// -----------------------------------------------------------------------------
// CMceComMediaStream::SetLinkId
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetLinkId( TUint32 aLinkId )
    {
    
    iLinkId = aLinkId;
    
    if ( BoundStream() )
        {
        iLinkedStream->iLinkId = aLinkId;
        }
        
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::Direction
// -----------------------------------------------------------------------------
//
TInt CMceComMediaStream::Direction()
    {
    TInt sdpType = SdpStreamType();
    
    switch( sdpType  )
        {
        case SdpCodecStringConstants::EAttributeSendrecv:
            {
            if ( !IsEnabled() && !iLinkedStream->IsEnabled() )
                {
                sdpType = SdpCodecStringConstants::EAttributeInactive;
                }
            else if ( !IsEnabled() && iLinkedStream->IsEnabled() )
                {
                sdpType = iLinkedStream->iStreamType == EReceiveStream ? 
                            SdpCodecStringConstants::EAttributeRecvonly :
                            SdpCodecStringConstants::EAttributeSendonly;
                }
            else if ( IsEnabled() && !iLinkedStream->IsEnabled() )
                {
                sdpType = iStreamType == EReceiveStream ? 
                            SdpCodecStringConstants::EAttributeRecvonly :
                            SdpCodecStringConstants::EAttributeSendonly;
                }
            else 
                {
                //NOP
                }
            break;
            }
        case SdpCodecStringConstants::EAttributeRecvonly: 
        case SdpCodecStringConstants::EAttributeSendonly:
            {
            sdpType = IsEnabled() ? sdpType : SdpCodecStringConstants::EAttributeInactive;
            break;
            }
        default:
            {
            break;
            }
    
        }
    
    return sdpType;
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::SetDirectionL
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetDirectionL( TInt aDirection )
    {
    TInt streamDirection = Direction();
    TBool valid = aDirection == SdpCodecStringConstants::EAttributeInactive;

    if ( !valid )
        {
        if ( streamDirection == SdpCodecStringConstants::EAttributeRecvonly )
            {
            valid = aDirection == SdpCodecStringConstants::EAttributeSendonly;
            }
        else if ( streamDirection == SdpCodecStringConstants::EAttributeSendonly )
            {
            valid = aDirection == SdpCodecStringConstants::EAttributeRecvonly;
            }
        else
            {
            valid = ETrue;
            }
        }
    
    CMceSdpSession& sSession = Session()->SdpSession();       
    if ( sSession.iOOldSchool == 0 &&  
    	 !valid && 
    	 ( Session()->Modifier( KMceMediaDirection ) == KMceMediaDirectionWithAddress ) )
        {
        sSession.iOOldSchool = 1;    
	    User::Leave( KMceErrOldSchool );
        }
        
	User::LeaveIfError( !valid ? KErrArgument : KErrNone );
		
    SetDirection( aDirection, KMceNoEndpointAdjustment );
        
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::DowngradedEndpoints
// -----------------------------------------------------------------------------
//
TBool& CMceComMediaStream::DowngradedEndpoints()
    {
    return iDowngradedEnpoints;
    }

// -----------------------------------------------------------------------------
// CMceComMediaStream::Enable
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::Enable( TBool aEnable, TBool aEndpointAdjustment )
    {
    DowngradedEndpoints() = EFalse;
    
    iIsEnabled = aEnable;
    if ( aEndpointAdjustment )
        {
        DowngradedEndpoints() = !aEnable;
        if ( iSource )
            {
            iSource->Enabled( aEnable );
            }
        for( TInt i = 0;i<iSinks.Count();i++)
            {
            iSinks[ i ]->Enabled( aEnable );
            }
        }
    }
        
// -----------------------------------------------------------------------------
// CMceComMediaStream::SetDirection
// -----------------------------------------------------------------------------
//
void CMceComMediaStream::SetDirection( TInt aDirection, TBool aEndpointAdjustment )
    { 
	if ( !aEndpointAdjustment && Direction() != aDirection &&
	     !(Direction() == SdpCodecStringConstants::EAttributeSendonly 
	       && aDirection == SdpCodecStringConstants::EAttributeRecvonly) &&
	     ! (Direction() == SdpCodecStringConstants::EAttributeRecvonly 
	       && aDirection == SdpCodecStringConstants::EAttributeSendonly) )//direction has changed
		{
		aEndpointAdjustment = KMceEndpointAdjustment;
		}
   
    switch( aDirection )
        {
        case SdpCodecStringConstants::EAttributeSendonly:
            {
            if ( iStreamType == EReceiveStream && 
                 iLinkedStream->iStreamType == ESendStream )
                {
                Enable( ETrue, aEndpointAdjustment );
                iLinkedStream->Enable( EFalse, aEndpointAdjustment );
                }
            else if ( iStreamType == ESendStream && 
                      iLinkedStream->iStreamType == EReceiveStream )
                {
                Enable( EFalse, aEndpointAdjustment );
                iLinkedStream->Enable( ETrue, EFalse );
                }
            else if ( iStreamType == ESendOnlyStream )
                {
                Enable( EFalse, aEndpointAdjustment );
                }
            else if ( iStreamType == EReceiveOnlyStream )
                {
                Enable( ETrue, aEndpointAdjustment );
                }
            break;
            }
        case SdpCodecStringConstants::EAttributeRecvonly:
            {
            if ( iStreamType == EReceiveStream && 
                 iLinkedStream->iStreamType == ESendStream )
                {
                Enable( EFalse, aEndpointAdjustment );
                iLinkedStream->Enable( ETrue, aEndpointAdjustment );
                }
            else if ( iStreamType == ESendStream && 
                      iLinkedStream->iStreamType == EReceiveStream )
                {
                Enable( ETrue, EFalse );
                iLinkedStream->Enable( EFalse, aEndpointAdjustment );
                }
            else if ( iStreamType == EReceiveOnlyStream )
                {
                Enable( EFalse, aEndpointAdjustment );
                }
            else if ( iStreamType == ESendOnlyStream )
                {
                Enable( ETrue, aEndpointAdjustment );
                }
            break;
            }
        case SdpCodecStringConstants::EAttributeSendrecv:
        case KErrNotFound:
            {
            if ( !IsEnabled() && iLinkedStream && iLinkedStream->IsEnabled() )
                {
                iLinkedStream->Enable( ETrue, EFalse );
                Enable( ETrue, aEndpointAdjustment ); 
                }
            else if ( IsEnabled() && iLinkedStream && !iLinkedStream->IsEnabled() )
                {
                Enable( ETrue, EFalse ); 
                iLinkedStream->Enable( ETrue, aEndpointAdjustment ); 
                }
            else if ( !IsEnabled() && iLinkedStream && !iLinkedStream->IsEnabled() ) 
                {
                Enable( ETrue, aEndpointAdjustment );   
                iLinkedStream->Enable( ETrue, aEndpointAdjustment );   
                }
            else if ( !iLinkedStream )
                {
                Enable( ETrue, EFalse ); 
                }
            else
                {
                iLinkedStream->Enable( ETrue, EFalse );
                Enable( ETrue, EFalse );    
                }
            
            break;
            }
        case SdpCodecStringConstants::EAttributeInactive:
            {
            Enable( EFalse, KMceEndpointAdjustment );
            if ( iLinkedStream )
                {
                iLinkedStream->Enable( EFalse, KMceEndpointAdjustment );
                }
            break;
            }
        default:
            {
            //NOP
            break;
            }
        }
    }
    
    
// -----------------------------------------------------------------------------
// CMceComMediaStream::SdpStreamType
// -----------------------------------------------------------------------------
//
TInt CMceComMediaStream::SdpStreamType()
    {
    TInt sdpType = KErrNotFound;
    
    switch( iStreamType  )
        {
        case EReceiveStream:
        case ESendStream:
            {
            sdpType = SdpCodecStringConstants::EAttributeSendrecv;
            break;
            }
        case EReceiveOnlyStream: 
            {
            sdpType = SdpCodecStringConstants::EAttributeRecvonly;
            break;
            }
        case ESendOnlyStream:
            {
            sdpType = SdpCodecStringConstants::EAttributeSendonly;
            break;
            }
        default:
            {
            break;
            }
    
        }
    
    return sdpType;
    
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::PreconditionsL
// -----------------------------------------------------------------------------
//
TMcePreconditions* CMceComMediaStream::PreconditionsL( TMceSessionModifier aModifier,
														CSdpMediaField* aMediaLine )
    {
    CMceComMediaStream& main = (*this)();
    TInt count = main.iPreconditions.Count();
    TMcePreconditions* precondition = NULL;
    
    for ( TInt n = 0; n < count; n++ )
 		{
 		TMcePreconditions* search = main.iPreconditions[ n ];
 		if ( aModifier == KMceSecPreconditions &&
 		search->Type() == TMcePreconditions :: ESecPreconds )
 			{
 			precondition = search;
 			}
 		if ( aModifier == KMcePreconditions &&
 		search->Type() == TMcePreconditions :: EQosPreconds )
 			{
 			precondition = search;
 			}
 		}
    
    if ( !precondition ) 
    	{
    	
    	TMcePreconditionsFactory factory;	
        
        if ( aModifier == KMceSecPreconditions ) 
        	{
        	//only create sec precondition 
        	precondition = factory.CreateL( *this, *aMediaLine );
        	}
		else
			{
			//create qos precondition
        	precondition = factory.CreateL( *this );
			}
			
	    if ( precondition )
	    	{
	    	CleanupStack::PushL( precondition );
        	main.iPreconditions.AppendL( precondition );
	    	CleanupStack::Pop( precondition );
	    	}
	   
        }
    
        
    
    return precondition;
    
    }


// -----------------------------------------------------------------------------
// CMceComMediaStream::Preconditions
// -----------------------------------------------------------------------------
//
const RPointerArray<TMcePreconditions>& CMceComMediaStream::Preconditions() const
    {
    
    const CMceComMediaStream& main = (*this)();
    return main.iPreconditions;
    }


    
#endif//MCE_COMMON_SERVER_SIDE

