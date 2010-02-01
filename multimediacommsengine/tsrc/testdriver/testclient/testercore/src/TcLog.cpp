/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <f32file.h>

#include "TcLog.h"

_LIT( KLogDir, "c:\\logs\\TC" );
_LIT( KLogFile, "c:\\logs\\TC\\TestClient.log" );
const TInt KTcBufferSize( 160 );

// CLASS DEFINITION
class TTcDes16Overflow
	: public TDes16Overflow
	{
	public:
		void Overflow( TDes16& ) {};
	};

// CLASS DEFINITION
class TTcDes8Overflow :public TDes8Overflow
	{
	public:
		void Overflow( TDes8& ) {};
	};

EXPORT_C void TcLog::WriteFormat( TRefByValue< const TDesC8 > aFormat, ... )
	{
	VA_LIST list;
	VA_START( list, aFormat );

	TBuf8< KTcBufferSize > buf;
	TTcDes8Overflow handler;
	buf.AppendFormatList( aFormat, list, &handler );

	Write( buf );
	}

EXPORT_C void TcLog::WriteFormat( TRefByValue< const TDesC16 > aFormat, ... )
	{
	VA_LIST list;
	VA_START( list, aFormat );

	TBuf16< KTcBufferSize > buf;
	TTcDes16Overflow handler;
	buf.AppendFormatList( aFormat, list, &handler );

	Write( buf );
	}

EXPORT_C void TcLog::Write( const TDesC16& aBuf )
	{
	// Convert to narrow
	TBuf8< KTcBufferSize > buf;
	buf.Copy( aBuf.Left( KTcBufferSize ) );
	Write( buf );
	}

EXPORT_C void TcLog::Write( const TDesC8& aBuf )
	{
	RFs fs;
	TInt status = fs.Connect();
	if( status == KErrNone )
		{
		// Check that the log folder exists
		TUint ignored;
		status = fs.Att( KLogDir, ignored );
		if( status == KErrNone )
			{
			RFile file;
			// If file exists, open it. Otherwise create a new file
			if( fs.Att( KLogFile, ignored ) == KErrNone )
				{
				status = file.Open( fs, KLogFile, EFileWrite | EFileShareAny );
				}
			else
				{
				status = file.Create( fs, KLogFile, EFileWrite | EFileShareAny );
				}

			if( status == KErrNone )
				{
				// Ignore errors from now on
				TInt pos( 0 );
				file.Seek( ESeekEnd, pos );
				file.Write( aBuf );
				}
			file.Close();
			}
		}
	fs.Close();
	}

EXPORT_C void TcLog::ClearLog()
	{
	RFs fs;
	TInt status = fs.Connect();
	if( status == KErrNone )
		{
		status = fs.Delete( KLogFile );
		}
	fs.Close();
	}

EXPORT_C HBufC8* TcLog::ReadLogLC()
	{
	RFs fs;
	if( fs.Connect() != KErrNone )
		{
		return NULL;
		}

	RFile in;
	if( !in.Open( fs, KLogFile, EFileRead | EFileShareAny ) )
		{
		TInt fileSize( 0 );
		in.Size( fileSize );
		HBufC8* logText = HBufC8::NewLC( fileSize );
		TPtr8 logTextPtr( logText->Des() );
		in.Read( logTextPtr );
		in.Close();
		fs.Close();
		return logText;
		}

	fs.Close();
	return NULL;
	}
