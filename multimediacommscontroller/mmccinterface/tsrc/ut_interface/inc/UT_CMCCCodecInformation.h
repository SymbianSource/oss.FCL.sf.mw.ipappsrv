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




#ifndef __UT_CMccCODECINFORMATION_H__
#define __UT_CMccCODECINFORMATION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mmccinterfacedef.h"
#include "mmccinterface.h"
#include "mmcccodecinformation.h"
#include "mmccevents.h"


//  FORWARD DECLARATIONS
class CMccCodecInformation;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMccCodecInformation
     : public CEUnitTestSuiteClass,
       public MMccCtrlObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecInformation* NewL();
        static UT_CMccCodecInformation* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecInformation();

    private:    // Constructors and destructors

        UT_CMccCodecInformation();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CMccCodecInformation_CloneDefaultsLL();
        
        
         void UT_CMccCodecInformation_CloneDetailedLL();
        
        
         void UT_CMccCodecInformation_RequireSignallingL();
        
        
         void UT_CMccCodecInformation_SetValuesL();
        
        
         void UT_CMccCodecInformation_GetValuesL();
        
        
         void UT_CMccCodecInformation_TypeL();
        
        
         void UT_CMccCodecInformation_EnableVADL();
        
        
         void UT_CMccCodecInformation_SetBitrateL();
        
        
         void UT_CMccCodecInformation_SetSamplingFreqL();
        
        
         void UT_CMccCodecInformation_SetSdpNameL();
        
        
         void UT_CMccCodecInformation_SetPayloadTypeL();
        
        
         void UT_CMccCodecInformation_SetCodecModeL();
        
        
         void UT_CMccCodecInformation_SetAllowedBitratesL();
        
        
         void UT_CMccCodecInformation_SetPTimeL();
        
        
         void UT_CMccCodecInformation_SetMaxPTimeL();
        
        
         void UT_CMccCodecInformation_SetJitterBufBufferLengthL();
        
        
         void UT_CMccCodecInformation_SetJitterBufThresholdL();
        
        
         void UT_CMccCodecInformation_SetJitterBufInactivityTimeOutL();
        
        
         void UT_CMccCodecInformation_SetMaxBitrateL();
        
        
         void UT_CMccCodecInformation_SetAverageBitrateL();
        
        
         void UT_CMccCodecInformation_SetFramerateL();
        
        
         void UT_CMccCodecInformation_SetFrameHeightL();
        
        
         void UT_CMccCodecInformation_SetFrameWidthL();
        
        
         void UT_CMccCodecInformation_SetAlgoL();
        
        
         void UT_CMccCodecInformation_SetRedCountL();
        
        
         void UT_CMccCodecInformation_SetRedundancyPTL();
        
        
         void UT_CMccCodecInformation_SetPriorityL();
        
        
         void UT_CMccCodecInformation_SetPriorityPreferenceL();
        
        
         void UT_CMccCodecInformation_SetKeepAliveTimerL();
         
         
         void UT_CMccCodecInformation_SetKeepAliveTimerMicroSecsL();
        
        
         void UT_CMccCodecInformation_SetKeepAlivePTL();
        
        
         void UT_CMccCodecInformation_SetKeepAliveDataL();
        
        
         void UT_CMccCodecInformation_SetComfortNoiseGenerationL();
        
        
         void UT_CMccCodecInformation_SetChannelsL();
        
        
         void UT_CMccCodecInformation_SetMaxRedL();
        
        
         void UT_CMccCodecInformation_VADL();
        
        
         void UT_CMccCodecInformation_BitrateL();
        
        
         void UT_CMccCodecInformation_AllowedBitratesL();
        
        
         void UT_CMccCodecInformation_SamplingFreqL();
        
        
         void UT_CMccCodecInformation_SdpNameL();
        
        
         void UT_CMccCodecInformation_PayloadTypeL();
        
        
         void UT_CMccCodecInformation_CodecModeL();
        
        
         void UT_CMccCodecInformation_PTimeL();
        
        
         void UT_CMccCodecInformation_MaxPTimeL();
        
        
         void UT_CMccCodecInformation_GetFmtpLL();
        
        
         void UT_CMccCodecInformation_FourCCL();
        
        
         void UT_CMccCodecInformation_FrameTimeL();
        
        
         void UT_CMccCodecInformation_FrameSizeL();
        
        
         void UT_CMccCodecInformation_AlgoL();
        
        
         void UT_CMccCodecInformation_RedCountL();
        
        
         void UT_CMccCodecInformation_RedundancyPTL();
        
        
         void UT_CMccCodecInformation_JitterBufInactivityTimeOutL();
        
        
         void UT_CMccCodecInformation_JitterBufThresholdL();
        
        
         void UT_CMccCodecInformation_JitterBufBufferLengthL();
        
        
         void UT_CMccCodecInformation_MaxBitrateL();
        
        
         void UT_CMccCodecInformation_AverageBitrateL();
        
        
         void UT_CMccCodecInformation_FramerateL();
        
        
         void UT_CMccCodecInformation_FrameHeightL();
        
        
         void UT_CMccCodecInformation_FrameWidthL();
        
        
         void UT_CMccCodecInformation_PriorityL();
        
        
         void UT_CMccCodecInformation_PriorityPreferenceL();
        
        
         void UT_CMccCodecInformation_KeepAliveTimerL();
         
         
         void UT_CMccCodecInformation_KeepAliveTimerMicroSecsL();
        
        
         void UT_CMccCodecInformation_KeepAlivePTL();
        
        
         void UT_CMccCodecInformation_KeepAliveDataL();
        
        
         void UT_CMccCodecInformation_ComfortNoiseGenerationL();
        
        
         void UT_CMccCodecInformation_GetChannelsL();
        
        
         void UT_CMccCodecInformation_MaxRedL();
        
        
         void UT_CMccCodecInformation_SetFmtpAttrLL();
        
        
         void UT_CMccCodecInformation_ParseFmtpAttrLL();
        
        
         void UT_CMccCodecInformation_CreateFmtpAttrListLL();
        
        
         void UT_CMccCodecInformation_CreateFmtpAttrListL_1L();
        
        
         void UT_CMccCodecInformation_SetModeChangePeriodL();
        
        
         void UT_CMccCodecInformation_ModeChangePeriodL();
        
        
         void UT_CMccCodecInformation_SetModeChangeNeighborL();
        
        
         void UT_CMccCodecInformation_ModeChangeNeighborL();
        
        
         void UT_CMccCodecInformation_SetCrcL();
        
        
         void UT_CMccCodecInformation_GetCrcL();
        
        
         void UT_CMccCodecInformation_SetRobustSortingL();
        
        
         void UT_CMccCodecInformation_GetRobustSortingL();
        
        
         void UT_CMccCodecInformation_SetInterleavingL();
        
        
         void UT_CMccCodecInformation_GetInterleavingL();
         
         void UT_CMccCodecInformation_SetConfigKeyL();
         
         void UT_CMccCodecInformation_ConfigKeyL();
         
         void UT_CMccCodecInformation_SetPreferredEncodingDecodingDeviceL();
        
        
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


    private:
    
        void GetCapabilitiesL( RPointerArray<CMccCodecInformation>& aCodecArray );
        
    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
	    CMccInterface* iInterface;	    
        RPointerArray<CMccCodecInformation> iCodecArray;
        TInt iCount;
        
    };

#endif      //  __UT_CMccCODECINFORMATION_H__

// End of file
