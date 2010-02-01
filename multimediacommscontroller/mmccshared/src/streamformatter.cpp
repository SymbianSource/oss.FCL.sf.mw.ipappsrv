/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    PayloadFormat plugin capable to read RTP payload containing
*                AMR audio.
*
*/




// INCLUDE FILES
#include "streamformatter.h"

// CONSTANTS
const TInt KBitsIn4Bytes = 32;
const TUint8 KZeroMask = 0xFF;
const TInt KBitIndex7 = 7;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TStreamEncoder::Initialize
// Initialize the stream encoder.
// -----------------------------------------------------------------------------
//
void TStreamEncoder::Initialize( TUint8* aBuffer, TInt aByteIndex, TInt aBitIndex )
    {
    iBuffer = aBuffer;
    iByteIndex = aByteIndex;
    iBitIndex = aBitIndex;
    };

// -----------------------------------------------------------------------------
// TStreamEncoder::Encode
// Put a number of bit values ( up to 32 bits ) into the bit-stream buffer 
// -----------------------------------------------------------------------------
//
void TStreamEncoder::Encode( TUint32 aValue, TInt aBitCount )
    {
    // Bit count has to be between 1 and 32 so if number of bits is out of range
    // return from here
    if ( aBitCount < 1 || aBitCount > KBitsIn4Bytes )
        {
        return;
        }
        
    TInt bitCount = aBitCount;
    TInt vacantBits;
    TInt bitsWritten;
    TUint32 oldValue;
    TUint32 newValue;
    TUint8* p;

    while ( bitCount > 0 )
        {
        p = iBuffer + iByteIndex;
        vacantBits = KBitsIn1Byte - iBitIndex;

        if ( bitCount >= vacantBits )
            {
            // get old value and zero out vacant bits
            oldValue = ( *p ) & ( KZeroMask << vacantBits );
            
            newValue = ( ( bitCount - vacantBits ) < KBitsIn4Bytes ) ?
                 TUint8( ( aValue >> ( bitCount - vacantBits ) ) ) : 0;
                 
            bitsWritten = vacantBits;
            }
        else
            {
            oldValue = ( *p ) 
                & ( ( KZeroMask << vacantBits ) 
                | ( KZeroMask >> ( KBitsIn1Byte - vacantBits + bitCount ) ) );
            newValue = TUint8( ( aValue << ( vacantBits-bitCount ) ) );
            bitsWritten = bitCount;
            }
        
        // zero out unwanted bits
        newValue &= ( KZeroMask ) >> ( iBitIndex );
         // write the byte
        *p = TUint8( ( oldValue | newValue ) );

        // update variables
        bitCount -= bitsWritten;
        iBitIndex += bitsWritten;
        if ( iBitIndex > KBitIndex7 )
            {
            iBitIndex &= KBitIndex7;
            iByteIndex += 1;
            }
        }
    };

// -----------------------------------------------------------------------------
// TStreamEncoder::Encode
// Put a number of bit values into the bit-stream buffer at current location.
// -----------------------------------------------------------------------------
//
void TStreamEncoder::Encode( const TUint8* aFromBuffer, TInt aFromByteIndex, 
        TInt aFromBitIndex, TInt aBitCount )
    {
    if ( aFromBitIndex == 0 && iBitIndex == 0 )
        {
        // Mem::Copy is applicable
        TUint8* desBuffer = iBuffer + iByteIndex;
        const TUint8* srcBuffer = aFromBuffer + aFromByteIndex;
        
        TInt byteLen = aBitCount / KBitsIn1Byte;
        TInt bitsLeft = aBitCount % KBitsIn1Byte;
        
        Mem::Copy( desBuffer, srcBuffer, byteLen );
        iByteIndex += byteLen;
        
        if ( bitsLeft != 0 )
            {
            TUint32 value;
            TStreamDecoder decoder;
            decoder.Initialize( aFromBuffer, aFromByteIndex + byteLen, aFromBitIndex );
            value = decoder.Decode( bitsLeft );
            Encode( value, bitsLeft );
            }
            
        return;
        }

    // Note that Mem::Copy is not applicable, have to do bit shifting
    TInt bitFilled = 0;
    TUint32 value;
    TStreamDecoder decoder;
    decoder.Initialize( aFromBuffer, aFromByteIndex, aFromBitIndex );

    while ( bitFilled < aBitCount )
        {
        if ( aBitCount - bitFilled < KBitsIn4Bytes )
            {
            value = decoder.Decode( aBitCount - bitFilled );
            Encode( value, ( aBitCount - bitFilled ) );
            bitFilled = aBitCount;
            }
        else
            {
            value = decoder.Decode( KBitsIn4Bytes );
            Encode( value, KBitsIn4Bytes );
            bitFilled += KBitsIn4Bytes;
            }
        }
    };

