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




#ifndef CNATFWUNSAFRESPONSEADDRESSATTRIBUTE_H
#define CNATFWUNSAFRESPONSEADDRESSATTRIBUTE_H

// INCLUDES
#include "natfwunsafaddressattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for RESPONSE-ADDRESS attribute.
 */
class CNATFWUNSAFResponseAddressAttribute : public CNATFWUNSAFAddressAttribute
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
        * Creates a new instance of CNATFWUNSAFResponseAddressAttribute
        * @param aAddress the address
        * @return A new instance of CNATFWUNSAFResponseAddressAttribute,
        *          ownership is transferred.
        */
        static CNATFWUNSAFResponseAddressAttribute* NewL(
            const TInetAddr& aAddress);

        /**
        * Creates a new instance of CNATFWUNSAFResponseAddressAttribute and
        * pushes it to the CleanupStack.
        * @param aAddress the address
        * @return A new instance of CNATFWUNSAFResponseAddressAttribute,
        *          ownership is transferred.
        */
        static CNATFWUNSAFResponseAddressAttribute* NewLC(
            const TInetAddr& aAddress);

        /**
        * Destructor, deletes the resources of
        * CNATFWUNSAFResponseAddressAttribute.
        */
        ~CNATFWUNSAFResponseAddressAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

    private: // Constructors

        CNATFWUNSAFResponseAddressAttribute(const TInetAddr& aAddress);
    };

#endif // CNATFWUNSAFRESPONSEADDRESSATTRIBUTE_H


