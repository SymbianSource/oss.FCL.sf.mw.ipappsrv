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




#ifndef CNATFWUNSAFFINGERPRINTATTRIBUTE_H
#define CNATFWUNSAFFINGERPRINTATTRIBUTE_H

// INCLUDES
#include <in_sock.h>
#include "natfwunsafattribute.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF FINGERPRINT attribute.
 */
class CNATFWUNSAFFingerprintAttribute : public CNATFWUNSAFAttribute
    {

public: // Constructors and destructor

    /**
    * Constructs a CNATFWUNSAFFingerprintAttribute from
    * its value part's binary representation
    * @param aValue a value part of a FINGERPRINT-attribute
    * @return A new instance of CNATFWUNSAFFingerprintAttribute,
    *          ownership is transferred.
    */
    static CNATFWUNSAFFingerprintAttribute* DecodeL(
        const TDesC8& aValue);

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
    * Creates a new instance of CNATFWUNSAFFingerprintAttribute
    * @param aNATFWUNSAFMessage Message to be hashed
    * @return A new instance of CNATFWUNSAFFingerprintAttribute,
    *          ownership is transferred.
    */
    static CNATFWUNSAFFingerprintAttribute*
        NewL(CBufBase& aNATFWUNSAFMessage);

    /**
    * Creates a new instance of CNATFWUNSAFFingerprintAttribute and
    * pushes it to the CleanupStack.
    * @param aNATFWUNSAFMessage Message to be hashed
    * @return A new instance of CNATFWUNSAFFingerprintAttribute,
    *          ownership is transferred.
    */
    static CNATFWUNSAFFingerprintAttribute*
        NewLC(CBufBase& aNATFWUNSAFMessage);

    /**
    * Destructor, deletes the resources of
    * CNATFWUNSAFFingerprintAttribute.
    */
    ~CNATFWUNSAFFingerprintAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New functions

    /**
    * Computes a XORed CRC-32 for aNATFWUNSAFMessage
    * and compares the result with the value of this
    * CNATFWUNSAFFingerprintAttribute instance.
    * @param aNATFWUNSAFMessage UNSAF message as byte stream
    * @return ETrue Fingerprint check passes
    *         EFalse Otherwise, CRC-32 differs
    */
    IMPORT_C TBool CheckFingerprintL(
        const TDesC8& aNATFWUNSAFMessage) const;

protected: // New functions

    /**
    * Decodes the value part of the attribute and updates iHash
    * accordingly.
    * @param aValue value to be decoded
    */
    void DecodeValueL(const TDesC8& aValue);

    /**
    * Check the attribute's length field has a valid value.
    * @param aByteStream Stream containing attribute in binary format,
    *    starting from the type-field.
    * @leave KErrCorrupt If the length field in aByteStream is invalid
    */
    static void CheckLengthL(const TDesC8& aByteStream);

private: // Constructors

    CNATFWUNSAFFingerprintAttribute();

    /**
    * Second phase constructor.
    * @param aNATFWUNSAFMessage Message to be hashed
    */
    void ConstructL(CBufBase& aNATFWUNSAFMessage);

private: // New functions, for internal use

    /**
    * Compute a CCITT CRC-32 checksum over aNATFWUNSAFMessage,
    * Write the result into aCRC.
    * @param aNATFWUNSAFMessage Message to be checked
    * @param aCRC Resulting CRC-32 is written here
    */
    void ComputeCRC32(
        CBufBase& aNATFWUNSAFMessage,
        TUint32& aCRC) const;

private: // Data

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4,
        EXORedValue = 0x5354554e
        };

    TUint32 iCRC;

private: // For testing purposes

    UNIT_TEST(UT_CNATFWUNSAFFingerprintAttribute)
    };

#endif // CNATFWUNSAFFINGERPRINTATTRIBUTE_H


