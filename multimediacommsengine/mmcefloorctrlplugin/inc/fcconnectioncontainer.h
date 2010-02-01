/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    fcconnectioncontainer.h
*
*/




#ifndef __CFCCONNECTIONCONTAINER_H__
#define __CFCCONNECTIONCONTAINER_H__

// INCLUDES
#include <in_sock.h>
#include <commdbconnpref.h>

// FORWARD DECLARATIONS
class MFCConnectionNotifier;

/**
 *  interface for opening Connection
 *
 *  CFCConnectionContainer defines an interface for opening Connection
 *
 *  @lib FCTBCP.lib
 *  @since S60 v3.0
 */
class CFCConnectionContainer : public CActive
	{
	public: // Constructors and destructor
		/**
		* Static constructor.
		* @param aNotifier Reference to notifier
		* @return An initialized instance of this class.
		*/
		static CFCConnectionContainer* NewL( MFCConnectionNotifier& aNotifier );

		/**
		* Destructor.
		*/
		~CFCConnectionContainer();

	public:
		/**
		* Gets the RConnection
		* @return the intance of RConnection
		*/ 
		RConnection& Connection();
		
		/**
		* Open RConnection after RSocketServer has open
		* @param aIapId IapId
		* @param aServer a RSocket Server
		* @return the error code if there is connection error
		*/ 
		TInt OpenConnection( TUint32 aIapId, RSocketServ& aServer );

		/**
		* Close RConnection
		*/ 
		void CloseConnection();

	protected: // From CActive
		/**
		* Cancel any outstanding request
		*/ 
		void DoCancel();
		
		/**
		* Handles an active object’s request completion event
		*/ 
		void RunL();

	private: // Constructors
		/**
		* Constructor
		* @param aNotifier, MFCConnectionNotifier
		* @return An initialized instance of this class.
		*/
		CFCConnectionContainer( MFCConnectionNotifier& aNotifier );
	
	private: // Data
		MFCConnectionNotifier& iNotifier;
		RConnection	iConnection;
		TCommDbConnPref iPrefs;
	
	private: // For testing purposes
		friend class UT_CFCConnectionContainer;
		friend class UT_CTBCPFloorControlImpl;
		friend class UT_CFCSender;
		friend class UT_CFCReceiver;
	};

#endif	// __CCONNECTIONCONTAINER_H__
