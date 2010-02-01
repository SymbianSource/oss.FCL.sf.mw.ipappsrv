/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Sends keepalive messages to STUN Client in order to keep
*                binding in STUN Server alive
*
*/




#ifndef C_NATFWSTUNREFRESHTIMER_H
#define C_NATFWSTUNREFRESHTIMER_H

#include <e32base.h>

class MNATFWRefreshObserver;

/**
 *  Sends keepalive messages to STUN Client.
 *
 *  Sends keepalive messages to STUN Client in order to keep
 *  binding in STUN Server alive. Gets interval in microseconds from 
 *  CNATFWStunConnectionHandler.
 *
 *  @lib stunplugin.lib
 *  @since S60 v3.2
 */
class CNATFWStunRefreshTimer : public CActive
    {
public:

    /**
     * Creates a new instance of NAT FW STUN Refresh Timer.
     *
     * @since S60 v3.2
     * @param aObserver Observer for callbacks to timer's user
     * @return A new instance, ownership is transferred.
     */
    static CNATFWStunRefreshTimer* NewL( MNATFWRefreshObserver& aObserver );

    /**
     * Creates a new instance of NAT FW STUN Refresh Timer and
     * pushes it to CleanupStack.
     *
     * @since S60 v3.2
     * @param aObserver Observer for callbacks to timer's user
     * @return A new instance, ownership is transferred.
     */
    static CNATFWStunRefreshTimer* NewLC( MNATFWRefreshObserver& aObserver );

    ~CNATFWStunRefreshTimer();

    /**
     * Waits for given time (aInterval) and calls SetActive().
     * If interval is 0, timer is cancelled.
     *
     * @since S60 v3.2
     * @param aInterval Interval in microseconds
     */
    void StartStunRefresh( TUint aInterval );
    
    /**
     * Client can query if refresh is going on.
     *
     * @since S60 v3.2
     * @return ETrue if refreshing
     */
    TBool IsRunning() const;
    
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

    CNATFWStunRefreshTimer( MNATFWRefreshObserver& aObserver );

    void ConstructL();

private: // data

    /**
     * Interval in microseconds
     */
    TUint iInterval;

    /**
     * Asynchronous timer service.
     */
    RTimer iTimer;

    /**
     * MNATFWRefreshObserver reference.
     */
    MNATFWRefreshObserver& iObserver;

    };

#endif // C_NATFWSTUNREFRESHTIMER_H
