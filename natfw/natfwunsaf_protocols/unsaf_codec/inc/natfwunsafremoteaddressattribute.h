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




#ifndef CNATFWUNSAFREMOTEADDRESSATTRIBUTE_H
#define CNATFWUNSAFREMOTEADDRESSATTRIBUTE_H

#include "natfwunsafaddressattribute.h"

/**
 * Class for REMOTE-ADDRESS attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFRemoteAddressAttribute :
    public CNATFWUNSAFAddressAttribute
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
    * Creates a new instance of CNATFWUNSAFRemoteAddressAttribute
    * @param aAddress the address
    * @return A new instance of CNATFWUNSAFRemoteAddressAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRemoteAddressAttribute* NewL(
        const TInetAddr& aAddress);

    /**
    * Creates a new instance of CNATFWUNSAFRemoteAddressAttribute and
    * pushes it to the CleanupStack.
    * @param aAddress the address
    * @return A new instance of CNATFWUNSAFRemoteAddressAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRemoteAddressAttribute* NewLC(
        const TInetAddr& aAddress);

    /**
    * Destructor, deletes the resources of
    * CNATFWUNSAFRemoteAddressAttribute.
    */
    ~CNATFWUNSAFRemoteAddressAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

private: // Constructors

    CNATFWUNSAFRemoteAddressAttribute(const TInetAddr& aAddress);
    };

#endif // CNATFWUNSAFREMOTEADDRESSATTRIBUTE_H


