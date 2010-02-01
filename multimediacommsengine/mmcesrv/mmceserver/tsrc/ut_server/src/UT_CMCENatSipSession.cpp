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
#include "UT_CMCENatSipSession.h"

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

_LIT8( KIceLoopback,
"v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 192.168.0.12\r\n\
s=-\r\n\
c=IN IP4 127.0.0.1\r\n\
t=0 0\r\n\
m=audio 14433 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14434 IN IP4 127.0.0.1\r\n" );

_LIT8( KIceUpdate,
"v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 192.168.0.12\r\n\
s=-\r\n\
c=IN IP4 172.0.0.19\r\n\
t=0 0\r\n\
m=audio 5356 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:6333 IN IP4 127.0.0.1\r\n" );

_LIT8( KPrevOffer, 
"v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n\
s=-\r\n\
c=IN IP4 192.0.0.19\r\n\
t=0 0\r\n\
a=ice-ufrag:xStE\r\n\
a=ice-pwd:ermyyyErTTeseEWWtr\r\n\
m=audio 6231 RTP/AVP 96\r\n\
a=candidate:1 1 UDP 11435664 192.0.0.19 6231 typ srflx\r\n\
a=candidate:1 2 UDP 10435664 192.0.0.19 6232 typ srflx\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:14444 IN IP4 131.117.0.4\r\n" );

