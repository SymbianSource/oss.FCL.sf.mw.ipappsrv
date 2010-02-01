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




#include <e32math.h>
#include <sipinvitedialogassoc.h>
#include <sipfromheader.h>
#include <siptoheader.h>
#include <sipcontenttypeheader.h>
#include <sipextensionheader.h>
#include <sipaddress.h>
#include <sipprofile.h>
#include <sipmessageelements.h>
#include <sipsupportedheader.h>
#include <siprequireheader.h>
#include <sdpdocument.h>
#include <siprseqheader.h>
#include "mceactionset.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcecomsession.h"
#include "mcecomrtpsink.h"
#include "mcecomrtpsource.h"
#include "mcecomspeakersink.h"
#include "mcecomfilesource.h"
#include "mcecommicsource.h"
#include "mcesipsession.h"
#include "mcemediamanager.h"
#include "mcesipconnection.h"
#include "mcesipextensions.h"
#include "mcesip.h"
#include "mcesrvlogs.h"
#include "mcetimermanager.h"
#include "mcereliablesender.h"
#include "mcecomcamerasource.h"

const TInt KMaxLengthOfSDPMessage = 1000;
const TInt KMaxLengthOfSessionExpires = 40;

#define MCE_HANDLER_WRITE( aType, aData )\
    aType writeBuf( aData );\
    iWriteData->Copy( writeBuf );
    
#define MCE_HANDLER_READ_DATA( aCastType, aType, aData, aBuffer )\
    aCastType readBuf;\
    readBuf.Copy( *aBuffer );\
    aType aData = readBuf();
#define MCE_HANDLER_READ( aCastType, aType, aData )\
    MCE_HANDLER_READ_DATA( aCastType, aType, aData, iReadData )


// -----------------------------------------------------------------------------
// TMceActionSet::TMceActionSet
// -----------------------------------------------------------------------------
//
TMceActionSet::TMceActionSet( CMceSipSession& aSession )
  : iSession ( aSession )
	{
	}

    
// -----------------------------------------------------------------------------
// TMceActionSet::StateChanged
// -----------------------------------------------------------------------------
//
void TMceActionSet::StateChanged( TMceStateIndex aNewState )
    {    
    iSession.NextState( aNewState );
    //for long session, once session established, key updated need to be turn on
    
    if ( aNewState ==  KMceStateEstablished && 
    	ActiveBody().SecureSession())
    	{
    	if ( ActiveBody().SecureSession()->iCryptoContextUpdate )
    		{
    		ActiveBody().SecureSession()->iKeyNeedUpdated = ETrue; 	
    		}
    	else 
    		{
			ActiveBody().SecureSession()->iKeyNeedUpdated = EFalse;
			}    	
    	}
    	
    }


// -----------------------------------------------------------------------------
// TMceActionSet::ClientStateChanged
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientStateChanged( TMceIds& aIds, 
                                        CMceSession::TState aNewState,
                                        TInt aStatus )
    {
    MCESRV_DEBUG("TMceActionSet::ClientStateChanged( sync ), Entry");
    MCESRV_DEBUG_CLISTATE("new state", aNewState );
    MCESRV_DEBUG_DVALUE("status", aStatus );
    
    ActiveBody().iState = aNewState;
	aIds.iState = aNewState;
	aIds.iStatus = aStatus;
	
	
    MCESRV_DEBUG("TMceActionSet::ClientStateChanged( sync ), Exit");
    }


// -----------------------------------------------------------------------------
// TMceActionSet::ClientStateChangedL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientStateChangedL( CMceSession::TState aNewState, 
                                         TInt aReason )
    {
    MCESRV_DEBUG("TMceActionSet::ClientStateChanged, Entry");
    MCESRV_DEBUG_CLISTATE("new state", aNewState );

    if ( &ActiveBody() )
        {
        ActiveBody().iState = aNewState;
        }

    TMceIds ids;
    ids.iCallbackID = EMceItcStateChanged;
	ids.iState = aNewState;
	
    if ( aReason == KErrNone )
        {
        iSession.SendToClient( ids );
        }
    else
        {
        MCESRV_DEBUG_DVALUE("error", aReason )
        ids.iStatus = aReason;
        
    	CMceMsgSIPReply* replyMsg = new (ELeave) CMceMsgSIPReply();
    	CleanupStack::PushL( replyMsg );
        replyMsg->iCode = aReason;
        
        SendSIPDataToClientL( ids, *replyMsg, NULL );
    	CleanupStack::PopAndDestroy( replyMsg );
        }
    
                                      
    MCESRV_DEBUG("TMceActionSet::ClientStateChanged, Exit");
    }



    
    

    
// -----------------------------------------------------------------------------
// TMceActionSet::ClientStateChangedL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientStateChangedL( CMceSession::TState aNewState, 
                                         CSIPClientTransaction& aResponse,
                                         TInt aReason )
    {
    MCESRV_DEBUG("TMceActionSet::ClientStateChangedL( response ), Entry");
    MCESRV_DEBUG_CLISTATE("new state", aNewState );
    MCESRV_DEBUG_DVALUE("error", aReason );
   
    
    const CSIPResponseElements* response = aResponse.ResponseElements();
    if ( !response )
	    {
        ClientStateChangedL( aNewState, aReason );
        return;
	    }

    MCESRV_DEBUG_DVALUE("status code", response->StatusCode() );
    MCESRV_DEBUG_SVALUE("reason", response->ReasonPhrase().DesC() );

    ActiveBody().iState = aNewState;
    
    TMceIds ids;
    ids.iCallbackID = EMceItcStateChanged;
	ids.iState = aNewState;
    ids.iStatus = aReason;

    HBufC8* content = NULL;
    
	CMceMsgSIPReply* replyMsg = new (ELeave) CMceMsgSIPReply();
	CleanupStack::PushL( replyMsg );
    replyMsg->PushL();
	
    const CSIPMessageElements* elements = ResponseDataL( *replyMsg, aResponse, ActiveBody() );
    
    if ( elements )
        {
        content = elements->Content().AllocLC();
        }
        
    SendSIPDataToClientL( ids, *replyMsg, content );
    
    if ( content )
        {
        CleanupStack::Pop( content );
        }
        
	CleanupStack::PopAndDestroy( replyMsg );
	
	
    MCESRV_DEBUG("TMceActionSet::ClientStateChangedL( response ), Exit");
    	
    }
    
// -----------------------------------------------------------------------------
// TMceActionSet::ReceiveExtensionResponseL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ReceiveExtensionResponseL( CSIPClientTransaction& aResponse, 
														CMceComSession& aBody )
    {

	MCESRV_DEBUG("TMceActionSet::ReceiveExtensionResponseL( response ), Entry");
  	TUint count = KErrNone;
	TUint TransactoinID = KMceNotAssigned;
	count = iSession.PendingTransactions().Count();
  	
  	if ( count )
  		{
  			for ( TUint i=0; i < count; i++ )
  				{
  				if ( *iSession.PendingTransactions()[i] == aResponse )
  					{
  					TransactoinID = reinterpret_cast <TUint> 
  										(iSession.PendingTransactions()[i]);
  					}
  				}
  		}
   
    const CSIPMessageElements* elements = NULL;
    const CSIPResponseElements* response = aResponse.ResponseElements();
    
    HBufC8* content = NULL;

    if ( response )
	    {
        MCESRV_DEBUG_DVALUE("status code", response->StatusCode() );
        MCESRV_DEBUG_SVALUE("reason", response->ReasonPhrase().DesC() );

        CMceMsgSIPReply* replyMsg = replyMsg = new (ELeave) CMceMsgSIPReply();
        CleanupStack::PushL( replyMsg );
        
        replyMsg->PushL();//takes ownership of its content 
        
        elements = ResponseDataL( *replyMsg, aResponse, aBody );
             

    	TMceIds ids;
		ids.iCallbackID = EMceItcResponseReceived;
		ids.iTransactionID = TransactoinID;
           
    	content = elements ? elements->Content().AllocLC() : NULL;
      
       	SendSIPDataToClientL( ids, *replyMsg, content );
          
    	if ( content )
	        {
	        CleanupStack::Pop( content );
	        }
		
		CleanupStack::PopAndDestroy( replyMsg );	        
	    
	    MCESRV_DEBUG(
	        "TMceActionSet::ReceiveExtensionResponseL( session, response ), Exit");
	    }
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ReceiveExtensionRequestL 
// -----------------------------------------------------------------------------
//
void TMceActionSet::ReceiveExtensionRequestL( TMceStateTransitionEvent& aEvent )
    {
	MCESRV_DEBUG("TMceActionSet::ReceiveExtensionRequestL, Entry");

	CSIPServerTransaction& serverTransaction  = aEvent.Session().Request();
    
    const CSIPMessageElements* elements = 
    								MceSip::ToMessageElements( serverTransaction );
    
	HBufC8* method = MceSip::Method( serverTransaction ).DesC().AllocLC();

	CDesC8Array* aSIPHeaders = NULL;
	
	if ( aEvent.Dialog())
		{
		aSIPHeaders = MceSip::UserHeadersToTextArrayL( serverTransaction,
                                               *aEvent.Dialog() );		
        CleanupStack::PushL( aSIPHeaders );                                       
		}
	
	else
		{
		aSIPHeaders = MceSip::UserHeadersToTextArrayL( serverTransaction,
 	                                     iSession.Dialog()->Dialog() );		
 	    CleanupStack::PushL( aSIPHeaders );                                 
		}
       	                                     
	HBufC8* aSIPContentType = ( elements && elements->ContentType() ) ? 
         	                         elements->ContentType()->ToTextValueL() :
        	                         KNullDesC8().AllocL();
        	                         
        
	CleanupStack::PushL( aSIPContentType );  
	  	                                                
    TMceIds ids;
    ids.iCallbackID = EMceItcRequestReceived;
    ids.iTransactionID = reinterpret_cast <TUint> (&serverTransaction);   
    
    
	CMceMsgSIPRequest* requestMsg = new (ELeave) CMceMsgSIPRequest( *method, 
											*aSIPHeaders, *aSIPContentType );
	requestMsg->PushL();													
													
	CleanupStack::Pop( aSIPContentType );
	CleanupStack::Pop( aSIPHeaders );
	CleanupStack::Pop( method );

	CleanupStack::PushL( requestMsg );
       
    HBufC8* content = 
    	elements ? elements->Content().AllocLC() : KNullDesC8().AllocLC();

    SendSIPDataToClientL( ids, *requestMsg, content );

    if ( content )
        {
        CleanupStack::Pop( content );
        }
       
	CleanupStack::PopAndDestroy( requestMsg );
	
	MCESRV_DEBUG("TMceActionSet::ReceiveExtensionRequestL, Exit");
	}



// -----------------------------------------------------------------------------
// TMceActionSet::ReceiveExtensionError 
// -----------------------------------------------------------------------------
//
void TMceActionSet::ReceiveExtensionError( CSIPTransactionBase& aTransaction, 
											TInt aError )
    {
	MCESRV_DEBUG("TMceActionSet::ReceiveExtensionErrorL, Entry");

	TUint count = KErrNone;
	TUint TransactoinID = KMceNotAssigned;
	TBool found = EFalse;

	// If Error is for Client Transaction:
	if ( aTransaction.IsSIPClientTransaction() )
	
		{
		MCESRV_DEBUG("Error Received for Client Transaction");
		CSIPClientTransaction& clientTransaction = 
            static_cast<CSIPClientTransaction&>( aTransaction );
        
		count = iSession.PendingTransactions().Count();

	  	if ( count )
	  		{
  			for ( TUint i=0; i < count; i++ )
  				{
  				if ( *iSession.PendingTransactions()[i] == clientTransaction )
  					{
  					TransactoinID = reinterpret_cast <TUint> 
  										(iSession.PendingTransactions()[i]);
  					found = ETrue;					
  					}
  				}
	  		}
		}


	// If Error is for Client Transaction:
	else
		{
		MCESRV_DEBUG("Error Received for Server Transaction");
		CSIPServerTransaction& serverTransaction = 
            static_cast<CSIPServerTransaction&>( aTransaction );
            
		count = iSession.PendingReceivedRequests().Count();

	  	if ( count )
	  		{
  			for ( TUint i=0; i < count; i++ )
  				{
  				if ( *iSession.PendingReceivedRequests()[i] == serverTransaction )
  					{
  					TransactoinID = reinterpret_cast <TUint> 
  										(iSession.PendingReceivedRequests()[i]);
  					found = ETrue;					
  					}
  				}
	  		}
		}

	// Send an Error Response to the Client if client or server transcation exisit
	// for which we receive an error.
	if ( found )
		{
		MCESRV_DEBUG("Tranaction Matched Sending Error Response");
	    TMceIds ids;
	    ids.iTransactionID = TransactoinID;  
		iSession.SendErrorToClient( ids, aError );
		}

	MCESRV_DEBUG("TMceActionSet::ReceiveExtensionErrorL, Exit");
    }


// -----------------------------------------------------------------------------
// TMceActionSet::SendExtensionResponseL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendExtensionResponseL( CMceMsgBase& aData, TMceIds& aIds )
    {
	MCESRV_DEBUG("TMceActionSet::SendExtensionResponseL, Entry");

	TUint count = KErrNone;
	TInt index = KErrNotFound;
	CSIPServerTransaction* PendingRequest = NULL;
		
	count = iSession.PendingReceivedRequests().Count();
  	
  	if ( count )
  		{
  			for ( TUint i=0; i < count; i++ )
  				{
  				if ( reinterpret_cast <TUint> (iSession.PendingReceivedRequests()[i]) ==
  					aIds.iTransactionID )
  					{
  					PendingRequest = iSession.PendingReceivedRequests()[i];
  					index = i;
  					}
  				}
    
    	if ( index != KErrNotFound )

    		{
    		CMceMsgSIPReply* data = static_cast<CMceMsgSIPReply*>( &aData );  				
       		SendResponseL( *PendingRequest, *data );		
    		}
    	
    	else 
    		{
    		User::Leave( KErrNotFound );
    		}
    	
  		}
  	   
   	MCESRV_DEBUG("TMceActionSet::SendExtensionResponseL, Exit");
    }




// -----------------------------------------------------------------------------
// TMceActionSet::ClientStateChangedL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientStateChangedL( CMceSession::TState aNewState, 
                                         CSIPServerTransaction& aRequest )
    {
    MCESRV_DEBUG("TMceActionSet::ClientStateChangedL( request ), Entry");
    MCESRV_DEBUG_CLISTATE("new state", aNewState );
    MCESRV_DEBUG_SVALUE("request", MceSip::Method( aRequest ).DesC() );

    
    const CSIPRequestElements* request = aRequest.RequestElements();
    
    if ( !request )
	    {
        ClientStateChangedL( aNewState );
        return;
	    }

    ActiveBody().iState = aNewState;
    
    TMceIds ids;
    ids.iCallbackID = EMceItcStateChanged;
	ids.iState = aNewState;
    
	CMceMsgSIPData* requestMsg = new (ELeave) CMceMsgSIPData();
	CleanupStack::PushL( requestMsg );
    requestMsg->PushL();
	
    const CSIPMessageElements* elements = 
        RequestDataL( *requestMsg, aRequest );
    
    HBufC8* content = elements ? elements->Content().AllocLC() : 
        KNullDesC8().AllocLC();

    SendSIPDataToClientL( ids, *requestMsg, content );

    if ( content )
        {
        CleanupStack::Pop( content );
        }
        
	CleanupStack::PopAndDestroy( requestMsg );

    MCESRV_DEBUG("TMceActionSet::ClientStateChangedL( request ), Exit");
    
    
    }
    


