/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/


#ifndef MCCMSRPSOURCESINKLOGS_H
#define MCCMSRPSOURCESINKLOGS_H

#include "mcclogs.h"

_LIT(KMsrpSourceSink, "Mcc/MsrpSourceSink:");

#ifdef _DEBUG
	#define __MCC_MSRPSOURCESINK_CONTROLL
#endif // end of _DEBUG

#ifdef __MCC_MSRPSOURCESINK_CONTROLL
	#define __MSRPSOURCESINK_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KMsrpSourceSink, KStr ); }
	#define __MSRPSOURCESINK_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KMsrpSourceSink, KStr, b ); }
#else
	#define __MSRPSOURCESINK_CONTROLL( a )
	#define __MSRPSOURCESINK_CONTROLL_INT1( a, b )
#endif // end of ifdef __MCC_MSRPSOURCESINK_CONTROLL

#endif // End of define MCCMSRPSOURCESINKLOGS_H

// end of file 