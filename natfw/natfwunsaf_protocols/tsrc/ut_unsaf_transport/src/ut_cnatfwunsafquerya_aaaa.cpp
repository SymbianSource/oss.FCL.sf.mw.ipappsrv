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
#include "ut_cnatfwunsafquerya_aaaa.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "cnatfwunsafquerya_aaaa.h"

// CONSTRUCTION
UT_CNATFWUNSAFQueryA_AAAA* UT_CNATFWUNSAFQueryA_AAAA::NewL()
    {
    UT_CNATFWUNSAFQueryA_AAAA* self = UT_CNATFWUNSAFQueryA_AAAA::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFQueryA_AAAA* UT_CNATFWUNSAFQueryA_AAAA::NewLC()
    {
    UT_CNATFWUNSAFQueryA_AAAA* self = new( ELeave ) UT_CNATFWUNSAFQueryA_AAAA();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFQueryA_AAAA::~UT_CNATFWUNSAFQueryA_AAAA()
    {
    }

// Default constructor
UT_CNATFWUNSAFQueryA_AAAA::UT_CNATFWUNSAFQueryA_AAAA()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFQueryA_AAAA::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFQueryA_AAAA::SetupL()
    {
    iQuery = CNATFWUNSAFQueryA_AAAA::NewL(_L8("test.test.net"));
    }

void UT_CNATFWUNSAFQueryA_AAAA::Teardown()
    {
     delete iQuery;
     iQuery = NULL;
    }

void UT_CNATFWUNSAFQueryA_AAAA::NewLTestL ()
    {
    delete iQuery;
    iQuery = NULL,
    iQuery = CNATFWUNSAFQueryA_AAAA::NewL(_L8("testi.com"));
    EUNIT_ASSERT(iQuery->iTarget->Compare(_L("testi.com")) == 0);

    delete iQuery;
    iQuery = NULL;

    TRAPD(err, iQuery = CNATFWUNSAFQueryA_AAAA::NewL(_L8("")));
    if(err == KErrNone)
        {
        User::Leave(KErrArgument);
        }
    if(err == KErrNoMemory)
        {
        User::Leave(KErrNoMemory);
        }

    delete iQuery;
    iQuery = NULL,
    iQuery = CNATFWUNSAFQueryA_AAAA::NewL(_L8("testi.com"));
    TRAPD(error, iQuery->ResultTargetL())
    if(error != KErrNotSupported)
        {
        User::Leave(error);
        }

    delete iQuery;
    iQuery = NULL,
    iQuery = CNATFWUNSAFQueryA_AAAA::NewL(_L8("testi.com"));
    TRAPD(error2, iQuery->RemoveElementL(0))
    if(error2 != KErrNotSupported)
        {
        User::Leave(error2);
        }

    delete iQuery;
    iQuery = NULL,
    iQuery = CNATFWUNSAFQueryA_AAAA::NewL(_L8("testi.com"));
    TRAPD(error3, iQuery->ArrayCountL())
    if(error3 != KErrNotSupported)
        {
        User::Leave(error3);
        }

    delete iQuery;
    iQuery = NULL,
    iQuery = CNATFWUNSAFQueryA_AAAA::NewL(_L8("testi.com"));
    //above value is never used
    MNATFWUNSAFHostResolver* aResolver = NULL;

    TRAPD(error4, iQuery->HandleQueryResultL(*aResolver))
    if(error4 != KErrNotSupported)
        {
        User::Leave(error4);
        }
    TRAPD(errorlast, iQuery->SetTargetL(_L8("")));
    EUNIT_ASSERT(errorlast != KErrNone);
    }

void UT_CNATFWUNSAFQueryA_AAAA::QueryResultBufTestL ()
    {
    //No query made
    TInetAddr addr(iQuery->QueryResultBuf());

    EUNIT_ASSERT(addr.Address() == KInetAddrNone);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFQueryA_AAAA,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewLTestL - test ",
    "CNATFWUNSAFQueryA_AAAA",
    "NewLTestL",
    "FUNCTIONALITY",
    SetupL, NewLTestL, Teardown)

EUNIT_TEST(
    "QueryResultBufTestL - test ",
    "CNATFWUNSAFQueryA_AAAA",
    "QueryResultBufTestL",
    "FUNCTIONALITY",
    SetupL, QueryResultBufTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
