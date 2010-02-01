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





#ifndef MCCCAMERAHANDLER_H
#define MCCCAMERAHANDLER_H

//  INCLUDES

#include <e32base.h>
#include <ecam.h>
#include <gdi.h>
#include "mmccinterfacedef.h"
#include "mccexpirationhandler.h"

#ifdef MCC_CAMERAHANDLER_TEST
#include <f32file.h>
#include <fbs.h>
#endif

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

class CMccScreen;
class MMccResources;
class CFbsBitmap;

// CLASS DECLARATION

class MMccCameraHandlerObserver
    {
    public:
    
        virtual void CameraReady( TInt aError ) = 0;
    };
    
class MMccCameraHandler
    {
    public:
    
        /**
        * Gets camera handle.
        */
        virtual TUint32 Handle() = 0;
        
        /**
        * Checks whether camera is ready to be used (resources reserved and
        * power on).
        */
        virtual TBool IsCameraReady() = 0;
        
        /**
        * Monitor camera reservation completion.
        * @param aForceCameraReserve, reserves camera resource forcefully if ETrue
        */
        virtual TInt MonitorCameraReservation( MMccCameraHandlerObserver& aObserver, 
                                               TBool aForceCameraReserve ) = 0;
        
        /**
        * Stop monitoring
        * @param aForceCameraRelease, releases camera resource if it was reserved
        *   just by forcing it via MonitorCameraReservation.
        */
        virtual TInt StopCameraMonitoring( MMccCameraHandlerObserver& aObserver,
                                           TBool aForceCameraRelease ) = 0;
    };



/**
*  
*/
class CMccCameraHandler : 
    public CBase, 
    public MMccCameraHandler, 
    public MCameraObserver,
    public MCameraObserver2,
    public MMccExpirationHandler                       
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMccCameraHandler* NewL( MMccResources& aResources,
                                        TInt aCameraIndex = 0, 
                                        TInt aPriority = 0 );
          
        /**
        * Destructor.
        */
        virtual ~CMccCameraHandler();

    public: // New functions
    
        static TInt GetNumOfCamerasL();
        
        inline TInt CameraIndex() const
            {
            return iCameraIndex;
            }	
    	
    	void EnableViewFinderL( TMccVideoSinkSetting& aSetting );

    	void DisableViewFinderL();
    	
    	TBool IsViewFinderEnabled() const;
    	
    	void GetViewFinderSettingsL( TMccVideoSinkSetting& aSetting );

        void GetCamInfoL( TCameraInfo& aInfo, TBool aForceCameraCreation = EFalse );
            
        void GetValueL( TUint32 aParam, TInt& aVal );
        
        void SetValueL( TUint32 aParam, TInt aVal );
        
        void GetValueL( TUint32 aParam, TReal& aVal );
        
        void SetValueL( TUint32 aParam, TReal aVal );
        
        void ReserveResourcesL( TInt aCameraIndex, TBool aForceCameraCreation = EFalse );
        
        void ReleaseResources();

    public: // Functions from MMccCameraHandler
    
        TUint32 Handle();
        
        TBool IsCameraReady();
        
        TInt MonitorCameraReservation( MMccCameraHandlerObserver& aObserver, 
                                       TBool aForceCameraReserve );
        
        TInt StopCameraMonitoring( MMccCameraHandlerObserver& aObserver,
                                   TBool aForceCameraRelease );
                                       
    public: // Functions from MCameraObserver
    
		void ReserveComplete( TInt aError );

		void PowerOnComplete( TInt aError );

		void ViewFinderFrameReady( CFbsBitmap& aFrame );

		void ImageReady( CFbsBitmap* aBitmap, HBufC8* aData, TInt aError );

		void FrameBufferReady( MFrameBuffer* aFrameBuffer, TInt aError );
                                       
    public: // Functions from MCameraObserver2
    
        void HandleEvent( const TECAMEvent& aEvent );

	    void ViewFinderReady( MCameraBuffer& aCameraBuffer,TInt aError );

	    void ImageBufferReady( MCameraBuffer& aCameraBuffer,TInt aError );

	    void VideoBufferReady( MCameraBuffer& aCameraBuffer,TInt aError );
	
	public: // Functions from MMccExpirationHandler
	
	    void TimerExpiredL( TMccTimerId aTimerId, TAny* aTimerParam );
	    	
    private:
        
        void DoReserveComplete( TInt aError );

		void DoPowerOnComplete( TInt aError );
		
		void DoViewFinderFrameReady( CFbsBitmap& aFrame );
		
        void NotifyObservers( TInt aError );
        
        TBool SettingsChanged( const TMccVideoSinkSetting& aSetting ) const;
        
        void UpdateCameraSettingsL();
        
        CCamera* CreateCameraL();
        
        void DoReleaseScreen();
        
        void DoReleaseCamera();
              
        #ifdef MCC_CAMERAHANDLER_TEST
        
        void CreateDependencyFileL();
		void RemoveDependencyFile();
		
		#endif
		/* Re-Intiallize the internal member variables 
		 * if camera index changed.
		 */
        void ResetCameraHandler();
		   
    private:
        /**
        * C++ default constructor.
        */
        CMccCameraHandler( MMccResources& aResources,
                           TInt aCameraIndex, 
                           TInt aPriority );
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


    private: // Data
    
    	enum TCameraHandlerState
    		{
    		EInit = 0,
    		EReserving,
    		EReserved,
    		EPowered,
    		EFailed
    		};
            
        MMccResources& iResources;
	
		TCameraHandlerState iState;

        // Priority settings
        TInt iPriority;

        // index of camera device
        TInt iCameraIndex;

        // Camera handle shared by all components
		CCamera* iCamera;
		
		//Screen for local camera view
		CMccScreen* iScreen;
		
		TMccVideoSinkSetting iViewFinderSettings;
		
		TBool iViewFinderEnabled;
		
		RPointerArray<MMccCameraHandlerObserver> iObservers;
		
		TInt iZoom;
		TInt iDigitalZoom;
		TInt iContrast;
		TInt iBrightness;
		TInt iWhiteBalance;
		TInt iExposure;
		
		TMccTimerId iTimerId;
		
		TBool iForcedResourceReservation;
		
		#ifdef MCC_CAMERAHANDLER_TEST
		RFbsSession iFbsSession;
		RFs iFsSession;
		#endif
		        
    	#ifdef TEST_EUNIT
    	friend class UT_CMccCameraHandler;
    	friend class UT_CMccResourceItem;
    	friend class UT_CMccResourcePool;
    	#endif
    };

#endif
