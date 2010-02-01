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
* Description:  Media stream class description
*
*/

#ifndef NSPMEDIASTREAM_H
#define NSPMEDIASTREAM_H

#include "nsppluginreturndef.h"
#include "natfwconnectivityapidefs.h"
#include "mnatfwconnectivityobserver.h"
#include "nsputdefinitions.h"

class CSdpMediaField;
class CNATFWCandidate;
class CNATFWCandidatePair;
class CNATFWCredentials;
class MNSPControllerIF;
class CNSPMediaStreamComponent;

/**
 *  Class that is used to describe m-line from the Sdp.
 *
 *  Media line specific operations and data are within this class. 
 *  Media stream components, that correspond NATFW stream, are owned by
 *  objects of this type. Sdp modifications are handled by methods
 *  introduced in this class, since each stream component must be handled
 *  separately.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPMediaStream : public CBase
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPMediaStream* NewL( MNSPControllerIF& aController,
            CSdpMediaField& aMediaField, TUint aSessionId, TUint aCollectionId,
            const TDesC8& aAddress, TUint aProtocol, TUint aMediaTos );
    
    /**
     * A two-phase constructor.
     */
    static CNSPMediaStream* NewLC( MNSPControllerIF& aController,
            CSdpMediaField& aMediaField, TUint aSessionId, TUint aCollectionId,
            const TDesC8& aAddress, TUint aProtocol, TUint aMediaTos );
    
    /**
     * Destructor.
     */
    virtual ~CNSPMediaStream();


private: // Constructors and destructor

    CNSPMediaStream( MNSPControllerIF& aController, TUint aSessionId,
            TUint aCollectionId );
    
    void ConstructL( CSdpMediaField& aMediaField, const TDesC8& aAddress,
            TUint aProtocol, TUint aMediaTos );


