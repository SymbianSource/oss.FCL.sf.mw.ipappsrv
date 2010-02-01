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




#ifndef CMceSipEVENT_H
#define CMceSipEVENT_H

#include <e32base.h>
#include <badesca.h>

#include "ut_cmcecomevent.h"
#include "sipdialogassocbase.h"
#include "sipconnection.h"
#include "mmcetesteventobserver.h"
#include "mcerefer.h"
#include "mceevent.h"
#include "MCEEventTestConsts.h"

//  FORWARD DECLARATION
class UT_CMceComEvent;
class CSIPDialogAssocBase;
class CMceComEvent;
class CMceSipConnection;


// CLASS DECLARATION

enum TEventStateType 
    {
	EMoSubscribe=1,  
	EMtSubscribe,
	EMoRefer,
	EMtRefer,  
	};  

const TInt KDefaultSubscrExpire = 3600;

_LIT8(KStateActive, "active");
_LIT8(KStateTerminated, "terminated");

// CMceSipEvent stub
/**
*  MCE SIP session.
*
*  @lib 
*  @since
*/
class CMceSipEvent : public CBase
	{
	
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aConnection, connection stub
	* @param aObserver, test observer 
	*/
	static CMceSipEvent* NewL ( CMceSipConnection& aConnection,
	                            MMCETestEventObserver& aObserver );

	/**
	* Two-phased constructor.
	* @param aConnection, connection stub
	* @param aObserver, test observer 
	*/
	static CMceSipEvent* NewLC ( CMceSipConnection& aConnection, 
	                             MMCETestEventObserver& aObserver );
	
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

    CMceSipConnection& SIPConnection() const;
	/**
	* Provides services of MCE server subsession
	* @param aIds, ids from the client.
	* @param aFunction, function from the client.
	* @param aMessage, message from the client.
	* @param aContent, content from the client.
	*/
    void ServiceL();
			 

	/**
	* Returns ETrue, if session is pending, and thus
	* cannot be deleted
	*@return ETrue, if session is pending
	*/
    TBool IsPending();
			 
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
    void ErrorOccured( TInt aError, CSIPTransactionBase& aTransaction );
     
    
    /**
    * Called when connection state has been changed
    * @param aState new state
    */
    void ConnectionStateChanged( CSIPConnection::TState aState );

    /**
    * Called when dialog has been cancelled
    */
    void Canceled();
    
    
    TEventStateType EventStateType() const;
    
    //For Unit testing
    CSIPDialogAssocBase* Dialog() const;
    CSIPServerTransaction& Request();
    CSIPClientTransaction& Response();
    void SetDialog( CSIPDialogAssocBase* aDialog );
    
    const RPointerArray<CSIPClientTransaction>& PendingTransactions();
    void SetPendingTransactionL( CSIPClientTransaction* aTransaction );
    CDesC8Array& DefaultHeaders( TInt aMethodInd );

    void IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody);
									
    void IncomingReferL(
					HBufC8* aReferTo,
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody);
	
	void ClientStateChangedL( TUint32 aState, TBool aIsResponse );
		
	void NotifyReceivedL(CDesC8Array* aHeaders,
                    				HBufC8* aContentType,
									HBufC8* aBody) ;
		

public: // Testing utility
    void SetCurrentState( TInt aIndex );

protected: //CMceCsSubSession

    /**
    * Called when request has been received. 
    * Request can be obtained by calling Request() method
    * @return TConsumeStatus
    */
    void RequestReceived();
    
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
    void StandAloneResponseReceived( );
    
    
private:

	CMceSipEvent ( CMceSipConnection& aConnection,
	               MMCETestEventObserver& aClientSession );

	void ConstructL ();
	
    void HandleSIPEvent( );
    
    void SetEventStateType(const TEventStateType aEventStateType );
   
    void SetResponseL( CSIPClientTransaction& aTransaction );

private: // Own Data
    CMceSipConnection& iConnection;
    MMCETestEventObserver& iObserver;
    CMceComEvent* iEvent;
	TEventStateType iStateType;
    CSIPDialogAssocBase* iDialog;
    CSIPServerTransaction* iRequest;
    RPointerArray<CSIPClientTransaction> iPendingTransactions;
    CSIPClientTransaction* iResponse;
    CDesC8Array* iDefaultHeaders;
    
    MCE_EVENT_UNITTEST_DEFS
    };
    
#endif

// End of File
