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


#include <e32base.h>
#include <e32svr.h>
#include <utf.h>

inline void NATTraversalLog::Print(
    const TDesC16& aStr)
    {
    _LIT(KLogFormat, "NTC/%S");
    TBuf<256> str;
    str.Format(KLogFormat, &aStr);
    RDebug::Print(str);
    }

inline void NATTraversalLog::Print(
    const TDesC16& aStr1,
    const TDesC8& aStr2)
    {
    _LIT(KLogFormat, "NTC/%S: %S");
    TBuf<100> str2;
    CnvUtfConverter::ConvertToUnicodeFromUtf8(str2, aStr2);
    TBuf<256> str;
    str.Format(KLogFormat, &aStr1, &str2);
    RDebug::Print(str);
    }

inline void NATTraversalLog::Print(
    const TDesC16& aStr,
    TInt aValue)
    {
    _LIT(KLogFormat, "NTC/%S: %d");
    TBuf<256> str;
    str.Format(KLogFormat, &aStr, aValue);
    RDebug::Print(str);
    }

inline void NATTraversalLog::Print(
    const TDesC16& aStr,
    const TInetAddr& aAddr)
    {
    _LIT(KLogFormat, "NTC/%S: %S port %d");
    const TInt KIPv6AddrMaxLen = 39;
    TBuf<KIPv6AddrMaxLen> addrBuf;
    aAddr.Output(addrBuf);
    TBuf<256> str;
    str.Format(KLogFormat, &aStr, &addrBuf, aAddr.Port());
    RDebug::Print(str);
    }
