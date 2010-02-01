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
#include "nspcontroller.h"
#include "nspactive.h"
#include "nspcontentparser.h"
#include "nspsession.h"
#include "nspstatemachine.h"
#include "nspstatebase.h"
#include "nspmediastreamcontainer.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "nspdefs.h"
#include "ut_nspusecases.h"
#include "nsptesthelper.h"
#include "nsptls.h"

_LIT8( KStunSdpOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=audio 2330 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223 IN IP4 10.0.0.6\r\n\
m=video 2430 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n" );


_LIT8( KStunSdpOffer1, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.10\r\n\
t=0 0\r\n\
m=audio 49152 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n");

_LIT8( KStunSdpOffer2, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.10\r\n\
t=0 0\r\n\
m=audio 49154 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n");

_LIT8( KStunSdpOffer3, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 10.57.0.10\r\n\
t=0 0\r\n\
m=audio 21388 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n");

_LIT8( KStunSdpAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.4\r\n\
t=0 0\r\n\
m=audio 12322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n");


_LIT8( KStunSdpResolveOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.2\r\n\
t=0 0\r\n\
m=audio 12321 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14445 IN IP4 10.0.0.6\r\n\
m=video 14322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:16445 IN IP4 10.0.0.6\r\n");

_LIT8( KStunSdpResolveOffer2, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.2\r\n\
t=0 0\r\n\
m=audio 12321 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14445 IN IP4 10.0.0.6\r\n");


_LIT8( KStunSdpResolveAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=audio 2332 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223 IN IP4 131.117.0.4\r\n");

_LIT8( KStunRejectVideo, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.4\r\n\
t=0 0\r\n\
m=audio 12322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n\
m=video 0 RTP/AVP 96\r\n");

_LIT8( KStunSdpOfferModConnField, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 2330 RTP/AVP 96\r\n\
c=IN IP4 10.0.0.5\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223 IN IP4 10.0.0.16\r\n\
m=video 2430 RTP/AVP 96\r\n\
c=IN IP4 10.0.0.4\r\n\
a=rtpmap:96 AMR/8000\r\n" );

_LIT8( KStunSdpAnswerModConnField, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 12322 RTP/AVP 96\r\n\
c=IN IP4 131.117.0.4\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n\
m=video 2530 RTP/AVP 96\r\n\
c=IN IP4 131.117.0.8\r\n\
a=rtpmap:96 AMR/8000\r\n" );

_LIT8( KStunSdpOldwayHold, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 0.0.0.0\r\n\
t=0 0\r\n\
m=audio 12321 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14445 IN IP4 10.0.0.6\r\n\
m=video 14322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:16445 IN IP4 10.0.0.6\r\n" );

_LIT8( KIceSdpOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 131.231.0.196\r\n\
t=0 0\r\n\
m=audio 20330 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.196 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 131.231.0.196 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:20332 IN IP4 131.231.0.196\r\n" );

_LIT8( KIceSdpOfferUpdate1, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 131.231.0.196\r\n\
t=0 0\r\n\
m=audio 20330 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.196 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 131.231.0.196 20332 typ host\r\n\
a=candidate:3124 1 udp 200566 131.231.0.198 20330 typ relay\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:20332 IN IP4 131.231.0.196\r\n" );

_LIT8( KIceSdpOfferUpdate2, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 131.231.0.196\r\n\
t=0 0\r\n\
m=audio 20330 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.196 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 131.231.0.196 20332 typ host\r\n\
a=candidate:3124 1 udp 200566 131.231.0.198 20330 typ relay\r\n\
a=candidate:3124 2 udp 200568 131.231.0.196 20332 typ relay\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:20332 IN IP4 131.231.0.196\r\n" );

_LIT8( KIceSdpOfferHold, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 193.65.183.237\r\n\
t=0 0\r\n\
m=audio 100 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.196 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 131.231.0.196 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:101 IN IP4 193.65.183.237\r\n" );

_LIT8( KIceSdpAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.7\r\n\
s=-\r\n\
c=IN IP4 131.231.0.197\r\n\
t=0 0\r\n\
m=audio 20330 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.197 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 131.231.0.197 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:20332 IN IP4 131.231.0.197\r\n" );

_LIT8( KIceSdpMismatchOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 131.231.0.196\r\n\
t=0 0\r\n\
m=audio 4096 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.196 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 131.231.0.196 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:4098 IN IP4 131.231.0.196\r\n" );

_LIT8( KIceSdpMismatchAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 131.231.0.196\r\n\
t=0 0\r\n\
m=audio 4096 RTP/AVP 96\r\n\
a=ice-mismatch\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:4098 IN IP4 131.231.0.196\r\n" );

_LIT8( KIceSdpUpdatedOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 131.231.0.196\r\n\
t=0 0\r\n\
m=audio 20330 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.196 20330 typ host\r\n\
a=candidate:4123 1 udp 190534 193.65.183.237 2330 typ srflx\r\n\
a=candidate:3123 2 udp 200535 131.231.0.196 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:20332 IN IP4 131.231.0.196\r\n" );



// METHODS:
// ---------------------------------------------------------------------------
// UT_CNSPUseCases::RTCPAddr
// ---------------------------------------------------------------------------
// 
TDesC8& UT_CNSPUseCases::RTCPAddr( CSdpMediaField& aMediaField )
    {
    TUint port( 0 );
    TBuf8<255> address;
    iController->iContentParser->FindRTCP( aMediaField, address, port );
    HBufC8* addr8 = address.AllocL();
    iRtcpAddr.Close();
    iRtcpAddr.Assign( addr8 );
    return iRtcpAddr;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::RTCPPort
// ---------------------------------------------------------------------------
// 
TUint UT_CNSPUseCases::RTCPPort( CSdpMediaField& aMediaField )
    {
    TUint port( 0 );
    TBuf8<255> address;
    iController->iContentParser->FindRTCP( aMediaField, address, port );
    return port;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPUseCases* UT_CNSPUseCases::NewL()
    {
    UT_CNSPUseCases* self = UT_CNSPUseCases::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPUseCases* UT_CNSPUseCases::NewLC()
    {
    UT_CNSPUseCases* self = new( ELeave ) UT_CNSPUseCases();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::~UT_CNSPUseCases
// ---------------------------------------------------------------------------
// 
UT_CNSPUseCases::~UT_CNSPUseCases()
    {
    delete iCenrep;
    iRtcpAddr.Close();
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::UT_CNSPUseCases
// ---------------------------------------------------------------------------
// 
UT_CNSPUseCases::UT_CNSPUseCases()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::ConstructL()
    {
    iRtcpAddr.Assign( KNullDesC8().AllocL() );
    
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
// UT_CNSPUseCases::SetupStunL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::SetupStunL()
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
    iLastError = KErrNone;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::SetupIceL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::SetupIceL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    NSPTestHelper::SetCenrepTDesC8toAProtocolL( *iCenrep,
            iCurrentDomainKey, KNokiaAll() );
    iController = CNSPController::NewL();
	iSessionId = iController->NewSessionL( *this, KIapId,
            KTestDomain(), KProtocolUdp );
    iSession = &iController->FindSessionObjectL( iSessionId );
    iLastError = KErrNone;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::Teardown()
    {
    NSP_DEL( iDocument );
    NSP_DEL( iController );
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// OBSERVER:
// ---------------------------------------------------------------------------
// UT_CNSPUseCases::Initialized
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::Initialized( TUint aSessionId )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_Initialized;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::OfferReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::OfferReady( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_OfferReady;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::AnswerReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::AnswerReady( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_AnswerReady;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::UpdateSdp
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::UpdateSdp( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_UpdateSdp;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::ErrorOccurred( TUint aSessionId, TInt aError )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_ErrorOccurred;
    iLastError = aError;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::IcmpErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::IcmpErrorOccurred( TUint aSessionId, TInt aError )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_IcmpErrorOccurred;
    iLastError = aError;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    }



// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPUseCases::CreateStunOfferLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::CreateStunOfferLL()
    {
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer
    CSdpDocument* document = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->CreateOfferL( document ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( document );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp, 
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    	   MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("312333"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    	   MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp, 
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]),
    				 _L8("312333"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexNegotiating );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() ==
                iSession->iStreamContainer->iStreamArray[1]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::ResolveStunLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::ResolveStunLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;    
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer & answer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP

    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );    
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() == 0 );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::DecodeStunAnswerLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::DecodeStunAnswerLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    NSP_SET_STATE( iSession, KStateIndexNegotiating );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    iSession->iStreamContainer->CreateStreamsL( *offer );
    CleanupStack::PopAndDestroy( offer );
    
    // Add local candidates
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		 NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate
    
    // Answer
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    CleanupStack::PopAndDestroy( answer );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunUpdateLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunUpdateLL()
    {
    TBuf8<255> address;
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and answer
    CSdpDocument* initialOffer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    CSdpDocument* initialAnswer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( initialOffer, initialAnswer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( initialAnswer );
    CleanupStack::PopAndDestroy( initialOffer );
    
    // Incoming INVITE with updated SDP.
    CSdpDocument* updateOffer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( updateOffer ) );
    CleanupStack::PopAndDestroy( updateOffer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Incoming INVITE with updated SDP.
    updateOffer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( updateOffer ) );
    CleanupStack::PopAndDestroy( updateOffer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunRejectMTL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunRejectMTL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and answer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunRejectVideo() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( 1 == iSession->iStreamContainer->iStreamArray.Count() );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP

    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() == 0 );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunRejectMOL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunRejectMOL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;    
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->CreateOfferL( offer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( 2 == iSession->iStreamContainer->iStreamArray.Count() );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(), 
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexNegotiating );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() ==
                iSession->iStreamContainer->iStreamArray[1]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // New Answer
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunRejectVideo() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( 1 == iSession->iStreamContainer->iStreamArray.Count() );
    CleanupStack::PopAndDestroy( answer );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunCreateOfferModConnFieldL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunCreateOfferModConnFieldL()
    {
    TBuf8<255> address;
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOfferModConnField() );
    NSP_ASSERT_NO_LEAVE( status = iSession->CreateOfferL( offer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( 2 == iSession->iStreamContainer->iStreamArray.Count() );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
   		 NSPTestHelper::CreateCandLC(
   				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
   				 _L8("31233"), KProtocolInetUdp,
   				 CNATFWCandidate::ERelay, iSession->iSessionId, ETrue ) ); // RTP
	EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
          MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
	
	NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
	EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexNegotiating );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->MediaFields()[0]->ConnectionFields()[0]->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !iDocument->MediaFields()[1]->ConnectionFields()[0]->Address().Compare(
                iSession->iStreamContainer->iStreamArray[1]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() ==
                iSession->iStreamContainer->iStreamArray[1]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunResolveModConnFieldL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunResolveModConnFieldL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and Answer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOfferModConnField() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswerModConnField() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( 2 == iSession->iStreamContainer->iStreamArray.Count() );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
   		 	 NSPTestHelper::CreateCandLC(
   		 			 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
   		 			 _L8("31233"), KProtocolInetUdp,
   		 			 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::ERelay, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->MediaFields()[0]->ConnectionFields()[0]->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !iDocument->MediaFields()[1]->ConnectionFields()[0]->Address().Compare(
                iSession->iStreamContainer->iStreamArray[1]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() ==
                iSession->iStreamContainer->iStreamArray[1]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->MediaFields()[0]->ConnectionFields()[0]->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[1]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->MediaFields()[1]->ConnectionFields()[0]->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunDecodeAnswerModConnFieldL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunDecodeAnswerModConnFieldL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    NSP_SET_STATE( iSession, KStateIndexNegotiating );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOfferModConnField() );
    iSession->iStreamContainer->CreateStreamsL( *offer );
    CleanupStack::PopAndDestroy( offer );
    
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate

    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate

    iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::ERelay, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate

    iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate

    // Answer to Offer
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswerModConnField() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( 2 == iSession->iStreamContainer->iStreamArray.Count() );
    CleanupStack::PopAndDestroy( answer );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->MediaFields()[0]->ConnectionFields()[0]->Address() ) ) ;
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[1]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->MediaFields()[1]->ConnectionFields()[0]->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunMtHoldLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunMtHoldLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and answer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // New resolve
    offer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatReady == status );
    
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( !answer->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) ); // RTP(answer)
    NSP_ASSERT( answer->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*answer->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*answer->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP(offer)
    NSP_ASSERT( !address.Compare( offer->ConnectionField()->Address() ) );
    NSP_ASSERT( offer->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP(offer)
    NSP_ASSERT( !RTCPAddr(*offer->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*offer->MediaFields()[0]) == port );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunMtOldwayHoldLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunMtOldwayHoldLL()
    {
    TBuf8<255> rtpAddress, rtcpAddress;
    TUint rtpPort( 0 ), rtcpPort( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and answer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check Answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check Offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( rtpAddress, rtpPort ) );// RTP
    NSP_ASSERT( !rtpAddress.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == rtpPort );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( rtcpAddress, rtcpPort ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( rtcpAddress ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == rtcpPort );
    
    // Store remote addresses
    rtpAddress = iSession->iStreamContainer->iStreamArray[0]->
    		iStreamComponentArray[0]->RemoteAddress();
    rtpPort = iSession->iStreamContainer->iStreamArray[0]->
    		iStreamComponentArray[0]->RemotePort();
    rtcpAddress = iSession->iStreamContainer->iStreamArray[0]->
    		iStreamComponentArray[1]->RemoteAddress();
    rtcpPort = iSession->iStreamContainer->iStreamArray[0]->
    		iStreamComponentArray[1]->RemotePort();
    
    // New resolve
    offer = CSdpDocument::DecodeLC( KStunSdpOldwayHold() );
    answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatReady == status );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    
    // Check Answer
    NSP_ASSERT( !answer->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) ); // RTP(answer)
    NSP_ASSERT( answer->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*answer->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*answer->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Check Offer
    NSP_ASSERT( offer->ConnectionField()->InetAddress()->IsUnspecified() );
    NSP_ASSERT( !rtpAddress.Compare( iSession->iStreamContainer->iStreamArray[0]->
				iStreamComponentArray[0]->RemoteAddress() ) );
    NSP_ASSERT( iSession->iStreamContainer->iStreamArray[0]->
				iStreamComponentArray[0]->RemotePort() == rtpPort );
    NSP_ASSERT( !rtcpAddress.Compare( iSession->iStreamContainer->iStreamArray[0]->
				iStreamComponentArray[1]->RemoteAddress() ) );
    NSP_ASSERT( iSession->iStreamContainer->iStreamArray[0]->
				iStreamComponentArray[1]->RemotePort() == rtcpPort );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::StunMoAnswerUpdateLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::StunMoAnswerUpdateLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
	NSP_SET_STATE( iSession, KStateIndexNegotiating );
    iSession->iStreamContainer->CreateStreamsL( *offer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate

    // New Answer
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    CleanupStack::PopAndDestroy( answer );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // Secondary answer
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( answer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *answer ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *answer ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *answer ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*answer->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*answer->MediaFields()[0]) == port );
    CleanupStack::PopAndDestroy( answer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::RemoteAddressUpdateLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::RemoteAddressUpdateLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOffer2() );
	NSP_SET_STATE( iSession, KStateIndexNegotiating );
    iSession->iStreamContainer->CreateStreamsL( *offer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate
    
    // New Answer
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( iSession->DecodeAnswerL( answer ) );
    CleanupStack::PopAndDestroy( answer );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check Answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // Update remote addresses
    CSdpDocument* resolveoffer = CSdpDocument::DecodeL( KStunSdpResolveOffer2() );
    CleanupStack::PushL( resolveoffer );
    offer = resolveoffer->CloneL();
    CleanupStack::Pop( resolveoffer );
    CleanupStack::PushL( offer );
    CleanupStack::PushL( resolveoffer );
    CSdpDocument* resolveanswer = CSdpDocument::DecodeLC( KStunSdpOffer2() );
    
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( resolveoffer, resolveanswer ) );
    NSP_ASSERT( KNatReady == status );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    CleanupStack::PopAndDestroy( resolveanswer );
    CleanupStack::PopAndDestroy( resolveoffer );

    
    // Check remote addresses
    NSP_ASSERT( !offer->ConnectionField()->Address().Compare(
    			iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->RemoteAddress() ) );
    NSP_ASSERT( offer->MediaFields()[0]->Port() ==
    			iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->RemotePort() );
    NSP_ASSERT( !RTCPAddr(*offer->MediaFields()[0]).Compare(
    			iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->RemoteAddress() ) );
    NSP_ASSERT( RTCPPort(*offer->MediaFields()[0]) ==
    			iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->RemotePort() );
    CleanupStack::PopAndDestroy( offer );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP

    // Check state
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::SyncClosingL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::SyncClosingL()
    {
    // Idle
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    NSP_ASSERT_NO_LEAVE( iController->ClosingSessionL( iSession->iSessionId ) );
    NSP_ASSERT( !iController->iSessionArray.Count() );
    NSP_ASSERT( !iController->iClosingSessionArray.Count() );
    
    // Localcand
    iSessionId = iController->NewSessionL( *this, KIapId, KTestDomain(), KProtocolUdp );
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    iSession = &iController->FindSessionObjectL( iSessionId );
	NSP_SET_STATE( iSession, KStateIndexLocalCand );
	
    NSP_ASSERT_NO_LEAVE( iController->ClosingSessionL( iSessionId ) );
    NSP_ASSERT( !iController->iSessionArray.Count() );
    NSP_ASSERT( !iController->iClosingSessionArray.Count() );
    
    // Negotiating
    iSessionId = iController->NewSessionL( *this, KIapId, KTestDomain(), KProtocolUdp );
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    iSession = &iController->FindSessionObjectL( iSessionId );
	NSP_SET_STATE( iSession, KStateIndexNegotiating );
	
    NSP_ASSERT_NO_LEAVE( iController->ClosingSessionL( iSessionId ) );
    NSP_ASSERT( !iController->iSessionArray.Count() );
    NSP_ASSERT( !iController->iClosingSessionArray.Count() );    
    
    // Resolving
    iSessionId = iController->NewSessionL( *this, KIapId, KTestDomain(), KProtocolUdp );
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    iSession = &iController->FindSessionObjectL( iSessionId );
	NSP_SET_STATE( iSession, KStateIndexResolving );
	
    NSP_ASSERT_NO_LEAVE( iController->ClosingSessionL( iSessionId ) );
    NSP_ASSERT( !iController->iSessionArray.Count() );
    NSP_ASSERT( !iController->iClosingSessionArray.Count() );
    
    // Activating
    iSessionId = iController->NewSessionL( *this, KIapId, KTestDomain(), KProtocolUdp );
    EVENT( KErrNone, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    iSession = &iController->FindSessionObjectL( iSessionId );
	NSP_SET_STATE( iSession, KStateIndexActivating );
	
    NSP_ASSERT_NO_LEAVE( iController->ClosingSessionL( iSessionId ) );
    NSP_ASSERT( !iController->iSessionArray.Count() );
    NSP_ASSERT( !iController->iClosingSessionArray.Count() );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::FetchCandFailL
// ---------------------------------------------------------------------------
//
void UT_CNSPUseCases::FetchCandFailL()
    {
    TNatReturnStatus status = KErrNone;
        
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
        
    // New Offer
    CSdpDocument* document = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->CreateOfferL( document ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( document );
    
    NSP_ASSERT_NO_LEAVE( iController->ClosingSessionL( iSession->iSessionId ) );
    
    TInt activeObjectsCount = iController->iActiveObjects.Count();
    for ( TInt i = 0; i < activeObjectsCount; i++  )
        {
        NSP_ASSERT( iController->iActiveObjects[i]->SessionId() != iSessionId );
        }
    
    }

void UT_CNSPUseCases::CreatePullModeOfferL()
	{
	TBuf8<255> address;
    TUint port( 0 );
	    
	CSdpDocument* offer1 = CSdpDocument::DecodeLC( KStunSdpOffer1() );
	CSdpDocument* offer2 = CSdpDocument::DecodeLC( KStunSdpOffer2() );
	NSP_SET_STATE( iSession, KStateIndexConnected );
	iSession->iStreamContainer->CreateStreamsL( *offer1 );
	
	
	NSP_ASSERT_NO_LEAVE( iSession->CreateOfferL( offer2 ) );
	CleanupStack::PopAndDestroy( offer2 );
	CleanupStack::PopAndDestroy( offer1 );
	
	NSP_ASSERT_STATE( iSession, KStateIndexLocalCand );
	
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp, 
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    	   MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("312333"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    	   MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    // Check received NAT offer
    NSP_ASSERT_STATE( iSession, KStateIndexNegotiating );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( iSession->DecodeAnswerL( answer ) );
    CleanupStack::PopAndDestroy( answer );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
             MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
             MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
               MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
               MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
      
      // Check received NAT answer
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    iSession->iStreamContainer->iStreamArray[0]->
              iStreamComponentArray[0]->OutgoingAddress( address, port );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
	
	}
	
// ---------------------------------------------------------------------------
// UT_CNSPUseCases::Reslove_ConnectedL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::Reslove_ConnectedL()
	{
	TBuf8<255> address;
	TUint port( 0 );
	    
	CSdpDocument* offer1 = CSdpDocument::DecodeLC( KStunSdpOffer1() );
	CSdpDocument* offer2 = CSdpDocument::DecodeLC( KStunSdpOffer2() );
	CSdpDocument* offer3 = CSdpDocument::DecodeLC( KStunSdpOffer3() );
	NSP_SET_STATE( iSession, KStateIndexConnected );
	iSession->iStreamContainer->CreateStreamsL( *offer1 );
	
	
	NSP_ASSERT_NO_LEAVE( iSession->ResolveL( offer3, offer2 ) );
	CleanupStack::PopAndDestroy( offer3 );
	CleanupStack::PopAndDestroy( offer2 );
	CleanupStack::PopAndDestroy( offer1 );
	
	NSP_ASSERT_STATE( iSession, KStateIndexLocalCand );
	
	NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
	    		 NSPTestHelper::CreateCandLC(
	    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
	    				 _L8("31233"), KProtocolInetUdp,
	    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
	           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP

    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
	    		 NSPTestHelper::CreateCandLC(
	    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
	    				 _L8("31233"), KProtocolInetUdp,
	    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
	           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
	    
	    // Check received NAT answer
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
	    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
	    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
	    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
	    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );    
	    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
	                iSession->iStreamContainer->iStreamArray[0]->
	                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
	    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
	                iSession->iStreamContainer->iStreamArray[0]->
	                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
	    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
	                iSession->iStreamContainer->iStreamArray[0]->
	                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
	    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
	                iSession->iStreamContainer->iStreamArray[0]->
	                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
	    
	    // Activate sendrecv
	    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
	           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
	    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
	           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
	    
	    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
	           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
	    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
	           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
	    
	    // Check received NAT offer
	    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
	    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
	    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
	    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
	    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
	    iSession->iStreamContainer->iStreamArray[0]->
	            iStreamComponentArray[0]->OutgoingAddress( address, port );// RTP
	    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
	    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
	    iSession->iStreamContainer->iStreamArray[0]->
	            iStreamComponentArray[1]->OutgoingAddress( address, port );// RTCP
	    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
	    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
	}
	
// ---------------------------------------------------------------------------
// UT_CNSPUseCases::AsyncClosing_ConnectedL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::AsyncClosing_ConnectedL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer
    CSdpDocument* document = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->CreateOfferL( document ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( document );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
   		 	 NSPTestHelper::CreateCandLC(
   		 			 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
   		 			 _L8("312333"), KProtocolInetUdp,
   		 			 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]),
    				 _L8("312333"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexNegotiating );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );

    // Answer
    document = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( document ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( document );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // Close session
    iController->ClosingSessionL( iSessionId );
    
    // DeActivate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingDeactivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingDeactivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingDeactivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingDeactivated, KErrNone, NULL ); // RTCP
    
    // Check whether session is deleted
    NSP_ASSERT( !iController->iSessionArray.Count() );
    NSP_ASSERT( !iController->iClosingSessionArray.Count() );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::CreateIceOfferLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::CreateIceOfferLL()
    {
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New offer
    iSession->SetUseIce( ETrue );
    CSdpDocument* document = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->CreateOfferL( document ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( document );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
   		 	 NSPTestHelper::CreateCandLC(
   		 			 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
   		 			 _L8("312333"), KProtocolInetUdp,
   		 			 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(), 
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]),
    				 _L8("312333"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexNegotiating );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( iController->iContentParser->IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::ResolveIceLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::ResolveIceLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer, and Answer
    iSession->SetUseIce( ETrue );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexResolving );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( iController->iContentParser->IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Candidate pairs for streams
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTP
    
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTCP
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
	
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::DecodeIceAnswerLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::DecodeIceAnswerLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpOffer() );
    iSession->SetUseIce( ETrue );
    iSession->iSessionData->SetUpdatedSdp( offer->CloneL() );
    iSession->iSessionData->SetRole( EIceRoleControlled );
	NSP_SET_STATE( iSession, KStateIndexNegotiating );
    iSession->iStreamContainer->CreateStreamsL( *offer );
    CleanupStack::PopAndDestroy( offer );

    // Local candidates
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate
    
    // Answer
    CSdpDocument* answer = CSdpDocument::DecodeLC( KIceSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    
    // Candidate pairs for streams
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTP
    
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTCP
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT Answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::IceUpdateLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::IceUpdateLL()
    {
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and Answer
    iSession->SetUseIce( ETrue );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // #3 Incoming INVITE, with updated SDP.
    CSdpDocument* updateOffer = CSdpDocument::DecodeLC( KIceSdpOfferUpdate1() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( updateOffer ) );
    CleanupStack::PopAndDestroy( updateOffer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexResolving );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( iController->iContentParser->IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Incoming INVITE with updated SDP.
    updateOffer = CSdpDocument::DecodeLC( KIceSdpOfferUpdate2() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( updateOffer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexResolving );
    CleanupStack::PopAndDestroy( updateOffer );
    
    // Candidate pairs for streams
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTP
    
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTCP
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    
    // Incoming INVITE with updated SDP.
    updateOffer = CSdpDocument::DecodeLC( KIceSdpOfferUpdate2() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( updateOffer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    CleanupStack::PopAndDestroy( updateOffer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::IceMtHoldLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::IceMtHoldLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and Answer
    iSession->SetUseIce( ETrue );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexResolving );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( iController->iContentParser->IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Candidate pairs for streams
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTP
    
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTCP
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // New Resolve
    offer = CSdpDocument::DecodeLC( KIceSdpOfferHold() );
    answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *offer ) );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *answer ) );
    NSP_ASSERT( KNatReady == status );
    NSP_ASSERT( !answer->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP(answer)
    NSP_ASSERT( answer->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*answer->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*answer->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP(offer)
    NSP_ASSERT( !address.Compare( offer->ConnectionField()->Address() ) );
    NSP_ASSERT( offer->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP(offer)
    NSP_ASSERT( !RTCPAddr(*offer->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*offer->MediaFields()[0]) == port );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::TestMTMismatchL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::TestMTMismatchL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );

    // New Offer and Answer
    iSession->SetUseIce( ETrue );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpMismatchOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *iDocument ) );
    NSP_ASSERT( iController->iContentParser->IsMismatchL( *iDocument ) );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iController->iContentParser->IsMismatchL( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::TestMOMismatchL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::TestMOMismatchL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // Offer
    iSession->SetUseIce( ETrue );
	NSP_SET_STATE( iSession, KStateIndexNegotiating );
	CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpOffer() );
    iSession->iStreamContainer->CreateStreamsL( *offer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate
    
    // Answer to Offer
    CSdpDocument* answer = CSdpDocument::DecodeLC( KIceSdpMismatchAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );

    // Check that old candidates are removed
    NSP_ASSERT( 0 == iSession->iStreamContainer->iStreamArray[0]->
    		iStreamComponentArray[0]->LocalCandidates().Count() ); // RTP
    NSP_ASSERT( 0 == iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->LocalCandidates().Count() ); // RTCP
    
    // Give new candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Activate sendrecv
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
	
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iController->iContentParser->IsMismatchL( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::TestLocalUpdateL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::TestLocalUpdateL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // Initial Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpUpdatedOffer() );
    iSession->SetUseIce( ETrue );
    iSession->iSessionData->SetUpdatedSdp( offer->CloneL() );
    iSession->iSessionData->SetRole( EIceRoleControlling );
	NSP_SET_STATE( iSession, KStateIndexNegotiating );
    iSession->iStreamContainer->CreateStreamsL( *offer );
    CleanupStack::PopAndDestroy( offer );
    
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				_L8("31233"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				_L8("31232"), KProtocolInetUdp,
    				CNATFWCandidate::ERelay, iSession->iSessionId, ETrue ) ); // RTP
    CleanupStack::Pop();//candidate
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				_L8("31231"), KProtocolInetUdp,
    				CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate
    iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->iLocalCandidates.AppendL(
    		NSPTestHelper::CreateCandLC(
    				(*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				_L8("31230"), KProtocolInetUdp,
    				CNATFWCandidate::ERelay, iSession->iSessionId, ETrue ) ); // RTCP
    CleanupStack::Pop();//candidate
    
    // Answer
    CSdpDocument* answer = CSdpDocument::DecodeLC( KIceSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    
    
    // Candidate pairs for streams
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    			 CNATFWCandidatePair::NewLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->iLocalCandidates[1]),
    					 (*iSession->iSessionData->RemoteCandidates()[0]) )); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTP
    
    NEWCANDPAIR( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    			 NSPTestHelper::CreateCandPairLC(
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue,
    					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    					 _L8("31233"), KProtocolInetUdp,
    					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EConnChecksCompleted, KErrNone, NULL ); // RTCP
    
    // Activate sendrecv
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // Wait for async update
    iUseInternalSchedulerLoop = ETrue;
    CActiveScheduler::Start();
    
    // Check received NAT update
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_UpdateSdp == iLastCall );
    NSP_ASSERT( !iController->iContentParser->IsIceSupported( *iDocument ) );
    const CNATFWCandidate& localrtp = iSession->iStreamContainer->
            iStreamArray[0]->iStreamComponentArray[0]->IceLocalCandidateL();// RTP
    address = localrtp.TransportDomainAddr();
    port = localrtp.TransportDomainPort();
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( port == iDocument->MediaFields()[0]->Port() );
    const CNATFWCandidate& localrtcp = iSession->iStreamContainer->
            iStreamArray[0]->iStreamComponentArray[1]->IceLocalCandidateL();// RTCP
    address = localrtcp.TransportDomainAddr();
    port = localrtcp.TransportDomainPort();
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );
    NSP_ASSERT( port == RTCPPort(*iDocument->MediaFields()[0]) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases::TestTimerLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::TestTimerLL()
    {
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer
    iSession->SetUseIce( ETrue );
    CSdpDocument* document = CSdpDocument::DecodeLC( KStunSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->CreateOfferL( document ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( document );
    
    // Local candidates
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(), 
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("312333"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
   		 	 NSPTestHelper::CreateCandLC(
   		 			 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
   		 			 _L8("31233"), KProtocolInetUdp,
   		 			 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]->StreamId(), 
   		 	NSPTestHelper::CreateCandLC(
   		 			(*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[1]),
   		 			_L8("312333"), KProtocolInetUdp,
   		 			CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    
    // Wait for timer expiration
    iUseInternalSchedulerLoop = ETrue;
    CActiveScheduler::Start();
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexNegotiating );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( iController->iContentParser->IsIceSupported( *iDocument ) );
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Actual events must be ignored.
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check that session state has not changed.
    NSP_ASSERT_STATE( iSession, KStateIndexNegotiating );
    }


// ---------------------------------------------------------------------------
// UT_CNSPUseCases::TestRtcpIcmpIgnoring
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases::TestRtcpIcmpIgnoringL()
	{
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and Answer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Local candidates
	NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
			 NSPTestHelper::CreateCandLC(
					 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]),
					 _L8("31233"), KProtocolInetUdp,
					 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTP

    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL ); // RTCP
    
    // Check received NAT answer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    NSP_ASSERT( ENSP_UT_AnswerReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );    
    NSP_ASSERT( !iDocument->ConnectionField()->Address().Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTP
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainPort() );
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare(
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) ==
                iSession->iStreamContainer->iStreamArray[0]->
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainPort() );
    
    // Activate sendrecv
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTP
    
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::EReceivingActivated, KErrNone, NULL ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
           MNATFWConnectivityObserver::ESendingActivated, KErrNone, NULL ); // RTCP
    
    // Check received NAT offer
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
    NSP_ASSERT( !iController->ContentParser().IsMismatchL( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsLiteAttribute( *iDocument ) );
    NSP_ASSERT( !iController->ContentParser().IsIceSupported( *iDocument ) );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->ConnectionField()->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() == 0 );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );

    // Rtcp stream component fails with KErrCommsLineFail(ICMP) due to keepalive.
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    	   MNATFWConnectivityObserver::EReceivingActivated, KErrCommsLineFail, NULL );
    NSP_ASSERT( ENSP_UT_OfferReady == iLastCall );
	}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPUseCases,
    "Use case tests for session",
    "UNIT" )

NSP_TEST_USECASE(
    "CreateOfferL - test",
    "CNSPUseCases",
    "CreateStunOfferL",
    "FUNCTIONALITY",
    SetupStunL, CreateStunOfferLL, Teardown )

NSP_TEST_USECASE(
    "ResolveL - test",
    "CNSPUseCases",
    "ResolveStunL",
    "FUNCTIONALITY",
    SetupStunL, ResolveStunLL, Teardown )

NSP_TEST_USECASE(
    "DecodeAnswerL - test",
    "CNSPUseCases",
    "DecodeStunAnswerL",
    "FUNCTIONALITY",
    SetupStunL, DecodeStunAnswerLL, Teardown )

NSP_TEST_USECASE(
    "UpdateL - test",
    "CNSPUseCases",
    "StunUpdateL",
    "FUNCTIONALITY",
    SetupStunL, StunUpdateLL, Teardown )

NSP_TEST_USECASE(
    "StunRejectMT - test",
    "CNSPUseCases",
    "StunRejectMTL",
    "FUNCTIONALITY",
    SetupStunL, StunRejectMTL, Teardown )

NSP_TEST_USECASE(
    "StunRejectMO - test",
    "CNSPUseCases",
    "StunRejectMOL",
    "FUNCTIONALITY",
    SetupStunL, StunRejectMOL, Teardown )

NSP_TEST_USECASE(
    "StunCreateOfferModConnField - test",
    "CNSPUseCases",
    "StunCreateOfferModConnFieldL",
    "FUNCTIONALITY",
    SetupStunL, StunCreateOfferModConnFieldL, Teardown )

NSP_TEST_USECASE(
    "StunResolveModConnField - test",
    "CNSPUseCases",
    "StunResolveModConnFieldL",
    "FUNCTIONALITY",
    SetupStunL, StunResolveModConnFieldL, Teardown )

NSP_TEST_USECASE(
    "StunDecodeAnswerModConnField - test",
    "CNSPUseCases",
    "StunDecodeAnswerModConnFieldL",
    "FUNCTIONALITY",
    SetupStunL, StunDecodeAnswerModConnFieldL, Teardown )

NSP_TEST_USECASE(
    "StunMtHold - test",
    "CNSPUseCases",
    "StunMtHoldLL",
    "FUNCTIONALITY",
    SetupStunL, StunMtHoldLL, Teardown )

NSP_TEST_USECASE(
	"StunMtOldwayHold - test",
	"CNSPUseCases",
	"StunMtOldwayHoldLL",
	"FUNCTIONALITY",
	SetupStunL, StunMtOldwayHoldLL, Teardown )

NSP_TEST_USECASE(
    "StunMoAnswerUpdate - test",
    "CNSPUseCases",
    "StunMoAnswerUpdateLL",
    "FUNCTIONALITY",
    SetupStunL, StunMoAnswerUpdateLL, Teardown )

NSP_TEST_USECASE(
    "RemoteAddressUpdate - test",
    "CNSPUseCases",
    "RemoteAddressUpdateLL",
    "FUNCTIONALITY",
    SetupStunL, RemoteAddressUpdateLL, Teardown )

NSP_TEST_USECASE(
    "SyncClosingL - test",
    "CNSPUseCases",
    "SyncClosingL",
    "FUNCTIONALITY",
    SetupStunL, SyncClosingL, Teardown )

NSP_TEST_USECASE(
    "AsyncClosing(connected) - test",
    "CNSPUseCases",
    "AsyncClosing_ConnectedL",
    "FUNCTIONALITY",
    SetupStunL, AsyncClosing_ConnectedL, Teardown )

NSP_TEST_USECASE(
    "CreateOfferL - test",
    "CNSPUseCases",
    "CreateIceOfferL",
    "FUNCTIONALITY",
    SetupIceL, CreateIceOfferLL, Teardown )

NSP_TEST_USECASE(
    "ResolveIceL - test",
    "CNSPUseCases",
    "ResolveIceL",
    "FUNCTIONALITY",
    SetupIceL, ResolveIceLL, Teardown )

NSP_TEST_USECASE(
    "DecodeAnswerL - test",
    "CNSPUseCases",
    "DecodeIceAnswerL",
    "FUNCTIONALITY",
    SetupIceL, DecodeIceAnswerLL, Teardown )

NSP_TEST_USECASE(
    "UpdateL - test",
    "CNSPUseCases",
    "IceUpdateL",
    "FUNCTIONALITY",
    SetupIceL, IceUpdateLL, Teardown )

NSP_TEST_USECASE(
    "IceMtHoldLL - test",
    "CNSPUseCases",
    "IceMtHoldLL",
    "FUNCTIONALITY",
    SetupIceL, IceMtHoldLL, Teardown )

NSP_TEST_USECASE(
    "TestMTMismatch - test",
    "CNSPUseCases",
    "TestMTMismatchL",
    "FUNCTIONALITY",
    SetupIceL, TestMTMismatchL, Teardown )

NSP_TEST_USECASE(
    "TestMOMismatch - test",
    "CNSPUseCases",
    "TestMOMismatchL",
    "FUNCTIONALITY",
    SetupIceL, TestMOMismatchL, Teardown )

NSP_TEST_USECASE(
    "TestLocalUpdateL - test",
    "CNSPUseCases",
    "TestLocalUpdateL",
    "FUNCTIONALITY",
    SetupIceL, TestLocalUpdateL, Teardown )

NSP_TEST_USECASE(
    "Test Timer - test",
    "CNSPUseCases",
    "TestTimer",
    "FUNCTIONALITY",
    SetupIceL, TestTimerLL, Teardown )

NSP_TEST_USECASE(
	"Rtcp Icmp Ignoring - test",
	"CNSPUseCases",
	"TestRtcpIcmpIgnoringL",
	"FUNCTIONALITY",
	SetupStunL, TestRtcpIcmpIgnoringL, Teardown )

NSP_TEST_USECASE(
    "Fetching candidate fail - test",
    "CNSPUseCases",
    "FetchCandFailL",
    "FUNCTIONALITY",
    SetupStunL, FetchCandFailL, Teardown )
    
NSP_TEST_USECASE(
    "CreatePullModeOfferL - test",
    "CNSPUseCases",
    "CreatePullModeOfferL",
    "FUNCTIONALITY",
    SetupStunL, CreatePullModeOfferL, Teardown )
    
NSP_TEST_USECASE(
    "Reslove_ConnectedL - test",
    "CNSPUseCases",
    "Reslove_ConnectedL",
    "FUNCTIONALITY",
    SetupStunL, Reslove_ConnectedL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
