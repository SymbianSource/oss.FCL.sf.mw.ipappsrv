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




#ifndef MCEFILESINK_H
#define MCEFILESINK_H

// INCLUDES
#include <e32std.h>  
#include <mcemediasink.h>

// FORWARD DECLARATIONS

// DATA TYPES
const TMceSinkType KMceFileSink = 4;


// CLASS DECLARATION

/**
* Class representing file sink in Mce.
*
* CMceFileSink can be used to record a stream to the file. The file may consist of
* both audio and video data. If both media elements are available and should be
* recorded to the same file, the same file sink should be attached to audio stream
* and video stream. Recording only one of the media elements is possible as well.
*
* Enable (continue) and Disable (pause) operations to it are considered
* to be local, so they are not signalled to remote terminal.
* 
* @lib mceclient.lib
*/
class CMceFileSink : public CMceMediaSink
	{
	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    * @param aFileName name of the file including full path info
	    */
		IMPORT_C static CMceFileSink* NewL( const TFileName& aFileName );

	    /**
	    * Two-phased constructor.
	    * @param aFileName name of the file including full path info
	    */
		IMPORT_C static CMceFileSink* NewLC( const TFileName& aFileName );

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceFileSink();


	public: // From CMceMediaSink

	    /**
	    * Enables the sink explicitly.
	    */
		IMPORT_C void EnableL(); 

	    /**
	    * Disables the sink explicitly.
	    */
		IMPORT_C void DisableL(); 	    

	public: 

	    /**
	    * Two-phased constructor.
	    */
	    static CMceFileSink* NewL();

	    /**
	    * Two-phased constructor.
	    */
		static CMceFileSink* NewLC();


	public: // internal from CMceMediaSink

	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	    void InitializeL( CMceManager* aManager );
	    	    
	private:

	    /**
	    * C++ default constructor.
	    */
		CMceFileSink();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL( const TFileName& aFileName );
    
        void GetFileInfoL();

	};

#endif 
