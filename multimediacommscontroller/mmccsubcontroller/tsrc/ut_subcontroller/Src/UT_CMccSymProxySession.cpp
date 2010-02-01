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
#include "UT_CMccSymProxySession.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "Mccsymproxysession.h"

// CONSTRUCTION
UT_CMccSymProxySession* UT_CMccSymProxySession::NewL()
    {
    UT_CMccSymProxySession* self = UT_CMccSymProxySession::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccSymProxySession* UT_CMccSymProxySession::NewLC()
    {
    UT_CMccSymProxySession* self = new( ELeave ) UT_CMccSymProxySession();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccSymProxySession::~UT_CMccSymProxySession()
    {
    }

// Default constructor
UT_CMccSymProxySession::UT_CMccSymProxySession()
    {
    }

// Second phase construct
void UT_CMccSymProxySession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccSymProxySession::SetupL(  )
    {
    //iSession = CMccSymProxySession::NewL( *server );
    }

void UT_CMccSymProxySession::Teardown(  )
    {
    if ( iSession )
        {
        delete iSession;
        }
    }

void UT_CMccSymProxySession::UT_CMccSymProxySession_ServiceLL(  )
    {
    //EUNIT_ASSERT( ETrue );
    }

void UT_CMccSymProxySession::UT_CMccSymProxySession_ServiceErrorL(  )
    {
    //EUNIT_ASSERT( ETrue );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccSymProxySession,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "ServiceL - test ",
    "CMccSymProxySession",
    "ServiceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymProxySession_ServiceLL, Teardown)

EUNIT_TEST(
    "ServiceError - test ",
    "CMccSymProxySession",
    "ServiceError",
    "FUNCTIONALITY",
    SetupL, UT_CMccSymProxySession_ServiceErrorL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
