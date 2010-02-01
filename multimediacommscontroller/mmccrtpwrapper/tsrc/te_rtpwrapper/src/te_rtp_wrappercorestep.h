// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Header for class specific to rtpwrapper unit tests
// 
//

/**
 @file
 @internalComponent
*/

#if (!defined TE_RTP_WRAPPERCORE_STEP_H)
#define TE_RTP_WRAPPERCORE_STEP_H
#include <test/testexecuteserverbase.h>
#include "rtpapi.h"
#include "rtpmanager.h"
#include "srtpmastersalt.h"
#include "srtpmasterkey.h"
#include "msrtprekeyingobserver.h"
#include "srtpcryptocontext.h"

class CTe_rtpwrapperCoreServer;

class CTe_rtpwrapperCoreStep : public CTestStep, public MRtpErrNotify, public MRtcpObserver, public MRtpObserver
	{
public:
	~CTe_rtpwrapperCoreStep() {};
	inline void SetServer(CTe_rtpwrapperCoreServer* aServer);
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL() = 0;
    virtual TVerdict doTestStepPostambleL();
	
    //From observer interface
    void ErrorNotify( TInt aErrCode ) ;
    
    // from MRtcpObserver

    void SdesReceived( TRtpSSRC aSSRC,
                       const TRtpSdesParams& aParams );

    void ByeReceived( TRtpId aStreamId, TRtpSSRC aSSRC,
                      const TDesC8& aReason );

    void AppReceived( TRtpId aStreamId, TRtpSSRC aSSRC,
                      const TRtcpApp& aApp );

    void SrReceived( TRtpId aStreamId, TRtpSSRC aSSRC,
                     const TTimeStamps& aTimeStamps );

    void RrReceived( TRtpId aStreamId, TRtpSSRC aSSRC );
    
    // From MRtpObserver
    void RtpPacketReceived( TRtpId aStreamId, const TRtpRecvHeader& aHeaderInfo, const TDesC8& aPayloadData );
        
	
	TInt iError;
	TBool iIgnoreIncrease;
protected:
	CTe_rtpwrapperCoreServer* iServer;
public:
    CRtpAPI* iRtpApi ;
    RSocketServ iSocketServ;
    RSocket iSocket;    
    RConnection iConnection;
    
    TRtpSdesParams iParams;
    TCreateSessionParams iSessionParams;
    TBool       iEnableRtcp;
    TRtcpParams iRtcpParams;
    TRtpId iSessionId;
    
	};

inline void CTe_rtpwrapperCoreStep::SetServer(CTe_rtpwrapperCoreServer* aServer)
	{
	iServer = aServer;
	}



class CRtpManager_StartConnectionL : public CTe_rtpwrapperCoreStep
    {
public:
        virtual TVerdict doTestStepPreambleL();
        virtual TVerdict doTestStepL();
        virtual TVerdict doTestStepPostambleL();
    };

class CRtpManager_GetLocalIPAddress : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepL();
    };

class CRtpManager_CreateSession : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_RegisterRtcpObserver : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_RegisterRtpObserver : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_GetRtpSocket : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_CreateReceiveStream : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_CreateTransmitStream : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SendData : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_CancelSend : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SendRtcpByePacket : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SendRtcpSrPacket : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SendRtcpRRPacket : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SendRtcpAppPacket : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SuspendRtcpSending : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_GetSessionId : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SetRtcpParameters : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SetRemoteAddress : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_SendRtpPacket : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CWaiter : public CActive
    {
 public:
    static CWaiter* NewLC(CTe_rtpwrapperCoreServer* aServer);

    ~CWaiter();
    
    void RunL();
    void DoCancel();

    void Ready() 
        {
        SetActive();
        };
    
    CTe_rtpwrapperCoreServer* iServer;
  
 private:
    CWaiter(CTe_rtpwrapperCoreServer* aServer) : CActive(0), iServer(aServer) 
            {
            CActiveScheduler::Add(this);
            };

    friend class CRtpManager_SendRtpPacket1;
    };

