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




#ifndef MCCH263FORMATLOGS_H
#define MCCH263FORMATLOGS_H

#include "mcclogs.h"
_LIT(KH263Format, "Mcc/H263Format:");

#ifdef _DEBUG
	#define __MCC_H263FORMAT_CONTROLL
	#define __MCC_H263FORMAT_MEDIA
#endif // end of _DEBUG

#ifdef __MCC_H263FORMAT_CONTROLL
	#define __H263FORMAT_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KH263Format, KStr ); }
	#define __H263FORMAT_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KH263Format, KStr, b ); }
	#define __H263FORMAT_CONTROLL_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KH263Format, KStr1, b, KStr2, d ); }
#else
	#define __H263FORMAT_CONTROLL( a )
	#define __H263FORMAT_CONTROLL_INT1( a, b )
	#define __H263FORMAT_CONTROLL_INT2( a, b, c, d )
#endif // end of ifdef __MCC_H263FORMAT_CONTROLL

#ifdef __MCC_H263FORMAT_MEDIA
	#define __H263FORMAT_MEDIA( a )	\
		{  _LIT( KStr, a ); TMccLog::Print( KH263Format, KStr ); }
	#define __H263FORMAT_MEDIA_INT1( a, b ) 	\
		{  _LIT( KStr, a ); TMccLog::Print( KH263Format, KStr, b ); }
	#define __H263FORMAT_MEDIA_INT2( a, b, c, d ) \
		{  _LIT( KStr1, a ); _LIT( KStr2, c ); \
		   TMccLog::Print( KH263Format, KStr1, b, KStr2, d ); }
#else
	#define __H263FORMAT_MEDIA( a )
	#define __H263FORMAT_MEDIA_INT1( a, b )
	#define __H263FORMAT_MEDIA_INT2( a, b, c, d )
#endif // end of ifdef __MCC_H263FORMAT_MEDIA

#endif // End of define MCCH263FORMATLOGS_H

// end of file 