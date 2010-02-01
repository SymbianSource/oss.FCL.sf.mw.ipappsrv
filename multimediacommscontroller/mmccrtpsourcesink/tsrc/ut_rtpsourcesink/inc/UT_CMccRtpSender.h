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




#ifndef __UT_CMccRtpSender_H__
#define __UT_CMccRtpSender_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

#include "mccrtpsender.h"
#include "rtpdef.h"
#include "rtpapi.h"

//  FORWARD DECLARATIONS

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccRtpSender : 
    public CEUnitTestSuiteClass, 
    public MRtpErrNotify, 
    public MMccRtpSenderObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRtpSender* NewL();
        static UT_CMccRtpSender* NewLC();

        /**
         * Destructor
         */
        ~UT_CMccRtpSender();

    private:    // Constructors and destructors

        UT_CMccRtpSender();
        void ConstructL();
        
    protected: // From MRtpErrNotify
    
        void ErrorNotify( TInt aErrCode );
        
    protected: // From MMccRtpSenderObserver
    
        void SendErrorOccured( TInt aError );
   
    private:    // New methods

         void SetupL();
        
         void Teardown();        
        
         void UT_CMccRtpSender_SendRtpPacketTest();
         
         void UT_CMccRtpSender_SendDataLL();

         void UT_CMccRtpSender_ClearTest();   

         void UT_CMccRtpSender_SendingTimeoutTest();
        
         void UT_CMccRtpSender_MarkerBasedClearingTest();
         
         void UT_CMccRtpSender_MarkerBasedClearingTest2();

    private:    // Data

        CRtpAPI* iRtpApi;
        CMccRtpSender* iSender;
        TInt iLastError;
        
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMccRtpSender_H__

// End of file
