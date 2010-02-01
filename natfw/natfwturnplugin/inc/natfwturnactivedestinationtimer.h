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
* Description:    Is used to time actions related to setting Active
*                Destination request.
*
*/




#ifndef C_NATFWTURNACTIVEDESTINATIONHTIMER_H
#define C_NATFWTURNACTIVEDESTINATIONHTIMER_H

#include <e32base.h>

class MNATFWTimerObserver;

/**
 *  Active Destination Timer class for TURN plug-in.
 *
 *  Triggers actions required with setting Active Destination request.
 *  Gets interval in microseconds from CNATFWTurnConnectionHandler.
 *
 *  @lib turnplugin.lib
 *  @since S60 v3.2
 */
class CNATFWTurnActiveDestinationTimer : public CActive
    {

public:

    /**
     * Creates a new instance of NAT FW TURN Active Destination Timer.
     *
     * @since S60 v3.2
     * @param aObserver Observer for callbacks to timer's user
     * @return A new instance, ownership is transferred.
     */
    static CNATFWTurnActiveDestinationTimer* NewL(
        MNATFWTimerObserver& aObserver );
        
    /**
     * Creates a new instance of NAT FW TURN Active Destination Timer
     * pushes it to CleanupStack.
     *
     * @since S60 v3.2
     * @param aObserver Observer for callbacks to timer's user
     * @return A new instance, ownership is transferred.
     */
    static CNATFWTurnActiveDestinationTimer* NewLC(
        MNATFWTimerObserver& aObserver );

    ~CNATFWTurnActiveDestinationTimer();

    /**
     * Waits for given time (aInterval) and calls SetActive().
     * If interval is 0, timer is cancelled.
     *
     * @since S60 v3.2
     * @param  aInterval        Interval in microseconds
     * @param  aStreamId        To identify the binding that the
     *                          timer triggering is related to
     * @param  aConnectionId    To identify the binding that the
     *                          timer triggering is related to
     */
    void StartTimer( TUint32 aInterval, TUint aStreamId,
        TUint aConnectionId );

protected:

    /**
     * from CActive
     */
    void RunL();
    
    /**
     * from CActive
     */
    void DoCancel();

private:

    CNATFWTurnActiveDestinationTimer( MNATFWTimerObserver& aObserver );

    void ConstructL();

private: // data

    /**
     * Asynchronous timer service.
     */
    RTimer iTimer;

    /**
     * MNATFWTimerObserver reference.
     */
    MNATFWTimerObserver& iObserver;

    /**
     * Stream ID.
     */
    TUint iStreamId;

    /**
     * Connection ID.
     */
    TUint iConnectionId;

    };

#endif // C_NATFWTURNACTIVEDESTINATIONHTIMER_H
