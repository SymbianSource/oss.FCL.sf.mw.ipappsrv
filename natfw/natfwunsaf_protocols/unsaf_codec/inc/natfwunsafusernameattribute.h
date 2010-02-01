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




#ifndef CNATFWUNSAFUSERNAMEATTRIBUTE_H
#define CNATFWUNSAFUSERNAMEATTRIBUTE_H

// INCLUDES
#include "natfwunsaftextattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF USERNAME attribute.
 */
class CNATFWUNSAFUsernameAttribute : public CNATFWUNSAFTextAttribute
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
        * Creates a new instance of CNATFWUNSAFUsernameAttribute
        * @pre Username's length has to be a multiple of 4
        * @param aUsername Username
        * @return A new instance of CNATFWUNSAFUsernameAttribute
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFUsernameAttribute* NewL(
            const TDesC8& aUsername);

        /**
        * Creates a new instance of CNATFWUNSAFUsernameAttribute and
        * pushes it to the CleanupStack.
        * @pre Username's length has to be a multiple of 4
        * @param aUsername Username
        * @return A new instance of CNATFWUNSAFUsernameAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFUsernameAttribute* NewLC(
            const TDesC8& aUsername);

        /**
        * Destructor, deletes the resources of CNATFWUNSAFUsernameAttribute.
        */
        ~CNATFWUNSAFUsernameAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

    private: // Constructors

        CNATFWUNSAFUsernameAttribute();
    };

#endif // CNATFWUNSAFUSERNAMEATTRIBUTE_H


