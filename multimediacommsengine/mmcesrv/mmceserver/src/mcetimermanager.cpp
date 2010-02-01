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




#include "mcetimermanager.h"
#include "mcesrvlogs.h"
#include "mcesip.h"

// Granularity for timer entry array iEntries
const TInt KArrayGranularity( 8);

// One millisecond as microseconds
const TInt KOneMillisAsMicros( 1000);

// -----------------------------------------------------------------------------
// CMceTimerManager::NewL
// -----------------------------------------------------------------------------
//

CMceTimerManager* CMceTimerManager::NewL()
    {
    CMceTimerManager* self = CMceTimerManager::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::NewLC
// -----------------------------------------------------------------------------
//
CMceTimerManager* CMceTimerManager::NewLC()
    {
    CMceTimerManager* self = new( ELeave ) CMceTimerManager;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::~CMceTimerManager
// -----------------------------------------------------------------------------
//
CMceTimerManager::~CMceTimerManager()
    {
    if(iTimerContainer)
    	{
    	iTimerContainer->Cancel();
    	delete iTimerContainer;
    	}
	iEntries.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::StartL
// -----------------------------------------------------------------------------
//
TMceTimerId CMceTimerManager::StartL(MMCEExpirationHandler* aObserver,
        TUint aMilliseconds)
    {
    MCESRV_DEBUG("2 CMceTimerManager::StartL, Entry");
    return StartL(aObserver, aMilliseconds, NULL);
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::StartL
// -----------------------------------------------------------------------------
//
TMceTimerId CMceTimerManager::StartL(MMCEExpirationHandler* aObserver,
        TUint aMilliseconds, TAny* aTimerParam)
    {
    MCESRV_DEBUG("CMceTimerManager::StartL, Entry");
    // Do not accept a NULL observer
    if ( !aObserver)
        {
        User::Leave(KErrArgument);
        }

    // Get new unique timer id

    TMceTimerId newTimerId = NewTimerId();
    MCESRV_DEBUG_DVALUES( "timer", newTimerId, "timeout", aMilliseconds );

    // Define a new entry with the new id
    TEntry *newEntry = new ( ELeave ) TEntry();
    CleanupStack::PushL( newEntry );
    
    newEntry->iId = newTimerId;
    newEntry->iObserver = aObserver;
    newEntry->iObserverParam = aTimerParam;
    newEntry->iCallBack = TCallBack( TimerExpiredCallBack, newEntry );
    newEntry->iEntry = TDeltaTimerEntry( newEntry->iCallBack );
    newEntry->iTimerManager = this;

    iEntries.InsertL(newEntry, 0);
    CleanupStack::Pop( newEntry );
    
    TTimeIntervalMicroSeconds interval( TInt64( aMilliseconds ) * KOneMillisAsMicros );

    if (KErrNone != iTimerContainer->QueueLong( interval, newEntry->iEntry ) )
        {
        MCESRV_DEBUG("CMceTimerManager:: CDeltaTimer::QueueLong Error!!!");
        return CMceTimerManager::KNoSuchTimer;
        }

    MCESRV_DEBUG("CMceTimerManager::StartL, Exit");
    return newEntry->iId;

    }

// -----------------------------------------------------------------------------
// CMceTimerManager::Stop
// -----------------------------------------------------------------------------
//
TInt CMceTimerManager::Stop(TMceTimerId aTimerId)
    {
    MCESRV_DEBUG("CMceTimerManager::Stop, Entry");
    MCESRV_DEBUG_DVALUE("timer", aTimerId );

    // Find the timer entry to be stopped
    TInt entryIndex = FindEntry(aTimerId);
    if (entryIndex == KErrNotFound)
        {
        MCESRV_DEBUG("CMceTimerManager::Stop - Invalid TimerId, timer not found");
        return KErrNotFound;
        }

    // Remove timer information from container and delete entry
    TEntry* entry = iEntries[entryIndex];
    iTimerContainer->Remove(entry->iEntry);
    iEntries.Remove(entryIndex);
    delete entry;
    entry = NULL;

    MCESRV_DEBUG("CMceTimerManager::Stop, Exit");
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::IsRunning
// -----------------------------------------------------------------------------
//
TBool CMceTimerManager::IsRunning(TMceTimerId aTimerId) const
    {

    TInt entryIndex = FindEntry(aTimerId);
    if (entryIndex != KErrNotFound)
        {
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::NewTimerId
// -----------------------------------------------------------------------------
//
TMceTimerId CMceTimerManager::NewTimerId()
    {
    // Note that a rolling unsigned 32bit integer id counter
    // is not strictly unique - however, it is not possible
    // to have a timer with id 1 and id 2^32-1 at the same time
    // due to time and memory constraints.
    iTimerIdCounter++;
    if (iTimerIdCounter == KNoSuchTimer)
        {
        iTimerIdCounter++;
        }
    return iTimerIdCounter;
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::FindEntry
// -----------------------------------------------------------------------------
//
TInt CMceTimerManager::FindEntry(TMceTimerId aTimerId) const
    {
    // Search for an entry with a matching id
    TInt count = iEntries.Count();
    for (TInt i = 0; i < count; i++)
        {

        if (iEntries[ i ]->iId == aTimerId)
            {

            return i;

            }
        }

    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::CMceTimerManager
// -----------------------------------------------------------------------------
//
CMceTimerManager::CMceTimerManager() :
    iEntries(KArrayGranularity), iTimerIdCounter(KNoSuchTimer),
            iTimerContainer(NULL)

    {
    }

// -----------------------------------------------------------------------------
// CMceTimerManager::ConstructL
// -----------------------------------------------------------------------------
//
void CMceTimerManager::ConstructL()
    {

    iTimerContainer = CDeltaTimer::NewL(CActive::EPriorityStandard);

    }

// -----------------------------------------------------------------------------
// CMceTimerManager::TimerExpiredCallBack
// -----------------------------------------------------------------------------
//
TInt CMceTimerManager::TimerExpiredCallBack(TAny *aPtr)
    {

    MCESRV_DEBUG("TimerExpiredCallBack, Entry");
    if (aPtr != NULL)
        {

        TEntry* entry = static_cast<TEntry*> (aPtr );
        MCESRV_DEBUG_DVALUE(
                "CMceTimerManager:timer expired - calling observer.timer", entry->iId );

        // Call timer expired function
        TRAP_IGNORE( entry->iObserver->TimerExpiredL( entry->iId,
                        entry->iObserverParam ) );

        // Remove the TEntry pointer from iEntries
        TInt index = entry->iTimerManager->iEntries.Find(entry);
        if (index != KErrNotFound)
            {
            entry->iTimerManager->iEntries.Remove(index);
            }

        // Delete entry
        delete entry;
        return KErrNone;

        }

    MCESRV_DEBUG("TimerExpiredCallBack, Exit");
    return KErrArgument;

    }

// -----------------------------------------------------------------------------
// CMceTimerManager::TEntry::TEntry
// -----------------------------------------------------------------------------
//
CMceTimerManager::TEntry::TEntry() :
    iId(KNoSuchTimer), iObserver( NULL), iObserverParam( NULL), iTimerManager( NULL )
    {
    }

