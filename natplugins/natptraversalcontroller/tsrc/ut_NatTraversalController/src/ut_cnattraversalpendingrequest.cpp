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
#include "ut_cnattraversalpendingrequest.h"

//  INTERNAL INCLUDES
#include "TNATTraversalPendingRequest.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


// CONSTRUCTION
UT_CNATTraversalPendingRequest* UT_CNATTraversalPendingRequest::NewL()
    {
    UT_CNATTraversalPendingRequest* self = UT_CNATTraversalPendingRequest::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATTraversalPendingRequest* UT_CNATTraversalPendingRequest::NewLC()
    {
    UT_CNATTraversalPendingRequest* self = new( ELeave ) UT_CNATTraversalPendingRequest();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATTraversalPendingRequest::~UT_CNATTraversalPendingRequest()
    {
    }

// Default constructor
UT_CNATTraversalPendingRequest::UT_CNATTraversalPendingRequest()
    {
    }

// Second phase construct
void UT_CNATTraversalPendingRequest::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_CNATTraversalPendingRequest::SetupL()
    {
    iRequestCompletedCalled = EFalse;
    iLastRequestId = 0;
    }


void UT_CNATTraversalPendingRequest::Teardown()
    {
    }


// From MNATTraversalPendingRequestContainer:
void UT_CNATTraversalPendingRequest::RequestCompleted(TUint32 aRequestId)
    {
    iRequestCompletedCalled = ETrue;
    iLastRequestId = aRequestId;
    }


// From MSIPNATTraversalRequestObserver
void UT_CNATTraversalPendingRequest::RequestComplete(
    TUint32 aRequestId,
    TInt /*aCompletionCode*/)
    {
    iLastRequestId = aRequestId;
    }

// From MSIPNATTraversalRequestObserver
void UT_CNATTraversalPendingRequest::PublicAddrResolved(
    TUint32 /*aRequestId*/,
    const TInetAddr& /*aPublicAddr*/)
    {
    }


void UT_CNATTraversalPendingRequest::TestL()
    {
    TUint32 requestId1 = 1;

    TNATTraversalPendingRequest request1(requestId1,this,*this);
    EUNIT_ASSERT(requestId1 == request1.RequestId());

    request1.CompleteRequest();
    EUNIT_ASSERT(iLastRequestId == request1.RequestId());

    iLastRequestId = 0;
    TNATTraversalPendingRequest::TimerExpired(&request1);
    EUNIT_ASSERT(iLastRequestId == request1.RequestId());
    EUNIT_ASSERT(iRequestCompletedCalled);

    TUint32 requestId2 = 2;
    TNATTraversalPendingRequest request2(requestId2,NULL,*this);
    // observer NULL
    iLastRequestId = 0;
    request2.CompleteRequest();
    EUNIT_ASSERT(0 == iLastRequestId);
    // compare
    EUNIT_ASSERT(-1 ==
        TNATTraversalPendingRequest::RequestOrder(request1,request2));
    EUNIT_ASSERT(1 ==
        TNATTraversalPendingRequest::RequestOrder(request2,request1));
    EUNIT_ASSERT(0 ==
        TNATTraversalPendingRequest::RequestOrder(request1,request1));
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATTraversalPendingRequest,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestL - test ",
    "UT_CNATTraversalPendingRequest",
    "TestL",
    "FUNCTIONALITY",
    SetupL, TestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
