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
* Description:  Container for NAT FW streams, class description.
*
*/

#ifndef NSPMEDIASTREAMCONTAINER_H
#define NSPMEDIASTREAMCONTAINER_H

#include <e32base.h>
#include <badesca.h> 
#include "nsppluginreturndef.h"
#include "mnatfwconnectivityobserver.h"
#include "natfwconnectivityapidefs.h"
#include "nsputdefinitions.h"

class TInetAddr;
class CSdpDocument;
class CSdpMediaField;
class CNATFWCredentials;
class MNSPControllerIF;
class CNSPContentParser;
class CNSPMediaStream;
class CNSPMediaStreamComponent;

/**
 *  Objects created from this class contain all NATFW related objects.
 *
 *  These objects work as all around interface for all NATFW prosessing.    
 *  State machine uses this object to initiate async calls for NATFW, 
 *  which then causes NATFW callbacks are sent to here.
 *	
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPMediaStreamContainer : public CBase
    {
public: // Constructors and destructor
    
    /**
     * A two-phase constructor.
     */
    static CNSPMediaStreamContainer* NewL( MNSPControllerIF& aController,
            TUint aSessionId, TUint aProtocol );
    
    /**
     * A two-phase constructor.
     */
    static CNSPMediaStreamContainer* NewLC( MNSPControllerIF& aController,
            TUint aSessionId, TUint aProtocol );
    
    /**
     * Destructor.
     */
    virtual ~CNSPMediaStreamContainer();


private: // Constructors and destructor

    CNSPMediaStreamContainer( MNSPControllerIF& aController,
            TUint aSessionId, TUint aProtocol );

    
