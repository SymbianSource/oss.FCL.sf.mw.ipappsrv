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

#ifndef __CTCBTMANAGER_H__
#define __CTCBTMANAGER_H__

// INCLUDES
#include <e32base.h>
#include <btsdp.h>
#include "MTcBearerManager.h"
#include "MTcBearerObserver.h"
#include "CTcBtConnection.h"

// FORWARD DECLARATIONS
class TTcBtFactory;

// CLASS DEFINITION
/**
 * CTcBtManager implements Bluetooth specific connection management,
 * like security settings and service advertisement.
 * It also owns and controls the CTcBtConnection instance
 */
class CTcBtManager
	: public CBase,
	  public MTcBearerManager,
	  public MTcBearerObserver
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aFactory Specifies the used bearer type
		 * @return An initialized instance of this class
		 */
		static CTcBtManager* NewL( const TTcBtFactory& aFactory );

		/// Destructor
		~CTcBtManager();

	private: // Constructors

		/**
		 * Constructor
		 *
		 * @param aFactory Specifies the used bearer type
		 */
		CTcBtManager( const TTcBtFactory& aFactory );

		/// Default constructor. Not implemented.
		CTcBtManager();

		/// 2nd phase constructor.
		void ConstructL();

	public: // From MTcBearerManager

		void ConnectL( TInetAddr* aRemoteAddr );
		void Close();
		void Send( const TDesC8& aDes );
		void Receive( TDes8& aDes );
		void ReceiveOneOrMore( TDes8& aDes );
		void SetObserver( MTcBearerObserver* aObserver );
		void GetLocalAddressL( TDes& aDes );

	public: // From MTcBearerObserver

		void BearerCompletion( MTcBearerObserver::TOperation aOp,
							   TInt aStatus );

	private: // New methods

		/**
		 * Add our service to the Service Discovery Database (SDP)
		 *
		 * @param aChannel Bluetooth channel/port number of the service
		 */
		void StartAdvertiserL( TInt aChannel );

		/// Remove our service from the SDP
		void StopAdvertiser();

		/**
		 * Mark our service as available or unavailable in the SDP.
		 *
		 * @param aIsAvailable ETrue for available,
		 *					   EFalse for unavailable
		 */
		void MakeAvailableL( TBool aIsAvailable );

		/**
		 * Define security requirements for incoming connections.
		 *
		 * @param aChannel Bluetooth channel/port number
		 * @param aAuthentication ETrue = required, EFalse = not required
		 * @param aEncryption ETrue = required, EFalse = not required
		 * @param aAuthorisation ETrue = required, EFalse = not required
		 */
#ifndef __BLUETOOTH_API_V2__
		void SetSecurityL( TInt aChannel,
						   TBool aAuthentication,
						   TBool aEncryption,
						   TBool aAuthorisation );
#endif
		/// Turn on BT if it's not already on. May result in a dialog
		/// shown to the user.
		void TurnOnBtL();

	private: // Data

		/// Connection implementation. Owned.
		CTcBtConnection iConn;

		/// Connection observer. Not owned.
		MTcBearerObserver* iObserver;

		/// SDP session. Owned.
		RSdp iSdpServ;

		/// SDP database handle. Owned.
		RSdpDatabase iSdpDb;

		/// SDP record for our service
		TSdpServRecordHandle iSdpRecord;

		/// State (bit like a version number) of SDP records
		TInt iSdpRecordState;
		
		/// Socket server session. Owned.
		RSocketServ iSockServ;

	};

#endif // __CTCBTMANAGER_H__
