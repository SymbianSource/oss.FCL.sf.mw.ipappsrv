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




#ifndef __UT_CMccINTERFACE_H__
#define __UT_CMccINTERFACE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <mmccinterfacedef.h>
#include "mmccinterface.h"
#include "mmcccodecinformation.h"
#include "mmccevents.h"
#include "mmccevent.h"

//  FORWARD DECLARATIONS
class CMccInterface;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccInterface
     : public CEUnitTestSuiteClass,
       public MMccCtrlObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccInterface* NewL();
        static UT_CMccInterface* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccInterface();

    private:    // Constructors and destructors

        UT_CMccInterface();
        void ConstructL();
        
        void WaitForEvent( TUint32 aSessionId, TUint32 aLinkId,
                TUint32 aStreamId, TMccEventType aEventType );

	public: 	// From observer interface
		
		

    private:    // New methods

         //Interface
         void SetupL();
         void Teardown();
         
         //Interface, session
         void Setup2L();
         void Teardown2();
        
         void UT_CMccInterface_NewLL();
        
        
         void UT_CMccInterface_CloseL();
        
        
         void UT_CMccInterface_CreateSessionL();
        
        
         void UT_CMccInterface_CreateLinkL();
        
        
         void UT_CMccInterface_CloseSessionL();
        
        
         void UT_CMccInterface_CloseLinkL();
        
        
         void UT_CMccInterface_CreateStreamL();
        
        
         void UT_CMccInterface_PrepareStreamL();
        
        
         void UT_CMccInterface_StartStreamL();
        
         void UT_CMccInterface_StartStream2L();
         
         void UT_CMccInterface_PauseStreamL();
        
        
         void UT_CMccInterface_ResumeStreamL();
        
        
         void UT_CMccInterface_StopStreamL();
        
        
         void UT_CMccInterface_DeleteStreamL();
        
        
         void UT_CMccInterface_StartInactivityTimerL();
         
         void UT_CMccInterface_StartInactivityTimer2L();
        
         void UT_CMccInterface_StopInactivityTimerL();
        
        
         void UT_CMccInterface_SendRTCPReceiverReportL();
        
        
         void UT_CMccInterface_SendRTCPSenderReportL();
        
        
         void UT_CMccInterface_SendRTCPAnyDataL();
        
        
         void UT_CMccInterface_SetRemoteAddressL();
        
        
         void UT_CMccInterface_SetPriorityL();
        
        
         void UT_CMccInterface_VolumeL();
        
        
         void UT_CMccInterface_MaxVolumeL();
        
        
         void UT_CMccInterface_SetVolumeL();
        
        
         void UT_CMccInterface_BalanceL();
        
        
         void UT_CMccInterface_SetBalanceL();
        
        
         void UT_CMccInterface_GetGainL();
        
        
         void UT_CMccInterface_MaxGainL();
        
        
         void UT_CMccInterface_SetGainL();
        
        
         void UT_CMccInterface_GetCodecLL();
        
        
         void UT_CMccInterface_SetCodecL();
        
        
         void UT_CMccInterface_GetSupportedCodecsL();
        
        
         void UT_CMccInterface_GetCapabilitiesL();
        
        
         void UT_CMccInterface_SendMediaSignalLL();
        
        
         void UT_CMccInterface_VersionL();


         void UT_CMccInterface_TestEventsL();


         void UT_CMccInterface_GetSSRCL();


         void UT_CMccInterface_CreateFmtpFromCodecL();
         
         void UT_CMccInterface_TestCameraConfigsL();
         
         void UT_CMccInterface_TestAudioRoutingL();
         
         void UT_CMccInterface_TestFileConfigsL();
         
         void UT_CMccInterface_TestResourceAddAndRemovalL();
         
         void UT_CMccInterface_TestTranscodeFileL();
         
         void UT_CMccInterface_TestCancelTranscodeFileL();
         
         void UT_CMccInterface_AddBundleL();
         void UT_CMccInterface_RemoveBundle();
         void UT_CMccInterface_GetBundles();
         void UT_CMccInterface_GetSupportedBitrates();
         void UT_CMccInterface_SendRTCPReceiverReport();
         void UT_CMccInterface_SendRTCPSenderReport();
         void UT_CMccInterface_SendRTCPAnyData();
         void UT_CMccInterface_EnableViewFinder();
         void UT_CMccInterface_DisableViewFinder();
         void UT_CMccInterface_GetViewFinderSettings();
         void UT_CMccInterface_GetCameraParameter();
         void UT_CMccInterface_SetCameraParameter();
         void UT_CMccInterface_Balance();
         void UT_CMccInterface_SetBalance();
         
         void UT_CMccInterface_StreamsExists();
         void UT_CMccInterface_UpdateDataSink();
         void UT_CMccInterface_UpdateDataSource();
         void UT_CMccInterface_MediaQualityRequest();
         void UT_CMccInterface_CancelMediaQualityRequest();
         
    public: //MMccCtrlObserver, pure virtual functions 
        /**
        * Callback function to receive media signalling
        * @param aSignal Received media signal
        * @return void
        */
        void MccEventReceived( const TMccEvent& aEvent );
        
        /**
        * Callback function to receive stream started event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaStarted( TUint32 aSessionId,
                                    TUint32 aLinkId,     
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );
        
        /**
        * Callback function to receive stream stopped event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaStopped( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive stream paused event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaPaused( TUint32 aSessionId,
                                   TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive stream resumed event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaResumed( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive stream prepared event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaPrepared( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive stream inactivity event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaInactive( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );
        
        /**
        * Callback function to receive stream activity event.
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        * @param aStreamId Stream ID
        */
        void MccMediaActive( TUint32 aSessionId,
                                    TUint32 aLinkId,
                                    TUint32 aStreamId,
                                    TUint32 aSinkSourceId );

        /**
        * Callback function to receive error event.
        * @param aError Error code
        * @return None
        */
        void MccCtrlError( TInt aError );

        /**
        * Callback function to receive error event.
        * @param aError Error code
        * @param aSessionId Session ID
        * @param aLinkId Link ID
        */
        void MccCtrlError( TInt aError,
                                   TUint32 aSessionId,
                                   TUint32 aLinkId,
                                   TUint32 aStreamId,
                                   TUint32 aSinkSourceId );

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 

		CMccInterface* iInterface;
		TUint32 iSessionId;
        TUint32 iUpLinkId;
        TUint32 iDownLinkId;
		
		TMccNetSettings iNetsettings;
		
		TBool iTest;
		TBool iActive;

    };

#endif      //  __UT_CMccINTERFACE_H__

// End of file
