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
* Description:    
*
*/





#ifndef CNATFWCONNECTIONHANDLER_H
#define CNATFWCONNECTIONHANDLER_H


#include <e32base.h>
#include <in_sock.h>
#include "mncmconnectionobserver.h"
#include "natfwstunclientobserver.h"
#include "mncmincomingconnectionobserver.h"
#include "micenatplugineventobs.h"
#include "icecandidatehandlerdefs.h"
#include "natfwcandidate.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class TInetAddr;
class CSTUNClient;
class CSTUNBinding;
class MNcmConnectionMultiplexer;
class CIceSessionData;
class MIceConnCheckObserver;
class MIceConnHandlerObserver;
class TBindingData;
class CIceNatPluginContainer;
class CNATFWCandidatePair;

/**
 *  Hides connection handling related tasks from rest of ICE plugin.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceConnectionHandler : public CBase,
                                public MSTUNClientObserver,
                                public MNcmIncomingConnectionObserver,
                                public MNcmConnectionObserver,
                                public MIceNatPluginEventObs
    {
    
    UNIT_TEST( UT_CIceConnectionHandler )
    
public:

    /** ICE connection type */
    enum TIceConnectionType
        {
        EIceConnTypeHost    = 1,
        EIceConnTypeTurn    = 2
        };
        
    /**
     * Two-phased constructor.
     * @param   aMultiplexer        Multiplexer
     * @param   aEventObserver      Observer for events
     * @param   aSessionData        Session data
     * @param   aPluginContainer    Plugin container
     * @param   aConnHandlerObs     Observer for the connection handler
     */
    static CIceConnectionHandler* NewL(
        MNcmConnectionMultiplexer& aMultiplexer,
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer,
        MIceConnHandlerObserver& aConnHandlerObs );

    /**
     * Two-phased constructor.
     * @param   aMultiplexer        Multiplexer
     * @param   aEventObserver      Observer for events
     * @param   aSessionData        Session data
     * @param   aPluginContainer    Plugin container
     * @param   aConnHandlerObs     Observer for the connection handler
     */
    static CIceConnectionHandler* NewLC(
        MNcmConnectionMultiplexer& aMultiplexer,
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer,
        MIceConnHandlerObserver& aConnHandlerObs );
    
    /**
     * Destructor.
     */
    virtual ~CIceConnectionHandler();

    /**
     * Enables/disables media receiving for the specified local candidate.
     * 
     * @since   S60 v3.2
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The receiving state
     */
    void SetReceivingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState );

    /**
     * Enables/disables media sending from the specified local candidate.
     * 
     * @since   S60 v3.2
     * @param   aLocalCandidate The local end point for a media
     * @param   aState          The sending state
     * @param   aDestAddr       The remote end point for a media
     */        
    void SetSendingStateL( const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr );

    /**
     * Gets identifier for a connection that is associated with the
     * given local end point.
     * 
     * @since   S60 v3.2
     * @param   aLocalCandidate     The local end point for a media
     * @param   aConnectionId       Reference to found id
     */        
    void GetConnectionIdL( const CNATFWCandidate& aLocalCandidate,
        TUint& aConnectionId );     
    
    /**
     * Creates a new connection for the specified candidate pair.
     *
     * @since   S60 3.2
     * @param   aPair               The candidate pair
     * @return  Connection identifier
     */
    TUint CreateConnectionL( const CNATFWCandidatePair& aPair );
    
    /**
     * Removes specified connection.
     *
     * @since   S60 3.2
     * @param   aStreamId           The stream identifier
     * @param   aConnectionId       The connection identifier
     */
    void RemoveConnection( TUint aStreamId, TUint aConnectionId );
    
    /**
     * Performs connectivity check using specified connection.
     * MIceConnectionHandlerObserver::ConnCheckCompleted is
     * called after completion of check.
     *
     * @since   S60 3.2
     * @param   aStreamId       The stream identifier
     * @param   aConnectionId   The connection identifier
     * @param   aPriority       The value of PRIORITY attribute
     * @param   aUseCandidate   Whether to put USE-CANDIDATE to request
     * @param   aRtoValue       The retransmission timeout value in ms
     * @param   aDestAddr       The remote address where to send check
     * @param   aObserver       Connectivity check observer
     */
    void PerformConnCheckL( TUint aStreamId, TUint aConnectionId, 
        TUint aPriority, TBool aUseCandidate, TUint aRtoValue,
        const TInetAddr& aDestAddr,
        MIceConnCheckObserver& aObserver );
    
    /**
     * Cancels ongoing connectivity check.
     *
     * @since   S60 3.2
     * @param   aStreamId           The stream identifier
     * @param   aConnectionId       The connection identifier
     * @param   aDestAddr           Destination address of the check
     */
    void CancelCheck( TUint aStreamId, TUint aConnectionId,
        const TInetAddr& aDestAddr );
    
    /**
     * Starts fetching local(host, server reflexive, relay) candidates.
     *
     * @since S60 3.2
     * @param aStreamCollectionId   Stream collection identifier
     * @param aStreamId             Stream identifier
     * @param aComponentId          Stream collection identifier
     */
    void FetchCandidatesL( TUint aStreamCollectionId, TUint aStreamId, 
        TUint aComponentId );

    /**
     * Releases plugins.
     *
     * @since   S60 3.2
     * @param   aRetainRelay          Whether relay should be kept up
     */
    void ReleasePlugins( TBool aRetainRelay );
    
