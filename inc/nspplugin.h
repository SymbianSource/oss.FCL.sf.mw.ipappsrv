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
* Description:    Plugin interface description.
*
*/



#ifndef NSPPLUGIN_H
#define NSPPLUGIN_H

#include <e32base.h>
#include "nsppluginreturndef.h"

class CSdpDocument;
class MNSPSessionObserver;

/** Interface UID of this ECOM interface */
const TUid KNSPPluginIFUid = { 0x102832F0 };

/**
 *  Plugin interface.
 *
 *  ECOM Plugin interface.
 *
 *  @lib
 *  @since S60 3.2
 */
class CNSPPlugin : public CBase
    {
public: // Enumerations

    /** Session parameter keys */
    enum TNSPSessionParamKey
        {
        ENSPResourseReservationStatusKey=0, /*read-only*/
        ENSPMediaTypeOfServiceKey=1
        };

    /** NAT traversal resource reservation status */
    enum TNSPResourseReservationStatus
        {
        ENSPResourcesNotReserved=0,
        ENSPResourcesReserved=1
        };

public: // Constructors and destructor

    /**
     * Ecom plugin initialization.
     */
    static inline CNSPPlugin* NewL( TUid aUid );
    
    /**
     * Default destructor. Any remaining NAT sessions are deleted.
     */
    inline ~CNSPPlugin();


public: // New functions

    /**
     * Creates session object, and returns a handle to that object.
     * Client of the class uses services via this handle.
     *
     * @since       S60 3.2
     * @param       aSessionObserver    Observer for created session,
     *                                  for async callbacks.
     * @param       aIapId              Internet access point identifier.
     * @param       aDomain             Used domain for settings query.
     * @param       aProtocol           Used transport protocol, values:
     *                                  1 = TCP,
     *                                  2 = UDP.
     * @return      Session ID, via this handle the session is used.
     */
    virtual TUint NewSessionL( MNSPSessionObserver& aSessionObserver,
            TUint32 aIapId, const TDesC8& aDomain, TUint aProtocol ) = 0;
    
    /**
     * With this call NAT session is terminated.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     */
    virtual void CloseSessionL( TUint aSessionId ) = 0;
    
    /**
     * Sets the value of a session parameter.
     * 
     * @param       aSessionId          The ID identifying session.
     * @param       aParamKey           identifies the parameter.
     * @param       aParamValue         value of the parameter.
     * @return      KErrNotFound if the key is not found or
     *              other system wide error code on failure.
     */
    virtual TInt SetSessionParam( TUint aSessionId,
            TNSPSessionParamKey aParamKey, TUint aParamValue );
    
    /**
     * Gets a value of a session parameter.
     * 
     * @param       aSessionId          The ID identifying session.
     * @param       aParamKey           identifies the parameter.
     * @return      value of the parameter or
     *              KErrNotFound if the value is not available or
     *              other system wide error code on failure.
     */
    virtual TInt GetSessionParam( TUint aSessionId,
            TNSPSessionParamKey aParamKey );
    
    /**
     * Initial NAT specific Sdp content is added to Offer in here.
     * If 'KNatAsync' return value is used, then async callback
     * OfferReady is expected.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aOffer              Sdp offer document.
     * @return      NAT status, if async signaling expected.
     */
    virtual TNatReturnStatus CreateOfferL( TUint aSessionId,
            CSdpDocument*& aOffer ) = 0;
    
    /**
     * NAT FW specific Sdp content is added both Offer and Answer.
     * If 'KNatAsync' return value is used, then async callbacks
     * AnswerReady and OfferReady are expected.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aOffer              Sdp offer document.
     * @param       aAnswer             Sdp answer document.
     * @return      NAT status, if async signaling expected.
     */
    virtual TNatReturnStatus ResolveL( TUint aSessionId,
            CSdpDocument*& aOffer, CSdpDocument*& aAnswer ) = 0;
    
    /**
     * Answer to initial Offer interpred in here.
     * If 'KNatAsync' return value is used, then async callback
     * AnswerReady is expected.
     *
     * @since       S60 3.2
     * @param       aSessionId          The ID identifying session.
     * @param       aAnswer             Sdp answer document.
     * @return      NAT status, if async signaling expected.
     */
    virtual TNatReturnStatus DecodeAnswerL( TUint aSessionId,
            CSdpDocument*& aAnswer ) = 0;
    
    /**
     * Offer can be updated while ResolveL is still ongoing, i.e.
     * OfferReady callback is not yet received. Operation does not
     * affect on state, hence only synchronous update.
     *
     * @since       S60 3.2
     * @param       aSessionId          Session unique identier.
     * @param       aOffer              Sdp offer document.
     */
    virtual void UpdateL( TUint aSessionId, CSdpDocument*& aOffer ) = 0;
    
    /**
     * Empty INVITE, with certain NAT protocols, can be interpret as request
     * to restart connection checks. This method must be called is such
     * a message is received. If 'KNatAsync' return value is used,
     * then async callbacks OfferReady and AnswerReady are expected.
     *
     * @since       S60 3.2
     * @param       aSessionId          Session unique identier.
     * @return      NAT status, if async signaling expected.
     */
    virtual TNatReturnStatus RestartL( TUint aSessionId ) = 0;


public: // data

    /**
     * Unique instance identifier key
     */
    TUid iDtor_ID_Key;
    };

#include "nspplugin.inl"

#endif // NSPPLUGIN_H

// end of file
