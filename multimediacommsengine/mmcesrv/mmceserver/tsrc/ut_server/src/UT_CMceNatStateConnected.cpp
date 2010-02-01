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
#include "UT_CMceNatStateConnected.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcetesthelper.h"
#include "mceservercore.h"
#include "cmcetls.h"
#include "mceactionset.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcenatsipsession.h"
#include "mcenatsipstatemachine.h"
#include "mcemediamanager.h"
#include "../inc/mcenatpluginmanager.h"
#include "nspplugin.h"
#include "mcetesthelper.h"

_LIT8( KIceOffer,
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


_LIT8( KIceAnswer,
"v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 192.168.0.12\r\n\
s=-\r\n\
c=IN IP4 172.0.0.19\r\n\
t=0 0\r\n\
a=ice-ufrag:xStB\r\n\
a=ice-pwd:ermyyyErRReseEWWtr\r\n\
m=audio 9345 RTP/AVP 96\r\n\
a=candidate:1 1 UDP 11435664 172.0.0.19 5356 typ srflx\r\n\
a=candidate:1 2 UDP 10435664 172.0.0.19 6333 typ srflx\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:7232 IN IP4 172.0.0.19\r\n" );




// CONSTRUCTION
UT_CMceNatStateConnected* UT_CMceNatStateConnected::NewL()
    {
    UT_CMceNatStateConnected* self =
    	UT_CMceNatStateConnected::NewLC();
    CleanupStack::Pop();
    return self;
    }

UT_CMceNatStateConnected* UT_CMceNatStateConnected::NewLC()
    {
    UT_CMceNatStateConnected* self =
    	new ( ELeave ) UT_CMceNatStateConnected();
    CleanupStack::PushL( self );
	self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CMceNatStateConnected::~UT_CMceNatStateConnected()
    {
    }

// Default constructor
UT_CMceNatStateConnected::UT_CMceNatStateConnected()
    {
    }

// Second phase construct
void UT_CMceNatStateConnected::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceNatStateConnected::SetupL()
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
	
	iIds.iAppUID = 10;
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iServerCore = CMceServerCore::NewL();
    
    iStorage = CMCETls::Storage();
    
    iNatSessionId = 3;
    
    iServerCore->Manager().MediaManager().NatPluginManager().iEnabled = ETrue;    
    
    CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );

    iSession = CMceCsSession::NewL(*iServerCore);
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipSession = static_cast<CMceSipSession*>
	   	((iSession->Implementation().SubSessions())[0]); 
	   	
	CMceSipManager& manager = iServerCore->Manager();
    CSIPProfile& profile = manager.ProfileL( iIds.iProfileID );
       	
	iNatSession = CMceNatSipSession::NewL( *iSession,
		iSipSession->SIPConnection(),
		*iSipSession->iStateMachine,
		profile,
		NULL );
	iNatSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iState = iNatSession->iNatStateMachine->iStates[
    	iNatSession->iNatStateMachine->iCurrentState ]; 
    	
	 
		   	
    }

void UT_CMceNatStateConnected::Teardown()
    {
    CMCETls::Close();
	delete iSession;
	iSession = NULL;
	delete iNatSession;
	iNatSession = NULL;
    delete iServerCore;
    iServerCore = NULL;

    SdpCodecStringPool::Close();
    }

void UT_CMceNatStateConnected::
	UT_CMceNatStateConnected_AcceptLL()
    {
    EUNIT_ASSERT( DoesAccept( EMceNatUpdated ) );
    EUNIT_ASSERT( DoesAccept( EMceNatHandleSIP ) );
    EUNIT_ASSERT( DoesAccept( EMceNatUpdateSdp ) );
    EUNIT_ASSERT( DoesAccept( EMceNatError ) );
    EUNIT_ASSERT( DoesAccept( EMceNatICMPError ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatInitialized ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatOffer ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatAnswer ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatSync ) );
    
    // EMceNatUpdated, 
    // return value must be based only on the media manager's need to signal
    iNatSession->IncrementSentSdpCount();
    iNatSession->IncrementReceivedSdpCount();
    iNatSession->IncrementReceivedSdpCount();
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( !DoesAccept( EMceNatUpdated ) );
    }

