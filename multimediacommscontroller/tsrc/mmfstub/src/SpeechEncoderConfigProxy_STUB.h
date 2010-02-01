/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Interface proxy for speech encoder configuration.
*
*/




#ifndef CSPEECHENCODERCONFIGPROXY_STUB_H
#define CSPEECHENCODERCONFIGPROXY_STUB_H

// INCLUDES
#include <SpeechEncoderConfig.h>
#include <mmf/common/mmfcontrollerframework.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Proxy for speech encoder configuration.
*  This proxy translates the interface API calls
*  to messages and sends them to the message handler.
*
*  @lib SpeechEncoderConfigProxy.lib
*  @since S60 3.0
*/
class CSpeechEncoderConfigProxy_STUB: public CSpeechEncoderConfig
	{
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
		IMPORT_C static CSpeechEncoderConfigProxy_STUB* NewL();
		
        /**
        * Destructor.
        */
		IMPORT_C virtual ~CSpeechEncoderConfigProxy_STUB();

    public: // New functions
        
    public: // Functions from base classes

        /**
        * From CSpeechEncoderConfig
        */
		IMPORT_C virtual TInt GetSupportedBitrates(
		                                  RArray<TUint>& aSupportedBitrates);

        /**
        * From CSpeechEncoderConfig
        */
		IMPORT_C virtual TInt SetBitrate(TUint aBitrate);

        /**
        * From CSpeechEncoderConfig
        */
		IMPORT_C virtual TInt GetBitrate(TUint& aBitrate);

        /**
        * From CSpeechEncoderConfig
        */
		IMPORT_C virtual TInt SetVadMode(TBool aVadMode);

        /**
        * From CSpeechEncoderConfig
        */
		IMPORT_C virtual TInt GetVadMode(TBool& aVadMode);
        
    protected:  // New functions
    protected:  // Functions from base classes
        
    private:

        /**
        * C++ default constructor.
        */
		CSpeechEncoderConfigProxy_STUB();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();
        void GetSupportedBitratesL(RArray<TUint>& aSupportedBitrates);

    public:     // Data
    protected:  // Data

    private:    // Data
         
    public:     // Friend classes
    protected:  // Friend classes
    private:    // Friend classes

	};

#endif      // CSPEECHENCODERCONFIGPROXY_STUB_H
            
// End of File
