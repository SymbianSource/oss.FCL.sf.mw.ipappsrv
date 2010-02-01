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
#include "ut_cncmlocaladdressresolver.h"
#include "ut_testconstants.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <CommDbConnPref.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "cncmlocaladdressresolver.h"


// CONSTRUCTION
UT_CNcmLocalAddressResolver* UT_CNcmLocalAddressResolver::NewL()
    {
    UT_CNcmLocalAddressResolver* self = UT_CNcmLocalAddressResolver::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CNcmLocalAddressResolver* UT_CNcmLocalAddressResolver::NewLC()
    {
    UT_CNcmLocalAddressResolver* self = new( ELeave ) UT_CNcmLocalAddressResolver();
    CleanupStack::PushL( self );
    

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CNcmLocalAddressResolver::~UT_CNcmLocalAddressResolver()
    {
    }

// Default constructor
UT_CNcmLocalAddressResolver::UT_CNcmLocalAddressResolver()
    {
    }

// Second phase construct
void UT_CNcmLocalAddressResolver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNcmLocalAddressResolver::SetupL(  )
    {
    User::LeaveIfError( iSocketSrv.Connect() );

    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( KIapId );  
                                                 
    User::LeaveIfError( iConnection.Open( iSocketSrv ) );
    User::LeaveIfError( iConnection.Start( pref ) );
    
    iResolver = CNcmLocalAddressResolver::NewL( iSocketSrv );
    }


void UT_CNcmLocalAddressResolver::Teardown()
    {
	delete iResolver;
	
	iConnection.Close();    
    iSocketSrv.Close();
    }

void UT_CNcmLocalAddressResolver::UT_CNcmLocalAddressResolver_MatchL()
    {
    TSoInetInterfaceInfo info;
    TSoInetIfQuery query;    
    
    // Address unspecified
    info.iAddress.SetAddress( INET_ADDR( 0,0,0,0 ) );
    query.iZone[1] = KIapId;
    EUNIT_ASSERT( !iResolver->Match( info, query, KIapId ) );
    
    // Address loopback
    info.iAddress.SetAddress( INET_ADDR( 127,0,0,1 ) );
    EUNIT_ASSERT( !iResolver->Match( info, query, KIapId ) );

    // Address link local
    info.iAddress.SetAddress( INET_ADDR( 169,254,98,65 ) );
    EUNIT_ASSERT( !iResolver->Match( info, query, KIapId ) );    
    
    // IAP does not match 
    info.iAddress.SetAddress( INET_ADDR( 192,169,0,0 ) );
    query.iZone[1] = KIapId+1;
    EUNIT_ASSERT( !iResolver->Match( info, query, KIapId ) );    
    
    // Match
    query.iZone[1] = KIapId;
    EUNIT_ASSERT( iResolver->Match( info, query, KIapId ) );
    }
    
void UT_CNcmLocalAddressResolver::UT_CNcmLocalAddressResolver_GetLocalAddrLL()
    {

    
    }
    
    
void UT_CNcmLocalAddressResolver::UT_CNcmLocalAddressResolver_CheckAndSetAddrLL()
    {
    TInetAddr tempAddr;
    
    TSoInetInterfaceInfo info;
    
    TSoInetIfQuery query;    
    query.iZone[1] = KIapId;
    // IPv4
    info.iAddress.SetAddress( INET_ADDR( 1,2,3,4 ) );
    iResolver->CheckAndSetAddr( info, query, KIapId );  
    EUNIT_ASSERT( iResolver->iIPv6Addr.IsUnspecified() );
    EUNIT_ASSERT( iResolver->iIPv4Addr.CmpAddr( info.iAddress ) ); 


    // IPv6, unspecified before
    delete iResolver;
    iResolver = NULL;
    
    iResolver = CNcmLocalAddressResolver::NewL( iSocketSrv );
    info.iAddress.Input( _L("2001:490:ff2:1002:0:dead:cafe:fe" ) );
    iResolver->CheckAndSetAddr( info, query, KIapId ); 
    EUNIT_ASSERT( iResolver->iIPv6Addr.CmpAddr( info.iAddress ) );
    EUNIT_ASSERT( iResolver->iIPv4Addr.CmpAddr( KAFUnspec ) ); 
    
    // IPv4, IPv6 specified
    delete iResolver;
    iResolver = NULL;
    
    iResolver = CNcmLocalAddressResolver::NewL( iSocketSrv );
    
    tempAddr.Input( _L("2001:490:ff2:1002:0:dead:cafe:fe" ) );
    iResolver->iIPv6Addr = tempAddr;
    
    info.iAddress.SetAddress( INET_ADDR( 1,2,3,4 ) );
    
    iResolver->CheckAndSetAddr( info, query, KIapId ); 
    EUNIT_ASSERT( iResolver->iIPv6Addr.CmpAddr( tempAddr ) );
    EUNIT_ASSERT( iResolver->iIPv4Addr.CmpAddr( info.iAddress ) ); 
   

    // IPv6, IPv4 not set, IPv6 address can be converted to IPv4
    info.iAddress.Input( _L("::ffff:1.2.3.4" ) );
    iResolver->CheckAndSetAddr( info, query, KIapId );    
    EUNIT_ASSERT_EQUALS( KAfInet6, iResolver->iIPv6Addr.Family() ); 
    EUNIT_ASSERT_EQUALS( KAfInet, iResolver->iIPv4Addr.Family() );
        
    tempAddr.SetAddress( INET_ADDR(1,2,3,4) );
    EUNIT_ASSERT( iResolver->iIPv6Addr.CmpAddr( info.iAddress ) ); 
    EUNIT_ASSERT( iResolver->iIPv4Addr.CmpAddr( tempAddr ) );   
    }
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CNcmLocalAddressResolver,
    "UT_CNcmLocalAddressResolver",
    "UNIT" )

EUNIT_TEST(
    "Match - test ",
    "UT_CNcmLocalAddressResolver",
    "Match",
    "FUNCTIONALITY",
    SetupL, UT_CNcmLocalAddressResolver_MatchL, Teardown)
    
EUNIT_TEST(
    "GetLocalAddrL - test ",
    "UT_CNcmLocalAddressResolver",
    "GetLocalAddrL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmLocalAddressResolver_GetLocalAddrLL, Teardown)

EUNIT_TEST(
    "CheckAndSetAddrL - test ",
    "UT_CNcmLocalAddressResolver",
    "CheckAndSetAddrL",
    "FUNCTIONALITY",
    SetupL, UT_CNcmLocalAddressResolver_CheckAndSetAddrLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE


