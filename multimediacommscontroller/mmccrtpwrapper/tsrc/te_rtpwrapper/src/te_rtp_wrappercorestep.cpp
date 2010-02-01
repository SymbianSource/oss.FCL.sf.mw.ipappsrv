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
// class for the rtpwrapper unit test cases
// 
//

/**
 @file
 @internalComponent
*/
 
#include "te_rtp_wrappercorestep.h"
#include "te_rtp_wrappercoreserver.h"

#include <test/testexecutelog.h>
#include <connpref.h>
#include <es_sock.h>
#include <commdbconnpref.h>
#include "in_sock.h"
#include "srtpcryptoparams.h"
#include "srtpstreamout.h"

#include <libc/sys/time.h>
#include <badesca.h>
#include <random.h>
#include "rtpheader.h"

const TUint KIAPID = 6;
const TInt KLength = 20;
_LIT8(KHello, "RTPWrapper...testing");

//SRTP
_LIT8(KMasterKey,    "E1F97A0D3E018BE0D64FA32C06DE4139");
_LIT8(KMasterSalt,   "0EC675AD498AFEEBB6960B3AABE6");



void CTe_rtpwrapperCoreStep::ErrorNotify( TInt /*aErrCode*/ ) 
    {
    
    }

void CTe_rtpwrapperCoreStep::SdesReceived( TRtpSSRC /*aSSRC*/, const TRtpSdesParams& /*aParams*/ )
    {
    }
    
void CTe_rtpwrapperCoreStep::ByeReceived( TRtpId /*aStreamId*/, TRtpSSRC /*aSSRC*/, const TDesC8& /*aReason*/ )
    { 
    }
void CTe_rtpwrapperCoreStep::AppReceived( TRtpId /*aStreamId*/, TRtpSSRC /*aSSRC*/, const TRtcpApp& /*aApp*/ )
    {
    }
void CTe_rtpwrapperCoreStep::SrReceived( TRtpId /*aStreamId*/, TRtpSSRC /*aSSRC*/, const TTimeStamps& /*aTimeStamps*/ )
    {
    }
void CTe_rtpwrapperCoreStep::RrReceived( TRtpId /*aStreamId*/, TRtpSSRC /*aSSRC */)    
    {
    }

void CTe_rtpwrapperCoreStep::RtpPacketReceived(TRtpId /*aStreamId*/, const TRtpRecvHeader& /* aHeaderInfo*/, const TDesC8& /*aPayloadData*/)
    {
    }

TVerdict CTe_rtpwrapperCoreStep::doTestStepPreambleL()
    {
    SetTestStepResult(EPass);

    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );  
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId( KIAPID );//6
    
    
    TInt err( KErrCouldNotConnect );
        
    // Opens a session to the socket server
    // i.e. creates an IPC communication channel to the socket server
    if ( ( err = iSocketServ.Connect() ) != KErrNone )
        {
        User::LeaveIfError(err);
        }
    // Opens a new RConnection instance
    else if (( err = iConnection.Open( iSocketServ )) != KErrNone )
        {
        iConnection.Close(); 
        User::LeaveIfError(err);
        }
    else if (( err = iConnection.Start( prefs )) != KErrNone )
        {   
        iConnection.Close();
        User::LeaveIfError(err);
        }
    
    err = iSocket.Open( iSocketServ, 
                                  KAfInet,
                                  KSockDatagram, 
                                  KProtocolInetUdp, 
                                  iConnection 
                                );   
   
      
    iRtpApi = CRtpAPI::NewL(*this );

    /* There is no failure cases here */
	TVersion  version = iRtpApi->Version();
    
    TInt result(KErrNone);

    iRtpApi->OpenL( iParams, NULL, &iSocketServ, &iConnection );
    result = iRtpApi->StartConnection(KIAPID);
    if(result != KErrNone)
        {
        SetTestStepResult(EFail);
        return TestStepResult();
        }

    iSessionParams.iPriority = TCreateSessionParams::EPriorityStandard;
    iSessionParams.iSocketBufSize = KSocketBufSize;
    TUint port( 5000 );
    

    GetBoolFromConfig(ConfigSection(),_L("enableRtcp"), iEnableRtcp);

    iRtcpParams.iRtcpFraction = 0.10;
    iRtcpParams.iRtcpTimeOut = 0;
    iRtcpParams.iSessionBWidth = 32000;
    
    iServer->iScheduler = new(ELeave) CActiveScheduler;
    CActiveScheduler::Install(iServer->iScheduler);

    //Creating New session
    iSessionId  = KNullId;
    iSessionId = iRtpApi->CreateSessionL(iSessionParams, port, iEnableRtcp, &iRtcpParams);
    
    if(iSessionId == KErrNone)
        {
        return TestStepResult();
        }
    
    return TestStepResult();
    }


