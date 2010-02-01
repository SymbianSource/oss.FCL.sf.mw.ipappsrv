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
#include "UT_MNATFWSocketReceiverObserver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwsocketreceiverobserver.h"

// CONSTRUCTION
UT_MNATFWSocketReceiverObserver* UT_MNATFWSocketReceiverObserver::NewL()
    {
    UT_MNATFWSocketReceiverObserver* self = UT_MNATFWSocketReceiverObserver::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_MNATFWSocketReceiverObserver* UT_MNATFWSocketReceiverObserver::NewLC()
    {
    UT_MNATFWSocketReceiverObserver* self = new( ELeave ) UT_MNATFWSocketReceiverObserver();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_MNATFWSocketReceiverObserver::~UT_MNATFWSocketReceiverObserver()
    {
    }

// Default constructor
UT_MNATFWSocketReceiverObserver::UT_MNATFWSocketReceiverObserver()
    {
    }

// Second phase construct
void UT_MNATFWSocketReceiverObserver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_MNATFWSocketReceiverObserver::SetupL(  )
    {

    }

void UT_MNATFWSocketReceiverObserver::Teardown(  )
    {

    }

void UT_MNATFWSocketReceiverObserver::UT_MNATFWSocketReceiverObserver_DeliverBufferL(  )
    {
    EUNIT_ASSERT( ETrue );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_MNATFWSocketReceiverObserver,
    "MNATFWSocketReceiverObserver",
    "UNIT" )

EUNIT_TEST(
    "DeliverBuffer - test ",
    "MNATFWSocketReceiverObserver",
    "DeliverBuffer",
    "FUNCTIONALITY",
    SetupL, UT_MNATFWSocketReceiverObserver_DeliverBufferL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
