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



#ifndef __TMCETESTUIENGINECMDACCEPTSESSION_H__
#define __TMCETESTUIENGINECMDACCEPTSESSION_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"
#include "CMCETestUIEngineInSession.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/** 
 *
 * Executing this command leads to 2XX response to incoming session. 
 */
class TMCETestUIEngineCmdAcceptSession: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdAcceptSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInSession& aInSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineInSession& iInSession;
        
	};
	
// CLASS DEFINITION
/** 
 *
 * Executing this command leads to 2XX response to incoming session. 
 */
class TMCETestUIEngineCmdRespondSession: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdRespondSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInSession& aInSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineInSession& iInSession;
        
	};

#endif // __TMCETESTUIENGINECMDACCEPTSESSION_H__
