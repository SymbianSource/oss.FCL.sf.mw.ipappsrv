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




#ifndef CNATFWUNSAFDATAATTRIBUTE_H
#define CNATFWUNSAFDATAATTRIBUTE_H

#include "natfwunsaftextattribute.h"

/**
 * Class for DATA attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFDataAttribute : public CNATFWUNSAFTextAttribute
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
    * Creates a new instance of CNATFWUNSAFDataAttribute
    * @param aData Data
    * @return A new instance of CNATFWUNSAFDataAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFDataAttribute* NewL(const TDesC8& aData);

    /**
    * Creates a new instance of CNATFWUNSAFDataAttribute and
    * pushes it to the CleanupStack.
    * @param aData Data
    * @return A new instance of CNATFWUNSAFDataAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFDataAttribute* NewLC(const TDesC8& aData);

            /**
    * Destructor, deletes the resources of CNATFWUNSAFDataAttribute.
    */
    ~CNATFWUNSAFDataAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

private: // Constructors

    CNATFWUNSAFDataAttribute();
    };

#endif // CNATFWUNSAFDATAATTRIBUTE_H


