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




#ifndef CMCECSSUBSESSION_H
#define CMCECSSUBSESSION_H

//  INCLUDES
#include <e32base.h>
#include <stringpool.h>
#include <sipconnection.h>
#include "mcesrv.h"
#include "mceclientserver.h"
#include "mcesip.h"
#include "fcsession.h"


//FORWARD DECLARATION
class CSIPProfile;
class CMceCsSession;
class CMceCsSessionImplementation;
class CMceCsSubSessionReceiver;
class CMceServerCore;
class TMceIds;
class CMceMsgBase;
class CMceSipManager;
class CMceSipConnection;
class CSIPServerTransaction;
class CSIPClientTransaction;
class CSIPDialogAssocBase;
class CSIPClientTransaction;
class CSIPTransactionBase;
class CSIPDialog;
class CDesC8Array;
class TMceStateTransitionEvent;
class CSIPProfileRegistry;

#define LAST_REQUEST_IND iPendingReceivedRequests.Count()-1
#define CURRENT_REQUEST_IND LAST_REQUEST_IND

#define CURRENT_REQUEST()\
    ( iPendingReceivedRequests.Count() == 0 ?\
      NULL : iPendingReceivedRequests[ LAST_REQUEST_IND ] )

#define FIRST_REQUEST()\
    ( iPendingReceivedRequests.Count() == 0 ?\
      NULL : iPendingReceivedRequests[ 0 ] )

// CLASS DECLARATION

/**
* Client-Server sub-session is the base class
* for SIP sessions, which have created to encapsulate
* SIP dialogs.
* This class provides methods to handle CS subsession
* specifig ITC functions and to receive SIP requests and 
* responses.
*
*  @lib 
*  @since
*/
class CMceCsSubSession : public CBase
    {

public: //enums

    enum TType
        {
        EUnknown = -1,
        EInSession,
        EOutSession,
        EOutSIPSession
        };
        
public: // Constructors and destructor


	/**
	* Destructor.
	*/
	~CMceCsSubSession();

public: // New functions


	/**
	* Returns client type of session.
	* The type represents type of dialog used in session.
	* @return client type of session
	*/
    TMceCsSessionType ClientType() const;

	/**
	* Returns type of session
	* @return type of session
	*/
    TType Type() const;
    
	/**
	* Returns SIP connection instance
	* @return SIP connection
	*/
    CMceSipConnection& SIPConnection() const;

	/**
	* Returns SIP manager instance
	* @return SIP manager
	*/
    CMceSipManager& Manager() const;
    
	/**
	* Returns server core instance
	* @return server core
	*/
	CMceServerCore& ServerCore() const;

    /**
    * Returns client
    * @return client
    */
    CMceCsSessionImplementation& Client() const;

	/**
	* Returns floor control session instance
	* @return floor control session
	*/
	const CFCSession* FCSession() const;

	/**
	* Executes ITC function called by client
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @param aMessage message from the client.
	*/
	void ServiceL( TMceIds& aIds, 
	               TMceItcFunctions aFunction, 
	               const RMessage2& aMessage );
	
	/**
	* Sends data via callback mechanism to client session
	* @param aIds set of ids defining ITC callback context
	* @return system wide error
	*/
    TInt SendToClient( TMceIds& aIds );
    
	/**
	* Sends data via callback mechanism to client session
	* @param aIds set of ids defining ITC callback context
	* @param aContext encoded context of message
	* @param aContent content of message
	*/
    void SendToClientL( TMceIds& aIds, HBufC8* aContext, HBufC8* aContent );

	/**
	* Sends error via callback mechanism to client session
	* @param aIds set of ids defining ITC callback context
	* @param aError the error
	* @return system wide error
	*/
	TInt SendErrorToClient( TMceIds& aIds, TInt aError );
	
    
    /**
	* Resets the initial INVITE.
	*/
    void ResetInitialInvite();
    
    /**
	* Controls whether the session should generate automatically event 
	* at next state change.
	* @param aIsEnabled
	*/
    void SetAutoEvent( TBool aIsEnabled );
    
    /**
	* Checks whether the session should automatically generate an event 
	* at next state change.
	* @return ETrue if event should be generated, otherwise EFalse
	*/
	TBool AutoEvent();
	
	/**
	* Stores event which will be used when automatic event generation
	* occurs. Request which is associated with the event may be defined.
	* @param aEvent, ownership is not transferred
	* @param aAssociatedRequest, ownership is not transferred
	*/
	void StoreAutoEventL( TMceStateTransitionEvent* aEvent,
	                      CSIPServerTransaction* aAssociatedRequest );
	
    /**
	* Returns stored automatic event.
	* @return event or null
	*/
	TMceStateTransitionEvent* StoredAutoEvent();
    
    /**
    * Checks if profile configured.
    *
    * @return return ETrue if profile configured.
    */
	TBool ProfileConfigured() const;
	
	/**
    * Return profile
    *
    * @return return profile of the connection
    */
	CSIPProfile& Profile() const;

    /**
    * Set profile
    *
    * @param aProfile Profile to be used
    */
    void SetProfile( CSIPProfile& aProfile );

    /**
    * Check Profile contact secure status 
    *
    * @return True if profile contact with sipsUri or tls
    */
	TBool IsProfileContactSecureL();

	/**
    * Return NextHop
    *
    * @return NextHop TInetAddr
    */
	TInetAddr* NextHopL( TInetAddr& aNextHop );
	
	void RemoveCompletedRequests( TBool aAll = EFalse );
    
public://SIP adaptation methods


	/**
	* Initializes incoming dialog
	* @param aTransaction server transaction representing a request,
	* which caused dialog creation
	*/
    void InitializeIncomingDialogL( CSIPServerTransaction* aTransaction );

	/**
	* Handles SIP request, which has been received inside a dialog
	* @param aTransaction server transaction representing a request
	* @param aDialog dialog to wich transaction belongs
	* @return ETrue, if session handled the request
	*/
    TBool DialogRequestReceived( CSIPServerTransaction* aTransaction,
    											CSIPDialog& aDialog );

	/**
	* Handles SIP request, which has been received inside a dialog.
	* this will call DialogResponseReceived
	* @param aTransaction server transaction representing a request
	* @param aDialogAssoc the dialog
	*/
    void ResponseReceivedWithinDialog( CSIPClientTransaction& aTransaction,
                                       	CSIPDialogAssocBase& aDialogAssoc );
    
	/**
	* Handles SIP response, which has been received inside a dialog
    * @param aTransaction client transaction representing a response
	*/
    void DialogResponseReceived( CSIPClientTransaction& aTransaction  );

    /**
    * Handles SIP request, which has been received outside a dialog
    * @param aTransaction server transaction representing a request
	* @return ETrue, if session handled the request
    */
    TBool MessageRequestReceived( CSIPServerTransaction* aTransaction );
    
	/**
	* Handles SIP response, which has been received outside a dialog
    * @param aTransaction client transaction representing a response
	*/
    void MessageResponseReceived( CSIPClientTransaction& aTransaction );

    /*
	* Handles situation where SIP stack has completed UAC core INVITE 
	* transaction 64*T1 seconds after the reception of the first 
	* 2xx response. No more 2xx responses can be received to the issued 
	* single INVITE.
    * @param aTransaction client transaction representing a response
    */
    virtual void InviteCompleted( CSIPClientTransaction& aTransaction );
    
	/**
	* Handles situation where SIP connection state has changed.
    * If connection state has changed to EInactive or EUnavailable,
	* SIP stack terminates all pending SIP client transactions and no
    * errors are reported back to the client about the terminated
    * transactions.
	* @param aState indicates the current connection state        
	*/
    void ConnectionStateChanged( CSIPConnection::TState aState );

    /**
	* Handles situation where an asynchronous error has occured 
	* related to a request within an existing dialog.
    *
    * @param aError error code
    * @param aTransaction the transaction
    */
    virtual void ErrorOccured( TInt aError, CSIPTransactionBase& aTransaction );

	/**
	* Returns dialog id
	* @return dialog id
	*/
    TUint32 DialogId() const;
    
	/**
	* Returns ETrue, if client has called ready to receive
	* @return ETrue, if client has called ready to receive
	*/
    TBool ClientExists();
    
	/**
	* Returns SIP dialog (assoc)
	* @return SIP dialog or NULL if it does not exist
	*/
    virtual CSIPDialogAssocBase* Dialog() const;
    
	/**
	* Returns current (pending) request
	* @return server transaction representing the request
	*/
    CSIPServerTransaction& Request() const;
    
    /**
    * Returns initial request which caused session creation.
    * @return server transaction representing the initial request
    */
    CSIPServerTransaction& InitialRequest() const;
    
	/**
	* Transferes ownership of current (pending) request
	* @return server transaction representing the request
	*/
    CSIPServerTransaction* PopRequest();
    
	/**
	* Returns current (pending) response
	* @return client transaction representing the request
	*/
    CSIPClientTransaction& Response() const;

	/**
	* Transferes ownership of current (pending) response
	* @return completed client transaction or NULL if there is no 
	* pending response or transaction is not completed.
	*/
    CSIPClientTransaction* PopResponse();

	/**
	* Stores SIP dialog 
	* @param aType the type of session
	* @param aDialog SIP dialog
	* @param aDialogId generated dialog id
	*/
    void SetDialog( TType aType, 
                    CSIPDialogAssocBase* aDialog, 
                    TUint32 aDialogId );
    
	/**
	* Sets client transaction which is waiting response from peer
	* @param aTransaction the pending transaction
	*/
    void SetPendingTransactionL( CSIPClientTransaction* aTransaction );

	/**
	* Returns ETrue, if session will consume the transaction
	* @param aDialog SIP dialog
	* @param aTransaction the pending transaction
	* @return ETrue, if session will consume the transaction
	*/
    virtual TBool Consumes( CSIPDialog& aDialog, 
                            CSIPServerTransaction& aTransaction, 
                            TBool& aAssociatesWithDialog );

	/**
	* Returns ETrue, if session will consume the transaction
	* @param aTransaction pending or new (incoming) transaction
	* @return ETrue, if session will consume the transaction
	*/
    virtual TBool Consumes( CSIPTransactionBase& aTransaction );

	/**
	* Returns ETrue, if session accepts transaction
	* @param aTransaction new (incoming) transaction
	* @return ETrue, if session will accepts the transaction
	*/
    virtual TBool AcceptDialogTransaction( CSIPServerTransaction& aTransaction );

	/**
	* Returns ETrue, if session accepts stand-alone transaction
	* @param aTransaction new (incoming) stand-alone transaction
	* @return ETrue, if session will accepts the stand-alone transaction
	*/
    virtual TBool AcceptStandAloneTransaction( CSIPServerTransaction& aTransaction );


	/**
	* Returns all pending transactions
	* @return pending transactions as an array
	*/
    const RPointerArray<CSIPClientTransaction>& PendingTransactions();
    
    
    const RPointerArray<CSIPServerTransaction>& PendingReceivedRequests();

	/**
	* Returns pending transaction based on type
	* @param aType type of transaction
	* @return pending transactions or NULL. Ownership is NOT transfered
	*/
    CSIPClientTransaction* PendingTransaction( RStringF aType );

    /**
    * Returns the default headers
    * @param aMethodInd the methond index in SIP strings
    * @return default headers
    */
    CDesC8Array& DefaultHeaders( TInt aMethodInd );
    
    /**
    * Stores client's message content
    * @param aClientContent the message content
    */
    void StoreClientContent( HBufC8* aClientContent );

    /**
    * Transferes ownership of client message content
    * @return client content
    */
    HBufC8* PopClientContent();
    
    /**
    * Removes the pending transaction.
    * @param aTransaction the transaction to be removed
    * @return ETrue if the transaction was removed.
    */
    TBool RemovePendingTrx( CSIPClientTransaction& aTransaction );
    
public://abstract methods


	/**
	* Executes ITC function called by client.
	* Default implementation leaves with KErrNotSupported
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @return aReturnMessage buffer to store message context,
	* which will be wrote back to client
	*/
    virtual HBufC8* DoServiceL( TMceIds& aIds, 
                                TMceItcFunctions aFunction );

	/**
	* Executes ITC function called by client.
	* Default implementation leaves with KErrNotSupported
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @param aMessage message context from client
	*/
    virtual void DoServiceL( TMceIds& aIds, 
                             TMceItcFunctions aFunction, 
		     	             const TDesC8& aMessage );

	/**
	* Executes ITC function called by client.
	* Default implementation leaves with KErrNotSupported
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @param aMessage message context from client
	*/
    virtual void DoServiceL( TMceIds& aIds, 
                             TMceItcFunctions aFunction, 
		     	             TPtr8& aMessage );


	/**
	* Executes ITC function called by client.
	* Default implementation leaves with KErrNotSupported
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @param aMessage decoded message context from client
	*/
    virtual void DoServiceL( TMceIds& aIds, 
                             TMceItcFunctions aFunction, 
		     	             CMceMsgBase& aMessage );
	
	/**
	* Returns ETrue, if session can be deleted
	* @return ETrue, if session can be deleted
	*/
    virtual TBool CanDispose() = 0;

	/**
	* Initializes incoming dialog
	* @param aTransaction server transaction representing a request,
	* which caused dialog creation
	*/
    virtual void DoInitializeIncomingDialogL( 
                            CSIPServerTransaction& aTransaction ) = 0;
    
    /**
	* Handles situation where an asynchronous error has occured 
	* related to a request within an existing dialog.
    *
    * @param aError error code
    * @param aTransaction the transaction
    */
    virtual void DoErrorOccured( TInt aError, 
                                 CSIPTransactionBase& aTransaction ) = 0;
    
    /**
    * Called when error has been occured within dialog
    * @param aError error code
    */
    virtual void ErrorOccured( TInt aError ) = 0;
    
    /**
	* Handles situation where SIP connection state has changed.
    * If connection state has changed to EInactive or EUnavailable,
	* SIP stack terminates all pending SIP client transactions and no
    * errors are reported back to the client about the terminated
    * transactions.
	* @param aIsActive indicates if the current connection is active
    */
    virtual void DoConnectionStateChanged( TBool aIsActive ) = 0;


    /**
    * Handles situation where incoming INVITE was 
    * canceled with the CANCEL
    */
    virtual void Canceled() = 0;

    /**
    * Handles situation where session must be forcefully terminated
    * @return ETrue, if session can be deleted. If there are 
    * pending operations, which cannot be done within this call, EFalse
    * is returned.
    */
    virtual TBool Terminate() = 0;
    
    /**
    * Creates new incoming dialog (association)
    * @param aInitialRequest request causing creation
    * @return created dialog association
    */
    virtual CSIPDialogAssocBase* CreateIncomingDialogL( 
                                CSIPServerTransaction& aInitialRequest ) = 0;

    /**
    * Creates new outgoing dialog (association) from existing dialog
    * @param aDialogType type of new dialog
    * @param aParams parameters for creation
    * @param aExistingDialog existing dialog
    * @return created dialog association
    */
    virtual CSIPDialogAssocBase* CreateOutgoingDialogL( 
                                    TMceDialogType aDialogType, 
                                    CDesC8Array& aParams,
                                    CSIPDialog& aExistingDialog ) = 0;

    /**
    * Creates outgoing dialog (association) based on given parameters 
    * @param aDialogType type of new dialog
    * @param aParams parameters for creation
    * @return created dialog association
    */
    virtual CSIPDialogAssocBase* CreateOutgoingDialogL( 
                                    TMceDialogType aDialogType,
                                    CDesC8Array& aParams ) = 0;

    /**
    * Checks if session accepts method of incoming request
    * @param aMethod the method
    * @return ETrue, if session will accept
    */
    virtual TBool AcceptMethod( RStringF aMethod ) = 0;
    

    /**
    * Called when request has been received. 
    * Request can be obtained by calling Request() method
    * @param aInsideDialog tells whether request was received inside dialog
    */
    virtual void RequestReceived( TBool aInsideDialog, CSIPDialog& aDialog  ) = 0;

    
    /**
    * Called when response (2XX) has been received to pending transaction.
    * Response can be obtained by calling Response() method
    */
    virtual void ResponseReceived() = 0;
    
    /**
    * Called when response (1XX) has been received to pending transaction.
    * Response can be obtained by calling Response() method
    */
    virtual void ProvisionalResponseReceived() = 0;
    
    /**
    * Called when response (3XX) has been received to pending transaction.
    * Response can be obtained by calling Response() method
    */
    virtual void RedirectionResponseReceived() = 0;
     
    /**
    * Called when error response (4XX-7XX) has been received 
    * to pending transaction. 
    * Response can be obtained by calling Response() method
    */
    virtual void ErrorResponseReceived() = 0;
    
    /**
    * Called when stand-alone request has been received.
    * Request can be obtained by calling Request() method
    */
    virtual void StandAloneRequestReceived() = 0;
    
    /**
    * Called when stand-alone response (any) has been received 
    * to pending transaction. 
    * Response can be obtained by calling Response() method
    * @param aResponseType response type
    */
    virtual void StandAloneResponseReceived( 
                            TMceSipResponseType aResponseType ) = 0;
    

    
protected: 

	CMceCsSubSession ( TMceCsSessionType aType,
                       CMceCsSession& aClientSession,
	                   CMceSipConnection& aSIPConnection );

	CMceCsSubSession ( TMceCsSessionType aType,
                       CMceCsSession& aClientSession,
	                   CMceSipConnection& aSIPConnection,
	                   CSIPProfile& aProfile );

	void ConstructL( CMceCsSession& aClientSession );
    TBool IsOrphan();
    
    TBool HasInitialInviteTransactionBeenCompleted() const; 
	
private:

    void DoServiceL( TMceIds& aIds, 
                     TMceItcFunctions aFunction, 
		     	     const RMessage2& aMessage );
	void ClientReadyToReceiveL( const RMessage2& aMessage );
	void CancelClientReceiveL();
	void ReceiveL( const RMessage2& aMessage );

    void SetClient( CMceCsSessionImplementation& aClient );
    void SetSessionReceiver( CMceCsSubSessionReceiver* aSessionReceiver );
    void SetResponse( CSIPClientTransaction& aTransaction );
    TBool RemovePendingCompletedTrx( CSIPClientTransaction& aTransaction );
    void RegisterDialogAssoc( CSIPDialogAssocBase& aDialogAssoc );
    
    
private:// Owned Data    

    //type of session
    TType iType;
    //client type of session
    TMceCsSessionType iClientType;
    //receiver    
	CMceCsSubSessionReceiver* iReceiver;
    //dialog id
    TUint32 iDialogId;
    //current response
    CSIPClientTransaction* iResponse;
    //first outgoing invite
    CSIPClientTransaction* iInitialInvite;
    //does client exist
    TBool iClientExists;
    //client message content
	HBufC8* iClientContent;

protected: // Owned Data
    
    //SIP dialog
    CSIPDialogAssocBase* iDialog;
    //pending transactions
    RPointerArray<CSIPClientTransaction> iPendingTransactions;
    //pending received requests
    RPointerArray<CSIPServerTransaction> iPendingReceivedRequests;
    
    //data for automatic event
    //        
    TBool iAutoEventEnabled;
    TMceStateTransitionEvent* iAutoEvent;
    CMceMsgBase* iAutoEventMsg;
    TMceIds* iAutoEventIds;
    CSIPServerTransaction* iAutoEventAssociatedRequest;
    TMceStateTransitionEvent* iCurrentlyProcessedAutoEvent;

protected: // NOT Owned Data
    
    //current dialog, if forking has been happend
    CSIPDialogAssocBase* iCurrentDialog;
    

private:// NOT owned Data

    //client    
	CMceCsSessionImplementation* iClientSession;
	//SIP connection
	CMceSipConnection& iSIPConnection;
	//MCE server core
	CMceServerCore& iServerCore;
	
	// Sip Profile
    CSIPProfile* iProfile;
    //CSIPProfileRegistry* iProfileRegistry;
    
    	
protected:// NOT owned Data
	
	//floor control session
	CFCSession* iFCSession;


    friend class TMceFCActionSet;
    friend class CMceSipManager;

	//definitions for unit testing
	MCESRV_UT_DEFINITIONS


	};

#endif 

// End of File
