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
#include "ut_cnattraversalsession.h"

//  INTERNAL INCLUDES
#include "cnatbinding.h"
#include "cnatbindingrefresher.h"
#include "ctcpcrlfsender.h"
#include "cnatbindingcrlfrefresher.h"
#include "cnattraversalsession.h"
#include "cnattraversalconfig.h"
#include <unsafprotocolscrkeys.h>

//  EXTERNAL INCLUDES
#include <CommDbConnPref.h>
#include <digia/eunit/eunitmacros.h>

//  CONSTANTS
const TUint32 KIapId = 10;
_LIT8(KZeroAddr, "0.0.0.0");
_LIT(KLocalAddr, "10.10.10.10");
_LIT(KPublicAddr, "192.192.192.192");
_LIT(KNextHopAddr, "1.2.3.4");
_LIT8(KDomain, "mydomain.ok");
_LIT(KTLS1,"TLS1.0");

// CONSTRUCTION
UT_CNATTraversalSession* UT_CNATTraversalSession::NewL()
    {
    UT_CNATTraversalSession* self = UT_CNATTraversalSession::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATTraversalSession* UT_CNATTraversalSession::NewLC()
    {
    UT_CNATTraversalSession* self = new( ELeave ) UT_CNATTraversalSession();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATTraversalSession::~UT_CNATTraversalSession()
    {
    }

// Default constructor
UT_CNATTraversalSession::UT_CNATTraversalSession()
    {
    }

// Second phase construct
void UT_CNATTraversalSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS


void UT_CNATTraversalSession::SetupL()
    {
    iDeltaTimer = CDeltaTimer::NewL(CActive::EPriorityStandard);

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

    TInetAddr localAddr;
    User::LeaveIfError(localAddr.Input(KLocalAddr));

    TInetAddr nextHopAddr;
    User::LeaveIfError(nextHopAddr.Input(KNextHopAddr));

	PrepareCentralRepositoryL(KDomain);

    CNATTraversalConfig* config = CNATTraversalConfig::NewLC(KIapId,KDomain);

    iSession = CNATTraversalSession::NewL(*iDeltaTimer,
                                          iSocketServ,
                                          iConnection,
                                          localAddr,
                                          nextHopAddr,
                                          config,
                                          *this,
                                          *this);

    CleanupStack::Pop(config);
    }


void UT_CNATTraversalSession::Teardown()
    {
    delete iSession;
    iSocket.Close();
    iConnection.Close();
    iSocketServ.Close();
    delete iDeltaTimer;
    }



// From MSTUNClientObserver
void UT_CNATTraversalSession::STUNClientInitCompleted(
    const CSTUNClient& /*aClient*/,
    TInt /*aCompletionCode*/)
    {
    }


// From MSTUNClientObserver
void UT_CNATTraversalSession::STUNBindingEventOccurredL(
    TSTUNBindingEvent /*aEvent*/,
    const CBinding& /*aBinding*/)
    {
    HBufC8* tmp = HBufC8::NewL(100);
    delete tmp;
    }


// From MSTUNClientObserver
void UT_CNATTraversalSession::STUNBindingErrorOccurred(
    const CBinding& /*aBinding*/,
    TInt /*aError*/)
    {
    }


// From MSIPNATTraversalRequestObserver
void UT_CNATTraversalSession::RequestComplete(
    TUint32 /*aRequestId*/,
    TInt /*aCompletionCode*/)
    {
    }

// From MSIPNATTraversalRequestObserver
void UT_CNATTraversalSession::PublicAddrResolved(
    TUint32 /*aRequestId*/,
    const TInetAddr& /*aPublicAddr*/)
    {
    }


// From MSIPNATBindingObserver
void UT_CNATTraversalSession::FlowFailure(TInt aError)
    {
    iLastFlowFailure = aError;
    }


// From MNATTraversalSocketManager
TBool UT_CNATTraversalSession::AddToSendingQueueL(
    MNATTraversalSocketUser& /*aUser*/)
    {
    // Make this function leave in low memory conditions
    delete HBufC8::NewL(100);
    return ETrue;
    }


// From MNATTraversalSocketManager
void UT_CNATTraversalSession::SendingCompleted(
    MNATTraversalSocketUser& /*aUser*/)
    {
    }


// Test functions


void UT_CNATTraversalSession::TestConstructionL()
    {
    TInt err = KErrNone;

    TInetAddr localAddr;
    User::LeaveIfError(localAddr.Input(KLocalAddr));

    TInetAddr nextHopAddr;
    User::LeaveIfError(nextHopAddr.Input(KNextHopAddr));

    // Invalid configuration, NULL
    TRAP(err, CNATTraversalSession::NewL(*iDeltaTimer,iSocketServ,
                                         iConnection,localAddr,nextHopAddr,
                                         NULL,*this,*this));
    EUNIT_ASSERT(err != KErrNone);

    // Invalid configuration, 0.0.0.0 STUN server address and no CRLF refresh
    CNATTraversalConfig* config1 = CNATTraversalConfig::NewLC(KIapId,KDomain);

    delete config1->iSTUNServer;
    config1->iSTUNServer = NULL;
    config1->iSTUNServer = KZeroAddr().AllocL();
    config1->iCRLFRefreshEnabled = CNATTraversalConfig::EDisabled;
    TRAP(err, CNATTraversalSession::NewL(*iDeltaTimer,iSocketServ,
                                         iConnection,localAddr,nextHopAddr,
                                         config1,*this,*this));
    CleanupStack::PopAndDestroy(config1);
    EUNIT_ASSERT(err != KErrNone);

    // OK configuration, 0.0.0.0 STUN server address
    CNATTraversalConfig* config2 = CNATTraversalConfig::NewLC(KIapId,KDomain);
    delete config2->iSTUNServer;
    config2->iSTUNServer = NULL;
    config2->iSTUNServer = KZeroAddr().AllocL();
    config2->iCRLFRefreshEnabled = CNATTraversalConfig::EEnabled;
    delete iSession;
    iSession = NULL;
    iSession = CNATTraversalSession::NewL(*iDeltaTimer,iSocketServ,
                                          iConnection,localAddr,nextHopAddr,
                                          config2,*this,*this);
    CleanupStack::Pop(config2);

    // OK configuration, non-zero STUN server address
    CNATTraversalConfig* config3 = CNATTraversalConfig::NewLC(KIapId,KDomain);
    delete config3->iSTUNServer;
    config3->iSTUNServer = NULL;
    config3->iSTUNServer = KDomain().AllocL();
    config3->iCRLFRefreshEnabled = CNATTraversalConfig::EEnabled;
    delete iSession;
    iSession = NULL;
    iSession = CNATTraversalSession::NewL(*iDeltaTimer,iSocketServ,
                                          iConnection,localAddr,nextHopAddr,
                                          config3,*this,*this);
    CleanupStack::Pop(config3);
    }


void UT_CNATTraversalSession::TestBasicSettersAndGettersL()
    {
    EUNIT_ASSERT(!iSession->Domain().CompareF(KDomain));

    EUNIT_ASSERT(KIapId == iSession->IapId());

    EUNIT_ASSERT(!iSession->HasBindings());

    EUNIT_ASSERT(iSession->HasSTUNClient(*(iSession->iSTUNClient)));
    }


void UT_CNATTraversalSession::TestIsPrivateAddressL()
    {
    TInetAddr addr;

    // IPv6
    User::LeaveIfError(addr.Input(_L("::1")));
    EUNIT_ASSERT(!iSession->IsPrivateAddress(addr));

    // IPv4
    addr.SetAddress(INET_ADDR(9,255,255,255));
    EUNIT_ASSERT(!iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(10,0,0,0));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(10,1,2,3));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(10,255,255,255));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(11,0,0,0));
    EUNIT_ASSERT(!iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(172,15,255,255));
    EUNIT_ASSERT(!iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(172,16,0,0));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(172,20,255,255));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(172,31,255,255));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(172,32,0,0));
    EUNIT_ASSERT(!iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(192,167,255,255));
    EUNIT_ASSERT(!iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(192,168,0,0));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(192,168,128,128));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(192,168,255,255));
    EUNIT_ASSERT(iSession->IsPrivateAddress(addr));

    addr.SetAddress(INET_ADDR(192,169,0,0));
    EUNIT_ASSERT(!iSession->IsPrivateAddress(addr));
    }


