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
#include "ut_cmcecssubsession.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MCETestHelper.h"
#include "mcecssubsession.h"
#include "mcecssessionimplementation.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssubsessionreceiver.h"
#include "mcecsserveritc.h"
#include "mceserver.pan"
#include "mceserial.h"
#include "mcecomsession.h"
#include "mcecomaudiostream.h"
#include "mcecomamrcodec.h"
#include "mcecomrtpsink.h"
#include "mcecomrtpsource.h"
#include "mcemessagedispatcher.h"

_LIT8( KReceiveMessage, "received");


// CONSTRUCTION
UT_CMceCsSubSession* UT_CMceCsSubSession::NewL()
    {
    UT_CMceCsSubSession* self = UT_CMceCsSubSession::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceCsSubSession* UT_CMceCsSubSession::NewLC()
    {
    UT_CMceCsSubSession* self = new( ELeave ) UT_CMceCsSubSession();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }


// Destructor (virtual by CBase)
UT_CMceCsSubSession::~UT_CMceCsSubSession()
    {
    }

// Default constructor
UT_CMceCsSubSession::UT_CMceCsSubSession()
    {
    }

// Second phase construct
void UT_CMceCsSubSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceCsSubSession::SetupL(  )
    {

    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    
    
    //TMceIds ids;
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iServerCore = CMceServerCore::NewL();
    
    iStorage = CMCETls::Storage();
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    iSession = CMceCsSession::NewL( *iServerCore );
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSubSession =  static_cast<CMceCsSubSession*>((iSession->Implementation().SubSessions())[0]);
    } 

void UT_CMceCsSubSession::Teardown(  )
    {
    CMCETls::Close();
    delete iSession;
    delete iServerCore;
    SdpCodecStringPool::Close();
    }



void UT_CMceCsSubSession::UT_CMceCsSubSession_GetFunctionsL(  )
    {
    
    EUNIT_ASSERT( iSubSession->Type() == KMceCSSIPSession );
    EUNIT_ASSERT( iSubSession->DialogId() == 1 );

    CSIPDialogAssocBase* dialog = iSubSession->Dialog();
    EUNIT_ASSERT( dialog != NULL);
    CSIPServerTransaction& request = iSubSession->Request();
    EUNIT_ASSERT( &request == NULL);
    CSIPClientTransaction& response = iSubSession->Response();
    EUNIT_ASSERT( &response == NULL);
    CMceCsSessionImplementation& session = iSubSession->Client();
    EUNIT_ASSERT(&session != NULL);
    CMceSipConnection& sipconnection = iSubSession->SIPConnection();
    EUNIT_ASSERT(&sipconnection != NULL);
    CMceSipManager& manager = iSubSession->Manager();
    EUNIT_ASSERT(&manager != NULL);
    const RPointerArray<CSIPClientTransaction>& pendingTransactions = 
        iSubSession->PendingTransactions();
    EUNIT_ASSERT( pendingTransactions.Count() == 0 );
    const CFCSession* fc = iSubSession->FCSession();
    EUNIT_ASSERT(&fc != NULL);
    
//test setting and getting of spare slots of TMceIds    
    MCEIDS_DEFINE_SPARE_INDEX_1( kSpare1 );    
    MCEIDS_DEFINE_SPARE_INDEX_2( kSpare2 );    
    MCEIDS_DEFINE_SPARE_INDEX_3( kSpare3 );    
    
    iIds.iSpare1 = 1;
    iIds.iSpare2 = 2;
    iIds.iSpare3 = 3;
    
    EUNIT_ASSERT( iIds.Get( kSpare1 ) == 1 );
    EUNIT_ASSERT( iIds.Get( kSpare2 ) == 2 );
    EUNIT_ASSERT( iIds.Get( kSpare3 ) == 3 );
    
    iIds.Set( kSpare1, 0 );
    EUNIT_ASSERT( iIds.Get( kSpare1 ) == 0 );
    EUNIT_ASSERT( iIds.iSpare1 == 0 );
    
    iIds.Set( kSpare2, 0 );
    EUNIT_ASSERT( iIds.Get( kSpare2 ) == 0 );
    EUNIT_ASSERT( iIds.iSpare2 == 0 );
    
    iIds.Set( kSpare3, 0 );
    EUNIT_ASSERT( iIds.Get( kSpare3 ) == 0 );
    EUNIT_ASSERT( iIds.iSpare3 == 0 );

    }

void UT_CMceCsSubSession::UT_CMceCsSubSession_ServiceLL()
    {
    
    HBufC8* message = KReceiveMessage().AllocLC();
    iSubSession->iReceiver->SendToClientL( iStorage->iIds, message );
    CleanupStack::Pop( message );
    
    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcReceive;
    iSubSession->ServiceL( iIds, EMceItcReceive, RMessage2() );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
	EUNIT_ASSERT( iStorage->iContextServer != NULL );
	EUNIT_ASSERT( iStorage->iContextServer->Compare( KReceiveMessage ) == 0 );
    
    HBufC8* returnMessage = NULL;
    
    CMceMsgBase* msg = NULL;
    EUNIT_ASSERT_LEAVE( iSubSession->CMceCsSubSession::DoServiceL( iIds, EMceItcSetAppUid, *returnMessage ) );
    EUNIT_ASSERT_LEAVE( iSubSession->CMceCsSubSession::DoServiceL( iIds, EMceItcSetAppUid, KNullDesC8 ) );
    EUNIT_ASSERT_LEAVE( iSubSession->CMceCsSubSession::DoServiceL( iIds, EMceItcSetAppUid, *msg ) );
    
    }




void UT_CMceCsSubSession::UT_CMceCsSubSession_SetPendingTransactionLL(  )
    {
    
    CSIPClientTransaction* cltrans2 = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
        
    CSIPClientTransaction* cltrans = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
        
    CSIPTransactionBase* bstrans = ( CSIPTransactionBase* ) cltrans;
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 0 );
    iSubSession->SetPendingTransactionL( cltrans );
    CleanupStack::Pop( cltrans );
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT( iSubSession->PendingTransactions()[0] == cltrans );
    iSubSession->SetPendingTransactionL( cltrans2 );
    CleanupStack::Pop( cltrans2 );
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 2 );
    EUNIT_ASSERT( iSubSession->PendingTransactions()[1] == cltrans2 );
    }
    
