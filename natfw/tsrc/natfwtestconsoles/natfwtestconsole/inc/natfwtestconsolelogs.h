/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef NATFWTESTCONSOLELOGS_H
#define NATFWTESTCONSOLELOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFWTESTCONSOLE_LOGS
#endif

#ifdef __NATFWTESTCONSOLE_LOGS
    _LIT(KNATFWTestConsole, "NATFW/NATFWTestConsole:");

    #define __NATFWTESTCONSOLE(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWTestConsole, KStr ); }
    #define __NATFWTESTCONSOLE_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWTestConsole, KStr, b); }
    #define __NATFWTESTCONSOLE_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); \
        TNATFWLog::Print( KNATFWTestConsole, KStr1, b, KStr2, d); }
    #define __NATFWTESTCONSOLE_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWTestConsole, KStr, b); }
    #define __NATFWTESTCONSOLE_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWTestConsole, KStr, b); }
    #define __NATFWTESTCONSOLE_ADDRLOG( a, b ) \
          TNATFWLog::Print( _L( a ), b );
#else
    #define __NATFWTESTCONSOLE(a)
    #define __NATFWTESTCONSOLE_INT1(a, b)
    #define __NATFWTESTCONSOLE_INT2(a, b, c, d)
    #define __NATFWTESTCONSOLE_STR(a, b)
    #define __NATFWTESTCONSOLE_STR8(a, b)
    #define __NATFWTESTCONSOLE_ADDRLOG( a, b )
#endif

#endif // NCMNATFWTESTCONSOLELOGS_H