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




#ifndef __UT_CMCEMEDIAMANAGER_H__
#define __UT_CMCEMEDIAMANAGER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "MceMediaManagerTestHelper.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceMediaManager;
class CMccControllerStub;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceMediaManager : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceMediaManager* NewL();
        static UT_CMceMediaManager* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceMediaManager();

    private:    // Constructors and destructors

        UT_CMceMediaManager();
        void ConstructL();


    private:    // New methods

        void SetupL();
        void Setup_BasicL();
        void Teardown();

        /**
        * Basic
        */
        void UT_CMceMediaManager_BasicLL();

        /**
        * Tests
        *
        * CSdpDocument* EncodeL( CMceComSession& aSession );
        *
        */
        void UT_CMceMediaManager_EncodeLL();
        

        /**
        * Tests
        *
        * TMceSipWarningCode DecodeL( CMceComSession& aSession,
        *                             CSdpDocument& aSdpDocument );
        *
        */
        void UT_CMceMediaManager_DecodeLL();

        /**
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession );
        * (MO audio)
        *
        */
        void UT_CMceMediaManager_UpdateLL();

        /**
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession );
        * (MO live video)
        *
        */
        void UT_CMceMediaManager_UpdateL_1L();

        /**
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession );
        * (MT multimedia)
        *
        */
        void UT_CMceMediaManager_UpdateL_2L();
           
        /**
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession );
        * (session update with signalling )
        *
        */
        void UT_CMceMediaManager_UpdateL_3L();
        
        
        /*
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession )
        * (hold)
        *
        */
        void UT_CMceMediaManager_UpdateL_4L();

        /*
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession )
        * (hold, which fails )
        *
        */
        void UT_CMceMediaManager_UpdateL_4_1L();
         
        /**
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession )
        * (session update with no signalling )
        *
        */
        void UT_CMceMediaManager_UpdateL_5L();

        /**
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession )
        * (received session update )
        *
        */
        void UT_CMceMediaManager_UpdateL_6L();


        /**
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession )
        * (session received update with added and removed streams)
        *
        */
        void UT_CMceMediaManager_UpdateL_8L();
        
        /**
        * Tests
        *
        * TMceReturnStatus UpdateL( CMceComSession& aSession );
        * (MO POC audio)
        *
        */
        void UT_CMceMediaManager_UpdateL_9L();
        
        /**
        * Tests
        *
        * TMceReturnStatus ReserveL( CMceComSession& aSession, const TDesC8& aToken );
        *
        */
        void UT_CMceMediaManager_ReserveLL();

        /**
        * Tests
        *
        * TBool ReservationNeeded( CMceComSession& aSession );
        *
        */
        void UT_CMceMediaManager_ReservationNeededL();

        /**
        * Tests
        *
        * TBool NeedToNegotiate( CMceComSession& aSession );
        *
        */
        void UT_CMceMediaManager_NeedToNegotiateL();

        /**
        * Tests
        *
        * void CreateRejectedAnswer( CMceComSession& aSession );
        *
        */
        void UT_CMceMediaManager_CreateRejectedAnswerL();
        
        /**
        * Tests
        *
        * void PauseL( CMceComMediaSink& aSink );
        *
        */
        void UT_CMceMediaManager_PauseL_1L();
        
        /**
        * Tests
        *
        * void PauseL( CMceComMediaSource& aSource );
        *
        */
        void UT_CMceMediaManager_PauseL_2L();
        
        /**
        * Tests
        *
        * void ResumeL( CMceComMediaSink& aSink );
        *
        */
        void UT_CMceMediaManager_ResumeL_1L();
        
        /**
        * Tests
        *
        * void ResumeL( CMceComMediaSource& aSource );
        *
        */
        void UT_CMceMediaManager_ResumeL_2L();
        
        /**
        * Tests
        *
        * TInt CloseSession( CMceComSession& aSession );
        *
        */
        void UT_CMceMediaManager_CloseSessionL();
        
        /**
        * Tests
        *
        * TInt CloseSession( CMceComSession& aSession )
        * (merged)
        *
        */
        void UT_CMceMediaManager_CloseSession_1L();
        
        /**
        * Tests
        *
        * void StartInactivityTimerL( CMceComRtpSource& aRtpSource );
        *
        */
        void UT_CMceMediaManager_StartInactivityTimerLL();

        /**
        * Tests
        *
        * void StopInactivityTimerL( CMceComRtpSource& aRtpSource );
        *
        */
        void UT_CMceMediaManager_StopInactivityTimerLL();
        
        /**
        * Tests
        *
        * void SendRTCPAnyDataL( CMceComMediaStream& aMediaStream, const TDesC8& aData );
        *
        */
        void UT_CMceMediaManager_SendRTCPAnyDataLL();

        /**
        * Tests
        *
        * void SendRTCPReceiverReportL( CMceComMediaStream& aMediaStream );
        *
        */
        void UT_CMceMediaManager_SendRTCPReceiverReportLL();

        /**
        * Tests
        *
        * void SendRTCPSenderReportL( CMceComMediaStream& aMediaStream );
        *
        */
        void UT_CMceMediaManager_SendRTCPSenderReportLL();

        void UT_CMceMediaManager_TranscodeFileLL();
        void UT_CMceMediaManager_TranscodeFileLWithCodecL();
        void UT_CMceMediaManager_TranscodingSessionL();
        void UT_CMceMediaManager_TranscodingSession_2L();
        void UT_CMceMediaManager_CancelTranscodeFileLL();
        void UT_CMceMediaManager_ConfirmationNeededL();
        
        void UT_CMceMediaManager_SessionL();
        
        void UT_CMceMediaManager_InitializeMccStreamLL();
        void UT_CMceMediaManager_CreateMccLinkLL();
        void UT_CMceMediaManager_CreateMccStreamLL();
        void UT_CMceMediaManager_PrepareMccStreamLL();
        void UT_CMceMediaManager_StartMccStreamLL();
        void UT_CMceMediaManager_AdjustMccStreamLL();
        void UT_CMceMediaManager_ValidateMccStreamLL();
        void UT_CMceMediaManager_StopMccStreamL();
        void UT_CMceMediaManager_SynchronizeMccStreamLL();
        
        void UT_CMceMediaManager_CleanupLL();
        void UT_CMceMediaManager_DisposeMediaStreamLL();
        void UT_CMceMediaManager_DisposeCodecLL();
        void UT_CMceMediaManager_RequireSignallingLL();
        void UT_CMceMediaManager_CleanupSessionLL();
        
        /**
        * Tests
        *
        * TInt GetSupportedCodecs( RArray<TFourCC>& aCodecs );
        *
        */
        void UT_CMceMediaManager_GetSupportedCodecsL();
                                
        /**
        * Tests
        *
        * void GetCapabilitiesL( RPointerArray<CMceComAudioCodec>& aCapabilities );
        *
        */
        void UT_CMceMediaManager_GetCapabilitiesL_1L();

        /**
        * Tests
        *
        * void GetCapabilitiesL( RPointerArray<CMceComVideoCodec>& aCapabilities );
        *
        */
        void UT_CMceMediaManager_GetCapabilitiesL_2L();


        /**
        * Tests
        *
        * void MccEventReceived( const TMccEvent& aEvent );
        *
        */
        void UT_CMceMediaManager_MccEventReceivedL();
        

        /**
        * Tests
        *
        * void MccMediaPrepared( TUint32 aSessionId,
        *                        TUint32 aLinkId,
        *                        TUint32 aStreamId,
        *                        TUint32 aSinkSourceId );
        *
        */
        void UT_CMceMediaManager_MccMediaPreparedL();
                                       
        /**
        * Tests
        *
        * void MccMediaStarted( TUint32 aSessionId,
        *                        TUint32 aLinkId,     
        *                        TUint32 aStreamId,
        *                        TUint32 aSinkSourceId );
        *
        */
        void UT_CMceMediaManager_MccMediaStartedL();
        
        /**
        * Tests
        *
        * void MccMediaStopped( TUint32 aSessionId,
        *                       TUint32 aLinkId,
        *                       TUint32 aStreamId,
        *                       TUint32 aSinkSourceId );
        *
        */
        void UT_CMceMediaManager_MccMediaStoppedL();
        
        /**
        * Tests
        *
        * void MccMediaPaused( TUint32 aSessionId,
        *                      TUint32 aLinkId,
        *                      TUint32 aStreamId,
        *                      TUint32 aSinkSourceId );
        *
        */
        void UT_CMceMediaManager_MccMediaPausedL();

        /**
        * Tests
        *
        * void MccMediaResumed( TUint32 aSessionId,
        *                       TUint32 aLinkId,
        *                       TUint32 aStreamId,
        *                       TUint32 aSinkSourceId );
        *
        */
        void UT_CMceMediaManager_MccMediaResumedL();

        /**
        * Tests
        *
        * void MccMediaInactive( TUint32 aSessionId,
        *                        TUint32 aLinkId,
        *                        TUint32 aStreamId,
        *                        TUint32 aSinkSourceId );
        *
        */
        void UT_CMceMediaManager_MccMediaInactiveL();

        /**
        * Tests
        *
        * void MccMediaActive( TUint32 aSessionId,
        *                      TUint32 aLinkId,
        *                      TUint32 aStreamId,
        *                      TUint32 aSinkSourceId );
        *
        */
        void UT_CMceMediaManager_MccMediaActiveL();
        
        /**
        * Tests
        *
        * void MccCtrlError( TInt aError );
        *
        */
        void UT_CMceMediaManager_MccCtrlError_1L();

        /**
        * Tests
        *
        * void MccCtrlError( TInt aError,
        *                    TUint32 aSessionId,
        *                    TUint32 aLinkId,
        *                    TUint32 aStreamId,
        *                    TUint32 aSinkSourceId );
        *
        */
        void UT_CMceMediaManager_MccCtrlError_2L();

        void UT_CMceMediaManager_MccTranscodeEventReceivedL();
        void UT_CMceMediaManager_PrackUpdateLL();

        /**
        * Tests media attribute setting & getting
        */
        void UT_CMceMediaManager_GetVolumeLL();
        void UT_CMceMediaManager_GetMaxVolumeLL();
        void UT_CMceMediaManager_SetVolumeLL();
        void UT_CMceMediaManager_SetBalanceLL();
        void UT_CMceMediaManager_GetGainLL();
        void UT_CMceMediaManager_GetMaxGainLL();
        void UT_CMceMediaManager_SetGainLL();
        
        void UT_CMceMediaManager_FillMceFileInfoLL();
        void UT_CMceMediaManager_GetDisplayRotationLL();
        void UT_CMceMediaManager_GetFileInfoLL();
        void UT_CMceMediaManager_GetFileInfoL_2L();
        void UT_CMceMediaManager_SetDisplayRotationLL();
        void UT_CMceMediaManager_FastForwardLL();
        void UT_CMceMediaManager_FastRewindLL();
        void UT_CMceMediaManager_GetAudioRoutingLL();
        void UT_CMceMediaManager_GetBrightnessLL();
        void UT_CMceMediaManager_GetCameraInfoLL();
        void UT_CMceMediaManager_GetCamerasAvailableLL();
        void UT_CMceMediaManager_GetContrastLL();
        void UT_CMceMediaManager_GetDigitalZoomFactorLL();
        void UT_CMceMediaManager_GetDisplayPriorityLL();
        void UT_CMceMediaManager_GetDisplayRectLL();
        void UT_CMceMediaManager_GetDisplaysAvailableLL();
        void UT_CMceMediaManager_GetExposureLL();
        void UT_CMceMediaManager_GetFilePositionLL();
        void UT_CMceMediaManager_GetWhiteBalanceLL();
        void UT_CMceMediaManager_GetZoomFactorLL();
        void UT_CMceMediaManager_SetAudioRoutingLL();
        void UT_CMceMediaManager_SetBrightnessLL();
        void UT_CMceMediaManager_SetCameraIndexLL();
        void UT_CMceMediaManager_SetContrastLL();
        void UT_CMceMediaManager_SetDigitalZoomFactorLL();
        void UT_CMceMediaManager_SetDisplayIndexLL();
        void UT_CMceMediaManager_SetDisplayPriorityLL();
        void UT_CMceMediaManager_SetDisplayRectLL();
        void UT_CMceMediaManager_SetExposureLL();
        void UT_CMceMediaManager_SetFileInfoLL();
        void UT_CMceMediaManager_SetFilePositionLL();
        void UT_CMceMediaManager_SetWhiteBalanceLL();
        void UT_CMceMediaManager_SetZoomFactorLL();
        void UT_CMceMediaManager_SetCNameLL();
        void UT_CMceMediaManager_SetRemoteAddressLL();
        void UT_CMceMediaManager_UpdateConfigKeyLL();
        void UT_CMceMediaManager_ForceEnableSinkStateL();
        
        /**
        * Tests
        *
        * CMceSdpSession* SdpSession( CMceComSession& aSession )
        *
        */
        void UT_CMceMediaManager_SdpSessionL();
        
    private:    // helpers
        
        void Reset();
        
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 

		CMceMediaManager* iManager;
		CMceComSession* iSession; // received from the session
		CSdpDocument* iSdpDocument; // received from the session
		CMceServerStub* iServer;
		CMccControllerStub* iMcc;
		CMceComSession* iUpdate; // received from the session
		
    };

#endif      //  __UT_CMCEMEDIAMANAGER_H__

// End of file
