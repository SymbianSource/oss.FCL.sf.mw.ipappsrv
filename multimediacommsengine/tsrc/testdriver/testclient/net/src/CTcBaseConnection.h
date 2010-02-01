/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  See class definition below.
*
*/

#ifndef __CTCBASECONNECTION_H__
#define __CTCBASECONNECTION_H__

// INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include "MTcBearerObserver.h"
#include <in_sock.h>

// CLASS DEFINITION
/**
 * CTcBaseConnection implements a base class for socket connections.
 * Basically provides core send and receive operations. Send and receive
 * cannot be used at the same time (this is a design choice; those are
 * not actually ever needed simultaneously with the TestClient).
 */
class CTcBaseConnection
	: public CActive
	{
	protected: // Enumerations

		/// Internal connection state
		enum TState
			{
			/// Not connected
			EIdle,
			/// Waiting for incoming connections or connecting to remote address
			EConnecting,
			/// Waiting after unsuccesfull connection attempt
			EWaitingRetry,
			/// Connected with a remote party
			EConnected,
			/// Sending or waiting to send data
			ESending,
			/// Receiving or waiting to receive data
			EReceiving
			};

	public:	// Constructors and destructor

		/// Destructor
		~CTcBaseConnection();

	protected:	// Constructors and destructor

		/// Default constructor
		CTcBaseConnection();

	public: // From CActive

		void RunL();
		void DoCancel();

	public: // New methods

		/// Initialize port and start waiting for incoming connections
		void ConnectL();
		
		/// Connect to remote address
		void ConnectL( TInetAddr& aRemoteAddr );

		/// Shut down port and stop waiting for incoming connections
		void Close();

		/**
		 * Send a block of data contained in aDes
		 *
		 * @param aDes Data to be sent
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		void Send( const TDesC8& aDes );

		/**
		 * Receive a block of data to aDes. Completes when aDes is full.
		 *
		 * @param aDes Buffer for incoming data
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		void Receive( TDes8& aDes );

		/**
		 * Receive a block of data to aDes. Completes when at least one
		 * character (byte) has been received. Length of aDes is changed
		 * to reflect the amount of bytes received.
		 *
		 * @param aDes Buffer for incoming data
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		void ReceiveOneOrMore( TDes8& aDes );

		/**
		 * Set connection event observer. Use NULL to deregister.
		 *
		 * @param aObserver Pointer to observer object or NULL.
		 */
		void SetObserver( MTcBearerObserver* aObserver );
		
		/**
		 * Set connection.
		 *
		 * @param aSocketServ
		 * @param aConnection
		 */
		void SetConnection( RSocketServ* aSocketServ, RConnection* aConnection );

		/// @return Local port number for the connection
		TInt LocalPort() const;

	protected: // Abstract methods

		/// Called to perform bearer specific port setup. Implemented
		/// by derived classes.
		virtual void SetupPortL() = 0;
		
		virtual void SetupPort2L();

	protected: // New metods

		/**
		 * Notifies a registered observer about state transitions.
		 *
		 * @param aState Connection state
		 * @param aStatus System-wide error code or KErrNone
		 */
		void Notify( TState aState, TInt aStatus );

	protected: // Data

		/// Connection state.
		TState iState;

		/// Pointer to connection observer. Not owned.
		MTcBearerObserver* iObserver;

		/// Socket server session.  Not owned.
		RSocketServ* iSocketServ;

		/// Data transfer socket. Owned.
		RSocket iSocket;

		/// Listening socket. Owned.
		RSocket iListeningSocket;

		/// Socket address (generic). Owned.
		TSockAddr iSockAddr;

		/// Local port where we're listening
		TInt iLocalPort;

		/// Transfer length for RecvOneOrMore(), value not used but
		/// must still be present.
		TSockXfrLength iIgnoredLength;
		
		RConnection* iConnection;
		
		// Remote address for connection retry
		TInetAddr iRemoteAddr;
		
		// Timer for waiting for retry
		RTimer iTimer;

	};

#endif // __CTCBASECONNECTION_H__
