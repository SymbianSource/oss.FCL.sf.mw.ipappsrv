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




#ifndef CNATFWUNSAFREQUESTEDTRANSPORTATTRIBUTE_H
#define CNATFWUNSAFREQUESTEDTRANSPORTATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for REQUESTED-TRANSPORT attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFRequestedTransportAttribute : public CNATFWUNSAFAttribute
    {

public: // Enumerations

    enum TTransportProtocol
        {
        EProtocolUdp           = 0x00000000,
        EProtocolTcp           = 0x00000001
        };

public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFRequestedTransportAttribute
    * @param aProtocol Requested transport protocol
    * @return A new instance of CNATFWUNSAFRequestedTransportAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRequestedTransportAttribute*
        NewL(TTransportProtocol aProtocol);

    /**
    * Creates a new instance of CNATFWUNSAFRequestedTransportAttribute and
    * pushes it to the CleanupStack.
    * @param aProtocol Requested transport protocol
    * @return A new instance of CNATFWUNSAFRequestedTransportAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRequestedTransportAttribute*
        NewLC(TTransportProtocol aProtocol);

    /**
    * Decodes a UNSAF attribute from a byte stream.
    * This version is for lookup table.
    * @param aByteStream Byte stream, containing attribute type, length and
    *    value.
    * @return Decoded UNSAF attribute, ownership is transferred.
    */
    static CNATFWUNSAFAttribute* DecodeAttributeL(
        const TDesC8& aByteStream);

    /**
    * Destructor, deletes the resources of
    * CNATFWUNSAFRequestedTransportAttribute.
    */
    ~CNATFWUNSAFRequestedTransportAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;
    
public: // New functions

    IMPORT_C CNATFWUNSAFRequestedTransportAttribute::TTransportProtocol 
        TransportProtocol() const;
    
private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4,
        };

private: // Constructors

    CNATFWUNSAFRequestedTransportAttribute();

    void ConstructL(TTransportProtocol aProtocol);

private: // Data

    TTransportProtocol iProtocol;

    };

#endif // CNATFWUNSAFREQUESTEDTRANSPORTATTRIBUTE_H


