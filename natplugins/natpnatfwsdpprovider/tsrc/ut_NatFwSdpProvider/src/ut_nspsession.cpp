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

#include "mnatfwconnectivityobserver.h"
#include <e32math.h>
#include <s32mem.h>
#include <random.h>
#include <sdpcodecstringpool.h>
#include <sdpcodecstringconstants.h>
#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include <sdpmediafield.h>
#include <e32debug.h>
#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "natfwcredentials.h"
#include "mnatfwconnectivityobserver.h"
#include "nspcontentparser.h"
#include "nspsession.h"
#include "nspstatemachine.h"
#include "nspstatebase.h"
#include "nspmediastreamcontainer.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "nspdefs.h"
#include "nspcontroller.h"
#include "ut_nspsession.h"
#include "nsptesthelper.h"
#include "nsptls.h"
#include "s_natfwsocketmediaconnwrapper.h"

_LIT8( KStunSdpOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=audio 2330 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223 IN IP4 10.0.0.6\r\n" );

_LIT8( KStunSdpAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.4\r\n\
t=0 0\r\n\
m=audio 12322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n");



// METHODS:
// ---------------------------------------------------------------------------
// UT_CNSPSession::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPSession* UT_CNSPSession::NewL()
    {
    UT_CNSPSession* self = UT_CNSPSession::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPSession* UT_CNSPSession::NewLC()
    {
    UT_CNSPSession* self = new( ELeave ) UT_CNSPSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::~UT_CNSPSession
// ---------------------------------------------------------------------------
// 
UT_CNSPSession::~UT_CNSPSession()
    {
    delete iCenrep;
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::UT_CNSPSession
// ---------------------------------------------------------------------------
// 
UT_CNSPSession::UT_CNSPSession()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::ConstructL()
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
// UT_CNSPSession::SetupL
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::SetupL()
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
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::Teardown()
    {
    NSP_DEL( iDocument );
    NSP_DEL( iController );
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// OBSERVER:
// ---------------------------------------------------------------------------
// UT_CNSPSession::Initialized
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::Initialized( TUint aSessionId )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_Initialized;
    }
    
// ---------------------------------------------------------------------------
// UT_CNSPSession::OfferReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::OfferReady( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_OfferReady;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::AnswerReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::AnswerReady( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_AnswerReady;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::UpdateSdp
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::UpdateSdp( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_UpdateSdp;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::ErrorOccurred( TUint aSessionId, TInt /*aError*/ )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_ErrorOccurred;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    }

// ---------------------------------------------------------------------------
// UT_CNSPController::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::IcmpErrorOccurred( TUint aSessionId, TInt /*aError*/ )
	{
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_IcmpErrorOccurred;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );	
	}



// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPSession::UT_CreateOfferL
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::UT_CreateOfferL()
    {
    TNatReturnStatus status = KNatReady;
    CSdpDocument* offer = NULL;
    
    // ASSERT
    NSP_ASSERT_LEAVE( status = iSession->CreateOfferL( offer ) );
    NSP_ASSERT( KNatReady == status );
    
    // Leave due to missing 'INITIALIZED' callback
    offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_LEAVE( status = iSession->CreateOfferL( offer ) );
    NSP_ASSERT( KNatReady == status );
    CleanupStack::PopAndDestroy( offer );
    
    // OK
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->CreateOfferL( offer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( iSession->iStreamContainer->iStreamArray.Count() );
    NSP_ASSERT( iSession->iStreamContainer->iPendingArray.Count() );
    CleanupStack::PopAndDestroy( offer );
    
    // PENDING
    offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_LEAVE( iSession->CreateOfferL( offer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexLocalCand );
    CleanupStack::PopAndDestroy( offer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::UT_ResolveL
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::UT_ResolveL()
    {
    TNatReturnStatus status = KNatReady;
    CSdpDocument* offer = NULL;
    CSdpDocument* answer = NULL;
    
    // ASSERT
    NSP_ASSERT_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatReady == status );
    
    // Leave due to missing 'INITIALIZED' callback
    offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_LEAVE( iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatReady == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // OK
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( iSession->iStreamContainer->iStreamArray.Count() );
    NSP_ASSERT( iSession->iStreamContainer->iPendingArray.Count() );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // PENDING
    offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexLocalCand );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::UT_DecodeAnswerL
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::UT_DecodeAnswerL()
    {
    TNatReturnStatus status = KNatReady;
    CSdpDocument* answer = NULL;
    
    // ASSERT
    NSP_ASSERT_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatReady == status );
    
    // Leave due to missing 'INITIALIZED' callback
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatReady == status );
    CleanupStack::PopAndDestroy( answer );
    
    // changing state to 'negotiating'
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_SET_STATE( iSession, KStateIndexNegotiating );
    iSession->iStreamContainer->CreateStreamsL( *answer );
    CleanupStack::PopAndDestroy( answer );
    
    // Local candidates
    CNATFWCandidate* candidate = NSPTestHelper::CreateCandL(
			(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
		    _L8("31233"), KProtocolInetUdp, 
		    CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue );
    CleanupStack::PushL( candidate );
    iSession->iStreamContainer->iStreamArray[0]->
    		iStreamComponentArray[0]->iLocalCandidates.AppendL( candidate ); // RTP
	CleanupStack::Pop( candidate );
	candidate = NSPTestHelper::CreateCandL(
			(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
		    _L8("31233"), KProtocolInetUdp, 
		    CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue );
    CleanupStack::PushL( candidate );
	iSession->iStreamContainer->iStreamArray[0]->
			iStreamComponentArray[1]->iLocalCandidates.AppendL( candidate ); // RTCP
	CleanupStack::Pop( candidate );
    
	// OK
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
	NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
	NSP_ASSERT( KNatAsync == status );
	NSP_ASSERT( iSession->iStreamContainer->iStreamArray.Count() );
	NSP_ASSERT( iSession->iStreamContainer->iPendingArray.Count() );
    CleanupStack::PopAndDestroy( answer );
    
    // PENDING
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    CleanupStack::PopAndDestroy( answer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPSession::UT_UpdateL
// ---------------------------------------------------------------------------
// 
void UT_CNSPSession::UT_UpdateL()
    {
    CSdpDocument* offer = NULL;
    
    // ASSERT
    NSP_ASSERT_LEAVE( iSession->UpdateL( offer ) );
    
    // Leave due to missing 'INITIALIZED' callback
    offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_LEAVE( iSession->UpdateL( offer ) );
    CleanupStack::PopAndDestroy( offer ); 
    
    // OK
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_SET_STATE( iSession, KStateIndexResolving );
    iSession->iStreamContainer->CreateStreamsL( *offer );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( offer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexResolving );
    CleanupStack::PopAndDestroy( offer );
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPSession,
    "Individual method tests for session",
    "UNIT" )

NSP_TEST_METHOD(
    "CreateOfferL - test",
    "CNSPSession",
    "UT_CreateOfferLL",
    "FUNCTIONALITY",
    SetupL, UT_CreateOfferL, Teardown )

NSP_TEST_METHOD(
    "ResolveL - test",
    "CNSPSession",
    "UT_ResolveLL",
    "FUNCTIONALITY",
    SetupL, UT_ResolveL, Teardown )

NSP_TEST_METHOD(
    "DecodeAnswerL - test",
    "CNSPSession",
    "UT_DecodeAnswerLL",
    "FUNCTIONALITY",
    SetupL, UT_DecodeAnswerL, Teardown )

NSP_TEST_METHOD(
    "UpdateL - test",
    "CNSPSession",
    "UT_UpdateLL",
    "FUNCTIONALITY",
    SetupL, UT_UpdateL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
