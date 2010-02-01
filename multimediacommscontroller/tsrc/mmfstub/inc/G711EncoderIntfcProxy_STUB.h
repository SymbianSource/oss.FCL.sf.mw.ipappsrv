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
* Description:    Interface proxy for G711 encoder.
*
*/




#ifndef G711ENCODERINTFCPROXY_STUB_H
#define G711ENCODERINTFCPROXY_STUB_H

// INCLUDES
#include <G711EncoderIntfc.h>
#include <mmf/common/mmfcontrollerframework.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CCustomInterfaceUtility;
class MCustomCommand;

// CLASS DECLARATION

/**
*  Proxy for G711 encoder interface.
*  This proxy translates the interface API calls
*  to messages and sends them to the message handler.
*
*  @lib G711EncoderIntfcProxy.lib
*  @since S60 3.0
*/
class CG711EncoderIntfcProxy_STUB: public CG711EncoderIntfc
	{
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
		IMPORT_C static CG711EncoderIntfcProxy_STUB* NewL( );
		
        /**
        * Destructor.
        */
		IMPORT_C virtual ~CG711EncoderIntfcProxy_STUB();

    public: // New functions
        
    public: // Functions from base classes

        /**
        * From CG711EncoderIntfc
        */
		IMPORT_C virtual TInt SetEncoderMode(TEncodeMode aEncodeMode);

        /**
        * From CG711EncoderIntfc
        */
		IMPORT_C virtual TInt SetVadMode(TBool aVadMode);

        /**
        * From CG711EncoderIntfc
        */
		IMPORT_C virtual TInt GetVadMode(TBool& aVadMode);

    protected:  // New functions
        
    protected:  // Functions from base classes
        
    private:

        /**
        * C++ default constructor.
        */
		CG711EncoderIntfcProxy_STUB();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

    public:     // Data
    
    protected:  // Data

    private:    // Data
         
    public:     // Friend classes
    protected:  // Friend classes
    private:    // Friend classes

	};

#endif      // G711ENCODERINTFCPROXY_STUB_H
            
// End of File
