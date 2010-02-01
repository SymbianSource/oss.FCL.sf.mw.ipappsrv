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




#ifndef CMCESIPMANAGER_H
#define CMCESIPMANAGER_H

#include "mcesrv.h"
#include "mceclientserver.h"
#include "mcemediaobserver.h"
#include "mceclientresolver.h"
#include <es_sock.h>
#include <sipobserver.h>
#include <sipprofileregistryobserver.h>
#include <e32base.h>

//  FORWARD DECLARATION
class CSIP;
class CSIPProfile;
class CSIPHttpDigest;
class CMceServerCore;
class CMceSipConnection;
class CMceCsSubSession;
class CMceCsSessionImplementation;
class CMceCsSession;
class CMceMediaManager;
class TMceIds;
class CSIPProfileRegistry;
class CSIPServerTransaction;
class CSIPClientTransaction;
class CSIPInviteDialogAssoc;
class CSIPNotifyDialogAssoc;
class CMceSipStateMachine;
class CMceTimerManager;

/**
* This class provides life-cycle management of 
* SIP sessions and SIP events. It also acts as an
* observer for SIP API, SIP Client resolver and
* Media Mananger.
*
*  @lib 
*  @since
*/
class CMceSipManager : public CBase,
                       public MSIPObserver,
                       public MSIPProfileRegistryObserver,
                       public MMceMediaGeneralObserver,
                       public MMceClientResolverObserver
	{	
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aServerCore, reference to server core.
	*/
	static CMceSipManager* NewL( CMceServerCore& aServerCore );

	/**
	* Two-phased constructor.
	* @param aServerCore, reference to server core.
	*/
	static CMceSipManager* NewLC( CMceServerCore& aServerCore );

	/**
	* Destructor.
	*/
	~CMceSipManager();

public: // from MSIPObserver

    /**
    * A SIP request has been received from the network.        
    * @pre aTransaction != 0
    * @param aIapId The IapId from which the SIP request was received.
    * @param aTransaction contains local address, remote address of a SIP
    *        message, as well as optional SIP message method, headers and
    *        body. The ownership is transferred.
    */
	void IncomingRequest(TUint32 aIapId,
		                 CSIPServerTransaction* aTransaction);

    /**
    * The received SIP request time-outed and it is invalid i.e. cannot be used
    * anymore.
    * This will be called if the user fails to create a SIP connection
    * and does not send an appropriate SIP response.
    * @param aTransaction The time-outed transaction.
    */
    void TimedOut(CSIPServerTransaction& aTransaction);


public: // from MMceClientResolverObserver


    /**
    *  Called when client discovery subsystem has resolved
    *  the client the transaction should go to
    *
    *  @param aClientUid Client's uid
    *  @param aConnection the connection used
    *  @param aRequest the transaction
    */
    void IncomingResolvedRequest( TUid aClientUid,
                                  CMceSipConnection& aConnection,
                                  CSIPServerTransaction* aRequest );


public: //from MSIPProfileRegistryObserver 
	
	/** 
	* An event related to SIP Profile has accorred
	* @param aProfileId a profile Id
	* @param aEvent an occurred event
	**/
	void ProfileRegistryEventOccurred(
	            TUint32 aProfileId, 
	            MSIPProfileRegistryObserver::TEvent aEvent);

	/**
	* An asynchronous error has occurred related to SIP profile
	* Event is send to those observers, who have the
	* corresponding profile instantiated.
	* @param aProfileId the id of failed profile 
	* @param aError an occurred error
	*/
	void ProfileRegistryErrorOccurred(TUint32 aProfileId, TInt aError);

public: // from MMceMediaGeneralObserver

	/**
    * Callback function to indicate about media error
    * @param aError, error occured	 
    */
	void MediaError ( TInt aError );

    
public: // New

	/**
	* Reserves new dialog id
	* @return new dialog id
	*/
    TUint32 NextDialogId();
    
	/**
	* Returns ETrue, if manager will consume the transaction
	* @param aTransaction the pending transaction
	* @return ETrue, if manager will consume the transaction
	*/
    TBool Consumes( CSIPServerTransaction& aTransaction );
    
	/**
	* Registers a client
	* @param aClientSession the client
	*/
    void RegisterClientL( CMceCsSession& aClientSession );

	/**
	* Un-registers client
	* @param aClientSession the client
	*/
    void UnRegisterClient( CMceCsSession& aClientSession );
    
	/**
	* Creates outgoing (sub) session
	* @param aClientSession the client
    * @param aIds set of ids defining ITC call context
	* @param aParams parameter for dialog
	* @return sub session 
	*/
    CMceCsSubSession* CreateSubSessionL( CMceCsSession& aClientSession, 
                                         TMceIds& aIds,
                                         CDesC8Array& aParams );
    
	/**
	* Creates incoming (sub) session
	* @param aTransaction incoming request, which trigged creation
    * @param aConnection SIP connection to which session will be attached
	* @return sub session 
	*/
    void CreateSubSessionL( CSIPServerTransaction* aTransaction,
                            CMceSipConnection& aConnection );
    
	/**
	* Unregisters sub session
	* @param aSubSession sub session
	*/
    void UnRegisterSubSession( CMceCsSubSession& aSubSession );

    /**
	* Returns media mananger instance
	* @return media mananger
	*/
    CMceMediaManager& MediaManager();
    
	/**
	* Returns timer mananger instance
	* @return timer mananger
	*/
    CMceTimerManager& TimerManager();
    
	/**
	* Returns SIP client instance
	* @return SIP client
	*/
    CSIP& SIPClient();

	/**
	* Adds orphan sub session
	* @param aSubSession sub session
	*/
    void AddOrphanSubSessionL( CMceCsSubSession* aSubSession );
    
    /*
    * get Orphan Session's Count
    * @return number of orphan subsessions.
    */  
    TInt OrphanSubSessionCount();
    
	/**
	* Cleans up orphans sub sessions
	*/
    void Cleanup();
    
    /**
	* Creates incoming sub session
	* @param aTransaction incoming request, which trigged creation
	* @param aClient resolved client
	* @param aConnection IP connection to which session will be attached
	*/
    void CreateSubSessionL( CSIPServerTransaction* aTransaction,
                            CMceCsSession& aClient,
                            CMceSipConnection& aConnection,
                            CMceCsSubSession* aAssocSession = NULL );
	
	RSocketServ& SocketServ();
	
private:

	CMceSipManager (CMceServerCore& aServerCore);
	void ConstructL ();
	CSIPProfile& ProfileL( TUint32 aProfileId );
    CSIPProfile& ProfileL( TUint32 aIapId, 
                           const CUri8& aRequestUri );
    CMceSipConnection& ConnectionL( CSIPProfile& aProfile );
	CMceSipConnection& ConnectionL( TUint32 aIAPId );    
    TInt FindProfileById( RPointerArray<CSIPProfile>& aProfiles, TUint32 profileId );
    TInt FindProfileByRequestUriL( RPointerArray<CSIPProfile>& aProfiles, 
                                   const CUri8& aRequestUri );
                                   
    TUint32 ChannelL( CSIPServerTransaction& aRequest );

	/**
    * When a profile has been updated and its data has been read from Profile
    * server to aNewProfile, update the sub sessions that point to the old
    * profile instance, so that they will point to the new instance.
    * @param aNewProfile Profile's current data
    * @param aOldProfile Profile's data before it was updated
    */
    void UpdateProfileToSubSessions( CSIPProfile& aNewProfile,
									 CSIPProfile& aOldProfile );

private: // NOT owned Data

    //MCE server core
	CMceServerCore& iServerCore;
    //all clients registered to MCE server
    RPointerArray<CMceCsSession> iClients;
    
private: // Owned Data

    //SIP connections
	RPointerArray<CMceSipConnection> iSIPClientConnections;
    //SIP instance
	CSIP* iSIP;
    //media manager
	CMceMediaManager* iMediaManager;
    //profile registry
	CSIPProfileRegistry* iProfileRegistry;
    //SIP session FSM
	CMceSipStateMachine* iStateMachine;
    //profiles in use
	RPointerArray<CSIPProfile> iProfiles;
	//next dialog id
	TUint32 iNextDialogId;
	//client resolver
	CMceClientResolver* iClientResolver;
	//orphan sub sessions
	CMceCsSessionImplementation* iOrphanSubSessions;
    //timer manager
    CMceTimerManager* iTimerManager;
    // Socket server handle    
    RSocketServ iSocketServ;

	//definitions for unit testing
	MCESRV_UT_DEFINITIONS
	};

#endif // CMCESIPMANAGER_H

// End of File
