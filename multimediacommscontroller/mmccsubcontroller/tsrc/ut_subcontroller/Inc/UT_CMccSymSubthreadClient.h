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




#ifndef __UT_CMccSYMSUBTHREADCLIENT_H__
#define __UT_CMccSYMSUBTHREADCLIENT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/server/mmfdatasource.h>
#include <mmf/server/mmfdatasink.h>
#include <mmf/server/mmfaudiooutput.h>

//  INTERNAL INCLUDES
#include "MmccEvents.h"

//  FORWARD DECLARATIONS
class CMccSymSubthreadClient;
class CMccTestEventHandler;
class MDataSource;
class MDataSink;
class TMccSignal;
class CMccResourcePoolStub;
class CMccRtpMediaClock;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * This is a dummy implementation of MDataSource
 *
 */
NONSHARABLE_CLASS( UT_CMccSymSubthreadClientDataSource )
     : public MDataSource
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSymSubthreadClientDataSource* NewL();

        /**
         * Destructor
         */
        ~UT_CMccSymSubthreadClientDataSource();

    private:    // Constructors and destructors

        UT_CMccSymSubthreadClientDataSource();
        void ConstructL();

    public:     // from MDataSource

        TFourCC SourceDataTypeCode( TMediaId aMediaId );
            
        void FillBufferL( CMMFBuffer* /*aBuffer*/,
                          MDataSink* /*aConsumer*/,
                          TMediaId /*aMediaId*/ );
            
        void BufferEmptiedL( CMMFBuffer* /*aBuffer*/ );
            
        TBool CanCreateSourceBuffer();

        CMMFBuffer* CreateSourceBufferL( TMediaId /*aMediaId*/, TBool& /*aReference*/ );

        void ConstructSourceL( const TDesC8& /*aInitData*/ );

        TUid DataSourceType() const;
    };

/**
 *
 * This is a dummy implementation of MDataSink
 *
 */
NONSHARABLE_CLASS( UT_CMccSymSubthreadClientDataSink )
     : public MMMFAudioOutput
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSymSubthreadClientDataSink* NewL();

        /**
         * Destructor
         */
        ~UT_CMccSymSubthreadClientDataSink();

    private:    // Constructors and destructors

        UT_CMccSymSubthreadClientDataSink();
        void ConstructL();

    public:     // from MDataSink

        TFourCC SinkDataTypeCode( TMediaId aMediaId );
        
        void EmptyBufferL( CMMFBuffer *aBuffer, MDataSource *aSupplier, TMediaId aMediaId);
        
        void BufferFilledL( CMMFBuffer *aBuffer );
        
        TBool CanCreateSinkBuffer();
        
        CMMFBuffer *CreateSinkBufferL( TMediaId aMediaId, TBool &aReference );
        
        void ConstructSinkL( const TDesC8 &aInitData );

        TUid DataSinkType() const;

    public: // from MMMFAudioOutput
        CMMFDevSound& SoundDevice();
        
        TInt BytesPlayed();
        
        void HWEmptyBufferL(CMMFBuffer *aBuffer, MDataSource *aSupplier);

        void SetSinkPrioritySettings(const TMMFPrioritySettings &aPrioritySettings);
        
    private: // data
        CMMFDevSound* iDevSound;
        
    };