_LIT8( KCurrOffer,
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
// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::NewL
// ---------------------------------------------------------------------------
//
UT_CMceNatSipSession* UT_CMceNatSipSession::NewL()
    {
    UT_CMceNatSipSession* self = UT_CMceNatSipSession::NewLC(); 
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::NewLC
// ---------------------------------------------------------------------------
//
UT_CMceNatSipSession* UT_CMceNatSipSession::NewLC()
    {
    UT_CMceNatSipSession* self = new( ELeave ) UT_CMceNatSipSession();
    CleanupStack::PushL( self );
	self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::~UT_CMceNatSipSession
// ---------------------------------------------------------------------------
//
UT_CMceNatSipSession::~UT_CMceNatSipSession()
    {
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession
// ---------------------------------------------------------------------------
//
UT_CMceNatSipSession::UT_CMceNatSipSession()
    {
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::ConstructL
// ---------------------------------------------------------------------------
//
void UT_CMceNatSipSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

// ---------------------------------------------------------------------------
// CreateClientTransactionResp183L
// ---------------------------------------------------------------------------
//
CSIPClientTransaction* CreateClientTransactionResp183L()
    {
    CSIPClientTransaction* transact183 = MCETestHelper::ClientTransactionLC(
            SipStrConsts::EPhraseSessionProgress, KMceSipSessionProgress, 
            SipStrConsts::EEmpty, ETrue );
    MCETestHelper::SetResponseL( *transact183,
            KMceSipSessionProgress, SipStrConsts::EPhraseSessionProgress, ETrue );
    CleanupStack::Pop( transact183 );
    return transact183;
    }

// ---------------------------------------------------------------------------
// CreateClientTransactionResp200OKL
// ---------------------------------------------------------------------------
//
CSIPClientTransaction* CreateClientTransactionResp200OKL()
    {
    CSIPClientTransaction* transact200Ok = MCETestHelper::ClientTransactionLC(
            SipStrConsts::EPhraseOk, KMceSipOK, SipStrConsts::EEmpty, ETrue );
    MCETestHelper::SetResponseL( *transact200Ok,
            KMceSipSessionProgress, SipStrConsts::EPhraseOk, ETrue );
    CleanupStack::Pop( transact200Ok );
    return transact200Ok;
    }

// ---------------------------------------------------------------------------
// MediaManagerReturnStatusL
// ---------------------------------------------------------------------------
//
TInt& MediaManagerReturnStatusL()
    {
    CMCETls* tls = CMCETls::Storage();
    User::LeaveIfNull( tls );
    return tls->iMediaManagerUpdateStatus;
    }

// ---------------------------------------------------------------------------
// MediaManagerNeedToNegotiateL
// ---------------------------------------------------------------------------
//
TBool& MediaManagerNeedToNegotiateL()
    {
    CMCETls* tls = CMCETls::Storage();
    User::LeaveIfNull( tls );
    return tls->iMediaManagerNeedToNegotiate;
    }

//  METHODS
// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::NatPluginManagerReturnStatusL
// ---------------------------------------------------------------------------
TInt& UT_CMceNatSipSession::NatPluginManagerReturnStatusL()
    {
    return iServerCore->Manager().MediaManager().NatPluginManager().iReturnStatus;
    }
    
// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::SetupL
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::SetupL()
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
    iNatSipSession->SetOffer( CSdpDocument::DecodeL( KIceOffer() ) );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::Setup2L
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::Setup2L()
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    
    iIds.iAppUID = 10;
    iIds.iProfileID = 0;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iServerCore = CMceServerCore::NewL();    
    iStorage = CMCETls::Storage();
    
    iNatSessionId = 3;
    iServerCore->Manager().MediaManager().NatPluginManager().iEnabled = ETrue;    
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("sip:originator@domain.com") ); // originator
	params->AppendL( _L8("sip:recipient@domain.com") ); // recipient
	params->AppendL( _L8("6") ); // iap id
    
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
    
// -----------------------------------------------------------------------------
// UT_CMceNatSipSession::Teardown
// -----------------------------------------------------------------------------
void UT_CMceNatSipSession::Teardown()
    {
    CMCETls::Close();
	delete iSession;
	iSession = NULL;
    delete iServerCore;
    iServerCore = NULL;
    SdpCodecStringPool::Close();
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateMediaLL
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateMediaLL() 
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );

    CSIPServerTransaction* srvtransaction = 
       MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( srvtransaction );
    iNatSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    MediaManagerNeedToNegotiateL() = EFalse;
    MediaManagerReturnStatusL() = KMceAsync;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->UpdateMediaL();
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    MediaManagerNeedToNegotiateL() = ETrue;
    MediaManagerReturnStatusL() = KMceAsync;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->UpdateMediaL();
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateEstablished ); // MO SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    MediaManagerReturnStatusL() = KMceReady;
    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->UpdateMediaL();

    MCE_CHECK_MEMORY_LEAVE( KMceNatStateCreateOffer !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateCreateOffer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateEstablished ); // MO SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    MediaManagerReturnStatusL() = KMceReady;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->UpdateMediaL();
	
	MCE_CHECK_MEMORY_LEAVE( KMceNatStateWait !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateWait ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateServerEstablishing ); // MT SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    MediaManagerReturnStatusL() = KMceAsync;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->UpdateMediaL();
	MCE_CHECK_MEMORY_LEAVE( KMceNatStateConnected !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateServerEstablishing ); // MT SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    MediaManagerReturnStatusL() = KMceReady;
    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->UpdateMediaL();
	MCE_CHECK_MEMORY_LEAVE( KMceNatStateCreateAnswer !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateCreateAnswer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateServerEstablishing ); // MT SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    MediaManagerReturnStatusL() = KMceReady;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->UpdateMediaL();
	MCE_CHECK_MEMORY_LEAVE( KMceNatStateConnected !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdatedL
// The the test case UT_CMceNatSipSession_UpdatedL has been split into three
// smaller cases (into UT_CMceNatSipSession_UpdatedL,
// UT_CMceNatSipSession_Updated2L and UT_CMceNatSipSession_Updated3L), which
// can be executed under 60s each.
// This enables running of the cases so that the minute value of the clock in
// WINSCW emulator doesn't change, since that causes random memory leaks with
// EUnit.
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdatedL()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( srvtransaction );
    iNatSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );
    
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateEstablished ); // MO SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->Updated( *iNatSipSession->iBody );
	MCE_CHECK_MEMORY_LEAVE( KMceNatStateWait !=
                  iNatSipSession->iNatStateMachine->iCurrentState )   

    EUNIT_ASSERT( KMceNatStateWait ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateEstablished ); // MO SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->Updated( *iNatSipSession->iBody );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateCreateOffer !=
                  iNatSipSession->iNatStateMachine->iCurrentState )

    EUNIT_ASSERT( KMceNatStateCreateOffer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateEstablished ); // MO SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->Updated( *iNatSipSession->iBody );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateWait !=
                  iNatSipSession->iNatStateMachine->iCurrentState );

	EUNIT_ASSERT( KMceNatStateWait ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateEstablished ); // MO SIP state
    MediaManagerNeedToNegotiateL() = EFalse;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->Updated( *iNatSipSession->iBody );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateConnected !=
                  iNatSipSession->iNatStateMachine->iCurrentState );

    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_Updated2L
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_Updated2L() 
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    CSIPServerTransaction* srvtransaction = 
       MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( srvtransaction );
    iNatSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateServerEstablishing ); // MT SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->Updated( *iNatSipSession->iBody );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateConnected !=
                  iNatSipSession->iNatStateMachine->iCurrentState );

    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );

    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateServerEstablishing ); // MT SIP state
    MediaManagerNeedToNegotiateL() = EFalse;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->Updated( *iNatSipSession->iBody );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateConnected !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_Updated3L
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_Updated3L()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );

    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, ETrue );
    CleanupStack::PushL( srvtransaction );
    iNatSipSession->iPendingReceivedRequests.AppendL( srvtransaction );
    CleanupStack::Pop( srvtransaction );    
    
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateServerEstablishing ); // MT SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->Updated( *iNatSipSession->iBody );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateConnected !=
                  iNatSipSession->iNatStateMachine->iCurrentState );

    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );



    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateServerEstablishing ); // MT SIP state
    MediaManagerNeedToNegotiateL() = ETrue;
    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->Updated( *iNatSipSession->iBody );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateCreateAnswer !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateCreateAnswer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_HandleSIPEvent
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_HandleSIPEvent()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iNatSipSession->SetResponse( *CreateClientTransactionResp183L() );
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateWait;
    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->HandleSIPEvent( EMceProvisionalResponse, iNatSipSession->Dialog()->Dialog() );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateDecodeAnswer !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateDecodeAnswer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->SetResponse( *CreateClientTransactionResp183L() );
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateWait;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->HandleSIPEvent( EMceProvisionalResponse, iNatSipSession->Dialog()->Dialog() );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateConnected !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->SetResponse( *CreateClientTransactionResp200OKL() );
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateWait;
    NatPluginManagerReturnStatusL() = KMceAsync;
    iNatSipSession->HandleSIPEvent( EMceResponse, iNatSipSession->Dialog()->Dialog() );
    MCE_CHECK_MEMORY_LEAVE( KMceNatStateDecodeAnswer !=
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( KMceNatStateDecodeAnswer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->SetResponse( *CreateClientTransactionResp200OKL() );
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateWait;
    NatPluginManagerReturnStatusL() = KMceReady;
    iNatSipSession->HandleSIPEvent( EMceResponse, iNatSipSession->Dialog()->Dialog() );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_StorePrevOffer
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_StorePrevOffer()
    {    
    CSdpDocument* currOffer = CSdpDocument::DecodeLC( KCurrOffer() );
    CSdpDocument* prevOffer = CSdpDocument::DecodeLC( KIceUpdate() );
    
    iNatSipSession->SetOffer( prevOffer );// ownership changed
    CleanupStack::Pop( prevOffer );
    iNatSipSession->StorePrevOffer();
    	
    iNatSipSession->SetOffer( currOffer );// ownership changed
    CleanupStack::Pop( currOffer );
    
    MCE_CHECK_MEMORY_LEAVE( !iNatSipSession->PrevOffer() );
    EUNIT_ASSERT( iNatSipSession->PrevOffer() );
    EUNIT_ASSERT( iNatSipSession->Offer() != iNatSipSession->PrevOffer() );
    EUNIT_ASSERT( currOffer == iNatSipSession->Offer() );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_Offer
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_Offer()
    {
    CSdpDocument* currOffer = CSdpDocument::DecodeLC( KCurrOffer() );
    CSdpDocument* prevOffer = CSdpDocument::DecodeLC( KIceUpdate() );    
    
    iNatSipSession->SetOffer( prevOffer );// ownership changed
    CleanupStack::Pop( prevOffer );
    iNatSipSession->StorePrevOffer();
    
    iNatSipSession->SetOffer( currOffer );// ownership changed
    CleanupStack::Pop( currOffer );
    
    MCE_CHECK_MEMORY_LEAVE( !iNatSipSession->PrevOffer() );
    EUNIT_ASSERT( iNatSipSession->PrevOffer() );
    iNatSipSession->SetOffered( KMceNatPrevOfferSdp );
    EUNIT_ASSERT( iNatSipSession->Offer() != currOffer );
    iNatSipSession->SetOffered( KMceNatOfferSdp );
    EUNIT_ASSERT( iNatSipSession->Offer() == currOffer );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_InitializedMO
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_InitializedMO() 
    {
    iNatSipSession->iBody = 
        CMceComSession::NewL( CMceComSession::EOutSession );
    
    // Valid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateInitializing;
    iNatSipSession->Initialized( iNatSessionId );
    EUNIT_ASSERT( KMceNatStateCreateOfferRequested ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

    iNatSipSession->Updated( iNatSipSession->ActiveBody() );
	MCE_CHECK_MEMORY_LEAVE( KMceNatStateWait !=
                  iNatSipSession->iNatStateMachine->iCurrentState )
	EUNIT_ASSERT( KMceNatStateWait ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

    // Invalid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateInitializing;
    EUNIT_ASSERT_PANIC( iNatSipSession->Initialized( iNatSessionId+1 ),
            "MceServer", KErrArgument );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_InitializedMT
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_InitializedMT() 
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    iNatSipSession->iType = CMceCsSubSession::EInSession; // make this MT case

    // Valid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateInitializing;
    iNatSipSession->Initialized( iNatSessionId );
    EUNIT_ASSERT( KMceNatStateCreateOfferRequested ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

	
	iNatSipSession->Updated( iNatSipSession->ActiveBody() );
	EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

    // Invalid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateInitializing;
    EUNIT_ASSERT_PANIC( iNatSipSession->Initialized( iNatSessionId+1 ),
            "MceServer", KErrArgument );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_InitializedMT_PullMode
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_InitializedMT_PullMode() 
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    
    TMceIds ids;
    CMceMsgBase* msg = NULL;
  
    CSIPServerTransaction* trx = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite, EFalse );
    CleanupStack::PushL(trx);
    EUNIT_ASSERT( &iNatSipSession->Request() == NULL );
    iNatSipSession->iPendingReceivedRequests.AppendL( trx );
    CleanupStack::Pop(trx);
    EUNIT_ASSERT( &iNatSipSession->Request() == trx );
    iNatSipSession->iType = CMceCsSubSession::EInSession; // make this MT case

    // Valid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateInitializing;
    iNatSipSession->Initialized( iNatSessionId );
    EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );

    // Invalid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateInitializing;
    EUNIT_ASSERT_PANIC( iNatSipSession->Initialized( iNatSessionId+1 ),
            "MceServer", KErrArgument );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_Initialized_Panic2
// ---------------------------------------------------------------------------    
void UT_CMceNatSipSession::UT_CMceNatSipSession_Initialized_Panic2() 
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );    
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    EUNIT_ASSERT_PANIC( iNatSipSession->Initialized( iNatSessionId ),
            "MceServer", KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_OfferReady
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_OfferReady()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    // Valid
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateCreateOffer;
    iNatSipSession->OfferReady( iNatSessionId, CSdpDocument::DecodeL( KIceOffer() ) );
    EUNIT_ASSERT( KMceNatStateWait ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateDecodeOffer;
    iNatSipSession->OfferReady( iNatSessionId, CSdpDocument::DecodeL( KIceOffer() ) );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    // Invalid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    EUNIT_ASSERT_PANIC( iNatSipSession->OfferReady( iNatSessionId+1,
            CSdpDocument::DecodeL( KIceOffer() ) ), "MceServer", KErrArgument );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_OfferReady_Panic2
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_OfferReady_Panic2()
    {
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    EUNIT_ASSERT_PANIC( iNatSipSession->OfferReady( iNatSessionId,
            CSdpDocument::DecodeL( KIceOffer() ) ), "MceServer", KErrNotReady );
    }    

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_AnswerReady
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_AnswerReady()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    // Valid
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateDecodeAnswer;
    iNatSipSession->AnswerReady( iNatSessionId, CSdpDocument::DecodeL( KIceAnswer() ) );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateCreateAnswer;
    iNatSipSession->AnswerReady( iNatSessionId, CSdpDocument::DecodeL( KIceAnswer() ) );
    EUNIT_ASSERT( KMceNatStateDecodeOffer ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
    
    // Invalid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    EUNIT_ASSERT_PANIC( iNatSipSession->AnswerReady( iNatSessionId+1,
            CSdpDocument::DecodeL( KIceAnswer() ) ), "MceServer", KErrArgument );
    }
    
// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_AnswerReady_Panic2
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_AnswerReady_Panic2()
    {
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    EUNIT_ASSERT_PANIC( iNatSipSession->AnswerReady( iNatSessionId,
            CSdpDocument::DecodeL( KIceAnswer() ) ), "MceServer", KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );

    // Valid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    
    iNatSipSession->UpdateSdp( iNatSessionId, CSdpDocument::DecodeL( KIceUpdate() ) );
    
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    
    // Invalid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    EUNIT_ASSERT_PANIC( iNatSipSession->UpdateSdp( iNatSessionId+1,
            CSdpDocument::DecodeL( KIceUpdate() ) ), "MceServer", KErrArgument );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic2
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic2()
    {
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateInitializing;
    EUNIT_ASSERT_PANIC( iNatSipSession->UpdateSdp( iNatSessionId,
            CSdpDocument::DecodeL( KIceUpdate() ) ), "MceServer", KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic3
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic3()
    {
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateCreateOffer;
    EUNIT_ASSERT_PANIC( iNatSipSession->UpdateSdp( iNatSessionId,
            CSdpDocument::DecodeL( KIceUpdate() ) ), "MceServer", KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic4
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic4()
    {
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateCreateAnswer;
    EUNIT_ASSERT_PANIC( iNatSipSession->UpdateSdp( iNatSessionId,
            CSdpDocument::DecodeL( KIceUpdate() ) ), "MceServer", KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic5
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic5()
    {
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateDecodeOffer;
    EUNIT_ASSERT_PANIC( iNatSipSession->UpdateSdp( iNatSessionId,
            CSdpDocument::DecodeL( KIceUpdate() ) ), "MceServer", KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic6
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic6()
    {
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateDecodeAnswer;
    EUNIT_ASSERT_PANIC( iNatSipSession->UpdateSdp( iNatSessionId,
            CSdpDocument::DecodeL( KIceUpdate() ) ), "MceServer", KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic7
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateSdp_Panic7()
    {
    iNatSipSession->SetResponse( *CreateClientTransactionResp200OKL() );
    iNatSipSession->iSipEvent = EMceResponse;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateWait;
    EUNIT_ASSERT_PANIC( iNatSipSession->UpdateSdp( iNatSessionId,
            CSdpDocument::DecodeL( KIceUpdate() ) ), "MceServer", KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_ErrorOccurred
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_ErrorOccurred()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    // Valid
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->ErrorOccurred( iNatSessionId, KErrNotReady );
    EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( iNatSipSession->iNatDisabled );
    
    iNatSipSession->iNatDisabled = EFalse;
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateCreateOffer;
    iNatSipSession->ErrorOccurred( iNatSessionId, KErrNotReady );
    EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( iNatSipSession->iNatDisabled );
    
    iNatSipSession->iNatDisabled = EFalse;
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateCreateAnswer;
    iNatSipSession->ErrorOccurred( iNatSessionId, KErrNotReady );
    EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( iNatSipSession->iNatDisabled );
    
    iNatSipSession->iNatDisabled = EFalse;
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateDecodeOffer;
    iNatSipSession->ErrorOccurred( iNatSessionId, KErrNotReady );
    EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( iNatSipSession->iNatDisabled );
    
    iNatSipSession->iNatDisabled = EFalse;
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateDecodeAnswer;
    iNatSipSession->ErrorOccurred( iNatSessionId, KErrNotReady );
    EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( iNatSipSession->iNatDisabled );
    
    iNatSipSession->iNatDisabled = EFalse;
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateWait;
    iNatSipSession->ErrorOccurred( iNatSessionId, KErrNotReady );
    EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( iNatSipSession->iNatDisabled );
    
    // Invalid
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateIdle;
    EUNIT_ASSERT_PANIC( iNatSipSession->ErrorOccurred( iNatSessionId+1,
            KErrNotReady ), "MceServer", KErrArgument );
    }
    
// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_ErrorOccurred_Panic2
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_ErrorOccurred_Panic2()
    {
    //no panic
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateInitializing;
    iNatSipSession->ErrorOccurred( iNatSessionId, KErrNotReady );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_IcmpErrorOccurred
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_IcmpErrorOccurred()
    {
    iNatSipSession->iBody = 
        CMceComSession::NewL( CMceComSession::EOutSession );
    
    // ICMP Error Handled ( NatState = KMceNatStateConnected )
    // IcmpError Occured ( SipState = KMceStateEstablished )
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateEstablished ); // MO SIP state
   
    iNatSipSession->IcmpErrorOccurred( iNatSessionId, EMceNatICMPError );
    EUNIT_ASSERT( KMceNatStateIdle ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( iNatSipSession->iNatDisabled );
        
	// ICMP Error IGnored ( NatState = KMceNatStateConnected )
    // IcmpError Occured ( SipState = KMceStateServerOffering )
    iNatSipSession->iNatDisabled = EFalse;
    iNatSipSession->iEntryPoint = CMceNatSipSession::ENatEntryUpdated;
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    iNatSipSession->NextState( KMceStateServerOffering ); 
   
    iNatSipSession->IcmpErrorOccurred( iNatSessionId, EMceNatICMPError );
    EUNIT_ASSERT( KMceNatStateConnected ==
                  iNatSipSession->iNatStateMachine->iCurrentState );
    EUNIT_ASSERT( !iNatSipSession->iNatDisabled );
        
        
    // Invalid (NatState = KMceNatStateConnected )
    iNatSipSession->iNatStateMachine->iCurrentState = KMceNatStateConnected;
    EUNIT_ASSERT_PANIC( iNatSipSession->IcmpErrorOccurred( iNatSessionId+1,
            EMceNatICMPError ), "MceServer", KErrArgument );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_CreateOfferL
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_CreateOfferL()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    // Valid
    EUNIT_ASSERT_NO_LEAVE( iNatSipSession->CreateOfferL() );
    
    // Leave
    iNatSipSession->SetOffer( NULL );
    EUNIT_ASSERT_LEAVE( iNatSipSession->CreateOfferL() );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_ResolveL
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_ResolveL()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EInSession );
    
    // Valid
    iNatSipSession->StorePrevOffer();
    iNatSipSession->SetOffer( CSdpDocument::DecodeL( KIceAnswer() ) );
    MCE_EUNIT_ASSERT_NO_LEAVE( iNatSipSession->ResolveL());
    
    // Leave
    iNatSipSession->SetOffer( NULL );
    EUNIT_ASSERT_LEAVE( iNatSipSession->ResolveL() );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_DecodeAnswerL
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_DecodeAnswerL()
    {
    // Valid
    EUNIT_ASSERT_NO_LEAVE( iNatSipSession->DecodeAnswerL() );
    
    // Leave
    iNatSipSession->SetOffer( NULL );
    EUNIT_ASSERT_LEAVE( iNatSipSession->DecodeAnswerL() );
    }  

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateL()
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateL()
    {
    // Valid
    EUNIT_ASSERT_NO_LEAVE( iNatSipSession->UpdateL() );
    
    // Leave
    iNatSipSession->SetOffer( NULL );
    EUNIT_ASSERT_LEAVE( iNatSipSession->UpdateL() );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateRemoteAddressL()
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_UpdateRemoteAddressL()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    EUNIT_ASSERT_NO_LEAVE( iNatSipSession->UpdateRemoteAddressL() );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_Continue()
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_Continue()
    {
    iNatSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    // Invalid
    iNatSipSession->iEntryPoint = (CMceNatSipSession::TMceNatEntryPoint) 10;
    iNatSipSession->iSipEvent =  EMceResponse;
    EUNIT_ASSERT_PANIC( iNatSipSession->Continue(), 
            "MceServer", KErrTotalLossOfPrecision );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_NatErrorOccurred
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_NatErrorOccurred()
    {
    iNatSipSession->NatErrorOccurred( KErrNotFound );
    EUNIT_ASSERT( iNatSipSession->iNatDisabled );
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_NatSessionWithoutProfile
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_NatSessionWithoutProfile()
    {
    // Everything is fine if session was created fine in Setup2L
    EUNIT_ASSERT( ETrue ); 
    }

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_GetResourceReservationStatusL
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_GetResourceReservationStatusL()
	{
	TInt status = iNatSipSession->GetResourceReservationStatus();	
	EUNIT_ASSERT(status == CNSPPlugin::ENSPResourcesNotReserved );
	}

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_SetNeedToReceive
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_SetNeedToReceive()
	{
	iNatSipSession->SetNeedtoReceive();
	EUNIT_ASSERT( iNatSipSession->iForceSDPSending == ETrue );
	EUNIT_ASSERT( iNatSipSession->ForceSDPSendingStatus() == ETrue );
	}

// ---------------------------------------------------------------------------
// UT_CMceNatSipSession::UT_CMceNatSipSession_PluginInitAfterMedia
// ---------------------------------------------------------------------------
void UT_CMceNatSipSession::UT_CMceNatSipSession_PluginInitAfterMedia()
	{
	// Media is updated before the NAT plugin has been initialized. Should
	// wait plugin to get initialized in CreateOfferRequested state.
	
	// TODO:
	}
   
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// TEST TABLE
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceNatSipSession,
    "UT_CMceNatSipSession",
    "UNIT" )

EUNIT_TEST(
    "UpdateMediaL - test",
    "CMceNatSipSession",
    "UpdateMediaL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateMediaLL, Teardown)

EUNIT_TEST(
    "Updated - test",
    "CMceNatSipSession",
    "Updated",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdatedL, Teardown)

EUNIT_TEST(
    "Updated 2 - test",
    "CMceNatSipSession",
    "Updated",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_Updated2L, Teardown)

EUNIT_TEST(
    "Updated 3 - test",
    "CMceNatSipSession",
    "Updated",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_Updated3L, Teardown)

EUNIT_TEST(
    "HandleSIPEvent - test",
    "CMceNatSipSession",
    "HandleSIPEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_HandleSIPEvent, Teardown)

EUNIT_TEST(
    "StorePrevOffer - test",
    "CMceNatSipSession",
    "StorePrevOffer",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_StorePrevOffer, Teardown)

EUNIT_TEST(
    "Offer - test",
    "CMceNatSipSession",
    "Offer",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_Offer, Teardown)

EUNIT_TEST(
    "Initialized MO - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_InitializedMO",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_InitializedMO, Teardown)

EUNIT_TEST(
    "Initialized MT - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_InitializedMT",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_InitializedMT, Teardown)
    
EUNIT_TEST(
    "Initialized MT Pull Mode - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_InitializedMT_PullMode",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_InitializedMT_PullMode, Teardown)
        

EUNIT_TEST(
    "IniTialized_Panic2 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_Initialized_Panic2",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_Initialized_Panic2, Teardown)    

EUNIT_TEST(
    "OfferReady - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_OfferReady",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_OfferReady, Teardown)
     
EUNIT_TEST(
    "OfferReady_Panic2 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_OfferReady_Panic2",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_OfferReady_Panic2, Teardown)    
    
EUNIT_TEST(
    "AnswerReady - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_AnswerReady",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_AnswerReady, Teardown)
   
EUNIT_TEST(
    "AnswerReady_Panic2 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_AnswerReady_Panic2",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_AnswerReady_Panic2, Teardown)    

EUNIT_TEST(
    "UpdateSdp - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateSdp",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateSdp, Teardown)
 
EUNIT_TEST(
    "UpdateSdp_Panic2 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateSdp_Panic2",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateSdp_Panic2, Teardown)    

EUNIT_TEST(
    "UpdateSdp_Panic3 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateSdp_Panic3",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateSdp_Panic3, Teardown)

EUNIT_TEST(
    "UpdateSdp_Panic4 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateSdp_Panic4",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateSdp_Panic4, Teardown)

EUNIT_TEST(
    "UpdateSdp_Panic5 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateSdp_Panic5",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateSdp_Panic5, Teardown)

EUNIT_TEST(
    "UpdateSdp_Panic6 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateSdp_Panic6",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateSdp_Panic6, Teardown)

EUNIT_TEST(
    "UpdateSdp_Panic7 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateSdp_Panic7",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateSdp_Panic7, Teardown)

EUNIT_TEST(
    "ErrorOccurred - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_ErrorOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_ErrorOccurred, Teardown)
   
EUNIT_TEST(
    "ErrorOccurred_Panic2 - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_ErrorOccurred_Panic2",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_ErrorOccurred_Panic2, Teardown)

EUNIT_TEST(
    "IcmpErrorOccurred - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_IcmpErrorOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_IcmpErrorOccurred, Teardown)

EUNIT_TEST(
    "CreateOfferL - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_CreateOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_CreateOfferL, Teardown)

EUNIT_TEST(
    "ResolveL - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_ResolveL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_ResolveL, Teardown)

EUNIT_TEST(
    "DecodeAnswerL - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_DecodeAnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_DecodeAnswerL, Teardown)

EUNIT_TEST(
    "UpdateL - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateL, Teardown)

EUNIT_TEST(
    "UpdateRemoteAddressL - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_UpdateRemoteAddressL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_UpdateRemoteAddressL, Teardown)

EUNIT_TEST(
    "Continue - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_Continue",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_Continue, Teardown)

EUNIT_TEST(
    "NatErrorOccurred - test ",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_NatErrorOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_NatErrorOccurred, Teardown)

EUNIT_TEST(
    "NatSessionWithoutProfile - test ",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_NatSessionWithoutProfile",
    "FUNCTIONALITY",
    Setup2L, UT_CMceNatSipSession_NatSessionWithoutProfile, Teardown)
    
EUNIT_TEST(
    "GetResourceReservationStatus - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_GetResourceReservationStatusL",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_GetResourceReservationStatusL, Teardown)

EUNIT_TEST(
    "SetNeedtoReceive - test",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_SetNeedToReceive",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_SetNeedToReceive, Teardown)

EUNIT_TEST(
    "Plugin initializes after media is ready",
    "CMceNatSipSession",
    "UT_CMceNatSipSession_PluginInitAfterMedia",
    "FUNCTIONALITY",
    SetupL, UT_CMceNatSipSession_PluginInitAfterMedia, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
