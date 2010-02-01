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
#include "ut_cmcelocaladdrresolver.h"
#include "MCETestHelper.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <CommDbConnPref.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "mcelocaladdrresolver.h"

const TUint32 KIapId = 1;

// CONSTRUCTION
UT_CMceLocalAddrResolver* UT_CMceLocalAddrResolver::NewL()
    {
    UT_CMceLocalAddrResolver* self = UT_CMceLocalAddrResolver::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceLocalAddrResolver* UT_CMceLocalAddrResolver::NewLC()
    {
    UT_CMceLocalAddrResolver* self = new( ELeave ) UT_CMceLocalAddrResolver();
    CleanupStack::PushL( self );
    

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceLocalAddrResolver::~UT_CMceLocalAddrResolver()
    {
    }

// Default constructor
UT_CMceLocalAddrResolver::UT_CMceLocalAddrResolver()
    {
    }

// Second phase construct
void UT_CMceLocalAddrResolver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceLocalAddrResolver::SetupL(  )
    {
    iSocketServ.Connect();
    iResolver = CMceLocalAddrResolver::NewL( iSocketServ, KIapId );
    }


void UT_CMceLocalAddrResolver::Teardown()
    {
	delete iResolver;
	iSocketServ.Close();
    }


void UT_CMceLocalAddrResolver::UT_CMceLocalAddrResolver_MatchL()
    {
    TSoInetInterfaceInfo info;
    TSoInetIfQuery query;    
    
    // Address unspecified
    info.iAddress.SetAddress(INET_ADDR(0,0,0,0));
    query.iZone[1] = KIapId;
    EUNIT_ASSERT(!iResolver->Match(info,query));
    
    // Address loopback
    info.iAddress.SetAddress(INET_ADDR(127,0,0,1));
    EUNIT_ASSERT(!iResolver->Match(info,query));

    // Address link local
    info.iAddress.SetAddress(INET_ADDR(169,254,98,65));
    EUNIT_ASSERT(!iResolver->Match(info,query));    
    
    // IAP does not match
    info.iAddress.SetAddress(INET_ADDR(192,169,0,0));
    query.iZone[1] = KIapId+1;
    EUNIT_ASSERT(!iResolver->Match(info,query));    
    
    // Match
    query.iZone[1] = KIapId;
    EUNIT_ASSERT(iResolver->Match(info,query));
    
    }

void UT_CMceLocalAddrResolver::UT_CMceLocalAddrResolver_SetAddressesIfNotSetL()
    {
    TInetAddr addr;
    
    // IPv4
    addr.SetAddress(INET_ADDR(1,2,3,4));
    iResolver->SetAddressesIfNotSet(addr);
    EUNIT_ASSERT(iResolver->Addr().CmpAddr(addr));
    EUNIT_ASSERT(iResolver->IPv4Addr().CmpAddr(addr));
    
    // IPv4, address already set
    iResolver->SetAddressesIfNotSet(addr);
    EUNIT_ASSERT(iResolver->Addr().CmpAddr(addr));
    EUNIT_ASSERT(iResolver->IPv4Addr().CmpAddr(addr));
    
    // IPv6, unspecified before
    iResolver->iAddr.Init(KAfInet);
    iResolver->iIPv4Addr.Init(KAfInet);
    User::LeaveIfError(addr.Input(_L("2001:490:ff2:1002:0:dead:cafe:fe")));
    iResolver->SetAddressesIfNotSet(addr);
    EUNIT_ASSERT(iResolver->Addr().CmpAddr(addr));
    EUNIT_ASSERT(iResolver->IPv4Addr().IsUnspecified());
    
    // IPv6, set as IPv4 before
    iResolver->iAddr.SetAddress(INET_ADDR(1,2,3,4));    
    iResolver->SetAddressesIfNotSet(addr);
    EUNIT_ASSERT(iResolver->Addr().CmpAddr(addr));
    EUNIT_ASSERT(iResolver->IPv4Addr().IsUnspecified());    
    
    // IPv6, IPv4 not set, IPv6 address can be converted to IPv4
    iResolver->iAddr.Init(KAfInet);
    iResolver->iIPv4Addr.Init(KAfInet);
    User::LeaveIfError(addr.Input(_L("::ffff:1.2.3.4")));    
    iResolver->SetAddressesIfNotSet(addr);
    EUNIT_ASSERT(iResolver->Addr().CmpAddr(addr));
    EUNIT_ASSERT_EQUALS(KAfInet6, iResolver->Addr().Family());
    EUNIT_ASSERT_EQUALS(KAfInet, iResolver->IPv4Addr().Family());
    addr.SetAddress(INET_ADDR(1,2,3,4));
    EUNIT_ASSERT(iResolver->IPv4Addr().CmpAddr(addr));    
    }
    

void UT_CMceLocalAddrResolver::UT_CMceLocalAddrResolver_RefreshLocalAddressesLL()
    {
    iResolver->RefreshLocalAddressesL();
    MCE_CHECK_MEMORY_LEAVE( iResolver->Addr().IsUnspecified() )
    EUNIT_ASSERT(!iResolver->Addr().IsUnspecified());
    }
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceLocalAddrResolver,
    "UT_CMceLocalAddrResolver",
    "UNIT" )

EUNIT_TEST(
    "Match - test ",
    "CMceLocalAddrResolver",
    "Match",
    "FUNCTIONALITY",
    SetupL, UT_CMceLocalAddrResolver_MatchL, Teardown)

EUNIT_TEST(
    "SetAddressesIfNotSet - test ",
    "CMceLocalAddrResolver",
    "SetAddressesIfNotSet",
    "FUNCTIONALITY",
    SetupL, UT_CMceLocalAddrResolver_SetAddressesIfNotSetL, Teardown)

EUNIT_TEST(
    "RefreshLocalAddressesL - test ",
    "CMceLocalAddrResolver",
    "RefreshLocalAddressesL",
    "FUNCTIONALITY",
    SetupL, UT_CMceLocalAddrResolver_RefreshLocalAddressesLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE


