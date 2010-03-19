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




#ifndef CMCESIPSESSION_H
#define CMCESIPSESSION_H

// INCLUDES
#include <e32base.h>

#include "mcesrv.h"
#include "mcecssubsession.h"
#include "mcesipmanager.h"
#include "mcestate.h"
#include "fcobserver.h"
#include "mceexpirationhandler.h"
#include "mcereliableobserver.h"

// FORWARD DECLARATION
class CSIPProfile;
class CMceSipStateMachine;
class CMceComSession;
class TMceActionSet;
class TMceFCActionSet;
class CMceSipExtensions;
class CDesC8Array;
class CMceReliableSender;

// CLASS DECLARATION


/**
* This class represents SIP session
* 
* Class provides methods to handle SIP session
* specifig ITC functions and to receive SIP requests and 
* responses.

*  @lib 
*  @since
*/
class CMceSipSession : public CMceCsSubSession,
					   public MMceMediaSessionObserver,
                       public MMCEExpirationHandler,
                       public MFCObserver,
                       public MMceReliableObserver
	{
	
public: //enums

    enum TSubState
        {
        ENone,
        EOffering,
        EAnswering,
        EUpdating,
        ERefreshing,
        };

public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aClientSession client
	* @param aSIPConnection SIP connection 
	* @param aStateMachine SIP session state machine
	* @param aProfile SIP Profile
	* @return SIP session
	*/
	static CMceSipSession* NewL( CMceCsSession& aClientSession, 
	                             CMceSipConnection& aSIPConnection,
								 CMceSipStateMachine& aStateMachine, 
								 CSIPProfile& aProfile );

	/**
	* Two-phased constructor.
	* @param aClientSession client
	* @param aSIPConnection SIP connection 
	* @param aStateMachine SIP session state machine
	* @param aProfile SIP Profile
	* @return SIP session
	*/
	static CMceSipSession* NewLC( CMceCsSession& aClientSession, 
	                              CMceSipConnection& aSIPConnection,
								  CMceSipStateMachine& aStateMachine, 
								  CSIPProfile& aProfile );

	/**
	* Two-phased constructor.
	* @param aClientSession client
	* @param aSIPConnection SIP connection 
	* @param aStateMachine SIP session state machine
	* @return SIP session
	*/
	static CMceSipSession* NewLC( CMceCsSession& aClientSession, 
	                              CMceSipConnection& aSIPConnection,
								  CMceSipStateMachine& aStateMachine ); 
	
	/**
	* Destructor.
	*/
	~CMceSipSession();

public: // New

	/**
	* Returns actions set
	* @return action set
	*/
    TMceActionSet Actions();
    
    /**
	* Returns floor control actions set
	* @return floor control action set
	*/
    TMceFCActionSet FCActions();
    
	/**
	* Sets substate of session
	* @param aSubState substate of session
	*/
    void SetSubState( TSubState aSubState );
    
	/**
	* Returns substate of session
	* @return substate of session
	*/
    TSubState SubState();
 
    /**
    * Returns Reliable Sender 
    * @return Reliable Sender
    */
	CMceReliableSender& ReliableSender();
		
	/**
    * Re-Create an outgoing dialog 
    */
	void ReCreateOutgoingDialogL();
	
	/**
	* Create outgoing dialog from 3xx response
	*/ 
	void CreateDialogFrom3XXResponseL();
	
	/** 
	* Re-Use dialog
	* @return KErrNone if it passes
	*/
	
	TInt ReUseDialog();
	
	/** 
	* Returns Status for ForcedSDPSending Flag. 
	* @return ETrue when resource reservation ongoing otherwise EFalse
	*/
	
	TBool ForceSDPSendingStatus();
	
	/** 
	* Returns the previous state of sip session
	* @return StateInded
	*/
	
	TMceStateIndex PreviousState();	
   
public: // from CMceCsSubSession

	/**
	* Executes ITC function called by client.
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @return aReturnMessage buffer to store message context,
	* which will be wrote back to client
	*/
    HBufC8* DoServiceL( TMceIds& aIds, 
                        TMceItcFunctions aFunction );

	/**
	* Executes ITC function called by client.
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @param aMessage message context from client
	*/
    void DoServiceL( TMceIds& aIds, 
                     TMceItcFunctions aFunction, 
     	             const TDesC8& aMessage );

	/**
	* Executes ITC function called by client.
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @param aMessage message context from client
	*/
    void DoServiceL( TMceIds& aIds, 
                     TMceItcFunctions aFunction, 
     	             TPtr8& aMessage );


	/**
	* Executes ITC function called by client.
	* @param aIds set of ids defining ITC call context
	* @param aFunction ITC function to be executed
	* @param aMessage decoded message context from client
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
	* Initializes incoming dialog
	* @param aTransaction server transaction representing a request,
	* which caused dialog creation
	*/
    void DoInitializeIncomingDialogL( CSIPServerTransaction& aTransaction );
			 
    /**
    * Called when error has been occured within dialog
    * @param aError error code
    */
    void ErrorOccured( TInt aError );
    
    /**
	* Handles situation where an asynchronous error has occured 
	* related to a request within an existing dialog.
    *
    * @param aError error code
    * @param aTransaction the transaction
    */
    void DoErrorOccured( TInt aError, CSIPTransactionBase& aTransaction );
     
    
    /**
	* Handles situation where SIP connection state has changed.
    * If connection state has changed to EInactive or EUnavailable,
	* SIP stack terminates all pending SIP client transactions and no
    * errors are reported back to the client about the terminated
    * transactions.
	* @param aIsActive indicates if the current connection is active
    */
    void DoConnectionStateChanged( TBool aIsActive );

    /**
    * Handles situation where incoming INVITE was 
    * canceled with the CANCEL
    */
    void Canceled();
    
    /**
    * Creates new incoming dialog (association)
    * @param aInitialRequest request causing creation
    * @return created dialog association
    */
    CSIPDialogAssocBase* CreateIncomingDialogL( 
                                    CSIPServerTransaction& aInitialRequest );

    /**
    * Creates outgoing dialog (association) based on given parameters 
    * @param aDialogType type of new dialog
    * @param aParams parameters for creation
    * @return created dialog association
    */
    CSIPDialogAssocBase* CreateOutgoingDialogL( TMceDialogType aDialogType, 
                                                CDesC8Array& aParams,
                                                CSIPDialog& aExistingDialog );

    /**
    * Creates outgoing dialog (association) based on given parameters 
    * @param aDialogType type of new dialog
    * @param aParams parameters for creation
    * @return created dialog association
    */
    CSIPDialogAssocBase* CreateOutgoingDialogL( TMceDialogType aDialogType,
                                                CDesC8Array& aParams );
       

public: //CMceCsSubSession

	/**
	* Returns SIP dialog (assoc)
	* @return SIP dialog or NULL if it does not exist
	*/
    CSIPDialogAssocBase* Dialog() const;

    /**
    * Handles situation where SIP stack has completed UAC core INVITE 
    * transaction 64*T1 seconds after the reception of the first 
    * 2xx response. No more 2xx responses can be received to the issued 
    * single INVITE.
    * @param aTransaction client transaction representing a response
    */
    void InviteCompleted( CSIPClientTransaction& aTransaction );
    
   	/**
	* Returns ETrue, if session will consume the transaction
	* @param aDialog SIP dialog
	* @param aTransaction the pending transaction
	* @return ETrue, if session will consume the transaction
	*/

    TBool Consumes( CSIPDialog& aDialog, CSIPServerTransaction& aTransaction, 
                	 TBool& aAssociatesWithDialog );
                	 

protected: //CMceCsSubSession


    /**
    * Checks if session accepts method of incoming request
    * @param aMethod the method
    * @return ETrue, if session will accept
    */
    TBool AcceptMethod( RStringF aMethod );

    /**
    * Called when request has been received. 
    * Request can be obtained by calling Request() method
    * @param aInsideDialog tells whether request was received inside dialog
    * @param aDialog SIP Dialog to which request belongs
    */
    void RequestReceived( TBool aInsideDialog, CSIPDialog& aDialog  );
    
        
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
    * Called when error response (4XX-7XX) has been received 
    * to pending transaction. 
    * Response can be obtained by calling Response() method
    */
    void ErrorResponseReceived();
    
    /**
    * Called when stand-alone request has been received.
    * Request can be obtained by calling Request() method
    */
    void StandAloneRequestReceived();
    
    /**
    * Called when stand-alone response (any) has been received 
    * to pending transaction. 
    * Response can be obtained by calling Response() method
    * @param aResponseType response type
    */
    void StandAloneResponseReceived( TMceSipResponseType aResponseType );

    /**
    * Handles situation where session must be forcefully terminated
    * @return ETrue, if session can be deleted. If there are 
    * pending operations, which cannot be done within this call, EFalse
    * is returned.
    */
    TBool Terminate();
    
    
public: // from MMceMediaSessionObserver

    /**
    * Callback function to indicate MCC session
    * is updated.
    * @param aSession session body
    */
    virtual void Updated( CMceComSession& aSession );

    /**
    * Callback function to indicate that network 
    * resource reservation is now ready
    */
    void Reserved( CMceComSession& aSession );

    /**
    * Callback function to indicate that event has been received
    * @param aEvent, received event
    */
    void EventReceived( TMceMccComEvent& aEvent );

	/**
    * Callback function to indicate MCC session was stopped
    * @param aSession session body
    */
	void SessionClosed( CMceComSession& aSession );
	
    /**
    * Callback function to indicate about error
    * @param aEvent, received event
    */
    void MediaError( TMceMccComEvent& aEvent );
    
    /**
    * Callback function to indicate about new media session due fork
    * @param aSession, new session, onwership is been transfered
    */
    void SessionCreatedL ( CMceComSession* aSession );

	
public: // From MMCEExpirationHandler	

	/**
    * Callback function to indicate about expiration of
    * session timer. RFC 4028
    * @param aTimerId timer Id
    * @param aTimerParam timer param
    */
    void TimerExpiredL( TMceTimerId aTimerId, TAny* aTimerParam );


public: // From MFCObserver
	    
    /**
	* A FC message has been received from the network.        
    * @param aData The Data received from FCsession specific plugIn 
    *        The ownership is transferred.
	* @param aFCSession The Session which data is received to.
    */
	void FCReceivedData( HBufC8* aData, const CFCSession* aFCSession );

	/**
	* An asynchronous error has occurred in the FC related to the
	* given session indicated.
	* @param aError KErrNone succesfully, otherwise system wide or Socket 
	*               error code
	* @param aFCSession The Session which error is received to.
	*/
	void FCErrorNotify( TInt aErrCode, const CFCSession* aFCSession );



public: // From MCEReliableObserver
    
    /**
    * Callback Function, this will be called when reliable sender, retransmission 
    * timer expires, i.e. reliable sender has sent transaction for 64T1 and has not 
    * got the corresponding PRACK.
    * 
    */
    void NoPrackReceived( CSIPServerTransaction& aTransaction );
    
    
    /**
    * Callback Function indicates reliable sender has failed. This may be called 
    * when a transaction is in progress and UA tries to send another reliable response.
    */
    void ReliableFailed( CSIPServerTransaction& aTransaction, TInt aError );
    
    
public: //New Functions


	
	/**
    * Returns pending invite transaction
    * @return pending invite transaction
    */
    CSIPClientTransaction* InviteTransaction();
    
	/**
    * Returns current state
    * @param aMarkExit indicates, if current state is marked as exit state
    * @return current state
    */
    CMceState& CurrentState( TBool aMarkExit = EFalse );

	/**
    * Create body
    * @param aSession
    */
    void CMceSipSession::SetBodyL( CMceComSession* aSession = NULL );
    
	/**
    * Returns session body
    * @return session body
    */
    CMceComSession* Body();
    
	/**
    * Returns session body candidate
    * @return session body candidate
    */
    CMceComSession* BodyCandidate();

	/**
    * Returns active session body 
    * @return active session body 
    */
    CMceComSession& ActiveBody();
    
    /**
    * Sets the latest offer
    * @param aSdpDocument the latest offer
    */
    virtual void SetOffer( CSdpDocument* aSdpDocument );
    
	/**
    * Returns last offer (or answer) issued to session
    * @return last offer (or answer) issued to session
    */
    virtual CSdpDocument* Offer();

	/**
    * Sets the previous offer
    * @param aSdpDocument the latest offer
    */
    virtual void SetPrevOffer( CSdpDocument* aSdpDocument );
    
    /**
    * Returns one before last offer (or answer) issued to session
    * @return one before last offer (or answer) issued to session
    */
    virtual CSdpDocument* PrevOffer();
    
    /**
    * Sets the latest offer
    * @param aSdpDocument the latest offer
    */
    virtual void StorePrevOffer();
    
    /**
    * Method used to check if sip session is a Nat sip sesssion.
    * @return ETrue is Sip session is a Nat sip session, EFalse otherwise.
    */
    virtual TBool NatSession() const;
    
	/**
    * Returns Nat sip session sub state.
    * @return Enumeration depicting session's Nat substate.
    */
    virtual TInt NatState() const;
    
    /**
    * Check if the NAT has been disabled.
    * @return ETrue NAT is disabled, EFalse otherwise
    */
    virtual TBool IsNatDisabled() const;
    
    /**
    * Updates media
    * @return return status:
    *       -System error < 0: error situation
    *       -KMceReady: update was synchronous
    *       -KMceAsync: updating asynchronously
    */
    virtual TMceReturnStatus UpdateMediaL();
    
	/**
    * Resource Reservation Status 
    * @return return status TMceReturnStatus
    *       -KMceReady: resrouces reserved
    *       -KMceAsync: resource reservation asynchronously
    */
    
	virtual TMceReturnStatus ReserveL();
	
    /**
    * Tells whether SDP to be sent to network.
    * @param aSdpDocument, un-decoded received offer, 
    *        to check if it is session refresh.
    * @return ETrue, if SDP needs to be sent
    */    
    TBool NeedToSend( CSdpDocument* aSdpDocument );
    
    /**
    * Returns whether a SDP is expected to be received
    * @return ETrue, if expected to receive SDP
    */    
    TBool NeedToReceive();
    
	/**
    * Sets next state
    * @param aNextStateId next state id
    */
    void NextState( TUint aNextStateId );
    
    /**
    * Starts the session timer
    */
    void StartSessionTimerL();
    
    /**
    * Starts the pending timer
    * @param aInterval interval in milli seconds
    */
    void StartPendingTimerL( TUint aInterval );
    
    /**
    * Causes pending update to be accepted
    */
    void UpdateAccepted();
    
    /**
    * Causes pending update to be rejected
    */
    void UpdateRejected();
    
    /**
    * Prepares ITC callback context
    * @param aIds set of ids defining ITC callback context
    */
    void PrepareIdsL( TMceIds& aIds ) const;

    /**
    * Returns truth value, is FC in use or not.
    * Set to ETrue, when initializing FC for MO sessions and
    * when updating FC for MT sessions.
    * @return ETrue, if FC is in use.
    */
    TBool UseFC();
    
    /**
    * Invite has been received multiple replies and thus stack
    * has creted new dialog for others 
    * @param aTransaction The original invite transaction.
    * @param aDialogAssoc new dialog
    */
    void InviteForkedL( CSIPClientTransaction& aTransaction,
          		        CSIPInviteDialogAssoc* aDialogAssoc );
    
    /**
    * Returns as a boolean value whether the session is
    * waiting for a media callback
    * @return ETrue, if waiting for Media Manager callback
    */
    TBool WaitingMediaCallback() const;
    
    /**
    * Sets the boolean value whether the session is
    * waiting for a media callback
    * @param aWaiting the value to be set
    */
    void SetWaitingMediaCallback( TBool aWaiting );
    
    /**
    * Returns the media authorization token. Ownership is not transfered.
    */
    HBufC8* MediaAuthorizationToken() const;
    
    /**
    * Sets the media authorization token. Ownership is transfered.
    * aToken the media authorization token
    */
    void SetMediaAuthorizationToken( HBufC8* aToken );
    
    /**
    * Returns the SIP extensions.
    * @return the extensions
    */
    CMceSipExtensions& Extensions();
       
    /**
    * Sets the RSeq Value.
    * aRSeq RSeq Value
    */
    void SetRSeq( TUint aRSeq );
    
    /**
    * Gets the RSeq Value
    * @return int RSeq Value.
    */
    TInt RSeq();
    
    /**
    * Discards invite
    */
    void DiscardInvite();

    void CloseMedia( TBool aDelete = EFalse );
    void CloseBodyBucket();
    
    /**
    * Add the URIs in the Contact headers of aResponseElements
    */
    void AddRemoteUriFromResponseL( CSIPResponseElements& aResponseElements );

    /**
    * Change currently used sdp 
    */
    void SdpCleanup( CSdpDocument* aPrevious, CSdpDocument* aReplacement );

    void ServerInitializedL();

    void ContinueHandlingIncomingSessionL();
    
    void IncrementSentSdpCount();
    
    void IncrementReceivedSdpCount();
    
    void ResetSdpCounts();

    TBool DoesMediaManagerNeedToNegotiate( CSdpDocument* aSdpDocument );
    
    TBool IsNatSession();

    TInt ForkedDialogsCount();
	
    void ForceUpdateStreamL();
    void ResetCurrentDialog();
    void SetFirstOffer();
protected:


	CMceSipSession ( CMceCsSession& aClientSession, 
	                 CMceSipConnection& aSIPConnection,
					 CMceSipStateMachine& aStateMachine,
					 CSIPProfile& aProfile );
					 
	CMceSipSession ( CMceCsSession& aClientSession, 
	                 CMceSipConnection& aSIPConnection,
					 CMceSipStateMachine& aStateMachine );
					 
	void ConstructL( CMceCsSession& aClientSession );
    
    virtual void HandleSIPEvent( TMceSipEventCode aEventCode, CSIPDialog& aDialog );
    
    void HandleSIPEventErrorL( TMceSipEventCode aEventCode, TInt aError );
    
    void PrevOfferSafeDelete();
    
    
private:


    TMceSipEventCode MethodCode( RStringF method );
    
    void LocalErrorOccured( TMceStateIndex aNewState, TInt aWithError );
    
    void HandleForkedDialogs();
        
	CSIPInviteDialogAssoc* DoCreateOutgoingDialogL( CUri8* remoteURI, 
                                                  CSIPToHeader* to=0 );
    void DoTerminateL();
    
    void DoAutoEventL( TMceStateTransitionEvent& aEvent );
    
    /*
    * Add remote uri from invite request
    */
    void AddRemoteUriL( const CUri8& aRemoteUri );
    
    /*
    * return KErrNotFound if not found, otherwise the index of
    * the found uri in remoteUri Array.
    */
    TInt FindRemoteUri( const CUri8& aRemoteUri );
    
    void ClearRemoteUri( const TMceStateIndex aState );
    
    /**
    * Returns dialog confirmation status
    * @return ETrue if dialog is confirmed, EFalse otherwise
    */
    TBool IsSipDialogConfirmed() const;

protected: // Own Data

    //SDP offer/answer
    CSdpDocument* iOffer;
    CSdpDocument* iFirstOffer;
    
    // Force SDP Sending Flag used for Resrouce Reservation
    TBool iForceSDPSending;
            
    // Previous Offer/Answer document, own.
    CSdpDocument* iPrevOffer;
    
    //Flag indicating session type
    TBool iNatSession;
              
private: // NOT own Data

    //SIP session FSM
    CMceSipStateMachine* iStateMachine;
    CMceReliableSender * iReliableSender;

private: // Own Data

    //session body
    CMceComSession* iBody;
    //session body candidate
    CMceComSession* iNewBodyCandidate;
    //current state of session
    CMceState* iCurrentState;
    //state from which FSM has exited
    CMceState* iExitState;
    //previous state
    TUint iPreviousState;
    //session timer
    TMceTimerId iTimer;
    //sub-state of session
    TSubState iSubState;
    //is FC in use or not
    TBool iUseFC;
    // pending timer
    TMceTimerId iPendingTimer;
    // forked dialogs
    RPointerArray<CSIPInviteDialogAssoc> iForkedDialogs;
    // is the session waiting for a Media Manager callback
    TBool iWaitingMediaCallback;
    // media authorization token
    HBufC8* iMediaAuthorizationToken;
    // SIP extensions
    CMceSipExtensions* iExtensions;
    // RSeq Value from the Response
    TUint iRSeq; 
    
    RPointerArray<CUri8> iRemoteUri;
    RPointerArray<CMceComSession> iBodyBucket;
    
    TInt iSentSdpCount;
    TInt iReceivedSdpCount;
    
    friend class TMceActionSet;
    friend class TMceFCActionSet;
    
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS        
	};

#endif

// End of File
