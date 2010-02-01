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





#ifndef CMCETLS_H
#define CMCETLS_H

#include <e32base.h>
#include <bamdesca.h>
#include <stringpool.h>
#include "mceserial.h"
#include "mcesip.h"

class TMceIds;
class CSIPMessageElements;
class CSIPResponseElements;
class RStringF;
class CSdpDocument;
class CMceComSession;

class CMCETls : public CBase
	{

public:

    enum TMMActionType
        {
        ENone = -1,
        EEncode,
        EDecode,
        EUpdate,
        EReserve,
        ECreateRejectedAnswer,
        EPauseSink,
        EPauseSource,
        EResumeSink,
        EResumeSource,
        ECloseSession,
        ESendSR,
        ESendRR,
        EStartInactivityTimer,
        EStopInactivityTimer,
        EGetVolume,
        EGetMaxVolume,
        ESetVolume,
        ESetBalance,
        EGetGain,
        EGetMaxGain,
        ESetGain,
        ESendRTCPAnyData,
        ESetAudioRouting,
        EGetAudioRouting,
        EGetZoomFactor,
        EGetDigitalZoomFactor,
        EGetContrast,
        EGetBrightness,
        EGetExposure,
        EGetWhiteBalance,
        ESetCameraIndex,
        ESetZoomFactor,
        ESetDigitalZoomFactor,
        ESetContrast,
        ESetBrightness,
        ESetExposure,
        ESetWhiteBalance,
        EEnableViewFinder,
        EDisableViewFinder,
        EGetDisplayRect,
        ESetDisplayRect,
        EGetDisplayPriority,
        ESetDisplayPriority,
        ESetDisplayIndex,
        EGetDisplayRotation,
        ESetDisplayRotation,
        EDtmf
        };
        
public:

    static void OpenL();
    static void Close();
    static CMCETls* Storage();

    ~CMCETls();


    void Store( TMceIds aManagerIds );

    void StoreL( TMceIds aIds, 
                 CMceMsgBase& aContext,
                 HBufC8* aContent = NULL );
    
    void StoreL( TMceIds aIds,
                  const TDesC8& aContext );
    
    void SIPSends( RStringF aMethod, CSIPMessageElements* aMessage );
    void SIPSends( RStringF aMethod, CSIPResponseElements* aResponse );
    void MediaManagerAction( TMMActionType aType, const TAny* aObject = 0 );
    void MediaManagerSdpAction( TMMActionType aType, TAny* aObject = 0 );
    
    void SetCallback( TCallBack aMethod );
    void ExecuteCallback();
    void SetLeave( TInt aError, TUint32 aParam = KErrNone );
    TInt LeaveError( TUint32 aParam = KErrNone );
        
private:

    CMCETls();

public://data
	
	TMceIds iIds;
	TMceIds iManagerIds;
	TMceItcFunctions iItcFunction;
	
	HBufC8* iContextClient;
	HBufC8* iContentClient;
	HBufC8* iContextServer;
	HBufC8* iContentServer;
	TUint32 iValue;
    TMMActionType iMediaManagerAction;
    TMMActionType iMediaManagerSdpAction;
	TInt iCompletionCode;
	
	CSIPMessageElements* iSipSentMessage;
	CSIPResponseElements* iSipSentResponse;
	RStringF iSipSentMethod;
	RStringF iSipResponseToMethod;
	
    mutable const TAny* iMMObject;
    
    TBool iAckSent;
    
    TCallBack iCallBack;
    
    TMceMessageBufSizes iMessageSizes;
    
    TInt iIdsIndex;
    
    TMceReturnStatus iMediaManagerUpdateStatus;
    
    TMceSipWarningCode iMediaManagerDecodeStatus;
    
    TMceReturnStatus iMediaManagerReserveStatus;
    
    TBool iMediaManagerNeedToNegotiate;
    
    TMceNegotiationRole iRole;

private:
    
    // These are handled with SetLeave and LeaveError methods
    TInt iLeaveError;
    
    TUint32 iLeaveParam;        
    };



#endif 

// End of File
