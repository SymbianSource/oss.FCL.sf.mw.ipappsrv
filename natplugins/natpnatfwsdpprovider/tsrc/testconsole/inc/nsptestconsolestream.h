/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef NSPTESTCONSOLESTREAM_H
#define NSPTESTCONSOLESTREAM_H

class CSdpMediaField;

class CTestConsoleStream : public CBase
	{
public: // Enums
	enum TStreamState
		{
		EIdle,
		EPrepared,
		EStreaming,
		EStopped
		};

public: // Methods
	static CTestConsoleStream* NewL();
	static CTestConsoleStream* NewLC();
	~CTestConsoleStream();
	
	TInt EncodeL( CSdpMediaField& aMediaField );
	TInt DecodeL( CSdpMediaField& aMediaField );
	TInt Start();
	TInt Stop();
	
	const TDesC8& LocalAddress() const;
	const TDesC8& RemoteAddress() const;
	TUint LocalPort() const;
	TUint RemotePort() const;

private: // Methods
	CTestConsoleStream();
	void ConstructL();

private: // data
	TStreamState iState;
	HBufC8* iLocalAddress;
	TUint iLocalPort;
	HBufC8* iRemoteAddress;
	TUint iRemotePort;
	
	};

#endif // NSPTESTCONSOLESTREAM_H
