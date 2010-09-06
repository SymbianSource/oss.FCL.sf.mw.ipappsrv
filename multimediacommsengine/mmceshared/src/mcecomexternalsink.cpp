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




#include "mcecomexternalsink.h"
#include "mceexternalsink.h"
#include "mcecommediastream.h"
#include "mcecomsession.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccInterface.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"

#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComAvSink::NewL
// -----------------------------------------------------------------------------
//
CMceComExternalSink* CMceComExternalSink::NewL()
    {
    CMceComExternalSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComExternalSink::NewLC
// -----------------------------------------------------------------------------
//
CMceComExternalSink* CMceComExternalSink::NewLC()
    {
    CMceComExternalSink* self = new (ELeave) CMceComExternalSink();
    CleanupStack::PushL( self );		
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComExternalSink::~CMceComExternalSink
// -----------------------------------------------------------------------------
//
CMceComExternalSink::~CMceComExternalSink()
    {   
    }

// -----------------------------------------------------------------------------
// CMceComExternalSink::CMceComExternalSink
// -----------------------------------------------------------------------------
//
CMceComExternalSink::CMceComExternalSink()
  : CMceComMediaSink( KMceExternalSink )
    {
    }
	

// -----------------------------------------------------------------------------
// CMceComExternalSink::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComExternalSink::CloneL()
    {
    CMceComExternalSink* copy = new (ELeave) CMceComExternalSink();
    Mem::Copy( copy, this, sizeof(CMceComExternalSink) );
    copy->Zero();    
    return copy;
        
    }


// -----------------------------------------------------------------------------
// CMceComExternalSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComExternalSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSink::InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceComExternalSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComExternalSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSink::ExternalizeFlatL( aWriteStream );
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComExternalSink::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComExternalSink::MccType()
    {
    return KUidMccAnySink;
    }


// -----------------------------------------------------------------------------
// CMceComExternalSink::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComExternalSink::PrepareL()
    {
    //NOP  // Not required as the data path for message stream will be handled by applications
    }
   
#endif//MCE_COMMON_SERVER_SIDE

