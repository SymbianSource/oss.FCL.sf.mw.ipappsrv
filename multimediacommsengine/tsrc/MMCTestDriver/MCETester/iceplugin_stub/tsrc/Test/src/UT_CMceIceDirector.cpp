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


// CLASS HEADER
#include "UT_CMceIceDirector.h"
#include "cmceicedirector.h"
#include "cmceicepropertywatcher.h"
#include "cmceiceerrortrigger.h"
#include "mcetesterpskeys.h"

// EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <e32def.h>
#include <e32std.h>
#include <e32des8.h>
#include <sdpcodecstringpool.h>
#include <sdpdocument.h> 
#include <e32property.h>
#include <in_sock.h>
#include <nspplugin.h>
#include <SdpConnectionField.h>
#include <SdpMediaField.h>

       
const TInt KDefaultReserveStatus = CNSPPlugin::ENSPResourcesNotReserved;   	 // Resources Not Reserved
const TInt KDefaultDelay		 = 50000;   // MicroSeconds
const TInt KMyIapId 			 = 85;

_LIT8( KStunServerAddr, "stun.addr");

_LIT8( KLocalPublicAddressProperty, "1.1.5.6:1123" );
_LIT8( KLocalPublicAddress, "1.1.5.6" );
const TInt KLocalPublicPort = 1123;

_LIT8( KRemotePublicAddressProperty, "9.7.9.5:5408" );
_LIT8( KRemotePublicAddress, "9.7.9.5" );
const TInt KRemotePublicPort = 5408;

_LIT8( KErrorCallbackPropertyIcmp, "icmp 200" );
_LIT8( KErrorCallbackPropertyIcmp2, "ICMP -61" );
_LIT8( KErrorCallbackProperty, "error -1765" );

_LIT_SECURITY_POLICY_PASS( KAllowAllPolicy );


_LIT8( KSDPAddress, "10.20.180.72" );
_LIT8( KMceSDP, "v=0\r\n\
o=usera 1234567890 1234567890 IN IP4 10.20.180.72\r\n\
s=-\r\n\
i=-\r\n\
c=IN IP4 10.20.180.72\r\n\
t=0 0\r\n\
m=audio 5078 RTP/AVP 96\r\n\
a=sendrecv\r\n\
a=curr:qos local sendrecv\r\n\
a=curr:qos remote none\r\n\
a=des:qos mandatory local sendrecv\r\n\
a=des:qos none remote sendrecv\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ptime:20\r\n\
a=maxptime:400\r\n\
a=fmtp:96 octet-align=0; mode-set=0,1,2,3,4,5,6,7\r\n\
m=video 51372 RTP/AVP 31 32\r\n\
a=rtpmap:31 H261/90000\r\n\
a=rtpmap:32 MPV/90000\r\n");


