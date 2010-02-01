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




#include <es_sock.h>
#include "natfwunsafutils.h"

const TInt KBoundary = 4;

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::WriteNetworkOrder16L
// -----------------------------------------------------------------------------
//
void NATFWUNSAFUtils::WriteNetworkOrder16L(TDes8& aDes, TInt aOffset,
    TUint16 aValue)
    {
    __ASSERT_ALWAYS(
        aDes.Length() >= aOffset + static_cast<TInt>(sizeof(aValue)),
        User::Leave(KErrArgument));

    aDes[aOffset] = NATFWUNSAFUtils::HighByte(aValue);
    aDes[aOffset + 1] = NATFWUNSAFUtils::LowByte(aValue);
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::WriteNetworkOrder32L
// -----------------------------------------------------------------------------
//
void NATFWUNSAFUtils::WriteNetworkOrder32L(TDes8& aDes, TInt aOffset,
    TUint32 aValue)
    {
    __ASSERT_ALWAYS(
        aDes.Length() >= aOffset + static_cast<TInt>(sizeof(aValue)),
        User::Leave(KErrArgument));

    WriteNetworkOrder16L(aDes, aOffset, NATFWUNSAFUtils::HighWord(aValue));
    WriteNetworkOrder16L(aDes,
                         aOffset + sizeof(TUint16),
                         NATFWUNSAFUtils::LowWord(aValue));
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::WriteNetworkOrder64L
// -----------------------------------------------------------------------------
//
void NATFWUNSAFUtils::WriteNetworkOrder64L(TDes8& aDes, TInt aOffset,
    TUint64 aValue)
    {
    __ASSERT_ALWAYS(
        aDes.Length() >= aOffset + static_cast<TInt>(sizeof(aValue)),
        User::Leave(KErrArgument));

    WriteNetworkOrder32L(aDes, aOffset, NATFWUNSAFUtils::HighDWord(aValue));
    WriteNetworkOrder32L(aDes,
                         aOffset + sizeof(TUint32),
                         NATFWUNSAFUtils::LowDWord(aValue));
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::GetQuadWordL
// -----------------------------------------------------------------------------
//
TUint64 NATFWUNSAFUtils::GetQuadWordL(const TUint8* aPtr)
    {
    __ASSERT_ALWAYS( NULL != aPtr, User::Leave(KErrArgument));
    return  MAKE_TUINT64(BigEndian::Get32(aPtr),
                         BigEndian::Get32(&aPtr[KBoundary]));
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::IsQuotedPairChar
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::IsQuotedPairChar(const TChar& aCharacter)
    {
    if (aCharacter == '\n' || aCharacter == '\r')
        {
        return EFalse;
        }
    return (TInt(aCharacter) <= 127);
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::IsQdTextChar
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::IsQdTextChar(const TChar& aCharacter)
    {
    if (aCharacter == 33 || (aCharacter >= 35 && aCharacter <= 91) ||
            (aCharacter >= 93 && aCharacter <= 126))
        {
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::StartsAndEndsWithQuotes
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::StartsAndEndsWithQuotes(const TDesC8& aString)
    {
    // trim and check quotes
    TLex8 lex(aString);
    lex.SkipSpace();
    TPtrC8 trimmedVal(lex.Remainder());
    const TInt KTwoQuotesLength = 2;
    if (trimmedVal.Length() < KTwoQuotesLength)
        {
        return EFalse;
        }
    if (trimmedVal.Locate('"') != 0)
        {
        return EFalse;
        }
    if (trimmedVal.LocateReverse('"') != trimmedVal.Length()-1)
        {
        return EFalse;
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::IsQuotedString
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::IsQuotedString(const TDesC8& aString)
    {
    // trim and check quotes
    TLex8 lex(aString);
    lex.SkipSpace();
    TPtrC8 trimmedVal(lex.Remainder());
    if (!StartsAndEndsWithQuotes(trimmedVal))
        {
        return EFalse;
        }
    // remove quotes
    TPtrC8 withoutFirstQuote(trimmedVal.Mid(1));
    TPtrC8 withoutQuotes(withoutFirstQuote.Left(withoutFirstQuote.Length()-1));
    // check value
    return QuotedStringValue(withoutQuotes);
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::RealmValueFromQuotedStringL
// -----------------------------------------------------------------------------
//
TPtrC8 NATFWUNSAFUtils::RealmValueFromQuotedStringL(const TDesC8& aString)
    {
    // trim and check quotes
    TLex8 lex(aString);
    lex.SkipSpace();
    TPtrC8 trimmedVal(lex.Remainder());
    if (StartsAndEndsWithQuotes(trimmedVal))
        {
        // remove quotes
        TPtrC8 withoutFirstQuote(trimmedVal.Mid(1));
        TPtrC8 withoutQuotes(withoutFirstQuote.Left(
            withoutFirstQuote.Length()-1));
        if (!QuotedStringValue(withoutQuotes))
            {
            User::Leave(KErrArgument);
            }
        return withoutQuotes;
        }
    if (!QuotedStringValue(trimmedVal))
        {
        User::Leave(KErrArgument);
        }
    return trimmedVal;
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::QuotedStringValue
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::QuotedStringValue(const TDesC8& aValue)
    {
    TLex8 lex(aValue);
    TChar chr = lex.Get();
    while (chr)
        {
        TBool ok = ETrue;
        if (IsUTF8NonAsciiStartChar(chr))
            {
            ok = SkipAndCheckNonAscii(chr,lex);
            }
        else if (chr == '\\')
            {
            chr = lex.Get();
            ok = IsQuotedPairChar(chr);
            }
        else if (IsQdTextChar(chr) || chr == ' ' || chr == '\t')
            {
            ok = ETrue;
            }
        else
            {
            ok = EFalse;
            }
        if (!ok)
            {
            return EFalse;
            }
        chr = lex.Get();
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::SkipAndCheckNonAscii
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::SkipAndCheckNonAscii(const TChar& aChr, TLex8& aLex)
    {
    if (aChr >= 192 && aChr <= 253)
        {
        if (aChr <= 223)
            {
            return SkipAndCheckContChars(aLex,1);
            }
        else if (aChr <= 239)
            {
            return SkipAndCheckContChars(aLex,2);
            }
        else if (aChr <= 247)
            {
            return SkipAndCheckContChars(aLex,3);
            }
        else if (aChr <= 251)
            {
            return SkipAndCheckContChars(aLex,4);
            }
        else
            {
            return SkipAndCheckContChars(aLex,5);
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::IsUTF8NonAsciiStartChar
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::IsUTF8NonAsciiStartChar(const TChar& aChar)
    {
    if(aChar >= 192 && aChar <= 253)
        {
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::SkipAndCheckContChars
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::SkipAndCheckContChars(TLex8& aLex, TInt aCount)
    {
    TInt counter = 0;
    TChar chr = 0;
    while (aCount > counter++)
        {
        chr = aLex.Get();
        if (IsUTF8ContChar(chr))
            {
            return EFalse;
            }
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// NATFWUNSAFUtils::IsUTF8ContChar
// -----------------------------------------------------------------------------
//
TBool NATFWUNSAFUtils::IsUTF8ContChar(const TChar& aChar)
    {
    return (aChar >= 128 && aChar <= 191);
    }

