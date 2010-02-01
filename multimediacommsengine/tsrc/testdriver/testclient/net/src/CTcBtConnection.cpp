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

#include <bt_sock.h>

#include "CTcBtConnection.h"
#include "debuglog.h"

/// Protocol family name for Bluetooth RFCOMM
_LIT( KTcRFComm, "RFCOMM" );

CTcBtConnection::CTcBtConnection()
	{
	}

CTcBtConnection::~CTcBtConnection()
	{
	}

void CTcBtConnection::SetupPortL()
	{
	LOG( _L("CTcBtConnection::SetupPortL()") );

	// Open listening RFCOMM socket
	User::LeaveIfError( iListeningSocket.Open( *iSocketServ, KTcRFComm ) );

	// Get free RFCOMM port # and store it to iLocalPort
	User::LeaveIfError( iListeningSocket.GetOpt( KRFCOMMGetAvailableServerChannel,
												 KSolBtRFCOMM,
												 iLocalPort ) );

	LOG( _L("CTcBtConnection::ConnectL() done") );
	}