// CONSTRUCTION
UT_CMceIceDirector* UT_CMceIceDirector::NewL()
    {
    UT_CMceIceDirector* self = UT_CMceIceDirector::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMceIceDirector* UT_CMceIceDirector::NewLC()
    {
    UT_CMceIceDirector* self = new( ELeave ) UT_CMceIceDirector();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceIceDirector::~UT_CMceIceDirector()
    {
    }

// Default constructor
UT_CMceIceDirector::UT_CMceIceDirector()
    {
    }

// Second phase construct
void UT_CMceIceDirector::ConstructL()
    {
       
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

void UT_CMceIceDirector::Initialized( TUint /*aSessionId*/ )
	{
	CActiveScheduler::Stop(); 
	RDebug::Print( _L( "ICEPlugin: INITIALIZED" ) );
	++iInitializedReceived;
	}
        
void UT_CMceIceDirector::OfferReady( TUint /*aSessionId*/, CSdpDocument* aOffer )
	{
	CActiveScheduler::Stop(); 
	iReceivedAsyncOffer = aOffer;
	RDebug::Print( _L( "ICEPlugin: OFFER-READY" ) );
	++iOfferReadyReceived;
	}
    
void UT_CMceIceDirector::AnswerReady( TUint /*aSessionId*/, CSdpDocument* aAnswer )
	{
	CActiveScheduler::Stop(); 
	delete aAnswer;
	RDebug::Print( _L( "ICEPlugin:ANSWER-READY" ) );
	++iAnswerReadyReceived;
	}
    
void UT_CMceIceDirector::UpdateSdp( TUint /*aSessionId*/, CSdpDocument* aOffer )
	{
	CActiveScheduler::Stop(); 
	delete aOffer;
	RDebug::Print( _L( "ICEPlugin: UPDATE-SDP " ) );
	++iUpdateSdpReceived;
	}
    
void UT_CMceIceDirector::ErrorOccurred( TUint /*aSessionId*/, TInt /*aError*/ )
	{
	CActiveScheduler::Stop();
	RDebug::Print( _L( "ICEPlugin: ERROR-OCCURED" ) );
	++iErrorOccurredReceived;
	}

void UT_CMceIceDirector::IcmpErrorOccurred( TUint aSessionId, TInt /*aError*/ )
	{
	CActiveScheduler::Stop();
	RDebug::Print( _L( "ICEPlugin: ICMPERROR-OCCURED" ) );
	EUNIT_ASSERT( aSessionId == iNatSessionId );
	++iIcmpErrorOccurredReceived;
	}

void UT_CMceIceDirector::SetupL()
    {
    // Use the Publish & Subscribe to store the keys and values. 
    iIceDirector = CMceIceDirector::NewL();
    
    RProcess ownprocess;
    TUid myOwn = ownprocess.SecureId();
    ownprocess.Close();
    
   	iInitializedReceived = 0;
	iOfferReadyReceived = 0;
	iAnswerReadyReceived = 0;
	iUpdateSdpReceived = 0;
	iErrorOccurredReceived = 0;
	iIcmpErrorOccurredReceived = 0;

	SetAsyncMode();
	ResetResourceReservationStatus();
	SetPublicAddresses();
	ClearErrorCallbackProperty();
	}

void UT_CMceIceDirector::Setup2L()
    {
    // Use the Publish & Subscribe to store the keys and values. 
    iIceDirector = CMceIceDirector::NewL();

    iInitializedReceived = 0;
	iOfferReadyReceived = 0;
	iAnswerReadyReceived = 0;
	iUpdateSdpReceived = 0;
	iErrorOccurredReceived = 0;
	iIcmpErrorOccurredReceived = 0;

	// This must be deleted in each test case, before the SDP string pool is
	// closed. Otherwise shows that tests leak memory.
	delete iReceivedAsyncOffer;
	iReceivedAsyncOffer = NULL;
    }

void UT_CMceIceDirector::Teardown()
    {
    delete iIceDirector;
    iIceDirector = NULL;
	}

void UT_CMceIceDirector::UT_CMceIceDirector_NewSessionLL()
    {
    EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    if ( iIceDirector->iRequestType != CMceIceDirector::EResolve )
    	{
    	CActiveScheduler::Start();		
    	}
    EUNIT_ASSERT( iNatSessionId == iIceDirector->iSessionId );
    EUNIT_ASSERT( iIceDirector->iDTimer != NULL );
    }

void UT_CMceIceDirector::UT_CMceIceDirector_CloseSessionLL()
    {
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    iIceDirector->CloseSessionL( iNatSessionId );
    EUNIT_ASSERT( iIceDirector->iSessionId == 0 );
    EUNIT_ASSERT( !iIceDirector->iSessionOb );
    }

// CreateOfferL completes syncronously
void UT_CMceIceDirector::UT_CMceIceDirector_CreateOfferLL()
    {
    SetSyncMode();
    SdpCodecStringPool::OpenL();
    HBufC8* Offer = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer );
    CleanupStack::PushL( sdpOfferAnswer );
    
    EUNIT_ASSERT( iInitializedReceived == 0 );
    EUNIT_ASSERT( iOfferReadyReceived  == 0 );
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    CActiveScheduler::Start();
    iRequestStatus = iIceDirector->CreateOfferL( iNatSessionId, sdpOfferAnswer );
    EUNIT_ASSERT( iRequestStatus == KNatReady );
    
    // Check that address and port in offer, were updated by the plugin
    CSdpConnectionField* conn = sdpOfferAnswer->ConnectionField();
    EUNIT_ASSERT( conn != NULL );
    EUNIT_ASSERT( conn->Address() == KLocalPublicAddress );
    RPointerArray< CSdpMediaField >& media = sdpOfferAnswer->MediaFields();
	for ( TInt i = 0; i < media.Count(); ++i )
		{
		EUNIT_ASSERT( media[ i ]->Port() == KLocalPublicPort );
		}
    CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer );
    SdpCodecStringPool::Close();

    EUNIT_ASSERT( iInitializedReceived == 1 );
    // CreateOfferL completed syncronously, it doesn't use callback
    EUNIT_ASSERT( iOfferReadyReceived  == 0 );
    }

// CreateOfferL completes asyncronously
void UT_CMceIceDirector::UT_CMceIceDirector_CreateOfferLAsyncL()
    {
    SetAsyncMode();
    SdpCodecStringPool::OpenL();
    HBufC8* Offer = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer );
    CleanupStack::PushL( sdpOfferAnswer );
    
    EUNIT_ASSERT( iInitializedReceived == 0 );
    EUNIT_ASSERT( iOfferReadyReceived  == 0 );
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, _L8("stun.nokia.com"),
    										   KProtocolInetUdp );
 
    CActiveScheduler::Start();
    iRequestStatus = iIceDirector->CreateOfferL( iNatSessionId, sdpOfferAnswer );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    EUNIT_ASSERT( iOfferReadyReceived  == 0 );
    
    // Address has not yet been updated
    CSdpConnectionField* conn = sdpOfferAnswer->ConnectionField();
    EUNIT_ASSERT( conn != NULL );
    EUNIT_ASSERT( conn->Address() == KSDPAddress );

    CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer );
    CActiveScheduler::Start();

    // Check that address and port in offer, were updated by the plugin
    EUNIT_ASSERT( iReceivedAsyncOffer != NULL );
    conn = iReceivedAsyncOffer->ConnectionField();
    EUNIT_ASSERT( conn != NULL );
    EUNIT_ASSERT( conn->Address() == KLocalPublicAddress );
    RPointerArray< CSdpMediaField >& media = iReceivedAsyncOffer->MediaFields();
	for ( TInt i = 0; i < media.Count(); ++i )
		{
		EUNIT_ASSERT( media[ i ]->Port() == KLocalPublicPort );
		}
    
    delete iReceivedAsyncOffer;
	iReceivedAsyncOffer = NULL;
    SdpCodecStringPool::Close();
    EUNIT_ASSERT( iInitializedReceived == 1 );
    EUNIT_ASSERT( iOfferReadyReceived  == 1 );
    }
    