// from base class MSTUNClientObserver

    /**
     * From MSTUNClientObserver.
     * Indicates that asynchronous initialization of the related
     * CSTUNClient has been completed and it can be used
     * for creating CSTUNBinding(s) or CSTUNRelayBinding(s).
     *
     * @param   aClient the client for which the initialization 
     *          was completed.
     * @param   aCompletionCode value KErrNone indicates that 
     *          the client instance is ready for creating CSTUNBinding(s).
     *          Any other completion code indicates that 
     *          the client instance cannot be used and should be deleted.
     */
    void STUNClientInitCompleted( 
        const CSTUNClient& aClient,
        TInt aCompletionCode );

    /**
     * From MSTUNClientObserver.
     * A STUN/STUN Relay binding related event occurred. After this callback
     * the aBinding can be used again by calling its SendRequestL or 
     * AllocateRequestL function.
     *
     * @param   aEvent the STUN binding related event
     * @param   aBinding the STUN binding that is the target for the event.
     */
    void STUNBindingEventOccurredL( 
        TSTUNBindingEvent aEvent,
        const CBinding& aBinding );

    /**
     * From MSTUNClientObserver.
     * Error occurred during IP address resolution.
     * This callback will also be called if STUNBindingEventOccuredL leaves.
     *
     * @param   aBinding a binding for which the error occured
     * @param   aError error code.
     */
    void STUNBindingErrorOccurred( 
        const CBinding& aBinding,
        TInt aError );

// from base class MNcmConnectionObserver

    void ConnectionNotify( TUint aStreamId, TUint aConnectionId,
        MNcmConnectionObserver::TConnectionNotifyType aType, TInt aError );
        
// from base class MNcmIncomingConnectionObserver

    void IncomingMessageL( TUint aStreamId, const TDesC8& aMessage,
        const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr,
        const TInetAddr& aPeerRemoteAddress, TBool& aConsumed );

// from base class MIceNatPluginEventObs

    void PluginEventOccured( const CNATFWPluginApi* aPlugin, 
        TUint aStreamId, TNatPluginEvent aEventCode, 
        TInt aErrorCode, TAny* aEventData, TUint aStreamConnectionId = 0,
        TUint aComponentId = 0, TBool aIPv6After = EFalse );

