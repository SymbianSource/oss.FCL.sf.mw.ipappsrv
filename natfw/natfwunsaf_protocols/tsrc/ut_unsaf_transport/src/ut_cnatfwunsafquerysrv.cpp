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
#include "ut_cnatfwunsafquerysrv.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "cnatfwunsafquerysrv.h"

// CONSTRUCTION
UT_CNATFWUNSAFQuerySrv* UT_CNATFWUNSAFQuerySrv::NewL()
    {
    UT_CNATFWUNSAFQuerySrv* self = UT_CNATFWUNSAFQuerySrv::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWUNSAFQuerySrv* UT_CNATFWUNSAFQuerySrv::NewLC()
    {
    UT_CNATFWUNSAFQuerySrv* self = new( ELeave ) UT_CNATFWUNSAFQuerySrv();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWUNSAFQuerySrv::~UT_CNATFWUNSAFQuerySrv()
    {
    }

// Default constructor
UT_CNATFWUNSAFQuerySrv::UT_CNATFWUNSAFQuerySrv()
    {
    }

// Second phase construct
void UT_CNATFWUNSAFQuerySrv::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWUNSAFQuerySrv::SetupL()
    {
    iQuery = CNATFWUNSAFQuerySrv::NewL(_L8("test.test.net"));
    iQuery->RecordArray();
    }

void UT_CNATFWUNSAFQuerySrv::Teardown()
    {
     delete iQuery;
     iQuery = NULL;
    }

void UT_CNATFWUNSAFQuerySrv::QuerySrvNewLTestL ()
    {
    delete iQuery;
    iQuery = NULL,
    iQuery = CNATFWUNSAFQuerySrv::NewL(_L8("testi.com"));
    EUNIT_ASSERT(iQuery->iTarget->Compare(_L8("testi.com")) == 0);

    delete iQuery;
    iQuery = NULL;

    TRAPD(err, iQuery = CNATFWUNSAFQuerySrv::NewL(_L8("")));
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
    iQuery = CNATFWUNSAFQuerySrv::NewL(_L8("testi.com"));
    }

void UT_CNATFWUNSAFQuerySrv::QueryResultBufTestL ()
    {
    //No query made
    iQuery->QueryResultBuf();
    TDnsRespSRV srv(iQuery->QueryResult());
    EUNIT_ASSERT(srv.Priority() == 0);
    EUNIT_ASSERT(srv.Weight() == 0);

    TRAPD( err, iQuery->SRVRecordL( 100 ) );
    if ( err != KErrArgument )
        {
        User::Leave( err );
        }
    }

void UT_CNATFWUNSAFQuerySrv::SetTargetProtocolLTestL ()
    {
    iQuery->iTarget->Compare(_L8("testi.com"));
    HBufC8* temp = CNATFWUNSAFQuerySrv::SetTargetProtocolL(
        _L8("new.net"), _L8("tcp"), _L8("stun"));
    CleanupStack::PushL(temp);
    EUNIT_ASSERT(temp->Compare(_L8("_stun._tcp.new.net")) == 0);
    CleanupStack::PopAndDestroy();//temp
    temp = CNATFWUNSAFQuerySrv::SetTargetProtocolL(
        _L8("new.net"), _L8("udp"), _L8("stun"));
    CleanupStack::PushL(temp);
    EUNIT_ASSERT(temp->Compare(_L8("_stun._udp.new.net")) == 0);
    iQuery->SetTargetL(*temp);
    EUNIT_ASSERT(iQuery->iTarget->Compare(_L8("_stun._udp.new.net")) == 0);
    CleanupStack::PopAndDestroy();//temp
    }

void UT_CNATFWUNSAFQuerySrv::AddLTestL ()
    {
    TDnsRespSRV srv;
    TUint16 prior = 20;
    TUint16 weig  = 11;
    TUint16 port  = 2030;
    HBufC8* target = _L8("testinet.net").AllocLC();

    srv.SetPriority(prior);
    srv.SetWeight(weig);
    srv.SetPort(port);
    srv.SetTarget(*target);
    CleanupStack::PopAndDestroy();//target
    //array: srv
    iQuery->AddL(srv);

    TDnsRespSRV result(iQuery->SRVRecordL(0));
    EUNIT_ASSERT(result.Priority() == prior);
    EUNIT_ASSERT(result.Weight() == weig);
    EUNIT_ASSERT(result.Port() == port);
    EUNIT_ASSERT(result.Target().Compare(_L8("testinet.net")) == 0);

    //add second which should be put before first one
    //the priority has lower value and so it has a higher priority order
    TDnsRespSRV srv1;
    TUint16 prior1 = 10;
    TUint16 weig1  = 10;
    TUint16 port1  = 2040;
    HBufC8* target1 = _L8("testinet1.net").AllocLC();

    srv1.SetPriority(prior1);
    srv1.SetWeight(weig1);
    srv1.SetPort(port1);
    srv1.SetTarget(*target1);
    CleanupStack::PopAndDestroy();//target1
    //array: srv1,srv
    iQuery->AddL(srv1);

    TDnsRespSRV result1(iQuery->SRVRecordL(0));
    EUNIT_ASSERT(result1.Priority() == prior1);
    EUNIT_ASSERT(result1.Weight() == weig1);
    EUNIT_ASSERT(result1.Port() == port1);
    EUNIT_ASSERT(result1.Target().Compare(_L8("testinet1.net")) == 0);

    TDnsRespSRV result2(iQuery->SRVRecordL(1));
    EUNIT_ASSERT(result2.Priority() == prior);
    EUNIT_ASSERT(result2.Weight() == weig);
    EUNIT_ASSERT(result2.Port() == port);
    EUNIT_ASSERT(result2.Target().Compare(_L8("testinet.net")) == 0);

    //add third which should be inserted at last, priority is same as in first
    //inserted case (now in place 1 in array)
    //but weight is lower so it has minor priority
    TDnsRespSRV srv3;
    TUint16 prior3 = 20;
    TUint16 weig3  = 9;
    TUint16 port3  = 2030;
    HBufC8* target3 = _L8("testinet3.net").AllocLC();

    srv3.SetPriority(prior3);
    srv3.SetWeight(weig3);
    srv3.SetPort(port3);
    srv3.SetTarget(*target3);
    CleanupStack::PopAndDestroy();//target3
    //array: srv1,srv,srv3
    iQuery->AddL(srv3);

    EUNIT_ASSERT(iQuery->SRVRecordL(0).Priority() == prior1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Weight() == weig1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Port() == port1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Target().Compare(_L8("testinet1.net")) == 0);

    EUNIT_ASSERT(iQuery->SRVRecordL(1).Priority() == prior);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Weight() == weig);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Port() == port);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Target().Compare(_L8("testinet.net")) == 0);

    EUNIT_ASSERT(iQuery->SRVRecordL(2).Priority() == prior3);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Weight() == weig3);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Port() == port3);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Target().Compare(_L8("testinet3.net")) == 0);

    //priority higher, put in place 1
    TDnsRespSRV srv2;
    TUint16 prior2 = 11;
    TUint16 weig2  = 100;
    TUint16 port2  = 2050;
    HBufC8* target2 = _L8("testinet2.net").AllocLC();

    srv2.SetPriority(prior2);
    srv2.SetWeight(weig2);
    srv2.SetPort(port2);
    srv2.SetTarget(*target2);
    CleanupStack::PopAndDestroy();//target2
    //array: srv1,srv2,srv,srv3
    iQuery->AddL(srv2);

    EUNIT_ASSERT(iQuery->SRVRecordL(0).Priority() == prior1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Weight() == weig1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Port() == port1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Target().Compare(_L8("testinet1.net")) == 0);

    EUNIT_ASSERT(iQuery->SRVRecordL(1).Priority() == prior2);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Weight() == weig2);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Port() == port2);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Target().Compare(_L8("testinet2.net")) == 0);


    EUNIT_ASSERT(iQuery->SRVRecordL(2).Priority() == prior);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Weight() == weig);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Port() == port);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Target().Compare(_L8("testinet.net")) == 0);

    EUNIT_ASSERT(iQuery->SRVRecordL(3).Priority() == prior3);
    EUNIT_ASSERT(iQuery->SRVRecordL(3).Weight() == weig3);
    EUNIT_ASSERT(iQuery->SRVRecordL(3).Port() == port3);
    EUNIT_ASSERT(iQuery->SRVRecordL(3).Target().Compare(_L8("testinet3.net")) == 0);

    //same priority
    TDnsRespSRV srv4;
    TUint16 prior4 = 20;
    TUint16 weig4  = 10;
    TUint16 port4  = 2030;
    HBufC8* target4 = _L8("testinet4.net").AllocLC();

    srv4.SetPriority(prior4);
    srv4.SetWeight(weig4);
    srv4.SetPort(port4);
    srv4.SetTarget(*target4);
    CleanupStack::PopAndDestroy();//target4
    //array: srv1,srv2,srv,srv4,srv3
    iQuery->AddL(srv4);

    EUNIT_ASSERT(iQuery->SRVRecordL(0).Priority() == prior1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Weight() == weig1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Port() == port1);
    EUNIT_ASSERT(iQuery->SRVRecordL(0).Target().Compare(_L8("testinet1.net")) == 0);

    EUNIT_ASSERT(iQuery->SRVRecordL(1).Priority() == prior2);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Weight() == weig2);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Port() == port2);
    EUNIT_ASSERT(iQuery->SRVRecordL(1).Target().Compare(_L8("testinet2.net")) == 0);

    EUNIT_ASSERT(iQuery->SRVRecordL(2).Priority() == prior);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Weight() == weig);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Port() == port);
    EUNIT_ASSERT(iQuery->SRVRecordL(2).Target().Compare(_L8("testinet.net")) == 0);

    EUNIT_ASSERT(iQuery->SRVRecordL(3).Priority() == prior3);
    EUNIT_ASSERT(iQuery->SRVRecordL(3).Weight() == weig3);
    EUNIT_ASSERT(iQuery->SRVRecordL(3).Port() == port3);
    EUNIT_ASSERT(iQuery->SRVRecordL(3).Target().Compare(_L8("testinet3.net")) == 0);

    EUNIT_ASSERT(iQuery->SRVRecordL(4).Priority() == prior4);
    EUNIT_ASSERT(iQuery->SRVRecordL(4).Weight() == weig4);
    EUNIT_ASSERT(iQuery->SRVRecordL(4).Port() == port4);
    EUNIT_ASSERT(iQuery->SRVRecordL(4).Target().Compare(_L8("testinet4.net")) == 0);

    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWUNSAFQuerySrv,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "QuerySrvNewLTestL - test ",
    "CNATFWUNSAFQuerySrv",
    "QuerySrvNewLTestL",
    "FUNCTIONALITY",
    SetupL, QuerySrvNewLTestL, Teardown)

EUNIT_TEST(
    "QueryResultBufTestL - test ",
    "CNATFWUNSAFQuerySrv",
    "QueryResultBufTestL",
    "FUNCTIONALITY",
    SetupL, QueryResultBufTestL, Teardown)

EUNIT_TEST(
    "SetTargetProtocolLTestL - test ",
    "CNATFWUNSAFQuerySrv",
    "SetTargetProtocolLTestL",
    "FUNCTIONALITY",
    SetupL, SetTargetProtocolLTestL, Teardown)

EUNIT_TEST(
    "AddLTestL - test ",
    "CNATFWUNSAFQuerySrv",
    "AddLTestL",
    "FUNCTIONALITY",
    SetupL, AddLTestL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