// -----------------------------------------------------------------------------
// TMceActionSet::ClientStateChangedL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientStateChangedL( CMceSession::TState aNewState, 
                                         CMceComSession& aBody,
                                         CSIPClientTransaction& aResponse,
                                         TInt aReason )
    {
   
    MCESRV_DEBUG(
        "TMceActionSet::ClientStateChangedL( session, response ), Entry");
    MCESRV_DEBUG_CLISTATE("new state", aNewState );
    MCESRV_DEBUG_DVALUE("error", aReason );
    TMceComFactory factory;

    aBody.iState = aNewState;
    
    const CSIPMessageElements* elements = NULL;
    const CSIPResponseElements* response = aResponse.ResponseElements();
    
    HBufC8* content = NULL;
    
    CMceMsgArray<TMceComFactory>* messageArray = 
        new (ELeave) CMceMsgArray<TMceComFactory>( factory );
    CleanupStack::PushL( messageArray );
    
    
    if ( response )
	    {
        MCESRV_DEBUG_DVALUE("status code", response->StatusCode() );
        MCESRV_DEBUG_SVALUE("reason", response->ReasonPhrase().DesC() );

        CMceMsgSIPReply* replyMsg = replyMsg = new (ELeave) CMceMsgSIPReply();
        CleanupStack::PushL( replyMsg );
        replyMsg->PushL();//takes ownership of its content
        
        elements = ResponseDataL( *replyMsg, aResponse, aBody );

        messageArray->iArray.AppendL( replyMsg );
        CleanupStack::Pop( replyMsg );
        
	    }


    CMceMsgObject<CMceComSession>* sessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>
        ( aBody, EMceItcMsgTypeSession );
    CleanupStack::PushL( sessionMsg );
    
    messageArray->iArray.AppendL( sessionMsg );
    CleanupStack::Pop( sessionMsg );
    

    TMceIds ids;
	ids.iState = aNewState;
    ids.iCallbackID = EMceItcStateChanged;
    ids.iStatus = aReason;
    
    
    if ( aReason != KErrNone && 
         iSession.SubState() == CMceSipSession::EUpdating )
        {
        MCESRV_DEBUG("ClientStateChangedL: update failed");
        ids.iCallbackID = EMceItcUpdateFailed;
        }
    else
        {
        MCESRV_DEBUG("ClientStateChangedL: basic");
        }

    content = elements ? elements->Content().AllocLC() : NULL;

    
    SendSIPDataToClientL( ids, *messageArray, content );
    
    if ( content )
        {
        CleanupStack::Pop( content );
        }
        
    //pops and destroys also its content
    CleanupStack::PopAndDestroy( messageArray );
    

    MCESRV_DEBUG(
        "TMceActionSet::ClientStateChangedL( session, response ), Exit");
    	
    }