TVerdict CRtpManager_StartConnectionL::doTestStepPreambleL()
    {
    SetTestStepResult(EPass);
    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );  
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId( KIAPID );//6
    
    
    TInt err( KErrCouldNotConnect );
        
    // Opens a session to the socket server
    // i.e. creates an IPC communication channel to the socket server
    if ( ( err = iSocketServ.Connect() ) != KErrNone )
        {
        User::LeaveIfError(err);
        }
    // Opens a new RConnection instance
    else if (( err = iConnection.Open( iSocketServ )) != KErrNone )
        {
        iConnection.Close(); 
        User::LeaveIfError(err);
        }
    else if (( err = iConnection.Start( prefs )) != KErrNone )
        {   
        iConnection.Close();
        User::LeaveIfError(err);
        }
    
    err = iSocket.Open( iSocketServ, 
                                  KAfInet,
                                  KSockDatagram, 
                                  KProtocolInetUdp, 
                                  iConnection 
                                );  
    
    iRtpApi = CRtpAPI::NewL( *this );
    
    return TestStepResult();
    }

TVerdict CRtpManager_StartConnectionL::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TRtpSdesParams iParams;
    iRtpApi->OpenL( iParams, NULL, &iSocketServ, &iConnection );
    iRtpApi->StartConnection(KIAPID);
    
    //Passing -ve IAPID value
    iRtpApi->StartConnection(-1);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }


TVerdict CRtpManager_StartAsyncConnection::doTestStepPreambleL()
    {
    SetTestStepResult(EPass);
    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );  
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId( KIAPID );//6
    
    
    TInt err( KErrCouldNotConnect );
        
    // Opens a session to the socket server
    // i.e. creates an IPC communication channel to the socket server
    if ( ( err = iSocketServ.Connect() ) != KErrNone )
        {
        User::LeaveIfError(err);
        }
    // Opens a new RConnection instance
    else if (( err = iConnection.Open( iSocketServ )) != KErrNone )
        {
        iConnection.Close(); 
        User::LeaveIfError(err);
        }
    else if (( err = iConnection.Start( prefs )) != KErrNone )
        {   
        iConnection.Close();
        User::LeaveIfError(err);
        }
    
    err = iSocket.Open( iSocketServ, 
                                  KAfInet,
                                  KSockDatagram, 
                                  KProtocolInetUdp, 
                                  iConnection 
                                );  
    
    iRtpApi = CRtpAPI::NewL( *this );
    return TestStepResult();
    }
TVerdict CRtpManager_StartAsyncConnection::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TRtpSdesParams iParams;
    iRtpApi->OpenL( iParams, NULL, &iSocketServ, &iConnection );
    
    TRequestStatus status(KErrNone);
    iRtpApi->StartConnection(status, KIAPID);
    User::WaitForRequest(status);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_GetLocalIPAddress::doTestStepPreambleL()
    {
    SetTestStepResult(EPass);
     TCommDbConnPref prefs;
     prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );  
     prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
     prefs.SetIapId( KIAPID );//6
     
     
     TInt err( KErrCouldNotConnect );
         
     // Opens a session to the socket server
     // i.e. creates an IPC communication channel to the socket server
     User::LeaveIfError(iSocketServ.Connect()); 
     // Opens a new RConnection instance
     User::LeaveIfError(iConnection.Open( iSocketServ ));
     if (( err = iConnection.Start( prefs )) != KErrNone )
         {   
         iConnection.Close();
         User::LeaveIfError(err);
         } 
     return TestStepResult();
    }

