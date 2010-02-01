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
#include "ut_cnatbinding.h"

//  INTERNAL INCLUDES
#include "CNATBindingSTUNRefresher.h"
#include "CNATBinding.h"

//  EXTERNAL INCLUDES
#include <CommDbConnPref.h>
#include <digia/eunit/EUnitMacros.h>
#include "natfwstunclient.h"
#include "natfwstunbinding.h"

//  CONSTANTS
const TUint32 KIapId = 10;
const TUint32 KInitialRequestId = 2;

// CONSTRUCTION
UT_CNATBinding* UT_CNATBinding::NewL()
    {
    UT_CNATBinding* self = UT_CNATBinding::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATBinding* UT_CNATBinding::NewLC()
    {
    UT_CNATBinding* self = new( ELeave ) UT_CNATBinding();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATBinding::~UT_CNATBinding()
    {
    }

// Default constructor
UT_CNATBinding::UT_CNATBinding()
    {
    }

// Second phase construct
void UT_CNATBinding::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS
void UT_CNATBinding::SetupL()
    {
    iLocalAddr.SetAddress(INET_ADDR(1,2,3,4));
    iLastFlowFailure = 0;

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

    iSTUNClient = CSTUNClient::NewL(2000,
                                    _L8("stun.server.net"),
                                    3487,
                                    _L8("stun"),
                                    iSocketServ,
                                    iConnection,
                                    *iDeltaTimer,
                                    *this);


    iBinding = CNATBinding::NewL(
        KInitialRequestId,iSocket,iLocalAddr,*this,*this,*this);
    }


void UT_CNATBinding::TeardownL()
    {
    delete iBinding;
    delete iSTUNClient;
    iSocket.Close();
    iConnection.Close();
    iSocketServ.Close();
    delete iDeltaTimer;
    }



// From MSTUNClientObserver
void UT_CNATBinding::STUNClientInitCompleted(
    const CSTUNClient& /*aClient*/,
    TInt /*aCompletionCode*/)
    {
    }


// From MSTUNClientObserver
void UT_CNATBinding::STUNBindingEventOccurredL(
    TSTUNBindingEvent /*aEvent*/,
    const CBinding& /*aBinding*/)
    {
    HBufC8* tmp = HBufC8::NewL(100);
    delete tmp;
    }


// From MSTUNClientObserver
void UT_CNATBinding::STUNBindingErrorOccurred(
    const CBinding& /*aBinding*/,
    TInt /*aError*/)
    {
    }


// From MSIPNATTraversalRequestObserver
void UT_CNATBinding::RequestComplete(
    TUint32 aRequestId,
    TInt aCompletionCode)
    {
    EUNIT_ASSERT(iBinding->InitialRequestId() == aRequestId);
    iRequestCompleteCalled = ETrue;
    iRequestCompleteCode = aCompletionCode;
    }

// From MSIPNATTraversalRequestObserver
void UT_CNATBinding::PublicAddrResolved(
    TUint32 aRequestId,
    const TInetAddr& aPublicAddr)
    {
    EUNIT_ASSERT(iBinding->InitialRequestId() == aRequestId);
    iPublicAddrResolvedCalled = ETrue;
    EUNIT_ASSERT(aPublicAddr.Match(iBinding->PublicAddr()));
    }


// From MSIPNATBindingObserver
void UT_CNATBinding::FlowFailure(TInt aError)
    {
    iLastFlowFailure = aError;
    }


// From MNATTraversalSocketManager
TBool UT_CNATBinding::AddToSendingQueueL(
    MNATTraversalSocketUser& /*aUser*/)
    {
    // Make this function leave in low memory conditions
    delete HBufC8::NewL(100);
    return iSendingNotAllowed;
    }


// From MNATTraversalSocketManager
void UT_CNATBinding::SendingCompleted(
    MNATTraversalSocketUser& /*aUser*/)
    {
    iSendingCompletedCalled = ETrue;
    }


// Test functions

void UT_CNATBinding::TestBasicSettersAndGettersL()
    {
    // STUN socket
    EUNIT_ASSERT(iSocket.SubSessionHandle() ==
                      iBinding->STUNSocket().SubSessionHandle());

    // STUN binding
    EUNIT_ASSERT(!iBinding->Binding());
    CSTUNBinding* stunBinding =
        CSTUNBinding::NewL(*iSTUNClient,iSocket);
    iBinding->SetBinding(stunBinding);
    EUNIT_ASSERT(iBinding->Binding() == stunBinding);

    // Binding refresher
    EUNIT_ASSERT(!iBinding->Refresher());
    CNATBindingSTUNRefresher* refresher =
        new(ELeave)CNATBindingSTUNRefresher(*iDeltaTimer,1,*iBinding,
                                            *iBinding);
    iBinding->SetRefresher(refresher);
    EUNIT_ASSERT(iBinding->Refresher() == refresher);

    // Initial request ID
    EUNIT_ASSERT(KInitialRequestId == iBinding->InitialRequestId());

    // NAT binding observer
    EUNIT_ASSERT(this == &(iBinding->NATBindingObserver()));
    }


void UT_CNATBinding::TestSendSTUNRequestL()
    {
    TInt err = KErrNone;

    // STUN binding not set
    TRAP(err, iBinding->SendSTUNRequestL());
    EUNIT_ASSERT(KErrNotReady == err);

    // STUN binding set, request not put to queue
    CSTUNBinding* stunBinding =
        CSTUNBinding::NewL(*iSTUNClient,iSocket);
    iBinding->SetBinding(stunBinding);
    iBinding->SendSTUNRequestL();

    // STUN binding set, request put to queue
    iSendingNotAllowed = ETrue;
    iBinding->SendSTUNRequestL();
    iBinding->SendingAllowed();
    iBinding->CancelSending();
    iBinding->SetBinding(NULL);
    iBinding->SendingAllowed();
    iBinding->CancelSending();
    }


void UT_CNATBinding::TestPublicAddrL()
    {
    TInetAddr publicAddr;
    publicAddr.SetAddress(INET_ADDR(5,6,7,8));

    // Public address not set
    EUNIT_ASSERT(!iBinding->SetPublicAddr(publicAddr));
    EUNIT_ASSERT(iSendingCompletedCalled);
    EUNIT_ASSERT(iPublicAddrResolvedCalled);
    EUNIT_ASSERT(!iRequestCompleteCalled);
    EUNIT_ASSERT(iBinding->AddressResolved());
    EUNIT_ASSERT(iBinding->PublicAddr().Match(publicAddr));

    // Public address already set, address not changed
    iSendingCompletedCalled = EFalse;
    iPublicAddrResolvedCalled = EFalse;
    EUNIT_ASSERT(!iBinding->SetPublicAddr(publicAddr));
    EUNIT_ASSERT(iSendingCompletedCalled);
    EUNIT_ASSERT(!iPublicAddrResolvedCalled);
    EUNIT_ASSERT(!iRequestCompleteCalled);
    EUNIT_ASSERT(iBinding->AddressResolved());
    EUNIT_ASSERT(iBinding->PublicAddr().Match(publicAddr));

    // Public address already set, address changed
    TInetAddr publicAddr2;
    publicAddr2.SetAddress(INET_ADDR(6,7,8,9));
    iSendingCompletedCalled = EFalse;
    EUNIT_ASSERT(iBinding->SetPublicAddr(publicAddr2));
    EUNIT_ASSERT(iSendingCompletedCalled);
    EUNIT_ASSERT(!iPublicAddrResolvedCalled);
    EUNIT_ASSERT(!iRequestCompleteCalled);
    }


void UT_CNATBinding::TestBindingFailedL()
    {
    // Public address not set
    EUNIT_ASSERT(!iBinding->AddressResolved());
    iSendingCompletedCalled = EFalse;
    iBinding->SetBindingFailed(KErrGeneral);
    EUNIT_ASSERT(iSendingCompletedCalled);
    EUNIT_ASSERT(iRequestCompleteCalled);
    EUNIT_ASSERT(KErrNone == iRequestCompleteCode);
    EUNIT_ASSERT(KErrNone == iLastFlowFailure);

    // Public address set
    iSendingCompletedCalled = EFalse;
    iRequestCompleteCalled = EFalse;
    iLastFlowFailure = KErrNone;
    EUNIT_ASSERT(!iBinding->SetPublicAddr(iLocalAddr));
    EUNIT_ASSERT(iBinding->AddressResolved());
    EUNIT_ASSERT(iSendingCompletedCalled);
    EUNIT_ASSERT(iRequestCompleteCalled);
    EUNIT_ASSERT(KErrNone == iRequestCompleteCode);
    iRequestCompleteCalled = EFalse;
    iSendingCompletedCalled = EFalse;
    iBinding->SetBindingFailed(KErrGeneral);
    EUNIT_ASSERT(iSendingCompletedCalled);
    EUNIT_ASSERT(!iBinding->AddressResolved());
    EUNIT_ASSERT(KErrGeneral == iLastFlowFailure);
    EUNIT_ASSERT(!iRequestCompleteCalled);
    EUNIT_ASSERT(KErrNone == iRequestCompleteCode);
    }


void UT_CNATBinding::TestHandleDataL()
    {
    _LIT8(KTmpData, "data");
    TBool handled = EFalse;

    // STUN binding not set
    iBinding->HandleDataL(KTmpData,iSocket,handled);
    EUNIT_ASSERT(!handled);

    // STUN binding set
    CSTUNBinding* stunBinding =
        CSTUNBinding::NewL(*iSTUNClient,iSocket);
    iBinding->SetBinding(stunBinding);
    iBinding->HandleDataL(KTmpData,iSocket,handled);
    EUNIT_ASSERT(handled);
    }


void UT_CNATBinding::TestNATBindingRefreshCompleteL()
    {
    EUNIT_ASSERT(KErrNone == iLastFlowFailure);

    iBinding->NATBindingRefreshComplete(KErrNone);
    EUNIT_ASSERT(KErrNone == iLastFlowFailure);

    iBinding->NATBindingRefreshComplete(KErrGeneral);
    EUNIT_ASSERT(KErrGeneral == iLastFlowFailure);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATBinding,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestBasicSettersAndGettersL - test ",
    "UT_CNATBinding",
    "TestBasicSettersAndGettersL",
    "FUNCTIONALITY",
    SetupL, TestBasicSettersAndGettersL, TeardownL)

EUNIT_TEST(
    "TestSendSTUNRequestL - test ",
    "UT_CNATBinding",
    "TestSendSTUNRequestL",
    "FUNCTIONALITY",
    SetupL, TestSendSTUNRequestL, TeardownL)

EUNIT_TEST(
    "TestPublicAddrL - test ",
    "UT_CNATBinding",
    "TestPublicAddrL",
    "FUNCTIONALITY",
    SetupL, TestPublicAddrL, TeardownL)

EUNIT_TEST(
    "TestBindingFailedL - test ",
    "UT_CNATBinding",
    "TestSetRefreshL",
    "FUNCTIONALITY",
    SetupL, TestBindingFailedL, TeardownL)

EUNIT_TEST(
    "TestHandleDataL - test ",
    "UT_CNATBinding",
    "TestHandleDataL",
    "FUNCTIONALITY",
    SetupL, TestHandleDataL, TeardownL)

EUNIT_TEST(
    "TestNATBindingRefreshCompleteL - test ",
    "UT_CNATBinding",
    "TestNATBindingRefreshCompleteL",
    "FUNCTIONALITY",
    SetupL, TestNATBindingRefreshCompleteL, TeardownL)

EUNIT_END_TEST_TABLE

//  END OF FILE
