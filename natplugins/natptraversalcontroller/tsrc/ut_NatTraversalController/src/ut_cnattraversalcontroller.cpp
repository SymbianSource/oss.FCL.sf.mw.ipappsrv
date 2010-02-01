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


//  CLASS HEADER
#include "ut_cnattraversalcontroller.h"

//  INTERNAL INCLUDES
#include "cnattraversalcontroller.h"
#include "cnattraversalsession.h"
#include "cnattraversalconfig.h"
#include "cnatbinding.h"
#include "sipnattraversalcontrollerinitparams.h"
#include "cnatbindingstunrefresher.h"

//  EXTERNAL INCLUDES
#include <CommDbConnPref.h>
#include <digia/eunit/eunitmacros.h>

//  CONSTANTS
const TUint32 KIapId = 10;
_LIT8(KZeroAddr, "0.0.0.0");
_LIT(KLocalAddr, "5.6.7.8");
_LIT(KNextHopAddr, "1.2.3.4");
_LIT8(KDomain, "mydomain.ok");

// CONSTRUCTION
UT_CNATTraversalController* UT_CNATTraversalController::NewL()
    {
    UT_CNATTraversalController* self = UT_CNATTraversalController::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATTraversalController* UT_CNATTraversalController::NewLC()
    {
    UT_CNATTraversalController* self = new( ELeave ) UT_CNATTraversalController();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATTraversalController::~UT_CNATTraversalController()
    {
    }

// Default constructor
UT_CNATTraversalController::UT_CNATTraversalController()
    {
    }

// Second phase construct
void UT_CNATTraversalController::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS


void UT_CNATTraversalController::SetupL()
    {
    User::LeaveIfError(iSocketServ.Connect());
    User::LeaveIfError(iConnection.Open(iSocketServ));
    TCommDbConnPref prefs;
    prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
    prefs.SetDirection(ECommDbConnectionDirectionOutgoing);
    prefs.SetIapId(KIapId);
    User::LeaveIfError(iConnection.Start(prefs));
    User::LeaveIfError(iSocket.Open(iSocketServ,
                                    KAfInet,
                                    KSockDatagram,
                                    KProtocolInetUdp,
                                    iConnection));

    User::LeaveIfError(iLocalAddr.Input(KLocalAddr));
    User::LeaveIfError(iNextHopAddr.Input(KNextHopAddr));

    TSIPNATTraversalControllerInitParams initParams(iSocketServ);
    iController = CNATTraversalController::NewL(&initParams);
    }


void UT_CNATTraversalController::Teardown()
    {
    delete iController;
    iSocket.Close();
    iConnection.Close();
    iSocketServ.Close();
    }


// From MSIPNATTraversalRequestObserver
void UT_CNATTraversalController::RequestComplete(
    TUint32 aRequestId,
    TInt aCompletionCode)
    {
    iRequestCompletionCode = aCompletionCode;
    if (iRequestId > 0)
        {
        EUNIT_ASSERT(iRequestId == aRequestId);
        CActiveScheduler::Stop();
        }
    }

// From MSIPNATTraversalRequestObserver
void UT_CNATTraversalController::PublicAddrResolved(
    TUint32 /*aRequestId*/,
    const TInetAddr& /*aPublicAddr*/)
    {
    }


// From MSIPNATBindingObserver
void UT_CNATTraversalController::FlowFailure(TInt /*aError*/)
    {
    }


// MNATTraversalSocketUser
const RSocket& UT_CNATTraversalController::Socket() const
    {
    return iSocket;
    }


// MNATTraversalSocketUser
void UT_CNATTraversalController::SendingAllowed()
    {
    iIsSending = ETrue;
    }

// MNATTraversalSocketUser
TBool UT_CNATTraversalController::IsSending() const
    {
    return iIsSending;
    }


// MNATTraversalSocketUser
void UT_CNATTraversalController::CancelSending()
    {
    iIsSending = EFalse;
    }


// Test functions

void UT_CNATTraversalController::TestHandleNextFromSocketUserQueueL()
    {
    // Queue empty
    iController->HandleNextFromSocketUserQueue();

    // One in queue, not sending
    iController->AddToSendingQueueL(*this);
    iController->HandleNextFromSocketUserQueue();

    // One in queue, sending
    iController->HandleNextFromSocketUserQueue();
    }


void UT_CNATTraversalController::TestNextRequestIdL()
    {
    EUNIT_ASSERT(1 == iController->NextRequestId());
    EUNIT_ASSERT(2 == iController->NextRequestId());
    iController->iRequestIdCounter = KMaxTUint32;
    EUNIT_ASSERT(1 == iController->NextRequestId());
    }


void UT_CNATTraversalController::TestCreateBindingL()
    {
    TInt err = KErrNone;
    TBool asyncInitiated = EFalse;

    // No session, no binding
    CNATTraversalConfig* config1 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config1,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config1);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    EUNIT_ASSERT(asyncInitiated);

    // Session exists and binding exist, should fail
    asyncInitiated = EFalse;
    CNATTraversalConfig* config2 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    TRAP(err, iController->CreateBindingL(2,config2,iConnection,iLocalAddr,iSocket,
                                          iNextHopAddr,*this,*this,asyncInitiated));
    if (err == KErrNoMemory)
        {
        User::Leave(err);
        }
    EUNIT_ASSERT(KErrAlreadyExists == err);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    EUNIT_ASSERT(!asyncInitiated);
    CleanupStack::PopAndDestroy(config2);

    // Session exists, but binding does not
    // Fake other observer by casting
    asyncInitiated = EFalse;
    MSIPNATBindingObserver* obs = reinterpret_cast<MSIPNATBindingObserver*>(1);
    CNATTraversalConfig* config3 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config3,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*obs,*this,asyncInitiated);
    CleanupStack::Pop(config3);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    EUNIT_ASSERT(asyncInitiated);

    // STUN server configured as 0.0.0.0, CRLF refresh not enabled
    iController->FreeResources(KIapId);
    CNATTraversalConfig* config4 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    delete config4->iSTUNServer;
    config4->iSTUNServer = NULL;
    config4->iSTUNServer = KZeroAddr().AllocL();
    iController->CreateBindingL(1,config4,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config4);
    EUNIT_ASSERT(0 == iController->iSessions.Count());
    EUNIT_ASSERT(!asyncInitiated);

    // STUN server configured as 0.0.0.0, CRLF refresh enabled
    iController->FreeResources(KIapId);
    CNATTraversalConfig* config5 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    delete config5->iSTUNServer;
    config5->iSTUNServer = NULL;
    config5->iSTUNServer = KZeroAddr().AllocL();
    config5->iCRLFRefreshEnabled = CNATTraversalConfig::EEnabled;
    iController->CreateBindingL(1,config5,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config5);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    EUNIT_ASSERT(!asyncInitiated);
    }


