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
#include "UT_MCEUsesCases.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MCETestHelper.h"
#include "mcesipsession.h"
#include "mcesipstatemachine.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "cmcetls.h"
#include "mceactionset.h"
#include "mcesession.h"


const TUint32 KTestProfileId(1);
const TUint KCallIsBeingForwareded = 5000;
const TUint KSipCallQueued         = 5001;

#define MCE_ASSERT_SIPSESSION( session, type, state, subState, exitState, request, response, pendings )\
    EUNIT_ASSERT( session->PendingTransactions().Count() == pendings );\
    EUNIT_ASSERT( session->Type() == type );\
    EUNIT_ASSERT( session->ClientType() == KMceCSSIPSession );\
    EUNIT_ASSERT( &session->Client() != NULL );\
    EUNIT_ASSERT( session->Dialog() != NULL );\
    EUNIT_ASSERT( session->DialogId() > 0 );\
    if ( request == KErrNotFound )\
        {\
        EUNIT_ASSERT( &session->Request() == NULL );\
        }\
    else\
        {\
        EUNIT_ASSERT( &session->Request() != NULL );\
        EUNIT_ASSERT( MceSip::Method( session->Request() ) == SIPStrings::StringF( request ) );\
        }\
    if ( response == KErrNotFound )\
        {\
        EUNIT_ASSERT( &session->Response() == NULL );\
        }\
    else\
        {\
        EUNIT_ASSERT( &session->Response() != NULL );\
        EUNIT_ASSERT( MceSip::ResponseCode( session->Response() ) == response );\
        }\
    EUNIT_ASSERT( session->CurrentState().Id() == state );\
    EUNIT_ASSERT( session->SubState() == subState );\
    if ( exitState == KErrNotFound )\
        {\
        EUNIT_ASSERT( session->iExitState == NULL );\
        }\
    else\
        {\
        EUNIT_ASSERT( session->iExitState->Id() == exitState );\
        }
    

#define MCE_ASSERT_SIPSESSION2( session, type, state, subState, exitState, request, response, pendings )\
    EUNIT_ASSERT( session->PendingTransactions().Count() == pendings );\
    EUNIT_ASSERT( session->Type() == type );\
    EUNIT_ASSERT( session->ClientType() == KMceCSSIPSession );\
    EUNIT_ASSERT( &session->Client() != NULL );\
    EUNIT_ASSERT( session->Dialog() != NULL );\
    EUNIT_ASSERT( session->DialogId() > 0 );\
    if ( request != KErrNotFound )\
        {\
        EUNIT_ASSERT( &session->Request() == NULL );\
        }\
    else\
        {\
        EUNIT_ASSERT( &session->Request() != NULL );\
        EUNIT_ASSERT( MceSip::Method( session->Request() ) == SIPStrings::StringF( request ) );\
        }\
    if ( response == KErrNotFound )\
        {\
        EUNIT_ASSERT( &session->Response() == NULL );\
        }\
    else\
        {\
        EUNIT_ASSERT( &session->Response() != NULL );\
        EUNIT_ASSERT( MceSip::ResponseCode( session->Response() ) == response );\
        }\
    EUNIT_ASSERT( session->CurrentState().Id() == state );\
    EUNIT_ASSERT( session->SubState() == subState );\
    if ( exitState == KErrNotFound )\
        {\
        EUNIT_ASSERT( session->iExitState == NULL );\
        }\
    else\
        {\
        EUNIT_ASSERT( session->iExitState->Id() == exitState );\
        }
    
#define MCE_ASSERT_BODY( session, body, clientState )\
        EUNIT_ASSERT( session->Body()->iState == clientState );\
        EUNIT_ASSERT( session->Body()->Id() == reinterpret_cast<TUint32>(session) );\
        EUNIT_ASSERT( session->Body()->Id() == body->Id() )

#define MCE_ASSERT_BODY_1( session, body, clientState )\
        EUNIT_ASSERT( body->iState == clientState );\
        EUNIT_ASSERT( body->Id() == reinterpret_cast<TUint32>(session) )
    
    
#define MCE_ASSERT_CLIENT( session, clientState, status )\
    EUNIT_ASSERT( iStorage->iIds.iManagerType == session->ClientType() );\
    EUNIT_ASSERT( iStorage->iIds.iSessionID == session->Body()->Id() );\
    EUNIT_ASSERT( iStorage->iIds.iDialogID == session->DialogId() );\
    EUNIT_ASSERT( iStorage->iIds.iDialogType == KMceDlgTypeInvite );\
    EUNIT_ASSERT( iStorage->iIds.iProfileID == KTestProfileId );\
    EUNIT_ASSERT( iStorage->iIds.iState == clientState );\
    EUNIT_ASSERT( iStorage->iIds.iStatus == status )


#define MCE_ASSERT_CLIENT_1( callback, msgType )\
    EUNIT_ASSERT( iStorage->iIds.iCallbackID == callback );\
    EUNIT_ASSERT( iStorage->iIds.iMsgType == msgType )



#define MCE_ASSERT_MANAGER( callback, msgType )\
    EUNIT_ASSERT( iStorage->iManagerIds.iManagerType == KMceCSSIPSession );\
    EUNIT_ASSERT( iStorage->iManagerIds.iSessionID == KMceNotAssigned );\
    EUNIT_ASSERT( iStorage->iManagerIds.iDialogID != KMceNotAssigned );\
    EUNIT_ASSERT( iStorage->iManagerIds.iDialogType == KMceDlgTypeInvite );\
    EUNIT_ASSERT( iStorage->iManagerIds.iProfileID == KTestProfileId );\
    EUNIT_ASSERT( iStorage->iManagerIds.iStatus == KErrNone );\
    EUNIT_ASSERT( iStorage->iManagerIds.iCallbackID == callback );\
    EUNIT_ASSERT( iStorage->iManagerIds.iMsgType == msgType )


#define MCE_ASSERT_REPLY( clientReply, reasonCode, reason, contentType )\
    if ( clientReply )\
        {\
        EUNIT_ASSERT( clientReply->iCode == reasonCode );\
        HBufC8* _reason = clientReply->Reason();\
        CleanupStack::PushL( _reason );\
        HBufC8* _contentType = clientReply->ContentType();\
        CleanupStack::PushL( _contentType );\
        CDesC8Array* _headers = clientReply->Headers();\
        CleanupStack::PushL( _headers );\
        EUNIT_ASSERT( _headers != NULL );\
        EUNIT_ASSERT( _reason->Compare( reason ) == 0 );\
        EUNIT_ASSERT( _contentType->Compare( contentType ) == 0 );\
        CleanupStack::PopAndDestroy( _headers );\
        CleanupStack::PopAndDestroy( _contentType );\
        CleanupStack::PopAndDestroy( _reason );\
        }

#define MCE_ASSERT_DATA( clientData )\
    if ( clientData )\
        {\
        CDesC8Array* _headers = clientData->Headers();\
        CleanupStack::PushL( _headers );\
        EUNIT_ASSERT( _headers != NULL );\
        CleanupStack::PopAndDestroy( _headers );\
        }

#define SDP_CONTENT ETrue
#define INVITE_TRX 0
#define CANCEL_TRX 1
#define BYE_TRX 1
#define PRACK_TRX 1
#define UPDATE_TRX 1

#define SDP_CONTENT_2 2

#define BAD_SDP ETrue

_LIT8( KTestContentType, "application/foobaar" );
_LIT8( KTestContent, "some data");

// CONSTRUCTION
UT_MCEUsesCases* UT_MCEUsesCases::NewL()
    {
    UT_MCEUsesCases* self = UT_MCEUsesCases::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_MCEUsesCases* UT_MCEUsesCases::NewLC()
    {
    UT_MCEUsesCases* self = new( ELeave ) UT_MCEUsesCases();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_MCEUsesCases::~UT_MCEUsesCases()
    {
    }

// Default constructor
UT_MCEUsesCases::UT_MCEUsesCases()
    {
    }

// Second phase construct
void UT_MCEUsesCases::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_MCEUsesCases::SetupL()
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = CMCETls::Storage();
    
    iIds = TMceIds();
    iIds.iAppUID = 10;
    iIds.iManagerType = KMceCSSIPSession;
    iStorage->Store( iIds );
    
    iIds.iProfileID = KTestProfileId;
    iIds.iDialogType = KMceDlgTypeInvite;
    
    iServerCore = CMceServerCore::NewL();
    
    iSession = CMceCsSession::NewL( *iServerCore );
    
    MCETestHelper::ManagerReadyToReceiveL( iSession );
    iSession->SetClientUidL( iIds, RMessage2() );
    
    iStorage->iIds = iIds;
    }

void UT_MCEUsesCases::Teardown()
    {
    CMCETls::Close();
    
	delete iSession;
	iSession = NULL;
    delete iServerCore;
    iServerCore = NULL;

    SdpCodecStringPool::Close();    
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    
    CMceSipSession* outSession = NULL;
        
    for( TInt i = 0; i < 3; i++ )
        {
        MCE_RESET_STUBS();

        outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );

        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSession /*type*/, KMceStateIdle /*state*/, 
                                           CMceSipSession::ENone /*subState*/, KErrNotFound /*exitState*/,
                                           KErrNotFound /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_RESET_STUBS();                               

//client ready to receive
        MCETestHelper::ClientReadyToReceiveL( outSession );
        iStorage->iMediaManagerUpdateStatus = KMceAsync;

        if ( i == 0 )
            {
//establish session with no params
            clientSession = MCETestHelper::ClientEstablishL( outSession );
            }
        else if ( i == 1 )
            {
//establish session with all params
            clientSession = MCETestHelper::ClientEstablishL( outSession, KRequire, KTestContentType, KTestContent );
            }
        else if ( i == 2 )
            {
//establish session with sip headers
            clientSession = MCETestHelper::ClientEstablishL( outSession, KRequire );
            }


        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSession /*type*/, KMceStateClientEstablishing /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateIdle /*exitState*/,
                                           KErrNotFound /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( outSession, clientSession, CMceSession::EOffering /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
        MCE_RESET_STUBS();                               

        delete clientSession;

        
//offer created -> INVITE
        MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );


        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                           KErrNotFound /*request*/, KErrNotFound /*response*/, 1 /*pendings*/);
        MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                          CMCETls::EEncode /*mmsdpaction*/, 
                          SipStrConsts::EInvite /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
        MCE_RESET_STUBS();


//100
        EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
        MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

        MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/,   
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
        MCE_RESET_STUBS();


//180
        if ( i > 0 )
            {
            EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
            MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );

            MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                               CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                               KErrNotFound /*request*/, KMceSipRinging /*response*/, 1 /*pendings*/);

            MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
            MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                              CMCETls::ENone /*mmsdpaction*/,
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              KErrNotFound /*sentResponse*/);
            MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
            MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
            MCE_RESET_STUBS();


//client receive 180 response
            clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
         	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
            clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
            MCE_ASSERT_REPLY( clientReply, KMceSipRinging, _L8("Ringing"), _L8("")  );
            CleanupStack::PopAndDestroy( clientMessage );
            }
//200OK    

        if ( i > 0 )
            {
            EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
            iStorage->iMediaManagerUpdateStatus = KMceAsync;
            MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );

            MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                               CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                               KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

            MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
            MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                              CMCETls::EDecode /*mmsdpaction*/, 
                              SipStrConsts::EAck /*sentMethod*/, 
                              KErrNotFound /*sentResponse*/);
            MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
            MCE_RESET_STUBS();
            
            
//answer decoded
            MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

            MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                               CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                               KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

            MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
            MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/,
                              CMCETls::ENone /*mmsdpaction*/,    
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              KErrNotFound /*sentResponse*/);
            MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
            MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeMessageArray /*msgType*/ );
            MCE_RESET_STUBS();
            }
        else
            {
            EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
            iStorage->iMediaManagerUpdateStatus = KMceReady;
            MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );

            MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                               CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                               KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

            MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
            MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                              CMCETls::EDecode /*mmsdpaction*/,
                              SipStrConsts::EAck /*sentMethod*/, KErrNotFound /*sentResponse*/);
            MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
            MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/, EMceItcMsgTypeMessageArray /*msgType*/);
            MCE_RESET_STUBS();
            }
        

