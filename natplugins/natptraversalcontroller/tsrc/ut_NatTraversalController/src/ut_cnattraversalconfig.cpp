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
#include "ut_cnattraversalconfig.h"

//  INTERNAL INCLUDES
#include "CNATTraversalConfig.h"
#include <unsafprotocolscrkeys.h>

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <in_sock.h>


const TUint32 KIapId = 1;
_LIT8( KDomain, "mydomain.ok" );


UT_CNATTraversalConfig* UT_CNATTraversalConfig::NewL()
    {
    UT_CNATTraversalConfig* self = UT_CNATTraversalConfig::NewLC();
    CleanupStack::Pop();
    return self;
    }

UT_CNATTraversalConfig* UT_CNATTraversalConfig::NewLC()
    {
    UT_CNATTraversalConfig* self = new ( ELeave ) UT_CNATTraversalConfig();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

UT_CNATTraversalConfig::~UT_CNATTraversalConfig()
    {
    }

UT_CNATTraversalConfig::UT_CNATTraversalConfig()
    {
    }

void UT_CNATTraversalConfig::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

void UT_CNATTraversalConfig::SetupL()
    {
    iConfig = CNATTraversalConfig::NewL( KIapId, KDomain );
    iCR = CRepository::NewL(KCRUidUNSAFProtocols);
	RArray<TUint32> keys;
    CleanupClosePushL(keys);
    TInt err = iCR->FindEqL( KUNSAFProtocolsDomainMask,
                             KUNSAFProtocolsFieldTypeMask,
                             KDomain,
                             keys );
    if ( err == KErrNotFound )
        {
        iConfig->SetSharedSecretNotSupportedL(EFalse);
        err = iCR->FindEqL( KUNSAFProtocolsDomainMask,
                                     KUNSAFProtocolsFieldTypeMask,
                                     KDomain,
                                     keys );
        }
    if ( err == KErrNone && keys.Count() == 1 )
        {
        iDomainKey = ( KUNSAFProtocolsDomainMask ^ keys[0] );
        iDomainKey |= KUNSAFProtocolsDomainTableMask;        
        }
    else
    	{    	
    	User::Leave( err );
    	}

	CleanupStack::PopAndDestroy(&keys);
    }


void UT_CNATTraversalConfig::Teardown()
    {
	iCR->Delete( KUNSAFProtocolsDomainEnableCRLFRefresh | iDomainKey );

    delete iCR;
    iCR = NULL;
    delete iConfig;
    iConfig = NULL;
    iDomainKey = 0;
    }

// Test functions

void UT_CNATTraversalConfig::TestCRLFRefreshEnabledToProxyL()
	{
	iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EUndefined;
	EUNIT_ASSERT( CNATTraversalConfig::EEnabled ==
				  iConfig->CRLFRefreshEnabledToProxy( KProtocolInetTcp ) );
	EUNIT_ASSERT( CNATTraversalConfig::EDisabled ==
				  iConfig->CRLFRefreshEnabledToProxy( KProtocolInetUdp ) );

	iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EDisabled;
	EUNIT_ASSERT( CNATTraversalConfig::EDisabled ==
				  iConfig->CRLFRefreshEnabledToProxy( KProtocolInetTcp ) );
	EUNIT_ASSERT( CNATTraversalConfig::EDisabled ==
				  iConfig->CRLFRefreshEnabledToProxy( KProtocolInetUdp ) );

	iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EEnabled;
	EUNIT_ASSERT( CNATTraversalConfig::EEnabled ==
				  iConfig->CRLFRefreshEnabledToProxy( KProtocolInetTcp ) );
	EUNIT_ASSERT( CNATTraversalConfig::EEnabled ==
				  iConfig->CRLFRefreshEnabledToProxy( KProtocolInetUdp ) );
	}


void UT_CNATTraversalConfig::TestCRLFRefreshEnabledToProxyValueL()
	{
	iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EUndefined;
	EUNIT_ASSERT( iConfig->iCRLFRefreshEnabled ==
				  iConfig->CRLFRefreshEnabledToProxyValue() );
					   
	iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EDisabled;
	EUNIT_ASSERT( iConfig->iCRLFRefreshEnabled ==
				  iConfig->CRLFRefreshEnabledToProxyValue() );

	iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EEnabled;
	EUNIT_ASSERT( iConfig->iCRLFRefreshEnabled ==
				  iConfig->CRLFRefreshEnabledToProxyValue() );
	}


void UT_CNATTraversalConfig::TestReadCRLFRefreshEnabledL()
    {
	// Remove value from repository
	TUint32 crlfKey = KUNSAFProtocolsDomainEnableCRLFRefresh | iDomainKey;
	TInt err = iCR->Delete( crlfKey );
	if ( err != KErrNone && err != KErrNotFound )
		{
		User::Leave( err );
		}
	iConfig->iCRLFRefreshEnabled = CNATTraversalConfig::EUndefined;

	
	iConfig->ReadCRLFRefreshEnabled( *iCR, iDomainKey );
	EUNIT_ASSERT( CNATTraversalConfig::EUndefined ==
				  iConfig->CRLFRefreshEnabledToProxyValue() );


	// Set with unknown value (treated as enabled)
	iCR->Set( crlfKey, 800 );
	iConfig->ReadCRLFRefreshEnabled( *iCR, iDomainKey );
	EUNIT_ASSERT( CNATTraversalConfig::EEnabled ==
				  iConfig->CRLFRefreshEnabledToProxyValue() );
				  
				  
	// Set disabled
	iCR->Set( crlfKey, CNATTraversalConfig::EDisabled );
	iConfig->ReadCRLFRefreshEnabled( *iCR, iDomainKey );
	EUNIT_ASSERT( CNATTraversalConfig::EDisabled ==
				  iConfig->CRLFRefreshEnabledToProxyValue() );


	// Set enabled
	iCR->Set( crlfKey, CNATTraversalConfig::EEnabled );
	iConfig->ReadCRLFRefreshEnabled( *iCR, iDomainKey );
	EUNIT_ASSERT( CNATTraversalConfig::EEnabled ==
				  iConfig->CRLFRefreshEnabledToProxyValue() );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNATTraversalConfig,
    "Test suite for class CNATTraversalConfig",
    "UNIT" )

EUNIT_TEST(
    "CRLFRefreshEnabledToProxy - test ",
    "UT_CNATTraversalConfig",
    "CRLFRefreshEnabledToProxy",
    "FUNCTIONALITY",
    SetupL, TestCRLFRefreshEnabledToProxyL, Teardown)

EUNIT_TEST(
    "CRLFRefreshEnabledToProxyValue - test ",
    "UT_CNATTraversalConfig",
    "CRLFRefreshEnabledToProxyValue",
    "FUNCTIONALITY",
    SetupL, TestCRLFRefreshEnabledToProxyValueL, Teardown)

EUNIT_TEST(
    "ReadCRLFRefreshEnabled - test ",
    "UT_CNATTraversalConfig",
    "ReadCRLFRefreshEnabled",
    "FUNCTIONALITY",
    SetupL, TestReadCRLFRefreshEnabledL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
