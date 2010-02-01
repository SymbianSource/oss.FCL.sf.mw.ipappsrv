/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    
*
*/




#ifndef M_STUNBINDINGOBSERVER_H
#define M_STUNBINDINGOBSERVER_H

// INCLUDES
#include <e32def.h>
#include "natfwunsaftransactionid.h"
#include "tstuncallbackinfo.h"
#include "natfwstunclientdefs.h"

// FORWARD DECLARATIONS
class CSTUNClient;
class CBinding;

// CLASS DECLARATION

/**
 *  Defines the services that CSTUNClientImplementation provides to
 *  CSTUNBinding.
 */
class MSTUNBindingObserver
    {
public: // Destructor

    /**
      * Destructor
     */
    virtual ~MSTUNBindingObserver() {}

public: // New pure virtual functions

    /**
    * Returns the associated CSTUNClient instance.
    *
    * @return CSTUNClient
    */
    virtual const CSTUNClient& STUNClient() const = 0;

    /**
     * Retrieves the user timer service provider.
     * @return CDeltaTimer& Timer services
     */
    virtual CDeltaTimer& TimerProvider() = 0;
    
    /**
     * Retrieves the retransmission interval to use with Binding Requests.
     * @return Retransmission interval
     */
    virtual TInt RetransmitInterval() const = 0;

    /**
     * Retrieves the transport protocol to use with Binding Requests.
     * @return Transport protocol
     */
    virtual TTransportProtocol TransportProtocol() const = 0;
    
    /**
     * Binding has obtained a public IP address.
     * @param aBinding Binding for which the address was obtained
     */
    virtual void AddressResolvedL( const CBinding& aBinding ) const = 0;

    /**
     * Requests a shared secret (username and password).
     * STUN client provides the username and password to binding with
     * SharedSecretObtainedL function. If STUN client is currently retrieving a
     * new shared secret after the previous one expired, it may take some time
     * until it calls SharedSecretObtainedL.
     * @param aBinding Binding requesting the information
     */
    virtual void ObtainSharedSecretL( CBinding& aBinding ) = 0;

    /**
     * Inform the STUN client that a remote server has rejected certain
     * username and password.
     * @param aBinding Binding that was using the username and password.
     * @param aUsername Username that was rejected. Can be empty (zero length)
     *          descriptor, if STUN server provided such a value.
     * @param aPassword Password that was rejected. Can be empty (zero length)
     *          descriptor, if STUN server provided such a value.
     * @return ETrue if the Binding should retry, EFalse if binding should
     *           terminate.
     */
    virtual TBool SharedSecretRejectedL( const CBinding& aBinding,
                                         const TDesC8& aUsername,
                                         const TDesC8& aPassword ) = 0;

    /**
     * Requests the UDP address of a STUN server, where the Binding Request
     * will be sent.
     * @param aAddress IN: The address that this binding has already tried but
     *                       failed because the STUN server did not respond, or
     *                       there was a UNSAF transport layer error.
     *                       If the binding has not yet sent a Binding Request,
     *                       then aAddress should contain an unspecified address
     *                       (for IPv4 it means address 0.0.0.0, and for IPv6 an
     *                       address of ::).
     *                   OUT: The address that binding should try next.
     * @return ETrue  Address successfully obtained
     *           EFalse No address available
     */
    virtual TBool ObtainServerAddress( TInetAddr& aAddress ) = 0;

    /**
     * Requests a new UNSAF transaction ID.
     * @param aTransactionID OUT: New TransactionID
     */
    virtual void
        ObtainTransactionIDL( TNATFWUNSAFTransactionID& aTransactionID ) = 0;
    
    /**
     * A new binding has been created and it is associated with the STUN
     * client.
     * @param aBinding Binding to attach
     */
    virtual void AttachBindingL( const CBinding& aBinding ) = 0;

    /**
     * CSTUNBinding is being deleted, all references to it must be cleared,
     * including any pending callbacks referring to it.
     * @param aBinding Binding to detach
     */
    virtual void DetachBinding( const CBinding& aBinding ) = 0;

    /**
     * CSTUNBinding has terminated because an error occurred.
     * @param aBinding Binding that encountered the error
     * @param aError Error reason
     * @param aIsFatal ETrue if error is unrecoverable
     */
    virtual void BindingErrorL( const CBinding& aBinding,
                                TInt aError, TBool aIsFatal ) = 0;
    
    /**
     * Binding event occurred, client must be notified
     * 
     * @since   s60 3.2
     * @param   aBinding Binding
     * @param   aEvent  event
     * @return  void
     */
    virtual void BindingEventOccurred( 
        const CBinding& aBinding, TSTUNCallbackInfo::TFunction aEvent ) = 0;
    
    /**
     * Get username for indication
     * 
     * @since   s60 3.2
     * @return  username
     */
    virtual const TDesC8& UsernameForIndication() = 0;
    
    /**
     * Get password for indication
     * 
     * @since   s60 3.2
     * @return  password
     */
    virtual const TDesC8& PasswordForIndication() = 0;
    };

#endif // M_STUNBINDINGOBSERVER_H
