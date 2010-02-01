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
#include "UT_CNATFWTurnSettings.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include <unsafprotocolscrkeys.h>

//  INTERNAL INCLUDES
#include "cnatfwturnsettings.h"
#include "cnatfwserversettings.h"
#include "settingstore.h"
#include "cnatfwcenrephandler.h"
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
UT_CNATFWTurnSettings* UT_CNATFWTurnSettings::NewL()
    {
    UT_CNATFWTurnSettings* self = UT_CNATFWTurnSettings::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNATFWTurnSettings* UT_CNATFWTurnSettings::NewLC()
    {
    UT_CNATFWTurnSettings* self = new( ELeave ) UT_CNATFWTurnSettings();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNATFWTurnSettings::~UT_CNATFWTurnSettings()
    {
    }

// Default constructor
UT_CNATFWTurnSettings::UT_CNATFWTurnSettings()
    {
    }

// Second phase construct
void UT_CNATFWTurnSettings::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
//  METHODS


void UT_CNATFWTurnSettings::SetupL(  )
    {
    iCNATFWTurnSettings = CNATFWTurnSettings::NewL();
    }


void UT_CNATFWTurnSettings::Teardown(  )
    {
    delete iCNATFWTurnSettings; 
    iCNATFWTurnSettings = NULL; 
    }


void UT_CNATFWTurnSettings::SetupCenRepL(  )
    {
    SetupL();
    
    iSettingsStore = CSettingStore::NewL( KDomain, KIapId );
    iSettingsStore->EraseL( 0, 0 );
    
    iRepHandler = CNATFWCenRepHandler::NewL( KCRUidUNSAFProtocols );

    //Create Domain
    iDomainKey = iSettingsStore->CreateNewDomainL( KDomain );
    
    // turn settings
  	iSettingsStore->iTurnSettings->
  	    SetRetransmissionTimeout( KRto );
  	iSettingsStore->iTurnSettings->
  	    SetLatestConnectedServerAddrL( KLatestConnAddress );
  	iSettingsStore->iTurnSettings->SetLatestConnectedServerPort(
  	    KLatestConnPort );
  	
  	// turn servers
  	CArrayPtr<CNATFWServerSettings>* turnServers = new ( ELeave )
  	    CArrayPtrFlat<CNATFWServerSettings>( KDefaultGranularity );
  	
  	CleanupStack::PushL( turnServers );
    
    turnServers->AppendL( CNATFWServerSettings::NewL( KAddress1,
        KPort1, KUsername1, KPassword1, KUseSharedSecret ) );
    
  	turnServers->AppendL( CNATFWServerSettings::NewL( KAddress2,
        KPort2, KUsername2, KPassword2, KUseSharedSecret ) );
       
    turnServers->AppendL( CNATFWServerSettings::NewL( KAddress2AndPort,
        KPort3, KUsername2, KPassword2, KUseSharedSecret ) );
        
    CleanupStack::Pop( turnServers );
        
    // ownership transferred
    iSettingsStore->iTurnSettings->SetTurnServersL( turnServers );
    
    // additional general setting
    iSettingsStore->iUseSharedSecret = KUseSharedSecret; 
    
    CleanupStack::PushL( iSettingsStore );
    iSettingsStore->StoreSettingsL();
    CleanupStack::Pop( iSettingsStore );
    }


void UT_CNATFWTurnSettings::TeardownCenRepL(  )
    {
    Teardown();
    iSettingsStore->EraseL( 0, 0 );
    delete iSettingsStore;
    iSettingsStore = NULL;
    delete iRepHandler;
    iRepHandler = NULL;
    }


void UT_CNATFWTurnSettings::T_CNATFWTurnSettings_GetTurnServerArrayLL(  )
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
        iCNATFWTurnSettings->SetTurnServersL( serverArray ) );

    CleanupStack::Pop( server2 );
    CleanupStack::Pop( server );
    CleanupStack::Pop( serverArray );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWTurnSettings->GetTurnServerArrayL( serverArray2 ) );
    
    EUNIT_ASSERT_EQUALS( serverArray2.Count(), 2 );

    EUNIT_ASSERT_EQUALS( serverArray2[1]->Address(), KAddress1() );
    EUNIT_ASSERT_EQUALS( serverArray2[1]->Port(), KPort1 );
    EUNIT_ASSERT_EQUALS( serverArray2[1]->Username(), KUsername1() );
    EUNIT_ASSERT_EQUALS( serverArray2[1]->Password(), KPassword1() );
    EUNIT_ASSERT_EQUALS( serverArray2[1]->SharedSecretEnabled(),
        KUseSharedSecret );
    
    CleanupStack::PopAndDestroy( &serverArray2 );
    }


