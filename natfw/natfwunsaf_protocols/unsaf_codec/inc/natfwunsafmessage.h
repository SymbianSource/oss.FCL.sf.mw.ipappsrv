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




#ifndef CNATFWUNSAFMESSAGE_H
#define CNATFWUNSAFMESSAGE_H

// INCLUDES
#include <e32base.h>
#include "natfwinternaldefs.h"    //UNIT_TEST
#include "natfwunsaftransactionid.h"
#include "natfwunsafattribute.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* An abstract base class for NATFWUNSAF requests, indications and responses.
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFMessage : public CBase
    {
    public: // Enumerations

        enum TMethod // Values are from RFC 3489 and TURN draft
            {
            EBinding = 0x0001,
            ESharedSecret = 0x0002,
            EAllocate = 0x0003,
            ESetActiveDestination = 0x0004,
            EConnect = 0x0005,
            //NOTE: ESend changed from 0x0006 to conform with old TURN draft-03
            //to get things to work in our test infra for now
            ESend = 0x0001,
            //NOTE: EData changed from 0x0007 to conform with old TURN draft-03
            //to get things to work in our test infra for now
            EData = 0x0002,
            EConnectStatus = 0x0008
            };

        enum TClass // Values are from RFC 3489 bis 08
            {
            ERequestMask = 0x0000,
            EIndicationMask = 0x0010,
            EResponseMask = 0x0100,
            EErrorResponseMask = 0x0110 
            };

        enum TType // Currently supported message types
            {
            EBindingRequest =
                EBinding | ERequestMask,
            EBindingIndication =
                EBinding | EIndicationMask,
            EBindingResponse =
                EBinding | EResponseMask,
            EBindingErrorResponse =
                EBinding | EErrorResponseMask,
            ESharedSecretRequest =
                ESharedSecret | ERequestMask,
            ESharedSecretResponse =
                ESharedSecret | EResponseMask,
            ESharedSecretErrorResponse =
                ESharedSecret | EErrorResponseMask,
            EAllocateRequest =
                EAllocate | ERequestMask,
            EAllocateResponse =
                EAllocate | EResponseMask,
            EAllocateErrorResponse =
                EAllocate | EErrorResponseMask,
            ESendIndication =
                ESend | EIndicationMask,
            EDataIndication =
                EData | EIndicationMask,
            ESetActiveDestinationRequest =
                ESetActiveDestination | ERequestMask,
            ESetActiveDestinationResponse =
                ESetActiveDestination | EResponseMask,
            ESetActiveDestinationErrorResponse =
                ESetActiveDestination | EErrorResponseMask,
            EConnectRequest =
                EConnect | ERequestMask,
            EConnectResponse =
                EConnect | EResponseMask,
            EConnectErrorResponse =
                EConnect | EErrorResponseMask,
            EConnectStatusIndication =
                EConnectStatus | EIndicationMask
            };

        enum THeader // RFC 3489 bis 08
            {
            EMessageTypeOffset     = 0,
            EMessageLengthOffset   = 2,
            EMagicCookieOffset     = 4,
            ETransactionIDOffset   = 8,

            EMessageTypePrefixMask = 0xc000,
            EMessageTypeMask       = 0x3fff,
            EMessageMethodMask     = 0x0001,
            EMessageClassMask      = 0x0110,
            EMagicCookie           = 0x2112A442,
            EHeaderSize            = 20
            };

    public: // Constructors and destructor

        /**
        * Destructor, deletes the resources of CNATFWUNSAFMessage.
        */
        virtual ~CNATFWUNSAFMessage();


    public: // New pure virtual functions

        /**
        * The type of the UNSAF message
        * @return UNSAF message type
        */
        virtual TType Type() const = 0;

        /**
        * Validates the UNSAF message. In other words checks that
        * all the mandatory attributes are present.
        * @return ETrue if the message is valid. Otherwise EFalse.
        */
        virtual TBool Validate() const = 0;

        /**
        * Determine if the specified attribute is allowed to be present in the
        * UNSAF message in question.
        * @return ETrue  Attribute is allowed
        *          EFalse Otherwise
        */
        virtual TBool IsAllowed(TUint16 aAttributeType) const = 0;

    public: // New functions

        /**
        * Obtains the transaction ID.
        * @return NATFWUNSAF transaction ID
        */
        IMPORT_C TNATFWUNSAFTransactionID TransactionID() const;

        /**
        * Checks whether a UNSAF attribute is present in this UNSAF message.
        * @param aType the attribute type
        * @return ETrue if the attribute is present, otherwise EFalse.
        */
        IMPORT_C TBool HasAttribute(CNATFWUNSAFAttribute::TType aType) const;

        /**
        * Gets a UNSAF attribute from the UNSAF message.
        * @param aType the attribute type
        * @return NULL if the attribute is not present.
        *         Otherwise a pointer to the attribute.
        *         The ownership is not transferred.
        */
        IMPORT_C CNATFWUNSAFAttribute* Attribute(
            CNATFWUNSAFAttribute::TType aType) const;

        /**
        * Adds a UNSAF attribute to the UNSAF message.
        * @param aAttribute the attribute to be added.
        *        The ownership is transferred.
        */
        IMPORT_C void AddAttributeL(CNATFWUNSAFAttribute* aAttribute);

        /**
        * Detaches a UNSAF attribute from the UNSAF message.
        * @param aAttribute the attribute to be detached.
        *        The caller gets the ownership of the attribute.
        */
        IMPORT_C void DetachAttribute(const CNATFWUNSAFAttribute* aAttribute);

        /**
        * Removes and deletes all attribute of the specified type from UNSAF
        * message.
        * @param aType Attribute type to delete
        */
        IMPORT_C void DeleteAttribute(TUint16 aType);

        /**
        * Encodes a UNSAF message without MESSAGE-INTEGRITY attribute.
        * @return The encoded UNSAF message. The ownership is transferred.
        */
        IMPORT_C CBufBase* EncodeL() const;

         /**
        * Encodes a UNSAF message and adds MESSAGE-INTEGRITY attribute.
        * @param aSharedSecret shared secret used to calculate the
        *        value of the MESSAGE-INTEGRITY attribute.
        * @param  aUseFingerprint flag indicating whether to add a FINGERPRINT
        *         attribute.
        * @return The encoded UNSAF message. The ownership is transferred.
        */
        IMPORT_C CBufBase* EncodeL(const TDesC8& aSharedSecret,
            TBool aUseFingerprint=EFalse) const;

        /**
        * When decoding the UNSAF message, an unrecognized mandatory attribute
        * has been encountered.
        * @post iUnknownMandatoryAttributes == ETrue
        */
        void UnknownMandatoryAttributeFound();

        /**
        * Tells if there were unrecognized mandatory attributes when the
        * message was decoded. These attributes have not been decoded.
        * @return ETrue Unrecognized mandatory attributes were encountered
        *         EFalse Otherwise
        */
        IMPORT_C TBool HasUnknownMandatoryAttributes() const;

        /**
        * Retrieves the UNSAF message length from a UNSAF message header.
        * @param aNATFWUNSAFMessage a UNSAF message
        * @return KErrNotFound if length field cannot be parsed.
        *         Otherwise the value of the length field.
        */
        static TInt MessageLength(const TDesC8& aNATFWUNSAFMessage);

        /**
        * Writes the length field into NATFWUNSAF header.
        * @param aNATFWUNSAFMessage Encoded UNSAF message whose length is to be
        *    calculated and written.
        */
        static void SetMessageLength(CBufBase& aNATFWUNSAFMessage);

        /**
        * Writes the length field into NATFWUNSAF header.
        * @param aNATFWUNSAFMessage Encoded UNSAF message whose
        *        length is to be set.
        * @param aLength Value to write into the length field.
        */
        static void SetMessageLength(CBufBase& aNATFWUNSAFMessage,
            TInt aLength);

    protected: // Constructors

        CNATFWUNSAFMessage(const TNATFWUNSAFTransactionID& aTransactionID);

    private:

        /**
        * Encodes the UNSAF message header.
        * @return The encoded UNSAF message header. The ownership is
        * transferred.
        */
        CBufBase* EncodeMessageHeaderLC() const;

    private: // Data

        TNATFWUNSAFTransactionID iTransactionID;

        //Owned
        RPointerArray<CNATFWUNSAFAttribute> iAttributes;

        //ETrue if an unrecognized mandatory attribute was encountered when
        //decoding this UNSAF message.
        TBool iUnknownMandatoryAttributes;

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFMessage)
        UNIT_TEST(UT_CNATFWUNSAFMessageFactory)
        UNIT_TEST(UT_CNATFWUNSAFBindingRequest)
        UNIT_TEST(UT_CNATFWUNSAFAllocateRequest)
        UNIT_TEST(UT_CNATFWUNSAFConnectRequest)
        UNIT_TEST(UT_CNATFWUNSAFSendIndication)
        UNIT_TEST(UT_CNATFWUNSAFSetActiveDestinationRequest)
        UNIT_TEST(UT_CNATFWUNSAFTcpRelayPacketFactory)
        UNIT_TEST(UT_CNATFWUNSAFBindingResponse)
    };

#endif // CNATFWUNSAFMESSAGE_H


