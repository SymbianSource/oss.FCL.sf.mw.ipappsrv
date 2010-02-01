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
// Description: THIS FILE IS A PROTOTYPE AND IS NOT MEANT TO BE USED IN PRODUCTION CODE
// 
//

#include "rtpapi.h"
#include "rtpmanager.h" 

/**
* Two-phased constructor.
*/
EXPORT_C CRtpAPI* CRtpAPI::NewL( MRtpErrNotify& aErrNotify )
	{
    CRtpAPI *self = new(ELeave) CRtpAPI();
	CleanupStack::PushL(self);
    self->ConstructL(aErrNotify);
    CleanupStack::Pop();
    return self;
	}

CRtpAPI::~CRtpAPI()
	{
	delete iManager;
	}

EXPORT_C TInt CRtpAPI::OpenL( const TRtpSdesParams& aSdesInfo,
					const TDesC* aRtpPacketDll,
					const RSocketServ* aSocketServPtr,
					const RConnection* aConnPtr )
	{
	return iManager->OpenL(aSdesInfo,aRtpPacketDll, aSocketServPtr, aConnPtr);
	}


EXPORT_C TInt CRtpAPI::StartConnection( TInt aIapId  )
	{
	return iManager->StartConnection(aIapId);
	}


EXPORT_C TInt CRtpAPI::StartConnection( TRequestStatus& aStatus,TInt aIapId )
	{
	return iManager->StartConnection(aStatus, aIapId);
	}

/**
* Cancels asynchoronous start of connection.
* Any open requests will be completed with KErrCancel.
* @return None
*/
EXPORT_C void CRtpAPI::CancelStart()
	{
	iManager->CancelStart();
	return;
	}		

/**
* Close the CRtpAPI object.
*/
EXPORT_C void CRtpAPI::Close()
	{
	iManager->Close();
	return;
	}

/**
* Set SDES (Source Description) information of the local participant.
* Participant is defined as an application program and
* only one local participant is allowed.
*/
EXPORT_C void CRtpAPI::SetLocalSdes( const TRtpSdesParams& aSdesInfo )
	{
	iManager->SetLocalSdes(aSdesInfo);
	}

/**
* Get the local IP address
* @param None
* @return local ip address.
*/
EXPORT_C TInetAddr& CRtpAPI::GetLocalIPAddressL()
	{
	return iManager->GetLocalIPAddressL();
	}

/**
* Create a new RTP Session and return the Session ID as well as the
* local port number assigned for RTP. RTP uses an even port number and
* RTCP, if enabled, uses the next higher (odd) port number.
*/
EXPORT_C TRtpId CRtpAPI::CreateSessionL(const TCreateSessionParams& aSessionParams,
										TUint& aPort,
										TBool aEnableRtcp,
										const TRtcpParams* aRtcpParams )
	{
	return iManager->CreateSessionL(aSessionParams, aPort, aEnableRtcp, aRtcpParams);
	}
/**
* Create a new, secure RTP Session and return the Session ID as well as the
* local port number assigned for RTP. RTP uses an even port number and
* RTCP, if enabled, uses the next higher (odd) port number.
* User is expected to create corresponding SRTP session prior to calling this
* function. 
*/

EXPORT_C TRtpId CRtpAPI::CreateSessionL( 
	                const TCreateSessionParams& aSessionParams, 
	                TUint& aPort, 
	                TBool aEnableRtcp, 
	                const TRtcpParams* aRtcpParams, 
	                CSRTPSession& aSession)
	{
	return iManager->CreateSessionL(aSessionParams, aPort, aEnableRtcp, aRtcpParams, aSession);
	}


/**
* Start an RTP Session. If enabled, RTCP associated with the given
* session is also started.
*/
EXPORT_C TInt CRtpAPI::StartSession( TRtpId aSessionId )
	{
	return iManager->StartSession(aSessionId);
	}

