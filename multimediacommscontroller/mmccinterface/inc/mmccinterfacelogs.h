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




#ifndef MMCCINTERFACELOGS_H
#define MMCCINTERFACELOGS_H


#include "mcclogs.h"

#if defined( _DEBUG ) && !defined( TEST_EUNIT )

#define __MCC_INTERFACE

#endif

#ifdef __MCC_INTERFACE

_LIT(KAnyFormat, "Mcc/Interface:");

#define __INTERFACE(a) \
    {  _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr ); }
#define __INTERFACE_INT1(a, b) \
    {  _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr, b); }
#define __INTERFACE_INT2(a, b, c) \
    { _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr, b, c); }
#define __INTERFACE_STR(a, b) \
    { _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr, b); }
#define __INTERFACE_STR8(a, b) \
    { _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr, b); }

#else

#define __INTERFACE(a)
#define __INTERFACE_INT1(a, b)
#define __INTERFACE_INT2(a, b, c)
#define __INTERFACE_STR(a, b)
#define __INTERFACE_STR8(a, b)

#endif

#endif
