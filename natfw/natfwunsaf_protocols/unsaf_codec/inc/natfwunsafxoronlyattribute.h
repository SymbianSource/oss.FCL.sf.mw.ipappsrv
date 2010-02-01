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




#ifndef CNATFWUNSAFXORONLYATTRIBUTE_H
#define CNATFWUNSAFXORONLYATTRIBUTE_H

// INCLUDES
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for STUN XOR-ONLY attribute.
 */
class CNATFWUNSAFXorOnlyAttribute : public CNATFWUNSAFAttribute
    {
    public: // Constructors and destructor
        /**
        * Creates a new instance of CNATFWUNSAFXorOnlyAttribute
        * @return A new instance of CNATFWUNSAFXorOnlyAttribute
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFXorOnlyAttribute* NewL();

        /**
        * Creates a new instance of CNATFWUNSAFXorOnlyAttribute and
        * pushes it to the CleanupStack.
        * @return A new instance of CNATFWUNSAFXorOnlyAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFXorOnlyAttribute* NewLC();

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
        * Destructor, deletes the resources of CNATFWUNSAFXorOnlyAttribute.
        */
        ~CNATFWUNSAFXorOnlyAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

        HBufC8* EncodeValueL() const;

    private: // Constructors

        CNATFWUNSAFXorOnlyAttribute();
    };

#endif // CNATFWUNSAFXORONLYATTRIBUTE_H


