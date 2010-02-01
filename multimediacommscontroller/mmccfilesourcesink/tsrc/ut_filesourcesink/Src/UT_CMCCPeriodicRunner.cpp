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
#include "UT_CMccPeriodicRunner.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <e32std.h>


//  INTERNAL INCLUDES
#include "MccPeriodicRunner.h"

// CONSTRUCTION
UT_CMccPeriodicRunner* UT_CMccPeriodicRunner::NewL()
    {
    UT_CMccPeriodicRunner* self = UT_CMccPeriodicRunner::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccPeriodicRunner* UT_CMccPeriodicRunner::NewLC()
    {
    UT_CMccPeriodicRunner* self = new( ELeave ) UT_CMccPeriodicRunner();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccPeriodicRunner::~UT_CMccPeriodicRunner()
    {
    }

// Default constructor
UT_CMccPeriodicRunner::UT_CMccPeriodicRunner()
    {
    }

// Second phase construct
void UT_CMccPeriodicRunner::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
TInt UT_CMccPeriodicRunner::RequestCompletedL( TAny* /*aObject*/ )
	{
    CActiveScheduler::Stop();
    return KErrNone;	
	}
    
void UT_CMccPeriodicRunner::SetupL(  )
    {
	iTimerRunner = CMccPeriodicRunner::NewL( );
    }

void UT_CMccPeriodicRunner::Teardown(  )
    {
	delete iTimerRunner;
    }

void UT_CMccPeriodicRunner::UT_CMccPeriodicRunner_NewLL(  )
    {
    CMccPeriodicRunner* perRunner = CMccPeriodicRunner::NewL( );
    delete perRunner;
    }

void UT_CMccPeriodicRunner::UT_CMccPeriodicRunner_RunL()
	{
	TInt timeInterval = 10000;
	iTimerRunner->Start( timeInterval, TCallBack( RequestCompletedL, this ) );	
	CActiveScheduler::Start();
	}

void UT_CMccPeriodicRunner::UT_CMccPeriodicRunner_CancelL()
	{
	TInt timeInterval = 10000;
	iTimerRunner->Start( timeInterval, TCallBack( RequestCompletedL, this ) );	
    iTimerRunner->Stop();

    EUNIT_ASSERT( iTimerRunner->iStatus == KErrCancel || iTimerRunner->iStatus == KErrNone );
    EUNIT_ASSERT( !iTimerRunner->IsActive() );	
	}

void UT_CMccPeriodicRunner::UT_CMccPeriodicRunner_ErrorL()
	{
	TInt timeInterval = 10000;
	iTimerRunner->Start( timeInterval, TCallBack( RequestCompletedL, this ) );	
	CActiveScheduler::Start();
	}
    	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccPeriodicRunner,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMccPeriodicRunner",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMccPeriodicRunner_NewLL, Teardown)

EUNIT_TEST(
    "RunL - test ",
    "CMccPeriodicRunner",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMccPeriodicRunner_RunL, Teardown)

EUNIT_TEST(
    "CancelL - test ",
    "CMccPeriodicRunner",
    "CancelL",
    "FUNCTIONALITY",
    SetupL, UT_CMccPeriodicRunner_CancelL, Teardown)

EUNIT_TEST(
    "ErrorL - test ",
    "CMccPeriodicRunner",
    "ErrorL",
    "FUNCTIONALITY",
    SetupL, UT_CMccPeriodicRunner_ErrorL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
