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
#include "UT_CMccTimerManager.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcctimermanager.h"

// CONSTANTS
const TInt KMaxSeconds( 30 * 60 );


UT_CMccTimerManager* UT_CMccTimerManager::NewL()
    {
    UT_CMccTimerManager* self = UT_CMccTimerManager::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccTimerManager* UT_CMccTimerManager::NewLC()
    {
    UT_CMccTimerManager* self = new( ELeave ) UT_CMccTimerManager();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }
    
UT_CMccTimerManager::UT_CMccTimerManager()
    {
    }

// Destructor (virtual by CBase)
UT_CMccTimerManager::~UT_CMccTimerManager()
    {
    }

// Second phase construct
void UT_CMccTimerManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CMccTimerManager::SetupL(  )
    {
    iTimer = CMccTimerManager::NewL();
    } 

void UT_CMccTimerManager::Teardown(  )
    {
    delete iTimer;
    }


// INTERFACE IMPLEMENATIONS   

void UT_CMccTimerManager::TimerExpiredL( TMccTimerId aTimerId, TAny* aTimerParam )
	{
	iLastExpiredTimerId = aTimerId;
	iLastTimerParam = aTimerParam;
	}


// TEST FUNCTIONS
    

void UT_CMccTimerManager::TestIsRunningL()
	{
	EUNIT_ASSERT_EQUALS(EFalse, iTimer->IsRunning(1));
	EUNIT_ASSERT_EQUALS(EFalse, iTimer->IsRunning(0x100));

	TMccTimerId id = iTimer->StartL(this, 10);
	EUNIT_ASSERT_EQUALS(ETrue, iTimer->IsRunning(id));
	EUNIT_ASSERT_EQUALS(EFalse, iTimer->IsRunning(id + 1));
	EUNIT_ASSERT_EQUALS(EFalse, iTimer->IsRunning(id - 1));
	}
	
void UT_CMccTimerManager::TestIsRunning2L()
	{
	TMccTimerId id = iTimer->StartL(this, 1000);
	EUNIT_ASSERT_EQUALS(ETrue, iTimer->IsRunning(id));

	iTimer->Stop(id);
	EUNIT_ASSERT_EQUALS(EFalse, iTimer->IsRunning(id));
	}

void UT_CMccTimerManager::TestIsRunning3L()
	{
	EUNIT_ASSERT_EQUALS(EFalse, iTimer->IsRunning(0));
	}

void UT_CMccTimerManager::TestStartL()
	{	
	MMccExpirationHandler* callback = (MMccExpirationHandler*)0xce;
	TUint duration = 2288;
	TMccTimerId id = iTimer->StartL(callback, duration);


	EUNIT_ASSERT_EQUALS(1, iTimer->iEntries.Count());
	EUNIT_ASSERT_EQUALS(id, iTimer->iEntries[0].iId);
	EUNIT_ASSERT_EQUALS(callback, iTimer->iEntries[0].iObserver);
	}

void UT_CMccTimerManager::TestStart2L()
	{	 
	MMccExpirationHandler* callback = (MMccExpirationHandler*)0x88;
	TUint duration = 0x500;
	TUint32 param = 55;
	TMccTimerId id = iTimer->StartL(callback, duration, (TAny*)(param));

    EUNIT_ASSERT_EQUALS(1, iTimer->iEntries.Count());
	EUNIT_ASSERT_EQUALS(id, iTimer->iEntries[0].iId);
	EUNIT_ASSERT_EQUALS(callback, iTimer->iEntries[0].iObserver);
	EUNIT_ASSERT_EQUALS(param, (TUint32)iTimer->iEntries[0].iObserverParam);
	}

void UT_CMccTimerManager::TestStart3L()
	{	 
	MMccExpirationHandler* callback = (MMccExpirationHandler*)0x88;
	iTimer->StartL(callback, 1000);
	iTimer->StartL(callback, 0);
	iTimer->StartL(callback, 500);
	iTimer->StartL(callback, 500); //note: same duration as previous timer
	iTimer->StartL(callback, 2500);

	EUNIT_ASSERT_EQUALS(5, iTimer->iEntries.Count());	
	}

//Set several very long timers
void UT_CMccTimerManager::TestStart4L()
	{	 
	MMccExpirationHandler* callback = (MMccExpirationHandler*)0x88;
	iTimer->StartL(callback, KMaxSeconds + 5000);
	iTimer->StartL(callback, KMaxSeconds + 9000);
	iTimer->StartL(callback, KMaxSeconds + 1000);
	iTimer->StartL(callback, KMaxSeconds + 2000);
	iTimer->StartL(callback, KMaxSeconds + 100);
	iTimer->StartL(callback, KMaxSeconds + 100);

	EUNIT_ASSERT_EQUALS(6, iTimer->iEntries.Count());	
	}

void UT_CMccTimerManager::TestStart5L()
	{	 
	MMccExpirationHandler* callback = (MMccExpirationHandler*)0x88;

	TUint duration = 0;
	TMccTimerId id;
	//with n=1000, the loop takes ~45 secs without LDRA
	TInt n = 50;
	for (TInt i = 0; i < n; i++)
		{
		duration = (i%5 * 100) + (i % 2) * 10;
		id = iTimer->StartL(callback, duration);
	
		if (i % 5 == 4)
			iTimer->Stop(id - i % 3);
		}	
	}

//Start timer which is longer than KMaxTimerAfterLength
void UT_CMccTimerManager::TestStart6L()
	{
	MMccExpirationHandler* callback = (MMccExpirationHandler*)0xbd;
	iTimer->StartL(callback, KMaxSeconds + 100);
	}

//Start two timers, a very long and a short one, and then stop the short timer
void UT_CMccTimerManager::TestStart7L()
	{
	MMccExpirationHandler* callback = (MMccExpirationHandler*)0xbd;
	
	TMccTimerId id = iTimer->StartL(callback, 200);
	iTimer->StartL(callback, KMaxSeconds + 1000);
	
	iTimer->Stop(id);
	}

//Try to give observer as NULL
void UT_CMccTimerManager::TestStart8L()
	{		
	TRAPD(err, iTimer->StartL(NULL, 10));
        
	EUNIT_ASSERT_EQUALS(err, KErrArgument);
    }

//Try to give observer as NULL
void UT_CMccTimerManager::TestStart9L()
	{		
	TRAPD(err, iTimer->StartL(NULL, 10, NULL /*timerParam*/));
        
	EUNIT_ASSERT_EQUALS(err, KErrArgument);
    }

void UT_CMccTimerManager::TestStopL()
	{
	EUNIT_ASSERT_EQUALS(KErrNotFound, iTimer->Stop(77));
	}

void UT_CMccTimerManager::TestStop2L()
	{
	MMccExpirationHandler* callback = (MMccExpirationHandler*)0x88;	
	TMccTimerId id = iTimer->StartL(callback, 0);

	EUNIT_ASSERT_EQUALS(KErrNone, iTimer->Stop(id));
	EUNIT_ASSERT_EQUALS(0, iTimer->iEntries.Count());
	}


void UT_CMccTimerManager::TestStop3L()
	{
	EUNIT_ASSERT_EQUALS(KErrNotFound, iTimer->Stop(0));
	}

void UT_CMccTimerManager::TestNewTimerIdL()
	{
	TMccTimerId id = 760;	
	iTimer->iTimerIdCounter = id;

	EUNIT_ASSERT_EQUALS(id + 1, iTimer->NewTimerId());
	EUNIT_ASSERT_EQUALS(id + 2, iTimer->NewTimerId());
	}

void UT_CMccTimerManager::TestTimerExpiredL()
	{	
	TMccTimerId id = iTimer->StartL(this, 10);

	iTimer->Cancel();

	iTimer->RunL();
	
	EUNIT_ASSERT( iLastExpiredTimerId == id );
	}

void UT_CMccTimerManager::TestTimerExpired2L()
	{	
	TMccTimerId id = iTimer->StartL(this, 100);
	iTimer->Stop(id);

	//Must cancel timer, otherwise crash occurs in
	//CTimerManager::SetTimerWithShortestValueL
	iTimer->Cancel();

	iTimer->RunL();
	}

void UT_CMccTimerManager::TestTimerExpired3L()
	{
	//Set two very long timers (over CSingleTimer::KMaxTimerAfterDuration
	//milliseconds) then make the shorter of them "expire" by calling
	//TimerExpiredL
	iTimer->StartL(this, KMaxSeconds + 2000);
	TMccTimerId id = iTimer->StartL(this, KMaxSeconds + 500);

	iTimer->Cancel();

	iTimer->RunL();
	}
    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccTimerManager,
    "MccTimerManager test",
    "UNIT" )

