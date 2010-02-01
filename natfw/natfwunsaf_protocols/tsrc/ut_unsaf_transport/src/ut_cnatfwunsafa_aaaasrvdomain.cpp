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
#include "ut_cnatfwunsafa_aaaasrvdomain.h"

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
#include "cnatfwunsafa_aaaasrvdomain.h"
#include "cnatfwunsafquerysrv.h"
#include "ut_cnatfwunsafquerysrvstub.h"

//  CONSTANTS
_LIT8(KUdp8, "udp");
//_LIT8(KTcp8, "tcp");
_LIT8(KServiceName8, "stun");

// CONSTRUCTION
UT_CNATFWUNSAFA_AAAASRVDomain* UT_CNATFWUNSAFA_AAAASRVDomain::NewL()
    {
    UT_CNATFWUNSAFA_AAAASRVDomain* self = UT_CNATFWUNSAFA_AAAASRVDomain::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFA_AAAASRVDomain* UT_CNATFWUNSAFA_AAAASRVDomain::NewLC()
    {
    UT_CNATFWUNSAFA_AAAASRVDomain* self = new( ELeave ) UT_CNATFWUNSAFA_AAAASRVDomain();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFA_AAAASRVDomain::~UT_CNATFWUNSAFA_AAAASRVDomain()
    {
    }

// Default constructor
UT_CNATFWUNSAFA_AAAASRVDomain::UT_CNATFWUNSAFA_AAAASRVDomain()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFA_AAAASRVDomain::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFA_AAAASRVDomain::SetupL()
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
                               3478,
                               KUdp8,
                               _L8("testi.net"),
                               KServiceName8,
                               *iHostResolver,
                               *iResultArray );
    CleanupStack::PushL(query);
    iServerQuery = CNATFWUNSAFServerQuery::NewL(iObserver, query);
    CleanupStack::Pop(query);
    CNATFWUNSAFQuerySrv* srvQuery = CNATFWUNSAFQuerySrv::NewL(
        _L8("srvtest.net"));
    CleanupStack::PushL(srvQuery);
    iQueryCondition = CNATFWUNSAFA_AAAASRVDomain::NewL(
        *iServerQuery,srvQuery);
    CleanupStack::Pop(srvQuery);
    }

void UT_CNATFWUNSAFA_AAAASRVDomain::Teardown()
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

void UT_CNATFWUNSAFA_AAAASRVDomain::QueryDataNewLTestL ()
    {
    delete iQueryCondition;
    iQueryCondition = NULL;
    CNATFWUNSAFQuerySrv* srvQuery = CNATFWUNSAFQuerySrv::NewL(
        _L8("srvtest.net"));
    CleanupStack::PushL(srvQuery);

    iQueryCondition = CNATFWUNSAFA_AAAASRVDomain::NewL(
        *iServerQuery,srvQuery );
    CleanupStack::Pop(srvQuery);
    iQueryCondition->QueryBase();
    }

void UT_CNATFWUNSAFA_AAAASRVDomain::HandleQueryResultLTestL ()
    {
    //initialize srv records
    delete iQueryCondition->iSrvQuery;
    iQueryCondition->iSrvQuery = NULL;
    iQueryCondition->iSrvQuery = UT_CNATFWUNSAFQuerySrvStub::NewL(
        _L8("caseSrv.net"));

    //no srv records, return CNATFWUNSAFA_AAAAOrigDomain*

    CNATFWUNSAFQueryConditionBase* nextcondition =
                        iQueryCondition->HandleQueryResultL(KErrArgument);
    CleanupStack::PushL(nextcondition);
    EUNIT_ASSERT(nextcondition != NULL);
    EUNIT_ASSERT(iServerQuery->QueryData().Port() == 3478);
    EUNIT_ASSERT(iServerQuery->QueryData().DefaultPort() == 3478);
    EUNIT_ASSERT(iServerQuery->QueryData().Protocol().CompareF(KUdp8) == 0);
    EUNIT_ASSERT(iServerQuery->QueryData().Target().Compare(
        _L8("testi.net")) == 0);
    CleanupStack::PopAndDestroy();//nextcondition

    //srv records , return same condition
    iQueryCondition->iSrvQuery->Query(iServerQuery->QueryData().Resolver());
    iQueryCondition->iSrvQuery->HandleQueryResultL(
                                        iServerQuery->QueryData().Resolver());

    nextcondition = iQueryCondition->HandleQueryResultL(KErrArgument);

    EUNIT_ASSERT(nextcondition != NULL);
    EUNIT_ASSERT(iServerQuery->QueryData().Port() == 5093);
    EUNIT_ASSERT(iServerQuery->QueryData().Target().Compare(
        _L8("SrvFirst.net")) == 0);
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFA_AAAASRVDomain,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "QueryDataNewLTestL - test ",
    "CNATFWUNSAFA_AAAASRVDomain",
    "QueryDataNewLTestL",
    "FUNCTIONALITY",
    SetupL, QueryDataNewLTestL, Teardown)

EUNIT_TEST(
    "HandleQueryResultLTestL - test ",
    "CNATFWUNSAFA_AAAASRVDomain",
    "HandleQueryResultLTestL",
    "FUNCTIONALITY",
    SetupL, HandleQueryResultLTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
