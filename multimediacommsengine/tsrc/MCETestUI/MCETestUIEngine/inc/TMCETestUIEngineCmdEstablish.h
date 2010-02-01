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



#ifndef __TMCETESTUIENGINECMDESTABLISH_H__
#define __TMCETESTUIENGINECMDESTABLISH_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"

// FORWARD DECLARATIONS


// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdEstablish: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdEstablish( 
		        CMCETestUIEngine& aEngine,
		        CMCETestUIEngineOutSession& aOutSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;
		
		IMPORT_C void DeleteCodecs();

    private: // Data
    
        CMCETestUIEngineOutSession& iOutSession;

	};
	
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdEstablishWithValues: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdEstablishWithValues( 
		        CMCETestUIEngine& aEngine,
		        CMCETestUIEngineOutSession& aOutSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;
		
		IMPORT_C void DeleteCodecs();

    private: // Data
    
        CMCETestUIEngineOutSession& iOutSession;

	};	

#endif // __TMCETESTUIENGINECMDESTABLISH_H__
