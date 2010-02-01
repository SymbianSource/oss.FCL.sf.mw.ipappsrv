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

#ifndef __CTCCORONER_H__
#define __CTCCORONER_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MTcDeathObserver;

//  CLASS DEFINITION
/**
 * CTcCoroner acts as a thread death observer. Its task is to notify
 * a registered MTcDeathObserver about the death
 * and also write a log entry about the event.
 */
class CTcCoroner
    : public CActive
    {
    public: // Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aClient Reference to the client thread to be observed.
		 * @param aAppName Application name and full path, for restarting.
		 * @param aObserver Observer to be notified of client death
		 * @return An initialised instance of this class.
		 */
		static CTcCoroner* NewL( const RThread& aClient,
								 const TDesC& aAppName,
								 MTcDeathObserver& aObserver );

		/// Destructor
		~CTcCoroner();

    private: // Constructors

		/**
		 * Constructor.
		 *
		 * @param aObserver Observer to be notified of client death
		 * @param aAppName Application name and full path, for restarting.
		 */
		CTcCoroner( MTcDeathObserver& aObserver,
					const TDesC& aAppName );

		/// Default constructor, not implemented.
		CTcCoroner();

		/**
		 * 2nd phase constructor.
		 *
		 * @param aClient Reference to the client thread to be observed.
		 */
        void ConstructL( const RThread& aClient );

	protected: // from CActive

		void DoCancel();
		void RunL();

	private: // Data

		/// Handle to observed thread.
		RThread iClient;

		/// Client application name. Owned.
		TFileName iAppName;

		/// Client death observer. Not owned.
		MTcDeathObserver& iObserver;

    };

#endif // __CTCCORONER_H__
