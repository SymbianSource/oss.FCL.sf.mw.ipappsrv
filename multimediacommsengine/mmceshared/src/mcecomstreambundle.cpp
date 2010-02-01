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




#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>

#endif//MCE_COMMON_SERVER_SIDE

#include "mcecomstreambundle.h"
#include "mcecomsession.h"
#include "mcestreambundle.h"
#include "mcecomfactory.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mcecommediasink.h"
#include "mcecommediasource.h"
#include "mcecomendpointproxy.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComStreamBundle::NewL
// -----------------------------------------------------------------------------
//
CMceComStreamBundle* CMceComStreamBundle::NewL( CMceStreamBundle::TMceStreamBundleType aType )
    {
    CMceComStreamBundle* self = NewLC( aType );
    CleanupStack::Pop( self );
    return self;   
    
    }

// -----------------------------------------------------------------------------
// CMceComStreamBundle::NewLC
// -----------------------------------------------------------------------------
//
CMceComStreamBundle* CMceComStreamBundle::NewLC( CMceStreamBundle::TMceStreamBundleType aType )
    {
    CMceComStreamBundle* self = new (ELeave) CMceComStreamBundle( aType );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
     
// -----------------------------------------------------------------------------
// CMceComStreamBundle::~CMceComStreamBundle
// -----------------------------------------------------------------------------
//
CMceComStreamBundle::~CMceComStreamBundle()
    {
    iStreams.Close();
    }


// -----------------------------------------------------------------------------
// CMceComStreamBundle::Type
// -----------------------------------------------------------------------------
//
CMceStreamBundle::TMceStreamBundleType CMceComStreamBundle::Type() const
    {
    return iBundleType;
    }
    
// -----------------------------------------------------------------------------
// CMceComStreamBundle::AddStreamL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::AddStreamL( CMceComMediaStream& aStream )
    {
    iStreams.AppendL( &aStream );
    }
        
// -----------------------------------------------------------------------------
// CMceComStreamBundle::RemoveStreamL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::RemoveStreamL( CMceComMediaStream& aStream )
    {
    TInt index = iStreams.Find( &aStream );
    User::LeaveIfError( index );
    iStreams.Remove( index );
    }
        
// -----------------------------------------------------------------------------
// CMceComStreamBundle::Streams
// -----------------------------------------------------------------------------
//
const RPointerArray< CMceComMediaStream >& CMceComStreamBundle::Streams() const
    {
    return iStreams;
    }
        

// -----------------------------------------------------------------------------
// CMceComStreamBundle::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::InternalizeFlatL( RReadStream& aReadStream )
    {
    iBundleType = static_cast<CMceStreamBundle::TMceStreamBundleType>( aReadStream.ReadUint8L() );
    //iID
    MceSerial::DecodeL( iID, aReadStream );
    
    }

// -----------------------------------------------------------------------------
// CMceComStreamBundle::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
	aWriteStream.WriteUint8L( iBundleType );
    //iID
    MceSerial::EncodeL( iID, aWriteStream );
    }


// -----------------------------------------------------------------------------
// CMceComStreamBundle::CloneL
// -----------------------------------------------------------------------------
//
CMceComStreamBundle* CMceComStreamBundle::CloneL()
    {
    CMceComStreamBundle* copy = new (ELeave) CMceComStreamBundle( iBundleType );
    Mem::Copy( copy, this, sizeof(CMceComStreamBundle) );
    copy->Zero();
    return copy;
    
    }

// -----------------------------------------------------------------------------
// CMceComStreamBundle::Zero
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::Zero()
    {
    iStreams = RPointerArray<CMceComMediaStream>( KMceArrayGranularity );
    iSession = NULL;
    }
    
// -----------------------------------------------------------------------------
// CMceComStreamBundle::CloneL
// -----------------------------------------------------------------------------
//
CMceComStreamBundle* CMceComStreamBundle::CloneL( 
                            const RPointerArray<CMceComMediaStream>& aStreams )
    {
    CMceComStreamBundle* copy = static_cast<CMceComStreamBundle*>( CloneL() );
    CleanupStack::PushL( copy );
    
    for( TInt i = 0; i< aStreams.Count();i++ )
        {
        CMceComMediaStream* bundleStream = aStreams[ i ];
        CMceComMediaStream* stream = NULL;
        TInt index = 0;
        while( !stream && index < iStreams.Count() )
            {
            stream = iStreams[ index++ ];
            stream = stream->Id() == bundleStream->Id() ? bundleStream : NULL;
            }
        if ( stream )
            {
            copy->AddStreamL( *bundleStream );
            }
        }
    
    CleanupStack::Pop( copy );
    return copy;
    
    }


