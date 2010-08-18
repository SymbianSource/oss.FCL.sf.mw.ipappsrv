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




#ifndef __UT_CMccRTPDATASINK_H__
#define __UT_CMccRTPDATASINK_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>
#include "rtpdef.h"
#include "rtpapi.h"

//  INTERNAL INCLUDES
#include "mccinternaldef.h"
#include "mccinternalevents.h"
//  FORWARD DECLARATIONS
class CMccRtpDataSink;
class CMccRtpKeepaliveContainer;
class TMccInternalEvent;
class CMccRtpMediaClock;
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccRtpDataSink
     : public CEUnitTestSuiteClass,
       public MRtpErrNotify,
       public MMccEventHandler,
       public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRtpDataSink* NewL();
        static UT_CMccRtpDataSink* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccRtpDataSink();

    private:    // Constructors and destructors

        UT_CMccRtpDataSink();
        void ConstructL();

	public: 	// From observer interface
		
    public: // From MRtpErrNotify
        void ErrorNotify( TInt aErrCode );

    public: // From MMccEventHandler
        TInt SendMccEventToClient( TMccEvent& /*aEvent*/ )
            {
            return KErrNone;
            }

    	void StateChange( TInt /*aState*/, TUint32 /*aLink*/ ) {};

        void ErrorOccured( TInt /*aError*/, TUint32 /*aSession*/, TUint32 /*aLink*/,
	                       TUint32 /*aStreamId*/, TUint32 /*aEndpointId*/ ) {};

    public: // From MAsyncEventHandler
        TInt SendEventToClient( const TMMFEvent& /*aEvent*/ )
            {
            iEventType = KMccMasterKeyStaled;
            
            return KErrNone;
            }

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void InitializeStreamL();
        
         void UT_CMccRtpDataSink_MuteL();
         
         void UT_CMccRtpDataSink_SendMediaSignallingLL();
        
        
         void UT_CMccRtpDataSink_EmptyBufferLL();
        
        
         void UT_CMccRtpDataSink_SinkThreadLogonL();
        
        
         void UT_CMccRtpDataSink_SinkThreadLogoffL();
        
        
         void UT_CMccRtpDataSink_SinkDataTypeCodeL();
        
        
         void UT_CMccRtpDataSink_SetSinkDataTypeCodeL();
        
        
         void UT_CMccRtpDataSink_BufferFilledLL();
        
        
         void UT_CMccRtpDataSink_CanCreateSinkBufferL();
        
        
         void UT_CMccRtpDataSink_CreateSinkBufferLL();
        
        
         void UT_CMccRtpDataSink_SinkPrimeLL();
        
        
         void UT_CMccRtpDataSink_SinkPlayLL();
        
        
         void UT_CMccRtpDataSink_SinkPauseLL();
        
        
         void UT_CMccRtpDataSink_SinkStopLL();
        
        
         void UT_CMccRtpDataSink_EmptyBufferL_1L();
        
         void UT_CMccRtpDataSink_EmptyBufferL_2L();
         
         void UT_CMccRtpDataSink_SendRTCPReceiverReportL();
        
        
         void UT_CMccRtpDataSink_SendRTCPSenderReportL();


         void UT_CMccRtpDataSink_SendRTCPDataLL();


         void UT_CMccRtpDataSink_ConstructSinkLL();

		 void UT_CMccRtpDataSink_SRTPMasterKeyStaleEvent1L();
         
         void UT_CMccRtpDataSink_SRTPMasterKeyStaleEvent2L();


    // From CMccRtpInterface
        
         void UT_CMccRtpDataSink_AddRtpStackReferenceL();
        
        
         void UT_CMccRtpDataSink_RemoveRtpStackReferenceL();
        
        
         void UT_CMccRtpDataSink_PrepareLL();
        
        
         void UT_CMccRtpDataSink_CreateStreamLL();
        
        
         void UT_CMccRtpDataSink_RemoveStreamLL();
        
        
         void UT_CMccRtpDataSink_PlayStreamLL();
        
        
         void UT_CMccRtpDataSink_PauseStreamLL();
        
        
         void UT_CMccRtpDataSink_StopStreamLL();
        
        
         void UT_CMccRtpDataSink_StartInactivityTimerLL();


         void UT_CMccRtpDataSink_StopInactivityTimerLL();


         void UT_CMccRtpDataSink_SetSessionParamsLL();

		 


    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 

        TUint32 iSession;
        CRtpAPI* iRtpApi;
        CMccRtpDataSink* iSink;
        CMccRtpKeepaliveContainer* iRtpKeepaliveMechanism;
        CSRTPSession* iSecSession;
        TUint iEventType;
        CMccRtpMediaClock* iRtpMediaClock;
		
    };

#endif      //  __UT_CMccRTPDATASINK_H__

// End of file
