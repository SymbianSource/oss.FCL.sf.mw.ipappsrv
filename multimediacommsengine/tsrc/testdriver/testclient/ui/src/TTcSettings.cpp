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
* Description:  Implementation.
*
*/

#include <s32file.h>
#include "TTcSettings.h"
#include "TestClientConstants.h"

// Increment this value if changes are made to the settings file contents
const TInt KTcSettingsVersion( 7 );

TTcSettings::TTcSettings()
	: iBearerType( 1 ),
	  iAutoConnect( EFalse ),
	  iTCPPort( KTcDefaultPort ),
	  iIAPId( KTcDefaultIAPId ),
	  iIsFirstRun( ETrue ),
	  iCSYName( 0 ),
	  iCommPort( 0 ),
	  iBaudrate( KTcDefaultBaudrate ),
	  iTCPRemoteAddr()
	{
	iTCPRemoteAddr.SetAddress( INET_ADDR( 0, 0, 0, 0 ) );
	}

TTcSettings::~TTcSettings()
	{
	}

void TTcSettings::Load()
	{
	// Connect to file server
	RFs fs;
	if( fs.Connect() )
		{
		return;
		}

	// Open the settings file for reading, it might not exist yet
	RFileReadStream in;
	if( !in.Open( fs, KTcSettingsFile, EFileRead ) )
		{
		// Load actual data, ignore errors (the file might be of a previous
		// version and/or some items might be missing).
		TRAP_IGNORE( doLoadL( in ) )
		}

	in.Close();
	fs.Close();
	}

void TTcSettings::Store()
	{
	// Connect to file server
	RFs fs;
	if( fs.Connect() )
		{
		return;
		}

	// Open the settings file for writing, replace any existing data
	RFileWriteStream out;
	if( !out.Replace( fs, KTcSettingsFile, EFileWrite ) )
		{
		// Write actual data, ignore errors
		TRAP_IGNORE( doStoreL( out ) )
		}

	out.Close();
	fs.Close();
	}

void TTcSettings::doLoadL( RReadStream& aStream )
	{
	// Check that the file is of correct version
	TInt version = aStream.ReadInt16L();
	if( version == KTcSettingsVersion )
		{
		// Read bearer selection
		iBearerType = aStream.ReadInt32L() ;
		// Read autoconnect selection
		iAutoConnect = aStream.ReadInt16L();
		// Read TCP port selection
		iTCPPort = aStream.ReadInt16L();
		// Read IAP Id
		iIAPId = aStream.ReadInt16L();
		// Read CSY name
		iCSYName = aStream.ReadInt32L();
		// Read comms port name
		iCommPort = aStream.ReadInt32L();
		// Read baudrate of comms
		iBaudrate = aStream.ReadInt32L();
		// Read first run flag
		iIsFirstRun = aStream.ReadInt16L();
		
		// Read remote TCP address as string
		
		// Read length first
		TUint16 length = aStream.ReadInt16L();

		TBuf<40> buf;
		aStream.ReadL( buf, length);
		iTCPRemoteAddr.Input(buf);
			
		}
	}

void TTcSettings::doStoreL( RWriteStream& aStream )
	{
	aStream.WriteInt16L( KTcSettingsVersion );
	// Write bearer selection
	aStream.WriteInt32L( iBearerType );
	// Write autoconnect selection
	aStream.WriteInt16L( iAutoConnect );
	// Write TCP port selection
	aStream.WriteInt16L( iTCPPort );
	// Write IAP iD
	aStream.WriteInt16L( iIAPId );
	// Write CSY name
	aStream.WriteInt32L( iCSYName );
	// Write comms port name
	aStream.WriteInt32L( iCommPort );
	// Write baudrate of comms
	aStream.WriteInt32L( iBaudrate );
	// Write first run flag
	aStream.WriteInt16L( iIsFirstRun );
	
	// Write remote tcp address as string
	TBuf<40> buf;
	iTCPRemoteAddr.Output(buf);
	
	//write length first
	aStream.WriteInt16L( buf.Length() );
	aStream.WriteL(buf);
		
	}
