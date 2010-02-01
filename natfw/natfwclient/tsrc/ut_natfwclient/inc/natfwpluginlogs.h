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
* Description:    
*
*/




#ifndef NATFWPLUGINLOGS_H
#define NATFWPLUGINLOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFW_PLUGIN
#endif

#ifdef __NATFW_PLUGIN
    _LIT( KNATFWPlugin, "NATFW / Plug-in:" );

    #define __NATPLUGIN(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWPlugin, KStr ); }
    #define __NATPLUGIN_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWPlugin, KStr, b); }
    #define __NATPLUGIN_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); \
            TNATFWLog::Print( KNATFWlugin, KStr1, b, KStr2, d); }
    #define __NATPLUGIN_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWPlugin, KStr, b); }
    #define __NATPLUGIN_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWPlugin, KStr, b); }
    #define __NATPLUGIN_ADDRLOG( a, b ) \
          TNATFWLog::Print( _L( a ), b );
#else
    #define __NATPLUGIN(a)
    #define __NATPLUGIN_INT1(a, b)
    #define __NATPLUGIN_INT2(a, b, c, d)
    #define __NATPLUGIN_STR(a, b)
    #define __NATPLUGIN_STR8(a, b)
    #define __NATPLUGIN_ADDRLOG( a, b )
#endif

#endif // NATFWPLUGINLOGS_H
