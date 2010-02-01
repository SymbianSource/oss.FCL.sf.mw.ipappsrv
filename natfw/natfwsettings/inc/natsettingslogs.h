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




#ifndef NATSETTINGSLOGS_H
#define NATSETTINGSLOGS_H


#include "natfwlogs.h"

#ifdef _DEBUG
    #define __NATFW_NATSETTINGS
#endif

#ifdef __NATFW_NATSETTINGS
    _LIT(KNATSettings, "NATFW/NATSettings:");

    #define __NATSETTINGS(a) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATSettings, KStr ); }
    #define __NATSETTINGS_INT1(a, b) \
        {  _LIT( KStr, a); TNATFWLog::Print( KNATSettings, KStr, b); }
    #define __NATSETTINGS_INT2(a, b, c, d) \
        { _LIT( KStr1, a); _LIT( KStr2, c); \
          TNATFWLog::Print( KNATSettings, KStr1, b, KStr2, d); }
    #define __NATSETTINGS_STR(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATSettings, KStr, b); }
    #define __NATSETTINGS_STR8(a, b) \
        { _LIT( KStr, a); TNATFWLog::Print( KNATSettings, KStr, b); }
#else
    #define __NATSETTINGS(a)
    #define __NATSETTINGS_INT1(a, b)
    #define __NATSETTINGS_INT2(a, b, c, d)
    #define __NATSETTINGS_STR(a, b)
    #define __NATSETTINGS_STR8(a, b)
#endif

#endif // NATSETTINGSLOGS_H
