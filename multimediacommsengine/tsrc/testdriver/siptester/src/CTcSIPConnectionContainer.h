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

#ifndef __CTCSIPCONNECTIONCONTAINER_H__
#define __CTCSIPCONNECTIONCONTAINER_H__

// INCLUDES
#include <sipconnection.h>
#include <sipconnectionobserver.h>
#include <cdblen.h>
#include "TTcSIPReceived.h"

// FORWARD DECLARATIONS
class CTcSIPContext;
class CSIPHeaderBase;

// CLASS DEFINITION
/**
 * CTcSIPConnectionContainer implements a container for a single CSIPConnection
 * object. Its task is also to observe and queue SIP notifications for
 * future use.
 */
class CTcSIPConnectionContainer
	: public CBase,
	  public MSIPConnectionObserver
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aIAPName Internet Access Point name to be used.
		 * @return An initialized instance of this class.
		 */
		static CTcSIPConnectionContainer* NewL( CTcSIPContext& aContext,
												const TDesC8& aIAPName );

		/// Destructor
		~CTcSIPConnectionContainer();

	private:

		/**
		 * Constructor.
		 *
		 * @param aContext Reference to test context object.
		 * @param aIAPName Internet Access Point name to be used.
		 */
		CTcSIPConnectionContainer( CTcSIPContext& aContext,
								   const TDesC8& aIAPName );

		/// Default constructor. Not implementer.
		CTcSIPConnectionContainer();

		/// 2nd phase constructor
		void ConstructL();

	public: // From MSIPConnectionObserver

		void IncomingRequest( CSIPServerTransaction* aTransaction );
		void IncomingRequest( CSIPServerTransaction* aTransaction,
							  CSIPDialog& aDialog );

		void IncomingResponse( CSIPClientTransaction& aTransaction );
		void IncomingResponse( CSIPClientTransaction& aTransaction,
							   CSIPDialogAssocBase& aDialogAssoc );
		void IncomingResponse( CSIPClientTransaction& aTransaction,
							   CSIPInviteDialogAssoc* aDialogAssoc );
		void IncomingResponse( CSIPClientTransaction& aTransaction,
							   CSIPRegistrationBinding& aRegistration );

		void ErrorOccured( TInt aError, CSIPTransactionBase& aTransaction );
		void ErrorOccured( TInt aError, CSIPClientTransaction& aTransaction,
										CSIPRegistrationBinding& aRegistration );
		void ErrorOccured( TInt aError,	CSIPTransactionBase& aTransaction,
										CSIPDialogAssocBase& aDialogAssoc );
		void ErrorOccured( TInt aError, CSIPRefresh& aSIPRefresh );
		void ErrorOccured( TInt aError, CSIPRegistrationBinding& aRegistration );
		void ErrorOccured( TInt aError, CSIPDialogAssocBase& aDialogAssoc );

		void InviteCompleted( CSIPClientTransaction& aTransaction );

        void InviteCanceled( CSIPServerTransaction& aTransaction );
            
		void ConnectionStateChanged( CSIPConnection::TState aState );

	public: // New methods

		/**
		 * Returns the first item off the receive queue (FIFO). If no
		 * items are present, the function will wait for aTimeout seconds
		 * and then try fetching again.
		 *
		 * @param aTimeOut Time in seconds waited for items to arrive.
		 */
		TTcSIPReceived ReceivedItemL( TInt aTimeout );

		/// @return Reference to the contained SIP connection object.
		inline CSIPConnection& Connection() { return *iSipConnection; };

		/// @return Reference to the Internet Access Point name
		inline const TDesC8& IAPName() const { return iIAPName; };

	    /**
		 * Add the specified item to the received items queue.
		 * The item is copied to the internal array.
		 *
		 * @param aItem Referece to a Received object.
		 */
		void QueueReceivedL( TTcSIPReceived& aItem );

	private: // New methods

		/**
		 * CDeltaTimer callback. Called when the timer entry expires.
		 *
		 * @param aSelf Pointer to self
		 * @return KErrNone
		 */
		static TInt ReceiveTimeout( TAny* aSelf );

	private: // data

		/// Active scheduler wrapper for async waiting. Owned.
		CActiveSchedulerWait iActiveWait;

		/// Reference to the test contextg. Not owned.
		CTcSIPContext& iContext;

		/// Array of received items. Owned.
		CArrayFixSeg< TTcSIPReceived > iReceiveQueue;

		/// Timeout timer. Owned.
		CDeltaTimer* iTimer;

		/// Timeout entry. Owned.
		TDeltaTimerEntry iReceiveTimeout;

		/// SIP connection. Owned.
		CSIPConnection* iSipConnection;

		/// SIP Connection state
		CSIPConnection::TState iConnectionState;

		/// Internet Access Point name for the connection.
		TBuf8< KCommsDbSvrMaxFieldLength > iIAPName;

	};

#endif // __CTCSIPCONNECTIONCONTAINER_H__