void UT_CNATTraversalSession::TestIsBehindNATL()
    {
    TBool async(EFalse);

    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);

    // Address for the binding not resolved, local address private
    EUNIT_ASSERT(iSession->IsBehindNAT(binding));

    // Address for the binding not resolved, local address not private
    User::LeaveIfError(iSession->iLocalAddr.Input(KPublicAddr));
    EUNIT_ASSERT(!iSession->IsBehindNAT(binding));

    // Address for the binding resolved, local address not matching
    User::LeaveIfError(iSession->iLocalAddr.Input(KLocalAddr));
    TInetAddr publicAddr;
    User::LeaveIfError(publicAddr.Input(KPublicAddr));
    binding.SetPublicAddr(publicAddr);
    EUNIT_ASSERT(iSession->IsBehindNAT(binding));

    // Address for the binding resolved, local address matching
    User::LeaveIfError(iSession->iLocalAddr.Input(KPublicAddr));
    EUNIT_ASSERT(!iSession->IsBehindNAT(binding));
    }


void UT_CNATTraversalSession::TestCreateRefresherL()
    {
    TBool async(EFalse);
    TInt err = KErrNone;
    TBool persistentConnection = EFalse;

    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    binding.SetBinding(
        CSTUNBinding::NewL(*iSession->iSTUNClient,binding.STUNSocket()));

    // TCP socket
    RSocket tcpSocket;
    User::LeaveIfError(tcpSocket.Open(iSocketServ,KAfInet,KSockStream,
                                      KProtocolInetTcp,iConnection));
    CleanupClosePushL(tcpSocket);
    iSession->CreateRefresherL(binding,tcpSocket,persistentConnection);
    EUNIT_ASSERT(persistentConnection);
    EUNIT_ASSERT(binding.Refresher() != NULL);
    CleanupStack::PopAndDestroy(&tcpSocket);
    binding.SetRefresher(NULL);
    persistentConnection = EFalse;

    // UDP socket, binding ok, sender address different than STUN server address
    TInetAddr publicAddr;
    User::LeaveIfError(publicAddr.Input(KPublicAddr));
    binding.SetPublicAddr(publicAddr);
    iSession->CreateRefresherL(binding,iSocket,persistentConnection);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(binding.Refresher() != NULL);
    binding.SetRefresher(NULL);

    // UDP socket, binding ok, sender address same as STUN server address
    iSession->iNextHopAddr = iSession->iSTUNClient->STUNServerAddrL();
    iSession->CreateRefresherL(binding,iSocket,persistentConnection);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(binding.Refresher() != NULL);
    binding.SetRefresher(NULL);

    // UDP socket, binding failed, CRLF refresh not enabled
    binding.SetBindingFailed(KErrGeneral);
    iSession->CreateRefresherL(binding,iSocket,persistentConnection);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(binding.Refresher() == NULL);

    // UDP socket, binding failed, CRLF refresh enabled
    iSession->iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EEnabled;
    binding.SetBindingFailed(KErrGeneral);
    iSession->CreateRefresherL(binding,iSocket,persistentConnection);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(binding.Refresher() != NULL);
    binding.SetRefresher(NULL);
    iSession->iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EDisabled;

    // ICMP socket, leave with KErrNotSupported
    RSocket icmpSocket;
    User::LeaveIfError(icmpSocket.Open(iSocketServ, KAfInet, KSockDatagram,
                                       KProtocolInetIcmp));
    CleanupClosePushL(icmpSocket);
    TRAP(err,iSession->CreateRefresherL(binding,icmpSocket,persistentConnection));
    EUNIT_ASSERT(KErrNotSupported == err);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(binding.Refresher() == NULL);
    CleanupStack::PopAndDestroy(&icmpSocket);
    }