//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    	
    	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
        clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
        clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

        MCE_ASSERT_BODY( outSession, clientMsgSession->iObject, CMceSession::EEstablished /*clientState*/ );
        MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );

        CleanupStack::PopAndDestroy( clientMessage );

//invite completed
        outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );

        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                           CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);
        
//terminate
        if ( i == 0 )
            {
            MCETestHelper::ClientTerminateL( outSession );
            }
        else if ( i == 1 )
            {
            MCETestHelper::ClientTerminateL( outSession, KRequire, KTestContentType, KTestContent );
            }
        else if ( i == 2 )
            {
            MCETestHelper::ClientTerminateL( outSession, KRequire );
            }

        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminating /*state*/, 
                                           CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

        MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminating /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EBye /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminating /*clientState*/, KErrNone /*status*/ );
        MCE_RESET_STUBS();
        
//200OK 
        EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
        MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );

        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                           CMceSipSession::ENone /*subState*/, KMceStateTerminating /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
        MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/, EMceItcMsgTypeSIPReply /*msgType*/ );
        
        MCE_RESET_STUBS();
        
//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
        clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );

        MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );

        CleanupStack::PopAndDestroy( clientMessage );
        }
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;

    for ( TInt i = 0; i < 4; i++ )
        {
        MCE_RESET_STUBS();
        
//incoming session    
        CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession );

        EUNIT_ASSERT( inSession != NULL );
        MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateServerEstablishing /*state*/,
                                          CMceSipSession::EAnswering /*subState*/, KMceStateIdle /*exitState*/,
                                          SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EIncoming /*clientState*/ );

        MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                          CMCETls::EDecode /*mmsdpaction*/,
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
                          
        MCE_ASSERT_MANAGER( EMceItcObjectAdded /*callback*/, EMceItcMsgTypeSession /*msgType*/ );
        EUNIT_ASSERT( inSession->Body()->Streams().Count() > 0 );
        MCE_RESET_STUBS();


//client session ready to receive
        MCETestHelper::ClientReadyToReceiveL( inSession );


//update
        iStorage->iMediaManagerUpdateStatus = KMceAsync;
        CMceComSession* update = inSession->Body()->CloneL();
        CleanupStack::PushL( update );
        update->Streams()[0]->iLocalMediaPort = 5006;
        MCETestHelper::ClientUpdateSessionL( inSession, *update );

        MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdating /*state*/, 
                                          CMceSipSession::EAnswering /*subState*/, KMceStateServerEstablishing /*exitState*/,
                                          SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( inSession, update, CMceSession::EReserving /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/,
                          CMCETls::ENone /*mmsdpaction*/,  
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( inSession, CMceSession::EReserving /*clientState*/, KErrNone /*status*/ );
        MCE_RESET_STUBS();
        
        
        CleanupStack::PopAndDestroy( update );
        

//answer created
        MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

        MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                          CMceSipSession::EAnswering /*subState*/, KMceStateUpdating /*exitState*/,
                                          SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
        MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSession /*msgType*/ );
        MCE_RESET_STUBS();

//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
        clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMessage );
        clientSession = clientMsgSession->iObject;
        MCE_ASSERT_BODY( inSession, clientSession, CMceSession::EProceeding /*clientState*/ );
        CleanupStack::PopAndDestroy( clientMessage );

//accept session
        if ( i == 0 )
            {
            //NO ringing
            MCETestHelper::ClientAcceptL( inSession );
            }
        else if ( i == 1 )
            {
            MCETestHelper::ClientRingL( inSession );

            MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                              CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                              SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

            MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
            MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                              CMCETls::ENone /*mmsdpaction*/, 
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              KMceSipRinging /*sentResponse*/);
            MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
            MCE_RESET_STUBS();
            
            MCETestHelper::ClientAcceptL( inSession, KNullDesC8, 0 );//standard
            }
        else if ( i == 2 )
            {
            MCETestHelper::ClientRingL( inSession, KRequire );

            MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                              CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                              SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

            MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
            MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/,
                              CMCETls::ENone /*mmsdpaction*/,  
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              KMceSipRinging /*sentResponse*/);
            MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
            MCE_RESET_STUBS();
            
            MCETestHelper::ClientAcceptL( inSession, KNullDesC8, 0 );
            }
        else if ( i == 3 )
            {
            MCETestHelper::ClientRingL( inSession, KRequire, KTestContent, KTestContentType );

            MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                              CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                              SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

            MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
            MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                              CMCETls::ENone /*mmsdpaction*/, 
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              KMceSipRinging /*sentResponse*/);
            MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
            MCE_RESET_STUBS();
            
            MCETestHelper::ClientAcceptL( inSession, _L8("202 OK"), 202 );
            }
        

        MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateAnswering /*state*/, 
                                          CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                          SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EAnswering /*clientState*/ );
        
        if ( i != 3 )
            {
            MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                              CMCETls::EEncode /*mmsdpaction*/, 
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              KMceSipOK /*sentResponse*/);
            }
        else
            {
            MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                              CMCETls::EEncode /*mmsdpaction*/, 
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              202 /*sentResponse*/);
            }
            
        MCE_ASSERT_CLIENT( inSession, CMceSession::EAnswering /*clientState*/, KErrNone /*status*/ );
        MCE_RESET_STUBS();
        

//ACK received
        MCETestHelper::RequestL( inSession, SipStrConsts::EAck );

        MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                          CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                          SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

	    /*EUNIT_ASSERT( inSession->PendingTransactions().Count() == 0 );
	    EUNIT_ASSERT( inSession->Type() == CMceCsSubSession::EInSession );
	    EUNIT_ASSERT( inSession->ClientType() == KMceCSSIPSession );
	    EUNIT_ASSERT( &inSession->Client() != NULL );
	    EUNIT_ASSERT( inSession->Dialog() != NULL );
	    EUNIT_ASSERT( inSession->DialogId() > 0 );
	    if ( SipStrConsts::EAck == KErrNotFound )
	        {
	        EUNIT_ASSERT( &inSession->Request() == NULL );
	        }
	    else
	        {
	        EUNIT_ASSERT( &inSession->Request() != NULL );
	        EUNIT_ASSERT( MceSip::Method( inSession->Request() ) == SIPStrings::StringF( SipStrConsts::EAck ) );
	        }
	    if ( KErrNotFound == KErrNotFound )
	        {
	        EUNIT_ASSERT( &inSession->Response() == NULL );
	        }
	    else
	        {
	        EUNIT_ASSERT( &inSession->Response() != NULL );
	        EUNIT_ASSERT( MceSip::ResponseCode( inSession->Response() ) == KErrNotFound );
	        }
	    EUNIT_ASSERT( inSession->CurrentState().Id() == KMceStateEstablished );
	    EUNIT_ASSERT( inSession->SubState() == CMceSipSession::ENone );
	    if ( KMceStateAnswering == KErrNotFound )
	        {
	        EUNIT_ASSERT( inSession->iExitState == NULL );
	        }
	    else
	        {
	        EUNIT_ASSERT( inSession->iExitState->Id() == KMceStateAnswering );
	        }*/

        MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNone /*sentResponse*/);
        MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
        MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
        MCE_RESET_STUBS();

//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
        clientData = static_cast<CMceMsgSIPData*>( clientMessage );
        MCE_ASSERT_DATA( clientData );
        
        CleanupStack::PopAndDestroy( clientMessage );

//BYE received
        MCETestHelper::RequestL( inSession, SipStrConsts::EBye );

        MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateTerminated /*state*/, 
                                          CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                          SipStrConsts::EBye /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::ETerminated /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KMceSipOK /*sentResponse*/);
        MCE_ASSERT_CLIENT( inSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
        MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
        MCE_RESET_STUBS();

//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
        clientData = static_cast<CMceMsgSIPData*>( clientMessage );
        MCE_ASSERT_DATA( clientData );
        CleanupStack::PopAndDestroy( clientMessage );           
        }
    }
    
// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallAndUpdateL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallAndUpdateL()
    {    
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
    
    for( TInt i = 0; i < 4; i++ )
        {
//create session            
        outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
    
//client ready to receive
        MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
        iStorage->iMediaManagerUpdateStatus = KMceAsync;
        clientSession = MCETestHelper::ClientEstablishL( outSession );
        delete clientSession;

//offer created
        MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

//100
        EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
        MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

//180
        EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
        MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );

//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
        clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
        CleanupStack::PopAndDestroy( clientMessage );

//200OK    
        EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
        iStorage->iMediaManagerUpdateStatus = KMceAsync;
        MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
        

//answer decoded
        MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    	
    	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
        clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
        clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

        CleanupStack::PopAndDestroy( clientMessage );


//invite completed
        outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
        MCE_RESET_STUBS();
        
//update
        iStorage->iMediaManagerUpdateStatus = KMceAsync;
        CMceComSession* update = outSession->Body()->CloneL();
        CleanupStack::PushL( update );
        CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
        audio->Codecs()[0]->iPTime = 300;
        
        if ( i == 0 )
            {
            MCETestHelper::ClientUpdateSessionL( outSession, *update );
            }
        else if ( i == 1 )
            {
            MCETestHelper::ClientUpdateSessionL( outSession, *update, 180, KRequire, KNullDesC8, KNullDesC8 ,KNullDesC8 );
            }
        else if ( i == 2 )
            {
            MCETestHelper::ClientUpdateSessionL( outSession, *update, 180, KRequire, 
                                                 KTestContentType, KTestContent, KNullDesC8 );
            }
        else if ( i == 3 )
            {
            MCETestHelper::ClientUpdateSessionL( outSession, *update, 180, KRequire, 
                                                 KTestContentType, KTestContent, _L8("contentHeader") );
            } 
            
        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateClientEstablishing /*state*/, 
                                           CMceSipSession::EUpdating /*subState*/, KMceStateEstablished /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
        MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
        
        MCE_RESET_STUBS();

        CleanupStack::PopAndDestroy( update );



//update created -> INVITE
        MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );


        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EUpdating /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

        MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
        MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                          CMCETls::EEncode /*mmsdpaction*/, 
                          SipStrConsts::EInvite /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
        
        MCE_RESET_STUBS();


//200OK    
        iStorage->iMediaManagerUpdateStatus = KMceAsync;
        EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
        
        MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );

        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EUpdating /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                          CMCETls::EDecode /*mmsdpaction*/, 
                          SipStrConsts::EAck /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
        
        MCE_RESET_STUBS();


//answer to update decoded
        MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

        MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                           CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EEstablished /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
        MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeMessageArray /*msgType*/ );
        
        MCE_RESET_STUBS();


//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    	
    	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
        clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
        clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

        MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );
        MCE_ASSERT_BODY( outSession, clientMsgSession->iObject, CMceSession::EEstablished /*clientState*/ );

        CleanupStack::PopAndDestroy( clientMessage );
        
//terminate
        MCETestHelper::ClientTerminateL( outSession );
        
        MCE_RESET_STUBS();
//200OK 
        EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
        MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );
        
//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
        clientData = static_cast<CMceMsgSIPData*>( clientMessage );
        
        CleanupStack::PopAndDestroy( clientMessage );        
        }
    }
    
// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledAndUpdatedL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledAndUpdatedL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
    CMceComSession* clientSession = NULL;

//incoming session    
    CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession );
    EUNIT_ASSERT( inSession != NULL );

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );


//update
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );
    CleanupStack::PopAndDestroy( update );

//answer created
    MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );

//accept session
    MCETestHelper::ClientAcceptL( inSession );

//ACK received
    MCETestHelper::RequestL( inSession, SipStrConsts::EAck );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    CleanupStack::PopAndDestroy( clientMessage );
    

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                      SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);


    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

    
