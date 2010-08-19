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




#ifndef __UT_CMccRTPINTERFACE_H__
#define __UT_CMccRTPINTERFACE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "rtpdef.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccRtpInterface;
class TMccEvent;
class CRtpAPI;
class MMccRtpInterface;
class CMccRtpKeepaliveContainer;
class CMccRtpMediaClock;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccRtpInterface
     : public CEUnitTestSuiteClass,
       public MRtpErrNotify
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRtpInterface* NewL();
        static UT_CMccRtpInterface* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccRtpInterface();

    private:    // Constructors and destructors

        UT_CMccRtpInterface();
        void ConstructL();

    public: // From MRtpErrNotify
        void ErrorNotify( TInt aErrCode );

	public:     // Helpers
	
	    static void CreateRtcpEvent( TMccEvent& aEvent );	

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccRtpInterface_ConfigureLL();
         
         void UT_CMccRtpInterface_SendRTCPReceiverReportL();
        
        
         void UT_CMccRtpInterface_SendRTCPSenderReportL();
        
        
         void UT_CMccRtpInterface_StartInactivityTimerLL();
        
        
         void UT_CMccRtpInterface_StopInactivityTimerLL();
        
        
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE;
		
        TUint32 iRtpSessionId;
        CRtpAPI* iRtpApi;
        MMccRtpInterface* iInterface;
        CMccRtpKeepaliveContainer* iRtpKeepaliveMechanism;
        CMccRtpMediaClock* iRtpMediaClock;
    };

#endif      //  __UT_CMccRTPINTERFACE_H__

// End of file
