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

#ifndef __CTCWATCHER_H__
#define __CTCWATCHER_H__

//  INCLUDES
#include <e32base.h>
#include "RTcWatcher.h"

//  CLASS DEFINITION
/**
 * CTcWatcher implements a wrapper on top of RTcWatcher providing
 * a timer for automatically calling RTcWatcher::Ping().
 * This class should be preferred by clients instead of RTcWatcher.
 */
class CTcWatcher
    : public CActive
    {
    public: // Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aAppName Application name and full path, for restarting.
		 * @param aTimeout Life timeout
		 * @return An initialised instance of this class.
		 */
		IMPORT_C static CTcWatcher* NewL( const TDesC& aAppName, TInt aTimeout );

		/// Destructor
		IMPORT_C ~CTcWatcher();

    private: // Constructors

		/// Default constructor.
		CTcWatcher();

		/**
		 * Constructor.
		 *
		 * @param aAppName Application name and full path, for restarting.
		 * @param aTimeout Life timeout
		 */
        void ConstructL( const TDesC& aAppName, TInt aTimeout );

	protected: // from CActive

		void DoCancel();
		void RunL();

	private: // Data

		/// Timeout timer. Owned.
		RTimer iTimer;

		/// Life timeout
		TTimeIntervalMicroSeconds32 iTimeout;

		/// Watcher client. Owned.
		RTcWatcher iWatcher;

    };

#endif // __CTCWATCHER_H__
