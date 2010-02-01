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
#include "ut_cnatfwunsafserverquery.h"

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
#include "cnatfwunsafqueryconditionbase.h"
#include "cnatfwunsafsrvorigdomain.h"
#include "cnatfwunsafquerybase.h"
#include "ut_cnatfwunsafqueryconditionstub.h"

// CONSTRUCTION
UT_CNATFWUNSAFServerQuery* UT_CNATFWUNSAFServerQuery::NewL()
    {
    UT_CNATFWUNSAFServerQuery* self = UT_CNATFWUNSAFServerQuery::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFServerQuery* UT_CNATFWUNSAFServerQuery::NewLC()
    {
    UT_CNATFWUNSAFServerQuery* self = new( ELeave ) UT_CNATFWUNSAFServerQuery();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFServerQuery::~UT_CNATFWUNSAFServerQuery()
    {
    }

// Default constructor
UT_CNATFWUNSAFServerQuery::UT_CNATFWUNSAFServerQuery()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFServerQuery::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFServerQuery::SetupL()
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

    iObserver = UT_CNATFWUNSAFServerResolverObserverStub::NewL();
    CNATFWUNSAFQueryData* query = CNATFWUNSAFQueryData::NewL(
                               iObserver,
                               5060,
                               _L8("udp"),
                               _L8("testi.net"),
                               _L8("stun"),
                               *iHostResolver,
                               *iResultArray );
    CleanupStack::PushL(query);
    iServerQuery = CNATFWUNSAFServerQuery::NewL( iObserver, query );
    CleanupStack::Pop( query );
    }

void UT_CNATFWUNSAFServerQuery::Teardown()
    {
    delete iHostResolver;
    iHostResolver = NULL;
     delete iQueryCondition;
     iQueryCondition = NULL;
     delete iServerQuery;
     iServerQuery = NULL;
     delete iObserver;
     iObserver = NULL;
      if ( iResultArray )
        {
        iResultArray->Reset();
        delete iResultArray;
        iResultArray = NULL;
        }

    iConnection.Close();
    iServer.Close();
    }

void UT_CNATFWUNSAFServerQuery::NewLTestL()
    {
    Teardown();
    SetupL();
    }

void UT_CNATFWUNSAFServerQuery::QueryTestL()
    {
    iQueryCondition = UT_CNATFWUNSAFQueryConditionStub::NewL(*iServerQuery);
    CNATFWUNSAFQueryConditionBase* condition = iQueryCondition;
    iQueryCondition = NULL;
    CleanupStack::PushL(condition);
    iServerQuery->SetQueryConditionL(condition);
    CleanupStack::Pop( condition );//condition
    iServerQuery->Query(*iHostResolver);
    }

void UT_CNATFWUNSAFServerQuery::HandleQueryResultLTestL()
    {
    CNATFWUNSAFQueryConditionBase* condition =
                        UT_CNATFWUNSAFQueryConditionStub::NewL(*iServerQuery);
    CleanupStack::PushL(condition);
    iServerQuery->SetQueryConditionL(condition);
    CleanupStack::Pop( condition );
    iServerQuery->Query(*iHostResolver);
    EUNIT_ASSERT(!iServerQuery->HandleQueryResultL(KErrArgument));

    condition = UT_CNATFWUNSAFQueryConditionStub::NewL(*iServerQuery);
    CleanupStack::PushL(condition);
    iServerQuery->SetQueryConditionL(condition);
    CleanupStack::Pop( condition );
    iServerQuery->Query(*iHostResolver);
    EUNIT_ASSERT(iServerQuery->HandleQueryResultL(KErrCancel));

    condition = UT_CNATFWUNSAFQueryConditionStub::NewL(*iServerQuery);
    CleanupStack::PushL(condition);
    iServerQuery->SetQueryConditionL(condition);
    CleanupStack::Pop( condition );
    iServerQuery->Query(*iHostResolver);
    EUNIT_ASSERT(iServerQuery->HandleQueryResultL(KErrNone));

    }

void UT_CNATFWUNSAFServerQuery::ServerResolverObserverTestL()
    {
    EUNIT_ASSERT(iServerQuery->ServerResolverObserver() == iObserver);
    }

void UT_CNATFWUNSAFServerQuery::QueryDataTestL()
    {
    iServerQuery->QueryData();
    }

void UT_CNATFWUNSAFServerQuery::SetQueryConditionLTestL()
    {
    CNATFWUNSAFQueryConditionBase* condition =
                        UT_CNATFWUNSAFQueryConditionStub::NewL(*iServerQuery);
    CleanupStack::PushL(condition);
    iServerQuery->SetQueryConditionL(condition);
    CleanupStack::Pop( condition );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFServerQuery,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewLTestL - test ",
    "CNATFWUNSAFServerQuery",
    "NewLTestL",
    "FUNCTIONALITY",
    SetupL, NewLTestL, Teardown)

EUNIT_TEST(
    "QueryTestL - test ",
    "CNATFWUNSAFServerQuery",
    "QueryTestL",
    "FUNCTIONALITY",
    SetupL, QueryTestL, Teardown)

EUNIT_TEST(
    "HandleQueryResultLTestL - test ",
    "CNATFWUNSAFServerQuery",
    "HandleQueryResultLTestL",
    "FUNCTIONALITY",
    SetupL, HandleQueryResultLTestL, Teardown)

EUNIT_TEST(
    "ServerResolverObserverTestL - test ",
    "CNATFWUNSAFServerQuery",
    "ServerResolverObserverTestL",
    "FUNCTIONALITY",
    SetupL, ServerResolverObserverTestL, Teardown)

EUNIT_TEST(
    "QueryDataTestL - test ",
    "CNATFWUNSAFServerQuery",
    "QueryDataTestL",
    "FUNCTIONALITY",
    SetupL, QueryDataTestL, Teardown)

EUNIT_TEST(
    "SetQueryConditionLTestL - test ",
    "CNATFWUNSAFServerQuery",
    "SetQueryConditionLTestL",
    "FUNCTIONALITY",
    SetupL, SetQueryConditionLTestL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
