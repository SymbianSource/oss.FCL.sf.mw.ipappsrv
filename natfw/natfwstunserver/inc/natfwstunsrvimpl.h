/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implements functionality of NATFW STUN server client session
*
*/




#ifndef NATFWSTUNSRVIMPL_H
#define NATFWSTUNSRVIMPL_H

#include <e32base.h>
#include <in_sock.h>
#include "natfwconnectivityapidefs.h"

#include "mncmincomingconnectionobserver.h"

#include "natfwunsaftransactionid.h"

class MNATFWStunSrvObserver;
class CNATFWCredentials;
class CNATFWUNSAFMessage;
class MNcmConnectionMultiplexer;

/**
 * Implementation class of NATFW STUN server.
 *
 * @lib stunserver.lib
 * @since S60 v3.2
 */                                                   
NONSHARABLE_CLASS( CNATFWSTUNSrvImpl ) : public CBase, 
    public MNcmIncomingConnectionObserver
    {
    
    friend class ut_cnatfwstunsrvimpl;
    
public:
    
    /**
     * Two-phased constructor.
     * @param aObserver    A callback object to receive and handle
     *                     event notifications from NAT FW STUN server.
     * @param aObserver    Stun server observer
     * @param aMultiplexer A multiplexer
     */
    static CNATFWSTUNSrvImpl* NewL( MNATFWStunSrvObserver& aObserver,
        MNcmConnectionMultiplexer& aMultiplexer );

    /**
     * Destructor.
     */
    virtual ~CNATFWSTUNSrvImpl();

    /**
     * Adds authentication params which are used to authenticate
     * incoming binding requests.
     * 
     * Notifies the client of STUN requests that are valid.
     *  
     * @since S60 3.2
     * @param aIdentifications Identification array.
     * @return void
     */
    void AddAuthenticationParamsL( 
        const RPointerArray<CNATFWCredentials>& aIdentifications );
     
    /**
     * Removes specified authentication params from STUN server.
     *  
     * @since S60 3.2
     * @param aIdentifications Identification array.
     * @return void
     */
    void RemoveAuthenticationParamsL( 
        const RPointerArray<CNATFWCredentials>& aIdentifications );
        
    /**
     * Sets role for an agent. 
     *  
     * @since S60 3.2
     * @param aRole       Either controlling or controlled.  
     * @param aTieBreaker Value for determining which agent will change
     *                    Role if role conflict is present
     * @return void
     */             
    void SetRoleL( TNATFWIceRole aRole, TUint64 aTieBreaker );

// from base class MNcmIncomingConnectionObserver
 
    void IncomingMessageL( TUint aStreamId, 
        const TDesC8& aMessage, const TInetAddr& aLocalAddr, 
        const TInetAddr& aFromAddr, const TInetAddr& aPeerRemoteAddress,
        TBool& aConsumed );

private: // Constructors

    void ConstructL( MNcmConnectionMultiplexer& aMultiplexer );

    CNATFWSTUNSrvImpl( MNATFWStunSrvObserver& aObserver );
 
private: // private methods

    /**
     * Function that checks if role conflict is present between agents.
     *
     * @since S60 3.2   
     * @param aRequest UNSAF message.    
     * return ETrue if chosen role conflicts
     *        EFalse if not 
     */    
    TBool IsRoleConflictPresent( const CNATFWUNSAFMessage& aRequest );
        
    /**
     * Setting function for changing peer remote address.
     * @since S60 3.2
     * @param aFromAddr          Address the server saw in request.
     * @param aPeerRemoteAddress Peer remote address.
     * return void
     */
    void SetPeerRemoteAddress( const TInetAddr& aFromAddr,
        TInetAddr& aPeerRemoteAddress) const;
              
    /**
     * Decodes a byte stream and return the decoded UNSAF message.
     *  
     * @since S60 3.2
     * @param aData Incoming message data.
     * @return Decoded message, ownership is transferred.
     *         NULL if the stream didn't contain an UNSAF message.
     * @return One of the standard system-wide error codes.
     */    
    CNATFWUNSAFMessage* DecodeMessageL( const TDesC8& aData ) const;
    
    /**
     * Encodes an UNSAF message.
     *
     * @since S60 3.2
     * @param aRequest   UNSAF message.
     * @param aPassword  Key for checking the message integrity.
     * @param aErrorCode Error code.
     * @return
     */
    HBufC8* EncodeMessageL( const CNATFWUNSAFMessage& aRequest, 
        const TDesC8& aPassword, const TInt aErrorCode ) const;

    /**
     * Validates the request. Authenticate and check message integrity.
     * Sends binding response if everything goes right and error response
     * if there is missing argument or message integrity check fails.
     *  
     * @since S60 3.2
     * @param aRequest    UNSAF message.
     * @param aByteStream Same UNSAF message as aRequest, but in undecoded
     *                    format.
     * @return ETrue if needed attributes are present in binding request.
     *         EFalse if any needed attribute is missing.
     */
    TBool IsRequestValidL( const CNATFWUNSAFMessage& aRequest,
                           const TDesC8& aByteStream );
    
    /**
     * Verifies the integrity of a Binding Request.
     *  
     * @since S60 3.2
     * @param aRequest    UNSAF message.
     * @param aByteStream Same message as aRequest, but in undecoded format
     * @param aPassword   Key for checking the message integrity.
     * @return ETrue If integrity check succeeded.
     *         EFalse Integrity check fails.
     */    
    TBool CheckIntegrityL( const CNATFWUNSAFMessage& aRequest,
                           const TDesC8& aByteStream,
                           const TDesC8& aPassword );

    /**
     * Finds the credential which match for the given username and
     * returns the password.
     *  
     * @since S60 3.2
     * @param aRequest The received UNSAF message.
     * @param aError Returns error code via reference 
     * @return Password NUll if password not found.
     */                                     
    HBufC8* GetPassword( const CNATFWUNSAFMessage& aRequest, TInt& aError );
    
    /**
     * Creates and sends binding response or binding error response.
     *  
     * @since S60 3.2
     * @param aRequest   The received UNSAF message.
     * @param aErrorCode Error code.
     * @param aPassword  Password is used to calculate message integrity.
     * @return void
     */
    void SendResponseL( const CNATFWUNSAFMessage& aRequest, TInt aErrorCode, 
                        const TDesC8& aPassword );
    
    TBool IsRetransmittedRequest( const CNATFWUNSAFMessage& aRequest ) const;
    
    void SaveTransactionIdL( const TNATFWUNSAFTransactionID& aId );
    
    TInt FindTransactionId( const TNATFWUNSAFTransactionID& aId ) const;

private:
    
    /*
     * Observer for STUN request related notifies.
     */
    MNATFWStunSrvObserver& iObserver;
    
    /**
     * Identification array, password for authentication
     */
    RPointerArray<CNATFWCredentials> iIdentificationArray;
    
    /**
     * A Stream Id
     */
    TUint iStreamId; 
    
    /**
     * Remote address
     */
    TInetAddr iFromAddress;
    
    /**
     * Multiplexer instance
     * Not own.
     */
    MNcmConnectionMultiplexer* iMultiplexer;
    
    /**
     * Peer remote address
     */
    TInetAddr iPeerAddr;
    
    /**
     * Local address
     */
    TInetAddr iLocalAddress;
    
    /**
     * Username
     * Own.
     */
    HBufC8* iUsername;
    
    /**
     * Boolean indicating if data indication includes STUN binding request
     */
    TBool iRequestInsideIndication;
    
    /**
     * Current role
     */
    TInt iRole;
    
    /**
     * Chosen role conflicts with another agent if true
     */
    TBool iRoleConflict;
    
    /**
     * Value for determining which agent will change it's role if role 
     * conflict is present
     */
    TUint64 iTieBreaker;
    
    /**
     * Array containing latest transaction identifiers.
     */
    RArray<TNATFWUNSAFTransactionID> iTransactionIds;
    };

#endif // NATFWSTUNSRVIMPL_H
