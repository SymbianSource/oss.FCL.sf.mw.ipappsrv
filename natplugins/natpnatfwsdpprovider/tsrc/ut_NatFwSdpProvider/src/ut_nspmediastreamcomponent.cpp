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
#include "mnatfwsocketmediaconnwrapper.h"
#include <sdpcodecstringpool.h>
#include <sdpmediafield.h>
#include "nsppluginreturndef.h"
#include "nspcontroller.h"
#include "nspcontentparser.h"
#include "nspsession.h"
#include "nspstatemachine.h"
#include "nspstatebase.h"
#include "nspmediastreamcontainer.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "ut_nspmediastreamcomponent.h"
#include "nsptesthelper.h"
#include "nsptls.h"

const TUint KSessionId = 10;
const TUint KCollectionId = 200;

_LIT8( KMediaField, "m=audio 2330 RTP/AVP 96\r\n" );
_LIT8( KAddress, "127.0.0.1" );

// METHODS:
// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStreamComponent* UT_CNSPMediaStreamComponent::NewL()
    {
    UT_CNSPMediaStreamComponent* self = UT_CNSPMediaStreamComponent::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStreamComponent* UT_CNSPMediaStreamComponent::NewLC()
    {
    UT_CNSPMediaStreamComponent* self = new( ELeave ) UT_CNSPMediaStreamComponent();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::~UT_CNSPMediaStreamComponent
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStreamComponent::~UT_CNSPMediaStreamComponent()
    {
    delete iCenrep;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::UT_CNSPMediaStreamComponent
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStreamComponent::UT_CNSPMediaStreamComponent()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::ConstructL()
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
// UT_CNSPMediaStreamComponent::SetupL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::SetupL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    NSPTestHelper::SetCenrepTDesC8toAProtocolL( *iCenrep,
            iCurrentDomainKey, KNokiaStun() );
    
    iController = CNSPController::NewL();
    iSessionId = iController->NewSessionL( *this, KIapId,
            KTestDomain(), KProtocolUdp );
    iSession = &iController->FindSessionObjectL( iSessionId );
    NSP_SET_STATE( iSession, KStateIndexConnected );
    
    CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KMediaField() );
    iStream = CNSPMediaStream::NewL(
            *iController, *mediafield, KSessionId, KCollectionId,
            KAddress(), KProtocolUdp, KMediaTos );
    iSession->iStreamContainer->iStreamArray.AppendL( iStream );
    CleanupStack::PopAndDestroy( mediafield );
    iStreamComponent = iStream->iStreamComponentArray[0]; // RTP
    iLastCall = ENSP_UT_None;
    iLastError = KErrNone;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::Teardown()
    {
    delete iController;
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// OBSERVER:
// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::Initialized
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::Initialized( TUint aSessionId )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_Initialized;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::OfferReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::OfferReady( TUint aSessionId, CSdpDocument* /*aDocument*/ )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_OfferReady;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::AnswerReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::AnswerReady( TUint aSessionId, CSdpDocument* /*aDocument*/ )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_AnswerReady;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::UpdateSdp
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::UpdateSdp( TUint aSessionId, CSdpDocument* /*aDocument*/ )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_UpdateSdp;    
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::ErrorOccurred( TUint aSessionId, TInt aError )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_ErrorOccurred;
    iLastError = aError;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::IcmpErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::IcmpErrorOccurred( TUint aSessionId, TInt aError )
	{
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_IcmpErrorOccurred;
    iLastError = aError;
    }


// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::UT_ControlMediaLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::UT_ControlMediaLL()
    {
    // test scenario where terminal is not behind NAT
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    candidate->SetType( CNATFWCandidate::EServerReflexive );
    candidate->SetBase( TInetAddr( INET_ADDR( 10, 36, 7, 6 ), 5000 ) );
    candidate->SetTransportAddrL( TInetAddr( INET_ADDR( 10, 36, 7, 6 ), 5000 ) );
    iStreamComponent->iLocalCandidates.AppendL( candidate );
    CleanupStack::Pop( candidate );
    
    TNatReturnStatus status = iStreamComponent->ControlMediaL(
        MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    NSP_ASSERT( KNatNonFatalErrorLimit == status );
    
    // test scenario where terminal is behind NAT
    candidate->SetTransportAddrL( TInetAddr( INET_ADDR( 10, 36, 7, 6 ), 5002 ) );
    status = iStreamComponent->ControlMediaL(
        MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    NSP_ASSERT( KNatReady == status );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::UT_RtpIcmp_StateSendRecv
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::UT_RtpIcmp_StateSendRecv()
    {
    TNatReturnStatus status = KNatReady;
    
    // State transition to EFetching
    NSP_ASSERT_NO_LEAVE( iStreamComponent->FetchCandidateL() );
    
    CNATFWCandidate* candidate = NSPTestHelper::CreateCandL(
    		*iStreamComponent,
    		_L8("31233"), KProtocolInetUdp,
    		CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ); // RTP
    CleanupStack::PushL( candidate );
    iStreamComponent->iLocalCandidates.AppendL( candidate );
    CleanupStack::Pop( candidate );
    
    // State transition back to EIdle
    NSP_ASSERT_NO_LEAVE( status = iStreamComponent->ControlMediaL(
    		MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ) );
    NSP_ASSERT( KNatReady == status ) ;
    
    // ICE activation not accepted since no candidate pair available
    NSP_ASSERT_LEAVE( iStreamComponent->ActivateL() );
    
    // STUN activation successfull
    NSP_ASSERT_NO_LEAVE( iStreamComponent->ActivateL( _L8("192.168.0.2"), 4000 ) );
    
    // State transition to SendRecv after these events
    NSP_ASSERT_NO_LEAVE( status = iStreamComponent->ControlMediaL(
			MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT_NO_LEAVE( status = iStreamComponent->ControlMediaL(
			MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ) );
    NSP_ASSERT( KNatReady == status );
    
    // Icmp error has happened, for RTP component
    EVENT( iStreamComponent->iStreamId, MNATFWConnectivityObserver::EGeneralError,
     	   KErrCommsLineFail, NULL );
    NSP_ASSERT( ENSP_UT_IcmpErrorOccurred == iLastCall );
    NSP_ASSERT( KErrCommsLineFail == iLastError );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamComponent::UT_RtcpIcmp_StateSendRecv
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamComponent::UT_RtcpIcmp_StateSendRecv()
    {
    TNatReturnStatus status = KNatReady;
    iStreamComponent = iStream->iStreamComponentArray[1]; // RTCP
    
    // State transition to EFetching
    NSP_ASSERT_NO_LEAVE( iStreamComponent->FetchCandidateL() );
    
    CNATFWCandidate* candidate = NSPTestHelper::CreateCandL(
    		*iStreamComponent,
            _L8("31233"), KProtocolInetUdp,
            CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue );
    CleanupStack::PushL( candidate );
    iStreamComponent->iLocalCandidates.AppendL( candidate ); // RTP
    CleanupStack::Pop( candidate );
    
    // State transition back to EIdle
    NSP_ASSERT_NO_LEAVE( status = iStreamComponent->ControlMediaL(
    		MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ) );
    NSP_ASSERT( KNatReady == status );
    
    // ICE activation not accepted since no candidate pair available
    NSP_ASSERT_LEAVE( iStreamComponent->ActivateL() );
    
    // STUN activation successfull
    NSP_ASSERT_NO_LEAVE( iStreamComponent->ActivateL( _L8("192.168.0.2"), 4000 ) );
    
    // State transition to SendRecv after these events
    NSP_ASSERT_NO_LEAVE( status = iStreamComponent->ControlMediaL(
			MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT_NO_LEAVE( status = iStreamComponent->ControlMediaL(
			MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ) );
    NSP_ASSERT( KNatReady == status );
    
    // Icmp error has happened, for RTCP component, to be ignored
    EVENT( iStreamComponent->iStreamId, MNATFWConnectivityObserver::EGeneralError,
    	   KErrCommsLineFail, NULL );
    NSP_ASSERT( ENSP_UT_None == iLastCall );
    NSP_ASSERT( KErrNone == iLastError );
    }



// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPMediaStreamComponent,
    "Individual method tests for stream component",
    "UNIT" )

NSP_TEST_METHOD(
    "ControlMediaL - test",
    "CNSPMediaStreamContainer",
    "UT_ControlMediaLL",
    "FUNCTIONALITY",
    SetupL, UT_ControlMediaLL, Teardown )

NSP_TEST_USECASE(
    "RtpIcmp_StateSendRecv - test",
    "CNSPMediaStreamComponent",
    "UT_RtpIcmp_StateSendRecv",
    "FUNCTIONALITY",
    SetupL, UT_RtpIcmp_StateSendRecv, Teardown )

NSP_TEST_USECASE(
	"RtcpIcmp_StateSendRecv - test",
	"CNSPMediaStreamComponent",
	"UT_RtcpIcmp_StateSendRecv",
	"FUNCTIONALITY",
	SetupL, UT_RtcpIcmp_StateSendRecv, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
