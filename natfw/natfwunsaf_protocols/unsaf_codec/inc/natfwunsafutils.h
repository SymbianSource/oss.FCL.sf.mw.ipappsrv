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




#ifndef NATFWUNSAFUTILS_H
#define NATFWUNSAFUTILS_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
* Utility functions
*
*  @lib natfwunsafprotocols.lib
*/
NONSHARABLE_CLASS(NATFWUNSAFUtils)
    {
    public: // New functions

        inline static TUint8 HighByte(TUint16 aValue)
            {
            return aValue >> 8;
            }

        inline static TUint8 LowByte(TUint16 aValue)
            {
            return aValue & 0xff;
            }

        inline static TUint16 HighWord(TUint32 aValue)
            {
            return aValue >> 16;
            }

        inline static TUint16 LowWord(TUint32 aValue)
            {
            return aValue & 0xffff;
            }

        inline static TUint32 HighDWord(TUint64 aValue)
            {
            return aValue >> 32;
            }

        inline static TUint32 LowDWord(TUint64 aValue)
            {
            return aValue & 0xffffffff;
            }

        /**
        * Writes an unsigned 16 bit value in network byte order (big-endian)
        * into aPtr, at the given position.
        * @param aPtr Buffer where the aValue is written, must have enough
        *             space
        * @param aOffset Position inside the buffer pointed by aPtr, where the
        *                value is written.
        * @param aValue 16 bit unsigned value to write
        * @leave KErrArgument if aPtr is too short to contain enough data
        */
        static void WriteNetworkOrder16L(TDes8& aPtr,
                                         TInt aOffset,
                                         TUint16 aValue);

        /**
        * Writes an unsigned 32 bit value in network byte order (big-endian)
        * into aPtr, at the given position.
        * @param aPtr Buffer where the aValue is written, must have enough
        *             space
        * @param aOffset Position inside the buffer pointed by aPtr, where the
        *                value is written.
        * @param aValue 32 bit unsigned value to write
        * @leave KErrArgument if aPtr is too short to contain enough data
        */
        static void WriteNetworkOrder32L(TDes8& aPtr,
                                         TInt aOffset,
                                         TUint32 aValue);

        /**
        * Writes an unsigned 64 bit value in network byte order (big-endian)
        * into aPtr, at the given position.
        * @param aPtr Buffer where the aValue is written, must have enough
        *             space
        * @param aOffset Position inside the buffer pointed by aPtr, where the
        *                value is written.
        * @param aValue 64 bit unsigned value to write
        * @leave KErrArgument if aPtr is too short to contain enough data
        */
        static void WriteNetworkOrder64L(TDes8& aPtr,
                                         TInt aOffset,
                                         TUint64 aValue);

        /**
        * Gets a quadruple word from a byte stream in big-endian format.
        * @param aPtr Pointer to a byte stream buffer
        * @return A 64-bit integer in big-endian format
        */
        static TUint64 GetQuadWordL(const TUint8* aPtr);

        /**
        * Utility functions for string and character handling
        */
        static TBool IsQuotedPairChar(const TChar& aCharacter);

        static TBool IsQdTextChar(const TChar& aCharacter);

        static TBool IsQuotedString(const TDesC8& aString);

        static TPtrC8 RealmValueFromQuotedStringL(const TDesC8& aString );

        static TBool QuotedStringValue (const TDesC8& aValue);

        static TBool StartsAndEndsWithQuotes (const TDesC8& aString);

        static TBool SkipAndCheckNonAscii (const TChar& aChr, TLex8& aLex);

        static TBool IsUTF8NonAsciiStartChar (const TChar& aChar);

        static TBool SkipAndCheckContChars (TLex8& aLex, TInt aCount);

        static TBool IsUTF8ContChar (const TChar& aChar);

    };

#endif // NATFWUNSAFUTILS_H


