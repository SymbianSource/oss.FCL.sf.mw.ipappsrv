/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Handler for message observers on multiplexer
*
*/




#ifndef C_CNCMCONNECTIONOBSERVERHANDLER_H
#define C_CNCMCONNECTIONOBSERVERHANDLER_H

#include <e32base.h>

#include "ncmconnectionmultiplexer.hrh"
#include "natfwconnectivityapidefs.h"

class MNcmConnectionObserver;
class MNcmIncomingConnectionObserver;
class MNcmOutgoingConnectionObserver;
class MNcmMessageObserver;
class TInetAddr;
class CNcmMediaSource;
class MNcmSourceObserver;

/**
 *  Handler for connection observers on multiplexer
 *
 *  Allows registration of connection observers and revealing messages for
 *  those. Additionally connection notifications can be provided for observers
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmConnectionObserverHandler ) : public CBase
    {

public:

    static CNcmConnectionObserverHandler* NewL();

    static CNcmConnectionObserverHandler* NewLC();

    virtual ~CNcmConnectionObserverHandler();

    /**
     * Registers object to observe outgoing connections
     *
     * @since   S60 v3.2
     * @param   aOutgoingConnectionObserver     Observer to be registered
     * @return  void
     */
    void RegisterOutgoingConnectionObserverL(
        const MNcmOutgoingConnectionObserver& aOutgoingConnectionObserver );

    /**
     * Registers object to observe incoming connections
     *
     * @since   S60 v3.2
     * @param   aIncomingConnectionObserver     Observer to be registered
     * @return  void
     */
    void RegisterIncomingConnectionObserverL(
        const MNcmIncomingConnectionObserver& aIncomingConnectionObserver );

    /**
     * Removes object from observing outgoing connections
     *
     * @since   S60 v3.2
     * @param   aOutgoingConnectionObserver   Observer to be unregistered
     * @return void
     */
    void UnregisterOutgoingConnectionObserverL(
        const MNcmOutgoingConnectionObserver& aOutgoingConnectionObserver );

    /**
     * Removes object from observing incoming connections
     *
     * @since   S60 v3.2
     * @param   aIncomingConnectionObserver     Observer to be unregistered
     * @return void
     */
    void UnregisterIncomingConnectionObserverL(
        const MNcmIncomingConnectionObserver& aIncomingConnectionObserver );

    /**
     * Registers object to observe connection notifies.
     *
     * @since   S60 v3.2
     * @param   aConnectionObserver     Observer to be registered
     * @return  void
     */
    void RegisterConnectionObserverL(
        const MNcmConnectionObserver& aConnectionObserver );

    /**
     * Removes object from observing connection notifies.
     *
     * @since   S60 v3.2
     * @param   aConnectionObserver     Message observer to be unregistered
     * @return  void
     */
    void UnregisterConnectionObserverL(
        const MNcmConnectionObserver& aConnectionObserver );


    /**
     * Registers an observer for receive incoming and outgoing message
     * notifications. All notifications are offered first to this observer. 
     * There can be only one registered observer at a time.
     * Message observer is allowed to make modifications to offered
     * messages. 
     *
     * @since   S60 v3.2
     * @param   aMessageObserver       Message observer to be registered
     * @return  void
     */
    void RegisterMessageObserver( MNcmMessageObserver& aMessageObserver );

    /**
     * Unregisters an observer for receiving incoming and outgoing message
     * notifications.
     *
     * @since S60 v3.2
     * @param aMessageObserver  Observer to be unregistered
     * @return void
     */
    void UnregisterMessageObserver(
        const MNcmMessageObserver& aMessageObserver );
        
    /**
     * Reveal incoming message for observing objects and see if they
     * consumed it
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aMessage            Message is a reference to heap buffer
     *                              which can be rewritten by message
     *                              observers
     * @param   aLocalAddr          A local address.
     * @param   aFromAddr           A remote source address.
     * @return  ETrue if observer found message useful,
     *          EFalse otherwise
     */
    TBool RevealIncomingMessageL( TUint aStreamId, HBufC8 *&aMessage,
        const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr );

    /**
     * Reveal outgoing message for observing objects and see if they consumed
     * it
     *
     * @since   S60 v3.2
     * @param   aStreamId           The ID identifying stream
     * @param   aConnectionId       The ID identifying connection
     * @param   aMessage            Message is a reference to heap buffer
     *                              which can be rewritten by message
     * @param   aDestinationAddress Destination address
     * @return  ETrue if observer found message useful,
     *          EFalse otherwise
     */
    TBool RevealOutgoingMessageL( TUint aStreamId, TUint aConnectionId,
        HBufC8 *&aMessage, const TInetAddr& aDestinationAddress );

    /**
     * Make notify to observing objects
     * it
     *
     * @since S60 v3.2
     * @param aStreamId             The ID identifying stream
     * @param aConnectionId         The ID identifying connection
     * @param aType                 Type of notify
     * @param aError                Error code
     * @return void
     */
    void ConnectionNotify( TUint aStreamId, TUint aConnectionId,
        TMultiplexerConnectionNotifyType aType, TInt aError );

    /**
     * Register media source
     *
     * @since S60 v3.2
     * @param aMediaSource             The media source
     * @return void
     */       
    void RegisterMediaSourceL( CNcmMediaSource* aMediaSource );

    /**
     * Register media source observer
     *
     * @since S60 v3.2
     * @param aMediaSourceObserver      The media source observer
     * @return void
     */      
    void RegisterMediaSourceObserver(
        MNcmSourceObserver& aMediaSourceObserver );
    
    /**
     * Unregister media source observer
     *
     * @since S60 v3.2
     * @param aMediaSourceObserver      The media source observer
     * @return void
     */      
    void UnregisterMediaSourceObserver(
        const MNcmSourceObserver& aMediaSourceObserver ); 

    /**
     * Enables/disables receiving
     *
     * @since S60 v3.2
     * @param aState      The receiving state
     * @return void
     */
     void SetReceivingState( TNATFWStreamingState aState );
               
private:

    CNcmConnectionObserverHandler();
    
    TBool IsDuplicatePacket( const TDesC8& aMessage, TBool& aIsStunMessage );

private: // data

    /**
     * Messaging observers for outgoing connections
     */
    RPointerArray<MNcmOutgoingConnectionObserver> iOutgoingObservers;
    
    /**
     * Messaging observers for incoming connections
     */
    RPointerArray<MNcmIncomingConnectionObserver> iIncomingObservers;

    /**
     * Messaging observers for connection notifies.
     */
    RPointerArray<MNcmConnectionObserver> iConnectionObservers;

    /**
     * Media source
     * Own.
     */
    CNcmMediaSource* iMediaSource;  
    
    /**
     * Message observer.
     * Not own.
     */
    MNcmMessageObserver* iMessageObserver;
    
    /**
     * Hashes calculated from received packets. Used to filter replicated
     * packets.
     */
    RArray<TUint> iLatestHashes;
    };

#endif // C_CNCMCONNECTIONOBSERVERHANDLER_H
