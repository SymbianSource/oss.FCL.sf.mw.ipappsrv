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

#ifndef __CTCGRIMREAPER_H__
#define __CTCGRIMREAPER_H__

//  INCLUDES
#include <e32base.h>

//  CLASS DEFINITION
/**
 * CTcGrimreaper is responsible for killing a thread that has not reported
 * to the Watcher server withing the specified timeout time
 * (i.e. it seems to be hanging).
 */
class CTcGrimreaper
    : public CActive
    {
    public: // Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @param aClient Reference to the client thread to be hunted.
		 * @param aTimeout Life timeout
		 * @return An initialised instance of this class.
		 */
		static CTcGrimreaper* NewL( const RThread& aClient,
									TInt aTimeout );

		/// Destructor
		~CTcGrimreaper();

    private: // Constructors

		/**
		 * Constructor.
		 *
		 * @param aTimeout Life timeout
		 */
        CTcGrimreaper( TInt aTimeout );

		/// Default constructor. Not implemented.
		CTcGrimreaper();

		/**
		 * 2nd phase constructor.
		 *
		 * @param aClient Reference to the client thread to be hunted.
		 */
        void ConstructL( const RThread& aClient );

	protected: // from CActive

		void DoCancel();
		void RunL();

	public: // New methods

		/// Extends the lifetime of the client by iTimeout
		void NotYet();

	private: // Data

		/// Handle to observed thread.
		RThread iClient;

		/// Life timeout
		TTimeIntervalMicroSeconds32 iTimeout;

		/// Timeout timer. Owned.
		RTimer iTimer;

    };

#endif // __CTCGRIMREAPER_H__
