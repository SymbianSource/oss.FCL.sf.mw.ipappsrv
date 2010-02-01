/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CMccCameraHandler_H__
#define __UT_CMccCameraHandler_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

#include "mcccamerahandler.h"

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CMccResourcePool;


#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMccCameraHandler )
     : public CEUnitTestSuiteClass, public MMccCameraHandlerObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCameraHandler* NewL();
        static UT_CMccCameraHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCameraHandler();

    private:    // Constructors and destructors

        UT_CMccCameraHandler();
        void ConstructL();

    public: // From observer interface

         // From MMccCameraHandlerObserver
         void CameraReady( TInt aError );

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCameraHandler_GetNumOfCamerasL();
        
         void UT_CMccCameraHandler_EnableViewFinderL();
         
         void UT_CMccCameraHandler_DisableViewFinderL();
         
         void UT_CMccCameraHandler_GetCamInfoL();
            
         void UT_CMccCameraHandler_GetValueL();

         void UT_CMccCameraHandler_SetValueL();

         void UT_CMccCameraHandler_ReserveResourcesL();
         
         void UT_CMccCameraHandler_ReserveResources2L();
         
         void UT_CMccCameraHandler_ReleaseResources();
 
         void UT_CMccCameraHandler_MonitorCameraReservation();
         
         void UT_CMccCameraHandler_StopCameraMonitoring();
         
         void UT_CMccCameraHandler_IsViewFinderEnabled();
         
         void UT_CMccCameraHandler_TestCamera2L();

    private: // Helper methods
        
         void DoReserveL();
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        CMccCameraHandler* iCameraHandler;
        CMccResourcePool* iResources;
        TBool iCameraReadyNotified;

    };

#endif      //  __UT_CMccCameraHandler_H__

// End of file
