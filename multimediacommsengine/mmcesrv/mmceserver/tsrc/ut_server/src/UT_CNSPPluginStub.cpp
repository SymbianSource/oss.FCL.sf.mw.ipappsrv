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




//  CLASS HEADER
#include "UT_CNSPPluginStub.h"
#include "CNSPPluginStub.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <e32def.h>
#include <e32std.h>
#include <e32des8.h>
#include <sdpcodecstringpool.h>
#include <sdpdocument.h> 
#include <in_sock.h>
#include "nspplugin.h"

const TInt KDefaultReserveStatus      = CNSPPlugin::ENSPResourcesNotReserved;          
const TInt KDefaultDelay		      = 50000;   // MicroSeconds

_LIT8( KMceSDP,
"v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 192.0.0.19\r\n\
t=0 0\r\n\
a=ice-ufrag:xStE\r\n\
a=ice-pwd:ermyyyErTTeseEWWtr\r\n\
m=audio 6231 RTP/AVP 96\r\n\
a=candidate:1 1 UDP 11435664 192.0.0.19 4356 typ srflx\r\n\
a=candidate:1 2 UDP 10435664 192.0.0.19 5333 typ srflx\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:6232 IN IP4 192.0.0.19\r\n" );


// CONSTRUCTION
UT_CNSPPluginStub* UT_CNSPPluginStub::NewL()
    {
    UT_CNSPPluginStub* self = UT_CNSPPluginStub::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CNSPPluginStub* UT_CNSPPluginStub::NewLC()
    {
    UT_CNSPPluginStub* self = new( ELeave ) UT_CNSPPluginStub();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CNSPPluginStub::~UT_CNSPPluginStub()
    {

    }

// Default constructor
UT_CNSPPluginStub::UT_CNSPPluginStub()
    {

    }

// Second phase construct
void UT_CNSPPluginStub::ConstructL()
    {
       
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS 



void UT_CNSPPluginStub::SetupL(  )
    {
    SdpCodecStringPool::OpenL();
    // Use the Publish & Subscribe to store the keys and values. 
    iPlugin = CNSPPluginStub::NewL();
       
   	iInitializedReceived = 0;
	iOfferReadyReceived = 0;
	iAnswerReadyReceived = 0;
	iUpdateSdpReceived = 0;
	iErrorOccurredReceived = 0;
    }

void UT_CNSPPluginStub::Teardown(  )
    {
    SdpCodecStringPool::Close();
    delete iPlugin;
	} 


void UT_CNSPPluginStub::UT_CNSPPluginStub_NewSessionLL(  )
    {
    EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iPlugin->NewSessionL(*this, 6, _L8("stun.nokia.com"), KProtocolInetUdp);
    if ( iPlugin->iRequestType != CNSPPluginStub::EResolve )
    	{
    	CActiveScheduler::Start();		
    	}
    EUNIT_ASSERT( iNatSessionId == iPlugin->iSessionId );
    EUNIT_ASSERT( iPlugin->iReservStatus  == 0 );
    EUNIT_ASSERT( iPlugin->iReservStatus == KDefaultReserveStatus );
    EUNIT_ASSERT( iPlugin->iDelay == KDefaultDelay );
    }

void UT_CNSPPluginStub::UT_CNSPPluginStub_CloseSessionLL(  )
    {
    iNatSessionId = iPlugin->NewSessionL(*this, 6, _L8("stun.nokia.com"), KProtocolInetUdp);
    iPlugin->CloseSessionL( iNatSessionId );
    EUNIT_ASSERT( iNatSessionId == 0 );
    EUNIT_ASSERT( iPlugin->iReservStatus == KDefaultReserveStatus );
    EUNIT_ASSERT( iPlugin->iDelay == KDefaultDelay );
 
    }

void UT_CNSPPluginStub::UT_CNSPPluginStub_CreateOfferLL(  )
    {
    HBufC8* Offer1 = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer1 );
    CleanupStack::PushL( sdpOfferAnswer );
    
    EUNIT_ASSERT( iInitializedReceived == 0 );
    EUNIT_ASSERT( iOfferReadyReceived  == 0 );
    iNatSessionId = iPlugin->NewSessionL(*this, 6, _L8("stun.nokia.com"), KProtocolInetUdp);
    EUNIT_ASSERT( iPlugin->iReservStatus == KDefaultReserveStatus );
    EUNIT_ASSERT( iPlugin->iDelay == KDefaultDelay );
 
    CActiveScheduler::Start();
    iRequestStatus = iPlugin->CreateOfferL( iNatSessionId, sdpOfferAnswer );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer1 );
    EUNIT_ASSERT( iInitializedReceived == 1 );
    EUNIT_ASSERT( iOfferReadyReceived  == 1 );
    }

void UT_CNSPPluginStub::UT_CNSPPluginStub_ResolveLL(  )
    {
    HBufC8* Offer = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer );
    CleanupStack::PushL( sdpOfferAnswer );
       
    EUNIT_ASSERT( iInitializedReceived == 0 ); 
    EUNIT_ASSERT( iOfferReadyReceived  == 0 );
    EUNIT_ASSERT( iAnswerReadyReceived  == 0 );
    iPlugin->iRequestType = CNSPPluginStub::EResolve;
    iNatSessionId = iPlugin->NewSessionL(*this, 6, _L8("stun.nokia.com"), KProtocolInetUdp);
    EUNIT_ASSERT( iPlugin->iReservStatus == CNSPPlugin::ENSPResourcesNotReserved );
    EUNIT_ASSERT( iPlugin->iDelay == KDefaultDelay );
 
    CActiveScheduler::Start();
    iRequestStatus= iPlugin->ResolveL( iNatSessionId, sdpOfferAnswer, sdpOfferAnswer );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer );
    CActiveScheduler::Start();
    EUNIT_ASSERT( iInitializedReceived == 1 );
    EUNIT_ASSERT( iAnswerReadyReceived  == 1 );
    CActiveScheduler::Start();
    EUNIT_ASSERT( iPlugin->iReservStatus == CNSPPlugin::ENSPResourcesReserved );
    }

