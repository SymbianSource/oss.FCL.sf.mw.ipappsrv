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




#ifndef CNATFWUNSAFERRORCODEATTRIBUTE_H
#define CNATFWUNSAFERRORCODEATTRIBUTE_H

// INCLUDES
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF ERROR-CODE attribute.
 */
class CNATFWUNSAFErrorCodeAttribute : public CNATFWUNSAFAttribute
    {
    public: // Constructors and destructor
        /**
        * Creates a new instance of CNATFWUNSAFErrorCodeAttribute
        * @pre aResponseCode >= 100 && aResponseCode < 700
        * @param aResponseCode Response code
        * @param aReasonPhrase Reason phrase
        * @return A new instance of CNATFWUNSAFErrorCodeAttribute
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFErrorCodeAttribute*
            NewL(TInt aResponseCode, const TDesC8& aReasonPhrase);

        /**
        * Creates a new instance of CNATFWUNSAFErrorCodeAttribute and
        * pushes it to the CleanupStack.
        * @pre aResponseCode >= 100 && aResponseCode < 700
        * @param aResponseCode Response code
        * @param aReasonPhrase Reason phrase
        * @return A new instance of CNATFWUNSAFErrorCodeAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFErrorCodeAttribute*
            NewLC(TInt aResponseCode, const TDesC8& aReasonPhrase);

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
        * Destructor, deletes the resources of CNATFWUNSAFErrorCodeAttribute.
        */
        ~CNATFWUNSAFErrorCodeAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

        HBufC8* EncodeValueL() const;

    public: // New functions

        /**
        * Returns the response code.
        * @return Response code
        */
        IMPORT_C TInt ResponseCode() const;

        /**
        * Returns the reason phrase.
        * @return Reason phrase.
        */
        IMPORT_C const TDesC8& ReasonPhrase() const;

    private:

        enum TEncodedValue
            {
            //Bit mask for obtaining the class-value from the encoded attribute
            EClassMask = 0x7,

            //Offset to fields inside the attribute value
            EClassOffset = 2,
            ENumberOffset = 3,
            EReasonPhraseOffset = 4,

            //The length of the encoded reason phrase has to be a multiple of 4
            EGranularity = 4,

            E100 = 100
            };

    private: // Constructors

        CNATFWUNSAFErrorCodeAttribute();

        void ConstructL(TInt aResponseCode, const TDesC8& aReasonPhrase);

    private: // New functions

        /**
        * Check the aResponseCode is within allowed range.
        * @param aResponseCode Response code
        * @return ETrue if response code is valid, EFalse otherwise
        */
        TBool CheckResponseCode(TInt aResponseCode) const;

        /**
        * Check the length of the whole attribute (including header).
        * @param aLength Reason phrase lenght
        * @leave KErrCorrupt If the length field is invalid
        */
        static void CheckLengthL(TInt aLength);

        TInt EncodedReasonPhraseLength() const;

    private: // Data

        TInt iResponseCode;

        HBufC8* iReasonPhrase; //owned

    private: // For testing purposes

        __DECLARE_TEST;
    };

#endif // CNATFWUNSAFERRORCODEATTRIBUTE_H


