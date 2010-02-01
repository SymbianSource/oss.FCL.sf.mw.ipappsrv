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



#ifndef __TMCETESTUIENGINECMDCREATEEVENT_H__
#define __TMCETESTUIENGINECMDCREATEEVENT_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"

// FORWARD DECLARATIONS
class CMCETestUIEngineProfile;

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdCreateEvent: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCreateEvent( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdCreateEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineProfile& aProfile );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineProfile* iProfile;

	};

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdCreateRefer: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCreateRefer( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdCreateRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineProfile& aProfile );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineProfile* iProfile;

	};
	
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdCreateReferSession: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCreateReferSession( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdCreateReferSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineSession* iSession;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdCreateSessionFromRefer: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCreateSessionFromRefer( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdCreateSessionFromRefer( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineRefer& aRefer );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineRefer* iRefer;

	};
	

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdCreateReferEvent: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCreateReferEvent( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdCreateReferEvent( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineEvent& aEvent );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Not owned data
    
        CMCETestUIEngineEvent* iEvent;

	};

#endif // __TMCETESTUIENGINECMDCREATESESSION_H__
