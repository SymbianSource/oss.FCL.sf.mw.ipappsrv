/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation
*
*/

#ifndef __DEBUGLOG_H__
#define __DEBUGLOG_H__

#ifdef ENABLE_LOG_FILE

	#include <flogger.h>	// RFileLogger
	#include <f32file.h>	// RFs

	inline void WriteFormat( TRefByValue< const TDesC > aFormat, ... )
		{
		_LIT( KTcLogDir, "TC" );
		#ifndef LOG_NAME_DEFINED
			_LIT( KTcLogName, "TestClientDebug.log" );
		#endif

		VA_LIST args;
		VA_START( args, aFormat );
		RFileLogger::WriteFormat( KTcLogDir, KTcLogName, EFileLoggingModeAppend,
								  aFormat, args );
		VA_END( args );
		}

    #define LOG WriteFormat
    #define RESETLOG ResetLog

	#define IF_LOG( statement ) statement

#else	// ENABLE_LOG_FILE

	#ifdef ENABLE_LOG_RDEBUG

		#include <e32svr.h>		// RDebug
	    #define LOG RDebug::Print
		#define IF_LOG( statement ) statement

	#else	// ENABLE_LOG_RDEBUG

		inline void LOG( ... )
			{
			}

		#define IF_LOG( statement )

	#endif	// ENABLE_LOG_RDEBUG

	inline void RESETLOG()
		{
		}

#endif	// ENABLE_LOG_FILE

#endif // __DEBUGLOG_H__
