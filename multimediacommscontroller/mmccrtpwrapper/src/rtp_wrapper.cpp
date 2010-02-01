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
*/

#include <e32math.h>
#include <commdbconnpref.h> 
#include <rtp.h>
#include <rtcp.h>
#include <COMMDB.H> 
#include <srtpsession.h>
#include "rtpheader.h"
#include "rtpmanager.h"

TInt RtpSessionLinearOrderFunc(const  CRtpSessionInfo& aNew, const CRtpSessionInfo &aArrElem)
	{
	return aNew.iPortNumber - aArrElem.iPortNumber;
	}

CRtpSessionInfo* CRtpSessionInfo::NewL()
	{
	LOG_FUNC_ENTRY("CRtpSessionInfo::NewL");
	CRtpSessionInfo *self = new(ELeave) CRtpSessionInfo();
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();

	LOG_FUNC_EXIT("CRtpSessionInfo::NewL");
	return self;
	}

void CRtpSessionInfo::ConstructL()
	{
	LOG_FUNC_ENTRY("CRtpSessionInfo::ConstructL");
	}

/* Creates a CrtpManager object */
CRtpManager* CRtpManager::NewL(MRtpErrNotify& aErrNotify )
	{
	LOG_FUNC_ENTRY("CRtpManager::NewL");
    CRtpManager *self = new(ELeave) CRtpManager();
	
	CleanupStack::PushL(self);
	self->ConstructL(aErrNotify);
	CleanupStack::Pop();

	LOG_FUNC_EXIT("CRtpManager::NewL");
	return self;
	}

CRtpManager::CRtpManager( )
	{
	LOG_FUNC_ENTRY("CRtpManager::CRtpManager");
	iOwnsConnection   = EFalse;
	iOwnsSocketServer = EFalse;
	LOG_FUNC_EXIT("CRtpManager::CRtpManager");
	}

CRtpManager::~CRtpManager( )
	{
	LOG_FUNC_ENTRY("CRtpManager::~CRtpManager");
	
	Close();
	
	/* Clean up the Arrays */
	iRtpSessionArr.Close();
	iRtpStreamsArr.Close();
	
	if(iOwnsConnection)
		delete iConnection;
	if(iOwnsSocketServer)
		delete iSocketServ;
	
	LOG_FUNC_EXIT("CRtpManager::~CRtpManager");
	}

void CRtpManager::ConstructL(MRtpErrNotify& aErrNotify )
	{
	LOG_FUNC_ENTRY("CRtpManager::ConstructL");
	iRtpErrNotify = &aErrNotify;
	LOG_FUNC_EXIT("CRtpManager::ConstructL");
	}

        
TInt CRtpManager::OpenL( const TRtpSdesParams& aSdesInfo,  const TDesC* aRtpPacketDll,
                         const RSocketServ* aSocketServPtr, const RConnection* aConnPtr )
	{
	LOG_FUNC_ENTRY("CRtpManager::OpenL");
	/* The current implementation do not support having a different
	 * aRtpPacketDll
	 * So the argument is ignored */
	(void)aRtpPacketDll;

	if(aSocketServPtr)
		{
		iSocketServ = const_cast<RSocketServ*>(aSocketServPtr);
		}
	else
		{
		iSocketServ = new(ELeave) RSocketServ();
		iOwnsSocketServer = ETrue;
		}
	
	if(aConnPtr)
		{
		iConnection = const_cast<RConnection*>(aConnPtr);
		}
	else
		{
		iConnection = new(ELeave) RConnection();
		iOwnsConnection = ETrue;
		}
	
	iSdesInfo  = aSdesInfo;

	User::LeaveIfError(iSocketServ->Connect());
	
	User::LeaveIfError(iConnection->Open(*iSocketServ));
	
	LOG_FUNC_EXIT("CRtpManager::OpenL");
	return KErrNone;
	}


 /**
  * Open and initialize the CRtpAPI object.
  * @param aIapId - [input] IAP ID. If -1, no IAP selection dialog
  *   will pop up. Instead, the default IAP will be used.
  * @return KErrNone if successful; system wide error code otherwise
 */
TInt CRtpManager::StartConnection( TInt aIapId )
	{
	LOG_FUNC_ENTRY("CRtpManager::StartConnection");
	__RTP_LOG1("IapId = %d",aIapId)
	if(aIapId != -1)
		{
		TCommDbConnPref prefs;
		prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
		prefs.SetDirection(ECommDbConnectionDirectionOutgoing);
		prefs.SetIapId(aIapId);
		iIapId = aIapId;
		return iConnection->Start(prefs);
		}
	else
		{
		TRAPD(err,iIapId = GetDefaultIapIdL());
		if(KErrNone != err)
			{
			__RTP_LOG1("GetDefaultIapIdL returned  %d",err)
			return err;
			}
		__RTP_LOG1("GetDefaultIapIdL default Iap:  %d",iIapId)
		LOG_FUNC_EXIT("CRtpManager::StartConnection");
		return iConnection->Start();
		}
	}

TInt CRtpManager::StartConnection( TRequestStatus& aStatus, TInt aIapId)
	{
	LOG_FUNC_ENTRY("CRtpManager::StartConnection-Async");
	__RTP_LOG1("aIapId = %d",aIapId);

	TCommDbConnPref prefs;
	prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
	prefs.SetDirection(ECommDbConnectionDirectionOutgoing);
	
	if(aIapId != -1)
		{
		prefs.SetIapId(aIapId);	
		iConnection->Start(prefs, aStatus);
		iIapId = aIapId;
		}
	else
		{
		TRAPD(err,iIapId = GetDefaultIapIdL());
		if(KErrNone != err)
			{
			__RTP_LOG1("GetDefaultIapIdL returned  %d",err)
			return err;
			}
		__RTP_LOG1("GetDefaultIapIdL default Iap:  %d",iIapId)
		prefs.SetIapId(iIapId);
		iConnection->Start(prefs, aStatus);
		}
	LOG_FUNC_EXIT("CRtpManager::StartConnection-Async");
	return KErrNone;
	}


/**
* Cancels asynchoronous start of connection.
* Any open requests will be completed with KErrCancel.
* @return None
*/
void CRtpManager::CancelStart()
	{
	LOG_FUNC_ENTRY("CRtpManager::CancelStart");
	return;
	}


void CRtpManager::Close()
	{
	LOG_FUNC_ENTRY("CRtpManager::Close");
	/* Iterate through the session array and close */
	for(int i=0; i < iRtpSessionArr.Count(); i++)
		{
		__RTP_LOG1("Closing Session Key %u",iRtpSessionArr[i]->iKey);
		iRtpSessionArr[i]->iRtpSession.Close();
		ASSERT(iRtpSessionArr[i]->iPendingReqQue->IsEmpty());
		delete iRtpSessionArr[i]->iPendingReqQue;
		iRtpSessionArr[i]->iPendingReqQue = NULL;
		}
	if(iConnection)
		iConnection->Close();
	
	if(iSocketServ)
		iSocketServ->Close();
	
	LOG_FUNC_EXIT("CRtpManager::Close");
	}

/**
* Set SDES (Source Description) information of the local participant.
* Participant is defined as an application program and
* only one local participant is allowed.
*/
 void CRtpManager::SetLocalSdes( const TRtpSdesParams& aSDES)
	{
	/* Should the SDES list be updated again here ? */
	LOG_FUNC_ENTRY("CRtpManager::SetLocalSdes");
	iSdesInfo = aSDES;
	}

/**
* Get the local IP address
* @param None
* @return local ip address.
*/
 TInetAddr& CRtpManager::GetLocalIPAddressL()
	{
	LOG_FUNC_ENTRY("CRtpManager::GetLocalIPAddressL");
	/* Open an RSocket and find its address */
	RSocket socket;
	User::LeaveIfError(socket.Open(*iSocketServ, KAfInet, KSockDatagram, KProtocolInetUdp,*iConnection));
    
	CleanupClosePushL(socket);

	TSoInetInterfaceInfo networkInfo;
	TPckg<TSoInetInterfaceInfo> opt(networkInfo);
	User::LeaveIfError(socket.SetOpt(KSoInetEnumInterfaces, KSolInetIfCtrl));
	TSoInetIfQuery query;
	TPckg<TSoInetIfQuery> queryBuf(query);
	TInt res = KErrNone;
	do 
	{
		res = socket.GetOpt(KSoInetNextInterface, KSolInetIfCtrl, opt);
		if (!res && opt().iState == EIfUp)
		{
			query.iName = opt().iName;
			res = socket.GetOpt(KSoInetIfQueryByName, KSolInetIfQuery, queryBuf);
			if (!res && query.iZone[1] == iIapId)
			{
				networkInfo = opt();
				break;
			}
		}
	}
	while (res == KErrNone);
    if(res!= KErrNone)
    	{
    	__RTP_LOG1("Found nothing! ret %d",res)
    	/* Didn't find :( */
    	User::Leave(res);
    	}
	CleanupStack::PopAndDestroy(1);
    iSockAddr = networkInfo.iAddress;
    LOG_FUNC_EXIT("CRtpManager::GetLocalIPAddressL");
	return iSockAddr;
	}
 /**
 * Makes a CName that is unique for a given seesion 
 */
