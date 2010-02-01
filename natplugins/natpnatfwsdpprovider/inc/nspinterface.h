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
* Description:  Plugin interface implementation, class description.
*
*/

#ifndef NSPINTERFACE_H
#define NSPINTERFACE_H

#include "nspplugin.h"
#include "nsputdefinitions.h"

class CNSPController;

/**
 *  NAT FW Sdp plugin interface implementation.
 *
 *  This class implements 'CNSPPlugin' ECOM interface.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPInterface : public CNSPPlugin
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPInterface* NewL();
    
    /**
     * Destructor.
     */
    virtual ~CNSPInterface();


private: // Constructors and destructor

    CNSPInterface();
    
    void ConstructL();


public: // From CNSPPlugin

    /**
     * Creates session object, and returns a handle to that object.
     * Client of the class uses services via this handle.
     *
     * @since       S60 3.2
     * @param       aSessionObserver    Observer for created session, 
     *                                  for async callbacks.
     * @param       aIapId              Internet access point identifier, unique.
     * @param       aProtocol           Used transport protocol, values are:
     *                                  1 = TCP
     *                                  2 = UDP
     * @return      Session identifier, via this handle session is used.
     */
    TUint NewSessionL( MNSPSessionObserver& aSessionObserver,
            TUint32 aIapId, const TDesC8& aDomain, TUint aProtocol );
    
    /**
     * With this call NAT FW session is terminated.
     *
     * @since       S60 3.2
     * @param       aSessionId          Session unique identier.
     */
    void CloseSessionL( TUint aSessionId );
    
    /**
     * Sets the value of a session parameter.
     * 
     * @param       aSessionId          The ID identifying session.
     * @param       aParamKey           identifies the parameter.
     * @param       aParamValue         value of the parameter.
     * @return      KErrNotFound if the key is not found or
     *              other system wide error code on failure.
     */
    TInt SetSessionParam( TUint aSessionId, TNSPSessionParamKey aParamKey,
            TUint aParamValue );
    
    /**
     * Gets the value of a session parameter.
     * 
     * @param       aSessionId          The ID identifying session.
     * @param       aParamKey           identifies the parameter.
     * @return      value of the parameter or
     *              KErrNotFound if the value is not available or
     *              other system wide error code on failure.
     */
    TInt GetSessionParam( TUint aSessionId, TNSPSessionParamKey aParamKey );
    
    /**
     * Initial NAT FW specific SDP content added in here.
     *
     * @since       S60 3.2
     * @param       aSessionId          Session unique identier.
     * @param       aOffer              Sdp offer document
     */
    TNatReturnStatus CreateOfferL( TUint aSessionId, CSdpDocument*& aOffer );
    
    /**
     * NAT FW specific SDP content is added to both Offer and Answer.
     *
     * @since       S60 3.2
     * @param       aSessionId          Session unique identier.
     * @param       aOffer              Sdp offer document
     * @param       aAnswer             Sdp answer document
     */
    TNatReturnStatus ResolveL( TUint aSessionId,
            CSdpDocument*& aOffer, CSdpDocument*& aAnswer );
    
    /**
     * Answer to initial Answer interpred in here.
     *
     * @since       S60 3.2
     * @param       aSessionId          Session unique identier.
     * @param       aAnswer             Sdp answer document.
     */
    TNatReturnStatus DecodeAnswerL( TUint aSessionId, CSdpDocument*& aAnswer );
    
    /**
     * Offer can be updated while ResolveL is still ongoing, i.e.
     * OfferReady callback is not yet received.
     *
     * @since       S60 3.2
     * @param       aSessionId          Session unique identier.
     * @param       aOffer              Sdp offer document.
     */
    void UpdateL( TUint aSessionId, CSdpDocument*& aOffer );
    
    /**
     * Empty INVITE, with certain NAT protocols, can be interpret as request
     * to restart connection checks. This method must be called if such
     * a message is received.
     *
     * @since       S60 3.2
     * @param       aSessionId          Session unique identier.
     * @return      NAT status, if async signaling expected.
     */
    TNatReturnStatus RestartL( TUint aSessionId );


private: // data

    /**
     * Controller object, which is used to find sessions and check Sdp.
     * Own.
     */
    CNSPController* iController;
    
    /**
     * Definitions for unit testing.
     */
    NSP_UT_DEFINITIONS

    };

#endif // NSPINTERFACE_H

// end of file