// -----------------------------------------------------------------------------
// CMceComStreamBundle::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::UpdateL( CMceComStreamBundle& /*aUpdate*/ )
    {
    //NOP
    }
     

// -----------------------------------------------------------------------------
// CMceComStreamBundle::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceComStreamBundle::SerializationId() const
    {
    return iID.Uint64();
    }
        
// -----------------------------------------------------------------------------
// CMceComStreamBundle::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrArgument ) );
    
    RReadStream& readStream = aSerCtx.ReadStream();

    CMceComStreamBundle::InternalizeFlatL( readStream );

    TUint32 i = 0;
    TInt j = 0;
    TMceMediaId streamId;

    TUint32 streamCount = readStream.ReadUint32L();
    for( i = 0; i<streamCount; i++)
        {
        MceSerial::DecodeL( streamId, readStream );
        for( j = 0; j < iSession->Streams().Count();j++ )
            {
            CMceComMediaStream* stream = iSession->Streams()[ j ];
            if ( stream->Id() == streamId )
                {
                AddStreamL( *stream );
                }
            }
        }

    }

// -----------------------------------------------------------------------------
// CMceComStreamBundle::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    RWriteStream& writeStream = aSerCtx.WriteStream();
    
    CMceComStreamBundle::ExternalizeFlatL( writeStream );
    
	writeStream.WriteUint32L( iStreams.Count() );
	for( TInt i = 0;i<iStreams.Count();i++)
	    {
        MceSerial::EncodeL( iStreams[ i ]->Id(), writeStream );
    	}

    }

// -----------------------------------------------------------------------------
// CMceComStreamBundle::InitializeL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::InitializeL( CMceComSession& aParent )
    {
    iSession = &aParent;
    }
    
// -----------------------------------------------------------------------------
// CMceComStreamBundle::CMceComStreamBundle
// -----------------------------------------------------------------------------
//
CMceComStreamBundle::CMceComStreamBundle( CMceStreamBundle::TMceStreamBundleType aType )
  : iBundleType( aType ),
    iID( TMceMediaId( (TUint32)&iID ) )
    {
    }

// -----------------------------------------------------------------------------
// CMceComStreamBundle::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMceComStreamBundle::AddedL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::AddedL()
    {
    MMceComEndPointProxy* proxy = NULL;
    CMceComEndpoint* endpoint = NULL;
    TInt i = 0;

    //sinks
    for( i = 0; i< iStreams.Count(); i++ )
        {
        
        CMceComMediaStream* stream = iStreams[ i ];
        AddedSinkL( *stream, endpoint, proxy );
        if ( stream->BoundStream() )
            {
            AddedSinkL( stream->BoundStreamL(), endpoint, proxy );
            }
        }

    //sources
    endpoint = NULL;
    proxy = NULL;
    for( i = 0;i<iStreams.Count();i++ )
        {
        CMceComMediaStream* stream = iStreams[ i ];
        AddedSourceL( *stream, endpoint, proxy );
        if ( stream->BoundStream() )
            {
            AddedSourceL( stream->BoundStreamL(), endpoint, proxy );
            }
        }
    }


// -----------------------------------------------------------------------------
// CMceComStreamBundle::AddedSinkL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::AddedSinkL( CMceComMediaStream& aStream,
                                      CMceComEndpoint*& aCurrentSink,
                                      MMceComEndPointProxy*& aProxy )

    {
    
    for( TInt j=0; j < aStream.Sinks().Count(); j++ )
        {
        aProxy = aCurrentSink ? aCurrentSink->EndpointProxy() : aProxy;
        aCurrentSink = aStream.Sinks()[ j ];
        aCurrentSink->SetupEndpointProxyL( aProxy );
        }
    }

// -----------------------------------------------------------------------------
// CMceComStreamBundle::AddedSourceL
// -----------------------------------------------------------------------------
//
void CMceComStreamBundle::AddedSourceL( CMceComMediaStream& aStream,
                                        CMceComEndpoint*& aCurrentSource,
                                        MMceComEndPointProxy*& aProxy )

    {
    aProxy = aCurrentSource ? aCurrentSource->EndpointProxy() : aProxy;
    aCurrentSource = aStream.Source();
    aCurrentSource->SetupEndpointProxyL( aProxy );
    
    }


