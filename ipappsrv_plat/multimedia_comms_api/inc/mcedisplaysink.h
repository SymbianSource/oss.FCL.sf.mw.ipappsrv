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




#ifndef MCEDISPLAYSINK_H
#define MCEDISPLAYSINK_H

// INCLUDES
#include <mcemediasink.h>
#include <e32std.h>  
#include <gdi.h>

// FORWARD DECLARATIONS
class CWindowGc;
class RWindow;
class CMceDisplaySink;
class CMceManager;
class CMceItcSender;

// DATA TYPES
const TMceSinkType KMceDisplaySink = 3;


// CLASS DECLARATION

/**
* Class representing display in MCE.
*
* CMceDisplaySink can be used to show video on screen or as a viewfinder
* of the camera.
*
* Enable and Disable operations to it are considered to be local, 
* so they are not signalled to remote terminal.
* 
* @lib mceclient.lib
*/
/*lint -e578 */

class CMceDisplaySink : public CMceMediaSink
	{
	
    public:  // Codec states
    
        enum TRotation
            {
            ENone, 
            EClockwise90Degree, 
            EClockwise180Degree,
            EClockwise270Degree
            };

	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    * @param aManager reference to the manager
	    */
		IMPORT_C static CMceDisplaySink* NewL( CMceManager& aManager );

	    /**
	    * Two-phased constructor.
	    * @param aManager reference to the manager
	    */
		IMPORT_C static CMceDisplaySink* NewLC( CMceManager& aManager );

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceDisplaySink();


	public: // From CMceMediaSink

	    /**
	    * Enables the sink explicitly.
	    */
		IMPORT_C void EnableL(); 

	    /**
	    * Disables the sink explicitly.
	    */
		IMPORT_C void DisableL(); 


    public: // New functions
    	
    	/**
	    * Gets the number of available displays on the device.
	    * @return number of available displays 
	    */
	    IMPORT_C TInt DisplaysAvailable() const;
	    
	    /**
	    * Sets index of physical display to be used.
	    * @param aDisplayIndex Index of the display to be used; number of 
	    *        available displays can be retrieved by calling 
	    *        CMceDisplaySink::DisplaysAvailable
	    * @leave KErrNotSupported if requested display cannot be used
	    */
	    IMPORT_C void SetDisplayIndexL( TInt aDisplayIndex ); 
	    	
		/**
		* Sets display resources.
		* @param aWindow handle to the display window of the user
		* @param aGc graphics context of the user
		*/
		IMPORT_C void SetDisplay( RWindow& aWindow, CWindowGc& aGc );
		
        /**
        * Sets display rectangle. Display data is drawn to the given area.
        * @param aRect display area
        */
		IMPORT_C void SetDisplayRectL( const TRect& aRect );
		
        /**
        * Gets current display rectagle. 
        * @return current display rectangle 
        */
		IMPORT_C const TRect DisplayRectL();
		
		/*
		* Sets display sink priority. Priority value 0 is the highest priority. 
		* Display sink with higher priority draws over less priority display
		* if display rectangles overlap.
		* @param aPriority priority value
		*/
		IMPORT_C void SetPriorityL( TUint aPriority );
		
		/*
		* Display sink's current priority.
		* @return priority value
		*/
		IMPORT_C TUint PriorityL();

		/*
		* Sets display sink rotation. 
		* @param aRotation rotation value
		*/
		IMPORT_C void SetRotationL( TRotation aRotation );
		
		/*
		* Display sink's current rotation.
		* @return rotation value
		*/
		IMPORT_C TRotation RotationL();

	public: // internal
	
	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	    void InitializeL( CMceManager* aManager );
	    
	    /**
	    * Called endpoint proxy must be created
	    * @return endpointProxy
	    */
        MMceEndPointProxy* CreateEndpointProxyL() const;

	    /**
	    * Should this endpoint use endpoint proxy
	    * @return ETrue, this endpoint uses endpoint proxy
	    */
        TBool UsesEndpointProxy() const;

	public: 

	    /**
	    * Two-phased constructor.
	    */
	    static CMceDisplaySink* NewL();

	    /**
	    * Two-phased constructor.
	    */
		static CMceDisplaySink* NewLC();

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
	    	    
	private:

	    /**
	     * C++ constructor.
	     */
		CMceDisplaySink();

	    /**
	    * second-phase constructor
	    */
		void ConstructL( CMceManager* aManager );
    
    private: // Owned data
        
    private: // Not owned data
        
        RWindow* iWindow; // Not owned      
        CWindowGc* iGc; // Not owned

    private: // Reserved for future use
    
        TAny* iReserved;         	

    //for testing

	MCE_UNIT_TEST_DEFS
    

	};

#endif 