void UT_CNATFWTurnSettings::T_CNATFWStunSettings_GetTurnServerArrayL2L(  )
    {
    RPointerArray<MNATFWServerSettings> serverArray;
    CleanupClosePushL( serverArray );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWTurnSettings->ReadSettingsL( iDomainKey, *iRepHandler ) );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWTurnSettings->GetTurnServerArrayL( serverArray ) );
    
    EUNIT_ASSERT_EQUALS( serverArray.Count(), 3 );
    
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
            
    CleanupStack::PopAndDestroy( &serverArray );
    }


void UT_CNATFWTurnSettings::T_CNATFWTurnSettings_RetransmissionTimeoutL(  )
    {
    iCNATFWTurnSettings->SetRetransmissionTimeout( KRto );
    EUNIT_ASSERT_EQUALS( iCNATFWTurnSettings->RetransmissionTimeout(), KRto );
    }


void UT_CNATFWTurnSettings::T_CNATFWTurnSettings_LatestConnectedServerAddrL(  )
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE( iCNATFWTurnSettings->SetLatestConnectedServerAddrL(
        KLatestConnAddress ) );
    EUNIT_ASSERT_EQUALS( iCNATFWTurnSettings->LatestConnectedServerAddr(),
        KLatestConnAddress() );
    }


void UT_CNATFWTurnSettings::T_CNATFWTurnSettings_LatestConnectedServerPortL(  )
    {
    iCNATFWTurnSettings->SetLatestConnectedServerPort( KLatestConnPort );
    EUNIT_ASSERT_EQUALS( iCNATFWTurnSettings->LatestConnectedServerPort(),
        KLatestConnPort );
    }


void UT_CNATFWTurnSettings::T_CNATFWTurnSettings_ReadSettingsL()
    {
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iCNATFWTurnSettings->ReadSettingsL( iDomainKey, *iRepHandler ) );
    
    EUNIT_ASSERT_EQUALS( iCNATFWTurnSettings->RetransmissionTimeout(),
        KRto );
    EUNIT_ASSERT_EQUALS( iCNATFWTurnSettings->LatestConnectedServerAddr(),
        KLatestConnAddress() );
    EUNIT_ASSERT_EQUALS( iCNATFWTurnSettings->LatestConnectedServerPort(),
        KLatestConnPort );
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNATFWTurnSettings,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "GetTurnServerArrayL - test1",
    "CNATFWTurnSettings",
    "GetTurnServerArrayL - test1",
    "FUNCTIONALITY",
    SetupL, T_CNATFWTurnSettings_GetTurnServerArrayLL, Teardown)
    
EUNIT_TEST(
    "GetTurnServerArrayL2 - test2",
    "CNATFWTurnSettings",
    "GetTurnServerArrayL2 - test2",
    "FUNCTIONALITY",
    SetupCenRepL, T_CNATFWStunSettings_GetTurnServerArrayL2L, TeardownCenRepL)

EUNIT_TEST(
    "RetransmissionTimeout - test3",
    "CNATFWTurnSettings",
    "RetransmissionTimeout - test3",
    "FUNCTIONALITY",
    SetupL, T_CNATFWTurnSettings_RetransmissionTimeoutL, Teardown)
    
EUNIT_TEST(
    "LatestConnectedServerAddr - test4",
    "CNATFWTurnSettings",
    "LatestConnectedServerAddr - test4",
    "FUNCTIONALITY",
    SetupL, T_CNATFWTurnSettings_LatestConnectedServerAddrL, Teardown)
    
EUNIT_TEST(
    "LatestConnectedServerPort - test5",
    "CNATFWTurnSettings",
    "LatestConnectedServerPort - test5",
    "FUNCTIONALITY",
    SetupL, T_CNATFWTurnSettings_LatestConnectedServerPortL, Teardown)

EUNIT_TEST(
    "ReadSettingsL - test6",
    "CNATFWTurnSettings",
    "ReadSettingsL - test6",
    "FUNCTIONALITY",
    SetupCenRepL, T_CNATFWTurnSettings_ReadSettingsL, TeardownCenRepL )
        
EUNIT_END_TEST_TABLE

//  END OF FILE