void UT_CNATTraversalController::TestFindSessionIndexL()
    {
    TBool asyncInitiated = EFalse;

    // No session found
    EUNIT_ASSERT(KErrNotFound ==
                      iController->FindSessionIndex(KIapId,KDomain));

    // Session found
    CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    EUNIT_ASSERT(0 == iController->FindSessionIndex(KIapId,KDomain));

    // Session not found, different IAP
    EUNIT_ASSERT(KErrNotFound ==
                      iController->FindSessionIndex(KIapId+1,KDomain));

    // Session not found, different proxy
    EUNIT_ASSERT(KErrNotFound ==
                      iController->FindSessionIndex(KIapId,_L8("different")));
    }


void UT_CNATTraversalController::TestHandleRequestL()
    {
    TUint32 requestId = 0;

    // Binding observer NULL
    requestId = iController->ResolvePublicAddrL(
        KIapId,iConnection,iLocalAddr,KDomain,iSocket,iNextHopAddr,NULL,*this);
    iController->Cancel(requestId);

    // Binding observer not NULL
    requestId = iController->ResolvePublicAddrL(
        KIapId,iConnection,iLocalAddr,KDomain,iSocket,iNextHopAddr,this,*this);

    // Binding observer not NULL, binding already exists
    requestId = iController->ResolvePublicAddrL(
        KIapId,iConnection,iLocalAddr,KDomain,iSocket,iNextHopAddr,this,*this);
    }


void UT_CNATTraversalController::TestCancelL()
    {
    TUint32 requestId = 0;

    // No request found
    iController->Cancel(requestId);

    // Pending request found
    requestId = iController->ResolvePublicAddrL(
        KIapId,iConnection,iLocalAddr,KDomain,iSocket,iNextHopAddr,this,*this);
    iController->Cancel(requestId);

    // Binding found
    requestId = iController->ResolvePublicAddrL(
        KIapId,iConnection,iLocalAddr,KDomain,iSocket,iNextHopAddr,this,*this);
    iController->Cancel(requestId);
    }


