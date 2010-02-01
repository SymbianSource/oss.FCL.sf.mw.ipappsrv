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
* Description: 
*
*/



#ifndef CSTUNBINDING_H
#define CSTUNBINDING_H

// INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include "natfwbindingbase.h"

class CBindingImplementation;
class CSTUNClient;
class TICEAttributes;

// CLASS DECLARATION
/**
 * A class for sending STUN Binding Requests and
 * obtaining the public IP address as seen by the STUN server.
 * @lib natfwstunclient.lib
 */
class CSTUNBinding : public CBinding
    {
    public:  // Constructors and destructor

        /**
         * Creates a new instance of CSTUNBinding.
         * Associates the local socket with the binding.
         *
         * @pre aSTUNClient.IsInitialized()
         * @pre aSocket must have been opened
         *
         * @param aSTUNClient an initialised STUN client
         * @param aSocket an opened socket that has the local port set.
         *
         * @return a new instance, the ownership is transferred.
         */
        IMPORT_C static CSTUNBinding* NewL( CSTUNClient& aSTUNClient,
                                            RSocket& aSocket );

        // overloaded version
        IMPORT_C static CSTUNBinding* NewL( CSTUNClient& aSTUNClient,
                                            TUint aStreamId,
                                            TUint aSubstreamId );


        /**
         * Creates a new instance of CSTUNBinding
         * and pushes it to CleanupStack.
         * Associates the local socket with the binding.
         *
         * @pre aSTUNClient.IsInitialized()
         * @pre aSocket must have been opened
         *
         * @param aSTUNClient an initialised STUN client
         * @param aSocket an opened socket that has the local port set.
         *
         * @return a new instance, the ownership is transferred.
         */
        IMPORT_C static CSTUNBinding* NewLC( CSTUNClient& aSTUNClient,
                                             RSocket& aSocket );

        // overloaded version
        IMPORT_C static CSTUNBinding* NewLC( CSTUNClient& aSTUNClient,
                                             TUint aStreamId,
                                             TUint aSubstreamId );


        /**
        * Destructor.
        */
        IMPORT_C ~CSTUNBinding();

    public: // New functions

        /**
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
         * Destination address is added for multiplexer to decide real
         * destination.
         *
         * @since   s60 3.2
         * @param   aDestAddr Destination where to send binding request
         * @return  void
         */
        IMPORT_C void SendRequestL( const TInetAddr& aDestAddr );

        /**
         * Cancels a previously issued call to SendRequestL.
         */
        IMPORT_C void CancelRequest();

        /**
         * Gets the socket used for obtaining the public address.
         *
         * @return socket
         */
        IMPORT_C const RSocket& Socket() const;

        /**
         * Checks whether the public address has a value set.
         *
         * @return ETrue if the public address has been obtained,
         *         otherwise EFalse.
         */
        IMPORT_C TBool AddressResolved() const;

        /**
         * Gets the public address for this binding as seen in public network.
         * If AddressResolved function returns EFalse, the return value of
         * this function is not defined.
         *
         * @return public address
         */
        IMPORT_C const TInetAddr& PublicAddr() const;

        /**
         * The client should call this function when it has received data
         * from the socket shared with this binding and
         * the client cannot itself handle the data.
         *
         * @param aData data received
         * @param aConsumed on return ETrue if the data was accepted,
         *        otherwise EFalse.
         * @param   aRemoteAddr remote address from remote address attribute
         * @return  Decoded data if data is indication and not unsaf message,
         *          otherwise NULL. Ownership is transferred.
         */
        IMPORT_C HBufC8* HandleDataL(
            const TDesC8& aData, TBool& aConsumed, TInetAddr& aRemoteAddr );

        /**
         * ICE specific attributes can be set for the request.
         *
         * @since   s60 3.2
         * @param   aAttributes     Contains ICE specific attributes.
         * @return  void
         */
        IMPORT_C void SetICESpecificAttributes(
            const TICEAttributes& aAttributes );

        /**
         * client can query used stream id
         *
         * @since   s60 3.2
         * @return  stream Id
         */
        IMPORT_C const TUint& StreamId() const;

        /**
         * Immediate retransmit can be executed. This should improve ICE
         * functionality and decrease time spent to connectivity checks.
         *
         * @since   s60 3.2
         * @return  none
         */
        IMPORT_C void ExecuteImmediateRetransmitL();

        /**
         * Returns the associated CSTUNClient instance.
         *
         * @return CSTUNClient, or NULL if the CSTUNClient has been deleted
         */
        IMPORT_C const CSTUNClient* STUNClient() const;


    protected: // Public functions for internal use

        /**
         * Returns the implementation instance.
         */
        CBindingImplementation& Implementation();

        /**
         * Returns a non-modifiable implementation instance.
         */
        const CBindingImplementation& Implementation() const;

   private: // Constructors, for internal use

        CSTUNBinding();
        CSTUNBinding( const CBinding& aBinding );

        void ConstructL( CSTUNClient& aSTUNClient, RSocket& aSocket );
        void ConstructL( CSTUNClient& aSTUNClient,
                         TUint aStreamId,
                         TUint aSubstreamId );

    private: // Data

        CBindingImplementation* iImplementation;

    private: // For testing purposes

        __DECLARE_TEST;

        friend class CSTUNBindingTest;
    };

#endif // CSTUNBINDING_H

// End of File