void UT_CMceCsSubSession::UT_CMceCsSubSession_ClientReadyToReceiveL(  )
    {
    iSubSession->ServiceL(iIds, EMceItcClientReadyToReceive, RMessage2() );
    TRAPD(err, iSubSession->ClientReadyToReceiveL(RMessage2()));
    EUNIT_ASSERT(err == KErrAlreadyExists);
    }
    
void UT_CMceCsSubSession::UT_CMceCsSubSession_CancelClientReceiveL(  )
    {
    iSubSession->ServiceL(iIds, EMceItcClientReadyToReceive, RMessage2() );
    iSubSession->ServiceL(iIds, EMceItcClientCancelReceive, RMessage2() );
    
    TRAPD(err, iSubSession->CancelClientReceiveL());
    EUNIT_ASSERT(err == KErrNotFound);
    }

void UT_CMceCsSubSession::UT_CMceCsSubSession_InviteCompletedL()
    {
    
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 0 );
    
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
    
    //not found
    iSubSession->InviteCompleted( *clitransaction );
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 0 );
    
	iSubSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 1 );
    
    iSubSession->InviteCompleted( *clitransaction );
    
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 0 );
    
    }


void UT_CMceCsSubSession::UT_CMceCsSubSession_PopRequestL(  )
    {

    CSIPServerTransaction* trx = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInvite );
    CleanupStack::PushL(trx);
    EUNIT_ASSERT( &iSubSession->Request() == NULL );
    iSubSession->iPendingReceivedRequests.AppendL( trx );
    CleanupStack::Pop(trx);
    EUNIT_ASSERT( &iSubSession->Request() == trx );
        
    CSIPServerTransaction* req = iSubSession->PopRequest();
    EUNIT_ASSERT( req == trx );
    EUNIT_ASSERT( &iSubSession->Request() == NULL );
    delete req;
    
    
    }
    