void UT_CMceIceDirector::UT_CMceIceDirector_ResolveLL()
    {
    SdpCodecStringPool::OpenL();
    HBufC8* Offer = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer );
    CleanupStack::PushL( sdpOfferAnswer );
       
    EUNIT_ASSERT( iInitializedReceived == 0 );
    EUNIT_ASSERT( iOfferReadyReceived  == 0 );
    EUNIT_ASSERT( iAnswerReadyReceived  == 0 );
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, _L8("stun.nokia.com"),
    										   KProtocolInetUdp );
 
    CActiveScheduler::Start(); // Initialized
    iRequestStatus = iIceDirector->ResolveL( iNatSessionId, sdpOfferAnswer, sdpOfferAnswer );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer );

    CActiveScheduler::Start(); // Answer ready
    EUNIT_ASSERT( iInitializedReceived == 1 );
    EUNIT_ASSERT( iOfferReadyReceived  == 0 );
    EUNIT_ASSERT( iAnswerReadyReceived == 1 );
    

    CActiveScheduler::Start(); // Offer ready
    EUNIT_ASSERT( iInitializedReceived == 1 );
    EUNIT_ASSERT( iOfferReadyReceived  == 1 );
    EUNIT_ASSERT( iAnswerReadyReceived == 1 );
    EUNIT_ASSERT( iReceivedAsyncOffer != NULL );
    delete iReceivedAsyncOffer;
	iReceivedAsyncOffer = NULL;

	SdpCodecStringPool::Close();
    }