EUNIT_TEST(
    "IsRunning - test ",
    "CMccTimerManager",
    "IsRunning",
    "FUNCTIONALITY",
    SetupL, TestIsRunningL, Teardown )

EUNIT_TEST(
    "IsRunning - test ",
    "CMccTimerManager",
    "IsRunning",
    "FUNCTIONALITY",
    SetupL, TestIsRunning2L, Teardown )
    
EUNIT_TEST(
    "StartL - test ",
    "CMccTimerManager",
    "StartL",
    "FUNCTIONALITY",
    SetupL, TestStartL, Teardown )

EUNIT_TEST(
    "StartL 2 - test ",
    "CMccTimerManager",
    "StartL 2",
    "FUNCTIONALITY",
    SetupL, TestStart2L, Teardown )
    
EUNIT_TEST(
    "StartL 3 - test ",
    "CMccTimerManager",
    "StartL 3",
    "FUNCTIONALITY",
    SetupL, TestStart3L, Teardown )
    
EUNIT_TEST(
    "StartL 4 - test ",
    "CMccTimerManager",
    "StartL 4",
    "FUNCTIONALITY",
    SetupL, TestStart4L, Teardown )
    
EUNIT_TEST(
    "StartL 5 - test ",
    "CMccTimerManager",
    "StartL 5",
    "FUNCTIONALITY",
    SetupL, TestStart5L, Teardown )
    
