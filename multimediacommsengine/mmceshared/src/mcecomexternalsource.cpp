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




#include "mcecomexternalsource.h"
#include "mceexternalsource.h"
#include "mcecommediastream.h"
#include "mcecomsession.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccInterface.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"

#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComExternalSink::NewL
// -----------------------------------------------------------------------------
//
CMceComExternalSource* CMceComExternalSource::NewL()
    {
    CMceComExternalSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComExternalSource::NewLC
// -----------------------------------------------------------------------------
//
CMceComExternalSource* CMceComExternalSource::NewLC()
    {
    CMceComExternalSource* self = new (ELeave) CMceComExternalSource();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComExternalSource::~CMceComExternalSource
// -----------------------------------------------------------------------------
//
CMceComExternalSource::~CMceComExternalSource()
    {   
    }

// -----------------------------------------------------------------------------
// CMceComExternalSource::CMceComExternalSource
// -----------------------------------------------------------------------------
//
CMceComExternalSource::CMceComExternalSource()
  : CMceComMediaSource( KMceExternalSource )
    {
    }

// -----------------------------------------------------------------------------
// CMceComExternalSource::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComExternalSource::CloneL()
    {
    CMceComExternalSource* copy = new (ELeave) CMceComExternalSource();
    Mem::Copy( copy, this, sizeof(CMceComExternalSource) );
    return copy;        
    }


// -----------------------------------------------------------------------------
// CMceComExternalSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComExternalSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSource::InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceComExternalSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComExternalSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSource::ExternalizeFlatL( aWriteStream );
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComExternalSource::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComExternalSource::MccType()
    {
    return KUidMccAnySource;
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSource::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComExternalSource::PrepareL()
    {
    //NOP  // Not required as the data path for message stream will be handled by applications
    }
   
#endif//MCE_COMMON_SERVER_SIDE

