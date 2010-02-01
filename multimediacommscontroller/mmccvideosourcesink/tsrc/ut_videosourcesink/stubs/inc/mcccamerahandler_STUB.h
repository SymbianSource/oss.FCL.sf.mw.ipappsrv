/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    
*
*/




#ifndef MCCCAMERAHANDLERSTUB_H
#define MCCCAMERAHANDLERSTUB_H

//  INCLUDES
#include <e32base.h>

#include "mcccamerahandler.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*
*/
class CMccCameraHandlerStub : public CBase, public MMccCameraHandler
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccCameraHandlerStub* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCameraHandlerStub();

    public: // New functions

    public: // Functions from MMccCameraHandler
    
        TUint32 Handle();
        
        TBool IsCameraReady();
        
        TInt MonitorCameraReservation( MMccCameraHandlerObserver& aObserver, 
                                       TBool aForceCameraReserve );
        
        TInt StopCameraMonitoring( MMccCameraHandlerObserver& aObserver, 
                                   TBool aForceCameraRelease );
        
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * C++ default constructor.
        */
        CMccCameraHandlerStub();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public:     // Data
    
        TBool iIsCameraReady;
        
        TBool iForce;
    
    protected:  // Data

    private:    // Data
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    private:
    	#ifdef TEST_EUNIT
			friend class UT_CMccVideoSourceImpl;	
   		#endif
    };

#endif      // MCCCAMERAHANDLERSTUB_H   
            
// End of File
