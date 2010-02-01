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
#include "UT_CMccSubThreadShutdownTimer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mccsubthreadshutdowntimer.h"

// CONSTRUCTION
UT_CMccSubThreadShutdownTimer* UT_CMccSubThreadShutdownTimer::NewL()
    {
    UT_CMccSubThreadShutdownTimer* self = UT_CMccSubThreadShutdownTimer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccSubThreadShutdownTimer* UT_CMccSubThreadShutdownTimer::NewLC()
    {
    UT_CMccSubThreadShutdownTimer* self = new( ELeave ) UT_CMccSubThreadShutdownTimer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccSubThreadShutdownTimer::~UT_CMccSubThreadShutdownTimer()
    {
    }

// Default constructor
UT_CMccSubThreadShutdownTimer::UT_CMccSubThreadShutdownTimer() :
    iTimer( NULL )
    {
    }

// Second phase construct
void UT_CMccSubThreadShutdownTimer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccSubThreadShutdownTimer::SetupL(  )
    {
    iTimer = CMccSubThreadShutdownTimer::NewL();
    }

void UT_CMccSubThreadShutdownTimer::Teardown(  )
    {
    if ( iTimer )
        {
        delete iTimer;
        }
    }

void UT_CMccSubThreadShutdownTimer::UT_CMccSubThreadShutdownTimer_StartLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iTimer->StartL() );

    EUNIT_ASSERT_LEAVE( iTimer->StartL(); );

    iTimer->Cancel();
    }

void UT_CMccSubThreadShutdownTimer::UT_CMccSubThreadShutdownTimer_ShutdownNowL(  )
    {
    iTimer->ShutdownNow();
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccSubThreadShutdownTimer,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "StartL - test ",
    "CMccSubThreadShutdownTimer",
    "StartL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadShutdownTimer_StartLL, Teardown)

EUNIT_TEST(
    "ShutdownNow - test ",
    "CMccSubThreadShutdownTimer",
    "ShutdownNow",
    "FUNCTIONALITY",
    SetupL, UT_CMccSubThreadShutdownTimer_ShutdownNowL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
