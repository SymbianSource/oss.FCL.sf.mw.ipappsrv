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




#ifndef CNATFWUNSAFMESSAGEINTEGRITYATTRIBUTE_H
#define CNATFWUNSAFMESSAGEINTEGRITYATTRIBUTE_H

// INCLUDES
#include <in_sock.h>
#include <hash.h>
#include "natfwunsafattribute.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for NATFWUNSAF MESSAGE-INTEGRITY attribute.
 */
class CNATFWUNSAFMessageIntegrityAttribute : public CNATFWUNSAFAttribute
    {
    public:

        //STUN RFC: size is 20 bytes
        typedef TBuf8<SHA1_HASH> TSHA1_Hash;

    public: // Constructors and destructor

        /**
        * Constructs a CNATFWUNSAFMessageIntegrityAttribute from
        * its value part's binary representation
        * @param aValue a value part of a MESSAGE-INTEGRITY-attribute
        * @return A new instance of CNATFWUNSAFMessageIntegrityAttribute,
        *          ownership is transferred.
        */
        static CNATFWUNSAFMessageIntegrityAttribute* DecodeL(
            const TDesC8& aValue);

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
        * Creates a new instance of CNATFWUNSAFMessageIntegrityAttribute
        * @param aSharedSecret Key for computing HMAC
        * @param aNATFWUNSAFMessage Message to be hashed
        * @param aLongTerm Flag for using long term shared secret
        * @return A new instance of CNATFWUNSAFMessageIntegrityAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFMessageIntegrityAttribute*
            NewL(const TDesC8& aSharedSecret,
                 CBufBase& aNATFWUNSAFMessage,
                 TBool aLongTerm=EFalse);

        /**
        * Creates a new instance of CNATFWUNSAFMessageIntegrityAttribute and
        * pushes it to the CleanupStack.
        * @param aSharedSecret Key for computing HMAC
        * @param aNATFWUNSAFMessage Message to be hashed
        * @param aLongTerm Flag for using long term shared secret
        * @return A new instance of CNATFWUNSAFMessageIntegrityAttribute,
        *          ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFMessageIntegrityAttribute*
            NewLC(const TDesC8& aSharedSecret,
                  CBufBase& aNATFWUNSAFMessage,
                  TBool aLongTerm=EFalse);

        /**
        * Destructor, deletes the resources of
        * CNATFWUNSAFMessageIntegrityAttribute.
        */
        ~CNATFWUNSAFMessageIntegrityAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

        HBufC8* EncodeValueL() const;

    public: // New functions

        /**
        * Computes a HMAC for aNATFWUNSAFMessage using aSharedSecret as a key,
        * and compares the result with the value of this
        * CNATFWUNSAFMessageIntegrityAttribute instance.
        * @param aNATFWUNSAFMessage UNSAF message as byte stream
        * @param aSharedSecret Key for computing the HMAC
        * @return ETrue  Message integrity check passes
        *          EFalse Otherwise, HMAC differs
        */
        IMPORT_C TBool CheckMessageIntegrityL(const TDesC8& aNATFWUNSAFMessage,
                                     const TDesC8& aSharedSecret) const;

        /**
        * Sets either long or short term credentials to use. This has an impact
        * on how the key for HMAC is calculated. The users of this object
        * should set this flag on whenever they receive a message containing a
        * REALM attribute.
        * @param aLongTerm ETrue if long term credentials are to be used
        *                  EFalse if short term credentials are to be used
        */
        IMPORT_C void UseLongTermCredentials(TBool aLongTerm);

    protected: // New functions

        /**
        * Decodes the value part of the attribute and updates iHash
        * accordingly.
        * @param aValue value to be decoded
        */
        void DecodeValueL(const TDesC8& aValue);

        /**
        * Check the attribute's length field has a valid value.
        * @param aByteStream Stream containing attribute in binary format,
        *    starting from the type-field.
        * @leave KErrCorrupt If the length field in aByteStream is invalid
        */
        static void CheckLengthL(const TDesC8& aByteStream);

    private: // Enumerations

        enum THashBlock
            {
            // The size of the data to be hashed, is a multiple of 64 bytes
            EInputBlockSize = 64
            };

    private: // Constructors

        CNATFWUNSAFMessageIntegrityAttribute();

        /**
        * Second phase constructor.
        * @param aSharedSecret Key for computing HMAC
        * @param aNATFWUNSAFMessage Message to be hashed
        * @param aLongTerm Flag for using long term shared secret
        */
        void ConstructL(const TDesC8& aSharedSecret,
                        CBufBase& aNATFWUNSAFMessage,
                        TBool aLongTerm);

    private: // New functions, for internal use

        /**
        * Compute a SHA1-HMAC hash over aNATFWUNSAFMessage,
        * using aSharedSecret as a key.
        * Write the result into aHash.
        * @param aSharedSecret Key for computing HMAC
        * @param aNATFWUNSAFMessage Message to be hashed
        * @param aHash Resulting hash is written here
        */
        void ComputeShortTermHMACL(const TDesC8& aSharedSecret,
                          CBufBase& aNATFWUNSAFMessage,
                          TSHA1_Hash& aHash) const;

        /**
        * Compute a SHA1-HMAC hash over aNATFWUNSAFMessage,
        * using the MD-5 hashed aSharedSecret as a key.
        * Write the result into aHash.
        * @param aSharedSecret Key for computing HMAC
        * @param aNATFWUNSAFMessage Message to be hashed
        * @param aHash Resulting hash is written here
        */
        void ComputeLongTermHMACL(const TDesC8& aSharedSecret,
                          CBufBase& aNATFWUNSAFMessage,
                          TSHA1_Hash& aHash) const;

        void DoComputeSHA1L(TSHA1_Hash& aHash,
                            CHMAC* aHmac,
                            CBufBase& aNATFWUNSAFMessage) const;

    private: // Data

        TSHA1_Hash iHash;

        TBool iLongTerm;

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFMessageIntegrityAttribute)
    };

#endif // CNATFWUNSAFMESSAGEINTEGRITYATTRIBUTE_H


