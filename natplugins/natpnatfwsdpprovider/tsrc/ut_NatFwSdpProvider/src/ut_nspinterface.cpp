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

#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>
#include <sdpcodecstringpool.h>
#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include <in_sock.h>
#include "nspinterface.h"
#include "nspcontroller.h"
#include "nspsession.h"
#include "nspsessiondata.h"
#include "nspstatemachine.h"
#include "nspstatebase.h"
#include "nspmediastreamcontainer.h"
#include "ut_nspinterface.h"
#include "nsptesthelper.h"
#include "nsptls.h"

const TUint KDefaultTos = 184;
const TUint KNonDefaultTos = 26;
const TUint KInvalidTos = 470;



// METHODS:
// ---------------------------------------------------------------------------
// UT_CNSPInterface::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPInterface* UT_CNSPInterface::NewL()
    {
    UT_CNSPInterface* self = UT_CNSPInterface::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPInterface* UT_CNSPInterface::NewLC()
    {
    UT_CNSPInterface* self = new( ELeave ) UT_CNSPInterface();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::~UT_CNSPInterface
// ---------------------------------------------------------------------------
// 
UT_CNSPInterface::~UT_CNSPInterface()
    {
    delete iCenrep;
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::UT_CNSPInterface
// ---------------------------------------------------------------------------
// 
UT_CNSPInterface::UT_CNSPInterface()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::ConstructL()
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
// UT_CNSPInterface::SetupL
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::SetupL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    NSPTestHelper::SetCenrepTDesC8toAProtocolL( *iCenrep,
            iCurrentDomainKey, KNokiaStun() );
    iInterface = CNSPInterface::NewL();
    iSessionId = iInterface->NewSessionL( *this, KIapId,
            KTestDomain, KProtocolUdp );
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::Teardown()
    {
    delete iInterface;
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// OBSERVER:
// ---------------------------------------------------------------------------
// UT_CNSPInterface::Initialized
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::Initialized( TUint /*aSessionId*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::OfferReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::OfferReady( TUint /*aSessionId*/, CSdpDocument* /*aOffer*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::AnswerReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::AnswerReady( TUint /*aSessionId*/, CSdpDocument* /*aAnswer*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::UpdateSdp
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::UpdateSdp( TUint /*aSessionId*/, CSdpDocument* /*aOffer*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::ErrorOccurred( TUint /*aSessionId*/, TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::IcmpErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::IcmpErrorOccurred( TUint /*aSessionId*/, TInt /*aError*/ )
    {
    }



// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPInterface::UT_NewLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::UT_NewLL()
    {
    // NOP, All done in SetupL
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::UT_SetSessionParam
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::UT_SetSessionParam()
    {
    TInt status = KErrNone;
    CNSPSession* session = NULL;
    NSP_ASSERT_NO_LEAVE( session = &iInterface->iController->FindSessionObjectL( iSessionId ) );
    
    // *** Reservation-Status( read-only )
    status = iInterface->SetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey, KNonDefaultTos );
    NSP_ASSERT( KErrNotSupported == status );
    
    status = iInterface->SetSessionParam( iSessionId+1,
            CNSPPlugin::ENSPResourseReservationStatusKey, KNonDefaultTos );
    NSP_ASSERT( KErrNotFound == status );
    
    // *** Type-of-service
    status = iInterface->SetSessionParam( iSessionId+1,
            CNSPPlugin::ENSPMediaTypeOfServiceKey, KInvalidTos );
    NSP_ASSERT( KErrNotFound == status );
    
    status = iInterface->SetSessionParam( iSessionId,
            CNSPPlugin::ENSPMediaTypeOfServiceKey, KInvalidTos );
    NSP_ASSERT( KErrArgument == status );
    NSP_ASSERT( KDefaultTos == session->Container().MediaTos() );
    
    status = iInterface->SetSessionParam( iSessionId,
            CNSPPlugin::ENSPMediaTypeOfServiceKey, KNonDefaultTos );
    NSP_ASSERT( KErrNone == status );
    NSP_ASSERT( KNonDefaultTos == session->Container().MediaTos() );
    }

// ---------------------------------------------------------------------------
// UT_CNSPInterface::UT_GetSessionParam
// ---------------------------------------------------------------------------
// 
void UT_CNSPInterface::UT_GetSessionParam()
    {
    TInt status = KErrNone;
    CNSPSession* session = NULL;
    NSP_ASSERT_NO_LEAVE( session = &iInterface->iController->FindSessionObjectL( iSessionId ) );
    
    // *** Reservation-Status( read-only )
    status = iInterface->GetSessionParam( iSessionId+1,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( KErrNotFound == status );
    
    // STUN & TURN
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( CNSPPlugin::ENSPResourcesReserved == status );
    
    // Idle
    session->Data().SetUseIce( ETrue );
    session->iStateMachine->iCurrentState =
            session->iStateMachine->iStateArray[ KStateIndexIdle ];
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( CNSPPlugin::ENSPResourcesNotReserved == status );
    
    // Localcand
    session->iStateMachine->iCurrentState =
            session->iStateMachine->iStateArray[ KStateIndexLocalCand ];
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( CNSPPlugin::ENSPResourcesNotReserved == status );
    
    // Negotiating
    session->iStateMachine->iCurrentState =
            session->iStateMachine->iStateArray[ KStateIndexNegotiating ];
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( CNSPPlugin::ENSPResourcesNotReserved == status );
    
    // Resolving
    session->iStateMachine->iCurrentState =
            session->iStateMachine->iStateArray[ KStateIndexNegotiating ];
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( CNSPPlugin::ENSPResourcesNotReserved == status );
    
    // Activating
    session->iStateMachine->iCurrentState =
            session->iStateMachine->iStateArray[ KStateIndexActivating ];
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( CNSPPlugin::ENSPResourcesReserved == status );
    
    // Connected
    session->iStateMachine->iCurrentState =
            session->iStateMachine->iStateArray[ KStateIndexConnected ];
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( CNSPPlugin::ENSPResourcesReserved == status );
    
    // DeActivating
    session->iStateMachine->iCurrentState =
            session->iStateMachine->iStateArray[ KStateIndexDeActivating ];
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPResourseReservationStatusKey );
    NSP_ASSERT( CNSPPlugin::ENSPResourcesReserved == status );
    
    // *** Type-of-service
    status = iInterface->GetSessionParam( iSessionId+1,
            CNSPPlugin::ENSPMediaTypeOfServiceKey );
    NSP_ASSERT( KErrNotFound == status );
    
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPMediaTypeOfServiceKey );
    NSP_ASSERT( KDefaultTos == status );
    
    session->Container().iMediaTos = KNonDefaultTos;
    status = iInterface->GetSessionParam( iSessionId,
            CNSPPlugin::ENSPMediaTypeOfServiceKey );
    NSP_ASSERT( KNonDefaultTos == status );
    }



// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPInterface,
    "Individual method tests for interface",
    "UNIT" )

NSP_TEST_METHOD(
	"NewL - test",
	"CNSPInterface",
	"UT_NewLL",
	"FUNCTIONALITY",
	SetupL, UT_NewLL, Teardown )

NSP_TEST_METHOD(
    "SetSessionParam - test",
    "CNSPInterface",
    "UT_SetSessionParam",
    "FUNCTIONALITY",
    SetupL, UT_SetSessionParam, Teardown )

NSP_TEST_METHOD(
    "GetSessionParam - test",
    "CNSPInterface",
    "UT_GetSessionParam",
    "FUNCTIONALITY",
    SetupL, UT_GetSessionParam, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
