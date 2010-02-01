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




#ifndef CNATFWUNSAFTIMERVALATTRIBUTE_H
#define CNATFWUNSAFTIMERVALATTRIBUTE_H

// INCLUDES
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF TIMER_VAL attribute.
 */
class CNATFWUNSAFTimerValAttribute : public CNATFWUNSAFAttribute
    {
public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFTimerValAttribute
    * @param aTimerVal TimerVal in milliseconds
    * @return A new instance of CNATFWUNSAFTimerValAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFTimerValAttribute*
        NewL(TUint aTimerVal);

    /**
    * Creates a new instance of CNATFWUNSAFTimerValAttribute and
    * pushes it to the CleanupStack.
    * @param aTimerVal TimerVal in milliseconds
    * @return A new instance of CNATFWUNSAFTimerValAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFTimerValAttribute*
        NewLC(TUint aTimerVal);

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
    * Destructor, deletes the resources of CNATFWUNSAFTimerValAttribute.
    */
    ~CNATFWUNSAFTimerValAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;

public: // New functions

    /**
    * Returns the TimerVal.
    * @return TimerVal
    */
    IMPORT_C TUint TimerVal() const;


private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4
        };

private: // Constructors

    CNATFWUNSAFTimerValAttribute(TUint aTimerVal);

private: // Data

    TUint iTimerVal;

    };

#endif // CNATFWUNSAFTIMERVALATTRIBUTE_H


