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




#ifndef CNATFWUNSAFICECONTROLLEDATTRIBUTE_H
#define CNATFWUNSAFICECONTROLLEDATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for ICE-CONTROLLED attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFIceControlledAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFIceControlledAttribute
    * @param aIceControlled IceControlled
    * @return A new instance of CNATFWUNSAFIceControlledAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFIceControlledAttribute*
        NewL(TUint64 aIceControlled);

    /**
    * Creates a new instance of CNATFWUNSAFIceControlledAttribute and
    * pushes it to the CleanupStack.
    * @param aIceControlled IceControlled value
    * @return A new instance of CNATFWUNSAFIceControlledAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFIceControlledAttribute*
        NewLC(TUint64 aIceControlled);

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
    * Destructor, deletes the resources of CNATFWUNSAFIceControlledAttribute.
    */
    ~CNATFWUNSAFIceControlledAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New functions

    /**
    * Returns the IceControlled value.
    * @return IceControlled value
    */
    IMPORT_C TUint64 IceControlled() const;

private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 8
        };

private: // Constructors

    CNATFWUNSAFIceControlledAttribute(TUint64 aIceControlled);

private: // Data

    TUint64 iIceControlled;

    };

#endif // CNATFWUNSAFICECONTROLLEDATTRIBUTE_H


