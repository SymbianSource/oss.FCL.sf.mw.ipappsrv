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
* Description:    Formatter classes for encoding/decoding of a bit stream.
*
*/



#ifndef STREAMFORMATTER_H
#define STREAMFORMATTER_H

// INCLUDES
#include <e32std.h>

// CONSTS
const TInt KBitsIn1Byte = 8;

// CLASS DECLARATION

/**
* The formatter class that sequentially encodes a bit stream.
*
* @lib      Mccamrplformat.lib
* @since    Series 60 3.0
*/
class TStreamEncoder
    {
    public:

        /**
        * Constructor.
        */
        TStreamEncoder() : iBuffer( NULL ), iByteIndex( 0 ), iBitIndex( 0 )
            {   };

        /**
        * Destructor.
        */
        virtual ~TStreamEncoder() { iBuffer = NULL; };

    public:

        /**
        * Initialize the stream encoder.
        * This function must be called before calling any other functions.
        * @since Series 60 3.0
        * @param aBuffer - [input] Pointer to the bit-stream buffer.
        * @param aByteIndex - [input] Starting byte index ( starts from zero ).
        * @param aBitIndex - [input] Starting bit index ( starts from zero ).
        * @return void
        */
        void Initialize( TUint8* aBuffer, TInt aByteIndex, TInt aBitIndex );

        /**
        * Put a number of bit values ( up to 32 bits ) into the bit-stream 
        * buffer at current location. Bits are put starting from MSB of the
        * buffer byte and advancing to LSB.
        * @since    Series 60 3.0
        * @param    aValue - [input] Value to put.
        * @param    aBitCount - [input] Number of bits to put; between 1 and 32,
        *           both inclusive.
        */
        void Encode( TUint32 aValue, TInt aBitCount );

        /**
        * Put a number of bit values into the bit-stream buffer at current 
        * location.
        * @since    Series 60 3.0
        * @param    aFromBuffer - [input] Buffer to get byte values from.
        * @param    aByteIndex - [input] Starting byte index ( starts from
        *           zero ).
        * @param    aBitIndex - [input] Starting bit index ( starts from zero ).
        * @param    aBitCount - [input] Number of bits to put.
        */
        void Encode( const TUint8* aFromBuffer, TInt aFromByteIndex,
                TInt aFromBitIndex, TInt aBitCount );

        /**
        * Get the current byte index value.
        * @since    Series 60 3.0
        * @return   TInt - Current byte index value.
        */
        inline TInt ByteIndex() const 
                { return iByteIndex; };

        /**
        * Get the current bit index value.
        * @since    Series 60 3.0
        * @return   TInt - Current bit index value.
        */
        inline TInt BitIndex() const 
                { return iBitIndex; };

        /**
        * Set the current byte index value.
        * @since    Series 60 3.0
        * @param    aByteIndex New byte index
        */
        inline void SetByteIndex( TInt aByteIndex ) 
                { iByteIndex = aByteIndex; };

        /**
        * Set the current bit index value.
        * @since    Series 60 3.0
        * @param    aBitIndex New bit index
        */
        inline void SetBitIndex( TInt aBitIndex ) 
                { iBitIndex = aBitIndex; };

    private:
    
        // Pointer to the bit-stream buffer.
        TUint8* iBuffer;

        // Current byte index ( starts from zero )
        TInt  iByteIndex;
        
        // Current bit index ( starts from zero )
        TInt  iBitIndex;
    };

/**
* The formatter class that sequentially decodes a bit stream.
*
* @lib      Mccamrplformat.lib
* @since    Series 60 3.0
*/
class TStreamDecoder
    {
    public:

        /**
        * Constructor.
        */
        TStreamDecoder() : iBuffer( NULL ), iByteIndex( 0 ), iBitIndex( 0 )
            {   };

        /**
        * Destructor.
        */
        virtual ~TStreamDecoder() { iBuffer = NULL; };

    public:

        /**
        * Initialize the stream decoder.
        * This function must be called before calling any other functions.
        * @since    Series 60 3.0
        * @param    aBuffer - [input] Pointer to the bit-stream buffer.
        * @param    aByteIndex - [input] Starting byte index ( starts from 
        *           zero ).
        * @param    aBitIndex - [input] Starting bit index ( starts from zero ).
        */
        void Initialize( const TUint8* aBuffer, TInt aByteIndex, 
                TInt aBitIndex );

        /**
        * Get a number of bit values ( up to 32 bits ) from the bit-stream
        * buffer at current location. Bits are extracted starting from MSB of
        * the buffer byte and advancing to LSB.
        * Returned value: ( MSB )-> bit31, bit30 .... bit1, bit0 <-( LSB, full )
        * If aBitCount < 32, vacant MSB bits of the returned value are filled
        * with zeros.
        * @since    Series 60 3.0
        * @param    aBitCount - [input] Number of bits to get; between 1 and 32,
        *           both inclusive.
        * @return   TUint32 - Bit values extracted from the buffer; zero if
        *           aBitCount is out of range.
        */
        TUint32 Decode( TInt aBitCount );

        /**
        * Get a number of byte values from the bit-stream buffer at current
        * location.
        * @since Series 60 3.0
        * @param    aToBuffer - [output] Buffer to store byte values.
        * @param    aByteIndex - [input] Starting byte index ( starts from 
        *           zero ).
        * @param    aBitIndex - [input] Starting bit index ( starts from zero ).
        * @param    aBitCount - [input] Number of bits to put.
        */
        void Decode( TUint8* aToBuffer, TInt aToByteIndex, TInt aToBitIndex,
                TInt aBitCount );

        /**
        * Get the current byte index value.
        * @since    Series 60 3.0
        * @return   TInt - Current byte index value.
        */
        inline TInt ByteIndex() const 
                { return iByteIndex; };

        /**
        * Get the current bit index value.
        * @since    Series 60 3.0
        * @return   TInt - Current bit index value.
        */
        inline TInt BitIndex() const 
                { return iBitIndex; };

        /**
        * Set the current byte index value.
        * @since    Series 60 3.0
        * @param    aByteIndex New byte index
        */
        inline void SetByteIndex( TInt aByteIndex ) 
                { iByteIndex = aByteIndex; };

        /**
        * Set the current bit index value.
        * @since    Series 60 3.0
        * @param    aBitIndex New bit index
        */
        inline void SetBitIndex( TInt aBitIndex )
                { iBitIndex = aBitIndex; };

    private:
    
        // Pointer to the bit-stream buffer.
        const TUint8* iBuffer;
        
        // Current byte index ( starts from zero )
        TInt  iByteIndex;
        
        // Current bit index ( starts from zero )
        TInt  iBitIndex;
    };

#endif  // STREAMFORMATTER_H
