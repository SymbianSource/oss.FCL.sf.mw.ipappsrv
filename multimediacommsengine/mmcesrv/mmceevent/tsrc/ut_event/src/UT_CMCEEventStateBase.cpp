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
#include "ut_cmceeventstatebase.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mceeventstatebase.h"

// CONSTRUCTION
UT_CMceEventStateBase* UT_CMceEventStateBase::NewL()
    {
    UT_CMceEventStateBase* self = UT_CMceEventStateBase::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceEventStateBase* UT_CMceEventStateBase::NewLC()
    {
    UT_CMceEventStateBase* self = new( ELeave ) UT_CMceEventStateBase();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceEventStateBase::~UT_CMceEventStateBase()
    {
    }

// Default constructor
UT_CMceEventStateBase::UT_CMceEventStateBase()
    {
    }

// Second phase construct
void UT_CMceEventStateBase::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceEventStateBase::SetupL(  )
    {

    } 

void UT_CMceEventStateBase::Teardown(  )
    {

    }

void UT_CMceEventStateBase::UT_CMceEventStateBase_ProcessStateLL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

void UT_CMceEventStateBase::UT_CMceEventStateBase_ProcessReqRecvStateLL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

void UT_CMceEventStateBase::UT_CMceEventStateBase_ProcessRespRecvStateLL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

void UT_CMceEventStateBase::UT_CMceEventStateBase_EnterStateL(  )
    {
    EUNIT_ASSERT( EFalse );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceEventStateBase,
    "UT_CMceEventStateBase",
    "UNIT" )

// Comment out empty cases
/*EUNIT_TEST(
    "ProcessStateL - test ",
    "CMceEventStateBase",
    "ProcessStateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventStateBase_ProcessStateLL, Teardown)

EUNIT_TEST(
    "ProcessReqRecvStateL - test ",
    "CMceEventStateBase",
    "ProcessReqRecvStateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventStateBase_ProcessReqRecvStateLL, Teardown)

EUNIT_TEST(
    "ProcessRespRecvStateL - test ",
    "CMceEventStateBase",
    "ProcessRespRecvStateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventStateBase_ProcessRespRecvStateLL, Teardown)

EUNIT_TEST(
    "EnterState - test ",
    "CMceEventStateBase",
    "EnterState",
    "FUNCTIONALITY",
    SetupL, UT_CMceEventStateBase_EnterStateL, Teardown)
*/

EUNIT_END_TEST_TABLE

//  END OF FILE
