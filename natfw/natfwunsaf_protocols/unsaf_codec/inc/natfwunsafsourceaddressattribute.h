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




#ifndef CNATFWUNSAFSOURCEADDRESSATTRIBUTE_H
#define CNATFWUNSAFSOURCEADDRESSATTRIBUTE_H

// INCLUDES
#include "natfwunsafaddressattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for SOURCE-ADDRESS attribute.
 */
class CNATFWUNSAFSourceAddressAttribute : public CNATFWUNSAFAddressAttribute
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
        * Creates a new instance of CNATFWUNSAFSourceAddressAttribute
        * @param aAddress the address
        * @return A new instance of CNATFWUNSAFSourceAddressAttribute
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFSourceAddressAttribute* NewL(
            const TInetAddr& aAddress);

        /**
        * Creates a new instance of CNATFWUNSAFSourceAddressAttribute and
        * pushes it to the CleanupStack.
        * @param aAddress the address
        * @return A new instance of CNATFWUNSAFSourceAddressAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFSourceAddressAttribute* NewLC(
            const TInetAddr& aAddress);

        /**
        * Destructor, deletes the resources of
        * CNATFWUNSAFSourceAddressAttribute.
        */
        ~CNATFWUNSAFSourceAddressAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

    private: // Constructors

        CNATFWUNSAFSourceAddressAttribute(const TInetAddr& aAddress);
    };

#endif // CNATFWUNSAFSOURCEADDRESSATTRIBUTE_H


