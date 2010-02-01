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




#ifndef CNATFWUNSAFTCPRELAYFACTORY_H
#define CNATFWUNSAFTCPRELAYFACTORY_H

#include <e32base.h>
#include "natfwunsaftcprelaypacket.h"

class CMessageLookupTable;

/**
* Class acts as a factory for UNSAF TCP relay packets
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFTcpRelayPacketFactory : public CBase
    {
public: // Constructors and destructors

    /**
    * Creates a CNATFWUNSAFTcpRelayPacketFactory instance
    * @return A new instance of CNATFWUNSAFTcpRelayPacketFactory
    */
    IMPORT_C static CNATFWUNSAFTcpRelayPacketFactory* NewL();

    /**
    * Creates a CNATFWUNSAFTcpRelayPacketFactory instance and pushes it to
    * CleanupStack
    * @return A new instance of CNATFWUNSAFTcpRelayPacketFactory
    */
    IMPORT_C static CNATFWUNSAFTcpRelayPacketFactory* NewLC();

    /**
    * Destructor
    */
    IMPORT_C virtual ~CNATFWUNSAFTcpRelayPacketFactory();

public: // New functions

    /**
    * Parses the UNSAF relay packet length from a UNSAF TCP relay packet
    * (bytes 3 and 4) and returns the parsed length.
    * @param aNATFWUNSAFTcpRelayPacket a UNSAF message
    * @return KErrNotFound if length field cannot be parsed.
    *         Otherwise the value of the length field.
    */
    TInt MessageLength(const TDesC8& aNATFWUNSAFTcpRelayPacket) const;

    /**
    * Parses a UNSAF relay including the 4 byte UNSAF
    * relay packet header
    * @param aNATFWUNSAFTcpRelayPacket a UNSAF TCPrelay packet
    * @return A new instance of CNATFWUNSAFTcpRelayPacket.
    *         The ownership is transferred.
    */
    IMPORT_C CNATFWUNSAFTcpRelayPacket* DecodeL(
        const TDesC8& aNATFWUNSAFTcpRelayPacket) const;

    /**
    * Creates a UNSAF relay packet instance
    * @param aType a UNSAF TCP relay packet type
    * @param aData Data to be relayed
    * @return A new instance of CNATFWUNSAFTcpRelayPacket.
    *         The ownership is transferred.
    */
    CNATFWUNSAFTcpRelayPacket* CreateL(
        const CNATFWUNSAFTcpRelayPacket::TRelayType& aType,
        const TDesC8& aData) const;

private:

    // Constructor
    CNATFWUNSAFTcpRelayPacketFactory();

    /**
    * Decodes a UNSAF relay packet header, instantiates a UNSAF relay
    * packet and pushes it into CleanupStack.
    * @param aNATFWUNSAFTcpRelayPacket Byte stream to decode as a
    *        UNSAF TCP relay packet
    * @return A new instance of CNATFWUNSAFTcpRelayPacket.
    * The ownership is transferred.
    */
    CNATFWUNSAFTcpRelayPacket* DecodeHeaderLC(
        const TDesC8& aNATFWUNSAFTcpRelayPacket) const;

    };

#endif // CNATFWUNSAFTCPRELAYPACKETFACTORY_H