// -----------------------------------------------------------------------------
// TMceActionSet::ClientStateChangedL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientStateChangedL( CMceSession::TState aNewState, 
                                         CMceComSession& aBody, 
                                         TInt aReason )
                              
    {

    MCESRV_DEBUG(
        "TMceActionSet::ClientStateChangedL( newState, body. reason ), Entry");
    MCESRV_DEBUG_CLISTATE("new state", aNewState );
    MCESRV_DEBUG_DVALUE("error", aReason );

    aBody.iState = aNewState;

    TMceIds ids;
	ids.iState = aNewState;

    if ( aReason == KErrNone )
        {
        ids.iCallbackID = EMceItcStateChanged;
        }
    else
        {
        ids.iCallbackID = EMceItcUpdateFailed;
        ids.iStatus = aReason;
        }

    SendBodyToClientL( ids, aBody, NULL );
    
    MCESRV_DEBUG(
        "TMceActionSet::ClientStateChangedL( newState, body. reason ), Entry");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::UpdateFailed
// -----------------------------------------------------------------------------
//
void TMceActionSet::UpdateFailed()
    {
    MCESRV_DEBUG("TMceActionSet::UpdateFailed, Entry");

    if ( iSession.SubState() == CMceSipSession::EUpdating )
        {
        iSession.UpdateRejected();
        }
    else
        {
        MCESRV_DEBUG("UpdateFailed: Called in wrong sub state!, Exit");    
        }
    
    MCESRV_DEBUG("TMceActionSet::UpdateFailed, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::UpdateSucceed
// -----------------------------------------------------------------------------
//
void TMceActionSet::UpdateSucceed()
    {
    if ( iSession.SubState() == CMceSipSession::EUpdating )
        {
        MCESRV_DEBUG("TMceActionSet::UpdateSucceed, Entry");
        iSession.UpdateAccepted();
        MCESRV_DEBUG("TMceActionSet::UpdateSucceed, Exit");
        }
    else
        {
        //NOP
        }
    }

// -----------------------------------------------------------------------------
// TMceActionSet::CreateSDP
// -----------------------------------------------------------------------------
//
TInt TMceActionSet::CreateSDP( CSIPTransactionBase& aSDPContainer )
    {
    CSdpDocument* sdp = NULL;
    TInt status = KErrNone;
        
    if ( aSDPContainer.IsSIPClientTransaction() )
        {
        CSIPClientTransaction& answer = 
            static_cast<CSIPClientTransaction&>( aSDPContainer );
        const CSIPResponseElements* elements = answer.ResponseElements();
        
        TRAP( status, sdp = 
            CSdpDocument::DecodeL( elements->MessageElements().Content() ) );
        }
    else
        {
        CSIPServerTransaction& offer = 
            static_cast<CSIPServerTransaction&>( aSDPContainer );
        const CSIPRequestElements* elements = offer.RequestElements();
        
        TRAP( status, sdp = 
            CSdpDocument::DecodeL( elements->MessageElements().Content() ) );
        }
    
    if ( status == KErrNone )
        {
        iSession.StorePrevOffer();
        iSession.SetOffer( sdp );
        iSession.IncrementReceivedSdpCount();
        }
        
    return status;       
    }
    
// -----------------------------------------------------------------------------
// TMceActionSet::CreateBodyCandidateL
// -----------------------------------------------------------------------------
//
void TMceActionSet::CreateBodyCandidateL( CMceComSession::TType aType )
    {
    MCESRV_DEBUG("TMceActionSet::CreateBodyCandidateL, Entry");
    __ASSERT_ALWAYS( iSession.Body(), User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( !iSession.BodyCandidate(), User::Leave( KErrNotReady ) );
   
	CSIPServerTransaction& request = iSession.Request();
	CSIPServerTransaction* requestPtr = &iSession.Request();
	
    if ( (requestPtr!=NULL) && !MceSip::HasContent( request ))
        {
        iSession.iNewBodyCandidate = iSession.Body()->CloneL( ETrue );
        }
    else
        {
        iSession.iNewBodyCandidate = iSession.Body()->CloneL();
        }
     
    iSession.iNewBodyCandidate->iType = aType;
        
    iSession.SetSubState( CMceSipSession::EUpdating );

    MCESRV_DEBUG("TMceActionSet::CreateBodyCandidateL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::UpdateBodyL
// -----------------------------------------------------------------------------
//
void TMceActionSet::UpdateBodyL( TMceIds& aIds, CMceMsgBase& aBody )
    {
    MCESRV_DEBUG("TMceActionSet::UpdateBodyL, Entry");
    
    CMceSipSession::TSubState subState = iSession.SubState();
    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    TMceComEventHandler handler( mediaManager );
    CMceComSession* body = NULL;
    CMceMsgObject<CMceComSession>* msg = 
        static_cast<CMceMsgObject<CMceComSession>*>( &aBody );
    
    if ( !iSession.Body() )
        {
        iSession.SetBodyL( msg->iObject );
        msg->Object();//ownership is transferred
        body = iSession.Body();
        }
    else if ( subState != CMceSipSession::EUpdating )
        {
        body = iSession.Body();
        TMceComEvent event( aIds, msg->iObject, EMceItcUpdate, handler );
        body->EventReceivedL( event );
        iSession.Extensions().Update( *body );
        }
    else
        {
        body = iSession.BodyCandidate();
        TMceComEvent event( aIds, msg->iObject, EMceItcUpdate, handler );
        body->EventReceivedL( event );
        iSession.Extensions().Update( *body );
        }
    
    aBody.Close();
    
    // transfer ownership of content to body
    MCE_DELETE( body->iSIPContent );
    body->iSIPContent = iSession.PopClientContent();
    
    // If body's origin member is not set, parse origin from SIP profile.
    if ( body->iType == CMceComSession::EOutSession &&
         ( !body->iOriginator || body->iOriginator->Length() == 0 ) )
        {
        const TDesC8* userAor = NULL;
        const CSIPProfile& profile = iSession.Profile();
        TInt result = profile.GetParameter( KSIPUserAor, userAor );
        if ( KErrNone == result && userAor && userAor->Size() != 0 )
            {
            MCESRV_DEBUG_SVALUE(
                "TMceActionSet::UpdateBodyL, UPDATE ORIGINATOR", *userAor );
            body->SetOriginatorL( *userAor );
            }
        }
    
    MCESRV_DEBUG("TMceActionSet::UpdateBodyL, Exit");
    }
    
// -----------------------------------------------------------------------------
// TMceActionSet::SendInviteL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendInviteL()
    {
    MCESRV_DEBUG("TMceActionSet::SendInviteL, Entry");
    CMceSipSession::TSubState subState = iSession.SubState();
    CMceComSession* body = NULL;
    
    if ( subState != CMceSipSession::EUpdating )
        {
    	body = iSession.Body();
        }
    else
        {
        body = iSession.BodyCandidate();
        }
    
    //send invite
    SendInviteL( *body );
    
    MCESRV_DEBUG("TMceActionSet::SendInviteL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendSessionRefreshL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendSessionRefreshL()
    {
    MCESRV_DEBUG("TMceActionSet::SendSessionRefreshL, Entry");
    
    iSession.SetSubState( CMceSipSession::ERefreshing );

    SendInviteL( ActiveBody() );

    MCESRV_DEBUG("TMceActionSet::SendSessionRefreshL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::EncodeL
// -----------------------------------------------------------------------------
//
void TMceActionSet::EncodeL()
    {
    MCESRV_DEBUG("TMceActionSet::EncodeL, Entry");
    
    CMceComSession& body = ActiveBody();
        
    CMceSipExtensions& ext = iSession.Extensions();
    TInt localQosPreconds = ext.Local( CMceSipExtensions::EQosPreconds );
    TInt remoteQosPreconds = ext.Remote( CMceSipExtensions::EQosPreconds );
    body.iUseLocalPreconditions = localQosPreconds != CMceSipExtensions::ENotSupported;
    body.iUseRemotePreconditions = remoteQosPreconds != CMceSipExtensions::ENotSupported;

    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    iSession.StorePrevOffer();
    iSession.SetOffer( mediaManager.EncodeL( body ) );
    
    MCESRV_DEBUG("TMceActionSet::EncodeL, Exit");
    }

    
// -----------------------------------------------------------------------------
// TMceActionSet::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceActionSet::DecodeL()
    {
    MCESRV_DEBUG("TMceActionSet::DecodeL, Entry");
    
    TMceSipWarningCode status = KErrNone;

    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    
    CMceComSession& body = ActiveBody();
    
    CMceSipExtensions& ext = iSession.Extensions();
    TInt localQosPreconds = ext.Local( CMceSipExtensions::EQosPreconds );
    TInt remoteQosPreconds = ext.Remote( CMceSipExtensions::EQosPreconds );
   
    body.iUseLocalPreconditions = localQosPreconds != CMceSipExtensions::ENotSupported;
    body.iUseRemotePreconditions = remoteQosPreconds != CMceSipExtensions::ENotSupported;
   	//set remote sec precondtions only when it is invite or update
   	if ( &iSession.Request() )
   		{
   		if (
   		MceSip::Method( iSession.Request() ) == SIPStrings::StringF( SipStrConsts::EInvite ) ||
   		MceSip::Method( iSession.Request() ) == SIPStrings::StringF( SipStrConsts::EUpdate ) )
			{
			body.iRemoteSecPreconditionsRequired = 
				ext.Remote( CMceSipExtensions::ESecPreconds ) == CMceSipExtensions::ERequired;
			}
   		}
   	else
   		{
   		body.iRemoteSecPreconditionsRequired = 
				ext.Remote( CMceSipExtensions::ESecPreconds ) != CMceSipExtensions::ENotSupported;
   		}
    status = mediaManager.DecodeL( body, *iSession.Offer() );
    
    MCESRV_DEBUG_DVALUE( "DecodeL: status", status);
        
    MCESRV_DEBUG("TMceActionSet::DecodeL, Exit");
    
    return status;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::Decode
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceActionSet::Decode()
    {
    MCESRV_DEBUG("TMceActionSet::Decode, Entry");
    
    TMceReturnStatus status = KErrNone;
    TInt error = KErrNone;    
    
    TRAP( error, status = DecodeL() );
            
    MCESRV_DEBUG_DVALUE( "Update: status", status );
    MCESRV_DEBUG_DVALUE( "Update: error", error );
        
    MCESRV_DEBUG("TMceActionSet::Decode, Exit");
    
    return error != KErrNone ? error : status;    
    }

// -----------------------------------------------------------------------------
// TMceActionSet::UpdateL
// -----------------------------------------------------------------------------
//
TMceReturnStatus TMceActionSet::UpdateL()
    {
    MCESRV_DEBUG("TMceActionSet::UpdateL, Entry");

    TMceReturnStatus status = KErrNone;

    status = iSession.UpdateMediaL();

    MCESRV_DEBUG_DVALUE( "UpdateL: status", status );

    MCESRV_DEBUG("TMceActionSet::UpdateL, Exit");

    return status;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::Update
// -----------------------------------------------------------------------------
//
TMceReturnStatus TMceActionSet::Update()
    {
    MCESRV_DEBUG("TMceActionSet::Update, Entry");

    TMceReturnStatus status = KErrNone;
    TInt error = KErrNone;

    TRAP( error, status = UpdateL() );

    MCESRV_DEBUG_DVALUE( "Update: status", status );
    MCESRV_DEBUG_DVALUE( "Update: error", error );

    MCESRV_DEBUG("TMceActionSet::Update, Exit");

    return error != KErrNone ? error : status;    
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ReserveL
// -----------------------------------------------------------------------------
//
TMceReturnStatus TMceActionSet::ReserveL()
    {
    MCESRV_DEBUG("TMceActionSet::ReserveL, Entry");
    
    TMceReturnStatus status = iSession.ReserveL();
    MCESRV_DEBUG_DVALUE( "ReserveL: status", status);
        
    MCESRV_DEBUG("TMceActionSet::ReserveL, Exit");
    
    return status;
    }
    
// -----------------------------------------------------------------------------
// TMceActionSet::ClientUpdateIncomingSessionL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientUpdateIncomingSessionL( 
                                            CSIPServerTransaction& aInvite )
    {
    MCESRV_DEBUG("TMceActionSet::ClientUpdateIncomingSessionL, Entry");

    ClientUpdateSessionL( aInvite, EMceItcObjectAdded );
    
    MCESRV_DEBUG("TMceActionSet::ClientUpdateIncomingSessionL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ClientUpdateSessionL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientUpdateSessionL( CSIPServerTransaction& aInvite, 
                                          TUint32 aCallback )
    {
    MCESRV_DEBUG("TMceActionSet::ClientUpdateSessionL, Entry");
    
    CMceComSession& body = ActiveBody();
    
    const CSIPMessageElements* elements = 
        ProcessInviteL( body, aInvite );
        
    RequestDataL( body, aInvite );
    
    HBufC8* content = elements ? elements->Content().AllocLC() : NULL;
    
    TMceIds ids;
    ids.iCallbackID = aCallback;
	ids.iState = CMceSession::EIncoming;
	
    if ( aCallback == EMceItcObjectAdded )
        {
        iSession.PrepareIdsL( ids );
        }

    SendBodyToClientL( ids, body, content );
    
    if ( content )
        {
        CleanupStack::Pop( content );
        }

    MCESRV_DEBUG("TMceActionSet::ClientUpdateSessionL, Entry");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ProcessInviteL
// -----------------------------------------------------------------------------
//
const CSIPMessageElements* 
TMceActionSet::ProcessInviteL( CMceComSession& aBody, 
                                CSIPServerTransaction& aInvite )
    {
    MCESRV_DEBUG("TMceActionSet::ProcessInviteL, Entry");

    const CSIPRequestElements* reqElements = aInvite.RequestElements();
    const CSIPMessageElements* elements = 
                reqElements ? &reqElements->MessageElements() : NULL;


    if ( reqElements )
        {
        const CSIPAddress& from = reqElements->FromHeader()->SIPAddress();
        const CSIPAddress& to = reqElements->ToHeader()->SIPAddress();
        
		MCE_DELETE( aBody.iOriginator );
		MCE_DELETE( aBody.iRecipient );
		
        aBody.iOriginator = MceSip::AddressToTextL( from );
        aBody.iRecipient = MceSip::AddressToTextL( to );
        }
    

    MCESRV_DEBUG("TMceActionSet::ProcessInviteL, Exit");
    
    return elements;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ResponseDataL
// -----------------------------------------------------------------------------
//
const CSIPMessageElements* 
TMceActionSet::ResponseDataL( CMceMsgSIPReply& aContainer, 
                               CSIPClientTransaction& aResponse,
                               CMceComSession& aBody )
    {
    const CSIPResponseElements* response = aResponse.ResponseElements();
    const CSIPMessageElements* elements = NULL;
    
    if ( response )
        {
        const CSIPAddress& to = response->FromHeader()->SIPAddress();
        HBufC8* originator = MceSip::AddressToTextL( to );
        if ( !aBody.iOriginator || aBody.iOriginator->Length() == 0 )
            {
    		MCE_DELETE( aBody.iOriginator );
            aBody.iOriginator = originator;
            }
        else
            {
            MCE_DELETE( originator );
            }
            
        elements = &response->MessageElements();
    
        aContainer.iCode = response->StatusCode();
        HBufC8* reason = response->ReasonPhrase().DesC().AllocLC();
        HBufC8* contentType = elements && elements->ContentType() ? 
    		                  elements->ContentType()->ToTextValueL() :
                              KNullDesC8().AllocL();
        CleanupStack::PushL( contentType );                             
        CDesC8Array* headers = 
            MceSip::UserHeadersToTextArrayL( aResponse,
                                             iSession.Dialog()->Dialog() );
        CleanupStack::Pop( contentType );
        CleanupStack::Pop( reason );
        aContainer.iReason = reason;
        aContainer.iSIPHeaders = headers;
        aContainer.iContentType = contentType;
        }
                          
    return elements;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::RequestDataL
// -----------------------------------------------------------------------------
//
const CSIPMessageElements* 
TMceActionSet::RequestDataL( CMceComSession& aContainer, 
                             CSIPServerTransaction& aRequest )
    {
    
    const CSIPMessageElements* elements = MceSip::ToMessageElements( aRequest );

    
    if ( elements )
    	{
		MCE_DELETE( aContainer.iSIPHeaders );
		MCE_DELETE( aContainer.iSIPContentType );
		
    	aContainer.iSIPHeaders = 
    	    MceSip::UserHeadersToTextArrayL( aRequest,
    	                                     iSession.Dialog()->Dialog() );
    	aContainer.iSIPContentType = elements->ContentType() ? 
         	                         elements->ContentType()->ToTextValueL() :
        	                         KNullDesC8().AllocL();
    	}
    
    return elements;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::RequestDataL
// -----------------------------------------------------------------------------
//
const CSIPMessageElements* 
TMceActionSet::RequestDataL( CMceMsgSIPData& aContainer, 
                             CSIPServerTransaction& aRequest )
    {
    const CSIPMessageElements* elements = MceSip::ToMessageElements( aRequest );

    
    if ( elements )
    	{
        aContainer.iTrxType = aRequest.Type().Index( SIPStrings::Table() );
    	aContainer.iSIPHeaders = 
    	    MceSip::UserHeadersToTextArrayL( aRequest,
    	                                     iSession.Dialog()->Dialog() );
    	aContainer.iContentType = elements->ContentType() ? 
         	                         elements->ContentType()->ToTextValueL() :
        	                         KNullDesC8().AllocL();
    	}
    
    return elements;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendBodyToClientL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendBodyToClientL( TMceIds& aIds,
                                       CMceComSession& aBody, 
                                       HBufC8* aContent )
    {
    MCESRV_DEBUG("TMceActionSet::SendBodyToClientL, Entry");
//PaisRic    MCESRV_DEBUG_COMSESSION((&aBody));
    
    aBody.iID = MCE_SESSION_ID(&iSession);
    aBody.iState = static_cast<CMceSession::TState>( aIds.iState );
    
    CMceMsgObject<CMceComSession>* sessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( aBody, 
                                                    EMceItcMsgTypeSession );
    CleanupStack::PushL( sessionMsg );
    sessionMsg->EncodeL();
    aIds.iMsgType = sessionMsg->Type();
        
	HBufC8* encSession = sessionMsg->EncodeBufferCloneL();
    CleanupStack::PopAndDestroy( sessionMsg );
    CleanupStack::PushL( encSession );
    
    if ( aIds.iCallbackID == EMceItcObjectAdded )
        {
        MCESRV_DEBUG("SendBodyToClientL: sending to manager");
        iSession.Client().SendToClientL( aIds, encSession, aContent );
        }
    else
        {
        MCESRV_DEBUG("SendBodyToClientL: sending to session");
        iSession.SendToClientL( aIds, encSession, aContent );
        }        
    
    
    CleanupStack::Pop( encSession );
    
    MCESRV_DEBUG("TMceActionSet::SendBodyToClientL, Entry");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendSIPDataToClientL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendSIPDataToClientL( TMceIds& aIds,
                                          CMceMsgBase& aData, 
                                          HBufC8* aContent )
                                         
    {
    aData.EncodeL();
    
    aIds.iMsgType = aData.Type();
    
	HBufC8* context = aData.EncodeBufferCloneL();    
    CleanupStack::PushL( context );

    iSession.SendToClientL( aIds, context, aContent );
    CleanupStack::Pop( context );    
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendAnswerL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendAnswerL( CSIPServerTransaction& aRequest )
    {
    MCESRV_DEBUG("TMceActionSet::SendAnswerL, Entry");
    
    CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply();
    CleanupStack::PushL( reply );
    
    reply->iCode = KMceSipOK;
    reply->iReasonAsPoolIndex = SipStrConsts::EPhraseOk;


    SendAnswerL( aRequest, *reply );
    
    CleanupStack::PopAndDestroy( reply );

    MCESRV_DEBUG("TMceActionSet::SendAnswerL, Exit");    
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendAnswerL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendAnswerL( CSIPServerTransaction& aRequest, 
                                 CMceMsgBase& aReply )
    {
    MCESRV_DEBUG("TMceActionSet::SendAnswerL( message ), Entry");
    
    //content, content type etc are stored to session in update
    
    CMceMsgSIPReply* reply = static_cast<CMceMsgSIPReply*>( &aReply );
    if ( !reply )
        {
        SendAnswerL( aRequest );
        return;
        }

    CSdpDocument* offer = iSession.Offer();
    HBufC8* sipContentType = ActiveBody().iSIPContentType;
    HBufC8* sipContent = ActiveBody().iSIPContent;
	if ( offer 
		 && MCE_HAS_DATA( sipContentType ) 
		 && MCE_HAS_DATA( sipContent )
		)
		{
        reply->iContentTypeId = EMceContentTypeMultiPart;
		}
    else if ( offer )
        {
        reply->iContentTypeId = EMceContentTypeSDP;
        }
    else
        {
        //NOP
        }
    
        
    SendResponseL( aRequest, *reply );
    aReply.Close();
    
    if ( iSession.SubState() == CMceSipSession::EAnswering )
        {
        iSession.StartSessionTimerL();
        }

    MCESRV_DEBUG("TMceActionSet::SendAnswerL( message ), Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::BodyToTextL
// -----------------------------------------------------------------------------
//
HBufC8* TMceActionSet::BodyToTextL()
    {
    HBufC8* encodedBody = NULL;
    
    CMceMsgObject<CMceComSession>* sessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *iSession.Body(), 
                                                    EMceItcMsgTypeSession );
    CleanupStack::PushL( sessionMsg );
    sessionMsg->EncodeL();
        
    encodedBody = sessionMsg->EncodeBuffer().Ptr(0).AllocL();
        
    CleanupStack::PopAndDestroy( sessionMsg );
        
    return encodedBody;
    }


//
// SIP Sending
//

// -----------------------------------------------------------------------------
// TMceActionSet::SendInviteL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendInviteL( CMceComSession& aData )
    {
    MCESRV_DEBUG("TMceActionSet::SendInviteL, Entry");
    
    SendRequestL( SipStrConsts::EInvite, &aData );
    
    MCESRV_DEBUG("TMceActionSet::SendInviteL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendUpdateRequestL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendUpdateRequestL()
    {
    MCESRV_DEBUG("TMceActionSet::SendUpdateRequestL, Entry");
    
    SendRequestL( SipStrConsts::EUpdate );
    
    MCESRV_DEBUG("TMceActionSet::SendUpdateMethodL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendPrackL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendPrackL( TBool aContent )
    {
    MCESRV_DEBUG("SendPrackL::SendUpdateRequestL, Entry");
    
    SendRequestL( SipStrConsts::EPrack, NULL, aContent );
    
    MCESRV_DEBUG("SendPrackL::SendUpdateMethodL, Exit");
    }
    

// -----------------------------------------------------------------------------
// TMceActionSet::SendExtensionRequestL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendExtensionRequestL( CMceMsgBase& aData, TMceIds& aIds )
    {
    MCESRV_DEBUG("TMceActionSet::SendExtensionRequestL, Entry");
        
    CMceMsgSIPRequest* data = static_cast<CMceMsgSIPRequest*>( &aData );  
            
    CSIPMessageElements* elements = CSIPMessageElements::NewLC();
        
    CSIPContentTypeHeader* contentType = NULL;
    
    HBufC8* content = iSession.PopClientContent();

    if ( MCE_HAS_DATA( content ) )
        {
        CleanupStack::PushL( content );            
        
        MCESRV_DEBUG_SVALUE( "Sending Session Request: content size", content->Length() );
        if ( MCE_HAS_DATA( data->iContentType ) )
            {
            MCESRV_DEBUG_SVALUE( "SendExtensionRequestL: content type", *data->iContentType );
            contentType = MceSip::ToContentTypeHeaderL( 
                                                    EMceContentTypeUserDefined, 
                                                    data->iContentType );
            CleanupStack::PushL( contentType );
            }
        else
            {
            MCESRV_DEBUG( "SendSessionRequestL: no content type -> no content" );
            CleanupStack::PopAndDestroy( content );
            content = NULL;
            }
        }
    else
        {
        delete content;
        content = NULL;
        }
    
    if ( content )
        {
        elements->SetContentL( content , contentType );
        CleanupStack::Pop( contentType );
        CleanupStack::Pop( content );
        }
   	
    RStringF methodStr = SIPStrings::Pool().OpenFStringL(data->RequestMethod()->Des());
    CleanupClosePushL( methodStr );
    
    TInt method = MceSip::SIPStringIndex( methodStr.DesC() );
    
    //create extension headers
    RPointerArray<CSIPHeaderBase> headers;
    CSIPHeaderBase::PushLC( &headers );
    
    // Add extension headers
    AddExtensionHeadersL( headers, method, ETrue );
    
    
    //Add Extra SIP Headers.
     AddHeadersL( headers, 
                     data->iSIPHeaders,
                     KErrNotFound );
        
    //create session timer headers
    AddSessionTimerFieldsClientL( headers );
   
  
    //send request within session
    elements->SetUserHeadersL( headers );
    CleanupStack::PopAndDestroy( &headers );

    CSIPInviteDialogAssoc* dialog =     
        static_cast<CSIPInviteDialogAssoc*>( iSession.Dialog() );

    MCESRV_DEBUG_METHOD("SendRequestL: sending ", method );
    
    CSIPClientTransaction* trx = NULL;
           
    trx = dialog->SendNonTargetRefreshRequestL( methodStr, elements );
    
    CleanupStack::PopAndDestroy(); //destroy methodStr
    CleanupStack::Pop( elements );
    
    CleanupStack::PushL( trx );
          
    iSession.SetPendingTransactionL( trx );
    
    CleanupStack::Pop( trx );
        
    MCESRV_DEBUG_METHOD("SendExtensionRequestL: sent ", method );
        		
	aIds.iTransactionID = reinterpret_cast <TUint> (trx);
    
    MCESRV_DEBUG("TMceActionSet::SendExtensionRequestL, Exit");
    }    

// -----------------------------------------------------------------------------
// TMceActionSet::SendRequestL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendRequestL( TInt aMethod, CMceComSession* aData, 
                                  TBool aContent )
    {
    MCESRV_DEBUG("TMceActionSet::SendRequestL, Entry");
    
    __ASSERT_ALWAYS( iSession.Dialog(), User::Leave( KErrGeneral ) );
    
    CSIPMessageElements* elements = CSIPMessageElements::NewLC();
    
    //create content
    CSdpDocument* offer = iSession.Offer();
    CSIPContentTypeHeader* contentType = NULL;
    HBufC8* content = NULL;
    
    if ( aContent )
        {
        
    	if ( offer 
    	     && aData  
    		 && MCE_HAS_DATA( aData->iSIPContentType ) 
    		 && MCE_HAS_DATA( aData->iSIPContent )
    		)
    		{
            MCESRV_DEBUG("SendRequestL: creating multi-part content");
    	    content = CreateMultipartContentLC( *offer,
                                                *aData->iSIPContentType,
                                                *aData->iSIPContent );
                                                
            contentType = MceSip::ToContentTypeHeaderL( EMceContentTypeMultiPart );
            CleanupStack::PushL( contentType );
    		}
        else if ( offer )
            {
            MCESRV_DEBUG("SendRequestL: creating SDP content");
    	    content = CreateSDPContentLC( *offer );	    
            contentType = MceSip::ToContentTypeHeaderL( EMceContentTypeSDP );
            CleanupStack::PushL( contentType );
            }
        else
            {
            MCESRV_DEBUG("SendRequestL: NO OFFER");
            //NOP
            }
        }
        else
            {
            MCESRV_DEBUG("SendRequestL: NO OFFER");
            //NOP
            }
    

    if ( content )
        {
        elements->SetContentL( content, contentType );
        CleanupStack::Pop( contentType );
    	CleanupStack::Pop( content );
        }

        
    //create extension headers
    RPointerArray<CSIPHeaderBase> headers;
    CSIPHeaderBase::PushLC( &headers );
    
    // Add extension headers
    AddExtensionHeadersL( headers, aMethod, ETrue );
    
    // Add user defined headers
    AddHeadersL( headers, 
               ( aData ? aData->iSIPHeaders : ActiveBody().iSIPHeaders ),
                 aMethod );
    
    //create session timer headers
    AddSessionTimerFieldsClientL( headers );
    
    //send invitation
    elements->SetUserHeadersL( headers );
    CleanupStack::PopAndDestroy( &headers );

    CSIPInviteDialogAssoc* dialog =     
        static_cast<CSIPInviteDialogAssoc*>( iSession.Dialog() );

    MCESRV_DEBUG_METHOD("SendRequestL: sending ", aMethod );
    
    CSIPClientTransaction* trx = NULL;
    switch ( aMethod )
        {
        case SipStrConsts::EInvite:
            {
            trx = dialog->SendInviteL( elements );
            break;
            }
        case SipStrConsts::EUpdate:
            {
            trx = dialog->SendUpdateL( elements );
            break;
            }
        case SipStrConsts::EPrack:
            {
            trx = dialog->SendPrackL( elements );
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
    CleanupStack::Pop( elements );
    
    CleanupStack::PushL( trx );
    iSession.SetPendingTransactionL( trx );

    MCESRV_DEBUG_METHOD("SendRequestL: sent ", aMethod );
    
    CleanupStack::Pop( trx );

    if ( content )
        {
        iSession.IncrementSentSdpCount();
        }

    MCESRV_DEBUG("TMceActionSet::SendRequestL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendDeclineL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendDeclineL( CSIPServerTransaction& aRequest, 
                                  CMceMsgBase& aReply )
    {
    MCESRV_DEBUG("TMceActionSet::SendDeclineL, Entry");
    
    CMceMsgSIPReply* reply = static_cast<CMceMsgSIPReply*>( &aReply );
    TBool userReply = MCE_NOT_NULL_PTR( reply );
    
    if ( !userReply )
        {
        reply = new (ELeave) CMceMsgSIPReply();
        CleanupStack::PushL( reply );
        reply->iCode = KMceSipDecline;
        reply->iReasonAsPoolIndex = SipStrConsts::EPhraseDecline;
        }
        
    SendResponseL( aRequest, *reply );
    reply->Close();
    
    if ( !userReply )
        {
        CleanupStack::PopAndDestroy( reply );
        }

    MCESRV_DEBUG("TMceActionSet::SendDeclineL, Exit");
    
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendRingL
// -----------------------------------------------------------------------------
//
TBool TMceActionSet::SendRingL( CSIPServerTransaction& aRequest,
                               CMceMsgBase& aReply )
    {
    MCESRV_DEBUG("TMceActionSet::SendRingL, Entry");

    CMceMsgSIPReply* reply = static_cast<CMceMsgSIPReply*>( &aReply );
    TBool userReply = MCE_NOT_NULL_PTR( reply );
    
    if ( !userReply )
        {
        reply = new (ELeave) CMceMsgSIPReply();
        CleanupStack::PushL( reply );
        reply->iCode = KMceSipRinging;
        reply->iReasonAsPoolIndex = SipStrConsts::EPhraseRinging;
        }
        
    TBool reliable = SendResponseL( aRequest, *reply );
    reply->Close();
    
    if ( !userReply )
        {
        CleanupStack::PopAndDestroy( reply );
        }
    
    MCESRV_DEBUG("TMceActionSet::SendRingL, Exit");
    return reliable;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendSipQueuedL  
// -----------------------------------------------------------------------------

TBool TMceActionSet::SendSipQueuedL( CSIPServerTransaction& aRequest,
                                                 CMceMsgBase& aReply )
    {
    MCESRV_DEBUG("TMceActionSet::SendSipQueuedL, Entry");

    CMceMsgSIPReply* reply = static_cast<CMceMsgSIPReply*>( &aReply );
    TBool userReply = MCE_NOT_NULL_PTR( reply );
    
    if ( !userReply )
        {
       	 User::Leave( KErrArgument );
        }
        
    TBool reliable = SendResponseL( aRequest, *reply );
    reply->Close();
    
    if ( !userReply )
        {
        CleanupStack::PopAndDestroy( reply );
        }
    
    MCESRV_DEBUG("TMceActionSet::SendSipQueuedL, Exit");
    return reliable;
    }    


// -----------------------------------------------------------------------------
// TMceActionSet::SendResponseL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendResponseL( CSIPServerTransaction& aRequest, 
                                   TUint32 aCode,
                                   TInt aReason )
    {
     MCESRV_DEBUG("TMceActionSet::SendResponseL, Entry");

        
    CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply();
    CleanupStack::PushL( reply );
    reply->iCode = aCode;
    reply->iReasonAsPoolIndex = aReason;
    reply->iContentTypeId = EMceContentTypeSDP;

    if ( aReason == KMceAutoReasonPhrase )
        {
        RStringF reasonPhrase = MceSip::ReasonPhraseL( aCode );
        CleanupClosePushL( reasonPhrase );
        reply->iReasonAsPoolIndex = reasonPhrase.Index( SIPStrings::Table() );
        reply->iContentTypeId = (TMceContentType)KMceNotAssigned;
        CleanupStack::PopAndDestroy();//reasonPhrase
    
        }
    if ( aReason == KMceSipPreconditionFailure )
    	{
        reply->iReasonAsPoolIndex = 0;
    	HBufC8* reason = KMceSipErrPhrase580().AllocLC();
    	MCE_DELETE( reply->iReason );
    	reply->iReason = reason;  
        CleanupStack::Pop( reason );
    	}
	
    SendResponseL( aRequest, *reply );
    //delete header, icontent type and iheader, reason
	reply->Close();
	CleanupStack::PopAndDestroy( reply );

    MCESRV_DEBUG("TMceActionSet::SendResponseL, Exit");
    
    }



// -----------------------------------------------------------------------------
// TMceActionSet::SendRejectOfferWithWarningL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendRejectOfferWithWarningL(    
                                            TMceSipWarningCode aReason, 
                                            CSIPServerTransaction& aInvite )
    {
    
    if ( aReason >= KMceSipBadRequest && 
         aReason <=  KMceSip6XXNotAcceptable )
        {
        if ( aReason ==  KMceSipPreconditionFailure )
        	{
        	SendResponseL( aInvite, (TUint32)aReason, KMceSipPreconditionFailure );
        	}
        else
        	{
        	SendResponseL( aInvite, (TUint32)aReason, KMceAutoReasonPhrase );
        	}
        }
    else
        {
        SendWarningResponseL( KMceSipNotAcceptableHere, aReason, aInvite );
        }
    }





// -----------------------------------------------------------------------------
// TMceActionSet::SendWarningResponseL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendWarningResponseL( const TUint aResponseCode,
                                          const TUint aWarnCode,
                                          CSIPServerTransaction& aRequest )
    {
    
    MCESRV_DEBUG("TMceActionSet::SendWarningResponseL, Entry");
    MCESRV_DEBUG_DVALUES( "response", aResponseCode, "warning", aWarnCode );
    
    const TInt KPopCount = 2;
    
    RStringF reasonPhrase = MceSip::ReasonPhraseL( aResponseCode );
    CleanupClosePushL( reasonPhrase );
    RStringF warningPhrase = MceSip::WarningPhraseL( aWarnCode );
    CleanupClosePushL( warningPhrase );

    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( aResponseCode, reasonPhrase );

    CSIPMessageElements& responseMessageElements =
            response->MessageElements();
                                    
    const CSIPRequestElements* requestElements = aRequest.RequestElements();
    const TDesC8& domain = 
        requestElements->RemoteUri().Uri().Extract( EUriHost );
    
    // Create SIPExtensionHeader:
    const TInt warnCodeLength = 3;
    TInt warnLenght = KMceSipWarningPattern().Length() + domain.Length() +
                      warningPhrase.DesC().Length() + warnCodeLength;
    HBufC8* warning = HBufC8::NewLC( warnLenght );
    warning->Des().AppendFormat( KMceSipWarningPattern, aWarnCode, 
                                   &domain, &(warningPhrase.DesC()) );
    
    RPointerArray< CSIPHeaderBase > headerArray;
    CSIPHeaderBase::PushLC( &headerArray );

    CSIPExtensionHeader* sipExtensionWarningHeader = 
        CSIPExtensionHeader::NewLC( KMceSipWarning, *warning );
    
    headerArray.AppendL( sipExtensionWarningHeader );
    CleanupStack::Pop( sipExtensionWarningHeader );
                
    responseMessageElements.SetUserHeadersL( headerArray );
    CleanupStack::Pop( &headerArray );// headerArray
    
    CleanupStack::PopAndDestroy( warning );
    
    aRequest.SendResponseL( response );

    
    CleanupStack::Pop( response );
    CleanupStack::PopAndDestroy( KPopCount );//warningPhrase,reasonPhrase

    MCESRV_DEBUG("TMceActionSet::SendWarningResponseL, Entry");    
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendResponseL
// -----------------------------------------------------------------------------
//
TBool TMceActionSet::SendResponseL( CSIPServerTransaction& aRequest, 
                                   CMceMsgSIPReply& aResponse )
    {
    MCESRV_DEBUG("TMceActionSet::SendResponseL( custom ), Entry");
    MCESRV_DEBUG_DVALUE( "reason", aResponse.iCode );

    CSIPResponseElements* response = NULL;
    
    if ( !aResponse.iReasonAsPoolIndex )
        {
        MCESRV_DEBUG_SVALUE( "phrase", *aResponse.iReason );
        
        RStringF reasonPhrase = 
            SIPStrings::Pool().OpenFStringL( *aResponse.iReason );
        CleanupClosePushL( reasonPhrase );

        response = 
            CSIPResponseElements::NewL( aResponse.iCode, reasonPhrase );
            
        CleanupStack::PopAndDestroy();//reasonPhrase
        CleanupStack::PushL( response );
        }
    else
        {
        MCESRV_DEBUG_SVALUE( "phrase", 
            SIPStrings::StringF( aResponse.iReasonAsPoolIndex ).DesC() );
            
        response = 
            CSIPResponseElements::NewLC( aResponse.iCode, 
                    SIPStrings::StringF( aResponse.iReasonAsPoolIndex ) );
        }

    CSIPMessageElements& elements = response->MessageElements();
    RPointerArray<CSIPHeaderBase> headers;
    CSIPHeaderBase::PushLC( &headers );

    TBool reliable = AddExtensionHeadersL( 
                headers,
                aRequest.Type().Index( SIPStrings::Table() ),
                aResponse.iContentTypeId == EMceContentTypeSDP ? ETrue : EFalse,
                ETrue,
                aResponse.iCode );
                                            
    if ( !MCE_HAS_DATA( aResponse.iSIPHeaders ) )
        {
        AddHeadersL( headers, 
                     ActiveBody().iSIPHeaders, 
                     KErrNotFound );
        }
    else
        {
        AddHeadersL( headers, 
                     aResponse.iSIPHeaders, 
                     KErrNotFound );
        }
    
    if ( ( aRequest.Type() == SIPStrings::StringF( SipStrConsts::EInvite ) ||
        aRequest.Type() == SIPStrings::StringF( SipStrConsts::EUpdate ) ) &&
        aResponse.iCode == KMceSipOK )
        {
        //add timer headers only to 200 OK of invite
        AddSessionTimerFieldsServerL( headers );
        }

    elements.SetUserHeadersL( headers );
    CleanupStack::Pop( &headers );

    CSIPContentTypeHeader* contentType = NULL;
    HBufC8* content = NULL;
    CSdpDocument* offer = iSession.Offer();
    
    if ( aResponse.iContentTypeId == EMceContentTypeMultiPart )
        {
        MCESRV_DEBUG( "SendResponseL: content type=\"multipart/mixed\"");
	    content = CreateMultipartContentLC( *offer,
                                            *ActiveBody().iSIPContentType,
                                            *ActiveBody().iSIPContent );
        
        contentType = 
            MceSip::ToContentTypeHeaderL( EMceContentTypeMultiPart );
        CleanupStack::PushL( contentType );
        }
    else if ( aResponse.iContentTypeId == EMceContentTypeSDP )
        {
        MCESRV_DEBUG( "SendResponseL: content type=\"application/sdp\"");
	    content = CreateSDPContentLC( *offer );
        contentType = MceSip::ToContentTypeHeaderL( EMceContentTypeSDP );
        CleanupStack::PushL( contentType );
        }
    else if ( aResponse.iContentTypeId == EMceContentTypeUserDefined && 
              MCE_HAS_DATA( aResponse.iContentType ) )
        {
        MCESRV_DEBUG_SVALUE( 
            "SendResponseL: content type", *aResponse.iContentType );
        content = iSession.PopClientContent();
        if ( content )
            {
            CleanupStack::PushL( content );            
            contentType = MceSip::ToContentTypeHeaderL( 
                                                EMceContentTypeUserDefined, 
                                                aResponse.iContentType );
            CleanupStack::PushL( contentType );
            }
        else
            {
            MCESRV_DEBUG( "SendResponseL: no content");
            }
        }
    else
        {
        MCESRV_DEBUG( "SendResponseL: no content");
        }
    

    if ( content )
        {
        elements.SetContentL( content, contentType );
        CleanupStack::Pop( contentType );
        CleanupStack::Pop( content );
        }
    
      
    if ( reliable )
        {
        CMceReliableSender& reliableSend = iSession.ReliableSender();
        reliableSend.SendResponseL( aRequest, response ); 
        CleanupStack::Pop( response );
        }
        
    else
        {
        aRequest.SendResponseL( response );
        CleanupStack::Pop( response ); 
        MCESRV_DEBUG("TMceActionSet::SendResponseL( custom ), Exit");
        }
        
    if ( aResponse.iContentTypeId == EMceContentTypeMultiPart || 
         aResponse.iContentTypeId == EMceContentTypeSDP )
        {
        iSession.IncrementSentSdpCount();
        }
        
    return reliable;                               
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendACKL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendACKL( CSIPClientTransaction& aResponse )
    {
    MCESRV_DEBUG("TMceActionSet::SendACKL, Entry");
    
    CSIPMessageElements* ack = CSIPMessageElements::NewLC();
    
    AddHeadersL( *ack, NULL, SipStrConsts::EAck );
    static_cast<CSIPInviteDialogAssoc*>
        ( iSession.Dialog() )->SendAckL( aResponse, ack );
    CleanupStack::Pop( ack );
    MCESRV_DEBUG("TMceActionSet::SendACKL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendBYEL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendBYEL()
    {
    MCESRV_DEBUG("TMceActionSet::SendBYEL, Entry");
    
    CSIPMessageElements* bye = CSIPMessageElements::NewLC();
    AddHeadersL( *bye, NULL, SipStrConsts::EBye );
    
    CSIPClientTransaction* trx = static_cast<CSIPInviteDialogAssoc*>
        ( iSession.Dialog() )->SendByeL( bye );
    CleanupStack::Pop( bye );
    CleanupStack::PushL( trx );
    iSession.SetPendingTransactionL( trx );
    CleanupStack::Pop( trx );
    
    MCESRV_DEBUG("TMceActionSet::SendBYEL, Exit");    
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendBYEL
// -----------------------------------------------------------------------------
//
void TMceActionSet::SendBYEL( CMceMsgBase& aData )
    {
    MCESRV_DEBUG("TMceActionSet::SendBYEL(data), Entry");
    
    CMceMsgSIPData* data = static_cast<CMceMsgSIPData*>( &aData );
    
    if ( !data )
        {
        SendBYEL();
        return;
        }
        
    CSIPMessageElements* bye = CSIPMessageElements::NewLC();
    
    AddHeadersL( *bye, data->iSIPHeaders, SipStrConsts::EBye );

    CSIPContentTypeHeader* contentType = NULL;
    HBufC8* content = iSession.PopClientContent();
    if ( MCE_HAS_DATA( content ) )
        {
        CleanupStack::PushL( content );            
        MCESRV_DEBUG_SVALUE( "SendBYEL: content size", content->Length() );
        if ( MCE_HAS_DATA( data->iContentType ) )
            {
            MCESRV_DEBUG_SVALUE( "SendBYEL: content type", *data->iContentType );
            contentType = MceSip::ToContentTypeHeaderL( 
                                                    EMceContentTypeUserDefined, 
                                                    data->iContentType );
            CleanupStack::PushL( contentType );
            }
        else
            {
            MCESRV_DEBUG( "SendBYEL: no content type -> no content" );
            CleanupStack::PopAndDestroy( content );
            content = NULL;
            }
        }
    else
        {
        delete content;
        content = NULL;
        }
    
    if ( content )
        {
        bye->SetContentL( content , contentType );
        CleanupStack::Pop( contentType );
        CleanupStack::Pop( content );
        }

    aData.Close();
    
    CSIPClientTransaction* trx = static_cast<CSIPInviteDialogAssoc*>
        ( iSession.Dialog() )->SendByeL( bye );
    CleanupStack::Pop( bye );
    CleanupStack::PushL( trx );
    iSession.SetPendingTransactionL( trx );
    CleanupStack::Pop( trx );
    
    MCESRV_DEBUG("TMceActionSet::SendBYEL(data), Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::Send200OKL
// -----------------------------------------------------------------------------
//
void TMceActionSet::Send200OKL( CSIPServerTransaction& aRequest )
    {
    MCESRV_DEBUG("TMceActionSet::Send200OKL, Entry");

    CMceMsgSIPReply* reply = new (ELeave) CMceMsgSIPReply();
    CleanupStack::PushL( reply );
    reply->iCode = KMceSipOK;
    reply->iReasonAsPoolIndex = SipStrConsts::EPhraseOk;
        
    SendResponseL( aRequest, *reply );

    CleanupStack::PopAndDestroy( reply );

    MCESRV_DEBUG("TMceActionSet::Send200OKL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::SendCancelL
// -----------------------------------------------------------------------------
//
TBool TMceActionSet::SendCancelL()
    {
    MCESRV_DEBUG("TMceActionSet::SendCancelL, Entry");
    TBool cancelSent = ETrue;
    
    CSIPClientTransaction* inviteTransaction = iSession.InviteTransaction();
    
    __ASSERT_ALWAYS( inviteTransaction, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( inviteTransaction->CancelAllowed(), 
        User::Leave( KErrGeneral ) );

    CSIPTransactionBase::TState trxState = 
        MceSip::TrxState( *inviteTransaction );
    
    if ( trxState == CSIPTransactionBase::EProceeding )
        {
        CSIPClientTransaction* trx = inviteTransaction->CancelL();
        CleanupStack::PushL( trx );
        iSession.SetPendingTransactionL( trx );
        CleanupStack::Pop( trx );
        }
    else
        {
        MCESRV_DEBUG_DVALUE(
            "SendCancelL: CANCEL is not sent. Trx state", trxState );
        cancelSent = EFalse;
        }
        
    MCESRV_DEBUG("TMceActionSet::SendCancelL, Exit");
    
    return cancelSent;
    
    }

// -----------------------------------------------------------------------------
// TMceActionSet::InterruptConfirming
// -----------------------------------------------------------------------------
//
TBool TMceActionSet::InterruptConfirming()
    {
    MCESRV_DEBUG("TMceActionSet::InterruptConfirming, Entry");
    
    TBool interrupted = EFalse;
    CSIPClientTransaction* pending = iSession.PendingTransaction(
                SIPStrings::StringF( SipStrConsts::EUpdate ) );
    pending = ( pending && !MceSip::TrxCompleted( *pending ) 
        ? pending : NULL );
    
    if ( !pending )
        {
        MCESRV_DEBUG("TMceActionSet::InterruptConfirming, search PRACK");
        // no UPDATE so search for PRACK
        pending = iSession.PendingTransaction(
            SIPStrings::StringF( SipStrConsts::EPrack ) );
        pending = ( pending && !MceSip::TrxCompleted( *pending ) 
            ? pending : NULL );
        }
    
    if ( pending )
        {
        MCESRV_DEBUG("TMceActionSet::InterruptConfirming, removing");
        iSession.RemovePendingTrx( *pending );
        delete pending;
        interrupted = ETrue;
        }
    
    MCESRV_DEBUG("TMceActionSet::InterruptConfirming, Exit");
    return interrupted;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ControlMediaL
// -----------------------------------------------------------------------------
//
HBufC8* TMceActionSet::ControlMediaL( TMceIds& aIds, TMceItcFunctions aAction )
                                   
    {
    MCESRV_DEBUG("TMceActionSet::ControlMediaL, Entry");
    MCESRV_DEBUG_EVENT("action", aAction );
    
    __ASSERT_ALWAYS( iSession.iBody, User::Leave( KErrNotReady ) );
    
    HBufC8* returnMessage = NULL;
    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    
    TMceComEventHandler handler( mediaManager, returnMessage );
    TMceComEvent event( aIds, NULL, aAction, handler );
    
    ActiveBody().EventReceivedL( event );
    
    MCESRV_DEBUG("TMceActionSet::ControlMediaL, Exit");
    
    return returnMessage;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ControlMediaL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ControlMediaL( TMceIds& aIds, TMceItcFunctions aAction,
                                   const TDesC8& aMessage )
    {
    MCESRV_DEBUG("TMceActionSet::ControlMediaL, Entry");
    MCESRV_DEBUG_EVENT("action", aAction );
    
    __ASSERT_ALWAYS( iSession.iBody, User::Leave( KErrNotReady ) );
    
    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    
    TMceComEventHandler handler( mediaManager, aMessage );
    TMceComEvent event( aIds, NULL, aAction, handler );
    
    ActiveBody().EventReceivedL( event );
    
    MCESRV_DEBUG("TMceActionSet::ControlMediaL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ControlMediaL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ControlMediaL( TMceIds& aIds, TMceItcFunctions aAction,
                                   TPtr8& aMessage )
    {
    MCESRV_DEBUG("TMceActionSet::ControlMediaL, Entry");
    MCESRV_DEBUG_EVENT("action", aAction );
    
    __ASSERT_ALWAYS( iSession.iBody, User::Leave( KErrNotReady ) );
    
    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    
    TMceComEventHandler handler( mediaManager, aMessage );
    TMceComEvent event( aIds, NULL, aAction, handler );
    
    ActiveBody().EventReceivedL( event );
    
    MCESRV_DEBUG("TMceActionSet::ControlMediaL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::StopMedia
// -----------------------------------------------------------------------------
//
void TMceActionSet::StopMedia()
    {
    MCESRV_DEBUG("TMceActionSet::StopMedia, Entry");

    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    
    if ( iSession.BodyCandidate() )
        {
        mediaManager.CloseSession( *iSession.BodyCandidate() );
        }
    if ( iSession.Body() )
        {
        iSession.CloseMedia();
        }
    
    MCESRV_DEBUG("TMceActionSet::StopMedia, Exit");
    }
   
// -----------------------------------------------------------------------------
// TMceActionSet::StopUpdatingMedia
// -----------------------------------------------------------------------------
// 
void TMceActionSet::StopUpdatingMedia()
    {
    MCESRV_DEBUG("TMceActionSet::StopUpdatingMedia, Entry");

    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    
    if ( iSession.BodyCandidate() && 
         iSession.SubState() == CMceSipSession::EUpdating)
        {
        mediaManager.CloseSession( *iSession.BodyCandidate() );
        }
    
    MCESRV_DEBUG("TMceActionSet::StopUpdatingMedia, Exit");   
    }
 
// -----------------------------------------------------------------------------
// TMceActionSet::ClientMediaStateChanged
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientMediaStateChangedL( TMceMccComEvent* aMccEvent,
                                             TMceMediaEventCode aEvent )
    {
    MCESRV_DEBUG("TMceActionSet::ClientMediaStateChanged, Entry");

    if ( aMccEvent &&
         aMccEvent->iItcEvent != EMceItcNotAssigned && 
         aMccEvent->iStream )
        {
		
        if ( aEvent == EMceMediaError && aMccEvent->iError == KErrHostUnreach )
        	{
        	// If there have been too many ICMP errors, pause the stream.
    	    const RPointerArray< CMceComMediaSink >& sinks = aMccEvent->iStream->Sinks();
        
	        for ( TInt i=0 ; i<sinks.Count() ; ++i )
	        	{
	        	if ( sinks[i]->MccType() == KUidMccRtpSink )
	        		{
		            iSession.Manager().MediaManager().PauseL( *sinks[i] );
	        		}
	        	}
       	
        	}
        	
        TMceIds ids;
        ids.iCallbackID = aMccEvent->iItcEvent;
    	ids.iMediaID = aMccEvent->iStream->Id();
    	ids.iSinkID = aMccEvent->iSink ? aMccEvent->iSink->Id() : KMceMediaIdNotAssigned;
    	ids.iSourceID = aMccEvent->iSource ? aMccEvent->iSource->Id() : KMceMediaIdNotAssigned;
    	ids.iState = aMccEvent->iItcData;
    	ids.iStatus = aMccEvent->iError;

        iSession.SendToClient( ids );
        }
    else
        {
        MCESRV_DEBUG("ClientMediaStateChanged: No target");    
        }
        
    MCESRV_DEBUG("TMceActionSet::ClientMediaStateChanged, Exit");
    }
    
// -----------------------------------------------------------------------------
// TMceActionSet::ClientErrorOccured
// -----------------------------------------------------------------------------
//
void TMceActionSet::ClientErrorOccured( TInt aError )
    {
    MCESRV_DEBUG("TMceActionSet::ClientErrorOccured, Entry");
    MCESRV_DEBUG_DVALUE("error", aError );
    
    TMceIds ids;
    
    ids.iCallbackID = EMceItcStateChanged;
	ids.iState = CMceSession::ETerminated;
	
    iSession.SendErrorToClient( ids, aError );

    MCESRV_DEBUG("TMceActionSet::ClientErrorOccured, Exit");
    }

 
//
// SDP manipulation
//    
    
// -----------------------------------------------------------------------------
// TMceComEventHandler::CreateMultipartContentL
// -----------------------------------------------------------------------------
//
HBufC8* TMceActionSet::CreateMultipartContentLC( CSdpDocument& aDocument,
                	                          const TDesC8& aMixedContentType, 
                	                          const TDesC8& aMixedContent )
    {
    MCESRV_DEBUG("TMceActionSet::CreateMultipartContentL, Entry");
    HBufC8* sdpOffer = CreateSDPContentLC( aDocument );
    
    const TDesC8& sdp = *sdpOffer;
    TInt length = KMceSipMPartMessagePattern().Length() + sdp.Length() + 
        aMixedContentType.Length() + aMixedContent.Length();
	HBufC8* content = HBufC8::NewL( length );
    
	content->Des().AppendFormat( KMceSipMPartMessagePattern, 
	                             &sdp, 
	                             &aMixedContentType, 
	                             &aMixedContent );
	                             
	CleanupStack::PopAndDestroy( sdpOffer );
    CleanupStack::PushL( content );
    
    MCESRV_DEBUG("TMceActionSet::CreateMultipartContentL, Exit");
	
    return content;
    }
   
// -----------------------------------------------------------------------------
// TMceActionSet::CreateSDPContentLC
// -----------------------------------------------------------------------------
//
HBufC8* TMceActionSet::CreateSDPContentLC( CSdpDocument& aDocument )
    {
    MCESRV_DEBUG("TMceActionSet::CreateSDPContentLC, Entry");
    
    CBufFlat* encBuf = CBufFlat::NewL( KMaxLengthOfSDPMessage );
    CleanupStack::PushL( encBuf );
    RBufWriteStream writeStream;
    writeStream.Open( *encBuf ); 
    aDocument.EncodeL( writeStream );
    writeStream.Close();
    TPtr8 encBufPtr = encBuf->Ptr(0);
    HBufC8* sdpBuf = sdpBuf = encBufPtr.AllocL();
    CleanupStack::PopAndDestroy( encBuf );
    CleanupStack::PushL( sdpBuf );
    
    MCESRV_DEBUG("TMceActionSet::CreateSDPContentLC, Exit");
    
    return sdpBuf;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::AddHeadersL
// -----------------------------------------------------------------------------
//
void TMceActionSet::AddHeadersL( CSIPMessageElements& aMessageElements, 
                                 CDesC8Array* aPrimaryHeaders,
                                 TInt aMethodInd ) 
    {
    RPointerArray<CSIPHeaderBase> headers;
    CSIPHeaderBase::PushLC( &headers );
    TInt allowInd = SipStrConsts::EAllowHeader;
    AddHeadersL( headers, aPrimaryHeaders, aMethodInd );
    
     //if allow header is there add all UA supported headers
        
    CSIPHeaderBase* allowHeader = MceSip::FindHeader( headers, 
    		SIPStrings::StringF( SipStrConsts::EAllowHeader ));		
    if ( allowHeader )
    	{
    	RArray<TInt> allowMethods;
    	CleanupClosePushL(allowMethods);
    	TInt index =0;
    	MceSip::BasicAllowMethodsL( allowMethods );
	    
	    for ( index =0; index < allowMethods.Count(); index ++)
	    	{
	   		 if ( !MceSip::HasHeaderValueL( aMessageElements, allowInd, 
	   		 					SIPStrings::StringF( allowMethods[ index ]).DesC() ))
	    		{
	        	MceSip::AddAllowHeaderL( headers, allowMethods[ index ]);
	    		}
	    	
	    	}
	    CleanupStack::PopAndDestroy(&allowMethods);
    	}
    aMessageElements.SetUserHeadersL( headers );
    CleanupStack::Pop( &headers );
    }

// -----------------------------------------------------------------------------
// TMceActionSet::AddHeadersL
// -----------------------------------------------------------------------------
//
void TMceActionSet::AddHeadersL( RPointerArray<CSIPHeaderBase>& aHeaders,
                                 CDesC8Array* aPrimaryHeaders,
                                 TInt aMethodInd ) 
    {
	
    TInt allowInd = SipStrConsts::EAllowHeader;
    RArray< TInt > allowMethods;
    CleanupClosePushL(allowMethods);
	TInt index =0;
	
    CDesC8Array& txtHeaders = aPrimaryHeaders && aPrimaryHeaders->Count() > 0 ? 
        *aPrimaryHeaders : iSession.DefaultHeaders( aMethodInd );
    if ( txtHeaders.Count() )
        {
        MCESRV_DEBUG("AddHeadersL: creating extension headers");
        MceSip::ToSIPExtensionHeadersL( aHeaders, txtHeaders );
        }
    //An Allow header field (Section 20.5) SHOULD be present in the INVITE.
	//It indicates what methods can be invoked within a dialog
    MceSip::BasicAllowMethodsL( allowMethods );    
    for ( index =0; index < allowMethods.Count(); index ++)
    	{
   		 if ( !MceSip::HasHeaderValueL( aHeaders, allowInd, 
   		 					SIPStrings::StringF( allowMethods[ index ]).DesC() ))
    		{
        	MceSip::AddAllowHeaderL( aHeaders, allowMethods[ index ]);
    		}
    	
    	}
    CleanupStack::PopAndDestroy(&allowMethods);  
    }

// -----------------------------------------------------------------------------
// TMceActionSet::AddExtensionHeadersL
// -----------------------------------------------------------------------------
//
TBool TMceActionSet::AddExtensionHeadersL( 
                                    RPointerArray<CSIPHeaderBase>& aHeaders,
                                    TInt aTransactionType,
                                    TBool aIsBody,
                                    TBool aIsResponse,
                                    TUint aResponseCode )
    {
    CMceSipExtensions& ext = iSession.Extensions();
    TMceSipResponseType type = MceSip::ResponseType( aResponseCode );
    
    TInt reliable = EFalse;
    if ( aTransactionType == SipStrConsts::EInvite )
        {
        // Allow update, subscribe, prack, refer, notify
        // extension methods for allow header
        RArray<TInt> allowExts;
        CleanupClosePushL(allowExts);
        RArray< CMceSipExtensions::TExtension > localExts;
        CleanupClosePushL(localExts);
    	TInt allowInd = SipStrConsts::EAllowHeader;
    	TInt index =0;
    	
    	ext.ExtensionAllowMethodsL( allowExts );
    	ext.LocalSipExtensionsL( localExts );
    	//Adding headers only if the headers does not contains the methods
    	for( index = 0; index < allowExts.Count(); index++ )
    		{
    		TInt localExt = ext.Local( localExts[ index ] );
	        if ( localExt != CMceSipExtensions::ENotSupported &&
	            ( !aIsResponse || ( type == E1XX && aIsBody  ) ) &&
	            !MceSip::HasHeaderValueL( aHeaders, allowInd, 
	   		 			SIPStrings::StringF( allowExts[ index ]).DesC() ))
	            {
	            // INVITE or 1XX to INVITE with SDP
	            MceSip::AddAllowHeaderL( aHeaders, allowExts[ index ] );
	            }
    		}
    	CleanupStack::PopAndDestroy(&localExts);
    	CleanupStack::PopAndDestroy(&allowExts);
        //clean arrays
        
        // Supported: secprecondition
        TInt localSecPreconds = ext.Local( CMceSipExtensions::ESecPreconds );
        
        TBool supportedPreconds = MceSip::HasHeaderValueL( 
                                        aHeaders, 
                                        SipStrConsts::ESupportedHeader,
                                        KMceSipPrecondition );
                                        
        TBool reqPreconds = MceSip::HasHeaderValueL( 
                                        aHeaders, 
                                        SipStrConsts::ERequireHeader,
                                        KMceSipPrecondition );                                
        
        if ( localSecPreconds != CMceSipExtensions::ENotSupported &&
           ( !aIsResponse || type == E1XX || type == E2XX )  &&
        	ActiveBody().Modifier( KMceSecPreconditions ) == KMcePreconditionsE2ESupported &&
        	!supportedPreconds )
            {
            MceSip::AddSupportedHeaderL( aHeaders, KMceSipPrecondition ); 
            supportedPreconds = ETrue; 
            }
            
        // Supported: 100rel
        TInt local100rel = ext.Local( CMceSipExtensions::E100rel );
        if ( local100rel != CMceSipExtensions::ENotSupported &&
           ( !aIsResponse || type == E1XX ) )
            {
            // INVITE or 1XX to INVITE
            MceSip::AddSupportedHeaderL( aHeaders, KMceSip100Rel );
            }
        
        // Supported: precondition
        TInt localQosPreconds = ext.Local( CMceSipExtensions::EQosPreconds );
        if ( localQosPreconds != CMceSipExtensions::ENotSupported &&
           ( !aIsResponse || type == E1XX || type == E2XX ) && 
           	!supportedPreconds )
            {
            // INVITE or 1XX or 2XX to INVITE
            MceSip::AddSupportedHeaderL( aHeaders, KMceSipPrecondition );
            }
        
        // Require: 100rel
        if ( !aIsResponse && local100rel == CMceSipExtensions::ERequired )
            {
            MceSip::AddRequireHeaderL( aHeaders, KMceSip100Rel );    
            }
        
        // Require: precondition
        if ( !aIsResponse && localQosPreconds == CMceSipExtensions::ERequired &&
        	!reqPreconds )
            {
            MceSip::AddRequireHeaderL( aHeaders, KMceSipPrecondition );  
            reqPreconds = ETrue;  
            }
        
         // Require: precondition
        if ( !aIsResponse && localSecPreconds == CMceSipExtensions::ERequired &&
        	!reqPreconds )
            {
            MceSip::AddRequireHeaderL( aHeaders, KMceSipPrecondition );    
            }
            
        if ( aIsResponse && type == E1XX )
            {
            // Find out are we sending a 1XX reliably
            TInt remote100rel = ext.Remote( CMceSipExtensions::E100rel );
            if ( local100rel == CMceSipExtensions::ERequired || 
                 remote100rel == CMceSipExtensions::ERequired || 
                 aIsBody  )
                {
                reliable = ETrue;
                // Add headers for reliable provisional response
                // Require: 100rel
                MceSip::AddRequireHeaderL( aHeaders, KMceSip100Rel );
               
                // RSeq
                TUint rseqValue;
                CMceReliableSender& reliableSend = iSession.ReliableSender();
                CSIPServerTransaction& serverTrans = iSession.Request();
                rseqValue = reliableSend.Rseq( serverTrans );
                CSIPRSeqHeader* rseq = new( ELeave ) CSIPRSeqHeader( rseqValue );
                CleanupStack::PushL( rseq );
                aHeaders.AppendL( rseq );
                CleanupStack::Pop( rseq );
              
                }
          	if ( aIsBody && !reqPreconds && 
          	   ActiveBody().Modifier( KMceSecPreconditions ) == KMcePreconditionsE2ESupported )
                {
                // Require: precondition 
                MceSip::AddRequireHeaderL( aHeaders, KMceSipPrecondition );
                ActiveBody().iRemoteSecPreconditionsRequired = ETrue;
                reqPreconds = ETrue;
                }   
            }
        }
    else if ( !aIsResponse && aTransactionType == SipStrConsts::EPrack )
        {
        // RAck header in PRACK request
        MceSip::AddRAckHeaderL( aHeaders, 
                                *iSession.Response().ResponseElements() );
                                
        TBool supportedPreconds = MceSip::HasHeaderValueL( 
                                        aHeaders, 
                                        SipStrConsts::ESupportedHeader,
                                        KMceSipPrecondition );
        TBool reqPreconds = MceSip::HasHeaderValueL( 
                                        aHeaders, 
                                        SipStrConsts::ERequireHeader,
                                        KMceSipPrecondition );                                      
        TInt localSecPreconds = ext.Local( CMceSipExtensions::ESecPreconds );
        if ( localSecPreconds != CMceSipExtensions::ENotSupported &&
           !aIsResponse  && !supportedPreconds &&
        	ActiveBody().Modifier( KMceSecPreconditions ) == KMcePreconditionsE2ESupported )
            {
            MceSip::AddSupportedHeaderL( aHeaders, KMceSipPrecondition );  
            }
        if ( localSecPreconds != CMceSipExtensions::ENotSupported &&
           !aIsResponse  && !reqPreconds &&
        	ActiveBody().Modifier( KMceSecPreconditions ) == KMcePreconditionsE2ESupported &&
        	ActiveBody().iRemoteSecPreconditionsRequired )
            {
            MceSip::AddRequireHeaderL( aHeaders, KMceSipPrecondition );  
            }                                     
        }
    else
        {
        //NOP
        }
    return reliable;
    }
 
//
// Session timers
//

// -----------------------------------------------------------------------------
// TMceActionSet::ProcessSessionTimerClientL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ProcessSessionTimerClientL( 
                                            CSIPClientTransaction& aResponse )
    {
    MCESRV_DEBUG("TMceActionSet::ProcessSessionTimerClientL, Entry");

    const TInt KPopCount = 4;
    
    const CSIPMessageElements& msgElements = aResponse.
        ResponseElements()->MessageElements();
    const RPointerArray<CSIPHeaderBase>& sipHeaders = msgElements.UserHeaders();
    
    RStringF expiresKey = 
        SIPStrings::Pool().OpenFStringL( KMceSipHeaderSessionExpires() );
    CleanupClosePushL( expiresKey );

    RStringF expiresKey2 = 
        SIPStrings::Pool().OpenFStringL( KMceSipHeaderCompactFormOfSessionExpires() );
    CleanupClosePushL( expiresKey2 );
    
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() );
	CleanupClosePushL( timerKey );
	
	RStringF minSeKey = SIPStrings::Pool().OpenFStringL( KMceSipHeaderMinSE() );
	CleanupClosePushL( minSeKey );

	TBool require( EFalse ); 
	
	// Require: timer
    for (int i = 0; i < sipHeaders.Count(); i++)
		{
		CSIPHeaderBase* anotherhead = sipHeaders[i]; 
		HBufC8* value = anotherhead->ToTextValueLC(); 
		if( anotherhead->Name() == 
		    SIPStrings::StringF(SipStrConsts::ERequireHeader ) 
		    && value 
		    && value->Find( KMceSipTimer() ) != KErrNotFound )
			{
			require = ETrue;
			}
		else if( minSeKey == anotherhead->Name() && 
			     MceSip::ResponseCode ( aResponse ) == 
			     KMceSipSessionIntervalTooSmall )
			{
			// 422 Response received including Min-SE field
			TUint uintValue = MceSip::HeaderValueL( value->Des() );
			ActiveBody().iMinSE = uintValue;
			ActiveBody().iTimeout = ActiveBody().iMinSE;
			CleanupStack::PopAndDestroy( value );
			break;
			}
		CleanupStack::PopAndDestroy( value );
		}
	if( require )
		{
		// look for Session-Expires field
	    for (int i = 0; i < sipHeaders.Count(); i++)
			{
			CSIPHeaderBase* anotherhead = sipHeaders[i];
			if( ( expiresKey == anotherhead->Name() ||
                 expiresKey2 == anotherhead->Name() ) && 
			    MceSip::ResponseCode ( aResponse ) == KMceSipOK )		
				{
				// Session-Expires: value;refresher=uac/uas
				HBufC8* value = anotherhead->ToTextValueLC();
				
            	//Need to have ; i.e. must contain refresher=uac/uas
            	User::LeaveIfError( value->Locate( KMceSipHeaderSeparator ) );
            	
				TUint uintValue =  MceSip::HeaderValueL( value->Des() );
				ActiveBody().iTimeout = uintValue;
				// find out who will perform the refresh requests
				if( value->FindF( KMceSipRefresher ) != KErrNotFound &&
					value->FindF( KMceSipRefresherUAS ) == KErrNotFound )
					{
					ActiveBody().iRefresh = ETrue;
					}
				else 
				 	{
					ActiveBody().iRefresh = EFalse;
					}
				CleanupStack::PopAndDestroy( value );
				iSession.StartSessionTimerL();
				break;
				}
			}
		}
		
	CleanupStack::PopAndDestroy( KPopCount ); // expiresKey, expiresKey2, timerKey, minSeKey
	
    MCESRV_DEBUG("TMceActionSet::ProcessSessionTimerClientL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ProcessSessionTimerServerL
// -----------------------------------------------------------------------------
//
void
TMceActionSet::ProcessSessionTimerServerL( CSIPServerTransaction& aRequest )
	{
    const CSIPRequestElements* reqElements = aRequest.RequestElements();
    const CSIPMessageElements* elements = reqElements ? 
                                          &reqElements->MessageElements() :
                                          NULL;
	if ( elements )
		{
		ProcessSessionTimerServerL( *elements );
		}
	}
	
// -----------------------------------------------------------------------------
// TMceActionSet::ProcessSessionTimerServerL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ProcessSessionTimerServerL( 
        const CSIPMessageElements& aElements )
	{
    MCESRV_DEBUG("TMceActionSet::ProcessSessionTimerServerL, Entry");
    
    const TInt KPopCount = 3;
	
	RStringF expiresKey = SIPStrings::Pool().
	    OpenFStringL( KMceSipHeaderSessionExpires() );
	CleanupClosePushL( expiresKey );

	RStringF expiresKey2 = SIPStrings::Pool().
		OpenFStringL( KMceSipHeaderCompactFormOfSessionExpires() );
	CleanupClosePushL( expiresKey2 );

	RStringF minseKey = SIPStrings::Pool().
	    OpenFStringL( KMceSipHeaderMinSE() );
	CleanupClosePushL( minseKey );
	
	CSIPHeaderBase* header = NULL;
	HBufC8* sessionExpires = NULL;
	
	// look for Session-Expires: field, if not present then no session timer
	ActiveBody().iTimeout = 0;
	
	header = MceSip::FindHeader( aElements, expiresKey );
	header = header ? header : ( MceSip::FindHeader( aElements, expiresKey2 ) );
	sessionExpires = header ? header->ToTextValueLC() : NULL;
	
	if ( sessionExpires )
	    {
	    TUint timeout = MceSip::HeaderValueL( sessionExpires->Des() );
		ActiveBody().iTimeout = timeout;
        
	    // Find out who will perform the refresh requests. Refresher role is 
	    // only updated when UAC has provided refresher-parameter or incoming 
	    // request is not a session refresh.
	    if ( iSession.SubState() != CMceSipSession::ERefreshing ||
	         sessionExpires->Find( KMceSipRefresherUAC ) != KErrNotFound ||
	         sessionExpires->Find( KMceSipRefresherUAS ) != KErrNotFound )
	        {
    		if ( sessionExpires->Find( KMceSipRefresherUAC ) != KErrNotFound )
    			{
    			ActiveBody().iRefresh = EFalse;
    			}
    		else 
    		 	{
    			ActiveBody().iRefresh = ETrue;
    			}
	        }
        
		CleanupStack::PopAndDestroy( sessionExpires );
	    }
	
	CleanupStack::PopAndDestroy( KPopCount ); // expiresKey, expiresKey2, minseKey
	
    MCESRV_DEBUG("TMceActionSet::ProcessSessionTimerServerL, Exit");
	}
	
// -----------------------------------------------------------------------------
// TMceActionSet::ProcessReliableResponseL   
// -----------------------------------------------------------------------------
//
TBool TMceActionSet::ProcessReliableResponse( 
        const CSIPResponseElements& aResponseElements )
    {
    TUint value = MceSip::RSeq( aResponseElements );
    
    if ( ( iSession.RSeq() == KErrNone )  || 
        ( iSession.RSeq() == value - 1 ) )
        {
        return ETrue;
        }
    
    return EFalse;               
    }
    
// -----------------------------------------------------------------------------
// TMceActionSet::NeedToProcess   
// -----------------------------------------------------------------------------
//
TBool TMceActionSet::NeedToProcessL( TMceStateTransitionEvent& aEvent )    
	{
	
	TBool needToProcess = ETrue;
	
	CMceSipSession& session = aEvent.Session();
    CSIPClientTransaction& response = session.Response();
            
	
	if ( MceSip::ResponseCode( response ) == KMceSipTrying )
		{
		needToProcess = EFalse;
		}
	else if ( (session.Extensions().LevelL( response, CMceSipExtensions::E100rel ) 
		        == CMceSipExtensions::ERequired ) )
		{
    	const CSIPResponseElements& responseElements = *response.ResponseElements();
	
		TInt rseq = MceSip::RSeq( responseElements );
     
       	if (rseq == KErrNotFound )
        	{
           	User::Leave( KErrArgument );
       		}	

		if ( session.RSeq() == KErrNone || 
       		 rseq > session.RSeq() )
       		       		
			{
			needToProcess = session.Actions().ProcessReliableResponse(
				*response.ResponseElements());
			}
		else 
			{
			needToProcess = EFalse;
			}
		}
		      	       		
	return needToProcess;
	}

// -----------------------------------------------------------------------------
// TMceActionSet::CheckContactIsSecureL
// -----------------------------------------------------------------------------    
//
void TMceActionSet::CheckContactIsSecureL( CSIPTransactionBase& aTransaction )
	{
	MCESRV_DEBUG("TMceActionSet::CheckResponseContactIsSecureL, Entry");
	
	const CSIPMessageElements* msg = MceSip::ToMessageElements( aTransaction );
    User::LeaveIfError( msg ? KErrNone : KErrArgument );
    
    TBool contactSec = MceSip::HasSipsUriInContactL( *msg ) ||
		 			   MceSip::HasTlsInContactL( *msg );
	
	CMceComSession& body = ActiveBody();
	body.iSipContactAddrSecure = CMceSession::EControlPathUnsecure;	 			   
	
	if ( iSession.IsProfileContactSecureL( ) && 
    	 contactSec  )
		{
		MCESRV_DEBUG("profile contact and session contact header is secure");
		body.iSipContactAddrSecure = CMceSession::EControlPathSecure;
		}
	MCESRV_DEBUG("TMceActionSet::CheckResponseContactIsSecureL, Exit");
	}


// -----------------------------------------------------------------------------
// TMceActionSet::IsExtensionRequest
// -----------------------------------------------------------------------------

TBool TMceActionSet::IsExtensionRequest ( RStringF method )
	{

	TBool extensionRequest = ETrue;

	if ( ( method == SIPStrings::StringF( SipStrConsts::ERegister )) ||
		 ( method == SIPStrings::StringF( SipStrConsts::EInvite )) ||
		 ( method == SIPStrings::StringF( SipStrConsts::ECancel)) ||
		 ( method == SIPStrings::StringF( SipStrConsts::EPrack )) ||
		 ( method == SIPStrings::StringF( SipStrConsts::EUpdate )) ||
		 ( method == SIPStrings::StringF( SipStrConsts::EAck )) ||
		 ( method == SIPStrings::StringF( SipStrConsts::EBye )) ||
		 ( method == SIPStrings::StringF( SipStrConsts::ESubscribe )) ||
		 ( method == SIPStrings::StringF( SipStrConsts::ERefer )) ||
		 ( method == SIPStrings::StringF( SipStrConsts::ENotify )))
		 

		{
		extensionRequest = EFalse;	
		}
		
	return extensionRequest;	
	}


// -----------------------------------------------------------------------------
// TMceActionSet::ResetSessionTimerL
// -----------------------------------------------------------------------------
//
void TMceActionSet::ResetSessionTimerL()
    {
    MCESRV_DEBUG("TMceActionSet::ResetSessionTimerL, Entry");
    
    if ( !ActiveBody().iRefresh )
        {
        iSession.StartSessionTimerL();
        }
        
    MCESRV_DEBUG("TMceActionSet::ResetSessionTimerL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::StopPendingTimer
// -----------------------------------------------------------------------------
//
void TMceActionSet::StopPendingTimer()
    {
    MCESRV_DEBUG("TMceActionSet::StopPendingTimer, Entry");
    CMceTimerManager& timerManager = iSession.Manager().TimerManager();
    if ( timerManager.IsRunning( iSession.iPendingTimer ) )
        {
        timerManager.Stop( iSession.iPendingTimer );
        }
    iSession.iPendingTimer = CMceTimerManager::KNoSuchTimer;    
    MCESRV_DEBUG("TMceActionSet::StopPendingTimer, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::AddSessionTimerFieldsServerL
// -----------------------------------------------------------------------------
// 
void TMceActionSet::AddSessionTimerFieldsServerL( 
        RPointerArray<CSIPHeaderBase>& aHeaders )
    {
    MCESRV_DEBUG("TMceActionSet::AddSessionTimerFieldsServerL, Entry");
    
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() );
	CleanupClosePushL( timerKey );
	// Supported: timer
    MCESRV_DEBUG_SVALUE("AddSessionTimerFieldsServerL: adding Supported header",
                        timerKey.DesC() );
	
	CSIPSupportedHeader* supportedHeader = 
	    CSIPSupportedHeader::NewLC( timerKey );
	
    User::LeaveIfError( aHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    
    if( ActiveBody().iTimeout > 0)
    	{
    	 // Require: timer
        MCESRV_DEBUG_SVALUE( 
            "AddSessionTimerFieldsServerL: adding Require header",
            timerKey.DesC() );
    	 
	    CSIPRequireHeader* requireHeader = 
	        CSIPRequireHeader::NewLC( timerKey );
	    
	    User::LeaveIfError( aHeaders.Append( requireHeader ) );
	    CleanupStack::Pop( requireHeader );
	    
	    // Session-Expires: XX
	    TUint timeoutInSec = ActiveBody().iTimeout;
	    
	    HBufC8* expiresValue = HBufC8::NewLC( KMaxLengthOfSessionExpires );
	    expiresValue->Des().Num( timeoutInSec, EDecimal );
	    expiresValue->Des().Append( KMceSipHeaderSeparator );
	    expiresValue->Des().Append( KMceSipRefresher );
	    expiresValue->Des().Append( KMceSipEqualsTo );
	    if( ActiveBody().iRefresh )
	   		{
	   		expiresValue->Des().Append( KMceSipRefresherUAS );
	   		}
	    else
			{
			expiresValue->Des().Append( KMceSipRefresherUAC );
			}	
			
        MCESRV_DEBUG_SVALUE(
            "AddSessionTimerFieldsServerL: adding Session-Expires header", 
            *expiresValue );
	    				
	    CSIPExtensionHeader* sessionExpiresHeader = 
	    	CSIPExtensionHeader::NewLC( KMceSipHeaderSessionExpires(), 
	    	*expiresValue );
	    	
	    User::LeaveIfError( aHeaders.Append( sessionExpiresHeader ) );
	    CleanupStack::Pop( sessionExpiresHeader );
		CleanupStack::PopAndDestroy( expiresValue );
    	}
    
	CleanupStack::PopAndDestroy(); // timerKey 

    MCESRV_DEBUG("TMceActionSet::AddSessionTimerFieldsServerL, Exit");
    }
    
// -----------------------------------------------------------------------------
// TMceActionSet::AddSessionTimerFieldsL
// -----------------------------------------------------------------------------
// 
void TMceActionSet::AddSessionTimerFieldsClientL( 
        RPointerArray<CSIPHeaderBase>& aSipHeaders  )
    {
    MCESRV_DEBUG("TMceActionSet::AddSessionTimerFieldsClientL, Entry");
    
    // Supported: timer 
    RStringF timerKey = SIPStrings::Pool().OpenFStringL( KMceSipTimer() );
	CleanupClosePushL( timerKey );
    
    MCESRV_DEBUG_SVALUE("AddSessionTimerFieldsClientL: adding Supported header", 
        timerKey.DesC() );
    
    CSIPSupportedHeader* supportedHeader = 
        CSIPSupportedHeader::NewLC( timerKey );
    
    User::LeaveIfError( aSipHeaders.Append( supportedHeader ) );
    CleanupStack::Pop( supportedHeader );
    CleanupStack::PopAndDestroy( &timerKey );
    if ( ActiveBody().iTimeout > 0 )
    	{
	    // Session-Expires: XX
	    TUint timeoutInSec = ActiveBody().iTimeout;
	    HBufC8* timeout = HBufC8::NewLC( KMaxLengthOfSessionExpires );
	    timeout->Des().Num( timeoutInSec, EDecimal );
        
        // Add refresher parameter if request is not the initial one. 
        // RFC4028, 7.4.
        if ( iSession.SubState() == CMceSipSession::ERefreshing )
            {
            timeout->Des().Append( KMceSipHeaderSeparator );
            timeout->Des().Append( KMceSipRefresher );
            timeout->Des().Append( KMceSipEqualsTo );
            
            if ( ActiveBody().iRefresh )
            	{
            	timeout->Des().Append( KMceSipRefresherUAC );
            	}
            else
            	{
            	timeout->Des().Append( KMceSipRefresherUAS );
            	}
            }
	    
	    CSIPExtensionHeader* sessionExpiresHeader = 
	        CSIPExtensionHeader::NewLC( KMceSipHeaderSessionExpires(), 
	                                    *timeout );
	    User::LeaveIfError( aSipHeaders.Append( sessionExpiresHeader ) );
	    CleanupStack::Pop( sessionExpiresHeader );
	    
	    if ( ActiveBody().iMinSE > 0 )
	    	{
	    	// if we have received 422, then include this field
            MCESRV_DEBUG_SVALUE(
                "AddSessionTimerFieldsClientL: adding Min-SE header", 
                *timeout );
	    	
	    	CSIPExtensionHeader* minSEHeader = CSIPExtensionHeader::NewLC(
	    	                                        KMceSipHeaderMinSE(),
	    						                    *timeout );
            
	    	User::LeaveIfError( aSipHeaders.Append( minSEHeader ) );
	    	CleanupStack::Pop( minSEHeader );
	    	}
		
		CleanupStack::PopAndDestroy( timeout );
    	}
    
    MCESRV_DEBUG("TMceActionSet::AddSessionTimerFieldsClientL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ReservationNeeded
// -----------------------------------------------------------------------------
// 
TBool TMceActionSet::ReservationNeeded()
    {
    MCESRV_DEBUG("TMceActionSet::ReservationNeeded, Entry");
    
    CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    TBool reservationNeeded = mediaManager.ReservationNeeded( ActiveBody() );
    
    MCESRV_DEBUG("TMceActionSet::ReservationNeeded, Exit");    
    
    return reservationNeeded;
    }

// -----------------------------------------------------------------------------
// TMceActionSet::ActiveBody
// -----------------------------------------------------------------------------
//
CMceComSession& TMceActionSet::ActiveBody()
    {
    return iSession.ActiveBody();
    }





//
// Event handler
//


// -----------------------------------------------------------------------------
// TMceComEventHandler::TMceComEventHandler
// -----------------------------------------------------------------------------
//
TMceComEventHandler::TMceComEventHandler( CMceMediaManager& aMediaManager )
    : iMediaManager( aMediaManager ),
      iWriteData( NULL ),
      iReadData( NULL ),
      iReturnData( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::TMceComEventHandler
// -----------------------------------------------------------------------------
//
TMceComEventHandler::TMceComEventHandler( CMceMediaManager& aMediaManager,
                                          TPtr8& aWriteData )
    : iMediaManager( aMediaManager ),
      iWriteData( &aWriteData ),
      iReadData( NULL ),
      iReturnData( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::TMceComEventHandler
// -----------------------------------------------------------------------------
//
TMceComEventHandler::TMceComEventHandler( CMceMediaManager& aMediaManager,
                                          const TDesC8& aReadData )
    : iMediaManager( aMediaManager ),
      iWriteData( NULL ),
      iReadData( &aReadData ),
      iReturnData( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::TMceComEventHandler
// -----------------------------------------------------------------------------
//
TMceComEventHandler::TMceComEventHandler( CMceMediaManager& aMediaManager,
                                          HBufC8*& aWriteData )
    : iMediaManager( aMediaManager ),
      iWriteData( NULL ),
      iReadData( NULL ),
      iReturnData( &aWriteData )
    {
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//
TInt TMceComEventHandler::HandleL( CMceComMediaSink& aTarget,
								   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( sink ), Entry");
    
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {
        case EMceItcEnable:
            {
    		MCESRV_DEBUG("resuming paused sink");
            iMediaManager.ResumeL( aTarget );
            aEvent.Id().iState = aTarget.IsEnabled();
            break;
            }
        case EMceItcDisable:
            {
    		MCESRV_DEBUG("pausing sink");
            iMediaManager.PauseL( aTarget );
            aEvent.Id().iState = aTarget.IsEnabled();
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            }
        }
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( sink ), Exit");
        
    return status;
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//
 TInt TMceComEventHandler::HandleL( CMceComMediaSource& aTarget, 
                                    TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( source ), Entry");
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {
        case EMceItcEnable:
            {
    		MCESRV_DEBUG("resuming paused source");	
            iMediaManager.ResumeL( aTarget );
            aEvent.Id().iState = aTarget.IsEnabled();
            break;
            }
        case EMceItcDisable:
            {
    		MCESRV_DEBUG("pausing source");
            iMediaManager.PauseL( aTarget );
            aEvent.Id().iState = aTarget.IsEnabled();
            break;
            }
        case EMceItcIsDtmfActive:
        case EMceItcStartDtmfTone:
        case EMceItcStopDtmfTone:
        case EMceItcSendDtmfTone:
        case EMceItcCancelSendDtmfToneSequence:
            {
            MCESRV_DEBUG("dtmf handling");
            iMediaManager.DtmfL( aTarget, aEvent );
            break;
            }
        case EMceItcSendDtmfToneSequence:
            {
            MCESRV_DEBUG("dtmf handling, sequence");
            __ASSERT_ALWAYS( MCE_NOT_NULL_PTR( iReadData ) , User::Leave( KErrNotFound ) );
            iMediaManager.DtmfL( aTarget, aEvent, *iReadData );
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            break;
            }
        }
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( source ), Exit");
    return status; 
    }
 
// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//
TInt TMceComEventHandler::HandleL( CMceComRtpSink& aTarget, 
                                   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( rtp sink ), Entry");
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {
        case EMceItcSendSR:
            {
    		MCESRV_DEBUG("sending SR");
            iMediaManager.SendRTCPSenderReportL( aTarget );
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            }
        }
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( rtp sink ), Exit");
    
    return status;
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//
TInt TMceComEventHandler::HandleL( CMceComRtpSource& aTarget, 
                                   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( rtp source ), Entry");
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {
        case EMceItcEnableInactivityTimer:
            {
    		MCESRV_DEBUG("enabling inactivity timer");
    		MCE_HANDLER_READ( TMceItcArgTUint32, TUint32, inactivityTimeout );
    		MCESRV_DEBUG_DVALUE("enabling inactivity timer. timeout", inactivityTimeout );
    		
            iMediaManager.StartInactivityTimerL( aTarget, inactivityTimeout ); 
            break;
            }
        case EMceItcDisableInactivityTimer:
            {
    		MCESRV_DEBUG("disabling inactivity timer");
            iMediaManager.StopInactivityTimerL( aTarget );
            break;
            }
        case EMceItcSendRR:
            {
    		MCESRV_DEBUG("sending RR");
            iMediaManager.SendRTCPReceiverReportL( aTarget );
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            }
        }
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( rtp source ), Exit");
    
    return status;
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//
TInt TMceComEventHandler::HandleL( CMceComSpeakerSink& aTarget, 
                                   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( speaker ), Entry");
        
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {
        case EMceItcVolume:
            {
            TInt volume = KErrNotFound;
            iMediaManager.GetVolumeL( aTarget, volume );
    		MCESRV_DEBUG_DVALUE("getting volume. volume", volume );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, volume );
            break;
            }
        case EMceItcAudioRouting:
            {
            TInt routing = KErrNotFound;
            iMediaManager.GetAudioRoutingL( aTarget, routing );
    		MCESRV_DEBUG_DVALUE("getting routing. value", routing );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, routing );
            break;
            }
        case EMceItcSetVolume:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, volume );
    		MCESRV_DEBUG_DVALUE("setting volume. new volume", volume );
            iMediaManager.SetVolumeL( aTarget, volume );
            break;
            }
        case EMceItcSetAudioRouting:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, routing );
    		MCESRV_DEBUG_DVALUE("setting routing. new value", routing );
            iMediaManager.SetAudioRoutingL( aTarget, routing );
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            }
        }
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( speaker ), Exit");
    
    return status;
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//    
TInt TMceComEventHandler::HandleL( CMceComDisplaySink& aTarget, 
                                   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( display ), Entry");
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {	
        case EMceItcDisplayRect:
            {
            TRect displayRect;
            iMediaManager.GetDisplayRectL( aTarget, displayRect );
    		MCESRV_DEBUG("getting display rect");
    		MCE_HANDLER_WRITE( TMceItcArgTRect, displayRect );
            break;
            }
        case EMceItcDisplayPriority:
            {
            TInt displayPriority = KErrNotFound;
            iMediaManager.GetDisplayPriorityL( aTarget, displayPriority );
    		MCESRV_DEBUG_DVALUE("getting display priority. value", displayPriority );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, displayPriority );
            break;
            }
        case EMceItcDisplayRotation:
            {
            TInt displayRotation = KErrNotFound;
            iMediaManager.GetDisplayRotationL( aTarget, displayRotation );
    		MCESRV_DEBUG_DVALUE("getting display rotation. value", displayRotation );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, displayRotation );
            break;
            }
        case EMceItcSetDisplayRect:
            {
    		MCE_HANDLER_READ( TMceItcArgTRect, TRect, rect );
            iMediaManager.SetDisplayRectL( aTarget, rect );
    		MCESRV_DEBUG("setting display rect");
            break;
            }
        case EMceItcSetDisplayRotation:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, displayRotation );
            iMediaManager.SetDisplayRotationL( aTarget, displayRotation );
    		MCESRV_DEBUG("setting display rotation");
            break;
            }
        case EMceItcSetDisplayPriority:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, displayPriority );
    		MCESRV_DEBUG_DVALUE("setting display priority. new value", displayPriority );
            iMediaManager.SetDisplayPriorityL( aTarget, displayPriority );
            break;
            }
        case EMceItcSetDisplayIndex:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, displayIndex );
    		MCESRV_DEBUG_DVALUE("setting display index. new value", displayIndex );
            iMediaManager.SetDisplayIndexL( aTarget, displayIndex );
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            }
        }
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( display ), Exit");
    
    return status;
    }
     
// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//
TInt TMceComEventHandler::HandleL( CMceComMicSource& aTarget, 
                                   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( mic ), Entry");
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {	
        case EMceItcGain:
            {
            TInt gain = KErrNotFound;
            iMediaManager.GetGainL( aTarget, gain );
    		MCESRV_DEBUG_DVALUE("getting gain. gain", gain );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, gain );
            break;
            }
        case EMceItcSetGain:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, gain );
            iMediaManager.SetGainL( aTarget, gain );
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            }
        }
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( mic ), Exit");
    
    return status;   
    }

// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//    
TInt TMceComEventHandler::HandleL( CMceComCameraSource& aTarget, 
                                   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( camera ), Entry");
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {	
        case EMceItcZoomFactor:
            {
            TInt zoomFactor = KErrNotFound;
            iMediaManager.GetZoomFactorL( aTarget, zoomFactor );
    		MCESRV_DEBUG_DVALUE("getting zoom factor. value", zoomFactor );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, zoomFactor );
            break;
            }
        case EMceItcDigitalZoomFactor:
            {
            TInt digitalZoomFactor = KErrNotFound;
            iMediaManager.GetDigitalZoomFactorL( aTarget, digitalZoomFactor );
    		MCESRV_DEBUG_DVALUE("getting digital zoom factor. value", digitalZoomFactor );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, digitalZoomFactor );
            break;
            }
        case EMceItcContrast:
            {
            TInt contrast = KErrNotFound;
            iMediaManager.GetContrastL( aTarget, contrast );
    		MCESRV_DEBUG_DVALUE("getting contrast. value", contrast );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, contrast );
            break;
            }
        case EMceItcBrightness:
            {
            TInt brightness = KErrNotFound;
            iMediaManager.GetBrightnessL( aTarget, brightness );
    		MCESRV_DEBUG_DVALUE("getting brightness. value", brightness );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, brightness );
            break;
            }
        case EMceItcExposure:
            {
            TInt exposure = KErrNotFound;
            iMediaManager.GetExposureL( aTarget, exposure );
    		MCESRV_DEBUG_DVALUE("getting exposure. value", exposure );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, exposure );
            break;
            }
        case EMceItcWhiteBalance:
            {
            TInt whiteBalance = KErrNotFound;
            iMediaManager.GetWhiteBalanceL( aTarget, whiteBalance );
    		MCESRV_DEBUG_DVALUE("getting whiteBalance. value", whiteBalance );
    		MCE_HANDLER_WRITE( TMceItcArgTInt, whiteBalance );
            break;
            }
        case EMceItcSetCameraIndex:
            {
            MCE_HANDLER_READ_DATA( TMceItcArgTCameraInfo, TMceCameraInfo, cameraInfo, iWriteData );
    		MCESRV_DEBUG_DVALUE("setting camera index. new camera index", cameraInfo.iIndex );
            iMediaManager.SetCameraIndexL( aTarget, cameraInfo.iIndex, cameraInfo.iInfo );
            MCE_HANDLER_WRITE( TMceItcArgTCameraInfo, cameraInfo );
            break;
            }
        case EMceItcSetZoomFactor:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, zoomFactor );
    		MCESRV_DEBUG_DVALUE("setting zoom factor. new zoom factor", zoomFactor );
            iMediaManager.SetZoomFactorL( aTarget, zoomFactor );
            break;
            }
        case EMceItcSetDigitalZoomFactor:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, digitalZoomFactor );
    		MCESRV_DEBUG_DVALUE("setting digital zoom factor. new digital zoom factor", digitalZoomFactor );
            iMediaManager.SetDigitalZoomFactorL( aTarget, digitalZoomFactor );
            break;
            }
        case EMceItcSetContrast:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, contrast );
    		MCESRV_DEBUG_DVALUE("setting contrast. new contrast", contrast );
            iMediaManager.SetContrastL( aTarget, contrast );
            break;
            }
        case EMceItcSetBrightness:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, brightness );
    		MCESRV_DEBUG_DVALUE("setting brightness. new brightness", brightness );
            iMediaManager.SetBrightnessL( aTarget, brightness );
            break;
            }
        case EMceItcSetExposure:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, exposure );
    		MCESRV_DEBUG_DVALUE("setting exposure. new exposure", exposure );
            iMediaManager.SetExposureL( aTarget, exposure );
            break;
            }
        case EMceItcSetWhiteBalance:
            {
    		MCE_HANDLER_READ( TMceItcArgTInt, TInt, whiteBalance );
    		MCESRV_DEBUG_DVALUE("setting white balance. new white balance", whiteBalance );
            iMediaManager.SetWhiteBalanceL( aTarget, whiteBalance );
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            break;
            }
		}
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( camera ), Exit");
    
    return status;
	}

// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//    
TInt TMceComEventHandler::HandleL( CMceComFileSource& aTarget, 
                                   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( file source ), Entry");
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {	
        case EMceItcFileInfo:
            {
            TMceFileInfo fileInfo;
            iMediaManager.GetFileInfoL( aTarget, fileInfo );
    		MCE_HANDLER_WRITE( TMceItcArgTFileInfo, fileInfo );
            break;
            }
        case EMceItcFilePosition:
            {
            TTimeIntervalMicroSeconds filePosition;
            iMediaManager.GetFilePositionL( aTarget, filePosition );
    		MCE_HANDLER_WRITE( TMceItcArgTime, filePosition );
            break;
            }
        case EMceItcSetFilePosition:
            {
    		MCE_HANDLER_READ( TMceItcArgTime, TTimeIntervalMicroSeconds, filePosition );
            iMediaManager.SetFilePositionL( aTarget, filePosition );
            break;
            }
        case EMceItcSetFileInfo:
            {
    		MCE_HANDLER_READ( TMceItcArgTFileInfo, TMceFileInfo, fileInfo );
            iMediaManager.SetFileInfoL( aTarget, fileInfo );
            break;
            }
        case EMceItcSetFastForward:
            {
            TBool onoff = static_cast<TBool>( aEvent.Id().iState );
            iMediaManager.FastForwardL( aTarget, onoff );
            aEvent.Id().iState = onoff;
            break;
            }
        case EMceItcSetFastRewind:
            {
            TBool onoff = static_cast<TBool>( aEvent.Id().iState );
            iMediaManager.FastRewindL( aTarget, onoff );
            aEvent.Id().iState = onoff;
            break;
            }
        default:
            {
            status = KMceEventNotConsumed;
            }
        }
        
    MCESRV_DEBUG("TMceComEventHandler::HandleL( file source ), Exit");
    
    return status;
    }    

// -----------------------------------------------------------------------------
// TMceComEventHandler::HandleL
// -----------------------------------------------------------------------------
//    
TInt TMceComEventHandler::HandleL( CMceComFileSink& /*aTarget*/, 
                                   TMceComEvent& aEvent )
    {
    MCESRV_DEBUG("TMceComEventHandler::HandleL( file sink ), Entry");
    
    TInt status = KMceEventConsumed;
    switch( aEvent.Action() )
        {	
        default:
            {
            status = KMceEventNotConsumed;
            }
        }

    MCESRV_DEBUG("TMceComEventHandler::HandleL( file sink ), Exit");

    return status;    
    }    
    

    
    
    
    
// End of File
