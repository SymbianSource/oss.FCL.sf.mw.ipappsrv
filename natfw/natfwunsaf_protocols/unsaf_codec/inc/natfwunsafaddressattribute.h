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




#ifndef CNATFWUNSAFADDRESSATTRIBUTE_H
#define CNATFWUNSAFADDRESSATTRIBUTE_H

// INCLUDES
#include <in_sock.h>
#include "natfwinternaldefs.h"    //UNIT_TEST
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * An abstract base class for UNSAF attributes having
 * the same syntax as MAPPED-ADDRESS.
 */
class CNATFWUNSAFAddressAttribute : public CNATFWUNSAFAttribute
    {
    public: // Constructors and destructor

        /**
        * Destructor, deletes the resources of CNATFWUNSAFAddressAttribute.
        */
        virtual ~CNATFWUNSAFAddressAttribute();

    public: // From CNATFWUNSAFAttribute

        HBufC8* EncodeValueL() const;

    public: // New functions

        /**
        * Returns modifiable address
        * @return the address
        */
        IMPORT_C TInetAddr& Address();

        /**
        * Returns non-modifiable address
        * @return the address
        */
        IMPORT_C const TInetAddr& Address() const;

    protected: // Constructors

        CNATFWUNSAFAddressAttribute();
        CNATFWUNSAFAddressAttribute(const TInetAddr& aInetAddr);

    protected: // New functions

        /**
        * Checks if the contained ip-address is an IPv6 address.
        * @return ETrue if IPv6 address, EFalse if IPv4 address.
        */
        TBool IsIPv6Address() const;

        /**
        * Checks if the ip-address is an IPv6 address.
        * @param aInetAddr IP-address
        * @return ETrue if IPv6 address, EFalse if IPv4 address.
        */
        TBool IsIPv6Address(const TInetAddr& aInetAddr) const;

        /**
        * Encodes the address part (IP address and port) of the contained
        * IP-address.
        * @param aResult Buffer where the result will be written
        */
        virtual void EncodeAddressL(TDes8& aResult) const;

        /**
        * Encodes the address part (IP address and port)
        * @param aInetAddr IP-address to encode
        * @param aResult Buffer where the result will be written
        */
        void EncodeAddressL(const TInetAddr& aInetAddr,
                            TDes8& aResult) const;

        /**
        * Decodes the value part of the attribute and updates iInetAddr
        * accordingly.
        * @param aValue Value part of the attribute to be decoded
        */
        void DecodeValueL(const TDesC8& aValue);

        /**
        * Check the attribute's length field has a valid value.
        * @param aByteStream Stream containing attribute in binary format,
        *    starting from the type-field.
        * @leave KErrCorrupt If the length field in aByteStream is invalid
        */
        static void CheckLengthL(const TDesC8& aByteStream);

    private: // New functions, for internal use

        /**
        * Decodes an IPv6 address and updates iInetAddr accordingly.
        * @param aValue Address part of the attribute to be decoded
        */
        void DecodeIPv6AddressL(const TDesC8& aValue);

    protected: // Data

        TInetAddr iInetAddr;

        //Values are from RFC 3489
        enum TEncodedValue
            {
            //Offset to fields inside the attribute value
            EFamilyOffset = 1,
            EPortOffset = 2,
            EAddressOffset = 4,

            KAddressFamilyIPv4 = 0x01,
            KAddressFamilyIPv6 = 0x02,

            //Size in bytes of the attribute value
            EAttributeValueSizeIPv4 = 8,
            EAttributeValueSizeIPv6 = 20
            };

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFMessage)
        UNIT_TEST(UT_CNATFWUNSAFMessageFactory)
        UNIT_TEST(UT_CNATFWUNSAFMappedAddressAttribute)
        UNIT_TEST(UT_CNATFWUNSAFAlternateServerAttribute)
        UNIT_TEST(UT_CNATFWUNSAFDestinationAddressAttribute)
        UNIT_TEST(UT_CNATFWUNSAFRelayAddressAttribute)
        UNIT_TEST(UT_CNATFWUNSAFRemoteAddressAttribute)
        UNIT_TEST(UT_CNATFWUNSAFRequestedIpAttribute)
    };

#endif // CNATFWUNSAFADDRESSATTRIBUTE_H