// -----------------------------------------------------------------------------
// TStreamDecoder::Initialize
// Initialize the stream decoder.
// -----------------------------------------------------------------------------
//
void TStreamDecoder::Initialize( const TUint8* aBuffer, TInt aByteIndex, 
        TInt aBitIndex )
    {
    iBuffer = aBuffer;
    iByteIndex = aByteIndex;
    iBitIndex = aBitIndex;
    };

// -----------------------------------------------------------------------------
// TStreamDecoder::Decode
// Get a number of bit values ( up to 32 bits ) from the bit-stream buffer at 
// current location.
// -----------------------------------------------------------------------------
//
TUint32 TStreamDecoder::Decode( TInt aBitCount )
    {
    // Bit count has to be between 1 and 32 so if number of bits is out of range
    // return from here
    if ( aBitCount < 1 || aBitCount > KBitsIn4Bytes )
        {
        return 0;
        }

    TInt bitCount = aBitCount;
    TInt availableBits;
    TInt bitsRead;
    TUint8* p;
    TUint32 temp;
    TUint32 value = 0;

    while ( bitCount > 0 )
        {
        p = const_cast<TUint8*>( iBuffer ) + iByteIndex;
        availableBits = KBitsIn1Byte - iBitIndex;

        // read one byte and zero out unwanted bits
        temp = ( *p ) & ( KZeroMask >> iBitIndex );

        // update return value
        if ( bitCount >= availableBits )
            {
            value = TUint8( temp ) | ( value<<availableBits );
            bitsRead = availableBits;
            }
        else
            {
            value = TUint8( ( temp >> ( availableBits - bitCount ) ) ) | ( value << bitCount );
            bitsRead = bitCount;
            }

        // update variables
        bitCount -= bitsRead;
        iBitIndex += bitsRead;
        if ( iBitIndex > KBitIndex7 )
            {
            iBitIndex &= KBitIndex7;
            iByteIndex += 1;
            }
        }
        
    return value;
    };

// -----------------------------------------------------------------------------
// TStreamDecoder::Decode
// Get a number of byte values from the bit-stream buffer at current location.
// -----------------------------------------------------------------------------
//
void TStreamDecoder::Decode( TUint8* aToBuffer, TInt aToByteIndex, 
        TInt aToBitIndex, TInt aBitCount )
    {
    if ( aToBitIndex == 0 && iBitIndex == 0 )
        {
        // Mem::Copy is applicable
        TUint8* desBuffer = aToBuffer + aToByteIndex;
        const TUint8* srcBuffer = iBuffer + iByteIndex;
        
        TInt byteLen = aBitCount / KBitsIn1Byte;
        TInt bitsLeft = aBitCount % KBitsIn1Byte;
        
        Mem::Copy( desBuffer, srcBuffer, byteLen );
        iByteIndex += byteLen;
        
        if ( bitsLeft != 0 )
            {
            TUint32 value;
            TStreamEncoder encoder;
            encoder.Initialize( aToBuffer, aToByteIndex + byteLen, aToBitIndex );
            value = Decode( bitsLeft );
            encoder.Encode( value, bitsLeft );
            }
            
        return;
        }

    // Note that Mem::Copy is not applicable, have to do bit shifting
    TInt bitFilled = 0;
    TUint32 value;
    TStreamEncoder encoder;
    encoder.Initialize( aToBuffer, aToByteIndex, aToBitIndex );

    while ( bitFilled < aBitCount )
        {
        // Check if number of remaining bits is less than 32.
        if ( aBitCount - bitFilled < KBitsIn4Bytes )
            {
            value = Decode( aBitCount - bitFilled );
            encoder.Encode( value, ( aBitCount - bitFilled ) );
            bitFilled = aBitCount;
            }
        else
            {
            value = Decode( KBitsIn4Bytes );
            encoder.Encode( value, KBitsIn4Bytes );
            bitFilled += KBitsIn4Bytes;
            }
        }
    };

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
