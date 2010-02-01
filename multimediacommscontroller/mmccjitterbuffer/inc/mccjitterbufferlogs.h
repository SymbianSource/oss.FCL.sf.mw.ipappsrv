/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCCJITTERBUFFERLOGS_H
#define MCCJITTERBUFFERLOGS_H

#include "mcclogs.h"
_LIT( KJitterBuffer, "Mcc/JitterBuffer:" );

#ifdef _DEBUG
	#define __MCC_JITTER_BUFFER
	//#define __MCC_CN_GEN
#endif // end of _DEBUG

#ifdef __MCC_JITTER_BUFFER
	#define __JITTER_BUFFER( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KJitterBuffer, KStr ); }
	#define __JITTER_BUFFER_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KJitterBuffer, KStr, b ); }
	#define __JITTER_BUFFER_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KJitterBuffer, KStr1, b, KStr2, d ); }
#else
	#define __JITTER_BUFFER( a )
	#define __JITTER_BUFFER_INT1( a, b )
	#define __JITTER_BUFFER_INT2( a, b, c, d )
#endif // end of ifdef __MCC_JITTER_BUFFER

#ifdef __MCC_CN_GEN
	#define __CN_GEN( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KJitterBuffer, KStr ); }
	#define __CN_GEN_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KJitterBuffer, KStr, b ); }
	#define __CN_GEN_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KJitterBuffer, KStr1, b, KStr2, d ); }
#else
	#define __CN_GEN( a )
	#define __CN_GEN_INT1( a, b )
	#define __CN_GEN_INT2( a, b, c, d )
#endif // end of ifdef __MCC_CN_GEN

#endif // End of define MCCJITTERBUFFERLOGS_H

// end of file 