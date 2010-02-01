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

#ifndef __CTCBEARER_H__
#define __CTCBEARER_H__

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MTcBearerFactory;
class MTcBearerManager;
class MTcBearerObserver;
class TInetAddr;

// CLASS DEFINITION
/**
 * CTcBearer acts as a façade to the rest of the connection services.
 * This is the only exported class in the module (in addition to the factories,
 * of course). The factory classes are used to select a bearer and this class
 * is then responsible for providing the connection services through
 * a bearer-independent interface.
 */
class CTcBearer
	: public CBase
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aFactory Specifies the used bearer type
		 * @return An initialized instance of this class
		 */
		IMPORT_C static CTcBearer* NewL( const MTcBearerFactory& aFactory );

		/// Destructor
		IMPORT_C ~CTcBearer();

	private: // Constructors

		/// Default constructor
		CTcBearer();

		/**
		 * 2nd phase constructor.
		 *
		 * @param aFactory Specifies the used bearer type
		 */
		void ConstructL( const MTcBearerFactory& aFactory );

	public: // New methods

		/// Initialize port and start waiting for incoming connections
		IMPORT_C void ConnectL( TInetAddr* aRemoteAddr = NULL );

		/// Shut down port and stop waiting for incoming connections
		IMPORT_C void Close();

		/**
		 * Send a block of data contained in aDes
		 *
		 * @param aDes Data to be sent
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		IMPORT_C void Send( const TDesC8& aDes );

		/**
		 * Receive a block of data to aDes. Completes when aDes is full.
		 *
		 * @param aDes Buffer for incoming data
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		IMPORT_C void Receive( TDes8& aDes );

		/**
		 * Receive a block of data to aDes. Completes when at least one
		 * character (byte) has been received. Length of aDes is changed
		 * to reflect the amount of bytes received.
		 *
		 * @param aDes Buffer for incoming data
		 * @exceptions Panics with KErrNotReady if not connected, and with
		 *			   KErrInUse if we were already sending or receiving.
		 */
		IMPORT_C void ReceiveOneOrMore( TDes8& aDes );

		/**
		 * Set connection event observer. Use NULL to deregister.
		 *
		 * @param aObserver Pointer to observer object or NULL.
		 */
		IMPORT_C void SetObserver( MTcBearerObserver* aObserver );

		/**
		 * Retrieves the bearer specific local address and formats
		 * it into human readable form.
		 *
		 * @param aDes Buffer for formatted local address.
		 */
		IMPORT_C void GetLocalAddressL( TDes& aDes );

	private: // Data

		/// Pointer to a bearer manager implementation. Owned.
		MTcBearerManager* iManager;

	};

#endif // __CTCBEARER_H__
