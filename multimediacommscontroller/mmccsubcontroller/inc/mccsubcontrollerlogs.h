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




#ifndef MCCSUBCONTROLLERLOGS_H
#define MCCSUBCONTROLLERLOGS_H


#include "mcclogs.h"

#if defined( _DEBUG ) && !defined( TEST_EUNIT )

#define __MCC_SUBCONTROLLER

#endif

#ifdef __MCC_SUBCONTROLLER

_LIT(KSubcontroller, "Mcc/Subcontroller:");

#define __SUBCONTROLLER(a) \
	{  _LIT( KStr, a); TMccLog::Print( KSubcontroller, KStr ); }
#define __SUBCONTROLLER_INT1(a, b) \
	{  _LIT( KStr, a); TMccLog::Print( KSubcontroller, KStr, b); }
#define __SUBCONTROLLER_INT2(a, b, c, d) \
	{ _LIT( KStr1, a); _LIT( KStr2, c); TMccLog::Print( KSubcontroller, KStr1, b, KStr2, d); }
#define __SUBCONTROLLER_STR(a, b) \
	{ _LIT( KStr, a); TMccLog::Print( KSubcontroller, KStr, b); }
#define __SUBCONTROLLER_STR8(a, b) \
	{ _LIT( KStr, a); TMccLog::Print( KSubcontroller, KStr, b); }

#define __SUBCONTROLLER_EVENT( event ){ \
_LIT( KMccEventLogFormat, "ses:%d, lnk:%d, str:%d, ep:%d, cat:%d, ty:%d, err:%d" );\
TBuf<110> eventTxt;\
eventTxt.Format( KMccEventLogFormat, event.iSessionId, event.iLinkId, event.iStreamId,\
event.iEndpointId, event.iEventCategory, event.iEventType, event.iErrorCode ); \
TMccLog::Print( KSubcontroller, eventTxt ); }

#else

#define __SUBCONTROLLER(a)
#define __SUBCONTROLLER_INT1(a, b)
#define __SUBCONTROLLER_INT2(a, b, c, d)
#define __SUBCONTROLLER_STR(a, b)
#define __SUBCONTROLLER_STR8(a, b)
#define __SUBCONTROLLER_EVENT( event )

#endif

#endif