//invite
    iStorage->iMediaManagerUpdateStatus = KMceReady;

    MCETestHelper::RequestL( inSession, SipStrConsts::EInvite, SDP_CONTENT_2 );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateServerEstablishing /*state*/, 
                                      CMceSipSession::EUpdating /*subState*/, KMceStateEstablished /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY_1( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_BODY_1( inSession, inSession->BodyCandidate(), CMceSession::EIncoming /*clientState*/ );
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EIncoming /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcIncomingUpdate /*callback*/ , EMceItcMsgTypeSession /*msgType*/ );
    
    
    MCE_RESET_STUBS();
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMessage );
    clientSession = clientMsgSession->iObject;
    
    MCE_ASSERT_BODY_1( inSession, clientSession, CMceSession::EIncoming );
    

//update
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( clientSession->Streams()[0] );
    audio->Codecs()[0]->iPTime = 60;
    clientSession->iSIPHeaders->AppendL( KRequire );
    MCETestHelper::ClientUpdateSessionL( inSession, *clientSession );
    
    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdating /*state*/, 
                                      CMceSipSession::EUpdating /*subState*/, KMceStateServerEstablishing /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY_1( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_BODY_1( inSession, inSession->BodyCandidate(), CMceSession::EReserving /*clientState*/ );
    
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EReserving /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
    
    CleanupStack::PopAndDestroy( clientMessage );

//answer to update created -> 200 OK
    MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateAnswering /*state*/, 
                                      CMceSipSession::EUpdating /*subState*/, KMceStateUpdating /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY_1( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
    EUNIT_ASSERT( inSession->BodyCandidate() == NULL );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , KMceNotAssigned /*msgType*/ );
    
    MCE_RESET_STUBS();
    
//client receive
    MCETestHelper::ClientReceiveLC( inSession, iIds );

//ACK received
    MCETestHelper::RequestL( inSession, SipStrConsts::EAck );

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                      SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY_1( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    EUNIT_ASSERT( inSession->BodyCandidate() == NULL );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );

    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );

//terminate
    MCETestHelper::ClientTerminateL( inSession );
    
    MCE_RESET_STUBS();
    
//200OK 
    EUNIT_ASSERT( inSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( inSession, KMceSipOK, SipStrConsts::EPhraseOk );
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    CleanupStack::PopAndDestroy( clientMessage );        
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallDeclinedL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallDeclinedL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

    MCE_RESET_STUBS();

//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRinging /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
    MCE_RESET_STUBS();

//client receive 180 response
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
 	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    MCE_ASSERT_REPLY( clientReply, KMceSipRinging, _L8("Ringing"), _L8("")  );
    CleanupStack::PopAndDestroy( clientMessage );

//603
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipDecline, SipStrConsts::EPhraseDecline );
    MCE_CHECK_MEMORY_LEAVE( iStorage->iIds.iState != CMceSession::ETerminated )
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipDecline /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    
    MCE_CHECK_MEMORY_LEAVE( iStorage->iIds.iMsgType != EMceItcMsgTypeSIPReply )
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );
    MCE_RESET_STUBS();


//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipDecline, _L8("Decline"), _L8("") );
    
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledDeclineL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledDeclineL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
    CMceComSession* clientSession = NULL;

    for( TInt i = 0; i < 5; i++ )
        {        
//incoming session
        CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession );
        EUNIT_ASSERT( inSession != NULL );

//client ready to receive
        MCETestHelper::ClientReadyToReceiveL( inSession );


//update
        iStorage->iMediaManagerUpdateStatus = KMceAsync;
        CMceComSession* update = inSession->Body()->CloneL();
        CleanupStack::PushL( update );
        update->Streams()[0]->iLocalMediaPort = 5006;
        MCETestHelper::ClientUpdateSessionL( inSession, *update );
        CleanupStack::PopAndDestroy( update );

        MCE_RESET_STUBS();

//answer created
        MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

        MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                          CMceSipSession::EAnswering /*subState*/, KMceStateUpdating /*exitState*/,
                                          SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
        MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                          CMCETls::ENone /*mmsdpaction*/, 
                          SipStrConsts::EEmpty /*sentMethod*/, 
                          KErrNotFound /*sentResponse*/);
        MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
        MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSession /*msgType*/ );
        MCE_RESET_STUBS();

//client receive
        clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
        clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMessage );
        clientSession = clientMsgSession->iObject;
        MCE_ASSERT_BODY( inSession, clientSession, CMceSession::EProceeding /*clientState*/ );
        CleanupStack::PopAndDestroy( clientMessage );

//decline
        if ( i == 0 )
            {
            //no params
            MCETestHelper::ClientDeclineL( inSession );
            }
        else if ( i == 1 )
            {
            //all
            MCETestHelper::ClientDeclineL( inSession, 
                                           KRequire, 
                                           KTestContent, 
                                           KTestContentType,
                                           KNullDesC8,
                                           0 );
            }
        else if ( i == 2 )
            {
            //no content, just headers
            MCETestHelper::ClientDeclineL( inSession, 
                                           KRequire, 
                                           KNullDesC8, 
                                           KNullDesC8,
                                           KNullDesC8,
                                           0 );
            }
        else if ( i == 3 )
            {
            //no headers
            MCETestHelper::ClientDeclineL( inSession, 
                                           KNullDesC8, 
                                           KTestContent, 
                                           KTestContentType,
                                           KNullDesC8,
                                           0 );
            }
        else if ( i == 4 )
            {
            //own reason, headers, no content
            MCETestHelper::ClientDeclineL( inSession, 
                                           KRequire, 
                                           KNullDesC8, 
                                           KNullDesC8,
                                           _L8("Declined"),
                                           610 );
            }
             
        MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateTerminated /*state*/, 
                                          CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                          SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

        MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::ETerminated /*clientState*/ );
        if ( i != 4 )
            {
                
            MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                              CMCETls::ENone /*mmsdpaction*/, 
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              KMceSipDecline /*sentResponse*/);
            }
        else
            {
            MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                              CMCETls::ENone /*mmsdpaction*/, 
                              SipStrConsts::EEmpty /*sentMethod*/, 
                              610 /*sentResponse*/);
            }
            
        MCE_ASSERT_CLIENT( inSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
        MCE_RESET_STUBS();
        }
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancelL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancelL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
	
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    

//CANCEL
    MCETestHelper::ClientCancelL( outSession );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );
    
    MCE_RESET_STUBS();
    
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );
    CleanupStack::PopAndDestroy( clientMessage );

    
//487
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRequestTerminated, SipStrConsts::EPhraseRequestTerminated, INVITE_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateTerminated /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRequestTerminated /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );    
    MCE_RESET_STUBS();
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancel2L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancel2L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
	
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//CANCEL
    MCETestHelper::ClientCancelL( outSession );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//487
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRequestTerminated, SipStrConsts::EPhraseRequestTerminated, INVITE_TRX );
    MCE_CHECK_MEMORY_LEAVE( iStorage->iIds.iState != CMceSession::ETerminated || 
                            iStorage->iMediaManagerAction != CMCETls::ENone )
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRequestTerminated /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );
    
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipRequestTerminated, _L8("Request Terminated"), _L8("") );
    CleanupStack::PopAndDestroy( clientMessage );

//200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateTerminated /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancel3L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancel3L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
	
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;


//offer created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//CANCEL
    MCETestHelper::ClientCancelL( outSession );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//200(i)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, ETrue );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminating /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 3 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminating /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    EUNIT_ASSERT( iStorage->iAckSent );                      
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminating /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );
    MCE_RESET_STUBS();
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );
    CleanupStack::PopAndDestroy( clientMessage );
    
//200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminating /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateTerminating /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminating /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminating /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//200(bye)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, BYE_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateTerminating /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );
    
    MCE_RESET_STUBS();
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancel4L
// -----------------------------------------------------------------------------
//    
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancel4L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
	
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

    MCE_RESET_STUBS();

//offer created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 1 /*pendings*/);
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();


//CANCEL
    MCETestHelper::ClientCancelL( outSession );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();


//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
	MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 )
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();


//200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );    
    CleanupStack::PopAndDestroy( clientMessage );

    
//487
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRequestTerminated, SipStrConsts::EPhraseRequestTerminated, INVITE_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateTerminated /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRequestTerminated /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    }
    
// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancel5L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancel5L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
	

    MCE_RESET_STUBS();                               
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSession /*type*/, KMceStateClientEstablishing /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateIdle /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, clientSession, CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
        
    delete clientSession;


//CANCEL
    MCETestHelper::ClientCancelL( outSession );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateCancelL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateCancelL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
        
//update
    CMceComSession* update = outSession->Body()->CloneL();
    CleanupStack::PushL( update );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
    audio->Codecs()[0]->iPTime = 300;
    
    MCETestHelper::ClientUpdateSessionL( outSession, *update );
    CleanupStack::PopAndDestroy( update );

    MCE_RESET_STUBS();

//update created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EUpdating /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

    MCE_RESET_STUBS();

//CANCEL
    MCETestHelper::ClientCancelL( outSession );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                       CMceSipSession::EUpdating /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
    
//200OK (i)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, ETrue );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
    MCE_RESET_STUBS();
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    	
	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );
    MCE_ASSERT_BODY( outSession, clientMsgSession->iObject, CMceSession::EEstablished /*clientState*/ );

    CleanupStack::PopAndDestroy( clientMessage );
        
//200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    MCE_RESET_STUBS();
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateCancel2L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateCancel2L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
        
//update
    CMceComSession* update = outSession->Body()->CloneL();
    CleanupStack::PushL( update );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
    audio->Codecs()[0]->iPTime = 300;
    
    MCETestHelper::ClientUpdateSessionL( outSession, *update );
    CleanupStack::PopAndDestroy( update );

    MCE_RESET_STUBS();

//update created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EUpdating /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

    MCE_RESET_STUBS();

//CANCEL
    MCETestHelper::ClientCancelL( outSession );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                       CMceSipSession::EUpdating /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
    
//200OK (c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX, ETrue );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
    MCE_RESET_STUBS();
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    	
	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );
    MCE_ASSERT_BODY( outSession, clientMsgSession->iObject, CMceSession::EEstablished /*clientState*/ );

    CleanupStack::PopAndDestroy( clientMessage );
        
//487(i)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRequestTerminated, SipStrConsts::EPhraseRequestTerminated );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRequestTerminated /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    
    MCE_RESET_STUBS();    
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledCanceledL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledCanceledL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
    CMceComSession* clientSession = NULL;
    

//incoming session    
    CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession );
    EUNIT_ASSERT( inSession != NULL );

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );


//update
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );
    CleanupStack::PopAndDestroy( update );

    inSession->Canceled();    
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallSessionRefreshL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallSessionRefreshL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );

    MCE_RESET_STUBS();

//session refresh
    TMceTimerId timerId = 1;
    outSession->iTimer = timerId;
    outSession->Body()->iRefresh = ETrue;
    outSession->TimerExpiredL( timerId, NULL );
    MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, KMceNotAssigned );
    
    MCE_RESET_STUBS();

//client receive
    MCETestHelper::ClientReceiveLC( outSession, iIds );
    
//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );

    MCE_RESET_STUBS();
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );
    CleanupStack::PopAndDestroy( clientMessage );

//session refresh
    outSession->iTimer = timerId;
    outSession->TimerExpiredL( timerId, NULL );
    MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, KMceNotAssigned );
    
    MCE_RESET_STUBS();

//client receive
    MCETestHelper::ClientReceiveLC( outSession, iIds );
    
//500    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipServerInternalError, SipStrConsts::EPhraseServerInternalError, INVITE_TRX );
    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateEstablished )    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipServerInternalError /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrCancel /*status*/ );
    
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );

    MCE_RESET_STUBS();
    
//expires
    outSession->Body()->iRefresh = EFalse;
    outSession->TimerExpiredL( (TMceTimerId)1, NULL );
	MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )
	MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 )
	
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminating /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipServerInternalError /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminating /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                                      
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrCancel /*status*/ );

    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
    
    MCE_RESET_STUBS();   
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallSessionRefreshedL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallSessionRefreshedL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );

    MCE_RESET_STUBS();
    
