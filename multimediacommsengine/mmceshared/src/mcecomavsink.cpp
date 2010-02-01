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




#include "mcecomavsink.h"
#include "mceavsink.h"
#include "mceevents.h"
#include "mcecomcodec.h"
#include "mcecommediastream.h"
#include "mcecomsession.h"
#include "mcedisplaysink.h"
#include "mcecomdisplaysink.h"
#include "mcespeakersink.h"
#include "mcecomspeakersink.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"

#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComAvSink::NewL
// -----------------------------------------------------------------------------
//
CMceComAvSink* CMceComAvSink::NewL()
    {
    CMceComAvSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComAvSink::NewLC
// -----------------------------------------------------------------------------
//
CMceComAvSink* CMceComAvSink::NewLC()
    {
    CMceComAvSink* self = new (ELeave) CMceComAvSink();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComAvSink::~CMceComAvSink
// -----------------------------------------------------------------------------
//
CMceComAvSink::~CMceComAvSink()
    {
    delete iSettings;
    iProxyClients.Close();
    
    }

// -----------------------------------------------------------------------------
// CMceComAvSink::ServesProxyClient
// -----------------------------------------------------------------------------
//
TBool CMceComAvSink::ServesProxyClient( const CMceComEndpoint& aClient ) const
    {
    TBool found = EFalse;
    TInt index = 0;
    while ( !found && index < iProxyClients.Count() )
        {
        found = iProxyClients[ index ++ ]->Id() == aClient.Id();
        }
    
    return found;
    
    }


// -----------------------------------------------------------------------------
// CMceComAvSink::AddProxyClientL
// -----------------------------------------------------------------------------
//
void CMceComAvSink::AddProxyClientL( CMceComEndpoint& aClient )
    {
    iProxyClients.AppendL( &aClient );
    ReferenceCount()++;
    }

// -----------------------------------------------------------------------------
// CMceComAvSink::RemoveProxyClient
// -----------------------------------------------------------------------------
//
void CMceComAvSink::RemoveProxyClient( CMceComEndpoint& aClient )
    {
    TInt index = iProxyClients.Find( &aClient );
    if ( index >= 0 )
        {
        iProxyClients.Remove( index );
        ReferenceCount()--;
        }
    }


// -----------------------------------------------------------------------------
// CMceComAvSink::CMceComAvSink
// -----------------------------------------------------------------------------
//
CMceComAvSink::CMceComAvSink()
  : CMceComMediaSink( KMceAvSink )
    {
    }

// -----------------------------------------------------------------------------
// CMceComAvSink::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComAvSink::CloneL()
    {
    CMceComAvSink* copy = new (ELeave) CMceComAvSink();
    Mem::Copy( copy, this, sizeof(CMceComAvSink) );
    copy->Zero();
    copy->iSettings = 0;
    return copy;
        
    }


// -----------------------------------------------------------------------------
// CMceComAvSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComAvSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSink::InternalizeFlatL( aReadStream );
    }



// -----------------------------------------------------------------------------
// CMceComAvSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComAvSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSink::ExternalizeFlatL( aWriteStream );
    }

  
    
// -----------------------------------------------------------------------------
// CMceComAvSink::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComAvSink::EventReceivedL( TMceComEvent& aEvent )
    {
    TInt status = CMceComMediaSink::EventReceivedL( aEvent );
    return status;
    
    }
    
    
// -----------------------------------------------------------------------------
// CMceComAvSink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComAvSink::UpdateL( CMceComMediaSink& aUpdate )
    {
    CMceComMediaSink::UpdateL( aUpdate );
    //CMceComAvSink& update = static_cast<CMceComAvSink&>( aUpdate );
    }
    

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComAvSink::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComAvSink::MccType()
    {
    return KUidMccVideoSink;
    }




// -----------------------------------------------------------------------------
// CMceComAvSink::InitParamL
// -----------------------------------------------------------------------------
//
const TDesC8& CMceComAvSink::InitParamL( const CMceComCodec& aCodec )
    {
    delete iSettings;
    iSettings = NULL;
    
    CMceComDisplaySink* displaySink = NULL;
    TInt index = 0;
    while( index < iProxyClients.Count() && !displaySink )
        {
        if ( iProxyClients[ index ]->iType == KMceDisplaySink )
            {
            displaySink = static_cast<CMceComDisplaySink*>( iProxyClients[ index ] );
            }
        index++;
        }
    
    if ( displaySink )
        {
        return displaySink->DoInitParamL( aCodec );
        }
    else
        {
        iSettings = KNullDesC8().AllocL();
        }
        
    return *iSettings;
    
    }