void UT_CMceNatStateConnected::
	UT_CMceNatStateConnected_EntryLL()
    {
    
    // TBD
    
    }
    

void UT_CMceNatStateConnected::
	UT_CMceNatStateConnected_ExitLL()
	{
	// NAT Error Occured
	iNatSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    TMceNatEventCode eventcode = EMceNatError;
	iNatSession->iNatDisabled = EFalse;
	iNatSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    
	TMceNatStateTransitionEvent event( NULL,*iNatSession,eventcode,
    								   *iNatSession, EMceInvite );
   	   	
   	EUNIT_ASSERT_LEAVE( iState->TransitionHandler().ExitL( event ));
   
          
    // NAT ICMP IGnored
    iNatSession->iNatDisabled = EFalse;
    eventcode = EMceNatICMPError;
    TMceNatStateTransitionEvent event1( NULL, *iNatSession,eventcode,
    								   *iNatSession, EMceInvite );
    EUNIT_ASSERT_NO_LEAVE( iState->TransitionHandler().ExitL( event1 ));
        
    
    // NAT ICMP Handled i.e MCE State is Established
    iNatSession->iNatDisabled = EFalse;
    eventcode = EMceNatICMPError;
    iNatSession->NextState( KMceStateEstablished ); 
	TMceNatStateTransitionEvent event2( NULL, *iNatSession,eventcode,
    								   *iNatSession, EMceInvite );
    EUNIT_ASSERT_LEAVE( iState->TransitionHandler().ExitL( event2 ));


	}

void UT_CMceNatStateConnected::UT_CMceNatStateConnected_ExitDefaultLL()
	{
    //EMceNatHandleSIP Reinvite for pullmode
    //preparation of initializing a object of TMceNatStateTransitionEvent
    iNatSession->iBody = CMceComSession::NewL(CMceComSession::EOutSession);
    iNatSession->iNatDisabled = EFalse;

    //initilize the NAT event
    TMceNatEventCode eventcode = EMceNatHandleSIP;
    iNatSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSession->NextState( KMceStateUpdating );

    //declear a object of class TMceNatStateTransitionEvent
    TMceNatStateTransitionEvent event3( NULL,*iNatSession,eventcode,
    *iNatSession, EMceUpdate);
    
    //initialize a NULL content
    CSIPServerTransaction* srvtransaction = 
       MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, EFalse );
    CleanupStack::PushL( srvtransaction );
    event3.Session().iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
    
    //make sure no sdp exist
    EUNIT_ASSERT(!MceSip::HasContent( event3.Session().Request() ));
    //assert any exception
    iState->TransitionHandler().ExitL( event3 );
    //assert if the State changed to KMceNatStateWait
    EUNIT_ASSERT(iNatSession->iNatStateMachine->iCurrentState==KMceNatStateWait);
    
    //initialize NOT NULL content
    srvtransaction = 
       MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( srvtransaction );
    event3.Session().iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
    
    //make sure sdp exist
    EUNIT_ASSERT(MceSip::HasContent( event3.Session().Request() ));
   iState->TransitionHandler().ExitL( event3 );
    //assert if the State changed to KMceNatStateConnected
    EUNIT_ASSERT(iNatSession->iNatStateMachine->iCurrentState==KMceNatStateConnected);
	}

TBool
UT_CMceNatStateConnected::DoesAccept( TMceNatEventCode aEventCode )
    {
    
    TMceNatStateTransitionEvent event( NULL,
    								   *iNatSession,
    								   aEventCode,
    								   *iNatSession,
    								   EMceInvite );
    return iState->Accept( event );
    }
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceNatStateConnected,
    "UT_CMceNatStateConnected",
    "UNIT" )

EUNIT_TEST(
    "DoAcceptL - test ",
    "CMceNatStateConnected",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateConnected_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceNatStateConnected",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateConnected_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceNatStateConnected",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateConnected_ExitLL, Teardown)
    
EUNIT_TEST(
		"ExitDefaultL - test ",
		"CMceNatStateConnected",
		"ExitDefaultL",
		"FUNCTIONALITY",
		SetupL, UT_CMceNatStateConnected_ExitDefaultLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
