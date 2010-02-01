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




#ifndef NATFWUNSAFLOG_H
#define NATFWUNSAFLOG_H

#include <e32std.h>

#ifdef _DEBUG
    // Comment the following line to turn off all kind of logging:
    #define USE_NATFWUNSAF_LOGS
#endif


#ifdef USE_NATFWUNSAF_LOGS

    #include <in_sock.h>

    class NATFWUNSAFLog
        {
        public:

            inline static void Print(const TDesC16& aStr);

            inline static void Print(const TDesC16& aStr1,
                                     const TDesC8& aStr2);
                                     
            inline static void Print(const TDesC16& aStr1,
                                     const TDesC16& aStr2);

            inline static void Print(const TDesC16& aStr,
                                     TInt aValue);

            inline static void Print(const TDesC16& aStr,
                                     const TInetAddr& aAddr);

            inline static void PrintByteStream(const TDesC16& aStr1,
                                               const TDesC8& aStr2);
        };

    #include "natfwunsaflog.inl"

    #define NATFWUNSAF_LOG( a ) NATFWUNSAFLog::Print( _L( a ) );
    #define NATFWUNSAF_INTLOG( a, b ) NATFWUNSAFLog::Print( _L( a ), b );
    #define NATFWUNSAF_STRLOG( a, b ) NATFWUNSAFLog::Print( _L( a ), b );
    #define NATFWUNSAF_STR16LOG( a, b ) NATFWUNSAFLog::Print( _L( a ), b );
    #define NATFWUNSAF_ADDRLOG( a, b ) NATFWUNSAFLog::Print( _L( a ), b );
    #define NATFWUNSAF_BYTESTREAMLOG( a, b ) NATFWUNSAFLog::PrintByteStream( _L( a ), b );

#else // USE_NATFWUNSAF_LOGS

    #define NATFWUNSAF_LOG( a )
    #define NATFWUNSAF_INTLOG( a, b )
    #define NATFWUNSAF_STRLOG( a, b )
    #define NATFWUNSAF_STR16LOG( a, b )
    #define NATFWUNSAF_ADDRLOG( a, b )
    #define NATFWUNSAF_BYTESTREAMLOG( a, b )

#endif // USE_NATFWUNSAF_LOGS


#endif // NATFWUNSAFLOG_H


