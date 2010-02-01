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

#ifndef __MTCBEARERMANAGER_H__
#define __MTCBEARERMANAGER_H__

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MTcBearerObserver;
class TInetAddr;

// CLASS DEFINITION
/**
 * MTcBearerManager defines an interface for bearer managers.
 * Managers are used for governing the connections and performing
 * non-connection related bearer initialization tasks.
 */
class MTcBearerManager
	{
	public:	// Constructors and destructor

		/// Virtual destructor. Allow deletion through this interface.
		virtual ~MTcBearerManager() {}

	public: // Abstract methods

		/// Initialize port and start waiting for incoming connections
		virtual void ConnectL( TInetAddr* aRemoteAddr = NULL ) = 0;

		/// Shut down port and stop waiting for incoming connections
		virtual void Close() = 0;

		/**
		 * Send a block of data contained in aDes
		 *
		 * @param aDes Data to be sent
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		virtual void Send( const TDesC8& aDes ) = 0;

		/**
		 * Receive a block of data to aDes. Completes when aDes is full.
		 *
		 * @param aDes Buffer for incoming data
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		virtual void Receive( TDes8& aDes ) = 0;

		/**
		 * Receive a block of data to aDes. Completes when at least one
		 * character (byte) has been received. Length of aDes is changed
		 * to reflect the amount of bytes received.
		 *
		 * @param aDes Buffer for incoming data
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		virtual void ReceiveOneOrMore( TDes8& aDes ) = 0;

		/**
		 * Set connection event observer. Use NULL to deregister.
		 *
		 * @param aObserver Pointer to observer object or NULL.
		 */
		virtual void SetObserver( MTcBearerObserver* aObserver ) = 0;

		/**
		 * Retrieves the bearer specific local address and formats
		 * it into human readable form.
		 *
		 * @param aDes Buffer for formatted local address.
		 */
		virtual void GetLocalAddressL( TDes& aDes ) = 0;

	};

#endif // __MTCBEARERMANAGER_H__
