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




#ifndef MCCANYSOURCESINKLOGS_H
#define MCCANYSOURCESINKLOGS_H

#include "mcclogs.h"

_LIT(KAnySourceSink, "Mcc/AnySourceSink:");

#ifdef _DEBUG
	#define __MCC_ANYSOURCESINK_CONTROLL
#endif // end of _DEBUG

#ifdef __MCC_ANYSOURCESINK_CONTROLL
	#define __ANYSOURCESINK_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KAnySourceSink, KStr ); }
	#define __ANYSOURCESINK_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KAnySourceSink, KStr, b ); }
#else
	#define __ANYSOURCESINK_CONTROLL( a )
	#define __ANYSOURCESINK_CONTROLL_INT1( a, b )
#endif // end of ifdef __MCC_ANYSOURCESINK_CONTROLL

#endif // End of define MCCANYSOURCESINKLOGS_H

// end of file 