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




#ifndef CNATFWUNSAFNONCEATTRIBUTE_H
#define CNATFWUNSAFNONCEATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for NONCE attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFNonceAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor

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
    * Creates a new instance of CNATFWUNSAFNonceAttribute
    * @pre Nonce's length has to be a multiple of 4
    * @param aNonce Nonce
    * @return A new instance of CNATFWUNSAFNonceAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFNonceAttribute* NewL(
        const TDesC8& aNonce);

    /**
    * Creates a new instance of CNATFWUNSAFNonceAttribute and
    * pushes it to the CleanupStack.
    * @pre Nonce's length has to be a multiple of 4
    * @param aNonce Nonce
    * @return A new instance of CNATFWUNSAFNonceAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFNonceAttribute* NewLC(
        const TDesC8& aNonce);

    /**
    * Destructor, deletes the resources of CNATFWUNSAFNonceAttribute.
    */
    ~CNATFWUNSAFNonceAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New function

    /**
    * Returns the attribute value
    * @return Attribute value
    */
    IMPORT_C const TDesC8& Value() const;

private: // Constructors

    CNATFWUNSAFNonceAttribute();

    void ConstructL(const TDesC8& aNonce);

private: // Data

    HBufC8* iNonce;
    };

#endif // CNATFWUNSAFNONCEATTRIBUTE_H


