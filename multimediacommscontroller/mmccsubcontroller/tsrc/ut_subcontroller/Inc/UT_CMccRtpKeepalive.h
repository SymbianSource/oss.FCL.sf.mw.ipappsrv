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




#ifndef __UT_CMccRtpKeepalive_H__
#define __UT_CMccRtpKeepalive_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>
#include "rtpdef.h"
#include "rtpapi.h"

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CRtpAPI;
class CMccTestEventHandler;
class MDataSink;
class MDataSource;
class CMccRtpKeepalive;
class CMccRtpMediaClock;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccRtpKeepalive : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRtpKeepalive* NewL();
        static UT_CMccRtpKeepalive* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccRtpKeepalive();

    private:    // Constructors and destructors

        UT_CMccRtpKeepalive();
        void ConstructL();

	public: 	// From observer interface

    private:    // New methods

         void SetupL();
         
         void Setup2L();
         
         void SetupAL();
            
         void SetupA2L();
         
         void Teardown();

         void UT_CMccRtpKeepalive_ContainerTestsL();
         
         void UT_CMccRtpKeepalive_StartKeepaliveLL();
         
         void UT_CMccRtpKeepalive_UpdateParamsLL();
         
         void UT_CMccRtpKeepalive_StopKeepaliveL();
         
         void UT_CMccRtpKeepalive_RemoteAddressSetL();
         
         void UT_CMccRtpKeepalive_RunLL();

    private:    // Data

        CRtpAPI* iRtpApi;
        CMccTestEventHandler* iEventHandler;
        MDataSink* iRtpSink;
        MDataSource* iRtpSource;
        CMccRtpKeepalive* iKeepaliveHandler;
        CMccRtpMediaClock* iRtpMediaClock;
        
        TBool iAlloc;

		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMccRtpKeepalive_H__

// End of file
