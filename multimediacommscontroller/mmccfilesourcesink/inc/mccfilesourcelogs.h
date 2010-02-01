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




#ifndef MCCFILESOURCELOGS_H
#define MCCFILESOURCELOGS_H

#include "mcclogs.h"

_LIT(KFileSource, "Mcc/FileSource:");

#ifdef _DEBUG
	#define __MCC_FILESOURCE_CONTROLL
	#define __MCC_FILESOURCE_MEDIA
#endif // end of _DEBUG

#ifdef __MCC_FILESOURCE_CONTROLL
	#define __FILESOURCE_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KFileSource, KStr ); }
	#define __FILESOURCE_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KFileSource, KStr, b ); }
	#define __FILESOURCE_CONTROLL_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KFileSource, KStr1, b, KStr2, d ); }
	#define __FILESOURCE_CONTROLL_INT4( a, b, c, d, e, f, g, h ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); _LIT( KStr3, e ); _LIT( KStr4, g );\
		  TMccLog::Print( KFileSource, KStr1, b, KStr2, d, KStr3, f, KStr4, h ); }
#else
	#define __FILESOURCE_CONTROLL( a )
	#define __FILESOURCE_CONTROLL_INT1( a, b )
	#define __FILESOURCE_CONTROLL_INT2( a, b, c, d )
	#define __FILESOURCE_CONTROLL_INT4( a, b, c, d, e, f, g, h )
#endif // end of ifdef __MCC_FILESOURCE_CONTROLL

#ifdef __MCC_FILESOURCE_MEDIA
	#define __FILESOURCE_MEDIA( a )	\
		{  _LIT( KStr, a ); TMccLog::Print( KFileSource, KStr ); }
	#define __FILESOURCE_MEDIA_INT1( a, b ) 	\
		{  _LIT( KStr, a ); TMccLog::Print( KFileSource, KStr, b ); }
	#define __FILESOURCE_MEDIA_INT2( a, b, c, d ) \
		{  _LIT( KStr1, a ); _LIT( KStr2, c ); \
		   TMccLog::Print( KFileSource, KStr1, b, KStr2, d );}
#else
	#define __FILESOURCE_MEDIA( a )
	#define __FILESOURCE_MEDIA_INT1( a, b )
	#define __FILESOURCE_MEDIA_INT2( a, b, c, d )
#endif // end of ifdef __MCC_FILESOURCE_MEDIA

#endif // End of define MCCFILESOURCELOGS_H

// end of file 