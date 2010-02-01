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



#ifndef __TMCETESTUIENGINECMDENABLEAUDIOSOURCE_H__
#define __TMCETESTUIENGINECMDENABLEAUDIOSOURCE_H__

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
class TMCETestUIEngineCmdEnableSource: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdEnableSource( 
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
class TMCETestUIEngineCmdEnableVideoSource: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdEnableVideoSource( 
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
	
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdEnableRtpSourceInActivityTimer: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdEnableRtpSourceInActivityTimer( 
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
class TMCETestUIEngineCmdSendRR: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdSendRR( 
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
class TMCETestUIEngineCmdAudioTranscode: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdAudioTranscode( 
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
class TMCETestUIEngineCmdAudioCancelTranscode: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdAudioCancelTranscode( 
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
class TMCETestUIEngineCmdVideoTranscode: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdVideoTranscode( 
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
	
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdVideoCancelTranscode: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdVideoCancelTranscode( 
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

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdStartDtmfToneL: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdStartDtmfToneL( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSource& aMediaSource );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineSource& iMediaSource;

	};

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdStopDtmfToneL: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdStopDtmfToneL( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSource& aMediaSource );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineSource& iMediaSource;

	};

// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdSendDtmfToneL: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdSendDtmfToneL( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSource& aMediaSource );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineSource& iMediaSource;

	};
// CLASS DEFINITION
/**
 * 
 */
class TMCETestUIEngineCmdSendDtmfToneSequenceL: public TMCETestUIEngineCmdBase
	{

	public:	// Constructors

		/**
		 * Constructor
		 */
		TMCETestUIEngineCmdSendDtmfToneSequenceL( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineSource& aMediaSource );

	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		IMPORT_C void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		IMPORT_C const TDesC& Caption() const;

    private: // Data
    
        CMCETestUIEngineSource& iMediaSource;

	};
			
#endif // __TMCETESTUIENGINECMDEnableStream_H__
