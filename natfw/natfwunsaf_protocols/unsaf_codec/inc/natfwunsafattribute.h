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




#ifndef CNATFWUNSAFATTRIBUTE_H
#define CNATFWUNSAFATTRIBUTE_H

// INCLUDES
#include <e32base.h>
#include "natfwinternaldefs.h"    //UNIT_TEST

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * An abstract base class for UNSAF attributes.
 */
class CNATFWUNSAFAttribute : public CBase
    {
    public: // Enumerations

        enum TType // Values are from RFC 3489 and TURN-draft
            {
            EMappedAddress = 0x0001,
            EResponseAddress = 0x0002,
            EChangeRequest = 0x0003,
            ESourceAddress = 0x0004,
            EChangedAddress = 0x0005,
            EUsername = 0x0006,
            EPassword = 0x0007,
            EMessageIntegrity = 0x0008,
            EErrorCode = 0x0009,
            EUnknownAttributes = 0x000a,
            EReflectedFrom = 0x000b,
            ELifetime = 0x000d,
            EMagicCookie = 0x000f,
            EBandwidth = 0x0010,
            EDestinationAddress = 0x0011,
            ERemoteAddress = 0x0012,
            EData = 0x0013,
            ERealm = 0x0014,
            ENonce = 0x0015,
            ERelayAddress = 0x0016,
            ERequestedPortProps = 0x0018,
            ERequestedTransport = 0x0019,
            EXorMappedAddress = 0x0020,
            ETimerVal = 0x0021,
            ERequestedIp = 0x0022,
            EConnectStat = 0x0023,
            EPriority = 0x0024,
            EUseCandidate = 0x0025,
            EXorOnly = 0x8021,      //was 0x0021, but clashes with TIMER_VAL
            EServer = 0x8022,
            EAlternateServer = 0x8023,
            EFingerprint = 0x8028,
            EIceControlled = 0x8029,
            EIceControlling = 0x802a
            };

    public: // Constructors and destructor

        /**
        * Destructor, deletes the resources of CNATFWUNSAFAttribute.
        */
        virtual ~CNATFWUNSAFAttribute();

    public: // New pure virtual functions

        /**
        * The type of the UNSAF attribute
        * @return UNSAF attribute type
        */
        virtual CNATFWUNSAFAttribute::TType Type() const = 0;

    public: // New functions

        /**
        * Encodes the UNSAF attribute including its type and length
        * @return The encoded UNSAF attribute. The ownership is
        *         transferred.
        */
        HBufC8* EncodeL() const;

        /**
        * Parses the byte stream for a UNSAF attribute type and length.
        * Leaves if stream does not contain enough data
        * @param aByteStream Stream to parse
        * @param aType OUT: attribute type
        * @param aTotalLength OUT: Total length of the attribute, including
        *    type, length and value elements
        * @leave KErrArgument If the stream doesn't contain enough data
        */
        static void ParseTypeAndLengthL(const TDesC8& aByteStream,
                                        TUint16& aType,
                                        TUint16& aLength);

        /**
        * Checks if the UNSAF attribute indicated by aType is mandatory to
        * understand.
        * @param aType UNSAF attribute type
        * @return ETrue  Attribute is mandatory and must be understood
        *         EFalse Attribute is optional
        */
        static TBool IsMandatory(TUint16 aType);

        /**
        * Checks if the UNSAF attribute indicated by aType is word boundary
        * aligned by definition.
        * @param aType UNSAF attribute type
        * @return ETrue  Attribute is word boundary aligned and must not be
        *                padded out.
        *         EFalse Attribute may need padding out.
        */
        static TBool IsWordBoundaryAligned(TUint16 aType);

    protected: // Enums

        enum TEncodedAttributeOffset
            {
            //Offset to fields inside the encoded attribute
            ETypeOffset = 0,
            ELengthOffset = 2,
            EValueOffset = 4
            };

    protected: // Constructors

        CNATFWUNSAFAttribute();


    protected: // New pure virtual functions

        /**
        * Encodes the UNSAF attribute value.
        * Used from CNATFWUNSAFAttribute::EncodeL.
        * @return The encoded UNSAF attribute value.
        *         The ownership is transferred.
        */
        virtual HBufC8* EncodeValueL() const = 0;

    protected: // New functions

        /**
        * Parses the byte stream and returns the value of length element.
        * @param aByteStream Stream to parse
        * @return Length of value element
        * @leave KErrArgument If the stream doesn't contain enough data
        */
        static TUint16 ParseLengthL(const TDesC8& aByteStream);

    private: // For testing purposes

        UNIT_TEST(CNATFWUNSAFMessageTest)
        UNIT_TEST(CNATFWUNSAFMessageFactoryTest)
        UNIT_TEST(CNATFWUNSAFMappedAddressAttributeTest)
        UNIT_TEST(CNATFWUNSAFMessageIntegrityAttributeTest)
    };

#endif // CNATFWUNSAFATTRIBUTE_H