void UT_CMceIceDirector::UT_CMceIceDirector_ResolveL_noSDP_L()
	{
	iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    CActiveScheduler::Start(); // Initialized
    CSdpDocument* nullSdp( NULL );
    CSdpDocument* dummySdp( ( CSdpDocument* )0x1 );
    
    TRAPD( err, iIceDirector->ResolveL( iNatSessionId, nullSdp, dummySdp ) );
    EUNIT_ASSERT( err == KErrArgument );
    
    TRAP( err, iIceDirector->ResolveL( iNatSessionId, dummySdp, nullSdp ) );
    EUNIT_ASSERT( err == KErrArgument );
    
    TRAP( err, iIceDirector->ResolveL( iNatSessionId, nullSdp, nullSdp ) );
    EUNIT_ASSERT( err == KErrArgument );
	}

void UT_CMceIceDirector::UT_CMceIceDirector_DecodeAnswerLL()
    {
	//TBD: Not Required yet, Contains dummy implementation
	EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    CActiveScheduler::Start();		
    SdpCodecStringPool::OpenL();
    HBufC8* Offer = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer );
    CleanupStack::PushL( sdpOfferAnswer );
    iRequestStatus  = iIceDirector->DecodeAnswerL( iNatSessionId, sdpOfferAnswer );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer );
    SdpCodecStringPool::Close();
    EUNIT_ASSERT( iNatSessionId == iIceDirector->iSessionId );
    }

void UT_CMceIceDirector::UT_CMceIceDirector_RestartLL()
    {
    //TBD: Not Required yet, Cotnains dummy implementation
    
    EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    CActiveScheduler::Start();		
    iRequestStatus = iIceDirector->RestartL( iNatSessionId );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    EUNIT_ASSERT( iNatSessionId == iIceDirector->iSessionId );
    }

void UT_CMceIceDirector::UT_CMceIceDirector_GetSessionParameterLL()
    {
    EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    CActiveScheduler::Start();
    EUNIT_ASSERT( iNatSessionId == iIceDirector->iSessionId );
    EUNIT_ASSERT( iInitializedReceived == 1 );
    EUNIT_ASSERT( iIceDirector->GetSessionParam( iNatSessionId,
		CNSPPlugin::ENSPResourseReservationStatusKey ) == 0 );
    }

void UT_CMceIceDirector::UT_CMceIceDirector_GetSessionParameterLReservedL()
    {
    SetResourceReservationStatus();
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    CActiveScheduler::Start();
    EUNIT_ASSERT( iIceDirector->GetSessionParam( iNatSessionId,
		CNSPPlugin::ENSPResourseReservationStatusKey ) == 1 );
    }

void UT_CMceIceDirector::UT_CMceIceDirector_GetDefaultValuesLL()
    {
    EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iIceDirector->NewSessionL( *this, 6, KStunServerAddr,
    										   KProtocolInetUdp );
    if ( iIceDirector->iRequestType != CMceIceDirector::EResolve )
    	{
    	CActiveScheduler::Start();		
    	}
    EUNIT_ASSERT( iNatSessionId == iIceDirector->iSessionId );
    }
    
