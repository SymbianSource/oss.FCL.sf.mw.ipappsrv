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



#ifndef __TMCETESTUIENGINECMDUPDATESESSION_H__
#define __TMCETESTUIENGINECMDUPDATESESSION_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"

// FORWARD DECLARATIONS
class CMCETestUIEngineSession;

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdUpdateSession: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdUpdateSession( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;
		
		IMPORT_C void DeleteCodecs();

    private: // Data
        
        CMCETestUIEngineSession& iSession;
 

	};
	
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdUpdateSessionWithValues: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdUpdateSessionWithValues( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;
		
		IMPORT_C void DeleteCodecs();

    private: // Data
        
        CMCETestUIEngineSession& iSession;
 

	};	
	
	


// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdHoldCall: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdHoldCall( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSession& aSession );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		IMPORT_C void HandleLocalHoldingL( CMceMediaStream& aMediaStream,
                                                CMceMediaStream& aBoundStream );                                           
		IMPORT_C void DisableAudioL();
		IMPORT_C void DisableAudioStreamL( CMceMediaStream& aAudioStream );
		IMPORT_C void HandleLocalResumingL( CMceMediaStream& aMediaStream, CMceMediaStream& aBoundStream );
	    IMPORT_C void EnableAudioL( );
        IMPORT_C void EnableAudioStreamL( CMceMediaStream& aAudioStream );

		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;
		

    private: // Data
        
        CMCETestUIEngineSession& iSession;
 

	};
	
#endif // __TMCETESTUIENGINECMDUPDATESESSION_H__
