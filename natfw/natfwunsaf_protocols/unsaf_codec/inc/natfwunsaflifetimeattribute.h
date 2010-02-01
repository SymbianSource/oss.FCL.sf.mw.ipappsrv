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




#ifndef CNATFWUNSAFLIFETIMEATTRIBUTE_H
#define CNATFWUNSAFLIFETIMEATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for LIFETIME attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFLifetimeAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFLifetimeAttribute
    * @param aLifetime Lifetime
    * @return A new instance of CNATFWUNSAFLifetimeAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFLifetimeAttribute*
        NewL(TInt aLifetime);

    /**
    * Creates a new instance of CNATFWUNSAFLifetimeAttribute and
    * pushes it to the CleanupStack.
    * @param aLifetime Lifetime
    * @return A new instance of CNATFWUNSAFLifetimeAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFLifetimeAttribute*
        NewLC(TInt aLifetime);

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
    * Destructor, deletes the resources of CNATFWUNSAFLifetimeAttribute.
    */
    ~CNATFWUNSAFLifetimeAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New functions

    /**
    * Returns the Lifetime.
    * @return Lifetime
    */
    IMPORT_C TInt Lifetime() const;


private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4
        };

private: // Constructors

    CNATFWUNSAFLifetimeAttribute();

    void ConstructL(TInt aLifetime);

private: // Data

    TInt iLifetime;

    };

#endif // CNATFWUNSAFLIFETIMEATTRIBUTE_H


