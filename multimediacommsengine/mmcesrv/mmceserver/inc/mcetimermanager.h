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
* Description:    This header file is included by the subsystem which is
*                  responsible for creating the CMceTimerManager instance.
*
*/




#ifndef __CMCETIMERMANAGER_H__
#define __CMCETIMERMANAGER_H__



// INCLUDES
#include <e32base.h>
#include "mceexpirationhandler.h"

// Timer call back function used by CDeltaTimer
typedef TInt(*MceTimerCallBack)(TAny *aPtr);

// CLASS DEFINITION
/**
 * CMceTimerManager is the main class of the Lightweight timer subsystem. It is
 * intended to be used in a way that there is only one instance of the
 * CMceTimerManager class, and the basic timer services are available through the
 * MTimerManager interface.
 *
 * Several logical timers can run simultaneously, but only one CTimer derived
 * timer is used.
 */
class CMceTimerManager :
    public CBase,
    public MMceTimerManager

    {
    public: // Constructors and destructor

        /**
        * Static constructor.
        *
        * @see CMceTimerManager::NewLC
        * @return An initialized instance of this class.
        */
        static CMceTimerManager* NewL();

        /**
        * Static constructor. Leaves pointer to cleanup stack.
        *
        * @see CMceTimerManager::NewL
        * @return An initialized instance of this class.
        */
        static CMceTimerManager* NewLC();

        /// Destructor
        ~CMceTimerManager();

    public: // From MTimerManager:

        TMceTimerId StartL( MMCEExpirationHandler* aObserver,
						 TUint aMilliseconds );

        TMceTimerId StartL( MMCEExpirationHandler* aObserver,
						 TUint aMilliseconds,
						 TAny* aTimerParam );
						 
        TInt Stop( TMceTimerId aTimerId );
		
        TBool IsRunning( TMceTimerId aTimerId ) const;

        static TInt TimerExpiredCallBack(TAny *aPtr);

    private: // New methods

		/**
		 * Generates a new unique TMceTimerId value. This is basically just
		 * a linear sequence of TUint32 values (0,1,2,3,4...n)
		 *
		 * @return New TimerId value
		 */
		TMceTimerId NewTimerId();

		/**
		 * Finds a timer entry from the iEntries array based on the supplied id.
		 *
		 * @return Timer entry index within iEntries or KErrNotFound.
		 */
		TInt FindEntry( TMceTimerId aTimerId ) const;

		/// Queue the first timer entry to the system timer (if any)
		void ActivateFirstEntry();
		
	private: //  Constructors and destructor

		/// Default constructor
		inline CMceTimerManager();
		
		/// 2nd phase constructor
		inline void ConstructL();

	private: // Nested classes

		// CLASS DEFINITION
		class TEntry //  : public TDeltaTimerEntry
			{
			public: // Constructors and destructor

				/// Default constructor. Resets all member variables.
			    TEntry();

			public: // Data

				/// Unique timer id within CMceTimerManager
				TMceTimerId iId;
			
				/// Observer that is notified on timer expiration. Not owned.
				MMCEExpirationHandler* iObserver;  
				
				/// Parameter value that is passed to the observer. Not owned
				TAny* iObserverParam;

				/// Parameter value that is passed to the CDeltaTimer::QueueLong
				TDeltaTimerEntry iEntry;

				/// Parameter value that is contain the call back function
				TCallBack iCallBack;

				/// Used to remove TEntry pointer from CMceTimerManager::iEntries. Not owned
				CMceTimerManager* iTimerManager;


			};

	private: // Data

		/// Timer entry array. Owned.
		RPointerArray< TEntry > iEntries;

		/// Counter used to produce unique TimerId values
		TMceTimerId iTimerIdCounter;

		/// Implemented the timer queue and scheduler logic
		CDeltaTimer *iTimerContainer;
		
	private: // For testing purposes

#ifdef MCE_TIMER_MANAGER_UNIT_TEST
		friend class CMceTimerManagerTest;
#endif
		
	};


#endif // __CMCETIMERMANAGER_H__
