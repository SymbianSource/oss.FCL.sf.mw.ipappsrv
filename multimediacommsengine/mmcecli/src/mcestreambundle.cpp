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




#include "mcestreambundle.h"
#include "mcesession.h"
#include "mcecomstreambundle.h"
#include "mcefactory.h"
#include "mceserial.h"
#include "mceendpointproxy.h"

#define _FLAT_DATA static_cast<CMceComStreamBundle*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceStreamBundle::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceStreamBundle* CMceStreamBundle::NewL( TMceStreamBundleType aType )
    {
    CMceStreamBundle* self = NewLC( aType );
    CleanupStack::Pop( self );
    return self;   
    
    }

// -----------------------------------------------------------------------------
// CMceStreamBundle::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceStreamBundle* CMceStreamBundle::NewLC( TMceStreamBundleType aType )
    {
    CMceStreamBundle* self = new (ELeave) CMceStreamBundle();
    CleanupStack::PushL( self );
    self->ConstructL( aType );
    return self;
    }
     
// -----------------------------------------------------------------------------
// CMceStreamBundle::~CMceStreamBundle
// -----------------------------------------------------------------------------
//
EXPORT_C CMceStreamBundle::~CMceStreamBundle()
    {
    delete iFlatData;
    iStreams.Close();
    iTempStreams.Close();
    }

// -----------------------------------------------------------------------------
// CMceStreamBundle::Type
// -----------------------------------------------------------------------------
//
EXPORT_C CMceStreamBundle::TMceStreamBundleType CMceStreamBundle::Type() const
    {
    return FLAT_DATA( Type() );
    }
    
// -----------------------------------------------------------------------------
// CMceStreamBundle::AddStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceStreamBundle::AddStreamL( CMceMediaStream& aStream )
    {
    iStreams.AppendL( &aStream );
    }
        
// -----------------------------------------------------------------------------
// CMceStreamBundle::RemoveStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceStreamBundle::RemoveStreamL( CMceMediaStream& aStream )
    {
    TInt index = iStreams.Find( &aStream );
    User::LeaveIfError( index );
    iStreams.Remove( index );
    }
        
// -----------------------------------------------------------------------------
// CMceStreamBundle::Streams
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray< CMceMediaStream >& CMceStreamBundle::Streams()
    {
    return iStreams;
    }
     
// -----------------------------------------------------------------------------
// CMceStreamBundle::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceStreamBundle::SerializationId() const
    {
    return FLAT_DATA( SerializationId() );
    }
     
// -----------------------------------------------------------------------------
// CMceStreamBundle::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::InternalizeFlatL( RReadStream& aReadStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->InternalizeFlatL( aReadStream );
    
    }

// -----------------------------------------------------------------------------
// CMceStreamBundle::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->ExternalizeFlatL( aWriteStream );
    }
     

// -----------------------------------------------------------------------------
// CMceStreamBundle::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrArgument ) );
    
    RReadStream& readStream = aSerCtx.ReadStream();

    InternalizeFlatL( readStream );

    TUint32 i = 0;
    TInt j = 0;
    TMceMediaId streamId;

    TUint32 streamCount = readStream.ReadUint32L();
    for( i = 0; i<streamCount; i++)
        {
        MceSerial::DecodeL( streamId, readStream );
        for( j = 0; j < iSession->Streams().Count();j++ )
            {
            CMceMediaStream* stream = iSession->Streams()[ j ];
            if ( stream->Id() == streamId )
                {
                AddStreamL( *stream );
                }
            }
        }

    }

// -----------------------------------------------------------------------------
// CMceStreamBundle::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    RWriteStream& writeStream = aSerCtx.WriteStream();
    
    ExternalizeFlatL( writeStream );
    
	writeStream.WriteUint32L( iStreams.Count() );
	for( TInt i = 0;i<iStreams.Count();i++)
	    {
        MceSerial::EncodeL( iStreams[ i ]->Id(), writeStream );
    	}

    }
        

// -----------------------------------------------------------------------------
// CMceStreamBundle::AddedL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::AddedL()
    {
    MMceEndPointProxy* proxy = NULL;
    CMceMediaSink* endpoint = NULL;
    TInt i = 0;

    //sinks
    for( i = 0; i< iStreams.Count(); i++ )
        {
        
        CMceMediaStream* stream = iStreams[ i ];
        AddedSinkL( *stream, endpoint, proxy );
        if ( stream->BoundStream() )
            {
            AddedSinkL( stream->BoundStreamL(), endpoint, proxy );
            }
        }

    }


// -----------------------------------------------------------------------------
// CMceStreamBundle::AddedSinkL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::AddedSinkL( CMceMediaStream& aStream,
                                   CMceMediaSink*& aCurrentSink,
                                   MMceEndPointProxy*& aProxy )

    {
    
    for( TInt j=0; j < aStream.Sinks().Count(); j++ )
        {
        aProxy = aCurrentSink ? aCurrentSink->EndpointProxy() : aProxy;
        aCurrentSink = aStream.Sinks()[ j ];
        aCurrentSink->SetupEndpointProxyL( aProxy );
        }
    }

 
 
// -----------------------------------------------------------------------------
// CMceStreamBundle::InitializeL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::InitializeL( CMceSession& aParent )
    {
    iSession = &aParent;
    }
    
// -----------------------------------------------------------------------------
// CMceStreamBundle::CMceStreamBundle
// -----------------------------------------------------------------------------
//
CMceStreamBundle::CMceStreamBundle()
    {
    }

// -----------------------------------------------------------------------------
// CMceStreamBundle::ConstructL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::ConstructL( CMceStreamBundle::TMceStreamBundleType aType )
    {
    
    iFlatData = CMceComStreamBundle::NewL( aType );
    
    }

