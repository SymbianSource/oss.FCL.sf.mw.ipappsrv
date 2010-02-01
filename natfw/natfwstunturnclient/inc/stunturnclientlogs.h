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
    #define __NATFW_STUNTURNCLIENT
#endif

#ifdef __NATFW_STUNTURNCLIENT
    _LIT(KSTUNTURNClient, "STUNClient:");

    #define __STUNTURNCLIENT(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KSTUNTURNClient, KStr ); }
    #define __STUNTURNCLIENT_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KSTUNTURNClient, KStr, b); }
    #define __STUNTURNCLIENT_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); \
          TNATFWLog::Print( KSTUNTURNClient, KStr1, b, KStr2, d); }
    #define __STUNTURNCLIENT_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KSTUNTURNClient, KStr, b); }
    #define __STUNTURNCLIENT_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KSTUNTURNClient, KStr, b); }
#else
    #define __STUNTURNCLIENT(a)
    #define __STUNTURNCLIENT_INT1(a, b)
    #define __STUNTURNCLIENT_INT2(a, b, c, d)
    #define __STUNTURNCLIENT_STR(a, b)
    #define __STUNTURNCLIENT_STR8(a, b)
#endif

#endif // STUNTURNCLIENTLOGS_H
