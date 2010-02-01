/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef CMccCONTROLLERSTUB_H
#define CMccCONTROLLERSTUB_H

#include <e32base.h>
#include <bamdesca.h>
#include "mceclientserver.h"

class MMccCtrlObserver;

const TInt KMce_MM_MaxArray = 50;
const TUint32 KMce_MM_LinkIdBase = 500;
const TUint32 KMce_MM_StreamIdBase = 1;
const TUint32 KMce_MM_EndpointIdBase = 1;

const TUint32 KNoEndpointId = 0;

class CMccControllerStub : public CBase
    {

public:

    enum TMMCActionType
        {
        ENone = -1,
        EMccClose,
        EMccCreateSession,
        EMccSetRemoteAddress,
        EMccCloseSession,
        EMccCreateLink,
        EMccCloseLink,
        EMccCreateStream,
        EMccPrepareStream,
        EMccInactivityTimerStart,
        EMccInactivityTimerStop,
        EMccDeleteStream,
        EMccStartStream,
        EMccStopStream,
        EMccPauseStream,
        EMccResumeStream,
        EMccGetCodec,
        EMccGetFmtpAttr,
        EMccSetCodec,
        EMccGetSupportedCodecs,
        EMccStreamSettings,
        EMccSetAudioPriority,
        EMccCancel,
        EMccSendMediaSignal,
        EMccSendRtcpRR,
        EMccSendRtcpSR,
        EMccSendRtcpData,
        EMccGetSupportedBitrates,
        EMccGetSSRC,
        EMccGetSinkId,
        EMccGetSourceId,
        EMccEnableViewFinder,
        EMccDisableViewFinder,
        EMccGetCamInfo,
        EMccGetNumOfCams,
        EMccSetCamValue1,
        EMccGetCamValue1,
        EMccSetCamValue2,
        EMccGetCamValue2,
        EMccLinkExists,
        EMccStreamsExists,
        EMccUpdateDataSource,
        EMccUpdateDataSink,
        EMccEnableDisplay,
        EMccDisableDisplay,
        EMccReuse,
        EMccSetAudioRoute,
        EMccGetAudioRoute,
        EMccTranscodeFile, 
        EMccCancelTranscodeFile,
        EMccRemoveDataSource,
        EMccRemoveDataSink
        };

    class TAction
        {
        public:
            inline TAction()
                {
                iAction = ENone;
                iSessionID = KMceNotAssigned;
                iLinkID = KMceNotAssigned;
                iStreamID = KMceNotAssigned;
                iEndpointID = KMceNotAssigned;
                iLinkType = KMceNotAssigned;
                iStreamType = KMceNotAssigned;
                iPaused = KMceNotAssigned;
                iEnableRTCP = KMceNotAssigned;
                iKey = KMceNotAssigned;
                iParamSet = KMceNotAssigned;
                iEventType = KMceNotAssigned;
                };
                
        TMMCActionType iAction;
        TUint32 iSessionID;
        TUint32 iLinkID;
        TUint32 iStreamID;
        TUint32 iEndpointID;
        TUint32 iLinkType;
        TUint32 iStreamType;
        TUint32 iPaused;
        TUint32 iEnableRTCP;
        TUint32 iKey;
        TUint32 iParamSet;
        TUint32 iEventType;
        
        };
        
    class TCodecInfo
        {
        public:
            inline TCodecInfo()
                {
                iSessionID = KMceNotAssigned;
                iLinkID = KMceNotAssigned;
                iStreamID = KMceNotAssigned;
                }
                
        public:
        TUint32 iFourCC;
        TUint32 iAlgoUsed;
        TUint32 iBitrate;
        TUint32 iCodecMode;
        TUint32 iEnableDTX;
        TUint32 iMaxPtime;
        TUint32 iPtime;
        TUint32 iPayloadType;
        TUint32 iRedundantPayload;
        TUint32 iRedundancyCount;
        TUint32 iFrameSize;
        TUint32 iHwFrameTime;
        TUint32 iBitrateMask;
        TUint32 iMaxBitrate;
        TUint32 iAverageBitrate;
        TUint32 iFramerate;
        TUint32 iVideoWidth;
        TUint32 iVideoHeight;
        TUint32 iJitterBufInactivityTimeOut;
        TUint32 iJitterBufThreshold;
        TUint32 iJitterBufBufferLength;
        TUint32 iJitterBufPlayToneTimeout;
        TUint32 iJitterBufPlayToneFrequency;
        TUint32 iJitterBufPlayToneDuration;
        TUint32 iSessionID;
        TUint32 iLinkID;
        TUint32 iStreamID;
        };

    class TLink
        {
        public:
            inline TLink()
                {
                iSessionID = KMceNotAssigned;
                iLinkID = KMceNotAssigned;
                iStreams = 0;
                }
        TUint32 iSessionID;
        TUint32 iLinkID;
        TInt iStreams;
        };
        
public:

    static void OpenL();
    static void Close();
    static CMccControllerStub* Stub();

    void Reset( TBool aAll = EFalse );
    TInt ControllerCalled( TMMCActionType aAction,
                           TUint32 aSessionID = KMceNotAssigned,
                           TUint32 aLinkID = KMceNotAssigned,
                           TUint32 aStreamID= KMceNotAssigned,
                           TUint32 aEndpointID = KMceNotAssigned,
                           TUint32 aLinkType = KMceNotAssigned,
                           TUint32 aStreamType = KMceNotAssigned);
                           
    TInt ControllerStreamCtrCalled( TMMCActionType aAction,
                                    TUint32 aSessionID,
                                    TUint32 aLinkID,
                                    TUint32 aStreamID,
                                    TUint32 aEndpointID,
                                    TUint32 aPaused = EFalse,
                                    TUint32 aEnableRTCP = ETrue );

    TInt ControllerStreamSettings(  TUint32 aSessionID,
                                    TUint32 aLinkID,
                                    TUint32 aStreamID,
                                    TUint32 aEndpointID,
                                    TUint32 aKey,
                                    TUint32 aSet );

    TInt ControllerSignal(  TUint32 aSessionID,
                            TUint32 aLinkID,
                            TUint32 aStreamID,
                            TUint32 aEndpointID,
                            TUint32 aSignal,
                            TUint32 aSet );
                                    
    TInt ControllerSignal(  TUint32 aSessionID,
                            TUint32 aLinkID,
                            TUint32 aStreamID,
                            TUint32 aEndpointID,
                            TUint32 aEventType );
                                    

    void SetCodec( TCodecInfo aCodecInfo );
    void GetCodec( TCodecInfo& aCodecInfo );

    void CreateStream( TLink aLink );
    TLink& GetLink( const TLink& aLink );

    
    TUint32 NextSessionId();
    TUint32 NextLinkId();
    TUint32 NextStreamId();
    TUint32 NextEndpointId();
    
    TBool StreamsExists( TUint32 aSessionId, TUint32 aLinkId );
    ~CMccControllerStub();
        
private:

    CMccControllerStub();


public://data
    
    TAction iActions[KMce_MM_MaxArray];
    TInt iActionPtr;
    
    
    TUint32 iSessionID;
    TUint32 iLinkID;
    TUint32 iStreamID;
    TUint32 iEndpointID;
    
    TInt iLeaveError;
    
    MMccCtrlObserver* iObserver;
    
    TBool iRequireSignalling;
    
    TUint32 iRefCount;
    TInt iStreamExists;
    
public: //codec data

    TCodecInfo iCodecs[KMce_MM_MaxArray];
    TLink iLinks[ KMce_MM_MaxArray ];
    
    TBool iSetFmtpFailure;
    
    };



#endif //CMccCONTROLLERSTUB_H

// End of File
