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
#include "ut_cnatfwunsafserverresolver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <es_sock.h>
#include <e32def.h>
#include <CommDbConnPref.h>

//  INTERNAL INCLUDES
#include "ut_cnatfwunsafserverresolverobserverstub.h"
#include "cnatfwunsafquerydata.h"
#include "cnatfwunsafa_aaaaorigdomain.h"
#include "natfwunsafserverresolverobserver.h"
#include "mnatfwunsafhostresolver.h"
#include "natfwunsafserverresolver.h"
#include "cnatfwunsafhostresolver.h"
#include "cnatfwunsafserverquery.h"
#include "cnatfwunsafquerybase.h"
#include "cnatfwunsafquerya_aaaa.h"

//  CONSTANTS
#define STUNDefaultPort (3478)

// CONSTRUCTION
UT_CNATFWUNSAFServerResolver* UT_CNATFWUNSAFServerResolver::NewL()
    {
    UT_CNATFWUNSAFServerResolver* self = UT_CNATFWUNSAFServerResolver::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFServerResolver* UT_CNATFWUNSAFServerResolver::NewLC()
    {
    UT_CNATFWUNSAFServerResolver* self = new( ELeave ) UT_CNATFWUNSAFServerResolver();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFServerResolver::~UT_CNATFWUNSAFServerResolver()
    {
    }

// Default constructor
UT_CNATFWUNSAFServerResolver::UT_CNATFWUNSAFServerResolver()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFServerResolver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFServerResolver::SetupL()
    {
    iResultArray = new (ELeave) RArray<TInetAddr>;
    User::LeaveIfError(iServer.Connect(KESockDefaultMessageSlots));
    User::LeaveIfError(iConnection.Open(iServer, KConnectionTypeDefault));
    TCommDbConnPref prefs;
    prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
    prefs.SetDirection(ECommDbConnectionDirectionOutgoing);
    prefs.SetIapId(10);
    User::LeaveIfError(iConnection.Start(prefs));
    iObserver = UT_CNATFWUNSAFServerResolverObserverStub::NewL();
    iServerResolver = CNATFWUNSAFServerResolver::NewL(iServer,
                                                        iConnection,
                                                      *iObserver,
                                                      EFalse);

    }

void UT_CNATFWUNSAFServerResolver::Teardown()
    {
    delete iServerResolver;iServerResolver = NULL;
     delete iObserver;iObserver = NULL;
     if ( iResultArray )
        {
        iResultArray->Reset();
        delete iResultArray;
        iResultArray = NULL;
        }

    iConnection.Close();
    iServer.Close();
    }

void UT_CNATFWUNSAFServerResolver::NewLTestL ()
    {
    delete iServerResolver;
    iServerResolver = NULL;
    iServerResolver = CNATFWUNSAFServerResolver::NewL(iServer,
                                                 iConnection,
                                                 *iObserver,
                                                 EFalse);
    }


void UT_CNATFWUNSAFServerResolver::ResolveL1TestL()
    {
    TRAPD(err, iServerResolver->ResolveL(_L8(""),
                                        _L8("servicename"),
                                        _L8("protocol"),
                                        3456,
                                        *iResultArray));
    EUNIT_ASSERT(err == KErrArgument);
    }

void UT_CNATFWUNSAFServerResolver::ResolveL2TestL()
    {
    TRAPD(err, iServerResolver->ResolveL(_L8("hostname"),
                                         _L8(""),
                                         _L8("protocol"),
                                         3456,
                                         *iResultArray));
    EUNIT_ASSERT(err == KErrArgument);
    }

void UT_CNATFWUNSAFServerResolver::ResolveL3TestL()
    {
    TRAPD(err, iServerResolver->ResolveL(_L8("hostname"),
                                         _L8("servicename"),
                                         _L8(""),
                                         3456,
                                         *iResultArray));
    EUNIT_ASSERT(err == KErrArgument);
    }

void UT_CNATFWUNSAFServerResolver::ResolveLTestL()
    {
    iServerResolver->ResolveL(_L8("ns.tester.testlab.net"),
                              _L8("stun"),
                              _L8("udp"),
                              3478,
                              *iResultArray);

    TRAPD(err, iServerResolver->ResolveL(_L8("ns.tester.testlab.net"),
                                         _L8("stun"),
                                         _L8("udp"),
                                         3478,
                                         *iResultArray));
    if (err == KErrNoMemory)
        {
        User::Leave(err);
        }

    EUNIT_ASSERT(err == KErrNone);


    TRAPD(err2, iServerResolver->ResolveL(_L8("1.2.3.4"),
                                         _L8("stun"),
                                         _L8("udp"),
                                         3478,
                                         *iResultArray));
    if (err2 == KErrNoMemory)
        {
        User::Leave(err2);
        }

    EUNIT_ASSERT(err2 == KErrNone);
    }


void UT_CNATFWUNSAFServerResolver::ChooseQueryLTestL()
    {
    iServerResolver->ChooseQueryL(_L8("ns.tester.testlab.net"),
                                  _L8("stun"),
                                  _L8("udp"),
                                  3478,
                                  *iResultArray);
    }

void UT_CNATFWUNSAFServerResolver::SetSRVQueryLTestL()
    {
    CNATFWUNSAFServerQuery* query =
            iServerResolver->CreateQueryLC(_L8("ns.tester.testlab.net"),
                                           _L8("stun"),
                                           _L8("udp"),
                                           3478,
                                           *iResultArray);
    iServerResolver->SetSRVQueryL(query);
    CleanupStack::Pop(query);
    }

void UT_CNATFWUNSAFServerResolver::CancelResolvingTestL()
    {
    iServerResolver->ResolveL(_L8("ns.tester.testlab.net"),
                              _L8("stun"),
                              _L8("udp"),
                              3478,
                              *iResultArray);
    iServerResolver->CancelResolving();
    }

void UT_CNATFWUNSAFServerResolver::CreateQueryLCTestL()
    {
    CNATFWUNSAFServerQuery* query =
            iServerResolver->CreateQueryLC(_L8("ns.tester.testlab.net"),
                                          _L8("stun"),
                                          _L8("udp"),
                                          3478,
                                          *iResultArray);
    CleanupStack::PopAndDestroy(query);

    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFServerResolver,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewLTestL - test ",
    "CNATFWUNSAFServerResolver",
    "NewLTestL",
    "FUNCTIONALITY",
    SetupL, NewLTestL, Teardown)

EUNIT_TEST(
    "ResolveLTestL - test ",
    "CNATFWUNSAFServerResolver",
    "ResolveLTestL",
    "FUNCTIONALITY",
    SetupL, ResolveLTestL, Teardown)

EUNIT_TEST(
    "ResolveL1TestL - test ",
    "CNATFWUNSAFServerResolver",
    "ResolveL1TestL",
    "FUNCTIONALITY",
    SetupL, ResolveL1TestL, Teardown)

EUNIT_TEST(
    "ResolveL2TestL - test ",
    "CNATFWUNSAFServerResolver",
    "ResolveL2TestL",
    "FUNCTIONALITY",
    SetupL, ResolveL2TestL, Teardown)

EUNIT_TEST(
    "ResolveL3TestL - test ",
    "CNATFWUNSAFServerResolver",
    "ResolveL3TestL",
    "FUNCTIONALITY",
    SetupL, ResolveL3TestL, Teardown)

EUNIT_TEST(
    "ChooseQueryLTestL - test ",
    "CNATFWUNSAFServerResolver",
    "ChooseQueryLTestL",
    "FUNCTIONALITY",
    SetupL, ChooseQueryLTestL, Teardown)

EUNIT_TEST(
    "SetSRVQueryLTestL - test ",
    "CNATFWUNSAFServerResolver",
    "SetSRVQueryLTestL",
    "FUNCTIONALITY",
    SetupL, SetSRVQueryLTestL, Teardown)

EUNIT_TEST(
    "CancelResolvingTestL - test ",
    "CNATFWUNSAFServerResolver",
    "CancelResolvingTestL",
    "FUNCTIONALITY",
    SetupL, CancelResolvingTestL, Teardown)

EUNIT_TEST(
    "CreateQueryLCTestL - test ",
    "CNATFWUNSAFServerResolver",
    "CreateQueryLCTestL",
    "FUNCTIONALITY",
    SetupL, CreateQueryLCTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE

