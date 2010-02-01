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




#include "mcecomfilesink.h"
#include "mcefilesink.h"
#include "mceevents.h"
#include "mceserial.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include <utf.h>

#endif//MCE_COMMON_SERVER_SIDE


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComFileSink::NewL
// -----------------------------------------------------------------------------
//
CMceComFileSink* CMceComFileSink::NewL()
    {
    CMceComFileSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComFileSink::NewLC
// -----------------------------------------------------------------------------
//
CMceComFileSink* CMceComFileSink::NewLC()
    {
    CMceComFileSink* self = new (ELeave) CMceComFileSink();
    CleanupStack::PushL( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComFileSink::~CMceComFileSink
// -----------------------------------------------------------------------------
//
CMceComFileSink::~CMceComFileSink()
    {
    delete iSettings;
    }
    
    
// -----------------------------------------------------------------------------
// CMceComFileSink::CMceComFileSink
// -----------------------------------------------------------------------------
//
CMceComFileSink::CMceComFileSink()
    :CMceComMediaSink( KMceFileSink ),
     iInitialInfoRetrieved( EFalse )
    {
    }


// -----------------------------------------------------------------------------
// CMceComFileSink::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComFileSink::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSink::InternalizeFlatL( aReadStream );

    TPckgBuf<TMceFileInfo> fileBuf;
    MceSerial::DecodeL( fileBuf, aReadStream );
    iFileInfo = fileBuf();
    
    iInitialInfoRetrieved = static_cast<TBool>( aReadStream.ReadUint8L() );
    
    }



// -----------------------------------------------------------------------------
// CMceComFileSink::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComFileSink::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSink::ExternalizeFlatL( aWriteStream );
    
    TPckgBuf<TMceFileInfo> fileBuf( iFileInfo );
    MceSerial::EncodeL( fileBuf, aWriteStream );
    
    aWriteStream.WriteUint8L( iInitialInfoRetrieved );
    
    
    }

    
    
// -----------------------------------------------------------------------------
// CMceComFileSink::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSink* CMceComFileSink::CloneL()
    {
    CMceComFileSink* copy = new (ELeave) CMceComFileSink();
    Mem::Copy( copy, this, sizeof(CMceComFileSink) );
    copy->Zero();
    copy->iSettings = NULL;
    return copy;
        
    }
    

// -----------------------------------------------------------------------------
// CMceComFileSink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComFileSink::UpdateL( CMceComMediaSink& aUpdate )
    {
    CMceComMediaSink::UpdateL( aUpdate );
    CMceComFileSink& update = static_cast<CMceComFileSink&>( aUpdate );

    iFileInfo = update.iFileInfo;
    
    }
    
    
#ifdef MCE_COMMON_SERVER_SIDE


// -----------------------------------------------------------------------------
// CMceComFileSink::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComFileSink::MccType()
    {
    return KUidMccFileSink;
    }

// -----------------------------------------------------------------------------
// CMceComFileSink::InitParamL
// -----------------------------------------------------------------------------
//
const TDesC8& CMceComFileSink::InitParamL( const CMceComCodec& /*aCodec*/ )
    {
    delete iSettings;
    iSettings = NULL;

    TPckgBuf<TFileName> settings( iFileInfo.iFileName );
    iSettings = settings.AllocL();
    
    return *iSettings;
    }

#endif//MCE_COMMON_SERVER_SIDE


    

