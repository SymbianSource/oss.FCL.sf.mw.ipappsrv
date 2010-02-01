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
* Description:  See class definition below.
*
*/

#ifndef __TCLOG_H__
#define __TCLOG_H__

// INCLUDES
#include <e32std.h>

// CLASS DEFINITION
/**
 * TcLog implements a file-based logger. All of its functions are static.
 * It uses c:\logs\TC\TestClient.log as its log file.
 */
class TcLog
	{
	private: // Constructors and destructor

		/// Default constructor. Not implemented.
		TcLog();

	public: // Static functions

		IMPORT_C static void WriteFormat( TRefByValue< const TDesC8 > aFormat, ... );

		IMPORT_C static void WriteFormat( TRefByValue< const TDesC16 > aFormat, ... );

		IMPORT_C static void Write( const TDesC16& aBuf );

		IMPORT_C static void Write( const TDesC8& aBuf );

		IMPORT_C static void ClearLog();

		IMPORT_C static HBufC8* ReadLogLC();

	};

#endif // __TCLOG_H__
