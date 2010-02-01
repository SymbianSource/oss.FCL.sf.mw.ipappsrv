/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef SOCKETMEDIACONNWRAPPERLOGS_H
#define SOCKETMEDIACONNWRAPPERLOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFW_SOCKETMEDIACONNWRAPPER
#endif

#ifdef __NATFW_SOCKETMEDIACONNWRAPPER
    _LIT(KSOCKETMEDIACONNWrapper, "NATFW/SocketMediaConnWrapper:");

    #define __SOCKETMEDIACONNWRAPPER(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KSOCKETMEDIACONNWrapper, KStr ); }
    #define __SOCKETMEDIACONNWRAPPER_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KSOCKETMEDIACONNWrapper, KStr, b); }
    #define __SOCKETMEDIACONNWRAPPER_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); TNATFWLog::Print\
        ( KSOCKETMEDIACONNWrapper, KStr1, b, KStr2, d); }
    #define __SOCKETMEDIACONNWRAPPER_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KSOCKETMEDIACONNWrapper, KStr, b); }
    #define __SOCKETMEDIACONNWRAPPER_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KSOCKETMEDIACONNWrapper, KStr, b); }
    #define __SOCKETMEDIACONNWRAPPER_ADDRLOG( a, b ) \
        { _LIT( KStr, a); TNATFWLog::Print( KSOCKETMEDIACONNWrapper, KStr, b ); }
#else
    #define __SOCKETMEDIACONNWRAPPER(a)
    #define __SOCKETMEDIACONNWRAPPER_INT1(a, b)
    #define __SOCKETMEDIACONNWRAPPER_INT2(a, b, c, d)
    #define __SOCKETMEDIACONNWRAPPER_STR(a, b)
    #define __SOCKETMEDIACONNWRAPPER_STR8(a, b)
    #define __SOCKETMEDIACONNWRAPPER_ADDRLOG( a, b )
#endif

#endif // SOCKETMEDIACONNWRAPPERLOGS_H
