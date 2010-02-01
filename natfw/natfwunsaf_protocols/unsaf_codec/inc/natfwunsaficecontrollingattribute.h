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




#ifndef CNATFWUNSAFICECONTROLLINGATTRIBUTE_H
#define CNATFWUNSAFICECONTROLLINGATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for ICE-CONTROLLING attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFIceControllingAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFIceControllingAttribute
    * @param aIceControlling IceControlling
    * @return A new instance of CNATFWUNSAFIceControllingAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFIceControllingAttribute*
        NewL(TUint64 aIceControlling);

    /**
    * Creates a new instance of CNATFWUNSAFIceControllingAttribute and
    * pushes it to the CleanupStack.
    * @param aIceControlling IceControlling value
    * @return A new instance of CNATFWUNSAFIceControllingAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFIceControllingAttribute*
        NewLC(TUint64 aIceControlling);

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
    * Destructor, deletes the resources of CNATFWUNSAFIceControllingAttribute.
    */
    ~CNATFWUNSAFIceControllingAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New functions

    /**
    * Returns the IceControlling value.
    * @return IceControlling value
    */
    IMPORT_C TUint64 IceControlling() const;

private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 8
        };

private: // Constructors

    CNATFWUNSAFIceControllingAttribute(TUint64 aIceControlling);

private: // Data

    TUint64 iIceControlling;

    };

#endif // CNATFWUNSAFICECONTROLLINGATTRIBUTE_H


