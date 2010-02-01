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




#ifndef CNATFWUNSAFCHANGEREQUESTATTRIBUTE_H
#define CNATFWUNSAFCHANGEREQUESTATTRIBUTE_H

// INCLUDES
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF CHANGE-REQUEST attribute.
 */
class CNATFWUNSAFChangeRequestAttribute : public CNATFWUNSAFAttribute
    {
    public: // Constructors and destructor
        /**
        * Creates a new instance of CNATFWUNSAFChangeRequestAttribute
        * @param aChangeIP Change IP flag
        * @param aChangePort Change port flag
        * @return A new instance of CNATFWUNSAFChangeRequestAttribute
        *          ownership is transferred.
        */
        static CNATFWUNSAFChangeRequestAttribute*
            NewL(TBool aChangeIP, TBool aChangePort);

        /**
        * Creates a new instance of CNATFWUNSAFChangeRequestAttribute and
        * pushes it to the CleanupStack.
        * @param aChangeIP Change IP flag
        * @param aChangePort Change port flag
        * @return A new instance of CNATFWUNSAFChangeRequestAttribute,
        *          ownership is transferred.
        */
        static CNATFWUNSAFChangeRequestAttribute*
            NewLC(TBool aChangeIP, TBool aChangePort);

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
        * Destructor, deletes the resources of
        * CNATFWUNSAFChangeRequestAttribute.
        */
        ~CNATFWUNSAFChangeRequestAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

        HBufC8* EncodeValueL() const;

    public: // New functions

        /**
        * Returns the change IP flag.
        * @return Flag value
        */
        TBool ChangeIP() const;

        /**
        * Returns the change port flag.
        * @return Flag value
        */
        TBool ChangePort() const;

    private:

        enum TEncodedValue
            {
            //Bit masks for obtaining the flag bits
            EChangeIPMask = 0x4,
            EChangePortMask = 0x2,
            //Size in bytes of the attribute value
            EAttributeValueSize = 4
            };

    private: // Constructors

        CNATFWUNSAFChangeRequestAttribute(TBool aChangeIP, TBool aChangePort);

    private: // Data

        TBool iChangeIP;
        TBool iChangePort;

    private: // For testing purposes

        __DECLARE_TEST;
    };

#endif // CNATFWUNSAFCHANGEREQUESTATTRIBUTE_H


