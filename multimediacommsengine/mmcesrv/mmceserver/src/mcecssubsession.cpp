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




#include <sipdialogassocbase.h>
#include <sipdialog.h>
#include <sipcontactheader.h>
#include <sipprofile.h>
#include "mceserver.pan"

#include "mcecssubsession.h"
#include "mcesipmanager.h"
#include "mcecssessionimplementation.h"
#include "mcecssession.h"
#include "mceservercore.h"
#include "mcesipconnection.h"
#include "mcecssubsessionreceiver.h"
#include "mcecsserveritc.h"
#include "mcecomsession.h"
#include "mcecommediastream.h"
#include "mcecommediastream.h"
#include "mcecomaudiocodec.h"
#include "mcecommediasink.h"
#include "mcecommediasource.h"
#include "mceserial.h"
#include "mcesrvlogs.h"
#include "mcemessagedispatcher.h"


const TInt KMaxAddressLength = 256;    
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::CMceCsSubSession
// -----------------------------------------------------------------------------
//

CMceCsSubSession::CMceCsSubSession ( TMceCsSessionType aType,
                                     CMceCsSession& aClientSession,
                                     CMceSipConnection& aSIPConnection )
: iType( EUnknown ),
  iClientType( aType ),
  iDialogId( KMceNotAssigned ),
  iClientExists( EFalse ),
  iCurrentDialog( NULL ),
  iClientSession (NULL ),
  iSIPConnection( aSIPConnection ),
  iServerCore( aClientSession.ServerCore() ),
  iProfile( NULL )
  	{
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::CMceCsSubSession
// -----------------------------------------------------------------------------
//
CMceCsSubSession::CMceCsSubSession ( TMceCsSessionType aType,
                                     CMceCsSession& aClientSession,
                                     CMceSipConnection& aSIPConnection,
                                     CSIPProfile& aProfile )
: iType( EUnknown ),
  iClientType( aType ),
  iDialogId( KMceNotAssigned ),
  iClientExists( EFalse ),
  iCurrentDialog( NULL ),
  iClientSession (NULL ),
  iSIPConnection( aSIPConnection ),
  iServerCore( aClientSession.ServerCore() ),
  iProfile( &aProfile )
  	{
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::ConstructL ( CMceCsSession& aClientSession )
	{
    MCESRV_DEBUG("CMceCsSubSession::ConstructL, Entry")
	
    SetSessionReceiver( aClientSession.CreateSubSessionReceiverL() );
    SetClient( aClientSession.Implementation() );
    
    MCESRV_DEBUG("CMceCsSubSession::ConstructL, Exit")
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::~CMceCsSubSession
// -----------------------------------------------------------------------------
//
CMceCsSubSession::~CMceCsSubSession()
	{
    MCESRV_DEBUG("CMceCsSubSession::~CMceCsSubSession, Entry")
		
    TInt index = iPendingTransactions.Find( iInitialInvite );
    if ( index != KErrNotFound )
        {
        iPendingTransactions.Remove( index );
        }
    index = iPendingTransactions.Find( iResponse );
    if ( index != KErrNotFound )
        {
        iPendingTransactions.Remove( index );
        }
    
    iPendingTransactions.ResetAndDestroy();
    iPendingTransactions.Close();
    
    iPendingReceivedRequests.ResetAndDestroy();
    
    if ( iInitialInvite != iResponse )
        {
        delete iInitialInvite;
        }
        
    iInitialInvite = NULL;
    delete iReceiver;
    delete iDialog;
    delete iResponse;
    delete iClientContent;
    Manager().UnRegisterSubSession( *this );
    
    delete iAutoEventMsg;
    delete iAutoEventIds;
    delete iAutoEvent;
    
    MCESRV_DEBUG("CMceCsSubSession::~CMceCsSubSession, Exit")
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::Type
// -----------------------------------------------------------------------------
//
CMceCsSubSession::TType CMceCsSubSession::Type() const
    {
    return iType;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::ClientType
// -----------------------------------------------------------------------------
//
TMceCsSessionType CMceCsSubSession::ClientType() const
    {
    return iClientType;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::ClientSession
// -----------------------------------------------------------------------------
//
CMceCsSessionImplementation& CMceCsSubSession::Client() const
    {
    return *iClientSession;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::SetClient
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::SetClient( CMceCsSessionImplementation& aClient )
    {
    iClientSession = &aClient;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::SetClient
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::SetSessionReceiver( 
                                  CMceCsSubSessionReceiver* aSessionReceiver )
    {
    delete iReceiver;
    iReceiver = aSessionReceiver;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::IsOrphan
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::IsOrphan()   
    {
    return !iReceiver;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::HasInitialInviteTransactionBeenCompleted()
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::HasInitialInviteTransactionBeenCompleted() const
    {
    TBool completed( ETrue );
    if ( iInitialInvite )
        {
        CSIPTransactionBase::TState state( CSIPTransactionBase::ETerminated  );
        TRAP_IGNORE( ( state = iInitialInvite->StateL() ) )
        completed = ( state == CSIPTransactionBase::ECompleted || 
                      state == CSIPTransactionBase::ETerminated );
        }
    return completed; 
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::SIPConnection
// -----------------------------------------------------------------------------
//
CMceSipConnection& CMceCsSubSession::SIPConnection() const
    {
    return iSIPConnection;
    }


// -----------------------------------------------------------------------------
// CMceCsSubSession::DialogId
// -----------------------------------------------------------------------------
//
TUint32 CMceCsSubSession::DialogId() const
    {
    return iDialogId;
    }


// -----------------------------------------------------------------------------
// CMceCsSubSession::Manager
// -----------------------------------------------------------------------------
//
CMceSipManager& CMceCsSubSession::Manager() const
    {
    return ServerCore().Manager();
    }


// -----------------------------------------------------------------------------
// CMceCsSubSession::ServerCore
// -----------------------------------------------------------------------------
//
CMceServerCore& CMceCsSubSession::ServerCore() const
    {
    return iServerCore;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::FCSession
// -----------------------------------------------------------------------------
//
const CFCSession* CMceCsSubSession::FCSession() const
	{
	return iFCSession;
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::Request
// -----------------------------------------------------------------------------
//
CSIPServerTransaction& CMceCsSubSession::Request() const
    {
    if ( iAutoEventEnabled && iAutoEventAssociatedRequest )
        {
        TInt index = iPendingReceivedRequests.Find( iAutoEventAssociatedRequest );
        if ( index != KErrNotFound )
            {
            return *iAutoEventAssociatedRequest;
            }
        }
    return *CURRENT_REQUEST();
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::InitialRequest
// -----------------------------------------------------------------------------
//
CSIPServerTransaction& CMceCsSubSession::InitialRequest() const
    {
    return *FIRST_REQUEST();
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::PopRequest
// -----------------------------------------------------------------------------
//
CSIPServerTransaction* CMceCsSubSession::PopRequest()
    {
    CSIPServerTransaction* request = NULL;
    
    request = CURRENT_REQUEST();
    if ( request )
        {
        iPendingReceivedRequests.Remove( CURRENT_REQUEST_IND );
        }
    return request;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::Response
// -----------------------------------------------------------------------------
//
CSIPClientTransaction& CMceCsSubSession::Response() const
    {
    return *iResponse;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::PopResponse
// -----------------------------------------------------------------------------
//
CSIPClientTransaction* CMceCsSubSession::PopResponse()
    {
    CSIPClientTransaction* response = iResponse;
    if ( response && MceSip::TrxCompleted( *response ) )
        {
        iResponse = NULL;
        }
    else
        {
        response = NULL;
        }
        
    return response;
    
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::SetDialog
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::SetDialog( CMceCsSubSession::TType aType, 
                                  CSIPDialogAssocBase* aDialog, 
                                  TUint32 aDialogId )
    {
    __ASSERT_ALWAYS( !iDialog, User::Panic( KMceServerPanic, KErrArgument ) );
    iType = aType;
    iDialogId = aDialogId;
    iDialog = aDialog;
    }

    
// -----------------------------------------------------------------------------
// CMceCsSubSession::SetPendingTransactionL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::SetPendingTransactionL( 
                                    CSIPClientTransaction* aTransaction )
    {
    __ASSERT_ALWAYS( aTransaction, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iPendingTransactions.Find( aTransaction ) == KErrNotFound, 
        User::Leave( KErrArgument ) );
    
    iPendingTransactions.AppendL( aTransaction );
    
    if ( Type() == EOutSession && 
         aTransaction->Type() == SIPStrings::StringF( SipStrConsts::EInvite ) )
        {
        iType = EOutSIPSession;
        iInitialInvite = aTransaction;
        }
    }


// -----------------------------------------------------------------------------
// CMceCsSubSession::Consumes
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::Consumes( CSIPTransactionBase& aTransaction )
    {
    TBool isPending = EFalse;

    if ( aTransaction.IsSIPClientTransaction() )
        {
        CSIPClientTransaction& transaction = 
            static_cast<CSIPClientTransaction&>( aTransaction );
        isPending = iPendingTransactions.Find( &transaction ) >= 0;
        }
    else
        {
        CSIPServerTransaction& transaction = 
            static_cast<CSIPServerTransaction&>( aTransaction );
        isPending = iPendingReceivedRequests.Find( &transaction ) >= 0; 
        }
        
    return isPending;    
    }
   
// -----------------------------------------------------------------------------
// CMceCsSubSession::AcceptDialogTransaction
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::AcceptDialogTransaction( 
                                    CSIPServerTransaction& /*aTransaction*/ )
    {        
    return ETrue;    
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::AcceptStandAloneTransaction
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::AcceptStandAloneTransaction( 
                                    CSIPServerTransaction& /*aTransaction*/ )
    {        
    return ETrue;    
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::Consumes
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::Consumes( CSIPDialog& aDialog, 
                                  CSIPServerTransaction& aTransaction,
                                  TBool& aAssociatesWithDialog )
    {
    aAssociatesWithDialog = ( Dialog()->Dialog() == aDialog );
    return aAssociatesWithDialog && ( Consumes( aTransaction ) ||
           AcceptMethod( MceSip::Method( aTransaction ) ) );
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::DoServiceL
// -----------------------------------------------------------------------------
//
HBufC8* CMceCsSubSession::DoServiceL( TMceIds& /*aIds*/, 
                                      TMceItcFunctions /*aFunction*/ )
    {
    MCESRV_DEBUG("CMceCsSubSession::DoServiceL( no message ), NOT SUPPORTED")
    User::Leave( KErrNotSupported );
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::DoServiceL( TMceIds& /*aIds*/, 
                                 TMceItcFunctions /*aFunction*/, 
                                 const TDesC8& /*aMessage*/ )
    {
    MCESRV_DEBUG("CMceCsSubSession::DoServiceL( data ), NOT SUPPORTED")
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::DoServiceL( TMceIds& /*aIds*/, 
                                 TMceItcFunctions /*aFunction*/, 
                                 TPtr8& /*aMessage*/ )
    {
    MCESRV_DEBUG("CMceCsSubSession::DoServiceL( data ), NOT SUPPORTED")
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::DoServiceL( TMceIds& /*aIds*/, 
                                 TMceItcFunctions /*aFunction*/, 
                                 CMceMsgBase& /*aMessage*/ )
    {
    MCESRV_DEBUG("CMceCsSubSession::DoServiceL( message ), NOT SUPPORTED")
    User::Leave( KErrNotSupported );
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::PendingTransactions
// -----------------------------------------------------------------------------
//
const RPointerArray<CSIPClientTransaction>& 
    CMceCsSubSession::PendingTransactions()
    {
    return iPendingTransactions;
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::PendingReceivedRequests 
// -----------------------------------------------------------------------------
//
const RPointerArray<CSIPServerTransaction>&
    CMceCsSubSession::PendingReceivedRequests()
    {
    return iPendingReceivedRequests;
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::PendingTransaction
// -----------------------------------------------------------------------------
//
CSIPClientTransaction* CMceCsSubSession::PendingTransaction( RStringF aType )
    {
    CSIPClientTransaction* trx = NULL;
    TInt index = PendingTransactions().Count()-1;
    
    while( !trx && index >= 0 )
        {
        trx = PendingTransactions()[ index-- ];
        TBool isMatch = trx->Type() == aType;
        trx = isMatch ? trx : NULL;
        }
        
    return trx;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::RegisterDialogAssoc
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::RegisterDialogAssoc( CSIPDialogAssocBase& aDialogAssoc )
    {
    iCurrentDialog = &aDialogAssoc;
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::ResponseReceivedWithinDialog
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::ResponseReceivedWithinDialog( CSIPClientTransaction& aTransaction,
                                                  	 CSIPDialogAssocBase& aDialogAssoc )
    {
    RegisterDialogAssoc( aDialogAssoc );
    DialogResponseReceived( aTransaction );
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::DialogRequestReceived
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::DialogRequestReceived( 
                                        CSIPServerTransaction* aTransaction, 
                                        CSIPDialog& aDialog )
    {
    RemoveCompletedRequests();
    TBool consumed = ETrue;
    
    CSIPServerTransaction* firstRequest = FIRST_REQUEST();
    
    MCESRV_DEBUG("CMceCsSubSession::DialogRequestReceived, Entry")
    if ( !AcceptDialogTransaction( *aTransaction ) )
        {
        consumed = EFalse;
        }
    else
        {
        MCESRV_DEBUG("no pending request")
        
        consumed = iPendingReceivedRequests.Append( aTransaction ) == KErrNone;
        if ( consumed )
            {
            RequestReceived( ETrue, aDialog );
            }
        }
    
    MCESRV_DEBUG("CMceCsSubSession::DialogRequestReceived, Exit")
    return consumed;    
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::SetResponse
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::SetResponse( CSIPClientTransaction& aTransaction )
    {
    
    if ( iResponse && 
         iPendingTransactions.Find( iResponse ) < 0 )
        {
        iInitialInvite = iResponse == iInitialInvite ? NULL : iInitialInvite;
        delete iResponse;
        iResponse = NULL;
        }

    iResponse = &aTransaction;
    
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::RemovePendingTrx
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::RemovePendingTrx( 
                                        CSIPClientTransaction& aTransaction )
    {
    TBool removed = EFalse;
    
    if ( iInitialInvite ? !( *iInitialInvite == aTransaction ) : ETrue )
        {
        TInt index = iPendingTransactions.Find( &aTransaction );
        iPendingTransactions.Remove( index );
        removed = ETrue;
        }
        
    return removed;    
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::RemovePendingCompletedTrx
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::RemovePendingCompletedTrx( 
                                        CSIPClientTransaction& aTransaction )
    {
    TBool removed = EFalse;
    
    TBool trxCompleted = MceSip::TrxCompleted( aTransaction );
    
    if ( trxCompleted )
        {
        RemovePendingTrx( aTransaction );
        }
        
    return removed;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::RemoveCompletedRequests
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::RemoveCompletedRequests( TBool aAll )
    {
    TInt ind = iPendingReceivedRequests.Count() - 1;
    while ( ind >= 0 )
        {
        CSIPServerTransaction* transaction = iPendingReceivedRequests[ ind ];
        if ( MceSip::TrxCompleted( *transaction ) )
            {
            // Don't remove the incoming invite until ACK has been received
            if ( MceSip::TrxType( *transaction ) != SipStrConsts::EInvite || aAll )
            	{
            	iPendingReceivedRequests.Remove( ind );
            	delete transaction;
            	}
            }
        ind--;
        }
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::InitializeIncomingDialogL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::InitializeIncomingDialogL( 
                                        CSIPServerTransaction* aTransaction )
    {
    MCESRV_DEBUG("CMceCsSubSession::InitializeIncomingDialogL, Entry")
    __ASSERT_ALWAYS( !CURRENT_REQUEST(), User::Leave( KErrArgument ) );
    DoInitializeIncomingDialogL( *aTransaction );
    iPendingReceivedRequests.AppendL( aTransaction );

    RequestReceived( EFalse, Dialog()->Dialog() );
        
    MCESRV_DEBUG("CMceCsSubSession::InitializeIncomingDialogL, Exit")
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::DialogResponseReceived
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::DialogResponseReceived( 
                                        CSIPClientTransaction& aTransaction )
    {
    MCESRV_DEBUG("CMceCsSubSession::DialogResponseReceived, Entry")
    SetResponse( aTransaction );
    switch ( MceSip::ResponseType( aTransaction ) )
        {
        case E1XX:
            {
            ProvisionalResponseReceived();
            break;
            }
        case E2XX:
            {
            ResponseReceived();
            break;
            }
        case E3XX:
            {
            RedirectionResponseReceived();
            break;
            }
        case E4XX:
        case E5XX:
        case E6XX:
        default:
            {
            ErrorResponseReceived();
            break;
            }
        }
        
    RemovePendingCompletedTrx( aTransaction );
    MCESRV_DEBUG("CMceCsSubSession::DialogResponseReceived, Exit")
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::MessageRequestReceived
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::MessageRequestReceived( 
                                        CSIPServerTransaction* aTransaction )
    {
    MCESRV_DEBUG("CMceCsSubSession::MessageRequestReceived, Entry")
    TBool consumed = AcceptStandAloneTransaction( *aTransaction );
    RemoveCompletedRequests();
    if ( consumed )
        {
        consumed = iPendingReceivedRequests.Append( aTransaction ) == KErrNone;
        if ( consumed )
            {
            StandAloneRequestReceived();
            }
        }
    MCESRV_DEBUG("CMceCsSubSession::MessageRequestReceived, Exit")
    return consumed;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::MessageResponseReceived
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::MessageResponseReceived( 
                                        CSIPClientTransaction& aTransaction )
    {
    MCESRV_DEBUG("CMceCsSubSession::MessageResponseReceived, Entry")
    
    SetResponse( aTransaction );
    
    StandAloneResponseReceived( MceSip::ResponseType( aTransaction ) );
    
    RemovePendingCompletedTrx( aTransaction );
    
    MCESRV_DEBUG("CMceCsSubSession::MessageResponseReceived, Exit")
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::InviteCompleted
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::InviteCompleted( CSIPClientTransaction& aTransaction )
    {
    MCESRV_DEBUG("CMceCsSubSession::InviteCompleted, Entry")
    
    TInt index = iPendingTransactions.Find( &aTransaction );
    if ( index != KErrNotFound )
        {
        iPendingTransactions.Remove( index );
        if ( iInitialInvite != iResponse )
            {
            delete iInitialInvite;
            iInitialInvite = NULL;
            }
        }
    else
        {
        MCESRV_DEBUG("InviteCompleted: NO PENDING TRANSACTION")
        }
        
    MCESRV_DEBUG("CMceCsSubSession::InviteCompleted, Exit")
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::ErrorOccured( TInt aError, 
                                     CSIPTransactionBase& aTransaction )
    {
    MCESRV_DEBUG("CMceCsSubSession::ErrorOccured, Entry")
    MCESRV_DEBUG_DVALUE("ERROR", aError )

    if ( aTransaction.IsSIPClientTransaction() )
        {
        CSIPClientTransaction& clientTransaction = 
            static_cast<CSIPClientTransaction&>( aTransaction );
        SetResponse( clientTransaction );
        DoErrorOccured( aError, clientTransaction );
        RemovePendingCompletedTrx( clientTransaction );
        }
    else
        {
        DoErrorOccured( aError, aTransaction );
        }
        
    MCESRV_DEBUG("CMceCsSubSession::ErrorOccured, Exit")
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::Dialog
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceCsSubSession::Dialog() const
    {
    return iDialog;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::ServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::ServiceL( TMceIds& aIds, 
                                 TMceItcFunctions aFunction, 
								 const RMessage2& aMessage)
    {
    MCESRV_DEBUG("CMceCsSubSession::ServiceL, Entry")
    MCESRV_DEBUG_EVENT("ITC", aFunction )
    
    User::LeaveIfNull( iReceiver );
        
    switch (aFunction)
		{
        case EMceItcClientReadyToReceive: // Asynchronous. Do not complete yet.
        	{
            ClientReadyToReceiveL (aMessage); 
            return;  
			}
        case EMceItcClientCancelReceive:
        	{
            CancelClientReceiveL ();
            break;		    
        	} 
        case EMceItcReceive:
        	{
            ReceiveL (aMessage);
            break;		    
        	} 
        default: 
        	{
        	DoServiceL(aIds, aFunction, aMessage );
        	}
        }
	iReceiver->ITC().WriteL(aMessage, aIds);
    iReceiver->ITC().Complete(aMessage, KErrNone);
    
    MCESRV_DEBUG("CMceCsSubSession::ServiceL, Exit")
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::DoServiceL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::DoServiceL( TMceIds& aIds, 
                                   TMceItcFunctions aFunction, 
								   const RMessage2& aMessage )
    {
    MCESRV_DEBUG("CMceCsSubSession::DoServiceL, Entry")
    MCESRV_DEBUG_IDS("IDS", aIds )

    // Clean completed requests
    RemoveCompletedRequests();
    
    TMceMessageDispatcher<CMceCsSubSession> dispatcher( *this, iReceiver->ITC() );
    
    dispatcher.DispatchL( aIds, aFunction, aMessage );
        
    MCESRV_DEBUG("CMceCsSubSession::DoServiceL, Exit")
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::SendErrorToClient
// -----------------------------------------------------------------------------
//
TInt CMceCsSubSession::SendErrorToClient( TMceIds& aIds, TInt aError )
    {
    MCESRV_DEBUG("CMceCsSubSession::SendErrorToClient, Entry")
    MCESRV_DEBUG_DVALUE("error", aError )
    
    TInt status = KErrNone;
    if ( !IsOrphan() )
        {
        TMceIds ids = iReceiver->Ids();
        ids.Copy( aIds );
        
        MCESRV_DEBUG_IDS("IDS", ids )

        TRAP( status, iReceiver->SendToClientL( ids, aError ) );
        }
    else
        {
        MCESRV_DEBUG("orphan -> no recipient to send")
        }
        
    MCESRV_DEBUG("CMceCsSubSession::SendErrorToClient, Exit")
    return status;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::SendToClient
// -----------------------------------------------------------------------------
//
TInt CMceCsSubSession::SendToClient( TMceIds& aIds )
    {
    MCESRV_DEBUG("CMceCsSubSession::SendToClient( ids ), Entry")
    
    TInt status = KErrNone;
    if ( !IsOrphan() )
        {
        TMceIds ids = iReceiver->Ids();
        ids.Copy( aIds );
        
        MCESRV_DEBUG_IDS("IDS", ids )
        
        TRAP( status, iReceiver->SendToClientL( ids ) );
        }
    else
        {
        MCESRV_DEBUG("orphan -> no recipient to send")
        }
        
    MCESRV_DEBUG("CMceCsSubSession::SendToClient( ids ), Exit")
    return status;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::SendToClientL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::SendToClientL( TMceIds& aIds, 
                                      HBufC8* aContext, 
                                      HBufC8* aContent )
    {
    MCESRV_DEBUG("CMceCsSubSession::SendToClient(ids,context,content), Entry")
    
    if ( !IsOrphan() )
        {
        TMceIds ids = iReceiver->Ids();
        ids.Copy( aIds );

        MCESRV_DEBUG_IDS("IDS", ids )
                        
        if ( aContent )
            {
            iReceiver->SendToClientL( ids, aContext, aContent );
            }
        else
            {
            iReceiver->SendToClientL( ids, aContext );
            }
        }
    else
        {
        delete aContext;
        delete aContent;
        }

    MCESRV_DEBUG("CMceCsSubSession::SendToClient(ids,context,content), Exit")
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::ClientReadyToReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::ClientReadyToReceiveL( const RMessage2& aMessage )
	{
    MCESRV_DEBUG("CMceCsSubSession::ClientReadyToReceiveL, Entry")
	
    iReceiver->ClientReadyToReceiveL( aMessage );
    iClientExists = ETrue;
    
    MCESRV_DEBUG("CMceCsSubSession::ClientReadyToReceiveL, Exit")
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::CancelClientReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::CancelClientReceiveL()
	{
    MCESRV_DEBUG("CMceCsSubSession::CancelClientReceiveL, Entry")
    
    iClientExists = EFalse;
    iReceiver->CancelClientReceiveL();
    
    MCESRV_DEBUG("CMceCsSubSession::CancelClientReceiveL, Exit")
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::ReceiveL
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::ReceiveL( const RMessage2& aMessage )
	{
    MCESRV_DEBUG("CMceCsSubSession::ReceiveL, Entry")
	
    iReceiver->ReceiveL (aMessage);	
    
    MCESRV_DEBUG("CMceCsSubSession::ReceiveL, Exit")
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::ClientExists
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::ClientExists()
	{
    return iClientExists;
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::DefaultHeaders
// -----------------------------------------------------------------------------
//
CDesC8Array& CMceCsSubSession::DefaultHeaders( TInt aMethodInd )
    {
    return Client().DefaultHeaders( aMethodInd );
    }
    
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::StoreClientContent
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::StoreClientContent( HBufC8* aClientContent )
    {
    delete iClientContent;
    iClientContent = aClientContent;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::PopClientContent
// -----------------------------------------------------------------------------
//
HBufC8* CMceCsSubSession::PopClientContent()
    {
    HBufC8* clientContent = iClientContent;
    iClientContent = NULL;
    return clientContent;
    }


// -----------------------------------------------------------------------------
// CMceCsSubSession::ConnectionStateChanged
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::ConnectionStateChanged( CSIPConnection::TState aState )
    {
    TBool isActive = EFalse;
    if ( aState == CSIPConnection::EActive )
        {
        isActive = ETrue;
        }
    
    DoConnectionStateChanged( isActive );
    
    TMceIds ids;
    ids.iCallbackID = EMceItcSessionConnectionStateChanged;
	ids.iState = isActive;
	SendToClient( ids );
    }
        
// ---------------------------------------------------------------------------------
// CMceCsSubSession::ResetInitialInvite
// ---------------------------------------------------------------------------------    
//
void CMceCsSubSession::ResetInitialInvite()
    {
    iInitialInvite = NULL;
    }

// ---------------------------------------------------------------------------------
// CMceCsSubSession::SetAutoEvent
// ---------------------------------------------------------------------------------    
//
void CMceCsSubSession::SetAutoEvent( TBool aIsEnabled )
    {
    iAutoEventEnabled = aIsEnabled;
    }

// ---------------------------------------------------------------------------------
// CMceCsSubSession::AutoEvent
// ---------------------------------------------------------------------------------    
//   
TBool CMceCsSubSession::AutoEvent()
    {
    return iAutoEventEnabled;
    }
	
// -----------------------------------------------------------------------------
// CMceCsSubSession::StoreAutoEventL
// -----------------------------------------------------------------------------
//
/*lint -e506*/
void CMceCsSubSession::StoreAutoEventL( 
    TMceStateTransitionEvent* aEvent, 
    CSIPServerTransaction* aAssociatedRequest )
    {
    delete iAutoEventMsg;
    iAutoEventMsg = NULL;
    delete iAutoEventIds;
    iAutoEventIds = NULL;
    delete iAutoEvent;
    iAutoEvent = NULL;
    
    if ( aEvent )
        {
        // Have to duplicate certain elements as their lifetime may end
        // while waiting for auto accept.
        TMceIds* ids = NULL;
        if ( &aEvent->ParamIDs() )
            {
            ids = new ( ELeave ) TMceIds( aEvent->ParamIDs() );
            }
        else
            {
            ids = new ( ELeave ) TMceIds;
            }
        CleanupStack::PushL( ids );
        
        CMceMsgBase& clonedMsg = aEvent->ParamClientMessage();
        if ( &clonedMsg )
            {
            CMceMsgSIPReply* msg = new ( ELeave ) CMceMsgSIPReply();
            CleanupStack::PushL( msg );
            msg->PushL();
            clonedMsg.EncodeL();
            msg->DecodeL( clonedMsg.EncodeBuffer().Ptr( 0 ) );
            iAutoEventMsg = msg;
            CleanupStack::Pop( msg );
            }

        iAutoEventIds = ids;
        CleanupStack::Pop( ids );
        
        iAutoEvent = new ( ELeave ) TMceStateTransitionEvent;
        *iAutoEvent = *aEvent;
        iAutoEvent->SetParamIDs( *iAutoEventIds );
        iAutoEvent->SetParamClientMessage( iAutoEventMsg );
        }
        
    iAutoEventAssociatedRequest = aAssociatedRequest;
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::StoredAutoEvent
// -----------------------------------------------------------------------------
//		
TMceStateTransitionEvent* CMceCsSubSession::StoredAutoEvent()
    {
    return iAutoEvent;
    }
    
// -----------------------------------------------------------------------------
// CMceCsSubSession::Profile
// -----------------------------------------------------------------------------
//
CSIPProfile& CMceCsSubSession::Profile() const
	{
	return *iProfile;
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::SetProfile
// -----------------------------------------------------------------------------
//
void CMceCsSubSession::SetProfile( CSIPProfile& aProfile )
	{
	iProfile = &aProfile;
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::ProfileConfigured
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::ProfileConfigured() const
	{
	return ( iProfile != 0 );
	}

// -----------------------------------------------------------------------------
// CMceCsSubSession::NextHopL
// -----------------------------------------------------------------------------
//
TInetAddr* CMceCsSubSession::NextHopL( TInetAddr& aNextHop )
    {
    TInetAddr* nextHop = NULL;
    
    const TDesC8* registeredContact = NULL;
    
    if ( ProfileConfigured() && Profile().GetParameter( KSIPRegisteredContact, 
    					registeredContact ) == KErrNone )
        {
        CSIPContactHeader* contact = MceSip::ToContactHeaderL( *registeredContact );
        CleanupStack::PushL( contact );
        const CUri8& contactUri = contact->SIPAddress()->Uri8();
        const TDesC8& contactUriHost = contactUri.Uri().Extract( EUriHost );
        TBuf<KMaxAddressLength> addr16;
        addr16.Copy( contactUriHost );
        if ( aNextHop.Input( addr16 ) == KErrNone )
            {
            CleanupStack::PopAndDestroy( contact );
            nextHop = &aNextHop;
            }
        }
        
    return nextHop;    
    }

// -----------------------------------------------------------------------------
// CMceCsSubSession::IsProfileContactSecureL
// -----------------------------------------------------------------------------
//
TBool CMceCsSubSession::IsProfileContactSecureL( )
    {
    MCESRV_DEBUG("CMceSipConnection::IsProfileContactSecureL, Entry")
    const TDesC8* registeredContact = NULL;
    TBool contactSecure = EFalse;
    if ( ProfileConfigured() &&
         Profile().GetParameter( KSIPRegisteredContact, 
                                 registeredContact ) == KErrNone )
        {
        CSIPContactHeader* contact = MceSip::ToContactHeaderL( *registeredContact );
        CleanupStack::PushL( contact );
        const CUri8& contactUri = contact->SIPAddress()->Uri8();
        const TDesC8& uriScheme = contactUri.Uri().Extract( EUriScheme );
		const TDesC8& uriPath = contactUri.Uri().Extract( EUriPath );
		if ( uriScheme.FindF( KMceSipUriSchemeSIPs ) != KErrNotFound ||
			uriPath.FindF( KMceSipTransportTLS ) != KErrNotFound )
			{
			contactSecure = ETrue;
			MCESRV_DEBUG("profile contact header has sips uri or tls")
			}
			
		CleanupStack::PopAndDestroy( contact );
		return contactSecure;
		}
	MCESRV_DEBUG("CMceSipConnection::IsProfileContactSecureL, Entry")
    return contactSecure;
    }

//  End of File