void UT_CMceIceDirector::UT_CMceIceDirector_CloneSdpLL()
	{
	SdpCodecStringPool::OpenL();
	iNatSessionId = iIceDirector->NewSessionL( *this, KMyIapId, KStunServerAddr,
    										   KProtocolInetUdp );
	EUNIT_ASSERT( !iIceDirector->iOffer );
	EUNIT_ASSERT( !iIceDirector->iAnswer );
	
	CSdpDocument* sdp = CSdpDocument::NewLC();
	iIceDirector->CloneSdpL( *sdp, iIceDirector->iOffer );

	EUNIT_ASSERT( iIceDirector->iOffer != NULL );
	EUNIT_ASSERT( !iIceDirector->iAnswer );
	
	iIceDirector->CloneSdpL( *sdp, iIceDirector->iAnswer );
	EUNIT_ASSERT( iIceDirector->iOffer != NULL );
	EUNIT_ASSERT( iIceDirector->iAnswer != NULL );
	CleanupStack::PopAndDestroy( sdp );
	SdpCodecStringPool::Close();
	}

void UT_CMceIceDirector::UT_CMceIceDirector_UpdateAddressLL()
	{
	_LIT( KPublicAddress16, "80.70.60.50" );
	_LIT8( KPublicAddress8, "80.70.60.50" );
	const TInt KPublicPort( 4085 );

	SdpCodecStringPool::OpenL();
	iNatSessionId = iIceDirector->NewSessionL( *this, KMyIapId, KStunServerAddr,
    										   KProtocolInetUdp );
	HBufC8* sdpBuf = KMceSDP().AllocLC();
    CSdpDocument* sdp = CSdpDocument::DecodeL( *sdpBuf );
    CleanupStack::PopAndDestroy( sdpBuf );
    CleanupStack::PushL( sdp );


	iIceDirector->iLocalAddressWatcher->iPropertyExists = ETrue;
	iIceDirector->iLocalAddressWatcher->iAddress.Input( KPublicAddress16 );
	iIceDirector->iLocalAddressWatcher->iAddress.SetPort( KPublicPort );


	iIceDirector->UpdateAddressL( *sdp, *iIceDirector->iLocalAddressWatcher );


	EUNIT_ASSERT( sdp->ConnectionField()->Address() == KPublicAddress8 );
	EUNIT_ASSERT( sdp->ConnectionField()->InetAddress() != NULL );

	RPointerArray< CSdpMediaField >& media = sdp->MediaFields();
	EUNIT_ASSERT( media.Count() == 2 );
	EUNIT_ASSERT( media[ 0 ]->Port() == KPublicPort );
	EUNIT_ASSERT( media[ 1 ]->Port() == KPublicPort );

	CleanupStack::PopAndDestroy( sdp );
	SdpCodecStringPool::Close();
	}

