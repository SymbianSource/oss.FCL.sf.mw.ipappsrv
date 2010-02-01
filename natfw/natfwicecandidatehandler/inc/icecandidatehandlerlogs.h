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
* Description:    Declarations of logging macros.
*
*/




#ifndef ICECANDIDATEHANDLERLOGS_H
#define ICECANDIDATEHANDLERLOGS_H

#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFW_ICECANDIDATEHANDLER
#endif

#ifdef __NATFW_ICECANDIDATEHANDLER
    _LIT(ICECandHandler, "NATFW/ICE Candidate Handler:");

    #define __ICEDP(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( ICECandHandler, KStr ); }
    #define __ICEDP_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( ICECandHandler, KStr, b); }
    #define __ICEDP_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); \
        TNATFWLog::Print( ICECandHandler, KStr1, b, KStr2, d); }
    #define __ICEDP_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( ICECandHandler, KStr, b); }
    #define __ICEDP_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( ICECandHandler, KStr, b); }
    #define __ICEDP_ADDRLOG( a, b ) \
        { _LIT( KStr, a); TNATFWLog::Print( ICECandHandler, KStr, b ); }
#else
    #define __ICEDP(a)
    #define __ICEDP_INT1(a, b)
    #define __ICEDP_INT2(a, b, c, d)
    #define __ICEDP_STR(a, b)
    #define __ICEDP_STR8(a, b)
    #define __ICEDP_ADDRLOG( a, b )
#endif

#endif // ICECANDIDATEHANDLERLOGS_H
