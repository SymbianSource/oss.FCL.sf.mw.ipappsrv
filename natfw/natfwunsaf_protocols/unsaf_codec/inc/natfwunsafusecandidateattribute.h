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




#ifndef CNATFWUNSAFUSECANDIDATEATTRIBUTE_H
#define CNATFWUNSAFUSECANDIDATEATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for USE_CANDIDATE attribute.
 * This attribute has no content according to ICE specification.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFUseCandidateAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFUseCandidateAttribute
    * @return A new instance of CNATFWUNSAFUseCandidateAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFUseCandidateAttribute* NewL();

    /**
    * Creates a new instance of CNATFWUNSAFUseCandidateAttribute and
    * pushes it to the CleanupStack.
    * @return A new instance of CNATFWUNSAFUseCandidateAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFUseCandidateAttribute* NewLC();

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
    * Destructor, deletes the resources of CNATFWUNSAFUseCandidateAttribute.
    */
    ~CNATFWUNSAFUseCandidateAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 0
        };

private: // Constructors

    CNATFWUNSAFUseCandidateAttribute();

    };

#endif // CNATFWUNSAFUSECANDIDATEATTRIBUTE_H