void CRtpManager::MakeACnameL(TDes8 &aCname)
	{
	/* As of now putting the IP Address. But this is not enough.
	 * multiple sessions should have unique CNAMES 
	 * */
	TBuf<50> ipAddr;
	TInetAddr locAddr = GetLocalIPAddressL();
	locAddr.Output(ipAddr);
	aCname.Format(_L8("%d@"),iRtpSessionArr.Count()+1);
	aCname.Append(ipAddr);
	}
/**
* Create a new RTP Session and return the Session ID as well as the
* local port number assigned for RTP. RTP uses an even port number and
* RTCP, if enabled, uses the next higher (odd) port number.
*/
 TRtpId CRtpManager::CreateSessionL(const TCreateSessionParams& aSessionParams,
                           	                TUint& aPort, TBool aEnableRtcp, const TRtcpParams* aRtcpParams )
	{
	LOG_FUNC_ENTRY("CRtpManager::CreateSessionL");
	__RTP_LOG1("RTCP stat %d",aEnableRtcp)
	TInt localPort;
	/* The RTP Session create takes in the LocalPort.*/
	if(iRtpSessionArr.Count() == 0)
		{
		//In this case start from begining. Start by using the default Even RTP
		//Port.
		localPort = KDefaultStartRtpAddress;
		__RTP_LOG1("LocalPort used %d",localPort)
		}
	else
		{
		/* Pick the next highest port number */
		localPort = iRtpSessionArr[iRtpSessionArr.Count()-1]->iPortNumber + 2;
		__RTP_LOG1("LocalPort used %d",localPort)
		}
	
	//if the user has specified a port then override our selection
	if(aPort)
		{
		localPort = aPort;
		}
	
	//Now use this Port and try opening the RTP Session. If the Session Open
	//fails try opening with the next even port.
    //Its much more easier to use the API which takes RSocket directly here. But
	//ths would mean we cannot use the New CFRTP API.
	TInetAddr locAddr(0,localPort);

	/* Remote address is uninitialized. Unfortunately its required by Symbian
	 * RTP stack. */
	TInetAddr remAddr;
	
	#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
	RRtpSession rtpSession;
	#else
	RRtpSession_Internal rtpSession;
	#endif	
 	
	TPtrC8 cnameTptr((TUint8*)"", 0);;
	if(aEnableRtcp)
		{
		TBuf8<50> cnameBuf;
		if(iSdesInfo.iCName == KNullDesC8)
			{
			MakeACnameL(cnameBuf);
			iSdesInfo.iCName.Set(cnameBuf);
			}
		cnameTptr.Set(iSdesInfo.iCName);
		}
	else
		{
		cnameTptr.Set(KNullDesC8);
		}

	TInt tryCount = 0;
	TInt errOpenl = KErrNone;

	__RTP_LOG1("Starting Port alloc and OpenL tryCount %d",tryCount);
	__RTP_LOG1("aSessionParams.iSocketBufSize    %d",aSessionParams.iSocketBufSize);	
   aPort = locAddr.Port();

    while(tryCount < 5 )
		{
    	TRAP(errOpenl,rtpSession.OpenL(*iSocketServ, locAddr, remAddr, aSessionParams.iSocketBufSize, *iConnection, EPriorityNormal, cnameTptr));
		if(errOpenl != KErrNone)
			{
			__RTP_LOG2("OpenL Failed with Error Code %d TryCount %d",errOpenl, tryCount);
			tryCount ++;
			locAddr.SetPort(locAddr.Port()+tryCount+1);
			localPort += tryCount+1;
			}
		else
			{
			break;
			}
		}

    User::LeaveIfError(errOpenl);
    
	__RTP_LOG(",rtpSession.OpenL Success!!");

	TTimeIntervalMicroSeconds32 rtcpInterval;
	if(aEnableRtcp && aRtcpParams)
		{
		rtpSession.SetBandwidth(aRtcpParams->iSessionBWidth);
		__RTP_LOG1(" aRtcpParams->iRtcpTimeOut %d", aRtcpParams->iRtcpTimeOut.Int());
		if(aRtcpParams->iRtcpTimeOut.Int())
		    {
			rtcpInterval = aRtcpParams->iRtcpTimeOut.Int();
		    }
		else
		    {
		    //TODO fix me!
		    //default 500 ms
		    rtcpInterval = 1000000;
		    }
        rtpSession.SetRtcpInterval(rtcpInterval);
		}
	else
		{
		__RTP_LOG("Setting default RTCP Properties!!");
		//Default Bandwidth.
		rtpSession.SetBandwidth(KRtpDefaultBandWidth);	
		rtcpInterval = 1000000;
		rtpSession.SetRtcpInterval(rtcpInterval);
		}
	
	//Phew! Now put these in our Array
	CRtpSessionInfo* sessionInfo = CRtpSessionInfo::NewL();
	//Assign a Key for this session.
	sessionInfo->iKey         = Math::Random();
	sessionInfo->iPortNumber  = localPort;
	sessionInfo->iRtpSession  = rtpSession;
	sessionInfo->iRtpManager  = this;
	sessionInfo->iRtcpEnabled = aEnableRtcp;
	sessionInfo->iPendingReqQue = new(ELeave)  TSglQue<TRtpSendReqNode>(_FOFF(TRtpSendReqNode,iLink));
	iRtpSessionArr.InsertInOrder(sessionInfo, TLinearOrder<CRtpSessionInfo>(RtpSessionLinearOrderFunc)); //InsertInOrder(sessionInfo,TLinearOrder<CRtpSessionInfo>(RtpSessionLinearOrderFunc));

	__RTP_LOG1("Session Created Key %u",sessionInfo->iKey)
    //Get back the reference to the object in Array.
	TInt idx = FindSessionL(sessionInfo->iKey);
	
    //Register for the Events  
	RegisterEventsOnSessionL(iRtpSessionArr[idx]);
	
	aPort = localPort;

	//TODO: Fix this in RTP Stack.
	/* Remote Address is uninitialized now. So if RTCP is enabled disable autosend so that
	 * no data is send to incorrect address.
	 */
	if(aEnableRtcp)
		{
		rtpSession.SetRTCPAutoSend(EFalse);
		}


	LOG_FUNC_EXIT("CRtpManager::CreateSessionL");

	return sessionInfo->iKey;
	}
   

/**
 *Create a new, secure RTP Session and return the Session ID as well as the
* local port number assigned for RTP. RTP uses an even port number and
* RTCP, if enabled, uses the next higher (odd) port number.
* User is expected to create corresponding SRTP session prior to calling this
* function. 
*/
 TRtpId CRtpManager::CreateSessionL( const TCreateSessionParams& aSessionParams, TUint& aPort, 
	                				TBool aEnableRtcp, const TRtcpParams* aRtcpParams, CSRTPSession& aSRTPSession)
	{
	LOG_FUNC_ENTRY("CRtpManager::CreateSessionL");
	__RTP_LOG("SRTP is not supported Leaving..!!")
	
	TUint32 key = CreateSessionL(aSessionParams, aPort, aEnableRtcp, aRtcpParams);
	TInt idx = FindSessionL(key);
	iRtpSessionArr[idx]->iSRTPSession = &aSRTPSession;
	iRtpSessionArr[idx]->iRtpSession.SetPrePostProcessingRegisterCallback(iRtpSessionArr[idx]);
	return key;
	}
                            
/**
* Start an RTP Session. If enabled, RTCP associated with the given
* session is also started.
*/
 TInt CRtpManager::StartSession( TRtpId aSessionId )
	{
	LOG_FUNC_ENTRY("CRtpManager::StartSession");
	if(FindSession(aSessionId)!= KErrNotFound)
		{
		/* Well we found the session. But Really nothing to do */
		//TODO verify
		return KErrNone;
		}
	else
		{
		return KErrNotFound;
		}
	}

/**
* Close an RTP Session.
*/
 void CRtpManager::CloseSession( TRtpId aSessionId )
	{
	LOG_FUNC_ENTRY("CRtpManager::CloseSession");
	__RTP_LOG1("SessID %u",aSessionId)
	TInt idx;
	idx = FindSession(aSessionId);
	if(KErrNotFound != idx)
		{
		/* This takes care of all cleaning up */
		iRtpSessionArr[idx]->iRtpSession.Close();
		/* There should be no more Pending requests on this session */
		ASSERT(iRtpSessionArr[idx]->iPendingReqQue->IsEmpty());
		delete iRtpSessionArr[idx]->iPendingReqQue;
		CRtpSessionInfo *session = iRtpSessionArr[idx];
		iRtpSessionArr.Remove(idx);
		delete session;
		}
	else
		{
		__RTP_LOG("Session not found")
	    //So already cleaned?	
		}
	LOG_FUNC_EXIT("CRtpManager::CloseSession");
	}

/**
* Set remote IP address and port number to RTP Session.
* Port number for RTP must be an even number and the corresponding
* RTCP, if enabled, will be set to use the next higher (odd) port.
*/
 TInt CRtpManager::SetRemoteAddress( TRtpId aSessionId,const TInetAddr& aRemoteAddr )
	{
	LOG_FUNC_ENTRY("CRtpManager::SetRemoteAddress");
	__RTP_LOG1("SessID %u",aSessionId)
	__RTP_LOG_STMT(TBuf<50> buf; aRemoteAddr.Output(buf););
	__RTP_LOG2("RemoteAddressAddress %S, Port %u", &buf, aRemoteAddr.Port());
	TInt ret;
	TInetAddr addr = aRemoteAddr;
	if((ret =FindSession(aSessionId))!= KErrNotFound)
		{
		iRtpSessionArr[ret]->iRtpSession.SetRemoteAddress(addr);
		iRtpSessionArr[ret]->iRemoteAddress = aRemoteAddr;
		/* SET the RTCP address as well here */
		iRtpSessionArr[ret]->iRemoteRTCPAddress = aRemoteAddr;
		iRtpSessionArr[ret]->iRemoteRTCPAddress.SetPort(aRemoteAddr.Port()+1);

		return KErrNone;
		}
	else
		{
		return KErrNotFound;
		}
	}

