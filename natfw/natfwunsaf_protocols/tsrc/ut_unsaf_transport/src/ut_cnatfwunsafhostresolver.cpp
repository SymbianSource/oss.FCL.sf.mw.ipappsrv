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
#include "ut_cnatfwunsafhostresolver.h"

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

// CONSTRUCTION
UT_CNATFWUNSAFHostResolver* UT_CNATFWUNSAFHostResolver::NewL()
    {
    UT_CNATFWUNSAFHostResolver* self = UT_CNATFWUNSAFHostResolver::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFHostResolver* UT_CNATFWUNSAFHostResolver::NewLC()
    {
    UT_CNATFWUNSAFHostResolver* self = new( ELeave ) UT_CNATFWUNSAFHostResolver();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFHostResolver::~UT_CNATFWUNSAFHostResolver()
    {
    }

// Default constructor
UT_CNATFWUNSAFHostResolver::UT_CNATFWUNSAFHostResolver()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFHostResolver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFHostResolver::SetupL()
    {
    iResultArray = new (ELeave) RArray<TInetAddr>;
    User::LeaveIfError(iServer.Connect(KESockDefaultMessageSlots));
    User::LeaveIfError(iConnection.Open(iServer, KConnectionTypeDefault));
    TCommDbConnPref prefs;
    prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
    prefs.SetDirection(ECommDbConnectionDirectionOutgoing);
    prefs.SetIapId(10);
    User::LeaveIfError(iConnection.Start(prefs));

    iHostResolver = CNATFWUNSAFHostResolver::NewL(iServer, iConnection);

    iObserver  = UT_CNATFWUNSAFServerResolverObserverStub::NewL();
    iObserver2 = UT_CNATFWUNSAFServerResolverObserverStub::NewL();
    CNATFWUNSAFQueryData* query = CNATFWUNSAFQueryData::NewL(
                               iObserver,
                               5060,
                               _L8("udp"),
                               _L8("testi.net"),
                               _L8("stun"),
                               *iHostResolver,
                               *iResultArray );
    CleanupStack::PushL(query);
    CNATFWUNSAFQueryData* query2 = CNATFWUNSAFQueryData::NewL(
                               iObserver2,
                               5060,
                               _L8("udp"),
                               _L8("testi.net"),
                               _L8("stun"),
                               *iHostResolver,
                               *iResultArray );
    CleanupStack::PushL(query2);

    iServerQuery2 = CNATFWUNSAFServerQuery::NewL( iObserver2, query2 );
    CleanupStack::Pop(query2);
    iServerQuery = CNATFWUNSAFServerQuery::NewL( iObserver, query );
    CleanupStack::Pop(query);
    iQueryCondition = CNATFWUNSAFA_AAAAOrigDomain::NewL( *iServerQuery );
    iServerQuery->SetQueryConditionL (iQueryCondition);
    iQueryCondition    = NULL;
    CNATFWUNSAFA_AAAAOrigDomain* que =
        CNATFWUNSAFA_AAAAOrigDomain::NewL( *iServerQuery2 );
    CleanupStack::PushL(que);
    iServerQuery2->SetQueryConditionL (que);
    CleanupStack::Pop(que);
    que = NULL;
    }

void UT_CNATFWUNSAFHostResolver::Teardown()
    {
    delete iHostResolver;iHostResolver = NULL;
     delete iQueryCondition;iQueryCondition = NULL;
     delete iServerQuery;iServerQuery = NULL;
     delete iServerQuery2;iServerQuery2 = NULL;
     if ( iResultArray )
        {
        iResultArray->Reset();
        delete iResultArray;
        iResultArray = NULL;
        }


     delete iObserver;iObserver = NULL;
     delete iObserver2;iObserver2 = NULL;

    iConnection.Close();
    iServer.Close();
    }

void UT_CNATFWUNSAFHostResolver::NewLTestL ()
    {
    Teardown();
    SetupL();
    }

void UT_CNATFWUNSAFHostResolver::GetByQueryTestL ()
    {
    CNATFWUNSAFServerQuery* query = iServerQuery;
    iServerQuery = NULL;
    CleanupStack::PushL(query);
    iHostResolver->GetByQueryL(query);
    CleanupStack::Pop(query);
    CNATFWUNSAFServerQuery* query2 = iServerQuery2;
    iServerQuery2 = NULL;
    CleanupStack::PushL(query2);
    iHostResolver->GetByQueryL(query2);
    CleanupStack::Pop(query2);
    iHostResolver->CancelResolving(iObserver);
    iHostResolver->CancelResolving(iObserver2);

    Teardown();
    SetupL();
    query = iServerQuery;
    iServerQuery = NULL;
    CleanupStack::PushL(query);
    iHostResolver->iQuery = query;
    iServerQuery = NULL;
    query2 = iServerQuery2;
    iServerQuery2 = NULL;
    CleanupStack::PushL(query2);
    iHostResolver->GetByQueryL(query2);
    CleanupStack::Pop(query2);
    CleanupStack::Pop(query);
    iHostResolver->CancelResolving(iObserver2);
    delete iHostResolver->iQuery;
    iHostResolver->iQuery = NULL;
    iHostResolver->CancelResolving(iObserver2);
    }

void UT_CNATFWUNSAFHostResolver::RunErrorTestL ()
    {
    iHostResolver->SetIPListFailed(ETrue);
    iHostResolver->SetIPListFailed(EFalse);
    iHostResolver->GetByQueryL(iServerQuery);
    iServerQuery = NULL;
    iHostResolver->RunError(KErrArgument);
    EUNIT_ASSERT(iObserver->iError == KErrArgument);
    EUNIT_ASSERT(iHostResolver->iQuery == NULL);
    iHostResolver->RunError(KErrNoMemory);
    }

void UT_CNATFWUNSAFHostResolver::CancelGetByURITestL ()
    {
    iHostResolver->GetByQueryL(iServerQuery);
    iServerQuery = NULL;
    iHostResolver->GetByQueryL(iServerQuery2);
    iServerQuery2 = NULL;

    iHostResolver->CancelResolving(iObserver2);
    iHostResolver->CancelResolving(iObserver);
    }

void UT_CNATFWUNSAFHostResolver::RunLTestL ()
    {
    iHostResolver->GetByQueryL(iServerQuery);
    iServerQuery = NULL;
    iHostResolver->GetByQueryL(iServerQuery2);
    iServerQuery2 = NULL;
    iHostResolver->iStatus = KErrArgument;
    iHostResolver->RunL();
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFHostResolver,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewLTestL - test ",
    "CNATFWUNSAFHostResolver",
    "NewLTestL",
    "FUNCTIONALITY",
    SetupL, NewLTestL, Teardown)

EUNIT_TEST(
    "GetByQueryTestL - test ",
    "CNATFWUNSAFHostResolver",
    "GetByQueryTestL",
    "FUNCTIONALITY",
    SetupL, GetByQueryTestL, Teardown)

EUNIT_TEST(
    "RunErrorTestL - test ",
    "CNATFWUNSAFHostResolver",
    "RunErrorTestL",
    "FUNCTIONALITY",
    SetupL, RunErrorTestL, Teardown)

EUNIT_TEST(
    "CancelGetByURITestL - test ",
    "CNATFWUNSAFHostResolver",
    "CancelGetByURITestL",
    "FUNCTIONALITY",
    SetupL, CancelGetByURITestL, Teardown)

EUNIT_TEST(
    "RunLTestL - test ",
    "CNATFWUNSAFHostResolver",
    "RunLTestL",
    "FUNCTIONALITY",
    SetupL, RunLTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
