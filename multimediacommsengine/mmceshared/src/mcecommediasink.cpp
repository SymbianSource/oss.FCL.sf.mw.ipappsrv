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




#include "mcecommediasink.h"
#include "mceserial.h"
#include "mceevents.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceComMediaSink::NewL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComMediaSink::NewL()
    {
    CMceComMediaSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMediaSink::NewLC
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComMediaSink::NewLC()
    {
    CMceComMediaSink* self = new (ELeave) CMceComMediaSink( KMceEmptySink );
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMediaSink::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComMediaSink::CloneL()
    {
    CMceComMediaSink* copy = new (ELeave) CMceComMediaSink( iType );
    Mem::Copy( copy, this, sizeof(CMceComMediaSink) );
    copy->Zero();
    return copy;
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaSink::~CMceComMediaSink
// -----------------------------------------------------------------------------
//
CMceComMediaSink::~CMceComMediaSink()
    {
    }
    

// -----------------------------------------------------------------------------
// CMceComMediaSink::CMceComMediaSink
// -----------------------------------------------------------------------------
//
CMceComMediaSink::CMceComMediaSink( TMceSinkType aType )
  : CMceComEndpoint( KMceCategorySink, aType )
    {
    }

// -----------------------------------------------------------------------------
// CMceComMediaSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMediaSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComEndpoint::InternalizeFlatL( aReadStream );
    }
    
// -----------------------------------------------------------------------------
// CMceComMediaSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMediaSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComEndpoint::ExternalizeFlatL( aWriteStream );
    }
   
        
// -----------------------------------------------------------------------------
// CMceComMediaSink::Factory
// -----------------------------------------------------------------------------
//
TMceComSinkFactory CMceComMediaSink::Factory()
    {
    return TMceComSinkFactory();
    }



// -----------------------------------------------------------------------------
// CMceComMediaSink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComMediaSink::UpdateL( CMceComMediaSink& aUpdate )
    {
    BaseUpdateL( aUpdate );
    }

   
// -----------------------------------------------------------------------------
// CMceComMediaSink::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComMediaSink::EventReceivedL( TMceComEvent& aEvent )
    {
    TInt status = KMceEventNotOwner;
    TBool idMatch = aEvent.Id().iSinkID == iID;
    if ( !idMatch )
        {
        return KMceEventNotOwner;
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


#ifdef MCE_COMMON_SERVER_SIDE

    
// -----------------------------------------------------------------------------
// CMceComMediaSink::SetSSRC
// -----------------------------------------------------------------------------
//
void CMceComMediaSink::SetSSRC( TUint32 /*aSSRC*/ )
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceComMediaSink::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComMediaSink::MccType()
    {
    return KUidMccAnySink;
    }
    
#endif//MCE_COMMON_SERVER_SIDE