void UT_CNSPPluginStub::UT_CNSPPluginStub_DecodeAnswerLL(  )
    {
	//TBD: Not Required yet, Contains dummy implementation
	EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iPlugin->NewSessionL(*this, 6, _L8("stun.nokia.com"), KProtocolInetUdp);
    CActiveScheduler::Start();		
    HBufC8* Offer = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer );
    CleanupStack::PushL( sdpOfferAnswer );
    iRequestStatus  = iPlugin->DecodeAnswerL( iNatSessionId, sdpOfferAnswer );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer );
    EUNIT_ASSERT( iNatSessionId == iPlugin->iSessionId );
    EUNIT_ASSERT( iPlugin->iReservStatus == KDefaultReserveStatus );
    }

void UT_CNSPPluginStub::UT_CNSPPluginStub_UpdateLL(  )
    {
    //TBD: Not Required yet, Cotnains dummy implementation
    EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iPlugin->NewSessionL(*this, 6, _L8("stun.nokia.com"), KProtocolInetUdp);
    CActiveScheduler::Start();		
	HBufC8* Offer = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer );
    CleanupStack::PushL( sdpOfferAnswer );
   	iPlugin->UpdateL( iNatSessionId, sdpOfferAnswer );
   	CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer );
    EUNIT_ASSERT( iNatSessionId == iPlugin->iSessionId );
    EUNIT_ASSERT( iPlugin->iReservStatus  == 0 );
    EUNIT_ASSERT( iPlugin->iReservStatus == KDefaultReserveStatus );
    EUNIT_ASSERT( iPlugin->iDelay == KDefaultDelay );
    
    }


void UT_CNSPPluginStub::UT_CNSPPluginStub_RestartLL(  )
    {
    //TBD: Not Required yet, Cotnains dummy implementation
    
    EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iPlugin->NewSessionL(*this, 6, _L8("stun.nokia.com"), KProtocolInetUdp);
    CActiveScheduler::Start();		
    iRequestStatus = iPlugin->RestartL( iNatSessionId );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    EUNIT_ASSERT( iNatSessionId == iPlugin->iSessionId );
    EUNIT_ASSERT( iPlugin->iReservStatus  == 0 );
    EUNIT_ASSERT( iPlugin->iReservStatus == KDefaultReserveStatus );
    EUNIT_ASSERT( iPlugin->iDelay == KDefaultDelay );
    }


void UT_CNSPPluginStub::UT_CNSPPluginStub_GetSessionParameterLL(  )
    {
    TInt status = KErrNone;
    EUNIT_ASSERT( iInitializedReceived == 0 );
    iNatSessionId = iPlugin->NewSessionL(*this, 6, _L8("stun.nokia.com"), KProtocolInetUdp);
    CActiveScheduler::Start();
    EUNIT_ASSERT( iNatSessionId == iPlugin->iSessionId );
    EUNIT_ASSERT( iInitializedReceived == 1 );
    status = iPlugin->GetSessionParam( iNatSessionId, CNSPPluginStub::ENSPResourseReservationStatusKey );
            
    EUNIT_ASSERT( status == CNSPPlugin::ENSPResourcesNotReserved );
    EUNIT_ASSERT( iPlugin->iDelay == KDefaultDelay );
    
    // Resolve and check if the resource reservation status has been changed.
    
    HBufC8* Offer = KMceSDP().AllocLC();
    CSdpDocument* sdpOfferAnswer = CSdpDocument::DecodeL( *Offer );
    CleanupStack::PushL( sdpOfferAnswer );
    
    iPlugin->iRequestType = CNSPPluginStub::EResolve;
    iRequestStatus= iPlugin->ResolveL( iNatSessionId, sdpOfferAnswer, sdpOfferAnswer );
    EUNIT_ASSERT( iRequestStatus == KNatAsync );
    CleanupStack::PopAndDestroy( sdpOfferAnswer );
    CleanupStack::PopAndDestroy( Offer );
    CActiveScheduler::Start();
    EUNIT_ASSERT( iInitializedReceived == 1 );
    EUNIT_ASSERT( iAnswerReadyReceived  == 1 );
    CActiveScheduler::Start();
    EUNIT_ASSERT( iPlugin->iReservStatus == CNSPPlugin::ENSPResourcesReserved );

	status = iPlugin->GetSessionParam( iNatSessionId, CNSPPluginStub::ENSPResourseReservationStatusKey );
    EUNIT_ASSERT( status == CNSPPlugin::ENSPResourcesReserved );
    }


