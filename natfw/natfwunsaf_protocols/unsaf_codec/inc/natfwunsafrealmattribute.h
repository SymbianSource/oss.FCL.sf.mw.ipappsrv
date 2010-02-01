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




#ifndef CNATFWUNSAFREALMATTRIBUTE_H
#define CNATFWUNSAFREALMATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for REALM attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFRealmAttribute : public CNATFWUNSAFAttribute
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
    * Creates a new instance of CNATFWUNSAFRealmAttribute
    * @pre Realm's length has to be a multiple of 4
    * @param aRealm Realm
    * @return A new instance of CNATFWUNSAFRealmAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRealmAttribute* NewL(
        const TDesC8& aRealm);

    /**
    * Creates a new instance of CNATFWUNSAFRealmAttribute and
    * pushes it to the CleanupStack.
    * @pre Realm's length has to be a multiple of 4
    * @param aRealm Realm
    * @return A new instance of CNATFWUNSAFRealmAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRealmAttribute* NewLC(
        const TDesC8& aRealm);

    /**
    * Destructor, deletes the resources of CNATFWUNSAFRealmAttribute.
    */
    ~CNATFWUNSAFRealmAttribute();

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

    CNATFWUNSAFRealmAttribute();

    void ConstructL(const TDesC8& aRealm);

private: // Data

    HBufC8* iRealm;
    };

#endif // CNATFWUNSAFREALMATTRIBUTE_H