void UT_CMceCsSubSession::UT_CMceCsSubSession_PopResponseL(  )
    {
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EBye );
    
    iSubSession->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT( &iSubSession->Response() == NULL );

    iSubSession->iResponse = clitransaction;
    EUNIT_ASSERT( &iSubSession->Response() == clitransaction );
    
    CSIPClientTransaction* rsp = iSubSession->PopResponse();
    EUNIT_ASSERT( rsp == NULL );
    EUNIT_ASSERT( &iSubSession->Response() == clitransaction );
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 1 );

    clitransaction->iState = CSIPTransactionBase::ECompleted;
    iSubSession->RemovePendingTrx( *clitransaction );
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 0 );
    
    rsp = iSubSession->PopResponse();
    EUNIT_ASSERT( rsp == clitransaction );
    EUNIT_ASSERT( &iSubSession->Response() == NULL );
    EUNIT_ASSERT( iSubSession->PendingTransactions().Count() == 0 );
    delete rsp;

    }
    
    
void UT_CMceCsSubSession::UT_CMceCsSubSession_ClientExistsL()
    {
    EUNIT_ASSERT(!iSubSession->ClientExists() )
    }
    
    
void UT_CMceCsSubSession::UT_CMceCsSubSession_DecodeLL()
    {
    CMceMsgBase* decodedMessage = NULL;
    TAny* object = NULL;
    TMceMessageDispatcher<CMceCsSubSession> dispacther( *iSubSession, iSubSession->iReceiver->ITC() );
    
//EMceItcMsgTypeSession:

    CMceComSession* session = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( session );
    
    CMceMsgObject<CMceComSession>* sessionMsg = new (ELeave) CMceMsgObject<CMceComSession>( *session, EMceItcMsgTypeSession );
    CleanupStack::PushL( sessionMsg );
    sessionMsg->EncodeL();
    
    decodedMessage = dispacther.DecodeL( sessionMsg->Type(), sessionMsg->EncodeBuffer().Ptr( 0 ) );
    CleanupStack::PopAndDestroy( sessionMsg );
    CleanupStack::PopAndDestroy( session );
    
    EUNIT_ASSERT( decodedMessage != NULL );
    decodedMessage->Close();
    delete decodedMessage;
    
    
//EMceItcMsgTypeStream:

    CMceComAudioStream* stream = CMceComAudioStream::NewLC();
    
    CMceMsgObject<CMceComMediaStream>* streamMsg = new (ELeave) CMceMsgObject<CMceComMediaStream>( *stream, EMceItcMsgTypeStream );
    CleanupStack::PushL( streamMsg );
    streamMsg->EncodeL();
    
    decodedMessage = dispacther.DecodeL( streamMsg->Type(), streamMsg->EncodeBuffer().Ptr( 0 ) );
    CleanupStack::PopAndDestroy( streamMsg );
    CleanupStack::PopAndDestroy( stream );

    EUNIT_ASSERT( decodedMessage != NULL );
    decodedMessage->Close();
    delete decodedMessage;

    
//EMceItcMsgTypeCodec:

    TBuf8<KMceMaxSdpNameLength> amr;
    amr.Append( _L("AMR") );
    
    CMceComAMRCodec* codec = CMceComAMRCodec::NewLC( amr );
    CMceMsgObject<CMceComAudioCodec>* codecMsg = new (ELeave) CMceMsgObject<CMceComAudioCodec>( *codec, EMceItcMsgTypeCodec );
    CleanupStack::PushL( codecMsg );
    codecMsg->EncodeL();
    
    decodedMessage = dispacther.DecodeL( codecMsg->Type(), codecMsg->EncodeBuffer().Ptr( 0 ) );
    CleanupStack::PopAndDestroy( codecMsg );
    CleanupStack::PopAndDestroy( codec );

    EUNIT_ASSERT( decodedMessage != NULL );
    decodedMessage->Close();
    delete decodedMessage;


    
//EMceItcMsgTypeSink:

    CMceComRtpSink* sink = CMceComRtpSink::NewLC();
    CMceMsgObject<CMceComMediaSink>* sinkMsg = new (ELeave) CMceMsgObject<CMceComMediaSink>( *sink, EMceItcMsgTypeSink );
    CleanupStack::PushL( sinkMsg );
    sinkMsg->EncodeL();
    
    decodedMessage = dispacther.DecodeL( sinkMsg->Type(), sinkMsg->EncodeBuffer().Ptr( 0 ) );
    CleanupStack::PopAndDestroy( sinkMsg );
    CleanupStack::PopAndDestroy( sink );

    EUNIT_ASSERT( decodedMessage != NULL );
    decodedMessage->Close();
    delete decodedMessage;

//EMceItcMsgTypeSource:

    CMceComRtpSource* source = CMceComRtpSource::NewLC();
    CMceMsgObject<CMceComMediaSource>* sourceMsg = new (ELeave) CMceMsgObject<CMceComMediaSource>( *source, EMceItcMsgTypeSource );
    CleanupStack::PushL( sourceMsg );
    sourceMsg->EncodeL();
    
    decodedMessage = dispacther.DecodeL( sourceMsg->Type(), sourceMsg->EncodeBuffer().Ptr( 0 ) );
    CleanupStack::PopAndDestroy( sourceMsg );
    CleanupStack::PopAndDestroy( source );

    EUNIT_ASSERT( decodedMessage != NULL );
    decodedMessage->Close();
    delete decodedMessage;



//EMceItcMsgTypeSIPReply:

    HBufC8* reason = _L8("reason").AllocLC();
    HBufC8* contentType = _L8("contentType").AllocLC();
    CDesC8Array* sipHeaders = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( sipHeaders );

    CMceMsgSIPReply* replyMsg = new (ELeave) CMceMsgSIPReply( *reason, 405,
                                                              *sipHeaders, *contentType );
                                                              
    CleanupStack::PushL( replyMsg );
    replyMsg->EncodeL();
    
    decodedMessage = dispacther.DecodeL( replyMsg->Type(), replyMsg->EncodeBuffer().Ptr( 0 ) );
    CleanupStack::PopAndDestroy( replyMsg );
    CleanupStack::PopAndDestroy( sipHeaders );
    CleanupStack::PopAndDestroy( contentType );
    CleanupStack::PopAndDestroy( reason );

    EUNIT_ASSERT( decodedMessage != NULL );
    decodedMessage->Close();
    delete decodedMessage;


//EMceItcMsgTypeTextArray:


    sipHeaders = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( sipHeaders );

    CMceMsgTextArray* hdrsMsg = new (ELeave) CMceMsgTextArray( *sipHeaders );
                                                              
    CleanupStack::PushL( hdrsMsg );
    hdrsMsg->EncodeL();
    
    decodedMessage = dispacther.DecodeL( hdrsMsg->Type(), hdrsMsg->EncodeBuffer().Ptr( 0 ) );
    CleanupStack::PopAndDestroy( hdrsMsg );
    CleanupStack::PopAndDestroy( sipHeaders );

    EUNIT_ASSERT( decodedMessage != NULL );
    decodedMessage->Close();
    delete decodedMessage;


//EMceItcMsgTypeEvent:
    
    }
    
