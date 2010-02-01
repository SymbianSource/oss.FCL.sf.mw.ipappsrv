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




#ifndef STUNTURNCLIENTLOGS_H
#define STUNTURNCLIENTLOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFW_CLIENT
#endif

#ifdef __NATFW_CLIENT
    _LIT(KNATFWClient, "NATFW/Client:");

    #define __NATFWCLIENT(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWClient, KStr ); }
    #define __NATFWCLIENT_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWClient, KStr, b); }
    #define __NATFWCLIENT_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); TNATFWLog::Print( KNATFWClient, KStr1, b, KStr2, d); }
    #define __NATFWCLIENT_INT3(a, b, c, d, e, f) \
        { _LIT( KStr1, a); _LIT( KStr2, c); _LIT( KStr3, e); \
        TNATFWLog::Print( KNATFWClient, KStr1, b, KStr2, d, KStr3, f ); }
    #define __NATFWCLIENT_INT4(a, b, c, d, e, f, g, h) \
        { _LIT( KStr1, a); _LIT( KStr2, c); _LIT( KStr3, e); _LIT( KStr4, g); \
        TNATFWLog::Print( KNATFWClient, KStr1, b, KStr2, d, KStr3, f, KStr4, h ); }
    #define __NATFWCLIENT_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWClient, KStr, b); }
    #define __NATFWCLIENT_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWClient, KStr, b); }
    #define __NATFWCLIENT_ADDR( a, b ) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWClient, KStr, b ); }
#else
    #define __NATFWCLIENT(a)
    #define __NATFWCLIENT_INT1(a, b)
    #define __NATFWCLIENT_INT2(a, b, c, d)
    #define __NATFWCLIENT_INT3(a, b, c, d, e, f)
    #define __NATFWCLIENT_INT4(a, b, c, d, e, f, g, h)
    #define __NATFWCLIENT_STR(a, b)
    #define __NATFWCLIENT_STR8(a, b)
    #define __NATFWCLIENT_ADDR( a, b )
#endif


#endif // STUNTURNCLIENTLOGS_H
