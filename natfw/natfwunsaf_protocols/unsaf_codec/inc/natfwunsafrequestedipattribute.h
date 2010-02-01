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




#ifndef CNATFWUNSAFREQUESTEDIPATTRIBUTE_H
#define CNATFWUNSAFREQUESTEDIPATTRIBUTE_H

#include "natfwunsafaddressattribute.h"

/**
 * Class for REQUESTED-IP attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFRequestedIpAttribute : public CNATFWUNSAFAddressAttribute
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
    * Creates a new instance of CNATFWUNSAFRequestedIpAttribute
    * @param aAddress the address
    * @return A new instance of CNATFWUNSAFRequestedIpAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRequestedIpAttribute* NewL(
        const TInetAddr& aAddress);

    /**
    * Creates a new instance of CNATFWUNSAFRequestedIpAttribute and
    * pushes it to the CleanupStack.
    * @param aAddress the address
    * @return A new instance of CNATFWUNSAFRequestedIpAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRequestedIpAttribute* NewLC(
        const TInetAddr& aAddress);

    /**
    * Destructor, deletes the resources of
    * CNATFWUNSAFRequestedIpAttribute.
    */
    ~CNATFWUNSAFRequestedIpAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

private: // Constructors

    CNATFWUNSAFRequestedIpAttribute(const TInetAddr& aAddress);
    };

#endif // CNATFWUNSAFREQUESTEDIPATTRIBUTE_H


