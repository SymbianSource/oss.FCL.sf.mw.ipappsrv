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
#include <sdpdocument.h>
#include <sdpmediafield.h>
#include "nsppluginreturndef.h"
#include <in_sock.h>
#include "nspcontroller.h"
#include "nspcontentparser.h"
#include "nspsession.h"
#include "nspmediastreamcontainer.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "s_natfwsocketmediaconnwrapper.h"
#include "../stubs/inc/natfwconnectivityapi.h"
#include "ut_nspmediastream.h"
#include "nsptesthelper.h"
#include "nsptls.h"

const TUint KSessionId = 10;
const TUint KCollectionId = 200;

_LIT8( KAddress, "127.0.0.1" );
_LIT8( KAddressLongAddr, "131.117.0.4" );
_LIT8( KMediaField, "m=audio 2330 RTP/AVP 96\r\n" );
_LIT8( KModMediaField, "m=audio 2330 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n" );
_LIT8( KModMediaFieldwithRtcp, "m=audio 2330 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:17777\r\n" );
_LIT8( KModMediaFieldwithRtcpLongAddr, "m=audio 2550 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:17777 IN IP4 131.117.0.4\r\n" );
_LIT8( KModMediaFieldwithConnField, "m=audio 2850 RTP/AVP 96\r\n\
c=IN IP4 131.117.0.4\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:17777 IN IP4 127.0.0.1\r\n" );


