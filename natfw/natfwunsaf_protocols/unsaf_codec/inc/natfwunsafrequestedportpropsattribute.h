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




#ifndef CNATFWUNSAFREQUESTEDPORTPROPSATTRIBUTE_H
#define CNATFWUNSAFREQUESTEDPORTPROPSATTRIBUTE_H

#include "natfwunsafattribute.h"

/**
 * Class for REQUESTED-PORT-PROPS attribute.
 *
 *  @lib natfwunsafprotocols.lib
 *  @since S60 3.2
 */
class CNATFWUNSAFRequestedPortPropsAttribute : public CNATFWUNSAFAttribute
    {

public: // Enumerations

    enum TPortAlignmentPref
        {
        EAlignmentPrefNone           = 0x0000,
        EAlignmentPrefOdd            = 0x0001,
        EAlignmentPrefEven           = 0x0002,
        EAlignmentPrefEvenAndNextOdd = 0x0003
        };

public: // Constructors and destructor
    /**
    * Creates a new instance of CNATFWUNSAFRequestedPortPropsAttribute
    * @param aPortRangePref Requested port range preference
    * @param aPortAlignmentPref Requested port alignment preference
    * @return A new instance of CNATFWUNSAFRequestedPortPropsAttribute
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRequestedPortPropsAttribute*
        NewL(TPortAlignmentPref aRangePref, TUint16 aPort);

    /**
    * Creates a new instance of CNATFWUNSAFRequestedPortPropsAttribute and
    * pushes it to the CleanupStack.
    * @param aPortRangePref Requested port range preference
    * @param aPortAlignmentPref Requested port alignment preference
    * @return A new instance of CNATFWUNSAFRequestedPortPropsAttribute,
    *          ownership is transferred.
    */
    IMPORT_C static CNATFWUNSAFRequestedPortPropsAttribute*
        NewLC(TPortAlignmentPref aRangePref, TUint16 aPort);

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
    * Destructor, deletes the resources of CNATFWUNSAFRequestedPortPropsAttribute.
    */
    ~CNATFWUNSAFRequestedPortPropsAttribute();

public: // From CNATFWUNSAFAttribute

    CNATFWUNSAFAttribute::TType Type() const;

    HBufC8* EncodeValueL() const;
    
public: // New functions

    IMPORT_C TUint16 Port() const;

    IMPORT_C CNATFWUNSAFRequestedPortPropsAttribute::TPortAlignmentPref
        PortAlignmentPref() const;

private:

    enum TEncodedValue
        {
        //Size in bytes of the attribute value
        EAttributeValueSize = 4,
        EPortOffset = 2,
        EPortAlignmentMask  = 0x0003
        };

private: // Constructors

    CNATFWUNSAFRequestedPortPropsAttribute(TPortAlignmentPref aAlignmentPref,
        TUint16 aPort);

private: // Data

    TPortAlignmentPref iPortAlignmentPref;

    TUint16 iPort;

    };

#endif // CNATFWUNSAFREQUESTEDPORTPROPSATTRIBUTE_H


