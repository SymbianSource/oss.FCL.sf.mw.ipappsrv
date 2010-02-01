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
#include "ut_cnatbindingstunrefresher.h"

//  INTERNAL INCLUDES
#include "CNATBindingSTUNRefresher.h"
#include "CNATBinding.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

#include "natfwstunclient.h"
#include "natfwstunbinding.h"

#include <CommDbConnPref.h>
#include <SecureSocket.h>

//  CONSTANTS
const TInt KRefreshInterval = 10;


// CONSTRUCTION
UT_CNATBindingSTUNRefresher* UT_CNATBindingSTUNRefresher::NewL()
    {
    UT_CNATBindingSTUNRefresher* self = UT_CNATBindingSTUNRefresher::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATBindingSTUNRefresher* UT_CNATBindingSTUNRefresher::NewLC()
    {
    UT_CNATBindingSTUNRefresher* self = new( ELeave ) UT_CNATBindingSTUNRefresher();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATBindingSTUNRefresher::~UT_CNATBindingSTUNRefresher()
    {
    }

// Default constructor
UT_CNATBindingSTUNRefresher::UT_CNATBindingSTUNRefresher()
    {
    }

// Second phase construct
void UT_CNATBindingSTUNRefresher::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS
void UT_CNATBindingSTUNRefresher::SetupL()
    {
    iDeltaTimer = CDeltaTimer::NewL(CActive::EPriorityStandard);

    User::LeaveIfError(iSocketServ.Connect());

    User::LeaveIfError(iConnection.Open(iSocketServ));
    TCommDbConnPref prefs;
    prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
    prefs.SetDirection(ECommDbConnectionDirectionOutgoing);
    prefs.SetIapId(5);
    
    User::LeaveIfError(iConnection.Start(prefs));

    User::LeaveIfError(iSocket.Open(iSocketServ,
                                    KAfInet,
                                    KSockDatagram,
                                    KProtocolInetUdp,
                                    iConnection));

    iSTUNClient = CSTUNClient::NewL(2000,_L8("STUNServer"),0,_L8("stun"),
                                    iSocketServ,iConnection,*iDeltaTimer,*this);

    CSTUNBinding* STUNBinding = CSTUNBinding::NewLC(*iSTUNClient,iSocket);

    TInetAddr addr;
    iNATBinding = CNATBinding::NewL(89,iSocket,addr,
        *(MSIPNATTraversalRequestObserver*)NULL,
        *(MSIPNATBindingObserver*)NULL,
        *this);
    CleanupStack::Pop(STUNBinding);
    // Ownership is transferred
    iNATBinding->SetBinding(STUNBinding);

    iRefresher = new(ELeave)CNATBindingSTUNRefresher(*iDeltaTimer,
                                                     KRefreshInterval,
                                                     *this,*iNATBinding);
    }


void UT_CNATBindingSTUNRefresher::Teardown()
    {
    iSocket.Close();
    iConnection.Close();
    iSocketServ.Close();
    delete iRefresher;
    iRefresher = 0;
    delete iSTUNClient;
    iSTUNClient = 0;
    //iNATBinding owns iSTUNBinding
    delete iNATBinding;
    iNATBinding = 0;
    delete iDeltaTimer;
    iDeltaTimer = 0;
    }


// From MNATBindingRefresherObserver
void UT_CNATBindingSTUNRefresher::NATBindingRefreshComplete(
    TInt /*aCompletionCode*/)
    {
    }


// From MSTUNClientObserver
void UT_CNATBindingSTUNRefresher::STUNClientInitCompleted(
    const CSTUNClient& /*aClient*/,
    TInt /*aCompletionCode*/)
    {
    }


// From MSTUNClientObserver
void UT_CNATBindingSTUNRefresher::STUNBindingEventOccurredL(
    TSTUNBindingEvent /*aEvent*/,
    const CBinding& /*aBinding*/)
    {
    HBufC8* tmp = HBufC8::NewL(100);
    delete tmp;
    }


// From MSTUNClientObserver
void UT_CNATBindingSTUNRefresher::STUNBindingErrorOccurred(
    const CBinding& /*aBinding*/,
    TInt /*aError*/)
    {
    }


// From MNATTraversalSocketManager
TBool UT_CNATBindingSTUNRefresher::AddToSendingQueueL(
    MNATTraversalSocketUser& /*aUser*/)
    {
    // Make this function leave in low memory conditions
    delete HBufC8::NewL(100);
    return EFalse;
    }


// From MNATTraversalSocketManager
void UT_CNATBindingSTUNRefresher::SendingCompleted(
    MNATTraversalSocketUser& /*aUser*/)
    {
    }


// Test functions

void UT_CNATBindingSTUNRefresher::TestRefreshIntervalL()
    {
    // Generate 10 refresh values and check them
    for (TInt i=0; i<10; i++)
        {
        TInt interval = iRefresher->RefreshInterval();
        EUNIT_ASSERT(interval >= KRefreshInterval*900000);
        EUNIT_ASSERT(interval <= KRefreshInterval*1100000);
        }
    // Invalid interval
    iRefresher->iRefreshInterval = 0;
    EUNIT_ASSERT_EQUALS(KErrUnderflow, iRefresher->RefreshInterval());
    }


void UT_CNATBindingSTUNRefresher::TestHasSocketL()
    {

    // Own UDP socket
    EUNIT_ASSERT_EQUALS(ETrue, iRefresher->HasSocket(iSocket));

    //TCP socket
    RSocket tcpSocket;
    CleanupClosePushL(tcpSocket);
    User::LeaveIfError(tcpSocket.Open(iSocketServ,KAfInet,KSockStream,
                                     KProtocolInetTcp,iConnection));
       
    // Because we have stubbed RSocket, normal subsessionhandle is not working
    // normally. Therefore we have to manually change its action here.                              
    tcpSocket.ChangeHandling(EFalse); // subsessionhandle, use own variation
                                                         
    EUNIT_ASSERT_EQUALS(EFalse, iRefresher->HasSocket(tcpSocket));

	tcpSocket.ChangeHandling(ETrue); // back to defaul behavior

    // TLS socket
    _LIT(KTLS1,"TLS1.0");
    CSecureSocket* secureSocket = CSecureSocket::NewL(tcpSocket,KTLS1);
    CleanupStack::PushL(secureSocket);
    

    EUNIT_ASSERT_EQUALS(EFalse, iRefresher->CNATBindingRefresher::HasSocket(*secureSocket));

    CleanupStack::PopAndDestroy(secureSocket);
    CleanupStack::PopAndDestroy(&tcpSocket);
    }


void UT_CNATBindingSTUNRefresher::TestSetRefreshL()
    {
    EUNIT_ASSERT(!iRefresher->IsRefreshed());
    iRefresher->SetRefresh(ETrue);
    EUNIT_ASSERT(iRefresher->IsRefreshed());
    iRefresher->SetRefresh(ETrue);
    EUNIT_ASSERT(iRefresher->IsRefreshed());
    iRefresher->SetRefresh(EFalse);
    EUNIT_ASSERT(!iRefresher->IsRefreshed());
    iRefresher->SetRefresh(EFalse);
    EUNIT_ASSERT(!iRefresher->IsRefreshed());
    }


void UT_CNATBindingSTUNRefresher::TestContinueRefreshingL()
    {
    // Not refreshed
    EUNIT_ASSERT(!iRefresher->IsRefreshed());
    iRefresher->ContinueRefreshing();

    // Refreshed
    iRefresher->SetRefresh(ETrue);
    EUNIT_ASSERT(iRefresher->IsRefreshed());
    iRefresher->ContinueRefreshing();
    }


void UT_CNATBindingSTUNRefresher::TestRefreshTimerExpiredL()
    {
    CNATBindingRefresher::RefreshTimerExpired(iRefresher);
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATBindingSTUNRefresher,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestRefreshIntervalL - test ",
    "UT_CNATBindingSTUNRefresher",
    "TestRefreshIntervalL",
    "FUNCTIONALITY",
    SetupL, TestRefreshIntervalL, Teardown)

EUNIT_TEST(
    "TestHasSocketL - test ",
    "UT_CNATBindingSTUNRefresher",
    "TestHasSocketL",
    "FUNCTIONALITY",
    SetupL, TestHasSocketL, Teardown)

EUNIT_TEST(
    "TestSetRefreshL - test ",
    "UT_CNATBindingSTUNRefresher",
    "TestSetRefreshL",
    "FUNCTIONALITY",
    SetupL, TestSetRefreshL, Teardown)

EUNIT_TEST(
    "TestContinueRefreshingL - test ",
    "UT_CNATBindingSTUNRefresher",
    "TestContinueRefreshingL",
    "FUNCTIONALITY",
    SetupL, TestContinueRefreshingL, Teardown)

EUNIT_TEST(
    "TestRefreshTimerExpiredL - test ",
    "UT_CNATBindingSTUNRefresher",
    "TestRefreshTimerExpiredL",
    "FUNCTIONALITY",
    SetupL, TestRefreshTimerExpiredL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
