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




#include "mcestreamobserver.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediasink.h"
#include "mcecommediasink.h"
#include "mceitcsender.h"
#include "mcefactory.h"
#include "mceevents.h"
#include "mceendpointproxy.h"
#include "mceclilogs.h"

#define _FLAT_DATA static_cast<CMceComMediaSink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMediaSink::~CMceMediaSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSink::~CMceMediaSink()
    {
    delete iFlatData;
    iParents.Reset();
    iParents.Close();
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSink::Type
// -----------------------------------------------------------------------------
//
EXPORT_C TMceSinkType CMceMediaSink::Type() const
    {
    return FLAT_DATA( Type() );
    }
    


// -----------------------------------------------------------------------------
// CMceMediaSink::IsEnabled
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceMediaSink::IsEnabled() const
    {
    return FLAT_DATA( IsEnabled() );
    }

    
// -----------------------------------------------------------------------------
// CMceMediaSink::Id
// -----------------------------------------------------------------------------
//
TMceMediaId CMceMediaSink::Id() const
    {
    return FLAT_DATA( iID ); 
    }

    
// -----------------------------------------------------------------------------
// CMceMediaSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::ConstructL( CMceComMediaSink* aFlatData ) 
    {
    __ASSERT_ALWAYS( aFlatData , User::Leave( KErrArgument ) );
    iFlatData = aFlatData;
    FLAT_DATA( iID ) = TMceMediaId();//as not assigned
    
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSink::CMceMediaSink
// -----------------------------------------------------------------------------
//
CMceMediaSink::CMceMediaSink()
    {
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::CreateEndpointProxyL
// -----------------------------------------------------------------------------
//
MMceEndPointProxy* CMceMediaSink::EndpointProxy() const
    {
    return iEndpointProxy;
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::CreateEndpointProxyL
// -----------------------------------------------------------------------------
//
MMceEndPointProxy* CMceMediaSink::CreateEndpointProxyL() const
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::UsesEndpointProxy
// -----------------------------------------------------------------------------
//
TBool CMceMediaSink::UsesEndpointProxy() const
    {
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSink::SetupEndpointProxyL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::SetupEndpointProxyL( MMceEndPointProxy* aEndpointProxy )
    {
    if ( !UsesEndpointProxy() )
        {
        return;
        }
        
    if ( !aEndpointProxy && !iEndpointProxy )
        {
        iEndpointProxy = CreateEndpointProxyL();
        iEndpointProxy->AddProxyClientL( *this );
        }
    else if ( aEndpointProxy && !iEndpointProxy )
        {
        aEndpointProxy->AddProxyClientL( *this );
        iEndpointProxy = aEndpointProxy;
        }
    else
        {
        iEndpointProxy->AddProxyClientL( *this );
        }
        
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceMediaSink::SerializationId() const
    {
    return FLAT_DATA( SerializationId() );
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->ExternalizeFlatL( aWriteStream );
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    InternalizeFlatL( aSerCtx.ReadStream() );
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    ExternalizeFlatL( aSerCtx.WriteStream() );
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSink::Factory
// -----------------------------------------------------------------------------
//
TMceSinkFactory CMceMediaSink::Factory()
    {
    return TMceSinkFactory();
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSink::DoITCSendL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::DoITCSendL(TUint8 aITCFunction, TBool aEnabled)
    {    
	__ASSERT_ALWAYS( iStream, User::Leave( KErrNotReady ) );
    CMceSession* session = iStream->Session();
	__ASSERT_ALWAYS( session, User::Leave( KErrNotReady ) );

	TMceIds ids;
	session->PrepareForITC( ids );
	ids.iMediaID   = iStream->Id();
	ids.iSinkID    = Id();
	ids.iState     = aEnabled;
	
    session->ITCSender().SendL( ids, static_cast<TMceItcFunctions>( aITCFunction ) );
    
	Enabled( static_cast<TBool>( ids.iState ) );
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::DoEnableL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::DoEnableL() 
    {
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
    	DoITCSendL( EMceItcEnable, ETrue );    
        }
    else
        {
        MCECLI_DEBUG("CMceMediaSink::DoEnableL, done locally");
    	Enabled( ETrue );
        }    
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::DoDisableL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::DoDisableL() 
    {
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        __ASSERT_ALWAYS( iStream->State() != CMceMediaStream::ETranscodingRequired, 
                         User::Leave( KErrNotReady ) );
    	DoITCSendL( EMceItcDisable, EFalse );    
        }
    else
        {
    	Enabled( EFalse );
        MCECLI_DEBUG("CMceMediaSink::DoDisableL, done locally");
        }    
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::Enabled
// -----------------------------------------------------------------------------
//
void CMceMediaSink::Enabled( TBool aValue )
    {
	_FLAT_DATA->Enabled( aValue );
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::InitializeL( CMceManager* /*aManager*/ )
    {
    // NOP
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSink::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::InitializeL( CMceMediaStream& aParent )
    {
    
    if ( !Id().IsAssigned() )
        {
	    __ASSERT_ALWAYS( aParent.Session(), User::Leave( KErrNotFound ) );
        FLAT_DATA( iID ) = aParent.Session()->Manager().NextMediaId();
        MCECLI_DEBUG_DVALUES2("CMceMediaSink::InitializeL, this: type", Type(),
                              "id", Id().iId, "enabled", IsEnabled() );
        }    
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::StreamAddedL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::StreamAddedL( CMceMediaStream& aParent )
    {
    
    if ( iParents.Find( &aParent ) == KErrNotFound )
        {
        iParents.AppendL( &aParent );
        }
        
    iStream = iParents[0];
    
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::UnInitialize
// -----------------------------------------------------------------------------
//
void CMceMediaSink::UnInitialize( CMceMediaStream& aParent )
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
// CMceMediaSink::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceMediaSink::EventReceivedL( TMceEvent& aEvent )
    {
    TInt status = KMceEventNotOwner;
    TBool idMatch = aEvent.Id().iSinkID == Id();
    if ( !idMatch )
        {
        return KMceEventNotOwner;
        }
        
    if ( aEvent.Id().IsSinkId() )
        {
        status = HandleEvent( aEvent );
        }
    else
        {
        status = KMceEventNotConsumed;
        }
    
    return status;
    
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::UpdateL( CMceMediaSink& aUpdate )
    {
    MCECLI_DEBUG("CMceMediaSink::UpdateL, Entry");
    MCECLI_DEBUG_DVALUES("this: type", Type(), "id", Id().iId );
    
    iFlatData->UpdateL( *aUpdate.iFlatData );
    
    MCECLI_DEBUG_DVALUE("CMceMediaSink::UpdateL, before exit. enabled", IsEnabled() );
    MCECLI_DEBUG("CMceMediaSink::UpdateL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::ReferenceCount
// -----------------------------------------------------------------------------
//
TInt& CMceMediaSink::ReferenceCount()
    {
    return FLAT_DATA( ReferenceCount() );
    }

// -----------------------------------------------------------------------------
// CMceMediaSink::Updated
// -----------------------------------------------------------------------------
//
void CMceMediaSink::Updated()
    {
    MMceStreamObserver* observer = iStream->Session()->Manager().MediaObserver();
    
    if ( observer )
        {
        observer->StreamStateChanged( *iStream, *this );
        }
    
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::HandleEvent
// -----------------------------------------------------------------------------
//
TInt CMceMediaSink::HandleEvent( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceMediaSink::HandleEvent, Entry");
    MCECLI_DEBUG_DVALUES("this: type", Type(), "id", Id().iId );
    
    TInt status = KMceEventNotConsumed;

    if ( aEvent.MessageType() == EMceItcMsgTypeSink )
        {
        MCECLI_DEBUG("CMceMediaSink::HandleEvent, content of sink changed");
        status = KMceEventUpdate;
        }
    else if ( aEvent.Action() == EMceItcStateChanged ) 
        {
        MCECLI_DEBUG("CMceMediaSink::HandleEvent, state changed");
    	Enabled( static_cast<TBool>( aEvent.ActionData() ) );
        if ( IsEnabled() )
            {
            iStream->SetState( CMceMediaStream::EStreaming );
            }
        else
            {
            iStream->SetState( CMceMediaStream::EDisabled );
            }
        Updated();
        status = KMceEventConsumed;
        }
    else if ( aEvent.Action() == EMceItcInProgress ) 
        {
        MCECLI_DEBUG("CMceMediaSink::HandleEvent, stream state changed");
        
        TInt stateContent( 0 );
        TBool isEnabled( EFalse );
        TUint32 actionData = aEvent.ActionData();
        MCE_DECODE_DOUBLE_STATE( actionData, stateContent, isEnabled );     
        CMceMediaStream::TState state = static_cast<CMceMediaStream::TState>( stateContent );
        
        MCECLI_DEBUG_DVALUE("CMceMediaSink::HandleEvent, state:", state );
        MCECLI_DEBUG_DVALUE("CMceMediaSink::HandleEvent, enabled:", isEnabled );
        
        Enabled( isEnabled );
        
        TBool stateChanged = iStream->SetState( state );
        
        if ( !iStream->HandleMediaError( state, isEnabled, aEvent.Id().iStatus ) && 
             stateChanged )
            {
            Updated();
            }
            
        status = KMceEventConsumed;
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceMediaSink::HandleEvent, not consumed");
        }
        
    MCECLI_DEBUG_DVALUE("CMceMediaSink::HandleEvent, before exit. enabled", IsEnabled() );
    MCECLI_DEBUG_DVALUE("CMceMediaSink::HandleEvent, Exit. status", status );
    
    return status;
    
    }
