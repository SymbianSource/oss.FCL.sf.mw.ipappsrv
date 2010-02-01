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
* Description:  Controller class description.
*
*/

#ifndef NSPCONTROLLER_H
#define NSPCONTROLLER_H

#include "mnatfwconnectivityobserver.h"
#include "nspcontrollerif.h"
#include "nsputdefinitions.h"

class CNATFWConnectivityApi;
class CNSPSession;
class CNSPActive;
class MNSPSessionObserver;
class CDesC8Array;
class TEventReturnStatus;

/**
 * Class implements NAT FW observer, and routes async events to sessions.
 * 
 * Main purpose of controller object is to route async events to correct
 * session/stream objects, which handle those locally. In addition,
 * controller introduces a set of auxiliary methods, which are available
 * for all session/stream objects.
 *
 * @lib natfwsdpprovider.dll
 * @since S60 3.2
 */
class CNSPController : public CBase,
                       public MNATFWConnectivityObserver,
                       public MNSPControllerIF
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPController* NewL();
    
    /**
     * A two-phase constructor.
     */
    static CNSPController* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNSPController();


private: // Constructors and destructor

    CNSPController();
    
    void ConstructL();


public: // From MNATFWConnectivityObserver

    /**
     * Notifies the client of NAT Connectivity FW events.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session
     * @param       aStreamId           The ID identifying stream
     * @param       aEvent              The event
     * @param       aError              The completion code of the operation
     * @param       aEventData          Optional event specific data
     */
    void EventOccured( TUint aSessionId, TUint aStreamId,
            TNATFWConnectivityEvent aEvent, TInt aError, TAny* aEventData );
    

public: // From MNSPControllerIF

    /**
     * Content parser object is a part of controller, used via this method.
     *
     * @since       S60 3.2
     * @return      Reference to a content parser object.
     */
    const CNSPContentParser& ContentParser();
    
    /**
     * Wrapper for NAT FW API method CreateSessionL
     *
     * @since       S60 3.2
     * @param       aIap                The IAP used for a connection.
     * @param       aDomain             Used domain for settings query.
     * @return      The ID for the created session.
     */
    TUint CreateSessionL( TUint32 aIapId, const TDesC8& aDomain );
    
    /**
     * Wrapper for NAT FW API method LoadPluginL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aPlugins            Array containing plugin strings.
     * @param       aPluginIndex        Index of loaded plugin.
     */
    void LoadPluginL( TUint aSessionId, const CDesC8Array& aPlugins,
            TInt& aPluginIndex );
    
    /**
     * Wrapper for NAT FW API method CreateStreamL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aProtocol           The protocol to be used.
     * @param       aQoS                The desired quality of service.
     * @return      The ID for the created stream.
     */
    TUint CreateStreamL( TUint aSessionId, TUint aProtocol, TInt aQoS );
    
    /**
     * Calls NAT FW API method CreateWrapperL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @return      The interface through which the client can 
     *              configure the wrapper.
     */
    MNATFWSocketMediaConnWrapper& CreateWrapperL(
            TUint aSessionId, TUint aStreamId );
    
    /**
     * Wrapper for NAT FW API method FetchCandidateL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     */
    void FetchCandidateL( TUint aSessionId, TUint aStreamId,
            TUint aAddrFamily );
    
    /**
     * Wrapper for NAT FW API method FetchCandidatesL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       aCollectionId       The stream collection identifier.
     * @param       aComponentId        The media component identifier.
     */
    void FetchCandidatesL( TUint aSessionId, TUint aStreamId,
            TUint aCollectionId, TUint aComponentId, TUint aAddrFamily );
    
    /**
     * Wrapper for NAT FW API method SetReceivingStateL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       aState              The streaming state.
     */
    void SetReceivingStateL( const CNATFWCandidate& aLocalCand,
            TNATFWStreamingState aState );
    
    /**
     * Wrapper for NAT FW API method SetSendingStateL
     * 
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       aDestAddr           The destination address for the media.
     * @param       aPort               The destination port for the media.
     * @param       aState              The streaming state.
     */
    void SetSendingStateL( const CNATFWCandidate& aLocalCand,
            TNATFWStreamingState aState, const TDesC8& aDestAddr, TUint aPort );
    
    /**
     * Wrapper for NAT FW API method SetRoleL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aRole               The role.
     */
    void SetRoleL( TUint aSessionId, TNATFWIceRole aRole );
    
    /**
     * Wrapper for NAT FW API method SetCredentialsL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       aCredentials        The credentials.
     */
    void SetCredentialsL( TUint aSessionId, TUint aStreamId,
            const CNATFWCredentials& aCredentials );
    
    /**
     * Wrapper for NAT FW API method PerformCandidateChecksL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aRemoteCands        The remote candidate array.
     */
    void PerformCandidateChecksL( TUint aSessionId,
            RPointerArray<CNATFWCandidate>& aRemoteCandidates );
    
    /**
     * Wrapper for NAT FW API method UpdateIceProcessingL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aNewRemoteCands     The new remote candidates array.
     */
    void UpdateIceProcessingL( TUint aSessionId,
            RPointerArray<CNATFWCandidate>& aNewRemoteCands );
    
    /**
     * Wrapper for NAT FW API method CloseStreamL
     * 
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     */
    void CloseStreamL( TUint aSessionId, TUint aStreamId );
    
    /**
     * Wrapper for NAT FW API method CloseSessionL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     */
    void CloseSessionL( TUint aSessionId );
    
    /**
     * Creates Unique id, which is used by NAT FW 
     * to map stream together.
     *
     * @since       S60 3.2
     * @return      Unique collection ID.
     */
    TUint CreateUniqueId();
    
    /**
     * Certains NAT FW Actions execution time is guarded, since network conditions
     * can cause events to be delayed. Hence Notify functionality is simulated by
     * timers.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       eEvent              The event from NAT FW.
     * @param       aTimerInSeconds     The timer value, in seconds.
     * @return      Unique timer transaction ID
     */
    TUint OrderTimeoutL( TUint aSessionId, TUint aStreamId,
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
        TUint aTimerInMicroSeconds );
    
    /**
     * Orders asyncronous call to session, since all updates must be asynchronous
     * by nature.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aDoc                Sdp document, ownership transferred.
     * @return      Unique timer transaction ID
     */
    TUint OrderUpdateSdpL( TUint aSessionId, CSdpDocument* aDoc );
    
    /**
     * Cancels timer transaction, identified by given transaction id.
     * 
     * @since       S60 3.2
     * @param       aTransactionId      Timer transaction Id.
     */
    void Cancel( TUint aTransactionId );
    
    /**
     * Removes all strings from the given array which contain postfix ".ice".
     * 
     * @since       S60 3.2
     * @param       aDesArray           Timer transaction Id.
     */
    void RemoveIce( CDesC8Array& aDesArray ) const;
    
    /**
     * Checks if given string contains postfix ".ice".
     * 
     * @since       S60 3.2
     * @param       aProtocol           Timer transaction Id.
     */
    TBool IsIce( const TDesC8& aProtocol ) const;
    
    /**
     * Wrapper for Crypto API services.
     *
     * @since       S60 3.2
     * @param       aUsername           Descriptor in which the username is
     *                                  inserted. Max length should be at 
     *                                  least 8.
     */
    void GenerateUsernameL( TDes8& aUsername );
    
    /**
     * Wrapper for Crypto API services.
     *
     * @since       S60 3.2
     * @param       aPassword           Descriptor in which the password is
     *                                  inserted. Max length should be at 
     *                                  least 26.
     */
    void GeneratePasswordL( TDes8& aPassword );


