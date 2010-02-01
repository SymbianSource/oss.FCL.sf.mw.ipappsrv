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
#include "UT_CNATFWCandidate.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwcandidate.h"
#include "natfwconnectivityapidefs.h"
#include "natfwunittestmacros.h"

const TUint KSessionId = 5;
const TUint KStreamId = 10;
const TUint KStreamCollectionId = 2;
const TUint KPriority = 1;
const TUint KProtocol = KProtocolInetTcp;
const TUint32 KInetAddr = INET_ADDR( 1, 2, 3, 4 );
const TUint32 KTransportInetAddr = INET_ADDR( 1, 2, 3, 4 );
_LIT8( KFoundation, "Foundation" );
_LIT8( KFoundation2, "Foundation2" );
const CNATFWCandidate::TCandidateType KCandType = CNATFWCandidate::ERelay;
const TUint KTypePreference = 25;
const TUint KLocalPreference = 7;
const TUint KComponentId = 2;
 
// CONSTRUCTION
UT_CNATFWCandidate* UT_CNATFWCandidate::NewL()
    {
    UT_CNATFWCandidate* self = UT_CNATFWCandidate::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWCandidate* UT_CNATFWCandidate::NewLC()
    {
    UT_CNATFWCandidate* self = new( ELeave ) UT_CNATFWCandidate();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWCandidate::~UT_CNATFWCandidate()
    {
    }

// Default constructor
UT_CNATFWCandidate::UT_CNATFWCandidate()
    {
    }

// Second phase construct
void UT_CNATFWCandidate::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CNATFWCandidate::SetupL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCandidate = CNATFWCandidate::NewL() );
    }

