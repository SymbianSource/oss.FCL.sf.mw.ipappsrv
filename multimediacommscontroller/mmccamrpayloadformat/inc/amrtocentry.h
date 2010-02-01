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




#ifndef AMRTOCENTRY_H
#define AMRTOCENTRY_H

#include <e32base.h>
#include <e32std.h>
#include "amrframe.h"

// MACROS


/**
* The Amr payload `table of contents' ( TOC ) entry in bandwidth efficient
* mode.
*
* @lib Mccamrplformat.dll
* @since Series 60 3.1
*/
class CAmrTocEntry : public CBase                                                                                                  
    {
    
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CAmrTocEntry* NewL();    

    /**
    * Destructor.
    */
    virtual ~CAmrTocEntry( );

public: // New functions

    /**
    * Encode the `table of contents' ( TOC ) entry fields into a given 
    * buffer at the given position. Values to encode are obtained from the
    * corresponding fields. Byte and bit positions are updated after 
    * encoding is done.
    * @since    Series 60 3.1
    * @param    aBuffer - [output] Buffer to store encoded data.
    * @param    aByteIndex - [input] Starting byte index ( starts from
    *           zero ).
    *           [output] Byte index updated after encoding.
    * @param    aBitIndex - [input] Starting bit index 
    *                        ( starts from zero ).
    *           [output] Bit index updated after encoding.
    */
    virtual void Encode( TUint8* aBuffer, TInt& aByteIndex, TInt& aBitIndex );

    /**
    * Decode the `table of contents' ( TOC ) entry fields from a given
    * buffer at the given position. Decoded values are returned in the
    * corresponding fields. Byte and bit positions are updated after
    * decoding is done.
    * @since    Series 60 3.1
    * @param    aBuffer - [input] Buffer to decode data from.
    * @param    aByteIndex - [input] Starting byte index ( starts from
    *           zero ).
    *           [output] Byte index updated after decoding.
    * @param    aBitIndex - [input] Starting bit index 
    *                        ( starts from zero ).
    *           [output] Bit index updated after decoding.
    */
    virtual void Decode( const TUint8* aBuffer, TInt& aByteIndex,
        TInt& aBitIndex );
    
protected: // Functions

    /**
    * Constructor.
    */
    CAmrTocEntry();

public:     // Data. Payload `table of contents' ( TOC ) entry fields
    
    // F flag ( 1 bit ); 1 if frame followed; 0 if last frame
    TUint8 iFrameFollowed;

    // Frame type ( FT, 4 bits )
    TAmrFrameType iFrameType;

    // Frame quality indicator ( Q, 1 bit ); 1 if frame OK if 0 damaged
    TUint8 iFrameQualityInd;
    
    };

#endif //        AMRTOCENTRY_H