private:

    CIceConnectionHandler(
        MNcmConnectionMultiplexer& aMultiplexer,
        MIceNatPluginEventObs& aEventObserver,
        CIceSessionData& aSessionData,
        CIceNatPluginContainer& aPluginContainer,
        MIceConnHandlerObserver& aConnHandlerObs );
    
    void ConstructL();
    
    TInt FindBindingIndex( TUint aStreamId, TUint aConnectionId ) const;
    
    TInt FindBindingIndex( const CBinding& aBinding ) const;
    
    TInt FindBindingIndex( TUint aStreamId, TUint aConnectionId,
        const TInetAddr& aDestAddr ) const;

    CNATFWPluginApi* SolvePluginL( const CNATFWCandidate& aLocalCandidate ); 
    
    void FreeResources();
    
    void ExecuteCallbackL( TInt aIndex, TInt aError );
    
    void DoRemoveConnection( TUint aStreamId, TUint aConnectionId, 
        TBool aRemoveMuxConn );

private: // data

    /**
     * Multiplexer
     * Not own.
     */
    MNcmConnectionMultiplexer& iMultiplexer;
    
    /**
     * ICE plugin observer
     * Not own.
     */
    MIceNatPluginEventObs& iEventObserver;
    
    /**
     * The session data
     */
    CIceSessionData& iSessionData;
    
    /**
     * NAT protocol plugin container
     * Not own.
     */
    CIceNatPluginContainer& iPluginContainer;
    
    /**
     * Notifies about closed streams
     */
    MIceConnHandlerObserver& iConnHandlerObs;
    
    /**
     * Stun client.
     * Own.
     */
    CSTUNClient* iStunClient;
    
    /**
     * Instance for timer services.
     * Own.
     */
    CDeltaTimer* iTimerServices;
    
    /**
     * Binding data array
     */
    RArray<TBindingData> iBindings;
    };

class TBindingData
    {
public:
// parametrized ctor
    inline TBindingData( CSTUNBinding* aBinding, 
        CIceConnectionHandler::TIceConnectionType aConnType,
        TUint aStreamId, TUint aConnectionId, 
        const TInetAddr& aDestAddr ) :
        iBinding( aBinding ), 
        iConnType( aConnType ),
        iStreamId( aStreamId ), 
        iConnectionId( aConnectionId ), iDestAddr( aDestAddr ),
        iObserver( NULL ),
        iStreamingStatus( EStreamingStatePassive ),
        iLocalAddr( KAFUnspec ), iFromAddr( KAFUnspec ),
        iPeerAddr( KAFUnspec ), iMappedAddr( KAFUnspec ),
        iCheckOngoing( EFalse ), iRtoValue( 0 ), iCancelled( EFalse )
        {
        }

public:
    
    /**
     * Stun binding instance.
     * Own.
     */
    CSTUNBinding* iBinding;
    
    /**
     * Connection type.
     */
    CIceConnectionHandler::TIceConnectionType iConnType;

    /**
     * ID of used stream.
     */
    TUint iStreamId;
    
    /**
     * ID of used connection.
     */
    TUint iConnectionId;
    
    /**
     * Destination address, where to send.
     */
    TInetAddr iDestAddr;
    
    /**
     * For notifying client.
     */
    MIceConnCheckObserver* iObserver;
    
    /**
     * Stream status.
     */
    TNATFWStreamingState iStreamingStatus;
    
    /**
     * local address.
     */
    TInetAddr iLocalAddr;
    
    /**
     * Where message came from ( can be relay address ).
     */
    TInetAddr iFromAddr;
    
    /**
     * Peer address ( not relay, but real peer address ).
     */
    TInetAddr iPeerAddr;

    /**
     * Result of binding request, public ip address.
     */
    TInetAddr iMappedAddr;
    
    /**
     * Indicates whether check is in progress for this binding.
     */
    TBool iCheckOngoing;
    
    /**
     * Retransmission timeout for the binding.
     */
    TUint iRtoValue;

    /**
     * Indicates that check is cancelled.
     */    
    TBool iCancelled;
    };
    

#endif // CNATFWCONNECTIONHANDLER_H