/**
* Set remote RTCP IP address and port number to RTP Session accoring to RFC 3605
* i.e. this should be used if RTCP port is different than RTP port + 1
*/
TInt CRtpManager::SetRemoteRtcpAddress( TRtpId aSessionId, const TInetAddr&  aRemoteAddr)
	{
	LOG_FUNC_ENTRY("CRtpManager::SetRemoteRtcpAddress");
	__RTP_LOG1("SessID %u",aSessionId)
	__RTP_LOG_STMT(TBuf<50> buf; aRemoteAddr.Output(buf););
	__RTP_LOG2("RemoteAddressAddress %S, Port %u", &buf, aRemoteAddr.Port());
	TInt ret;
	TInt port = aRemoteAddr.Port();
	if((ret =FindSession(aSessionId))!= KErrNotFound)
		{
		iRtpSessionArr[ret]->iRtpSession.SetRemoteRtcpPort(port);
		iRtpSessionArr[ret]->iRemoteRTCPAddress = aRemoteAddr;
        
		/*
		//VS Experiment. Try sending Door opener here to Remote end
    	TRequestStatus 	stat;
		__RTP_LOG("CRtpManager::SetRemoteAddress -- Sending DoorOpener --RTCP");
		RSocket *sock = NULL;
		sock = GetRtcpSocket(aSessionId);
		sock->SendTo(_L8("OpenSesame"),iRtpSessionArr[ret]->iRemoteAddress,0,stat);
		User::WaitForRequest(stat);
        __RTP_LOG1("DoorOpener --RTCP sent retCode %d", stat.Int());
		LOG_FUNC_EXIT("CRtpManager::SetRemoteRtcpAddress");
		*/
		return KErrNone;
		}
	else
		{
		return KErrNotFound;
		}
	}
        
/**
* Create a Receive stream for an RTP Session and return the stream
* ID which is unique for all RTP Sessions.
*/
 TRtpId CRtpManager::CreateReceiveStreamL( TRtpId aSessionId, const TRcvStreamParams& aParams )
	{
	LOG_FUNC_ENTRY("CRtpManager::CreateReceiveStreamL");
	__RTP_LOG1("SessID %u",aSessionId)
	/* Valide the session */
	FindSessionL(aSessionId);
	
	/* Check for Post-Created Streams. If present use it */
	for(TInt i=0; i < iRtpStreamsArr.Count(); i++)
		{
		if(iRtpStreamsArr[i].iSessionKey == aSessionId &&
				iRtpStreamsArr[i].iState == TRtpStreamInfo::eStreamPostCreated && 
				iRtpStreamsArr[i].iPayloadType == aParams.iPayloadType	)
			{
			__RTP_LOG1("Found Post Created Stream Key %u",iRtpStreamsArr[i].iKey)
			/* Post created stream exists. Return it */
			iRtpStreamsArr[i].iState = TRtpStreamInfo::eStreamReadyToUse;
			return iRtpStreamsArr[i].iKey;
			}
		}
	
	/* OK found nothing. So create a Pre-Created stream and return its ID */
	TRtpStreamInfo strmInfo;
	strmInfo.iKey = Math::Random();
	strmInfo.iSessionKey = aSessionId;
	strmInfo.iState = TRtpStreamInfo::eStreamPreCreated;
	strmInfo.iPayloadType = aParams.iPayloadType;
	strmInfo.iRtpManager = this;
	
	iRtpStreamsArr.AppendL(strmInfo);
	__RTP_LOG1("Created PreCreated Stream Key %u",strmInfo.iKey)
	
	LOG_FUNC_EXIT("CRtpManager::CreateReceiveStreamL");
	return strmInfo.iKey;
	}

 /**
  * Create a Transmit stream for an RTP Session and return the stream
  * ID which is unique for all RTP Sessions. SSRC value assigned for the
  * very first Transmit stream will be the same as the default SSRC
  * value reserved internally by CreateSession function. This function
  * is normally used to create a transmit stream where SSRC value is
  * randomly generated.
  */
   TRtpId CRtpManager::CreateTransmitStreamL( TRtpId aSessionId,
                      const TTranStreamParams& aParams,
                      TRtpSSRC& aSSRC )
	   {
	   LOG_FUNC_ENTRY("CRtpManager::CreateTransmitStreamL");
	   __RTP_LOG1("SessID %u",aSessionId)
	   TInt idx = FindSessionL(aSessionId);
	   if(!iRtpSessionArr[idx]->iRtpSendSource.IsOpen())
		   {
		   iRtpSessionArr[idx]->iRtpSendSource = iRtpSessionArr[idx]->iRtpSession.NewSendSourceL();
		   /* Now register for events in this session */
		   iRtpSessionArr[idx]->iRtpSendSource.RegisterEventCallbackL(ERtpSendSucceeded,
				                                          RtpSessionLevelCB,
				                                          iRtpSessionArr[idx],
				                                          ERtpNotOneShot);
		   iRtpSessionArr[idx]->iRtpSendSource.RegisterEventCallbackL(ERtpSendFail,
				                                          RtpSessionLevelCB,
				                                          iRtpSessionArr[idx],
				                                          ERtpNotOneShot);
		   aSSRC = iRtpSessionArr[idx]->iRtpSendSource.GetLocalSSRC();
		   
		   iRtpSessionArr[idx]->iPayloadType = aParams.iPayloadType;
		   
		   iRtpSessionArr[idx]->iRtpSendSource.SetPayloadType(aParams.iPayloadType);
		   
               
           //Note: The key is same as the SessionKey. We donot support
           //more than one Transmit Streams in one Session.
           LOG_FUNC_EXIT("CRtpManager::CreateTransmitStreamL");
               
           return iRtpSessionArr[idx]->iKey;
           }
	   __RTP_LOG("A SendStream was already Opened!")
	   //Already called still there might be Applications calling this
	   //API twice
	   //
	   LOG_FUNC_EXIT("CRtpManager::CreateTransmitStreamL");
	   return 0;
	   }

  /**
  * Create a Transmit stream, with a given SSRC value, for an RTP Session
  * and return the stream ID which is unique for all RTP Sessions. This
  * extended function is used for a special case where a specific SSRC
  * value needs to be associated with the transmit stream being created,
  * e.g. for retransmission purpose.
  */
   TRtpId CRtpManager::CreateTransmitStreamExtL( TRtpId , const TTranStreamParams& ,
                      							const TRtpSSRC )
	   {
	   LOG_FUNC_ENTRY("CRtpManager::CreateTransmitStreamL");
	   __RTP_LOG("NotSuppoted SSRC cannot be changed")
	   //This is not supported as of now.
	   //SSRC cannot be specified by Applications.
	   return 0;
	   }


   /**
    * Close a Transmit or Receive stream.
    */
void CRtpManager::CloseStream( TRtpId aRtpID)
	   {
	   LOG_FUNC_ENTRY("CRtpManager::CloseStream");
	   __RTP_LOG1("Stream ID %u",aRtpID)
       (void)aRtpID;
	   //Streams are closed when session is closed.
	   //so no requirement for an explicit close
	   //TODO Verify Requirements
	   return;
	   }

/**
 * Register a callback object for receiving RTP data packets from an RTP
 * Session. Only one receiver callback object is allowed to be
 * registered for one Session.
 */
TInt CRtpManager::RegisterRtpObserver( TRtpId aSessionId, MRtpObserver& aRtpObserver )
	{
	LOG_FUNC_ENTRY("CRtpManager::RegisterRtpObserver");
	__RTP_LOG1("SessID %u",aSessionId)
	TInt ret = FindSession(aSessionId);
	if(ret < 0)
		return ret;
	iRtpSessionArr[ret]->iRtpObserver = &aRtpObserver;
	return KErrNone;
	}

 /**
  * Unregister RTP observer callback object associated with an RTP
  * session.
  */
void CRtpManager::UnregisterRtpObserver(TRtpId aSessionId)
	{	
	LOG_FUNC_ENTRY("CRtpManager::UnregisterRtpObserver");
	__RTP_LOG1("SessID %u",aSessionId)

	TInt ret = FindSession(aSessionId);
	if(ret >= 0)
		iRtpSessionArr[ret]->iRtpObserver = NULL;
	}