public: // New functions

    /**
     * This method is used to query from media stream object if it owns
     * media stream component that has corresponding stream id.
     *
     * @since       S60 3.2
     * @param       aStreamId           The ID identifying stream.
     * @return      ETrue if media stream owns the corresponding component.
     */
    TBool HasMediaComponent( TUint aStreamId );
    
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
     * This method is used to replace given media field IP/port values,
     * for RTP and RTCP. Replaced values are NATFW's external
     * transport addresses, where peer can eventually try to connect.
     * This method is STUN specific.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     * @param       aAddress            Address from DEFAULT local candidate.
     */
    void ModifyStunConnL( CSdpMediaField& aMediaField, TDes8& aAddress ) const;
    
    /**
     * This method is used to replace given media field IP/port values,
     * for RTP and RTCP. Replaced values are NATFW's external
     * transport addresses, where peer can eventually try to connect.
     * This method is ICE specific.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     * @param       aAddress            Transport address from ICE solved
     *                                  candidate pair, from local candidate.
     */
    void ModifyIceConnL( CSdpMediaField& aMediaField, TDes8& aAddress ) const;
    
    /**
     * This method is used to replace given media field IP/port values,
     * for RTP and RTCP. Replaced values are NATFW's local transport 
     * addresses, i.e. loopback address and local port. In these addresses
     * host application can send real time media, so that media is routed
     * through NAT FW to correct peer.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     * @param       aAddress            Transport address from conn. wrapper
     */
    void ModifyLocalConnL( CSdpMediaField& aMediaField, TDes8& aAddress ) const;
    
    /**
     * This method is used to check media transport addresses, in order to initiate
     * remote address update to NATFW, if needed.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     * @param       aAddress            Transport address from session level,
     * 									if present.
     */
    TNatReturnStatus MediaConnInfoL( CSdpMediaField& aMediaField,
            const TDesC8& aAddress ) const;
    
    /**
     * This method adds candidates attributes to media field.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     */
    void AddLocalCandidatesL( CSdpMediaField& aMediaField ) const;
    
    /**
     * This method gets candidate attributes from media field,
     * with attached candidate objects.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     * @param       aRemoteCandidates   Candidate array, owner of new
     *                                  candidate objects.
     */
    void GetCandidatesL( CSdpMediaField& aMediaField,
            RPointerArray<CNATFWCandidate>& aRemoteCandidates ) const;
    
    /**
     * This method adds remote-candidates attribute to media field.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     */
    void AddRemoteCandidatesL( CSdpMediaField& aMediaField ) const;
    
    /**
     * This method is used set Ice credentials to Sdp media field.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     */
    void AddCredentialsL( CSdpMediaField& aMediaField ) const;
    
    /**
     * This method is used retrieve all local candidates from media stream,
     * from all its media stream components. Pointers in given array are 
     * NOT TO BE DELETED, ownership is not transferred.
     * 
     * @since       S60 3.2
     * @param       aLocalcand          Array where pointers are inserted.
     */
    void LocalCandidatesL( RPointerArray<CNATFWCandidate>& aLocalcand ) const;
    
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
     * @param       aMediaField         Sdp media field object.
     * @param       aAddress            address where sending is
     *                                  actived. Ports are retrieved from
     *                                  media field object.
     */
    void ActivateL( CSdpMediaField& aMediaField, const TDesC8& aAddress );
    
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
     * Getter method for Collection ID.
     *
     * @since       S60 3.2
     * @return      The stream collection identifier.
     */
    TUint CollectionId() const;
    
    /**
     * Getter method for media field object, used in comparision.
     *
     * @since       S60 3.2
     * @return      Reference to media field object.
     */
    const CSdpMediaField& MediaField() const;
    
    /**
     * This method is used to initiate SetCredentials - call to NATFW.
     * Credentials is own by this object, both inbound and outbound.
     *
     * @since       S60 3.2
     */
    void SetInboundCredentialsL();

    /**
     * This method is used to initiate SetCredentials - call to NATFW.
     * Credentials is own by this object, both inbound and outbound.
     *
     * @since       S60 3.2
     */
    void SetOutboundCredentialsL();
    
    /**
     * This method is used to store Outbound credentials.
     *
     * @since       S60 3.2
     * @param       aOutboundCredentials    Outbound credentials object.
     */
    void SetOutboundCredentials( CNATFWCredentials* aOutboundCredentials );
    
    /**
     * This method is used to check is updated Sdp O/A exchange is needed.
     *
     * @since       S60 3.2
     * @return      ETrue if this media stream requires update.
     */
    TBool NeedToUpdateL();
    
    /**
     * This method is used to reset and destroy local candidates array's.
     *
     * @since       S60 3.2
     */
    void ResetAndDestroyCandidates();


private: // New methods

    TNatReturnStatus RemovePending( TUint aStreamId, TBool aRemove );
    
    void AttachCandidatesL( RPointerArray<CNATFWCandidate>& aRemoteCandidates ) const;
    
    TInt FindMediaComponent( TUint aStreamId );


private: // data

    /**
     * Reference to a NATFW API.
     * Not own.
     */ 
    MNSPControllerIF& iController;
    
    /**
     * Local media description.
     * Own.
     */
    CSdpMediaField* iMediaField;
    
    /**
     * Local Credentials, media stream specific.
     * Own.
     */
    CNATFWCredentials* iInboundCredentials;
    
    /**
     * Remote Credentials, media stream specific.
     * Own.
     */
    CNATFWCredentials* iOutboundCredentials;
    
    /**
     * Session unique identifier.
     */ 
    TUint iSessionId;
    
    /**
     * Collection unique identifier, generated locally, used by NATFW.
     */ 
    TUint iCollectionId;
    
    /**
     * Array for media stream components, that are pending NATFW event.
     * Own.
     */
    RArray<TUint> iPendingArray;
    
    /**
     * Array for media stream components.
     * Own.
     */
    RPointerArray<CNSPMediaStreamComponent> iStreamComponentArray;
    
    /**
     * Definitions for unit testing.
     */
    NSP_UT_DEFINITIONS
    
    };

#endif // NSPMEDIASTREAM_H

// end of file
