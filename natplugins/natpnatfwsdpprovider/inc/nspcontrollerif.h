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
* Description:  Interface for session/streams objects. 
*
*/

#ifndef NSPCONTROLLERIF_H
#define NSPCONTROLLERIF_H

#include "natfwconnectivityapidefs.h"
#include "mnatfwconnectivityobserver.h"

class CDesC8Array;
class TInetAddr;
class CSdpDocument;
class MNATFWSocketMediaConnWrapper;
class CNATFWCandidate;
class CNATFWCredentials;
class CNSPContentParser;

/**
 *  Interface for session/streams objects.
 *
 *  Methods available for session/streams objects,
 *  including NAT FW methods.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class MNSPControllerIF
    {
public: // New functions

    /**
     * Content parser object is a part of controller, 
     * used via this method.
     *
     * @since       S60 3.2
     * @return      Reference to a content parser object.
     */
    virtual const CNSPContentParser& ContentParser() = 0;
    
    /**
     * Wrapper for NAT FW API method CreateSessionL
     *
     * @since       S60 3.2
     * @param       aIap                The IAP used for a connection.
     * @param       aDomain             Used domain for settings query.
     * @return      The ID for the created session.
     */
    virtual TUint CreateSessionL( TUint32 aIapId, const TDesC8& aDomain ) = 0;
    
    /**
     * Wrapper for NAT FW API method LoadPluginL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aPlugins            Array containing plugin strings.
     * @param       aPluginIndex        Index of loaded plugin.
     */
    virtual void LoadPluginL( TUint aSessionId, const CDesC8Array& aPlugins,
            TInt& aPluginIndex ) = 0;
    
    /**
     * Wrapper for NAT FW API method CreateStreamL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aProtocol           The protocol to be used.
     * @param       aQoS                The desired quality of service.
     * @return      The ID for the created stream.
     */
    virtual TUint CreateStreamL( TUint aSessionId, 
            TUint aProtocol, TInt aQoS ) = 0;
    
    /**
     * Calls NAT FW API method CreateWrapperL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @return      The interface through which the client can 
     *              configure the wrapper.
     */
    virtual MNATFWSocketMediaConnWrapper& CreateWrapperL(
            TUint aSessionId, TUint aStreamId ) = 0;
    
    /**
     * Wrapper for NAT FW API method FetchCandidateL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     */
    virtual void FetchCandidateL( TUint aSessionId, TUint aStreamId,
            TUint aAddrFamily ) = 0;
    
    /**
     * Wrapper for NAT FW API method FetchCandidatesL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       aCollectionId       The stream collection identifier.
     * @param       aComponentId        The media component identifier.
     */
    virtual void FetchCandidatesL( TUint aSessionId, TUint aStreamId,
            TUint aCollectionId, TUint aComponentId, TUint aAddrFamily ) = 0;
    
    /**
     * Wrapper for NAT FW API method SetReceivingStateL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       aState              The streaming state.
     */
    virtual void SetReceivingStateL( const CNATFWCandidate& aLocalCand,
            TNATFWStreamingState aState ) = 0;
    
    /**
     * Wrapper for NAT FW API method SetSendingStateL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       aDestAddr           The destination address for the media.
     * @param       aPort               The destination port.
     * @param       aState              The streaming state.
     */
    virtual void SetSendingStateL( const CNATFWCandidate& aLocalCand,
            TNATFWStreamingState aState,
            const TDesC8& aDestAddr, TUint aPort ) = 0;
    
    /**
     * Wrapper for NAT FW API method SetRoleL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aRole               The role.
     */
    virtual void SetRoleL( TUint aSessionId, TNATFWIceRole aRole ) = 0;
    
    /**
     * Wrapper for NAT FW API method SetCredentialsL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     * @param       aCredentials        The credentials.
     */
    virtual void SetCredentialsL( TUint aSessionId,
            TUint aStreamId, const CNATFWCredentials& aCredentials ) = 0;
    
    /**
     * Wrapper for NAT FW API method PerformCandidateChecksL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aRemoteCands        The remote candidate array.
     */
    virtual void PerformCandidateChecksL( TUint aSessionId,
            RPointerArray<CNATFWCandidate>& aRemoteCandidates ) = 0;
    
    /**
     * Wrapper for NAT FW API method UpdateIceProcessingL
     * 
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aNewRemoteCands     The new remote candidates array.
     */
    virtual void UpdateIceProcessingL( TUint aSessionId,
            RPointerArray<CNATFWCandidate>& aNewRemoteCands ) = 0;
    
    /**
     * Wrapper for NAT FW API method CloseStreamL
     * 
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aStreamId           The ID identifying stream.
     */
    virtual void CloseStreamL( TUint aSessionId, TUint aStreamId ) = 0;
    
    /**
     * Wrapper for NAT FW API method CloseSessionL
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     */
    virtual void CloseSessionL( TUint aSessionId ) = 0;
    
    /**
     * Creates Unique id, which is used by NAT FW 
     * to map stream together.
     *
     * @since       S60 3.2
     * @return      Unique collection ID.
     */
    virtual TUint CreateUniqueId() = 0;
    
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
    virtual TUint OrderTimeoutL( TUint aSessionId, TUint aStreamId,
        MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
        TUint aTimerInSeconds ) = 0;
    
    /**
     * Orders asyncronous call to session, since all updates must be asynchronous
     * by nature.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aDoc                Sdp document, ownership transferred.
     * @return      Unique timer transaction ID
     */
    virtual TUint OrderUpdateSdpL( TUint aSessionId, CSdpDocument* aDoc ) = 0;
    
    /**
     * Cancels timer transaction, identified by given transaction id.
     * 
     * @since       S60 3.2
     * @param       aTransactionId      Timer transaction Id.
     */
    virtual void Cancel( TUint aTransactionId ) = 0;
    
    /**
     * Removes all strings from the given array which contain postfix ".ice".
     * 
     * @since       S60 3.2
     * @param       aDesArray           Descriptor array, containing protocol strings.
     */
    virtual void RemoveIce( CDesC8Array& aDesArray ) const = 0;
    
    /**
     * Checks if given string contains postfix ".ice".
     * 
     * @since       S60 3.2
     * @param       aProtocol           Protocol string, should be formulated as "nokia.ice".
     */
    virtual TBool IsIce( const TDesC8& aProtocol ) const = 0;
    
    /**
     * Wrapper for Crypto API services.
     *
     * @since       S60 3.2
     * @param       aUsername           Descriptor in which the username is
     *                                  inserted. Max length should be at least 8.
     */
    virtual void GenerateUsernameL( TDes8& aUsername ) = 0;
    
    /**
     * Wrapper for Crypto API services.
     *
     * @since       S60 3.2
     * @param       aPassword           Descriptor in which the password is
     *                                  inserted. Max length should be at least 26.
     */
    virtual void GeneratePasswordL( TDes8& aPassword ) = 0;


    };

#endif // NSPCONTROLLERIF_H

// end of file