/**
* Sets/resets the observer for the non-RTP data.
* Only one receiver callback object is allowed to be
* registered for one Session.
* called when a non-RTP data packet is received.
*/
 TInt CRtpManager::SetNonRTPDataObserver( TRtpId aSessionId, MNonRTPDataObserver* aNonRtpObserver)
	{
	LOG_FUNC_ENTRY("CRtpManager::SetNonRTPDataObserver");
	__RTP_LOG1("SessID %u",aSessionId);
	
	TInt ret = FindSession(aSessionId);
	if(ret < 0)
		return ret;
	iRtpSessionArr[ret]->iNonRtpDataObserver = aNonRtpObserver;
	
	TRAPD(err,iRtpSessionArr[ret]->iRtpSession.RegisterEventCallbackL(ENonRtpDataReceived,
			                                          RtpSessionLevelCB,
			                                          iRtpSessionArr[ret],
			                                          ERtpOneShot));
	if(KErrNone != err)
		{
		__RTP_LOG1("err %d",err);
		return err;
		}

	TRAP(err,iRtpSessionArr[ret]->iRtpSession.RegisterEventCallbackL(ENonRtcpDataReceived,
				                                          RtpSessionLevelCB,
				                                          iRtpSessionArr[ret],
				                                          ERtpOneShot));
	
	if(KErrNone != err)
		{
		/* The already registered callback will be deleted when the first
		 * NonRtp data comes
		 */
		iRtpSessionArr[ret]->iNonRtpDataObserver = 0;
		}
	return err;
	}

/**
* Send an RTP data packet in a Transmit stream synchronously.
*/
 TInt CRtpManager::SendRtpPacket( TRtpId aTranStreamId,
                             const TRtpSendHeader& aHeaderInfo,
                             const TDesC8& aPayloadData )
	{
	LOG_FUNC_ENTRY("CRtpManager::SendRtpPacket");
	__RTP_LOG1("aTranStreamId: %u",aTranStreamId)

	/* The Transmit Id and Session ID are same. So
	 * find the session */
	TInt ret = FindSession(aTranStreamId);
	if(ret<0)
		return ret;
	ASSERT(iRtpSessionArr[ret]->iRtpSendSource.IsOpen());
	
	RRtpSendPacket sendPkt;
	TInt pktLen = 0;
    pktLen += aPayloadData.Length();
    __RTP_LOG1("pktLen: %d", pktLen );	
    if(iRtpSessionArr[ret]->iSRTPSession)
		{
		/* SRTP is enabled so allocate more bytes for MKI and AuthTag */
		pktLen += ( KSrtpMaxAuthTagLength + KSrtpMaxMKILen );
	    __RTP_LOG1("Added extra bytes for SRTP new pktLen: %d", pktLen );	
		}	
	TRAPD(err,sendPkt = iRtpSessionArr[ret]->iRtpSendSource.NewSendPacketL(pktLen, aHeaderInfo.iHeaderExtension?aHeaderInfo.iHeaderExtension->iLength:KRtpNoExtension));
	if(err != KErrNone)
		return err;
	
	__RTP_LOG2("TS: %u, PayloadType %d",aHeaderInfo.iTimestamp, aHeaderInfo.iPayloadType)
	sendPkt.SetTimestamp(aHeaderInfo.iTimestamp);
	sendPkt.SetMarker(aHeaderInfo.iMarker);
	sendPkt.SetPayloadType(aHeaderInfo.iPayloadType);
	
	if(iRtpSessionArr[ret]->iFirstPacketSent == EFalse)
	    {
	    iRtpSessionArr[ret]->iRtpSession.SetRTPTimeConversion(aHeaderInfo.iTimestamp, iRtpSessionArr[ret]->iSamplingRate/1000);
	    iRtpSessionArr[ret]->iFirstPacketSent = ETrue;
	    }

	TDes8 &wrtPayload = sendPkt.WritePayload();
	wrtPayload = aPayloadData;
	
	ret = sendPkt.SendSync();
	
	//TODO pre allocate a Max Size
	sendPkt.Close();
	
	LOG_FUNC_EXIT("CRtpManager::SendRtpPacket");	
	return ret;
	}
 
 
 /**
   * Send an RTP data packet in a Transmit stream asynchronously.
   */
 TInt CRtpManager::SendRtpPacket( TRtpId aTranStreamId,
                                const TRtpSendHeader& aHeaderInfo,
                                const TDesC8& aPayloadData,
                                TRequestStatus& aStatus )
	{
	LOG_FUNC_ENTRY("CRtpManager::SendRtpPacket");
	__RTP_LOG1("aTranStreamId: %u",aTranStreamId)
	/* The Transmit Id and Session ID are same. So
	 * find the session */
	TInt ret = FindSession(aTranStreamId);
	if(ret<0)
		return ret;
	ASSERT(iRtpSessionArr[ret]->iRtpSendSource.IsOpen());
	
	RRtpSendPacket sendPkt;
	TInt pktLen = 0;
    pktLen += aPayloadData.Length();
    __RTP_LOG1("pktLen: %d", pktLen );	
    if(iRtpSessionArr[ret]->iSRTPSession)
		{
		/* SRTP is enabled so allocate more bytes for MKI and AuthTag */
		pktLen += ( KSrtpMaxAuthTagLength + KSrtpMaxMKILen );
	    __RTP_LOG1("Added extra bytes for SRTP new pktLen: %d", pktLen );	
		}	
	TRAPD(err,sendPkt = iRtpSessionArr[ret]->iRtpSendSource.NewSendPacketL(pktLen,aHeaderInfo.iHeaderExtension?aHeaderInfo.iHeaderExtension->iLength:KRtpNoExtension));
	if(err != KErrNone)
		return err;

    __RTP_LOG2("TS: %u, PayloadType %d",aHeaderInfo.iTimestamp, aHeaderInfo.iPayloadType);	
	sendPkt.SetTimestamp(aHeaderInfo.iTimestamp);
	sendPkt.SetMarker(aHeaderInfo.iMarker);
	sendPkt.SetPayloadType(aHeaderInfo.iPayloadType);

	
	if(iRtpSessionArr[ret]->iFirstPacketSent == EFalse)
		{
		iRtpSessionArr[ret]->iRtpSession.SetRTPTimeConversion(aHeaderInfo.iTimestamp, 1000*1000/iRtpSessionArr[ret]->iSamplingRate);
		iRtpSessionArr[ret]->iFirstPacketSent = ETrue;
		}

	
	TDes8 &wrtPayload = sendPkt.WritePayload();
	wrtPayload = aPayloadData;
	
	/* This is an Async Send. So insert this packet in to the 
	 * the send queue.
	 */
	TRtpSendReqNode *sendReqNode = NULL;
	sendReqNode = new TRtpSendReqNode();
	if(!sendReqNode)
		{
		sendPkt.Close();
		return KErrNoMemory;
		}
	
	sendReqNode->iPktToSend = sendPkt;
	sendReqNode->iStatus = &aStatus;
	iRtpSessionArr[ret]->iPendingReqQue->AddFirst(*sendReqNode);
	*sendReqNode->iStatus = KRequestPending;
	/* If this is the only pending element in the queue schedule immediately */
	if(iRtpSessionArr[ret]->iPendingReqQue->IsLast(sendReqNode))
		{
		__RTP_LOG("Async Send ..Sending Immediately!");
	    return SendRtpDataFromPendingQueue(iRtpSessionArr[ret]);	
		}
	
	__RTP_LOG("Async Send ..Packet Queued!");
	//TODO pre allocate a Max Size
    // The packet will be closed when Send is completed
	return KErrNone;
	}

 /**
   * Send an RTP data packet asynchronously, with a given sequence number,
   * in a Transmit stream mainly for retransmission purpose.
 */
 TInt CRtpManager::SendRtpPacket( TRtpId aTranStreamId,
                                TRtpSequence ,
                                const TRtpSendHeader& aHeaderInfo,
                                const TDesC8& aPayloadData,
                                TRequestStatus& aStatus )
	{
    LOG_FUNC_ENTRY("CRtpManager::SendRtpPacket - With Sequence Called!!");
    //TODO We SHOULD consider the Sequence number as well!	
	return SendRtpPacket(aTranStreamId, aHeaderInfo, aPayloadData, aStatus) ;
	}


