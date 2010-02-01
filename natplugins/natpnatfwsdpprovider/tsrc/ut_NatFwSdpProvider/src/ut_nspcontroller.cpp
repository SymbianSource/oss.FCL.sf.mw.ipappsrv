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

// EXTERNAL INCLUDES
#include <sdpcodecstringpool.h>
#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>
#include <sdpdocument.h>

// INTERNAL INCLUDES
#include "nspcontroller.h"
#include "nspsession.h"
#include "nspdefs.h"
#include "ut_nspcontroller.h"
#include "nsptesthelper.h"
#include "nsptls.h"

_LIT8( KSTUN, "nokia.stun" );
_LIT8( KTURN, "nokia.turn" );
_LIT8( KICE, "nokia.ice" );



// METHODS:
// ---------------------------------------------------------------------------
// UT_CNSPController::UT_CNSPController
// ---------------------------------------------------------------------------
// 
UT_CNSPController::UT_CNSPController()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::ConstructL()
    {    
    CEUnitTestSuiteClass::ConstructL();
    
    iCenrep = CRepository::NewL( KCRUidUNSAFProtocols );
    NSPTestHelper::ClearCRepositoryL( *iCenrep );
    NSPTestHelper::CreateDomainKeyL( *iCenrep, KTestDomain() );
    
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    
    iCenrep->FindEqL( KUNSAFProtocolsDomainMask,
            KUNSAFProtocolsFieldTypeMask, KTestDomain(), keys );
    
    __ASSERT_ALWAYS( keys.Count(), User::Leave( KErrNotFound ) );
    
    iCurrentDomainKey = KUNSAFProtocolsDomainMask^keys[0];
    iCurrentDomainKey |= KUNSAFProtocolsUsedNATProtocolMask;
    
    CleanupStack::PopAndDestroy(); // keys
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::~UT_CNSPController
// ---------------------------------------------------------------------------
// 
UT_CNSPController::~UT_CNSPController()
    {
    delete iCenrep;
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPController* UT_CNSPController::NewL()
    {
    UT_CNSPController* self = UT_CNSPController::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPController* UT_CNSPController::NewLC()
    {
    UT_CNSPController* self = new( ELeave ) UT_CNSPController();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::SetupL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::SetupL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    iController = CNSPController::NewL();
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::Teardown()
    {
    NSP_DEL( iController );
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// OBSERVER:
// ---------------------------------------------------------------------------
// UT_CNSPController::Initialized
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::Initialized( TUint /*aSessionId*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::OfferReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::OfferReady( TUint /*aSessionId*/, CSdpDocument* /*aOffer*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::AnswerReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::AnswerReady( TUint /*aSessionId*/, CSdpDocument* /*aAnswer*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::UpdateSdp
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UpdateSdp( TUint /*aSessionId*/, CSdpDocument* /*aOffer*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::ErrorOccurred( TUint aSessionId, TInt aError )
    {
    iReturnedSessionId = aSessionId;
    iReturnedError = aError;
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::IcmpErrorOccurred( TUint /*aSessionId*/, TInt /*aError*/ )
	{
	}



// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPController::UT_CreateUniqueId
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_CreateUniqueId()
    {
    TUint collectionId1 = iController->CreateUniqueId();
    TUint collectionId2 = iController->CreateUniqueId();
    NSP_ASSERT( collectionId1 != collectionId2 );
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::UT_GenerateUsernameL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_GenerateUsernameL()
    {
    const TUint usernameMinLength = 8;
    TBuf8<usernameMinLength + 1> suitable;
    
    // OK
    NSP_ASSERT_NO_LEAVE( iController->GenerateUsernameL( suitable ) );
    NSP_ASSERT( suitable.Length() );
    
    TBuf8<1> tooShort;
    
    // TOO SHORT
    NSP_ASSERT_LEAVE( iController->GenerateUsernameL( tooShort ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::UT_GeneratePasswordL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_GeneratePasswordL()
    {
    const TUint passwordMinLength = 26;
    TBuf8<passwordMinLength + 1> suitable;
    
    // OK
    NSP_ASSERT_NO_LEAVE( iController->GeneratePasswordL( suitable ) );
    NSP_ASSERT( suitable.Length() );
    
    TBuf8<1> tooShort;
    
    // TOO SHORT
    NSP_ASSERT_LEAVE( iController->GeneratePasswordL( tooShort ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::UT_NewSessionL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_NewSessionL()
    {
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KEmpty() );
    
    // FAILURE IN CENREP SETTINGS, IN APROTOCOL
    NSP_ASSERT_LEAVE( iController->NewSessionL( *this,
            KIapId, KTestDomain(), KProtocolUdp ) );
    
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KNokiaStun() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iController->NewSessionL(
            *this, KIapId, KTestDomain(), KProtocolUdp ) );
    
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KNokiaIce() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iController->NewSessionL(
            *this, KIapId, KTestDomain(), KProtocolUdp ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::UT_FindSessionObjectL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_FindSessionObjectL()
    {
    CNSPSession* session = CNSPSession::NewLC(
    		*iController, *this, KIapId, KTestDomain(), KProtocolUdp );
    iController->iSessionArray.AppendL( session );
    CleanupStack::Pop( session );
    TUint sessionId = session->SessionId();
    
    // OK
    NSP_ASSERT_NO_LEAVE( iController->FindSessionObjectL( sessionId ) );
    
    TUint sessionIdFail = sessionId + 1;
    
    // NOT FOUND
    NSP_ASSERT_LEAVE( iController->FindSessionObjectL( sessionIdFail ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::UT_RandomByteL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_RandomByteL()
    {
    TUint8 randombyte1 = iController->RandomByteL();
    TUint8 randombyte2 = iController->RandomByteL();
    
    NSP_ASSERT( randombyte1 != randombyte2 );
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::UT_UseIceL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_UseIceL()
    {
    TBool useIce = EFalse;
    NSPTestHelper::ClearCRepositoryL( *iCenrep );
    NSPTestHelper::CreateDomainKeyL( *iCenrep, KTestDomain() );
    CDesC8Array* protocols = new (ELeave) CDesC8ArrayFlat( 5 );
    CleanupStack::PushL( protocols );
    
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KNokiaIce() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( useIce = iController->UseIceL( *protocols, KTestDomain() ) );
    NSP_ASSERT( ETrue == useIce );
    
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KNokiaAll() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( useIce = iController->UseIceL( *protocols, KTestDomain() ) );
    NSP_ASSERT( ETrue == useIce );
    
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KNonNokia() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( useIce = iController->UseIceL( *protocols, KTestDomain() ) );
    NSP_ASSERT( ETrue == useIce );
    
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KNokiaStun() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( useIce = iController->UseIceL( *protocols, KTestDomain() ) );
    NSP_ASSERT( EFalse == useIce );
    
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KNokiaTurn() );
    
    // OK, NOT ICE
    NSP_ASSERT_NO_LEAVE( useIce = iController->UseIceL( *protocols, KTestDomain() ) );
    NSP_ASSERT( EFalse == useIce );
    
    NSPTestHelper::SetCenrepTDesC8toAProtocolL(
            *iCenrep, iCurrentDomainKey, KEmpty() );
    
    // FAILURE IN CENREP SETTINGS, IN APROTOCOL
    NSP_ASSERT_LEAVE( iController->UseIceL( *protocols, KTestDomain() ) );
    
    CleanupStack::PopAndDestroy( protocols );
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::UT_ReplaceArrayL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_ReplaceArrayL()
    {
    _LIT8( KSTUNandICE, "nokia.stun,nokia.ice" );
    _LIT8( KInvalidSpacing, "nokia.turn , nokia.ice" );
    _LIT8( KInvalidSpacing2, "nokia.turn , nokia.ice,nokia.stun" );
    _LIT8( KInvalidSpacing3, " nokia.turn , nokia.ice,nokia.stun" );
    _LIT8( KEmptySpace, " " );
    
    CDesC8Array* protocols = new (ELeave) CDesC8ArrayFlat( 5 );
    CleanupStack::PushL( protocols );
    
    // OK, ICE
    NSP_ASSERT_NO_LEAVE( iController->ReplaceArrayL( *protocols, KICE() ) );
    NSP_ASSERT( 1 == protocols->MdcaCount() );
    NSP_ASSERT( !protocols->MdcaPoint( 0 ).Compare( KICE() ) );
    
    // OK, STUN & ICE
    NSP_ASSERT_NO_LEAVE( iController->ReplaceArrayL( *protocols, KSTUNandICE() ) );
    NSP_ASSERT( 2 == protocols->MdcaCount() );
    NSP_ASSERT( !protocols->MdcaPoint( 0 ).Compare( KSTUN() ) );
    NSP_ASSERT( !protocols->MdcaPoint( 1 ).Compare( KICE() ) );
    
    // OK, TURN & ICE, WITH INVALID SPACING IN STRING
    NSP_ASSERT_NO_LEAVE( iController->ReplaceArrayL( *protocols, KInvalidSpacing() ) );
    NSP_ASSERT( 2 == protocols->MdcaCount() );
    NSP_ASSERT( !protocols->MdcaPoint( 0 ).Compare( KTURN() ) );
    NSP_ASSERT( !protocols->MdcaPoint( 1 ).Compare( KICE() ) );
    
    // OK, TURN & ICE & STUN, WITH INVALID SPACING IN STRING
    NSP_ASSERT_NO_LEAVE( iController->ReplaceArrayL( *protocols, KInvalidSpacing2() ) );
    NSP_ASSERT( 3 == protocols->MdcaCount() );
    NSP_ASSERT( !protocols->MdcaPoint( 0 ).Compare( KTURN() ) );
    NSP_ASSERT( !protocols->MdcaPoint( 1 ).Compare( KICE() ) );
    NSP_ASSERT( !protocols->MdcaPoint( 2 ).Compare( KSTUN() ) );
    
    // OK, TURN & ICE & STUN, WITH INVALID SPACING IN STRING
    NSP_ASSERT_NO_LEAVE( iController->ReplaceArrayL( *protocols, KInvalidSpacing3() ) );
    NSP_ASSERT( 3 == protocols->MdcaCount() );
    NSP_ASSERT( !protocols->MdcaPoint( 0 ).Compare( KTURN() ) );
    NSP_ASSERT( !protocols->MdcaPoint( 1 ).Compare( KICE() ) );
    NSP_ASSERT( !protocols->MdcaPoint( 2 ).Compare( KSTUN() ) );
    
    // FAILURE IN CENREP SETTINGS, IN APROTOCOL
    NSP_ASSERT_LEAVE( iController->ReplaceArrayL( *protocols, KEmptySpace() ) );
    NSP_ASSERT( 0 == protocols->MdcaCount() );
    NSP_ASSERT_LEAVE( iController->ReplaceArrayL( *protocols, KNullDesC8() ) );
    NSP_ASSERT( 0 == protocols->MdcaCount() );
    
    CleanupStack::PopAndDestroy( protocols );
    }


// ---------------------------------------------------------------------------
// UT_CNSPController::UT_IsIceL
// ---------------------------------------------------------------------------
// 
void UT_CNSPController::UT_IsIceL()
    {
    _LIT8( KNonProtocol, "invalidteststring" );
    
    NSP_ASSERT( ETrue == iController->IsIce( KICE() ) );
    NSP_ASSERT( EFalse == iController->IsIce( KSTUN() ) );
    NSP_ASSERT( EFalse == iController->IsIce( KTURN() ) );
    NSP_ASSERT( EFalse == iController->IsIce( KNonProtocol() ) );
    }


// ---------------------------------------------------------------------------
// UT_CNSPController::UT_EventOccuredL
// ---------------------------------------------------------------------------
//
void UT_CNSPController::UT_EventOccuredL()
    {
    TUint KStreamId = 1;
    _LIT8( KData, "DATA" );
    TBuf8<4> str(KData);
    HBufC8* data = str.AllocL();
    CleanupStack::PushL( data );

    CNSPSession* session = CNSPSession::NewLC(
            *iController, *this, KIapId, KTestDomain(), KProtocolUdp );
    iController->iSessionArray.AppendL( session );
    CleanupStack::Pop( session );
    TUint sessionId = session->SessionId();
    
    iReturnedSessionId = 0;
    iReturnedError = 0;
    
    iController->EventOccured( sessionId, KStreamId,
            MNATFWConnectivityObserver::EGeneralError, KErrGeneral, data );
    
    NSP_ASSERT( iReturnedSessionId == sessionId );
    NSP_ASSERT( iReturnedError == KErrGeneral );
    
    CleanupStack::PushL( session );
    iController->iSessionArray.Reset();
    
    iController->iClosingSessionArray.AppendL( session );
    CleanupStack::Pop( session );

    
    iController->EventOccured( sessionId, KStreamId,
            MNATFWConnectivityObserver::EGeneralError, KErrGeneral, data );
    
    CleanupStack::PopAndDestroy( data );
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPController,
    "Individual method tests for controller",
    "UNIT" )

NSP_TEST_METHOD(
    "CreateCollectionId - test",
    "CNSPController",
    "UT_CreateCollectionId",
    "FUNCTIONALITY",
    SetupL, UT_CreateUniqueId, Teardown )

NSP_TEST_METHOD(
    "GenerateUsernameL - test",
    "CNSPController",
    "UT_GenerateUsernameL",
    "FUNCTIONALITY",
    SetupL, UT_GenerateUsernameL, Teardown )

NSP_TEST_METHOD(
    "GeneratePasswordL - test",
    "CNSPController",
    "UT_GeneratePasswordL",
    "FUNCTIONALITY",
    SetupL, UT_GeneratePasswordL, Teardown )

NSP_TEST_METHOD(
    "NewSessionL - test",
    "CNSPController",
    "UT_NewSessionL",
    "FUNCTIONALITY",
    SetupL, UT_NewSessionL, Teardown )

NSP_TEST_METHOD(
    "FindSessionObjectL - test",
    "CNSPController",
    "UT_FindSessionObjectL",
    "FUNCTIONALITY",
    SetupL, UT_FindSessionObjectL, Teardown )

NSP_TEST_METHOD(
    "RandomByteL - test",
    "CNSPController",
    "UT_RandomByteL",
    "FUNCTIONALITY",
    SetupL, UT_RandomByteL, Teardown )

NSP_TEST_METHOD(
    "UseIceL - test",
    "CNSPController",
    "UT_UseIceL",
    "FUNCTIONALITY",
    SetupL, UT_UseIceL, Teardown )

NSP_TEST_METHOD(
    "ReplaceArrayL - test",
    "CNSPController",
    "UT_ReplaceArrayL",
    "FUNCTIONALITY",
    SetupL, UT_ReplaceArrayL, Teardown )

NSP_TEST_METHOD(
    "IsIceL - test",
    "CNSPController",
    "UT_IsIceL",
    "FUNCTIONALITY",
    SetupL, UT_IsIceL, Teardown )

NSP_TEST_METHOD(
    "EventOccured - test",
    "CNSPController",
    "UT_EventOccuredL",
    "FUNCTIONALITY",
    SetupL, UT_EventOccuredL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
