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
#include "ut_cnatbindingcrlfrefresher.h"

//  INTERNAL INCLUDES
#include "CCRLFSenderStub.h"
#include "CNATBindingCRLFRefresher.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

// CONSTRUCTION
UT_CNATBindingCRLFRefresher* UT_CNATBindingCRLFRefresher::NewL()
    {
    UT_CNATBindingCRLFRefresher* self = UT_CNATBindingCRLFRefresher::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATBindingCRLFRefresher* UT_CNATBindingCRLFRefresher::NewLC()
    {
    UT_CNATBindingCRLFRefresher* self = new( ELeave ) UT_CNATBindingCRLFRefresher();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATBindingCRLFRefresher::~UT_CNATBindingCRLFRefresher()
    {
    }

// Default constructor
UT_CNATBindingCRLFRefresher::UT_CNATBindingCRLFRefresher()
    {
    }

// Second phase construct
void UT_CNATBindingCRLFRefresher::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS

void UT_CNATBindingCRLFRefresher::SetupL()
    {
    iDeltaTimer = CDeltaTimer::NewL(CActive::EPriorityStandard);

    iSender = new(ELeave)CCRLFSenderStub(*this);

    iRefresher = new(ELeave)CNATBindingCRLFRefresher(*iDeltaTimer,1,
                                                     *this,iSender);
    }


void UT_CNATBindingCRLFRefresher::Teardown()
    {
    delete iRefresher;
    delete iDeltaTimer;
    }


// From MNATBindingRefresherObserver
void UT_CNATBindingCRLFRefresher::NATBindingRefreshComplete(
    TInt /*aCompletionCode*/)
    {
    }


// Test functions

void UT_CNATBindingCRLFRefresher::TestSetRefreshL()
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


void UT_CNATBindingCRLFRefresher::TestRefreshTimerExpiredL()
    {
    CNATBindingRefresher::RefreshTimerExpired(iRefresher);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATBindingCRLFRefresher,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestSetRefreshL - test ",
    "UT_CNATBindingCRLFRefresher",
    "TestSetRefreshL",
    "FUNCTIONALITY",
    SetupL, TestSetRefreshL, Teardown)

EUNIT_TEST(
    "TestRefreshTimerExpiredL - test ",
    "UT_CNATBindingCRLFRefresher",
    "TestRefreshTimerExpiredL",
    "FUNCTIONALITY",
    SetupL, TestRefreshTimerExpiredL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
