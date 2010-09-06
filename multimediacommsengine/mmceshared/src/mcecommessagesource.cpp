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




#include "mcecommessagesource.h"
#include "mcemessagesource.h"
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
// CMceComMessageSink::NewL
// -----------------------------------------------------------------------------
//
CMceComMessageSource* CMceComMessageSource::NewL()
    {
    CMceComMessageSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMessageSource::NewLC
// -----------------------------------------------------------------------------
//
CMceComMessageSource* CMceComMessageSource::NewLC()
    {
    CMceComMessageSource* self = new (ELeave) CMceComMessageSource();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMessageSource::~CMceComMessageSource
// -----------------------------------------------------------------------------
//
CMceComMessageSource::~CMceComMessageSource()
    {   
    }

// -----------------------------------------------------------------------------
// CMceComMessageSource::CMceComMessageSource
// -----------------------------------------------------------------------------
//
CMceComMessageSource::CMceComMessageSource()
  : CMceComMediaSource( KMceMessageSource )
    {
    }

// -----------------------------------------------------------------------------
// CMceComMessageSource::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComMessageSource::CloneL()
    {
    CMceComMessageSource* copy = new (ELeave) CMceComMessageSource();
    Mem::Copy( copy, this, sizeof(CMceComMediaSource) );
    return copy;        
    }


// -----------------------------------------------------------------------------
// CMceComMessageSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMessageSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSource::InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceComMessageSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMessageSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSource::ExternalizeFlatL( aWriteStream );
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComMessageSource::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComMessageSource::MccType()
    {
    return KUidMccAnySource;
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSource::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComMessageSource::PrepareL()
    {
    //NOP  
    // Not required as the data path for message stream will be handled by applications
    }
   
#endif//MCE_COMMON_SERVER_SIDE

