/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    
*
*/




#ifndef CMCCTIMERMANAGER_H
#define CMCCTIMERMANAGER_H

// INCLUDES
#include <e32base.h>
#include "mccexpirationhandler.h"

// CLASS DEFINITION
/**
 * Several logical timers can run simultaneously, but only one RTimer is used.
 *
 */
class CMccTimerManager
	: public CActive,
	  public MMccTimerManager
	{
	public: // Constructors and destructor

		/**
		 * Static constructor.
		 *
		 * @see CMccTimerManager::NewLC
		 * @return An initialized instance of this class.
		 */
		static CMccTimerManager* NewL();

		/**
		 * Static constructor. Leaves pointer to cleanup stack.
		 *
		 * @see CMccTimerManager::NewL
		 * @return An initialized instance of this class.
		 */
		static CMccTimerManager* NewLC();

	    /// Destructor
		~CMccTimerManager();

	public: // From CActive
	
		void DoCancel();
		void RunL();
		TInt RunError( TInt aError );

	public: // From MMccTimerManager

		TMccTimerId StartL( MMccExpirationHandler* aObserver,
						 TUint aMilliseconds );

		TMccTimerId StartL( MMccExpirationHandler* aObserver,
						 TUint aMilliseconds,
						 TAny* aTimerParam );
						 
		TInt Stop( TMccTimerId aTimerId );
		
		TBool IsRunning( TMccTimerId aTimerId ) const;

	private: // New methods

		/**
		 * Generates a new unique TMccTimerId value. This is basically just
		 * a linear sequence of TUint32 values (0,1,2,3,4...n)
		 *
		 * @return New TimerId value
		 */
		TMccTimerId NewTimerId();

		/**
		 * Finds a timer entry from the iEntries array based on the supplied id.
		 *
		 * @return Timer entry index within iEntries or KErrNotFound.
		 */
		TInt FindEntry( TMccTimerId aTimerId ) const;

		/// Queue the first timer entry to the system timer (if any)
		void ActivateFirstEntry();
		
	private: //  Constructors and destructor

		/// Default constructor
		inline CMccTimerManager();
		
		/// 2nd phase constructor
		inline void ConstructL();

	private: // Nested classes

		// CLASS DEFINITION
		/**
		 * TEntry defines a simple timer entry container for CMccTimerManager.
		 */
		class TEntry
			{
			public: // Constructors and destructor

				/// Default constructor. Resets all member variables.
				TEntry();

			public: // Data

				/// Unique timer id within CMccTimerManager
				TMccTimerId iId;
				/// Timestamp of system clock when this entry should expire. 
				TTime iTimeStamp;
				/// Observer that is notified on timer expiration. Not owned.
				MMccExpirationHandler* iObserver;
				/// Parameter value that is passed to the observer.
				TAny* iObserverParam;

			};

	private: // Data

		/// System timer. Owned.
		RTimer iTimer;

		/// Timer entry array. Owned.
		CArrayFixFlat< TEntry > iEntries;

		/// Counter used to produce unique TimerId values
		TMccTimerId iTimerIdCounter;
		
    #ifdef TEST_EUNIT
    public:
        friend class UT_CMccTimerManager;
    #endif
	};

#endif // CMCCTIMERMANAGER_H
