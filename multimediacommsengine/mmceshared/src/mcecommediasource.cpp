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




#include "mcecommediasource.h"
#include "mceserial.h"
#include "mceevents.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComMediaSource::NewL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComMediaSource::NewL()
    {
    CMceComMediaSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMediaSource::NewLC
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComMediaSource::NewLC()
    {
    CMceComMediaSource* self = new (ELeave) CMceComMediaSource( KMceEmptySource );
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMediaSource::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComMediaSource::CloneL()
    {
    CMceComMediaSource* copy = new (ELeave) CMceComMediaSource( iType );
    Mem::Copy( copy, this, sizeof(CMceComMediaSource) );
    copy->Zero();
    return copy;
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaSource::~CMceComMediaSource
// -----------------------------------------------------------------------------
//
CMceComMediaSource::~CMceComMediaSource()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComMediaSource::CMceComMediaSource
// -----------------------------------------------------------------------------
//
CMceComMediaSource::CMceComMediaSource( TMceSourceType aType )
  : CMceComEndpoint( KMceCategorySource, aType )
    {
    
    }

// -----------------------------------------------------------------------------
// CMceComMediaSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMediaSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComEndpoint::InternalizeFlatL( aReadStream );
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMediaSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComEndpoint::ExternalizeFlatL( aWriteStream );
    }


// -----------------------------------------------------------------------------
// CMceComMediaSource::Factory
// -----------------------------------------------------------------------------
//
TMceComSourceFactory CMceComMediaSource::Factory()
    {
    return TMceComSourceFactory();
    }
   
// -----------------------------------------------------------------------------
// CMceComMediaSource::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComMediaSource::UpdateL( CMceComMediaSource& aUpdate )
    {
    BaseUpdateL( aUpdate );
    }
 
// -----------------------------------------------------------------------------
// CMceComMediaSource::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComMediaSource::EventReceivedL( TMceComEvent& aEvent )
    {
    TInt status = KMceEventNotOwner;
    TBool idMatch = aEvent.Id().iSourceID == iID;
    if ( !idMatch )
        {
        return KMceEventNotOwner;
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
   
    
#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComMediaSource::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComMediaSource::MccType()
    {
    return KUidMccAnySource;
    }
    
#endif//MCE_COMMON_SERVER_SIDE

// End of file

