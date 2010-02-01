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



#ifndef __TMCETESTUIENGINECMDDISABLEAUDIOSINK_H__
#define __TMCETESTUIENGINECMDDISABLEAUDIOSINK_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"

// FORWARD DECLARATIONS
class CMCETestUIEngineSink;
class CMCETestUIEngineVideoSink;

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDisableSink: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdDisableSink( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSink& aAudioSink );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineSink& iAudioSink;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDisableVideoSink: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdDisableVideoSink( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoSink& aVideoSink );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineVideoSink& iVideoSink;

	};
#endif // __TMCETESTUIENGINECMDDISABLESTREAM_H__