// METHODS:
// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::RTCPAddr
// ---------------------------------------------------------------------------
// 
TDesC8& UT_CNSPMediaStream::RTCPAddr( CSdpMediaField& aMediaField )
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
// UT_CNSPMediaStream::RTCPPort
// ---------------------------------------------------------------------------
// 
TUint UT_CNSPMediaStream::RTCPPort( CSdpMediaField& aMediaField )
    {
    TUint port( 0 );
    TBuf8<255> address;
    iController->iContentParser->FindRTCP( aMediaField, address, port );
    return port;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStream* UT_CNSPMediaStream::NewL()
    {
    UT_CNSPMediaStream* self = UT_CNSPMediaStream::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStream* UT_CNSPMediaStream::NewLC()
    {
    UT_CNSPMediaStream* self = new( ELeave ) UT_CNSPMediaStream();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::~UT_CNSPMediaStream
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStream::~UT_CNSPMediaStream()
    {
    delete iCenrep;
    iRtcpAddr.Close();
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::UT_CNSPMediaStream
// ---------------------------------------------------------------------------
// 
UT_CNSPMediaStream::UT_CNSPMediaStream()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::ConstructL()
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
// UT_CNSPMediaStream::SetupL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::SetupL()
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
    CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KMediaField() );
    iStream = CNSPMediaStream::NewL(
            *iController, *mediafield, KSessionId, KCollectionId,
            KAddress(), KProtocolUdp, KMediaTos );
    iSession->iStreamContainer->iStreamArray.AppendL( iStream );
    CleanupStack::PopAndDestroy( mediafield );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::Teardown()
    {
    delete iController;
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// OBSERVER:
// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::Initialized
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::Initialized( TUint /*aSessionId*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::OfferReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::OfferReady( TUint /*aSessionId*/, CSdpDocument* /*aDocument*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::AnswerReady
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::AnswerReady( TUint /*aSessionId*/, CSdpDocument* /*aDocument*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::UpdateSdp
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::UpdateSdp( TUint /*aSessionId*/, CSdpDocument* /*aDocument*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::ErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::ErrorOccurred( TUint /*aSessionId*/, TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::IcmpErrorOccurred
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::IcmpErrorOccurred( TUint /*aSessionId*/, TInt /*aError*/ )
	{	
	}



// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::UT_NewL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::UT_NewL()
    {
    TBuf<256> addr;
    TInetAddr address;
    addr.Copy( KAddress() );
    address.Input( addr );
    
    // OK
    NSP_ASSERT( 2 == iStream->iStreamComponentArray.Count() );
    NSP_ASSERT( iController->iInterface->iWrappers[0]->iIncomingAddress.Match( address ) );
    NSP_ASSERT( 2330 == iController->iInterface->iWrappers[0]->iIncomingAddress.Port() );
    NSP_ASSERT( iController->iInterface->iWrappers[1]->iIncomingAddress.Match( address ) );
    NSP_ASSERT( 2331 == iController->iInterface->iWrappers[1]->iIncomingAddress.Port() );
    
    iStream = NULL;
    iSession->iStreamContainer->iStreamArray.ResetAndDestroy();
    iController->iInterface->iWrappers.ResetAndDestroy();
    
    CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KModMediaFieldwithRtcpLongAddr() );
    iStream = CNSPMediaStream::NewLC(
            *iController, *mediafield, KSessionId, KCollectionId,
            KAddress(), KProtocolUdp, KMediaTos );
    iSession->iStreamContainer->iStreamArray.AppendL( iStream );
    CleanupStack::Pop( iStream );
    CleanupStack::PopAndDestroy( mediafield );
    
    // OK
    NSP_ASSERT( 2 == iStream->iStreamComponentArray.Count() );
    NSP_ASSERT( iController->iInterface->iWrappers[0]->iIncomingAddress.Match( address ) );
    NSP_ASSERT( 2550 == iController->iInterface->iWrappers[0]->iIncomingAddress.Port() );
    
    addr.Copy( KAddressLongAddr() );
    address.Input( addr );
    
    // OK
    NSP_ASSERT( iController->iInterface->iWrappers[1]->iIncomingAddress.Match( address ) );
    NSP_ASSERT( 17777 == iController->iInterface->iWrappers[1]->iIncomingAddress.Port() );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::UT_ModifyStunConnL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::UT_ModifyStunConnL()
    {
    TBuf8<255> address;
    
    // FAILURE DUE TO LACK OF LOCAL CANDIDATES
	CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KModMediaField() );
    NSP_ASSERT_LEAVE( iStream->ModifyStunConnL( *mediafield, address ) );
    CleanupStack::PopAndDestroy( mediafield );
    
    // Local candidates
    CNATFWCandidate* candidate = NSPTestHelper::CreateCandL(
    		*iStream->iStreamComponentArray[0],
    		_L8("31233"), KProtocolInetUdp,
    		CNATFWCandidate::ERelay, KSessionId, ETrue );
    CleanupStack::PushL( candidate );
    iStream->iStreamComponentArray[0]->iLocalCandidates.AppendL( candidate );
    CleanupStack::Pop( candidate );
    candidate = NSPTestHelper::CreateCandL(
    		*iStream->iStreamComponentArray[1],
    		_L8("31233"), KProtocolInetUdp,
    		CNATFWCandidate::ERelay, KSessionId, ETrue );
    CleanupStack::PushL( candidate );
    iStream->iStreamComponentArray[1]->iLocalCandidates.AppendL( candidate );
    CleanupStack::Pop( candidate );
    
    // OK
    mediafield = CSdpMediaField::DecodeLC( KModMediaFieldwithRtcp() );
    NSP_ASSERT_NO_LEAVE( iStream->ModifyStunConnL( *mediafield, address ) );
    NSP_ASSERT( !address.Compare(
            iStream->iStreamComponentArray[0]->iLocalCandidates[0]->TransportDomainAddr() ) );
    NSP_ASSERT( iStream->iStreamComponentArray[0]->StreamId() == mediafield->Port() );
    NSP_ASSERT( !RTCPAddr( *mediafield ).Compare(
            iStream->iStreamComponentArray[1]->iLocalCandidates[0]->TransportDomainAddr() ) );
    NSP_ASSERT( iStream->iStreamComponentArray[1]->StreamId() == RTCPPort( *mediafield ) );
    CleanupStack::PopAndDestroy( mediafield );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::UT_ModifyIceConnL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::UT_ModifyIceConnL()
    {
    TBuf8<255> address;
    
    // FAILURE DUE TO LACK OF CANDIDATE PAIRS
	CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KModMediaField() );
    NSP_ASSERT_LEAVE( iStream->ModifyStunConnL( *mediafield, address ) );
    CleanupStack::PopAndDestroy( mediafield );
    
    // Candidate pairs
    iStream->iStreamComponentArray[0]->iCandidatePair = 
            NSPTestHelper::CreateCandPairL(
                    (*iStream->iStreamComponentArray[0]),
                    _L8("31233"), KProtocolInetUdp,
                    CNATFWCandidate::EServerReflexive, KSessionId, ETrue,
                    (*iStream->iStreamComponentArray[0]),
                    _L8("31233"), KProtocolInetUdp,
                    CNATFWCandidate::EServerReflexive, KSessionId, ETrue );
    iStream->iStreamComponentArray[1]->iCandidatePair = 
            NSPTestHelper::CreateCandPairL(
                    (*iStream->iStreamComponentArray[1]),
                    _L8("31233"), KProtocolInetUdp,
                    CNATFWCandidate::EServerReflexive, KSessionId, ETrue,
                    (*iStream->iStreamComponentArray[1]),
                    _L8("31233"), KProtocolInetUdp,
                    CNATFWCandidate::EServerReflexive, KSessionId, ETrue );
    
    // OK
    mediafield = CSdpMediaField::DecodeLC( KModMediaField() );
    NSP_ASSERT_NO_LEAVE( iStream->ModifyIceConnL( *mediafield,  address ) );
    NSP_ASSERT( !address.Compare(
            iStream->iStreamComponentArray[0]->IceLocalCandidateL().TransportDomainAddr() ) );
    NSP_ASSERT( iStream->iStreamComponentArray[0]->StreamId() == mediafield->Port() );
    CleanupStack::PopAndDestroy( mediafield );
    
    // OK( with RTCP )
    mediafield = CSdpMediaField::DecodeLC( KModMediaFieldwithRtcp() );
    NSP_ASSERT_NO_LEAVE( iStream->ModifyIceConnL( *mediafield,  address ) );
    NSP_ASSERT( !address.Compare(
            iStream->iStreamComponentArray[0]->IceLocalCandidateL().TransportDomainAddr() ) );
    NSP_ASSERT( !RTCPAddr( *mediafield ).Compare(
            iStream->iStreamComponentArray[1]->IceLocalCandidateL().TransportDomainAddr() ) );
    NSP_ASSERT( iStream->iStreamComponentArray[1]->StreamId() == RTCPPort( *mediafield ) );
    CleanupStack::PopAndDestroy( mediafield );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::UT_ModifyLocalConnL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::UT_ModifyLocalConnL()
    {
    _LIT8( KLoopback, "127.0.0.1" );
    TBuf8<255> address;
    
    // OK
    CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KModMediaField() );
    NSP_ASSERT_NO_LEAVE( iStream->ModifyLocalConnL( *mediafield, address ) );
    NSP_ASSERT( !address.Compare( KLoopback() ) );
    NSP_ASSERT( iStream->iStreamComponentArray[0]->StreamId() == mediafield->Port() );
    CleanupStack::PopAndDestroy( mediafield );
    
    // OK
    mediafield = CSdpMediaField::DecodeLC( KModMediaFieldwithRtcp() );
    NSP_ASSERT_NO_LEAVE( iStream->ModifyLocalConnL( *mediafield, address ) );
    NSP_ASSERT( !address.Compare( KLoopback() ) );
    NSP_ASSERT( iStream->iStreamComponentArray[0]->StreamId() == mediafield->Port() );
    NSP_ASSERT( !RTCPAddr( *mediafield ).Compare( KLoopback() ) );
    NSP_ASSERT( iStream->iStreamComponentArray[1]->StreamId() == RTCPPort( *mediafield ) );
    CleanupStack::PopAndDestroy( mediafield );
    }

// ---------------------------------------------------------------------------
// UT_CNSPMediaStream::UT_MediaConnInfoL
// ---------------------------------------------------------------------------
// 
void UT_CNSPMediaStream::UT_MediaConnInfoL()
    {
    _LIT8( KRemoteAddress1, "192.168.0.14" );
    _LIT8( KRemoteAddress2, "192.168.0.18" );
    TBuf8<255> address;
    TNatReturnStatus status = KNatReady;
    
    // OK
    CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KModMediaField() );
    NSP_ASSERT_LEAVE( status = iStream->MediaConnInfoL( *mediafield, address ) );
    NSP_ASSERT( KNatReady == status );
    
    // Local candidates
    CNATFWCandidate* candidate = NSPTestHelper::CreateCandL(
    		*iStream->iStreamComponentArray[0],
            _L8("31233"), KProtocolInetUdp,
            CNATFWCandidate::EServerReflexive, KSessionId, ETrue );
    CleanupStack::PushL( candidate );
    iStream->iStreamComponentArray[0]->iLocalCandidates.AppendL( candidate );
    CleanupStack::Pop( candidate );
    candidate = NSPTestHelper::CreateCandL( 
    		*iStream->iStreamComponentArray[1],
            _L8("31233"), KProtocolInetUdp,
            CNATFWCandidate::EServerReflexive, KSessionId, ETrue );
    CleanupStack::PushL( candidate );
    iStream->iStreamComponentArray[1]->iLocalCandidates.AppendL( candidate );
    CleanupStack::Pop( candidate );
	
	// OK
	address = KRemoteAddress1();
	NSP_ASSERT_NO_LEAVE( status = iStream->MediaConnInfoL( *mediafield, address ) );
    NSP_ASSERT( KNatAsync == status );
	NSP_ASSERT_NO_LEAVE( status = iStream->MediaConnInfoL( *mediafield, address ) );
    NSP_ASSERT( KNatReady == status );
    
	// OK
	address = KRemoteAddress2();
	NSP_ASSERT_NO_LEAVE( status = iStream->MediaConnInfoL( *mediafield, address ) );
    NSP_ASSERT( KNatAsync == status );
	NSP_ASSERT_NO_LEAVE( status = iStream->MediaConnInfoL( *mediafield, address ) );
    NSP_ASSERT( KNatReady == status );
    CleanupStack::PopAndDestroy( mediafield );
    
    // OK
    mediafield = CSdpMediaField::DecodeLC( KModMediaFieldwithConnField() );
	NSP_ASSERT_NO_LEAVE( status = iStream->MediaConnInfoL( *mediafield, address ) );
    NSP_ASSERT( KNatAsync == status );
    NSP_ASSERT( !iStream->iStreamComponentArray[0]->iRemoteAddress.Compare( KAddressLongAddr() ) );
    NSP_ASSERT( 2850 == iStream->iStreamComponentArray[0]->iRemotePort );
    NSP_ASSERT( !iStream->iStreamComponentArray[1]->iRemoteAddress.Compare( KAddress() ) );
    NSP_ASSERT( 17777 == iStream->iStreamComponentArray[1]->iRemotePort );
	NSP_ASSERT_NO_LEAVE( status = iStream->MediaConnInfoL( *mediafield, address ) );
    NSP_ASSERT( KNatReady == status );
    NSP_ASSERT( !iStream->iStreamComponentArray[0]->iRemoteAddress.Compare( KAddressLongAddr() ) );
    NSP_ASSERT( 2850 == iStream->iStreamComponentArray[0]->iRemotePort );
    NSP_ASSERT( !iStream->iStreamComponentArray[1]->iRemoteAddress.Compare( KAddress() ) );
    NSP_ASSERT( 17777 == iStream->iStreamComponentArray[1]->iRemotePort );
    CleanupStack::PopAndDestroy( mediafield );
    }



// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPMediaStream,
    "Individual method tests for stream",
    "UNIT" )

NSP_TEST_METHOD(
	"NewL - test",
	"CNSPMediaStream",
	"UT_NewL",
	"FUNCTIONALITY",
	SetupL, UT_NewL, Teardown )

NSP_TEST_METHOD(
    "ModifyStunConnL - test",
    "CNSPMediaStream",
    "UT_ModifyStunConnL",
    "FUNCTIONALITY",
    SetupL, UT_ModifyStunConnL, Teardown )

NSP_TEST_METHOD(
    "ModifyIceConnL - test",
    "CNSPMediaStream",
    "UT_ModifyIceConnL",
    "FUNCTIONALITY",
    SetupL, UT_ModifyIceConnL, Teardown )

NSP_TEST_METHOD(
    "ModifyLocalConnL - test",
    "CNSPMediaStream",
    "UT_ModifyLocalConnL",
    "FUNCTIONALITY",
    SetupL, UT_ModifyLocalConnL, Teardown )

NSP_TEST_METHOD(
    "MediaConnInfoL - test",
    "CNSPMediaStream",
    "UT_MediaConnInfoL",
    "FUNCTIONALITY",
    SetupL, UT_MediaConnInfoL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
