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

#include <in_sock.h>

#include "CTcTcpConnection.h"

CTcTcpConnection::CTcTcpConnection( TInt aPort )
	{
	iLocalPort = aPort;
	}

CTcTcpConnection::~CTcTcpConnection()
	{
	}

void CTcTcpConnection::SetupPortL()
	{
	// Open listening TCP socket
	User::LeaveIfError( iListeningSocket.Open( *iSocketServ,
											   KAfInet,		// KAfInet6
											   KSockStream,
											   KProtocolInetTcp ) );
	}

void CTcTcpConnection::SetupPort2L()
    {
    if ( iConnection )
        {
        User::LeaveIfError( iSocket.Open( *iSocketServ,
    									  KAfInet,		// KAfInet6
    									  KSockStream,
    									  KProtocolInetTcp,
    									  *iConnection ) );
        }
    else
        {
        User::LeaveIfError( iSocket.Open( *iSocketServ,
    									  KAfInet,		// KAfInet6
    									  KSockStream,
    									  KProtocolInetTcp ) );
        }
    }
