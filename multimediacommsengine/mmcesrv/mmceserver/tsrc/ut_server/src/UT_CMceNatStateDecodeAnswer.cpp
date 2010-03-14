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
#include "UT_CMceNatStateDecodeAnswer.h"

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
UT_CMceNatStateDecodeAnswer* UT_CMceNatStateDecodeAnswer::NewL()
    {
    UT_CMceNatStateDecodeAnswer* self =
    	UT_CMceNatStateDecodeAnswer::NewLC();
    CleanupStack::Pop();
    return self;
    }

UT_CMceNatStateDecodeAnswer* UT_CMceNatStateDecodeAnswer::NewLC()
    {
    UT_CMceNatStateDecodeAnswer* self =
    	new ( ELeave ) UT_CMceNatStateDecodeAnswer();
    CleanupStack::PushL( self );
	self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CMceNatStateDecodeAnswer::~UT_CMceNatStateDecodeAnswer()
    {
    }

// Default constructor
UT_CMceNatStateDecodeAnswer::UT_CMceNatStateDecodeAnswer()
    {
    }

// Second phase construct
void UT_CMceNatStateDecodeAnswer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceNatStateDecodeAnswer::SetupL()
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
	iNatSession->iNatStateMachine->iCurrentState = KMceNatStateDecodeAnswer;
    iState = iNatSession->iNatStateMachine->iStates[
    							iNatSession->iNatStateMachine->iCurrentState ]; 	  	   	
    }

void UT_CMceNatStateDecodeAnswer::Teardown()
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

void UT_CMceNatStateDecodeAnswer::
	UT_CMceNatStateDecodeAnswer_AcceptLL()
    {
    EUNIT_ASSERT( DoesAccept( EMceNatAnswer ) );
    EUNIT_ASSERT( DoesAccept( EMceNatError ) );
    EUNIT_ASSERT( DoesAccept( EMceNatICMPError ) );
           
    EUNIT_ASSERT( !DoesAccept( EMceNatInitialized ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatUpdateSdp ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatOffer ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatHandleSIP ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatUpdated ) );
    EUNIT_ASSERT( !DoesAccept( EMceNatSync ) );
       
    }

void UT_CMceNatStateDecodeAnswer::
	UT_CMceNatStateDecodeAnswer_EntryLL()
    {
    iNatSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iNatSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSession->iNatStateMachine->iCurrentState = KMceNatStateDecodeAnswer;
    iNatSession->AnswerReady( iNatSessionId, CSdpDocument::DecodeL( KIceAnswer() ) );
   
   
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSession->iNatDisabled );
          
    }
    
	
void UT_CMceNatStateDecodeAnswer::
	UT_CMceNatStateDecodeAnswer_ExitLL()
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

    // NAT EMceNatHandleSIP Handled 
    iNatSession->iNatDisabled = EFalse;
	iNatSession->iEntryPoint = CMceNatSipSession::ENatEntryHandleSIPEvent;
    eventcode = EMceNatHandleSIP;
    iNatSession->NextState( KMceStateOffering ); 
	TMceNatStateTransitionEvent event3( NULL, *iNatSession,eventcode,
    								   *iNatSession, EMceProvisionalResponse );
    EUNIT_ASSERT_LEAVE( iState->TransitionHandler().ExitL( event3 ));

	}
	
TBool
UT_CMceNatStateDecodeAnswer::DoesAccept( TMceNatEventCode aEventCode, 
											TMceSipEventCode aSIPCode )
    {
    TMceNatStateTransitionEvent event( NULL,
    								   *iNatSession,
    								   aEventCode,
    								   *iNatSession,
    								   aSIPCode );
    return iState->Accept( event );
    }		

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceNatStateDecodeAnswer,
    "UT_CMceNatStateDecodeAnswer",
    "UNIT" )

EUNIT_TEST(
    "DoAcceptL - test ",
    "CMceNatStateDecodeAnswer",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateDecodeAnswer_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceNatStateDecodeAnswer",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateDecodeAnswer_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceNatStateDecodeAnswer",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatStateDecodeAnswer_ExitLL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE

