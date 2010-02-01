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




#ifndef CMCESIPEVENT_H
#define CMCESIPEVENT_H

#include <e32base.h>

#include "mcesrv.h"
#include "mcecssubsession.h"
#include "mcesipmanager.h"
#include "mceevents.h"
#include "mcerefer.h"
#include "mceevent.h"

//  FORWARD DECLARATION
class CMceSipStateMachine;
class CMceComSession;
class CMceState;
class TMceActionSet;
class CMceComEvent;
class CIdle;
class CMceMsgSIPEvent;
class CMceMsgSIPData;
class CSIPProfile;

// CLASS DECLARATION

enum TEventStateType 
    {
	EMoSubscribe=1,  
	EMtSubscribe,
	EMoRefer,
	EMtRefer,  
	};    
	

const TUint KMceDefaultSubscrExpire = 3600;

_LIT8(KStateActive, "active");
_LIT8(KStateTerminated, "terminated");

	
/**
*  MCE SIP session.
*
*  @lib 
*  @since
*/
class CMceSipEvent : public CMceCsSubSession
    {
	
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aClientSession, reference to client session.
	* @param aSIPConnection, reference to sip connection 
	*/
	static CMceSipEvent* NewL (CMceCsSession& aClientSession, 
							   CMceSipConnection& aSIPConnection,
							   CSIPProfile& aProfile);

	/**
	* Two-phased constructor.
	* @param aClientSession, reference to session.
	* @param aSIPConnection, reference to sip connection 
	*/
	static CMceSipEvent* NewLC (CMceCsSession& aClientSession, 
							   CMceSipConnection& aSIPConnection,
							   CSIPProfile& aProfile);
	
	/**
	* Destructor.
	*/
	~CMceSipEvent();
    
public: // New Functions

    /**
	* Handles the incoming SUBSCRIBE or REFER request that caused this
	* event to be created.
	* @param aTransaction The received server transaction
	*/
	void EventRequestedL( CSIPServerTransaction* aTransaction );
	
public: // from CMceCsSubSession

    /**
    * Creates new incoming dialog
    * @param aInitialRequest request causing creation
    * @return created dialog
    */
    CSIPDialogAssocBase* CreateIncomingDialogL( CSIPServerTransaction& aInitialRequest );

    /**
    * Creates outgoing dialog from existing dialog
    * @param aDialogType type of new dialog
    * @param aParams parameters
    * @param aExistingDialog existing dialog
    * @return created dialog
    */
    CSIPDialogAssocBase* CreateOutgoingDialogL( TMceDialogType aDialogType, 
                                                CDesC8Array& aParams,
                                                CSIPDialog& aExistingDialog );

    /**
    * Creates outgoing dialog based on given parameters 
    * @param aDialogType type of new dialog
    * @param aParams parameters
    * @return created dialog
    */
    CSIPDialogAssocBase* CreateOutgoingDialogL( TMceDialogType aDialogType,
                                                        CDesC8Array& aParams );

    /**
	* Provides services of MCE server subsession
	* @param aIds, ids from the client.
	* @param aFunction, function from the client.
	* @return aReturnMessage, return message for client
	*/
    HBufC8* DoServiceL( TMceIds& aIds, 
                        TMceItcFunctions aFunction );

	/**
	* Provides services of MCE server subsession
	* @param aIds, ids from the client.
	* @param aFunction, function from the client.
	* @param aMessage, encoded message from the client.
	*/
    void DoServiceL( TMceIds& aIds, 
                   TMceItcFunctions aFunction, 
     	           const TDesC8& aMessage );


	/**
	* Provides services of MCE server subsession
	* @param aIds, ids from the client.
	* @param aFunction, function from the client.
	* @param aMessage, message from the client.
	*/
    void DoServiceL( TMceIds& aIds, 
                   TMceItcFunctions aFunction, 
     	           CMceMsgBase& aMessage );
	
	
	/**
	* Returns ETrue, if session can be deleted
	* @return ETrue, if session can be deleted
	*/
    TBool CanDispose();

    /**
    * Called when session is forced to terminate
    * @return ETrue, if can be disposed
    */
    TBool Terminate();

	/**
	* Initializes incoming dialog
	* @param aTransaction, server transaction containing the request
	*/
    void DoInitializeIncomingDialogL( CSIPServerTransaction& aTransaction );
			 
    /**
    * Called when error has been occured within dialog
    * @param aError error code
    */
    void ErrorOccured( TInt aError );
    
   /**
    * Called when error has been occured 
    * @param aError error code
    * @param aTransaction the transaction
    */
    void DoErrorOccured( TInt aError, CSIPTransactionBase& aTransaction );
     
    
    /**
    * Called when connection state has been changed
    * @param aIsActive ETrue, if connection is active
    */
    void DoConnectionStateChanged( TBool aIsActive );

    /**
    * Called when dialog has been cancelled
    */
    void Canceled();
    
    TEventStateType EventStateType() const;
    
    void IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody);
									
    void IncomingReferL(
					HBufC8* aReferTo,
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody);
	
	void ClientStateChangedL( CMceRefer::TState aState, TBool aIsResponse );
	
	void ClientStateChangedL( CMceEvent::TState aState, TBool aIsResponse );
		
	void NotifyReceivedL(			CDesC8Array* aHeaders,
                    				HBufC8* aContentType,
									HBufC8* aBody) ;
    
    void SuppressedReferTerminated();

