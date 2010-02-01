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




#ifndef __UT_CMccRTCPRECEIVER_H__
#define __UT_CMccRTCPRECEIVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>
#include "rtpdef.h"
#include "rtpapi.h"

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CMccRtcpReceiver;
class CMccResourcePoolStub;
class CMccRtpManager;
class CMccTestEventHandler;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccRtcpReceiver
     : public CEUnitTestSuiteClass,
       public MAsyncEventHandler,
       public MRtpErrNotify
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRtcpReceiver* NewL();
        static UT_CMccRtcpReceiver* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccRtcpReceiver();

    private:    // Constructors and destructors

        UT_CMccRtcpReceiver();
        void ConstructL();

	public: 	// From observer interface
		
    public: // From MAsyncEventHandler
    
        TInt SendEventToClient( const TMMFEvent& aEvent );

    public: // From MRtpErrNotify
    
        void ErrorNotify( TInt aErrCode );

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
        
         void UT_CMccRtcpReceiver_SdesReceivedL();
        
        
         void UT_CMccRtcpReceiver_ByeReceivedL();
        
        
         void UT_CMccRtcpReceiver_AppReceivedL();
        
        
         void UT_CMccRtcpReceiver_SrReceivedL();
        
        
         void UT_CMccRtcpReceiver_RrReceivedL();
        

         void UT_CMccRtcpReceiver_NonRTPDataReceivedL();

    private:    // Data

        CRtpAPI* iRtpApi;
        CMccRtcpReceiver* iRtcpReceiver;
        TRtpSSRC iTmpSsrc;
        TInt iExpectedPackets;
        TUint32 iStream;
        CMccResourcePoolStub* iResources;
        CMccRtpManager* iRtpManager;
        CMccTestEventHandler* iHandler;
        TUint32 iMccSessionId;

		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMccRTCPRECEIVER_H__

// End of file
