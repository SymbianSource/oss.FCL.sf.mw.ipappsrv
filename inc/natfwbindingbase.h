/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides base class for binding services
*
*/




#ifndef CBINDING_H
#define CBINDING_H

#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include "natfwstunclientdefs.h"

class CSTUNClient;
class CBindingImplementation;

/**
 * A base class for sending binding requests and 
 * obtaining public IP address as seen by the STUN or TURN server.
 * Child class instances cannot be used directly 
 * through base class pointer.
 *
 * @lib     natfwstunclient.lib
 * @since   s60 v3.2
 */
class CBinding : public CBase
    {
public:

    /**
     * Cancels a previously issued SendRequestL or AllocateRequestL.
     *
     * @since   s60 v3.2
     * @return  void
     */
    virtual void CancelRequest() = 0;
    
    /**
     * Send indication to remote address. Indications are not
     * retransmitted and responses are not expected.
     *
     * @since   s60 v3.2
     * @param   aRemoteAddr     Address to send to (remote agent)
     * @param   aData           Data to send to remote agent. Use KNullDesC8
     *                          if there is no data available.
     * @param   aAddFingerprint Add fingerprint attribute if true
     * @return  void
     */
    virtual void SendIndicationL( const TInetAddr& aRemoteAddr, 
        const TDesC8& aData=KNullDesC8, TBool aAddFingerprint=EFalse ) = 0;
     
    /**
     * Checks whether the public address has a value set.
     *
     * @since   s60 v3.2
     * @return  ETrue if the public address has been obtained, 
     *          otherwise EFalse. 
     */
    virtual TBool AddressResolved() const = 0;    

    /**
     * Gets the public address for this binding as seen in public network.
     * If AddressResolved function returns EFalse, the return value of
     * this function is not defined.
     *
     * @since   s60 v3.2
     * @return  server reflexive transport address
     */
    virtual const TInetAddr& PublicAddr() const = 0;

    /**
     * This method should be called when receiving data
     * from the socket shared with this binding and 
     * the data cannot be handled by client.
     * 
     * @since   s60 v3.2
     * @param   aData data received
     * @param   aConsumed on return ETrue if the data was accepted,
     *          otherwise EFalse.
     * @param   aRemoteAddr remote address from remote address attribute
     * @return  Decoded data if data is indication and not unsaf message,
     *          otherwise NULL. Ownership is transferred.
     */
    virtual HBufC8* HandleDataL( 
        const TDesC8& aData, TBool& aConsumed, TInetAddr& aRemoteAddr ) = 0;
    
    /**
     * ICE specific attributes can be set for the request.
     *  
     * @since   s60 v3.2
     * @param   aAttributes     Contains ICE specific attributes.
     * @return  void
     */
    virtual void SetICESpecificAttributes( 
        const TICEAttributes& aAttributes ) = 0;
    
    /**
     * Query used stream ID
     *
     * @since   s60 v3.2
     * @return  stream Id
     */
    virtual TUint StreamId() const = 0;
    
    /**
     * Query used connection ID
     *
     * @since   s60 v3.2
     * @return  connection Id
     */
    virtual TUint ConnectionId() const = 0;
    
    /**
     * Returns alternate server transport address.
     *
     * @since   s60 v3.2
     * @return  alternate server address
     */
    virtual const TInetAddr& AlternateServerAddr() const = 0;
    
    /**
     * Returns realm value from error response.
     *
     * @since   s60 v3.2
     * @return  realm from error response         
     */
    virtual const HBufC8* RealmFromResponse() const = 0;         

    /**
     * Returns the associated CSTUNClient instance.
     *
     * @since   s60 v3.2 
     * @return  CSTUNClient, or NULL if the CSTUNClient has been deleted        
     */
    virtual const CSTUNClient* STUNClient() const = 0;
    
    /**
     * Returns the implementation instance.      
     */
    virtual CBindingImplementation& Implementation() = 0;
    
    /**
     * Returns a non-modifiable implementation instance.        
     */
    virtual const CBindingImplementation& Implementation() const = 0;
    

protected:
    
    virtual void ConstructL( CSTUNClient& aSTUNClient, TUint aStreamId, 
        TUint aConnectionId ) = 0;

    virtual void ConstructL( CSTUNClient& aSTUNClient, RSocket& aSocket ) = 0;

protected: // Data
    
    /**
     * Pointer to binding implementation.
     * Own.
     */
    CBindingImplementation* iImplementation;
    };

#endif // CBINDING_H

// End of File
