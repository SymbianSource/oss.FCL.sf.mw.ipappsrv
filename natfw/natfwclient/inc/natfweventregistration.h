/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    A container for event registration functionality
*
*/




#ifndef NATFWEVENTREGISTRATION_H
#define NATFWEVENTREGISTRATION_H

#include "mnatfwconnectivityobserver.h"

/**
 *  The container class for event registration data
 *
 *  @lib natconfw.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( TNATFWEventRegistration )
    {

public: // methods

    inline TNATFWEventRegistration(
        MNATFWConnectivityObserver& aObserver ) :
        iObserver( aObserver ),
        iEvents( 0 )
        {
        }

    inline MNATFWConnectivityObserver& Observer() const
        {
        return iObserver;
        }

    inline TUint16 Events() const
        {
        return iEvents;
        }

// Disabled a PC-Lint notice: Loss of precision. PC-Lint is giving false
// positives involving shift operations with subinteger variables.
/*lint -e734*/
    inline void ProcessRegistrationL( TUint16 aToBeRegistered )
        {
        // Force down to maximum value in case of an overflow
        aToBeRegistered &= MNATFWConnectivityObserver::EAllEvents;
        TUint16 backup( iEvents );
        TUint16 bitcount( 0 );
        do
            {
            if ( aToBeRegistered & 1 )
                {
                // Event requested for registration
                if ( ( iEvents >> bitcount ) & 1 )
                    {
                    // Event already registered, do backup before leaving
                    iEvents = backup;
                    User::Leave( KErrAlreadyExists );
                    }
                // Register event
                iEvents ^= ( 1 << bitcount );
                }

            // Examine the next event
            aToBeRegistered >>= 1;
            ++bitcount;
            } while ( aToBeRegistered );
        }

    inline void ProcessUnregistration( TUint16 aToBeUnregistered )
        {
        // Force down to maximum value in case of an overflow
        aToBeUnregistered &= MNATFWConnectivityObserver::EAllEvents;
        TUint16 bitcount( 0 );
        do
            {
            if ( ( aToBeUnregistered & 1 ) &&
                 ( ( iEvents >> bitcount ) & 1 ) )
                {
                // Unregister event as it is registered
                iEvents ^= ( 1 << bitcount );
                }

            // Examine the next event
            aToBeUnregistered >>= 1;
            ++bitcount;
            } while ( aToBeUnregistered );
        }

private: // data

    MNATFWConnectivityObserver& iObserver;
    TUint16 iEvents;
    };

#endif // NATFWEVENTREGISTRATION_H
