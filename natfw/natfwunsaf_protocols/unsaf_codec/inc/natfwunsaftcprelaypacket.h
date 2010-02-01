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




#ifndef CNATFWUNSAFTCPRELAYPACKET_H
#define CNATFWUNSAFTCPRELAYPACKET_H

#include <e32base.h>

/**
* A class for UNSAF TCP relay packets
*
*  @lib natfwunsafprotocols.lib
*  @since S60 3.2
*/
class CNATFWUNSAFTcpRelayPacket : public CBase
    {
public: // Enumerations

    enum TFrame // draft-ietf-behave-turn-02
        {
        EFrameTypeOffset     = 0,
        EFrameLengthOffset   = 2,
        
        EFrameTypePrefixMask = 0xff00,
        EFrameSize           = 4
        };
    
    enum TRelayType
        {
        EFrameTypeStun       = 0x02,
        EFrameTypeData       = 0x03,
        };
        
public: // Constructors and destructor
    
    /**
    * Creates a CNATFWUNSAFTcpRelayPacket instance
    * @param aData Data to be relayed
    * @param aType Type of relay (STUN/DATA)
    * @return A new instance of CNATFWUNSAFTcpRelayPacket
    */
    IMPORT_C static CNATFWUNSAFTcpRelayPacket*
                NewL(const TDesC8& aData, const TRelayType aType);

    /**
    * Creates a CNATFWUNSAFTcpRelayPacket instance
    * @param aData Data to be relayed
    * @param aType Type of relay (STUN/DATA)
    * @return A new instance of CNATFWUNSAFTcpRelayPacket
    */
    IMPORT_C static CNATFWUNSAFTcpRelayPacket*
                NewLC(const TDesC8& aData, const TRelayType aType);

    /**
    * Creates a CNATFWUNSAFTcpRelayPacket instance.
    * @param aData Data to be relayed
    * @param aType Type of relay (STUN/DATA)
    * @return A new instance of CNATFWUNSAFTcpRelayPacket
    *       ownership is transferred.
    */
    static CNATFWUNSAFTcpRelayPacket*
        CreateL(const TDesC8& aData, const TRelayType aType);

    /**
    * Destructor, deletes the resources of CNATFWUNSAFTcpRelayPacket.
    */
    virtual ~CNATFWUNSAFTcpRelayPacket();

public: // New functions

    /**
    * Encodes a relay packet
    * @return The encoded UNSAF TCP relay packet. The ownership is
    *         transferred.
    */
    IMPORT_C CBufBase* EncodeL() const;

    /**
    * Returns the relayed data
    * @return relayed data
    */
    IMPORT_C const TDesC8& Data() const;

    /**
    * Returns the relay type
    * @return relay type
    */
    IMPORT_C TRelayType Type() const;

    /**
    * Retrieves the UNSAF TCP relay packet length from a UNSAF TCP relay
    * packet header.
    * @param aNATFWUNSAFTcpRelayPacket a UNSAF TCP relay packet
    * @return KErrNotFound if length field cannot be parsed.
    *         Otherwise the value of the length field.
    */
    static TInt RelayLength(const TDesC8& aNATFWUNSAFTcpRelayPacket);

    /**
    * Writes the length field into WUNSAF TCP relay packet header.
    * @param aNATFWUNSAFTcpRelayPacket Encoded UNSAF TCP relay packet
    *        whose length is to be calculated and written.
    */
    static void SetRelayLength(CBufBase& aNATFWUNSAFTcpRelayPacket);

    /**
    * Writes the length field into NATFWUNSAF TCP relay packet header.
    * @param aNATFWUNSAFTcpRelayPacket Encoded UNSAF TCP relay packet whose
    *        length is to be set.
    * @param aLength Value to write into the length field.
    */
    static void SetRelayLength(CBufBase& aNATFWUNSAFTcpRelayPacket,
        TInt aLength);

protected: // Constructors

    CNATFWUNSAFTcpRelayPacket();
    
    /**
    * Second phase constructor.
    * @param aData data to be relayed
    * @param aType type of relay (STUN/DATA)
    */
    void ConstructL(const TDesC8& aData, const TRelayType aType);

private:

    /**
    * Encodes the UNSAF TCP relay packet header.
    * @return The encoded UNSAF relay packet header. The ownership is
    *         transferred.
    */
    CBufBase* EncodeRelayHeaderLC() const;
    
    /**
    * Checks that the length of relayed data is valid
    * @param aLength Length of the relayed data
    * @leave KErrCorrupt If the length is invalid
    */
    void CheckLengthL(TInt aLength) const;

private: // Data
    
    //Relayed data, owned
    HBufC8* iData;
    
    //Relay type
    TRelayType iType;
    
private:

    enum TValueDataGranularity
        {
        //The length of iValue has to be a multiple of 4
        EGranularity = 4
        };
    };

#endif // CNATFWUNSAFTCPRELAYPACKET_H


