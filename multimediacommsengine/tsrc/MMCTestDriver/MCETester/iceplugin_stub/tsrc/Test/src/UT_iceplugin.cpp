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
#include "UT_iceplugin.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES


// CONSTRUCTION
UT_iceplugin* UT_iceplugin::NewL()
    {
    UT_iceplugin* self = UT_iceplugin::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_iceplugin* UT_iceplugin::NewLC()
    {
    UT_iceplugin* self = new( ELeave ) UT_iceplugin();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_iceplugin::~UT_iceplugin()
    {
    }

// Default constructor
UT_iceplugin::UT_iceplugin()
    {
    }

// Second phase construct
void UT_iceplugin::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_iceplugin::SetupL(  )
    {

    }

void UT_iceplugin::Teardown(  )
    {

    }

void UT_iceplugin::TestL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_iceplugin,
    "Add test suite description here.",
    "MODULE" )

EUNIT_TEST(
    "Empty test skeleton",
    "Add tested class name here",
    "Add tested function name here",
    "FUNCTIONALITY",
    SetupL, TestL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
