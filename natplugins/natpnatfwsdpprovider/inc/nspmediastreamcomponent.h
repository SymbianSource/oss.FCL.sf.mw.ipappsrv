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
* Description:  Media stream component class description
*
*/

#ifndef NSPMEDIASTREAMCOMPONENT_H
#define NSPMEDIASTREAMCOMPONENT_H

#include "nsppluginreturndef.h"
#include "mnatfwconnectivityobserver.h"
#include "natfwconnectivityapidefs.h"
#include "nsputdefinitions.h"

class MNSPControllerIF;
class MNATFWSocketMediaConnWrapper;
class CNSPNATFWAsyncState;
class CNATFWCredentials;

/**
 *  Objects of this type corresponds NATFW's stream component.
 *
 *  Objects of this type handle, and use, NATFW events and interface.
 *  Calls from session state machine traverse to here, where each object
 *  determines actions according to state of individual stream component.
 *	
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPMediaStreamComponent : public CBase
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPMediaStreamComponent* NewL( MNSPControllerIF& aController,
            TUint aSessionId, TUint aCollectionId, TUint aComponentId,
            const TDesC8& aAddress, TUint aPort, TUint aProtocol, TUint aMediaTos );
    
    /**
     * A two-phase constructor.
     */
    static CNSPMediaStreamComponent* NewLC( MNSPControllerIF& aController,
            TUint aSessionId, TUint aCollectionId, TUint aComponentId,
            const TDesC8& aAddress, TUint aPort, TUint aProtocol, TUint aMediaTos );
    
    /**
     * Destructor.
     */
    virtual ~CNSPMediaStreamComponent();


private: // Constructors and destructor

    CNSPMediaStreamComponent( MNSPControllerIF& aController,
            TUint aSessionId, TUint aCollectionId, TUint aComponentId );
    
    void ConstructL( const TDesC8& aAddress, TUint aPort, TUint aProtocol, TUint aMediaTos );


public: // New functions

	/**
	 * Notifies media stream about NATFW event.
	 * 
	 * @since       S60 3.2
	 * @param       aEvent              The event
	 * @param       aError              The completion code of the operation
	 * @param		aData				Event specific data.
	 * @return      Returns status code, i.e. global error code,
	 * 				KNatReady or KNatAsync.
	 */
	TNatReturnStatus ControlMediaL(
			MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
			TInt aError, TAny* aData );
    
    /**
     * This method is used to reset and destroy local candidate objects.
     *
     * @since       S60 3.2
     */
    void ResetAndDestroyCandidates();
    
    /**
     * This method is used to initiate FetchCandidate - call to NATFW.
     * Method is STUN specific, only one candidate needed.
     * 
     * @since       S60 3.2
     */
    void FetchCandidateL();
    
    /**
     * This method is used to initiate FetchCandidates - call to NATFW.
     * Method is ICE specific.
     * 
     * @since       S60 3.2
     */
    void FetchCandidatesL();
    
    /**
     * This method is used to initiate PerformCandidateChecks - call
     * to NATFW. Method is ICE specific.
     * 
     * @since       S60 3.2
     */
    void PerformCandidateChecksL();
    
    /**
     * This method is used to initiate SetReceiving/SendingStatus - calls
     * to NATFW. Method is STUN specific.
     * 
     * @since       S60 3.2
     * @param       aAddress            Remote address as descriptor.
     */
    void ActivateL( const TDesC8& aAddress, TUint aPort );
    
    /**
     * This method is used to initiate SetReceiving/SendingStatus - calls
     * to NATFW. Method is ICE specific.
     * 
     * @since       S60 3.2
     */
    void ActivateL();
    
    /**
     * This method is used to initiate SetReceiving/SendingStatus - calls
     * to NATFW, to deactivate sending/receiving.
     * 
     * @since       S60 3.2
     */
    void DeActivateL();
    
    /**
     * Getter method for Peers remote address.
     */
    const TDesC8& RemoteAddress() const;
    
    /**
     * Getter method for Peers remote port.
     */
    TUint RemotePort() const;
    
    /**
     * Getter method for Stream ID.
     * 
     * @since       S60 3.2
     * @return      The stream collection identifier.
     */
    TUint StreamId() const;
    
    /**
     * Getter method for Component ID. Values are: RTP = 1, RTCP = 2
     *
     * @since       S60 3.2
     * @return      The stream collection identifier.
     */
    TUint ComponentId() const;
    
    /**
     * This method is used to retrieve a reference to local candidates array,
     * which is used to enter directly to individual elements.
     *
     * @since       S60 3.2
     * @return      Reference to array that contains stream component
     *              local candidates.
     */
    RPointerArray<CNATFWCandidate>& LocalCandidates();
    
    /**
     * This method is used to receive the result of ICE negotiation, the 
     * local candidate from working candidate pair.
     *
     * @since       S60 3.2
     * return       Reference to candidate object.
     */
    const CNATFWCandidate& IceLocalCandidateL();
    
    /**
     * This method is used to receive the result of ICE negotiation, the
     * remote candidate from working candidate pair.
     *
     * @since       S60 3.2
     * return       Reference to candidate object.
     */
    const CNATFWCandidate& IceRemoteCandidateL();
    
    /**
     * This method is used to get NATFW adresses.
     *
     * @since       S60 3.2
     * @param       aAddress              NATFW local addr.
     * @param       aPort                 NATFW local port
     * @return      Returns error code if failure address handling.
     */
    TInt OutgoingAddress( TDes8& aAddress, TUint& aPort ) const;
    
    /**
     * This method is used to set Ice credentials to NATFW. Method is 
     * Ice specific.
     *
     * @since       S60 3.2
     * @param       aCredentials              Credentials object.
     */
    void SetCredentialsL( const CNATFWCredentials& aCredentials );
    
    /**
     * This method is used to map component id to a certain stream id.
     * This method is ICE specific.
     *
     * @since       S60 3.2
     * @param       aCandidate          Candidate object reference.
     * @return      ETrue if component ids matched.
     */
    TBool AttachCandidateL( CNATFWCandidate& aCandidate ) const;


