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



#ifndef __TMCETESTUIENGINECMDNOTIFYEVENT_H__
#define __TMCETESTUIENGINECMDNOTIFYEVENT_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"
#include "CMCETestUIEngineInEvent.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdNotifyEvent: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdNotifyEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineInEvent& aInEvent );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineInEvent& iInEvent;
        
	};

#endif // __TMCETESTUIENGINECMDREJECTSESSION_H__
