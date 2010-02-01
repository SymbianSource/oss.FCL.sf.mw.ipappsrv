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
#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include "nsppluginreturndef.h"
#include <in_sock.h>
#include "natfwcredentials.h"
#include "nspcontroller.h"
#include "nspmediastreamcontainer.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "nsputil.h"
#include "ut_nspmediastreamcontainer.h"
#include "nsptesthelper.h"
#include "nsptls.h"

const TUint KSessionId = 10;

_LIT8( KStunSdpOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=audio 2330 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223 IN IP4 10.0.0.6\r\n" );

_LIT8( KInitialSdpOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=audio 2330 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:13223 IN IP4 10.0.0.6\r\n\
m=video 12330 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n" );

_LIT8( KInitialSdpAnswer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.4\r\n\
t=0 0\r\n\
m=video 12322 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n");

_LIT8( KIceOffer, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.4\r\n\
t=0 0\r\n\
a=ice-ufrag:eFPH\r\n\
a=ice-pwd:3/Ga2FvUrVmUvO1w5gsSJ/zI\r\n\
m=video 12322 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.196 20330 typ host\r\n\
a=candidate:4123 1 udp 190534 193.65.183.237 2330 typ srflx\r\n\
a=rtpmap:96 AMR/8000\r\n");

_LIT8( KIceOffer2, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 131.117.0.4\r\n\
t=0 0\r\n\
a=ice-ufrag:eFPq\r\n\
a=ice-pwd:3/Ga2F333rVmUvO1w5gsSJ/zI\r\n\
m=video 12322 RTP/AVP 96\r\n\
a=candidate:3123 1 udp 200534 131.231.0.196 20330 typ host\r\n\
a=candidate:4123 1 udp 190534 193.65.183.237 2330 typ srflx\r\n\
a=candidate:3123 2 udp 200535 131.231.0.196 20332 typ host\r\n\
a=rtpmap:96 AMR/8000\r\n");



// METHODS:
// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStreamContainer* UT_CNSPMediaStreamContainer::NewL()
    {
    UT_CNSPMediaStreamContainer* self = UT_CNSPMediaStreamContainer::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStreamContainer* UT_CNSPMediaStreamContainer::NewLC()
    {
    UT_CNSPMediaStreamContainer* self = new( ELeave ) UT_CNSPMediaStreamContainer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::~UT_CNSPMediaStreamContainer
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStreamContainer::~UT_CNSPMediaStreamContainer()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::UT_CNSPMediaStreamContainer
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStreamContainer::UT_CNSPMediaStreamContainer()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamContainer::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::SetupL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamContainer::SetupL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    iController = CNSPController::NewL();
    iContainer = CNSPMediaStreamContainer::NewL(
            *iController, KSessionId, KProtocolUdp );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamContainer::Teardown()
    {
    delete iContainer;
    delete iController;
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::UT_CreateStreamsL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamContainer::UT_CreateStreamsL()
    {
    TNatReturnStatus status = KNatReady;
    
    // OK
    CSdpDocument* offer = CSdpDocument::DecodeLC( KInitialSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iContainer->CreateStreamsL( *offer ) );
    NSP_ASSERT( 2 == iContainer->iStreamArray.Count() );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( offer );
    
    CSdpDocument* offer2 = CSdpDocument::DecodeLC( KInitialSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iContainer->CreateStreamsL( *offer2 ) );
    NSP_ASSERT( 2 == iContainer->iStreamArray.Count() );
    NSP_ASSERT( KNatReady == status );
    CleanupStack::PopAndDestroy( offer2 );
    
    // OK
    CSdpDocument* answer = CSdpDocument::DecodeLC( KInitialSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iContainer->CreateStreamsL( *answer ) );
    NSP_ASSERT( 2 == iContainer->iStreamArray.Count() );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    
    offer = NULL;
    status = KErrNotFound;
    iContainer->iStreamArray.ResetAndDestroy();
    
    // FAILURE DUE TO NULL POINTER
    NSP_ASSERT_LEAVE( status = iContainer->CreateStreamsL( *offer ) );
    NSP_ASSERT( 0 == iContainer->iStreamArray.Count() );
    NSP_ASSERT( KErrNotFound == status );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::UT_RemoveStreamsL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamContainer::UT_RemoveStreamsL()
    {
    TNatReturnStatus status = KNatReady;
    
    // OK
    CSdpDocument* offer = CSdpDocument::DecodeLC( KInitialSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iContainer->CreateStreamsL( *offer ) );
    NSP_ASSERT( 2 == iContainer->iStreamArray.Count() );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( offer );
    
    // OK
    CSdpDocument* answer = CSdpDocument::DecodeLC( KInitialSdpAnswer() );
    NSP_ASSERT_NO_LEAVE( status = iContainer->RemoveStreamsL( *answer ) );
    NSP_ASSERT( 1 == iContainer->iStreamArray.Count() );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( answer );
    
    offer = NULL;
    status = KErrNotFound;
    iContainer->iStreamArray.ResetAndDestroy();
    
    // FAILURE DUE TO NULL POINTER
    NSP_ASSERT_LEAVE( status = iContainer->RemoveStreamsL( *offer ) );
    NSP_ASSERT( 0 == iContainer->iStreamArray.Count() );
    NSP_ASSERT( KErrNotFound == status );    
    }

void UT_CNSPMediaStreamContainer::UT_RemoveStreamL()
    {
    TNatReturnStatus status = KNatReady;
    
    CSdpDocument* offer = CSdpDocument::DecodeLC( KInitialSdpOffer() );
    NSP_ASSERT_NO_LEAVE( status = iContainer->CreateStreamsL( *offer ) );
    NSP_ASSERT( 2 == iContainer->iStreamArray.Count() );
    NSP_ASSERT( KNatAsync == status );
    CleanupStack::PopAndDestroy( offer );
    
    CSdpDocument* answer1 = CSdpDocument::DecodeLC( KStunSdpOffer() );
    RPointerArray<CSdpMediaField>& mediafield1 = answer1->MediaFields();
    NSP_ASSERT( mediafield1.Count() > 0 );
    CNSPMediaStream* stream = NSPUtil::FindMediaStream ( 
	                         *mediafield1[0], iContainer->iStreamArray );
    NSP_ASSERT_NO_LEAVE ( status = iContainer->RemoveStream( stream ) );
    NSP_ASSERT( KErrNone == status );
    NSP_ASSERT( 1 == iContainer->iStreamArray.Count() );
    CleanupStack::PopAndDestroy( answer1 );
    
    CSdpDocument* answer2 = CSdpDocument::DecodeLC( KInitialSdpAnswer() );
    RPointerArray<CSdpMediaField>& mediafield2 = answer2->MediaFields();
    NSP_ASSERT( mediafield2.Count() > 0 );
    stream = NSPUtil::FindMediaStream ( *mediafield2[0], 
    		                             iContainer->iStreamArray );
    NSP_ASSERT_NO_LEAVE( status = iContainer->RemoveStream( stream ) );
    NSP_ASSERT( KErrNone == status );
    NSP_ASSERT( 0 == iContainer->iStreamArray.Count() );
    CleanupStack::PopAndDestroy( answer2 );
    
    }
// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::UT_GetCandidatesL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamContainer::UT_GetCandidatesL()
    {
    TNatReturnStatus status = KNatReady;
    RPointerArray<CNATFWCandidate> candidates;
    CleanupStack::PushL( TCleanupItem(
    		NSPUtil::CleanupArrayItem, &candidates ) );
    
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceOffer() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContainer->CreateStreamsL( *offer ) );
    NSP_ASSERT_NO_LEAVE( status = iContainer->GetCandidatesL( *offer, candidates ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( 2 == candidates.Count() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( status = iContainer->GetCandidatesL( *offer, candidates ) );
    NSP_ASSERT( KNatReady == status );
    NSP_ASSERT( 2 == candidates.Count() );
    
    CleanupStack::PopAndDestroy( offer );
    offer = CSdpDocument::DecodeLC( KIceOffer2() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( status = iContainer->GetCandidatesL( *offer, candidates ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( 3 == candidates.Count() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( status = iContainer->GetCandidatesL( *offer, candidates ) );
    NSP_ASSERT( KNatReady == status );
    NSP_ASSERT( 3 == candidates.Count() );
    
    CleanupStack::PopAndDestroy( offer );
    CleanupStack::PopAndDestroy( &candidates );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStreamContainer::UT_GetCredentialsL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStreamContainer::UT_GetCredentialsL()
    {
    TNatReturnStatus status = KNatReady;
    CNATFWCredentials* credentials = CNATFWCredentials::NewLC();
    credentials->SetDirection( CNATFWCredentials::EOutbound );
    CSdpDocument* offer = CSdpDocument::DecodeLC( KIceOffer() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContainer->CreateStreamsL( *offer ) );
    NSP_ASSERT_NO_LEAVE( status = iContainer->GetCredentialsL( *offer, *credentials ) );
    NSP_ASSERT( KNatAsync == status );
    
    // OK
    NSP_ASSERT_NO_LEAVE( status = iContainer->GetCredentialsL( *offer, *credentials ) );
    NSP_ASSERT( KNatReady == status );
    
    CleanupStack::PopAndDestroy( offer );
    offer = CSdpDocument::DecodeLC( KIceOffer2() );
    
    // OK
    NSP_ASSERT_NO_LEAVE( status = iContainer->GetCredentialsL( *offer, *credentials ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT_NO_LEAVE( status = iContainer->GetCredentialsL( *offer, *credentials ) );
    NSP_ASSERT( KNatReady == status );
    
    CleanupStack::PopAndDestroy( offer );
    CleanupStack::PopAndDestroy( credentials );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPMediaStreamContainer,
    "Individual method tests for stream container",
    "UNIT" )

NSP_TEST_METHOD(
    "CreateStreamsL - test",
    "CNSPMediaStreamContainer",
    "UT_CreateStreamsL",
    "FUNCTIONALITY",
    SetupL, UT_CreateStreamsL, Teardown )

NSP_TEST_METHOD(
    "RemoveStreamsL - test",
    "CNSPMediaStreamContainer",
    "UT_RemoveStreamsL",
    "FUNCTIONALITY",
    SetupL, UT_RemoveStreamsL, Teardown )
    
NSP_TEST_METHOD(
    "RemoveStreamL - test",
    "CNSPMediaStreamContainer",
    "UT_RemoveStreamL",
    "FUNCTIONALITY",
    SetupL, UT_RemoveStreamL, Teardown )

NSP_TEST_METHOD(
    "GetCandidatesL - test",
    "CNSPMediaStreamContainer",
    "UT_GetCandidatesL",
    "FUNCTIONALITY",
    SetupL, UT_GetCandidatesL, Teardown )

NSP_TEST_METHOD(
    "GetCredentialsL - test",
    "CNSPMediaStreamContainer",
    "UT_GetCredentialsL",
    "FUNCTIONALITY",
    SetupL, UT_GetCredentialsL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
