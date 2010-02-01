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




#ifndef CNATFWUNSAFMAPPEDADDRESSATTRIBUTE_H
#define CNATFWUNSAFMAPPEDADDRESSATTRIBUTE_H

// INCLUDES
#include "natfwunsafaddressattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for MAPPED-ADDRESS attribute.
 */
class CNATFWUNSAFMappedAddressAttribute : public CNATFWUNSAFAddressAttribute
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
        * Creates a new instance of CNATFWUNSAFMappedAddressAttribute
        * @param aAddress the address
        * @return A new instance of CNATFWUNSAFMappedAddressAttribute
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFMappedAddressAttribute* NewL(
            const TInetAddr& aAddress);

        /**
        * Creates a new instance of CNATFWUNSAFMappedAddressAttribute and
        * pushes it to the CleanupStack.
        * @param aAddress the address
        * @return A new instance of CNATFWUNSAFMappedAddressAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFMappedAddressAttribute* NewLC(
            const TInetAddr& aAddress);

        /**
        * Destructor, deletes the resources of
        * CNATFWUNSAFMappedAddressAttribute.
        */
        ~CNATFWUNSAFMappedAddressAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

    private: // Constructors

        CNATFWUNSAFMappedAddressAttribute(const TInetAddr& aAddress);
    };

#endif // CNATFWUNSAFMAPPEDADDRESSATTRIBUTE_H


