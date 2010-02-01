/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCCANYFORMATLOGS_H
#define MCCANYFORMATLOGS_H

#include "mcclogs.h"

_LIT(KAnyFormat, "Mcc/AnyFormat:");

#ifdef _DEBUG
	#define __MCC_ANYFORMAT_CONTROLL
	#define __MCC_ANYFORMAT_MEDIA
#endif // end of _DEBUG

#ifdef __MCC_ANYFORMAT_CONTROLL
	#define __ANYFORMAT_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KAnyFormat, KStr ); }
	#define __ANYFORMAT_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KAnyFormat, KStr, b ); }
	#define __ANYFORMAT_CONTROLL_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KAnyFormat, KStr1, b, KStr2, d ); }
#else
	#define __ANYFORMAT_CONTROLL( a )
	#define __ANYFORMAT_CONTROLL_INT1( a, b )
	#define __ANYFORMAT_CONTROLL_INT2( a, b, c, d )
#endif // end of ifdef __MCC_ANYFORMAT_CONTROLL

#ifdef __MCC_ANYFORMAT_MEDIA
	#define __ANYFORMAT_MEDIA( a )	\
		{  _LIT( KStr, a ); TMccLog::Print( KAnyFormat, KStr ); }
	#define __ANYFORMAT_MEDIA_INT1( a, b ) 	\
		{  _LIT( KStr, a ); TMccLog::Print( KAnyFormat, KStr, b ); }
	#define __ANYFORMAT_MEDIA_INT2( a, b, c, d ) \
		{  _LIT( KStr1, a ); _LIT( KStr2, c ); \
		   TMccLog::Print( KAnyFormat, KStr1, b, KStr2, d );}
#else
	#define __ANYFORMAT_MEDIA( a )
	#define __ANYFORMAT_MEDIA_INT1( a, b )
	#define __ANYFORMAT_MEDIA_INT2( a, b, c, d )
#endif // end of ifdef __MCC_ANYFORMAT_MEDIA

#endif // End of define MCCANYFORMATLOGS_H

// end of file 