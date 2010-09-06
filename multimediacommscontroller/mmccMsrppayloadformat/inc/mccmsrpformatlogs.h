/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MCCMSRPFORMATLOGS_H
#define MCCMSRPFORMATLOGS_H

#include "mcclogs.h"

_LIT(KMsrpFormat, "Mcc/MsrpFormat:");

#ifdef _DEBUG
	#define __MCC_MsrpFORMAT_CONTROLL
	#define __MCC_MsrpFORMAT_MEDIA
#endif // end of _DEBUG

#ifdef __MCC_MsrpFORMAT_CONTROLL
	#define __MsrpFORMAT_CONTROLL( a ) \
		{  _LIT( KStr, a ); TMccLog::Print( KMsrpFormat, KStr ); }
	#define __MsrpFORMAT_CONTROLL_INT1( a, b ) \
		{  _LIT( KStr, a ); TMccLog::Print( KMsrpFormat, KStr, b ); }
	#define __MsrpFORMAT_CONTROLL_INT2( a, b, c, d ) \
		{ _LIT( KStr1, a ); _LIT( KStr2, c ); \
		  TMccLog::Print( KMsrpFormat, KStr1, b, KStr2, d ); }
#else
	#define __MsrpFORMAT_CONTROLL( a )
	#define __MsrpFORMAT_CONTROLL_INT1( a, b )
	#define __MsrpFORMAT_CONTROLL_INT2( a, b, c, d )
#endif // end of ifdef __MCC_ANYFORMAT_CONTROLL

#ifdef __MCC_MsrpFORMAT_MEDIA
	#define __MsrpFORMAT_MEDIA( a )	\
		{  _LIT( KStr, a ); TMccLog::Print( KMsrpFormat, KStr ); }
	#define __MsrpFORMAT_MEDIA_INT1( a, b ) 	\
		{  _LIT( KStr, a ); TMccLog::Print( KMsrpFormat, KStr, b ); }
	#define __MsrpFORMAT_MEDIA_INT2( a, b, c, d ) \
		{  _LIT( KStr1, a ); _LIT( KStr2, c ); \
		   TMccLog::Print( KMsrpFormat, KStr1, b, KStr2, d );}
#else
	#define __MsrpFORMAT_MEDIA( a )
	#define __MsrpFORMAT_MEDIA_INT1( a, b )
	#define __MsrpFORMAT_MEDIA_INT2( a, b, c, d )
#endif // end of ifdef __MCC_ANYFORMAT_MEDIA

#endif 
// end of file 