void UT_CNATTraversalSession::TestInitCompletedL()
    {
    TBool async(EFalse);

    // No bindings
    iSession->InitCompletedL();

    // Bindings exist
    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    iSession->InitCompletedL();
    EUNIT_ASSERT(binding.Binding() != NULL);
    }


void UT_CNATTraversalSession::TestInitFailedL()
    {
    TBool async(EFalse);

    // No bindings
    iSession->InitFailed(KErrGeneral);

    // Bindings exist
    iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    EUNIT_ASSERT(iSession->HasBindings());
    iSession->InitFailed(KErrGeneral);
    EUNIT_ASSERT(iSession->HasBindings());
    }


void UT_CNATTraversalSession::TestAddressResolvedL()
    {
    TBool async(EFalse);
    CSTUNBinding* stunBinding =
        CSTUNBinding::NewLC(*iSession->iSTUNClient,iSocket);

    // No bindings
    iSession->AddressResolvedL(*stunBinding);

    // Binding exists, first time called
    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    binding.SetBinding(stunBinding);
    CleanupStack::Pop(stunBinding);
    iSession->AddressResolvedL(*stunBinding);

    // Bindings exist, non-matching STUN binding
    CSTUNBinding* stunBinding2 =
        CSTUNBinding::NewLC(*iSession->iSTUNClient,iSocket);
    iSession->AddressResolvedL(*stunBinding2);
    CleanupStack::PopAndDestroy(stunBinding2);

    // Binding exists, public address changed
     TInetAddr addr;
    addr.SetAddress(INET_ADDR(123,123,123,123));
    binding.SetPublicAddr(addr);
    iSession->AddressResolvedL(*stunBinding);
    }