/**
  * Send a non-RTP (control) data packet asynchronously
*/
 void CRtpManager::SendDataL( TRtpId aSessionId, TBool aUseRTPSocket, 
		                      const TDesC8& aData, TRequestStatus&  aStatus)
	{
	LOG_FUNC_ENTRY("CRtpManager::SendDataL");
	__RTP_LOG1("aSessionId: %u",aSessionId)
	TInt ret = FindSessionL(aSessionId);
	RSocket *sock = NULL;
	TInetAddr remAddr = iRtpSessionArr[ret]->iRemoteAddress;
	if(aUseRTPSocket)
		{
		sock = GetRtpSocket(aSessionId);
		}
	else
		{
		if(!iRtpSessionArr[ret]->iRtcpEnabled)
			User::Leave(KErrNotSupported);
		sock = GetRtcpSocket(aSessionId);
		remAddr = iRtpSessionArr[ret]->iRemoteRTCPAddress;
		}
	/* Send the data out through the Socket */
	sock->SendTo(aData,remAddr,0,aStatus);
	return;
	}

 /**
  * Cancel an outstanding SendRtpPacket() operation.
  */
 void CRtpManager::CancelSend( TRtpId aSessId)
	 {
	 /* Its a Bit tricky here. 
	  * We have two levels of Active Objects and the Symbian stack
	  * do not provide any API to cancel the Send request. As a workarnd we accesses the
	  * RTP Socket directly and cancels the send ( RSocket::CancelSend())
	  * Internal statistics are not rolled back in this case, but surely we can
	  * live with that.
	  * 
	  * The Active object who issued this call will do a User::WaitForRequest() 
	  * immediatly after calling Cancel. (See CActive::Cancel() implementation).
	  * This means we should complete the request with KErrCancel here itself or
	  * thread will block (Because we are not exactly an Async Service provider).
	  * Once the request is completed we can expect a ESendFail callback or an
	  * ESendSuccess callback from stack ( Second one beacuse the request might have
	  * been completed already).
	  */
	LOG_FUNC_ENTRY("CRtpManager::CancelSend");
	__RTP_LOG1("SessId: %u",aSessId)

	TInt ret = FindSession(aSessId);
	if(ret < 0)
		{
		ASSERT(ret!=KErrNone);
		return;
		}
	if(iRtpSessionArr[ret]->iPendingReqQue->IsEmpty())
		{
		/* There is no pending request. This probably means the request was
		 * already completed but the App is yet to see it..
		 */
		__RTP_LOG("NO pending request .. Returning!")
		return;
		}
	
	TRtpSendReqNode *reqNode = iRtpSessionArr[ret]->iPendingReqQue->Last();
    /* Once send is canceled complete Applications request. Also if the pending
	 * queue is not empty schedule a new transmission here itself */
	iRtpSessionArr[ret]->iRtpSendSource.Cancel();
	User::RequestComplete(reqNode->iStatus, KErrCancel);
	iRtpSessionArr[ret]->iPendingReqQue->Remove(*reqNode);
	reqNode->iPktToSend.Close();
	delete reqNode;

	if(!iRtpSessionArr[ret]->iPendingReqQue->IsEmpty())
		{
		__RTP_LOG("Scheduling another send from PendingQueue")
		SendRtpDataFromPendingQueue(iRtpSessionArr[ret]);
		}

	return;
	}

  /**
  * Register a callback object for receiving RTCP packets associated with
  * a given RTP Session. Only one observer callback object is allowed to
  * be registered. One of aRtcpObserver object's callback functions is
  * called when an RTCP packet of that type is received.
  */
 TInt CRtpManager::RegisterRtcpObserver( TRtpId aSessionId, MRtcpObserver& aRtcpObserver )
	 {
	LOG_FUNC_ENTRY("CRtpManager::RegisterRtcpObserver");
	__RTP_LOG1("SessId: %u",aSessionId)

	 TInt ret = FindSession(aSessionId);
	 if(ret < 0)
	 	return ret;
	 iRtpSessionArr[ret]->iRtcpObserver = &aRtcpObserver;
	 return KErrNone;
	 }

  /**
  * Unregister RTCP observer callback object associated with an RTP
  * session.
  */
 void CRtpManager::UnregisterRtcpObserver( TRtpId aSessionId )
	 {
	 LOG_FUNC_ENTRY("CRtpManager::UnregisterRtcpObserver");
	 __RTP_LOG1("SessId: %u",aSessionId)

	 TInt ret = FindSession(aSessionId);
	 if(ret >= 0)
		 iRtpSessionArr[ret]->iRtcpObserver = NULL; 
	 return;
	 }
 /**
 * Send an RTCP APP packet for a Transmit stream.
 */
 TInt CRtpManager::SendRtcpAppPacket( TRtpId aTranStreamId, const TRtcpApp& aApp )
	 {
	 LOG_FUNC_ENTRY("CRtpManager::SendRtcpAppPacket");
	 __RTP_LOG1("aTranStreamId: %u",aTranStreamId)

	 /* A send stream should be open to send an RTCP Bye packet */
	 //Transmit Stream ID and Session Id are same
	 TInt ret = FindSession(aTranStreamId);
	 if(ret < 0)
	 	return ret;
	 ASSERT(iRtpSessionArr[ret]->iRtcpEnabled);
	 
	 TPtrC8 appName;
	 appName.Set(aApp.iName,4); //Name is 4 Characters RFC 3550 Sec 6.7
	 
	 TPtrC8 appData;
	 appData.Set(aApp.iAppData,aApp.iAppDataLen);
	 
	 TUint8 subType = aApp.iSubType;
	 
	 TRAPD(err,iRtpSessionArr[ret]->iRtpSession.SendAPPL(appName,appData, subType));
	 
	 return err;
	 }

 /**
  * Send an RTCP BYE packet for a Transmit stream.
 */
 TInt CRtpManager::SendRtcpByePacket( TRtpId aTranStreamId, const TDesC8& aReason)
	 {
	 LOG_FUNC_ENTRY("CRtpManager::SendRtcpByePacket");
	 __RTP_LOG1("aTranStreamId: %u",aTranStreamId)

	 /* A send stream should be open to send an RTCP Bye packet */
	 //Transmit Stream ID and Session Id are same
	 TInt ret = FindSession(aTranStreamId);
	 if(ret < 0)
	 	return ret;
	 ASSERT(iRtpSessionArr[ret]->iRtpSendSource.IsOpen());
	 TRAPD(err,iRtpSessionArr[ret]->iRtpSendSource.ByeL(const_cast<TDesC8&>(aReason)));
	 return err;
	 }

/** 
 * Send an RTCP RR packet for a Reception stream.
*/
TInt CRtpManager::SendRtcpRrPacket( TRtpId aRcvStreamId )
	{
	 LOG_FUNC_ENTRY("CRtpManager::SendRtcpRrPacket");
	 __RTP_LOG1("aRcvStreamId: %u",aRcvStreamId)

    /* No specific way to Send an RR Report alone */
	//Do we need to?
	TInt ret = FindStream(aRcvStreamId);
	if(ret<0)
		{
		return ret;
		}

	ret = FindSession(iRtpStreamsArr[ret].iSessionKey);
	TUint8 flg = 0x81;
	TBuf8<1> flags;
	flags.Copy(&flg,1);
	TRAP(ret, iRtpSessionArr[ret]->iRtpSession.SendRTCPPacketL(flags));
	return ret;
	}
 
 /** 
 * Send an RTCP SR packet for a Transmit stream.
 */
 TInt CRtpManager::SendRtcpSrPacket( TRtpId aTranStreamId )
 	{
	 LOG_FUNC_ENTRY("CRtpManager::SendRtcpSrPacket");
	 __RTP_LOG1("aTranStreamId: %u",aTranStreamId)

	//NOTE: TransID same as SessionId
	TInt ret = FindSession(aTranStreamId);
	if(ret<0)
		return ret;
	
	TUint8 flg = 0x81;
	TBuf8<1> flags;
	flags.Copy(&flg,1);
    TRAP(ret, iRtpSessionArr[ret]->iRtpSession.SendRTCPPacketL(flags));
    return ret;  
 	}

/** 
 * Suspend RTCP sending on/off, calculations will continue. 
*/
TInt CRtpManager::SuspendRtcpSending( TRtpId aSessionId, TBool aAutoSending )
	{
	 LOG_FUNC_ENTRY("CRtpManager::SuspendRtcpSending");
	 __RTP_LOG2("aSessionId: %u aAutoSending %d",aSessionId,aAutoSending)

	//NOTE: TransID same as SessionId
	TInt ret = FindSession(aSessionId);
	if(ret<0)
		return ret;
	if(iRtpSessionArr[ret]->iRtcpEnabled)
		{
		iRtpSessionArr[ret]->iRtpSession.SetRTCPAutoSend(aAutoSending);
		return KErrNone;
		}
	else
		{
		return KErrNotSupported;
		}
	}
 
/** 
 * Gets the status of automatic RTCP sending.
 */
TInt CRtpManager::IsRtcpSendingSuspended( TRtpId aSessionId, TBool& aAutoSending )
	{
	LOG_FUNC_ENTRY("CRtpManager::IsRtcpSendingSuspended");

	TInt ret = FindSession(aSessionId);
	if(ret<0)
		return ret;
	if(iRtpSessionArr[ret]->iRtcpEnabled)
		{
		aAutoSending = iRtpSessionArr[ret]->iRtpSession.RTCPAutoSend();
		return KErrNone;
		}
	else
		{
		return KErrNotSupported;
		}
	}

/**
* Get the session ID of a stream, which belongs to that session.
*/
 TRtpId CRtpManager::GetSessionId( TRtpId aStreamId )
	 {
	 TInt ret = FindStream(aStreamId);
	 if(ret<0)
		{
		TInt idx = FindSession(aStreamId);
		if(idx < 0)
		    {
		    return idx;
		    }
		if(iRtpSessionArr[idx]->iRtpSendSource.IsOpen())
		    {
		    return iRtpSessionArr[idx]->iKey;
		    }
		return ret;
		}
	 return iRtpStreamsArr[ret].iSessionKey;
	 }

 
/**
* Get address of Socket object used by a given RTP Session
* to send/receive RTP data packets.
*/
 RSocket* CRtpManager::GetRtpSocket( TRtpId aSessionId )
 	{
 	LOG_FUNC_ENTRY("CRtpManager::GetRtpSocket");
 	__RTP_LOG1("aSessId %u",aSessionId)
 	
	TInt ret = FindSession(aSessionId);
	if( ret<0 )
		return NULL;
 	return iRtpSessionArr[ret]->iRtpSession.RtpSocket();
 	}

