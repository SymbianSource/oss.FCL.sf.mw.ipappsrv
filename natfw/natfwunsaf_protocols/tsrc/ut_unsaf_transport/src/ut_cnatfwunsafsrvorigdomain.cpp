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
#include "ut_cnatfwunsafsrvorigdomain.h"

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
#include "ut_cnatfwunsafquerysrvstub.h"
#include "cnatfwunsafsrvorigdomain.h"

// CONSTRUCTION
UT_CNATFWUNSAFSrvOrigDomain* UT_CNATFWUNSAFSrvOrigDomain::NewL()
    {
    UT_CNATFWUNSAFSrvOrigDomain* self = UT_CNATFWUNSAFSrvOrigDomain::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFSrvOrigDomain* UT_CNATFWUNSAFSrvOrigDomain::NewLC()
    {
    UT_CNATFWUNSAFSrvOrigDomain* self = new( ELeave ) UT_CNATFWUNSAFSrvOrigDomain();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFSrvOrigDomain::~UT_CNATFWUNSAFSrvOrigDomain()
    {
    }

// Default constructor
UT_CNATFWUNSAFSrvOrigDomain::UT_CNATFWUNSAFSrvOrigDomain()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFSrvOrigDomain::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFSrvOrigDomain::SetupL()
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
                               *iResultArray);
    CleanupStack::PushL(query);
    iServerQuery = CNATFWUNSAFServerQuery::NewL(iObserver, query);
    CleanupStack::Pop(query);
    iQueryCondition = CNATFWUNSAFSRVOrigDomain::NewL( *iServerQuery, EFalse );
    }

void UT_CNATFWUNSAFSrvOrigDomain::Teardown()
    {
    delete iHostResolver;iHostResolver = NULL;
     delete iQueryCondition;iQueryCondition = NULL;
     delete iServerQuery;iServerQuery = NULL;
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

void UT_CNATFWUNSAFSrvOrigDomain::QueryDataNewLTestL ()
    {
    delete iQueryCondition;
    iQueryCondition = NULL;
    iQueryCondition = CNATFWUNSAFSRVOrigDomain::NewL( *iServerQuery, EFalse );
    iQueryCondition->QueryBase();
    }

void UT_CNATFWUNSAFSrvOrigDomain::HandleQueryResultLTestL ()
    {
    delete iQueryCondition->iQueryBase;
    iQueryCondition->iQueryBase = NULL;
    iQueryCondition->iQueryBase =
                    UT_CNATFWUNSAFQuerySrvStub::NewL(_L8("caseSrv.net"));
    iQueryCondition->iQueryBase->Query(iServerQuery->QueryData().Resolver());
    iQueryCondition->iQueryBase->HandleQueryResultL(
                                        iServerQuery->QueryData().Resolver());
    UT_CNATFWUNSAFQuerySrvStub* srvQuery =
                static_cast<UT_CNATFWUNSAFQuerySrvStub*>
                    (iQueryCondition->iQueryBase);
    EUNIT_ASSERT(srvQuery->SRVRecordL( 0 ).Target().Compare(
        _L8("SrvSecond.net")) == 0);

    CNATFWUNSAFQueryConditionBase* nextcondition =
                        iQueryCondition->HandleQueryResultL(KErrArgument);
    CleanupStack::PushL(nextcondition);
    EUNIT_ASSERT(nextcondition != NULL);
    EUNIT_ASSERT(iServerQuery->QueryData().Port() == 5060);
    EUNIT_ASSERT(iServerQuery->QueryData().Target().Compare(
        _L8("testi.net")) == 0);
    CleanupStack::PopAndDestroy(nextcondition);

    nextcondition = iQueryCondition->HandleQueryResultL(KErrNone);
    CleanupStack::PushL(nextcondition);
    EUNIT_ASSERT(nextcondition != NULL);
    EUNIT_ASSERT(iServerQuery->QueryData().Port() == 5093);
    EUNIT_ASSERT(iServerQuery->QueryData().Target().Compare(
        _L8("SrvSecond.net")) == 0);
    CleanupStack::PopAndDestroy(nextcondition);

    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFSrvOrigDomain,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "QueryDataNewLTestL - test ",
    "CNATFWUNSAFSrvOrigDomain",
    "QueryDataNewLTestL",
    "FUNCTIONALITY",
    SetupL, QueryDataNewLTestL, Teardown)

EUNIT_TEST(
    "HandleQueryResultLTestL - test ",
    "CNATFWUNSAFSrvOrigDomain",
    "HandleQueryResultLTestL",
    "FUNCTIONALITY",
    SetupL, HandleQueryResultLTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
