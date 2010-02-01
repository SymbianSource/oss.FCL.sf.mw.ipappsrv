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




#include "mcefilesource.h"
#include "mcecomfilesource.h"
#include "mceevents.h"
#include "mceserial.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include <utf.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"

#endif//MCE_COMMON_SERVER_SIDE


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComFileSource::NewL
// -----------------------------------------------------------------------------
//
CMceComFileSource* CMceComFileSource::NewL()
    {
    CMceComFileSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComFileSource::NewLC
// -----------------------------------------------------------------------------
//
CMceComFileSource* CMceComFileSource::NewLC()
    {
    CMceComFileSource* self = new (ELeave) CMceComFileSource();
    CleanupStack::PushL( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComFileSource::~CMceComFileSource
// -----------------------------------------------------------------------------
//
CMceComFileSource::~CMceComFileSource()
    {
    delete iSettings;
    }
    
    
// -----------------------------------------------------------------------------
// CMceComFileSource::CMceComFileSource
// -----------------------------------------------------------------------------
//
CMceComFileSource::CMceComFileSource()
    :CMceComMediaSource( KMceFileSource ),
    iInitialInfoRetrieved( EFalse )
    {
    }


// -----------------------------------------------------------------------------
// CMceComFileSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComFileSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSource::InternalizeFlatL( aReadStream );
    
    iFastForward = aReadStream.ReadUint8L();
    iFastRewind = aReadStream.ReadUint8L();
    
    TPckgBuf<TTimeIntervalMicroSeconds> positionBuf;
    MceSerial::DecodeL( positionBuf, aReadStream );
    iPosition = positionBuf();

    TPckgBuf<TMceFileInfo> fileBuf;
    MceSerial::DecodeL( fileBuf, aReadStream );
    iFileInfo = fileBuf();

    TPckgBuf<TMceTranscodeInfo> tcBuf;
    MceSerial::DecodeL( tcBuf, aReadStream );
    iTranscodeInfo = tcBuf();
    
    iInitialInfoRetrieved = static_cast<TBool>( aReadStream.ReadUint8L() );
    
    }



// -----------------------------------------------------------------------------
// CMceComFileSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComFileSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSource::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteUint8L( iFastForward );
    aWriteStream.WriteUint8L( iFastRewind );
    
    TPckgBuf<TTimeIntervalMicroSeconds> positionBuf( iPosition );
    MceSerial::EncodeL( positionBuf, aWriteStream );

    TPckgBuf<TMceFileInfo> fileBuf( iFileInfo );
    MceSerial::EncodeL( fileBuf, aWriteStream );

    TPckgBuf<TMceTranscodeInfo> tcBuf( iTranscodeInfo );
    MceSerial::EncodeL( tcBuf, aWriteStream );
    
    aWriteStream.WriteUint8L( iInitialInfoRetrieved );
    
    
    }

    
    
// -----------------------------------------------------------------------------
// CMceComFileSource::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComFileSource::CloneL()
    {
    CMceComFileSource* copy = new (ELeave) CMceComFileSource();
    Mem::Copy( copy, this, sizeof(CMceComFileSource) );
    copy->Zero();
    copy->iSettings = NULL;
    return copy;
        
    }
    
    
// -----------------------------------------------------------------------------
// CMceComFileSource::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComFileSource::EventReceivedL( TMceComEvent& aEvent )
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
// CMceComFileSource::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComFileSource::UpdateL( CMceComMediaSource& aUpdate )
    {
    CMceComMediaSource::UpdateL( aUpdate );
    CMceComFileSource& update = static_cast<CMceComFileSource&>( aUpdate );
    
    iFastForward = update.iFastForward;
    iFastRewind = update.iFastRewind;
    iPosition = update.iPosition;
    iFileInfo = update.iFileInfo;
    iTranscodeInfo = update.iTranscodeInfo;
    
    }
    
    
#ifdef MCE_COMMON_SERVER_SIDE


// -----------------------------------------------------------------------------
// CMceComFileSource::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComFileSource::MccType()
    {
    return KUidMccFileSource;
    }

// -----------------------------------------------------------------------------
// CMceComFileSource::InitParamL
// -----------------------------------------------------------------------------
//
const TDesC8& CMceComFileSource::InitParamL( const CMceComCodec& /*aCodec*/ )
    {
    delete iSettings;
    iSettings = NULL;

    TPckgBuf<TFileName> settings( iFileInfo.iFileName );
    iSettings = settings.AllocL();
    
    return *iSettings;
    
    }

// -----------------------------------------------------------------------------
// CMceComFileSource::InitializedL
// -----------------------------------------------------------------------------
//
void CMceComFileSource::InitializedL()
    {
    if ( MediaStream() && MediaStream()->Session() && 
         &MediaStream()->Session()->SdpSession() )
        {
        MediaStream()->Session()->SdpSession().Manager().UpdateConfigKeyL( *this );
        }
    }
    
#endif//MCE_COMMON_SERVER_SIDE


    