//invite
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    iStorage->iMediaManagerNeedToNegotiate = EFalse; // refresh
    MCETestHelper::RequestL( outSession, SipStrConsts::EInvite, SDP_CONTENT );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateAnswering /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateEstablished /*exitState*/,
                                       SipStrConsts::EInvite /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EAnswering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EAnswering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPData );
                      
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
    
//ACK received
    MCETestHelper::RequestL( outSession, SipStrConsts::EAck );

    MCE_ASSERT_SIPSESSION2( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                       SipStrConsts::EAck /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPData );
                      
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );

//terminate
    MCETestHelper::ClientTerminateL( outSession );

    MCE_RESET_STUBS();
        
//200OK 
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );
        
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallFailedL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallFailedL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    
    CMceSipSession* outSession = NULL;
    
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created --> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//200OK -- decoding fails --> BYE
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
    MCE_CHECK_MEMORY_LEAVE( iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EBye ) ||
         iStorage->iMediaManagerSdpAction != CMCETls::EDecode ||
         iStorage->iIds.iStatus != KErrGeneral )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminating /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminating /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    EUNIT_ASSERT( iStorage->iAckSent );
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminating /*clientState*/, KErrGeneral /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );
    MCE_RESET_STUBS();
    
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );
    CleanupStack::PopAndDestroy( clientMessage );

//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
    MCE_RESET_STUBS();

//200OK (bye)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateTerminating /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );
    MCE_RESET_STUBS();
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallFailed2L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallFailed2L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceSipSession* outSession = NULL;
        
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created --> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    

//200OK
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 )
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );    
    MCE_RESET_STUBS();
    
// media error 
// Ensure Session gets established even we get media errors while establishing a session

    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaError, KErrGeneral );
	MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallFailed4L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallFailed4L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    
    CMceSipSession* outSession = NULL;
    
        
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

    MCE_RESET_STUBS();

//offer created --> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 1 /*pendings*/);
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    // media error 
	// Ensure Session gets established even we get media errors while establishing a session

    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaError, KErrGeneral );
	MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();   
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallFailed3L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallFailed3L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    
    CMceSipSession* outSession = NULL;
    
        
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created --> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
    
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaError, KErrGeneral );

	MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledFailedL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledFailedL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
    CMceComSession* clientSession = NULL;
    CMceSipSession* inSession = NULL;

    MCE_RESET_STUBS();

//incoming session
    inSession = MCETestHelper::ClientIncomingSessionL( iSession, BAD_SDP );
    EUNIT_ASSERT( inSession == NULL );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipBadRequest /*sentResponse*/);    
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledFailed2L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledFailed2L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
    CMceComSession* clientSession = NULL;
    CMceSipSession* inSession = NULL;

    MCE_RESET_STUBS();

    iStorage->iMediaManagerDecodeStatus = KMceSipWarnMediaTypeNotAvailable;
    inSession = MCETestHelper::ClientIncomingSessionL( iSession );
    EUNIT_ASSERT( inSession == NULL );

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipNotAcceptableHere /*sentResponse*/);
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledFailed3L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledFailed3L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
    CMceComSession* clientSession = NULL;
    CMceSipSession* inSession = NULL;

    inSession = MCETestHelper::ClientIncomingSessionL( iSession );
    EUNIT_ASSERT( inSession != NULL );
    
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );

    MCE_RESET_STUBS();

//update fails (sync)
    iStorage->iMediaManagerUpdateStatus = KErrNotSupported;
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );
    MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse ||
    	iStorage->iSipSentResponse->StatusCode() != KMceSipServerInternalError )
    CleanupStack::PopAndDestroy( update );
    

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateTerminated /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateServerEstablishing /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipServerInternalError /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::ETerminated /*clientState*/, KErrNotSupported /*status*/ );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledFailed4L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledFailed4L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
    CMceComSession* clientSession = NULL;
    CMceSipSession* inSession = NULL;

    inSession = MCETestHelper::ClientIncomingSessionL( iSession );
    EUNIT_ASSERT( inSession != NULL );
    
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );

//update
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );
    CleanupStack::PopAndDestroy( update );

//answer created with error
    MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated, KErrGeneral );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateFailedL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateFailedL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
    
    
//create session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
//update
    CMceComSession* update = outSession->Body()->CloneL();
    CleanupStack::PushL( update );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
    audio->Codecs()[0]->iPTime = 300;
    
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    MCETestHelper::ClientUpdateSessionL( outSession, *update );
    
    //get original
    clientSession = MCETestHelper::ClientGetSessionL( outSession );
    delete clientSession;
    
    CleanupStack::PopAndDestroy( update );

//terminate
    MCETestHelper::ClientTerminateL( outSession );

	MCE_RESET_STUBS();

//200OK 
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );
        
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    CleanupStack::PopAndDestroy( clientMessage );    
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateFailed3L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateFailed3L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
    
    
//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
        
//update
    CMceComSession* update = outSession->Body()->CloneL();
    CleanupStack::PushL( update );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
    audio->Codecs()[0]->iPTime = 300;
    
    MCETestHelper::ClientUpdateSessionL( outSession, *update );
    CleanupStack::PopAndDestroy( update );

//update created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
        

//200OK
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
	MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() == KMceStateTerminating )

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
	
	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

    CleanupStack::PopAndDestroy( clientMessage );
        
//terminate
    MCETestHelper::ClientTerminateL( outSession );
    
    MCE_RESET_STUBS();

//200OK 
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );
        
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateFailed4L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateFailed4L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
    
    
//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
        
//update
    CMceComSession* update = outSession->Body()->CloneL();
    CleanupStack::PushL( update );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
    audio->Codecs()[0]->iPTime = 300;
    
    MCETestHelper::ClientUpdateSessionL( outSession, *update );
    CleanupStack::PopAndDestroy( update );

//update created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
        
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
    
//answer to update decoded with error
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated, KErrGeneral );
        
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
	
	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

    CleanupStack::PopAndDestroy( clientMessage );
        
        
//terminate
    MCETestHelper::ClientTerminateL( outSession );
    
    MCE_RESET_STUBS();
    
//200OK 
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );
    CleanupStack::PopAndDestroy( clientMessage );        
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateFailed5L
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallUpdateFailed5L()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
    
    
//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
        
//update
    CMceComSession* update = outSession->Body()->CloneL();
    CleanupStack::PushL( update );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
    audio->Codecs()[0]->iPTime = 300;
    
    MCETestHelper::ClientUpdateSessionL( outSession, *update );
    CleanupStack::PopAndDestroy( update );

//update created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
        
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipServerInternalError, SipStrConsts::EPhraseServerInternalError );
    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateEstablished )
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
	
	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

    CleanupStack::PopAndDestroy( clientMessage );
        
        
//terminate
    MCETestHelper::ClientTerminateL( outSession );

    MCE_RESET_STUBS();

//200OK 
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledPendingL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledPendingL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
    
    
//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_RESET_STUBS();
    
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRinging /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );

//invite
    MCETestHelper::RequestL( outSession, SipStrConsts::EInvite, SDP_CONTENT_2 );
    MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse || 
                            iStorage->iSipSentResponse->StatusCode() != KMceSipRequestPending )
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRinging /*response*/, 1 /*pendings*/);

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipRequestPending /*sentResponse*/);
    MCE_RESET_STUBS();

//200OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );

//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
	
	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

    CleanupStack::PopAndDestroy( clientMessage );

//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
//terminate
    MCETestHelper::ClientTerminateL( outSession );
    
    MCE_RESET_STUBS();
        
//200OK 
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );
        
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallReInvitePendingL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallReInvitePendingL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );
//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;
//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//200OK
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );

    MCE_RESET_STUBS();

//session refresh
    outSession->Body()->iRefresh = ETrue;
    TMceTimerId timerId = 1;
    outSession->iTimer = timerId;
    outSession->TimerExpiredL( timerId, NULL );
    MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, KMceNotAssigned );
    
    MCE_RESET_STUBS();

//client receive
    MCETestHelper::ClientReceiveLC( outSession, iIds );
    
//491OK    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRequestPending, SipStrConsts::EPhraseBadRequest );
    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateEstablished )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRequestPending /*response*/, 0 /*pendings*/);

	MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrCancel /*status*/ );

    MCE_RESET_STUBS();
    
// terminate
    MCETestHelper::ClientTerminateL( outSession );    
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledReInvitePendingL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledReInvitePendingL()
    {    
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    
//incoming session    
    CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession );
    EUNIT_ASSERT( inSession != NULL );

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );


//update
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );
    CleanupStack::PopAndDestroy( update );

//answer created
    MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );

//accept session
    MCETestHelper::ClientAcceptL( inSession );

//ACK received
    MCETestHelper::RequestL( inSession, SipStrConsts::EAck );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    CleanupStack::PopAndDestroy( clientMessage );
    

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                      SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);
    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

    //session refresh
    inSession->Body()->iRefresh = ETrue;
    TMceTimerId timerId = 1;
    inSession->iTimer = timerId;
    inSession->TimerExpiredL( timerId, NULL );
    MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateEstablished /*exitState*/,
                                       SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, KMceNotAssigned );
    
    MCE_RESET_STUBS();

//client receive
    MCETestHelper::ClientReceiveLC( inSession, iIds );
    
//491OK    
    EUNIT_ASSERT( inSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( inSession, KMceSipRequestPending, SipStrConsts::EPhraseBadRequest );
    
    MCE_CHECK_MEMORY_LEAVE( inSession->CurrentState().Id() != KMceStateEstablished )
    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateOffering /*exitState*/,
                                       SipStrConsts::EAck /*request*/, KMceSipRequestPending /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrCancel /*status*/ );

    MCE_RESET_STUBS();
    
// terminate
    MCETestHelper::ClientTerminateL( inSession );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledReInvitePendingFailedL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledReInvitePendingFailedL()
    {    
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    
//incoming session    
    CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession );
    EUNIT_ASSERT( inSession != NULL );

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );


//update
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );
    CleanupStack::PopAndDestroy( update );

//answer created
    MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );

//accept session
    MCETestHelper::ClientAcceptL( inSession );

//ACK received
    MCETestHelper::RequestL( inSession, SipStrConsts::EAck );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    CleanupStack::PopAndDestroy( clientMessage );
    

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                      SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);
    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

    //session refresh
    inSession->Body()->iRefresh = ETrue;
    TMceTimerId timerId = 1;
    inSession->iTimer = timerId;
    inSession->TimerExpiredL( timerId, NULL );
    MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateEstablished /*exitState*/,
                                       SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, KMceNotAssigned );
    
    MCE_RESET_STUBS();

//client receive
    MCETestHelper::ClientReceiveLC( inSession, iIds );
    
//491OK    
    EUNIT_ASSERT( inSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( inSession, KMceSipRequestPending, SipStrConsts::EPhraseBadRequest );

	MCE_CHECK_MEMORY_LEAVE( inSession->CurrentState().Id() != KMceStateEstablished )
    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ERefreshing /*subState*/, KMceStateOffering /*exitState*/,
                                       SipStrConsts::EAck /*request*/, KMceSipRequestPending /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrCancel /*status*/ );

    MCE_RESET_STUBS();
// terminate
    MCETestHelper::ClientTerminateL( inSession );      
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallInviteForksL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallInviteForksL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    
    CMceSipSession* outSession = NULL;
        
    MCE_RESET_STUBS();
    
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSession /*type*/, KMceStateIdle /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KErrNotFound /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_RESET_STUBS();                               

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session with no params
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSession /*type*/, KMceStateClientEstablishing /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateIdle /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, clientSession, CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();                               

    delete clientSession;
        
//offer created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 1 /*pendings*/);
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//180 in one dialog
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRinging /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
    MCE_RESET_STUBS();


//client receive 180 response
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );	
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    MCE_ASSERT_REPLY( clientReply, KMceSipRinging, _L8("Ringing"), _L8("")  );
    CleanupStack::PopAndDestroy( clientMessage );


