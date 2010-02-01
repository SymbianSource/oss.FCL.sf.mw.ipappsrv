/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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





#include "MCETestHelper.h"
#include "mcesipsession.h"
#include "mcesipevent.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mceservercore.h"
#include "SIPExpiresHeader.h"
#include "SIPEventHeader.h"
#include "mcereliablesender.h"
#include "sipsupportedheader.h"
#include "siprackheader.h"
#include <siprequireheader.h>
#include <siprseqheader.h>
#include <sipcontactheader.h>

_LIT8(KFmtAttribute,"a=fmtp:TBCP format parameters\r\n");
_LIT8(KFmtAttribute2,"a=fmtp:TestPlugIn format parameters\r\n");


// -----------------------------------------------------------------------------
// MCETestHelper::ClientCreateSessionL()
// -----------------------------------------------------------------------------
//
CMceSipSession* MCETestHelper::ClientCreateSessionL( CMceCsSession* aClient, 
                                                     const TDesC8& aParam1, 
                                                     const TDesC8& aParam2 )
    {
    
    TMceIds& ids = CMCETls::Storage()->iIds;
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( aParam1 );
	params->AppendL( aParam2 );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    CMCETls::Storage()->StoreL( ids, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    TInt count = aClient->Implementation().SubSessions().Count();
    
    aClient->CreateSubSessionL(ids, RMessage2() );
    
    return static_cast<CMceSipSession*>(aClient->Implementation().SubSessions()[ count ]);
    
    }
    

// -----------------------------------------------------------------------------
// MCETestHelper::ManagerReadyToReceiveL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ManagerReadyToReceiveL( CMceCsSession* aClient )
    {
    TMceIds& ids = CMCETls::Storage()->iManagerIds;
    
    CMCETls::Storage()->iIdsIndex = 1;
    aClient->ClientReadyToReceiveL( ids, RMessage2() );
    CMCETls::Storage()->iIdsIndex = 0;
    }
    

// -----------------------------------------------------------------------------
// MCETestHelper::ClientReadyToReceiveL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ClientReadyToReceiveL( CMceSipSession* aSession )
    {
    TMceIds& ids = CMCETls::Storage()->iIds;

	delete CMCETls::Storage()->iContextServer;
	delete CMCETls::Storage()->iContentServer;
	CMCETls::Storage()->iContextServer = NULL;
	CMCETls::Storage()->iContentServer = NULL;
	CMCETls::Storage()->iMessageSizes = TMceMessageBufSizes();
    
    //client ready to receive
    ids.iSessionID = reinterpret_cast<TUint32>( aSession );
    aSession->ServiceL( ids, EMceItcClientReadyToReceive, RMessage2() );

    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientReceiveLC()
// -----------------------------------------------------------------------------
//
CMceMsgBase* MCETestHelper::ClientReceiveLC( CMceSipSession* aSession, TMceIds& aIds )
    {
    CMceMsgBase* message = NULL;
    TMceIds& ids = CMCETls::Storage()->iIds;
    aIds = ids;
    
    if ( CMCETls::Storage()->iMessageSizes.iContextBufSize > 0 )
        {
        
        //client receive
        aSession->ServiceL( ids, EMceItcReceive, RMessage2() );
        
        //client code
    	RDesReadStream readStream( *CMCETls::Storage()->iContextServer );
        CleanupClosePushL( readStream );
        
        message = TMceComFactory().CreateL( (TMceItcDataType)ids.iMsgType,
                                                         readStream );
    	CleanupStack::PopAndDestroy(); // readStream
    	
        }
        
    ClientReadyToReceiveL( aSession );
	
	if ( message )
	    {
    	CleanupStack::PushL( message );
	    
	    message->PushL();
	    }
	
    return message;    
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientEstablishL()
// -----------------------------------------------------------------------------
//
CMceComSession* MCETestHelper::ClientEstablishL( CMceSipSession* aSession,
                                                 const TDesC8& aHeader,
                                                 const TDesC8& aContentType,
                                                 const TDesC8& aContent,
                                                 TBool aRequire100rel )
    {
    TMceIds& ids = CMCETls::Storage()->iIds;

    CMceComSession* clientSession = ComSessionL( ids.iSessionID, aRequire100rel );
    CleanupStack::PushL( clientSession );
    
    if ( clientSession->iSIPHeaders->MdcaCount() > 0 && aHeader.Length() == 0 )
        {
        clientSession->iSIPHeaders->Delete( 0, clientSession->iSIPHeaders->MdcaCount() );
        }
        
    if ( aHeader.Length() > 0 )
        {
        clientSession->iSIPHeaders->AppendL( aHeader );
        }
    
    MCE_TH_SET( clientSession->iSIPContentType, NULL );
    clientSession->iSIPContentType = aContentType.AllocL();
    
    CMceMsgObject<CMceComSession>* clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *clientSession, EMceItcMsgTypeSession );
    CleanupStack::PushL( clientSessionMsg );

    HBufC8* content = aContent.AllocLC();
    
    CMCETls::Storage()->StoreL( ids, *clientSessionMsg, content );
    CleanupStack::Pop( content );
    

    CleanupStack::PopAndDestroy( clientSessionMsg );

    //establish
    aSession->ServiceL( ids, EMceItcEstablishSession, RMessage2() );
    CMCETls* storage = CMCETls::Storage();
  	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
  	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
  	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
    
    CleanupStack::Pop( clientSession );
    
    return clientSession;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientTerminateL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ClientTerminateL( CMceSipSession* aSession, 
                                      const TDesC8& aHeader,
                                      const TDesC8& aContentType,
                                      const TDesC8& aContent )
    {
    TMceIds& ids = CMCETls::Storage()->iIds;
    ids.iSessionID = aSession->Body()->Id();

    
    if ( aHeader.Length() == 0 && aContentType.Length() == 0 && aContent.Length() == 0 )
        {
        ids.iMsgType = KMceNotAssigned;
        aSession->ServiceL( ids, EMceItcTerminateSession, RMessage2() );
        return;
        }
        
    HBufC8* content = aContent.AllocLC();
    HBufC8* contentType = aContentType.AllocLC();
    CDesC8Array* headers = NULL;
    if ( aHeader.Length() > 0 )
        {
        headers = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( headers );
        headers->AppendL( aHeader );
        }
    
    CMceMsgSIPData* sipData = new (ELeave) CMceMsgSIPData( SipStrConsts::EBye,
                                                           *headers, *contentType );

    CleanupStack::PushL( sipData );
    
    CMCETls::Storage()->StoreL( ids, *sipData, content );
    CleanupStack::PopAndDestroy( sipData );
    
    if ( headers )
        CleanupStack::PopAndDestroy( headers );
    
    CleanupStack::PopAndDestroy( contentType );
    CleanupStack::Pop( content );

    //reject session
    aSession->ServiceL( ids, EMceItcTerminateSession, RMessage2() );
    CMCETls* storage = CMCETls::Storage();
  	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
  	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
  	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientAcceptL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ClientAcceptL( CMceSipSession* aSession,
                                    const TDesC8& aReason, 
                                    TInt aReasonCode )
    {
    TMceIds& ids = CMCETls::Storage()->iIds;
    ids.iSessionID = aSession->Body()->Id();
    
    CDesC8Array* headers = NULL;
    HBufC8* reason = NULL;
    HBufC8* ctype = NULL;
    
    if ( aReasonCode == -1 )
        {
        ids.iMsgType = KMceNotAssigned;
        aSession->ServiceL( ids, EMceItcAcceptSession, RMessage2() );
        CMCETls* storage = CMCETls::Storage();
      	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
      	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
      	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
        return;
        }
        
    if ( aReasonCode == 0 )
        {
        aReasonCode = KMceSipOK;
        }
        
    if ( aReason.Length() > 0 )
        {
        reason = aReason.AllocLC();
        }
    else
        {
        reason = SIPStrings::StringF( SipStrConsts::EPhraseOk ).DesC().AllocLC();
        }
    
    CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, (TUint32)aReasonCode, *headers, *ctype );
    CleanupStack::PushL( reply );
    
    CMCETls::Storage()->StoreL( ids, *reply, NULL );
    CleanupStack::PopAndDestroy( reply );
    CleanupStack::PopAndDestroy( reason );

    aSession->ServiceL( ids, EMceItcAcceptSession, RMessage2() );
    CMCETls* storage = CMCETls::Storage();
  	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
  	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
  	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientDeclineL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ClientDeclineL( CMceSipSession* aSession, 
                                    const TDesC8& aHeader, 
                                    const TDesC8& aContent,
                                    const TDesC8& aContentType,
                                    const TDesC8& aReason, 
                                    TInt aReasonCode )
    {
    TMceIds& ids = CMCETls::Storage()->iIds;
    ids.iSessionID = aSession->Body()->Id();
    
    if ( aReasonCode == -1 )
        {
        ids.iMsgType = KMceNotAssigned;
        aSession->ServiceL( ids, EMceItcRejectSession, RMessage2() );
        return;
        }
        
    if ( aReasonCode == 0 )
        {
        aReasonCode = KMceSipDecline;
        }
        
        
    HBufC8* content = NULL;
    HBufC8* reason = NULL;
    HBufC8* ctype = NULL;
    CDesC8Array* headers = NULL;

    if ( aContent.Length() > 0 )
        {
        content = aContent.AllocLC();
        }
    if ( aReason.Length() > 0 )
        {
        reason = aReason.AllocLC();
        }
    else
        {
        reason = SIPStrings::StringF( SipStrConsts::EPhraseDecline ).DesC().AllocLC();
        }
    
    if ( aHeader.Length() > 0 )
        {
        headers = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( headers );
        headers->AppendL( aHeader );
        }
    
    if ( aContentType.Length() > 0 )
        {
        ctype = aContentType.AllocLC();
        }
    
    CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, (TUint32)aReasonCode, *headers, *ctype );
    CleanupStack::PushL( reply );
    
    CMCETls::Storage()->StoreL( ids, *reply, content );
    CleanupStack::PopAndDestroy( reply );
    if ( ctype )
        CleanupStack::PopAndDestroy( ctype );
    
    if ( headers )
        CleanupStack::PopAndDestroy( headers );
    
    if ( reason )
        CleanupStack::PopAndDestroy( reason );
    
    if ( content )
        CleanupStack::Pop( content );

    //reject session
    aSession->ServiceL( ids, EMceItcRejectSession, RMessage2() );
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientRingL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ClientRingL( CMceSipSession* aSession, 
                                    const TDesC8& aHeader, 
                                    const TDesC8& aContent,
                                    const TDesC8& aContentType )
    {
    TMceIds& ids = CMCETls::Storage()->iIds;
    ids.iSessionID = aSession->Body()->Id();


    if ( aHeader.Length() == 0 && aContentType.Length() == 0 && aContent.Length() == 0 )
        {
        ids.iMsgType = KMceNotAssigned;
        aSession->ServiceL( ids, EMceItcRing, RMessage2() );
        CMCETls* storage = CMCETls::Storage();
      	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
      	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
      	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
        return;
        }
    
    TUint32 reasonCode = KMceSipRinging;
    HBufC8* content = NULL;
    HBufC8* reason = NULL;
    HBufC8* ctype = NULL;
    CDesC8Array* headers = NULL;

    content = aContent.AllocLC();
    reason = SIPStrings::StringF( SipStrConsts::EPhraseRinging ).DesC().AllocLC();
    
    if ( aHeader.Length() > 0 )
        {
        headers = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( headers );
        headers->AppendL( aHeader );
        }
    
    ctype = aContentType.AllocLC();
    
    CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, reasonCode, *headers, *ctype );
    CleanupStack::PushL( reply );
    
    CMCETls::Storage()->StoreL( ids, *reply, content );
    CleanupStack::PopAndDestroy( reply );
    CleanupStack::PopAndDestroy( ctype );
    
    if ( headers )
        CleanupStack::PopAndDestroy( headers );
    
    CleanupStack::PopAndDestroy( reason );
    CleanupStack::Pop( content );

    aSession->ServiceL( ids, EMceItcRing, RMessage2() );
    CMCETls* storage = CMCETls::Storage();
  	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
  	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
  	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientCancelL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ClientCancelL( CMceSipSession* aSession )
    {
    TMceIds& ids = CMCETls::Storage()->iIds;
    ids.iSessionID = aSession->Body()->Id();
    ids.iMsgType = KMceNotAssigned;
    aSession->ServiceL( ids, EMceItcCancel, RMessage2() );
    CMCETls* storage = CMCETls::Storage();
  	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
  	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
  	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
    }
    
// -----------------------------------------------------------------------------
// MCETestHelper::ClientIncomingSessionL()
// -----------------------------------------------------------------------------
//
CMceSipSession* MCETestHelper::ClientIncomingSessionL( CMceCsSession* aClient, 
                                                       TBool aInvalid,
                                                       TBool aSupported100rel )
    {
    CMceSipSession* session = NULL;
    TUint32 iapId = 1;
    
    //invite
    CSIPServerTransaction* invite = NULL;
    
    if ( !aInvalid )
        {
        RPointerArray<CSIPHeaderBase> hdrs;
        CSIPHeaderBase::PushLC( &hdrs );
        
        if ( aSupported100rel )
            {
            RStringF supp100rel = SIPStrings::Pool().OpenFStringL( KMceSip100Rel );
            CleanupClosePushL( supp100rel );
            CSIPSupportedHeader* supported = 
                CSIPSupportedHeader::NewLC( supp100rel );
            hdrs.AppendL( supported );
            CleanupStack::Pop( supported );
            CleanupStack::PopAndDestroy(); // supp100rel
            }
        invite = ServerTransactionL( SipStrConsts::EInvite, hdrs, ETrue );
        CleanupStack::Pop( &hdrs );
        }
    else
        {
        invite = ServerTransactionBadSDPL( SipStrConsts::EInvite );
        }
    
  	aClient->ServerCore().Manager().IncomingRequest( iapId, invite );
  	CMCETls* storage = CMCETls::Storage();
  	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
  	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
  	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
  	storage->ExecuteCallback();
  	MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
  	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
  	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )

    TInt count = aClient->Implementation().SubSessions().Count();
    if ( count > 0 )
        {
        session = static_cast<CMceSipSession*>(aClient->Implementation().SubSessions()[ count - 1 ]);
        }
    if ( session )
        {
        
        TMceIds ids = CMCETls::Storage()->iManagerIds;
        //receive
        aClient->ReceiveL( ids , RMessage2() );
        //client code
    	RDesReadStream readStream( *CMCETls::Storage()->iContextServer );
        CleanupClosePushL( readStream );
        
        CMceMsgObject<CMceComSession>* message = static_cast<CMceMsgObject<CMceComSession>*>
            (TMceComFactory().CreateL( (TMceItcDataType)ids.iMsgType,
                                                            readStream ) );
    	CleanupStack::PopAndDestroy(); // readStream
        
        CleanupStack::PushL( message );
        message->PushL();
        CleanupStack::PopAndDestroy( message );
        
        CMCETls::Storage()->iIds = TMceIds();
        CMCETls::Storage()->iIds.iAppUID = CMCETls::Storage()->iManagerIds.iAppUID;
        CMCETls::Storage()->iIds.iManagerType = CMCETls::Storage()->iManagerIds.iManagerType;
        CMCETls::Storage()->iIds.iDialogType = CMCETls::Storage()->iManagerIds.iDialogType;
        CMCETls::Storage()->iIds.iDialogID = CMCETls::Storage()->iManagerIds.iDialogID;
        CMCETls::Storage()->iIds.iProfileID = CMCETls::Storage()->iManagerIds.iProfileID;
        
        ManagerReadyToReceiveL( aClient );
        }
        
    return session;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientIncomingEventL()
// -----------------------------------------------------------------------------
//
CMceSipEvent* MCETestHelper::ClientIncomingEventL( CMceCsSession* aClient, 
                                                   TInt aMethod )
    {
    CMceSipEvent* event = NULL;
    TUint32 iapId = 1;
    TUint32 profileId = 1;
    
    RPointerArray<CSIPHeaderBase> sipHdrs;
    CSIPHeaderBase::PushLC( &sipHdrs );
    
    if ( aMethod == SipStrConsts::ESubscribe )
        {
        CSIPExpiresHeader* ex = new( ELeave ) CSIPExpiresHeader( 180 );
        CleanupStack::PushL( ex );
        sipHdrs.AppendL( ex );
        CleanupStack::Pop( ex );
        
        CSIPEventHeader* eve = CSIPEventHeader::DecodeL( KEvent );
        CleanupStack::PushL( eve );
        sipHdrs.AppendL( eve );
        CleanupStack::Pop( eve );
        }
    
    CSIPServerTransaction* sta = NULL;
    
    
    sta = ServerTransactionL( aMethod, sipHdrs, ETrue );
    CleanupStack::Pop( &sipHdrs );
    CMceSipManager& manager = aClient->ServerCore().Manager();
    CSIPProfile& profile = manager.ProfileL( profileId );
    
    CMceSipConnection& connection = manager.ConnectionL( profile );
    
    manager.CreateSubSessionL( sta, *aClient, connection );
  	
  	TInt count = aClient->Implementation().SubSessions().Count();
    if ( count > 0 )
        {
        event = static_cast<CMceSipEvent*>(aClient->Implementation().SubSessions()[ count - 1 ]);
        }
    
    return event;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientUpdateSessionL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ClientUpdateSessionL( CMceSipSession* aSession,
                                          CMceComSession& aUpdate,
                                          TUint32 aTimeout,
                                          const TDesC8& aHeader, 
                                          const TDesC8& aContentType,
                                          const TDesC8& aContent,
                                          const TDesC8& aContentHeader ) 
    {
    TMceIds& ids = CMCETls::Storage()->iIds;

    aUpdate.iTimeout = aTimeout;
    MCE_DELETE( aUpdate.iSIPHeaders );
    MCE_DELETE( aUpdate.iSIPContentType );
    MCE_DELETE( aUpdate.iSIPContentHeaders );
    
    if ( aHeader.Length() > 0 )
        {
        aUpdate.iSIPHeaders = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
        aUpdate.iSIPHeaders->AppendL( aHeader );
        }
    if ( aContentHeader.Length() > 0 )
        {
        aUpdate.iSIPContentHeaders = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
        aUpdate.iSIPContentHeaders->AppendL( aContentHeader );
        }
    aUpdate.iSIPContentType = aContentType.AllocL();
     
    
    CMceMsgObject<CMceComSession>* clientSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( aUpdate, EMceItcMsgTypeSession );
    CleanupStack::PushL( clientSessionMsg );

    HBufC8* content = aContent.AllocLC();
    
    ids.iSessionID = aSession->Body()->Id();
    CMCETls::Storage()->StoreL( ids, *clientSessionMsg, content );

    CleanupStack::Pop( content );
    CleanupStack::PopAndDestroy( clientSessionMsg );
    
    //update
    aSession->ServiceL( ids, EMceItcUpdate, RMessage2() );
    
    CMCETls* storage = CMCETls::Storage();
    MCE_CHECK_MEMORY_LEAVE( storage->iMediaManagerUpdateStatus >= KErrNone &&
         storage->iSipSentResponse && 
  	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
  	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
    }
    
// -----------------------------------------------------------------------------
// MCETestHelper::ClientGetSessionL()
// -----------------------------------------------------------------------------
//
CMceComSession* MCETestHelper::ClientGetSessionL( CMceSipSession* aSession )
    {
    TMceIds& ids = CMCETls::Storage()->iIds;
    ids.iSessionID = aSession->Body()->Id();
    ids.iMsgType = KMceNotAssigned;
    aSession->ServiceL( ids, EMceItcGetSession, RMessage2() );

    CMceComSession* session = NULL;
    CMceMsgObject<CMceComSession>* clientSessionMsg = 
            new (ELeave) CMceMsgObject<CMceComSession>();
    CleanupStack::PushL( clientSessionMsg );
    
    clientSessionMsg->DecodeL( *CMCETls::Storage()->iContextServer );
    session = clientSessionMsg->Object();
    
    CleanupStack::PopAndDestroy( clientSessionMsg );
    
    return session;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::MediaManagerCallBack()
// -----------------------------------------------------------------------------
//
void MCETestHelper::MediaManagerCallBack( CMceSipSession* aSession, 
                                          TMceMediaEventCode aEvent,
                                          TInt aError )
    {
    switch ( aEvent )
        {
        case EMceMediaUpdated:
            {
            aSession->Updated( *aSession->Body() );
            MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )
            break;
            }
        case EMceMediaSessionStopped:
            {
            aSession->SessionClosed( *aSession->Body() );
            MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )
            break;
            }
        case EMceMediaError:
            {
            TMceMccComEvent event( KMccStreamError );
            event.iError = aError;
            aSession->MediaError( event );
            MCE_CHECK_MEMORY_LEAVE( CMCETls::Storage()->iCompletionCode == KErrNoMemory )
            break;
            }
        default:
            {
            break;
            }
        }
    }
    
// -----------------------------------------------------------------------------
// MCETestHelper::MediaManagerSinkCallBack()
// -----------------------------------------------------------------------------
//
void MCETestHelper::MediaManagerSinkCallBack( CMceSipSession* aSession, 
                                              TMceItcFunctions aEventTo,
                                              TInt aStream,
                                              TInt aSink )
    {
    switch( aEventTo )
        {
        case EMceItcDisable:
            {
            TMceMccComEvent event( KMccStreamPaused );
            SinkEvent( event, aSession, aStream, aSink  );
            aSession->EventReceived( event );
            break;
            }
        case EMceItcEnable:
            {
            TMceMccComEvent event( KMccStreamResumed );
            SinkEvent( event, aSession, aStream, aSink );
            aSession->EventReceived( event );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// MCETestHelper::MediaManagerSourceCallBack()
// -----------------------------------------------------------------------------
//
void MCETestHelper::MediaManagerSourceCallBack( CMceSipSession* aSession, 
                                              TMceItcFunctions aEventTo,
                                              TInt aStream )
    {
    switch( aEventTo )
        {
        case EMceItcDisable:
            {
            TMceMccComEvent event( KMccStreamPaused );
            SourceEvent( event, aSession, aStream );
            aSession->EventReceived( event );
            break;
            }
        case EMceItcEnable:
            {
            TMceMccComEvent event( KMccStreamResumed );
            SourceEvent( event, aSession, aStream );
            aSession->EventReceived( event );
            break;
            }
        case EMceItcEnableInactivityTimer:
            {
            TMceMccComEvent event( KMccInactivityEvent );
            SourceEvent( event, aSession, aStream );
            aSession->EventReceived( event );
            break;
            }
        }    
    }

// -----------------------------------------------------------------------------
// MCETestHelper::Sink()
// -----------------------------------------------------------------------------
//
CMceComMediaSink*
MCETestHelper::Sink( CMceSipSession* aSession, TInt aStream, TInt aSink )
    {

    CMceComMediaSink* sink = NULL;
    
    if ( aStream >= 0 )
        {
        sink = aSession->Body()->Streams()[ aStream ]->Sinks()[ aSink ];
        }
    else
        {
        sink = aSession->Body()->Streams()[ 0 ]->BoundStreamL().Sinks()[ aSink ];
        }
        
    return sink;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::Source()
// -----------------------------------------------------------------------------
//
CMceComMediaSource*
MCETestHelper::Source( CMceSipSession* aSession, TInt aStream )
    {
    CMceComMediaSource* source = NULL;
    
    if ( aStream >= 0 )
        {
        source = aSession->Body()->Streams()[ aStream ]->Source();
        }
    else
        {
        source = aSession->Body()->Streams()[ 0 ]->BoundStreamL().Source();
        }
        
    return source;    
    }

// -----------------------------------------------------------------------------
// MCETestHelper::SinkEvent()
// -----------------------------------------------------------------------------
//
void MCETestHelper::SinkEvent( TMceMccComEvent& aEvent,
							   CMceSipSession* aSession,
							   TInt aStream,
							   TInt aSink )
    {
    CMceComMediaSink* sink = Sink( aSession, aStream, aSink );

    aEvent.iStream = sink->MediaStream();
    aEvent.iSink = sink;
    switch( aEvent.iEvent )
        {
        case KMccStreamPaused:
            {
            aEvent.iItcEvent = EMceItcStateChanged;
            aEvent.iItcData = 0;
            break;
            }
        case KMccStreamResumed:
            {
            aEvent.iItcEvent = EMceItcStateChanged;
            aEvent.iItcData = 1;
            break;
            }
        case KMccInactivityEvent:
            {
            aEvent.iItcEvent = EMceItcInactivityTimeout;
            break;
            }
        case KMccRtcpReceived:
            {
            if ( aEvent.iEventSubType == KRtcpRrPacket )
                {
                aEvent.iItcEvent = EMceItcRRReceived;
                }
            else if ( aEvent.iEventSubType == KRtcpSrPacket )
                {
                aEvent.iItcEvent = EMceItcSRReceived;
                }
            else if ( aEvent.iEventSubType == KRtcpPacketUndefined )
                {
                aEvent.iItcEvent = EMceItcFCMsgReceived;
                }
            break;
            }
        default:
            {
            aEvent.iItcEvent = EMceItcInProgress;
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// MCETestHelper::SourceEvent()
// -----------------------------------------------------------------------------
//
void MCETestHelper::SourceEvent( TMceMccComEvent& aEvent,
								 CMceSipSession* aSession,
								 TInt aStream )
    {
    CMceComMediaSource* source = Source( aSession, aStream );

    aEvent.iStream = source->MediaStream();
    aEvent.iSource = source;
    switch( aEvent.iEvent )
        {
        case KMccStreamPaused:
            {
            aEvent.iItcEvent = EMceItcStateChanged;
            aEvent.iItcData = 0;
            break;
            }
        case KMccStreamResumed:
            {
            aEvent.iItcEvent = EMceItcStateChanged;
            aEvent.iItcData = 1;
            break;
            }
        case KMccInactivityEvent:
            {
            aEvent.iItcEvent = EMceItcInactivityTimeout;
            break;
            }
        case KMccRtcpReceived:
            {
            if ( aEvent.iEventSubType == KRtcpRrPacket )
                {
                aEvent.iItcEvent = EMceItcRRReceived;
                }
            else if ( aEvent.iEventSubType == KRtcpSrPacket )
                {
                aEvent.iItcEvent = EMceItcSRReceived;
                }
            else if ( aEvent.iEventSubType == KRtcpPacketUndefined )
                {
                aEvent.iItcEvent = EMceItcFCMsgReceived;
                }
            break;
            }
        default:
            {
            aEvent.iItcEvent = EMceItcInProgress;
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ResponseL()
// -----------------------------------------------------------------------------
//
CSIPClientTransaction& MCETestHelper::ResponseL( CMceSipSession* aSession,
                               TUint aReasonCode, 
                               TInt aReasonPhrase, 
                               TInt aTrx,
                               TBool aSDPContent,
                               TBool aRequire100rel,
                               TUint aRSeqV,
                               TBool aExpectOkStateChange )
    {    
    CSIPClientTransaction* trx = aSession->PendingTransactions()[ aTrx ];

    SetResponseL( *trx, aReasonCode, aReasonPhrase, aSDPContent, aRequire100rel, aRSeqV );
    
    if ( MceSip::ResponseType( *trx ) == E1XX )
        {
        trx->iState = CSIPTransactionBase::EProceeding;
        }
    else
        {
        if ( MceSip::ResponseType( *trx ) == E2XX )
            {
            aSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed;
            }
        else
            {
            aSession->Dialog()->Dialog().iState = CSIPDialog::ETerminated;
            }
        trx->iState = CSIPTransactionBase::ETerminated;
        }

	aSession->DialogResponseReceived( *trx );
	
	CMCETls* storage = CMCETls::Storage();
	if ( aExpectOkStateChange &&
	     ( aReasonCode == KMceSipOK ||
	   	   aReasonCode == KMceSipRinging ||
	   	   aReasonCode == KMceSipTrying ) )
	    {
        MCE_CHECK_MEMORY_LEAVE( storage->iMediaManagerUpdateStatus >= KErrNone &&
             storage->iMediaManagerAction == CMCETls::ECloseSession )
	    }

    return *trx;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ForkedResponseL()
// -----------------------------------------------------------------------------
//
CSIPClientTransaction& MCETestHelper::ForkedResponseL( 
                               CMceSipSession* aSession,
                               CSIPInviteDialogAssoc* dialog,
                               TUint aReasonCode, 
                               TInt aReasonPhrase, 
                               TInt aTrx,
                               TBool aSDPContent )
    {
    CleanupStack::PushL( dialog );
    CSIPClientTransaction* trx = aSession->PendingTransactions()[ aTrx ];

    SetResponseL( *trx, aReasonCode, aReasonPhrase, aSDPContent );
    
    if ( MceSip::ResponseType( *trx ) == E1XX )
        {
        trx->iState = CSIPTransactionBase::EProceeding;
        }
    else
        {
        if ( MceSip::ResponseType( *trx ) == E2XX )
            {
            aSession->Dialog()->Dialog().iState = CSIPDialog::EConfirmed;
            }
        else
            {
            aSession->Dialog()->Dialog().iState = CSIPDialog::ETerminated;
            }
        trx->iState = CSIPTransactionBase::ETerminated;
        }

	aSession->InviteForkedL( *trx, dialog );
	CleanupStack::Pop( dialog );

    CMCETls* storage = CMCETls::Storage();
	if ( aReasonCode == KMceSipOK || aReasonCode == KMceSipRinging || aReasonCode == KMceSipTrying )
	    {
        MCE_CHECK_MEMORY_LEAVE( storage->iMediaManagerUpdateStatus >= KErrNone &&
             storage->iMediaManagerAction == CMCETls::ECloseSession )
	    }
    return *trx;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::RequestL()
// -----------------------------------------------------------------------------
//
CSIPServerTransaction& MCETestHelper::RequestL( CMceSipSession* aSession, 
                                                TInt aRequest,
                                                TInt aSDPContent,
                                                TBool aExpectOkStateChange )
    {
    CSIPServerTransaction* trx = NULL;
    RPointerArray<CSIPHeaderBase> hdrs;
    CSIPHeaderBase::PushLC( &hdrs );
    if ( aRequest == SipStrConsts::EPrack )
        {
        CSIPRAckHeader* rack = CSIPRAckHeader::NewLC( aSession->ReliableSender().iSequenceNumber, 
                                                      KInviteCSeq, 
                                                      SIPStrings::StringF( SipStrConsts::EInvite ) );
        hdrs.AppendL( rack );
        CleanupStack::Pop( rack );                    
        }
        
    if ( aSDPContent == 0 )
        {
        trx = MCETestHelper::ServerTransactionL( aRequest, hdrs );
        }
    else if ( aSDPContent == 1 )
        {
        trx = MCETestHelper::ServerTransactionL( aRequest, hdrs, ETrue );
        }
    else
        {
        trx = MCETestHelper::ServerTransaction2L( aRequest );
        }
        
    if ( aRequest == SipStrConsts::EAck )
        {
        trx->iState = CSIPTransactionBase::ETerminated;
        }
    CleanupStack::Pop( &hdrs );    

	TBool consumed = aSession->DialogRequestReceived( trx, aSession->Dialog()->Dialog() );
	CleanupStack::PushL( trx );
    User::LeaveIfError( consumed ? KErrNone : KErrNoMemory );
    CleanupStack::Pop( trx );

 	if ( aExpectOkStateChange )
        {
        CMCETls* storage = CMCETls::Storage();
        MCE_CHECK_MEMORY_LEAVE( storage->iSipSentResponse && 
      	   ( storage->iSipSentResponse->StatusCode() == KMceSipServerInternalError || 
      	     storage->iSipSentResponse->StatusCode() == KMceSipDecline ) )
        }

    return *trx;    
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ServerTransactionL()
// -----------------------------------------------------------------------------
//
CSIPServerTransaction* MCETestHelper::ServerTransactionL( TInt aMethod, 
                                                          TBool aContent,
                                                          TBool aToAndFromHeaders )
    {
    CSIPServerTransaction* srvtransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( aMethod )); 
    CleanupStack::PushL( srvtransaction );
    
  	CUri8* uri = CUri8::NewLC(); //OK
    MCE_SET_URIL( uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );

    if ( aToAndFromHeaders )
        {
        CSIPToHeader* toheader = CSIPToHeader::DecodeL( KSIPProfileDefAOR );
        CleanupStack::PushL( toheader );
        CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
        CleanupStack::PushL( fromheader );
        
        requestelements->SetFromHeaderL(fromheader); 
        CleanupStack::Pop( fromheader );
        requestelements->SetToHeaderL(toheader); 
        CleanupStack::Pop( toheader );
        }
    
    CSIPMessageElements& msgelements = requestelements->MessageElements();
    
    if ( aContent )
        {
        SetMessageElementsL( msgelements );
        }
    
    requestelements->SetMethodL( SIPStrings::StringF( aMethod ));
    
    srvtransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    CleanupStack::Pop( srvtransaction );

    return srvtransaction;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ServerTransaction2L()
// -----------------------------------------------------------------------------
//
CSIPServerTransaction* MCETestHelper::ServerTransaction2L( TInt aMethod )
    {
    CSIPServerTransaction* srvtransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( aMethod )); 
    CleanupStack::PushL( srvtransaction );
    
  	CUri8* uri = CUri8::NewLC(); //OK
    MCE_SET_URIL( uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );

    CSIPToHeader* toheader = CSIPToHeader::DecodeL( KSIPProfileDefAOR );
    CleanupStack::PushL( toheader );
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );
    
    requestelements->SetFromHeaderL(fromheader); 
    CleanupStack::Pop( fromheader );
    requestelements->SetToHeaderL(toheader); 
    CleanupStack::Pop( toheader );
    
    CSIPMessageElements& msgelements = requestelements->MessageElements();
    
    SetMessageElementsL( msgelements, 2 );
    
    requestelements->SetMethodL( SIPStrings::StringF( aMethod ));
    
    srvtransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    CleanupStack::Pop( srvtransaction );

    return srvtransaction;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ServerTransactionBadSDPL()
// -----------------------------------------------------------------------------
//
CSIPServerTransaction* MCETestHelper::ServerTransactionBadSDPL( TInt aMethod )
    {
    CSIPServerTransaction* srvtransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( aMethod )); 
    CleanupStack::PushL( srvtransaction );
    
  	CUri8* uri = CUri8::NewLC(); //OK
    MCE_SET_URIL( uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );

    CSIPToHeader* toheader = CSIPToHeader::DecodeL( KSIPProfileDefAOR );
    CleanupStack::PushL( toheader );
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );
    
    requestelements->SetFromHeaderL(fromheader); 
    CleanupStack::Pop( fromheader );
    requestelements->SetToHeaderL(toheader); 
    CleanupStack::Pop( toheader );
    
    CSIPMessageElements& msgelements = requestelements->MessageElements();
    
    SetMessageElementsL( msgelements, 3 );
    
    requestelements->SetMethodL( SIPStrings::StringF( aMethod ));
    
    srvtransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    CleanupStack::Pop( srvtransaction );

    return srvtransaction;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ServerTransactionL()
// -----------------------------------------------------------------------------
//
CSIPServerTransaction* MCETestHelper::ServerTransactionL( TInt aMethod, 
                                                          RPointerArray<CSIPHeaderBase>& aSipHeaders, 
                                                          TBool aContent )
    {
    CSIPServerTransaction* srvtransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( aMethod )); 
    CleanupStack::PushL( srvtransaction );
    
  	CUri8* uri = CUri8::NewLC(); //OK
    MCE_SET_URIL( uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );

    CSIPToHeader* toheader = CSIPToHeader::DecodeL( KSIPProfileDefAOR );
    CleanupStack::PushL( toheader );
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL(_L8("User <user2@host>")); 
    CleanupStack::PushL( fromheader );
    
    requestelements->SetFromHeaderL(fromheader); 
    CleanupStack::Pop( fromheader );
    requestelements->SetToHeaderL(toheader); 
    CleanupStack::Pop( toheader );
    
    CSIPMessageElements& msgelements = requestelements->MessageElements();
    
    if ( aContent )
        {
        SetMessageElementsL( msgelements );
        }
    
    requestelements->SetMethodL( SIPStrings::StringF( aMethod ));
    msgelements.SetUserHeadersL( aSipHeaders );

    srvtransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    CleanupStack::Pop( srvtransaction );

    return srvtransaction;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientTransactionLC()
// -----------------------------------------------------------------------------
//
CSIPClientTransaction* MCETestHelper::ClientTransactionLC( TInt aToMethod, 
                                                           TUint aResponse, 
                                                           TInt aPhrase,
                                                           TBool aContent )
    {
    CSIPResponseElements* response = NULL;
    
    CSIPClientTransaction* clitransaction = 
      CSIPClientTransaction::NewL( SIPStrings::StringF( aToMethod ));
    CleanupStack::PushL( clitransaction );

    if ( aResponse > 0 )
        {
        response = SetResponseL( *clitransaction, aResponse, aPhrase );
        
        if ( aContent )
            {
            CSIPMessageElements& msgelements = response->MessageElements();
            SetMessageElementsL( msgelements );
            }
        if ( aResponse >= KMceSipOK )
            {
            clitransaction->SetState( CSIPTransactionBase::ETerminated );
            }
        }
        
    return clitransaction;    
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ClientTransactionLC()
// -----------------------------------------------------------------------------
//
CSIPClientTransaction* MCETestHelper::ClientTransactionLC( TInt aToMethod, 
                                                           RPointerArray<CSIPHeaderBase>& aSipHeaders, 
                                                           TUint aResponse, 
                                                           TInt aPhrase,
                                                           TBool aContent )
    {
    CSIPResponseElements* response = NULL;
    
    CSIPClientTransaction* clitransaction = 
      CSIPClientTransaction::NewL( SIPStrings::StringF( aToMethod ));
    CleanupStack::PushL( clitransaction );

    if ( aResponse > 0 )
        {
        response = SetResponseL( *clitransaction, aResponse, aPhrase );
        CSIPMessageElements& msgelements = response->MessageElements();
        
        if ( aContent )
            {
            CSIPMessageElements& msgelements = response->MessageElements();
            SetMessageElementsL( msgelements );
            }

        msgelements.SetUserHeadersL( aSipHeaders );    
        }
        
    return clitransaction;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::SetResponseL()
// -----------------------------------------------------------------------------
//
CSIPResponseElements* MCETestHelper::SetResponseL( CSIPClientTransaction& aTrx, 
                                                   TUint aResponse, 
                                                   TInt aPhrase, 
                                                   TBool aContent,
                                                   TBool aRequire100rel, 
                                                   TUint aRSeqValue )                               
    {
    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( aResponse, 
                                     SIPStrings::StringF( aPhrase ) );
    
    CSIPMessageElements& msgelements = response->MessageElements();
    
    CSIPFromHeader* from = CSIPFromHeader::DecodeL( KFrom );
    CleanupStack::PushL( from );
    msgelements.SetFromL( from );
    CleanupStack::Pop( from );
    
    CSIPToHeader* to = CSIPToHeader::DecodeL( KTo );
    CleanupStack::PushL( to );
    msgelements.SetToL( to );
    CleanupStack::Pop( to );
    
    RPointerArray<CSIPHeaderBase> usrhdrs;
    CSIPHeaderBase::PushLC( &usrhdrs );
    CSIPContactHeader* contact = MceSip::ToContactHeaderL( KContactValue );
    CleanupStack::PushL( contact );
    usrhdrs.AppendL( contact );
    CleanupStack::Pop( contact );
    msgelements.SetUserHeadersL( usrhdrs );
    CleanupStack::Pop( &usrhdrs );
    
    if ( aResponse == KMceSipSessionProgress || aRequire100rel )
        {
        // Require: 100rel
        RPointerArray<CSIPHeaderBase> hdrs;
        CSIPHeaderBase::PushLC( &hdrs );
        RStringF require = SIPStrings::Pool().OpenFStringL( KMceSip100Rel );
        CleanupClosePushL( require );
        CSIPRequireHeader* require100rel = CSIPRequireHeader::NewLC( require );
        hdrs.AppendL( require100rel );
        CleanupStack::Pop( require100rel );
        CleanupStack::PopAndDestroy(); // require
        
        // RSeq
        CSIPRSeqHeader* rseq = new( ELeave ) CSIPRSeqHeader( aRSeqValue );
        CleanupStack::PushL( rseq );
        hdrs.AppendL( rseq );
        CleanupStack::Pop( rseq );
        
        msgelements.SetUserHeadersL( hdrs );
        CleanupStack::Pop( &hdrs );
        }
    else if ( aResponse == KMceSipExtensionRequired )
    	{
    	// This branch is special for UT_MCEUsesCases::UT_MCEUsesCases_TestIMSR6toR5IOPL()
    	//
        // Require: precondition
    	// if response is the kind of KMceSipExtensionRequired, then, response must have
    	// "Require: precondition" field.
        RPointerArray<CSIPHeaderBase> hdrs;
        CSIPHeaderBase::PushLC( &hdrs );
        RStringF require = SIPStrings::Pool().OpenFStringL( KMceSipPrecondition );
        CleanupClosePushL( require );
        CSIPRequireHeader* requireProcond = CSIPRequireHeader::NewLC( require );
        hdrs.AppendL( requireProcond );
        CleanupStack::Pop( requireProcond );
        CleanupStack::PopAndDestroy(); // require
        
        msgelements.SetUserHeadersL( hdrs );
        CleanupStack::Pop( &hdrs );
    	}
    
    if ( aContent )
        {
        SetMessageElementsL( msgelements );
        }
        
    aTrx.SetResponseElements( response );
    CleanupStack::Pop( response );
    
    return response;    
    }

// -----------------------------------------------------------------------------
// MCETestHelper::SetResponse2L()
// -----------------------------------------------------------------------------
//
CSIPResponseElements* MCETestHelper::SetResponse2L( CSIPClientTransaction& aTrx,
													TUint aResponse,
													TInt aPhrase )
    {
    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( aResponse, 
                                     SIPStrings::StringF( aPhrase ) );
    
    CSIPMessageElements& msgelements = response->MessageElements();
    
    SetMessageElementsL( msgelements, 2 );
        
    aTrx.SetResponseElements( response );
    CleanupStack::Pop( response );
    
    return response;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::SetResponseBADL()
// -----------------------------------------------------------------------------
//
CSIPResponseElements*
MCETestHelper::SetResponseBADL( CSIPClientTransaction& aTrx,
								TUint aResponse,
								TInt aPhrase )
    {
    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( aResponse, 
                                     SIPStrings::StringF( aPhrase ) );
    
    CSIPMessageElements& msgelements = response->MessageElements();
    
    SetMessageElementsL( msgelements, 3 );
        
    aTrx.SetResponseElements( response );
    CleanupStack::Pop( response );
    
    return response;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::SetMessageElementsL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::SetMessageElementsL( CSIPMessageElements& aElements,
										 TInt sdpId )
    {
    CSIPContentTypeHeader* contenttype = 
        CSIPContentTypeHeader::NewL( _L8("application"), _L8("sdp") ); //OK
    CleanupStack::PushL( contenttype );
        
    HBufC8* content = NULL;
    if ( sdpId == 1 )
        {
        content = KSDPMessage_OK1().AllocLC();
        }
    else if ( sdpId == 2 )
        {
        content = KSDPMessage_OK2().AllocLC();
        }
    else
        {
        content = KSDPMessage_BAD().AllocLC();
        }
        
    aElements.SetContentL( content, contenttype );
    CleanupStack::Pop( content );
    CleanupStack::Pop( contenttype );
    }

// -----------------------------------------------------------------------------
// MCETestHelper::AppendContactToUserHeaders
// -----------------------------------------------------------------------------
//
void MCETestHelper::AppendContactToUserHeaders(
	RPointerArray<CSIPHeaderBase>& aSipHeaders, const TDesC8& aContact )
    {
    CSIPContactHeader* contact = MceSip::ToContactHeaderL( aContact );
    CleanupStack::PushL( contact );
    aSipHeaders.AppendL( contact );
    CleanupStack::Pop( contact );
    }

// -----------------------------------------------------------------------------
// MCETestHelper::OfferL()
// -----------------------------------------------------------------------------
//
CSdpDocument* MCETestHelper::OfferL()
    {
    return CSdpDocument::DecodeL( KSDPMessage_OK1 );
    }    
    
// -----------------------------------------------------------------------------
// MCETestHelper::ComSessionL()
// -----------------------------------------------------------------------------
//
CMceComSession* MCETestHelper::ComSessionL( TUint32 aSessionId, 
                                            TBool aRequire100rel )
    {
    _LIT8( originator, "sip:originator@foo.bar" );
    _LIT8( recipient, "sip:recipient@foo.bar" );
    _LIT8( contentType, "contenttype" );
    _LIT8( header, "header" );
    _LIT8( amr, "AMR" );
    
    
    TBuf8<KMceMaxSdpNameLength> amrName;
    amrName.Append( amr );
    
    CMceComSession* clientSession = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( clientSession );
    
    
    CMceComAudioStream* inStream = CMceComAudioStream::NewLC();
    CMceComAudioStream* outStream = CMceComAudioStream::NewLC();
    inStream->BindL( outStream );
    CleanupStack::Pop( outStream );
    
    CMceComAMRCodec* codec1 = CMceComAMRCodec::NewLC( amrName );
    inStream->AddCodecL( codec1 );
    CleanupStack::Pop( codec1 );

    CMceComAMRCodec* codec2 = CMceComAMRCodec::NewLC( amrName );
    outStream->AddCodecL( codec2 );
    CleanupStack::Pop( codec2 );
        
    CMceComRtpSource* rtpSource = CMceComRtpSource::NewLC();
    inStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    
    CMceComSpeakerSink* speaker = CMceComSpeakerSink::NewLC();
    inStream->AddSinkL( speaker );
    CleanupStack::Pop( speaker );
    

    CMceComRtpSink* rtpSink = CMceComRtpSink::NewLC();
    outStream->AddSinkL( rtpSink );
    CleanupStack::Pop( rtpSink );
    
    CMceComMicSource* mic = CMceComMicSource::NewLC();
    outStream->SetSourceL( mic );
    CleanupStack::Pop( mic );
    
    clientSession->AddStreamL( inStream );
    CleanupStack::Pop( inStream );
    clientSession->InitializeL();
    
    //set ids
    TUid appUid = TUid::Uid( 10 );
    clientSession->iID = aSessionId;
    inStream->iID   = TMceMediaId(appUid,1);
    outStream->iID  = TMceMediaId(appUid,2);
    rtpSource->iID  = TMceMediaId(appUid,3);
    speaker->iID    = TMceMediaId(appUid,4);
    rtpSink->iID    = TMceMediaId(appUid,5);
    mic->iID        = TMceMediaId(appUid,6);
    
    
    //set session
    clientSession->iIsConnectionActive = ETrue;
    clientSession->iDialogId = 434;
    clientSession->iIapId = 4;
    clientSession->iMccID = 101;
    clientSession->iMinSE = 200;
    clientSession->iTimeout = 30;
    clientSession->iLocalIpAddress = TInetAddr( 5005 );
    clientSession->iRemoteIpAddress = TInetAddr( 5010 );
    clientSession->SetOriginatorL( originator );
    clientSession->SetRecipientL( recipient );
    MCE_TH_SET( clientSession->iSIPContentType, contentType().AllocL() );
    MCE_TH_SET( clientSession->iSIPHeaders, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    MCE_TH_SET( clientSession->iLocalSessionSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    MCE_TH_SET( clientSession->iRemoteSessionSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
	clientSession->iSIPHeaders->AppendL( header );
    if ( aRequire100rel )
        {
        clientSession->Modifier( KMce100Rel ) = KMce100RelRequired;
        }
    
	inStream->iLocalMediaPort = 5001;
	inStream->iIsEnabled = ETrue;
	inStream->iRemoteMediaPort = 5002;
	inStream->iLinkId = 31;
	inStream->iRemoteRtcpAddress = TInetAddr( 3004 );
    MCE_TH_SET( inStream->iLocalMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    MCE_TH_SET( inStream->iRemoteMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
	inStream->iLocalMediaSDPLines->AppendL( KFmtAttribute );

	outStream->iLocalMediaPort = 521;
	outStream->iRemoteMediaPort = 554;
	outStream->iLinkId = 51;
	outStream->iIsEnabled = EFalse;
	outStream->iRemoteRtcpAddress = TInetAddr( 5004 );
    MCE_TH_SET( outStream->iLocalMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
    MCE_TH_SET( outStream->iRemoteMediaSDPLines, new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity ) );
	outStream->iRemoteMediaSDPLines->AppendL( KFmtAttribute );
	
	
    codec1->iEnableVAD = ETrue;
    codec1->iSamplingFreq = 111;
    codec1->iPTime = 150;
    codec1->iMaxPTime = 190;
    codec1->iBitrate = 1122;
    codec1->iAllowedBitrates = 11111;
    codec1->iPayloadType = 197;
    codec1->iCodecMode = 12;
    codec1->iFourCC = 1567;
    codec1->iFrameSize = 165;
    MCE_TH_SET( codec1->iFmtpAttr, KFmtAttribute().AllocL() );
    
    codec2->iEnableVAD = ETrue;
    codec2->iSamplingFreq = 211;
    codec2->iPTime = 250;
    codec2->iMaxPTime = 290;
    codec2->iBitrate = 2122;
    codec2->iAllowedBitrates = 21111;
    codec2->iPayloadType = 297;
    codec2->iCodecMode = 22;
    codec2->iFourCC = 2567;
    codec2->iFrameSize = 265;
    MCE_TH_SET( codec2->iFmtpAttr, KFmtAttribute().AllocL() );
    
    
    mic->Enabled( ETrue );
    rtpSource->Enabled( EFalse );
    rtpSink->Enabled( ETrue );
    speaker->Enabled( EFalse );
    
    rtpSource->iInactivityTimer = 999;
    rtpSource->iBufferLength = 999;
    rtpSource->iBufferTreshold = 999;
    MCE_TH_SET( rtpSource->iTalkBurstIndicator, KFmtAttribute().AllocL() );


    rtpSink->iSuppressRTCP = ETrue;
    rtpSink->iSSRC = 888;
    rtpSink->iLastPacket = 8899;
    
    CleanupStack::Pop( clientSession );
    
    return clientSession;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::EventMsg()
// -----------------------------------------------------------------------------
//
CMceMsgBase* MCETestHelper::EventMsg( TInt aEventType )
    {
    HBufC8* recipient = KRecipient().AllocLC();
    HBufC8* originator = KFrom().AllocLC();
    CDesC8Array* headers = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( headers );
    HBufC8* contentType = KContentType().AllocLC();
    TInt interval = 0;
    HBufC8* event = NULL;
    HBufC8* referTo = NULL;
    
    if ( aEventType == EMceItcEventTypeRefer )
        {
        event = KNullDesC8().AllocLC();
        referTo = KReferTo().AllocLC();
        }
    else if ( aEventType == EMceItcEventTypeSubscribe )
        {
        event = KEvent().AllocLC();
        referTo = KNullDesC8().AllocLC();
        interval = 180;
        }
    else if ( EMceItcEventTypeEither )
        {
        event = KNullDesC8().AllocLC();
        referTo = KNullDesC8().AllocLC();
        }
    CMceMsgSIPEvent* msg = new( ELeave ) CMceMsgSIPEvent( 1, // aId 
                                         1, // aDialogId
                                         (TMceItcEventType) aEventType,
                                         recipient,
                                         originator,
                                         event,
                                         referTo,
                                         CMceRefer::ENoSuppression,
                                         interval,
                                         headers,
                                         contentType );

    CleanupStack::Pop( referTo );
    CleanupStack::Pop( event );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( headers );
    CleanupStack::Pop( originator );
    CleanupStack::Pop( recipient );
    return msg;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::ReplyMsg()
// -----------------------------------------------------------------------------
//
CMceMsgBase* MCETestHelper::ReplyMsg( TUint aReasonCode )
    {
    HBufC8* reason = KReason().AllocLC();
    CDesC8Array* headers = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( headers );
    HBufC8* contentType = KContentType().AllocLC();
    
    CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply( *reason, 
                                                           aReasonCode, 
                                                          *headers, 
                                                          *contentType );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( headers );
    CleanupStack::Pop( reason );
    
    return reply;
    }

// -----------------------------------------------------------------------------
// MCETestHelper::RequestMsg()
// -----------------------------------------------------------------------------
//
CMceMsgBase* MCETestHelper::RequestMsg( const TDesC8& aMethod )
    {
    HBufC8* method = aMethod.AllocLC(); 
    
    CDesC8Array* hdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( hdrs );
    
    HBufC8* ct = KContentType().AllocLC();

	CMceMsgSIPRequest* msg = new (ELeave) CMceMsgSIPRequest( *method, *hdrs, 
																	*ct );
	CleanupStack::Pop( ct );
	CleanupStack::Pop( hdrs );
	CleanupStack::Pop( method ) ;
    
    return msg;
    }
    

// -----------------------------------------------------------------------------
// MCETestHelper::ChangeResponseTypeL()
// -----------------------------------------------------------------------------
//
void MCETestHelper::ChangeResponseTypeL( const CSIPResponseElements& aResponse, 
                                         TUint aResponseCode,
                                         TInt aReasonPhrase )
    {
    CSIPResponseElements& response = 
        const_cast< CSIPResponseElements& >( aResponse );
    response.SetStatusCodeL( aResponseCode );
    response.SetReasonPhraseL( SIPStrings::StringF( aReasonPhrase ) );
    }
