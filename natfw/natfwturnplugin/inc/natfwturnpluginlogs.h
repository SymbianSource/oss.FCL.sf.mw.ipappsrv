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




#ifndef NATFWTURNPLUGINLOGS_H
#define NATFWTURNPLUGINLOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFW_TURNPLUGIN
#endif

#ifdef __NATFW_TURNPLUGIN
    _LIT( KNATFWTurnPlugin, "NATFW/Turn Plug-in:" );

    #define __TURNPLUGIN(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWTurnPlugin, KStr ); }
    #define __TURNPLUGIN_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWTurnPlugin, KStr, b); }
    #define __TURNPLUGIN_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c);\
          TNATFWLog::Print( KNATFWTurnPlugin, KStr1, b, KStr2, d ); }
    #define __TURNPLUGIN_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWTurnPlugin, KStr, b); }
    #define __TURNPLUGIN_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWTurnPlugin, KStr, b); }
    #define __TURNPLUGIN_ADDRLOG( a, b ) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWTurnPlugin, KStr, b ); }
#else
    #define __TURNPLUGIN( a )
    #define __TURNPLUGIN_INT1( a, b )
    #define __TURNPLUGIN_INT2( a, b, c, d )
    #define __TURNPLUGIN_STR( a, b )
    #define __TURNPLUGIN_STR8( a, b )
    #define __TURNPLUGIN_ADDRLOG( a, b )
#endif

#endif // NATFWTURNPLUGINLOGS_H
