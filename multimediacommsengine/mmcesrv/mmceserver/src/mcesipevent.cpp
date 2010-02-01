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




#include <sipmessageelements.h>
#include <sipnotifydialogassoc.h>
#include <siprefertoheader.h>
#include <sipreferdialogassoc.h>
#include <sipsubscribedialogassoc.h>
#include <sipsubscriptionstateheader.h>
#include <siptoheader.h>
#include <sipexpiresheader.h>
#include <sipeventheader.h>
#include <siprequestelements.h>
#include <sipservertransaction.h>
#include <sipprofile.h>
#include <sipdialog.h>
#include <siptransactionbase.h>
#include <sipdialogassocbase.h>
#include "mcesip.h"

#include "mceevent.h"
#include "mcerefer.h"

#include "mcesipevent.h"
#include "mcesipconnection.h"
#include "mceserial.h"
#include "mceevents.h"
#include "mceclientserver.h"
#include "mcesrvlogs.h"
#include "mcecomevent.h"
#include "mcesipeventhelper.h"

#include "mcecssessionreceiver.h"
#include "mcecssubsessionreceiver.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcecomevent.h"
#include "mcesipmanager.h"
#include "mcesipconnection.h"


// -----------------------------------------------------------------------------
// CMceSipEvent::NewL
// -----------------------------------------------------------------------------
//
CMceSipEvent* CMceSipEvent::NewL( CMceCsSession& aClientSession, 
								  CMceSipConnection& aSIPConnection,
								  CSIPProfile& aProfile)
	{
    CMceSipEvent* self = CMceSipEvent::NewLC( aClientSession, 
    										  aSIPConnection, 
    										  aProfile );
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::NewLC
// -----------------------------------------------------------------------------
//
CMceSipEvent* CMceSipEvent::NewLC ( CMceCsSession& aClientSession, 
									CMceSipConnection& aSIPConnection,
									CSIPProfile& aProfile)
	{
    CMceSipEvent* self = 
        new (ELeave) CMceSipEvent( aClientSession, aSIPConnection, aProfile );
    CleanupStack::PushL(self);
    self->ConstructL( aClientSession );
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::CMceSipEvent
// -----------------------------------------------------------------------------
//
CMceSipEvent::CMceSipEvent ( CMceCsSession& aClientSession, 
							 CMceSipConnection& aSIPConnection,
							 CSIPProfile& aProfile )
: CMceCsSubSession( KMceCSSIPEvent, aClientSession, aSIPConnection, aProfile )
	{
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ConstructL ( CMceCsSession& aClientSession )
	{
	CMceCsSubSession::ConstructL( aClientSession );
			
    MCESRV_DEBUG("MCE SIP event created") 	
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::~CMceSipEvent
// -----------------------------------------------------------------------------
//
CMceSipEvent::~CMceSipEvent()
	{
    MCESRV_DEBUG("MCE SIP event deleted") ;	
    if(iEvent)
    	{
   		 delete iEvent; 		
    	}
    if ( iIdle )
        {
        delete iIdle;    
        }
    }
// -----------------------------------------------------------------------------
// CMceSipEvent::CanDispose
// -----------------------------------------------------------------------------
//
TBool CMceSipEvent::CanDispose()
    {
    return !iEvent || 
        iEvent->EventContext().CurrentState() == KMceTerminatedEventStateIndex;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::HandleSIPEvent
// -----------------------------------------------------------------------------
//
void CMceSipEvent::HandleSIPEvent( TMceSipEventCode /*aEventCode*/ )
    {
    TRAPD( error, iEvent->ProceedL() );
    if ( error )
        {
        ErrorOccured(error);
        }
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::ResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ResponseReceived()
    {
    HandleSIPEvent( EMceResponse );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::ProvisionalResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ProvisionalResponseReceived()
    {
    HandleSIPEvent( EMceProvisionalResponse );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::RedirectionResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::RedirectionResponseReceived()
    {
    HandleSIPEvent( EMceRedirectionResponse );
    }
 
// -----------------------------------------------------------------------------
// CMceSipEvent::ErrorResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ErrorResponseReceived()
    {
    HandleSIPEvent( EMceErrorResponse );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::RequestReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::RequestReceived( TBool /*aInsideDialog*/, CSIPDialog& /*aDialog*/ )
    {
    
    MCESRV_DEBUG( "CMceSipEvent::RequestReceived -- begin" )            				
   	TInt error=KErrNone;
    RStringF method = MceSip::Method( Request() );
    
    if ( AcceptMethod( method ) && iEvent )
        {         
    	MCESRV_DEBUG( "CMceSipEvent::RequestReceived -- passing to event" )
        TRAP ( error, iEvent->ReceivedRequestL() );	
        MCESRV_DEBUG( "CMceSipEvent::RequestReceived -- handled" )
        }
    else
        {
        error = KErrGeneral;
        }
    
    if ( error != KErrNone )
        {
        MceSip::DiscardRequest( Request() );
        }
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::StandAloneRequestReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::StandAloneRequestReceived()
    {
    MCESRV_DEBUG("CMceSipEvent::StandAloneRequestReceived, Entry");
    MceSip::DiscardRequest( Request() );
    MCESRV_DEBUG("CMceSipEvent::StandAloneRequestReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::StandAloneResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::StandAloneResponseReceived( TMceSipResponseType /*aResponseType*/ )
    {
    MCESRV_DEBUG("CMceSipEvent::StandAloneResponseReceived, Entry");
    // NOP
    MCESRV_DEBUG("CMceSipEvent::StandAloneResponseReceived, Exit");
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ErrorOccured( TInt aError )
    {
    TMceIds ids;
    ids.iCallbackID = EMceItcStateChanged;
    ids.iState = CMceEvent::ETerminated;
    
    SendErrorToClient( ids, aError );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipEvent::DoErrorOccured( TInt aError, 
                                   CSIPTransactionBase& /*aTransaction*/ )
    {
    ErrorOccured( aError );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoConnectionStateChanged
// -----------------------------------------------------------------------------
//
void CMceSipEvent::DoConnectionStateChanged( TBool aIsActive )
    {
    if ( iEvent && !aIsActive )
        {
        TRAP_IGNORE( iEvent->PerformActionL( EMceItcTerminateEvent ) );
        }
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::Canceled
// -----------------------------------------------------------------------------
//
void CMceSipEvent::Canceled()
    {
    if ( ClientType() == KMceDlgTypeRefer )
        {
        TRAP_IGNORE( ClientStateChangedL( CMceRefer::ETerminated, EFalse ) );
        }
    else
        {
        TRAP_IGNORE( ClientStateChangedL( CMceEvent::ETerminated, EFalse ) );
        }
    
    MCE_DELETE( iEvent );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::Terminate
// -----------------------------------------------------------------------------
//
TBool CMceSipEvent::Terminate()
    {
    return CanDispose();
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoServiceL
// -----------------------------------------------------------------------------
//
HBufC8* CMceSipEvent::DoServiceL( TMceIds& aIds, 
               TMceItcFunctions aFunction )
    {
    MCESRV_DEBUG( "CMceSipEvent::DoServiceL(ids), Entry" );
    
    LocalDoServiceL( aIds, aFunction );
        
    MCESRV_DEBUG( "CMceSipEvent::DoServiceL(ids), Exit" );
    
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::DoServiceL( TMceIds& /*aIds*/, 
               TMceItcFunctions /*aFunction*/, 
 	           const TDesC8& /*aMessage*/ )
    {
    MCESRV_DEBUG( "CMceSipEvent::DoServiceL(str), Entry" );
    
    User::Leave( KErrNotSupported );
    MCESRV_DEBUG( "CMceSipEvent::DoServiceL(str), Exit" );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::DoServiceL( TMceIds& aIds, 
                               TMceItcFunctions aFunction, 
							   CMceMsgBase& aMessage ) 
    {
    MCESRV_DEBUG( "CMceSipEvent::DoServiceL(message), Entry" );
    
    if ( !iEvent )
        {
        User::LeaveIfError( aMessage.Type() == EMceItcMsgTypeEvent ? 
            KErrNone : KErrNotSupported );
        CMceMsgSIPEvent* eventMsg = 
        	    static_cast< CMceMsgSIPEvent* >( &aMessage );
        DoInitializeL( aFunction, *eventMsg );
        }
    
    CMceMsgSIPData* dataMsg = static_cast< CMceMsgSIPData* >( &aMessage );
    UpdateDataL( aFunction, *dataMsg );
    
    LocalDoServiceL( aIds, aFunction, *dataMsg );
    
    ClearDataL();
    MCESRV_DEBUG( "CMceSipEvent::ServiceL(message), Exit" );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoInitializeIncomingDialogL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::DoInitializeIncomingDialogL( 
                                        CSIPServerTransaction& aTransaction )
    {
    User::LeaveIfError( Dialog() ? KErrNone : KErrTotalLossOfPrecision );
    User::LeaveIfError( 
        Dialog()->Type() == SIPStrings::StringF( SipStrConsts::ENotify ) ?
        KErrNone : KErrNotSupported );
        
    if( aTransaction.Type() == 
        SIPStrings::StringF( SipStrConsts::ESubscribe ) )
    	{
    	SetEventStateType( EMtSubscribe );		
    	}
    else
    	{
    	SetEventStateType( EMtRefer );		
    	}
    	
    const CSIPNotifyDialogAssoc* notify = 
        static_cast< CSIPNotifyDialogAssoc* >( Dialog() );
    
    iEvent = CMceComEvent::NewL( 
                            *this,
                		    notify->SubscriptionState().ExpiresParameter() );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::IncomingSubscribeL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::IncomingSubscribeL(CDesC8Array* aHeaders,
                    				  HBufC8* aContentType,
									  HBufC8* aBody)
	{
	// get ID of SIP profile this SUBSCRIBE was received to.
	TUint32 matchId;
    User::LeaveIfError( Profile().GetParameter( KSIPProfileId, matchId ) );
    
	 // Copy input parameters to avoid ownership transfer problem if leave occurs
    CDesC8Array* tempHeaders = CopyArrayLC( aHeaders ); 
    HBufC8* tempContentType = CopyBufferLC( aContentType );
	
    // make and encode message for ITC
    HBufC8* empty = KMceEmpty().AllocLC();
	HBufC8* recipient = Dialog()->Dialog().ToHeader().ToTextValueLC();
	HBufC8* originator = Dialog()->Dialog().FromHeader().ToTextValueLC();
	HBufC8* eventHeader = 
	    ( ( CSIPNotifyDialogAssoc* ) Dialog() )->Event().ToTextValueLC();
	TUint32 id = reinterpret_cast<TUint32>(this);
	CMceMsgSIPEvent* eventMsg = 
        new (ELeave) CMceMsgSIPEvent( id,
                                      DialogId(),
                                      EMceItcEventTypeSubscribe,
                                      recipient, // recipient
                                      originator, // originator
                                      eventHeader, // event header 
                                      empty, // refer to
                                      CMceRefer::ENoSuppression, // suppress
                                      ( TUint )KErrNotFound, // interval
                                      tempHeaders, 
                                      tempContentType );
    
    CleanupStack::Pop( eventHeader );
    CleanupStack::Pop( originator );
    CleanupStack::Pop( recipient );
    CleanupStack::Pop( empty );
	if ( tempContentType )
        {
        CleanupStack::Pop( tempContentType );
        }
    if ( tempHeaders )
        {
        CleanupStack::Pop( tempHeaders );
        }

    CleanupStack::PushL( eventMsg );                                
    eventMsg->EncodeL();
	HBufC8* encMsg = eventMsg->EncodeBufferCloneL();
	CleanupStack::PopAndDestroy( eventMsg );
	CleanupStack::PushL( encMsg );
    	
    TMceIds ids;
    ids.iManagerType = KMceCSSIPEvent;
    ids.iProfileID = matchId;
    ids.iCallbackID = EMceItcObjectAdded;
	ids.iState = CMceEvent::EIdle;
	ids.iMsgType = EMceItcMsgTypeEvent;
	
    // send	
    Client().SendToClientL( ids, encMsg, aBody );

	CleanupStack::Pop( encMsg );

	delete aHeaders;
	delete aContentType;
    }
					
// -----------------------------------------------------------------------------
// CMceSipEvent::IncomingReferL
// -----------------------------------------------------------------------------
//									
void CMceSipEvent::IncomingReferL(HBufC8* aReferTo,
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody )
	{
	MCESRV_DEBUG( "CMceSipEvent::IncomingReferL -- Begin" );
	// get ID of SIP profile this REFER was received to.
	TUint32 matchId;
    User::LeaveIfError( Profile().GetParameter( KSIPProfileId, matchId ) );
	
    // Copy input parameters to avoid ownership transfer problem if leave occurs
    HBufC8* tempReferTo = CopyBufferLC( aReferTo );
    CDesC8Array* tempHeaders = CopyArrayLC( aHeaders ); 
    HBufC8* tempContentType = CopyBufferLC( aContentType );

	// make and encode message for ITC
	HBufC8* recipient = Dialog()->Dialog().ToHeader().ToTextValueLC();
	HBufC8* originator = Dialog()->Dialog().FromHeader().ToTextValueLC();
	HBufC8* eventHeader = 
	    ( ( CSIPNotifyDialogAssoc* ) Dialog() )->Event().ToTextValueLC();
	
	
	TUint32 id = reinterpret_cast<TUint32>(this);
	
	CMceMsgSIPEvent* eventMsg = 
        new (ELeave) CMceMsgSIPEvent( id,
                                      DialogId(),
                                      EMceItcEventTypeRefer,
                                      recipient, // recipient
                                      originator, // originator
                                      eventHeader, // event header
                                      tempReferTo, 
                                      iEvent->ReferType(),
                                      ( TUint32 )KErrNotFound, // interval
                                      tempHeaders, 
                                      tempContentType );
    
    CleanupStack::Pop( eventHeader );
    CleanupStack::Pop( originator );
    CleanupStack::Pop( recipient );
    if ( tempContentType )
        {
        CleanupStack::Pop( tempContentType );
        }
    if ( tempHeaders )
        {
        CleanupStack::Pop( tempHeaders );
        }
    if ( tempReferTo )
        {
        CleanupStack::Pop( tempReferTo );
        }

    CleanupStack::PushL( eventMsg );
    eventMsg->EncodeL();
	HBufC8* encMsg = eventMsg->EncodeBufferCloneL();
	CleanupStack::PopAndDestroy( eventMsg ); 
	CleanupStack::PushL( encMsg );
	
    // send	
    TMceIds ids;
    ids.iManagerType = KMceCSSIPEvent;
    ids.iProfileID = matchId;
    ids.iCallbackID = EMceItcObjectAdded;
	ids.iState = CMceRefer::EIdle;
	ids.iMsgType = EMceItcMsgTypeEvent;
	
    Client().SendToClientL( ids, encMsg, aBody );

	CleanupStack::Pop( encMsg );

	delete aHeaders;
	delete aContentType;
	delete aReferTo;

    MCESRV_DEBUG( "Incoming REFER sent to client" );
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::ClientStateChangedL
// -----------------------------------------------------------------------------
//									
void CMceSipEvent::ClientStateChangedL( CMceRefer::TState aState, 
                                        TBool aIsResponse )
    {
    DoClientStateChangedL( aState, aIsResponse );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::ClientStateChangedL
// -----------------------------------------------------------------------------
//									
void CMceSipEvent::ClientStateChangedL( CMceEvent::TState aState, 
                                        TBool aIsResponse )
	{
	DoClientStateChangedL( aState, aIsResponse );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoClientStateChangedL
// -----------------------------------------------------------------------------
//									
void CMceSipEvent::DoClientStateChangedL( TUint32 aState, TBool aIsResponse )
	{
	MCESRV_DEBUG_DVALUE( "Event state changed. new state", aState );
	
    // update IDs
	TMceIds ids;
    ids.iState = aState;
    ids.iCallbackID = EMceItcStateChanged;
    ids.iMsgType = EMceItcMsgTypeSIPReply;
    
    if ( aIsResponse )
        {
        const CSIPResponseElements* response = Response().ResponseElements();
        RStringF reasonPhrase = response->ReasonPhrase();
	    TUint statusCode = response->StatusCode();
		CDesC8ArrayFlat* headers = new (ELeave) CDesC8ArrayFlat(1);		
	    CleanupStack::PushL( headers );
	    
		const RPointerArray<CSIPHeaderBase>& sipHeaders = 
		    response->MessageElements().UserHeaders();
		
		for (int i=0; i < sipHeaders.Count(); i++)
			{
			CSIPHeaderBase* anotherhead = sipHeaders[i];
			HBufC8* head = anotherhead->ToTextLC();
			headers->AppendL(*head);
			CleanupStack::PopAndDestroy( head );
			}    

	    HBufC8* reason = reasonPhrase.DesC().AllocLC();
		HBufC8* empty = KMceEmpty().AllocLC();    

    	CMceMsgSIPReply* replyMsg = new (ELeave) CMceMsgSIPReply( 
    	    *reason, 
    	    statusCode, 
    	    *headers, 
    	    *empty );
    	CleanupStack::PushL( replyMsg );
    	replyMsg->EncodeL();
    	
		HBufC8* context = replyMsg->EncodeBufferCloneL();
	    CleanupStack::PopAndDestroy( replyMsg );
	    
    	CleanupStack::PushL( context );
		HBufC8* content = response->MessageElements().Content().AllocLC();
		
	    SendToClientL( ids, context, content );
    
	    CleanupStack::Pop( content);
	    CleanupStack::Pop( context);
    	CleanupStack::PopAndDestroy( empty );
    	CleanupStack::PopAndDestroy( reason );
    	CleanupStack::PopAndDestroy( headers );  
	    }
    else
    	{
	    SendToClient( ids );
    	}
    MCESRV_DEBUG( "New event state sent to client" );
    }		

// -----------------------------------------------------------------------------
// CMceSipEvent::NotifyReceived
// -----------------------------------------------------------------------------
//				
void CMceSipEvent::NotifyReceivedL(CDesC8Array* aHeaders,
                    				HBufC8* aContentType,
									HBufC8* aBody )
	{	
    // update IDs
    TMceIds ids;
    ids.iCallbackID = EMceItcNotifyReceived;
    ids.iMsgType = EMceItcMsgTypeEvent;
    ids.iState = CMceEvent::EActive;

	// Copy input parameters to avoid ownership transfer problem if leave occurs
    CDesC8Array* tempHeaders = CopyArrayLC( aHeaders ); 
    HBufC8* tempContentType = CopyBufferLC( aContentType );

    // make and encode message for ITC
    HBufC8* recipient = Dialog()->Dialog().ToHeader().ToTextValueLC();
	HBufC8* originator = Dialog()->Dialog().FromHeader().ToTextValueLC();
	HBufC8* emptyEvent = KMceEmpty().AllocLC();
	HBufC8* emptyReferTo = KMceEmpty().AllocLC();
	
	CMceMsgSIPEvent* eventMsg = 
        new (ELeave) CMceMsgSIPEvent( ids.iSessionID,
                                      DialogId(),
                                      EMceItcEventTypeEither,
                                      recipient, // recipient
                                      originator, // originator
                                      emptyEvent, // event header 
                                      emptyReferTo, // referTo
                                      CMceRefer::ENoSuppression, // suppress
                                      ( TUint )KErrNotFound, // interval
                                      tempHeaders, 
                                      tempContentType );
    CleanupStack::Pop( emptyReferTo );                                  
    CleanupStack::Pop( emptyEvent );
    CleanupStack::Pop( originator );
    CleanupStack::Pop( recipient );
    if ( tempContentType )
        {
        CleanupStack::Pop( tempContentType );
        }
    if ( tempHeaders )
        {
        CleanupStack::Pop( tempHeaders );
        }

    CleanupStack::PushL( eventMsg );
    eventMsg->EncodeL();
	HBufC8* encMsg = eventMsg->EncodeBufferCloneL();
	CleanupStack::PopAndDestroy( eventMsg );
	CleanupStack::PushL( encMsg );
	
	// send
    SendToClientL( ids, encMsg, aBody );
    
    CleanupStack::Pop( encMsg );
    
    delete aHeaders;
    delete aContentType;
    }
	
// -----------------------------------------------------------------------------
// CMceSipEvent::CreateIncomingDialogL
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSipEvent::CreateIncomingDialogL( 
                                          CSIPServerTransaction& aTransaction )
    {
    User::LeaveIfError( 
        aTransaction.Type() == SIPStrings::StringF( SipStrConsts::ERefer ) || 
        aTransaction.Type() == SIPStrings::StringF( SipStrConsts::ESubscribe ) ? 
        KErrNone : KErrNotSupported );
    
    User::LeaveIfError( 
        aTransaction.RequestElements() ? KErrNone : KErrArgument );
        
    const CSIPMessageElements& message = 
        aTransaction.RequestElements()->MessageElements();
    
    // get expires header value, if exists.
    CSIPHeaderBase* expiresHeaderTmp = MceSip::FindHeader( message,
        SIPStrings::StringF(SipStrConsts::EExpiresHeader ) );	
        
    TUint expires = KMceDefaultSubscrExpire;
    if ( expiresHeaderTmp )
        {
        CSIPExpiresHeader* expiresHeader = 
        static_cast< CSIPExpiresHeader* >( expiresHeaderTmp );
        expires = expiresHeader->Value();    
        }
        
        // get event header from the server transaction
    CSIPHeaderBase* eventHeaderTmp = MceSip::FindHeader( message,
        SIPStrings::StringF( SipStrConsts::EEventHeader ) );
        
    CSIPEventHeader* eventHeader = NULL;
    if ( eventHeaderTmp )
        {
        // use event from request, if present
        eventHeader =   
            static_cast< CSIPEventHeader* >( eventHeaderTmp->CloneL() );
        }
    else if ( aTransaction.Type() == 
        SIPStrings::StringF(SipStrConsts::ERefer ) )
        {
        // For REFER, Event is "refer"
        eventHeader = CSIPEventHeader::NewL( KMceSipEventPackageRefer );
        }
    else
        {
        // SUBSCRIBE with no Event header
        User::Leave( KErrNotSupported );
        }
    CleanupStack::PushL( eventHeader );
    
    // create subscription state header
    CSIPSubscriptionStateHeader* subscrStateHeader = NULL;
    if ( expires > 0 )
        {
        subscrStateHeader = 
            CSIPSubscriptionStateHeader::NewLC( KStateActive );
        }
    else
        {
        subscrStateHeader = 
            CSIPSubscriptionStateHeader::NewLC( KStateTerminated );
        }
    subscrStateHeader->SetExpiresParameterL( expires );
    // create notify dialog assoc.
    const MSIPRegistrationContext& profile = Profile();    
    CSIPNotifyDialogAssoc* dialog = CSIPNotifyDialogAssoc::NewL( 
                                                        aTransaction, 
                                                        profile,
                                                        eventHeader, 
                                                        subscrStateHeader );
    CleanupStack::Pop( subscrStateHeader );
    CleanupStack::Pop( eventHeader );
    return dialog;
    }


// -----------------------------------------------------------------------------
// CMceSipEvent::CreateOutgoingDialogL
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSipEvent::CreateOutgoingDialogL( 
                                                TMceDialogType aDialogType, 
                                                CDesC8Array& aParams,
                                                CSIPDialog& aExistingDialog )
                                                          
    {
    User::LeaveIfError( aDialogType == KMceDlgTypeRefer ||
                     aDialogType == KMceDlgTypeSubscribe ?
                     KErrNone : KErrArgument );
                            
    CSIPDialogAssocBase* dialog = NULL;
    
    if ( aDialogType == KMceDlgTypeSubscribe )
        {
        TPtrC8 eventHeader = aParams.MdcaPoint( KMceArrayIndexEventHeader );
        
        CSIPEventHeader* sipEventHeader = 
            CSIPEventHeader::DecodeL( eventHeader );
        CleanupStack::PushL( sipEventHeader );
    
        dialog = CSIPSubscribeDialogAssoc::NewL( aExistingDialog, 
                                                 sipEventHeader );         
    
        CleanupStack::Pop( sipEventHeader );
        }
    else
        {
        TPtrC8 referTo = aParams.MdcaPoint( KMceArrayIndexEventReferTo );
            
        CSIPReferToHeader* referToHeader = 
            CSIPReferToHeader::DecodeL( referTo ); 
        CleanupStack::PushL( referToHeader );
        dialog = CSIPReferDialogAssoc::NewL( aExistingDialog,
                                             referToHeader );
                                             
        CleanupStack::Pop( referToHeader );
        
        }
        
        
    return dialog;
    
    }


// -----------------------------------------------------------------------------
// CMceSipEvent::CreateOutgoingDialogL
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSipEvent::CreateOutgoingDialogL( 
                                                    TMceDialogType aDialogType,
                                                    CDesC8Array& aParams )
    {
    User::LeaveIfError( aDialogType == KMceDlgTypeRefer ||
                        aDialogType == KMceDlgTypeSubscribe ?
                        KErrNone : KErrArgument );
                            
    CSIPDialogAssocBase* dialog = NULL;
    
    CSIPConnection& connection = SIPConnection().Connection();
    const CSIPProfile& profile = Profile();
    TPtrC8 eventRemoteUri = aParams.MdcaPoint( KMceArrayIndexEventRemoteURI );
    TPtrC8 originator = aParams.MdcaPoint( KMceArrayIndexEventOriginator );
    CSIPToHeader* toHeader = CSIPToHeader::DecodeL( eventRemoteUri );
    CleanupStack::PushL( toHeader );
	CUri8* remoteUri = CUri8::NewLC( toHeader->SIPAddress().Uri8().Uri() );
	
	CSIPFromHeader* fromHeader = NULL;
    if ( originator.Length() > 0 )
        {
        fromHeader = MceSip::ToFromHeaderL( originator );
        }
    CleanupStack::PushL( fromHeader );
        
    if ( aDialogType == KMceDlgTypeSubscribe )
        {
        TPtrC8 eventHeader = aParams.MdcaPoint( KMceArrayIndexEventHeader );
        
        CSIPEventHeader* sipEventHeader = 
            CSIPEventHeader::DecodeL( eventHeader );
        CleanupStack::PushL( sipEventHeader );
    
        dialog = CSIPSubscribeDialogAssoc::NewL( connection,
                                                 remoteUri,
                                                 profile,
                                                 sipEventHeader,
                                                 fromHeader,
                                                 toHeader );         
    
        CleanupStack::Pop( sipEventHeader );
        }
    else
        {
        TPtrC8 referTo = aParams.MdcaPoint( KMceArrayIndexEventReferTo );
            
        CSIPReferToHeader* referToHeader = 
            CSIPReferToHeader::DecodeL( referTo ); 
        CleanupStack::PushL( referToHeader );
        dialog = CSIPReferDialogAssoc::NewL( connection,
                                             remoteUri,
                                             profile,
                                             referToHeader,
                                             fromHeader,
                                             toHeader );
                                             
        CleanupStack::Pop( referToHeader );
        
        }
    CleanupStack::Pop( fromHeader );
    CleanupStack::Pop( remoteUri );
    CleanupStack::Pop( toHeader );
        
    return dialog;
    
    }
		

// -----------------------------------------------------------------------------
// CMceSipEvent::AcceptDialogTransaction
// -----------------------------------------------------------------------------
//
TBool CMceSipEvent::AcceptDialogTransaction( 
    CSIPServerTransaction& aTransaction )
    {
    TBool ret = EFalse;
    TRAP_IGNORE((ret = AcceptDialogTransactionL (aTransaction ) ));
    return ret;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::AcceptDialogTransactionL
// -----------------------------------------------------------------------------
//
TBool CMceSipEvent::AcceptDialogTransactionL( 
                                    CSIPServerTransaction& aTransaction )
    {
    TBool ret = EFalse;
    User::LeaveIfError( Dialog() ? KErrNone : KErrArgument );
    User::LeaveIfError( iEvent ? KErrNone : KErrTotalLossOfPrecision );
    User::LeaveIfError( 
        aTransaction.RequestElements() ? KErrNone : KErrArgument );
        
    CSIPHeaderBase* head = 
      MceSip::FindHeader( aTransaction.RequestElements()->MessageElements(),
                          SIPStrings::StringF( SipStrConsts::EEventHeader ) );
    User::LeaveIfError( head ? KErrNone : KErrArgument );
    
    const RStringF tranType = aTransaction.Type();
    const RStringF dlgType = Dialog()->Type();
            
    CSIPEventHeader* tranEventHeader = static_cast<CSIPEventHeader*>( head );
    if ( tranType == SIPStrings::StringF( SipStrConsts::ESubscribe ) )
        {
        // We have MT SUBSCRIBE or MT REFER
        // The event package and the id must match to 
        // the one of the NOTIFY dialog assoc.
        if ( dlgType == SIPStrings::StringF( SipStrConsts::ENotify ) )
            {
            // match to NOTIFY dialog event header
            ret = MCESIPEventHelper::MatchEvents( 
                static_cast< CSIPNotifyDialogAssoc* >( Dialog() )->Event(), 
                *tranEventHeader );
            }
        }
    else if ( tranType == SIPStrings::StringF( SipStrConsts::ENotify ) )
        {
        // We have MO SUBSCRIBE or MO REFER
        if ( iStateType == EMoSubscribe )
            {
            if ( dlgType == SIPStrings::StringF( SipStrConsts::ESubscribe ) )
                {
                // The event package and id must match to the
                // one of the subscribe dialog assoc.
                ret = MCESIPEventHelper::MatchEvents( 
                                static_cast< CSIPSubscribeDialogAssoc* >
                                    ( Dialog() )->Event(),
                                *tranEventHeader );
                }
            }
        else if ( iStateType == EMoRefer )
            {
            // The event package must be "refer" and the
            // id is either not present or must mach to the
            // CSeq number of the sent REFER.
            ret = MCESIPEventHelper::MatchEventsL( KMceSipEventPackageRefer,
                                                  iEvent->IdValue(),
                                                  *tranEventHeader );
            }
        }
    return ret;        
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::AcceptMethod
// -----------------------------------------------------------------------------
//
TBool CMceSipEvent::AcceptMethod( RStringF aMethod )
    {
    
    return aMethod == SIPStrings::StringF(SipStrConsts::ERefer ) || 
           aMethod == SIPStrings::StringF(SipStrConsts::ESubscribe)||
           aMethod == SIPStrings::StringF(SipStrConsts::ENotify );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::SetEventStateType
// -----------------------------------------------------------------------------
// 
void CMceSipEvent::SetEventStateType(const TEventStateType aEventStateType )
	{
	iStateType = aEventStateType;	
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::EventStateType
// -----------------------------------------------------------------------------
// 	
TEventStateType CMceSipEvent::EventStateType() const
	{
	return iStateType;	
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::ClearDataL
// -----------------------------------------------------------------------------
// 	
void CMceSipEvent::ClearDataL()
    {
    iEvent->SetMsgHeaders( NULL );    
    iEvent->SetMsgContentL( NULL, NULL );
    iEvent->SetReasonPhraseL( ( TUint32 ) KErrNotFound, NULL );
    }
    
// -----------------------------------------------------------------------------
// CMceSipEvent::DoInitializeL
// -----------------------------------------------------------------------------
// 	    
void CMceSipEvent::DoInitializeL( TMceItcFunctions aFunction, 
                                  CMceMsgSIPEvent& aMessage )
    {
    switch( aFunction )
        {
        case EMceItcSubscribe:
            {
        	//Create MO Subscribe event
			SetEventStateType( EMoSubscribe );
            break;
            }
        case EMceItcReferEvent:
            {
            //Create MO Refer event
            SetEventStateType( EMoRefer );
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
    iEvent = CMceComEvent::NewL( *this, aMessage.RefreshInterval() );
    iEvent->SetReferType( 
        static_cast< CMceRefer::TType >( aMessage.ReferType() ) );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::LocalDoServiceL( TMceIds& aIds, 
                               TMceItcFunctions aFunction, 
                               CMceMsgSIPData& aMessage )
    {
    MCESRV_DEBUG( "CMceSipEvent::LocalDoServiceL( message ), Entry" );
    switch( aFunction )
        {
        case EMceItcRespondEvent:
            {
            CMceMsgSIPReply* replyMsg = 
                static_cast< CMceMsgSIPReply* >( &aMessage );
            TBool terminated = 
                ( replyMsg->iCode >= KMceSipMultipleChoices ? ETrue : EFalse );
            
            if ( iEvent->ReferType() != CMceRefer::ENoSuppression )
                {
                if ( terminated )
        	        {
        	        aIds.iState = CMceRefer::ETerminated;
        	        }
        	    else if ( replyMsg->iCode >= KMceSipOK )
        	        {
        	        aIds.iState = CMceRefer::EAccepted;
                    //Asynchronically put the event in terminated state
                    User::LeaveIfError( !iIdle ? 
                        KErrNone : KErrTotalLossOfPrecision );
                    iIdle = CIdle::NewL( CActive::EPriorityIdle );
                    iIdle ->Start( TCallBack( Idle, this ) );
        	        }
        	    else
        	        {
        	        // provisional
        	        // NOP
        	        }
                }
            else // not suppressed
                {
                aIds.iState = ( terminated ? 
                    CMceEvent::ETerminated : CMceEvent::EPending );
                }
            break;
            }
        default :
            {
            // NOP 
            break;
            }
        }
        
    LocalDoServiceL( aIds, aFunction );
    
    MCESRV_DEBUG( "CMceSipEvent::LocalDoServiceL( message ), Exit" );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::LocalDoServiceL( TMceIds& aIds, TMceItcFunctions aFunction )
    {
    MCESRV_DEBUG( "CMceSipEvent::LocalDoServiceL, Entry" );
    User::LeaveIfError( iEvent ? KErrNone : KErrTotalLossOfPrecision );
    // call state machine
    iEvent->PerformActionL( aFunction );	
    
    // update ids and other actions, if needed.
    switch ( aFunction )
        {
        case EMceItcSubscribe:
        case EMceItcSubscribeUpdate:
        case EMceItcTerminateEvent:
            {
            aIds.iState = CMceEvent::EPending;
            break;
            }
        case EMceItcReferEvent:
        case EMceItcNotify:
            {
            aIds.iState = ( iEvent->ReferType() != CMceRefer::ENoSuppression ?  
                CMceRefer::EPending : CMceEvent::EPending );
            break;
            }
        case EMceItcRespondEvent:
            {
            // NOP
            break;
            }
        case EMceItcAcceptEvent:
            { 
            if ( iEvent->ReferType() != CMceRefer::ENoSuppression )
                {
                aIds.iState = CMceRefer::EAccepted;
                //Asynchronically put the event in terminated state
                User::LeaveIfError( !iIdle ? 
                    KErrNone : KErrTotalLossOfPrecision );
                iIdle = CIdle::NewL( CActive::EPriorityIdle );
                iIdle ->Start( TCallBack( Idle, this ) );
                }
            else
                {
                aIds.iState = CMceEvent::EPending;
                }
            break;
            }
        case EMceItcRejectEvent:
    		{
    		aIds.iState = ( iEvent->ReferType() != CMceRefer::ENoSuppression ?  
                CMceRefer::ETerminated : CMceEvent::ETerminated );
            break;	
    		}
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
            
    MCESRV_DEBUG( "CMceSipEvent::LocalDoServiceL, Exit" );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::UpdateDataL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::UpdateDataL( TMceItcFunctions aFunction, 
                                CMceMsgSIPData& aMessage )
    {
    MCESRV_DEBUG( "CMceSipEvent::UpdateDataL, Entry" );
        
    switch( aFunction )
        {
        case EMceItcSubscribe:
        case EMceItcNotify:
        case EMceItcReferEvent:
        case EMceItcTerminateEvent://only in MT
            {
            User::LeaveIfError ( aMessage.Type() == EMceItcMsgTypeEvent ?
                KErrNone : KErrNotSupported );
            						
        	break;
            }
        case EMceItcSubscribeUpdate:
            {
            User::LeaveIfError ( aMessage.Type() == EMceItcMsgTypeEvent ?
                KErrNone : KErrNotSupported );
            
            CMceMsgSIPEvent* eventMsg = 
                static_cast< CMceMsgSIPEvent* >( &aMessage );
            
            iEvent->SetRefreshInterval( eventMsg->RefreshInterval() );
            break;
            }
      case EMceItcRespondEvent:
        	{
        	User::LeaveIfError ( aMessage.Type() == EMceItcMsgTypeSIPReply ?
                KErrNone : KErrNotSupported );
            
        	CMceMsgSIPReply* replyMsg = 
        	    static_cast< CMceMsgSIPReply* >( &aMessage );
        	HBufC8* reason = replyMsg->Reason();
        	CleanupStack::PushL( reason );
            iEvent->SetReasonPhraseL( replyMsg->iCode, reason );
        	CleanupStack::Pop( reason );
            break;	
        	}
      default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
        
    iEvent->SetMsgHeaders( aMessage.Headers() );
    
    HBufC8* content = PopClientContent();
    CleanupStack::PushL( content );
    HBufC8* contentType = aMessage.ContentType();
    CleanupStack::PushL( contentType );
    
    iEvent->SetMsgContentL( contentType, content );
    CleanupStack::Pop( contentType );
    CleanupStack::Pop( content );
    
    
    MCESRV_DEBUG( "CMceSipEvent::UpdateDataL, Exit" );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::SuppressedReferTerminated
// -----------------------------------------------------------------------------
// 	
void CMceSipEvent::SuppressedReferTerminated()
    {
    delete iIdle;
    iIdle = NULL;
    TRAP_IGNORE( ClientStateChangedL( CMceRefer::ETerminated, EFalse ) );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::Idle
// -----------------------------------------------------------------------------
// 	
TInt CMceSipEvent::Idle( TAny* aObject )
    {
    CMceSipEvent* event = reinterpret_cast< CMceSipEvent* >( aObject );
    event->SuppressedReferTerminated();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::CopyArrayLC
// -----------------------------------------------------------------------------
// 
CDesC8Array* CMceSipEvent::CopyArrayLC( const CDesC8Array* aArray )
    {
    CDesC8Array* tempArray = NULL;
    if ( aArray )
        {
        tempArray = new ( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( tempArray );
        for ( TInt i = 0; i < aArray->MdcaCount(); i++ )
            {
            tempArray->AppendL( aArray->MdcaPoint( i ) );
            }
        }
    return tempArray;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::CopyBufferLC
// -----------------------------------------------------------------------------
// 
HBufC8* CMceSipEvent::CopyBufferLC( const HBufC8* aBuffer )
    {
    HBufC8* tempBuffer = NULL;
    if ( aBuffer )
        {
        tempBuffer = aBuffer->AllocLC();
        }
    return tempBuffer;
    }
    
//  End of File
