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




#include "mceavsink.h"
#include "mcemanager.h"
#include "mcecomavsink.h"
#include "mce.h"
#include "mcemediastream.h"
#include "mcefactory.h"
#include "mceevents.h"
#include "mceclilogs.h"

#include <w32std.h>


#define _FLAT_DATA static_cast<CMceComAvSink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMceAvSink* CMceAvSink::NewL()
    {
    CMceAvSink* self = CMceAvSink::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMceAvSink* CMceAvSink::NewLC()
    {
    CMceAvSink* self = new (ELeave) CMceAvSink();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMceAvSink::~CMceAvSink()
    {
    delete iFlatData;
    iParents.Close();
    iProxyClients.Close();
    }

// -----------------------------------------------------------------------------
// CMceAvSink::Type
// -----------------------------------------------------------------------------
//
TMceSinkType CMceAvSink::Type() const
    {
    return FLAT_DATA( Type() );
    }
    


// -----------------------------------------------------------------------------
// CMceAvSink::IsEnabled
// -----------------------------------------------------------------------------
//
TBool CMceAvSink::IsEnabled() const
    {
    return FLAT_DATA( IsEnabled() );
    }

    
// -----------------------------------------------------------------------------
// CMceAvSink::Id
// -----------------------------------------------------------------------------
//
TMceMediaId CMceAvSink::Id() const
    {
    return FLAT_DATA( iID ); 
    }

    
// -----------------------------------------------------------------------------
// CMceAvSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAvSink::ConstructL() 
    {
    iFlatData = CMceComAvSink::NewL();
    FLAT_DATA( iID ) = TMceMediaId();//as not assigned
    
    }
    

// -----------------------------------------------------------------------------
// CMceAvSink::CMceAvSink
// -----------------------------------------------------------------------------
//
CMceAvSink::CMceAvSink()
    {
    }

// -----------------------------------------------------------------------------
// CMceAvSink::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceAvSink::SerializationId() const
    {
    return FLAT_DATA( SerializationId() );
    }

// -----------------------------------------------------------------------------
// CMceAvSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceAvSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceAvSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceAvSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->ExternalizeFlatL( aWriteStream );
    }

// -----------------------------------------------------------------------------
// CMceAvSink::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceAvSink::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    InternalizeFlatL( aSerCtx.ReadStream() );
    }

// -----------------------------------------------------------------------------
// CMceAvSink::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceAvSink::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    ExternalizeFlatL( aSerCtx.WriteStream() );
    }
    

// -----------------------------------------------------------------------------
// CMceAvSink::Factory
// -----------------------------------------------------------------------------
//
TMceSinkFactory CMceAvSink::Factory()
    {
    return TMceSinkFactory();
    }


// -----------------------------------------------------------------------------
// CMceAvSink::ServesProxyClient
// -----------------------------------------------------------------------------
//
TBool CMceAvSink::ServesProxyClient( const CMceMediaSink& aClient ) const
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
// CMceAvSink::AddProxyClientL
// -----------------------------------------------------------------------------
//
void CMceAvSink::AddProxyClientL( CMceMediaSink& aClient )
    {
    iProxyClients.AppendL( &aClient );
    ReferenceCount()++;
    }

// -----------------------------------------------------------------------------
// CMceAvSink::RemoveProxyClient
// -----------------------------------------------------------------------------
//
void CMceAvSink::RemoveProxyClient( CMceMediaSink& aClient )
    {
    TInt index = iProxyClients.Find( &aClient );
    if ( index >= 0 )
        {
        iProxyClients.Remove( index );
        ReferenceCount()--;
        }
    }

// -----------------------------------------------------------------------------
// CMceAvSink::EnableL
// -----------------------------------------------------------------------------
//
void CMceAvSink::EnableL()
    {
	_FLAT_DATA->Enabled( ETrue );
    }


// -----------------------------------------------------------------------------
// CMceAvSink::DisableL
// -----------------------------------------------------------------------------
//
void CMceAvSink::DisableL()
    {
	_FLAT_DATA->Enabled( EFalse );
    }



// -----------------------------------------------------------------------------
// CMceAvSink::InitializeL
// -----------------------------------------------------------------------------
//
void CMceAvSink::InitializeL( CMceManager* /*aManager*/ )
    {
    // NOP
    }
    

// -----------------------------------------------------------------------------
// CMceAvSink::InitializeL
// -----------------------------------------------------------------------------
//
void CMceAvSink::InitializeL( CMceMediaStream& aParent )
    {
    StreamAddedL( aParent );
    iStream = iParents[0];
    
    if ( !Id().IsAssigned() )
        {
	    __ASSERT_ALWAYS( iStream->Session(), User::Leave( KErrNotFound ) );
        FLAT_DATA( iID ) = iStream->Session()->Manager().NextMediaId();
        MCECLI_DEBUG("CMceAvSink::InitializeL, initialized:" );
        MCECLI_DEBUG_DVALUE("id", Id().iId );
        
        }    
    }


// -----------------------------------------------------------------------------
// CMceAvSink::StreamAddedL
// -----------------------------------------------------------------------------
//
void CMceAvSink::StreamAddedL( CMceMediaStream& aParent )
    {
    
    if ( iParents.Find( &aParent ) == KErrNotFound )
        {
        iParents.AppendL( &aParent );
        }
    
    }

// -----------------------------------------------------------------------------
// CMceAvSink::UnInitialize
// -----------------------------------------------------------------------------
//
void CMceAvSink::UnInitialize( CMceMediaStream& aParent )
    {
    TInt index = iParents.Find( &aParent );
    if ( index >= 0 )
        {
        iParents.Remove( index );
        }
    iStream = NULL;
    
    if ( iParents.Count() > 0 )
        {
        iStream = iParents[0];
        }
    
    }
    
// -----------------------------------------------------------------------------
// CMceAvSink::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceAvSink::EventReceivedL( TMceEvent& /*aEvent*/ )
    {
    TInt status = KMceEventNotOwner;
    return status;
    
    }

// -----------------------------------------------------------------------------
// CMceAvSink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceAvSink::UpdateL( CMceAvSink& aUpdate )
    {
    _FLAT_DATA->UpdateL( *aUpdate.iFlatData );
    }

// -----------------------------------------------------------------------------
// CMceAvSink::ReferenceCount
// -----------------------------------------------------------------------------
//
TInt& CMceAvSink::ReferenceCount()
    {
    return FLAT_DATA( ReferenceCount() );
    }

// -----------------------------------------------------------------------------
// CMceAvSink::Updated
// -----------------------------------------------------------------------------
//
void CMceAvSink::Updated()
    {
    //NOP
    }


// -----------------------------------------------------------------------------
// CMceAvSink::HandleEvent
// -----------------------------------------------------------------------------
//
TInt CMceAvSink::HandleEvent( TMceEvent& /*aEvent*/ )
    {
    TInt status = KMceEventNotConsumed;
    return status;
    
    }





