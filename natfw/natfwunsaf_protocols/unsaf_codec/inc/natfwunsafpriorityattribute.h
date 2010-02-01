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




#ifndef CNATFWUNSAFPRIORITYATTRIBUTE_H
#define CNATFWUNSAFPRIORITYATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for PRIORITY attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFPriorityAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFPriorityAttribute
    * @param aPriority Priority
    * @return A new instance of CNATFWUNSAFPriorityAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFPriorityAttribute*
        NewL(TUint aPriority);

    /**
    * Creates a new instance of CNATFWUNSAFPriorityAttribute and
    * pushes it to the CleanupStack.
    * @param aPriority Priority
    * @return A new instance of CNATFWUNSAFPriorityAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFPriorityAttribute*
        NewLC(TUint aPriority);

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
    * Destructor, deletes the resources of CNATFWUNSAFPriorityAttribute.
    */
    ~CNATFWUNSAFPriorityAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New functions

    /**
    * Returns the Priority.
    * @return Priority
    */
    IMPORT_C TUint Priority() const;


private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4
        };

private: // Constructors

    CNATFWUNSAFPriorityAttribute(TUint aPriority);

private: // Data

    TUint iPriority;

    };

#endif // CNATFWUNSAFPRIORITYATTRIBUTE_H