//180 in another dialog
    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    CUri8* uri = CUri8::NewLC(); //OK
    MCE_SET_URIL( uri );
  	
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );

    CSIPInviteDialogAssoc* forked = 
        CSIPInviteDialogAssoc::NewL( outSession->SIPConnection().Connection(),
                                     fromheader, uri );
    CleanupStack::Pop( fromheader );
    CleanupStack::Pop( uri );
    
    // ForkedResponseL immediately pushes "forked" parameter to CleanupStack
    MCETestHelper::ForkedResponseL( outSession, forked, KMceSipRinging, SipStrConsts::EPhraseRinging );
    EUNIT_ASSERT( outSession->iForkedDialogs.Count() == 1 );
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRinging /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
    MCE_RESET_STUBS();


//client receive 180 response
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    MCE_ASSERT_REPLY( clientReply, KMceSipRinging, _L8("Ringing"), _L8("")  );
    CleanupStack::PopAndDestroy( clientMessage );

//200OK in another dialog

    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CUri8* uri2 = CUri8::NewLC(); //OK
    MCE_SET_URIL( uri );
  	
    CSIPFromHeader* fromheader2 = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader2 );

    CSIPInviteDialogAssoc* forked2 = 
        CSIPInviteDialogAssoc::NewL( outSession->SIPConnection().Connection(),
                                     fromheader2, uri2 );
    CleanupStack::Pop( fromheader2 );
    CleanupStack::Pop( uri2 );

	// ForkedResponseL immediately pushes "forked2" parameter to CleanupStack    
    MCETestHelper::ForkedResponseL( outSession, forked2, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
    EUNIT_ASSERT( outSession->iForkedDialogs.Count() == 2 );
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    
//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeMessageArray /*msgType*/ );
    MCE_RESET_STUBS();            

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
	
	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

    MCE_ASSERT_BODY( outSession, clientMsgSession->iObject, CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );

    CleanupStack::PopAndDestroy( clientMessage );


//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
		EUNIT_ASSERT( outSession->iForkedDialogs.Count() == 0 );
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                           CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);
        
//terminate
    MCETestHelper::ClientTerminateL( outSession );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminating /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminating /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminating /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
        
//200OK 
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateTerminating /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/, EMceItcMsgTypeSIPReply /*msgType*/ );
    
    MCE_RESET_STUBS();
        
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );

    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );

    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallRetransmissionL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallRetransmissionL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    
    CMceSipSession* outSession = NULL;
        
    MCE_RESET_STUBS();
    
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSession /*type*/, KMceStateIdle /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KErrNotFound /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_RESET_STUBS();                               

//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session with no params
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSession /*type*/, KMceStateClientEstablishing /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateIdle /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, clientSession, CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();                               

    delete clientSession;
        
//offer created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KErrNotFound /*response*/, 1 /*pendings*/);
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//180
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRinging, SipStrConsts::EPhraseRinging );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRinging /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeMessageArray );
    MCE_RESET_STUBS();


//client receive 180 response
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
 	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    MCE_ASSERT_REPLY( clientReply, KMceSipRinging, _L8("Ringing"), _L8("")  );
    CleanupStack::PopAndDestroy( clientMessage );
            
//200OK
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                               CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                               KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

// retransmission of 200OK
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KErrGeneral;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EAck /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//answer decoded
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeMessageArray /*msgType*/ );
    MCE_RESET_STUBS();            

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
	
	clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMsgArray->iArray[ 1 ] );

    MCE_ASSERT_BODY( outSession, clientMsgSession->iObject, CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("application/sdp") );

    CleanupStack::PopAndDestroy( clientMessage );


//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateEstablished /*state*/, 
                                           CMceSipSession::ENone /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);
        
//terminate
    MCETestHelper::ClientTerminateL( outSession );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminating /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminating /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminating /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
        
//200OK 
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::ENone /*subState*/, KMceStateTerminating /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/, EMceItcMsgTypeSIPReply /*msgType*/ );
    
    MCE_RESET_STUBS();
        
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );

    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );

    CleanupStack::PopAndDestroy( clientMessage );
    }
    
// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestIMSR6toR5IOPL 
// -----------------------------------------------------------------------------
//
 /**
  * INVITE ->
  * 100    <-     100 Trying from the UAS.
  * 421    <-     Extenssion Required request from the server.
  * ACK    ->  -- ACK for 421 Transaction.
  * INVITE ->  -- Precondition supported.
  * 183    <-     includes SDP
  * PRACK  ->  -- includes SDP
  * 200    <-  -- includes SDP for the PRACK:
  * UPDATE ->  -- includes SDP
  * 200    <-  -- includes SDP for the UPDATE.
  * 180    <-  -- ringing
  * 200    <-  -- no SDP
  * SESSION ESTABLISHED.
  */

void UT_MCEUsesCases::UT_MCEUsesCases_TestIMSR6toR5IOPL()
    {
    // Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
      
//421
    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, KMceSipExtensionRequired, SipStrConsts::EPhraseExtensionRequired );
    MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction != CMCETls::ENone )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipExtensionRequired /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EInvite /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
       
                     
    MCE_RESET_STUBS();
 
// 183 Session in progress : Client Receives
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, KMceSipSessionProgress, SipStrConsts::EPhraseSessionProgress, INVITE_TRX, SDP_CONTENT );
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 || 
                            outSession->CurrentState().Id() != KMceStateConfirming )
// Ensure Client Receives the Session In progress Response    
    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
   
// TODO, check that PRACK has been sent 
 
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::EEncode /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
   
// 200-OK for the PRACK is received.
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, PRACK_TRX, SDP_CONTENT );
    
// TODO: check that UPDATE is sent   
    TInt c = outSession->PendingTransactions().Count();
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::EEncode /*mmsdpaction*/, 
                        SipStrConsts::EUpdate /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();

// 200-OK for the UPDATE is received.
    TInt count = outSession->PendingTransactions().Count();
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, UPDATE_TRX, SDP_CONTENT );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    
    MCE_RESET_STUBS();
      
         
// 200   
// Client receives 200-OK for an initial INVITE.
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX );
        
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EAck /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
       
    MCE_RESET_STUBS(); 
    
    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
    MCE_RESET_STUBS();
	}

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalled100RelL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalled100RelL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;


    MCE_RESET_STUBS();
    
//incoming session    
    CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession, 
                                                                       EFalse /*aInvalidSdp*/,
                                                                       ETrue /*aSupported100rel*/ );

    EUNIT_ASSERT( inSession != NULL );
    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateServerEstablishing /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateIdle /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EIncoming /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/,
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                      
    MCE_ASSERT_MANAGER( EMceItcObjectAdded /*callback*/, EMceItcMsgTypeSession /*msgType*/ );
    EUNIT_ASSERT( inSession->Body()->Streams().Count() > 0 );
    MCE_RESET_STUBS();


//client session ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );


//update
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdating /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateServerEstablishing /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, update, CMceSession::EReserving /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/,
                      CMCETls::ENone /*mmsdpaction*/,  
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EReserving /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    
    CleanupStack::PopAndDestroy( update );
    

//answer created
    iStorage->iMediaManagerNeedToNegotiate = ETrue; // this causes sending 183 
    MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateAcknowledgementRequired /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateUpdating /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EReserving /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipSessionProgress /*sentResponse*/);
    MCE_RESET_STUBS();

//receive prack
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    MCETestHelper::RequestL( inSession, SipStrConsts::EPrack );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateAcknowledgementRequired /*exitState*/,
                                      SipStrConsts::EPrack /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMessage );
    clientSession = clientMsgSession->iObject;
    MCE_ASSERT_BODY( inSession, clientSession, CMceSession::EProceeding /*clientState*/ );
    CleanupStack::PopAndDestroy( clientMessage );
    
    MCETestHelper::ClientRingL( inSession );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipRinging /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    MCETestHelper::ClientAcceptL( inSession );//standard
    MCE_ASSERT_CLIENT( inSession, CMceSession::EAnswering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    

//ACK received
    MCETestHelper::RequestL( inSession, SipStrConsts::EAck );

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                      SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNone /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    
    CleanupStack::PopAndDestroy( clientMessage );

//BYE received
    MCETestHelper::RequestL( inSession, SipStrConsts::EBye );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateTerminated /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                      SipStrConsts::EBye /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCall100RelL
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCall100RelL()
    {
    // Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
       
// 180 Session in progress : Client Receives
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipRinging, 
                              SipStrConsts::EPhraseRinging, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/ );
     
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipRinging /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
     
//	Ensure Client Recives the Ringing Message.   	
        
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipRinging )
    CleanupStack::PopAndDestroy( clientMessage );
    
      
// 200-OK for the PRACK is received.
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, PRACK_TRX );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
       
    MCE_RESET_STUBS();
// 200   
// Client receives 200-OK for an initial INVITE.
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
        
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::EDecode /*mmsdpaction*/, 
                        SipStrConsts::EAck /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );

    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
       
    MCE_RESET_STUBS();
        
    CleanupStack::PopAndDestroy( clientMessage );

//BYE received
    MCETestHelper::RequestL( outSession, SipStrConsts::EBye );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                      SipStrConsts::EBye /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestSubsequentPrack
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestSubsequentPrack()
    {
// Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
       
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 )
    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateConfirming )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    // Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
    
   
// 200-OK for the PRACK is received.
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, PRACK_TRX );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
    
    // Sending Another Provisional Response (181: Call is being forwarded) with an RSeq Value of 501
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipCallIsBeingForwarded, 
                              KCallIsBeingForwareded, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              501   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateConfirming )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipCallIsBeingForwarded /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    // Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipCallIsBeingForwarded )
    CleanupStack::PopAndDestroy( clientMessage );
    
   
// 200-OK for the PRACK is received.
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, PRACK_TRX );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
       
    MCE_RESET_STUBS();
    
          
    // Sending Another Provisional Response (181: Call is being forwarded) with an RSeq Value of 508
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipCallIsBeingForwarded, 
                              KCallIsBeingForwareded, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              508   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction != CMCETls::ENone )

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);


                
    // Sending Another Provisional Response (182: Call is being forwarded) with an RSeq Value of 509)
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipQueued, 
                              KSipCallQueued, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              509   /* RSeq Value */ );

	MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction != CMCETls::ENone )

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);             

                            
    
    // Sending Another Provisional Response (180: Call is being forwarded) with an RSeq Value of 508)
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipRinging, 
                              SipStrConsts::EPhraseRinging, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              508   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction != CMCETls::ENone )

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);   

        
    // Sending Another Provisional Response (180: Call is being forwarded) with an RSeq Value of 502)
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipRinging, 
                              SipStrConsts::EPhraseRinging, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              502   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateConfirming ||
         iStorage->iMediaManagerAction != CMCETls::ENone ||
         iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EPrack ) )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipRinging /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    // Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipRinging )
    CleanupStack::PopAndDestroy( clientMessage );
    
   
// 200-OK for the PRACK is received.
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, PRACK_TRX );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
       
    MCE_RESET_STUBS();
 
    
// Client receives 200-OK for an initial INVITE.
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
        
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::EDecode /*mmsdpaction*/, 
                        SipStrConsts::EAck /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);

    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );

    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
       
    MCE_RESET_STUBS();
        
    CleanupStack::PopAndDestroy( clientMessage );

//BYE received
    MCETestHelper::RequestL( outSession, SipStrConsts::EBye );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                      SipStrConsts::EBye /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );              
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestEarlyResponseToInvite
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestEarlyResponseToInvite()
    {  
    // Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();

    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 )
    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateConfirming )
 
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
// Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
           
// Client receives 200-OK for an initial INVITE.
// 200 OK with SDP
    iStorage->iMediaManagerNeedToNegotiate = EFalse; 
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, SDP_CONTENT );
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 )
    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() == 1 );
      
    MCE_ASSERT_STUBS( CMCETls::EUpdate  /*mmaction*/, 
                        CMCETls::EDecode /*mmsdpaction*/,  
                        SipStrConsts::EAck /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);

    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );

    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
    MCE_RESET_STUBS();
    
