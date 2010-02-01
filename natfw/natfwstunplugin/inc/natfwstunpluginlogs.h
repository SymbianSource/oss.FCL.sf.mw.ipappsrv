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




#ifndef NATFWSTUNPLUGINLOGS_H
#define NATFWSTUNPLUGINLOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFW_STUNPLUGIN
#endif

#ifdef __NATFW_STUNPLUGIN
    _LIT( KNATFWStunPlugin, "NATFW/Stun Plug-in:" );

    #define __STUNPLUGIN(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWStunPlugin, KStr ); }
    #define __STUNPLUGIN_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWStunPlugin, KStr, b); }
    #define __STUNPLUGIN_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); \
            TNATFWLog::Print( KNATFWStunPlugin, KStr1, b, KStr2, d); }
    #define __STUNPLUGIN_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWStunPlugin, KStr, b); }
    #define __STUNPLUGIN_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWStunPlugin, KStr, b); }
    #define __STUNPLUGIN_ADDRLOG( a, b ) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWStunPlugin, KStr, b ); }
#else
    #define __STUNPLUGIN(a)
    #define __STUNPLUGIN_INT1(a, b)
    #define __STUNPLUGIN_INT2(a, b, c, d)
    #define __STUNPLUGIN_STR(a, b)
    #define __STUNPLUGIN_STR8(a, b)
    #define __STUNPLUGIN_ADDRLOG( a, b )
#endif


#endif // NATFWSTUNPLUGINLOGS_H
