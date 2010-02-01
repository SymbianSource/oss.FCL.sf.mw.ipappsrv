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



#ifndef __TMCETESTUIENGINECMDENABLESTREAM_H__
#define __TMCETESTUIENGINECMDENABLESTREAM_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"

// FORWARD DECLARATIONS
class CMCETestUIEngineVideoStream;
class CMCETestUIEngineAudioStream;

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdEnableStream: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdEnableStream( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineAudioStream& aAudioStream );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineAudioStream& iAudioStream;

	};

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdEnableVideoStream: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdEnableVideoStream( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoStream& aVideoStream );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineVideoStream& iVideoStream;

	};

#endif // __TMCETESTUIENGINECMDEnableStream_H__