/**
* Get address of Socket object used by a given RTP Session
* to send/receive RTCP control packets.
*/
 RSocket* CRtpManager::GetRtcpSocket( TRtpId  aSessionId)
	{
	LOG_FUNC_ENTRY("CRtpManager::GetRtcpSocket");
 	__RTP_LOG1("aSessId %u",aSessionId)
	
	TInt ret = FindSession(aSessionId);
	if(ret<0 || !iRtpSessionArr[ret]->iRtcpEnabled)
		return NULL;
 	return iRtpSessionArr[ret]->iRtpSession.RtcpSocket();
	}

 /**
 * Retrieve statistical information for a stream
 * based on the reports from RTCP SR & RR packets.
 * @param aStreamId - [input] ID of stream
 * @param aStat - [output] Statistical information
 * @return KErrNone if successful; system wide error code otherwise
 */
 TInt CRtpManager::GetStreamStatistics( TRtpId aStreamId, TRtpPeerStat& aStat )
	 {
	 LOG_FUNC_ENTRY("CRtpManager::GetStreamStatistics");
	 /* Find the session and the stream */
	 TInt ret = FindStream(aStreamId);
	 if(ret<0)
		{
		return ret;
		}
	 
	 RRtcpSRPart srPart = iRtpStreamsArr[ret].iRecvSource.GetSR();
	 //Filling up the SR part first
	 aStat.iNumPacketsSent   = srPart.PacketCount();    
	 aStat.iCumNumOctetsSent = srPart.ByteCount();       
	 aStat.iRoundTripDelay   = 0; //TODO To be supported       
	 aStat.iTxBandwidth = 0; //TODO find the use cases
	 
	 return KErrNone;
	 }

 /**
 * Get sampling rate setting for a payload type.
 */
 TUint32 CRtpManager::GetSamplingRate( TUint8 aPayloadType )
	  {
	  LOG_FUNC_ENTRY("CRtpManager::GetSamplingRate");
	  __RTP_LOG1("PayLoadType %d",aPayloadType);
	  /* Iterate through the sessions and return its sampling rate*/
	  for(int i=0; i< iRtpSessionArr.Count(); i++)
		  {
		  __RTP_LOG1("iPayloadtype -->> :%d", iRtpSessionArr[i]->iPayloadType);
		  __RTP_LOG1("aPayloadtype -->> :%d", aPayloadType);
		  if(iRtpSessionArr[i]->iPayloadType == aPayloadType)
			  {
			  return iRtpSessionArr[i]->iSamplingRate;
			  }
		  }
	  //Not found
	  return 0;
	  }

 /**
 * Set sampling rate for a payload type.
 */
 TInt CRtpManager::SetSamplingRate( TUint8 aPayloadType, TUint32 aSamplingRate )
	 {
	 LOG_FUNC_ENTRY("CRtpManager::SetSamplingRate");
	 __RTP_LOG2("PayloadType %d SamplingRate %u",aPayloadType,aSamplingRate)
	  /* Iterate through the sessions and set its sampling rate*/
	  for(int i=0; i< iRtpSessionArr.Count(); i++)
		  {
		  if(iRtpSessionArr[i]->iPayloadType == aPayloadType || KRtpPayloadTypeUnspecified == iRtpSessionArr[i]->iPayloadType)
			  {
			  iRtpSessionArr[i]->iPayloadType = aPayloadType;
			  iRtpSessionArr[i]->iSamplingRate = aSamplingRate;
			  iRtpSessionArr[i]->iRtpSession.SetRTPTimeConversion(0,aSamplingRate/1000);
			  }
		  /* Also update the Sampling rates so that received streams are properly handled */
		  iRtpSessionArr[i]->iRtpSession.SetSamplingRate(aPayloadType,aSamplingRate);
		  }
	  return KErrNone;
	 }

 /**
 * Set RTCP parameters for a given RTP Session.
 * This function does nothing if RTCP was not enabled previously.
 */
 TInt CRtpManager::SetRtcpParameters( TRtpId aSessionId, const TRtcpParams& aRtcpParams )
	 {
	 TInt ret = FindSession(aSessionId);
	 if(ret<0)
	 	return ret;
	 ASSERT(iRtpSessionArr[ret]->iRtcpEnabled);
	 iRtpSessionArr[ret]->iRtpSession.SetRtcpInterval(const_cast<TTimeIntervalMicroSeconds32&>(aRtcpParams.iRtcpTimeOut));
	 iRtpSessionArr[ret]->iRtpSession.SetBandwidth(aRtcpParams.iSessionBWidth);
	 
	 return KErrNone;
	 }


 /**
  * Synchronous custom command interface for future extensions.
  * Input/output data and return value are defined by each custom command
  */
   TInt CRtpManager::CustomCommandSync( TInt , const TDesC8& ,
                           const TDesC8& , TDes8&  )
	   {
	   /* Not Supported. But is it ok to Fail? */
	   return KErrNotSupported;
	   }

  /**
  * ASynchronous custom command interface for future extensions.
  * Input/output data and return value are defined by each custom command
  */
   TInt CRtpManager::CustomCommandAsync( TInt , const TDesC8& ,  const TDesC8& ,
                                    	 TDes8& ,TRequestStatus&  )
	   {
	   /* Not Supported. But is it ok to Fail? */
	   return KErrNotSupported;
	   }


 TInt CRtpManager::FindSession(TRtpId aSessionId)
	{
	/* A simple iteration to find the session */
	for(TInt i=0; i < iRtpSessionArr.Count(); i++)
		{
		if(iRtpSessionArr[i]->iKey == aSessionId)
			{
			return i;
			}
		}
	__RTP_LOG("****FindSession Session Not Found!****")
	return KErrNotFound;
	}

TInt CRtpManager::FindSessionL(TRtpId aSessionId)
	{
	TInt ret = FindSession(aSessionId);
	User::LeaveIfError(ret);
	return ret;
	}

TInt CRtpManager::FindStream(TRtpId aStreamId)
	{
	/* A simple iteration to find the session */
	for(TInt i=0; i < iRtpStreamsArr.Count(); i++)
		{
		if(iRtpStreamsArr[i].iKey == aStreamId)
			{
			return i;
			}
		}
	__RTP_LOG("****FindStream Stream Not Found!****")
	return KErrNotFound;
	}

TInt CRtpManager::FindStreamL(TRtpId aStreamId)
	{
	TInt ret = FindStream(aStreamId);
	User::LeaveIfError(ret);
	return ret;
	}

TInt CRtpManager::GetDefaultIapIdL()
	{
	LOG_FUNC_ENTRY("CRtpManager::GetDefaultIapIdL");
	TInt apId;
	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeIAP);
	CleanupStack::PushL(db);
	CCommsDbConnectionPrefTableView* view= db->OpenConnectionPrefTableInRankOrderLC(ECommDbConnectionDirectionOutgoing);
	CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref Pref;
	view->GotoFirstRecord();
	view->ReadConnectionPreferenceL(Pref);
	apId = Pref.iBearer.iIapId;
	CleanupStack::PopAndDestroy(2);
	__RTP_LOG1("IapId %d",apId)
	LOG_FUNC_EXIT("CRtpManager::GetDefaultIapIdL");
	return apId;
	}

/* Schedules RTP transmission from the Head of the pending queue
 */
TInt CRtpManager::SendRtpDataFromPendingQueue(CRtpSessionInfo *aSessInfo)
	{
	ASSERT(!aSessInfo->iPendingReqQue->IsEmpty());
	TRtpSendReqNode *reqNode = aSessInfo->iPendingReqQue->Last();
	reqNode->iPktToSend.Send();
	return KErrNone;
	}

void CRtpManager::RegisterEventsOnSessionL(CRtpSessionInfo *aSessionInfo)
	{
	RRtpSession session = aSessionInfo->iRtpSession;
	
	session.RegisterEventCallbackL(ERtpSessionFail,
			                       RtpSessionLevelCB,
			                       aSessionInfo,
			                       ERtpNotOneShot);	
	
	session.RegisterEventCallbackL(ERtpNewSource,
			                       RtpSessionLevelCB,
			                       aSessionInfo,
			                       ERtpNotOneShot);			                       
	
	session.RegisterEventCallbackL(ERtpNewRR,
			                       RtpSessionLevelCB,
			                       aSessionInfo,
			                       ERtpNotOneShot);			                       
	
	session.RegisterEventCallbackL(ERtpBufferOverflow,
			                       RtpSessionLevelCB,
			                       aSessionInfo,
			                       ERtpNotOneShot);			                       
	
	session.RegisterEventCallbackL(ERtpUndersizedPacket,
			                       RtpSessionLevelCB,
			                       aSessionInfo,
			                       ERtpNotOneShot);			                       	                       
	}


void CRtpManager::RegisterEventsOnRtpRcvSourceL(RRtpReceiveSource &aRcvSrc, TRtpStreamInfo *apStrmInfo)
	{
        aRcvSrc.RegisterEventCallbackL(ERtpPacketReceived,
				                         CRtpManager::RtpStreamLevelCB,
				                         apStrmInfo,
				                         ERtpNotOneShot);		
		
		aRcvSrc.RegisterEventCallbackL(ERtpSourceFail,
				                         CRtpManager::RtpStreamLevelCB,
				                         apStrmInfo,
				                         ERtpNotOneShot);
		
		aRcvSrc.RegisterEventCallbackL(ERtpAPP,
				                         CRtpManager::RtpStreamLevelCB,
				                         apStrmInfo,
				                         ERtpNotOneShot);

		
		aRcvSrc.RegisterEventCallbackL(ERtpBYE,
				                         CRtpManager::RtpStreamLevelCB,
				                         apStrmInfo,
				                         ERtpNotOneShot);
		
		aRcvSrc.RegisterEventCallbackL(ERtpSR,
				                         CRtpManager::RtpStreamLevelCB,
				                         apStrmInfo,
				                         ERtpNotOneShot);
		
		aRcvSrc.RegisterEventCallbackL(ERtpRR,
				                         CRtpManager::RtpStreamLevelCB,
				                         apStrmInfo,
				                         ERtpNotOneShot);
		
		aRcvSrc.RegisterEventCallbackL(ERtpSDES,
				                         CRtpManager::RtpStreamLevelCB,
				                         apStrmInfo,
				                         ERtpNotOneShot);                      	                       
	}

