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
#include "ut_cnatbindingstunandcrlfrefresher.h"

//  INTERNAL INCLUDES
#include "CNATBindingSTUNAndCRLFRefresher.h"
#include "CNATBinding.h"

//  EXTERNAL INCLUDES
#include "natfwstunclient.h"
#include "natfwstunbinding.h"

#include <CommDbConnPref.h>
#include <SecureSocket.h>

#include <digia/eunit/eunitmacros.h>

//  CONSTANTS

const TUint32 KIapId = 10;
const TInt KRefreshInterval = 10;

// CONSTRUCTION
UT_CNATBindingSTUNAndCRLFRefresher* UT_CNATBindingSTUNAndCRLFRefresher::NewL()
    {
    UT_CNATBindingSTUNAndCRLFRefresher* self = UT_CNATBindingSTUNAndCRLFRefresher::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATBindingSTUNAndCRLFRefresher* UT_CNATBindingSTUNAndCRLFRefresher::NewLC()
    {
    UT_CNATBindingSTUNAndCRLFRefresher* self = new( ELeave ) UT_CNATBindingSTUNAndCRLFRefresher();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATBindingSTUNAndCRLFRefresher::~UT_CNATBindingSTUNAndCRLFRefresher()
    {
    }

// Default constructor
UT_CNATBindingSTUNAndCRLFRefresher::UT_CNATBindingSTUNAndCRLFRefresher()
    {
    }

// Second phase construct
void UT_CNATBindingSTUNAndCRLFRefresher::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS


void UT_CNATBindingSTUNAndCRLFRefresher::SetupL()
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

    iSTUNClient = CSTUNClient::NewL(2000,_L8("STUNServer"),0,_L8("stun"),
                                    iSocketServ,iConnection,*iDeltaTimer,
                                    *this);

    iSTUNBinding = CSTUNBinding::NewL(*iSTUNClient,iSocket);


    TInetAddr addr;
    iNATBinding = CNATBinding::NewL(89,iSocket,addr,
        *(MSIPNATTraversalRequestObserver*)NULL,
        *(MSIPNATBindingObserver*)NULL,
        *this);
    // Ownership is transferred
    iNATBinding->SetBinding(iSTUNBinding);


    TInetAddr sender;
    sender.SetAddress(INET_ADDR(123,123,123,123));

    iRefresher = CNATBindingSTUNAndCRLFRefresher::NewL(
        *iDeltaTimer,KRefreshInterval,
        *this,*iNATBinding,
        iSocket,sender,*this);
    }


void UT_CNATBindingSTUNAndCRLFRefresher::Teardown()
    {
    delete iRefresher;

    //iNATBinding owns iSTUNBinding
    delete iNATBinding;

    delete iSTUNClient;
    iSocket.Close();
    iConnection.Close();
    iSocketServ.Close();
    delete iDeltaTimer;
    }


// From MNATBindingRefresherObserver
void UT_CNATBindingSTUNAndCRLFRefresher::NATBindingRefreshComplete(
    TInt aCompletionCode)
    {
    iLastCompletionCode = aCompletionCode;
    }


// From MSTUNClientObserver
void UT_CNATBindingSTUNAndCRLFRefresher::STUNClientInitCompleted(
    const CSTUNClient& /*aClient*/,
    TInt /*aCompletionCode*/)
    {
    }


// From MSTUNClientObserver
void UT_CNATBindingSTUNAndCRLFRefresher::STUNBindingEventOccurredL(
    TSTUNBindingEvent /*aEvent*/,
    const CBinding& /*aBinding*/)
    {
    HBufC8* tmp = HBufC8::NewL(100);
    delete tmp;
    }


// From MSTUNClientObserver
void UT_CNATBindingSTUNAndCRLFRefresher::STUNBindingErrorOccurred(
    const CBinding& /*aBinding*/,
    TInt /*aError*/)
    {
    }


// From MNATTraversalSocketManager
TBool UT_CNATBindingSTUNAndCRLFRefresher::AddToSendingQueueL(
    MNATTraversalSocketUser& /*aUser*/)
    {
    // Make this function leave in low memory conditions
    delete HBufC8::NewL(100);
    return iSendingNotAllowed;
    }


// From MNATTraversalSocketManager
void UT_CNATBindingSTUNAndCRLFRefresher::SendingCompleted(
    MNATTraversalSocketUser& /*aUser*/)
    {
    }


// Test functions

void UT_CNATBindingSTUNAndCRLFRefresher::TestNATBindingRefreshCompleteL()
    {
    // KErrNone, STUN binding exists
    iRefresher->NATBindingRefreshComplete(KErrNone);

    // KErrNone, no STUN binding
    iNATBinding->SetBinding(NULL);
    iRefresher->NATBindingRefreshComplete(KErrNone);
    EUNIT_ASSERT(KErrNotFound == iLastCompletionCode);

    // Error
    iRefresher->NATBindingRefreshComplete(KErrGeneral);
    EUNIT_ASSERT(KErrGeneral == iLastCompletionCode);
    }


void UT_CNATBindingSTUNAndCRLFRefresher::TestRefreshTimerExpiredL()
    {
    CNATBindingRefresher::RefreshTimerExpired(iRefresher);

    // Test UDP sender through this function
    iSendingNotAllowed = ETrue;
    CNATBindingRefresher::RefreshTimerExpired(iRefresher);
    }


void UT_CNATBindingSTUNAndCRLFRefresher::TestCancelRefreshL()
    {
    iRefresher->SetRefresh(ETrue);
    iRefresher->SetRefresh(EFalse);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATBindingSTUNAndCRLFRefresher,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestNATBindingRefreshCompleteL - test ",
    "UT_CNATBindingSTUNAndCRLFRefresher",
    "TestNATBindingRefreshCompleteL",
    "FUNCTIONALITY",
    SetupL, TestNATBindingRefreshCompleteL, Teardown)

EUNIT_TEST(
    "TestRefreshTimerExpiredL - test ",
    "UT_CNATBindingSTUNAndCRLFRefresher",
    "TestRefreshTimerExpiredL",
    "FUNCTIONALITY",
    SetupL, TestRefreshTimerExpiredL, Teardown)

EUNIT_TEST(
    "TestCancelRefreshL - test ",
    "UT_CNATBindingSTUNAndCRLFRefresher",
    "TestCancelRefreshL",
    "FUNCTIONALITY",
    SetupL, TestCancelRefreshL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
