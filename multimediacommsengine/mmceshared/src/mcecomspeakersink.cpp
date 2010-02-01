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




#include "mcespeakersink.h"
#include "mcecomspeakersink.h"
#include "mceevents.h"
#include "mcecomavsink.h"


#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"

#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComSpeakerSink::NewL
// -----------------------------------------------------------------------------
//
CMceComSpeakerSink* CMceComSpeakerSink::NewL()
    {
    CMceComSpeakerSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComSpeakerSink::NewLC
// -----------------------------------------------------------------------------
//
CMceComSpeakerSink* CMceComSpeakerSink::NewLC()
    {
    CMceComSpeakerSink* self = new (ELeave) CMceComSpeakerSink();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComSpeakerSink::~CMceComSpeakerSink
// -----------------------------------------------------------------------------
//
CMceComSpeakerSink::~CMceComSpeakerSink()
    {
    if ( iEndpointProxy )
        {
        CMceComAvSink* endpointProxy = static_cast<CMceComAvSink*>( iEndpointProxy );
        endpointProxy->RemoveProxyClient( *this );
        if ( endpointProxy->ReferenceCount() == 0 )
            {
            delete endpointProxy;
            }
        iEndpointProxy = NULL;
        }
    
    }


// -----------------------------------------------------------------------------
// CMceComSpeakerSink::CMceComSpeakerSink
// -----------------------------------------------------------------------------
//
CMceComSpeakerSink::CMceComSpeakerSink()
  : CMceComMediaSink( KMceSpeakerSink ),
    iAudioRoute( 0 ),
    iVolume( KMceNotAssignedInt )
    {
    }

// -----------------------------------------------------------------------------
// CMceComSpeakerSink::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComSpeakerSink::CloneL()
    {
    CMceComSpeakerSink* copy = new (ELeave) CMceComSpeakerSink();
    Mem::Copy( copy, this, sizeof(CMceComSpeakerSink) );
    copy->Zero();
    return copy;
        
    }


// -----------------------------------------------------------------------------
// CMceComSpeakerSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComSpeakerSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSink::InternalizeFlatL( aReadStream );
    
    iAudioRoute = aReadStream.ReadInt32L();
    iVolume = aReadStream.ReadInt32L();
    }



// -----------------------------------------------------------------------------
// CMceComSpeakerSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComSpeakerSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSink::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteInt32L( iAudioRoute );
    aWriteStream.WriteInt32L( iVolume );
    }

// -----------------------------------------------------------------------------
// CMceComSpeakerSink::UsesEndpointProxy()
// -----------------------------------------------------------------------------
//
TBool CMceComSpeakerSink::UsesEndpointProxy() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMceComSpeakerSink::CreateEndpointProxyL()
// -----------------------------------------------------------------------------
//
MMceComEndPointProxy* CMceComSpeakerSink::CreateEndpointProxyL() const
    {
    return CMceComAvSink::NewL();
    }


    
// -----------------------------------------------------------------------------
// CMceComSpeakerSink::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComSpeakerSink::EventReceivedL( TMceComEvent& aEvent )
    {
    TInt status = CMceComMediaSink::EventReceivedL( aEvent );
    if ( status != KMceEventNotConsumed )
        {
        return status;
        }
                
    if ( aEvent.Id().IsSinkId() )
        {
        status = aEvent.Handler().HandleL( *this, aEvent );
        }
    else
        {
        status = KMceEventNotConsumed;
        }
    
    return status;
    
    }
    
// -----------------------------------------------------------------------------
// CMceComSpeakerSink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComSpeakerSink::UpdateL( CMceComMediaSink& aUpdate )
    {
    CMceComMediaSink::UpdateL( aUpdate );
    
    iAudioRoute = static_cast<CMceComSpeakerSink&>( aUpdate ).iAudioRoute;
    iVolume = static_cast<CMceComSpeakerSink&>( aUpdate ).iVolume;
    
    }
    
// -----------------------------------------------------------------------------
// CMceComSpeakerSink::Reusable
// -----------------------------------------------------------------------------
//
TBool CMceComSpeakerSink::Reusable( const CMceComMediaStream& /*aParent*/ ) const
    {
    if ( EndpointProxy() )
        {
        // If speaker is in bundled stream, sharing is allowed as speaker is 
        // actually replaced with display
        return ETrue;
        }
    return EFalse;
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComSpeakerSink::InitParamL
// -----------------------------------------------------------------------------
//
const TDesC8& CMceComSpeakerSink::InitParamL( const CMceComCodec& aCodec )
    {
    if ( EndpointProxy() )
        {
        CMceComAvSink* endpointProxy = static_cast<CMceComAvSink*>( EndpointProxy() );
        return endpointProxy->InitParamL( aCodec );
        }
        
    return CMceComMediaSink::InitParamL( aCodec );
    }

// -----------------------------------------------------------------------------
// CMceComSpeakerSink::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComSpeakerSink::PrepareL()
    {
     if ( EndpointProxy() )
        {
        CMceComAvSink* endpointProxy = static_cast<CMceComAvSink*>( EndpointProxy() );
        endpointProxy->PrepareL();
        }
    else
        {   
        if ( MediaStream() && MediaStream()->Session() )
            {
            CMceMediaManager& mediaManager = MediaStream()->Session()->SdpSession().Manager();
            if ( iAudioRoute )
                {
                mediaManager.SetAudioRoutingL( *this, iAudioRoute );
                }
                
            if ( iVolume != KMceNotAssignedInt )
                {
                mediaManager.SetVolumeL( *this, iVolume );
                }
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CMceComSpeakerSink::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComSpeakerSink::MccType()
    {
    TUid id = KUidMmfAudioOutput;
    if ( EndpointProxy() )
        {
        CMceComAvSink* endpointProxy = static_cast<CMceComAvSink*>( EndpointProxy() );
        id = endpointProxy->MccType();
        }
    return id;
    }

#endif//MCE_COMMON_SERVER_SIDE


