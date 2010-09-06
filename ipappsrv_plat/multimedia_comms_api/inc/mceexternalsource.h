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



#ifndef CMCEEXTERNALSOURCE_H
#define CMCEEXTERNALSOURCE_H

// INCLUDES
#include <e32std.h>
#include <mcemediasource.h>

// CONSTANTS
const TMceSourceType KMceExternalSource = 6;

// CLASS DECLARATION
/**
* Class representing a source for a media stream that is implemented outside MCE.
* 
*  @lib mceclient.lib
*/
class CMceExternalSource: public CMceMediaSource
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMceExternalSource* NewL();

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMceExternalSource* NewLC();

        /**
        * Destructor.
        */
        IMPORT_C ~CMceExternalSource();

    public: // From CMceMediaSource

        /**
        * Sets the source enabled.
        */
        IMPORT_C void EnableL(); 

        /**
        * Sets the source disabled.
        */
        IMPORT_C void DisableL(); 
	
	private: //methods

	    /**
	     * C++ default constructor.
	     */
		CMceExternalSource();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL();
		
        //for testing

        MCE_UNIT_TEST_DEFS 
    };


#endif // CMCEEXTERNALSOURCE_H