//BYE received
    MCETestHelper::RequestL( outSession, SipStrConsts::EBye );
    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateTerminated ||
         !iStorage->iSipSentResponse || 
         iStorage->iSipSentResponse->StatusCode() != KMceSipOK )
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                      SipStrConsts::EBye /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestEarlyResponseToInvite1
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestEarlyResponseToInvite1()
    {     
// Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
       
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 )
    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateConfirming )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
    
// Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
       
// Client receives 200-OK for an initial INVITE.
// 200 OK with NO-SDP.

    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX );
    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() == 2 );
        
    MCE_ASSERT_STUBS( CMCETls::ENone  /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/,  
                        SipStrConsts::EAck /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);

    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering  /*clientState*/, KErrNone /*status*/ );
    // 200-OK for the PRACK is received.
    MCE_RESET_STUBS();
    iStorage->iMediaManagerNeedToNegotiate = EFalse; 
    iStorage->iMediaManagerUpdateStatus =   KMceReady;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, PRACK_TRX, SDP_CONTENT );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 )

    EUNIT_ASSERT( outSession->PendingTransactions().Count() == 1 );
     
    
    MCE_ASSERT_STUBS( CMCETls::ENone, 
                      CMCETls::ENone,
                      SipStrConsts::EEmpty, 
                      KErrNotFound );
    
    
    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
   
    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
      
    MCE_ASSERT_DATA( clientReply );

    CleanupStack::PopAndDestroy( clientMessage );
           
    MCE_RESET_STUBS();
    
    //BYE received
    MCETestHelper::RequestL( outSession, SipStrConsts::EBye );

    MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateTerminated ||
         !iStorage->iSipSentResponse || 
         iStorage->iSipSentResponse->StatusCode() != KMceSipOK )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                      SipStrConsts::EBye /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);
    
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated );
    
    MCE_ASSERT_STUBS( CMCETls::ECloseSession, 
                      CMCETls::ENone, 
                      SipStrConsts::EEmpty, 
                      KMceSipOK );
                          
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated, KErrNone );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPData );
    
    
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestEarlyResponseToInvite2
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestEarlyResponseToInvite2()
    {     
// Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
       
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              EFalse /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 )
	MCE_CHECK_MEMORY_LEAVE( outSession->CurrentState().Id() != KMceStateConfirming )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
    
// Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );

    // Client receives 200-OK for an initial INVITE.
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX );
    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() == 2 );
        
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EAck /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);

    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    
    TUint States = outSession->CurrentState().Id();
                       
    outSession->SIPConnection().ErrorOccured( KErrTimedOut, *outSession->PendingTransactions()[ PRACK_TRX ] );

    MCE_CHECK_MEMORY_LEAVE( iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EBye ) ||
                            iStorage->iIds.iState != CMceSession::ETerminating )

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EBye /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);   
                               

    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminating /*clientState*/, KErrTimedOut  /*status*/ );                      
    
                  
    EUNIT_ASSERT( outSession->Dialog()->Dialog().State() == CSIPDialog::EConfirmed );  
                           
    MCE_RESET_STUBS();
    }
       
/**
 * INVITE	        -> with SDP
 * 183(RSeq 500)	    <- 100 rel required with SDP 
 * PRACK		        ->	with SDP
 * ((Prack Time out occurs )
 * CANCEL            ->
 * 200 (c)           <-
 * SESSION TERMINATED.
 */

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestProvisionalResponseTimeOut 
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestProvisionalResponseTimeOut()
    { 
// Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
       
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              ETrue /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 || 
                             outSession->CurrentState().Id() != KMceStateConfirming )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
// Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
        
    outSession->Dialog()->Dialog().iState = CSIPDialog::EEarly;
            
    // Force the Dialog state to be EEarly

    outSession->SIPConnection().ErrorOccured( KErrTimedOut, *outSession->PendingTransactions()[ PRACK_TRX ] );
    MCE_CHECK_MEMORY_LEAVE( iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::ECancel ) ||
                            iStorage->iIds.iStatus != KErrTimedOut ||
                            outSession->PendingTransactions().Count() != 3 ||
                            outSession->iReceiver->iClientReadyToReceive )
                                     
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);   
                               

    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrTimedOut /*status*/ );
        
    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
       
    MCE_RESET_STUBS();
    
    
    //200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateTerminated )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                      
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
        
    MCE_RESET_STUBS();
    }
   
/**
 * INVITE	            -> with SDP
 * 183(RSeq 500)	    <- 100 rel required with SDP
 * PRACK		        ->	with SDP
 * 200 (PRACK)          <-  with SDP
 * UPDATE               ->  with SDP
 * ((UPDATE Time out occurs )
 * CANCEL               ->
 * 200 (c)              <-
 * SESSION TERMINATED
 */
 
// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestProvisionalResponseTimeOut1
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestProvisionalResponseTimeOut1()
    {
// Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
       
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              ETrue /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateConfirming )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
// Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
        

// 200-OK for the PRACK is received.
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, PRACK_TRX, SDP_CONTENT );

    MCE_CHECK_MEMORY_LEAVE( iStorage->iIds.iState != CMceSession::EOffering )

    TInt c = outSession->PendingTransactions().Count();
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::EEncode /*mmsdpaction*/, 
                        SipStrConsts::EUpdate /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
       
    MCE_RESET_STUBS();
    
    
    // Force the Dialog state to be EEarly
    
    outSession->Dialog()->Dialog().iState = CSIPDialog::EEarly;
    outSession->SIPConnection().ErrorOccured( KErrTimedOut, *outSession->PendingTransactions()[ UPDATE_TRX ] );

    MCE_CHECK_MEMORY_LEAVE( iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::ECancel ) ||
                            iStorage->iIds.iStatus != KErrTimedOut ||
                            outSession->PendingTransactions().Count() != 3 ||
                            outSession->iReceiver->iClientReadyToReceive )
        
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);   
                               

    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrTimedOut /*status*/ );
        
    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
       
    MCE_RESET_STUBS();
    
    
    //200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateTerminated )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                      
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
    }
                
/**
 * INVITE	            -> with SDP
 * 183(RSeq 500)	    <- 100 rel required with SDP
 * PRACK		        ->	with SDP
 * 4xx                  <-
 * CANCEL               ->
 * 200 (c)              <-
 * SESSION TERMINATED
 */ 

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestProvisionalBadRequest()
// ----------------------------------------------------------------------------- 
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestProvisionalBadRequest()
    {
    // Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 ||
                            outSession->CurrentState().Id() != KMceStateOffering )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
       
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              ETrue /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateConfirming )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    
 // Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
    

    // KMceSipRequestTerminated 487
    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipRequestTerminated, 
                              SipStrConsts::EPhraseRequestTerminated, 
                              PRACK_TRX );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateCanceled ||
                            iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::ECancel ) ||
                            iStorage->iIds.iState != CMceSession::ECancelling ||
                            outSession->iReceiver->iClientReadyToReceive )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateConfirming /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipRequestTerminated /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::ECancel /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );

    // MCE_RESET_STUBS();
    
    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
       
    MCE_RESET_STUBS();
    
    
    //200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                      
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
        
    MCE_RESET_STUBS();
    }

/**
 * INVITE	            -> with SDP
 * 183(RSeq 500)	    <- 100 rel required with SDP
 * PRACK		        -> 
 * 200                  <- 
 * UPDATE               ->
 * 5xx                  <-
 * CANCEL               ->
 * 200 (c)              <-
 * SESSION TERMINATED
 */ 

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestProvisionalBadRequest1()
// ----------------------------------------------------------------------------- 
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestProvisionalBadRequest1()
    {
// Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
       
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              ETrue /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateConfirming ||
                            iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EPrack ) )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
// Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );

// 200-OK for the PRACK is received & UPDATE is Sent.

    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, PRACK_TRX, SDP_CONTENT );
    
    TInt c = outSession->PendingTransactions().Count();
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::EEncode /*mmsdpaction*/, 
                        SipStrConsts::EUpdate /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
            
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipServerInternalError, 
                              SipStrConsts::EPhraseServerInternalError, 
                              UPDATE_TRX );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateCanceled ||
                            iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::ECancel ) ||
                            iStorage->iIds.iState != CMceSession::ECancelling ||
                            outSession->iReceiver->iClientReadyToReceive )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateConfirming /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipServerInternalError /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::ECancel /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
    
   
    //client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
    
    MCE_RESET_STUBS();
    
    
    //200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                      
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
        
    MCE_RESET_STUBS();
    }

/**
 * INVITE	            -> with SDP
 * 183(RSeq 500)	    <- 100 rel required with SDP
 * PRACK		        ->	with SDP
 * 4xx (INVITE)         <-
 * SESSION TERMINATED
 */ 
// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestInviteBadRequest()
// ----------------------------------------------------------------------------- 
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestInviteBadRequest()
    {
    // Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );

    MCE_RESET_STUBS();
       
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              ETrue /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateConfirming ||
                            iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EPrack ) ||
                            iStorage->iIds.iState != CMceSession::EOffering )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
	
// Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
    
	

    // KMceSipRequestTerminated 487
    
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipRequestTerminated, 
                              SipStrConsts::EPhraseRequestTerminated, 
                              INVITE_TRX );

    MCE_CHECK_MEMORY_LEAVE( iStorage->iMediaManagerAction != CMCETls::ENone )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                   CMceSipSession::EOffering /*subState*/, KMceStateConfirming /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipRequestTerminated /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EEmpty /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    }

/** 
 * INVITE        with SDP
 * 100        
 * 200 (INVITE)
 * ACK 
 * INVITE-2      with SDP 
 * 180 (RSeq 500) (Session in Progress)
 * PRACK
 * 4xx ( INVITE-2 ) ( 4xx != KMceSipSessionIntervalTooSmall (422)
 *                  && 4xx != KMceSipExtensionRequired (421)
 *                  && 4xx != KMceSipRequestPending
 *                  && 4xx != KMceSipCallOrTransactionDoesNotExist
 *                  && 4xx != KMceSipRequestTimeout )
 * 
 * SESSION ESTABLISHED
 */                  

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestReInviteBadRequest()
// ----------------------------------------------------------------------------- 
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestReInviteBadRequest()
    {
    // Creating an Outgoing Invite Transaction 
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
    
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, ETrue );
        
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::EDecode /*mmsdpaction*/, 
                        SipStrConsts::EAck /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);

    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
    MCE_RESET_STUBS();
    
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
    MCE_RESET_STUBS();
       
   //update
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CMceComSession* update = outSession->Body()->CloneL();
    CleanupStack::PushL( update );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
    audio->Codecs()[0]->iPTime = 300;
    
    MCETestHelper::ClientUpdateSessionL( outSession, *update );
                
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateClientEstablishing /*state*/, 
                                       CMceSipSession::EUpdating /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();

    CleanupStack::PopAndDestroy( update );


//update created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );


    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EUpdating /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
  
    // Send PRACK.
    // Revice 200OK for the PRACK.
    
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              ETrue /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );
    
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateConfirming ||
                            iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EPrack ) ||
                            iStorage->iIds.iState != CMceSession::EOffering )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EUpdating /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
 // Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );

    // Receive Error Response for pending INVITE.
    // This sets iStorage->iIds.iStatus to KErrCancel
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipRequestTerminated, 
                              SipStrConsts::EPhraseRequestTerminated, 
                              INVITE_TRX ); 

    MCE_CHECK_MEMORY_LEAVE( iStorage->iIds.iState != CMceSession::EEstablished )
    MCE_CHECK_MEMORY_LEAVE( iStorage->iIds.iStatus != KErrCancel )

    EUNIT_ASSERT( outSession->PendingTransactions().Count() == 1 );

    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrCancel /*status*/ );             
    MCE_RESET_STUBS();      
    }