/**
* Close an RTP Session.
*/
EXPORT_C void CRtpAPI::CloseSession( TRtpId aSessionId )
	{
	iManager->CloseSession(aSessionId);
	return;
	}

/**
* Set remote IP address and port number to RTP Session.
* Port number for RTP must be an even number and the corresponding
* RTCP, if enabled, will be set to use the next higher (odd) port.
*/
EXPORT_C TInt CRtpAPI::SetRemoteAddress( TRtpId aSessionId,const TInetAddr& aRemoteAddr )
	{
	return iManager->SetRemoteAddress(aSessionId, aRemoteAddr);
	}

/**
* Set remote RTCP IP address and port number to RTP Session accoring to RFC 3605
* i.e. this should be used if RTCP port is different than RTP port + 1
*/
EXPORT_C TInt CRtpAPI::SetRemoteRtcpAddress( TRtpId aSessionId, const TInetAddr& aRemoteRtcpAddr )
	{
	return iManager->SetRemoteRtcpAddress(aSessionId, aRemoteRtcpAddr);
	}

/**
* Create a Receive stream for an RTP Session and return the stream
* ID which is unique for all RTP Sessions.
*/
EXPORT_C TRtpId CRtpAPI::CreateReceiveStreamL( TRtpId aSessionId,const TRcvStreamParams& aParams )
	{
	return iManager->CreateReceiveStreamL(aSessionId, aParams);
	}

/**
* Create a Transmit stream for an RTP Session and return the stream
* ID which is unique for all RTP Sessions. SSRC value assigned for the
* very first Transmit stream will be the same as the default SSRC
* value reserved internally by CreateSession function. This function
* is normally used to create a transmit stream where SSRC value is
* randomly generated.
*/
EXPORT_C TRtpId CRtpAPI::CreateTransmitStreamL( TRtpId aSessionId, const TTranStreamParams& aParams, TRtpSSRC& aSSRC )
	{
	return iManager->CreateTransmitStreamL(aSessionId,aParams,aSSRC);
	}

/**
* Create a Transmit stream, with a given SSRC value, for an RTP Session
* and return the stream ID which is unique for all RTP Sessions. This
* extended function is used for a special case where a specific SSRC
* value needs to be associated with the transmit stream being created,
* e.g. for retransmission purpose.
*/
EXPORT_C TRtpId CRtpAPI::CreateTransmitStreamExtL( TRtpId aSessionId, const TTranStreamParams& aParams, const TRtpSSRC aSSRC )
	{
	return iManager->CreateTransmitStreamExtL(aSessionId, aParams, aSSRC);
	}

/**
* Close a Transmit or Receive stream.
*/
EXPORT_C void CRtpAPI::CloseStream( TRtpId aStreamId )
	{
	iManager->CloseStream(aStreamId);
	return;
	}

/**
* Register a callback object for receiving RTP data packets from an RTP
* Session. Only one receiver callback object is allowed to be
* registered for one Session.
* MRtpObserver::RtpPacketReceived function from aRtpObserver object is
* called when an RTP data packet is received.
*/
EXPORT_C TInt CRtpAPI::RegisterRtpObserver( TRtpId aSessionId, MRtpObserver& aRtpObserver )
	{
	return iManager->RegisterRtpObserver(aSessionId, aRtpObserver);
	}

/**
* Unregister RTP observer callback object associated with an RTP
* session.
*/
EXPORT_C void CRtpAPI::UnregisterRtpObserver( TRtpId aSessionId )
	{
	iManager->UnregisterRtpObserver(aSessionId);
	}

/**
* Sets/resets the observer for the non-RTP data.
* Only one receiver callback object is allowed to be
* registered for one Session.
*/
EXPORT_C TInt CRtpAPI::SetNonRTPDataObserver( TRtpId aSessionId, MNonRTPDataObserver* aNonRTPDataObserver )
	{
	return iManager->SetNonRTPDataObserver(aSessionId, aNonRTPDataObserver);
	}