void UT_CMceIceDirector::UT_CMceIceDirector_IcmpErrorL()
	{
	SdpCodecStringPool::OpenL();

	iIceDirector->iErrorTrigger->ParsePropertyL();
	// Callback is not done yet, as observer isn't yet set
	EUNIT_ASSERT( iIcmpErrorOccurredReceived == 0 );
	EUNIT_ASSERT( iErrorOccurredReceived == 0 );

	iNatSessionId = iIceDirector->NewSessionL( *this, KMyIapId, KStunServerAddr,
    										   KProtocolInetUdp );
	CActiveScheduler::Start(); // initialized


	TriggerIcmpErrorCallback();
	CActiveScheduler::Start(); // property updated
	EUNIT_ASSERT( iIcmpErrorOccurredReceived == 1 );
	EUNIT_ASSERT( iErrorOccurredReceived == 0 );


	TriggerIcmpErrorCallback();
	CActiveScheduler::Start(); // property updated with same value
	EUNIT_ASSERT( iIcmpErrorOccurredReceived == 2 );
	EUNIT_ASSERT( iErrorOccurredReceived == 0 );


	// Trigger ICMP error callback
	SetTextProperty( KMceTesterNatPluginErrorCallback,
					 KErrorCallbackPropertyIcmp2 );
	CActiveScheduler::Start(); // property updated with a different value
	EUNIT_ASSERT( iIcmpErrorOccurredReceived == 3 );
	EUNIT_ASSERT( iErrorOccurredReceived == 0 );


	// Trigger error callback
	SetTextProperty( KMceTesterNatPluginErrorCallback,
					 KErrorCallbackProperty );
	CActiveScheduler::Start(); // property updated
	EUNIT_ASSERT( iErrorOccurredReceived == 1 );
	EUNIT_ASSERT( iIcmpErrorOccurredReceived == 3 );


	TriggerIcmpErrorCallback();
	CActiveScheduler::Start(); // property updated
	EUNIT_ASSERT( iErrorOccurredReceived == 1 );
	EUNIT_ASSERT( iIcmpErrorOccurredReceived == 4 );


	// Trigger error callback
	SetTextProperty( KMceTesterNatPluginErrorCallback,
					 KErrorCallbackProperty );
	CActiveScheduler::Start(); // property updated
	EUNIT_ASSERT( iErrorOccurredReceived == 2 );
	EUNIT_ASSERT( iIcmpErrorOccurredReceived == 4 );


	// Trigger error callback
	SetTextProperty( KMceTesterNatPluginErrorCallback,
					 KErrorCallbackProperty );
	CActiveScheduler::Start(); // property updated
	EUNIT_ASSERT( iErrorOccurredReceived == 3 );
	EUNIT_ASSERT( iIcmpErrorOccurredReceived == 4 );

	SdpCodecStringPool::Close();
	}

void UT_CMceIceDirector::UT_CMceIceDirector_ParseErrorCodeL()
	{
	_LIT8( KError1, "icmp -489" );
	_LIT8( KError2, "iCmP 8" );
	_LIT8( KError3, "ICMP 0" );
	_LIT8( KError4, "iCMp -500984" );
	_LIT8( KError5, "ICmp100" );
	_LIT8( KError6, "eRRor -204" );
	_LIT8( KError7, "-99105" );
	_LIT8( KError8, "xyz 4860abc" );
	_LIT8( KError9, "error   123  " );
	_LIT8( KError10, "abcd" );
	_LIT8( KError11, "   icmp" );
	_LIT8( KError12, "ICMP" );
	_LIT8( KError13, "Icmp  " );

	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError1 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == -489 );

	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError2 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == 8 );
	
	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError3 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == 0 );
	
	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError4 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == -500984 );
	
	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError5 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == 100 );

	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError6 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == -204 );
	
	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError7 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == -99105 );

	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError8 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == 4860 );
	
	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError9 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == 123 );

	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError10 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == KErrNotFound );
	
	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError11 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == KErrNotFound );
	
	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError12 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == KErrNotFound );
	
	iIceDirector->iErrorTrigger->iValue->Des().Copy( KError13 );
	EUNIT_ASSERT( iIceDirector->iErrorTrigger->ParseErrorCode() == KErrNotFound );
	}

void UT_CMceIceDirector::SetSyncMode()
	{
	RProperty::Delete( KUidMceTesterPSKeys, KMceTesterNatPluginCallbackDelay );
	}

void UT_CMceIceDirector::SetAsyncMode()
	{
	SetIntProperty( KMceTesterNatPluginCallbackDelay, KDefaultDelay );
	}

void UT_CMceIceDirector::SetResourceReservationStatus()
	{
	SetIntProperty( KMceTesterNatPluginResourceReservationStatus,
					KDefaultReserveStatus );
	}

void UT_CMceIceDirector::ResetResourceReservationStatus()
	{
	RProperty::Delete( KUidMceTesterPSKeys,
					   KMceTesterNatPluginResourceReservationStatus );
	}

void UT_CMceIceDirector::SetPublicAddresses()
	{
	SetTextProperty( KMceTesterNatPluginLocalAddress,
					 KLocalPublicAddressProperty );
	SetTextProperty( KMceTesterNatPluginRemoteAddress,
					 KRemotePublicAddressProperty );
	}

