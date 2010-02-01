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
* Description:    See class definition below.
*
*/



#ifndef __TMCETESTUIENGINECMDBASE_H__
#define __TMCETESTUIENGINECMDBASE_H__

// INCLUDES
#include "CMCETestUIEngine.h"
#include "MCETestUIEngineConstants.h"

// FORWARD DECLARATIONS

// CLASS DEFINITION
/**
 * TMCETestUIEngineCmdBase implements the base class for all 
 * MCETestUI commands. It provides common interface for resolving 
 * meaning of a command and to execute it. 
 */
class TMCETestUIEngineCmdBase
	{
	public:	// destructor

   	    /**
    	* Virtual destructor.
    	*/
		IMPORT_C virtual ~TMCETestUIEngineCmdBase();

	protected:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdBase( CMCETestUIEngine& aEngine );

	private: // Constructors

        /**
        * C++ default constructor. Not implemented.
        */
		TMCETestUIEngineCmdBase();

	public: // Abstract methods

        /**
        * Executes a sequence of operations.
        */
		virtual void ExecuteL() = 0;

        /**
        * @return short textual representation for behavior of command
        */		
		virtual const TDesC& Caption() const = 0;

	protected:	// Data

		/// Reference to the UI Engine. Not owned.
		CMCETestUIEngine& iEngine;

	};

#endif // __TMCETESTUIENGINECMDBASE_H__
