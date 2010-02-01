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



#ifndef __TMCETESTUIENGINECMDACCEPTEVENT_H__
#define __TMCETESTUIENGINECMDACCEPTEVENT_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"
#include "CMCETestUIEngineInEvent.h"
#include "CMCETestUIEngineInRefer.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/** 
 *
 * Executing this command leads to 2XX response to incoming session. 
 */
class TMCETestUIEngineCmdAcceptEvent: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdAcceptEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInEvent& aInEvent );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineInEvent& iInEvent;
        
	};

// CLASS DEFINITION
/** 
 *
 * Executing this command leads to 2XX response to incoming session. 
 */
class TMCETestUIEngineCmdAcceptRefer: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdAcceptRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInRefer& aInRefer );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineInRefer& iInRefer;
        
	};
	
// CLASS DEFINITION
/** 
 *
 * Executing this command leads to 2XX response to incoming session. 
 */
class TMCETestUIEngineCmdRespondRefer: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdRespondRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInRefer& aInRefer );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineInRefer& iInRefer;
        
	};
#endif // __TMCETESTUIENGINECMDACCEPTSESSION_H__