TVerdict CRtpManager_GetLocalIPAddress::doTestStepL()
    {
    SetTestStepResult(EFail);
    iRtpApi = CRtpAPI::NewL( *this );
    TRtpSdesParams iParams;
    iRtpApi->OpenL( iParams, NULL, &iSocketServ, &iConnection );
    iRtpApi->StartConnection(KIAPID);
    //TInetAddr locAddr;
    TBuf8<50> cnameBuf;
    CRtpManager *pRtpManager = iRtpApi->iManager;
    pRtpManager->MakeACnameL(cnameBuf);
    INFO_PRINTF2(_L("Cname is : %S"), &cnameBuf);

	TInetAddr locIpAddr = iRtpApi->GetLocalIPAddressL();
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_CreateSession::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TInt result(KErrNone);
    result = iRtpApi->StartSession(iSessionId);
    if(result != KErrNone)
        {
        return TestStepResult();
        }
    
    //Not Supported function, for Coverage increase
    iRtpApi->CancelStart();
    SetTestStepResult(EPass);

    return TestStepResult();
    }

TVerdict CRtpManager_RegisterRtcpObserver::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TInt result(KErrNone);    
    result = iRtpApi->StartSession(iSessionId);
    if(result != KErrNone)
        {
        return TestStepResult();
        }
    
    //Passing Wrong iSessionId to RegisterRtcpObserver, to increase coverage
    iRtpApi->RegisterRtcpObserver( iSessionId+1, *this );
    
    //Correct SessionId
    iRtpApi->RegisterRtcpObserver( iSessionId, *this );
    
    //Passing Wrong iSessionId to RegisterRtcpObserver, to increase coverage
    iRtpApi->UnregisterRtcpObserver( iSessionId+1 );
    
    //Correct SessionId
    iRtpApi->UnregisterRtcpObserver( iSessionId );
    
            
    SetTestStepResult(EPass);    
    return TestStepResult();
    }

TVerdict CRtpManager_RegisterRtpObserver::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TInt result(KErrNone);
    result = iRtpApi->StartSession(iSessionId);
    if(result != KErrNone)
        {
        return TestStepResult();
        }
    
    iRtpApi->RegisterRtpObserver(iSessionId, *this); 
    iRtpApi->UnregisterRtpObserver( iSessionId );
    
    SetTestStepResult(EPass);    
    return TestStepResult();
    }

TVerdict CRtpManager_GetRtpSocket::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    //Passing Wrong iSessionId to increase coverage
    iRtpApi->GetRtpSocket(iSessionId+1);
    iRtpApi->GetRtcpSocket(iSessionId+1);
    
    //Passing correct SessionId values
    iRtpApi->GetRtpSocket(iSessionId);
    iRtpApi->GetRtcpSocket(iSessionId);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_CreateReceiveStream::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TInt result(KErrNone);
    TRcvStreamParams rcvStreamParams;
    rcvStreamParams.iPayloadType = 96;
    TRtpId streamId = iRtpApi->CreateReceiveStreamL(iSessionId, rcvStreamParams);
    INFO_PRINTF2(_L("Created Recv Stream Id is : %d"), streamId);
    INFO_PRINTF1(_L("SetSamplingRate..."));
    iRtpApi->SetSamplingRate(rcvStreamParams.iPayloadType, 8000);
    
    INFO_PRINTF1(_L("Find Stream ..."));
    CRtpManager *pRtpManager = iRtpApi->iManager;
    result = pRtpManager->FindStreamL(streamId);
    if(result == KErrNotFound)
        {
        return TestStepResult();
        }
    
    INFO_PRINTF1(_L("Closing Stream ..."));
    iRtpApi->CloseStream(streamId);
    
    SetTestStepResult(EPass);
    
    return TestStepResult();
    }

