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




#ifndef CNATFWUNSAFPASSWORDATTRIBUTE_H
#define CNATFWUNSAFPASSWORDATTRIBUTE_H

// INCLUDES
#include "natfwunsaftextattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF PASSWORD attribute.
 */
class CNATFWUNSAFPasswordAttribute : public CNATFWUNSAFTextAttribute
    {
    public: // Constructors and destructor

        /**
        * Decodes an UNSAF attribute from a byte stream.
        * This version is for lookup table.
        * @param aByteStream Byte stream, containing attribute type, length and
        *    value.
        * @return Decoded UNSAF attribute, ownership is transferred.
        */
        static CNATFWUNSAFAttribute* DecodeAttributeL(
            const TDesC8& aByteStream);

        /**
        * Creates a new instance of CNATFWUNSAFPasswordAttribute
        * @pre Password's length has to be a multiple of 4
        * @param aPassword Password
        * @return A new instance of CNATFWUNSAFPasswordAttribute
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFPasswordAttribute* NewL(
            const TDesC8& aPassword);

        /**
        * Creates a new instance of CNATFWUNSAFPasswordAttribute and
        * pushes it to the CleanupStack.
        * @pre Password's length has to be a multiple of 4
        * @param aPassword Password
        * @return A new instance of CNATFWUNSAFPasswordAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFPasswordAttribute* NewLC(
            const TDesC8& aPassword);

        /**
        * Destructor, deletes the resources of CNATFWUNSAFPasswordAttribute.
        */
        ~CNATFWUNSAFPasswordAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

    private: // Constructors

        CNATFWUNSAFPasswordAttribute();
    };

#endif // CNATFWUNSAFPASSWORDATTRIBUTE_H


