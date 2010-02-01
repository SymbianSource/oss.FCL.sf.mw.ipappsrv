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




#ifndef MCCVIDEOSOURCESINKLOGS_H
#define MCCVIDEOSOURCESINKLOGS_H

#include "mcclogs.h"
_LIT(KVideoSourceSink, "Mcc/VideoSourceSink:");

#ifdef _DEBUG
	#define __MCC_VIDEO_SOURCESINK_CONTROLL
	#define __MCC_VIDEO_SOURCESINK_MEDIA
#endif // end of _DEBUG

#ifdef __MCC_VIDEO_SOURCESINK_CONTROLL
	#define __V_SOURCESINK_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KVideoSourceSink, KStr ); }
	#define __V_SOURCESINK_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KVideoSourceSink, KStr, b ); }
	#define __V_SOURCESINK_CONTROLL_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KVideoSourceSink, KStr1, b, KStr2, d ); }	  
    #define __V_SOURCESINK_CONTROLL_STR8( a, b ) \
		{ _LIT( KStr1, a ); \
		  TMccLog::Print( KVideoSourceSink, KStr1, b ); }
    #define __V_SOURCESINK_CONTROLL_REAL( a, b ) \
		{  _LIT( KStr, a ); TMccLog::PrintReal( KVideoSourceSink, KStr, b ); }
#else
	#define __V_SOURCESINK_CONTROLL( a )
	#define __V_SOURCESINK_CONTROLL_INT1( a, b )
	#define __V_SOURCESINK_CONTROLL_INT2( a, b, c, d )
	#define __V_SOURCESINK_CONTROLL_STR8( a, b )
	#define __V_SOURCESINK_CONTROLL_REAL( a, b )
#endif // end of ifdef __MCC_VIDEO_SOURCESINK_CONTROLL

#ifdef __MCC_VIDEO_SOURCESINK_MEDIA
	#define __V_SOURCESINK_MEDIA( a )	\
		{  _LIT( KStr, a ); TMccLog::Print( KVideoSourceSink, KStr ); }
	#define __V_SOURCESINK_MEDIA_INT1( a, b ) 	\
		{  _LIT( KStr, a ); TMccLog::Print( KVideoSourceSink, KStr, b ); }
	#define __V_SOURCESINK_MEDIA_INT2( a, b, c, d ) \
		{  _LIT( KStr1, a ); _LIT( KStr2, c ); \
		   TMccLog::Print( KVideoSourceSink, KStr1, b, KStr2, d ); }
#else
	#define __V_SOURCESINK_MEDIA( a )
	#define __V_SOURCESINK_MEDIA_INT1( a, b )
	#define __V_SOURCESINK_MEDIA_INT2( a, b, c, d )
#endif // end of ifdef __MCC_VIDEO_SOURCESINK_MEDIA

#endif // End of define MCCVIDEOSOURCESINKLOGS_H

// end of file 