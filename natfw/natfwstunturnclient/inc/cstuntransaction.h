/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_CSTUNTRANSACTION_H
#define C_CSTUNTRANSACTION_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>
#include "mnatfwunsafudpsenderobserver.h"
#include "cstuntimeruser.h"
#include "cnatfwunsafmediaconnsender.h"
#include "mncmsenderobserver.h"
#include "natfwstunclientdefs.h"

// FORWARD DECLARATIONS
class RSocket;
class CNATFWUNSAFMessage;
class CNATFWUNSAFBindingRequest;
class CNATFWUNSAFUnknownAttributesAttribute;
class CNATFWUNSAFUdpSender;
class MSTUNTransactionObserver;
class MNcmConnectionMultiplexer;

// CLASS DECLARATION

/**
 *  This class presents a single STUN transaction. 
 */
class CSTUNTransaction :
    public CSTUNTimerUser,
    public MNATFWUNSAFUdpSenderObserver,
    public MNcmSenderObserver
    {
    
    friend class ut_cstuntransaction;

    public: // Constructors and destructor

        /**
        * Creates a new instance of CSTUNTransaction
        * @pre aAddress.IsUnspecified() == EFalse
        * @param aSocket UDP socket to use
        * @param aAddress STUN server's address where the request is sent to
        * @param aSharedSecret Shared secret to be used to calculate
        *        a hash over the encoded UNSAF message.
        * @param aObserver An observer to receive callbacks
        * @param aTimer Timer provider
        * @param aRetransmitInterval Initial retransmission interval
        * @param aTransportProtocol Used transport protocol.
        * @return A new instance of CSTUNTransaction, ownership is transferred.
        */
        static CSTUNTransaction*
            NewL( CNATFWUNSAFMessage& aRequest,
                  const TInetAddr& aAddress,
                  const TDesC8& aSharedSecret,
                  RSocket& aSocket,
                  CDeltaTimer& iTimer,
                  TInt aRetransmitInterval,
                  MSTUNTransactionObserver& aObserver,
                  const TDesC8& aProtocol,
                  TTransportProtocol aTransportProtocol );
        
        /**
         * Overload version of NewL
         * 
         * Creates a new instance of CSTUNTransaction
         * @pre aAddress.IsUnspecified() == EFalse
         * @param aRequest Binding Request message
         * @param aAddress STUN server's address where the request is sent to
         * @param aSharedSecret Shared secret to be used to calculate
         *        a hash over the encoded UNSAF message.
         * @param aStreamId Used natfw stream id
         * @param aSubstreamId Used natfw stream related sub stream id
         * @param aTimer Timer provider
         * @param aRetransmitInterval Initial retransmission interval 
         * @param aObserver An observer to receive callbacks
         * @param aProtocol Used protocol, either "stun" or "stun-relay"
         * @param aMux instance of connection multiplexer 
         * @param aTransportProtocol Used transport protocol.
         * @param aUseFingerprint ETrue if fingerprint is added to request
         * @param aDestAddr Destination address
         * @return A new instance of CSTUNTransaction, ownership is transferred.
         */
        static CSTUNTransaction*
            NewL( CNATFWUNSAFMessage& aRequest,
                  const TInetAddr& aAddress,
                  const TDesC8& aSharedSecret,
                  TUint aStreamId,
                  TUint aSubstreamId,
                  CDeltaTimer& aTimer,
                  TInt aRetransmitInterval,
                  MSTUNTransactionObserver& aObserver,
                  const TDesC8& aProtocol,
                  MNcmConnectionMultiplexer& aMux,
                  TTransportProtocol aTransportProtocol,
                  TBool aUseFingerprint=EFalse,
                  const TInetAddr& aDestAddr=KAFUnspec );

        ~CSTUNTransaction();

    public: // From CSTUNTimerUser

        void TimerExpiredL();

        void LeaveFromTimerExpired( TInt aError );

    public: // From MUNSAFUdpSenderObserver
    
        void UNSAFUdpMessageSentL();

        void UNSAFUdpMessageFailure( TInt aError );
    
    public: // From MNcmSenderObserver
        
        void MessageSent();
        
        void MessageSentFailure( TInt aError );

    public: // New functions

        void SendRequestL();

        void RetransmitRequestL();

        /**
        * Transaction receives a Binding (Error) Response message.
        * @pre aResponse.Type() == CUNSAFMessage::EBindingResponse ||
        *      aResponse.Type() == CUNSAFMessage::EBindingErrorResponse
        * @param aResponse STUN message, either a Binding Response or a
        *                   Binding Error Response
        * @param aByteStream Same STUN message as aResponse, but in undecoded
        *                     format.        
        */
        void ReceiveL( CNATFWUNSAFMessage& aResponse, const TDesC8& aByteStream );

        /**
        * Transaction has encountered an error and enters terminated state.
        * @param aError Error reason. If transaction ends successfully, value
        *         is KErrNone.
        */
        void Terminate( TInt aError );
        
        /**
         * TimerValue can be queried. This timer value is from 
         * SetActiveDestinationResponse TIMERVAL attribute.
         *
         * @since   s60 3.2
         * @return  timer value
         */
        const TUint32& TimerValue() const;
        
        /**
         * ICE specific cancel for stopping ongoing message retransmission.
         * Response is waited as long as sending timer expires. After that
         * client will be notified with error response.
         *
         * @since   s60 3.2
         * @return  void
         */
        void CancelRetransmission();


    private: // Enumerations

        enum TRetransmissionCount
            {
            // Maximum amount of Binding Requests to send
            KMaxRequestSentCount = 7     // rfc3489bis-06#section-7.1
            };

    private: // Constructors

        CSTUNTransaction( CNATFWUNSAFMessage& aRequest,
                          const TInetAddr& aAddress,
                          const TDesC8& aSharedSecret,
                          CDeltaTimer& iTimer,
                          TInt aRetransmitInterval,
                          MSTUNTransactionObserver& aObserver,
                          TTransportProtocol aTransportProtocol );

        CSTUNTransaction( CNATFWUNSAFMessage& aRequest,
                          const TInetAddr& aAddress,
                          const TDesC8& aSharedSecret,
                          TUint aStreamId,
                          TUint aSubstreamId,
                          CDeltaTimer& iTimer,
                          TInt aRetransmitInterval,
                          MSTUNTransactionObserver& aObserver,
                          const TInetAddr& aDestAddr,
                          TTransportProtocol aTransportProtocol,
                          TBool aUseFingerprint );

        CSTUNTransaction();

        CSTUNTransaction( const CSTUNTransaction& aTransaction );

        void ConstructL( RSocket& aSocket, const TDesC8& aProtocol );
        
        void ConstructL( const TDesC8& aProtocol, 
                         MNcmConnectionMultiplexer& aMux );

    private: // New functions, for internal use

        /**
        * Return the current retransmission interval in milliseconds.
        * @return Retransmission interval
        */
        TInt RetransmitInterval();
        
        /**
        * Computes how long transaction waits for response after it has received
        * a Binding Error Response with response code 100-399.
        * @return Duration in milliseconds.
        */
        TInt ComputeWaitDuration();

        /**
        * Determines if the response should be ignored or processed.
        * @param aResponse STUN Binding (Error) Response
        * @return ETrue Response must be ignored, EFalse otherwise
        */
        TBool ShouldIgnoreResponse( const CNATFWUNSAFMessage& aResponse ) const;

        /**
        * Determines if the response should be ignored or processed.
        * @pre aResponse.Type() == CUNSAFMessage::EBindingResponse ||
        *      aResponse.Type() == CUNSAFMessage::EBindingErrorResponse
        * @param aResponse STUN message, either a Binding Response or a
        *                  Binding Error Response
        * @param aByteStream Same STUN message as aResponse, but in undecoded
        *                     format.
        * @return KErrNone if transaction ends successfully, 400-699 if a
        *          Binding Error Response was received, or a system wide error
        *          code.
        */
        TInt ProcessResponseL( CNATFWUNSAFMessage& aResponse,
                                const TDesC8& aByteStream );

        /**
        * Verify the integrity of a Binding Response.
        * @pre aResponse.Type() == CUNSAFMessage::EBindingResponse
        * @param aResponse Binding Response
        * @param aByteStream Same message as aResponse, but in undecoded format
        * @return KErrNone If integrity check succeeded
        *         ERetryAfterAddingXorOnly If integrity fails because a NAT has
        *         overwritten ip-address in MAPPED-ADDRESS.
        *         KErrCorrupt Integrity fails for some other reason.
        */
        TInt CheckIntegrityL( const CNATFWUNSAFMessage& aResponse,
                                const TDesC8& aByteStream );

        /**
        * Check if the Binding Response has a different MAPPED-ADDRESS or
        * XOR-MAPPED-ADDRESS from the ones that have been received earlier.
        * @pre aResponse.Type() == CUNSAFMessage::EBindingResponse
        * @param aResponse STUN Binding Response
        * @return ETrue The address in MAPPED-ADDRESS or XOR-MAPPED-ADDRESS of
        *         the aResponse differs from the earlier address.
        *         EFalse otherwise
        */
        TBool AddressDiffersL( const CNATFWUNSAFMessage& aResponse ) const;

        /**
        * Determines if the message integrity check failed because of a NAT has
        * overwritten the IP-address in the MAPPED-ADDRESS.
        * @pre aResponse.Type() == CUNSAFMessage::EBindingResponse
        * @param aResponse STUN Binding Response
        * @return ETrue NAT probably overwrote the IP-address in MAPPED-ADDRESS
        *          EFalse Integrity check failed for some other reason
        */
        TBool CheckForNatOverwriteL( const CNATFWUNSAFMessage& aResponse );

        /**
        * Gets the response code value from a ERROR-CODE attribute of the
        * aResponse.        
        * @param aResponse UNSAF Binding Response or UNSAF Binding Error
        *         Response
        * @return Response code, or KErrNotFound if ERROR-CODE was not present.
        */
        TInt GetResponseCode( const CNATFWUNSAFMessage& aResponse ) const;
        
        /**
         * Checks if used protocol is valid. Will leave with KErrArgument
         * if protocol is not valid.
         *
         * @since   s60 3.2
         * @param   aProtocol   Used protocol ("stun" or "stun-relay").
         * @return  void
         */
        void CheckProtocolL( const TDesC8& aProtocol );
        
        /**
         * Gets the response code value from a ERROR-CODE attribute of the
         * aResponse.
         *
         * @since   s60 3.2
         * @return  true if relay used
         */
        TBool StunRelayUsed();
        
        /**
         * Method for new RTT sample measuring
         *
         * @since   s60 3.2
         * @return  void
         */
        void MeasureNewRTTSample();
            

    private: // Data

        MSTUNTransactionObserver& iObserver;

        // Binding Request message. Not owned.
        CNATFWUNSAFMessage& iRequest;

        // STUN server's address where the request is sent to
        const TInetAddr& iAddress;
        
        // Shared secret for calculating a hash
        const TDesC8& iSharedSecret;

        // Provides UDP sending services. Owned.
        CNATFWUNSAFUdpSender* iSender;
        
        // Provides sending services. Owned.
        CNATFWUNSAFMediaConnSender* iMediaConSender;

        // Amount of sent Binding Requests
        TInt iSendCount;
        
         // Initial retransmission interval, milliseconds
        TInt iInitialRetransmitInterval;

        // Current retransmission interval, milliseconds
        TInt iRetransmitInterval;

        // Public address received from a Binding Response's MAPPED-ADDRESS
        // attribute
        TInetAddr iMappedAddress;
        
        // Public address received from a Binding Response's XOR-MAPPED-ADDRESS
        // attribute
        TInetAddr iXorMappedAddress;

        // Public address received from a Binding Response's RELAY-ADDRESS
        // attribute
        TInetAddr iRelayAddress;

        // UNKNOWN-ATTRIBUTES received from a 420 error response. Owned.
        CNATFWUNSAFUnknownAttributesAttribute* iUnknownAttr;

        // ETrue if the request should be retransmitted with specified interval.
        // EFalse when retransmission must not be done any longer.
        TBool iRetransmit;
        
        // Has the transaction terminated
        TBool iTerminated;
        
        // Used natfw stream id
        TUint iStreamId;
        
        // Used natfw substream id
        TUint iSubstreamId;
        
        /**
         * Used protocol, either "stun" or "stun-relay"
         */
        HBufC8* iProtocol;
        
        /**
         * Timer value from attribute
         */
        TUint32 iTimerValue;
        
        /**
         * Destination Address
         */
        TInetAddr iDestAddr;

        /**
         * Contains state information, e.g. request is pending --> ETrue
         */
        TBool iRequestPending;
        
        /**
         * Used transport protocol
         */
        TTransportProtocol iTransportProtocol;

        /**
         * Indicates hometime for last sent transaction.
         */
        TTime iSendTime;
                
        /**
         * This is compare value to find out is the request retransmitted.
         * Default value is one.
         */
        TInt iRetransmitCompareValue;
        
        /**
         * ETrue if fingerprint will be added to transactions
         */
        TBool iUseFingerprint;
        
        /**
         * Disable resending packets in this transaction, only consume time
         * while this flag is ETrue.
         */
        TBool iDisableSending;
        
    private: // For testing purposes        

#ifdef TEST_EUNIT
        friend class CSTUNClientTest;
        friend class CSTUNBindingTest;
        friend class CSTUNBindingImplementationTest;
        friend class CSTUNTransactionTest;
#endif
        __DECLARE_TEST;
    };

#endif // C_CSTUNTRANSACTION_H
