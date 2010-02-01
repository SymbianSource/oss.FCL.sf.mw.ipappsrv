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

//To direct logs to RFileLogger, define the following flag
#undef WRITE_LOGS_TO_FILE

#if defined( WRITE_LOGS_TO_FILE )

#include <flogger.h>

//Write logs to /c/logs/natfw/unsaflog.txt
_LIT(KLogFileDir, "natfw");
_LIT(KLogFileName, "unsaflog.txt");

#define DO_WRITE(s)     RFileLogger::Write(KLogFileDir, KLogFileName, \
EFileLoggingModeAppend, (s))

#else

#define DO_WRITE(s)        RDebug::RawPrint(s)

#endif


inline void NATFWUNSAFLog::Print(
    const TDesC16& aStr)
    {
    _LIT(KLogFormat, "NATFW/UNSAF Protocols: %S");
    TBuf<256> str;
    str.Format(KLogFormat, &aStr);
    str.Append('\n');
    DO_WRITE(str);
    }

inline void NATFWUNSAFLog::Print(
    const TDesC16& aStr1,
    const TDesC8& aStr2)
    {
    _LIT(KLogFormat, "NATFW/UNSAF Protocols: %S: %S");
    TBuf<100> str2;
    CnvUtfConverter::ConvertToUnicodeFromUtf8(str2, aStr2);
    TBuf<256> str;
    str.Format(KLogFormat, &aStr1, &str2);
    str.Append('\n');
    DO_WRITE(str);
    }

inline void NATFWUNSAFLog::Print(
    const TDesC16& aStr1,
    const TDesC16& aStr2)
    {
    _LIT(KLogFormat, "NATFW/UNSAF Protocols: %S: %S");
    TBuf<256> str;
    str.Format(KLogFormat, &aStr1, &aStr2);
    str.Append('\n');
    DO_WRITE(str);
    }

inline void NATFWUNSAFLog::Print(
    const TDesC16& aStr,
    TInt aValue)
    {
    _LIT(KLogFormat, "NATFW/UNSAF Protocols: %S: %d");
    TBuf<256> str;
    str.Format(KLogFormat, &aStr, aValue);
    str.Append('\n');
    DO_WRITE(str);
    }

inline void NATFWUNSAFLog::Print(
    const TDesC16& aStr,
    const TInetAddr& aAddr)
    {
    _LIT(KLogFormat, "NATFW/UNSAF Protocols: %S: %S port %d");
    const TInt KIPv6AddrMaxLen = 39;
    TBuf<KIPv6AddrMaxLen> addrBuf;
    aAddr.Output(addrBuf);
    TBuf<256> str;
    str.Format(KLogFormat, &aStr, &addrBuf, aAddr.Port());
    str.Append('\n');
    DO_WRITE(str);
    }

inline void NATFWUNSAFLog::PrintByteStream(
    const TDesC16& aStr1,
    const TDesC8& aStr2)
    {
    _LIT(KLogFormat, "NATFW/UNSAF Protocols: %S:" );

    const TInt KLengthOfByte = 3;
    const TInt KMaxLineLength = 100;
    TBuf< KMaxLineLength > logBuf;

    logBuf.Format( KLogFormat, &aStr1 );
    DO_WRITE( logBuf );

    TInt inputPos = 0;
    while ( inputPos < aStr2.Length() )
        {
        logBuf.Zero();

        TInt outputPos = 0;
        while ( ( outputPos < KMaxLineLength - KLengthOfByte ) &&
                ( inputPos < aStr2.Length() ) )
            {
            logBuf.AppendFormat( _L( "%02x " ), aStr2[ inputPos++ ] );
            outputPos += KLengthOfByte;
            }
        logBuf.Append('\n');
        DO_WRITE( logBuf );
        }
    }
