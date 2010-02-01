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




#include "testserver.h"

#include <in_sock.h>
#include <e32debug.h>


CTestServer* CTestServer::NewL( MTestServerObserver* aObserver )
    {
    CTestServer* self = CTestServer::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

CTestServer* CTestServer::NewLC( MTestServerObserver* aObserver )
    {   
    CTestServer* self = new( ELeave ) CTestServer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

CTestServer::CTestServer( MTestServerObserver* aObserver ) :
    CActive( EPriorityStandard ), iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

void CTestServer::ConstructL()
    {
    RDebug::Print( _L( "CTestServer::ConstructL" ) );
    User::LeaveIfError( iServer.Connect() );
    iState = EServerDisconnected;

    iTimer = CTimeOutTimer::NewL( EPriorityStandard, *this );
    }

CTestServer::~CTestServer()
    {
    RDebug::Print( _L( "CTestServer::~CTestServer" ) );
    Cancel();
    iSocket.Close();
    iListenSocket.Close();  
    iServer.Close();
    iTimer->Cancel();
    delete iTimer;
    RDebug::Print( _L( "CTestServer::~CTestServer - done" ) );
    }

// ---------------------------------------------------------------------------
// CTestServer::OpenL
// ---------------------------------------------------------------------------
//
void CTestServer::OpenL( TInetAddr& aAddr, TUint aTimeoutTime )
    {
    RDebug::Print( _L( "CTestServer::OpenL" ) );
    __ASSERT_ALWAYS( EServerDisconnected == iState, User::Leave( KErrAlreadyExists ) );
    
    User::LeaveIfError( iListenSocket.Open( iServer, KAfInet,
        KSockStream, KProtocolInetTcp ) );
    
    User::LeaveIfError( iSocket.Open( iServer ) );
                                                                  
    User::LeaveIfError( iListenSocket.SetOpt( KSoReuseAddr,
        KProtocolInetIp, 1 ) );

    User::LeaveIfError( iListenSocket.Bind( aAddr ) );
    
    User::LeaveIfError( iListenSocket.Listen( 1 ) );
    
    iListenSocket.Accept( iSocket, iStatus );
    
    iState = EServerWaitingConnection;
    
    if ( aTimeoutTime )
        {
        iTimer->After( aTimeoutTime );
        }
    
    SetActive();
    }

// ---------------------------------------------------------------------------
// CTestServer::RunL
// from CActive
// ---------------------------------------------------------------------------
//
void CTestServer::RunL()
    {
    RDebug::Print( _L( "CTestServer::RunL - iStatus: %d" ), iStatus.Int() );
    
    switch( iState )
        {
        case EServerWaitingConnection:
            if( iStatus.Int() == KErrNone )
                {
                iState = EServerConnected;
                iObserver->Notify( MTestServerObserver::EConnected, KErrNone );
                }
            else
                {
                iState = EServerDisconnected;
                iObserver->Notify( MTestServerObserver::EConnecting, iStatus.Int() );
                }
            break;
        case EServerConnected:
            iObserver->Notify( MTestServerObserver::EConnected, iStatus.Int() );
            break;
        case EServerDisconnected:
            iObserver->Notify( MTestServerObserver::EDisconnected, iStatus.Int() );
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTestServer::DoCancel
// from CActive
// ---------------------------------------------------------------------------
//
void CTestServer::DoCancel()
    {
    iTimer->Cancel();
    }

// ---------------------------------------------------------------------------
// CTestServer::TimerExpired
// from MTimeoutNotifier
// ---------------------------------------------------------------------------
//
void CTestServer::TimerExpired()
    {
    iObserver->Notify( MTestServerObserver::EShutdownTimer, KErrNone );
    }
