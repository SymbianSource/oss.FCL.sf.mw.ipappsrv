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
#include "UT_CNATFWCandidatePair.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"
#include "natfwconnectivityapidefs.h"
#include "natfwunittestmacros.h"

const TUint KIceTestPort = 5000;
const TUint KIceTestSessionId = 45;
const TUint KIceTestStreamId = 5;
const TUint KIceTestPriority = 8;
const TUint64 KIceTestPriorityOrder = 10;

const TUint KIceTestStreamCollectionId = 7;
const TUint KIceTestComponentId = 6;
const TBool KIceTestSelected = ETrue;
const TUint32 KTransportInetAddr = INET_ADDR( 10, 0, 0, 1 );
const TUint32 KBaseInetAddr = INET_ADDR( 10, 0, 0, 2 );
const TUint32 KRemotetInetAddr = INET_ADDR( 10, 0, 0, 3 );
const CNATFWCandidate::TCandidateType KIceTestCandType = CNATFWCandidate::ERelay;
const TUint KTransportProtocol = KProtocolInetTcp;
_LIT8( KIceTestFoundation, "Foundation" );


// CONSTRUCTION
UT_CNATFWCandidatePair* UT_CNATFWCandidatePair::NewL()
    {
    UT_CNATFWCandidatePair* self = UT_CNATFWCandidatePair::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWCandidatePair* UT_CNATFWCandidatePair::NewLC()
    {
    UT_CNATFWCandidatePair* self = new( ELeave ) UT_CNATFWCandidatePair();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWCandidatePair::~UT_CNATFWCandidatePair()
    {
    }

// Default constructor
UT_CNATFWCandidatePair::UT_CNATFWCandidatePair()
    {
    }

// Second phase construct
void UT_CNATFWCandidatePair::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CNATFWCandidatePair::SetupL(  )
    {
    iCandPair = CNATFWCandidatePair::NewL();
    iCandidate = CNATFWCandidate::NewL();
    }

void UT_CNATFWCandidatePair::Setup2L(  )
    {
    SetupL();
    
    CNATFWCandidate* localCand = CNATFWCandidate::NewL();
    
    localCand->SetBase( TInetAddr( KBaseInetAddr, KIceTestPort ) );
    localCand->SetType( KIceTestCandType );
    localCand->SetComponentId( KIceTestComponentId );
    localCand->SetFoundationL( KIceTestFoundation );
    localCand->SetPriority( KIceTestPriority );
    localCand->SetSessionId( KIceTestSessionId );
    localCand->SetStreamCollectionId( KIceTestStreamCollectionId );
    localCand->SetStreamId( KIceTestStreamId );
    localCand->SetTransportAddrL( TInetAddr( KTransportInetAddr, KIceTestPort ) );
    localCand->SetTransportProtocol( KTransportProtocol );

    CNATFWCandidate* remoteCand = CNATFWCandidate::NewL( *localCand );
    
    iCandPair->SetLocalCandidateL( *localCand );
    iCandPair->SetRemoteCandidateL( *remoteCand );
    iCandPair->SetPriority( KIceTestPriorityOrder );
    iCandPair->SetSelected( KIceTestSelected );
    
    delete iCandidate;
    iCandidate = NULL;
    iCandidate = CNATFWCandidate::NewL( *localCand );
    
    delete localCand;
    delete remoteCand;
    }

void UT_CNATFWCandidatePair::Teardown(  )
    {
    delete iCandPair;
    delete iCandidate;
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_NewLL(  )
    {
    CNATFWCandidatePair* candPair1;
    CNATFWCandidatePair* candPair2;

    NATFW_EUNIT_ASSERT_NO_LEAVE( candPair1 = CNATFWCandidatePair::NewL() ); 
    CleanupStack::PushL( candPair1 );
    
    candPair1->SetRemoteCandidateL( *iCandidate );
    candPair1->SetLocalCandidateL( *iCandidate );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        candPair2 = CNATFWCandidatePair::NewL( *candPair1 ) );

    CleanupStack::PopAndDestroy( candPair1 );
    delete candPair2;
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_NewL_1L(  )
    {
    CNATFWCandidatePair* candPair;
    CNATFWCandidate* cand1;
    CNATFWCandidate* cand2;

    NATFW_EUNIT_ASSERT_NO_LEAVE( cand1 = CNATFWCandidate::NewL() );
    CleanupStack::PushL( cand1 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( cand2 = CNATFWCandidate::NewL() ); 
    CleanupStack::PushL( cand2 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        candPair = CNATFWCandidatePair::NewL( *cand1, *cand2 ) );
    
    CleanupStack::PopAndDestroy( cand2 );
    CleanupStack::PopAndDestroy( cand1 );
    delete candPair;
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_SetLocalCandidateLL(  )
    {
    CNATFWCandidate* candi = CNATFWCandidate::NewLC();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( candi->SetSessionId( KIceTestSessionId ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCandPair->SetLocalCandidateL( *candi ) );
    EUNIT_ASSERT_EQUALS( KIceTestSessionId,
        iCandPair->LocalCandidate().SessionId() );
    
    CleanupStack::PopAndDestroy( candi );
    }
    
void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_SetRemoteCandidateLL(  )
    {
    CNATFWCandidate* candi = CNATFWCandidate::NewLC();
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( candi->SetSessionId( KIceTestSessionId ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCandPair->SetRemoteCandidateL( *candi ) );
    EUNIT_ASSERT_EQUALS( KIceTestSessionId,
        iCandPair->RemoteCandidate().SessionId() );

    CleanupStack::PopAndDestroy( candi );
    }
    
void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_SetPriorityL(  )
    {
    iCandPair->SetPriority( KIceTestPriorityOrder );
    EUNIT_ASSERT_EQUALS( KIceTestPriorityOrder, iCandPair->Priority() );
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_CopyLL(  )
    {
    const TUint64 KPriorityToComp = 69;
    CNATFWCandidatePair* candPair = CNATFWCandidatePair::NewLC( *iCandPair );
    candPair->SetPriority( KPriorityToComp );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCandPair->CopyL( *candPair ) );
    CleanupStack::PopAndDestroy( candPair );
    
    EUNIT_ASSERT_EQUALS( KPriorityToComp, iCandPair->Priority() );
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_SetSelectedL(  )
    {
    iCandPair->SetSelected( ETrue );
    EUNIT_ASSERT( iCandPair->Selected() );
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_PriorityOrderL(  )
    {
    iCandPair->SetPriority( 1 );
    EUNIT_ASSERT( iCandPair->PriorityOrder( *iCandPair, *iCandPair ) == 0 );
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_CompareL(  )
    {
    EUNIT_ASSERT( iCandPair->Compare( *iCandPair, *iCandPair ) );
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_MatchAddressesL(  )
    {
    CNATFWCandidatePair* candPair = CNATFWCandidatePair::NewL( *iCandPair );
    
    EUNIT_ASSERT( iCandPair->MatchAddresses(  *iCandPair, *candPair ) );
    
    delete candPair;
    candPair = NULL;
    
    candPair = CNATFWCandidatePair::NewL();
    
    EUNIT_ASSERT( !iCandPair->MatchAddresses(  *iCandPair, *candPair ) );
    
    delete candPair;
    candPair = NULL;
    }

void UT_CNATFWCandidatePair::UT_CNATFWCandidatePair_operatorsL(  )
    {
    CNATFWCandidatePair* candpair1 =
        CNATFWCandidatePair::NewLC();

    NATFW_EUNIT_ASSERT_NO_LEAVE(
        candpair1->SetLocalCandidateL( *iCandidate ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( 
        candpair1->SetRemoteCandidateL( *iCandidate ) );

    EUNIT_ASSERT( *candpair1 == *candpair1 );
    EUNIT_ASSERT( *candpair1 != *iCandPair );
    
    CleanupStack::PopAndDestroy( candpair1 );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWCandidatePair,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CNATFWCandidatePair",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidatePair_NewLL, Teardown)
    
EUNIT_TEST(
    "NewL - test 2 ",
    "CNATFWCandidatePair",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidatePair_NewL_1L, Teardown)

EUNIT_TEST(
    "SetLocalCandidateL - test ",
    "CNATFWCandidatePair",
    "SetLocalCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidatePair_SetLocalCandidateLL, Teardown)

EUNIT_TEST(
    "SetRemoteCandidateL - test ",
    "CNATFWCandidatePair",
    "SetRemoteCandidateL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidatePair_SetRemoteCandidateLL, Teardown)

EUNIT_TEST(
    "SetPriority - test ",
    "CNATFWCandidatePair",
    "SetPriority",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidatePair_SetPriorityL, Teardown)

EUNIT_TEST(
    "CopyL - test ",
    "CNATFWCandidatePair",
    "CopyL",
    "FUNCTIONALITY",
    Setup2L, UT_CNATFWCandidatePair_CopyLL, Teardown)

EUNIT_TEST(
    "SetSelected - test ",
    "CNATFWCandidatePair",
    "SetSelected",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidatePair_SetSelectedL, Teardown)

EUNIT_TEST(
    "PriorityOrder - test ",
    "CNATFWCandidatePair",
    "PriorityOrder",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWCandidatePair_PriorityOrderL, Teardown)

EUNIT_TEST(
    "Compare - test ",
    "CNATFWCandidatePair",
    "Compare",
    "FUNCTIONALITY",
    Setup2L, UT_CNATFWCandidatePair_CompareL, Teardown)

EUNIT_TEST(
    "MatchAddresses - test ",
    "CNATFWCandidatePair",
    "MatchAddresses",
    "FUNCTIONALITY",
    Setup2L, UT_CNATFWCandidatePair_MatchAddressesL, Teardown)

EUNIT_TEST(
    "operators - test ",
    "CNATFWCandidatePair",
    "operators",
    "FUNCTIONALITY",
    Setup2L, UT_CNATFWCandidatePair_operatorsL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
