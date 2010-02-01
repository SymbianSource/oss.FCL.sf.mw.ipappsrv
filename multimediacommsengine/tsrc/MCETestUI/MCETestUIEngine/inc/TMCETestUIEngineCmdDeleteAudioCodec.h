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



#ifndef __TMCETESTUIENGINECMDDELETEAUDIOCODEC_H__
#define __TMCETESTUIENGINECMDDELETEAUDIOCODEC_H__

// INCLUDES
#include "TMCETestUIEngineCmdBase.h"

// FORWARD DECLARATIONS
class CMCETestUIEngineAudioStream;

// CLASS DEFINITION
/**
 * 
 */
 
class TMCETestUIEngineCmdDeleteAudioCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdDeleteAudioCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineCodec& aCodec );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineCodec& iCodec;

	};
	
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDeleteVideoCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdDeleteVideoCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoCodec& aCodec );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineVideoCodec& iCodec;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdAddCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdAddCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineAudioStream& aStream );

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
class TMCETestUIEngineCmdEnableAudioCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdEnableAudioCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineCodec& aCodec );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineCodec& iCodec;

	};
	
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDisableAudioCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdDisableAudioCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineCodec& aCodec );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineCodec& iCodec;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdStandByAudioCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdStandByAudioCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineCodec& aCodec );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineCodec& iCodec;

	};


// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdEnableVideoCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdEnableVideoCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoCodec& aCodec );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineVideoCodec& iCodec;

	};
	
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdDisableVideoCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdDisableVideoCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoCodec& aCodec );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineVideoCodec& iCodec;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdStandByVideoCodec: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor.
		 *
		 * @param aEngine UI Engine
		 */
		TMCETestUIEngineCmdStandByVideoCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoCodec& aCodec );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
        
        CMCETestUIEngineVideoCodec& iCodec;

	};

#endif // __TMCETESTUIENGINECMDDELETESESSION_H__