void UT_CNATTraversalController::TestRefreshNATBindingL()
    {
    TBool asyncInitiated = EFalse;

    // Bindings do not exist
    iController->RefreshNATBindingL(iSocket,this);

    // Binding exists
    CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    iController->RefreshNATBindingL(iSocket,this);

    // Binding exists, but observer does not match
    iController->RefreshNATBindingL(iSocket,NULL);
    }


void UT_CNATTraversalController::TestRefreshTlsNATBindingL()
    {
    RSocket tcpSocket;
    User::LeaveIfError(tcpSocket.Open(iSocketServ,KAfInet,KSockStream,
                                      KProtocolInetTcp,iConnection));
    CleanupClosePushL(tcpSocket);
    _LIT(KTLS1,"TLS1.0");
    CSecureSocket* secureSocket = CSecureSocket::NewL(tcpSocket,KTLS1);
    CleanupStack::PushL(secureSocket);
    TBool asyncInitiated = EFalse;

    // Bindings do not exist
    iController->RefreshNATBindingL(*secureSocket,this);

    // Binding exists
     CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    iController->RefreshNATBindingL(*secureSocket,this);

    // Binding exists, but observer does not match
    iController->RefreshNATBindingL(*secureSocket,NULL);

    CleanupStack::PopAndDestroy(secureSocket);
    CleanupStack::PopAndDestroy(&tcpSocket);
    }


void UT_CNATTraversalController::TestUpdateNextHopL()
    {
    // No binding
    iController->UpdateNextHop(iNextHopAddr, this);

    // Binding exists, same next hop
    TBool dummy(EFalse);
     CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,dummy);
    CleanupStack::Pop(config);
    iController->UpdateNextHop(iNextHopAddr, this);
    EUNIT_ASSERT(iController->iSessions[0]->iNextHopAddr.CmpAddr(iNextHopAddr));

    // Binding exists, next hop changes
    TInetAddr newNextHop;
    newNextHop.SetAddress(INET_ADDR(123,123,123,123));
    iController->UpdateNextHop(newNextHop, this);
    EUNIT_ASSERT(iController->iSessions[0]->iNextHopAddr.CmpAddr(newNextHop));
    }


void UT_CNATTraversalController::TestSocketIdleL()
    {
    TBool asyncInitiated = EFalse;

    // Bindings do not exist
    iController->SocketIdle(EFalse,iSocket);
    iController->SocketIdle(ETrue,iSocket);

    // Binding exists
     CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    iController->SocketIdle(EFalse,iSocket);
    iController->SocketIdle(ETrue,iSocket);

    // Socket user queue not empty
    iController->AddToSendingQueueL(*this);
    iController->SocketIdle(EFalse,iSocket);
    }


void UT_CNATTraversalController::TestTlsSocketIdleL()
    {
     RSocket tcpSocket;
    User::LeaveIfError(tcpSocket.Open(iSocketServ,KAfInet,KSockStream,
                                      KProtocolInetTcp,iConnection));
    CleanupClosePushL(tcpSocket);
    _LIT(KTLS1,"TLS1.0");
    CSecureSocket* secureSocket = CSecureSocket::NewL(tcpSocket,KTLS1);
    CleanupStack::PushL(secureSocket);
    TBool asyncInitiated = EFalse;

    // Bindings do not exist
    iController->SocketIdle(EFalse,*secureSocket);
    iController->SocketIdle(ETrue,*secureSocket);

    // Binding exists
     CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    iController->SocketIdle(EFalse,*secureSocket);
    iController->SocketIdle(ETrue,*secureSocket);

    CleanupStack::PopAndDestroy(secureSocket);
    CleanupStack::PopAndDestroy(&tcpSocket);
    }


void UT_CNATTraversalController::TestDataReceivedL()
    {
    _LIT8(KMyData, "data");
    TBool handled = EFalse;
    TBool asyncInitiated = EFalse;

    // Bindings do not exist
    iController->DataReceivedL(KMyData,iSocket,handled);
    EUNIT_ASSERT(!handled);

    // Binding exists
     CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    iController->STUNClientInitCompleted(*(iController->iSessions[0]->iSTUNClient),
                                         KErrNone);
    iController->DataReceivedL(KMyData,iSocket,handled);
    }