void CRtpManager::HandleNonRtpDataL(CRtpSessionInfo *apSessionInfo)
    {
    apSessionInfo->iNonRtpDataObserver->NonRTPDataReceived(apSessionInfo->iPortNumber,
                                                           ETrue,
                                                           apSessionInfo->iRtpSession.NonRtpDataL());
    apSessionInfo->iRtpSession.RegisterEventCallbackL(ENonRtpDataReceived,
                                                       RtpSessionLevelCB,
                                                       apSessionInfo,
                                                       ERtpOneShot);
    }

void CRtpManager::HandleNonRtcpDataL(CRtpSessionInfo *apSessionInfo)
    {
     apSessionInfo->iNonRtpDataObserver->NonRTPDataReceived(apSessionInfo->iPortNumber+1,
                                                             EFalse,
                                                             apSessionInfo->iRtpSession.NonRtcpDataL());
     apSessionInfo->iRtpSession.RegisterEventCallbackL(ENonRtcpDataReceived,
                                                       RtpSessionLevelCB,
                                                       apSessionInfo,
                                                       ERtpOneShot);
    }

void CRtpManager::RtpSessionLevelCB(CRtpSessionInfo* apSessionInfo, const TRtpEvent& aEvent)
	{
	 LOG_FUNC_ENTRY("CRtpManager::RtpSessionLevelCB");
	 __RTP_LOG1("EventType %x",aEvent.Type())
	 CRtpManager *pSelf = apSessionInfo->iRtpManager;

	/* Event Send succeded or Failed */
	if(ERtpSendSucceeded == aEvent.Type() || ERtpSendFail == aEvent.Type())
		{
		/* We are  here only if Async Send was initiated */
		TInt retCode = (aEvent.Type()==ERtpSendFail)?aEvent.Status():KErrNone;
		
		/* An Async send is Complete. Notify the Application and schedule for next
		 * transmission if send queue is not empty */
		ASSERT(!apSessionInfo->iPendingReqQue->IsEmpty());
		TRtpSendReqNode *reqNode = apSessionInfo->iPendingReqQue->First();
		reqNode->iPktToSend.Close();
			
		__RTP_LOG("Complete Async Request")
		/* Complete the Async Request */
		User::RequestComplete(reqNode->iStatus,retCode);
		apSessionInfo->iPendingReqQue->Remove(*reqNode);
		delete reqNode;
		
		if(!apSessionInfo->iPendingReqQue->IsEmpty())
			{
			//OK to ignore Send Errors ?
			apSessionInfo->iRtpManager->SendRtpDataFromPendingQueue(apSessionInfo);
			}
		}
	/* A new source was detected */
	if(ERtpNewSource == aEvent.Type())
		{
		/* Create a ReceiveSource for this stream. */
		RRtpReceiveSource rtpRcvSrc;
		TRAPD(err, rtpRcvSrc = apSessionInfo->iRtpSession.NewReceiveSourceL());
	    if(err)
	         {
	          __RTP_LOG1("Receive source could not be created %d Returning", err)
	          return;
	         }
		
		/* See if there is a Pre-Created stream that will accept this PayloadType.
		   If there are no Pre Created stream that accepts this PayloadType just
		   Blindly latch on to the First PreCreated Stream. This behaviour is not
		   really correct but is inline with the S60 RTP Stack behaviour. */
		TUint32 strmKey = 0;
		TInt tmpIdx = -1; //Used to remember the last index of a eStreamPreCreated stream

		TUint payloadType = 0;
		TRAP(err, payloadType=rtpRcvSrc.PayLoadTypeL());
		if(err == KErrNotFound)
			{
			__RTP_LOG1("Payload type was not found", err)
			}

		__RTP_LOG1("PayloadType %d",payloadType)

		for(int i=0; i<pSelf->iRtpStreamsArr.Count();i++)
			{
			if(pSelf->iRtpStreamsArr[i].iState == TRtpStreamInfo::eStreamPreCreated)
				{
				/* To make later access easier */
				tmpIdx = i;
				}

			/* Try to match the Payload type first */
			if(pSelf->iRtpStreamsArr[i].iPayloadType ==  payloadType
					&& pSelf->iRtpStreamsArr[i].iState == TRtpStreamInfo::eStreamPreCreated)
				{
				__RTP_LOG1("Found PreCreated Stream at %d",i);
				/* Ok so there is a PreCreated stream available */
				pSelf->iRtpStreamsArr[i].iIsRecvStream = ETrue;
				pSelf->iRtpStreamsArr[i].iRecvSource = rtpRcvSrc;
				pSelf->iRtpStreamsArr[i].iState = TRtpStreamInfo::eStreamReadyToUse;
				strmKey = pSelf->iRtpStreamsArr[i].iKey;
				break;
				}
			}
		
		if(!strmKey && tmpIdx>=0)
			{
			__RTP_LOG1("No PreCreated Stream : Using ID %d",tmpIdx)
			/* Latch on to the unspecified stream */
			pSelf->iRtpStreamsArr[tmpIdx].iIsRecvStream = ETrue;
			pSelf->iRtpStreamsArr[tmpIdx].iRecvSource = rtpRcvSrc;
			pSelf->iRtpStreamsArr[tmpIdx].iState = TRtpStreamInfo::eStreamReadyToUse;
			strmKey = pSelf->iRtpStreamsArr[tmpIdx].iKey;
			/* Because we ignored the original user selection */
			pSelf->iRtpStreamsArr[tmpIdx].iPayloadType = payloadType;
			}
		
		if(!strmKey)
			{
			__RTP_LOG("No PreCreated Stream : Creating a New Stream")
			/* Create a new stream */
			TRtpStreamInfo strmInfo;
			strmInfo.iKey = Math::Random();
			strmInfo.iSessionKey = apSessionInfo->iKey;
			strmInfo.iState = TRtpStreamInfo::eStreamPostCreated;
			strmInfo.iRecvSource  = rtpRcvSrc;
			TRAPD(error, strmInfo.iPayloadType = rtpRcvSrc.PayLoadTypeL());
			if(error == KErrNotFound)
			{
			__RTP_LOG1("Payload type was not found", error)
			}
			strmInfo.iRtpManager  = pSelf;
			/* Append to the Array */
			pSelf->iRtpStreamsArr.Append(strmInfo); //TODO handle append failures
			strmKey = strmInfo.iKey;
			__RTP_LOG1("Session Did not have a PreCreated Stream New StreamID %u",strmInfo.iKey)
			}
		/* Get a reference to the Strm from Array for Callback Registration */
		TInt idx = pSelf->FindStream(strmKey);

	    TRAP_IGNORE(pSelf->RegisterEventsOnRtpRcvSourceL(rtpRcvSrc,  &(pSelf->iRtpStreamsArr[idx])));
		}
	if(ERtpNewRR == aEvent.Type())
		{
		/* Session Level RTCP RR . An Empty RR was received */
		if(apSessionInfo->iRtcpObserver)
			{
			apSessionInfo->iRtcpObserver->RrReceived(apSessionInfo->iKey,aEvent.Status());
			}
		}

	if(ENonRtpDataReceived == aEvent.Type())
		{
		/* Session Level CB. A non rtp data was received */
		if(apSessionInfo->iNonRtpDataObserver)
			{
			TRAP_IGNORE(pSelf->HandleNonRtpDataL(apSessionInfo));
			}
		}
	if(ENonRtcpDataReceived == aEvent.Type())
		{
		/* Session Level CB. A non rtp data was received */
		if(apSessionInfo->iNonRtpDataObserver)
			{
			__RTP_LOG("CRtpManager::RtpSessionLevelCB");
			__RTP_LOG("Non rtcp data - issuing non rtp data received callback");
			TRAP_IGNORE(pSelf->HandleNonRtcpDataL(apSessionInfo));    
			}
		}
	}

