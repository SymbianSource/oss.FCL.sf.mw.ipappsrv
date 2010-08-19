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




#ifndef __UT_CMccRTPDATASOURCE_H__
#define __UT_CMccRTPDATASOURCE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>
#include <mmf/server/mmfdatasink.h>
#include "rtpdef.h"
#include "rtpapi.h"

//  INTERNAL INCLUDES
#include "mccinternaldef.h"


//  FORWARD DECLARATIONS
class CMccRtpDataSource;
class CMccRtpKeepaliveContainer;
class CMccTestEventHandler;
class CMccRtpMediaClock;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccRtpDataSource
     : public CEUnitTestSuiteClass,
       public MDataSink
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccRtpDataSource* NewL();
        static UT_CMccRtpDataSource* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccRtpDataSource();

    private:    // Constructors and destructors

        UT_CMccRtpDataSource();
        void ConstructL();

    public: // From MDataSink
    
       	TFourCC SinkDataTypeCode( TMediaId /*aMediaId*/ ) 
       	    {
            return TFourCC( ' ','A','M','R' );
       	    }
		
        void EmptyBufferL( CMMFBuffer* /*aBuffer*/,
                           MDataSource* /*aSupplier*/,
                           TMediaId /*aMediaId*/ )
            {
            }

        void BufferFilledL( CMMFBuffer* /*aBuffer*/ )
            {
            }

        TBool CanCreateSinkBuffer()
            {
            return EFalse;
            }
    
        CMMFBuffer* CreateSinkBufferL( TMediaId /*aMediaId*/, TBool &/*aReference*/)
            {
            return NULL;
            }

    protected: // From MDataSink

        void ConstructSinkL( const TDesC8& /*aInitData*/)
            {
            }

    private:

         void SetupL();
        
         void Teardown();
         
         void InitializeL( TBool aCreateStream = ETrue, TBool aEnableRtcp = EFalse );
         
         void CreateStreamL();
        
         void UT_CMccRtpDataSource_NewSourceLL();
             
         void UT_CMccRtpDataSource_SetPayloadSizeL();
            
         void UT_CMccRtpDataSource_RtpPacketReceivedL();    
        
         void UT_CMccRtpDataSource_StartInactivityTimerLL();
        
         void UT_CMccRtpDataSource_StopInactivityTimerLL();
             
         void UT_CMccRtpDataSource_SendMediaSignallingLL();
           
         void UT_CMccRtpDataSource_SourceDataTypeCodeL();
          
         void UT_CMccRtpDataSource_SetSourceDataTypeCodeL();
   
         void UT_CMccRtpDataSource_BufferEmptiedLL();
          
         void UT_CMccRtpDataSource_CanCreateSourceBufferL();
        
         void UT_CMccRtpDataSource_CreateSourceBufferLL();
        
         void UT_CMccRtpDataSource_SourceThreadLogonL();
        
         void UT_CMccRtpDataSource_SourceThreadLogoffL();

         void UT_CMccRtpDataSource_SourcePrimeLL();
        
         void UT_CMccRtpDataSource_SourcePlayLL();

         void UT_CMccRtpDataSource_SourcePauseLL();
        
         void UT_CMccRtpDataSource_SourceStopLL();
        
         void UT_CMccRtpDataSource_FillBufferLL();

         void UT_CMccRtpDataSource_SendRTCPReceiverReportL();
        
         void UT_CMccRtpDataSource_SendRTCPSenderReportL();

         void UT_CMccRtpDataSource_SendRTCPDataLL();

         void UT_CMccRtpDataSource_NegotiateSourceLL();
        
         void UT_CMccRtpDataSource_ConstructSourceLL();


         void UT_CMccRtpDataSource_ValidatePacketL();

         void UT_CMccRtpDataSource_CreateStreamLL();
         
         void UT_CMccRtpDataSource_DoCreateSrtpStreamL();
         
         void UT_CMccRtpDataSource_TestSendEventL();
         
         void UT_CMccRtpDataSource_StandbyL();
         
         void UT_CMccRtpDataSource_SourceCustomCommandL();
         
         void UT_CMccRtpDataSource_TestBaseClassL();
         
         void UT_CMccRtpDataSource_DoStandbyDecisionL();
         
         void UT_CMccRtpDataSource_RegisterPayloadTypesLL();
         
         void UT_CMccRtpDataSource_SRTPMasterKeyStaleEvent1L();
         
         void UT_CMccRtpDataSource_SRTPMasterKeyStaleEvent2L();


    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
        TUint32 iSession;
        CRtpAPI* iRtpApi;
        CMccRtpDataSource* iSource;
        CMccRtpKeepaliveContainer* iRtpKeepaliveMechanism;
        CMccTestEventHandler* iEventHandler;
        CSRTPSession* iSecSession; 
		CMccRtpMediaClock* iRtpMediaClock;
		
    };

#endif      //  __UT_CMccRTPDATASOURCE_H__

// End of file
