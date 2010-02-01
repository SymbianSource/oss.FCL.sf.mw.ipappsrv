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




#ifndef CNATFWUNSAFRELAYADDRESSATTRIBUTE_H
#define CNATFWUNSAFRELAYADDRESSATTRIBUTE_H

#include "natfwunsafaddressattribute.h"

/**
 * Class for RELAY-ADDRESS attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFRelayAddressAttribute : public CNATFWUNSAFAddressAttribute
    {
public: // Constructors and destructor

    /**
    * Decodes a UNSAF attribute from a byte stream.
    * This version is for lookup table.
    * @param aByteStream Byte stream, containing attribute type,length and
    *    value.
    * @return Decoded UNSAF attribute, ownership is transferred.
    */
    static CNATFWUNSAFAttribute* DecodeAttributeL(
        const TDesC8& aByteStream);

    /**
    * Creates a new instance of CNATFWUNSAFRelayAddressAttribute
    * @param aAddress the address
    * @return A new instance of CNATFWUNSAFRelayAddressAttribute
    *          ownership is transferred.
    */
    static CNATFWUNSAFRelayAddressAttribute* NewL(
        const TInetAddr& aAddress);

    /**
    * Creates a new instance of CNATFWUNSAFRelayAddressAttribute and
    * pushes it to the CleanupStack.
    * @param aAddress the address
    * @return A new instance of CNATFWUNSAFRelayAddressAttribute,
    *          ownership is transferred.
    */
    static CNATFWUNSAFRelayAddressAttribute* NewLC(
        const TInetAddr& aAddress);

    /**
    * Destructor, deletes the resources of
    * CNATFWUNSAFRelayAddressAttribute.
    */
    ~CNATFWUNSAFRelayAddressAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

private: // Constructors

    CNATFWUNSAFRelayAddressAttribute(const TInetAddr& aAddress);
    };

#endif // CNATFWUNSAFRELAYADDRESSATTRIBUTE_H


