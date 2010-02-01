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
#include "UT_CMceNatStateIdle.h"

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


// CONSTRUCTION
UT_CMceNatStateIdle* UT_CMceNatStateIdle::NewL()
    {
    UT_CMceNatStateIdle* self =
    	UT_CMceNatStateIdle::NewLC();
    CleanupStack::Pop();
    return self;
    }

UT_CMceNatStateIdle* UT_CMceNatStateIdle::NewLC()
    {
    UT_CMceNatStateIdle* self =
    	new ( ELeave ) UT_CMceNatStateIdle();
    CleanupStack::PushL( self );
	self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CMceNatStateIdle::~UT_CMceNatStateIdle()
    {
    }

// Default constructor
UT_CMceNatStateIdle::UT_CMceNatStateIdle()
    {
    }

// Second phase construct
void UT_CMceNatStateIdle::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceNatStateIdle::SetupL()
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
	iNatSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    iState = iNatSession->iNatStateMachine->iStates[
    	iNatSession->iNatStateMachine->iCurrentState ];    	
    		   	
    }

void UT_CMceNatStateIdle::Teardown()
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

void UT_CMceNatStateIdle::
	UT_CMceNatStateIdle_AcceptLL()
    {
    EUNIT_ASSERT( DoesAccept( EMceNatUpdated ) );
    EUNIT_ASSERT( DoesAccept( EMceNatError ) );
    EUNIT_ASSERT( DoesAccept( EMceNatICMPError ) );
    
    EUNIT_ASSERT( !DoesAccept( EMceNatInitialized ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatOffer ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatAnswer ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatHandleSIP ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatSync ) );
    
    }

void UT_CMceNatStateIdle::
	UT_CMceNatStateIdle_EntryLL()
    {
	// TBD    
	}
    

void UT_CMceNatStateIdle::
	UT_CMceNatStateIdle_ExitLL()
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
	
void UT_CMceNatStateIdle::
	UT_CMceNatStateIdle_Exit2LL()
	
	{
	// Exit Default (MO)
	iNatSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
  	iNatSession->iType = CMceCsSubSession::EOutSession; // make this MT case
   	
   	TMceNatEventCode eventcode = EMceNatUpdated;
   	
	iNatSession->iNatDisabled = EFalse;
    TMceNatStateTransitionEvent event1( NULL, *iNatSession,eventcode,
    								   *iNatSession, EMceInvite );
    MCE_EUNIT_ASSERT_NO_LEAVE( iState->TransitionHandler().ExitL( event1 ));
		
	EUNIT_ASSERT( KMceNatStateWait ==
                  iNatSession->iNatStateMachine->iCurrentState );
	}
	
	
void UT_CMceNatStateIdle::
	UT_CMceNatStateIdle_Exit2_1LL()
	{
	// Pull mode Exit Default (MT)
	
	iNatSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
  
    CSIPServerTransaction* trx = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, EFalse );
    CleanupStack::PushL(trx);
    EUNIT_ASSERT( &iNatSession->Request() == NULL );
    iNatSession->iPendingReceivedRequests.AppendL( trx );
    CleanupStack::Pop(trx);
    EUNIT_ASSERT( &iNatSession->Request() == trx );
        
    iNatSession->iType = CMceCsSubSession::EInSession; // make this MT case
   	
   	TMceNatEventCode eventcode = EMceNatUpdated;
   	
	iNatSession->iNatDisabled = EFalse;
    TMceNatStateTransitionEvent event1( NULL, *iNatSession,eventcode,
    								   *iNatSession, EMceInvite );
    MCE_EUNIT_ASSERT_NO_LEAVE( iState->TransitionHandler().ExitL( event1 ));
		
	EUNIT_ASSERT( KMceNatStateWait ==
                  iNatSession->iNatStateMachine->iCurrentState );
                                
    	
	}	
		
	
TBool
UT_CMceNatStateIdle::DoesAccept( TMceNatEventCode aEventCode )
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
    UT_CMceNatStateIdle,
    "UT_CMceNatStateIdle",
    "UNIT" )

EUNIT_TEST(
    "DoAcceptL - test ",
    "CMceNatStateIdle",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateIdle_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceNatStateIdle",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateIdle_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceNatStateIdle",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateIdle_ExitLL, Teardown)
    
EUNIT_TEST(
    "ExitL Default- test ",
    "CMceNatStateIdle",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateIdle_Exit2LL, Teardown)
    
EUNIT_TEST(
    "ExitL Default Pull Mode - test ",
    "CMceNatStateIdle",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateIdle_Exit2_1LL, Teardown)
    
         

EUNIT_END_TEST_TABLE

//  END OF FILE