void UT_CMceIceDirector::TriggerIcmpErrorCallback()
	{
	SetTextProperty( KMceTesterNatPluginErrorCallback,
					 KErrorCallbackPropertyIcmp );
	}

void UT_CMceIceDirector::ClearErrorCallbackProperty()
	{
	RProperty::Delete( KUidMceTesterPSKeys,
					   KMceTesterNatPluginErrorCallback );
	}

void UT_CMceIceDirector::SetIntProperty( TUint aKey, TInt aValue )
	{
	DefineProperty( aKey, RProperty::EInt );
	RProperty::Set( KUidMceTesterPSKeys, aKey, aValue );
	}

void UT_CMceIceDirector::SetTextProperty( TUint aKey, const TDesC8 &aValue )
	{
	DefineProperty( aKey, RProperty::EText );
	RProperty::Set( KUidMceTesterPSKeys, aKey, aValue );
	}

void UT_CMceIceDirector::DefineProperty( TUint aKey, TInt aAttr )
	{
	RProperty::Define( KUidMceTesterPSKeys,
					   aKey,
					   aAttr,
					   KAllowAllPolicy,
					   KAllowAllPolicy );
	}

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMceIceDirector,
    "MCE ICE Director.",
    "UNIT" )

EUNIT_TEST(
    "NewSessionL - test ",
    "CMceIceDirector",
    "NewSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_NewSessionLL, Teardown)

EUNIT_TEST(
    "CloseSessionL - test ",
    "CMceIceDirector",
    "CloseSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_CloseSessionLL, Teardown)

EUNIT_TEST(
    "CreateOfferL - sync test ",
    "CMceIceDirector",
    "CreateOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_CreateOfferLL, Teardown)

EUNIT_TEST(
    "CreateOfferL - async test ",
    "CMceIceDirector",
    "CreateOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_CreateOfferLAsyncL, Teardown)

EUNIT_TEST(
    "ResolveL - test ",
    "CMceIceDirector",
    "ResolveL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_ResolveLL, Teardown)

EUNIT_TEST(
    "ResolveL - null SDP ",
    "CMceIceDirector",
    "ResolveL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_ResolveL_noSDP_L, Teardown)

EUNIT_TEST(
    "DecodeAnswerL - test ",
    "CMceIceDirector",
    "DecodeAnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_DecodeAnswerLL, Teardown)

EUNIT_TEST(
    "ReStart - test ",
    "CMceIceDirector",
    "ReStart",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_RestartLL, Teardown)

EUNIT_TEST(
    "GetSessionParameter - test ",
    "CMceIceDirector",
    "GetSessionParameter",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_GetSessionParameterLL, Teardown)

EUNIT_TEST(
    "GetSessionParameter - reserved test ",
    "CMceIceDirector",
    "GetSessionParameter",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_GetSessionParameterLReservedL, Teardown)

EUNIT_TEST(
    "GetDefaultValues - test ",
    "CMceIceDirector",
    "GetDefaultValues",
    "FUNCTIONALITY",
    Setup2L, UT_CMceIceDirector_GetDefaultValuesLL, Teardown)

EUNIT_TEST(
    "CloneSdpL - test ",
    "CMceIceDirector",
    "CloneSdpL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_CloneSdpLL, Teardown)

EUNIT_TEST(
    "UpdateAddressL - test ",
    "CMceIceDirector",
    "UpdateAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_UpdateAddressLL, Teardown)

EUNIT_TEST(
    "ICMP error - test ",
    "CMceIceDirector",
    "IcmpError callback",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_IcmpErrorL, Teardown)

EUNIT_TEST(
    "ParseErrorCode - test ",
    "CMceIceErrorTrigger",
    "ParseErrorCode",
    "FUNCTIONALITY",
    SetupL, UT_CMceIceDirector_ParseErrorCodeL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