void UT_CNATTraversalController::TestFreeResourcesL()
    {
    TBool asyncInitiated = EFalse;

    // By IAP ID, not found
    EUNIT_ASSERT(0 == iController->iSessions.Count());
    iController->FreeResources(KIapId);

    // By IAP ID, found and removed
     CNATTraversalConfig* config1 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config1,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config1);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    iController->FreeResources(KIapId);
    EUNIT_ASSERT(0 == iController->iSessions.Count());

    // By binding observer, not found
    iController->FreeResources(*this);

    // By binding observer, found and removed
     CNATTraversalConfig* config2 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config2,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config2);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    iController->FreeResources(*this);
    EUNIT_ASSERT(0 == iController->iSessions.Count());
    }


void UT_CNATTraversalController::TestRequestCompletedL()
    {
    // No request found
    iController->RequestCompleted(0);

    // Request found, asynchronous
    iController->ResolvePublicAddrL(
        KIapId,iConnection,iLocalAddr,KDomain,iSocket,iNextHopAddr,this,*this);
    // Binding exists already
    iRequestId = iController->ResolvePublicAddrL(
        KIapId,iConnection,iLocalAddr,KDomain,iSocket,iNextHopAddr,this,*this);

    CActiveScheduler::Start();
    // The control returns here after
    // callback UT_CNATTraversalController::RequestComplete gets called
    }


void UT_CNATTraversalController::TestSTUNClientInitCompletedL()
    {
    TBool asyncInitiated = EFalse;

    // Bindings do not exist
    CSTUNClient* stunClient = CSTUNClient::NewLC(2000,
                                                 _L8("stun.server.net"),
                                                 3487,
                                                 _L8("stun"),
                                                 iSocketServ,
                                                 iConnection,
                                                 *(iController->iDeltaTimer),
                                                 *iController);
    iController->STUNClientInitCompleted(*stunClient,KErrNone);
    iController->STUNClientInitCompleted(*stunClient,KErrGeneral);
    CleanupStack::PopAndDestroy(stunClient);

    // Binding exists, error during init
     CNATTraversalConfig* config1 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config1,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config1);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    iController->STUNClientInitCompleted(*(iController->iSessions[0]->iSTUNClient),
                                         KErrGeneral);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    iController->iSessions.ResetAndDestroy();

    // Binding exists, init ok
    CNATTraversalConfig* config2 =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config2,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config2);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    iController->STUNClientInitCompleted(*(iController->iSessions[0]->iSTUNClient),
                                         KErrNone);
    EUNIT_ASSERT(1 == iController->iSessions.Count());
    }


void UT_CNATTraversalController::TestSTUNBindingEventOccurredL()
    {
    TBool asyncInitiated = EFalse;

    CSTUNClient* stunClient = CSTUNClient::NewLC(2000,
                                                 _L8("stun.server.net"),
                                                 3487,
                                                 _L8("stun"),
                                                 iSocketServ,
                                                 iConnection,
                                                 *(iController->iDeltaTimer),
                                                 *iController);
    CSTUNBinding* stunBinding = CSTUNBinding::NewLC(*stunClient,iSocket);

    // Bindings do not exist
    iController->STUNBindingEventOccurredL(MSTUNClientObserver::EPublicAddressResolved,
                                           *stunBinding);

    // Bindings exists, no match
    CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    iController->STUNBindingEventOccurredL(MSTUNClientObserver::EPublicAddressResolved,
                                           *stunBinding);

    // Bindings exists, match
    iController->iSessions[0]->iBindings[0]->SetBinding(stunBinding);
    CleanupStack::Pop(stunBinding);
    iController->STUNBindingEventOccurredL(MSTUNClientObserver::EPublicAddressResolved,
                                           *stunBinding);

    CleanupStack::PopAndDestroy(stunClient);
    }


void UT_CNATTraversalController::TestSTUNBindingErrorOccurredL()
    {
    TBool asyncInitiated = EFalse;

    CSTUNClient* stunClient = CSTUNClient::NewLC(2000,
                                                 _L8("stun.server.net"),
                                                 3487,
                                                 _L8("stun"),
                                                 iSocketServ,
                                                 iConnection,
                                                 *(iController->iDeltaTimer),
                                                 *iController);
    CSTUNBinding* stunBinding = CSTUNBinding::NewLC(*stunClient,iSocket);

    // Bindings do not exist
    iController->STUNBindingErrorOccurred(*stunBinding,KErrGeneral);

    // Bindings exists, no match
    CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    iController->STUNBindingErrorOccurred(*stunBinding,KErrGeneral);

    // Bindings exists, match
    iController->iSessions[0]->iBindings[0]->SetBinding(stunBinding);
    CleanupStack::Pop(stunBinding);
    iController->STUNBindingErrorOccurred(*stunBinding,KErrGeneral);

    CleanupStack::PopAndDestroy(stunClient);
    }


