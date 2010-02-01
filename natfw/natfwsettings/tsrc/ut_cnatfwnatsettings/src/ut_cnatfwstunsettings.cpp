/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "UT_CNATFWStunSettings.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <unsafprotocolscrkeys.h>

//  INTERNAL INCLUDES
#include "cnatfwstunsettings.h"
#include "cnatfwserversettings.h"
#include "cnatfwcenrephandler.h"
#include "settingstore.h" 
#include "natfwdefaultsettingsvalues.h"
#include "natfwunittestmacros.h"

_LIT8( KDomain,"www.domain.fi" );
const TUint KIapId = 1979;

_LIT8( KAddress1, "10.30.0.1" );
_LIT8( KAddress2, "200.255.255.100" );
_LIT8( KAddress2AndPort, "200.255.255.100:3333" );
const TInt KPort1 = 55;
const TInt KPort2 = 255;
const TInt KPort3 = 3;
const TInt KAddress2Port = 3333;
_LIT8( KUsername1, "username1" );
_LIT8( KPassword1, "password1" );
_LIT8( KUsername2, "username2" );
_LIT8( KPassword2, "password2" );
const TBool KUseSharedSecret = ETrue; 
const TInt KRto = 500;
_LIT8( KLatestConnAddress, "100.100.100.100" );
const TUint KLatestConnPort = 1000;

const TUint KDefaultGranularity = 1;

// CONSTRUCTION
UT_CNATFWStunSettings* UT_CNATFWStunSettings::NewL()
    {
    UT_CNATFWStunSettings* self = UT_CNATFWStunSettings::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWStunSettings* UT_CNATFWStunSettings::NewLC()
    {
    UT_CNATFWStunSettings* self = new( ELeave ) UT_CNATFWStunSettings();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWStunSettings::~UT_CNATFWStunSettings()
    {
    }

// Default constructor
UT_CNATFWStunSettings::UT_CNATFWStunSettings()
    {
    }

// Second phase construct
void UT_CNATFWStunSettings::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CNATFWStunSettings::SetupL(  )
    {
    iCNATFWStunSettings = CNATFWStunSettings::NewL();
    }


void UT_CNATFWStunSettings::Teardown(  )
    {
    delete iCNATFWStunSettings;
    iCNATFWStunSettings = NULL;
    }


void UT_CNATFWStunSettings::SetupCenRepL(  )
    {
    SetupL();
    
    iSettingsStore = CSettingStore::NewL( KDomain, KIapId );
    iSettingsStore->EraseL( 0, 0 );
    
    iRepHandler = CNATFWCenRepHandler::NewL( KCRUidUNSAFProtocols );

    //Create Domain
  	iDomainKey = iSettingsStore->CreateNewDomainL( KDomain );
    
    // Stun settings
  	iSettingsStore->iStunSettings->
  	    SetRetransmissionTimeout( KRto );
  	iSettingsStore->iStunSettings->
  	    SetLatestConnectedServerAddrL( KLatestConnAddress );
  	iSettingsStore->iStunSettings->SetLatestConnectedServerPort(
  	    KLatestConnPort );
  	
  	// Stun servers
  	CArrayPtr<CNATFWServerSettings>* stunServers = new ( ELeave )
  	    CArrayPtrFlat<CNATFWServerSettings>( KDefaultGranularity );
  	
  	CleanupStack::PushL( stunServers );

  	stunServers->AppendL( CNATFWServerSettings::NewL( KAddress1,
        KPort1, KUsername1, KPassword1, KUseSharedSecret ) );

    stunServers->AppendL( CNATFWServerSettings::NewL( KAddress2,
        KPort2, KUsername2, KPassword2, KUseSharedSecret ) );
    
    stunServers->AppendL( CNATFWServerSettings::NewL( KAddress2AndPort,
        KPort3, KUsername2, KPassword2, KUseSharedSecret ) );
    
    stunServers->AppendL( CNATFWServerSettings::NewL( KAddress1,
            0, KNullDesC8, KNullDesC8, 0 ) );
    
    CleanupStack::Pop( stunServers );
    
    // ownership transferred
    iSettingsStore->iStunSettings->SetStunServersL( stunServers );
    // additional general setting
    iSettingsStore->iUseSharedSecret = KUseSharedSecret;  
      
    CleanupStack::PushL( iSettingsStore );
    iSettingsStore->StoreSettingsL();
    CleanupStack::Pop( iSettingsStore );
    }


void UT_CNATFWStunSettings::TeardownCenRepL(  )
    {
    Teardown();
    iSettingsStore->EraseL( 0, 0 );
    delete iSettingsStore;
    iSettingsStore = NULL;
    delete iRepHandler;
    iRepHandler = NULL;
    }


void UT_CNATFWStunSettings::T_CNATFWStunSettings_GetStunServerArrayLL(  )
    {
    RPointerArray<MNATFWServerSettings> serverArray2;
    CleanupClosePushL( serverArray2 );
    
    CArrayPtr<CNATFWServerSettings>* serverArray = new ( ELeave )
        CArrayPtrFlat<CNATFWServerSettings>( KDefaultGranularity );
    CleanupStack::PushL( serverArray );

    CNATFWServerSettings* server = CNATFWServerSettings::NewLC( KAddress1,
        KPort1, KUsername1, KPassword1, KUseSharedSecret );
     
    CNATFWServerSettings* server2 = CNATFWServerSettings::NewLC( KAddress2,
        KPort2, KUsername2, KPassword2, KUseSharedSecret );

    serverArray->AppendL( server2 );
    
    serverArray->AppendL( server );
   
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWStunSettings->SetStunServersL( serverArray ) );
    
    CleanupStack::Pop( server2 );
    CleanupStack::Pop( server );
    CleanupStack::Pop( serverArray );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWStunSettings->GetStunServerArrayL( serverArray2 ) );
    
    EUNIT_ASSERT_EQUALS( serverArray2.Count(), 2 );

    EUNIT_ASSERT_EQUALS( serverArray2[1]->Address(), KAddress1() );
    EUNIT_ASSERT_EQUALS( serverArray2[1]->Port(), KPort1 );
    EUNIT_ASSERT_EQUALS( serverArray2[1]->Username(), KUsername1() );
    EUNIT_ASSERT_EQUALS( serverArray2[1]->Password(), KPassword1() );
    EUNIT_ASSERT_EQUALS( serverArray2[1]->SharedSecretEnabled(),
        KUseSharedSecret );
    
    CleanupStack::PopAndDestroy( &serverArray2 );
    }


