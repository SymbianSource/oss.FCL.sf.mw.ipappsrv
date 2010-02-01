/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




// INCLUDES
#include "createCameraSourceDialog.h"
#include "CMCETestUIEngineVideoStream.h"
#include "CMCETestUIEngineVideoSource.h"
#include <MCEAudioStream.h>
#include <MCEcameraSource.h>
#include <MCEMediaSource.h>
#include <ecam.h>

// CONSTANTS
const TInt KMCETestUiUseDefaultCamera = 99;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateCameraSourceDialog::CCreateCameraSourceDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateCameraSourceDialog::CCreateCameraSourceDialog(
						 CMCETestUIEngineVideoStream& aStream, TInt aIndex ) :
    iStream(aStream),iSourceIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateCameraSourceDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateCameraSourceDialog::PreLayoutDynInitL()
    {
    CAknForm::PreLayoutDynInitL();
    const RPointerArray<CMCETestUIEngineVideoSource>& sources = 
            iStream.SourcesL();
   
   	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    CMceCameraSource& camSource = static_cast<CMceCameraSource&>(mediaSource);
    	
    CEikNumberEditor* cameraIndex = static_cast<CEikNumberEditor*>(
                                  Control( ECameraSourceDialogCameraIndex ) );
    cameraIndex->SetNumber( KMCETestUiUseDefaultCamera ); // Current value cannot be retrieved from MCE API
        
    TCameraInfo camerainfo;
    camSource.GetCameraInfo( camerainfo );
    
    
    if ( camSource.ZoomFactorL() < camerainfo.iMaxZoom )
        {
        CEikNumberEditor* zoomFactor = static_cast<CEikNumberEditor*>(
                                  Control( ECameraSourceDialogCameraZoomFactor ) );
        zoomFactor->SetNumber(camSource.ZoomFactorL());
        
        //camera->SetZoomFactorL( camera->ZoomFactorL() + 1 );
        }
    else if ( camSource.DigitalZoomFactorL() < camerainfo.iMaxDigitalZoom )
        {
        CEikNumberEditor* digZoomFactor = static_cast<CEikNumberEditor*>(
                                  Control( ECameraSourceDialogCameraDigitalZoomFactor ) );
        digZoomFactor->SetNumber( camSource.DigitalZoomFactorL() );
        
        //camera->SetDigitalZoomFactorL( camera->DigitalZoomFactorL() + 1 );
        }   


    
/*    CEikNumberEditor* maxdigital = static_cast<CEikNumberEditor*>(
                                  Control( ECameraSourceDialogZoomMax ) );
    maxdigital->SetNumber( camera.iMaxDigitalZoom );*/
    /*
    CEikNumberEditor* brightness = static_cast<CEikNumberEditor*>(
                                  Control( ECameraSourceDialogCameraBrightness ) );
    brightness->SetNumber(camSource.BrightnessL());
    
    CAknPopupFieldText* popupFieldText1 = 
        static_cast <CAknPopupFieldText*> (Control(ECameraSourceDialogExposure));
    TInt exposureState = camSource.ExposureL();
    switch(exposureState) 
    	{
    	//	case CCamera::EExposureAuto:
    		case 0:
    			{
    			popupFieldText1->SetCurrentValueIndex(0);
    			break;
    			}
    	// 	case CCamera::EExposureNight:
    		case 1:
    			{
    			popupFieldText1->SetCurrentValueIndex(1);
    			break;
    			}
    		//case CCamera::EExposureBacklight:
    		case 2:
    			{
    			popupFieldText1->SetCurrentValueIndex(2);
    			break;
    			}
    		//case CCamera::EExposureCenter:
    		case 3:
    			{
    			popupFieldText1->SetCurrentValueIndex(3);
    			break;
    			}
    		default:
    			{
    			popupFieldText1->SetCurrentValueIndex(0);
    			break;
    			}
    	}
    CAknPopupFieldText* popupFieldText2 = 
        static_cast <CAknPopupFieldText*> (Control(ECameraSourceDialogCameraWhiteBalance));
    TInt wbState = camSource.WhiteBalanceL();
    switch(wbState) 
    	{
    		//case CCamera::EWBAuto:
    		case 0:
    			{
    			popupFieldText2->SetCurrentValueIndex(0);
    			break;
    			}
    		//case CCamera::EWBDaylight:
    		case 1:
    			{
    			popupFieldText2->SetCurrentValueIndex(1);
    			break;
    			}
    		//case CCamera::EWBCloudy:
    		case 2:
    			{
    			popupFieldText2->SetCurrentValueIndex(2);
    			break;
    			}
    		//case CCamera::EWBTungsten:
    		case 3:
    			{
    			popupFieldText2->SetCurrentValueIndex(3);
    			break;
    			}
    		//case CCamera::EWBFlash:
    		case 4:
    			{
    			popupFieldText2->SetCurrentValueIndex(4);
    			break;
    			}
    		default:
    			{
    			popupFieldText2->SetCurrentValueIndex(0);
    			break;
    			}
    	}*/
    }

