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




#ifndef CNATFWUNSAFSERVERATTRIBUTE_H
#define CNATFWUNSAFSERVERATTRIBUTE_H

// INCLUDES
#include "natfwunsaftextattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF SERVER attribute.
 */
class CNATFWUNSAFServerAttribute : public CNATFWUNSAFTextAttribute
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
        * Creates a new instance of CNATFWUNSAFServerAttribute
        * @pre Server's length has to be a multiple of 4
        * @param aServer Server
        * @return A new instance of CNATFWUNSAFServerAttribute
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFServerAttribute* NewL(
            const TDesC8& aServer);

        /**
        * Creates a new instance of CNATFWUNSAFServerAttribute and
        * pushes it to the CleanupStack.
        * @pre Server's length has to be a multiple of 4
        * @param aServer Server
        * @return A new instance of CNATFWUNSAFServerAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFServerAttribute* NewLC(const TDesC8& aServer);

        /**
        * Destructor, deletes the resources of CNATFWUNSAFServerAttribute.
        */
        ~CNATFWUNSAFServerAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

    private: // Constructors

        CNATFWUNSAFServerAttribute();
    };

#endif // CNATFWUNSAFSERVERATTRIBUTE_H


