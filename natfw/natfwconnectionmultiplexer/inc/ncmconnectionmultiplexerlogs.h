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
* Description:    Macroes for logging
*
*/





#ifndef NCMCONNECTIONMULTIPLEXERLOGS_H
#define NCMCONNECTIONMULTIPLEXERLOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __CONNECTIONMULTIPLEXER_LOGS
#endif

#ifdef __CONNECTIONMULTIPLEXER_LOGS
    _LIT(KConnectionMux, "NATFW/Connection Multiplexer:");

    #define __CONNECTIONMULTIPLEXER(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KConnectionMux, KStr ); }
    #define __CONNECTIONMULTIPLEXER_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KConnectionMux, KStr, b); }
    #define __CONNECTIONMULTIPLEXER_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); \
        TNATFWLog::Print( KConnectionMux, KStr1, b, KStr2, d); }
    #define __CONNECTIONMULTIPLEXER_INT3(a, b, c, d, e, f) \
        { _LIT( KStr1, a); _LIT( KStr2, c); _LIT( KStr3, e); \
        TNATFWLog::Print( KConnectionMux, KStr1, b, KStr2, d, KStr3, f); }
    #define __CONNECTIONMULTIPLEXER_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KConnectionMux, KStr, b); }
    #define __CONNECTIONMULTIPLEXER_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KConnectionMux, KStr, b); }
    #define __CONNECTIONMULTIPLEXER_ADDRLOG( a, b ) \
        { _LIT( KStr, a); TNATFWLog::Print( KConnectionMux, KStr, b ); }
#else
    #define __CONNECTIONMULTIPLEXER(a)
    #define __CONNECTIONMULTIPLEXER_INT1(a, b)
    #define __CONNECTIONMULTIPLEXER_INT2(a, b, c, d)
    #define __CONNECTIONMULTIPLEXER_INT3(a, b, c, d, e, f)
    #define __CONNECTIONMULTIPLEXER_STR(a, b)
    #define __CONNECTIONMULTIPLEXER_STR8(a, b)
    #define __CONNECTIONMULTIPLEXER_ADDRLOG( a, b )
#endif

#endif // NCMCONNECTIONMULTIPLEXERLOGS_H