void CRtpManager::RtpStreamLevelCB(TRtpStreamInfo* apStrmInfo, const TRtpEvent& aEvent)
	{
	 LOG_FUNC_ENTRY("CRtpManager::RtpStreamLevelCB");
	 __RTP_LOG1("EventType %x",aEvent.Type())

	TInt ret;
	if(ERtpPacketReceived == aEvent.Type())
		{
		/* New Packet Arrived. Issue callback */
		TRtpRecvHeader rcvHeader;
		RRtpReceivePacket pkt  = apStrmInfo->iRecvSource.Packet();
		rcvHeader.iMarker      = pkt.Marker();
		rcvHeader.iPadding     = 0; //TODO Pading bits are stripped by Stack? Verify
		rcvHeader.iPayloadType = pkt.PayloadType();
		rcvHeader.iSeqNum      = pkt.SequenceNumber();
		rcvHeader.iTimestamp   = pkt.Timestamp();
		
		ret = apStrmInfo->iRtpManager->FindSession(apStrmInfo->iSessionKey);
		ASSERT(ret>=0);
		if(apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtpObserver)
			{
			__RTP_LOG("Issuing RtpReceived callback")
			apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtpObserver->RtpPacketReceived(apStrmInfo->iKey,rcvHeader,pkt.Payload());
			}
		else
			{
			__RTP_LOG("Session Did not have RTP Observer Registered")
			}
		pkt.Close();
		}
	if(ERtpRR == aEvent.Type())
		{
		/* Fetch the information on the RR Received from specif SSRC */
		ret = apStrmInfo->iRtpManager->FindSession(apStrmInfo->iSessionKey);
		ASSERT(ret>=0);
		if(apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtcpObserver)
			{
			__RTP_LOG("Issuing RtcpRrReceived callback")
			apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtcpObserver->RrReceived(apStrmInfo->iKey, aEvent.Status());
			}
		else
			{
			__RTP_LOG("Session Did not have RCTP Observer Reqistered")
			}
		}	
	if(ERtpSDES == aEvent.Type())
		{
		/* Fetch the information on the SDES Received from specif SSRC */
		ret = apStrmInfo->iRtpManager->FindSession(apStrmInfo->iSessionKey);
		ASSERT(ret>=0);
		if(apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtcpObserver)
			{
			TBuf8<100> cname;
			TBuf8<100> name;
			TBuf8<100> email;
			
			/* Extract the SDES information and pass it up */
			TRtpSdesParams sdesParams;
			if(KErrNotFound != apStrmInfo->iRecvSource.GetSDES(1,cname))
				{
				sdesParams.iCName.Set(cname);
				}
			if(KErrNotFound != apStrmInfo->iRecvSource.GetSDES(2,name))
				{
				sdesParams.iUserName.Set(name);
				}
			if(KErrNotFound != apStrmInfo->iRecvSource.GetSDES(3,email))
				{
				sdesParams.iEmail.Set(name);
				}
			__RTP_LOG("Issuing SdesReceived callback")
			apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtcpObserver->SdesReceived(aEvent.Status(),sdesParams);
			}
		else
			{
			__RTP_LOG("RTCP Observer was not registered. Callback not issued")
			}
		}
	if(ERtpSR == aEvent.Type())
		{
		/* Fetch the information on the SR Received from specific SSRC */
		ret = apStrmInfo->iRtpManager->FindSession(apStrmInfo->iSessionKey);
		ASSERT(ret>=0);
		if(apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtcpObserver)
			{
			RRtcpSRPart srPart(apStrmInfo->iRecvSource.GetSR());
			
			TTimeStamps timeStamps;
			srPart.NTPTimestamp(timeStamps.iNTPTimeStampSec, timeStamps.iNTPTimeStampFrac);
			timeStamps.iTimeStamp = srPart.RTPTimestamp();
			
			__RTP_LOG("Issuing SrReceived callback")
			apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtcpObserver->SrReceived(apStrmInfo->iKey, apStrmInfo->iRecvSource.SSRC(), timeStamps);
			}
		else
			{
			__RTP_LOG("RTCP Observer was not registered. Callback not issued")
			}
		}	
	if(ERtpAPP == aEvent.Type())
		{
		/* Fetch the information on the RR Received from specif SSRC */
		ret = apStrmInfo->iRtpManager->FindSession(apStrmInfo->iSessionKey);
		ASSERT(ret>=0);
		if(apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtcpObserver)
			{
			TRtcpApp app;
			TPtrC8 name;
			TPtrC8 appData;
			TUint  subType;
	
			apStrmInfo->iRecvSource.GetLastApp(name, appData, subType );
			
			//Create Tpr8 from TRtcpApp ..Duh!!
			TPtr8 name1(app.iName,4);
			TPtr8 appData1(app.iAppData,KMaxRtcpAppData);
			name1 = name;
			appData1 = appData;
			app.iAppDataLen = appData.Length();
			app.iSubType = subType;
			
			__RTP_LOG("Issuing SrReceived callback")
			apStrmInfo->iRtpManager->iRtpSessionArr[ret]->iRtcpObserver->AppReceived(apStrmInfo->iKey, aEvent.Status(),app );
			}
		else
			{
			__RTP_LOG("RTCP Observer was not registered. Callback not issued")
			}
		}	
	}


// Decrypting RTP Packet recieved from network
TInt CRtpSessionInfo::PreRtpProcessing(TDes8 &aRTPPacket)
	{
	__RTP_LOG("CRtpSessionInfo::PreRtpProcessing")
	//SSRC of recieved packet KRtpSSRCOffset (8)
	const TUint KRtpSSRCOffset = 8;
    HBufC8 *rtpPacket = NULL;
	TUint8* packet = const_cast<TUint8*>(aRTPPacket.Ptr());
	TUint32 ssrc = (*(packet+KRtpSSRCOffset+3))+
						(*(packet+KRtpSSRCOffset+2)<<8)+
						(*(packet+KRtpSSRCOffset+1)<<16)+
						(*(packet+KRtpSSRCOffset)<<24);
	
	TRAPD(error, rtpPacket = iSRTPSession->UnprotectRTPL(ssrc, aRTPPacket));
	if(error == KErrNone)
		{
		__RTP_LOG("RTP Packet Decrypted sucessfully")
		aRTPPacket.Copy(rtpPacket->Des());
		}
	else
		{
		__RTP_LOG("RTP Packet Decrypted Failed")
		error = KErrCorrupt;
		}
	delete rtpPacket;
	return error;
	}

// Encrypting RTP packet before sending
void CRtpSessionInfo::PostRtpProcessing(TDes8 &aRTPPacket)
	{
	__RTP_LOG("CRtpSessionInfo::PostRtpProcessing")
	//SSRC of recieved packet KRtpSSRCOffset (8)
	const TUint KRtpSSRCOffset = 8;
    HBufC8 *rtpPacket = NULL;
	TUint8* packet = const_cast<TUint8*>(aRTPPacket.Ptr());
	TUint32 ssrc = (*(packet+KRtpSSRCOffset+3))+
						(*(packet+KRtpSSRCOffset+2)<<8)+
						(*(packet+KRtpSSRCOffset+1)<<16)+
						(*(packet+KRtpSSRCOffset)<<24);
	
    TRAPD(error, rtpPacket = iSRTPSession->ProtectRTPL(ssrc, aRTPPacket));
    if(error != KErrNone) 
        { 
        __RTP_LOG("RTP Packet was not encrypted") 
        aRTPPacket.FillZ(); 
        } 
    else 
        { 
        __RTP_LOG("RTP Packet was encrypted successfully") 
        aRTPPacket.Copy(rtpPacket->Des());   
        delete rtpPacket;            
        }
	}

// DeCrypting RTCP Packet recieved from network
TInt CRtpSessionInfo::PreRtcpProcessing(TDes8 &aRTCPPacket) 
	{
	__RTP_LOG("CRtpSessionInfo::PreRtcpProcessing")
	//SSRC of recieved packet KRtcpSSRCOffset (4)
	const TUint KRtcpSSRCOffset = 4;
	HBufC8 *rtcpPacket = NULL;
	TUint8* packet = const_cast<TUint8*>(aRTCPPacket.Ptr());
	TUint32 ssrc = (*(packet+KRtcpSSRCOffset+3))+
						(*(packet+KRtcpSSRCOffset+2)<<8)+
						(*(packet+KRtcpSSRCOffset+1)<<16)+
						(*(packet+KRtcpSSRCOffset)<<24);
	
	TRAPD(error, rtcpPacket = iSRTPSession->UnprotectRTCPL(ssrc, aRTCPPacket));
	if(error == KErrNone)
		{
		__RTP_LOG("RTCP Packet Decrypted sucessfully")
		aRTCPPacket.Copy(rtcpPacket->Des());
		}
	else
		{
		__RTP_LOG("RTCP Packet Decrypted Failed")
		error = KErrCorrupt;
		}
	delete rtcpPacket;
	return error;
	}

// Encrypting RTCP Packet before sending
void CRtpSessionInfo::PostRtcpProcessing(TDes8 &aRTCPPacket) 
	{
	__RTP_LOG("CRtpSessionInfo::PostRtcpProcessing")
	//SSRC of recieved packet KRtcpSSRCOffset (4)
	const TUint KRtcpSSRCOffset = 4;
	HBufC8 *rtcpPacket = NULL;
	// Assign a pointer to a packet
	TUint8* packet = const_cast<TUint8*>(aRTCPPacket.Ptr());
	TUint32 ssrc = (*(packet+KRtcpSSRCOffset+3))+
						(*(packet+KRtcpSSRCOffset+2)<<8)+
						(*(packet+KRtcpSSRCOffset+1)<<16)+
						(*(packet+KRtcpSSRCOffset)<<24);
	
	TRAPD(error, rtcpPacket = iSRTPSession->ProtectRTCPL(ssrc, aRTCPPacket)); 
	if(error != KErrNone) 
        { 
        __RTP_LOG("RCTP Packet was not encrypted") 
        aRTCPPacket.FillZ(); 
        } 
	else 
        { 
        __RTP_LOG("RCTP Packet was encrypted Successfully")
        aRTCPPacket.Copy(rtcpPacket->Des());   
        delete rtcpPacket;            
        }
	}