// ----------------------------------------------------------------------------
// CCreateCameraSourceDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateCameraSourceDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineVideoSource>& sources = 
            iStream.SourcesL();
            
    if(sources[iSourceIndex]->Type() == KMceCameraSource)
    {
    	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    	CMceCameraSource& camSource = static_cast<CMceCameraSource&>(mediaSource);
    
	/*    CEikNumberEditor* cameraIndex = static_cast<CEikNumberEditor*>(
	                                  Control( ECameraSourceDialogCameraIndex ) );
	    camSource.SetCameraIndexL(cameraIndex->Number());*/
	    
	    CEikNumberEditor* zoomFactor = static_cast<CEikNumberEditor*>(
	                                  Control( ECameraSourceDialogCameraZoomFactor ) );
	         
        TCameraInfo camerainfo;
        camSource.GetCameraInfo( camerainfo );
        
        CEikNumberEditor* digZoomFactor = static_cast<CEikNumberEditor*>(
	                                  Control( ECameraSourceDialogCameraDigitalZoomFactor ) );
	                                  
        if ( zoomFactor->Number() < camerainfo.iMaxZoom )
            {
            camSource.SetZoomFactorL(zoomFactor->Number());
            }
        else if ( digZoomFactor->Number() < camerainfo.iMaxDigitalZoom )
            {
            camSource.SetDigitalZoomFactorL(digZoomFactor->Number());
            }   
   
	    CEikNumberEditor* cameraIndex = static_cast<CEikNumberEditor*>(
                                  Control( ECameraSourceDialogCameraIndex ) );  
	    if ( camSource.CamerasAvailable() > 1 && cameraIndex->Number() != KMCETestUiUseDefaultCamera )
	        {
	        // Index cannot be changed if camera is in use, pause/resume
	        //
	        TBool enabled( camSource.IsEnabled() );
	        if ( enabled )
	            {
	            camSource.DisableL();
	            }
	            
	        camSource.SetCameraIndexL( cameraIndex->Number() );
	        
	        if( enabled )
	            {
	            camSource.EnableL();
	            }
	        }
	        
	/*    CEikNumberEditor* contrast = static_cast<CEikNumberEditor*>(
	                                  Control( ECameraSourceDialogCameraContrast ) );
	    camSource.SetContrastL(contrast->Number());
	    
	    CEikNumberEditor* brightness = static_cast<CEikNumberEditor*>(
	                                  Control( ECameraSourceDialogCameraBrightness ) );
	    camSource.SetBrightnessL(brightness->Number());
	    
	    CAknPopupFieldText* popupFieldText1 = 
	        static_cast <CAknPopupFieldText*> (Control(ECameraSourceDialogExposure));
	     switch(popupFieldText1->CurrentValueIndex())	
	     	{
	     		case 0:
	     			{
	     		//	camSource.SetExposureL(CCamera::EExposureAuto);
	     				camSource.SetExposureL(0);
	     			break;
	     			}
	     		case 1:
	     			{
	     			//camSource.SetExposureL(CCamera::EExposureNight);
	     				camSource.SetExposureL(1);
	     			break;
	     			}
	     		case 2:
	     			{
	     			//camSource.SetExposureL(CCamera::EExposureBacklight);
	     			camSource.SetExposureL(2);
	     			break;
	     			}
	     		case 3:
	     			{
	     			//camSource.SetExposureL(CCamera::EExposureCenter);
	     			camSource.SetExposureL(3);
	     			break;
	     			}
	     		default:
	     			{
	     			//camSource.SetExposureL(CCamera::EExposureAuto);
	     			camSource.SetExposureL(0);
	     			break;
	     			}
	     	}
	    CAknPopupFieldText* popupFieldText2 = 
	        static_cast <CAknPopupFieldText*> (Control(ECameraSourceDialogCameraWhiteBalance));
	      switch(popupFieldText2->CurrentValueIndex())	
	     	{
	     		case 0:
	     			{
		//			camSource.SetWhiteBalanceL(CCamera::EWBAuto);
					camSource.SetWhiteBalanceL(0);
	     			break;
	     			}
	     		case 1:
	     			{
	     			//camSource.SetWhiteBalanceL(CCamera::EWBDaylight);
	     			camSource.SetWhiteBalanceL(1);
	     			break;
	     			}
	     		case 2:
	     			{
	     			//camSource.SetWhiteBalanceL(CCamera::EWBCloudy);
	     			camSource.SetWhiteBalanceL(2);
	     			break;
	     			}
	     		case 3:
	     			{
	     			//camSource.SetWhiteBalanceL(CCamera::EWBTungsten);
	     			camSource.SetWhiteBalanceL(3);
	     			break;
	     			}
	     		case 4:
	     			{
	     			camSource.SetWhiteBalanceL(4);
	     			//camSource.SetWhiteBalanceL(CCamera::EWBFlash);
	     			break;
	     			}
	     		default:
	     			{
	     			camSource.SetWhiteBalanceL(0);
	     			//camSource.SetWhiteBalanceL(CCamera::EWBAuto);
	     			break;
	     			}
	     	}*/
      }
    
        
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateCameraSourceDialog::~CCreateCameraSourceDialog)
// .
// ----------------------------------------------------------------------------
//
CCreateCameraSourceDialog::~CCreateCameraSourceDialog()
    {
    return;
    } 

// End of File