TVerdict CRtpManager_CreateTransmitStream::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc); 
    INFO_PRINTF2(_L("Created Recv Stream Id is : %d"), streamId);
    INFO_PRINTF1(_L("SetSamplingRate..."));
    iRtpApi->SetSamplingRate(tranStreamParams.iPayloadType, 8000);

    
    INFO_PRINTF1(_L("Closing Stream ..."));
    iRtpApi->CloseStream(streamId);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SendData::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    //iRtpApi->iStandardRtp=EFalse;
    
    HBufC8* data = HBufC8::NewLC(KLength);
    data->Des().Copy(KHello);
    CleanupStack::Pop(data);
    TRequestStatus status(KErrNone);
    
    iRtpApi->SendDataL(iSessionId, ETrue, *data, status);    
    
    User::WaitForRequest(status);
    //close
    delete data;
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_CancelSend::doTestStepL()
    {
    SetTestStepResult(EFail);

    //iRtpApi->iStandardRtp=ETrue;
    
    iRtpApi->CancelSend(iSessionId);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SendRtcpByePacket::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    // A send stream should be open to send an RTCP Bye packet
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc); 
    
    CRtpManager *pRtpManager = iRtpApi->iManager;
    
    iRtpApi->RegisterRtcpObserver( iSessionId, *this );
    TInt err = iRtpApi->SendRtcpByePacket( streamId, _L8( "ByePacket" )  );
    if(err)
        {
        return TestStepResult();
        }
    iRtpApi->UnregisterRtcpObserver( iSessionId );
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SendRtcpSrPacket::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    // A send stream should be open to send an RTCP SR packet
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc); 
    
    CRtpManager *pRtpManager = iRtpApi->iManager;
    iRtpApi->RegisterRtcpObserver( iSessionId, *this );
    
    
    TBuf8<30> cname(KHello);
    iParams.iCName.Set(cname);
    iRtpApi->SetLocalSdes(iParams);
    
    TInt err = iRtpApi->SendRtcpSrPacket( streamId );
    if(err)
        {
        return TestStepResult();
        }
    iRtpApi->UnregisterRtcpObserver( iSessionId );
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SendRtcpRRPacket::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    // We will send RTCP RR packet on receive stream
    TRcvStreamParams rcvStreamParams;
    rcvStreamParams.iPayloadType = 96;
    TRtpId streamId = iRtpApi->CreateReceiveStreamL(iSessionId, rcvStreamParams);
    
    //iRtpApi->iStandardRtp=ETrue;
    iRtpApi->RegisterRtcpObserver( iSessionId, *this );
    TInt err = iRtpApi->SendRtcpRrPacket( streamId );
    if(err)
        {
        return TestStepResult();
        }
    iRtpApi->UnregisterRtcpObserver( iSessionId );
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }


TVerdict CRtpManager_SendRtcpAppPacket::doTestStepL()
    {
    SetTestStepResult(EFail);
     
    // A send stream should be open to send an RTCP App packet
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc);
    
    //iRtpApi->iStandardRtp=ETrue;
    TRtcpApp app;
    app.iSubType = 0;
    app.iName[0] = 0;
    app.iAppData[0] = 0;
    app.iAppDataLen = KMaxRtcpAppData + 1;
    iRtpApi->RegisterRtcpObserver( iSessionId, *this );
    TInt err = iRtpApi->SendRtcpAppPacket( streamId, app  );
    if(err)
        {
        return TestStepResult();
        }
    iRtpApi->UnregisterRtcpObserver( iSessionId );
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SuspendRtcpSending::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    INFO_PRINTF1(_L("Suspend RTCP sending..."));
    TInt err = iRtpApi->SuspendRtcpSending( iSessionId, EFalse );
    if(err)
        {
        return TestStepResult();
        }
    TBool autosending;
    INFO_PRINTF1(_L("IsRtcpSendingSuspended..."));
    iRtpApi->IsRtcpSendingSuspended( iSessionId, autosending );
    
    if(autosending)
        {
        return TestStepResult();
        }

    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_GetSessionId::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc); 
    
    /*   TRcvStreamParams rcvStreamParams;
    rcvStreamParams.iPayloadType = 96;
    TRtpId streamId = iRtpApi->CreateReceiveStreamL(iSessionId, rcvStreamParams); */
    

    TRtpId sessionId1 = iRtpApi->GetSessionId(streamId);
    if(sessionId1 != iSessionId)
        {
        return TestStepResult();
        }
  
    TRtpPayloadType payloadType( 96 );
    TUint32 samplingrate( 8000 );
    TInt err = iRtpApi->SetSamplingRate(payloadType, samplingrate);
    if(err)
        {
        return TestStepResult();
        }

    TUint32 samplingrate1 = iRtpApi->GetSamplingRate(payloadType);
    if(samplingrate != samplingrate1)
        {
        return TestStepResult();
        }
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SetRtcpParameters::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TRtcpParams iParams;
    iParams.iRtcpFraction = 0.20;
    iParams.iRtcpTimeOut = 0;
    iParams.iSessionBWidth = 16000;
    iRtpApi->SetRtcpParameters(iSessionId, iParams);

    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SetRemoteAddress::doTestStepL()
    {
    SetTestStepResult(EFail);
 
    TInetAddr addr(KInetAddrLoop, 9000);
    iRtpApi->SetRemoteAddress(iSessionId, addr);
    
    TInetAddr rtcpaddr(KInetAddrLoop, 9001);
    iRtpApi->SetRemoteRtcpAddress(iSessionId, rtcpaddr);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SendRtpPacket::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc);
    
    HBufC8* data = HBufC8::NewLC(KLength);
    data->Des().Copy(KHello);
    CleanupStack::Pop(data);
    
    TRtpSendHeader headerinfo;
    headerinfo.iHeaderExtension = 0;
    headerinfo.iPayloadType = 96;
    headerinfo.iTimestamp = 12345678;
    
    iRtpApi->SetSamplingRate(headerinfo.iPayloadType, 8000);
    iRtpApi->SendRtpPacket(streamId, headerinfo, *data);
    
    delete data;
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