/** 
 * INVITE        with SDP
 * 100        
 * 200 (INVITE)
 * ACK 
 * INVITE-2      with SDP 
 * 180 (RSeq 500) (Session in Progress)
 * PRACK
 * 4xx ( INVITE-2 ) && 4xx != KMceSipCallOrTransactionDoesNotExist
 *                  && 4xx != KMceSipRequestTimeout )
 * 
 * SESSION TERMINATED
 */                  

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestReInviteBadRequest1()
// ----------------------------------------------------------------------------- 
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestReInviteBadRequest1()
    {
    // Creating an Outgoing Invite Transaction 
    
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;
        
//create session            
     outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                        _L8("User <user2@host>"), 
                                                        _L8("User <user@host>") );
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession );
    delete clientSession;

//offer created
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );
    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                           CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                           KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,   
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
    
    iStorage->iMediaManagerUpdateStatus = KMceReady;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, INVITE_TRX, ETrue );
        
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                        CMCETls::EDecode /*mmsdpaction*/, 
                        SipStrConsts::EAck /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);

    MCE_ASSERT_CLIENT( outSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    CleanupStack::PopAndDestroy( clientMessage );
    MCE_RESET_STUBS();
    
//invite completed
    outSession->InviteCompleted( *outSession->PendingTransactions()[ INVITE_TRX ] );
        
    MCE_RESET_STUBS();
       
   //update
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CMceComSession* update = outSession->Body()->CloneL();
    CleanupStack::PushL( update );
    CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( update->Streams()[0] );
    audio->Codecs()[0]->iPTime = 300;
    
    MCETestHelper::ClientUpdateSessionL( outSession, *update );
                
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateClientEstablishing /*state*/, 
                                       CMceSipSession::EUpdating /*subState*/, KMceStateEstablished /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();

    CleanupStack::PopAndDestroy( update );


//update created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );


    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EUpdating /*subState*/, KMceStateClientEstablishing /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EInvite /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    
    MCE_RESET_STUBS();
  
    // Send PRACK.
    // Revice 200OK for the PRACK.
    
    // Sending Provisional Response (183: Call in Progress ) with an RSeq Value of 500
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipSessionProgress, 
                              SipStrConsts::EPhraseSessionProgress, 
                              INVITE_TRX, 
                              ETrue /*aSdpConsts*/,
                              ETrue /*aRequire100rel*/, 
                              500   /* RSeq Value */ );
    
    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            outSession->CurrentState().Id() != KMceStateConfirming ||
                            iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::EPrack ) ||
                            iStorage->iIds.iState != CMceSession::EOffering )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateConfirming /*state*/, 
                                   CMceSipSession::EUpdating /*subState*/, KMceStateOffering /*exitState*/,
                                   KErrNotFound /*request*/, KMceSipSessionProgress /*response*/, 2 /*pendings*/);
              
    MCE_ASSERT_BODY_1( outSession, (&outSession->Actions().ActiveBody()), CMceSession::EOffering /*clientState*/ );

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                        CMCETls::ENone /*mmsdpaction*/, 
                        SipStrConsts::EPrack /*sentMethod*/, 
                        KErrNotFound /*sentResponse*/);
                            
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
	
// Ensure Client Receives the Provisional Response i.e. <Session in Progress>    
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds  );
    clientMsgArray = static_cast<CMceMsgArray<TMceComFactory>*>( clientMessage );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMsgArray->iArray[ 0 ] );
    
    EUNIT_ASSERT( clientReply->iCode == KMceSipSessionProgress )
    CleanupStack::PopAndDestroy( clientMessage );
    
    

    // Receive Error Resonse for pending INVITE
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );    
    MCETestHelper::ResponseL( outSession, 
                              KMceSipCallOrTransactionDoesNotExist, 
                              SipStrConsts::EPhraseCallTransactionDoesNotExist, 
                              INVITE_TRX ); 

    MCE_CHECK_MEMORY_LEAVE( iStorage->iIds.iState != CMceSession::ETerminated )

    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );             
    MCE_RESET_STUBS();    
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledPrackedInWrongStateL
// ----------------------------------------------------------------------------- 
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledPrackedInWrongStateL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;


    MCE_RESET_STUBS();
    
//incoming session    
    CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession );

    EUNIT_ASSERT( inSession != NULL );
    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateServerEstablishing /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateIdle /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EIncoming /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/,
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                      
    MCE_ASSERT_MANAGER( EMceItcObjectAdded /*callback*/, EMceItcMsgTypeSession /*msgType*/ );
    EUNIT_ASSERT( inSession->Body()->Streams().Count() > 0 );
    MCE_RESET_STUBS();


//client session ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );


//update
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdating /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateServerEstablishing /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, update, CMceSession::EReserving /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/,
                      CMCETls::ENone /*mmsdpaction*/,  
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EReserving /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    
    CleanupStack::PopAndDestroy( update );
    

//answer created
    MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateUpdating /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSession /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMessage );
    clientSession = clientMsgSession->iObject;
    MCE_ASSERT_BODY( inSession, clientSession, CMceSession::EProceeding /*clientState*/ );
    CleanupStack::PopAndDestroy( clientMessage );

    
//ringing
    MCETestHelper::ClientRingL( inSession );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipRinging /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//receive PRACK
    CSIPServerTransaction* prack = MCETestHelper::ServerTransactionL( SipStrConsts::EPrack );
    // Takes ownership of prack
    inSession->iSIPConnection.IncomingRequest( prack, inSession->Dialog()->Dialog() );

    MCE_CHECK_MEMORY_LEAVE( !iStorage->iSipSentResponse ||
                            iStorage->iSipSentResponse->StatusCode() != KMceSipCallOrTransactionDoesNotExist )

    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipCallOrTransactionDoesNotExist /*sentResponse*/);

// answer    
    MCETestHelper::ClientAcceptL( inSession, KNullDesC8, 0 );//standard
    MCE_ASSERT_CLIENT( inSession, CMceSession::EAnswering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    

//ACK received
    MCETestHelper::RequestL( inSession, SipStrConsts::EAck );

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                      SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNone /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    
    CleanupStack::PopAndDestroy( clientMessage );

//BYE received
    MCETestHelper::RequestL( inSession, SipStrConsts::EBye );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateTerminated /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                      SipStrConsts::EBye /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );                    
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCalledReliableRingingL
// ----------------------------------------------------------------------------- 
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCalledReliableRingingL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;


    MCE_RESET_STUBS();
    
//incoming session    
    CMceSipSession* inSession = MCETestHelper::ClientIncomingSessionL( iSession );

    EUNIT_ASSERT( inSession != NULL );
    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateServerEstablishing /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateIdle /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EIncoming /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/, 
                      CMCETls::EDecode /*mmsdpaction*/,
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
                      
    MCE_ASSERT_MANAGER( EMceItcObjectAdded /*callback*/, EMceItcMsgTypeSession /*msgType*/ );
    EUNIT_ASSERT( inSession->Body()->Streams().Count() > 0 );
    MCE_RESET_STUBS();


//client session ready to receive
    MCETestHelper::ClientReadyToReceiveL( inSession );


//update
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    CMceComSession* update = inSession->Body()->CloneL();
    CleanupStack::PushL( update );
    update->Streams()[0]->iLocalMediaPort = 5006;
    // set require 100rel on in the update body
    update->Modifier( KMce100Rel ) = KMce100RelRequired;
    MCETestHelper::ClientUpdateSessionL( inSession, *update );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdating /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateServerEstablishing /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, update, CMceSession::EReserving /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::EUpdate /*mmaction*/,
                      CMCETls::ENone /*mmsdpaction*/,  
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EReserving /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
    
    
    CleanupStack::PopAndDestroy( update );
    

//answer created
    MCETestHelper::MediaManagerCallBack( inSession, EMceMediaUpdated );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateUpdating /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSession /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMessage );
    clientSession = clientMsgSession->iObject;
    MCE_ASSERT_BODY( inSession, clientSession, CMceSession::EProceeding /*clientState*/ );
    CleanupStack::PopAndDestroy( clientMessage );

// send 180 ringing
    MCETestHelper::ClientRingL( inSession );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateAcknowledgementRequired /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipRinging /*sentResponse*/);
    iStorage->iMediaManagerNeedToNegotiate = EFalse;
    MCETestHelper::RequestL( inSession, SipStrConsts::EPrack );
    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateUpdated /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateAcknowledgementRequired /*exitState*/,
                                      SipStrConsts::EPrack /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);
    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EProceeding /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EProceeding /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientMsgSession = static_cast<CMceMsgObject<CMceComSession>*>( clientMessage );
    clientSession = clientMsgSession->iObject;
    MCE_ASSERT_BODY( inSession, clientSession, CMceSession::EProceeding /*clientState*/ );
    CleanupStack::PopAndDestroy( clientMessage );  
  
    MCE_RESET_STUBS();
            
//accept session    
    MCETestHelper::ClientAcceptL( inSession );//standard
    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateAnswering /*state*/, 
                                      CMceSipSession::EAnswering /*subState*/, KMceStateUpdated /*exitState*/,
                                      SipStrConsts::EInvite /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EAnswering /*clientState*/ );
    
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::EEncode /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
        
    MCE_ASSERT_CLIENT( inSession, CMceSession::EAnswering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();
        

//ACK received
    MCETestHelper::RequestL( inSession, SipStrConsts::EAck );

    MCE_ASSERT_SIPSESSION2( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateEstablished /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateAnswering /*exitState*/,
                                      SipStrConsts::EAck /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::EEstablished /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNone /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::EEstablished /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    
    CleanupStack::PopAndDestroy( clientMessage );

//BYE received
    MCETestHelper::RequestL( inSession, SipStrConsts::EBye );

    MCE_ASSERT_SIPSESSION( inSession, CMceCsSubSession::EInSession /*type*/, KMceStateTerminated /*state*/, 
                                      CMceSipSession::ENone /*subState*/, KMceStateEstablished /*exitState*/,
                                      SipStrConsts::EBye /*request*/, KErrNotFound /*response*/, 0 /*pendings*/);

    MCE_ASSERT_BODY( inSession, inSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KMceSipOK /*sentResponse*/);
    MCE_ASSERT_CLIENT( inSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged /*callback*/ , EMceItcMsgTypeSIPData /*msgType*/ );
    MCE_RESET_STUBS();

//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( inSession, iIds );
    clientData = static_cast<CMceMsgSIPData*>( clientMessage );
    MCE_ASSERT_DATA( clientData );
    CleanupStack::PopAndDestroy( clientMessage );
    }

// -----------------------------------------------------------------------------
// UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancelAfterWrongReliable
// -----------------------------------------------------------------------------
//
void UT_MCEUsesCases::UT_MCEUsesCases_TestCallCancelAfterWrongReliableL()
    {
    CMceMsgBase* clientMessage = NULL;
    CMceMsgSIPReply* clientReply = NULL;
    CMceComSession* clientSession = NULL;
    CMceMsgObject<CMceComSession>* clientMsgSession = NULL;
	CMceMsgArray<TMceComFactory>* clientMsgArray = NULL;
    CMceMsgSIPData* clientData = NULL;
    CMceSipSession* outSession = NULL;

//create session            
    outSession = MCETestHelper::ClientCreateSessionL( iSession, 
                                                    _L8("User <user2@host>"), 
                                                    _L8("User <user@host>") );
	
    
//client ready to receive
    MCETestHelper::ClientReadyToReceiveL( outSession );

//establish session
    iStorage->iMediaManagerUpdateStatus = KMceAsync;
    clientSession = MCETestHelper::ClientEstablishL( outSession,
                                                     KNullDesC8 /*aHeader*/,
                                                     KNullDesC8 /*aContentType*/,
                                                     KNullDesC8 /*aContent*/,
                                                     ETrue /*aRequire100rel*/ );
    delete clientSession;

//offer created -> INVITE
    MCETestHelper::MediaManagerCallBack( outSession, EMceMediaUpdated );

    MCE_RESET_STUBS();

//100
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipTrying, SipStrConsts::EPhraseTrying );
    
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateOffering /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipTrying /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::EOffering /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::EOffering /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

// 180 Ringing with no Require:100rel
    outSession->Dialog()->Dialog().iState = CSIPDialog::EEarly;
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );

    MCETestHelper::ResponseL( outSession, KMceSipRinging,
    	SipStrConsts::EPhraseRinging, 0, EFalse, EFalse, 1, EFalse );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 2 ||
                            iStorage->iSipSentMethod != SIPStrings::StringF( SipStrConsts::ECancel ) ||
                            iStorage->iIds.iState != CMceSession::ECancelling )

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateCanceled /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateOffering /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRinging /*response*/, 2 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ECancelling /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ECloseSession /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/,
                      SipStrConsts::ECancel /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ECancelling /*clientState*/, KErrNone /*status*/ );
    MCE_RESET_STUBS();

    //client receive 180 response
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    EUNIT_ASSERT( !clientMessage );
    