/**
* Send an RTP data packet in a Transmit stream synchronously.
*/
EXPORT_C TInt CRtpAPI::SendRtpPacket( TRtpId aTranStreamId, const TRtpSendHeader& aHeaderInfo,
							const TDesC8& aPayloadData )
	{
	return iManager->SendRtpPacket(aTranStreamId, aHeaderInfo, aPayloadData);
	}

/**
* Send an RTP data packet in a Transmit stream asynchronously.
*/
EXPORT_C TInt CRtpAPI::SendRtpPacket( TRtpId aTranStreamId, const TRtpSendHeader& aHeaderInfo, const TDesC8& aPayloadData,
							 TRequestStatus& aStatus )
	{
	return iManager->SendRtpPacket(aTranStreamId, aHeaderInfo, aPayloadData, aStatus);
	}

/**
* Send an RTP data packet asynchronously, with a given sequence number,
* in a Transmit stream mainly for retransmission purpose.
*/
EXPORT_C TInt CRtpAPI::SendRtpPacket( TRtpId aTranStreamId, TRtpSequence aSequenceNum, const TRtpSendHeader& aHeaderInfo,
									  const TDesC8& aPayloadData, TRequestStatus& aStatus )
	{
	return iManager->SendRtpPacket(aTranStreamId, aSequenceNum, aHeaderInfo, aPayloadData, aStatus);
	}


/**
* Send a non-RTP (control) data packet asynchronously
*/
EXPORT_C void CRtpAPI::SendDataL( TRtpId aSessionId, TBool aUseRTPSocket, const TDesC8& aData, TRequestStatus& aStatus )
	{
	iManager->SendDataL(aSessionId, aUseRTPSocket, aData, aStatus);
	}

/**
* Cancel an outstanding SendRtpPacket() operation.
*/
EXPORT_C void CRtpAPI::CancelSend( TRtpId aSessionId )
	{
	iManager->CancelSend(aSessionId);
	return;
	}

/**
* Register a callback object for receiving RTCP packets associated with
* a given RTP Session. Only one observer callback object is allowed to
* be registered. One of aRtcpObserver object's callback functions is
* called when an RTCP packet of that type is received.
*/
EXPORT_C TInt CRtpAPI::RegisterRtcpObserver( TRtpId aSessionId, MRtcpObserver& aRtcpObserver )
	{
	return iManager->RegisterRtcpObserver(aSessionId, aRtcpObserver);
	}

/**
* Unregister RTCP observer callback object associated with an RTP
* session.
*/
EXPORT_C void CRtpAPI::UnregisterRtcpObserver( TRtpId aSessionId )
	{
	iManager->UnregisterRtcpObserver(aSessionId);
	return;
	}

/**
* Send an RTCP BYE packet for a Transmit stream.
*/
EXPORT_C TInt CRtpAPI::SendRtcpByePacket( TRtpId aTranStreamId, const TDesC8& aReason )
	{
	return iManager->SendRtcpByePacket(aTranStreamId, aReason);
	}

/**
* Send an RTCP APP packet for a Transmit stream.
*/
EXPORT_C TInt CRtpAPI::SendRtcpAppPacket( TRtpId aTranStreamId, const TRtcpApp& aApp )
	{
	return iManager->SendRtcpAppPacket(aTranStreamId, aApp);
	}

/** 
* Send an RTCP RR packet for a Reception stream.
*/
EXPORT_C TInt CRtpAPI::SendRtcpRrPacket( TRtpId aRcvStreamId )
	{
	return iManager->SendRtcpRrPacket(aRcvStreamId);
	}

/** 
* Send an RTCP SR packet for a Transmit stream.
*/
EXPORT_C TInt CRtpAPI::SendRtcpSrPacket( TRtpId aTranStreamId )
	{
	return iManager->SendRtcpSrPacket(aTranStreamId);
	}

		/** 
* Suspend RTCP sending on/off, calculations will continue. 
*/
EXPORT_C TInt CRtpAPI::SuspendRtcpSending( TRtpId aSessionId, TBool aAutoSending )
	{
	return iManager->SuspendRtcpSending(aSessionId, aAutoSending);
	}