private: // New functions

    TNatReturnStatus HandleReceivingActivated( TInt aError );
    
    TNatReturnStatus HandleSendingActivated( TInt aError );
    
    TNatReturnStatus HandleReceivingDeActivated( TInt aError );
    
    TNatReturnStatus HandleSendingDeActivated( TInt aError );
    
    TNatReturnStatus HandleFetchingCompleted( TInt aError );
    
    TNatReturnStatus HandleConnChecksCompleted( TInt aError );
    
    TNatReturnStatus Continue();
    
    TNatReturnStatus ErrorOccurred( TInt aError );
    
    TBool SuppressRtcpIcmp( TInt aError ) const;


private: // data

    /**
     * Reference to a NATFW API.
     * Not own.
     */
    MNSPControllerIF& iController;
    
    /**
     * Peers transport connection address, stored for conn. updates.
     */
    RBuf8 iRemoteAddress;
    
    /**
     * Peers transport connection port, stored for conn. updates.
     */
    TUint iRemotePort;
    
    /**
     * Reference to a Session unique identifier.
     */ 
    TUint iSessionId;
    
    /**
     * Collection unique identifier, generated locally, used by NAT FW.
     */
    TUint iCollectionId;
    
    /**
     * Component Id, separating RTP and RTCP streams from each others.
     */
    TUint iComponentId;
    
    /**
     * Stream unique identifier, received from NAT FW.
     */ 
    TUint iStreamId;
    
    /**
     * Async call timer id, used only with FetchCandidates - call.
     */
    TUint iTimerTransactionId;
    
    /**
     * Default address family as TUint.
     */
    TUint iAddrFamily;
    
    /**
     * Media socket connection wrapper, used to inform NAT FW about IP:port.
     * Not own.
     */
    MNATFWSocketMediaConnWrapper* iWrapper;
    
    /**
     * Final candidate-pear pair, only one received from NAT FW.
     * if differs from NULL, then Own.
     */
    CNATFWCandidatePair* iCandidatePair;
    
    /**
     * Array containing local candidate objects, received from NAT FW.
     * Own.
     */
    RPointerArray<CNATFWCandidate> iLocalCandidates;
    
    /**
     * State engine for this stream, updated always in async calls.
     * Own.
     */
    CNSPNATFWAsyncState* iState;
    
    /**
     * Definitions for unit testing.
     */
    NSP_UT_DEFINITIONS

    };

#endif // NSPMEDIASTREAMCOMPONENT_H

// end of file
