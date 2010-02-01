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



#ifndef __TMCETESTUIENGINECMDDELETEEVENT_H__
#define __TMCETESTUIENGINECMDDELETEEVENT_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"

// FORWARD DECLARATIONS
class CMCETestUIEngineEvent;
class CMCETestUIEngineRefer;

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDeleteEvent: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdDeleteEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineEvent& aEvent);

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineEvent& iEvent;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDeleteRefer: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdDeleteRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineRefer& aRefer);

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineRefer& iRefer;

	};

#endif // __TMCETESTUIENGINECMDDELETESESSION_H__