/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMccSymSubthreadClient )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccSymSubthreadClient* NewL();
        static UT_CMccSymSubthreadClient* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccSymSubthreadClient();

    private:    // Constructors and destructors

        UT_CMccSymSubthreadClient();
        void ConstructL();
        
             
    public: // Helpers
    
        static void CreateRtcpEvent( TMccEvent& aEvent, 
                                     TMccEventType aRtcpEventType = KMccRtcpReceived );

    private:    // New methods

         void SetupL();
         void Setup2L();
         
         void Teardown();
         void Teardown2();
        
         void UT_CMccSymSubthreadClient_HandleEventL();
   
         void UT_CMccSymSubthreadClient_DoMccEvent();
         
         void UT_CMccSymSubthreadClient_SendEventToClientL();
         
         void UT_CMccSymSubthreadClient_OpenLL();
        
        
         void UT_CMccSymSubthreadClient_SetBalanceLL();
        
        
         void UT_CMccSymSubthreadClient_GetBalanceLL();
        
        
         void UT_CMccSymSubthreadClient_PrepareLL();
         void UT_CMccSymSubthreadClient_PrepareL2L();
        
        
         void UT_CMccSymSubthreadClient_PlayLL();
         void UT_CMccSymSubthreadClient_PlayL2L();


         void UT_CMccSymSubthreadClient_StopLL();
         void UT_CMccSymSubthreadClient_StopL2L();
        
        
         void UT_CMccSymSubthreadClient_PauseLL();
         void UT_CMccSymSubthreadClient_PauseL2L();
        
        
         void UT_CMccSymSubthreadClient_ResumeLL();
         void UT_CMccSymSubthreadClient_ResumeL2L();
        
        
         void UT_CMccSymSubthreadClient_CloseLL();
        
        
         void UT_CMccSymSubthreadClient_CloseL_1L();
                
         void UT_CMccSymSubthreadClient_SetVolumeLL();
        
        
         void UT_CMccSymSubthreadClient_SetGainLL();
        
        
         void UT_CMccSymSubthreadClient_MaxVolumeLL();
        
        
         void UT_CMccSymSubthreadClient_MaxGainLL();
        
        
         void UT_CMccSymSubthreadClient_GetVolumeLL();
        
        
         void UT_CMccSymSubthreadClient_GetGainLL();
        
        
         void UT_CMccSymSubthreadClient_SendMediaSignalLL();
         void UT_CMccSymSubthreadClient_SendMediaSignalL2L();
        
         void UT_CMccSymSubthreadClient_SendRTCPReceiverReportLL();
        
        
         void UT_CMccSymSubthreadClient_SendRTCPSenderReportLL();
        
        
         void UT_CMccSymSubthreadClient_SendRTCPDataLL();
        
        
         void UT_CMccSymSubthreadClient_StartInactivityTimerLL();
        
        
         void UT_CMccSymSubthreadClient_StopInactivityTimerLL();
        
        
         void UT_CMccSymSubthreadClient_GetSupportedBitratesLL();
        
        
         void UT_CMccSymSubthreadClient_GetSSRCLL();
        
        
         void UT_CMccSymSubthreadClient_SetUplinkIdL();
        
        
         void UT_CMccSymSubthreadClient_SetDownlinkIdL();
        
        
         void UT_CMccSymSubthreadClient_GetUplinkIdL();
        
        
         void UT_CMccSymSubthreadClient_GetDownlinkIdL();
        
        
         void UT_CMccSymSubthreadClient_DownlinkCreatedL();
        
        
         void UT_CMccSymSubthreadClient_UplinkCreatedL();
        
        
         void UT_CMccSymSubthreadClient_SetDownlinkCreatedL();
        
        
         void UT_CMccSymSubthreadClient_SetUplinkCreatedL();
        
        
         void UT_CMccSymSubthreadClient_GetFmtpAttrLL();
        
        
         void UT_CMccSymSubthreadClient_GetCodecLL();


         void UT_CMccSubThreadClient_UnuseL();

         void UT_CMccSubThreadClient_GetLocalIpAddressesL();

         void UT_CMccSubThreadClient_BindContextIntoStreamL();
         
         void UT_CMccSubThreadClient_RemoveContextL();
         
         void UT_CMccSubThreadClientBase_GetCodecL2L();        


         void UT_CMccSubThreadClientBase_CreateSessionLL();
        
        
         void UT_CMccSubThreadClientBase_GetCodecLL();
        
        
         void UT_CMccSubThreadClientBase_SetPrioritySettingsLL();
        
        
         void UT_CMccSubThreadClientBase_SetLinkIdL();
        
        
         void UT_CMccSubThreadClientBase_GetLinkIdL();
        
        
         void UT_CMccSubThreadClientBase_SetRemoteAddressLL();
        

         void UT_CMccSubThreadClientBase_SetRemoteAddressL2L();

        
         void UT_CMccSubThreadClientBase_PrepareLL();
        
        
         void UT_CMccSubThreadClientBase_PlayLL();
        
        
         void UT_CMccSubThreadClientBase_StopLL();
        
         void UT_CMccSubThreadClient_SetGetParameterL();
         
         void UT_CMccSubThreadClientBase_PauseLL();
        
        
         void UT_CMccSubThreadClientBase_ResumeLL();
        
        
         void UT_CMccSubThreadClientBase_CloseLL();
        
        
         void UT_CMccSubThreadClientBase_CloseL_1L();
        
        
         void UT_CMccSubThreadClientBase_SendMediaSignalLL();
        
        
         void UT_CMccSubThreadClientBase_SetCodecInformationLL();
        
        
         void UT_CMccSubThreadClientBase_SendRTCPReceiverReportLL();
        
        
         void UT_CMccSubThreadClientBase_SendRTCPSenderReportLL();
        
        
         void UT_CMccSubThreadClientBase_SendRTCPDataLL();
        
        
         void UT_CMccSubThreadClientBase_StartInactivityTimerLL();
        
        
         void UT_CMccSubThreadClientBase_StopInactivityTimerLL();
        
        
         void UT_CMccSubThreadClientBase_GetSupportedBitratesLL();
        
        
         void UT_CMccSubThreadClientBase_GetSSRCLL();
        
        
         void UT_CMccSubThreadClientBase_GetFmtpAttrLL();
        
        
         void UT_CMccSubThreadClientBase_SetUplinkIdL();
        
        
         void UT_CMccSubThreadClientBase_SetDownlinkIdL();
        
        
         void UT_CMccSubThreadClientBase_GetUplinkIdL();
        
        
         void UT_CMccSubThreadClientBase_GetDownlinkIdL();
        
        
         void UT_CMccSubThreadClientBase_DownlinkCreatedL();
        
        
         void UT_CMccSubThreadClientBase_UplinkCreatedL();
        
        
         void UT_CMccSubThreadClientBase_SetDownlinkCreatedL();
        
        
         void UT_CMccSubThreadClientBase_SetUplinkCreatedL();
        
        
         void UT_CMccSubThreadClientBase_SetPrioritySettingsL_1L();
        
        
         void UT_CMccSubThreadClientBase_RestartEventMonitorL();
        
        
         void UT_CMccSubThreadClientBase_IsDtmfL();

         void UT_CMccSubThreadClientBase_DataSink();

         void UT_CMccSubThreadClientBase_DataSource();

         void UT_CMccSubThreadClientBase_CreateEventReceiverLL();
         
         
         void UT_CMccSubThreadClientBase_CancelEventReceiverL();
         
         void UT_CMccSubThreadClientBase_CreateRtpSessionL();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

    	CMccTestEventHandler* iHandler;

        CMccSymSubthreadClient* iClient;
        MDataSource* iSourceStub;
        MDataSink*   iSinkStub;
        TUint32 iStreamId;
        CMccRtpMediaClock* iRtpMediaClock;        
        CMccResourcePoolStub* iResources;
        TUint32 iMccSessionId;
    };

#endif      //  __UT_CMccSYMSUBTHREADCLIENT_H__

// End of file
