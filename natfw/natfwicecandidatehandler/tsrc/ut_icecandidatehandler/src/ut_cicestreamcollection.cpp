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
#include "UT_CIceStreamCollection.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include "natfwcandidatepair.h"
#include "natfwcandidate.h"

//  INTERNAL INCLUDES
#include "cicestreamcollection.h"
#include "icecandidatehandlertestdefs.h"

// CONSTRUCTION
UT_CIceStreamCollection* UT_CIceStreamCollection::NewL()
    {
    UT_CIceStreamCollection* self = UT_CIceStreamCollection::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CIceStreamCollection* UT_CIceStreamCollection::NewLC()
    {
    UT_CIceStreamCollection* self = new( ELeave ) UT_CIceStreamCollection();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CIceStreamCollection::~UT_CIceStreamCollection()
    {
    delete iCollection;
    }

// Default constructor
UT_CIceStreamCollection::UT_CIceStreamCollection()
    {
    }

// Second phase construct
void UT_CIceStreamCollection::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CIceStreamCollection::SetupL(  )
    {
    iCollection = CIceStreamCollection::NewL( KTestStreamCollectionId1 );
    }

void UT_CIceStreamCollection::Teardown(  )
    {
    delete iCollection;
    iCollection = NULL;
    }

void UT_CIceStreamCollection::UT_CNATFWStreamCollection_StreamCollectionIdL(  )
    {
    EUNIT_ASSERT( 
        KTestStreamCollectionId1 == iCollection->StreamCollectionId() );
    }


void UT_CIceStreamCollection::UT_CNATFWStreamCollection_AddMediaComponentLL()
    {
    if ( TIceTestUtils::AllocTestEnabled() )
        {
        iCollection->AddMediaComponentL( 1, KComponentIdRtp );
        EUNIT_ASSERT_LEAVE( 
            iCollection->AddMediaComponentL( 1, KComponentIdRtp ) );
        }
    else
        {
        EUNIT_ASSERT_NO_LEAVE( 
            iCollection->AddMediaComponentL( 1, KComponentIdRtp ) );
        EUNIT_ASSERT_LEAVE( 
            iCollection->AddMediaComponentL( 1, KComponentIdRtp ) );
        }
    }

void UT_CIceStreamCollection::UT_CNATFWStreamCollection_RemoveMediaComponentL()
    {
    iCollection->AddMediaComponentL( 1, KComponentIdRtp );
    EUNIT_ASSERT( 1 == iCollection->iMediaComponents.Count() );
    
    iCollection->RemoveMediaComponent( 2 );
    EUNIT_ASSERT( 1 == iCollection->iMediaComponents.Count() );
    
    iCollection->RemoveMediaComponent( 1 );
    EUNIT_ASSERT( 0 == iCollection->iMediaComponents.Count() );
    }

void UT_CIceStreamCollection::UT_CNATFWStreamCollection_HasMediaComponentL()
    {
    EUNIT_ASSERT( !iCollection->HasMediaComponent( 1 ) );
    
    iCollection->AddMediaComponentL( 1, KComponentIdRtp );
    EUNIT_ASSERT( !iCollection->HasMediaComponent( 2 ) );
    EUNIT_ASSERT( iCollection->HasMediaComponent( 1 ) );
    }

void UT_CIceStreamCollection::UT_CNATFWStreamCollection_NumOfMediaComponentsL()
    {
    EUNIT_ASSERT( 0 == iCollection->NumOfMediaComponents() );
    
    iCollection->AddMediaComponentL( 1, KComponentIdRtp );
    EUNIT_ASSERT( 1 == iCollection->NumOfMediaComponents() );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CIceStreamCollection,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "StreamCollectionId - test ",
    "CIceStreamCollection",
    "StreamCollectionId",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStreamCollection_StreamCollectionIdL, Teardown)

EUNIT_TEST(
    "AddMediaComponentL - test ",
    "CIceStreamCollection",
    "AddMediaComponentL",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStreamCollection_AddMediaComponentLL, Teardown)

EUNIT_TEST(
    "RemoveMediaComponent - test ",
    "CIceStreamCollection",
    "RemoveMediaComponent",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStreamCollection_RemoveMediaComponentL, Teardown)

EUNIT_TEST(
    "HasMediaComponent - test ",
    "CIceStreamCollection",
    "HasMediaComponent",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStreamCollection_HasMediaComponentL, Teardown)

EUNIT_TEST(
    "NumOfMediaComponents - test ",
    "CIceStreamCollection",
    "NumOfMediaComponents",
    "FUNCTIONALITY",
    SetupL, UT_CNATFWStreamCollection_NumOfMediaComponentsL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
