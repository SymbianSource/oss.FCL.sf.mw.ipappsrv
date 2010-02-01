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
#include "UT_CMCENatUseCases.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcetesthelper.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "cmcetls.h"
#include "mcesipmanager.h"
#include "mcemediamanager.h"
#include "mcenatsipsession.h"
#include "mcenatsipstatemachine.h"
#include "../inc/mcenatpluginmanager.h"

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
UT_CMCENatUseCases* UT_CMCENatUseCases::NewL()
    {
    UT_CMCENatUseCases* self = UT_CMCENatUseCases::NewLC();
    CleanupStack::Pop();
    return self;
    }

UT_CMCENatUseCases* UT_CMCENatUseCases::NewLC()
    {
    UT_CMCENatUseCases* self = new( ELeave ) UT_CMCENatUseCases();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CMCENatUseCases::~UT_CMCENatUseCases()
    {
    }

// Default constructor
UT_CMCENatUseCases::UT_CMCENatUseCases()
    {
    }

// Second phase construct
void UT_CMCENatUseCases::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

CSIPClientTransaction* CreateClientTransactResp183L()
    {
    CSIPClientTransaction* transact183 = MCETestHelper::ClientTransactionLC(
            SipStrConsts::EPhraseSessionProgress, KMceSipSessionProgress, 
            SipStrConsts::EEmpty, ETrue );
    MCETestHelper::SetResponseL( *transact183,
            KMceSipSessionProgress, SipStrConsts::EPhraseSessionProgress, ETrue );
    CleanupStack::Pop( transact183 );
    return transact183;
    }

CSIPClientTransaction* CreateClientTransactResp200OKL()
    {
    CSIPClientTransaction* transact200Ok = MCETestHelper::ClientTransactionLC(
            SipStrConsts::EPhraseOk, KMceSipOK, SipStrConsts::EEmpty, ETrue );
    MCETestHelper::SetResponseL( *transact200Ok,
            KMceSipSessionProgress, SipStrConsts::EPhraseOk, ETrue );
    CleanupStack::Pop( transact200Ok );
    return transact200Ok;
    }

TInt& MediaManagerReturnStatL()
    {
    CMCETls* tls = CMCETls::Storage();
    User::LeaveIfNull( tls );
    return tls->iMediaManagerUpdateStatus;
    }

TBool& MediaManagerNeedToNegotL()
    {
    CMCETls* tls = CMCETls::Storage();
    User::LeaveIfNull( tls );
    return tls->iMediaManagerNeedToNegotiate;
    }

//  METHODS
// ---------------------------------------------------------------------------
// UT_CMCENatUseCases::NatPluginManagerReturnStatusL
// ---------------------------------------------------------------------------
TInt& UT_CMCENatUseCases::NatPluginManagerReturnStatusL()
    {
    return iServerCore->Manager().MediaManager().NatPluginManager().iReturnStatus;
    }

// ---------------------------------------------------------------------------
// UT_CMCENatUseCases::SetupL
// ---------------------------------------------------------------------------
void UT_CMCENatUseCases::SetupL()
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
	params->AppendL( _L8("") );
	params->AppendL( _L8("user@domain.com") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    iSession = CMceCsSession::NewL( *iServerCore );
    iSession->SetClientUidL( iIds, RMessage2() );    
    iSession->CreateSubSessionL( iIds, RMessage2() );
    
    iNatSipSession = static_cast<CMceNatSipSession*>(
                            (iSession->Implementation().SubSessions())[0] );
    }

// ---------------------------------------------------------------------------
// UT_CMCENatUseCases::Teardown
// ---------------------------------------------------------------------------
void UT_CMCENatUseCases::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    SdpCodecStringPool::Close();
    }

// ---------------------------------------------------------------------------
// UT_CMCENatUseCases::UT_MOcall_success
// ---------------------------------------------------------------------------
void UT_CMCENatUseCases::UT_MOcall_success()
    {
    iNatSipSession->iType = CMceCsSubSession::EOutSession;
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    MediaManagerNeedToNegotL() = ETrue;
    
    /*
    Sequence:
    (MM)Updated -->
                        << OfferReady >>
                        Updated(NAT) -->
                                                        <-- HandleSIPEvent 183/200OK(SIP/SDP)
                        << AnswerReady >>
                        <-- HandleSIPEvent(NAT)
    */

    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->Updated( iNatSipSession->ActiveBody() );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateCreateOffer !=
                  iNatSipSession->iNatStateMachine->iCurrentState )
    EUNIT_ASSERT( KMceNatStateCreateOffer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->OfferReady(
            iNatSessionId, CSdpDocument::DecodeL( KIceOffer() ) );
    EUNIT_ASSERT( KMceNatStateWait ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->SetResponse( *CreateClientTransactResp200OKL() );
    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->HandleSIPEvent( EMceResponse, iNatSipSession->Dialog()->Dialog() ); // 200 OK.
    EUNIT_ASSERT( KMceNatStateDecodeAnswer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->AnswerReady(
            iNatSessionId, CSdpDocument::DecodeL( KIceAnswer() ) );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    }

// ---------------------------------------------------------------------------
// UT_CMCENatUseCases::UT_MTcall_success
// ---------------------------------------------------------------------------
void UT_CMCENatUseCases::UT_MTcall_success()
    {
    iNatSipSession->iType = CMceCsSubSession::EInSession;
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );   
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    MediaManagerNeedToNegotL() = ETrue;
    
    CSdpDocument* document = CSdpDocument::DecodeL( KIceAnswer() );
    iNatSipSession->SetOffer( document );
    iNatSipSession->StorePrevOffer();
    /*
    Sequence:
                                                        <-- INVITE(SIP/SDP)
    (MM)Updated -->     
                        << AnswerReady >>
                        Updated(NAT) -->
                        << OfferReady >>
                        <-- SetRemoteAddress(NAT)
    */

    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->Updated( iNatSipSession->ActiveBody() );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateCreateAnswer !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateCreateAnswer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->AnswerReady( iNatSessionId,
            CSdpDocument::DecodeL( KIceAnswer() ) );
    EUNIT_ASSERT( KMceNatStateDecodeOffer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->OfferReady( iNatSessionId,
            CSdpDocument::DecodeL( KIceOffer() ) );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    }

//  TEST TABLE
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
EUNIT_BEGIN_TEST_TABLE(
    UT_CMCENatUseCases,
    "Use cases for Mce Nat code",
    "UT_CMceServer" )

EUNIT_TEST(
    "MO call success - test",
    "UT_CMCENatUseCases",
    "UT_MOcall_success",
    "FUNCTIONALITY",
    SetupL, UT_MOcall_success, Teardown)

EUNIT_TEST(
    "MT call success - test",
    "UT_CMCENatUseCases",
    "UT_MTcall_success",
    "FUNCTIONALITY",
    SetupL, UT_MTcall_success, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
