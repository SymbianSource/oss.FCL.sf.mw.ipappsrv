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
#include "nspcontentparser.h"
#include "nspsession.h"
#include "nspstatemachine.h"
#include "nspstatebase.h"
#include "nspmediastreamcontainer.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "nspdefs.h"
#include "ut_nspusecases_fqdn.h"
#include "nsptesthelper.h"
#include "nsptls.h"

_LIT8( KStunSdpOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=audio 2330 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223\r\n\
m=video 2430 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n" );

_LIT8( KStunSdpAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 www.example.com\r\n\
t=0 0\r\n\
m=audio 12322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444\r\n");

_LIT8( KStunSdpResolveOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 www.example2.com\r\n\
t=0 0\r\n\
m=audio 12322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14445 IN IP4 www.example3.com\r\n\
m=video 14322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:16445 IN IP4 www.example.com\r\n");

_LIT8( KStunSdpResolveAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=audio 2332 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223 IN IP4 131.117.0.4\r\n");

_LIT8( KStunSdpOfferModConnField, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 2330 RTP/AVP 96\r\n\
c=IN IP4 10.0.0.6\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223 IN IP4 10.0.0.6\r\n\
m=video 2430 RTP/AVP 96\r\n\
c=IN IP4 10.0.0.6\r\n\
a=rtpmap:96 AMR/8000\r\n" );

_LIT8( KStunSdpAnswerModConnField, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
t=0 0\r\n\
m=audio 12322 RTP/AVP 96\r\n\
c=IN IP4 www.example.com\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n\
m=video 2530 RTP/AVP 96\r\n\
c=IN IP4 www.example2.com\r\n\
a=rtpmap:96 AMR/8000\r\n" );

_LIT8( KIceSdpOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 www.example.com\r\n\
t=0 0\r\n\
m=audio 20330 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 www.example.com 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 www.example.com 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:20332 IN IP4 www.example.com\r\n" );

_LIT8( KIceSdpOfferHold, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 193.65.183.237\r\n\
t=0 0\r\n\
m=audio 100 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 www.example.com 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 www.example.com 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:101 IN IP4 193.65.183.237\r\n" );

_LIT8( KIceSdpAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.7\r\n\
s=-\r\n\
c=IN IP4 www.example2.com\r\n\
t=0 0\r\n\
m=audio 20330 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 www.example2.com 20330 typ host\r\n\
a=candidate:3123 2 udp 200535 www.example2.com 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:20332 IN IP4 131.231.0.197\r\n" );

_LIT8( KIceSdpUpdatedOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 www.example.com\r\n\
t=0 0\r\n\
m=audio 20330 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 www.example.com 20330 typ host\r\n\
a=candidate:4123 1 udp 190534 www.example2.com 2330 typ srflx\r\n\
a=candidate:3123 2 udp 200535 www.example.com 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:20332 IN IP4 www.example.com\r\n" );

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::RTCPAddr
// ---------------------------------------------------------------------------
// 
TDesC8& UT_CNSPUseCases_fqdn::RTCPAddr( CSdpMediaField& aMediaField )
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
// UT_CNSPUseCases_fqdn::RTCPPort
// ---------------------------------------------------------------------------
// 
TUint UT_CNSPUseCases_fqdn::RTCPPort( CSdpMediaField& aMediaField )
    {
    TUint port( 0 );
    TBuf8<255> address;
    iController->iContentParser->FindRTCP( aMediaField, address, port );
    return port;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPUseCases_fqdn* UT_CNSPUseCases_fqdn::NewL()
    {
    UT_CNSPUseCases_fqdn* self = UT_CNSPUseCases_fqdn::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPUseCases_fqdn* UT_CNSPUseCases_fqdn::NewLC()
    {
    UT_CNSPUseCases_fqdn* self = new( ELeave ) UT_CNSPUseCases_fqdn();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::~UT_CNSPUseCases_fqdn
// ---------------------------------------------------------------------------
// 
UT_CNSPUseCases_fqdn::~UT_CNSPUseCases_fqdn()
    {
    iRtcpAddr.Close();
    delete iCenrep;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::UT_CNSPUseCases_fqdn
// ---------------------------------------------------------------------------
// 
UT_CNSPUseCases_fqdn::UT_CNSPUseCases_fqdn()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::ConstructL()
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
// UT_CNSPUseCases_fqdn::SetupStunL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::SetupStunL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    NSPTestHelper::SetCenrepTDesC8toAProtocolL( *iCenrep,
            iCurrentDomainKey, KNokiaStun() );
    iController = CNSPController::NewL();
    iSessionId = iController->NewSessionL( *this, KIapId, KTestDomain(), KProtocolUdp );
    iSession = &iController->FindSessionObjectL( iSessionId );
    iLastError = KErrNone;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::SetupIceL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::SetupIceL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    NSPTestHelper::SetCenrepTDesC8toAProtocolL( *iCenrep,
            iCurrentDomainKey, KNokiaAll() );
    iController = CNSPController::NewL();
    iSessionId = iController->NewSessionL( *this, KIapId, KTestDomain(), KProtocolUdp );
    iSession = &iController->FindSessionObjectL( iSessionId );
    iLastError = KErrNone;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::Teardown()
    {
    NSP_DEL( iDocument );
    NSP_DEL( iController );
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// OBSERVER:
// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::Initialized
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::Initialized( TUint aSessionId )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_Initialized;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::OfferReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::OfferReady( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_OfferReady;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::AnswerReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::AnswerReady( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_AnswerReady;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::UpdateSdp
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::UpdateSdp( TUint aSessionId, CSdpDocument* aDocument )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_UpdateSdp;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    NSP_DEL( iDocument );
    iDocument = aDocument;
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::ErrorOccurred( TUint aSessionId, TInt aError )
    {
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_ErrorOccurred;
    iLastError = aError;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::IcmpErrorOccurred( TUint aSessionId, TInt aError )
	{
    NSP_ASSERT( iSession->iSessionId == aSessionId );
    iLastCall = ENSP_UT_IcmpErrorOccurred;
    iLastError = aError;
    NSP_STOPINTERNALTIMERLOOP( iUseInternalSchedulerLoop );
	}



// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::CreateStunOfferLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::CreateStunOfferLL()
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
    				 _L8("31233"), KProtocolInetUdp, 
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
// UT_CNSPUseCases_fqdn::ResolveStunLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::ResolveStunLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
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
// UT_CNSPUseCases_fqdn::DecodeStunAnswerLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::DecodeStunAnswerLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
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
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->DecodeAnswerL( answer ) );
    NSP_ASSERT( KNatAsync == status );
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
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::StunUpdateLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::StunUpdateLL()
    {
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and answer
    CSdpDocument* initialOffer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    CSdpDocument* initialAnswer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( initialOffer, initialAnswer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( initialAnswer );
    CleanupStack::PopAndDestroy( initialOffer );
    
    // Incoming INVITE with updated SDP.
    CSdpDocument* updateOffer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
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
    updateOffer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( updateOffer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    CleanupStack::PopAndDestroy( updateOffer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::StunCreateOfferModConnFieldL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::StunCreateOfferModConnFieldL()
    {
    TBuf8<255> address;
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
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
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::ERelay, iSession->iSessionId, ETrue ) ); // RTP
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
// UT_CNSPUseCases_fqdn::StunResolveModConnFieldL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::StunResolveModConnFieldL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and Answer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOfferModConnField() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
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
                iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );
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
    NSP_ASSERT( !address.Compare( iDocument->MediaFields()[0]->ConnectionFields()[0]->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::StunDecodeAnswerModConnFieldL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::StunDecodeAnswerModConnFieldL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
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
    NSP_ASSERT( !address.Compare( iDocument->MediaFields()[0]->ConnectionFields()[0]->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[0]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[1]->
            iStreamComponentArray[0]->OutgoingAddress( address, port ) );// RTP
    NSP_ASSERT( !address.Compare( iDocument->MediaFields()[1]->ConnectionFields()[0]->Address() ) );
    NSP_ASSERT( iDocument->MediaFields()[1]->Port() == port );
    User::LeaveIfError( iSession->iStreamContainer->iStreamArray[0]->
            iStreamComponentArray[1]->OutgoingAddress( address, port ) );// RTCP
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::StunMtHoldLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::StunMtHoldLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
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
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // New Resolve
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
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP(answer)
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
// UT_CNSPUseCases_fqdn::RemoteAddressUpdateLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::RemoteAddressUpdateLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
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
    
    // Answer to Offer
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
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // #5 Update remote addresses
    CSdpDocument* resolveoffer = CSdpDocument::DecodeLC( KStunSdpResolveOffer() );
    CSdpDocument* resolveanswer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( resolveoffer, resolveanswer ) );
    NSP_ASSERT( KNatReady == status );
    NSP_ASSERT_STATE( iSession, KStateIndexConnected );
    CleanupStack::PopAndDestroy( resolveanswer );
    CleanupStack::PopAndDestroy( resolveoffer );
    
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
// UT_CNSPUseCases_fqdn::CreateIceOfferLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::CreateIceOfferLL()
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
	EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
	
    NEWCAND( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(), 
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]),
    				 _L8("31233"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTP
    EVENT( iSession->iStreamContainer->iStreamArray[1]->iStreamComponentArray[0]->StreamId(),
           MNATFWConnectivityObserver::EFetchingCompleted, KErrNone, NULL );
    
    NEWCAND( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
    		 NSPTestHelper::CreateCandLC(
    				 (*iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]),
    				 _L8("312333"), KProtocolInetUdp,
    				 CNATFWCandidate::EServerReflexive, iSession->iSessionId, ETrue ) ); // RTCP
    EVENT( iSession->iStreamContainer->iStreamArray[0]->iStreamComponentArray[1]->StreamId(),
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
// UT_CNSPUseCases_fqdn::ResolveIceLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::ResolveIceLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and answer
    iSession->SetUseIce( ETrue );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpOffer() );
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
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::DecodeIceAnswerLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::DecodeIceAnswerLL()
    {
    TBuf8<255> address;
    TUint port( 0 );
    TNatReturnStatus status = KErrNone;
    
    // Initial Offer
    CSdpDocument* offer = CSdpDocument::DecodeLC( KStunSdpOffer() );
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
    
    // Answer to Offer
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
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
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
// UT_CNSPUseCases_fqdn::IceUpdateLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::IceUpdateLL()
    {
    TNatReturnStatus status = KErrNone;
    
    // Initialized
    EVENT( iSessionId, MNATFWConnectivityObserver::ESessionCreated, KErrNone, NULL );
    NSP_ASSERT_STATE( iSession, KStateIndexIdle );
    
    // New Offer and Answer
    iSession->SetUseIce( ETrue );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceSdpOffer() );
    CSdpDocument* answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    CleanupStack::PopAndDestroy( offer );
    
    // Incoming INVITE with updated SDP.
    CSdpDocument* updateOffer = CSdpDocument::DecodeLC( KIceSdpUpdatedOffer() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( updateOffer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexLocalCand );
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
    updateOffer = CSdpDocument::DecodeLC( KIceSdpUpdatedOffer() );
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
    
    // Incoming INVITE with updated SDP.
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) );
    updateOffer = CSdpDocument::DecodeLC( KIceSdpUpdatedOffer() );
    NSP_ASSERT_NO_LEAVE( iSession->UpdateL( updateOffer ) );
    NSP_ASSERT_STATE( iSession, KStateIndexActivating );
    CleanupStack::PopAndDestroy( updateOffer );
    }

// ---------------------------------------------------------------------------
// UT_CNSPUseCases_fqdn::IceMtHoldLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPUseCases_fqdn::IceMtHoldLL()
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
    NSP_ASSERT( !RTCPAddr(*iDocument->MediaFields()[0]).Compare( address ) );// RTCP
    NSP_ASSERT( RTCPPort(*iDocument->MediaFields()[0]) == port );
    
    // #8 New Resolve
    offer = CSdpDocument::DecodeLC( KIceSdpOfferHold() );
    answer = CSdpDocument::DecodeLC( KStunSdpResolveAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iSession->ResolveL( offer, answer ) );
    
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
                iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );// RTCP(answer)
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
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPUseCases_fqdn,
    "Use case tests for session, fqdn related",
    "UNIT" )

NSP_TEST_USECASE(
    "CreateOfferL - test",
    "CNSPUseCases_fqdn",
    "CreateStunOfferL",
    "FUNCTIONALITY",
    SetupStunL, CreateStunOfferLL, Teardown )

NSP_TEST_USECASE(
    "ResolveL - test",
    "CNSPUseCases_fqdn",
    "ResolveStunL",
    "FUNCTIONALITY",
    SetupStunL, ResolveStunLL, Teardown )

NSP_TEST_USECASE(
    "DecodeAnswerL - test",
    "CNSPUseCases_fqdn",
    "DecodeStunAnswerL",
    "FUNCTIONALITY",
    SetupStunL, DecodeStunAnswerLL, Teardown )

NSP_TEST_USECASE(
    "UpdateL - test",
    "CNSPUseCases_fqdn",
    "StunUpdateL",
    "FUNCTIONALITY",
    SetupStunL, StunUpdateLL, Teardown )

NSP_TEST_USECASE(
    "StunCreateOfferModConnField - test",
    "CNSPUseCases_fqdn",
    "StunCreateOfferModConnFieldL",
    "FUNCTIONALITY",
    SetupStunL, StunCreateOfferModConnFieldL, Teardown )

NSP_TEST_USECASE(
    "StunResolveModConnField - test",
    "CNSPUseCases_fqdn",
    "StunResolveModConnFieldL",
    "FUNCTIONALITY",
    SetupStunL, StunResolveModConnFieldL, Teardown )

NSP_TEST_USECASE(
    "StunDecodeAnswerModConnField - test",
    "CNSPUseCases_fqdn",
    "StunDecodeAnswerModConnFieldL",
    "FUNCTIONALITY",
    SetupStunL, StunDecodeAnswerModConnFieldL, Teardown )

NSP_TEST_USECASE(
    "StunMtHold - test",
    "CNSPUseCases_fqdn",
    "StunMtHoldLL",
    "FUNCTIONALITY",
    SetupStunL, StunMtHoldLL, Teardown )

NSP_TEST_USECASE(
    "RemoteAddressUpdate - test",
    "CNSPUseCases_fqdn",
    "RemoteAddressUpdateLL",
    "FUNCTIONALITY",
    SetupStunL, RemoteAddressUpdateLL, Teardown )

NSP_TEST_USECASE(
    "CreateOfferL - test",
    "CNSPUseCases_fqdn",
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
    "CNSPUseCases_fqdn",
    "DecodeIceAnswerL",
    "FUNCTIONALITY",
    SetupIceL, DecodeIceAnswerLL, Teardown )

NSP_TEST_USECASE(
    "UpdateL - test",
    "CNSPUseCases_fqdn",
    "IceUpdateL",
    "FUNCTIONALITY",
    SetupIceL, IceUpdateLL, Teardown )

NSP_TEST_USECASE(
    "IceMtHoldLL - test",
    "CNSPUseCases_fqdn",
    "IceMtHoldLL",
    "FUNCTIONALITY",
    SetupIceL, IceMtHoldLL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
