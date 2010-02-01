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

#ifndef __CTCTCPMANAGER_H__
#define __CTCTCPMANAGER_H__

// INCLUDES
#include <es_sock.h>
#include <commdbconnpref.h>
#include "CTcTcpConnection.h"
#include "MTcBearerManager.h"
#include "MTcBearerObserver.h"

// FORWARD DECLARATIONS
class TTcTcpFactory;

// CLASS DEFINITION
/**
 * CTcTcpManager implements TCP/IP specific connection management,
 * like Internet access point settings. It also owns and controls
 * the CTcTcpConnection instance.
 */
class CTcTcpManager
	: public CActive,
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
		static CTcTcpManager* NewL( const TTcTcpFactory& aFactory );

		/// Destructor
		~CTcTcpManager();

	private: // Constructors

		/**
		 * Constructor
		 *
		 * @param aFactory Specifies the used bearer type
		 */
		CTcTcpManager( const TTcTcpFactory& aFactory );

		/// Default constructor. Not implemented.
		CTcTcpManager();

		/// 2nd phase constructor.
		void ConstructL();

	public: // From CActive

		void RunL();
		void DoCancel();

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

	private: // Data

		/// Connection implementation. Owned.
		CTcTcpConnection iConn;

		/// Connection observer. Not owned.
		MTcBearerObserver* iObserver;

		/// Connection server session. Owned.
		RConnection iConnServ;

		/// Socket server session. Owned.
		RSocketServ iSockServ;

		/// Internet Access Point Id.
		/// 0 = ask user, >0 = use quietly
		TInt iIAPId;
		
		TCommDbConnPref iPrefs;
		
		TInetAddr* iRemoteAddr;

	};

#endif // __CTCTCPMANAGER_H__