public: // New functions

    /**
     * Implements plugin interface 'NewSessionL' method.
     *
     * @since       S60 3.2
     * @param       aSessionObserver    Session observer, for async callbacks.
     * @param       aIapId              Internet access point identifier.
     * @param       aDomain             Used domain for settings query.
     * @param       aProtocol           Used transport protocol.
     * @return      The ID identifying session.
     */
    TUint NewSessionL( MNSPSessionObserver& aSessionObserver,
            TUint32 aIapId, const TDesC8& aDomain, TUint aProtocol );
    
    /**
     * Implements plugin interface 'CloseSessionL' method.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     */
    void ClosingSessionL( TUint aSessionId );
    
    /**
     * Used by all non-initializing methods to map session Id to a object.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @return      Reference to the object.
     */
    CNSPSession& FindSessionObjectL( TUint aSessionId );


private: // New functions

    TInt FindSession( TUint aSessionId );
    
    TInt FindClosingSession( TUint aSessionId );
    
    TInt FindActiveObject( TUint aTransactionId );
    
    void RemoveActiveObjects( TUint aSessionId );
    
    TUint8 RandomByteL() const;
    
    TBool UseIceL( CDesC8Array& aArray, const TDesC8& aDomain ) const;
    
    void ReplaceArrayL( CDesC8Array& aDesArray, const TDesC8& aString ) const;
    
    void Callback( const CNSPSession& aSession, TEventReturnStatus& aCallback ) const;
    
    void ExecuteInitialized( const CNSPSession& aSession ) const;
    
    void ExecuteOfferReady( const CNSPSession& aSession, CSdpDocument* aOffer ) const;
    
    void ExecuteAnswerReady( const CNSPSession& aSession, CSdpDocument* aAnswer ) const;
    
    void ExecuteUpdateSdp( const CNSPSession& aSession, CSdpDocument* aOffer ) const;
    
    void ExecuteErrorOccurred( const CNSPSession& aSession, TInt aError ) const;


private: // data

    /**
     * Pointer to the NAT FW - instance.
     * Own.
     */
    CNATFWConnectivityApi* iInterface;
    
    /**
     * Pointer to Content parser object, used by all sessions.
     * Own.
     */
    CNSPContentParser* iContentParser;
    
    /**
     * Array for session pointers.
     * Own.
     */
    RPointerArray<CNSPSession> iSessionArray;
    
    /**
     * Array for session pointers.
     * Own.
     */
    RPointerArray<CNSPSession> iClosingSessionArray;
    
    /**
     * Array containing pointers to active objects.
     * Own.
     */
    RPointerArray<CNSPActive> iActiveObjects;
    
    /**
     * Class of active objects, objects can remove themself from
     * above array.
     */
    friend class CNSPActive;
    
    /**
     * Base number for unique Id, randomly selected in construction.
     */
    TUint iBase;
    
    /**
     * Definitions for unit testing.
     */
    NSP_UT_DEFINITIONS
    
    };


#endif // NSPCONTROLLER_H