class CRtpManager_SendRtpPacket1 : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CTe_SrtpKeyRingObserver: public CBase, public MSRTPReKeyingObserver
    {
    CTe_rtpwrapperCoreStep* iParentStep;
public:
    static CTe_SrtpKeyRingObserver* NewL(CTe_rtpwrapperCoreStep *aParent);
    virtual void SRTPMasterKeyStaleEvent(const CSRTPStream& aStream)
        {
        (void)aStream;
        }
    virtual void SRTPMasterKeyStaleEvent(const CSRTPSession& aSession)
        {
        (void)aSession;
        }

    };

class CRtpManager_SrtpCreateSession : public CTe_rtpwrapperCoreStep
    {
public:
    
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepL();
    
    TBool      iReKeyCalled;
    CSRTPMasterSalt* iMasterSalt; 
    CSRTPMasterKey*  iMasterKey; 
    CTe_SrtpKeyRingObserver *iKeyRingObs;
   
    };

class CRtpManager_SrtpCreateSession1 : public CTe_rtpwrapperCoreStep
    {
public:
    
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepL();
    
    TBool      iReKeyCalled;
    CSRTPMasterSalt* iMasterSalt; 
    CSRTPMasterKey*  iMasterKey; 
    CTe_SrtpKeyRingObserver *iKeyRingObs;
 
    };

class CRtpManager_CreateMultipleSessions : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_CreateTransmitStreamExt : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_CustomCommandsync : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_CustomCommandAsync : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepL();
    };

class CRtpManager_StartAsyncConnection : public CTe_rtpwrapperCoreStep
    {
public:
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepL();
    virtual TVerdict doTestStepPostambleL();
    };

class CRtpManager_SetNonRTPDataObserver : public CTe_rtpwrapperCoreStep, public MNonRTPDataObserver
    {
public:
    virtual TVerdict doTestStepL();
    
    // From MNonRTPDataObserver
    void NonRTPDataReceived( TUint aPort, TBool aRTPport, const TDesC8& aNonRTPData );
    };

_LIT(KStartConnection,"StartConnection");
_LIT(KGetLocalIPAddress, "GetLocalIPAddress");
_LIT(KCreateSession, "CreateSession");
_LIT(KRegisterRtcpObserver, "RegisterRtcpObserver");
_LIT(KRegisterRtpObserver, "RegisterRtpObserver");
_LIT(KGetRtpSocket, "GetRtpSocket");
_LIT(KCreateReceiveStream, "CreateReceiveStream");
_LIT(KCreateTransmitStream, "CreateTransmitStream");
_LIT(KSendData, "SendData");
_LIT(KCancelSend, "CancelSend");
_LIT(KSendRtcpByePacket, "SendRtcpByePacket");
_LIT(KSendRtcpSrPacket, "SendRtcpSrPacket");
_LIT(KSendRtcpRRPacket, "SendRtcpRRPacket");
_LIT(KSendRtcpAppPacket, "SendRtcpAppPacket");
_LIT(KSuspendRtcpSending, "SuspendRtcpSending");
_LIT(KGetSessionId, "GetSessionId");
_LIT(KSetRtcpParameters, "SetRtcpParameters");
_LIT(KSetRemoteAddress, "SetRemoteAddress");
_LIT(KSendRtpPacket, "SendRtpPacket");
_LIT(KSendRtpPacket1, "SendRtpPacket1");
_LIT(KSrtpCreateSession, "SrtpCreateSession");
_LIT(KSrtpCreateSession1, "SrtpCreateSession1");
_LIT(KCreateMultipleSessions, "CreateMultipleSessions");
_LIT(KCreateTransmitStreamExt, "CreateTransmitStreamExt");
_LIT(KCustomCommandsync, "CustomCommandsync");
_LIT(KCustomCommandAsync, "CustomCommandAsync");
_LIT(KStartAsyncConnection, "StartAsyncConnection");
_LIT(KSetNonRTPDataObserver, "SetNonRTPDataObserver");
        
#endif