EUNIT_TEST(
    "StartL 6 - test ",
    "CMccTimerManager",
    "StartL 6",
    "FUNCTIONALITY",
    SetupL, TestStart6L, Teardown )
    
EUNIT_TEST(
    "StartL 7 - test ",
    "CMccTimerManager",
    "StartL 7",
    "FUNCTIONALITY",
    SetupL, TestStart7L, Teardown )
    
EUNIT_TEST(
    "StartL 8 - test ",
    "CMccTimerManager",
    "StartL 8",
    "FUNCTIONALITY",
    SetupL, TestStart8L, Teardown )

EUNIT_TEST(
    "StartL 9 - test ",
    "CMccTimerManager",
    "StartL 9",
    "FUNCTIONALITY",
    SetupL, TestStart9L, Teardown )

EUNIT_TEST(
    "Stop - test ",
    "CMccTimerManager",
    "Stop",
    "FUNCTIONALITY",
    SetupL, TestStopL, Teardown )

EUNIT_TEST(
    "Stop 2 - test ",
    "CMccTimerManager",
    "Stop 2",
    "FUNCTIONALITY",
    SetupL, TestStop2L, Teardown )
    
EUNIT_TEST(
    "Stop 3 - test ",
    "CMccTimerManager",
    "Stop 43",
    "FUNCTIONALITY",
    SetupL, TestStop3L, Teardown )

EUNIT_TEST(
    "NewTimerIdL - test ",
    "CMccTimerManager",
    "NewTimerIdL",
    "FUNCTIONALITY",
    SetupL, TestNewTimerIdL, Teardown )    

EUNIT_TEST(
    "TimerExpiredL - test ",
    "CMccTimerManager",
    "TimerExpiredL",
    "FUNCTIONALITY",
    SetupL, TestTimerExpiredL, Teardown )

EUNIT_TEST(
    "TimerExpiredL 2 - test ",
    "CMccTimerManager",
    "TimerExpiredL 2",
    "FUNCTIONALITY",
    SetupL, TestTimerExpired2L, Teardown )  
    
EUNIT_TEST(
    "TimerExpiredL 3 - test ",
    "CMccTimerManager",
    "TimerExpiredL 3",
    "FUNCTIONALITY",
    SetupL, TestTimerExpired3L, Teardown )
            
EUNIT_END_TEST_TABLE

//  END OF FILE
