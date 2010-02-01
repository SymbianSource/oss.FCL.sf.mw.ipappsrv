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
#include "ut_cnatfwunsafquerydata.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <es_sock.h>
#include <e32def.h>
#include <CommDbConnPref.h>

//  INTERNAL INCLUDES
#include "ut_cnatfwunsafserverresolverobserverstub.h"
#include "cnatfwunsafquerydata.h"
#include "natfwunsafserverresolverobserver.h"
#include "mnatfwunsafhostresolver.h"
#include "natfwunsafserverresolver.h"
#include "cnatfwunsafhostresolver.h"
#include "ut_cnatfwunsafquerya_aaaastub.h"

// CONSTRUCTION
UT_CNATFWUNSAFQueryData* UT_CNATFWUNSAFQueryData::NewL()
    {
    UT_CNATFWUNSAFQueryData* self = UT_CNATFWUNSAFQueryData::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFQueryData* UT_CNATFWUNSAFQueryData::NewLC()
    {
    UT_CNATFWUNSAFQueryData* self = new( ELeave ) UT_CNATFWUNSAFQueryData();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFQueryData::~UT_CNATFWUNSAFQueryData()
    {
    }

// Default constructor
UT_CNATFWUNSAFQueryData::UT_CNATFWUNSAFQueryData()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFQueryData::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS
void UT_CNATFWUNSAFQueryData::SetupL()
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

    iProtocol = HBufC8::NewL(3);
    TPtr8 ptr = iProtocol->Des();
    ptr.Append(_L8("udp"));

    iServiceName = HBufC8::NewL(4);
    ptr = iServiceName->Des();
    ptr.Append(_L8("stun"));
    iPort = 3478;
    iQuery = CNATFWUNSAFQueryData::NewL(
                               iObserver,
                               iPort,
                               *iProtocol,
                               _L8("testi.net"),
                               *iServiceName,
                               *iHostResolver,
                               *iResultArray );
    }

void UT_CNATFWUNSAFQueryData::Teardown()
    {
    delete iHostResolver;
    iHostResolver = NULL;
    delete iProtocol;
    iProtocol = NULL;
    delete iServiceName;
    iServiceName = NULL;
    if ( iResultArray )
        {
        iResultArray->Reset();
        delete iResultArray;
        iResultArray = NULL;
        }
     delete iQuery;
     iQuery = NULL;
     delete iObserver;
     iObserver = NULL;
    iConnection.Close();
    iServer.Close();
    }


void UT_CNATFWUNSAFQueryData::QueryDataNewLTestL ()
    {
    this->Teardown();
    this->SetupL();
    EUNIT_ASSERT (iQuery->DefaultPort() == iPort);
    EUNIT_ASSERT(iQuery->Protocol().CompareF(*iProtocol) == 0);
    EUNIT_ASSERT(iQuery->ServiceName().CompareF(*iServiceName) == 0);
    EUNIT_ASSERT (iQuery->Target().Compare(_L8("testi.net")) == 0);
    EUNIT_ASSERT (iQuery->OriginalTarget().Compare(_L8("testi.net")) == 0);
    EUNIT_ASSERT (iQuery->Observer() == iObserver);
    EUNIT_ASSERT (iQuery->ResultArray().Count() == 0);
    iQuery->Resolver();
    delete iQuery;
    iQuery = NULL;
    }

void UT_CNATFWUNSAFQueryData::SetTestL ()
    {
    EUNIT_ASSERT (iQuery->Port() == 0);
    iQuery->SetPort(2004);
    EUNIT_ASSERT (iQuery->Port() == 2004);
    EUNIT_ASSERT (iQuery->Protocol().CompareF(*iProtocol) == 0);
    iQuery->SetProtocolL(_L8("tcp") );
    EUNIT_ASSERT (iQuery->Protocol().CompareF(_L8("tcp")) == 0);
    EUNIT_ASSERT (iQuery->Target().Compare( _L8( "testi.net" ) ) == 0);
    iQuery->SetTargetL( _L8( "new.net" ) );
    EUNIT_ASSERT (iQuery->Target().Compare( _L8( "new.net" ) ) == 0);
    EUNIT_ASSERT (iQuery->OriginalTarget().Compare( _L8( "testi.net" ) ) == 0);
    iQuery->SetOriginalTargetL( _L8( "old.net" ) );
    EUNIT_ASSERT ( iQuery->OriginalTarget().Compare(_L8( "old.net" )) == 0 );
    delete iQuery;
    iQuery = NULL;
    }

void UT_CNATFWUNSAFQueryData::CreateResultLTestL()
    {
    UT_CNATFWUNSAFQueryA_AAAAStub* aaaaQuery =
                    UT_CNATFWUNSAFQueryA_AAAAStub::NewL(_L8("testi.net"));
    CleanupStack::PushL(aaaaQuery);
    iQuery->iQuery = aaaaQuery;
    EUNIT_ASSERT(iQuery->AddResultToArrayL());
    CleanupStack::PopAndDestroy();//aaaaQuery
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFQueryData,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "QueryDataNewLTestL - test ",
    "CNATFWUNSAFQueryData",
    "QueryDataNewLTestL",
    "FUNCTIONALITY",
    SetupL, QueryDataNewLTestL, Teardown)

EUNIT_TEST(
    "SetTestL - test ",
    "CNATFWUNSAFQueryData",
    "SetTestL",
    "FUNCTIONALITY",
    SetupL, SetTestL, Teardown)

EUNIT_TEST(
    "CreateResultLTestL - test ",
    "CNATFWUNSAFQueryData",
    "CreateResultLTestL",
    "FUNCTIONALITY",
    SetupL, CreateResultLTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
