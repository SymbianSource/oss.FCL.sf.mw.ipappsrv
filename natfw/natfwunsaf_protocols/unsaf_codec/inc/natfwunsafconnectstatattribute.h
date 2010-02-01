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




#ifndef CNATFWUNSAFCONNECTSTATATTRIBUTE_H
#define CNATFWUNSAFCONNECTSTATATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for CONNECT-STAT attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFConnectStatAttribute : public CNATFWUNSAFAttribute
    {

public: // Enumerations

    enum TStatus
        {
        EListen           = 0x00000000,
        EEstablished      = 0x00000001,
        EClosed           = 0x00000002
        };

public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFConnectStatAttribute
    * @param aStatus Status of the connection
    * @return A new instance of CNATFWUNSAFConnectStatAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFConnectStatAttribute*
        NewL(TStatus aStatus);

    /**
    * Creates a new instance of CNATFWUNSAFConnectStatAttribute
    * @param aStatus Status of the connection
    * @return A new instance of CNATFWUNSAFConnectStatAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFConnectStatAttribute*
        NewLC(TStatus aStatus);

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
    * CNATFWUNSAFConnectStatAttribute.
    */
    ~CNATFWUNSAFConnectStatAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;
    
    HBufC8* EncodeValueL() const;

public: // New functions

    IMPORT_C CNATFWUNSAFConnectStatAttribute::TStatus 
        ConnectionStatus() const;

private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4,
        };

private: // Constructors

    CNATFWUNSAFConnectStatAttribute();

    void ConstructL(TStatus aStatus);

private: // Data

    TStatus iStatus;

    };

#endif // CNATFWUNSAFCONNECTSTATATTRIBUTE_H


