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
* Description:    Session observer class, implemented by plugin client.
*
*/



#ifndef NSPSESSIONOBSERVER_H
#define NSPSESSIONOBSERVER_H

#include <e32def.h>

class CSdpDocument;

/**
 * Error limit which can be used to signal situations where Nat failed in
 * asynchronous prosessing, and resume of normal SIP signaling is expected.
 * All errors below this limit are considered as non-fatal, and normal SIP
 * session is maintained without informing MCE Client.
 */
const TInt KNatNonFatalErrorLimit = -11000;

/**
 *  Session observer, implemented by plugin client to receive async calls.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class MNSPSessionObserver
    {
public: // New functions
    
    /**
     * Callback for signaling that session is initialized, and ready for use.
     */
    virtual void Initialized( TUint aSessionId ) = 0;
    
    /**
     * Callback for signaling that Offer is modified to point NAT.
     *
     * @param   aSessionId       Session unique identier.
     * @param   aOffer           Modified Offer, ownership transferred.
     */
    virtual void OfferReady( TUint aSessionId, CSdpDocument* aOffer ) = 0;
    
    /**
     * Callback for signaling that Answer is modified to point NAT.
     *
     * @param   aSessionId       Session unique identier.
     * @param   aAnswer          Modified Answer, ownership transferred.
     */
    virtual void AnswerReady( TUint aSessionId, CSdpDocument* aAnswer ) = 0;
    
    /**
     * Callback for signaling that NAT negotiation has resulted
     * a different result than original SDP A/O. Hence updated SDP content
     * must be sent. Only Controlling agent receives this callback.
     *
     * @param   aSessionId       Session unique identier.
     * @param   aOffer           Last SDP document, but with modified content.
     *                           Ownership transferred.
     */
    virtual void UpdateSdp( TUint aSessionId, CSdpDocument* aOffer ) = 0;
    
    /**
     * Session errors. Session which is identified by aSessionId is deleted.
     *
     * @param   aSessionId       Session unique identier.
     * @param   aError           System wide error code.
     */
    virtual void ErrorOccurred( TUint aSessionId, TInt aError ) = 0;

    /**
     * Session media related ICMP error occured.
     *
     * @param   aSessionId       Session unique identier.
     * @param   aError           ICMP error code.
     */
    virtual void IcmpErrorOccurred( TUint aSessionId, TInt aError ) = 0;
    
    };

#endif // NSPSESSIONOBSERVER_H

// end of file
