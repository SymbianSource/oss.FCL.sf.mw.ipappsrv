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


#ifndef CBINDING_H
#define CBINDING_H

// INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include "natfwstunclientdefs.h"

// FORWARD DECLARATIONS
class CSTUNClient;
class CBindingImplementation;
class TICEAttributes;


// CLASS DECLARATION
/**
 * A base class for sending binding requests and
 * obtaining the public IP address as seen by the STUN or TURN server.
 *
 * @lib     natfwstunclient.lib
 * @since   S60 v3.2
 */
class CBinding : public CBase
    {
    public:  // Constructors and destructor
        virtual ~CBinding();

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
         * @since   s60 3.2
         * @return  void
         */
        virtual void SendRequestL();

        /**
         * Overloaded version of SendRequestL().
         * Destination address is added for multiplexer to decide real
         * destination.
         *
         * @since   s60 3.2
         * @param   aDestAddr Destination where to send binding request
         * @return  void
         */
        virtual void SendRequestL( const TInetAddr& aDestAddr );

        /**
         * Cancels a previously issued call to SendRequestL.
         *
         * @since   s60 3.2
         * @return  void
         */
        virtual void CancelRequest() = 0;

        /**
         * For STUN relay binding request
         *
         * @since   s60 3.2
         * @return  void
         */
        virtual void AllocateRequestL();

        /**
         * Gets the socket used for obtaining the public address.
         *
         * @since   s60 3.2
         * @return socket
         */
        virtual const RSocket& Socket() const = 0;

        /**
         * Checks whether the public address has a value set.
         *
         * @since   s60 3.2
         * @return ETrue if the public address has been obtained,
         *         otherwise EFalse.
         */
        virtual TBool AddressResolved() const = 0;

        /**
         * Gets the public address for this binding as seen in public network.
         * If AddressResolved function returns EFalse, the return value of
         * this function is not defined.
         *
         * @since   s60 3.2
         * @return public address
         */
        virtual const TInetAddr& PublicAddr() const = 0;

        /**
         * The client should call this function when it has received data
         * from the socket shared with this binding and
         * the client cannot itself handle the data.
         *
         * @since   s60 3.2
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
         * After successfully AllocateRequestL
         * and AddressResolved calling the client can fetch the candidate
         * through this method.
         *
         * @since   s60 3.2
         * @param   aReflexiveAddr  resolved public source IP address
         * @param   aRelayAddr      resolved relay IP address
         * @return  void
         */
        virtual void FetchCandidateL( TInetAddr& aReflexiveAddr,
                                      TInetAddr& aRelayAddr );

        /**
         * ICE specific attributes can be set for the request.
         *
         * @since   s60 3.2
         * @param   aAttributes     Contains ICE specific attributes.
         * @return  void
         */
        virtual void SetICESpecificAttributes(
            const TICEAttributes& aAttributes ) = 0;

        /**
         * Once the client wants to primarily receive from one peer, it can send
         * a SetActiveDestination request.  All subsequent data received from
         * the active peer is forwarded directly to the client and vice versa,
         * except that it is wrapped or framed according to the protocol used
         * between the STUN relay client and STUN relay server.
         * The SetActiveDestination Request does not close other bindings. Data
         * to and from other peers is still wrapped in Send and Data indications
         * respectively.
         *
         * @since   s60 3.2
         * @param   aRemoteAddr Client can specify remote address
         * @param   aTimerValue Client gets timer value after response
         * @return  void
         */
        virtual void SetActiveDestinationRequestL(
            const TInetAddr& aRemoteAddr, TUint32& aTimerValue );

        /**
         * Before receiving any UDP or TCP data, a client has to send first.
         * Prior to the establishment of an active destination, or while the
         * client is in the transitioning state, transmission of data towards a
         * peer through the relay is done using the Send Indication.  Indeed, if
         * the client is in the transitioning state, and it wishes to send data
         * through the relay, it MUST use a Send indication.
         * The Send Indication MUST contain a REMOTE-ADDRESS attribute, which
         * contains the IP address and port that the data is being sent to.
         * Since Send is an Indication, it generates no response.
         *
         * @since   s60 3.2
         * @param   aRemoteAddr Address to send to (remote agent)
         * @param   aData       Data to send to remote agent. Use KNullDesC8 if
         *                      there is no data available.
         * @return  void
         */
        virtual void SendIndicationL( const TInetAddr& aRemoteAddr,
                                      const TDesC8& aData );


        /**
         * The Connect Request is used by a client when it has obtained an
         * allocated transport address that is TCP.  The client can use the
         * Connect Request to ask the server to open a TCP connection to a
         * specified destination address included in the request.
         *
         * @since   s60 3.2
         * @param   aRemoteAddr remote address
         * @return  void
         */
        virtual void ConnectRequestL( const TInetAddr& aRemoteAddr );

        /**
         * client can query used stream id
         *
         * @since   s60 3.2
         * @return  stream Id
         */
        virtual const TUint& StreamId( ) const = 0;

        /**
         * Immediate retransmit can be executed. This should improve ICE
         * functionality and decrease time spent to connectivity checks.
         *
         * @since   s60 3.2
         * @return  none
         */
        virtual void ExecuteImmediateRetransmitL() = 0;

        /**
         * Returns the associated CSTUNClient instance.
         *
         * @since   s60 3.2
         * @return CSTUNClient, or NULL if the CSTUNClient has been deleted
         */
        virtual const CSTUNClient* STUNClient() const = 0;


    public: // Public functions for internal use

        /**
         * Returns the implementation instance.
         */
        virtual CBindingImplementation& Implementation() = 0;

        /**
         * Returns a non-modifiable implementation instance.
         */
        virtual const CBindingImplementation& Implementation() const = 0;

    protected: // Constructors, for internal use

        virtual void ConstructL( CSTUNClient& aSTUNClient, RSocket& aSocket ) = 0;
        virtual void ConstructL( CSTUNClient& aSTUNClient,
                                 TUint aStreamId,
                                 TUint aSubstreamId ) = 0;

    protected: // Data

        /**
         * pointer to binding implementation
         * Own.
         */
        CBindingImplementation* iImplementation;


    };

#endif // CBINDING_H

// End of File
