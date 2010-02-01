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
* Description:   
*
*/




#ifndef M_NATFWCONNECTIVITYOBSERVER_H
#define M_NATFWCONNECTIVITYOBSERVER_H

#include <e32base.h>

class CNATFWCandidatePair;
class CNATFWCandidate;

/**
 *  Observer interface for clients to receive events from NATFW.
 *
 *  @lib natconfw.lib
 *  @since S60 v3.2
 */
class MNATFWConnectivityObserver
    {

public:

    /**  NAT connectivity framework events. */
    enum TNATFWConnectivityEvent
        {
        /** Completing event for CreateSessionL. */
        ESessionCreated         = 0x0001,

        /** This event is triggered when a new local candidate has been
        found. Eventdata is an instance of CNATFWCandidate class. Ownership
        of the candidate is transferred. */
        ELocalCandidateFound    = 0x0002,

        /** Completing event for FetchCandidate(s). */
        EFetchingCompleted      = 0x0004,

        /** Completing event for SetReceivingStateL. */
        EReceivingActivated     = 0x0008,

        /** Completing event for SetReceivingStateL. */
        EReceivingDeactivated   = 0x0010,

        /** Completing event for SetSendingStateL. */
        ESendingActivated       = 0x0020,

        /** Completing event for SetSendingStateL. */
        ESendingDeactivated     = 0x0040,

        /** This event is triggered when a new working ICE candidate pair
        has been found. Eventdata is an instance of CNATFWCandidatePair class.
        Ownership of the candidate pair is transferred.*/
        ECandidatePairFound     = 0x0080,

        /** Completing event for PerformConnectivityChecksL. */
        EConnChecksCompleted    = 0x0100,

        /** This event is triggered when an error within a stream has occured
        and no asynchnorous operation is ongoing. Error during asynchronous
        operation is reported by completing that operation with error code.*/
        EGeneralError           = 0x0200,

        /** Defines all the supported events. Can only be used for registering
        and unregistering observers for events.*/
        EAllEvents              = 0x03ff

        };

    /**
     * Notifies about NAT connectivity framework events.
     *
     * @since   S60 v3.2
     * @param   aSessionId  The session identifier
     * @param   aStreamId   The stream identifier
     * @param   aEvent      The event
     * @param   aError      The error code
     * @param   aEventData  Optional event data specified by particular event
     */
    virtual void EventOccured( TUint aSessionId, TUint aStreamId,
        TNATFWConnectivityEvent aEvent, TInt aError, TAny* aEventData ) = 0;
    };

#endif // M_NATFWCONNECTIVITYOBSERVER_H
