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




#ifndef MCCCONTROLLERLOGS_H
#define MCCCONTROLLERLOGS_H


#include "mcclogs.h"

#if defined( _DEBUG ) && !defined( TEST_EUNIT )

#define __MCC_CONTROLLER

#endif

#ifdef __MCC_CONTROLLER

_LIT(KAnyFormat, "Mcc/Controller:");

#define __CONTROLLER(a) \
	{  _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr ); }
#define __CONTROLLER_INT1(a, b) \
	{  _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr, b); }
#define __CONTROLLER_INT2(a, b, c) \
	{ _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr, b, c); }
#define __CONTROLLER_STR(a, b) \
	{ _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr, b); }
#define __CONTROLLER_STR8(a, b) \
	{ _LIT( KStr, a); TMccLog::Print( KAnyFormat, KStr, b); }

#else

#define __CONTROLLER(a)
#define __CONTROLLER_INT1(a, b)
#define __CONTROLLER_INT2(a, b, c)
#define __CONTROLLER_STR(a, b)
#define __CONTROLLER_STR8(a, b)

#endif

#endif
