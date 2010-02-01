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



#ifndef __TMCETESTUIENGINECMDAnswerSession_H__
#define __TMCETESTUIENGINECMDAnswerSession_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"
#include "CMCETestUIEngineInSession.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/**
 *
 *
 */
class TMCETestUIEngineCmdAnswerSession: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdAnswerSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInSession& aInSession );
		            
	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineInSession& iInSession;

	};

#endif // __TMCETESTUIENGINECMDAnswerSession_H__
