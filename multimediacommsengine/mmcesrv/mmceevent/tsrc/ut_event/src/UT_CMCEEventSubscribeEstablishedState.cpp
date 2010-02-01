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
#include "UT_CMceEventSubscribeEstablishedState.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mceeventsubscribemtestablishedstate.h"

// CONSTRUCTION
UT_CMceEventSubscribeEstablishedState* UT_CMceEventSubscribeEstablishedState::NewL()
    {
    UT_CMceEventSubscribeEstablishedState* self = UT_CMceEventSubscribeEstablishedState::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventSubscribeEstablishedState* UT_CMceEventSubscribeEstablishedState::NewLC()
    {
    UT_CMceEventSubscribeEstablishedState* self = new( ELeave ) UT_CMceEventSubscribeEstablishedState();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventSubscribeEstablishedState::~UT_CMceEventSubscribeEstablishedState()
    {
    }

// Default constructor
UT_CMceEventSubscribeEstablishedState::UT_CMceEventSubscribeEstablishedState()
    {
    }

// Second phase construct
void UT_CMceEventSubscribeEstablishedState::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventSubscribeEstablishedState::SetupL(  )
    {

    } 

void UT_CMceEventSubscribeEstablishedState::Teardown(  )
    {

    }

void UT_CMceEventSubscribeEstablishedState::UT_CMceEventSubscribeEstablishedState_NewLL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventSubscribeEstablishedState,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMceEventSubscribeEstablishedState",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventSubscribeEstablishedState_NewLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
