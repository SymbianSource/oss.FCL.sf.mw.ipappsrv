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




#ifndef MCCFILESINKLOGS_H
#define MCCFILESINKLOGS_H

#include "mcclogs.h"

_LIT(KFileSink, "Mcc/FileSink:");

#ifdef _DEBUG
	#define __MCC_FILESINK_CONTROLL
	#define __MCC_FILESINK_MEDIA
#endif // end of _DEBUG

#ifdef __MCC_FILESINK_CONTROLL
	#define __FILESINK_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KFileSink, KStr ); }
	#define __FILESINK_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KFileSink, KStr, b ); }
	#define __FILESINK_CONTROLL_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KFileSink, KStr1, b, KStr2, d ); }
	#define __FILESINK_CONTROLL_INT4( a, b, c, d, e, f, g, h ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); _LIT( KStr3, e ); _LIT( KStr4, g );\
		  TMccLog::Print( KFileSink, KStr1, b, KStr2, d, KStr3, f, KStr4, h ); }
    #define __FILESINK_CONTROLL_STR8( a, b ) \
		{ _LIT( KStr1, a ); \
		  TMccLog::Print( KFileSink, KStr1, b ); }
#else
	#define __FILESINK_CONTROLL( a )
	#define __FILESINK_CONTROLL_INT1( a, b )
	#define __FILESINK_CONTROLL_INT2( a, b, c, d )
	#define __FILESINK_CONTROLL_INT4( a, b, c, d, e, f, g, h )
	#define __FILESINK_CONTROLL_STR8( a, b )
#endif // end of ifdef __MCC_FILESINK_CONTROLL

#ifdef __MCC_FILESINK_MEDIA
	#define __FILESINK_MEDIA( a )	\
		{  _LIT( KStr, a ); TMccLog::Print( KFileSink, KStr ); }
	#define __FILESINK_MEDIA_INT1( a, b ) 	\
		{  _LIT( KStr, a ); TMccLog::Print( KFileSink, KStr, b ); }
	#define __FILESINK_MEDIA_INT2( a, b, c, d ) \
		{  _LIT( KStr1, a ); _LIT( KStr2, c ); \
		   TMccLog::Print( KFileSink, KStr1, b, KStr2, d );}
#else
	#define __FILESINK_MEDIA( a )
	#define __FILESINK_MEDIA_INT1( a, b )
	#define __FILESINK_MEDIA_INT2( a, b, c, d )
#endif // end of ifdef __MCC_FILESINK_MEDIA

#endif // End of define MCCFILESINKLOGS_H

// end of file 