/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#include <SecureSocket.h>
#include <GenericSecureSocket.h>
#include <commsdattypesv1_1.h>

// -----------------------------------------------------------------------------
// CSecureSocket::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSecureSocket*
CSecureSocket::NewL(RSocket& /*aSocket*/,const TDesC& /*aProtocol*/)
    {
    delete HBufC::NewL( 1 );
    CSecureSocket* self = new(ELeave) CSecureSocket;
    return self;
    }
    
// -----------------------------------------------------------------------------
// CSecureSocket::~CSecureSocket
// -----------------------------------------------------------------------------
//    
CSecureSocket::~CSecureSocket()
	{
	}
	
// -----------------------------------------------------------------------------
// CSecureSocket::Send
// -----------------------------------------------------------------------------
// 	
EXPORT_C void
CSecureSocket::Send( const TDesC8& /*aDesc*/, TRequestStatus& /*aStatus*/ )
	{
	}
	
// -----------------------------------------------------------------------------
// CSecureSocket::CancelSend
// -----------------------------------------------------------------------------
// 	
EXPORT_C void CSecureSocket::CancelSend()
	{	
	}
