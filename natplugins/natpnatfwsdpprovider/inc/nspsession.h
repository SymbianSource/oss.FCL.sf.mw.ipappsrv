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
* Description:  Session class description.
*
*/

#ifndef NSPSESSION_H
#define NSPSESSION_H

#include <e32base.h>
#include <in_sock.h>
#include "nspplugin.h"
#include "nsppluginreturndef.h"
#include "natfwconnectivityapidefs.h"
#include "mnatfwconnectivityobserver.h"
#include "nspsessiondata.h"
#include "nspdefs.h"
#include "nsputdefinitions.h"

class CDesC8Array;
class CSdpDocument;
class CNATFWCandidate;
class CNATFWCandidatePair;
class MNSPSessionObserver;
class MNSPControllerIF;
class CNSPContentParser;
class CNSPMediaStreamContainer;
class CNSPStateMachine;
class TNSPActionSet;
class TEventReturnStatus;

/**
 *  Session object that handles client requests.
 *
 *  Session encloses all Sdp data in separate object, which is then 
 *  used by state machine object. After state machine completes 
 *  processing, session notifies client via observer callbacks.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPSession : public CBase
    {
public: // Enumerations

    /**
     * The role of session, in signalling.
     */
    enum TSdpRole
        {
        EUndefined,
        EOfferer,
        EAnswerer
        };


public: // Constructors and destructor
    
    /**
     * A two-phase constructor.
     */
    static CNSPSession* NewL( MNSPControllerIF& aController,
            MNSPSessionObserver& aSessionObserver, TUint32 aIapId,
            const TDesC8& aDomain, TUint aProtocol );
    
    /**
     * A two-phase constructor.
     */
    static CNSPSession* NewLC( MNSPControllerIF& aController,
            MNSPSessionObserver& aSessionObserver, TUint32 aIapId,
            const TDesC8& aDomain, TUint aProtocol );
    
    /**
     * Destructor.
     */
    virtual ~CNSPSession();
    

private: // Constructors and destructor

    CNSPSession( MNSPControllerIF& aController, MNSPSessionObserver& aSessionObserver );
    
    void ConstructL( TUint32 aIapId, const TDesC8& aDomain, TUint aProtocol );


