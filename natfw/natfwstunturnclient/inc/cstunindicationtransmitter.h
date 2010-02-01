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
* Description:    Transmit indications to sender
*
*/




#ifndef C_STUNINDICATIONTRANSMITTER_H
#define C_STUNINDICATIONTRANSMITTER_H


#include <e32base.h>
#include <in_sock.h>
#include "cnatfwunsafmediaconnsender.h"
#include "mncmsenderobserver.h"
#include "cnatfwunsaftcprelaypacketsender.h"


class CNATFWUNSAFMessage;
class CNATFWUNSAFBindingRequest;
class MNcmConnectionMultiplexer;


/**
 *  Class for transmitting indications to multiplexer.
 *  Multiplexer acts as sender, meaning it will send indications 
 *  eventually to network .
 *
 *  @lib    natfwstunclient.lib
 *  @since  S60 v3.2
 */

class CStunIndicationTransmitter : public CBase
    {
public:

    /**
     * Two-phased constructor.
     * @param aMux          multiplexer
     * @param aStreamId     stream identifier
     * @param aConnectionId connection identifier
     */
    static CStunIndicationTransmitter* NewL( MNcmConnectionMultiplexer& aMux,
                                             TUint aStreamId,
                                             TUint aConnectionId );
    
    /**
     * Destructor.
     */
    virtual ~CStunIndicationTransmitter();

    /**
     * Transmits indication to multiplexer's send queue.
     *
     * @since   S60 v3.2
     * @param   aIndication     indication to send 
     * @return  void
     */
    void TransmitL( CNATFWUNSAFMessage& aIndication );
   
    /**
     * Transmits indication to multiplexer's send queue.
     *
     * @since   S60 v3.2
     * @param   aIndication     indication to send 
     * @param   aAddress        address to send to (destination)
     * @param   aSharedSecret   shared secret
     *                          optional - added if authentication needed
     * @param   aAddFingerprint fingerprint; added or not
     *                          optional - by default EFalse
     * @return  void
     */
    void TransmitL( CNATFWUNSAFMessage& aIndication,
                    const TInetAddr& aAddress,
                    const TDesC8& aSharedSecret=KNullDesC8(),
                    TBool aAddFingerprint=EFalse );
                    
    /**
     * Transmits indication to multiplexer's send queue by using TCP relay
     * packet sender.
     *
     * @since   S60 v3.2
     * @param   aMessage TCP relay message
     * @return  void
     */
    void TransmitL( const CNATFWUNSAFTcpRelayPacket& aMessage );                    

private:

    CStunIndicationTransmitter();
    
    CStunIndicationTransmitter( MNcmConnectionMultiplexer& aMux,
                                TUint aStreamId,
                                TUint aConnectionId );

    void ConstructL();


private: // data

    /**
     * multiplexer
     * not own.
     */
    MNcmConnectionMultiplexer& iMux;

    /**
     * stream identifier
     */
    TUint iStreamId;

    /**
     * connection identifier
     */
    TUint iConnectionId;
    
    /**
     * media connection sender
     * own.
     */
    CNATFWUNSAFMediaConnSender* iSender;
    
    /**
     * TCP relay packet sender
     * Own.
     */
    CNATFWUNSAFTcpRelayPacketSender* iTcpRelaySender; 
    };


#endif // C_STUNINDICATIONTRANSMITTER_H
