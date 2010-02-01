/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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



#include "mcctimermanager.h"
#include "mcccontrollerlogs.h"


// Granularity for timer entry array iEntries
const TInt KArrayGranularity( 8 );

// 30min is the longest time we wait using RTimer::After() and millisecond
// precision (absolute max would be ~35min). Anything longer goes for
// RTimer::At() and one second precision.
const TInt KMaxSeconds( 30 * 60 );

// One second as microseconds
const TInt KOneSecondAsMicros( 1000000 );

// Minimum time interval for waiting with system timers (8 ms)
// (max +-50% error with smallest delays (timer resolution in hw = ~15ms)
const TInt KMinMicroseconds( 8000 );

// -----------------------------------------------------------------------------
// CMccTimerManager::NewL
// -----------------------------------------------------------------------------
//
CMccTimerManager* CMccTimerManager::NewL()
	{
	CMccTimerManager* self = CMccTimerManager::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::NewLC
// -----------------------------------------------------------------------------
//
CMccTimerManager* CMccTimerManager::NewLC()
	{
	CMccTimerManager* self = new( ELeave ) CMccTimerManager;
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::~CMccTimerManager
// -----------------------------------------------------------------------------
//
CMccTimerManager::~CMccTimerManager()
	{
	Cancel();
	iTimer.Close();
	iEntries.Reset();
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::DoCancel
// -----------------------------------------------------------------------------
//
void CMccTimerManager::DoCancel()
	{
	iTimer.Cancel();
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::RunL
// -----------------------------------------------------------------------------
//
void CMccTimerManager::RunL()
	{
    __CONTROLLER("CMccTimerManager::RunL, Entry")
    
    if ( iEntries.Count() > 0 )
        {
    	// The first entry is the active one (which has expired)
    	// Make a copy so that we can delete the instance from the array
    	TEntry entry = iEntries[ 0 ];
    	iEntries.Delete( 0 );

    	// Schedule the next timer entry for execution (if any)
    	ActivateFirstEntry();

    	// Notify the observer, leave errors are ignored in RunError	
    	entry.iObserver->TimerExpiredL( entry.iId, entry.iObserverParam );
        }
	
	__CONTROLLER("CMccTimerManager::RunL, Exit")
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::RunError
// -----------------------------------------------------------------------------
//
TInt CMccTimerManager::RunError( TInt aError )
    {
    __CONTROLLER_INT1("CMccTimerManager::RunError, error:", aError)
    
    if ( aError != KErrNoMemory )
        {
        aError = KErrNone;
        }
        
    return aError;
    }
    
// -----------------------------------------------------------------------------
// CMccTimerManager::StartL
// -----------------------------------------------------------------------------
//
TMccTimerId CMccTimerManager::StartL( MMccExpirationHandler* aObserver,
								TUint aMilliseconds )
	{
	return StartL( aObserver, aMilliseconds, NULL );
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::StartL
// -----------------------------------------------------------------------------
//
TMccTimerId CMccTimerManager::StartL( MMccExpirationHandler* aObserver,
								TUint aMilliseconds,
								TAny* aTimerParam )
	{
    __CONTROLLER_INT1( "CMccTimerManager::StartL aMilliseconds: ", aMilliseconds )
    
	// Do not accept a NULL observer
	if( !aObserver )
		{
		User::Leave( KErrArgument );
		}

	// Get new unique timer id
	TMccTimerId newTimerId = NewTimerId();

	// Define a new entry with the new id
	TEntry newEntry;
	newEntry.iId = newTimerId;
	newEntry.iObserver = aObserver;
	newEntry.iObserverParam = aTimerParam;

	// Calculate a timestamp for the delay (aMilliseoncds from current time)
	TInt64 microSeconds( aMilliseconds );
	microSeconds *= 1000;
	newEntry.iTimeStamp.HomeTime();
	newEntry.iTimeStamp += TTimeIntervalMicroSeconds( microSeconds );

	// Find an insertion point from the array
	// Sort entries in timestamp order ("nearest" timestamp first)
	TInt entryPos;
	TInt count = iEntries.Count();
	for( entryPos = 0; entryPos < count; entryPos++ )
		{
		if( iEntries[ entryPos ].iTimeStamp > newEntry.iTimeStamp )
			{
			break;
			}
		}

	// Insert the new entry to the array
	iEntries.InsertL( entryPos, newEntry );

	// If the entry ended being the first entry, cancel any previous entries
	// and schedule this one to the system timer instead.
	if( entryPos == 0 )
		{
		ActivateFirstEntry();
		}

    __CONTROLLER("CMccTimerManager::StartL, Exit")
	return newEntry.iId;
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::Stop
// -----------------------------------------------------------------------------
//
TInt CMccTimerManager::Stop( TMccTimerId aTimerId )
	{
    __CONTROLLER("CMccTimerManager::Stop, Entry")
	__CONTROLLER_INT1("timer", aTimerId )
	
	// Find the timer entry to be stopped
	TInt entryIndex = FindEntry( aTimerId );
	if( entryIndex == KErrNotFound )
		{
        __CONTROLLER("CMccTimerManager::Stop - timer not active, Exit")
		return KErrNotFound;
		}

	// Delete the timer entry from our array
	iEntries.Delete( entryIndex );

	// Cancel system timer if this entry was the active one
	if( entryIndex == 0 )
		{
		// Schedule the next timer entry for execution (if any)
		ActivateFirstEntry();
		}

    __CONTROLLER("CMccTimerManager::Stop, Exit")
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::IsRunning
// -----------------------------------------------------------------------------
//
TBool CMccTimerManager::IsRunning( TMccTimerId aTimerId ) const
	{

	TInt entryIndex = FindEntry( aTimerId );
	if( entryIndex != KErrNotFound )
		{
		return ETrue;
		}
	return EFalse;
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::NewTimerId
// -----------------------------------------------------------------------------
//
TMccTimerId CMccTimerManager::NewTimerId()
	{
	// Note that a rolling unsigned 32bit integer id counter
	// is not strictly unique - however, it is not possible
	// to have a timer with id 1 and id 2^32-1 at the same time
	// due to time and memory constraints.
	iTimerIdCounter++;
	if( iTimerIdCounter == KNoSuchTimer )
		{
		iTimerIdCounter++;
		}
	return iTimerIdCounter;
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::FindEntry
// -----------------------------------------------------------------------------
//
TInt CMccTimerManager::FindEntry( TMccTimerId aTimerId ) const
	{
	// Search for an entry with a matching id
	// (entries are sorted by timestamp, not by id)
	TInt count = iEntries.Count();
	for( TInt i = 0; i < count; i++ )
		{
		if( iEntries[ i ].iId == aTimerId )
			{
			return i;
			}
		}

	return KErrNotFound;
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::ActivateFirstEntry
// -----------------------------------------------------------------------------
//
void CMccTimerManager::ActivateFirstEntry()
	{
	Cancel();
	if( iEntries.Count() > 0 )
		{
		// Calculate required interval from current time
		TTime now;
		now.HomeTime();
		TTime timeStamp = iEntries[ 0 ].iTimeStamp;
		TInt64 interval( timeStamp.MicroSecondsFrom( now ).Int64() );
		TInt64 intervalSeconds = interval / KOneSecondAsMicros;

		// Check that the interval is long enough, otherwise complete
		// immediately
		if( interval > KMinMicroseconds )
			{
			// If interval is greater than ~33minutes (32bits), we have to use
			// RTimer::At() instead of RTimer::After()
			// At(), however, uses a precision of one second as compared
			// to 1/64th (1/10th in emulator) precision of After()
			if( intervalSeconds < KMaxSeconds )
				{
				iTimer.After( iStatus,
						TTimeIntervalMicroSeconds32( I64INT( interval ) ) );
				}
			else
				{
				iTimer.At( iStatus, iEntries[ 0 ].iTimeStamp );
				}
			}
		else
			{
			// Complete immediately
			TRequestStatus* status = &iStatus;
			User::RequestComplete( status, KErrNone );
			}
		SetActive();
		}
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::CMccTimerManager
// -----------------------------------------------------------------------------
//
CMccTimerManager::CMccTimerManager()
	: CActive( CActive::EPriorityStandard ),
	  iEntries( KArrayGranularity ),
	  iTimerIdCounter( KNoSuchTimer )
	{
	CActiveScheduler::Add( this );
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::ConstructL
// -----------------------------------------------------------------------------
//
void CMccTimerManager::ConstructL()
	{
	User::LeaveIfError( iTimer.CreateLocal() );
	}

// -----------------------------------------------------------------------------
// CMccTimerManager::TEntry::TEntry
// -----------------------------------------------------------------------------
//
CMccTimerManager::TEntry::TEntry()
	: iId( KNoSuchTimer ), iTimeStamp(),
	  iObserver( NULL ), iObserverParam( NULL )
	{
	}