/** 
* Gets the status of automatic RTCP sending.
*/
EXPORT_C TInt CRtpAPI::IsRtcpSendingSuspended( TRtpId aSessionId, TBool& aAutoSending )
	{
	return iManager->IsRtcpSendingSuspended(aSessionId, aAutoSending);
	}

/**
* Get the session ID of a stream, which belongs to that session.
*/
EXPORT_C TRtpId CRtpAPI::GetSessionId( TRtpId aStreamId )
	{
	return iManager->GetSessionId(aStreamId);
	}

/**
* Get address of Socket object used by a given RTP Session
* to send/receive RTP data packets.
*/
EXPORT_C RSocket* CRtpAPI::GetRtpSocket( TRtpId aSessionId )
	{
	return iManager->GetRtpSocket(aSessionId);
	}

/**
* Get address of Socket object used by a given RTP Session
* to send/receive RTCP control packets.
*/
EXPORT_C RSocket* CRtpAPI::GetRtcpSocket( TRtpId aSessionId )
	{
	return iManager->GetRtcpSocket(aSessionId);
	}

/**
* Retrieve statistical information for a stream
* based on the reports from RTCP SR & RR packets.
*/
EXPORT_C TInt CRtpAPI::GetStreamStatistics( TRtpId aStreamId, TRtpPeerStat& aStat )
	{
	return iManager->GetStreamStatistics(aStreamId, aStat);
	}

/**
* Get sampling rate setting for a payload type.
*/
EXPORT_C TUint32 CRtpAPI::GetSamplingRate( TUint8 aPayloadType )
	{
	return iManager->GetSamplingRate(aPayloadType);
	}

/**
* Set sampling rate for a payload type.
*/
EXPORT_C TInt CRtpAPI::SetSamplingRate( TUint8 aPayloadType, TUint32 aSamplingRate )
	{
	return iManager->SetSamplingRate(aPayloadType, aSamplingRate);
	}

/**
* Set RTCP parameters for a given RTP Session.
* This function does nothing if RTCP was not enabled previously.
*/
EXPORT_C TInt CRtpAPI::SetRtcpParameters( TRtpId aSessionId, const TRtcpParams& aRtcpParams )
	{
	return iManager->SetRtcpParameters(aSessionId, aRtcpParams);
	}

/**
* Synchronous custom command interface for future extensions.
* Input/output data and return value are defined by each custom command
*/
EXPORT_C TInt CRtpAPI::CustomCommandSync( TInt aFunction, const TDesC8& aInputData1, const TDesC8& aInputData2,
								 TDes8& aOutputData )
	{
	return iManager->CustomCommandSync(aFunction, aInputData1, aInputData2, aOutputData);	
	}

/**
* ASynchronous custom command interface for future extensions.
* Input/output data and return value are defined by each custom command
*/
EXPORT_C TInt CRtpAPI::CustomCommandAsync( TInt aFunction, const TDesC8& aInputData1, const TDesC8& aInputData2, TDes8& aOutputData,
								           TRequestStatus& aStatus )
	{
	return iManager->CustomCommandAsync(aFunction, aInputData1, aInputData2, aOutputData, aStatus);
	}
/**
* Version of RtpService.dll
* @return Version of current RtpService.dll
*/
EXPORT_C TVersion CRtpAPI::Version() const
	{
	return TVersion(0,1,1008);
	}


/**
* C++ default constructor.
*/
CRtpAPI::CRtpAPI()
	{
	}

/**
* By default Symbian 2nd phase constructor is private.
*/
void CRtpAPI::ConstructL( MRtpErrNotify& aErrNotify )
	{
	iManager = CRtpManager::NewL(aErrNotify);
	}

