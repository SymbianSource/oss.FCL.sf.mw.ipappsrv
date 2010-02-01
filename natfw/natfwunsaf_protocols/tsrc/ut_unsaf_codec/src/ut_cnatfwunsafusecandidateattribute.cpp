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
#include "UT_CNATFWUNSAFUseCandidateAttribute.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwunsafusecandidateattribute.h"

// CONSTRUCTION
UT_CNATFWUNSAFUseCandidateAttribute* UT_CNATFWUNSAFUseCandidateAttribute::NewL()
    {
    UT_CNATFWUNSAFUseCandidateAttribute* self = UT_CNATFWUNSAFUseCandidateAttribute::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFUseCandidateAttribute* UT_CNATFWUNSAFUseCandidateAttribute::NewLC()
    {
    UT_CNATFWUNSAFUseCandidateAttribute* self = new( ELeave ) UT_CNATFWUNSAFUseCandidateAttribute();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFUseCandidateAttribute::~UT_CNATFWUNSAFUseCandidateAttribute()
    {
    }

// Default constructor
UT_CNATFWUNSAFUseCandidateAttribute::UT_CNATFWUNSAFUseCandidateAttribute()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFUseCandidateAttribute::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNATFWUNSAFUseCandidateAttribute::SetupL(  )
    {
    iAttribute = CNATFWUNSAFUseCandidateAttribute::NewL();
    }

void UT_CNATFWUNSAFUseCandidateAttribute::Teardown(  )
    {
    delete iAttribute;
    iAttribute = NULL;
    }

void UT_CNATFWUNSAFUseCandidateAttribute::TestGettersL()
    {
    EUNIT_ASSERT(CNATFWUNSAFAttribute::EUseCandidate == iAttribute->Type());
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFUseCandidateAttribute,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "TestGettersL - test ",
    "CNATFWUNSAFUseCandidateAttribute",
    "TestGettersL",
    "FUNCTIONALITY",
    SetupL, TestGettersL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