protected: //CMceCsSubSession


	/**
	* Returns ETrue, if session accepts transaction
	* @param aTransaction the pending transaction
	* @return ETrue, if session will accepts the transaction
	*/
    TBool AcceptDialogTransaction( CSIPServerTransaction& aTransaction );

	/**
	* Returns ETrue, if session accepts transaction
	* @param aTransaction the pending transaction
	* @return ETrue, if session will accepts the transaction
	*/
    TBool AcceptDialogTransactionL( CSIPServerTransaction& aTransaction );
    
    /**
    * Checks if session accepts method containing in incoming request
    * @param aMethod the method
    * @return ETrue, if accepted
    */
    TBool AcceptMethod( RStringF aMethod );

    /**
    * Called when request has been received. 
    * Request can be obtained by calling Request() method
    * @param aInsideDialog tells whether request was received inside dialog
    * @param aDialog Dialog for which request is received
    */
    void RequestReceived( TBool aInsideDialog, CSIPDialog& aDialog);
    
    /**
    * Called when response (2XX) has been received to pending transaction.
    * Response can be obtained by calling Response() method
    */
    void ResponseReceived();
    
    /**
    * Called when response (1XX) has been received to pending transaction.
    * Response can be obtained by calling Response() method
    */
    void ProvisionalResponseReceived();
    
    /**
    * Called when response (3XX) has been received to pending transaction.
    * Response can be obtained by calling Response() method
    */
    void RedirectionResponseReceived();
     
    /**
    * Called when error response (4XX-7XX) has been received to pending transaction.
    * Response can be obtained by calling Response() method
    */
    void ErrorResponseReceived();
    
    /**
    * Called when stand-alone request has been received.
    * method must return ETrue, if session consumes the request 
    * Request can be obtained by calling Request() method
    *@return TConsumeStatus
    */
    void StandAloneRequestReceived();
    
    /**
    * Called when stand-alone response (any) has been received to pending transaction.
    * Response can be obtained by calling Response() method
    * @param aResponseType response type
    */
    void StandAloneResponseReceived( TMceSipResponseType aResponseType );
    
    
private:

	CMceSipEvent ( CMceCsSession& aClientSession, 
				   CMceSipConnection& aSIPConnection,
				   CSIPProfile& aProfile);

	void ConstructL ( CMceCsSession& aClientSession );
	
    void HandleSIPEvent( TMceSipEventCode aEventCode );
    
    void SetEventStateType(const TEventStateType aEventStateType );
   
    void DoClientStateChangedL( TUint32 aState, TBool aIsResponse );
    
    void ClearDataL();
    
    void UpdateDataL( TMceItcFunctions aFunction, 
                      CMceMsgSIPData& aMessage );
    
    void DoInitializeL( TMceItcFunctions aFunction, CMceMsgSIPEvent& aMessage );
    
    void LocalDoServiceL( TMceIds& aIds, 
                     TMceItcFunctions aFunction );
    
    void LocalDoServiceL( TMceIds& aIds, 
                     TMceItcFunctions aFunction,
                     CMceMsgSIPData& aMessage );
    
    static TInt Idle( TAny* aObject );

    CDesC8Array* CopyArrayLC( const CDesC8Array* aArray );
    
    HBufC8* CopyBufferLC( const HBufC8* aBuffer );

private: // Own Data

    CMceComEvent* iEvent;
	TEventStateType iStateType;
	CIdle* iIdle;
    friend class TMceActionSet;
    
	// Definitions for unit testing
	MCESRV_UT_DEFINITIONS    
	};

#endif

// End of File