CWaiter* CWaiter::NewLC(CTe_rtpwrapperCoreServer* aServer)
    {
    CWaiter* that = new (ELeave) CWaiter(aServer);
    CleanupStack::PushL(that);
    return that;
    }

void CWaiter::RunL()
    {
    CActiveScheduler::Stop();
    iServer->iStopping = ETrue;
    }

void CWaiter::DoCancel()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrCancel);
    }


CWaiter::~CWaiter()
    {
    Cancel();
    }

TVerdict CRtpManager_SendRtpPacket1::doTestStepL()
    {
    SetTestStepResult(EFail);

    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc);
    
    CWaiter* waiter = CWaiter::NewLC(iServer);
    HBufC8* data = HBufC8::NewLC(KLength);
    data->Des().Copy(KHello);
    CleanupStack::Pop(data);
    TRequestStatus status(KErrNone);
    
    TRtpSendHeader headerinfo;
    headerinfo.iHeaderExtension = 0;
    headerinfo.iPayloadType = 96;
    headerinfo.iTimestamp = 12345678;
    
    iRtpApi->SetSamplingRate(headerinfo.iPayloadType, 8000);
    
    TInetAddr addr(KInetAddrLoop, 9000);
    iRtpApi->SetRemoteAddress(iSessionId, addr);
    
    TRequestStatus stat;
    TInt err = iRtpApi->SendRtpPacket(streamId, headerinfo, *data, waiter->iStatus);
    waiter->Ready();
    CActiveScheduler::Start();
    
    // Sending an RTP data packet asynchronously, with a given sequence number
    iServer->iStopping = EFalse;
    TRtpSequence seqno = 27890;
    err = iRtpApi->SendRtpPacket(streamId, seqno, headerinfo, *data, waiter->iStatus);
    waiter->Ready();
    CActiveScheduler::Start();

    
    CleanupStack::PopAndDestroy(waiter);
    delete data;
           
    SetTestStepResult(EPass);
    return TestStepResult();
    }

CTe_SrtpKeyRingObserver* CTe_SrtpKeyRingObserver::NewL(CTe_rtpwrapperCoreStep *aStep)
    {
    CTe_SrtpKeyRingObserver *self = new(ELeave)CTe_SrtpKeyRingObserver();
    self->iParentStep = aStep;
    return self;
    }

TVerdict CRtpManager_SrtpCreateSession::doTestStepPreambleL()
    {
    SetTestStepResult(EPass);
    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );  
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId( KIAPID );//6
    
    
    TInt err( KErrCouldNotConnect );
        
    // Opens a session to the socket server
    // i.e. creates an IPC communication channel to the socket server
    if ( ( err = iSocketServ.Connect() ) != KErrNone )
        {
        User::LeaveIfError(err);
        }
    // Opens a new RConnection instance
    else if (( err = iConnection.Open( iSocketServ )) != KErrNone )
        {
        iConnection.Close(); 
        User::LeaveIfError(err);
        }
    else if (( err = iConnection.Start( prefs )) != KErrNone )
        {   
        iConnection.Close();
        User::LeaveIfError(err);
        }
    
    err = iSocket.Open( iSocketServ, 
                                  KAfInet,
                                  KSockDatagram, 
                                  KProtocolInetUdp, 
                                  iConnection 
                                );   
   
      
    iRtpApi = CRtpAPI::NewL( *this );
    
    return TestStepResult();
    }

