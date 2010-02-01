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
#include "UT_CIceCandidatePrioritizer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidate.h"

//  INTERNAL INCLUDES
#include "cicecandidateprioritizer.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceCandidatePrioritizer* UT_CIceCandidatePrioritizer::NewL()
    {
    UT_CIceCandidatePrioritizer* self = UT_CIceCandidatePrioritizer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceCandidatePrioritizer* UT_CIceCandidatePrioritizer::NewLC()
    {
    UT_CIceCandidatePrioritizer* self = new( ELeave ) UT_CIceCandidatePrioritizer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceCandidatePrioritizer::~UT_CIceCandidatePrioritizer()
    {
    }

// Default constructor
UT_CIceCandidatePrioritizer::UT_CIceCandidatePrioritizer()
    {
    }

// Second phase construct
void UT_CIceCandidatePrioritizer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIceCandidatePrioritizer::SetupL(  )
    {
    iPioritizer = CIceCandidatePrioritizer::NewL( KTestDomain );
    }

void UT_CIceCandidatePrioritizer::Teardown(  )
    {
    delete iPioritizer;
    }


void UT_CIceCandidatePrioritizer::UT_CICECandidatePrioritizer_PrioritizeL(  )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetType( CNATFWCandidate::EHost );
    candidate->SetTransportProtocol( KProtocolInetUdp );
    candidate->SetComponentId( 1 );
    
    iPioritizer->PrioritizeL( *candidate );
    
    candidate->SetType( CNATFWCandidate::EServerReflexive );
    iPioritizer->PrioritizeL( *candidate );
    
    candidate->SetType( CNATFWCandidate::EPeerReflexive );
    iPioritizer->PrioritizeL( *candidate );
    
    candidate->SetType( CNATFWCandidate::ERelay );
    iPioritizer->PrioritizeL( *candidate );
    
    CleanupStack::PopAndDestroy( candidate );
    }

void UT_CIceCandidatePrioritizer::UT_CICECandidatePrioritizer_Prioritize2L(  )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    const TInt KInvalidCandidateType( -1 );
    candidate->SetType( 
        static_cast<CNATFWCandidate::TCandidateType>( KInvalidCandidateType ) );
    candidate->SetTransportProtocol( KProtocolInetUdp );
    candidate->SetComponentId( 1 );
    
    EUNIT_ASSERT_PANIC(
        iPioritizer->PrioritizeL( *candidate ), "USER", 0 );
    
    CleanupStack::PopAndDestroy( candidate );
    }


void UT_CIceCandidatePrioritizer::UT_CICECandidatePrioritizer_Prioritize3L(  )
    {
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetType( CNATFWCandidate::EHost );
    
    const TInt KInvalidProtocol( -1 );
    candidate->SetTransportProtocol( 
        static_cast<TUint>(KInvalidProtocol) );
    candidate->SetComponentId( 1 );
    
    EUNIT_ASSERT_PANIC(
        iPioritizer->PrioritizeL( *candidate ), "USER", 0 );
    
    CleanupStack::PopAndDestroy( candidate );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceCandidatePrioritizer,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "Prioritize - test ",
    "CIceCandidatePrioritizer",
    "Prioritize",
    "FUNCTIONALITY",
    SetupL, UT_CICECandidatePrioritizer_PrioritizeL, Teardown)

EUNIT_TEST(
    "Prioritize2 - test ",
    "CIceCandidatePrioritizer",
    "Prioritize2",
    "FUNCTIONALITY",
    SetupL, UT_CICECandidatePrioritizer_Prioritize2L, Teardown)

EUNIT_TEST(
    "Prioritize3 - test ",
    "CIceCandidatePrioritizer",
    "Prioritize3",
    "FUNCTIONALITY",
    SetupL, UT_CICECandidatePrioritizer_Prioritize3L, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