void UT_CNATFWCandidate::Teardown(  )
    {
    delete iCandidate;
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_NewLL(  )
    {
    CNATFWCandidate* natCandi;
    CNATFWCandidate* natCandi2;
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( natCandi = CNATFWCandidate::NewL() );
    CleanupStack::PushL( natCandi );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( natCandi2 = CNATFWCandidate::NewL( *natCandi ) );
    
    CleanupStack::PopAndDestroy( natCandi );
    delete natCandi2;
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetSessionIdL(  )
    {
    iCandidate->SetSessionId( KSessionId );
    EUNIT_ASSERT_EQUALS( KSessionId, iCandidate->SessionId() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetStreamCollectionIdL(  )
    {
    iCandidate->SetStreamCollectionId( KStreamCollectionId );
    EUNIT_ASSERT_EQUALS( KStreamCollectionId, iCandidate->StreamCollectionId() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetStreamIdL(  )
    {
    iCandidate->SetStreamId( KStreamId );
    EUNIT_ASSERT_EQUALS( KStreamId, iCandidate->StreamId() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetPriorityL(  )
    {
    iCandidate->SetPriority( KPriority );
    EUNIT_ASSERT_EQUALS( KPriority, iCandidate->Priority() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetTransportAddrLL(  )
    {
    TInetAddr address( KInetAddr, 0 );

    NATFW_EUNIT_ASSERT_NO_LEAVE( iCandidate->SetTransportAddrL( address ) );
    EUNIT_ASSERT_EQUALS( KInetAddr, iCandidate->TransportAddr().Address() );
    }
    
void UT_CNATFWCandidate::UT_CNATFWCandidate_SetTransportDomainAddrLL(  )
    {
    _LIT8(KFqdn,"my.domain.org");
    _LIT8(KAddr,"195.36.45.3");
    const TUint KPort( 5000 );
    const TUint32 KIpAddr = INET_ADDR( 195, 36, 45, 3 );

    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCandidate->SetTransportDomainAddrL( KFqdn, KPort ) );
    EUNIT_ASSERT_EQUALS(
        0, iCandidate->TransportDomainAddr().Compare( KFqdn ) );
    EUNIT_ASSERT_EQUALS( KPort, iCandidate->TransportDomainPort() );

    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCandidate->SetTransportDomainAddrL( KAddr, KPort ) );
    EUNIT_ASSERT_EQUALS( KIpAddr, iCandidate->TransportAddr().Address() );
    EUNIT_ASSERT_EQUALS( KPort, iCandidate->TransportAddr().Port() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetTransportProtocolL(  )
    {
    iCandidate->SetTransportProtocol( KProtocol );
    EUNIT_ASSERT_EQUALS( KProtocol, iCandidate->TransportProtocol() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetBaseL(  )
    {
    TInetAddr address( KInetAddr, 0 );
    iCandidate->SetBase( address );
    EUNIT_ASSERT_EQUALS( KInetAddr, iCandidate->Base().Address() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetFoundationLL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCandidate->SetFoundationL( KFoundation ) );
    EUNIT_ASSERT_EQUALS( KFoundation(), iCandidate->Foundation() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetCandidateTypeL(  )
    {
    iCandidate->SetType( KCandType );
    EUNIT_ASSERT_EQUALS( KCandType, iCandidate->Type() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_SetComponentIdL(  )
    {
    iCandidate->SetComponentId( KComponentId );
    EUNIT_ASSERT_EQUALS( KComponentId, iCandidate->ComponentId() );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_CompareFoundationsL(  )
    {
    CNATFWCandidate* candidate1 = CNATFWCandidate::NewLC();
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewLC();
    NATFW_EUNIT_ASSERT_NO_LEAVE( candidate1->SetFoundationL( KFoundation ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( candidate2->SetFoundationL( KFoundation2 ) );
    
    EUNIT_ASSERT( !candidate1->CompareFoundations( *candidate1, *candidate2 ) );
    
    CleanupStack::PopAndDestroy( candidate2 );
    CleanupStack::PopAndDestroy( candidate1 );
    }

void UT_CNATFWCandidate::UT_CNATFWCandidate_PriorityOrderL(  )
    {
    CNATFWCandidate* candidate1 = CNATFWCandidate::NewLC();
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewLC();
    NATFW_EUNIT_ASSERT_NO_LEAVE( candidate1->SetPriority( 1 ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( candidate2->SetPriority( 2 ) );
    
    EUNIT_ASSERT( iCandidate->PriorityOrder( *candidate1, *candidate2 ) > 0 );
    
    CleanupStack::PopAndDestroy( candidate2 );
    CleanupStack::PopAndDestroy( candidate1 );
    }
    
void UT_CNATFWCandidate::UT_CNATFWCandidate_CopyLL()
    {
    CNATFWCandidate* candidate1;
    CNATFWCandidate* candidate2;
    
    TBuf8<2> foundation;
    foundation.Append( '1' );
    foundation.Append( '2' );
    
    candidate1 = CNATFWCandidate::NewLC();
    candidate2 = CNATFWCandidate::NewLC();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( candidate1->SetFoundationL( foundation ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( candidate1->CopyL( *candidate2 ) );
    EUNIT_ASSERT( *candidate1 == *candidate2 );
    
    CleanupStack::PopAndDestroy( candidate2 );
    CleanupStack::PopAndDestroy( candidate1 );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWCandidate,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CNATFWCandidate",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_NewLL, Teardown)

EUNIT_TEST(
    "SetSessionId - test ",
    "CNATFWCandidate",
    "SetSessionId",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetSessionIdL, Teardown)
    
EUNIT_TEST(
    "SetStreamCollectionId - test ",
    "CNATFWCandidate",
    "SetStreamCollectionId",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetStreamCollectionIdL, Teardown)

EUNIT_TEST(
    "SetStreamId - test ",
    "CNATFWCandidate",
    "SetStreamId",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetStreamIdL, Teardown)

EUNIT_TEST(
    "SetPriority - test ",
    "CNATFWCandidate",
    "SetPriority",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetPriorityL, Teardown)

EUNIT_TEST(
    "SetTransportAddr - test ",
    "CNATFWCandidate",
    "SetTransportAddr",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetTransportAddrLL, Teardown)

EUNIT_TEST(
    "SetTransportDomainAddrL - test ",
    "CNATFWCandidate",
    "SetTransportDomainAddrL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetTransportDomainAddrLL, Teardown)

EUNIT_TEST(
    "SetTransportProtocol - test ",
    "CNATFWCandidate",
    "SetTransportProtocol",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetTransportProtocolL, Teardown)

EUNIT_TEST(
    "SetBase - test ",
    "CNATFWCandidate",
    "SetBase",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetBaseL, Teardown)

EUNIT_TEST(
    "SetFoundation - test ",
    "CNATFWCandidate",
    "SetFoundation",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetFoundationLL, Teardown)

EUNIT_TEST(
    "SetCandidateType - test ",
    "CNATFWCandidate",
    "SetCandidateType",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetCandidateTypeL, Teardown)

EUNIT_TEST(
    "SetComponentId - test ",
    "CNATFWCandidate",
    "SetComponentId",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_SetComponentIdL, Teardown)

EUNIT_TEST(
    "CompareFoundations - test ",
    "CNATFWCandidate",
    "CompareFoundations",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_CompareFoundationsL, Teardown)

EUNIT_TEST(
    "PriorityOrder - test ",
    "CNATFWCandidate",
    "PriorityOrder",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_PriorityOrderL, Teardown)

EUNIT_TEST(
    "CopyL - test ",
    "CNATFWCandidate",
    "CopyL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidate_CopyLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
