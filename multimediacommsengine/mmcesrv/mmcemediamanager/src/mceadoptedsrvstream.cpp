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




#include "mceadoptedsrvstream.h"
#include "mceevents.h"
#include "mcecomcodec.h"
#include "mcesrvsink.h"
#include "mcesrvsource.h"
#include "mcecommediasink.h"
#include "mcecommediasource.h"
#include "mcemediamanager.h"



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::NewL
// -----------------------------------------------------------------------------
//
CMceAdoptedSrvStream* CMceAdoptedSrvStream::NewL( const CMceSrvStream& aStream )
    {
    __ASSERT_ALWAYS( !aStream.IsAdopted(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aStream.IsMccPrepared(), User::Leave( KErrArgument ) );
    
    CMceAdoptedSrvStream* self = new (ELeave) CMceAdoptedSrvStream( aStream );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::CMceAdoptedSrvStream
// -----------------------------------------------------------------------------
//
CMceAdoptedSrvStream::CMceAdoptedSrvStream( const CMceSrvStream& aStream )
    : CMceSrvStream( NULL, NULL, NULL )
    {
    
    iCodecId = aStream.Codec().Id();
    iSourceId = aStream.Source().Data().Id();
    iSinkId = aStream.Sink().Data().Id();
    iSessionId = aStream.SessionId();
    iLinkId = aStream.LinkId();
    iIapId = aStream.IapId();
    iLocalMediaPort = aStream.LocalMediaPort();
    
    iSourceEndpointId = aStream.Source().Id();
    iSinkEndpointId = aStream.Sink().Id();
    iManager = aStream.iManager;

    }
    
    
// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::~CMceAdoptedSrvStream
// -----------------------------------------------------------------------------
//
CMceAdoptedSrvStream::~CMceAdoptedSrvStream()
    {
    }

// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::IsAdopted
// -----------------------------------------------------------------------------
//
TBool CMceAdoptedSrvStream::IsAdopted() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::State
// -----------------------------------------------------------------------------
//
CMceSrvStream::TState CMceAdoptedSrvStream::State() const
    {
    
    CMceSrvStream::TState state = EAdopted;
    
    if ( iMerged )
        {
        state = EStopped;
        }

    return state;
            
    }

// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::IsMccPrepared
// -----------------------------------------------------------------------------
//
TBool CMceAdoptedSrvStream::IsMccPrepared() const
    {
    return ETrue;
    }


// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::SessionId
// -----------------------------------------------------------------------------
//
TUint32 CMceAdoptedSrvStream::SessionId() const
    {
    return iSessionId;
    }


// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::LinkId
// -----------------------------------------------------------------------------
//
TUint32 CMceAdoptedSrvStream::LinkId() const
    {
    return iLinkId;
    }
    
// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::SetLinkId
// -----------------------------------------------------------------------------
//
void CMceAdoptedSrvStream::SetLinkId( TUint32 aLinkId )
    {
    iLinkId = aLinkId;
    }
    
// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::IapId
// -----------------------------------------------------------------------------
//
TUint32 CMceAdoptedSrvStream::IapId() const
    {
    return iIapId;
    }
    
// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::LocalMediaPort
// -----------------------------------------------------------------------------
//
TUint CMceAdoptedSrvStream::LocalMediaPort() const
    {
    return iLocalMediaPort;
    }

// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::Cleanup
// -----------------------------------------------------------------------------
//
void CMceAdoptedSrvStream::Cleanup()
    {

    if ( iManager->CanReleaseEndpoint( *this, iSourceEndpointId ) )
        {
        iManager->ReleaseSource( *this, iSourceEndpointId );
        }
        
    if ( iManager->CanReleaseEndpoint( *this, iSinkEndpointId ) )
        {
        iManager->ReleaseSink( *this, iSinkEndpointId );
        }
        
    }



// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::IsEqual
// -----------------------------------------------------------------------------
//
TBool CMceAdoptedSrvStream::IsEqual( const CMceSrvStream& aStream ) const
    {
    TBool equal = EFalse;
    if ( &aStream )
        {
        if ( !aStream.IsAdopted() )
            {
            
            equal = iCodecId == aStream.Codec().Id() &&
                    iSourceId == aStream.Source().Data().Id() &&
                    iSinkId == aStream.Sink().Data().Id();
            }
        else
            {
            const CMceAdoptedSrvStream& stream = 
                reinterpret_cast<const CMceAdoptedSrvStream&>( aStream );
            equal = iCodecId == stream.iCodecId &&
                    iSourceId == stream.iSourceId &&
                    iSinkId == stream.iSinkId;
            }
        }
    
    return equal;

    }


// -----------------------------------------------------------------------------
// CMceAdoptedSrvStream::Invalidate
// -----------------------------------------------------------------------------
//
void CMceAdoptedSrvStream::Invalidate()
    {
    iMerged = ETrue;
    }



