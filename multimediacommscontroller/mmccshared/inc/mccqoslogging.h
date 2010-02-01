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
* Description:    QoS features logging facilities
*
*/




#ifndef MCCQOSLOGGING_H
#define MCCQOSLOGGING_H


#include "mcclogs.h"

#ifdef _DEBUG

#define __MCC_QOSLOG

#endif

#ifdef __MCC_QOSLOG

_LIT( KQosLogging, "Mcc/QosLog: " );

#define __QOSLOG( a ) \
	{  _LIT( KStr, a ); TMccLog::Print( KQosLogging, KStr ); }

#define __QOSLOG_INT1( a, b ) \
	{  _LIT( KStr, a ); TMccLog::Print( KQosLogging, KStr, b ); }

#else

#define __QOSLOG(a)
#define __QOSLOG_INT1(a, b)

#endif

#endif  // MCCQOSLOGGING_H