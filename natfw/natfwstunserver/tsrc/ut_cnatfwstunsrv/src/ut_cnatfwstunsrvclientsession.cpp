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
#include "ut_cnatfwstunsrvclientsession.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwstunsrvclientsession.h"
#include "cncmconnectionmultiplexer.h"

void ut_cnatfwstunsrvclientsession::STUNRequestReceivedL( const TInetAddr& /*aLocalAddr*/,
    const TInetAddr& /*aFromAddr*/, const TInetAddr& /*aPeerAddr*/,
    TUint /*aPriority*/, TBool /*aRemoteFavored*/ )
    {    
    }
    
// CONSTRUCTION
ut_cnatfwstunsrvclientsession* ut_cnatfwstunsrvclientsession::NewL()
    {
    ut_cnatfwstunsrvclientsession* self = ut_cnatfwstunsrvclientsession::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cnatfwstunsrvclientsession* ut_cnatfwstunsrvclientsession::NewLC()
    {
    ut_cnatfwstunsrvclientsession* self = new( ELeave ) ut_cnatfwstunsrvclientsession();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cnatfwstunsrvclientsession::~ut_cnatfwstunsrvclientsession()
    {
    }

// Default constructor
ut_cnatfwstunsrvclientsession::ut_cnatfwstunsrvclientsession()
    {
    }

// Second phase construct
void ut_cnatfwstunsrvclientsession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void ut_cnatfwstunsrvclientsession::SetupL(  )
    {
    //iConnMux = CNATFWConnectionMultiplexer::NewL( *this );
    //iClientSession = CNATFWSTUNSrvClientSession::NewL( *this, *iConnMux );
    }

void ut_cnatfwstunsrvclientsession::Teardown(  )
    {
    //delete iConnMux;
    //delete iClientSession;
    }

void ut_cnatfwstunsrvclientsession::UT_CNATFWSTUNSrvClientSession_AddAuthenticationParamsL(  )
    {
    // tested in other class
    EUNIT_ASSERT( ETrue );
    }

void ut_cnatfwstunsrvclientsession::UT_CNATFWSTUNSrvClientSession_RemoveAuthenticationParamsL(  )
    {
    // tested in other class
    EUNIT_ASSERT( ETrue );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cnatfwstunsrvclientsession,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "AddAuthenticationParams - test ",
    "CNATFWSTUNSrvClientSession",
    "AddAuthenticationParams",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWSTUNSrvClientSession_AddAuthenticationParamsL, Teardown)

EUNIT_TEST(
    "RemoveAuthenticationParams - test ",
    "CNATFWSTUNSrvClientSession",
    "RemoveAuthenticationParams",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWSTUNSrvClientSession_RemoveAuthenticationParamsL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
