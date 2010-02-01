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




#include "mcecamerasource.h"
#include "mcecomcamerasource.h"
#include "mceevents.h"
#include "mceserial.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccinterface.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"
#include "mcevideostream.h"
#include "mcecomvideocodec.h"

#endif//MCE_COMMON_SERVER_SIDE

const TInt KInitialCameraIndex = 0;
const TInt KInitialZoomFactor = 1;
const TInt KInitialDigitalZoomFactor = 1;
const TInt KInitialContrast = 0;
const TInt KInitialBrightness = 0;
const TInt KInitialExposure = CCamera::EExposureAuto;
const TInt KInitialWhiteBalance = CCamera::EWBAuto;

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceComCameraSource::NewL
// -----------------------------------------------------------------------------
//
CMceComCameraSource* CMceComCameraSource::NewL()
    {
    CMceComCameraSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComCameraSource::NewLC
// -----------------------------------------------------------------------------
//
CMceComCameraSource* CMceComCameraSource::NewLC()
    {
    CMceComCameraSource* self = new (ELeave) CMceComCameraSource();
    CleanupStack::PushL( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceComCameraSource::~CMceComCameraSource
// -----------------------------------------------------------------------------
//
CMceComCameraSource::~CMceComCameraSource()
    {
    delete iSettings;
    }
    
// -----------------------------------------------------------------------------
// CMceComCameraSource::CMceComCameraSource
// -----------------------------------------------------------------------------
//
CMceComCameraSource::CMceComCameraSource()
    :CMceComMediaSource( KMceCameraSource ),
     iCameraIndex( KInitialCameraIndex ),
     iZoomFactor( KInitialZoomFactor ),
     iDigitalZoomFactor( KInitialDigitalZoomFactor ),
     iContrast( KInitialContrast ),
     iBrightness( KInitialBrightness ),
     iExposure( KInitialExposure ),
     iWhiteBalance( KInitialWhiteBalance ),
     iCamerasAvailable( 0 ),
     iInitialInfoRetrieved( EFalse )
    {
    }


// -----------------------------------------------------------------------------
// CMceComCameraSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComCameraSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSource::InternalizeFlatL( aReadStream );
    
    iCameraIndex = aReadStream.ReadInt32L();
    iZoomFactor = aReadStream.ReadInt32L();
    iDigitalZoomFactor = aReadStream.ReadInt32L();
    iContrast = aReadStream.ReadInt32L();
    iBrightness = aReadStream.ReadInt32L();
    iExposure = aReadStream.ReadInt32L();
    iWhiteBalance = aReadStream.ReadInt32L();
    iCamerasAvailable = aReadStream.ReadInt32L();
    
    TPckgBuf<TCameraInfo> cameraBuf;
    MceSerial::DecodeL( cameraBuf, aReadStream );
    iCameraInfo = cameraBuf();
    
    iInitialInfoRetrieved = static_cast<TBool>( aReadStream.ReadUint8L() );
    
    }



// -----------------------------------------------------------------------------
// CMceComCameraSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComCameraSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSource::ExternalizeFlatL( aWriteStream );
    
    aWriteStream.WriteInt32L( iCameraIndex );
    aWriteStream.WriteInt32L( iZoomFactor );
    aWriteStream.WriteInt32L( iDigitalZoomFactor );
    aWriteStream.WriteInt32L( iContrast );
    aWriteStream.WriteInt32L( iBrightness );
    aWriteStream.WriteInt32L( iExposure );
    aWriteStream.WriteInt32L( iWhiteBalance );
    aWriteStream.WriteInt32L( iCamerasAvailable );
    
    TPckgBuf<TCameraInfo> cameraBuf( iCameraInfo );
    MceSerial::EncodeL( cameraBuf, aWriteStream );
    
    aWriteStream.WriteUint8L( iInitialInfoRetrieved );

    }

    
// -----------------------------------------------------------------------------
// CMceComCameraSource::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComCameraSource::CloneL()
    {
    CMceComCameraSource* copy = new (ELeave) CMceComCameraSource();
    Mem::Copy( copy, this, sizeof(CMceComCameraSource) );
    copy->Zero();
    copy->iSettings = 0;
    return copy;
        
    }
    
    
// -----------------------------------------------------------------------------
// CMceComCameraSource::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceComCameraSource::EventReceivedL( TMceComEvent& aEvent )
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
// CMceComCameraSource::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComCameraSource::UpdateL( CMceComMediaSource& aUpdate )
    {
    CMceComMediaSource::UpdateL( aUpdate );
    CMceComCameraSource& update = static_cast<CMceComCameraSource&>( aUpdate );
    
    iCameraIndex = update.iCameraIndex;
    iZoomFactor = update.iZoomFactor;
    iDigitalZoomFactor = update.iDigitalZoomFactor;
    iContrast = update.iContrast;
    iBrightness = update.iBrightness;
    iExposure = update.iExposure;
    iWhiteBalance = update.iWhiteBalance;
    iCamerasAvailable = update.iCamerasAvailable;
    iCameraInfo = update.iCameraInfo;
    iInitialInfoRetrieved = update.iInitialInfoRetrieved;
    
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComCameraSource::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComCameraSource::MccType()
    {
    return KUidMccVideoSource;
    }

// -----------------------------------------------------------------------------
// CMceComCameraSource::InitParamL
// -----------------------------------------------------------------------------
//    
const TDesC8& CMceComCameraSource::InitParamL( const CMceComCodec& aCodec )
    {
    delete iSettings;
    iSettings = NULL;

    // Camera index is the only effective value
    TMccVideoSourceSetting cameraSetting;
    cameraSetting.iSize = TSize( 0, 0 );
	cameraSetting.iVideoPayloadType = 0;
	cameraSetting.iAudioPayloadType = 0;
	cameraSetting.iDeviceIndex = iCameraIndex;
	cameraSetting.iPriority.iPriority = 0;
	cameraSetting.iPriority.iPref = EMdaPriorityPreferenceNone;
	
	if ( aCodec.Stream() && aCodec.Stream()->iType == KMceVideo )
        {
        const CMceComVideoCodec& videoCodec = 
            reinterpret_cast<const CMceComVideoCodec&>( aCodec );
        if ( videoCodec.IsConfigKeyValid() )
            {
            cameraSetting.iConfigKey = *videoCodec.iConfigKey;
            }
        else
            {
            cameraSetting.iConfigKey = KNullDesC8;
            }
        }

    TMccVideoSourceSettingBuf settings( cameraSetting );
    
    iSettings = settings.AllocL();
    
    return *iSettings;
    }

// -----------------------------------------------------------------------------
// CMceComCameraSource::DoPrepared
// -----------------------------------------------------------------------------
// 
TInt CMceComCameraSource::DoPrepared()
    {
    TInt err( KErrNone );
    if ( MediaStream() && MediaStream()->Session() &&
         &MediaStream()->Session()->SdpSession() )
        {
        TRAP( err, 
        MediaStream()->Session()->SdpSession().Manager().UpdateConfigKeyL( *this ) );
        }
    return err;
    }
    
#endif//MCE_COMMON_SERVER_SIDE

   
    


    

