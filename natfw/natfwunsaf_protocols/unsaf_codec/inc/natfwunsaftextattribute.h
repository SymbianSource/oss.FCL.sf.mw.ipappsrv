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




#ifndef CNATFWUNSAFTEXTATTRIBUTE_H
#define CNATFWUNSAFTEXTATTRIBUTE_H

// INCLUDES
#include <in_sock.h>
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * An abstract base class for variable length text attributes.
 * The length of the value must be a multiple of 4 measured in bytes.
 */
class CNATFWUNSAFTextAttribute : public CNATFWUNSAFAttribute
    {
    public: // Constructors and destructor

        /**
        * Destructor, deletes the resources of CNATFWUNSAFTextAttribute.
        */
        virtual ~CNATFWUNSAFTextAttribute();

    public: // From CNATFWUNSAFAttribute

        HBufC8* EncodeValueL() const;

    public: // New functions

        /**
        * Returns the attribute value
        * @return Attribute value
        */
        IMPORT_C const TDesC8& Value() const;

    protected: // Constructors

        CNATFWUNSAFTextAttribute();

        /**
        * Second phase constructor.
        * @param aValue Attribute value, does not contain the attribute length
        */
        void ConstructL(const TDesC8& aValue);

    protected: // New functions

        /**
        * Decodes an UNSAF attribute from a byte stream.
        * @param aByteStream Byte stream, containing attribute type, length and
        *    value.
        */
        void DecodeL(const TDesC8& aByteStream);

    private:

        enum TValueGranularity
            {
            //The length of iValue has to be a multiple of 4
            EGranularity = 4
            };

    private: // Data

        //Attribute value, owned
        HBufC8* iValue;

    private: // For testing purposes

        __DECLARE_TEST;
    };

#endif // CNATFWUNSAFTEXTATTRIBUTE_H