void UT_CNATFWStunSettings::T_CNATFWStunSettings_GetStunServerArrayL2L(  )
    {
    RPointerArray<MNATFWServerSettings> serverArray;
    CleanupClosePushL( serverArray );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWStunSettings->ReadSettingsL( iDomainKey, *iRepHandler ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWStunSettings->GetStunServerArrayL( serverArray ) );
    
    EUNIT_ASSERT_EQUALS( serverArray.Count(), 4 );
    
    EUNIT_ASSERT_EQUALS( serverArray[1]->Address(), KAddress2() );
    EUNIT_ASSERT_EQUALS( serverArray[1]->Port(), KPort2 );
    EUNIT_ASSERT_EQUALS( serverArray[1]->Username(), KUsername2() );
    EUNIT_ASSERT_EQUALS( serverArray[1]->Password(), KPassword2() );
    EUNIT_ASSERT_EQUALS( serverArray[1]->SharedSecretEnabled(),
        KUseSharedSecret );
    
    EUNIT_ASSERT_EQUALS( serverArray[2]->Address(), KAddress2() );
    EUNIT_ASSERT_EQUALS( serverArray[2]->Port(), KAddress2Port );
    EUNIT_ASSERT_EQUALS( serverArray[2]->Username(), KUsername2() );
    EUNIT_ASSERT_EQUALS( serverArray[2]->Password(), KPassword2() );
    EUNIT_ASSERT_EQUALS( serverArray[2]->SharedSecretEnabled(),
        KUseSharedSecret );
    
    EUNIT_ASSERT_EQUALS( serverArray[3]->Address(), KAddress1() );
    EUNIT_ASSERT_EQUALS( serverArray[3]->Port(), KNATFWDefaultSTUNServerPort );
    EUNIT_ASSERT_EQUALS( serverArray[3]->Username(), KNullDesC8() );
    EUNIT_ASSERT_EQUALS( serverArray[3]->Password(), KNullDesC8() );
    
    CleanupStack::PopAndDestroy( &serverArray );
    }


