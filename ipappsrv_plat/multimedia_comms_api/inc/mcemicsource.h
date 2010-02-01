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




#ifndef CMCEMICSOURCE_H
#define CMCEMICSOURCE_H

// INCLUDES
#include <e32std.h>
#include <mcemediasource.h>
  
// DATA TYPES
const TMceSourceType KMceMicSource = 2;

// CLASS DECLARATION

/**
* Class representing microphone in MCE.
*
* CMceMicSource can be used to record audio from microphone.
*
* Enable (un-mute) and Disable (mute) operations to it are considered
* to be local, so they are not signalled to remote terminal.
* 
*  @lib mceclient.lib
*/
class CMceMicSource: public CMceMediaSource
	{
	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceMicSource* NewL();
		
	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceMicSource* NewLC();

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceMicSource();

	public: // Functions

	    /**
	    * Enables streaming explicitly (aka un-mute).
	    */
		IMPORT_C void EnableL();

	    /**
	    * Disables streaming explicitly (aka mute).
	    */
		IMPORT_C void DisableL();
		
        /**
        * Sets gain 
        * @param aGain, 
        */
		IMPORT_C void SetGainL( TInt aGain );
		
        /**
        * Gets current gain 
        * @return value of current volume, 
        */
		IMPORT_C TInt GainL() const;
		
        /**
        * Gets maximum gain
        * @return value of mamimum gain, 
        */
		IMPORT_C TInt MaxGainL() const;		


	private:

	    /**
	     * C++ default constructor.
	     */
		CMceMicSource();

	    /**
	     * second-phase constructor
	     */
		void ConstructL();

	private: // Data

		//for testing

	    MCE_UNIT_TEST_DEFS	
	};


#endif