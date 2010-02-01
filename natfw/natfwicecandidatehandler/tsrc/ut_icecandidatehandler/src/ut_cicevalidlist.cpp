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
#include "ut_cicevalidlist.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"

//  INTERNAL INCLUDES
#include "cicevalidlist.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceValidList* UT_CIceValidList::NewL()
    {
    UT_CIceValidList* self = UT_CIceValidList::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceValidList* UT_CIceValidList::NewLC()
    {
    UT_CIceValidList* self = new( ELeave ) UT_CIceValidList();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceValidList::~UT_CIceValidList()
    {
    delete iValidList;
    }

// Default constructor
UT_CIceValidList::UT_CIceValidList()
    {
    }

// Second phase construct
void UT_CIceValidList::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIceValidList::SetupL(  )
    {
    iValidList = CIceValidList::NewL( KTestStreamCollectionId1 );
    }

void UT_CIceValidList::Teardown(  )
    {
    delete iValidList;
    iValidList = NULL;
    }

void UT_CIceValidList::UT_CICEValidList_StreamCollectionIdL(  )
    {
    EUNIT_ASSERT( 
        KTestStreamCollectionId1 == iValidList->StreamCollectionId() );
    }

void UT_CIceValidList::UT_CICEValidList_AddValidPairLL(  )
    {
    CNATFWCandidate* localCand = CNATFWCandidate::NewLC();
    localCand->SetComponentId( KComponentIdRtp );
    CNATFWCandidate* remoteCand = CNATFWCandidate::NewLC();
    remoteCand->SetComponentId( KComponentIdRtp );
    CNATFWCandidatePair* pair 
        = CNATFWCandidatePair::NewLC( *localCand, *remoteCand );
    pair->SetPriority( 200 );
    
    iValidList->AddValidPairL( *pair );
    EUNIT_ASSERT( iValidList->iValidList.Count() == 1 );
    
    // there is existing pair, copy it
    CNATFWCandidatePair* copyPair = CNATFWCandidatePair::NewLC( *pair );
    iValidList->AddValidPairL( *copyPair );
    EUNIT_ASSERT( iValidList->iValidList.Count() == 1 );
    
    CNATFWCandidate* localCandRtcp = CNATFWCandidate::NewLC();
    localCandRtcp->SetComponentId( KComponentIdRtcp );
    CNATFWCandidate* remoteCandRtcp = CNATFWCandidate::NewLC();
    remoteCandRtcp->SetComponentId( KComponentIdRtcp );
    CNATFWCandidatePair* pairRtcp 
        = CNATFWCandidatePair::NewLC( *localCandRtcp, *remoteCandRtcp );
    pairRtcp->SetPriority( 100 );
    iValidList->AddValidPairL( *pairRtcp );
    EUNIT_ASSERT( iValidList->iValidList.Count() == 2 );
    
    CleanupStack::PopAndDestroy( pairRtcp );
    CleanupStack::PopAndDestroy( remoteCandRtcp );
    CleanupStack::PopAndDestroy( localCandRtcp );
    CleanupStack::PopAndDestroy( copyPair );
    
    CleanupStack::PopAndDestroy( pair );
    CleanupStack::PopAndDestroy( remoteCand );
    CleanupStack::PopAndDestroy( localCand );
    }

void UT_CIceValidList::UT_CICEValidList_HasPairL()
    {
    CNATFWCandidate* localCand = CNATFWCandidate::NewLC();
    localCand->SetComponentId( KComponentIdRtp );
    TInetAddr localAddr( KInetAddrAny );
    localAddr.SetFamily( KAfInet );
    localCand->SetTransportAddrL( localAddr );
    CNATFWCandidate* remoteCand = CNATFWCandidate::NewLC();
    remoteCand->SetComponentId( KComponentIdRtp );
    TInetAddr remoteAddr( KInetAddrAny );
    remoteAddr.SetFamily( KAfInet );    
    remoteCand->SetTransportAddrL( remoteAddr );
    CNATFWCandidatePair* pair 
        = CNATFWCandidatePair::NewLC( *localCand, *remoteCand );
    
    EUNIT_ASSERT( !iValidList->HasPair( *pair ) );
    
    iValidList->AddValidPairL( *pair );
    EUNIT_ASSERT( iValidList->iValidList.Count() == 1 );
    EUNIT_ASSERT( iValidList->HasPair( *pair ) );
    
    CleanupStack::PopAndDestroy( pair );
    CleanupStack::PopAndDestroy( remoteCand );
    CleanupStack::PopAndDestroy( localCand );
    }

void UT_CIceValidList::UT_CICEValidList_HasPairForComponentsL()
    {
    RArray<TUint> components;
    CleanupClosePushL( components );
    
    components.AppendL( 5 );
    EUNIT_ASSERT( 
        EFalse == iValidList->HasPairForComponents( components ) );
    
    UT_CICEValidList_AddValidPairLL();
    EUNIT_ASSERT( 
        EFalse == iValidList->HasPairForComponents( components ) );
    components.AppendL( KComponentIdRtp );
    EUNIT_ASSERT( 
        EFalse == iValidList->HasPairForComponents( components ) );
    components.Remove( 0 );
    EUNIT_ASSERT( 
        ETrue == iValidList->HasPairForComponents( components ) );
    
    components.Reset();
    EUNIT_ASSERT_PANIC( 
        iValidList->HasPairForComponents( components ), "USER", 0 );
    
    CleanupStack::PopAndDestroy( &components );
    }

void UT_CIceValidList::UT_CICEValidList_SetPairNominatedLL()
    {
    UT_CICEValidList_AddValidPairLL();
    
    CNATFWCandidate* localCand = CNATFWCandidate::NewLC();
    TInetAddr addr( INET_ADDR( 10, 0, 0, 1 ), 5000 );
    localCand->SetTransportAddrL( addr );
    CNATFWCandidate* remoteCand = CNATFWCandidate::NewLC();
    CNATFWCandidatePair* invalidPair 
        = CNATFWCandidatePair::NewLC( *localCand, *remoteCand );
    
    EUNIT_ASSERT_LEAVE( iValidList->SetPairNominatedL( *invalidPair ) );
    EUNIT_ASSERT_NO_LEAVE( 
        iValidList->SetPairNominatedL( *iValidList->iValidList[0] ) );
    EUNIT_ASSERT( iValidList->iValidList[0]->Selected() );

    CleanupStack::PopAndDestroy( invalidPair );
    CleanupStack::PopAndDestroy( remoteCand );
    CleanupStack::PopAndDestroy( localCand );
    }

void UT_CIceValidList::UT_CICEValidList_NominatedPairsExistL()
    {
    RArray<TUint> ids;
    CleanupClosePushL( ids );
    
    ids.AppendL( KComponentIdRtp );
    EUNIT_ASSERT( !iValidList->NominatedPairsExist( ids ) );
    
    UT_CICEValidList_SetPairNominatedLL();
    EUNIT_ASSERT( iValidList->NominatedPairsExist( ids ) );
    
    ids.Reset();
    EUNIT_ASSERT_PANIC( 
        iValidList->NominatedPairsExist( ids ), "USER", 0 );
    CleanupStack::PopAndDestroy( &ids );
    }
    
void UT_CIceValidList::UT_CICEValidList_SelectedPairL()
    {
    EUNIT_ASSERT( !iValidList->SelectedPair( KComponentIdRtp ) );

    UT_CICEValidList_SetPairNominatedLL();
    EUNIT_ASSERT( !iValidList->SelectedPair( 5 ) );
    
    EUNIT_ASSERT( iValidList->SelectedPair( KComponentIdRtp ) );
    }

void UT_CIceValidList::UT_CICEValidList_HighestPriorityPairL()
    {
    EUNIT_ASSERT( 
        NULL == iValidList->HighestPriorityPair( KComponentIdRtp ) );
    
    UT_CICEValidList_AddValidPairLL();
    EUNIT_ASSERT( 
        NULL != iValidList->HighestPriorityPair( KComponentIdRtp ) );
    EUNIT_ASSERT( 
        NULL != iValidList->HighestPriorityPair( KComponentIdRtcp ) );
    }

void UT_CIceValidList::UT_CICEValidList_ValidListL(  )
    {
    EUNIT_ASSERT( !iValidList->iValidList.Count() );
    }

void UT_CIceValidList::UT_CICEValidList_FlushValidListL(  )
    {
    CNATFWCandidate* localCand = CNATFWCandidate::NewLC();
    CNATFWCandidate* remoteCand = CNATFWCandidate::NewLC();
    CNATFWCandidatePair* pair = CNATFWCandidatePair::NewLC(
        *localCand, *remoteCand );
    
    iValidList->AddValidPairL( *pair );
    EUNIT_ASSERT( iValidList->iValidList.Count() == 1 );
    
    CleanupStack::PopAndDestroy( pair );
    CleanupStack::PopAndDestroy( remoteCand );
    CleanupStack::PopAndDestroy( localCand );
    
    iValidList->FlushValidList();
    EUNIT_ASSERT( !iValidList->iValidList.Count() );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceValidList,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "StreamCollectionId - test ",
    "CIceValidList",
    "StreamCollectionId",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_StreamCollectionIdL, Teardown)

EUNIT_TEST(
    "AddValidPairL - test ",
    "CIceValidList",
    "AddValidPairL",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_AddValidPairLL, Teardown)

EUNIT_TEST(
    "HasPairL - test ",
    "CIceValidList",
    "HasPairL",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_HasPairL, Teardown)

EUNIT_TEST(
    "HasPairForComponents - test ",
    "CIceValidList",
    "HasPairForComponents",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_HasPairForComponentsL, Teardown)

EUNIT_TEST(
    "SetPairNominatedL - test ",
    "CIceValidList",
    "SetPairNominatedL",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_SetPairNominatedLL, Teardown)

EUNIT_TEST(
    "NominatedPairsExist - test ",
    "CIceValidList",
    "NominatedPairsExist",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_NominatedPairsExistL, Teardown)

EUNIT_TEST(
    "SelectedPair - test ",
    "CIceValidList",
    "SelectedPair",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_SelectedPairL, Teardown)
    
EUNIT_TEST(
    "HighestPriorityPairL - test ",
    "CIceValidList",
    "HighestPriorityPairL",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_HighestPriorityPairL, Teardown)

EUNIT_TEST(
    "ValidList - test ",
    "CIceValidList",
    "ValidList",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_ValidListL, Teardown)

EUNIT_TEST(
    "FlushValidList - test ",
    "CIceValidList",
    "FlushValidList",
    "FUNCTIONALITY",
    SetupL, UT_CICEValidList_FlushValidListL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
