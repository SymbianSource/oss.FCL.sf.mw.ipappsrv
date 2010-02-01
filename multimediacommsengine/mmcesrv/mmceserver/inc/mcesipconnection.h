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




#ifndef CMCESIPCONNECTION_H
#define CMCESIPCONNECTION_H

#include <e32base.h>
#include <in_sock.h>
#include <sipconnectionobserver.h>

#include "mcesrv.h"

//  FORWARD DECLARATION

// CLASS DECLARATION
class CSIPProfile;
class CSIPConnection;
class CMceCsSubSession;
class CMceSipManager;
class CMceLocalAddrResolver;
class CSIPProfileRegistry;

/**
*  MCE SIP Connection.
*
*  @lib 
*  @since
*/
class CMceSipConnection : public CBase, 
                          public MSIPConnectionObserver 
	{
	
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aSessionManager, reference to session manager
	* @param aProfile, reference to profile.
	*/
	static CMceSipConnection* NewLC ( CMceSipManager& aSessionManager,
									  CSIPProfile& aProfile,
								      CSIPProfileRegistry& aProfileRegistry );

	/**
	* Two-phased constructor.
	* @param aSessionManager, reference to session manager
	* @param aIAPId, access point id to be used.
	*/
	static CMceSipConnection* NewLC ( CMceSipManager& aSessionManager, TUint32 aIAPId );

	/**
	* Destructor.
	*/
	~CMceSipConnection();


public: //New

    /**
    * return access point id
    *
    * @return return access point id
    */
    TUint32 AccessPointId() const;

    /**
    * State of connection
    *
    * @State of connection
    */
    CSIPConnection::TState State() const;
        
    /**
    * return connection
    *
    * @return connection
    */
    CSIPConnection& Connection() const;
    
    /**
    * Numbers of sessions
    *
    * @param
    * @return number of sessions attached to this connection
    */
    TBool IsProfileUsedInSession( CSIPProfile& aProfile );

    /**
    * Attach session to this connection
    * Leaves, if SIP connection is not active or
    *         session is allready attached
    * @param aSession session
    */
    void AttachL( CMceCsSubSession& aSession );
    
    /**
    * De-Attach session to this connection
    *
    * @param aSession session
    * @return number of sessions attached to this connection
    */
    TInt Detach( CMceCsSubSession& aSession );

    /**
    * Gets local IP address of the connection
    *
    * @return local IP address
    */
	TInetAddr LocalIPAddressL(TInetAddr* aNextHop);
	
	/**
    * Gets IP address of IAP
    *
    * @return IP address
    */
	static TInetAddr IPAddressL( TUint32 aIapId );
	
	/**
	* Sets signalling type of service for connection if profile is
	* not used.
	* @param aSignallingTypeOfService
	*/
	void UpdateConnectionL( TUint aSignallingTypeOfService );
	        
public: // from MSIPConnectionObserver


    /**
    * A SIP request outside a dialog has been received from the network.
    *
    * @pre aTransaction != 0
    * @param aTransaction SIP server transaction. The ownership is
    *   transferred.
    */
    void IncomingRequest (CSIPServerTransaction*
    							  aTransaction);

    /**
    * A SIP request within a dialog has been received from the network.
    * The client must resolve the actual dialog association to which
    * this request belongs.
    *
    * @pre aTransaction != 0
    * @param aTransaction SIP server transaction. The ownership is
    *   transferred.
    * @param aDialog the dialog that this transaction belongs to.        
    */
    void IncomingRequest (CSIPServerTransaction* aTransaction,
    			                  CSIPDialog& aDialog);

    /**
    * A SIP response received from the network.
    *
    * @param aTransaction contains response elements.
    */
    void IncomingResponse (CSIPClientTransaction&
                                   aTransaction);

    /**
    * A SIP response received from the network that is within a dialog
    * association or creates a dialog association.
    *
    * @param aTransaction contains response elements.
    * @param aDialogAssoc a dialog association.        
    */		
    void IncomingResponse (
    			CSIPClientTransaction& aTransaction,
    			CSIPDialogAssocBase& aDialogAssoc);

    /**
    * Multiple SIP responses have been received to the single INVITE due
    * to the forking proxy. Note that each response creates a separate
    * INVITE dialog association.
    * Multiple responses can arrive until SIP stack completes UAC core
    * INVITE transaction.
    * If clients deletes INVITE transaction after first SIP response
    * other possible responses will be consumed by the implementation.
    * 
    * @param aTransaction contains response elements
    * @param aDialogAssoc INVITE dialog association; the ownership is 
    *        transferred
    */
    void IncomingResponse (
    			CSIPClientTransaction& aTransaction,
    			CSIPInviteDialogAssoc* aDialogAssoc);

    /**
    * A SIP response related a registration binding or an error response
    * that is related to registration binding has been received
    * from the network.
    *
    * @param aTransaction contains response elements
    * @param aRegistration registration binding this transaction belongs to
    */
    void
    	IncomingResponse (CSIPClientTransaction& aTransaction,
                          CSIPRegistrationBinding& aRegistration);

    /**
    * An asynchronous error has occurred in the stack related to the
    * request indicated by the given transaction.
    *
    * @param aError system wide or SIP error code
    * @param aTransaction failed transaction
    * @param aSIPConnection a SIP connection        
    */
    void ErrorOccured (TInt aError,
                               CSIPTransactionBase& aTransaction);

    /**
    * An asynchronous error has occurred in the stack related
    * to the request indicated by the given transaction.
    *
    * @param aError system wide or SIP error code
    * @param aTransaction the failed transaction
    * @param aRegistration the failed registration binding
    */
    void ErrorOccured (TInt aError,
    		                   CSIPClientTransaction& aTransaction,
    			               CSIPRegistrationBinding& aRegistration);

    /**
    * An asynchronous error has occured related to a request within
    * an existing dialog.
    *
    * @param aError system wide or SIP error code
    * @param aTransaction the failed transaction.
    * @param aDialogAssoc the failed dialog associoation.        
    */
    void ErrorOccured (
    			TInt aError,
    		    CSIPTransactionBase& aTransaction,
    			CSIPDialogAssocBase& aDialogAssoc);

    /**
    * An asynchronous error has occured related to a refresh 
    *
    * @param aError system wide or SIP error code
    * @param aSIPRefresh original refresh object.        
    */
    void ErrorOccured (TInt aError, CSIPRefresh& aSIPRefresh);

    /**
    * An asynchronous error has occured related to a periodical refresh
    * that relates to a registration.
    *
    * @param aError system wide or SIP error code; 
    *		 KErrCouldNotConnect if the refresh has failed
    *		 due to the suspended connection.
    * @param aRegistration associated registration binding
    */
    void ErrorOccured (TInt aError,					
    			               CSIPRegistrationBinding& aRegistration);

    /**
    * An asynchronous error has occured related to a periodical refresh
    * that belongs to SIP dialog association.
    *
    * @param aError system wide or SIP error code; 
    *        KErrCouldNotConnect if the refresh has failed
    *		 due to the suspended connection.
    * @param aDialogAssoc SIP dialog association.        
    */
    void ErrorOccured (TInt aError,			
    			               CSIPDialogAssocBase& aDialogAssoc);

    /**
    * SIP stack has completed UAC core INVITE transaction 64*T1 seconds
    * after the reception of the first 2xx response. No more 2xx responses
    * can be received to the issued single INVITE.
    *
    * If the INVITE transaction does not create a dialog, or the INVITE
    * transaction encounters an error, this event will not be sent.
    *
    * @param aTransaction a complete UAC core INVITE transaction
    */
    void InviteCompleted (CSIPClientTransaction& aTransaction);

    /**
    * Invite was canceled with the CANCEL
    * @param aTransaction a canceled INVITE UAS transaction
    */
    void InviteCanceled (CSIPServerTransaction& aTransaction);
            
    /**
    * Connection state has changed.
    * If connection state has changed to EInactive or EUnavailable,
    * SIP stack has removed all stand-alone SIP refreshes, registrations 
    * and dialog associations that client requested to refresh. Client may
    * re-issue refresh requests (stand-alone, registration or dialog 
    * association related) when connection becomes EActive again.
    * SIP stack also terminates all pending SIP client transactions and no
    * errors are reported back to the client about the terminated
    * transactions nor about removed refreshes in order to avoid event
    * flood.
    * 
    * @param aState indicates the current connection state        
    */
    void ConnectionStateChanged (CSIPConnection::TState aState);

private:

	CMceSipConnection ( CMceSipManager& aSessionManager, 
                        CSIPProfile& aProfile );

	CMceSipConnection ( CMceSipManager& aSessionManager,
	                    TUint32 aIAPId );
	void ConstructL ( CSIPProfile& aProfile, CSIPProfileRegistry& aProfileRegistry );
	
	void ConstructL ( );

    void Reject ( CSIPInviteDialogAssoc* aDialogAssoc );
    
    void FillWithMatchingAddrFamily( TInetAddr& aAddr,
                                     const TInetAddr* aNextHop );
   
private: // Data

    TUint32 iIAPId;
    
    CSIPConnection* iClientConnection;
    
    CMceLocalAddrResolver* iLocalAddrResolver;
    
    TUint iCurrentSignallingTypeOfService;
    
private: //NOT owned data


    RPointerArray<CMceCsSubSession> iSessions;
    
    CMceSipManager& iSessionManager;
    
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS
    

	};

#endif

// End of File
