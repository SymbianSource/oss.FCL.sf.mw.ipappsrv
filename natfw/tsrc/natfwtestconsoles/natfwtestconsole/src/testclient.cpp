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




#include "testclient.h"

#include <in_sock.h>
#include <e32debug.h>
#include <commdbconnpref.h>


CTestClient* CTestClient::NewL( MTestClientNotifier* aNotifier )
    {
    CTestClient* self = CTestClient::NewLC( aNotifier );
    CleanupStack::Pop( self );
    return self;
    }

CTestClient* CTestClient::NewLC( MTestClientNotifier* aNotifier )
    {   
    CTestClient* self = new( ELeave ) CTestClient( aNotifier );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }
    
CTestClient::CTestClient( MTestClientNotifier* aNotifier ) :
    CActive( EPriorityStandard ), iNotifier( aNotifier )
    {
    CActiveScheduler::Add( this );
    }

void CTestClient::ConstructL()
    {
    RDebug::Print( _L( "CTestClient::ConstructL" ) );
    User::LeaveIfError( iServer.Connect() );
    
    iTimer = CTimeOutTimer::NewL( EPriorityStandard, *this );
    }

CTestClient::~CTestClient()
    {
    RDebug::Print( _L( "CTestClient::~CTestClient" ) );
    Cancel();
    iSocket.Close();
    iConnection.Close();
    iServer.Close();
    iTimer->Cancel();
    delete iTimer;
    } 

// ---------------------------------------------------------------------------
// CTestClient::OpenL
// ---------------------------------------------------------------------------
//
void CTestClient::OpenL( TUint aIapId )
    {
    RDebug::Print( _L( "TestClient::OpenL" ) );
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( aIapId );
                                                 
    User::LeaveIfError( iConnection.Open( iServer ) );
    
    iConnection.Start( pref, iStatus );
    
    iState = EOpen;
    
    SetActive();
    }

// ---------------------------------------------------------------------------
// CTestClient::ConnectL
// ---------------------------------------------------------------------------
//
void CTestClient::ConnectL( TInetAddr& aAddr, TUint aTimeoutTime )
    {
    RDebug::Print( _L( "TestClient::ConnectL" ) );
    __ASSERT_ALWAYS( EReady == iState, User::Leave( KErrInUse ) );

    iSocket.Connect( aAddr, iStatus );
    iState = EConnecting;
    
    if ( aTimeoutTime )
        {
        iTimer->After( aTimeoutTime );        
        }

    SetActive();
    }

// ---------------------------------------------------------------------------
// CTestClient::RunL
// from CActive
// ---------------------------------------------------------------------------
//
void CTestClient::RunL()
    {
    RDebug::Print( _L( "CTestClient::RunL, State: %d, iStatus: %d" ), iState, iStatus.Int() );
    iTimer->Cancel();
    
    switch( iState )
        {
        case EOpen:
            if ( KErrNone == iStatus.Int() )
                {
                User::LeaveIfError( iSocket.Open( iServer, KAfInet, KSockStream, KProtocolInetTcp ) );
                User::LeaveIfError( iSocket.SetOpt( KSoReuseAddr, KProtocolInetIp, 1 ) );
            
                iNotifier->Notify( MTestClientNotifier::EOpened, KErrNone );
                iState = EReady;
                }
            else
                {
                iNotifier->Notify( MTestClientNotifier::EOpened, iStatus.Int() );
                }
            break;
        case EConnecting:
            iState = EConnected;
            iNotifier->Notify( MTestClientNotifier::EConnected, iStatus.Int() );
            break;   
        case EConnected:
            iNotifier->Notify( MTestClientNotifier::EConnected, iStatus.Int() );
            break;
        case EReady:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CTestClient::DoCancel
// from CActive
// ---------------------------------------------------------------------------
//
void CTestClient::DoCancel()
    {
    iTimer->Cancel();
    }

// ---------------------------------------------------------------------------
// CTestClient::TimerExpired
// from MTimeoutNotifier
// ---------------------------------------------------------------------------
//
void CTestClient::TimerExpired()
    {
    RDebug::Print( _L( "CTestClient::TimerExpired" ) );
    iNotifier->Notify( MTestClientNotifier::ETimerOccured, KErrNone );
    }
    
// ---------------------------------------------------------------------------
// CTestClient::ResolveLocalAddrL
// ---------------------------------------------------------------------------
//
void CTestClient::ResolveLocalAddrL( TInetAddr& aAddr, TUint32 aIap )
    {
    RDebug::Print( _L( "CTestClient::ResolveLocalAddrL" ) );
    
    TBuf<60> buffer;
  
    RSocket socket;
    CleanupClosePushL( socket );
    
    User::LeaveIfError( socket.Open( iServer, KAfInet, KSockDatagram, 
                                     KProtocolInetUdp ) );

    if ( socket.SetOpt( KSoInetEnumInterfaces, KSolInetIfCtrl ) == KErrNone )
        {
        TPckgBuf<TSoInetInterfaceInfo> opt;

        while ( KErrNone == socket.GetOpt(
            KSoInetNextInterface, KSolInetIfCtrl, opt ) )
            {
            TPckgBuf<TSoInetIfQuery> optifquery;
            optifquery().iName = opt().iName;

            if( KErrNone == socket.GetOpt(
                KSoInetIfQueryByName, KSolInetIfQuery, optifquery ) )
                {
                CheckAndSetAddr( aAddr, ( TInetAddr& )opt().iAddress,
                                 optifquery().iZone[1], aIap );
                }
            }
        }
    aAddr.Output( buffer );
    
    RDebug::Print( _L( "CTestClient::ResolveLocalAddrL() - ADDRESS: %S" ),
        &buffer );

    CleanupStack::PopAndDestroy( &socket );
    }

// ---------------------------------------------------------------------------
// CTestClient::CheckAndSetAddr
// ---------------------------------------------------------------------------
//
void CTestClient::CheckAndSetAddr( TInetAddr& aTarget,
                                                TInetAddr& aCandidate,
                                                TUint32 aCandidateIap,
                                                TUint32 aSpecifiedIap ) const
    {
    if ( !aCandidate.IsUnspecified() && !aCandidate.IsLoopback() )
        {
        if ( aCandidate.IsV4Mapped())
            {
            aCandidate.ConvertToV4();
            }
        if ( aCandidateIap == aSpecifiedIap )
            {
            SetAddr( aTarget, aCandidate );
            }
        }
    }

// ---------------------------------------------------------------------------
// CTestClient::SetAddr
// ---------------------------------------------------------------------------
//
void CTestClient::SetAddr( TInetAddr& aTarget,
    const TInetAddr& aSource ) const
    {
    if ( KAfInet6 == aSource.Family() )
        {
        if ( !aSource.IsLinkLocal() )
            {
            aTarget = aSource;
            aTarget.SetScope( 0 );
            }
        }
    else
        {
        aTarget = aSource;
        aTarget.SetScope( 0 );
        }
    }