void UT_CNATFWStunSettings::T_CNATFWStunSettings_RetransmissionTimeoutL(  )
    {
    iCNATFWStunSettings->SetRetransmissionTimeout( KRto );
    EUNIT_ASSERT_EQUALS( iCNATFWStunSettings->RetransmissionTimeout(), KRto );
    }


void UT_CNATFWStunSettings::T_CNATFWStunSettings_LatestConnectedServerAddrL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWStunSettings->SetLatestConnectedServerAddrL(
        KLatestConnAddress ) );
    EUNIT_ASSERT_EQUALS( iCNATFWStunSettings->LatestConnectedServerAddr(),
        KLatestConnAddress() );
    }


void UT_CNATFWStunSettings::T_CNATFWStunSettings_LatestConnectedServerPortL(  )
    {
    iCNATFWStunSettings->SetLatestConnectedServerPort( KLatestConnPort );
    EUNIT_ASSERT_EQUALS( iCNATFWStunSettings->LatestConnectedServerPort(),
        KLatestConnPort );
    }


void UT_CNATFWStunSettings::T_CNATFWStunSettings_ReadSettingsL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWStunSettings->ReadSettingsL( iDomainKey, *iRepHandler ) );
    EUNIT_ASSERT_EQUALS( iCNATFWStunSettings->RetransmissionTimeout(), KRto );
    EUNIT_ASSERT_EQUALS( iCNATFWStunSettings->LatestConnectedServerAddr(),
        KLatestConnAddress() );
    EUNIT_ASSERT_EQUALS( iCNATFWStunSettings->LatestConnectedServerPort(),
        KLatestConnPort );
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWStunSettings,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "GetStunServerArrayL - test1",
    "CNATFWStunSettings",
    "GetStunServerArrayL - test1",
    "FUNCTIONALITY",
    SetupL, T_CNATFWStunSettings_GetStunServerArrayLL, Teardown)

EUNIT_TEST(
    "GetStunServerArrayL2 - test2",
    "CNATFWStunSettings",
    "GetStunServerArrayL2 - test2",
    "FUNCTIONALITY",
    SetupCenRepL, T_CNATFWStunSettings_GetStunServerArrayL2L, TeardownCenRepL )

EUNIT_TEST(
    "RetransmissionTimeout - test3",
    "CNATFWStunSettings",
    "RetransmissionTimeout - test3",
    "FUNCTIONALITY",
    SetupL, T_CNATFWStunSettings_RetransmissionTimeoutL, Teardown)

EUNIT_TEST(
    "LatestConnectedServerAddr - test4",
    "CNATFWStunSettings",
    "LatestConnectedServerAddr - test4",
    "FUNCTIONALITY",
    SetupL, T_CNATFWStunSettings_LatestConnectedServerAddrL, Teardown)

EUNIT_TEST(
    "LatestConnectedServerPort - test5",
    "CNATFWStunSettings",
    "LatestConnectedServerPort - test5",
    "FUNCTIONALITY",
    SetupL, T_CNATFWStunSettings_LatestConnectedServerPortL, Teardown)

EUNIT_TEST(
    "ReadSettingsL - test6",
    "CNATFWStunSettings",
    "ReadSettingsL - test6",
    "FUNCTIONALITY",
    SetupCenRepL, T_CNATFWStunSettings_ReadSettingsL, TeardownCenRepL )

EUNIT_END_TEST_TABLE

//  END OF FILE
