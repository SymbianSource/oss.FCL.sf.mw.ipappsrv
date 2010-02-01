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
// server class for the rtpwrapper unit test cases
// 
//

/**
 @file
 @internalComponent
*/
 
#include "te_rtp_wrappercoreserver.h"
#include "te_rtp_wrappercorestep.h"



// __EDIT_ME__ - Substitute the name of your test server 
_LIT(KServerName,"te_rtpwrapper");
// __EDIT_ME__ - Use your own server class name
CTe_rtpwrapperCoreServer* CTe_rtpwrapperCoreServer::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CTe_rtpwrapperCoreServer* server = new (ELeave) CTe_rtpwrapperCoreServer();
	CleanupStack::PushL(server);
	//CServer call
	server->ConstructL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

CTe_rtpwrapperCoreServer::CTe_rtpwrapperCoreServer()
	{
//	iUsingConnectedSocket = EFalse;
	}

CTe_rtpwrapperCoreServer::~CTe_rtpwrapperCoreServer()
	{
	delete iScheduler;
	//
//	delete iController;
//	delete iSendController;
	}

#if (!defined EKA2)
LOCAL_C void MainL()
/**
 * REQUIRES semaphore to sync with client as the Rendezvous()
 * calls are not available
 */
	{
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CleanupStack::PushL(sched);
	CActiveScheduler::Install(sched);
	CTe_rtpwrapperCoreServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTe_rtpwrapperCoreServer::NewL());
	if(!err)
		{
		CleanupStack::PushL(server);
		RSemaphore sem;
		// The client API will already have created the semaphore
		User::LeaveIfError(sem.OpenGlobal(KServerName));
		CleanupStack::Pop(server);
		// Sync with the client then enter the active scheduler
		sem.Signal();
		sem.Close();
		sched->Start();
		}
	CleanupStack::Pop(sched);
	delete server;
	delete sched;
	}
#else
// EKA2 much simpler
// Just an E32Main and a MainL()
LOCAL_C void MainL()
/**
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	// __EDIT_ME__ Your server name
	CTe_rtpwrapperCoreServer* server = NULL;
	// Create the CTestServer derived server
	// __EDIT_ME__ Your server name
	TRAPD(err,server = CTe_rtpwrapperCoreServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}
#endif

// Only a DLL on emulator for typhoon and earlier
#if (defined __WINS__ && !defined EKA2)
// and E32Dll(enum TDllReason) defined in rtputils.cpp
/*
GLDEF_C TInt E32Dll(enum TDllReason)
	{
	return 0;
	}
*/
#else
GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on exit
 */
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(!cleanup)
		{
		return KErrNoMemory;
		}
	TInt err = 0;
	TRAP(err,MainL());
	delete cleanup;
	return err;
	}
#endif

// Create a thread in the calling process
// Emulator typhoon and earlier
#if (defined __WINS__ && !defined EKA2)
TInt ThreadFunc (TAny* /*aParam*/)
/**
 * @return - Server exit code
 * @param - unused
 * Server Thread function. Guts of the code in the MainL() function
 */
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(!cleanup)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	delete cleanup;
	return KErrNone;
	}

EXPORT_C TInt NewServer() 
/**
 * @return - Standard Epoc error codes
 * 1st and only ordinal, called by the client API to initialise the server
 */
	{
	_LIT(KThread,"Thread");
	RThread thread;
	// __EDIT_ME__ - Make sure the TBuf is large enough
	TBuf<KMaxTestExecuteNameLength> threadName(KServerName);
	// Create a hopefully unique thread name and use the ThreadFunc
	threadName.Append(KThread);
	const TInt KMaxHeapSize = 0x1000000;			//< Allow a 1Mb max heap
	TInt err = thread.Create(threadName, ThreadFunc, KDefaultStackSize,
													KMinHeapSize, KMaxHeapSize,
													NULL, EOwnerProcess);
	if(err)
		return err;
	thread.Resume();
	thread.Close();
	return KErrNone;
	}
#endif

#define DEF_TEST(_n, _f) if (aStepName == (_n)) { testStep = new _f; testStep->SetTestStepName((_n));}


CTestStep* CTe_rtpwrapperCoreServer::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Secure and non-secure variants
 * Implementation of CTestServer pure virtual
 */
	{
	CTe_rtpwrapperCoreStep* testStep = NULL;
	// __EDIT_ME__ - Create your own test steps here
	// This server creates just one step but create as many as you want
	// They are created "just in time" when the worker thread is created
	
	DEF_TEST(KStartConnection, CRtpManager_StartConnectionL);
	DEF_TEST(KGetLocalIPAddress, CRtpManager_GetLocalIPAddress);
	DEF_TEST(KCreateSession, CRtpManager_CreateSession);
	DEF_TEST(KRegisterRtcpObserver, CRtpManager_RegisterRtcpObserver);
	DEF_TEST(KRegisterRtpObserver, CRtpManager_RegisterRtpObserver);
	DEF_TEST(KGetRtpSocket, CRtpManager_GetRtpSocket);
	DEF_TEST(KCreateReceiveStream, CRtpManager_CreateReceiveStream);
	DEF_TEST(KCreateTransmitStream, CRtpManager_CreateTransmitStream);
	DEF_TEST(KSendData, CRtpManager_SendData);
	DEF_TEST(KCancelSend, CRtpManager_CancelSend);
	DEF_TEST(KSendRtcpByePacket, CRtpManager_SendRtcpByePacket);
	DEF_TEST(KSendRtcpSrPacket, CRtpManager_SendRtcpSrPacket);
	DEF_TEST(KSendRtcpRRPacket, CRtpManager_SendRtcpRRPacket);
	DEF_TEST(KSendRtcpAppPacket, CRtpManager_SendRtcpAppPacket);
	DEF_TEST(KSuspendRtcpSending, CRtpManager_SuspendRtcpSending);
	DEF_TEST(KGetSessionId, CRtpManager_GetSessionId);
	DEF_TEST(KSetRtcpParameters, CRtpManager_SetRtcpParameters);
	DEF_TEST(KSetRemoteAddress, CRtpManager_SetRemoteAddress);
	DEF_TEST(KSendRtpPacket, CRtpManager_SendRtpPacket);
	DEF_TEST(KSendRtpPacket1, CRtpManager_SendRtpPacket1);
	DEF_TEST(KSrtpCreateSession, CRtpManager_SrtpCreateSession);
	DEF_TEST(KSrtpCreateSession1, CRtpManager_SrtpCreateSession1);
	DEF_TEST(KCreateMultipleSessions, CRtpManager_CreateMultipleSessions);
	DEF_TEST(KCreateTransmitStreamExt, CRtpManager_CreateTransmitStreamExt);
	DEF_TEST(KCustomCommandsync, CRtpManager_CustomCommandsync);
	DEF_TEST(KCustomCommandAsync, CRtpManager_CustomCommandAsync);
	DEF_TEST(KStartAsyncConnection, CRtpManager_StartAsyncConnection);
	DEF_TEST(KSetNonRTPDataObserver, CRtpManager_SetNonRTPDataObserver);
	
	if (testStep)
		testStep->SetServer(this);
	

	return testStep;
	}

