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



#ifndef __TMCETESTUIENGINECMDCREATESESSION_H__
#define __TMCETESTUIENGINECMDCREATESESSION_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"
#include<ES_SOCK.h>
#include <CommDbConnPref.h>

// FORWARD DECLARATIONS
class CMCETestUIEngineProfile;

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdCreateSession: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCreateSession( CMCETestUIEngine& aEngine );

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 * @param aProfile Profile to be used to create new session
		 */
		TMCETestUIEngineCmdCreateSession( 
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
class TMCETestUIEngineCmdCreateSessionUsingAPList: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCreateSessionUsingAPList( CMCETestUIEngine& aEngine );

	

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;
		
	//	TInt PrepareConnection( TCommDbConnPref& aPrefs, TInt aIapId );
		
	
    private: // Not owned data
    
      
	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdCreateSessionUsingAPNumber: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdCreateSessionUsingAPNumber( CMCETestUIEngine& aEngine );

	

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;
	
	

    private: // Not owned data
    
       

	};
#endif // __TMCETESTUIENGINECMDCREATESESSION_H__