void UT_CMceCsSubSession::UT_CMceCsSubSession_ProfileL()
    {
  	CSIPProfile* profile = &iSubSession->Profile();
    EUNIT_ASSERT( profile != NULL );  	
    }    
    

void UT_CMceCsSubSession::UT_CMceCsSubSession_HasInitialInviteTransactionBeenCompletedL()
    {
    CSIPClientTransaction* initialInviteTransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite );
    CleanupStack::Pop( initialInviteTransaction );  
    iSubSession->iInitialInvite = initialInviteTransaction;
    
    // Transaction not completed
    EUNIT_ASSERT( !iSubSession->HasInitialInviteTransactionBeenCompleted() )

    // Transaction completed but exists
    initialInviteTransaction->SetState( CSIPTransactionBase::ECompleted );
    EUNIT_ASSERT( iSubSession->HasInitialInviteTransactionBeenCompleted() )
    
    // Transaction terminated but exists
    initialInviteTransaction->SetState( CSIPTransactionBase::ETerminated );
    EUNIT_ASSERT( iSubSession->HasInitialInviteTransactionBeenCompleted() )
    
    // Transaction does not exist
    delete iSubSession->iInitialInvite;
    iSubSession->iInitialInvite = NULL;
    EUNIT_ASSERT( iSubSession->HasInitialInviteTransactionBeenCompleted() )
    }

