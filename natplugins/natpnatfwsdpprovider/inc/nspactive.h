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
* Description:  Timer objects for NAT FW SDP Provider
*
*/

#ifndef NSPACTIVE_H
#define NSPACTIVE_H

#include <e32base.h>
#include "mnatfwconnectivityobserver.h"

class CSdpDocument;
class CNSPController;

/**
 *  Simple timer for NSP, also initiates UpdateSdp callbacks.
 *
 *  Class implements simple timer that is used as a waiting timer for
 *  FetchCandidate(s)L async NAT FW calls. If expected objects are not
 *  received in time, then session is continued, if possible.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPActive : public CActive
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPActive* NewL( CNSPController& aController,
            TUint aSessionId, TUint aStreamId, TUint aTransactionId,
            MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
            TUint aTimerInMicroSeconds, CSdpDocument* aDocument = NULL );
    
    /**
     * A two-phase constructor.
     */
    static CNSPActive* NewLC( CNSPController& aController,
            TUint aSessionId, TUint aStreamId, TUint aTransactionId,
            MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
            TUint aTimerInMicroSeconds, CSdpDocument* aDocument = NULL );
    
    /**
     * Destructor.
     */
    virtual ~CNSPActive();


private: // Constructors and destructor

    CNSPActive( CNSPController& aController, 
                TUint aSessionId, TUint aStreamId, TUint aTransactionId,
                MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
                CSdpDocument* aDocument = NULL );
    
    void ConstructL( TUint aTimerInMicroSeconds );


public: // From CActive

    /**
     * See base class.
     */
    void RunL();
    
    /**
     * See base class.
     */
    void DoCancel();
    
    /**
     * See base class.
     */
    TInt RunError( TInt aError );


public: // New functions

    /**
     * Returns session identifier, used to map to correct session.
     *
     * @since           S60 3.2
     * @return          The session identifier
     */
    TUint SessionId() const;
    
    /**
     * Returns transaction identifier, used to map to correct transaction.
     *
     * @since           S60 3.2
     * @return          The transaction identifier.
     */
    TUint TransactionId() const;


private: // data

    /**
     * Handle to timer resource
     */
    RTimer iTimer;

    /**
     * Controller object reference
     */
    CNSPController& iController;
    
    /**
     * Session identifier.
     */
    TUint iSessionId;
    
    /**
     * Stream identifier.
     */
    TUint iStreamId;
    
    /**
     * transaction identifier
     */
    TUint iTransactionId;
    
    /**
     * NAT FW event type
     */
    MNATFWConnectivityObserver::TNATFWConnectivityEvent iEvent;
    
    /**
     * Sdp document.
     * Own.
     */ 
    CSdpDocument* iDocument;
    
    };

#endif // NSPACTIVE_H

// end of file
