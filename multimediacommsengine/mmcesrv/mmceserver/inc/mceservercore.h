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




#ifndef CMCESERVERCORE_H
#define CMCESERVERCORE_H

//  INCLUDES
#include <e32base.h>

#include "mcecssubsession.h"
#include "fcsession.h"
#include "fcobserver.h"

// FORWARD DECLARATION
class CMceCsServer;
class CMceCsServerCloseTimer;
class CMceSipManager;
class CFCPlugInEngine;

class TMceFCSessionItem
    {
	public: 
		inline TMceFCSessionItem ( MFCObserver& aFCObserver, 
		                           const CFCSession& aFCSession):
	                               iFCObserver(aFCObserver), 
	                               iFCSession(aFCSession){}
	
		MFCObserver& iFCObserver;
		
		const CFCSession& iFCSession;
    };

// CLASS DECLARATION

/**
*  MCE CS server core.
*
*  @lib 
*  @since
*/
class CMceServerCore : public CBase, public MFCObserver
	{
	public: // Constructors and destructor
    
		/**
		* Two-phased constructor.
		*/         
		static CMceServerCore* NewL();

		/**
		* Two-phased constructor.
		*/         
		static CMceServerCore* NewLC();

		/**
		* Destructor.
		*/         
		~CMceServerCore ();

	public: // New functions

		/**
		* Increments the count of sessions.
		*/         
		void IncrementSessions();

		/**
		* Decrements the count of sessions.
		*/         
		void DecrementSessions();

        /**
        * when there is no orphan sessions and iSessionCount is 0
        * stop mce server after time out.
        */
        void InactiveServerCleanup();
        
		/**
		* Gets reference to server.
		*/         
		CServer2& Server() const;
		
		/**
		* Gets reference to manager.
		*/         
		CMceSipManager& Manager() const;
		
		/**
		* Gets reference to FC plugin engine.
		*/         
		CFCPlugInEngine& FCPluginEngine() const;		
		
	public: // New functions
	
		/**
		* Registers subsession as an owner of floor control session.
		* @param aFCObserver floor control observer to be registered.        
        * @param aFCSession floor control session to be registered.
        */
    	void RegisterFCSessionL( MFCObserver& aFCObserver, 
                                 const CFCSession& aFCSession );

		/**
		* Deregisters subsession as an owner of floor control session.        
        * @param aFCObserver floor control observer to be registered.        
        * @param aFCSession floor control session to be unregistered.
        */
    	void DeregisterFCSession( MFCObserver& aFCObserver, 
                                  const CFCSession& aFCSession );

    	
	public: // From MFCObserver
	    
	    /**
		* A message from specific FC session plugin has been received from the network.        
        * @param aData The Data received from FCsession specific plugIn 
        *        The ownership is transferred.
		* @param aFCSession The Session which data is received to.
        */
    	void FCReceivedData( HBufC8* aData, const CFCSession* aFCSession );

    	/**
		* An asynchronous error has occurred in the FC related to the
		* given session indicated.
		* @param aError KErrNone succesfully, otherwise system wide or Socket error code
		* @param aFCSession The Session which error is received to.
		*/
		void FCErrorNotify( TInt aErrCode, const CFCSession* aFCSession );
	
	    
	private: // Second phase constructors

		CMceServerCore ();

		void ConstructL ();
		

	private: // Data:

		CMceCsServer*             iServer;			      
		CActiveScheduler*         iActiveScheduler;
		CMceCsServerCloseTimer*   iCloseTimer;
		CMceSipManager*     	  iManager;
		
    	RArray<TMceFCSessionItem> iFCSessions;
		CFCPlugInEngine*		  iFCPlugInEngine;
		TUint                     iSessionCount;
	};

#endif 

// End of File
