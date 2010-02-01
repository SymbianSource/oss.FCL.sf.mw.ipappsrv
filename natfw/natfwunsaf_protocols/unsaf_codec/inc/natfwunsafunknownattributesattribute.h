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




#ifndef CNATFWUNSAFUNKNOWNATTRIBUTESATTRIBUTE_H
#define CNATFWUNSAFUNKNOWNATTRIBUTESATTRIBUTE_H

// INCLUDES
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF UNKNOWN-ATTRIBUTES attribute.
 */
class CNATFWUNSAFUnknownAttributesAttribute : public CNATFWUNSAFAttribute
    {
    public: // Constructors and destructor

        /**
        * Creates a new instance of CNATFWUNSAFUnknownAttributesAttribute
        * @return A new instance of CNATFWUNSAFUnknownAttributesAttribute
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFUnknownAttributesAttribute* NewL();

        /**
        * Creates a new instance of CNATFWUNSAFUnknownAttributesAttribute and
        * pushes it to the CleanupStack.
        * @return A new instance of CNATFWUNSAFUnknownAttributesAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFUnknownAttributesAttribute* NewLC();

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
        * CNATFWUNSAFUnknownAttributesAttribute.
        */
        IMPORT_C virtual ~CNATFWUNSAFUnknownAttributesAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

        HBufC8* EncodeValueL() const;

    public: // New functions

        /**
        * Adds an attribute type.
        * @leave KErrNoMemory If no memory to add the new attribute type
        * @leave KErrAlreadyExists If aAttributeType is already present.
        */
        void AddContainedAttributeL(TUint16 aAttributeType);

        /**
        * Remove an attribute type.
        * @return KErrNone If attribute type was successfully removed
        * @return KErrNotFound If attribute type was not present.
        */
        TInt RemoveContainedAttribute(TUint16 aAttributeType);

        /**
        * Gets an array containing all the attribute types contained in this
        * attribute.
        * @return Contained attribute types. Ownership of the
        *         array or the items inside it is not transferred.
        */
        IMPORT_C const RArray<TUint32>& ContainedAttributes() const;

    private:

        enum TEncodedValue
            {
            //Size in bytes of a single entry (attribute) in the encoded value
            //part of UNKNOWN-ATTRIBUTES
            EElementSize = 2,

            //The length of the encoded value has to be a multiple of 4
            EGranularity = 4
            };

    private: // Constructors

        CNATFWUNSAFUnknownAttributesAttribute();

    private: // New functions

        /**
        * Check the length of attribute's value part is valid
        * @param aLength Length of the attribute's value part
        * @leave KErrCorrupt If the length is invalid
        */
        static void CheckLengthL(TInt aLength);

        void DecodeValueL(const TDesC8& aValue);

    private: // Data

        //Use 32bit values, since RArray stores objects in the array as 4 byte
        //aligned quantities.
        RArray<TUint32> iContainedAttributes;

    private: // For testing purposes

        __DECLARE_TEST;
    };

#endif // CNATFWUNSAFUNKNOWNATTRIBUTESATTRIBUTE_H