//200(c)
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipOK, SipStrConsts::EPhraseOk, CANCEL_TRX );

    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateCanceled /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipOK /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );
    MCE_ASSERT_CLIENT_1( EMceItcStateChanged, EMceItcMsgTypeSIPReply );
    
    MCE_RESET_STUBS();
    
    
//client receive
    clientMessage = MCETestHelper::ClientReceiveLC( outSession, iIds );
    clientReply = static_cast<CMceMsgSIPReply*>( clientMessage );
    MCE_ASSERT_REPLY( clientReply, KMceSipOK, _L8("OK"), _L8("") );
    CleanupStack::PopAndDestroy( clientMessage );

    
//487
    EUNIT_ASSERT( outSession->PendingTransactions().Count() > 0 );
    MCETestHelper::ResponseL( outSession, KMceSipRequestTerminated, SipStrConsts::EPhraseRequestTerminated, INVITE_TRX );

    MCE_CHECK_MEMORY_LEAVE( outSession->PendingTransactions().Count() != 1 ||
                            iStorage->iIds.iState != CMceSession::ETerminated )
                            
    MCE_ASSERT_SIPSESSION( outSession, CMceCsSubSession::EOutSIPSession /*type*/, KMceStateTerminated /*state*/, 
                                       CMceSipSession::EOffering /*subState*/, KMceStateTerminated /*exitState*/,
                                       KErrNotFound /*request*/, KMceSipRequestTerminated /*response*/, 1 /*pendings*/);

    MCE_ASSERT_BODY( outSession, outSession->Body(), CMceSession::ETerminated /*clientState*/ );
    MCE_ASSERT_STUBS( CMCETls::ENone /*mmaction*/, 
                      CMCETls::ENone /*mmsdpaction*/, 
                      SipStrConsts::EEmpty /*sentMethod*/, 
                      KErrNotFound /*sentResponse*/);
    MCE_ASSERT_CLIENT( outSession, CMceSession::ETerminated /*clientState*/, KErrNone /*status*/ );    
    MCE_RESET_STUBS();
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_MCEUsesCases,
    "UT_CMCEUsesCases",
    "UNIT" )

EUNIT_TEST(
    "Basic call - test ",
    "CMceSipSession",
    "CALL",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallL, Teardown)

EUNIT_TEST(
    "Basic called - test ",
    "CMceSipSession",
    "CALLED",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledL, Teardown)
    
EUNIT_TEST(
    "Basic call and update - test ",
    "CMceSipSession",
    "CALL & UPDATE",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallAndUpdateL, Teardown)

EUNIT_TEST(
    "Basic called and updated - test ",
    "CMceSipSession",
    "CALLED & UPDATED",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledAndUpdatedL, Teardown)

EUNIT_TEST(
    "Basic call and declined  - test ",
    "CMceSipSession",
    "CALL & DECLINED",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallDeclinedL, Teardown)

EUNIT_TEST(
    "Basic called and decline with and without paramters - test ",
    "CMceSipSession",
    "CALLED & DECLINE",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledDeclineL, Teardown)

EUNIT_TEST(
    "Basic call and cancel (200 to CANCEL, 487 to INVITE) - test ",
    "CMceSipSession",
    "CALL & CANCEL",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallCancelL, Teardown)

EUNIT_TEST(
    "Basic call and cancel (487 to INVITE, 200 to CANCEL ) - test ",
    "CMceSipSession",
    "CALL & CANCEL",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallCancel2L, Teardown)
    
EUNIT_TEST(
    "Basic call and cancel (200 to INVITE ) - test ",
    "CMceSipSession",
    "CALL & CANCEL",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallCancel3L, Teardown)
    

EUNIT_TEST(
    "Basic call and cancel before 100 trying - test ",
    "CMceSipSession",
    "CALL & CANCEL",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallCancel4L, Teardown)
    
EUNIT_TEST(
    "Basic call and cancel before INVITE - test ",
    "CMceSipSession",
    "CALL & CANCEL",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallCancel5L, Teardown)

EUNIT_TEST(
    "Basic call and update cancel - test ",
    "CMceSipSession",
    "CALL & UPDATE CANCEL",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallUpdateCancelL, Teardown)

EUNIT_TEST(
    "Basic call and update cancel - test 2",
    "CMceSipSession",
    "CALL & UPDATE CANCEL 2",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallUpdateCancel2L, Teardown)

EUNIT_TEST(
    "Basic called and canceled - test ",
    "CMceSipSession",
    "CALLED & CANCELED",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledCanceledL, Teardown)

EUNIT_TEST(
    "Basic call and refresh - test ",
    "CMceSipSession",
    "CALL & REFRESH",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallSessionRefreshL, Teardown)

EUNIT_TEST(
    "Basic call and refreshed - test ",
    "CMceSipSession",
    "CALL & REFRESHED",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallSessionRefreshedL, Teardown)

EUNIT_TEST(
    "Basic call and negotiation fails - test ",
    "CMceSipSession",
    "CALL & FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallFailedL, Teardown)

EUNIT_TEST(
    "Basic call and negotiation fails 2 - test ",
    "CMceSipSession",
    "CALL & FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallFailed2L, Teardown)

EUNIT_TEST(
    "Basic call and negotiation fails 3 - test ",
    "CMceSipSession",
    "CALL & FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallFailed3L, Teardown)

EUNIT_TEST(
    "Basic call and negotiation fails 4 - test ",
    "CMceSipSession",
    "CALL & FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallFailed4L, Teardown)


EUNIT_TEST(
    "Basic called and negotiation fails - test ",
    "CMceSipSession",
    "CALLED & FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledFailedL, Teardown)

EUNIT_TEST(
    "Basic called and negotiation fails - test ",
    "CMceSipSession",
    "CALLED & FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledFailed2L, Teardown)

EUNIT_TEST(
    "Basic called and negotiation fails - test ",
    "CMceSipSession",
    "CALLED & FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledFailed3L, Teardown)

EUNIT_TEST(
    "Basic called and negotiation fails - test ",
    "CMceSipSession",
    "CALLED & FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledFailed4L, Teardown)

EUNIT_TEST(
    "Basic call and update fails - test ",
    "CMceSipSession",
    "CALL & UPDATE FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallUpdateFailedL, Teardown)

EUNIT_TEST(
    "Basic call and update fails - test ",
    "CMceSipSession",
    "CALL & UPDATE FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallUpdateFailed3L, Teardown)

EUNIT_TEST(
    "Basic call and update fails - test ",
    "CMceSipSession",
    "CALL & UPDATE FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallUpdateFailed4L, Teardown)

EUNIT_TEST(
    "Basic call and update fails - test ",
    "CMceSipSession",
    "CALL & UPDATE FAILS",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallUpdateFailed5L, Teardown)

EUNIT_TEST(
    "Basic called and 491 for re-INVITE - test ",
    "CMceSipSession",
    "CALLED & PENDING",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledPendingL, Teardown)

EUNIT_TEST(
    "Basic call, and 491 for re-INVITE - test ",
    "CMceSipSession",
    "CALL & UPDATE & PENDING",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallReInvitePendingL, Teardown)

EUNIT_TEST(
    "Basic called, and 491 for re-INVITE - test ",
    "CMceSipSession",
    "CALLED & UPDATE & PENDING",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledReInvitePendingL, Teardown)

EUNIT_TEST(
    "Basic called, and 2x491 for re-INVITE - test ",
    "CMceSipSession",
    "CALLED & UPDATE & 2xPENDING",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledReInvitePendingFailedL, Teardown)

EUNIT_TEST(
    "Call, INVITE forks - test ",
    "CMceSipSession",
    "CALL & INVITE forks",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallInviteForksL, Teardown)  

EUNIT_TEST(
    "Call, 200 received twice - test ",
    "CMceSipSession",
    "CALL & retransmission",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallRetransmissionL, Teardown)  

EUNIT_TEST(
    "IMS R6 to R5 Interoperatbility - test ",
    "CMceSipSession",
    "IMS Call Setup with R5 Entity",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestIMSR6toR5IOPL, Teardown)  

EUNIT_TEST(
    "Called, pracked - test ",
    "CMceSipSession",
    "CALLED & 183 reliable & PRACK",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalled100RelL, Teardown)  

EUNIT_TEST(
    "Call, send prack - test ",
    "CMceSipSession",
    "CALL & 180 reliable & PRACK",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCall100RelL, Teardown)     

EUNIT_TEST(
    "Call, Subsequest prack - test ",
    "CMceSipSession",
    "Handling Subsequent Prack",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestSubsequentPrack, Teardown)     

EUNIT_TEST(
    "Call, Early Invite - test ",
    "CMceSipSession",
    "Handling Early Invite",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestEarlyResponseToInvite, Teardown)     

EUNIT_TEST(
    "Early Invite 1 - test ",
    "CMceSipSession",
    "Handling Early Invite 1",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestEarlyResponseToInvite1, Teardown)     
    
EUNIT_TEST(
    "Early Invite 2 - test ",
    "CMceSipSession",
    "Handling Early Invite 2",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestEarlyResponseToInvite2, Teardown)     

EUNIT_TEST(
    "Provisonal Response Timeout - test ",
    "CMceSipSession",
    "Provisional Response TimeOut - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestProvisionalResponseTimeOut, Teardown)     
   
EUNIT_TEST(
    "Provisonal Response Timeout 1 - test ",
    "CMceSipSession",
    "Provisional Response Timeout 1 - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestProvisionalResponseTimeOut1, Teardown)     

EUNIT_TEST(
    "Provisonal Bad Request - test ",
    "CMceSipSession",
    "Provisional Bad Request - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestProvisionalBadRequest, Teardown)
    
EUNIT_TEST(
    "Provisonal Bad Request - test ",
    "CMceSipSession",
    "Provisional Bad Request - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestProvisionalBadRequest1, Teardown) 
    
EUNIT_TEST(
    "INVITE Bad Request - test ",
    "CMceSipSession",
    "INVITE Bad Request - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestInviteBadRequest, Teardown) 

EUNIT_TEST(
    "RE-INVITE Bad Request - test ",
    "CMceSipSession",
    "RE-INVITE Bad Request - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestReInviteBadRequest, Teardown)     

EUNIT_TEST(
    "RE-INVITE Bad Request - test 1",
    "CMceSipSession",
    "RE-INVITE Bad Request - test 1",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestReInviteBadRequest1, Teardown)     

EUNIT_TEST(
    "CALLED, PRACK in wrong state - test",
    "CMceSipSession",
    "CALLED, PRACK in wrong state - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledPrackedInWrongStateL, Teardown)

EUNIT_TEST(
    "CALLED, send 180 reliably - test",
    "CMceSipSession",
    "CALLED, send 180 reliably - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCalledReliableRingingL, Teardown)    

EUNIT_TEST(
    "CALL, CANCEL after wrong 1xx - test",
    "CMceSipSession",
    "CALL, CANCEL after wrong 1xx - test",
    "FUNCTIONALITY",
    SetupL, UT_MCEUsesCases_TestCallCancelAfterWrongReliableL, Teardown)    
     
EUNIT_END_TEST_TABLE

//  END OF FILE
