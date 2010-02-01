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
* Description:  Session Sdp related storage, class description.
*
*/

#ifndef NSPSESSIONDATA_H
#define NSPSESSIONDATA_H

#include <e32base.h>
#include "natfwconnectivityapidefs.h"

class CSdpDocument;
class CSdpConnectionField;
class CNATFWCandidate;
class CNATFWCredentials;

/**
 *  Data container for all Sdp related data.
 *
 *  All session related Sdp data is stored in here, 
 *  for later use.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPSessionData : public CBase
    {
public: // Enumerations

    /**
     * Substate signals special situation in session peer.
     */
    enum TSubState
        {
        ESubNone,
        ESubMismatch,
        ESubRemoteLite,
        ESubAll
        };

public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPSessionData* NewL();
    
    /**
     * A two-phase constructor.
     */
    static CNSPSessionData* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNSPSessionData();


private: // Constructors and destructor

    CNSPSessionData();
    
    void ConstructL();


public: // New functions

    /**
     * Sdp offer stored here for state machine to use.
     * 
     * @since       S60 3.2
     * @param       aOffer              Sdp offer document, ownership changed.
     */
    void SetOffer( CSdpDocument* aOffer );
    
    /**
     * Sdp offer used in state machine via this method.
     * 
     * @since       S60 3.2
     * @return      Pointer to Sdp offer document, ownership not changed.
     */
    CSdpDocument& Offer();
    
    /**
     * Sdp offer from the data container is released with this method.
     * 
     * @since       S60 3.2
     * @return      Pointer to Sdp offer document, ownership changed.
     */
    CSdpDocument* ReleaseOffer();
    
    /**
     * Sdp answer stored here for state machine to use.
     * 
     * @since       S60 3.2
     * @param       aAnswer             Sdp offer document, ownership changed.
     */
    void SetAnswer( CSdpDocument* aAnswer );
    
    /**
     * Sdp answer used in state machine via this method.
     * 
     * @since       S60 3.2
     * @return      Pointer to Sdp answer document, ownership not changed.
     */
    CSdpDocument& Answer();
    
    /**
     * Sdp answer from the data container is released with this method.
     * 
     * @since       S60 3.2
     * @return      Pointer to Sdp offer document, ownership changed.
     */
    CSdpDocument* ReleaseAnswer();
    
    /**
     * Used to set role flag, either Controlling or Controlled.
     *
     * @since       S60 3.2
     * @param       aRole      Role enumeration.
     */
    void SetRole( TNATFWIceRole aRole );
    
    /**
     * Used to check role flag.
     *
     * @since       S60 3.2
     * @return      The role enumeration.
     */
    TNATFWIceRole Role() const;
    
    /**
     * Used to set session substate.
     *
     * @since       S60 3.2
     * @return      Reference to the sub state
     */
    void SetSubState( TSubState aSubState );
    
    /**
     * Used to access session substate.
     *
     * @since       S60 3.2
     * @return      Reference to the sub state
     */
    TSubState SubState() const;
    
    /**
     * Used to set flag incidicating whether to use Ice Sdp grammar.
     *
     * @since       S60 3.2
     * @param       aUseIce             Flag indicating Ice support.
     */
    void SetUseIce( TBool aUseIce );
    
    /**
     * Used to check flag incidicating whether to use Ice Sdp grammar.
     *
     * @since       S60 3.2
     * @return      ETrue if Ice grammar is supported.
     */
    TBool UseIce() const;
    
    /**
     * Returns a reference to array, which has all remote candidates for
     * this session.
     *
     * @since       S60 3.2
     * @return      Reference to array, which contains all remote candidates.
     */
    RPointerArray<CNATFWCandidate>& RemoteCandidates();
    
    /**
     * Returns session level credentials, in a credentials - object.
     *
     * @since       S60 3.2
     * @return      Session credentials reference
     */
    CNATFWCredentials& Credentials();
    
    /**
     * Initial Sdp in O/A.
     *
     * @since       S60 3.2
     * @return      Local Sdp document reference, ownership not changed.
     */
    CSdpDocument& UpdatedSdp();
    
    /**
     * Initial Sdp stored here incase updated O/A is needed.
     *
     * @since       S60 3.2
     * @param       aDocument           Sdp document object, ownership changed.
     */
    void SetUpdatedSdp( CSdpDocument* aDocument );
    
    /**
     * Release initial local Sdp
     * 
     * @since		S60 3.2
     * @return		Local Sdp document pointer, ownership changed.
     */
    CSdpDocument* ReleaseUpdatedSdp();
    
    
private: // data

    /**
     * Pointer to a SDP Offer.
     * Own.
     */
    CSdpDocument* iOffer;
    
    /**
     * Pointer to a SDP Answer.
     * Own.
     */
    CSdpDocument* iAnswer;
    
    /**
     * Flag indicating whether this terminal is controlling, or controlled.
     */
    TNATFWIceRole iRole;
    
    /**
     * Sub state which is used signal special situation in session peer.
     */
    TSubState iSubState;
    
    /**
     * Flag indicating whether to Ice grammar in SDP.
     */
    TBool iUseIce;
    
    /**
     * Array containing remote candidates received from O/A exchange.
     * Own.
     */
    RPointerArray<CNATFWCandidate> iRemoteCandidates;
    
    /**
     * Object containing peer session level credentials.
     * Own.
     */
    CNATFWCredentials* iCredentials;
    
    /**
     * Local Sdp that is used incase ICE protocol requires updated 
     * O/A exchange. Own.
     */
    CSdpDocument* iUpdatedSdp;

    };

#endif // NSPSESSIONDATA_H

// end of file