void UT_CNATTraversalSession::TestBindingFailedL()
    {
    TBool async(EFalse);
    CSTUNBinding* stunBinding =
        CSTUNBinding::NewLC(*iSession->iSTUNClient,iSocket);

    // No bindings
    iSession->BindingFailed(*stunBinding,KErrGeneral);

    // Bindings exist, non-matching STUN binding
    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    binding.SetBinding(stunBinding);
    CleanupStack::Pop(stunBinding);
    EUNIT_ASSERT(iSession->HasBindings());
    CSTUNBinding* stunBinding2 =
        CSTUNBinding::NewLC(*iSession->iSTUNClient,iSocket);
    iSession->BindingFailed(*stunBinding2,KErrGeneral);
    CleanupStack::PopAndDestroy(stunBinding2);
    EUNIT_ASSERT(iSession->HasBindings());

    // Bindings exist, matching STUN binding
    iSession->BindingFailed(*stunBinding,KErrGeneral);
    EUNIT_ASSERT(iSession->HasBindings());
    }


void UT_CNATTraversalSession::TestRefreshNATBindingL()
    {
    TBool async(EFalse);
    TBool persistentConnection = EFalse;
    TBool handled = EFalse;

    // Bindings do not exist
    iSession->RefreshNATBindingL(iSocket,this,persistentConnection,handled);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(!handled);

    // Binding exists, no refresher created, in public address space
    User::LeaveIfError(iSession->iLocalAddr.Input(KPublicAddr));
    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    binding.SetBinding(
        CSTUNBinding::NewL(*iSession->iSTUNClient,binding.STUNSocket()));
    iSession->RefreshNATBindingL(iSocket,this,persistentConnection,handled);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(handled);
    EUNIT_ASSERT(binding.Refresher() == NULL);
    handled = EFalse;

    // Binding exists, refresher created, in public address space, CRLF refresh
    iSession->iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EEnabled;
    User::LeaveIfError(iSession->iLocalAddr.Input(KPublicAddr));
    iSession->iNextHopAddr = iSession->iSTUNClient->STUNServerAddrL();
    TInetAddr publicAddr;
    User::LeaveIfError(publicAddr.Input(KPublicAddr));
    binding.SetPublicAddr(publicAddr);
    iSession->RefreshNATBindingL(iSocket,this,persistentConnection,handled);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(handled);
    EUNIT_ASSERT(binding.Refresher() != NULL);
    iSession->iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EDisabled;

    // Binding and refresher exist
    iSession->RefreshNATBindingL(iSocket,this,persistentConnection,handled);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(handled);
    EUNIT_ASSERT(binding.Refresher() != NULL);
    }


void UT_CNATTraversalSession::TestRefreshTlsNATBindingL()
    {
    TBool async(EFalse);
    TBool persistentConnection = EFalse;
    TBool handled = EFalse;

    RSocket tcpSocket;
    User::LeaveIfError(tcpSocket.Open(iSocketServ,KAfInet,KSockStream,
                                      KProtocolInetTcp,iConnection));
    CleanupClosePushL(tcpSocket);
    CSecureSocket* secureSocket = CSecureSocket::NewL(tcpSocket,KTLS1);
    CleanupStack::PushL(secureSocket);

    // Bindings do not exist
    iSession->RefreshNATBindingL(
        *secureSocket,this,persistentConnection,handled);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(!handled);

    // Binding exists, no need for refreshing
    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    binding.SetBinding(
        CSTUNBinding::NewL(*iSession->iSTUNClient,binding.STUNSocket()));
    User::LeaveIfError(iSession->iLocalAddr.Input(KPublicAddr));
    iSession->RefreshNATBindingL(
        *secureSocket,this,persistentConnection,handled);
    EUNIT_ASSERT(!persistentConnection);
    EUNIT_ASSERT(handled);
    EUNIT_ASSERT(binding.Refresher() == NULL);
    handled = EFalse;

    // Binding exists, needs a refresh
    User::LeaveIfError(iSession->iLocalAddr.Input(KLocalAddr));
    TInetAddr publicAddr;
    User::LeaveIfError(publicAddr.Input(KPublicAddr));
    binding.SetPublicAddr(publicAddr);
    iSession->RefreshNATBindingL(
        *secureSocket,this,persistentConnection,handled);
    EUNIT_ASSERT(persistentConnection);
    EUNIT_ASSERT(handled);
    EUNIT_ASSERT(binding.Refresher() != NULL);
    handled = EFalse;

    // Binding and refresher exist
    iSession->RefreshNATBindingL(
        *secureSocket,this,persistentConnection,handled);
    EUNIT_ASSERT(persistentConnection);
    EUNIT_ASSERT(handled);

    CleanupStack::PopAndDestroy(secureSocket);
    CleanupStack::PopAndDestroy(&tcpSocket);
    }


