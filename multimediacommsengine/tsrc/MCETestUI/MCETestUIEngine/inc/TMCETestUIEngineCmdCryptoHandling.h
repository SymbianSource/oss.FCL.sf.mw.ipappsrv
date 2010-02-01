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



#ifndef __TMCETESTUIENGINECMDCRYPTOHANDLING_H__
#define __TMCETESTUIENGINECMDCRYPTOHANDLING_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"


// FORWARD DECLARATIONS
class CMCETestUIEngineSession;

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdCurrentCryptoContext: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCurrentCryptoContext( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdCurrentCryptoContext( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineSession& iSession;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdSupportedCryptoContext: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdSupportedCryptoContext( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdSupportedCryptoContext( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineSession& iSession;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdSetCryptoContext: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdSetCryptoContext( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdSetCryptoContext( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineSession& iSession;

	};




#endif // __TMCETESTUIENGINECMDCREATESESSION_H__
