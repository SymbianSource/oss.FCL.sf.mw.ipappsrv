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




#include "mcertpsource.h"
#include "mcecomrtpsource.h"
#include "mceserial.h"
#include "mceevents.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include "mcemediaobserver.h"
#include "mcesdpsession.h"
#include "mcemediamanager.h"

#endif//MCE_COMMON_SERVER_SIDE


const TUint KTimeConversion = 1000;

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComRtpSource::NewL
// -----------------------------------------------------------------------------
//
CMceComRtpSource* CMceComRtpSource::NewL()
    {
    CMceComRtpSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComRtpSource::NewLC
// -----------------------------------------------------------------------------
//
CMceComRtpSource* CMceComRtpSource::NewLC()
    {
    CMceComRtpSource* self = new (ELeave) CMceComRtpSource();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComRtpSource::ConstructL
// -----------------------------------------------------------------------------
//
void CMceComRtpSource::ConstructL( CMceComRtpSource& aSource )
    {
    Mem::Copy( this, &aSource, sizeof(CMceComMediaSource) );
    Zero();
    iSettings = NULL;
    
    iInactivityTimer = aSource.iInactivityTimer;
    iBufferLength = aSource.iBufferLength;
    iBufferTreshold = aSource.iBufferTreshold;
    iTalkBurstIndicator = aSource.iTalkBurstIndicator ? 
                            aSource.iTalkBurstIndicator->AllocL() : NULL;
                            
    iIdentity = aSource.iIdentity ? aSource.iIdentity->AllocL() : NULL;
    }


// -----------------------------------------------------------------------------
// CMceComRtpSource::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComRtpSource::CloneL()
    {
    CMceComRtpSource* copy = new (ELeave) CMceComRtpSource();
    CleanupStack::PushL( copy );
    copy->ConstructL( *this );
    CleanupStack::Pop( copy );    
    return copy;
        
    }

// -----------------------------------------------------------------------------
// CMceComRtpSource::~CMceComRtpSource
// -----------------------------------------------------------------------------
//
CMceComRtpSource::~CMceComRtpSource()
    {
    delete iTalkBurstIndicator;
    delete iIdentity;
    delete iSettings;
    
    }


// -----------------------------------------------------------------------------
// CMceComRtpSource::CMceComRtpSource
// -----------------------------------------------------------------------------
//
CMceComRtpSource::CMceComRtpSource()
  : CMceComMediaSource( KMceRTPSource ),
    iInactivityTimer( 0 ),
	iBufferLength( KMceJitterBufferLength ),
	iBufferTreshold( KMceJitterBufferTreshold ),
	iTalkBurstIndicator( NULL ),
	iIdentity( NULL )
    {
    }


// -----------------------------------------------------------------------------
// CMceComRtpSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComRtpSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSource::InternalizeFlatL( aReadStream );
    
    iInactivityTimer = aReadStream.ReadUint32L();
	iBufferLength = aReadStream.ReadUint32L();
	iBufferTreshold = aReadStream.ReadUint32L();
    MceSerial::DecodeL( iTalkBurstIndicator, aReadStream ); 
    MceSerial::DecodeL( iIdentity, aReadStream ); 
    }

// -----------------------------------------------------------------------------
// CMceComRtpSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComRtpSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSource::ExternalizeFlatL( aWriteStream );
    
	aWriteStream.WriteUint32L( iInactivityTimer );
	aWriteStream.WriteUint32L( iBufferLength );
	aWriteStream.WriteUint32L( iBufferTreshold );
    MceSerial::EncodeL( iTalkBurstIndicator, aWriteStream ); 
    MceSerial::EncodeL( iIdentity, aWriteStream ); 
    }

  
// -----------------------------------------------------------------------------
// CMceComRtpSource::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComRtpSource::UpdateL( CMceComMediaSource& aUpdate )
    {
    CMceComMediaSource::UpdateL( aUpdate );
    CMceComRtpSource& update = static_cast<CMceComRtpSource&>( aUpdate );
    
    iInactivityTimer = update.iInactivityTimer;
    iBufferLength = update.iBufferLength;
    iBufferTreshold = update.iBufferTreshold;

	delete iTalkBurstIndicator;
	iTalkBurstIndicator = NULL;
	iTalkBurstIndicator = 
	    update.iTalkBurstIndicator ? update.iTalkBurstIndicator->AllocL() : NULL;
    
    delete iIdentity;
    iIdentity = NULL;
    iIdentity = update.iIdentity ? update.iIdentity->AllocL() : NULL;
    }

    
    
// -----------------------------------------------------------------------------
// CMceComRtpSource::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComRtpSource::EventReceivedL( TMceComEvent& aEvent )
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
// CMceComRtpSource::InactivityTimer
// -----------------------------------------------------------------------------
//
TUint CMceComRtpSource::InactivityTimer() const
    {
    return iInactivityTimer * KTimeConversion;
    }

    
#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComRtpSource::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComRtpSource::MccType()
    {
    return KUidMccRtpSource;
    }


// -----------------------------------------------------------------------------
// CMceComRtpSource::InitParamL
// -----------------------------------------------------------------------------
//
const TDesC8& CMceComRtpSource::InitParamL( const CMceComCodec& /*aCodec*/ )
    {
    delete iSettings;
    iSettings = NULL;
    TMccRtpSourceSetting setting;
    setting.iStandByTimerValue = iInactivityTimer;
    TMccRtpSourceSettingBuf settingBuf( setting );
    
    iSettings = settingBuf.AllocL();

    return *iSettings;
    }

// -----------------------------------------------------------------------------
// CMceComRtpSource::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComRtpSource::PrepareL()
    {
    if ( MediaStream() && MediaStream()->Session() && 
         iIdentity &&
         iIdentity->Length() > 0 )
        {
        MediaStream()->Session()->SdpSession().Manager().SetCNameL( *this, *iIdentity );
        }
    }
    
#endif//MCE_COMMON_SERVER_SIDE