void UT_CNATTraversalSession::TestSocketIdleL()
    {
    TBool async(EFalse);
    RSocket tcpSocket;
    User::LeaveIfError(tcpSocket.Open(iSocketServ,KAfInet,KSockStream,
                                      KProtocolInetTcp,iConnection));
    CleanupClosePushL(tcpSocket);

    // No bindings
    iSession->SocketIdle(ETrue,tcpSocket);
    iSession->SocketIdle(EFalse,tcpSocket);

    // Binding exists, but refresher does not
    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    CSTUNBinding* stunBinding =
        CSTUNBinding::NewL(*iSession->iSTUNClient,binding.STUNSocket());
    binding.SetBinding(stunBinding);
    iSession->SocketIdle(ETrue,tcpSocket);
    iSession->SocketIdle(EFalse,tcpSocket);

    // Binding and TCP refresher exist
    TInetAddr addr;
    addr.SetAddress(INET_ADDR(1,2,3,4));
    CTcpCRLFSender* sender =
        new(ELeave)CTcpCRLFSender(iSocket,binding);
    CleanupStack::PushL(sender);
    CNATBindingRefresher* refresher =
        new(ELeave)CNATBindingCRLFRefresher(*iDeltaTimer,120,binding,sender);
    CleanupStack::Pop(sender);
    binding.SetRefresher(refresher);
    iSession->SocketIdle(ETrue,tcpSocket);
    iSession->SocketIdle(EFalse,tcpSocket);

    // TLS socket, no binding
    CSecureSocket* secureSocket = CSecureSocket::NewL(tcpSocket,KTLS1);
    CleanupStack::PushL(secureSocket);
    iSession->SocketIdle(ETrue,*secureSocket);
    iSession->SocketIdle(EFalse,*secureSocket);
    CleanupStack::PopAndDestroy(secureSocket);

    CleanupStack::PopAndDestroy(&tcpSocket);
    }


void UT_CNATTraversalSession::TestDataReceivedL()
    {
    TBool async(EFalse);
    _LIT8(KMyTestData, "test data");
    TBool handled = EFalse;

    // No bindings
    iSession->DataReceivedL(KMyTestData,iSocket,handled);
    EUNIT_ASSERT(!handled);

    // Binding exists
    CNATBinding& binding =
        iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    iSession->DataReceivedL(KMyTestData,iSocket,handled);
    EUNIT_ASSERT(handled);
    }


void UT_CNATTraversalSession::TestRemoveBindingL()
    {
    TBool async(EFalse);

    // By request ID, bindings do not exist, not found
    EUNIT_ASSERT(!iSession->RemoveBinding(1));

    // By request ID, binding exists, does not match
    iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    EUNIT_ASSERT(iSession->HasBindings());
    EUNIT_ASSERT(!iSession->RemoveBinding(2));
    EUNIT_ASSERT(iSession->HasBindings());

    // By request ID, bindings exists, matches
    iSession->CreateNATBindingL(2,iSocket,*this,*this,async);
    EUNIT_ASSERT(iSession->HasBindings());
    EUNIT_ASSERT(iSession->RemoveBinding(2));
    EUNIT_ASSERT(iSession->HasBindings());
    EUNIT_ASSERT(iSession->RemoveBinding(1));
    EUNIT_ASSERT(!iSession->HasBindings());

    // By observer, bindings do not exist, not found
    EUNIT_ASSERT(!iSession->RemoveBinding(*this));

    // By observer, binding exists and matches
    iSession->CreateNATBindingL(1,iSocket,*this,*this,async);
    EUNIT_ASSERT(iSession->HasBindings());
    EUNIT_ASSERT(iSession->RemoveBinding(*this));
    EUNIT_ASSERT(!iSession->HasBindings());
    }


