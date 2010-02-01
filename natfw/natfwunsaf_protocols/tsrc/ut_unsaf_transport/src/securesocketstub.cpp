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



#include <securesocket.h>

EXPORT_C CSecureSocket* CSecureSocket::NewL( RSocket& /*aSocket*/,const TDesC& /*aProtocol*/ )
    {
    CSecureSocket* self = new ( ELeave ) CSecureSocket();
    return self;
    }


EXPORT_C CSecureSocket::~CSecureSocket()
    {
    }


EXPORT_C TInt CSecureSocket::AvailableCipherSuites( TDes8& aCiphers )
    {
    if ( aCiphers.MaxLength() < 64 )
        {
        return KErrArgument;
        }
    aCiphers.SetLength( 6 );

    BigEndian::Put16( &aCiphers[ 0 ], 4 );

    const TUint TLS_RSA_WITH_AES_128_CBC_SHA_Value = 0x2F;
    BigEndian::Put16( &aCiphers[ 2 ], TLS_RSA_WITH_AES_128_CBC_SHA_Value );

    BigEndian::Put16( &aCiphers[ 4 ], 64 );

    return KErrNone;
    }

EXPORT_C void CSecureSocket::CancelAll()
    {
    }

EXPORT_C void CSecureSocket::CancelHandshake()
    {
    }

EXPORT_C void CSecureSocket::CancelRecv()
    {
    }

EXPORT_C void CSecureSocket::CancelSend()
    {
    }

EXPORT_C const CX509Certificate* CSecureSocket::ClientCert()
    {
    return NULL;
    }

EXPORT_C TClientCertMode ClientCertMode()
    {
    return EClientCertModeIgnore;
    }

EXPORT_C TDialogMode CSecureSocket::DialogMode()
    {
    return EDialogModeAttended;
    }

EXPORT_C void CSecureSocket::Close()
    {
    }

EXPORT_C TInt CSecureSocket::CurrentCipherSuite( TDes8& /*aCipherSuite*/ )
    {
    return KErrNone;
    }

EXPORT_C void CSecureSocket::FlushSessionCache()
    {
    }

EXPORT_C TInt CSecureSocket::GetOpt( TUint /*aOptionName*/, TUint /*aOptionLevel*/, TDes8& /*aOption*/ )
    {
    return KErrNone;
    }

EXPORT_C TInt CSecureSocket::GetOpt( TUint /*aOptionName*/, TUint /*aOptionLevel*/, TInt& /*aOption*/ )
    {
    return KErrNone;
    }

EXPORT_C TInt CSecureSocket::Protocol( TDes& /*aProtocol*/ )
    {
    return KErrNone;
    }

EXPORT_C void CSecureSocket::Recv( TDes8& /*aDesc*/, TRequestStatus& aStatus )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

EXPORT_C void CSecureSocket::RecvOneOrMore( TDes8& /*aDesc*/, TRequestStatus& aStatus, TSockXfrLength& /*aLen*/ )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

EXPORT_C void CSecureSocket::RenegotiateHandshake( TRequestStatus& aStatus )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

EXPORT_C void CSecureSocket::Send( const TDesC8& /*aDesc*/, TRequestStatus& aStatus, TSockXfrLength& /*aLen*/ )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

EXPORT_C void CSecureSocket::Send( const TDesC8& /*aDesc*/, TRequestStatus& aStatus )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

EXPORT_C const CX509Certificate* CSecureSocket::ServerCert()
    {
    return NULL;
    }

EXPORT_C TInt CSecureSocket::SetAvailableCipherSuites( const TDesC8& /*aCiphers*/ )
    {
    return KErrNone;
    }

EXPORT_C TInt CSecureSocket::SetClientCert( const CX509Certificate& /*aCert*/ )
    {
    return KErrNone;
    }

EXPORT_C TInt CSecureSocket::SetClientCertMode( const TClientCertMode /*aClientCertMode*/ )
    {
    return KErrNone;
    }

EXPORT_C TInt CSecureSocket::SetDialogMode( const TDialogMode /*aDialogMode*/ )
    {
    return KErrNone;
    }
EXPORT_C TInt CSecureSocket::SetProtocol( const TDesC& /*aProtocol*/ )
    {
    return KErrNone;
    }

EXPORT_C TInt CSecureSocket::SetOpt( TUint /*aOptionName*/, TUint /*aOptionLevel*/, const TDesC8& /*aOption*/ )
    {
    return KErrNone;
    }

EXPORT_C TInt CSecureSocket::SetOpt( TUint /*aOptionName*/, TUint /*aOptionLevel*/, TInt /*aOption*/ )
    {
    return KErrNone;
    }

EXPORT_C TInt CSecureSocket::SetServerCert( const CX509Certificate& /*aCert*/ )
    {
    return KErrNone;
    }

EXPORT_C void CSecureSocket::StartClientHandshake( TRequestStatus& aStatus )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

EXPORT_C void CSecureSocket::StartServerHandshake( TRequestStatus& aStatus )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

void CSecureSocket::ConstructL( RSocket& /*aSocket*/,const TDesC& /*aProtocol*/ )
    {
    }