// STUN Binding error occurs, causing Registration subsystem to be notified,
// which calls FreeResources, leading ConnectionMgr to call
// CNATTraversalController::FreeResources.
void UT_CNATTraversalController::TestSTUNBindingErrorOccurred2L()
    {
    TBool asyncInitiated = EFalse;

    CSTUNClient* stunClient = CSTUNClient::NewLC(2000,
                                                 _L8("stun.server.net"),
                                                 3487,
                                                 _L8("stun"),
                                                 iSocketServ,
                                                 iConnection,
                                                 *(iController->iDeltaTimer),
                                                 *iController);
    // Create binding and refresher
    CNATTraversalConfig* config =
        CNATTraversalConfig::NewLC(KIapId,KDomain);
    iController->CreateBindingL(1,config,iConnection,iLocalAddr,iSocket,
                                iNextHopAddr,*this,*this,asyncInitiated);
    CleanupStack::Pop(config);
    CSTUNBinding* stunBinding = CSTUNBinding::NewL(*stunClient,iSocket);
    CNATBinding& natBinding = *(iController->iSessions[0]->iBindings[0]);
    natBinding.SetBinding(stunBinding);

    CNATBindingRefresher* refresher =
        new(ELeave)CNATBindingSTUNRefresher(*(iController->iDeltaTimer),
                                            100,
                                            natBinding,
                                            natBinding);
    natBinding.SetRefresher(refresher);


    iController->STUNBindingErrorOccurred(*stunBinding,KErrGeneral);

    // Act like ConnectionMgr calls CNATTraversalController::FreeResources
    iController->FreeResources(*this);


    CleanupStack::PopAndDestroy(stunClient);
    }


void UT_CNATTraversalController::TestAddToSendingQueueL()
    {
    // Empty queue
    EUNIT_ASSERT(!iController->AddToSendingQueueL(*this));
    EUNIT_ASSERT(1 == iController->iSocketUserQueue.Count());

    // Non-empty queue
    EUNIT_ASSERT(iController->AddToSendingQueueL(*this));
    EUNIT_ASSERT(2 == iController->iSocketUserQueue.Count());
    }


void UT_CNATTraversalController::TestSendingCompletedL()
    {
    // Empty queue
    EUNIT_ASSERT(!iController->AddToSendingQueueL(*this));
    EUNIT_ASSERT(1 == iController->iSocketUserQueue.Count());
    iController->SendingCompleted(*this);
    EUNIT_ASSERT(0 == iController->iSocketUserQueue.Count());

    // Non-empty queue
    EUNIT_ASSERT(!iController->AddToSendingQueueL(*this));
    EUNIT_ASSERT(iController->AddToSendingQueueL(*this));
    EUNIT_ASSERT(2 == iController->iSocketUserQueue.Count());
    iController->SendingCompleted(*this);
    EUNIT_ASSERT(1 == iController->iSocketUserQueue.Count());
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATTraversalController,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestHandleNextFromSocketUserQueueL - test ",
    "UT_CNATTraversalController",
    "TestHandleNextFromSocketUserQueueL",
    "FUNCTIONALITY",
    SetupL, TestHandleNextFromSocketUserQueueL, Teardown)

EUNIT_TEST(
    "TestNextRequestIdL - test ",
    "UT_CNATTraversalController",
    "TestNextRequestIdL",
    "FUNCTIONALITY",
    SetupL, TestNextRequestIdL, Teardown)

EUNIT_TEST(
    "TestCreateBindingL - test ",
    "UT_CNATTraversalController",
    "TestCreateBindingL",
    "FUNCTIONALITY",
    SetupL, TestCreateBindingL, Teardown)

EUNIT_TEST(
    "TestFindSessionIndexL - test ",
    "UT_CNATTraversalController",
    "TestFindSessionIndexL",
    "FUNCTIONALITY",
    SetupL, TestFindSessionIndexL, Teardown)

EUNIT_TEST(
    "TestHandleRequestL - test ",
    "UT_CNATTraversalController",
    "TestHandleRequestL",
    "FUNCTIONALITY",
    SetupL, TestHandleRequestL, Teardown)

