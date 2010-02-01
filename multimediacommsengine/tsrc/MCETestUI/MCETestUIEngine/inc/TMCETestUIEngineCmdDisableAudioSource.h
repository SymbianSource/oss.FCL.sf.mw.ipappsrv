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



#ifndef __TMCETESTUIENGINECMDDISABLEAUDIOSOURCE_H__
#define __TMCETESTUIENGINECMDDISABLEAUDIOSOURCE_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"

// FORWARD DECLARATIONS
class CMCETestUIEngineAudioSource;
class CMCETestUIEngineVideoSource;
class CMCETestUIEngineSource;

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDisableSource: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdDisableSource( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSource& aAudioSource );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
         CMCETestUIEngineSource& iAudioSource;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDisableVideoSource: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdDisableVideoSource( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoSource& aVideoSource );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
         CMCETestUIEngineVideoSource& iVideoSource;

	};
#endif // __TMCETESTUIENGINECMDDISABLESTREAM_H__
