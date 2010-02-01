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




#ifndef C_NATFWCONNECTIONHANDLER_H
#define C_NATFWCONNECTIONHANDLER_H

#include <e32base.h>
#include "natfwconnectivityapidefs.h"

class TInetAddr;
class CNATFWPluginApi;
class MNcmConnectionMultiplexer;
class MNATFWPluginObserver;
class CNATFWCandidate;
class CNATFWCandidatePair;
class RTimer;

/**
 *  Implements NAT Plug-in interface
 *
 *  The client uses this interface to send STUN requests. Forwards
 *  client requests to STUN Connection Handler.
 *
 *  @lib stunplugin.lib
 *  @since S60 v3.2
 */
class CNATFWConnectionHandler : public CActive
    {

public:

    /**
     * Creates a new instance of NAT FW Plug-in class.
     *
     * @since S60 v3.2
     * @param aInitParams Initial parameters that will be given to
     *                    STUN Connection Handler class.
     * @return A new instance, ownership is transferred.
     */
    static CNATFWConnectionHandler* NewL( MNATFWPluginObserver* aPluginObserver,
        CNATFWPluginApi& aPlugin );


    virtual ~CNATFWConnectionHandler();

    /**
     * FetchCandidate
     *
     * @since S60 v3.2
     * @param
     * @return void
     */
    void PluginInitializeL( TUint32 aIapId, const TDesC8& aDomain,
        MNcmConnectionMultiplexer& aMultiplexer );


    /**
     * FetchCandidate
     *
     * @since S60 v3.2
     * @param aStreamId Stream ID
     * @return void
     */
    void FetchCandidateL( TUint aStreamId );

    
    /**
     * FetchCandidates
     *
     * @since S60 v3.2
     * @return void
     */
    void FetchCandidatesL( TUint aStreamId );
    
    /**
     * PerformConnectivityChecksL
     *
     * @since S60 v3.2
     * @return void
     */
    void PerformConnectivityChecksL();
    
    /**
     * Connect
     *
     * @since S60 v3.2
     * @return void
     */
    void ConnectL();

    /**
     * MNATFWPluginObserver::Notify is called (returned) with stream's
     * current sending status.
     *
     * @since   S60 3.2
     * @param   aStreamId   The stream identifier
     * @param   aState      The streaming status
     * @return  void
     */
    void SetReceivingStateL( TUint aStreamId,
        TNATFWStreamingState aState );
        
    /**
     * MNATFWPluginObserver::Notify is called (returned) with stream's
     * current sending status.
     *
     * @since   S60 3.2
     * @param   aStreamId   The stream identifier
     * @param   aDestAddr   The address where to send media
     * @param   aState      The streaming status
     * @return  void
     */
    void SetSendingStateL( TUint aStreamId,
                            const TInetAddr& aDestAddr,
                            TNATFWStreamingState aState );

// From CActive

    void RunL();
    
    void DoCancel();

private:

    CNATFWConnectionHandler( MNATFWPluginObserver* aPluginObserver,
        CNATFWPluginApi& aPlugin );

    void ConstructL();
    

private: // data
    
    enum TNATFWTestPlugInStates
        {
        EIdle = 0,
        EFetchCandidate,
        EFetchCandidates,
        EConnectivityChecks,
        ESetReceivingState,
        ESetSendingState,
        EErrorTestAddress
        };
    
    TNATFWTestPlugInStates iState;
   
    /**
     * IapId
     */
    TUint iIapId;
    
    /**
     * Domain name
     * own.
     */
    HBufC8* iDomain;
    
    /**
     * Not own.
     */
    MNcmConnectionMultiplexer* iConnMux;
    
    /**
     * Sends notifications to NAT Plug-in observer.
     * Not own.
     */
    MNATFWPluginObserver* iPluginObserver;
    
    /**
     * Candidate
     * own.
     */
    CNATFWCandidate* iCandidate;
    
    /**
     * Candidate
     * own.
     */
    CNATFWCandidatePair* iCandidatePair;
    
    /**
     * Timer
     */
    RTimer iTimer;
    
    /**
     * StreamId
     */
    TUint iStreamId;
    
    /**
     * FetchQueue
     */   
    TUint iFetchQueue;
    
    TNATFWStreamingState iStreamingStatus;
    
    CNATFWPluginApi& iPlugin;
    };

#endif // C_NATFWCONNECTIONHANDLER_H
