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




#ifndef MCECAMERASOURCE_H
#define MCECAMERASOURCE_H

// INCLUDES
#include <e32std.h>
#include <ecam.h>
#include <mcemediasource.h>

// CONSTANTS
const TMceSourceType KMceCameraSource = 3;

//FORWARD DECLARATIONS
class CMceManager;
class CMceItcSender;


// CLASS DECLARATION

/**
* Class representing camera in MCE.
*
* CMceCameraSource can be used to record video from camera.
*
* Enable (continue) and Disable (pause) operations to it are considered
* to be local, so they are not signalled to remote terminal.
*
* In common situation camera source is combined with display sink which acts
* as a viewfinder for the camera.
* 
* @lib mceclient.lib
*/
class CMceCameraSource : public CMceMediaSource
	{

	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    * @param aManager reference to the manager
	    */
		IMPORT_C static CMceCameraSource* NewL( CMceManager& aManager );

	    /**
	    * Two-phased constructor.
	    * @param aManager reference to the manager
	    */
		IMPORT_C static CMceCameraSource* NewLC( CMceManager& aManager );
		
	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceCameraSource();

	public: // From CMceMediaSource

	    /**
	    * Enables the source explicitly.
	    */
		IMPORT_C void EnableL(); 

	    /**
	    * Disables the source explicitly.
	    */
		IMPORT_C void DisableL(); 

	public: // New functions
	
	    /**
	    * Gets the number of cameras on the device.
	    * @return number of available cameras 
	    */
	    IMPORT_C TInt CamerasAvailable() const;

        /**
        * Sets index of physical camera to be used.
        * @param aCameraIndex Index of the camera to be used; First camera has
        *        index 0 and number of available cameras can be retrieved by 
        *        calling CMceCameraSource::CamerasAvailable                                  
        */
        IMPORT_C void SetCameraIndexL( TInt aCameraIndex );
        
        /** 
    	* Gets information about the camera device.
    	* @param aInfo on return information about the camera device
    	*/
        IMPORT_C void GetCameraInfo( TCameraInfo& aInfo ) const;
        
        /** 
    	* Sets the zoom factor.
    	* This must be in the range of TCameraInfo::iMinZoom to 
    	* TCameraInfo::iMaxZoom inclusive.
    	* @param aZoomFactor required zoom factor
    	* @leave KErrArgument if the specified zoom factor is out of range
    	*/
    	IMPORT_C void SetZoomFactorL( TInt aZoomFactor );
    	
    	/** 
	    * Gets the currently set zoom factor.
        * @return current zoom factor
	    */
	    IMPORT_C TInt ZoomFactorL();
    	
        /** 
        * Sets the digital zoom factor.
        * This must be in the range of 0 to TCameraInfo::iMaxDigitalZoom 
        * inclusive.
        * @param aDigitalZoomFactor The required digital zoom factor
        * @leave KErrArgument if the zoom factor is out of range
        */
        IMPORT_C void SetDigitalZoomFactorL( TInt aDigitalZoomFactor );

        /** 
        * Gets the currently set digital zoom factor.
        * @return current digital zoom factor 
        */
        IMPORT_C TInt DigitalZoomFactorL();

        /**
        * Sets the contrast adjustment of the device.
        * @param aContrast required contrast value; This must be in the 
        *        range of -100 to +100 or CCamera::EContrastAuto.
        * @leave KErrNotSupported if contrast adjustment is not supported, 
        *        see TCameraInfo::TOptions
        * @leave KErrArgument if the specified contrast value is out of 
        *        range
        */
        IMPORT_C void SetContrastL( TInt aContrast );

        /** 
        * Gets the current contrast value.
        * @return current contrast value
        */
        IMPORT_C TInt ContrastL();

        /** 
        * Sets the brightness adjustment of the device.
        * No effect if this is not supported, see TCameraInfo::TOptions.
        * 
        * @param aBrightness required brightness adjustment; This must be 
        *        in the range of -100 to +100 or CCamera::EBrightnessAuto
        * @leave KErrNotSupported if brightness adjustment is not supported, 
        *        see TCameraInfo::TOptions
        * @leave KErrArgument if the brightness adjustment is out of range
        */
        IMPORT_C void SetBrightnessL( TInt aBrightness );

        /** 
        * Gets the currently set brightness adjustment value.
        * @return current brightness adjustment value
        */
        IMPORT_C TInt BrightnessL();

        /** 
        * Sets the exposure adjustment of the device.
        * @param aExposure required exposure adjustment
        * @leave KErrNotSupported if the specified exposure adjustment is
        *        not supported, see CameraInfo::iExposureModesSupported
        */
        IMPORT_C void SetExposureL( CCamera::TExposure aExposure );

        /** 
        * Gets the currently set exposure setting value.
        * @return current exposure setting value
        */
        IMPORT_C CCamera::TExposure ExposureL();

        /** 
        * Sets the white balance adjustment of the device.
        * @param aWhiteBalance required white balance adjustment
        * @leave KErrNotSupported if the specified white balance adjustment is 
        *        not supported, see TCameraInfo::iWhiteBalanceModesSupported
        */
        IMPORT_C void SetWhiteBalanceL( CCamera::TWhiteBalance aWhiteBalance );

        /** 
        * Gets the currently set white balance adjustment value.
        * @return current white balance adjustment value
        */
        IMPORT_C CCamera::TWhiteBalance WhiteBalanceL();

	public: // internal
	
	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	    void InitializeL( CMceManager* aManager );


    public:

	    /**
	    * Two-phased constructor.
	    */
		static CMceCameraSource* NewL();

	    /**
	    * Two-phased constructor.
	    */
		static CMceCameraSource* NewLC();

	private:
                           
        /**
	    * Handles ITC
	    */
        TInt GetITCValueL( TUint8 aITCFunction );
        
        /**
	    * Handles ITC
	    */
        TInt SetITCValueL( TUint8 aITCFunction, 
                           TInt aValue ); 
						   
        /**
        * Gets camera info
        */
        void GetITCCameraInfoL( CMceManager& aManager, 
                                TCameraInfo& aInfo ) const;
        
        TCameraInfo SetITCCameraIndexL( TInt aCameraIndex ) const;
						   

	private:

	    /**
	    * C++ constructor.
	    */
		CMceCameraSource();

	    /**
	    * Second-phase constructor.
	    * @param aManager Pointer to the manager
	    */
		void ConstructL( CMceManager* aManager );
    
    private: // Owned data
        
	private: // Not owned data

        //CMceManager* iManager;

    private: // Reserved for future use
    
        TAny* iReserved;       	
        
	//for testing

    MCE_UNIT_TEST_DEFS
	    	    
	};

#endif
