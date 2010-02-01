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
* Description:    This header file is included by the subsystems which are
*                  using timer services, but are not responsible for creating
*                  the CTimerManager instance.
*
*/



#ifndef __MCETIMER_H__
#define __MCETIMER_H__

// INCLUDES
#include <e32def.h>

/// Every timer is associated with an identifier of this type.
/// When a timer is set, TMceTimerId is returned to the user of the Lightweight
/// timer subsystem.
/// When a timer expires, its TMceTimerId is passed in the callback.
/// If the user wishes to stop or adjust a certain timer, it has to pass the
/// TMceTimerId of the timer in question to the Lightweight timer subsystem.
typedef TUint32 TMceTimerId;

// CLASS DEFINITION
/**
 * When a timer expires, this interface is called by the Ligthweight timer
 * subsystem.
 */
class MMCEExpirationHandler
	{
	public: // Constructors and destructor

		/// Virtual destructor
		virtual ~MMCEExpirationHandler() {}

	public: // Abstract methods

		/**
		 * Indicates that a timer has expired.
		 *
		 * @param aTimerId Identifies the expired timer
		 * @param aTimerParam User specified value which was given when the timer
		 *	was set. It can be used to identify the timer in case multiple timers
		 *  are running simultaneously. Value is NULL if the timer was set using
		 *  CTimerManager::StartL() without parameter aTimerParam. Ownership isn't
	     *  transferred.
		 */
		virtual void TimerExpiredL( TMceTimerId aTimerId, TAny* aTimerParam ) = 0;
	};


// CLASS DEFINITION
/**
 * MMceTimerManager provides methods for starting, stopping and adjusting timers.
 */
class MMceTimerManager
	{
	public: // Enumerations

		/// This exists for backward compatibility
		enum TMceTimerIdValues
			{
			/// TimerId that is never used with a valid timer
			KNoSuchTimer = 0
			};

	public: // Constructors and destructor

		/// Virtual destructor
		virtual ~MMceTimerManager() {}

	public:	// Abstract methods

		/**
		 * Creates a new timer and starts it.
		 *
		 * @pre aObserver != NULL
		 * @see See also this method 'MMCEExpirationHandler::TimerExpiredL'
		 *
		 * @param aObserver	IN: Callback to use when timer expires. Ownership is
		 *	not transferred.
		 * @param aMilliseconds	Timer duration in milliseconds (with 32 bits, the
		 *	max value is ~50 days)
		 * @return value TimerId value identifying the new timer
		 *
		 * In case of an error, this function leaves.
		 */
		virtual TMceTimerId StartL( MMCEExpirationHandler* aObserver,
							     TUint aMilliseconds ) = 0;
							     
		/**
		 * Creates a new timer and starts it.
		 *
		 * @pre aObserver != NULL
		 * @see See also this method 'MMCEExpirationHandler::TimerExpiredL'
		 *
		 * @param aObserver	IN: Callback to use when timer expires. Ownership is
		 *	not transferred.
		 * @param aMilliseconds	Timer duration in milliseconds (with 32 bits, the
		 *	max value is ~50 days)
		 * @param aTimerParam User specified value which will be passed to the
		 *	aObserver function MMCEExpirationHandler::TimerExpiredL() when the timer
		 *	expires. Ownership isn't transferred.
		 * @return value TimerId value identifying the new timer
		 *
		 * In case of an error, this function leaves.
		 */
		virtual TMceTimerId StartL( MMCEExpirationHandler* aObserver,
							     TUint aMilliseconds,
							     TAny* aTimerParam ) = 0;							     

		/**
		 * Stops the specified timer.
		 *
		 * This function doesn't leave in case of error, as it is thought that this
		 * function is usually also called from destructors.
		 *
		 * @param aTimerId Identifies the timer to be stopped
		 * @return value KErrNone: successful
	     *               KErrNotFound : no such timer exists
		 */
		virtual TInt Stop( TMceTimerId aTimerId ) = 0;

		/**
		 * Checks if there exists a timer which hasn't expired yet, with the
		 * specified TimerId.
		 *
		 * @param aTimerId Identifies the timer
		 * @return value ETrue: timer exists, KFalse: no such timer
		 */
		virtual TBool IsRunning( TMceTimerId aTimerId ) const = 0;

	};

#endif // __MCETIMER_H__