TVerdict CRtpManager_SrtpCreateSession::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TInt result(KErrNone);
    TRtpSdesParams iParams;
    iRtpApi->OpenL( iParams, NULL, &iSocketServ, &iConnection );

    result = iRtpApi->StartConnection(KIAPID);
    if(result != KErrNone)
        {
        return TestStepResult();
        }
    
    TCreateSessionParams iSessionParams;
    iSessionParams.iPriority = TCreateSessionParams::EPriorityStandard;
    iSessionParams.iSocketBufSize = KSocketBufSize;
    TUint port( 5000 );
    
    // RTCP enabled
    TBool iEnableRtcp( ETrue );
    TRtcpParams iRtcpParams;
    iRtcpParams.iRtcpFraction = 0.10;
    iRtcpParams.iRtcpTimeOut = 0;
    iRtcpParams.iSessionBWidth = 32000;

    TRtpId iSessionId( KNullId );
    
    // Master Key
    HBufC8* masterKey = HBufC8::NewL(KMasterKey().Length());
    CleanupStack::PushL( masterKey );
    *masterKey = KMasterKey;
    
    // Master Salt
    HBufC8* masterSalt =HBufC8::NewL(KMasterSalt().Length());
    CleanupStack::PushL( masterSalt);
    *masterSalt = KMasterSalt;
    
    iMasterKey = CSRTPMasterKey::NewL(*masterKey, _L8(""));
    CleanupStack::PushL(iMasterKey);
    iMasterSalt = CSRTPMasterSalt::NewL(*masterSalt);
    CleanupStack::PushL(iMasterSalt);
    
    TSrtpCryptoParams srtpparams;
    CSRTPCryptoContext *srtpContext = CSRTPCryptoContext::NewL(iMasterKey, iMasterSalt, srtpparams);
    
    // Ownership of keys is with context
    CleanupStack::Pop(4);
    CleanupStack::PushL(srtpContext);
    
    TInetAddr destination;
    destination.Input(_L("127.0.0.1"));
    
    iKeyRingObs = CTe_SrtpKeyRingObserver::NewL(this);
    
    CSRTPSession *srtpSession = CSRTPSession::NewL(destination, srtpContext, *iKeyRingObs);
    
    iServer->iScheduler = new(ELeave) CActiveScheduler;
    CActiveScheduler::Install(iServer->iScheduler);
    
    iSessionId = iRtpApi->CreateSessionL(iSessionParams, port, iEnableRtcp, &iRtcpParams, *srtpSession);
    if(iSessionId == KNullId)
        {
        SetTestStepResult(EFail);
        return TestStepResult();
        }    
    
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc);
    
    CSRTPStreamOut* iStreamout = CSRTPStreamOut::NewL(*srtpSession, ssrc);

    
    HBufC8* data = HBufC8::NewLC(KLength);
    data->Des().Copy(KHello);
    CleanupStack::Pop(data);
    
    TRtpSendHeader headerinfo;
    headerinfo.iHeaderExtension = 0;
    headerinfo.iPayloadType = 96;
    headerinfo.iTimestamp = 12345678;
    
    
    TInetAddr addr(KInetAddrLoop, 9000);
    iRtpApi->SetRemoteAddress(iSessionId, addr);

    
    iRtpApi->SetSamplingRate(headerinfo.iPayloadType, 8000);
    TInt err = iRtpApi->SendRtpPacket(streamId, headerinfo, *data);
    if(err)
        {
       // return TestStepResult();
        }

    CleanupStack::Pop(srtpContext);
    
    delete data;
    delete iStreamout;
    delete srtpSession;
    delete iKeyRingObs;
    delete masterKey;
    delete masterSalt;

    iRtpApi->CloseSession(iSessionId);
    iRtpApi->Close();
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_SrtpCreateSession1::doTestStepPreambleL()
    {
    SetTestStepResult(EPass);
    TCommDbConnPref prefs;
    prefs.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );  
    prefs.SetDirection( ECommDbConnectionDirectionOutgoing );
    prefs.SetIapId( KIAPID );//6
    
    
    TInt err( KErrCouldNotConnect );
        
    // Opens a session to the socket server
    // i.e. creates an IPC communication channel to the socket server
    if ( ( err = iSocketServ.Connect() ) != KErrNone )
        {
        User::LeaveIfError(err);
        }
    // Opens a new RConnection instance
    else if (( err = iConnection.Open( iSocketServ )) != KErrNone )
        {
        iConnection.Close(); 
        User::LeaveIfError(err);
        }
    else if (( err = iConnection.Start( prefs )) != KErrNone )
        {   
        iConnection.Close();
        User::LeaveIfError(err);
        }
    
    err = iSocket.Open( iSocketServ, 
                                  KAfInet,
                                  KSockDatagram, 
                                  KProtocolInetUdp, 
                                  iConnection 
                                );   
   
      
    iRtpApi = CRtpAPI::NewL( *this );
    
    return TestStepResult();
    }