// Since the behaviour for TCP has changed when
// KUNSAFProtocolsDomainEnableCRLFRefresh is not defined in the repository,
// set it here.
void UT_CNATTraversalSession::PrepareCentralRepositoryL(const TDesC8& aDomain)
	{	
    CRepository* cr = CRepository::NewLC(KCRUidUNSAFProtocols);

	RArray<TUint32> keys;
    CleanupClosePushL(keys);
    TInt err = cr->FindEqL(KUNSAFProtocolsDomainMask,
                           KUNSAFProtocolsFieldTypeMask,
                           aDomain,
                           keys);
    if (err == KErrNone && keys.Count() == 1)
        {
        TUint32 key = (KUNSAFProtocolsDomainMask^(keys[0]));
        key |= KUNSAFProtocolsDomainTableMask;        

        // Set "CRLF refresh" parameter to value "disabled"
		User::LeaveIfError(cr->Set(KUNSAFProtocolsDomainEnableCRLFRefresh | key,
								   CNATTraversalConfig::EDisabled));
        }

	CleanupStack::PopAndDestroy(&keys);
    CleanupStack::PopAndDestroy(cr);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATTraversalSession,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestConstructionL - test ",
    "UT_CNATTraversalSession",
    "TestConstructionL",
    "FUNCTIONALITY",
    SetupL, TestConstructionL, Teardown)

EUNIT_TEST(
    "TestBasicSettersAndGettersL - test ",
    "UT_CNATTraversalSession",
    "TestBasicSettersAndGettersL",
    "FUNCTIONALITY",
    SetupL, TestBasicSettersAndGettersL, Teardown)

EUNIT_TEST(
    "TestIsPrivateAddressL - test ",
    "UT_CNATTraversalSession",
    "TestIsPrivateAddressL",
    "FUNCTIONALITY",
    SetupL, TestIsPrivateAddressL, Teardown)

EUNIT_TEST(
    "TestIsBehindNATL - test ",
    "UT_CNATTraversalSession",
    "TestIsBehindNATL",
    "FUNCTIONALITY",
    SetupL, TestIsBehindNATL, Teardown)

EUNIT_TEST(
    "TestCreateRefresherL - test ",
    "UT_CNATTraversalSession",
    "TestCreateRefresherL",
    "FUNCTIONALITY",
    SetupL, TestCreateRefresherL, Teardown)

EUNIT_TEST(
    "TestInitCompletedL - test ",
    "UT_CNATTraversalSession",
    "TestInitCompletedL",
    "FUNCTIONALITY",
    SetupL, TestInitCompletedL, Teardown)

EUNIT_TEST(
    "TestInitFailedL - test ",
    "UT_CNATTraversalSession",
    "TestInitFailedL",
    "FUNCTIONALITY",
    SetupL, TestInitFailedL, Teardown)

EUNIT_TEST(
    "TestAddressResolvedL - test ",
    "UT_CNATTraversalSession",
    "TestAddressResolvedL",
    "FUNCTIONALITY",
    SetupL, TestAddressResolvedL, Teardown)

EUNIT_TEST(
    "TestBindingFailedL - test ",
    "UT_CNATTraversalSession",
    "TestBindingFailedL",
    "FUNCTIONALITY",
    SetupL, TestBindingFailedL, Teardown)

EUNIT_TEST(
    "TestRefreshNATBindingL - test ",
    "UT_CNATTraversalSession",
    "TestRefreshNATBindingL",
    "FUNCTIONALITY",
    SetupL, TestRefreshNATBindingL, Teardown)

EUNIT_TEST(
    "TestRefreshTlsNATBindingL - test ",
    "UT_CNATTraversalSession",
    "TestRefreshTlsNATBindingL",
    "FUNCTIONALITY",
    SetupL, TestRefreshTlsNATBindingL, Teardown)

EUNIT_TEST(
    "TestSocketIdleL - test ",
    "UT_CNATTraversalSession",
    "TestSocketIdleL",
    "FUNCTIONALITY",
    SetupL, TestSocketIdleL, Teardown)

EUNIT_TEST(
    "TestDataReceivedL - test ",
    "UT_CNATTraversalSession",
    "TestDataReceivedL",
    "FUNCTIONALITY",
    SetupL, TestDataReceivedL, Teardown)

EUNIT_TEST(
    "TestRemoveBindingL - test ",
    "UT_CNATTraversalSession",
    "TestRemoveBindingL",
    "FUNCTIONALITY",
    SetupL, TestRemoveBindingL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
