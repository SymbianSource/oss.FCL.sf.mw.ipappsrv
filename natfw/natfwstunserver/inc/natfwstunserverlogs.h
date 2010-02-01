/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    macros for logging
*
*/




#ifndef NATFWSTUNSERVERLOGS_H
#define NATFWSTUNSERVERLOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFW_STUNSERVER
#endif

#ifdef __NATFW_STUNSERVER
    _LIT(KNATFWStunServer, "NATFW/Stun Server:");

    #define __STUNSERVER(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWStunServer, KStr ); }
    #define __STUNSERVER_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATFWStunServer, KStr, b); }
    #define __STUNSERVER_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); TNATFWLog::Print\
        ( KNATFWStunServer, KStr1, b, KStr2, d); }
    #define __STUNSERVER_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWStunServer, KStr, b); }
    #define __STUNSERVER_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWStunServer, KStr, b); }
    #define __STUNSERVER_ADDRLOG( a, b ) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATFWStunServer, KStr, b ); }
#else
    #define __STUNSERVER(a)
    #define __STUNSERVER_INT1(a, b)
    #define __STUNSERVER_INT2(a, b, c, d)
    #define __STUNSERVER_STR(a, b)
    #define __STUNSERVER_STR8(a, b)
    #define __STUNSERVER_ADDRLOG( a, b )
#endif


#endif // NATFWSTUNSERVERLOGS_H
