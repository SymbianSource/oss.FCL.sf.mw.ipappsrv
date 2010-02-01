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




#ifndef CNATFWUNSAFBANDWIDTHATTRIBUTE_H
#define CNATFWUNSAFBANDWIDTHATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for BANDWIDTH attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFBandwidthAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFBandwidthAttribute
    * @param aBandwidth Bandwidth
    * @return A new instance of CNATFWUNSAFBandwidthAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFBandwidthAttribute*
        NewL(TInt aBandwidth);

    /**
    * Creates a new instance of CNATFWUNSAFBandwidthAttribute and
    * pushes it to the CleanupStack.
    * @param aBandwidth Bandwidth
    * @return A new instance of CNATFWUNSAFBandwidthAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFBandwidthAttribute*
        NewLC(TInt aBandwidth);

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
    * Destructor, deletes the resources of CNATFWUNSAFBandwidthAttribute.
    */
    ~CNATFWUNSAFBandwidthAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New functions

    /**
    * Returns the bandwidth.
    * @return Bandwidth
    */
    IMPORT_C TInt Bandwidth() const;


private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4
        };

private: // Constructors

    CNATFWUNSAFBandwidthAttribute();

    void ConstructL(TInt aBandwidth);

private: // Data

    TInt iBandwidth;

    };

#endif // CNATFWUNSAFBANDWIDTHATTRIBUTE_H


