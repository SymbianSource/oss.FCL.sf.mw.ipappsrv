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




#ifndef CNATFWUNSAFXORMAPPEDADDRESSATTRIBUTE_H
#define CNATFWUNSAFXORMAPPEDADDRESSATTRIBUTE_H

// INCLUDES
#include "natfwunsafaddressattribute.h"
#include "natfwunsaftransactionid.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * Class for XOR-MAPPED-ADDRESS attribute.
 */
class CNATFWUNSAFXorMappedAddressAttribute : public CNATFWUNSAFAddressAttribute
    {
    public: // Constructors and destructor

        /**
        * Decodes an attribute from a byte stream.
        * This version is for lookup table.
        * @param aByteStream Byte stream, containing attribute type,
        *        length and value.
        * @return Decoded UNSAF attribute, ownership is transferred.
        */
        static CNATFWUNSAFAttribute* DecodeAttributeL(
            const TDesC8& aByteStream);

        /**
        * Creates a new instance of CNATFWUNSAFXorMappedAddressAttribute
        * @param aAddress the address
        * @param aTransactionID transaction ID to be used when
        *        encoding the attribute value.
        * @return A new instance of CNATFWUNSAFXorMappedAddressAttribute
        *         ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFXorMappedAddressAttribute* NewL(
            const TInetAddr& aAddress,
            const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Creates a new instance of CNATFWUNSAFXorMappedAddressAttribute and
        * pushes it to the CleanupStack.
        * @param aAddress the address
        * @param aTransactionID transaction ID to be used when
        *        encoding the attribute value.
        * @return A new instance of CNATFWUNSAFXorMappedAddressAttribute,
        *         ownership is transferred.
        */
        IMPORT_C static CNATFWUNSAFXorMappedAddressAttribute* NewLC(
            const TInetAddr& aAddress,
            const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Destructor, deletes the resources of
        * CNATFWUNSAFXorMappedAddressAttribute.
        */
        ~CNATFWUNSAFXorMappedAddressAttribute();

    public: // From CNATFWUNSAFAttribute

        CNATFWUNSAFAttribute::TType Type() const;

    public: // New functions

        /**
        * Sets the transaction ID used when encoding the attribute value.
        * @param aTransactionID transaction ID to be used when
        *        encoding the attribute value.
        */
        void SetTransactionIdL(const TNATFWUNSAFTransactionID& aTransactionID);

        /**
        * Gets the transaction ID used when encoding the attribute value.
        */
        const TNATFWUNSAFTransactionID& TransactionID() const;

        /**
        * Sets the transaction ID used when encoding the attribute value.
        * Modifies the address by XORing it with the new transaction ID.
        * @param aTransactionID transaction ID to be used when
        *        encoding the attribute value.
        */
        IMPORT_C void SetTransactionIDAndXorL(
            const TNATFWUNSAFTransactionID& aTransactionID);

    private: // From CNATFWUNSAFAddressAttribute

        virtual void EncodeAddressL(TDes8& aResult) const;

    private: // New functions

        TUint16 XorPort() const;

        TInetAddr XorAddress() const;

    private: // Constructors

        CNATFWUNSAFXorMappedAddressAttribute();

        CNATFWUNSAFXorMappedAddressAttribute(const TInetAddr& aAddress);

    private: // Data

        TNATFWUNSAFTransactionID iTransactionID;
    };

#endif // CNATFWUNSAFXORMAPPEDADDRESSATTRIBUTE_H


