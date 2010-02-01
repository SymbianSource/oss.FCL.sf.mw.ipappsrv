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
#include "mcesession.h"
#include "mcemediastream.h"
#include "mceitcsender.h"
#include "mceavsink.h"
#include "mceclilogs.h"
#include "mcemanager.h"


#define _FLAT_DATA static_cast<CMceComSpeakerSink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceSpeakerSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSpeakerSink* CMceSpeakerSink::NewL()
    {
    CMceSpeakerSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;    
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSpeakerSink* CMceSpeakerSink::NewLC()
    {
    CMceSpeakerSink* self = new (ELeave) CMceSpeakerSink();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::~CMceSpeakerSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSpeakerSink::~CMceSpeakerSink()
    {
    if ( iEndpointProxy )
        {
        CMceAvSink* endpointProxy = static_cast<CMceAvSink*>( iEndpointProxy );
        endpointProxy->RemoveProxyClient( *this );
        if ( endpointProxy->ReferenceCount() == 0 )
            {
            delete endpointProxy;
            }
        iEndpointProxy = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSpeakerSink::EnableL() 
    {
    MCECLI_DEBUG("CMceSpeakerSink::EnableL, Entry");
    
    CMceMediaSink::DoEnableL();
    
    MCECLI_DEBUG("CMceSpeakerSink::EnableL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSpeakerSink::DisableL() 
    {
    MCECLI_DEBUG("CMceSpeakerSink::DisableL, Entry");
    
    CMceMediaSink::DoDisableL();
    
    MCECLI_DEBUG("CMceSpeakerSink::DisableL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::SetVolumeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSpeakerSink::SetVolumeL( TInt aVolume )
	{
    MCECLI_DEBUG("CMceSpeakerSink::SetVolumeL, Entry");
    MCECLI_DEBUG_DVALUE("volume", aVolume );

    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
    	CMceSession* session = iStream->Session();

    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID = iStream->Id();
    	ids.iSinkID  = Id();

    	TMceItcArgTInt volume( aVolume );
    	
        session->ITCSender().WriteL( ids, EMceItcSetVolume, volume );
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceSpeakerSink::SetVolumeL, done locally");
        }  
    
    FLAT_DATA( iVolume ) = aVolume;
    
    MCECLI_DEBUG("CMceSpeakerSink::SetVolumeL, Exit");
    
	}

// -----------------------------------------------------------------------------
// CMceSpeakerSink::VolumeL
// -----------------------------------------------------------------------------
//		        
EXPORT_C TInt CMceSpeakerSink::VolumeL() const
	{	
	MCECLI_DEBUG("CMceSpeakerSink::VolumeL, Entry");
	
	TInt volume( 0 );
	
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
    	CMceSession* session = iStream->Session();

    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID = iStream->Id();
    	ids.iSinkID  = Id();

    	TMceItcArgTInt volumeArg;

        session->ITCSender().ReadL( ids, EMceItcVolume, volumeArg );
        	
        volume = volumeArg();
        }
    else
        {
        volume = FLAT_DATA( iVolume );
        }

    MCECLI_DEBUG("CMceSpeakerSink::VolumeL, Exit");
    
	return volume;
	}

// -----------------------------------------------------------------------------
// CMceSpeakerSink::MaxVolumeL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceSpeakerSink::MaxVolumeL() const
	{	
	MCECLI_DEBUG("CMceSpeakerSink::MaxVolumeL");
	
	TMceItcArgTInt maxVolume;
    
    if ( iStream && iStream->Session() && &iStream->Session()->Manager() )
        {
        CMceManager& manager = iStream->Session()->Manager();
        TMceIds ids;
        ids.iAppUID = manager.AppUid().iUid;
        _FLAT_DATA->SenderL( 
            manager.ServerSession() ).ReadL( ids, EMceItcMaxVolume, maxVolume );
        }
    else
        {
        MCECLI_DEBUG("CMceSpeakerSink::MaxVolumeL, not ready");
        User::Leave( KErrNotReady );
        }
    
    // If volume is not set, set it to half of the max volume
    if ( FLAT_DATA( iVolume ) == KMceNotAssignedInt )
        {
        const TInt KMceVolumeDivider = 2;
        FLAT_DATA( iVolume ) = ( maxVolume() / KMceVolumeDivider );
        }
        
	return maxVolume();
	}

// -----------------------------------------------------------------------------
// CMceSpeakerSink::SetRoutingL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSpeakerSink::SetRoutingL( TInt aRoute )
    {
    MCECLI_DEBUG("CMceSpeakerSink::SetRoutingL, Entry");
    MCECLI_DEBUG_DVALUE("route", aRoute );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
    	CMceSession* session = iStream->Session();

    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID = iStream->Id();
    	ids.iSinkID  = Id();
    	
    	TMceItcArgTInt route( aRoute );

        session->ITCSender().WriteL( ids, EMceItcSetAudioRouting, route );
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceSpeakerSink::SetRoutingL, done locally");
        }  
        
    FLAT_DATA( iAudioRoute ) = aRoute;

    MCECLI_DEBUG("CMceSpeakerSink::SetRoutingL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::RoutingL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceSpeakerSink::RoutingL() const
    {
    MCECLI_DEBUG("CMceSpeakerSink::RoutingL, Entry");
    
    TInt route( 0 );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
    	CMceSession* session = iStream->Session();

    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID = iStream->Id();
    	ids.iSinkID  = Id();

    	TMceItcArgTInt routeArg;

        session->ITCSender().ReadL( ids, EMceItcAudioRouting, routeArg );
        
        route = routeArg();
        }
    else
        {
        route = FLAT_DATA( iAudioRoute );
        }

    MCECLI_DEBUG("CMceSpeakerSink::RoutingL, Exit");
    
	return route;
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::UsesEndpointProxy()
// -----------------------------------------------------------------------------
//
TBool CMceSpeakerSink::UsesEndpointProxy() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::CreateEndpointProxyL()
// -----------------------------------------------------------------------------
//
MMceEndPointProxy* CMceSpeakerSink::CreateEndpointProxyL() const
    {
    return CMceAvSink::NewL();
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::CMceSpeakerSink
// -----------------------------------------------------------------------------
//
CMceSpeakerSink::CMceSpeakerSink()
    {
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSpeakerSink::ConstructL()
    {
    CMceComSpeakerSink* sink = CMceComSpeakerSink::NewLC();
    CMceMediaSink::ConstructL( sink );
    CleanupStack::Pop( sink );
    }


