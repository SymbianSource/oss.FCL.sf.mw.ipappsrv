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




#ifndef CNATFWUNSAFMAGICCOOKIEATTRIBUTE_H
#define CNATFWUNSAFMAGICCOOKIEATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for MAGIC-COOKIE attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFMagicCookieAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFMagicCookieAttribute
    * @return A new instance of CNATFWUNSAFMagicCookieAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFMagicCookieAttribute* NewL();

    /**
    * Creates a new instance of CNATFWUNSAFMagicCookieAttribute and
    * pushes it to the CleanupStack.
    * @return A new instance of CNATFWUNSAFMagicCookieAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFMagicCookieAttribute* NewLC();

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
    * Destructor, deletes the resources of CNATFWUNSAFMagicCookieAttribute.
    */
    ~CNATFWUNSAFMagicCookieAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New functions

    /**
    * Returns the magic cookie.
    * @return Magic cookie
    */
    IMPORT_C TUint32 MagicCookie() const;

private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4,
        EMagicCookieValue = 0x72c64bc6
        };

private: // Constructors

    CNATFWUNSAFMagicCookieAttribute();

    };

#endif // CNATFWUNSAFMAGICCOOKIEHATTRIBUTE_H