TVerdict CRtpManager_SrtpCreateSession1::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TInt result(KErrNone);
    TRtpSdesParams iParams;
    iRtpApi->OpenL( iParams, NULL, &iSocketServ, &iConnection );

    result = iRtpApi->StartConnection(KIAPID);
    if(result != KErrNone)
        {
        return TestStepResult();
        }
    
    TCreateSessionParams iSessionParams;
    iSessionParams.iPriority = TCreateSessionParams::EPriorityStandard;
    iSessionParams.iSocketBufSize = KSocketBufSize;
    TUint port( 5000 );
    
    // RTCP enabled
    TBool iEnableRtcp( ETrue );
    TRtcpParams iRtcpParams;
    iRtcpParams.iRtcpFraction = 0.10;
    iRtcpParams.iRtcpTimeOut = 0;
    iRtcpParams.iSessionBWidth = 32000;

    TRtpId iSessionId( KNullId );
      
    iMasterKey = CSRTPMasterKey::NewL(KMasterKey(), _L8(""));
    CleanupStack::PushL(iMasterKey);
    iMasterSalt = CSRTPMasterSalt::NewL(KMasterSalt());
    CleanupStack::PushL(iMasterSalt);
    
    TSrtpCryptoParams srtpparams;
    CSRTPCryptoContext *srtpContext = CSRTPCryptoContext::NewL(iMasterKey, iMasterSalt, srtpparams);
    
    // Ownership of keys is with context
    CleanupStack::Pop(2);
    CleanupStack::PushL(srtpContext);
    
    TInetAddr destination;
    destination.Input(_L("127.0.0.1"));
    
    iKeyRingObs = CTe_SrtpKeyRingObserver::NewL(this);
    
    CSRTPSession *srtpSession = CSRTPSession::NewL(destination, srtpContext, *iKeyRingObs);
    
    iServer->iScheduler = new(ELeave) CActiveScheduler;
    CActiveScheduler::Install(iServer->iScheduler);
    
    iSessionId = iRtpApi->CreateSessionL(iSessionParams, port, iEnableRtcp, &iRtcpParams, *srtpSession);
    if(iSessionId == KNullId)
        {
        SetTestStepResult(EFail);
        return TestStepResult();
        }    
    
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc);
    
    CSRTPStreamOut* iStreamout = CSRTPStreamOut::NewL(*srtpSession, ssrc);

    
    iRtpApi->RegisterRtcpObserver( iSessionId, *this );
        
    TBuf8<30> cname(KHello);
    iParams.iCName.Set(cname);
    iRtpApi->SetLocalSdes(iParams);
 
    TInt err = iRtpApi->SendRtcpSrPacket( streamId );
    if(err)
        {
        return TestStepResult();
        }
    iRtpApi->UnregisterRtcpObserver( iSessionId );

    CleanupStack::Pop(srtpContext);
    
    delete iStreamout;
    delete srtpSession;
    delete iKeyRingObs;
    iRtpApi->CloseSession(iSessionId);
    iRtpApi->Close();
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }


TVerdict CRtpManager_CreateMultipleSessions::doTestStepL()
    {
    SetTestStepResult(EFail);

    iSessionParams.iPriority = TCreateSessionParams::EPriorityStandard;
    iSessionParams.iSocketBufSize = KSocketBufSize;
    TUint port( 6000 );
    

    GetBoolFromConfig(ConfigSection(),_L("enableRtcp"), iEnableRtcp);

    iRtcpParams.iRtcpFraction = 0.10;
    iRtcpParams.iRtcpTimeOut = 0;
    iRtcpParams.iSessionBWidth = 32000;

    //Creating New session
    TRtpId iSessionId2  = KNullId;
    iSessionId2 = iRtpApi->CreateSessionL(iSessionParams, port, iEnableRtcp, &iRtcpParams);
    
    if(iSessionId2 == KErrNone)
        {
        return TestStepResult();
        }
    
    iRtpApi->CloseSession(iSessionId2);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_CreateTransmitStreamExt::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamExtL(iSessionId, tranStreamParams, ssrc);
    //TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc); 
    INFO_PRINTF2(_L("Created Recv Stream Id is : %d"), streamId);
    INFO_PRINTF1(_L("SetSamplingRate..."));
    iRtpApi->SetSamplingRate(tranStreamParams.iPayloadType, 8000);

    
    INFO_PRINTF1(_L("Closing Stream ..."));
    iRtpApi->CloseStream(streamId);
    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_CustomCommandsync::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc);
    
  
    TRtpSendHeader headerinfo;
    headerinfo.iHeaderExtension = 0;
    headerinfo.iPayloadType = 96;
    headerinfo.iTimestamp = 12345678;
    
    TInt num = 100;
    TBuf8<5> buf1;
    TBuf8<5> buf2;
    TBuf8<5> buf3;
    
    iRtpApi->CustomCommandSync(num, buf1, buf2, buf3);

    
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_CustomCommandAsync::doTestStepL()
    {
    SetTestStepResult(EFail);
    
    TTranStreamParams tranStreamParams;
    tranStreamParams.iPayloadType = 96;
    TRtpSSRC ssrc = 4567890;
    TRtpId streamId = iRtpApi->CreateTransmitStreamL(iSessionId, tranStreamParams, ssrc);
    
  
    TRtpSendHeader headerinfo;
    headerinfo.iHeaderExtension = 0;
    headerinfo.iPayloadType = 96;
    headerinfo.iTimestamp = 12345678;
    
    TInt num = 100;
    TBuf8<5> buf1;
    TBuf8<5> buf2;
    TBuf8<5> buf3;
    
    TRequestStatus status(KErrNone);
    
    iRtpApi->CustomCommandAsync(num, buf1, buf2, buf3, status);

    
    SetTestStepResult(EPass);
    return TestStepResult();
    }


void CRtpManager_SetNonRTPDataObserver::NonRTPDataReceived( TUint /*aPort*/, TBool /*aRTPport*/, const TDesC8& /*aNonRTPData*/)
    {    
    }

TVerdict CRtpManager_SetNonRTPDataObserver::doTestStepL()
    {
    SetTestStepResult(EFail);

    TInt err = iRtpApi->SetNonRTPDataObserver(iSessionId, this);
    if(err)
        {
        return TestStepResult();
        }
    SetTestStepResult(EPass);
    return TestStepResult();
    }

TVerdict CRtpManager_StartConnectionL::doTestStepPostambleL()
    {
    delete iRtpApi; 
    iRtpApi=NULL;
    iSocket.Close(); 
    iConnection.Close(); 
    iSocketServ.Close();
    return TestStepResult();
    }

TVerdict CRtpManager_StartAsyncConnection::doTestStepPostambleL()
    {
    delete iRtpApi; 
    iRtpApi=NULL;
    iSocket.Close(); 
    iConnection.Close(); 
    iSocketServ.Close();
    return TestStepResult();
    }

TVerdict CTe_rtpwrapperCoreStep::doTestStepPostambleL()
    {
    iRtpApi->CloseSession(iSessionId);
    iRtpApi->Close();
    
    delete iRtpApi; 
    iRtpApi=NULL;
    iSocket.Close(); 
    iConnection.Close(); 
    iSocketServ.Close();
    delete iServer->iScheduler;
    iServer->iScheduler = 0;
    CActiveScheduler::Install(0);
    return TestStepResult();
    } 

