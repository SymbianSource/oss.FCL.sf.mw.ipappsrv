/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcecommessagesink.h"
#include "mcemessagesink.h"
#include "mcecommediastream.h"
#include "mcecomsession.h"
#include "mmccinterface.h"

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
CMceComMessageSink* CMceComMessageSink::NewL()
    {
    CMceComMessageSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMessageSink::NewLC
// -----------------------------------------------------------------------------
//
CMceComMessageSink* CMceComMessageSink::NewLC()
    {
    CMceComMessageSink* self = new (ELeave) CMceComMessageSink();
    CleanupStack::PushL( self );		
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMessageSink::~CMceComMessageSink
// -----------------------------------------------------------------------------
//
CMceComMessageSink::~CMceComMessageSink()
    {   
    }

// -----------------------------------------------------------------------------
// CMceComMessageSink::CMceComMessageSink
// -----------------------------------------------------------------------------
//
CMceComMessageSink::CMceComMessageSink()
  : CMceComMediaSink( KMceMessageSink )
    {
    }
	

// -----------------------------------------------------------------------------
// CMceComMessageSink::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComMessageSink::CloneL()
    {
    CMceComMessageSink* copy = new (ELeave) CMceComMessageSink();
    Mem::Copy( copy, this, sizeof(CMceComMediaSink) );
    copy->Zero();    
    return copy;
        
    }


// -----------------------------------------------------------------------------
// CMceComMessageSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMessageSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSink::InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceComMessageSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMessageSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSink::ExternalizeFlatL( aWriteStream );
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComMessageSink::MccType
// -----------------------------------------------------------------------------
//  
const TUid CMceComMessageSink::MccType()
    {
    return KUidMccAnySink;
    }


// -----------------------------------------------------------------------------
// CMceComMessageSink::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComMessageSink::PrepareL()
    {
    //NOP
    }
   
#endif//MCE_COMMON_SERVER_SIDE

