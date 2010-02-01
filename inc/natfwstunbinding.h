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
* Description:    Provides STUN binding services.
*
*/





#ifndef CSTUNBINDING_H
#define CSTUNBINDING_H

#include <e32base.h>
#include "natfwbindingbase.h"

/**
 * A class for sending STUN Binding Requests and 
 * obtaining the public IP address as seen by the STUN server. 
 * @lib natfwstunclient.lib
 */
class CSTUNBinding : public CBinding
    {
    friend class CSTUNBindingTest;
    friend class ut_cstunclientimplementation;

public:

    /**
     * Two-phased constructor.
     * Associates the local socket with the binding.
     *
     * @pre     aSTUNClient.IsInitialized()
     * @pre     aSocket must have been opened
     * @param   aSTUNClient an initialised STUN client
     * @param   aSocket an opened socket that has the local port set.
     */
    IMPORT_C static CSTUNBinding* NewL( CSTUNClient& aSTUNClient,
        RSocket& aSocket );

    /**
     * Two-phased constructor.
     * Overloaded version.
     *
     * @since   s60 v3.2
     * @pre     aSTUNClient.IsInitialized()
     * @param   aSTUNClient an initialised STUN client
     * @param   aStreamId Stream identifier
     * @param   aConnectionId Connection identifier
     */
    IMPORT_C static CSTUNBinding* NewL( CSTUNClient& aSTUNClient, 
        TUint aStreamId, TUint aConnectionId );
        
    /**
     * Two-phased constructor.
     *
     * @pre     aSTUNClient.IsInitialized()
     * @pre     aSocket must have been opened
     * @param   aSTUNClient an initialised STUN client
     * @param   aSocket an opened socket that has the local port set.
     */
    IMPORT_C static CSTUNBinding* NewLC( CSTUNClient& aSTUNClient, 
        RSocket& aSocket );          

    /**
     * Two-phased constructor.
     *
     * @since   s60 v3.2
     * @pre     aSTUNClient.IsInitialized()
     * @param   aSTUNClient an initialised STUN client
     * @param   aStreamId Stream identifier
     * @param   aConnectionId Connection identifier
     */
    IMPORT_C static CSTUNBinding* NewLC( CSTUNClient& aSTUNClient, 
        TUint aStreamId, TUint aConnectionId );          

    /**
     * Destructor.
     */
    IMPORT_C ~CSTUNBinding();

    /**
     * Executes STUN binding request.
     * Refreshes the binding causing a new Binding Request to be sent
     * to the STUN server. The public IP address is available when 
     * MSTUNClientObserver gets called.
     * If application provided credentials with CSTUNClient::SetCredentialsL
     * or if parameter aObtainSharedSecret was ETrue when CSTUNClient was
     * created, USERNAME and MESSAGE-INTEGRITY attributes are put into the
     * Binding Request.
     *
     * @return  void
     */
    IMPORT_C void SendRequestL();

    /**
     * Overloaded version of SendRequestL().
     *
     * @since   s60 v3.2
     * @param   aDestAddr       Destination where to send request 
     *                          (KAFUnspec=default address)
     * @param   aAddFingerprint Whether to include fingerprint attribute
     *                          to request
     * @param   aRtoValue       Retransmission timeout
     *                          (0 = default value to be used)
     * @return  void
     */
    IMPORT_C void SendRequestL( const TInetAddr& aDestAddr,
        TBool aAddFingerprint, TUint aRtoValue );
    
    /**
     * Gets the socket used for obtaining the public address.
     *
     * @return  socket
     */
    IMPORT_C const RSocket& Socket() const;    

    /**
     * Specific cancel for stopping ongoing message retransmission.
     * Response is waited as long as transaction waiting time expires.
     * Client is notified by calling
     * MSTUNClientObserver::STUNBindingErrorOccurred.
     *
     * @since   s60 v3.2
     * @return  void
     */
    IMPORT_C void CancelRetransmission();
    
    /**
     * From CBinding.
     * Cancels a previously issued SendRequestL.
     */
    IMPORT_C void CancelRequest();
    
    /**
     * From CBinding
     * Send binding indication. Indications are not
     * retransmitted and responses are not expected.
     *
     * @since   s60 v3.2
     * @param   aRemoteAddr     address to send to (remote agent).
     * @param   aAddFingerprint add fingerprint attribute if true
     * @return  void
     */
    IMPORT_C void SendIndicationL( const TInetAddr& aRemoteAddr,
        const TDesC8& aData, TBool aAddFingerprint );
    
    /**
     * From CBinding.
     * Checks whether the public address has a value set.
     *
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
     * @return  public transport address
     */
    IMPORT_C const TInetAddr& PublicAddr() const;

    /**
     * From CBinding.
     * This method should be called when receiving data
     * from the socket shared with this binding and 
     * the data cannot be handled by client.
     *
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
     * Query used stream ID
     *
     * @since   s60 v3.2
     * @return  stream Id
     */
    IMPORT_C TUint StreamId() const;
    
    /**
     * From CBinding.
     * Query used connection ID
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
     * @return  alternate server address
     */
    IMPORT_C const TInetAddr& AlternateServerAddr() const;
    
    /**
     * From CBinding.
     * Returns realm value from error response.
     *
     * @since   s60 v3.2
     * @return  realm from error response         
     */
    IMPORT_C const HBufC8* RealmFromResponse() const;    
        
    /**
     * From CBinding.
     * Returns the associated CSTUNClient instance.
     *
     * @return CSTUNClient, or NULL if the CSTUNClient has been deleted        
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

    CSTUNBinding();
    CSTUNBinding( const CBinding& aBinding );

    void ConstructL( CSTUNClient& aSTUNClient, RSocket& aSocket );
    void ConstructL( CSTUNClient& aSTUNClient, TUint aStreamId, 
        TUint aConnectionId );

    __DECLARE_TEST;
    };

#endif // CSTUNBINDING_H

