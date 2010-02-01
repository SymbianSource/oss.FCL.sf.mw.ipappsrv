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




#ifndef __UT_CMccQOSCONTROLLER_H__
#define __UT_CMccQOSCONTROLLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mccresourcepool.h"

//  FORWARD DECLARATIONS
class CMccQosController;
class CMMccRateAdaptationObserverStub;
class MMccRateAdaptationObserver;
class CMccRtpDataSink;
class CMccRtpDataSource;

//class CMMFDataBuffer;
//class CMccDlDataPath;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

#define TEST_EUNIT

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMccQosController )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccQosController* NewL();

        /**
         * Destructor
         */
        ~UT_CMccQosController();

    private:    // Constructors and destructors

        UT_CMccQosController();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

        void SetupL();
        
        void Teardown();
        
        void UT_CMccQosController_NewLL();
	 	void UT_CMccQosController_EventReceivedL();
	 	void UT_CMccQosController_ConstructLL();
	 	void UT_CMccQosController_CheckRateChangeResultL();
	 	void UT_CMCCQosController_GetSinkSourceL();
	 	void UT_CMccQosController_DoAdaptationCalculationsLL();
	 	void UT_CMccQosController_FillRateAdaptationEventLL();
	 	void UT_CMccQosController_ClearRateEventDataL();
	 	void UT_CMccQosController_StartTimerForAsyncL();
	 	void UT_CMccQosController_AsyncTimerExpiredL();
	 	void UT_CMccQosController_IcmpErrorHandlingL();
        
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMccQosController*  iQosController;
    //    CMccResourcePoolStub*   iResourcePool;
        CMMccRateAdaptationObserverStub* iObserver;
        CMccRtpDataSink* iRtpDataSinkStub;
        CMccRtpDataSource* iRtpDataSourceStub;
        
//        MMccRateAdaptationObserver* iObserver;
		

    };

#endif      //  __UT_CMccQOSCONTROLLER_H__

// End of file
