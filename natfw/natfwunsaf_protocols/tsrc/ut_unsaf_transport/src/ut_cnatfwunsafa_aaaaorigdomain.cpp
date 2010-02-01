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
#include "ut_cnatfwunsafa_aaaaorigdomain.h"

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
#include "ut_cnatfwunsafquerya_aaaastub.h"

// CONSTRUCTION
UT_CNATFWUNSAFA_AAAAOrigDomain* UT_CNATFWUNSAFA_AAAAOrigDomain::NewL()
    {
    UT_CNATFWUNSAFA_AAAAOrigDomain* self = UT_CNATFWUNSAFA_AAAAOrigDomain::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFA_AAAAOrigDomain* UT_CNATFWUNSAFA_AAAAOrigDomain::NewLC()
    {
    UT_CNATFWUNSAFA_AAAAOrigDomain* self = new( ELeave ) UT_CNATFWUNSAFA_AAAAOrigDomain();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFA_AAAAOrigDomain::~UT_CNATFWUNSAFA_AAAAOrigDomain()
    {
    }

// Default constructor
UT_CNATFWUNSAFA_AAAAOrigDomain::UT_CNATFWUNSAFA_AAAAOrigDomain()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFA_AAAAOrigDomain::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFA_AAAAOrigDomain::SetupL()
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
    iServerQuery = CNATFWUNSAFServerQuery::NewL(iObserver, query);
    CleanupStack::Pop( query );
    iQueryCondition = CNATFWUNSAFA_AAAAOrigDomain::NewL( *iServerQuery );
    }

void UT_CNATFWUNSAFA_AAAAOrigDomain::Teardown()
    {
    delete iHostResolver;iHostResolver = NULL;
    if ( iResultArray )
        {
        iResultArray->Reset();
        delete iResultArray;
        iResultArray = NULL;
        }

     delete iQueryCondition;iQueryCondition = NULL;
     delete iServerQuery;iServerQuery = NULL;
     delete iObserver;iObserver = NULL;
    iConnection.Close();
    iServer.Close();
    }

void UT_CNATFWUNSAFA_AAAAOrigDomain::QueryDataNewLTestL ()
    {
    delete iQueryCondition;
    iQueryCondition = NULL;
    iQueryCondition = CNATFWUNSAFA_AAAAOrigDomain::NewL( *iServerQuery );
    iQueryCondition->QueryBase();
    }

void UT_CNATFWUNSAFA_AAAAOrigDomain::HandleQueryResultLTestL ()
    {
    iQueryCondition->HandleQueryResultL(KErrArgument);

    delete iQueryCondition->iQueryBase;
    iQueryCondition->iQueryBase = NULL;
    iQueryCondition->iQueryBase =
        UT_CNATFWUNSAFQueryA_AAAAStub::NewL(_L8("testi.net"));
    iQueryCondition->iQueryBase->Query(*iHostResolver);
    iServerQuery->QueryData().iQuery =
              static_cast<CNATFWUNSAFQueryA_AAAA*>
              (iQueryCondition->iQueryBase);
    iServerQuery->QueryData().AddResultToArrayL();
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFA_AAAAOrigDomain,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "QueryDataNewLTestL - test ",
    "CNATFWUNSAFA_AAAAOrigDomain",
    "QueryDataNewLTestL",
    "FUNCTIONALITY",
    SetupL, QueryDataNewLTestL, Teardown)

EUNIT_TEST(
    "HandleQueryResultLTestL - test ",
    "CNATFWUNSAFA_AAAAOrigDomain",
    "HandleQueryResultLTestL",
    "FUNCTIONALITY",
    SetupL, HandleQueryResultLTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