// -----------------------------------------------------------------------------
// CMceComAvSink::PrepareL
// -----------------------------------------------------------------------------
//   
void CMceComAvSink::PrepareL()
    {
    CMceComSpeakerSink* speakerSink = NULL;
    TInt index = 0;
    while( index < iProxyClients.Count() && !speakerSink )
        {
        if ( iProxyClients[ index ]->iType == KMceSpeakerSink )
            {
            speakerSink = static_cast<CMceComSpeakerSink*>( iProxyClients[ index ] );
            }
        index++;
        }
        
    if ( speakerSink )
        {
        __ASSERT_ALWAYS( speakerSink->MediaStream() && 
                         speakerSink->MediaStream()->Session(),
                         User::Leave( KErrNotFound ) );
        
        CMceMediaManager& manager = 
            speakerSink->MediaStream()->Session()->SdpSession().Manager();
                
        if ( speakerSink->iVolume != KMceNotAssignedInt )
            {
            // Set initial volume 
            iVolume = speakerSink->iVolume;
            manager.SetVolumeL( *speakerSink, iVolume );
            }
        else
            {
            // Get initial volume
            manager.GetVolumeL( *speakerSink, iVolume );
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CMceComAvSink::ClientEnableL
// -----------------------------------------------------------------------------
//
TBool CMceComAvSink::ClientEnableL( 
    CMceComEndpoint& aClient, 
    const CMceComCodec& /*aCodec*/ )
    {
    if ( aClient.iType != KMceSpeakerSink )
        {
        return KMceSrvStreamDefaultSequence;
        }
        
    __ASSERT_ALWAYS( ServesProxyClient( aClient ), User::Leave( KErrNotFound ) );
    
    __ASSERT_ALWAYS( aClient.MediaStream() && aClient.MediaStream()->Session(),
                     User::Leave( KErrNotFound ) );
    
    CMceMediaManager& manager = 
        aClient.MediaStream()->Session()->SdpSession().Manager();
    
    // No need to route command from mediamanager back to here
    TPckgBuf<TInt> volBuf( iVolume );
    manager.SetMccParameterL( 
            static_cast<const CMceComMediaSink&>( aClient ), 
            KMccSpeakerVolume, 
            volBuf );
    
    return KMceSrvStreamAlternativeSequence;
    }
    
// -----------------------------------------------------------------------------
// CMceComAvSink::ClientDisableL
// -----------------------------------------------------------------------------
//
TBool CMceComAvSink::ClientDisableL( CMceComEndpoint& aClient )
    {
    if ( aClient.iType != KMceSpeakerSink )
        {
        return KMceSrvStreamDefaultSequence;
        }
        
    __ASSERT_ALWAYS( ServesProxyClient( aClient ), User::Leave( KErrNotFound ) );
    
    __ASSERT_ALWAYS( aClient.MediaStream() && aClient.MediaStream()->Session(),
                     User::Leave( KErrNotFound ) );

    CMceMediaManager& manager = 
        aClient.MediaStream()->Session()->SdpSession().Manager();
    
    // Disable by setting volume to zero, remember the original volume
    //
    manager.GetVolumeL( static_cast<CMceComMediaSink&>( aClient ), iVolume );
    
    // No need to route command from mediamanager back to here
    TPckgBuf<TInt> volBuf( 0 );
    manager.SetMccParameterL( 
            static_cast<const CMceComMediaSink&>( aClient ), 
            KMccSpeakerVolume, 
            volBuf );
    
    return KMceSrvStreamAlternativeSequence;
    }

// -----------------------------------------------------------------------------
// CMceComAvSink::SetParameterL
// -----------------------------------------------------------------------------
//    
TBool CMceComAvSink::SetParameterL( 
    const CMceComEndpoint& aClient, 
    TUint32 aParam,
    const TDesC8& aValue )
    {
    // At the moment, only supported parameter is volume for speaker
    //
    if ( aClient.iType != KMceSpeakerSink || aParam != KMccSpeakerVolume )
        {
        return KMceSrvStreamDefaultSequence;
        }
    
    __ASSERT_ALWAYS( ServesProxyClient( aClient ), User::Leave( KErrNotFound ) );
    
    __ASSERT_ALWAYS( aClient.MediaStream() && aClient.MediaStream()->Session(),
                     User::Leave( KErrNotFound ) );
   
    CMceMediaManager& manager = 
        aClient.MediaStream()->Session()->SdpSession().Manager();
    
    TPckgBuf<TInt> volBuf;
    volBuf.Copy( aValue );
    
    iVolume = volBuf();
    
    if ( aClient.IsEnabled() )
        {
        manager.SetMccParameterL( 
            static_cast<const CMceComMediaSink&>( aClient ), aParam, aValue );
        }
        
    return KMceSrvStreamAlternativeSequence;
    }
    
#endif//MCE_COMMON_SERVER_SIDE

