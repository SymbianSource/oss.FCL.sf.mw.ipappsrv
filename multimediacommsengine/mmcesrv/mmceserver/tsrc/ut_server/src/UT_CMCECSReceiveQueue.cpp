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
#include "ut_cmcecsreceivequeue.h"
#include "MCETestHelper.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcecsreceivequeue.h"

// CONSTRUCTION
UT_CMceCsReceiveQueue* UT_CMceCsReceiveQueue::NewL()
    {
    UT_CMceCsReceiveQueue* self = UT_CMceCsReceiveQueue::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceCsReceiveQueue* UT_CMceCsReceiveQueue::NewLC()
    {
    UT_CMceCsReceiveQueue* self = new( ELeave ) UT_CMceCsReceiveQueue();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceCsReceiveQueue::~UT_CMceCsReceiveQueue()
    {
    }

// Default constructor
UT_CMceCsReceiveQueue::UT_CMceCsReceiveQueue()
    {
    }

// Second phase construct
void UT_CMceCsReceiveQueue::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceCsReceiveQueue::SetupL(  )
    {
    iReceiveQueue = new (ELeave) CMceCsReceiveQueue;

    } 

void UT_CMceCsReceiveQueue::Teardown(  )
    {
    delete iReceiveQueue;

    }


void UT_CMceCsReceiveQueue::UT_CMceCsReceiveQueue_IsEmptyL(  )
    {
    TMceIds ids;

    // Test empty
    EUNIT_ASSERT (iReceiveQueue->IsEmpty());
	EUNIT_ASSERT_EQUALS (0, iReceiveQueue->iReceiveItems.Count());

    // Add first item
    HBufC8* headers = HBufC8::NewL(1);
    CleanupStack::PushL (headers);
    HBufC8* content = HBufC8::NewLC(1);
    TMceCSReceiveItem item1(ids,headers,content,ETrue);
	User::LeaveIfError(iReceiveQueue->AddLast(item1));
    CleanupStack::Pop (2); // content, headers 
    EUNIT_ASSERT (!iReceiveQueue->IsEmpty());
    EUNIT_ASSERT_EQUALS (1, iReceiveQueue->iReceiveItems.Count());

    // Add second item
    TMceCSReceiveItem item2(ids,KErrGeneral);
    User::LeaveIfError(iReceiveQueue->AddLast(item2));
    EUNIT_ASSERT (!iReceiveQueue->IsEmpty());
	EUNIT_ASSERT_EQUALS (2, iReceiveQueue->iReceiveItems.Count());

    // Test remove first item
    iReceiveQueue->RemoveFirst();
    EUNIT_ASSERT (!iReceiveQueue->IsEmpty());
	EUNIT_ASSERT_EQUALS (1, iReceiveQueue->iReceiveItems.Count());

    // Test remove second item
    iReceiveQueue->RemoveFirst();
    EUNIT_ASSERT (iReceiveQueue->IsEmpty());
    EUNIT_ASSERT_EQUALS (0, iReceiveQueue->iReceiveItems.Count());
    }

void UT_CMceCsReceiveQueue::UT_CMceCsReceiveQueue_Add_RemoveL(  )
    {
    TMceIds ids;

    EUNIT_ASSERT (iReceiveQueue->IsEmpty());
    iReceiveQueue->RemoveFirst();
    EUNIT_ASSERT (iReceiveQueue->IsEmpty());

    // Add first item
	ids.iSessionID = 1;
    HBufC8* headers = HBufC8::NewL(1);
    CleanupStack::PushL (headers);
    HBufC8* content = HBufC8::NewLC(1);
    TMceCSReceiveItem item1(ids,headers,content);
	User::LeaveIfError(iReceiveQueue->AddLast(item1));
    CleanupStack::Pop (2); // content, headers
    EUNIT_ASSERT (iReceiveQueue->First().Ids().iSessionID ==  1);
    EUNIT_ASSERT (iReceiveQueue->First().Context() == headers);
    EUNIT_ASSERT (iReceiveQueue->First().Content() == content);
    EUNIT_ASSERT (iReceiveQueue->First().Error() == KErrNone);

    // Add second item to the end of the queue.
    // Test that first added is still first in queue
	ids.iSessionID = 2;
    TMceCSReceiveItem item2(ids,KErrGeneral);
    User::LeaveIfError(iReceiveQueue->AddLast(item2));
    EUNIT_ASSERT (iReceiveQueue->First().Ids().iSessionID == 1);
    EUNIT_ASSERT (iReceiveQueue->First().Context() == headers);
    EUNIT_ASSERT (iReceiveQueue->First().Content() == content);
    EUNIT_ASSERT (iReceiveQueue->First().Error() == KErrNone);

    // Remove first. Last added should be first now.
    iReceiveQueue->RemoveFirst();
    EUNIT_ASSERT (iReceiveQueue->First().Ids().iSessionID == 2);
    EUNIT_ASSERT (iReceiveQueue->First().Context() == 0);
    EUNIT_ASSERT (iReceiveQueue->First().Content() == 0);
    EUNIT_ASSERT (iReceiveQueue->First().Error() == KErrGeneral);

    // Test that the queue is empty after removing the only item:
    iReceiveQueue->RemoveFirst();
    EUNIT_ASSERT (iReceiveQueue->IsEmpty());
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceCsReceiveQueue,
    "UT_CMceCsReceiveQueue",
    "UNIT" )

EUNIT_TEST(
    "IsEmpty - test ",
    "CMceCsReceiveQueue",
    "IsEmpty",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsReceiveQueue_IsEmptyL, Teardown)

EUNIT_TEST(
    "First - test ",
    "CMceCsReceiveQueue",
    "First",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsReceiveQueue_Add_RemoveL, Teardown)



EUNIT_END_TEST_TABLE

//  END OF FILE
