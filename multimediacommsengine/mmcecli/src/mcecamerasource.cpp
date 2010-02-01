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




#include "mcecamerasource.h"
#include "mcecomcamerasource.h"
#include "mcesession.h"
#include "mcemediastream.h"
#include "mceitcsender.h"
#include "mcemanager.h"
#include "mce.h"
#include "mceserial.h"
#include "mceclilogs.h"


#define _FLAT_DATA static_cast<CMceComCameraSource*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data


const TInt KMinDigitalZoom = 0;
const TInt KMinContrast = -100;
const TInt KMaxContrast = 100;
const TInt KMinBrightness = -100;
const TInt KMaxBrightness = 100; 


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCameraSource* CMceCameraSource::NewL( CMceManager& aManager )
    {
    CMceCameraSource* self = CMceCameraSource::NewLC( aManager );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCameraSource* CMceCameraSource::NewLC( CMceManager& aManager )
    {
    CMceCameraSource* self = new (ELeave) CMceCameraSource();
    CleanupStack::PushL( self );
    self->ConstructL( &aManager );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCameraSource::~CMceCameraSource()
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCameraSource::EnableL()
    {
    MCECLI_DEBUG("CMceCameraSource::EnableL, Entry");
    
    CMceMediaSource::DoEnableL();

    MCECLI_DEBUG("CMceCameraSource::EnableL, Exit");
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCameraSource::DisableL()
    { 
    MCECLI_DEBUG("CMceCameraSource::DisableL, Entry");
    
    CMceMediaSource::DoDisableL();
    
    MCECLI_DEBUG("CMceCameraSource::DisableL, Exit");
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	    
EXPORT_C TInt CMceCameraSource::CamerasAvailable() const
    {
    return FLAT_DATA( iCamerasAvailable );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCameraSource::SetCameraIndexL( TInt aCameraIndex )	  
    {
    MCECLI_DEBUG("CMceCameraSource::SetCameraIndexL, Entry");
    MCECLI_DEBUG_DVALUE("index", aCameraIndex );
    
    
    __ASSERT_ALWAYS( aCameraIndex >= 0 &&
                     aCameraIndex < this->CamerasAvailable(),
                     User::Leave( KErrArgument ) );
                     
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        
        
        FLAT_DATA( iCameraInfo ) = SetITCCameraIndexL( aCameraIndex );
        FLAT_DATA( iCameraIndex ) = aCameraIndex;
        
        
        }
    else
        {
    	FLAT_DATA( iCameraIndex ) = aCameraIndex;
        MCECLI_DEBUG("CMceCameraSource::SetCameraIndexL, done locally");
        }                 
        
    MCECLI_DEBUG("CMceCameraSource::SetCameraIndexL, Exit");
    
    }          
     
       
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	     
EXPORT_C void CMceCameraSource::GetCameraInfo( TCameraInfo& aInfo ) const
    {
    aInfo = FLAT_DATA( iCameraInfo );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetZoomFactorL( TInt aZoomFactor )
    {
    MCECLI_DEBUG("CMceCameraSource::SetZoomFactorL, Entry");
    MCECLI_DEBUG_DVALUE("factor", aZoomFactor );
    
    __ASSERT_ALWAYS( aZoomFactor >= FLAT_DATA( iCameraInfo.iMinZoom ) &&
                     aZoomFactor <= FLAT_DATA( iCameraInfo.iMaxZoom ), 
                     User::Leave( KErrArgument ) );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iZoomFactor ) = SetITCValueL( EMceItcSetZoomFactor,
                                                 aZoomFactor ); 
        }
    else
        {
    	FLAT_DATA( iZoomFactor ) = aZoomFactor;
        MCECLI_DEBUG("CMceCameraSource::SetZoomFactorL, done locally");
        }       
        
    MCECLI_DEBUG("CMceCameraSource::SetZoomFactorL, Exit");
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCameraSource::ZoomFactorL()
    {
    MCECLI_DEBUG("CMceCameraSource::ZoomFactorL, Entry");
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iZoomFactor ) = GetITCValueL( EMceItcZoomFactor );
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceCameraSource::ZoomFactorL, done locally");
        }

    MCECLI_DEBUG("CMceCameraSource::ZoomFactorL, Exit");
        
    return FLAT_DATA( iZoomFactor ); 
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetDigitalZoomFactorL( TInt aDigitalZoomFactor )
    {
    MCECLI_DEBUG("CMceCameraSource::SetDigitalZoomFactorL, Entry");
    MCECLI_DEBUG_DVALUE("factor", aDigitalZoomFactor );
    
    __ASSERT_ALWAYS( aDigitalZoomFactor >= KMinDigitalZoom &&
                aDigitalZoomFactor <= FLAT_DATA( iCameraInfo.iMaxDigitalZoom ), 
                User::Leave( KErrArgument ) );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iDigitalZoomFactor ) = 
                SetITCValueL( EMceItcSetDigitalZoomFactor,
                              aDigitalZoomFactor ); 
        }
    else
        {
    	FLAT_DATA( iDigitalZoomFactor ) = aDigitalZoomFactor;
        MCECLI_DEBUG("CMceCameraSource::SetDigitalZoomFactorL, done locally");
        }       
    
    MCECLI_DEBUG("CMceCameraSource::SetDigitalZoomFactorL, Exit");
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCameraSource::DigitalZoomFactorL()
    {
    MCECLI_DEBUG("CMceCameraSource::DigitalZoomFactorL, Entry");
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iDigitalZoomFactor ) = 
                    GetITCValueL( EMceItcDigitalZoomFactor );
        }                          
    else
        {
        //NOP
        MCECLI_DEBUG("CMceCameraSource::DigitalZoomFactorL, done locally");
        }

    MCECLI_DEBUG("CMceCameraSource::DigitalZoomFactorL, Exit");
    return FLAT_DATA( iDigitalZoomFactor );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetContrastL( TInt aContrast )
    {
    MCECLI_DEBUG("CMceCameraSource::SetContrastL, Entry");
    MCECLI_DEBUG_DVALUE("contrast", aContrast );
    
    __ASSERT_ALWAYS( ( FLAT_DATA( iCameraInfo.iOptionsSupported ) & 
                       TCameraInfo::EContrastSupported ) ==
                     TCameraInfo::EContrastSupported,
                     User::Leave( KErrNotSupported ) );
                    
    __ASSERT_ALWAYS( (( aContrast >= KMinContrast &&
                        aContrast <= KMaxContrast ) ||
                        aContrast == CCamera::EContrastAuto ),
                     User::Leave( KErrArgument ) );
                      
                                           
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iContrast ) = SetITCValueL( EMceItcSetContrast,
                                               aContrast ); 
        }
    else
        {
    	FLAT_DATA( iContrast ) = aContrast;
        MCECLI_DEBUG("CMceCameraSource::SetContrastL, done locally");
        }
        
    MCECLI_DEBUG("CMceCameraSource::SetContrastL, Exit");
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCameraSource::ContrastL()
    {
    MCECLI_DEBUG("CMceCameraSource::ContrastL, Entry");
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iContrast ) = GetITCValueL( EMceItcContrast );
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceCameraSource::ContrastL, done locally");
        }
    
    MCECLI_DEBUG("CMceCameraSource::ContrastL, Exit");
    return FLAT_DATA( iContrast );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetBrightnessL( TInt aBrightness )
    {
    MCECLI_DEBUG("CMceCameraSource::SetBrightnessL, Entry");
    MCECLI_DEBUG_DVALUE("brightness", aBrightness );
    
    __ASSERT_ALWAYS( ( FLAT_DATA( iCameraInfo.iOptionsSupported ) & 
                       TCameraInfo::EBrightnessSupported ) ==
                     TCameraInfo::EBrightnessSupported,
                     User::Leave( KErrNotSupported ) );
                   
    __ASSERT_ALWAYS( (( aBrightness >= KMinBrightness &&
                        aBrightness <= KMaxBrightness ) ||
                        aBrightness == CCamera::EBrightnessAuto ),
                     User::Leave( KErrArgument ) );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iBrightness ) = SetITCValueL( EMceItcSetBrightness,
                                                 aBrightness ); 
        }
    else
        {
    	FLAT_DATA( iBrightness ) = aBrightness;
        MCECLI_DEBUG("CMceCameraSource::SetBrightnessL, done locally");
        }
        
    MCECLI_DEBUG("CMceCameraSource::SetBrightnessL, Exit");

    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCameraSource::BrightnessL()
    {
    MCECLI_DEBUG("CMceCameraSource::BrightnessL, Entry");
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iBrightness ) = GetITCValueL( EMceItcBrightness );
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceCameraSource::BrightnessL, done locally");
        }

    MCECLI_DEBUG("CMceCameraSource::BrightnessL, Exit");
    return FLAT_DATA( iBrightness ); 
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetExposureL( CCamera::TExposure aExposure )
    {
    MCECLI_DEBUG("CMceCameraSource::SetExposureL, Entry");
    MCECLI_DEBUG_DVALUE("exposure", aExposure );
    
    __ASSERT_ALWAYS( 
          ( FLAT_DATA( iCameraInfo.iExposureModesSupported ) & aExposure ) ==
                        aExposure,
                        User::Leave( KErrNotSupported ) );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iExposure ) = SetITCValueL( EMceItcSetExposure,
                                               aExposure ); 
        }
    else
        {
    	FLAT_DATA( iExposure ) = aExposure;
        MCECLI_DEBUG("CMceCameraSource::SetExposureL, done locally");
        }
        
    MCECLI_DEBUG("CMceCameraSource::SetExposureL, Exit");
        
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C CCamera::TExposure CMceCameraSource::ExposureL()
    {
    MCECLI_DEBUG("CMceCameraSource::ExposureL, Entry");
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iExposure ) = GetITCValueL( EMceItcExposure );
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceCameraSource::ExposureL, done locally");
        }

    MCECLI_DEBUG("CMceCameraSource::ExposureL, Exit");
    
    return static_cast<CCamera::TExposure>( FLAT_DATA( iExposure ) );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetWhiteBalanceL( 
                                    CCamera::TWhiteBalance aWhiteBalance )
    {
    MCECLI_DEBUG("CMceCameraSource::SetWhiteBalanceL, Entry");
    MCECLI_DEBUG_DVALUE("white balance", aWhiteBalance );
    
    __ASSERT_ALWAYS( 
        ( FLAT_DATA( iCameraInfo.iWhiteBalanceModesSupported ) & aWhiteBalance ) == 
        aWhiteBalance,
        User::Leave( KErrNotSupported ) );
   
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iWhiteBalance ) = SetITCValueL( EMceItcSetWhiteBalance,
                                                   aWhiteBalance ); 
        }
    else
        {
    	FLAT_DATA( iWhiteBalance ) = aWhiteBalance;
        MCECLI_DEBUG("CMceCameraSource::SetWhiteBalanceL, done locally");
        }
        
    MCECLI_DEBUG("CMceCameraSource::SetWhiteBalanceL, Exit");
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C CCamera::TWhiteBalance CMceCameraSource::WhiteBalanceL()
    {
    MCECLI_DEBUG("CMceCameraSource::WhiteBalanceL, Entry");
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iWhiteBalance ) = GetITCValueL( EMceItcWhiteBalance );
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceCameraSource::WhiteBalanceL, done locally");
        }
    
    MCECLI_DEBUG("CMceCameraSource::WhiteBalanceL, Exit");
    return static_cast<CCamera::TWhiteBalance>( FLAT_DATA( iWhiteBalance ) );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMceCameraSource::InitializeL( CMceManager* aManager )
    {
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );

    if ( !FLAT_DATA( iInitialInfoRetrieved ) )
        {   
        // Receive available camera count
        MCECLI_DEBUG("CMceCameraSource::InitializeL, querying available cameras");
        TMceIds ids;
        ids.iAppUID = aManager->AppUid().iUid;

    	TMceItcArgTInt camerasAvailable;
    	
        _FLAT_DATA->SenderL( 
            aManager->ServerSession() ).ReadL( ids, EMceItcCamerasAvailable, 
                                               camerasAvailable );
    	
        FLAT_DATA( iCamerasAvailable ) = camerasAvailable();

        MCECLI_DEBUG_DVALUE("CMceCameraSource::InitializeL,\
querying camera info for camera ", FLAT_DATA( iCameraIndex ) );
        
        // Receive TCameraInfo
        GetITCCameraInfoL( *aManager, FLAT_DATA( iCameraInfo ) );
    
        FLAT_DATA( iInitialInfoRetrieved ) = ETrue;
        
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMceCameraSource::GetITCCameraInfoL( CMceManager& aManager, 
                                          TCameraInfo& aInfo ) const
    {
    
    // Receive TCameraInfo
    TMceCameraInfo info;
    info.iIndex = FLAT_DATA( iCameraIndex );
    
    TMceItcArgTCameraInfo cameraInfo( info );
    
    TMceIds infoIds;
    infoIds.iAppUID = aManager.AppUid().iUid;
    _FLAT_DATA->SenderL( 
        aManager.ServerSession() ).ReadL( infoIds, EMceItcCameraInfo, 
                                          cameraInfo );
    
    aInfo = cameraInfo().iInfo;
    
    }




// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TCameraInfo CMceCameraSource::SetITCCameraIndexL( TInt aCameraIndex ) const
    {
    CMceSession* session = NULL;
    
    if ( iStream )
        {
        session = iStream->Session();
        }
    
    if ( !session )
        {
        User::Leave( KErrNotReady );
        }
    
    TMceIds ids;
    session->PrepareForITC( ids );
    ids.iMediaID   = iStream->Id();
    ids.iSourceID  = Id();
    
    TMceCameraInfo info;
    info.iIndex = aCameraIndex;
    
    TMceItcArgTCameraInfo value( info );
    
    session->ITCSender().ReadL( ids, EMceItcSetCameraIndex, value );
    
    
    return value().iInfo;
    }


// -----------------------------------------------------------------------------
// Factory method to inner usage
// -----------------------------------------------------------------------------
//
CMceCameraSource* CMceCameraSource::NewL()
    {
    CMceCameraSource* self = CMceCameraSource::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Factory method to inner usage
// -----------------------------------------------------------------------------
//
CMceCameraSource* CMceCameraSource::NewLC()
    {
    CMceCameraSource* self = new (ELeave) CMceCameraSource();
    CleanupStack::PushL( self );
    self->ConstructL( NULL );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CMceCameraSource::GetITCValueL( TUint8 aITCFunction )
    {
    CMceSession* session = NULL;
    
    if ( iStream )
        {
        session = iStream->Session();
        }
    
    if ( !session )
        {
        User::Leave( KErrNotReady );
        }
    
    TMceIds ids;
	session->PrepareForITC( ids );
	ids.iMediaID  = iStream->Id();
	ids.iSourceID = Id();
	
	TMceItcArgTInt value;
	
	session->ITCSender().ReadL( ids, 
	    static_cast<TMceItcFunctions>(aITCFunction), value );
    
    return value();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CMceCameraSource::SetITCValueL( TUint8 aITCFunction, 
                                     TInt aValue )
    {
    CMceSession* session = NULL;
    
    if ( iStream )
        {
        session = iStream->Session();
        }
    
    if ( !session )
        {
        User::Leave( KErrNotReady );
        }

	TMceIds ids;
	session->PrepareForITC( ids );
	ids.iMediaID   = iStream->Id();
	ids.iSourceID  = Id();
	
	TMceItcArgTInt value( aValue );
	
    session->ITCSender().WriteL( ids, 
        static_cast<TMceItcFunctions>(aITCFunction), value );
    
    return value();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMceCameraSource::CMceCameraSource()
    :CMceMediaSource()
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMceCameraSource::ConstructL( CMceManager* aManager )
    {
    CMceComCameraSource* mic = CMceComCameraSource::NewLC();
    CMceMediaSource::ConstructL( mic );
    CleanupStack::Pop( mic );
    
    if ( aManager )
        {
        InitializeL( aManager );
        }
    }

