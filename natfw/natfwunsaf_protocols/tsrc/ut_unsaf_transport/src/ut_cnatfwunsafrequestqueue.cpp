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
#include "ut_cnatfwunsafrequestqueue.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <es_sock.h>
#include <e32def.h>
#include <CommDbConnPref.h>

//  INTERNAL INCLUDES
#include "ut_cnatfwunsafserverresolverobserverstub.h"
#include "cnatfwunsafrequestqueue.h"
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
UT_CNATFWUNSAFRequestQueue* UT_CNATFWUNSAFRequestQueue::NewL()
    {
    UT_CNATFWUNSAFRequestQueue* self = UT_CNATFWUNSAFRequestQueue::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFRequestQueue* UT_CNATFWUNSAFRequestQueue::NewLC()
    {
    UT_CNATFWUNSAFRequestQueue* self = new( ELeave ) UT_CNATFWUNSAFRequestQueue();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFRequestQueue::~UT_CNATFWUNSAFRequestQueue()
    {
    }

// Default constructor
UT_CNATFWUNSAFRequestQueue::UT_CNATFWUNSAFRequestQueue()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFRequestQueue::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFRequestQueue::SetupL()
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
    CleanupStack::Pop( query );
    iRequestQueue = CNATFWUNSAFRequestQueue::NewL();
    }

void UT_CNATFWUNSAFRequestQueue::Teardown()
    {
    delete iRequestQueue;
    iRequestQueue = NULL;
    delete iHostResolver;
    iHostResolver = NULL;
    if ( iResultArray )
        {
        iResultArray->Reset();
        delete iResultArray;
        iResultArray = NULL;
        }

     delete iServerQuery;
     iServerQuery = NULL;
     delete iObserver;
     iObserver = NULL;
    iConnection.Close();
    iServer.Close();
    }

void UT_CNATFWUNSAFRequestQueue::NewLTestL ()
    {
    CNATFWUNSAFRequestQueue* que = CNATFWUNSAFRequestQueue::NewL();
    delete que;
    que = NULL;
    }

void UT_CNATFWUNSAFRequestQueue::CancelQueryTestL ()
    {
    CNATFWUNSAFServerQuery* query = iServerQuery;
    iServerQuery = NULL;
    CleanupStack::PushL(query);
    iRequestQueue->Add(*query);
    CleanupStack::Pop( query );
    iRequestQueue->CancelQuery(iObserver);
    UT_CNATFWUNSAFServerResolverObserverStub* observer =
                    UT_CNATFWUNSAFServerResolverObserverStub::NewL();
    iRequestQueue->CancelQuery(observer);
    delete observer;
    observer = NULL;
    }

void UT_CNATFWUNSAFRequestQueue::AddLTestL ()
    {
    CNATFWUNSAFServerQuery* query = iServerQuery;
    iServerQuery = NULL;
    CleanupStack::PushL(query);
    iRequestQueue->Add(*query);
    CleanupStack::Pop( query );
    CNATFWUNSAFQueryData* querydata = CNATFWUNSAFQueryData::NewL(
                               iObserver,
                               5060,
                               _L8("udp"),
                               _L8("testi.net"),
                               _L8("stun"),
                               *iHostResolver,
                               *iResultArray);
    CleanupStack::PushL(querydata);
    CNATFWUNSAFServerQuery* serverQuery =
        CNATFWUNSAFServerQuery::NewL(iObserver, querydata);
    CleanupStack::Pop( querydata );
    CleanupStack::PushL(serverQuery);
    TRAPD(error, iRequestQueue->Add(*serverQuery));
    if(error != KErrNone)
        {
        User::Leave(error);
        }
    CleanupStack::Pop(serverQuery);
    }

void UT_CNATFWUNSAFRequestQueue::NextQueryTestL ()
    {
    CNATFWUNSAFServerQuery* query = iServerQuery;
    iServerQuery = NULL;
    CleanupStack::PushL(query);
    iRequestQueue->Add(*query);
    CleanupStack::Pop( query );
    CNATFWUNSAFServerQuery* query2 = iRequestQueue->NextQuery();
    CleanupStack::PushL(query2);
    EUNIT_ASSERT(query == query2);
    CleanupStack::PopAndDestroy( query2 );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFRequestQueue,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewLTestL - test ",
    "CNATFWUNSAFRequestQueue",
    "NewLTestL",
    "FUNCTIONALITY",
    SetupL, NewLTestL, Teardown)

EUNIT_TEST(
    "CancelQueryTestL - test ",
    "CNATFWUNSAFRequestQueue",
    "CancelQueryTestL",
    "FUNCTIONALITY",
    SetupL, CancelQueryTestL, Teardown)

EUNIT_TEST(
    "AddLTestL - test ",
    "CNATFWUNSAFRequestQueue",
    "AddLTestL",
    "FUNCTIONALITY",
    SetupL, AddLTestL, Teardown)

EUNIT_TEST(
    "NextQueryTestL - test ",
    "CNATFWUNSAFRequestQueue",
    "NextQueryTestL",
    "FUNCTIONALITY",
    SetupL, NextQueryTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
