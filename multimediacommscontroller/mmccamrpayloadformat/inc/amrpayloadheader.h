/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef AMRPAYLOADHEADER_H
#define AMRPAYLOADHEADER_H

#include <e32base.h>
#include "amrpayloadformatter.h"


// FORWARD DECLARATIONS
class TStreamEncoder;
class TStreamDecoder;

// MACROS


// CLASS DECLARATION

/**
* The Amr payload header in bandwidth efficient mode.
*
* @lib Mccamrplformat.dll
* @since Series 60 3.1
*/
class CAmrPayloadHeader : public CBase
    {

public:  // Constructors and destructor
        
    public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CAmrPayloadHeader* NewL();
        
    /**
    * Destructor.
    */
    ~CAmrPayloadHeader();


public: // New functions

    /**
    * Encode the payload header fields into a given buffer at the given
    * position. Values to encode are obtained from the corresponding 
    * header fields. Byte and bit positions are updated after encoding 
    * is done.
    * @since    Series 60 3.1
    * @param    aBuffer - [output] Buffer to store encoded data.
    * @param    aByteIndex - [input] Starting byte index ( starts from
    *           zero ).
    *           [output] Byte index updated after encoding.
    * @param    aBitIndex - [input] Starting bit index 
    *                        ( starts from zero ).
    *           [output] Bit index updated after encoding.
    */
    virtual void Encode( TUint8* aBuffer, TInt& aByteIndex,
        TInt& aBitIndex );

    /**
    * Decode the payload header from a given buffer at the given position.
    * Decoded values are returned in the corresponding header fields.
    * Byte and bit positions are updated after decoding is done.
    * @since    Series 60 3.1
    * @param    aBuffer - [input] Buffer to decode data from.
    * @param    aByteIndex - [input] Starting byte index ( starts from
    *           zero ).
    *           [output] Byte index updated after decoding.
    * @param    aBitIndex - [input] Starting bit index 
    *                         ( starts from zero ).
    *           [output] Bit index updated after decoding.
    */
    virtual void Decode( const TUint8* aBuffer, TInt& aByteIndex, 
            TInt& aBitIndex );
            
    
protected: // Functions

    /**
    * Constructor.
    */
    CAmrPayloadHeader();

public:     // Data. Payload header fields

    // Codec mode request ( CMR, 4 bits )
    TAmrModeRequest iCmr;  

    };

#endif      // AMRPAYLOADHEADER_H