EUNIT_TEST(
    "TestCancelL - test ",
    "UT_CNATTraversalController",
    "TestCancelL",
    "FUNCTIONALITY",
    SetupL, TestCancelL, Teardown)

EUNIT_TEST(
    "TestRefreshNATBindingL - test ",
    "UT_CNATTraversalController",
    "TestRefreshNATBindingL",
    "FUNCTIONALITY",
    SetupL, TestRefreshNATBindingL, Teardown)

EUNIT_TEST(
    "TestRefreshTlsNATBindingL - test ",
    "UT_CNATTraversalController",
    "TestRefreshTlsNATBindingL",
    "FUNCTIONALITY",
    SetupL, TestRefreshTlsNATBindingL, Teardown)

EUNIT_TEST(
    "TestUpdateNextHopL - test ",
    "UT_CNATTraversalController",
    "TestUpdateNextHopL",
    "FUNCTIONALITY",
    SetupL, TestUpdateNextHopL, Teardown)

EUNIT_TEST(
    "TestRefreshNATBindingL - test ",
    "UT_CNATTraversalController",
    "TestRefreshNATBindingL",
    "FUNCTIONALITY",
    SetupL, TestRefreshNATBindingL, Teardown)

EUNIT_TEST(
    "TestRefreshTlsNATBindingL - test ",
    "UT_CNATTraversalController",
    "TestRefreshTlsNATBindingL",
    "FUNCTIONALITY",
    SetupL, TestRefreshTlsNATBindingL, Teardown)

EUNIT_TEST(
    "TestSocketIdleL - test ",
    "UT_CNATTraversalController",
    "TestSocketIdleL",
    "FUNCTIONALITY",
    SetupL, TestSocketIdleL, Teardown)

EUNIT_TEST(
    "TestTlsSocketIdleL - test ",
    "UT_CNATTraversalController",
    "TestTlsSocketIdleL",
    "FUNCTIONALITY",
    SetupL, TestTlsSocketIdleL, Teardown)

EUNIT_TEST(
    "TestDataReceivedL - test ",
    "UT_CNATTraversalController",
    "TestDataReceivedL",
    "FUNCTIONALITY",
    SetupL, TestDataReceivedL, Teardown)

EUNIT_TEST(
    "TestFreeResourcesL - test ",
    "UT_CNATTraversalController",
    "TestFreeResourcesL",
    "FUNCTIONALITY",
    SetupL, TestFreeResourcesL, Teardown)

EUNIT_TEST(
    "TestRequestCompletedL - test ",
    "UT_CNATTraversalController",
    "TestRequestCompletedL",
    "FUNCTIONALITY",
    SetupL, TestRequestCompletedL, Teardown)

EUNIT_TEST(
    "TestSTUNClientInitCompletedL - test ",
    "UT_CNATTraversalController",
    "TestSTUNClientInitCompletedL",
    "FUNCTIONALITY",
    SetupL, TestSTUNClientInitCompletedL, Teardown)

EUNIT_TEST(
    "TestSTUNBindingEventOccurredL - test ",
    "UT_CNATTraversalController",
    "TestSTUNBindingEventOccurredL",
    "FUNCTIONALITY",
    SetupL, TestSTUNBindingEventOccurredL, Teardown)

EUNIT_TEST(
    "TestSTUNBindingErrorOccurredL - test ",
    "UT_CNATTraversalController",
    "TestSTUNBindingErrorOccurredL",
    "FUNCTIONALITY",
    SetupL, TestSTUNBindingErrorOccurredL, Teardown)

EUNIT_TEST(
    "TestSTUNBindingErrorOccurred2L - test ",
    "UT_CNATTraversalController",
    "TestSTUNBindingErrorOccurred2L",
    "FUNCTIONALITY",
    SetupL, TestSTUNBindingErrorOccurred2L, Teardown)

EUNIT_TEST(
    "TestAddToSendingQueueL - test ",
    "UT_CNATTraversalController",
    "TestAddToSendingQueueL",
    "FUNCTIONALITY",
    SetupL, TestAddToSendingQueueL, Teardown)


EUNIT_TEST(
    "TestSendingCompletedL - test ",
    "UT_CNATTraversalController",
    "TestSendingCompletedL",
    "FUNCTIONALITY",
    SetupL, TestSendingCompletedL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
