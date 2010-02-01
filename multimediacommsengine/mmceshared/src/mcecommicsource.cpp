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




#include "mcemicsource.h"
#include "mcecommicsource.h"
#include "mceevents.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"

#endif//MCE_COMMON_SERVER_SIDE


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComMicSource::NewL
// -----------------------------------------------------------------------------
//
CMceComMicSource* CMceComMicSource::NewL()
    {
    CMceComMicSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMicSource::NewLC
// -----------------------------------------------------------------------------
//
CMceComMicSource* CMceComMicSource::NewLC()
    {
    CMceComMicSource* self = new (ELeave) CMceComMicSource();
    CleanupStack::PushL( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComMicSource::~CMceComMicSource
// -----------------------------------------------------------------------------
//
CMceComMicSource::~CMceComMicSource()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceComMicSource::CMceComMicSource
// -----------------------------------------------------------------------------
//
CMceComMicSource::CMceComMicSource()
  : CMceComMediaSource( KMceMicSource ),
    iGain( KMceNotAssignedInt )
    {
    }


// -----------------------------------------------------------------------------
// CMceComMicSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMicSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSource::InternalizeFlatL( aReadStream );
    
    iGain = aReadStream.ReadInt32L();
    }
    
// -----------------------------------------------------------------------------
// CMceComMicSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMicSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSource::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteInt32L( iGain );
    }
    
    
// -----------------------------------------------------------------------------
// CMceComMicSource::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComMicSource::CloneL()
    {
    CMceComMicSource* copy = new (ELeave) CMceComMicSource();
    Mem::Copy( copy, this, sizeof(CMceComMicSource) );
    copy->Zero();
    return copy;
        
    }
    
    
// -----------------------------------------------------------------------------
// CMceComMicSource::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComMicSource::EventReceivedL( TMceComEvent& aEvent )
    {
    TInt status = CMceComMediaSource::EventReceivedL( aEvent );
    if ( status != KMceEventNotConsumed )
        {
        return status;
        }
                
    if ( aEvent.Id().IsSourceId() )
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
// CMceComMicSource::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComMicSource::UpdateL( CMceComMediaSource& aUpdate )
    {
    CMceComMediaSource::UpdateL( aUpdate );
    
    iGain = static_cast<CMceComMicSource&>( aUpdate ).iGain;
    }
    
// -----------------------------------------------------------------------------
// CMceComMicSource::Reusable
// -----------------------------------------------------------------------------
//
TBool CMceComMicSource::Reusable( const CMceComMediaStream& /*aParent*/ ) const
    {
    return EFalse;
    }
    
#ifdef MCE_COMMON_SERVER_SIDE


// -----------------------------------------------------------------------------
// CMceComMicSource::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComMicSource::MccType()
    {
    return KUidMmfAudioInput;
    }

// -----------------------------------------------------------------------------
// CMceComMicSource::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComMicSource::PrepareL()
    {
    if ( MediaStream() && MediaStream()->Session() && iGain != KMceNotAssignedInt )
        {
        MediaStream()->Session()->SdpSession().Manager().SetGainL( *this, iGain );
        }
    }
    
#endif//MCE_COMMON_SERVER_SIDE


    

