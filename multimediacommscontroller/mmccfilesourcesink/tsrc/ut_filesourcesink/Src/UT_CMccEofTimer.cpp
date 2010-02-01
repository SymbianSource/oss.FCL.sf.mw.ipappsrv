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
#include "UT_CMccEofTimer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <e32std.h>


//  INTERNAL INCLUDES
#include "MccEofTimer.h"

// CONSTRUCTION
UT_CMccEofTimer* UT_CMccEofTimer::NewL()
    {
    UT_CMccEofTimer* self = UT_CMccEofTimer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccEofTimer* UT_CMccEofTimer::NewLC()
    {
    UT_CMccEofTimer* self = new( ELeave ) UT_CMccEofTimer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccEofTimer::~UT_CMccEofTimer()
    {
    }

// Default constructor
UT_CMccEofTimer::UT_CMccEofTimer()
    {
    }

// Second phase construct
void UT_CMccEofTimer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

    
void UT_CMccEofTimer::SetupL(  )
    {
    iEofTimerExpiredCalled = EFalse;
	iTimer = CMccEofTimer::NewL( this );
    }

void UT_CMccEofTimer::Teardown(  )
    {
	delete iTimer;
    }

void UT_CMccEofTimer::UT_CMccEofTimer_NewLL(  )
    {
    CMccEofTimer* myDummyTimer = CMccEofTimer::NewL( this );
    delete myDummyTimer;
    }

void UT_CMccEofTimer::UT_CMccEofTimer_RunLL()
	{
	EUNIT_ASSERT_EQUALS(iEofTimerExpiredCalled, EFalse);
	iTimer->RunL();
	EUNIT_ASSERT_EQUALS(iEofTimerExpiredCalled, ETrue);
	}

void UT_CMccEofTimer::EofTimerExpiredL()
	{
	iEofTimerExpiredCalled = ETrue;
	}

    	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccEofTimer,
    "UT_CMccEofTimer",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccEofTimer",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccEofTimer_NewLL, Teardown)

EUNIT_TEST(
    "RunL - test ",
    "CMccEofTimer",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMccEofTimer_RunLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