// =========================================================================================================================
// Implemenation of NoPrackReceived Virtual Function.
// =========================================================================================================================
//

void UT_CNSPPluginStub::Initialized( TUint /*aSessionId*/ )
	{
	CActiveScheduler::Stop(); 
	RDebug::Print(_L("ICEPlugin: INITIALIZED )"));   	
	iInitializedReceived = 1;
	}
        
void UT_CNSPPluginStub::OfferReady( TUint /*aSessionId*/, CSdpDocument* /*aOffer*/ )
	{
	CActiveScheduler::Stop(); 
	RDebug::Print(_L("ICEPlugin: OFFER-READY )"));   		
	iOfferReadyReceived = 1;
	}
    
void UT_CNSPPluginStub::AnswerReady( TUint /*aSessionId*/, CSdpDocument* /*aAnswer*/ )
	{
	CActiveScheduler::Stop(); 
	RDebug::Print(_L("ICEPlugin:ANSWER-READY )"));   			
	iAnswerReadyReceived = 1;
	}
    
void UT_CNSPPluginStub::UpdateSdp( TUint /*aSessionId*/, CSdpDocument* /*aOffer*/ )
	{
	CActiveScheduler::Stop(); 
	RDebug::Print(_L("ICEPlugin: UPDATE-SDP )"));   			
	iUpdateSdpReceived = 1;
	}
    
void UT_CNSPPluginStub::ErrorOccurred( TUint /*aSessionId*/, TInt /*aError*/ )
	{
	CActiveScheduler::Stop(); 
	RDebug::Print(_L("ICEPlugin: ERROR-OCCURED )"));   					
	iErrorOccurredReceived = 0;
	}
	
void UT_CNSPPluginStub::IcmpErrorOccurred( TUint /*aSessionId*/, TInt /*aError*/ )
	{
	/*
	CActiveScheduler::Stop(); 
	RDebug::Print(_L("ICEPlugin: ICMP ERROR-OCCURED )"));   					
	iErrorOccurredReceived = 0;
	*/
	}	

//  TEST TABLE
 
EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPPluginStub, 
    "UT_CNSPPluginStub",
    "UNIT" )
    
EUNIT_TEST(
    "NewSessionL - test ",
    "UT_CNSPPluginStub",
    "UT_CNSPPluginStub_NewSessionLL",
    "FUNCTIONALITY",
    SetupL, UT_CNSPPluginStub_NewSessionLL, Teardown)

EUNIT_TEST(
    "CloseSessionL - test ",
    "UT_CNSPPluginStub",
    "CloseSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CNSPPluginStub_CloseSessionLL, Teardown)

EUNIT_TEST(
    "CreateOfferL - test ",
    "UT_CNSPPluginStub",
    "UT_CNSPPluginStub_CreateOfferLL",
    "FUNCTIONALITY",
    SetupL, UT_CNSPPluginStub_CreateOfferLL, Teardown)

EUNIT_TEST(
    "ResolveL - test ",
    "UT_CNSPPluginStub",
    "UT_CNSPPluginStub_ResolveLL",
    "FUNCTIONALITY",
    SetupL, UT_CNSPPluginStub_ResolveLL, Teardown)

EUNIT_TEST(
    "DecodeAnswerL - test ",
    "UT_CNSPPluginStub",
    "UT_CNSPPluginStub_DecodeAnswerLL",
    "FUNCTIONALITY",
    SetupL, UT_CNSPPluginStub_DecodeAnswerLL, Teardown)

EUNIT_TEST(
    "UpdateL - test ",
    "UT_CNSPPluginStub",
    "UT_CNSPPluginStub_UpdateLL",
    "FUNCTIONALITY",
    SetupL, UT_CNSPPluginStub_UpdateLL, Teardown)

EUNIT_TEST(
    "ReStart - test ",
    "UT_CNSPPluginStub",
    "UT_CNSPPluginStub_RestartLL",
    "FUNCTIONALITY",
    SetupL, UT_CNSPPluginStub_RestartLL, Teardown)


EUNIT_TEST(
    "GetSessionParameter - test ",
    "UT_CNSPPluginStub",
    "UT_CNSPPluginStub_GetSessionParameterLL",
    "FUNCTIONALITY",
    SetupL, UT_CNSPPluginStub_GetSessionParameterLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE

