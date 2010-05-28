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




// INCLUDE FILES
#include <e32svr.h>

#include "mcccamerahandler.h"
#include "mccscreen.h"
#include "mcccontrollerlogs.h"
#include "mccresources.h"
#include "mcctimermanager.h"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

const TInt KMccCameraHandlerValueNotAssigned = KMaxTInt;


// MACROS

#define MCC_ASSERT_CAMERA_VAL_L( a ) \
__ASSERT_ALWAYS( a != KMccCameraHandlerValueNotAssigned, User::Leave( KErrNotReady ) );  

#define MCC_USE_CAMERA( func_call ) \
if ( iCamera ) { iCamera->func_call; }

#define MCC_USE_READY_CAMERA( func_call ) \
if ( iCamera && IsCameraReady() ) { iCamera->func_call; }

#define MCC_USE_SCREEN( func_call ) \
if ( iScreen ) { iScreen->func_call; }    

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCameraHandler::CMccCameraHandler
// -----------------------------------------------------------------------------
//
CMccCameraHandler::CMccCameraHandler( 
    MMccResources& aResources, TInt aCameraIndex, TInt aPriority ):
    iResources( aResources ),
	iState( EInit ),
	iPriority( aPriority ), 
	iCameraIndex( aCameraIndex ),
	iZoom( KMccCameraHandlerValueNotAssigned ),
	iDigitalZoom( KMccCameraHandlerValueNotAssigned ),
	iContrast( KMccCameraHandlerValueNotAssigned ),
	iBrightness( KMccCameraHandlerValueNotAssigned ),
	iWhiteBalance( KMccCameraHandlerValueNotAssigned ),
	iExposure( KMccCameraHandlerValueNotAssigned )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::~CMccCameraHandler
// -----------------------------------------------------------------------------
//
CMccCameraHandler::~CMccCameraHandler()
    {
	__CONTROLLER( "CMccCameraHandler::~CMccCameraHandler" )
	
	NotifyObservers( KErrCancel );
	
	iObservers.Reset();
    iObservers.Close();
    
    DoReleaseCamera();
    DoReleaseScreen();

    
#ifdef MCC_CAMERAHANDLER_TEST
    RemoveDependencyFile();
#endif
    
	__CONTROLLER( "CMccCameraHandler::~CMccCameraHandler, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::NewL
// -----------------------------------------------------------------------------
//
CMccCameraHandler* CMccCameraHandler::NewL( 
    MMccResources& aResources, TInt aCameraIndex, TInt aPriority )
    {    
    CMccCameraHandler* self = 
        new ( ELeave ) CMccCameraHandler( aResources, aCameraIndex, aPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ConstructL()
    {
	__CONTROLLER( "CMccCameraHandler::ConstructL" )
	
#ifdef MCC_CAMERAHANDLER_TEST
    CreateDependencyFileL();
#endif
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::EnableViewFinderL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::EnableViewFinderL( TMccVideoSinkSetting& aSetting )
	{
	__CONTROLLER( "CMccCameraHandler::EnableViewFinderL" )

    if ( iScreen && SettingsChanged( aSetting ) )
        {
        DoReleaseScreen();
        }
    
    iViewFinderSettings = aSetting;
    
    if ( !iViewFinderSettings.iEnabled ){
        __CONTROLLER( "CMccCameraHandler::EnableViewFinderL, param update, exit" )
        return;
    }
    __CONTROLLER( "CMccCameraHandler::EnableViewFinderL, starting vf" )

    CCamera* cam = IsCameraReady() ? iCamera : NULL;
    if ( !iScreen )
        {                
	    __CONTROLLER( "CMccCameraHandler::EnableViewFinderL, starting window" )
	    __CONTROLLER_INT2( "CMccCameraHandler::EnableViewFinderL, location",  
	                       iViewFinderSettings.iLocation.iX, 
	                       iViewFinderSettings.iLocation.iY)
		iScreen = CMccScreen::NewL( cam,
                                    iViewFinderSettings.iLocation, 
		                            iViewFinderSettings.iSize, 
		                            iViewFinderSettings.iDeviceIndex,
		                            iViewFinderSettings.iWindowOrdinalPosition,
		                            iViewFinderSettings.iWindowOrdinalPriority );
        }
    else
        {
        User::LeaveIfError( iScreen->SetCamera( cam ) );
        }
   
    iScreen->StartL();
    
    iViewFinderEnabled = ETrue;   
        
	__CONTROLLER( "CMccCameraHandler::EnableViewFinderL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::DisableViewFinderL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::DisableViewFinderL()
	{
	__CONTROLLER( "CMccCameraHandler::DisableViewFinderL" )
	
    DoReleaseScreen();   
    
    iViewFinderEnabled = EFalse;
	
	__CONTROLLER( "CMccCameraHandler::DisableViewFinderL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::IsViewFinderEnabled
// -----------------------------------------------------------------------------
//  
TBool CMccCameraHandler::IsViewFinderEnabled() const
    {
    // Zero size vf is considered as disabled
    return ( iViewFinderEnabled &&  
           ( iViewFinderSettings.iSize.iHeight != 0 || 
             iViewFinderSettings.iSize.iWidth != 0 ) );
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetViewFinderSettingsL
// -----------------------------------------------------------------------------
//	
void CMccCameraHandler::GetViewFinderSettingsL( TMccVideoSinkSetting& aSetting )
    {
    __CONTROLLER( "CMccCameraHandler::GetViewFinderSettingsL" )
    __CONTROLLER_INT2( "CMccCameraHandler::GetViewFinderSettingsL, size",  
	                       iViewFinderSettings.iSize.iHeight, 
	                       iViewFinderSettings.iSize.iWidth)
    __CONTROLLER_INT2( "CMccCameraHandler::GetViewFinderSettingsL, location",  
	                       iViewFinderSettings.iLocation.iX, 
	                       iViewFinderSettings.iLocation.iY)               
    aSetting = iViewFinderSettings;  
    aSetting.iEnabled = iViewFinderEnabled;
    __CONTROLLER( "CMccCameraHandler::GetViewFinderSettingsL,exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetCamInfoL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetCamInfoL( TCameraInfo& aInfo, TBool aForceCameraCreation )
    {
	__CONTROLLER( "CMccCameraHandler::GetCamInfoL" )
	
	TBool removeTemporaryCamera( EFalse );
	if ( aForceCameraCreation && !iCamera )
	    {
	    iCamera = CreateCameraL();
	    removeTemporaryCamera = ETrue;
	    }
	    
	__ASSERT_ALWAYS( iCamera, User::Leave( KErrNotReady ) );
    
    iCamera->CameraInfo( aInfo );
    
    if ( removeTemporaryCamera )
        {
        delete iCamera;
        iCamera = 0;
        }
        
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, orientation", aInfo.iOrientation )
    __CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, options", aInfo.iOptionsSupported )
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, flash modes", aInfo.iFlashModesSupported )
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, exposure modes", aInfo.iExposureModesSupported )
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, white balance modes", aInfo.iWhiteBalanceModesSupported )
    __CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, min zoom", aInfo.iMinZoom )
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, max zoom", aInfo.iMaxZoom )
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, max digital zoom", aInfo.iMaxDigitalZoom )
    __CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, min zoom factor", aInfo.iMinZoomFactor )
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, max zoom factor", aInfo.iMaxZoomFactor )
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, max digital zoom factor", aInfo.iMaxDigitalZoomFactor )
    __CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, image sizes", aInfo.iNumImageSizesSupported )
	__CONTROLLER_INT1( "CMccCameraHandler::GetCamInfoL, image formats", aInfo.iImageFormatsSupported )	
    
	__CONTROLLER( "CMccCameraHandler::GetCamInfoL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetNumOfCamerasL
// -----------------------------------------------------------------------------
//
TInt CMccCameraHandler::GetNumOfCamerasL()
    {
	__CONTROLLER( "CMccCameraHandler::GetNumOfCamerasL" )
    
    TInt count = CCamera::CamerasAvailable();
    
	__CONTROLLER_INT1( "CMccCameraHandler::GetNumOfCamerasL, count ", count )
	__CONTROLLER( "CMccCameraHandler::GetNumOfCamerasL, exit" )
	return count;
    }
// -----------------------------------------------------------------------------
// CMccCameraHandler::GetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetValueL( TUint32 aParam, TInt& aVal )
    {
	__CONTROLLER( "CMccCameraHandler::GetValue" )
	switch( aParam )
	    {
	    case KMccCamZoomFactor:
    	    {
            TInt temp = iCamera ? iCamera->ZoomFactor() : iZoom;
            MCC_ASSERT_CAMERA_VAL_L( temp )      
            aVal = temp;
	        break;   
    	    }
	    case KMccCamDigitZoomFactor:
	        {
            TInt temp = iCamera ? iCamera->DigitalZoomFactor() : iDigitalZoom;
            MCC_ASSERT_CAMERA_VAL_L( temp )
            aVal = temp;
	        break;
	        }    
	    case KMccCamContrast:
	    	{
            TInt temp = iCamera ? iCamera->Contrast() : iContrast;
            MCC_ASSERT_CAMERA_VAL_L( temp )
            aVal = temp;
	        break;
	    	}
	    case KMccCamBrightness:
	        {
            TInt temp = iCamera ? iCamera->Brightness() : iBrightness;
            MCC_ASSERT_CAMERA_VAL_L( temp )
            aVal = temp;
	        break;
	        }
	    case KMccCamWhiteBalance:
	        {
            TInt temp = iCamera ? iCamera->WhiteBalance() : iWhiteBalance;
            MCC_ASSERT_CAMERA_VAL_L( temp )
            aVal = temp;
	        break;
	        }
	    case KMccCamExposure:
	        {
	        TInt temp = iCamera ? iCamera->Exposure() : iExposure;
	        MCC_ASSERT_CAMERA_VAL_L( temp )
            aVal = temp;
	        break;
	        }
	    default:
	        {
	        User::Leave( KErrNotSupported );  
	        break;
	        }
	    }
	__CONTROLLER_INT2( "CMccCameraHandler::GetValue, param, value", aParam, aVal )
	__CONTROLLER( "CMccCameraHandler::GetValue, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::SetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::SetValueL( TUint32 aParam, TInt aVal )
    {
	__CONTROLLER( "CMccCameraHandler::SetValue" )
	
	if ( aVal == KMccCameraHandlerValueNotAssigned )
	    {
	    __CONTROLLER( "CMccCameraHandler::SetValue, value not assigned, exit" )
	    return;
	    }
	    
	__CONTROLLER_INT2( "CMccCameraHandler::SetValue, param, value", aParam, aVal )
	switch( aParam )
	    {
	    case KMccCamZoomFactor:
            MCC_USE_CAMERA( SetZoomFactorL( aVal ) )
            iZoom = aVal;
	        break;
	    case KMccCamDigitZoomFactor:
            MCC_USE_CAMERA( SetDigitalZoomFactorL( aVal ) )
            iDigitalZoom = aVal;
	        break;
	    case KMccCamContrast:
            MCC_USE_CAMERA( SetContrastL( aVal ) )
            iContrast = aVal;
	        break;
	    case KMccCamBrightness:
            MCC_USE_CAMERA( SetBrightnessL( aVal ) )
            iBrightness = aVal;
	        break;
	    case KMccCamWhiteBalance:
            MCC_USE_CAMERA( SetWhiteBalanceL( static_cast<CCamera::TWhiteBalance>( aVal ) ) )
            iWhiteBalance = aVal;
	        break;
	    case KMccCamExposure:
	        MCC_USE_CAMERA( SetExposureL( static_cast<CCamera::TExposure>( aVal ) ) )
	        iExposure = aVal;
	        break;
	    default:
	        User::Leave( KErrNotSupported );    
	    }
	__CONTROLLER( "CMccCameraHandler::SetValue, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::GetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::GetValueL( TUint32 /*aParam*/, TReal& /*aVal*/ )
    {
	User::Leave( KErrNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::SetValue
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::SetValueL( TUint32 /*aParam*/, TReal /*aVal*/ )
    {
	User::Leave( KErrNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ReserveResourcesL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ReserveResourcesL( 
    TInt aCameraIndex, TBool aForceCameraCreation )
    {
    __CONTROLLER_INT1( "CMccCameraHandler::ReserveResourcesL, forced:", 
                       aForceCameraCreation )
    
    // If camera is in use, camera with different index cannot be reserved
    if ( iCamera )
        {
        __ASSERT_ALWAYS( iCameraIndex == aCameraIndex, User::Leave( KErrInUse ) );
        }
    else
        {
        __CONTROLLER_INT1( "CMccCameraHandler::ReserveResourcesL, creating camera (index):", 
                           aCameraIndex )
        TBool bIndexChanged = (iCameraIndex != aCameraIndex);
        iCameraIndex = aCameraIndex;        
        iCamera = CreateCameraL();
        if ( bIndexChanged )
            {
            /* Reset the camera handler internal values , for example zoom factor */
            ResetCameraHandler();
            }
        }   
        
    __ASSERT_ALWAYS( iCamera, User::Leave( KErrNotReady ) );

    if ( iState == EFailed )
        {
        __CONTROLLER( "CMccCameraHandler::ReserveResourcesL, async failure has occured!" )  
        User::Leave( KErrInUse );
        }
    else if ( iState == EInit )
        {
        __CONTROLLER( "CMccCameraHandler::ReserveResourcesL, reserving" )  
        
        iCamera->Reserve();
        iState = EReserving;
        }
    else
        {
        __CONTROLLER( "CMccCameraHandler::ReserveResourcesL, already reserved" )  
        }
    
    iForcedResourceReservation = aForceCameraCreation;
    
    __CONTROLLER( "CMccCameraHandler::ReserveResourcesL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ReleaseResources
// -----------------------------------------------------------------------------
//       
void CMccCameraHandler::ReleaseResources()
    {
    __CONTROLLER( "CMccCameraHandler::ReleaseResources" )
    
    if ( iForcedResourceReservation )
        {
        __CONTROLLER( "CMccCameraHandler::  forcefully reserved, not releasing" )
        }
    else if ( !IsViewFinderEnabled() )
        {
        DoReleaseScreen();
        DoReleaseCamera();
        }
    else
        {
        // Camera is not really released in case viewfinder is still enabled.
        __CONTROLLER( "CMccCameraHandler:: Do not release yet, wait for vf disabling" )
        }
    
	  
    __CONTROLLER( "CMccCameraHandler::ReleaseResources, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::Handle
// -----------------------------------------------------------------------------
//
TUint32 CMccCameraHandler::Handle()
	{
	if ( iCamera )
	    {
	    return iCamera->Handle();
	    }
	else
	    {
	    return 0;
	    }
	}
	    
// -----------------------------------------------------------------------------
// CMccCameraHandler::IsCameraReady
// -----------------------------------------------------------------------------
//        
TBool CMccCameraHandler::IsCameraReady()
    {
    return ( iState == EPowered ); 
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::MonitorCameraReservation
// -----------------------------------------------------------------------------
//        
TInt CMccCameraHandler::MonitorCameraReservation( 
    MMccCameraHandlerObserver& aObserver, 
    TBool aForceCameraReserve )
    {
    __CONTROLLER_INT1( "CMccCameraHandler::MonitorCameraReservation, force", 
                       aForceCameraReserve )
    TInt err( KErrNone );
    if ( iState != EPowered )
        {
        __CONTROLLER( "CMccCameraHandler::MonitorCameraReservation, start monitoring" )
        err = iObservers.Append( &aObserver );
        if ( !err && aForceCameraReserve )
            {

            TRAP( err, ReserveResourcesL( iCameraIndex, aForceCameraReserve ) );    
            }
        }
    else
        {
        // Camera is already ready for usage
        err = KErrArgument;
        }
    
    __CONTROLLER_INT1( "CMccCameraHandler::MonitorCameraReservation, exit with err", err )   
    return err;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::StopCameraMonitoring
// -----------------------------------------------------------------------------
//  
TInt CMccCameraHandler::StopCameraMonitoring( 
    MMccCameraHandlerObserver& aObserver,
    TBool aForceCameraRelease )
    {
    __CONTROLLER_INT1( "CMccCameraHandler::StopCameraMonitoring, force",
                       aForceCameraRelease )   
    TInt err( KErrNone );
    TInt index = iObservers.Find( &aObserver );
    if ( index != KErrNotFound )
        {
        iObservers.Remove( index );
        }
    else
        {
        err = KErrNotFound;
        }

    if ( iForcedResourceReservation && aForceCameraRelease )
        {
        __CONTROLLER( "CMccCameraHandler::StopCameraMonitoring, releasing resource" )
        iForcedResourceReservation = EFalse;
        ReleaseResources();
        }
    
    __CONTROLLER_INT1( "CMccCameraHandler::StopCameraMonitoring, exit with err", err )   
    return err;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::HandleEvent
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::HandleEvent( const TECAMEvent& aEvent )
    {
    __CONTROLLER( "CMccCameraHandler::HandleEvent" )
    
    if ( aEvent.iEventType == KUidECamEventReserveComplete )
        {
        ReserveComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PowerOnComplete( aEvent.iErrorCode );
        }
    else
        {
        __CONTROLLER( "CMccCameraHandler::HandleEvent, unknown event" )
        }
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ViewFinderReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ViewFinderReady( 
    MCameraBuffer& aCameraBuffer, 
    TInt aError )
    {
    if ( !aError )
		{
		if ( aCameraBuffer.NumFrames() > 0 )
		    {
		    TRAP( aError, DoViewFinderFrameReady( aCameraBuffer.BitmapL( 0 ) ) );
		    }
		else
		    {
		    #if ( defined __WINSCW__ ) || ( defined __WINS__ )
		    // Bitmap might be missing in emulator, ignore the problem
		    __CONTROLLER( "CMccCameraHandler::ViewFinderReady, no bitmap" )
		    #else  
		    aError = KErrNotReady;
		    #endif
		    }
		}
	if ( aError )
	    {
	    __CONTROLLER_INT1( "CMccCameraHandler::ViewFinderReady, error:", aError )
	    iState = EFailed;
	    }
	    
	aCameraBuffer.Release();
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ImageBufferReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ImageBufferReady( 
    MCameraBuffer& /*aCameraBuffer*/,TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::VideoBufferReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::VideoBufferReady( 
    MCameraBuffer& /*aCameraBuffer*/,TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::TimerExpiredL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::TimerExpiredL( TMccTimerId /*aTimerId*/, TAny* /*aTimerParam*/ )
    { 
    __CONTROLLER( "CMccCameraHandler::TimerExpiredL, NOP" )
    }
    
// -----------------------------------------------------------------------------
// CMccCameraHandler::ReserveComplete
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ReserveComplete( TInt aError )
	{
	__CONTROLLER( "CMccCameraHandler::ReserveComplete" )
	DoReserveComplete( aError );
	}
	
// -----------------------------------------------------------------------------
// CMccCameraHandler::PowerOnComplete
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::PowerOnComplete( TInt aError )
	{
	__CONTROLLER( "CMccCameraHandler::PowerOnComplete" )
	DoPowerOnComplete( aError );
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::ViewFinderFrameReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ViewFinderFrameReady( CFbsBitmap& aFrame )
	{
	DoViewFinderFrameReady( aFrame );
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::ImageReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ImageReady( CFbsBitmap* /*aBitmap*/, HBufC8* /*aData*/, TInt /*aError*/ )
	{
	
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::FrameBufferReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::FrameBufferReady( MFrameBuffer* /*aFrameBuffer*/, TInt /*aError*/ )
	{	
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::DoReserveComplete
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::DoReserveComplete( TInt aError )
	{
	__CONTROLLER_INT1( "CMccCameraHandler::DoReserveComplete, with value", aError )
	if ( aError == KErrNone )
		{
		__CONTROLLER( "CMccCameraHandler::DoReserveComplete, powering on camera" )
		iState = EReserved;
		iCamera->PowerOn();
		}
	else
		{
		__CONTROLLER_INT1( "CMccCameraHandler::DoReserveComplete, reserving failed", aError )
		iState = EFailed;
		
		// Notify observers immediately since reservation is not continued
		NotifyObservers( aError );
		}	
	__CONTROLLER( "CMccCameraHandler::DoReserveComplete, exit" )
	}
	
// -----------------------------------------------------------------------------
// CMccCameraHandler::DoPowerOnComplete
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::DoPowerOnComplete( TInt aError )
	{
	__CONTROLLER_INT1( "CMccCameraHandler::DoPowerOnComplete, with value", aError )
	
	if ( aError == KErrNone )
		{
		iState = EPowered;
		
	    // Notify observers about success, important to do before VF handling
		// as VF behaves more nicely at enabling when it sees that media recorder
		// is already using camera.
	    NotifyObservers( aError );
		
		// Viewfinder was enabled before resource release, enable again.
		// Also set old camera settings.
		if ( iViewFinderEnabled )
		    {
		    TRAP( aError, 
		        {
		        EnableViewFinderL( iViewFinderSettings );
		        UpdateCameraSettingsL();
		        } 
		        );
		    if ( aError )
		        {
		        __CONTROLLER_INT1( "CMccCameraHandler::DoPowerOnComplete, setting renew failure", 
		                           aError )
		        iState = EFailed;    
		        } 
		    }
		}
	else
		{
		__CONTROLLER_INT1( "CMccCameraHandler::DoPowerOnComplete, powering failed", aError )
		iState = EFailed;
		
	    // Notify observers about failure
	    NotifyObservers( aError );
		}
    
	__CONTROLLER( "CMccCameraHandler::DoPowerOnComplete, exit" )
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::DoViewFinderFrameReady
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::DoViewFinderFrameReady( CFbsBitmap& aFrame )
	{
	MCC_USE_SCREEN( Draw( aFrame ) )
	}
		
// -----------------------------------------------------------------------------
// CMccCameraHandler::NotifyObservers
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::NotifyObservers( TInt aError )
	{	
	__CONTROLLER_INT1( "CMccCameraHandler::NotifyObservers, error", aError )
	
	TInt lastIndex( iObservers.Count() - 1 );
	for ( TInt i = lastIndex; i >= 0; i-- )
	    {
	    iObservers[ i ]->CameraReady( aError );
	    iObservers.Remove( i );
	    }
	    
    __CONTROLLER( "CMccCameraHandler::NotifyObservers, exit" )	
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::SettingsChanged
// -----------------------------------------------------------------------------
//	
TBool CMccCameraHandler::SettingsChanged( const TMccVideoSinkSetting& aSetting ) const
    {
    return ( iViewFinderSettings.iLocation != aSetting.iLocation ||
             iViewFinderSettings.iSize != aSetting.iSize ||
             iViewFinderSettings.iDeviceIndex != aSetting.iDeviceIndex ||
             iViewFinderSettings.iWindowOrdinalPosition != aSetting.iWindowOrdinalPosition ||
             iViewFinderSettings.iWindowOrdinalPriority != aSetting.iWindowOrdinalPriority );
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::UpdateCameraSettingsL
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::UpdateCameraSettingsL()
    {
    __CONTROLLER( "CMccCameraHandler::UpdateCameraSettingsL" )
    
    SetValueL( KMccCamZoomFactor, iZoom );
	SetValueL( KMccCamDigitZoomFactor, iDigitalZoom );
    SetValueL( KMccCamContrast, iContrast );
	SetValueL( KMccCamBrightness, iBrightness );
	SetValueL( KMccCamWhiteBalance, iWhiteBalance );
	SetValueL( KMccCamExposure, iExposure );
	
	__CONTROLLER( "CMccCameraHandler::UpdateCameraSettingsL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::CreateCameraL
// -----------------------------------------------------------------------------
//
CCamera* CMccCameraHandler::CreateCameraL()
    {
    __CONTROLLER( "CMccCameraHandler::CreateCameraL" )
    
    CCamera* camera = NULL;
    TRAPD( err, camera = CCamera::New2L( *this, iCameraIndex, iPriority ) );
    __CONTROLLER_INT1( "CMccCameraHandler::CreateCameraL, camera2 creation, err:", err )
    
    if ( err == KErrNotSupported )
        {
        camera = CCamera::NewL( *this, iCameraIndex );
        }
    else
        {
        User::LeaveIfError( err );
        }
    
    __CONTROLLER( "CMccCameraHandler::CreateCameraL, exit" )
    return camera;
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::DoReleaseScreen
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::DoReleaseScreen()
    {
    __CONTROLLER( "CMccCameraHandler::DoReleaseScreen" );
    
    delete iScreen;
    iScreen = NULL;
    
    __CONTROLLER( "CMccCameraHandler::DoReleaseScreen, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::DoReleaseCamera
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::DoReleaseCamera()
    {
    __CONTROLLER( "CMccCameraHandler::DoReleaseCamera" )
    
    MCC_USE_SCREEN( SetCamera( NULL ) )
    
    if ( iCamera )
        {	
        iCamera->Release();
        }
    
    delete iCamera;
    iCamera = NULL;

    iState = EInit;
    
    __CONTROLLER( "CMccCameraHandler::DoReleaseCamera, exit" )
    }

// -----------------------------------------------------------------------------
// CMccCameraHandler::ResetCameraHandler
// -----------------------------------------------------------------------------
//
void CMccCameraHandler::ResetCameraHandler()
    {
    __CONTROLLER( "CMccCameraHandler::ResetCameraHandler, entry" )
    iZoom = KMccCameraHandlerValueNotAssigned;
    iDigitalZoom = KMccCameraHandlerValueNotAssigned;
    iContrast = KMccCameraHandlerValueNotAssigned;
    iBrightness = KMccCameraHandlerValueNotAssigned;
    iWhiteBalance = KMccCameraHandlerValueNotAssigned;
    iExposure = KMccCameraHandlerValueNotAssigned;
    __CONTROLLER( "CMccCameraHandler::ResetCameraHandler, exit" )
    }    

#ifdef MCC_CAMERAHANDLER_TEST

_LIT( KTestCameraPluginName, "C:\\102070cc.txt" );

// -----------------------------------------------------------------------------
// CMccCameraHandler::CreateDependencyFileL
// -----------------------------------------------------------------------------
//	
void CMccCameraHandler::CreateDependencyFileL()
	{
	// Enforce dependency
	// create the file the plugin depends on.
	User::LeaveIfError( iFsSession.Connect() );
	RFile file;
	TInt err = file.Replace( iFsSession, KTestCameraPluginName, EFileShareAny );
  	file.Close();
  	if ( err != KErrNone )
  		{
	    User::LeaveIfError( KErrNotSupported );
  		}
   	User::LeaveIfError( iFbsSession.Connect( iFsSession ) );
	}

// -----------------------------------------------------------------------------
// CMccCameraHandler::RemoveDependencyFile
// -----------------------------------------------------------------------------
//	
void CMccCameraHandler::RemoveDependencyFile()
	{
	iFbsSession.Disconnect();
	iFsSession.Delete( KTestCameraPluginName );
	iFsSession.Close();
	}		

#endif
	
// End of file