public: // New functions

    /**
     * This method is used to create NAT FW modified Sdp content, 
     * based on initially given Sdp offer.
     * 
     * @since       S60 3.2
     * @param       aOffer              Sdp document object, initial Offer.
     */
    TNatReturnStatus CreateOfferL( CSdpDocument*& aOffer );
    
    /**
     * This method is used to create NAT FW modified Sdp content, 
     * Both Offer and Answer, according to received Offer.
     * 
     * @since       S60 3.2
     * @param       aOffer              Sdp document object, initially 
     *                                  received Offer.
     * @param       aAnswer             Sdp document object, initial Answer.
     *                                  ( non NAT FW modified )
     */
    TNatReturnStatus ResolveL( CSdpDocument*& aOffer, CSdpDocument*& aAnswer );
    
    /**
     * This method is used to interpret NAT FW modified Sdp content, 
     * decode Answer based on initially modified Offer.
     * 
     * @since       S60 3.2
     * @param       aAnswer             Sdp document object, Answer to 
     *                                  initial offer.
     */
    TNatReturnStatus DecodeAnswerL( CSdpDocument*& aAnswer );
    
    /**
     * Offer can be updated while ResolveL is still ongoing, i.e.
     * OfferReady callback is not yet received.
     * 
     * @since       S60 3.2
     * @param       aOffer              Sdp offer document
     */
    void UpdateL( CSdpDocument*& aOffer );
    
    /**
     * Session closing must be done in asynchronous manner.
     *
     * @since       S60 3.2
     */
    TNatReturnStatus CloseSessionL();
    
    /**
     * This method is used to mediate NAT FW events to corresponding 
     * handler object.
     * 
     * @since       S60 3.2
     * @param       aStreamId           The ID identifying stream(from NAT FW)
     * @param       aEvent              The event.
     * @param       aError              The completion code of the operation
     * @return      Returns error code if failure in event handling.
     */
    TEventReturnStatus EventOccured( TUint aStreamId,
    		MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
    		TInt aError, TAny* aData );
    
    /**
     * Callback method that is used to inform about updated Sdp Offer, asynchronously.
     * Ownership of the argument is transferred.
     * 
     * @since       S60 3.2
     * @param       aDocument           Sdp document object, updated Offer.
     */
    void UpdateSdpAsyncL( CSdpDocument* aDocument );
    
    /**
     * This method is used to get unique session identifier.
     *
     * @since       S60 3.2
     * @return      The ID identifying stream.
     */
    TUint SessionId() const;
    
    /**
     * This method is used to access peer specific session data.
     *
     * @since       S60 3.2
     * @return      Reference to session data object.
     */
    CNSPSessionData& Data();
    
    /**
     * This method is used to access host specific media stream container.
     *
     * @since       S60 3.2
     * @return      Reference to stream container object.
     */
    CNSPMediaStreamContainer& Container();
    
    /**
     * This method is used to access array containing protocol strings.
     *
     * @since       S60 3.2
     * @return      Reference to descriptor array.
     */
    CDesC8Array& Plugins();
    
    /**
     * Removes all strings from the given array which contain postfix ".ice".
     * 
     * @since       S60 3.2
     * @param       aDesArray           Array of descriptors
     */    
    void RemoveIce( CDesC8Array& aDesArray ) const;
    
    /**
     * Checks if given string contains postfix ".ice".
     * 
     * @since       S60 3.2
     * @param       aProtocol           Protocol string
     */
    TBool IsIce( const TDesC8& aProtocol ) const;

    /**
     * This method is used to access content parsing methods.
     *
     * @since       S60 3.2
     * @return      Reference to content parser object.
     */
    const CNSPContentParser& Parser() const;
    
    /**
     * This method is used to retrieve O/A - role from the session.
     *
     * @since       S60 3.2
     * @return      Current role of the session
     */
    CNSPSession::TSdpRole& Role();
    
    /**
     * This method is used to get session specific identifier.
     *
     * @since       S60 3.2
     * @return      Session observer reference
     */
    MNSPSessionObserver& SessionObserver() const;
    
    /**
     * This method is used to get state machine actions.
     *
     * @since       S60 3.2
     * @return      Action set object, automatic object from stack.
     */
    TNSPActionSet Actions();
    
    /**
     * This method is used to set session specific parameters.
     *
     * @since       S60 3.2
     * @param       aParamKey           Enumeration key identifying parameter.
     * @param       aParamValue         Value of the parameter.
     * @return      status code
     */
    TInt SetSessionParam( CNSPPlugin::TNSPSessionParamKey aParamKey,
            TUint aParamValue );
    
    /**
     * This method is used to set gession specific parameters.
     *
     * @since       S60 3.2
     * @param       aParamKey           Enumeration key identifying parameter.
     * @return      parameter value, or error in failure situations.
     */
    TInt GetSessionParam( CNSPPlugin::TNSPSessionParamKey aParamKey );
    
    /**
     * This method is used to set NAT FW specific flag for session, 
     * specifying which NAT protocol should be used as default, 
     * STUN or ICE/TURN.
     *
     * @since       S60 3.2
     * @param       aUseIce             ETrue if ICE is in use.
     */
    void SetUseIce( TBool aUseIce );



private: // New functions

    void Proceed( TInt aRequest, TNatReturnStatus& aStatus );


private: // data

    /**
     * This reference is needed for informing controller that session was
     * deleted by the user, and hence NAT FW events are no longer acceptable.
     * Not own.
     */
    MNSPControllerIF& iController;
    
    /**
     * Session observer reference.
     * Not own.
     */
    MNSPSessionObserver& iSessionObserver;

    /**
     * Session identifier, received from NAT FW.
     */
    TUint iSessionId;
    
    /**
     * Flag indicating the role of the session, Offerer / Answerer.
     */
    TSdpRole iRole;
    
    /**
     * Session data container.
     * Own.
     */
    CNSPSessionData* iSessionData;
    
    /**
     * Session media stream container.
     * Own.
     */
    CNSPMediaStreamContainer* iStreamContainer;
    
    /**
     * Session state machine, i.e. actions.
     * Own.
     */
    CNSPStateMachine* iStateMachine;
    
    /**
     * Protocols array, strings containing the name of used protocol.
     * Own.
     */
    CDesC8Array* iProtocolsArray;
    
    /**
     * Definitions for unit testing.
     */
    NSP_UT_DEFINITIONS

    };

#define OFFERER( role ) ( CNSPSession::EOfferer == role )
#define ANSWERER( role ) ( CNSPSession::EAnswerer == role )

#endif // NSPSESSION_H

// end of file
