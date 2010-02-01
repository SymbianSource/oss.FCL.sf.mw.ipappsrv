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
* Description:  See class definition below.
*
*/

#ifndef __CTCSERIALMANAGER_H__
#define __CTCSERIALMANAGER_H__

// INCLUDES
#include <e32base.h>
#include <c32comm.h>
#include "MTcBearerManager.h"
#include "MTcBearerObserver.h"
#include "CTcSerialConnection.h"

// FORWARD DECLARATIONS
class TTcSerialFactory;

// CLASS DEFINITION
/**
 * CTcSerialManager implements Serial port connection management.
 * It also owns and controls the CTcSerialConnection instance
 */
class CTcSerialManager
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
		static CTcSerialManager* NewL( const TTcSerialFactory& aFactory );

		/// Destructor
		~CTcSerialManager();

	private: // Constructors

		/**
		 * Constructor
		 *
		 * @param aFactory Specifies the used bearer type
		 */
		CTcSerialManager( const TTcSerialFactory& aFactory );

		/// Default constructor. Not implemented.
		CTcSerialManager();

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

        void LoadDriversAndCommModuleL();
        
	private: // Data

        /// Factory. Not owned.
        const TTcSerialFactory& iFactory;
        
        RCommServ iCommServ;
       
		/// Connection implementation. Owned.
		CTcSerialConnection* iConn;

		/// Connection observer. Not owned.
		MTcBearerObserver* iObserver;

	};

#endif // __CTCSERIALMANAGER_H__