public: // New functions
	
	/**
	 * Notifies media stream about NATFW event.
	 *
	 * @since       S60 3.2
	 * @param       aStreamId           The ID identifying stream.
	 * @param       aEvent              The event.
	 * @param       aError              The completion code of the operation.
	 * @param		aData				Event specific data.
	 * @return      Returns status code, i.e. global error code,
	 * 				KNatReady or KNatAsync.
	 */
	TNatReturnStatus ControlMediaL( TUint aStreamId,
			MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
			TInt aError, TAny* aData );
    
    /**
     * This method is used to update host-peer candidate pairs while 
     * candidate checks are ongoing.
     *
     * @since       S60 3.2
     * @param       aLocalCands         Sdp document containing local candidates.
     * @param       aRemoteCands        Sdp document containing remote candidates.
     */
    void UpdateL( RPointerArray<CNATFWCandidate>& aRemoteCands );
    
    /**
     * This method is used to setup ICE role to NATFW. Role is
     * stream specific, hence it is handled as FetchCandidates, but without
     * state transitions. This method is ICE specific.
     * 
     * @since       S60 3.2
     * @param       aRole               Role enumeration.
     */
    void SetRoleL( TNATFWIceRole aRole );
    
    /**
     * This method is used to setup inbound ICE credentials to NATFW. Credentials are
     * stream specific. This method is ICE specific.
     * 
     * @since       S60 3.2
     */
    void SetInboundCredentialsL();
    
    /**
     * This method is used to setup outbound ICE credentials to NATFW. Credentials are
     * stream specific. This method is ICE specific.
     * 
     * @since       S60 3.2
     */
    void SetOutboundCredentialsL();
    
    /**
     * This method is used to Reset and Destroy local candidates from
     * individual stream components.
     * 
     * @since       S60 3.2
     */
    void ResetAndDestroyCandidates();
    
    /**
     * This method is used to load approriate plugin in NATFW.
     * 
     * @since       S60 3.2
     * @param       aPlugins            Array containing protocol strings.
     * @param       aPluginIndex        Reference that is used to return
     *                                  loaded plugin index.
     */
    void LoadPluginL( CDesC8Array& aPlugins, TInt& aPluginIndex );
    
    /**
     * This method is part of services that are given for session state machine.
     * Method initiates local candidate solving, by FetchCandidate(s)L call.
     * This method is STUN specific.
     * 
     * @since       S60 3.2
     */
    void FetchCandidateL();
    
    /**
     * This method is part of services that are given for session state machine.
     * Method initiates local candidate solving, by FetchCandidates call.
     * This method is ICE specific.
     * 
     * @since       S60 3.2
     */
    void FetchCandidatesL();
    
    /**
     * This method is part of services that are given for session state machine.
     * Method initiates candidate pair resolving. This method is ICE specific.
     * 
     * @since       S60 3.2
     * @param       aRemoteCandidates   Remote candidates object array.
     */
    void PerformCandidateChecksL(
            RPointerArray<CNATFWCandidate>& aRemoteCandidates );
    
    /**
     * This method is part of services that are given for session state machine.
     * Method initiates stream activation for NATFW. This method is STUN specific.
     * 
     * @since       S60 3.2
     * @param       aDocument           Sdp document object, connection info in this
     *                                  object is used as peer transport address.
     */
    void ActivateL( CSdpDocument& aDocument );
    
    /**
     * This method is part of services that are given for session state machine.
     * Method initiates stream activation for NATFW. This method is ICE specific.
     * Peer transport address in ICE situation is received from NATFW.
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
     * This method is used to create media streams from media field
     * streams in given SDP.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     */
    TNatReturnStatus CreateStreamsL( CSdpDocument& aDoc );
    
    /**
     * This method is used to remove existing media streams.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     */
    TNatReturnStatus RemoveStreamsL( CSdpDocument& aDoc );
    
     /**
     * This method is used to remove existing media streams.
     * 
     * @since       S60 3.2
     * @param       aStream                nspmediastream.
     */
    TInt  RemoveStream(CNSPMediaStream* aStream);
    
    /**
     * This method is used to modify Sdp document so that Mobile originating
     * data from NATFW is changed to m/c - lines.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     */
    void ModStunSolvedL( CSdpDocument& aDoc );
    
    /**
     * This method is used to modify Sdp document so that Mobile originating
     * data from NATFW is changed to m/c - lines.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     */
    void ModIceSolvedL( CSdpDocument& aDoc );
    
    /**
     * This method is used to modify Sdp document so that Mobile terminating
     * data from NATFW is changed to m/c - lines.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     */
    void ModLocalSolvedL( CSdpDocument& aDoc );
    
    /**
     * This method is used to add ICE specific attributes to media field.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     */
    void AddIceContentL( CSdpDocument& aDoc );
    
    /**
     * This method is used to add ICE specific remote-candidates - attribute
     * to media field. Used only in final updated offer.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     */
    void AddRemoteCandidatesL( CSdpDocument& aDoc );
    
    /**
     * This method is used to get ICE candidates from media field.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     * @param       aRemoteCandidates   Array where received candidates are inserted.
     * @return      Nat status, if asynchronous processing needed,
     *              KNatAsync is returned. Otherwise KNatReady, or error code.
     */
    TNatReturnStatus GetCandidatesL( CSdpDocument& aDoc,
            RPointerArray<CNATFWCandidate>& aRemoteCandidates );
    
    /**
     * This method is used to get ICE identification from media field, or
     * from session level.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     * @param       aSessionlevel       Reference to a credentials object.
     * @return      Nat status, if asynchronous processing needed,
     *              KNatAsync is returned. Otherwise KNatReady.
     */
    TNatReturnStatus GetCredentialsL( CSdpDocument& aDoc,
            CNATFWCredentials& aSessionlevel );
    
    /**
     * This method is used to check whether peers remote transport address
     * has changed in latter O/A exchange.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     * @return      Nat status, if asynchronous processing needed,
     *              KNatAsync is returned. Otherwise KNatReady.
     */
    TNatReturnStatus GetSessionConnInfoL( CSdpDocument& aDoc );
    
    /**
     * This method is used to check if the initial Offer must be updated,
     * since Ice concluded on different transport addresses than DEFAULT
     * values in initial Offer.
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     * @return      New sdp document if update is needed, else NULL.
     */
    CSdpDocument* CheckUpdateL( CSdpDocument& aDoc );
    
    /**
     * This method is used to setup Type-of-service field for media sockets.
     * Parameter is given to the NATFW in stream creation.
     * 
     * @since       S60 3.2
     * @param       aMediaTos           Type-of-service value, valid range [0,63].
     * @return      KErrArgument returned if argument was not in valid range.
     *              Otherwise KErrNone returned.
     */
    TInt SetMediaTos( TUint aMediaTos );
    
    /**
     * This method is used to get Type-of-service field value.
     * 
     * @since       S60 3.2
     * @return      Type-of-service value, valid range [0,63].
     */
    TUint MediaTos() const;


private: // New functions
    
    TNatReturnStatus RemovePending( TUint aComponentId, TBool aRemove );
    
    TInt FindMediaStream( TUint aStreamId );
    
    TInt FindCollection( TUint aCollectionId );


private: // data

    /**
     * Reference to a Ice Sdp Controller, a wrapper for NATFW API.
     * Not own.
     */ 
    MNSPControllerIF& iController;
    
    /**
     * Session unique identifier.
     */
    TUint iSessionId;

    /**
     * Used protocol.
     */
    TUint iProtocol;

    /**
     * Media type-of-service
     */
    TInt iMediaTos;

    /**
     * Array containing media streams collection Ids, that are pending for
     * NATFW event.
     * Own.
     */
    RArray<TUint> iPendingArray;

    /**
     * Array containing Media streams.
     * Own.
     */
    RPointerArray<CNSPMediaStream> iStreamArray;

    /**
     * Definitions for unit testing.
     */
    NSP_UT_DEFINITIONS
    
    };

#endif // NSPMEDIASTREAMCONTAINER_H

// end of file
