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




#ifndef MCCMULTIPLEXERLOGS_H
#define MCCMULTIPLEXERLOGS_H

#include "mcclogs.h"

_LIT(KAnyFormat, "Mcc/Multiplexer:");

#ifdef _DEBUG
	#define __MCC_MULTIPLEXER

#endif // end of _DEBUG

#ifdef __MCC_MULTIPLEXER
	#define __MULTIPLEXER( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KAnyFormat, KStr ); }
	#define __MULTIPLEXER_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KAnyFormat, KStr, b ); }
	#define __MULTIPLEXER_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KAnyFormat, KStr1, b, KStr2, d ); }
#else
	#define __MULTIPLEXER( a )
	#define __MULTIPLEXER_INT1( a, b )
	#define __MULTIPLEXER_INT2( a, b, c, d )
#endif // end of ifdef __MCC_MULTIPLEXER

#endif // End of define MCCMULTIPLEXERLOGS_H

// end of file 