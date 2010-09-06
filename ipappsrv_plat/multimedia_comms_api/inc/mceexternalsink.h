/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CMCEEXTERNALSINK_H
#define CMCEEXTERNALSINK_H

// INCLUDES
#include <e32std.h>  
#include <MceMediaSink.h>

// CONSTANTS
const TMceSinkType KMceExternalSink = 6;

// CLASS DECLARATION

/**
* Class representing a sink for a media stream that is implemented outside MCE.
* 
*  @lib mceclient.lib
*/
class CMceExternalSink: public CMceMediaSink
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMceExternalSink* NewL();

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMceExternalSink* NewLC();

        /**
        * Destructor.
        */
        IMPORT_C ~CMceExternalSink();

    public: // From CMceMediaSource

        /**
        * Sets the sink enabled. 
        */
        IMPORT_C void EnableL(); 

        /**
        * Sets the sink disabled.
        */
        IMPORT_C void DisableL();
	
	private:
		/**
	     * C++ default constructor.
	     */
		CMceExternalSink();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL();
		
        //for testing

        MCE_UNIT_TEST_DEFS 
    };

#endif // CMCEEXTERNALSINK_H