void UT_CMceCsSubSession::UT_CMceCsSubSession_GetRequestL()
    {
        
    const RPointerArray<CSIPClientTransaction>& pendingReceivedRequest = 
        iSubSession->PendingTransactions();
        
    EUNIT_ASSERT( iSubSession->PendingReceivedRequests().Count() == 0 );
    
    CSIPServerTransaction* trx =         
    MCETestHelper::ServerTransactionL( SipStrConsts::EInfo );
    CleanupStack::PushL(trx);
    iSubSession->iPendingReceivedRequests.AppendL( trx );
    CleanupStack::Pop(trx);
    EUNIT_ASSERT( iSubSession->PendingReceivedRequests().Count() == 1 );
    
    EUNIT_ASSERT( iSubSession->PendingReceivedRequests()[0] == trx );
    
    }
    
    
void UT_CMceCsSubSession::UT_CMceCsSubSession_DialogRequestReceivedL  ()
	{
    CSIPServerTransaction* srvtransaction = 
        MCETestHelper::ServerTransactionL( SipStrConsts::EInfo );
    CleanupStack::PushL( srvtransaction );

	TBool consumed = iSubSession->DialogRequestReceived( srvtransaction, iSubSession->Dialog()->Dialog() );
	MCE_CHECK_MEMORY_LEAVE( !consumed )
  	EUNIT_ASSERT( iSubSession->PendingReceivedRequests()[0] == srvtransaction );
  	CleanupStack::Pop( srvtransaction );
	}



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceCsSubSession,
    "UT_CMceCsSubSession",
    "UNIT" )

EUNIT_TEST(
    "GetFunctions - test ",
    "CMceCsSubSession",
    "GetFunctions",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_GetFunctionsL, Teardown)

EUNIT_TEST(
    "ServiceL - test ",
    "CMceCsSubSession",
    "ServiceL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_ServiceLL, Teardown)


EUNIT_TEST(
    "SetPendingTransactionL - test ",
    "CMceCsSubSession",
    "SetPendingTransactionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_SetPendingTransactionLL, Teardown)

EUNIT_TEST(
    "ClientReadyToReceive - test ",
    "CMceCsSubSession",
    "ClientReadyToReceive",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_ClientReadyToReceiveL, Teardown)
    
EUNIT_TEST(
    "CancelClientReceive - test ",
    "CMceCsSubSession",
    "CancelClientReceive",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_CancelClientReceiveL, Teardown)
    
EUNIT_TEST(
    "InviteCompleted - test ",
    "CMceCsSubSession",
    "InviteCompleted",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_InviteCompletedL, Teardown)

EUNIT_TEST(
    "PopRequest - test ",
    "CMceCsSubSession",
    "PopRequest",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_PopRequestL, Teardown)

EUNIT_TEST(
    "PopResponse - test ",
    "CMceCsSubSession",
    "PopResponse",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_PopResponseL, Teardown)

EUNIT_TEST(
    "ClientExists - test ",
    "CMceCsSubSession",
    "ClientExists",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_ClientExistsL, Teardown)


EUNIT_TEST(
    "DecodeL - test ",
    "CMceCsSubSession",
    "DecodeL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_DecodeLL, Teardown)

EUNIT_TEST(
    "Profile - test ",
    "CMceCsSubSession",
    "Profile",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_ProfileL, Teardown)

EUNIT_TEST(
    "HasInitialInviteTransactionBeenCompleted - test ",
    "CMceCsSubSession",
    "HasInitialInviteTransactionBeenCompleted",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_HasInitialInviteTransactionBeenCompletedL, Teardown)

EUNIT_TEST(
    "Pending Request - test ",
    "CMceCsSubSession",
    "GetRequest",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_GetRequestL, Teardown)

EUNIT_TEST(
    "DialogRequestReceived - test ",
    "CMceCsSubSession",
    "DialogRequestReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSubSession_DialogRequestReceivedL, Teardown)
    


    
    

EUNIT_END_TEST_TABLE

//  END OF FILE
