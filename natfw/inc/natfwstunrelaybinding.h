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
* Description:    Provides STUN relay binding services.
*
*/




#ifndef CSTUNRELAYBINDING_H
#define CSTUNRELAYBINDING_H

#include <e32base.h>
#include "natfwbindingbase.h"

/**
 * A class for sending Allocate Requests and 
 * obtaining the public IP address as seen by the STUN relay server.
 *
 * @lib      natfwstunclient.lib
 * @since    s60 v3.2
 */
class CSTUNRelayBinding : public CBinding
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since   s60 v3.2
     * @pre     aSTUNClient.IsInitialized()
     * @param   aSTUNClient an initialized STUNClient
     * @param   aStreamId Stream identifier
     * @param   aConnectionId Connection identifier
     */
    IMPORT_C static CSTUNRelayBinding* NewL( CSTUNClient& aSTUNClient, 
        TUint aStreamId, TUint aConnectionId );

    /**
     * Two-phased constructor.
     *
     * @since   s60 v3.2
     * @pre     aSTUNClient.IsInitialized()
     * @param   aSTUNClient an initialised STUN client
     * @param   aStreamId Stream identifier
     * @param   aConnectionId Connection identifier
     */
    IMPORT_C static CSTUNRelayBinding* NewLC( CSTUNClient& aSTUNClient, 
        TUint aStreamId, TUint aConnectionId );        

    /**
     * Destructor.
     */
    IMPORT_C ~CSTUNRelayBinding();

    /**
     * Executes STUN relay binding request.
     * Refreshes the binding causing a new Allocate Request to be sent
     * to the STUN relay server. The public IP address is available when 
     * MSTUNClientObserver gets called.
     * If application provided credentials with CSTUNClient::SetCredentialsL
     * or if parameter aObtainSharedSecret was ETrue when CSTUNClient was
     * created, USERNAME and MESSAGE-INTEGRITY attributes are put into the
     * Binding Request.
     *
     * @since   s60 v3.2
     * @param   aRtoValue Retransmission timeout(0 = default value to be used)
     * @return  void
     */
    IMPORT_C void AllocateRequestL( TUint aRtoValue );

    /**
     * Once the client wants to primarily receive from one peer, it can send
     * SetActiveDestination request.
     *
     * @since   s60 v3.2
     * @param   aRemoteAddr Specified remote address
     * @param   aTimerValue Timer value got from server
     * @return  void
     */
    IMPORT_C void SetActiveDestinationRequestL( 
        const TInetAddr& aRemoteAddr, TUint32& aTimerValue );

    /**
     * After successfully AllocateRequestL and AddressResolved() calling
     * relayed transport address can be queried. 
     *  
     * @since   s60 v3.2
     * @return  relay transport address
     */
    IMPORT_C const TInetAddr& RelayAddr() const;

    /**
     * Transmission of data towards a peer
     * through the relay can be done using the SendIndicationL.
     * Indication does not generate response.
     *
     * @since   s60 v3.2
     * @param   aRemoteAddr     Address to send to (remote agent)
     * @param   aData           Data to send to remote agent. Use KNullDesC8
     *                          if there is no data available.
     * @param   aAddFingerprint true if fingerprint is added
     * @return  void
     */
    IMPORT_C void SendIndicationL( const TInetAddr& aRemoteAddr, 
        const TDesC8& aData, TBool aAddFingerprint );
        
    /**
     * ConnectRequestL is used (when obtained an
     * allocated transport address that is TCP) to request server to open
     * TCP connection to specific destination address.
     *
     * @since   s60 v3.2
     * @param   aRemoteAddr Specific remote address
     * @return  void
     */
    IMPORT_C void ConnectRequestL( const TInetAddr& aRemoteAddr );

    /**
     * From CBinding.
     * Cancels previously issued AllocateRequestL.
     *
     * @since   s60 v3.2
     * @return  void
     */
    IMPORT_C void CancelRequest();

    /**
     * From CBinding.
     * Checks whether the public address has a value set.
     *
     * @since   s60 v3.2
     * @return  ETrue if the public address has been obtained, 
     *          otherwise EFalse. 
     */
    IMPORT_C TBool AddressResolved() const;    

    /**
     * From CBinding.
     * Gets the public address for this binding as seen in public network.
     * If AddressResolved function returns EFalse, the return value of
     * this function is not defined.
     *
     * @since   s60 v3.2
     * @return public address
     */
    IMPORT_C const TInetAddr& PublicAddr() const;

    /**
     * From CBinding.
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
    IMPORT_C HBufC8* HandleDataL( 
        const TDesC8& aData, TBool& aConsumed, TInetAddr& aRemoteAddr );

    /**
     * From CBinding.
     * ICE specific attributes can be set for the request.
     *  
     * @since   s60 v3.2
     * @param   aAttributes     Contains ICE specific attributes.
     * @return  void
     */
    IMPORT_C void SetICESpecificAttributes( 
        const TICEAttributes& aAttributes );

    /**
     * From CBinding.
     * Returns used stream Id
     *
     * @since   s60 v3.2
     * @return  stream Id
     */
    IMPORT_C TUint StreamId() const;

    /**
     * From CBinding.
     * Returns used connection Id
     *
     * @since   s60 v3.2
     * @return  connection Id
     */
    IMPORT_C TUint ConnectionId() const;
        
    /**
     * From CBinding.
     * Returns alternate server transport address.
     *
     * @since   s60 v3.2
     * @return alternate server address
     */
    IMPORT_C const TInetAddr& AlternateServerAddr() const;

    /**
     * From CBinding.
     * Returns realm value from error response.
     *
     * @since   s60 v3.2
     * @return realm from error response         
     */
    IMPORT_C const HBufC8* RealmFromResponse() const;          
            
    /**
     * From CBinding.
     * Returns the associated CSTUNClient instance.
     *
     * @since   s60 v3.2
     * @return  CSTUNClient, or NULL if the CSTUNClient has been deleted        
     */
    IMPORT_C const CSTUNClient* STUNClient() const;


protected:

    /**
     * From CBinding.
     * Returns the implementation instance.        
     */
    CBindingImplementation& Implementation();
    
    /**
     * From CBinding.
     * Returns a non-modifiable implementation instance.        
     */
    const CBindingImplementation& Implementation() const;

private:

    CSTUNRelayBinding();
    CSTUNRelayBinding( const CBinding& aBinding );

    void ConstructL( CSTUNClient& aSTUNClient, TUint aStreamId,
        TUint aConnectionId );
    
    // This constructor is not used in this implementation    
    void ConstructL( CSTUNClient& aSTUNClient, RSocket& aSocket );
    
    __DECLARE_TEST;
    };

#endif // CSTUNRELAYBINDING_H

