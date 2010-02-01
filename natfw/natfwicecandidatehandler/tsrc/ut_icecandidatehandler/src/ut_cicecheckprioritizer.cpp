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
#include "UT_CIceCheckPrioritizer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"

//  INTERNAL INCLUDES
#include "cicecheckprioritizer.h"
#include "ciceconnectivitycheck.h"

// CONSTRUCTION
UT_CIceCheckPrioritizer* UT_CIceCheckPrioritizer::NewL()
    {
    UT_CIceCheckPrioritizer* self = UT_CIceCheckPrioritizer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceCheckPrioritizer* UT_CIceCheckPrioritizer::NewLC()
    {
    UT_CIceCheckPrioritizer* self = new( ELeave ) UT_CIceCheckPrioritizer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceCheckPrioritizer::~UT_CIceCheckPrioritizer()
    {
    delete iPrioritizer;
    }

// Default constructor
UT_CIceCheckPrioritizer::UT_CIceCheckPrioritizer()
    {
    }

// Second phase construct
void UT_CIceCheckPrioritizer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIceCheckPrioritizer::SetupL(  )
    {
    iPrioritizer = CIceCheckPrioritizer::NewL();
    }

void UT_CIceCheckPrioritizer::Teardown(  )
    {
    delete iPrioritizer;
    iPrioritizer = NULL;
    }

void UT_CIceCheckPrioritizer::UT_CICECheckPrioritizer_PrioritizeCheckL()
    {
    CNATFWCandidate* candidate1 = CNATFWCandidate::NewLC();
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewLC();
    candidate1->SetPriority( 100 );
    candidate2->SetPriority( 200 );
    CNATFWCandidatePair* pair = CNATFWCandidatePair::NewLC( 
        *candidate1, *candidate2 );
    
    iPrioritizer->PrioritizePair( *pair, EIceRoleControlling );
    TUint64 priority1 = pair->Priority();
    
    iPrioritizer->PrioritizePair( *pair, EIceRoleControlled );
    TUint64 priority2 = pair->Priority();
    
    TUint64 difference = priority1 > priority2 
        ? ( priority1 - priority2 )
        : ( priority2 - priority1 );
    EUNIT_ASSERT( 1 == difference );
    
    CleanupStack::PopAndDestroy( pair );
    CleanupStack::PopAndDestroy( candidate2 );
    CleanupStack::PopAndDestroy( candidate1 );
    }

void UT_CIceCheckPrioritizer::UT_CICECheckPrioritizer_PrioritizeChecksL()
    {
    CNATFWCandidate* candidate1 = CNATFWCandidate::NewLC();
    CNATFWCandidate* candidate2 = CNATFWCandidate::NewLC();
    CNATFWCandidatePair* pair = CNATFWCandidatePair::NewLC( 
        *candidate1, *candidate2 );

    CNATFWCandidatePair* pair2 = CNATFWCandidatePair::NewLC( *pair );
    
    RPointerArray<CNATFWCandidatePair> checks;
    checks.Append( pair );
    checks.Append( pair2 );
    iPrioritizer->PrioritizePairs( checks, EIceRoleControlling );

    CleanupStack::PopAndDestroy( pair2 );
    CleanupStack::PopAndDestroy( pair );
    CleanupStack::PopAndDestroy( candidate2 );
    CleanupStack::PopAndDestroy( candidate1 );
    
    checks.Close();
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceCheckPrioritizer,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "PrioritizeCheck - test ",
    "CIceCheckPrioritizer",
    "PrioritizeCheck",
    "FUNCTIONALITY",
    SetupL, UT_CICECheckPrioritizer_PrioritizeCheckL, Teardown)

EUNIT_TEST(
    "PrioritizeChecks - test ",
    "CIceCheckPrioritizer",
    "PrioritizeChecks",
    "FUNCTIONALITY",
    SetupL, UT_CICECheckPrioritizer_PrioritizeChecksL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
