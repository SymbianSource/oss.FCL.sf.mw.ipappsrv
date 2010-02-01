/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Handles socket connecting
*
*/




#include "natfwconnectivityapidefs.h"

#include "natfwsockethandler.h"
#include "natfwsocketsender.h"
#include "natfwsocketmediaconnwrapperlogs.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::CNATFWSocketHandler
// ---------------------------------------------------------------------------
//
CNATFWSocketHandler::CNATFWSocketHandler( TUint aPort ) : 
    CActive(EPriorityStandard), iPort( aPort ), iSendState ( EUnitialized ),
    iReceiveState( ENotReceiving )
    {   
    CActiveScheduler::Add(this);
    }


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::ConstructL(
    MNATFWSocketReceiverObserver& aRecObs,
    MNATFWSocketSenderObserver& aSendObs, TUint aProtocol )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::ConstructL" )

    __ASSERT_ALWAYS( KProtocolInetTcp == aProtocol || 
        KProtocolInetUdp == aProtocol, User::Leave( KErrArgument ) );
    
    iProtocol = aProtocol;
    iSender = CNATFWSocketSender::NewL( aSendObs, iProtocol );
    iReceiver = CNATFWSocketReceiver::NewL( aRecObs, iProtocol );
    }


// ---------------------------------------------------------------------------
// CNATFWSocketHandler* CNATFWSocketHandler::NewL
// ---------------------------------------------------------------------------
//
CNATFWSocketHandler* CNATFWSocketHandler::NewL( 
    MNATFWSocketReceiverObserver& aRecObs,
    MNATFWSocketSenderObserver& aSendObs, TUint aProtocol, TUint aPort )
    {
    CNATFWSocketHandler* self = CNATFWSocketHandler::NewLC( aRecObs, aSendObs,
        aProtocol, aPort );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWSocketHandler* CNATFWSocketHandler::NewLC
// ---------------------------------------------------------------------------
//
CNATFWSocketHandler* CNATFWSocketHandler::NewLC( 
    MNATFWSocketReceiverObserver& aRecObs, 
    MNATFWSocketSenderObserver& aSendObs, TUint aProtocol, TUint aPort )
    {
    CNATFWSocketHandler* self 
            = new( ELeave ) CNATFWSocketHandler( aPort );
    CleanupStack::PushL( self );
    self->ConstructL( aRecObs, aSendObs, aProtocol );
    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::~CNATFWSocketHandler()
// ---------------------------------------------------------------------------
//
CNATFWSocketHandler::~CNATFWSocketHandler()
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::~CNATFWSocketHandler" )   
    
    iMediaObserver = NULL;
    iTcpConnectionObserver = NULL;
    Cancel();    
    delete iSender;
    delete iReceiver;
    iSocket.Close();
    iListeningSocket.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::RunL( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::RunL( )
    {
    __SOCKETMEDIACONNWRAPPER_INT1( 
        "CNATFWSocketHandler::RunL iStatus:", iStatus.Int() ) 
     
    if ( KErrNone == iStatus.Int() )
        {
        iSendState = EConnected;        
        }
    else
        {
        iSendState = EUnitialized;
        }
    iTcpConnectionObserver->ConnectingCompleted( iStatus.Int() );
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSocketHandler::DoCancel( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::DoCancel( )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::DoCancel" )

    iSocket.CancelAll();
    if ( ETcpSetupPassive == iTcpConnectionType )
        {
        iListeningSocket.CancelAll();
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::LocalAddress( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::LocalAddress( TSockAddr& aSocketToSendForPeer )
    {
    __SOCKETMEDIACONNWRAPPER_ADDRLOG(
    "CNATFWSocketHandler::LocalAddress - Local IP Address: ", iLocalAddress )
    
    aSocketToSendForPeer = iLocalAddress;
    }

// ---------------------------------------------------------------------------
// CNATFWSocketHandler::GetRemoteAddress( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::GetRemoteAddress( TSockAddr& aRemoteAddress )
    {
    __SOCKETMEDIACONNWRAPPER_ADDRLOG(
    "CNATFWSocketHandler::GetRemoteAddress - Remote IP Address: ", iRemoteAddress )
    
    aRemoteAddress = iRemoteAddress;
    }
    
// ---------------------------------------------------------------------------
// CNATFWSocketHandler::SetAddrL( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::SetAddrL( const TSockAddr& aSocketToSendForPeer )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::SetAddrL" )
    
    __ASSERT_ALWAYS( EUnitialized == iSendState, User::Leave( KErrArgument ) );
    
    if ( KAfInet6 == aSocketToSendForPeer.Family() )
        {
        iRemoteAddress.SetAddress( KInet6AddrLoop );
        }
    else
        {
        iRemoteAddress.SetAddress( INET_ADDR( 127, 0, 0, 1 ) );
        }
    
    iRemoteAddress.SetPort( aSocketToSendForPeer.Port() );
    
    TBool requiredPortOpen( EFalse );
    TInt error( KErrNone );
    
    if ( KProtocolInetTcp == iProtocol )
        {
        User::LeaveIfError( iSocket.Open( iSocketServer,
        KAfInet, KSockStream, KProtocolInetTcp ));
        }
    else if ( KProtocolInetUdp == iProtocol )
        {
        User::LeaveIfError( iSocket.Open( iSocketServer,
        KAfInet, KSockDatagram, KProtocolInetUdp ));
        }
    else
        {
        // Nothing to do here
        }

    while ( !requiredPortOpen )
        {
        if ( KProtocolInetUdp == iProtocol )
            {
            error = iSocket.SetLocalPort( iLocalAddress.Port() );
            }
        if ( KProtocolInetTcp == iProtocol )
            {
            error = iSocket.Bind( iLocalAddress );
            }

        if ( KErrNone != error )
            {
            TUint port( 0 );
            iMediaObserver->GetNewFreePort( port );
            iLocalAddress.SetPort( port );
            }
        else
            {
            requiredPortOpen = ETrue; 
            }
        }
        
    if ( KProtocolInetUdp == iProtocol )
        {
        iSendState = EConnected;
        }
    __SOCKETMEDIACONNWRAPPER_ADDRLOG( "Remote IP Address: ", iRemoteAddress )
    __SOCKETMEDIACONNWRAPPER_ADDRLOG( "Local IP Address: ", iLocalAddress )
    }


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::SetReceivingStateL( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::SetReceivingStateL( )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::SetReceivingStateL" )
    
    __ASSERT_ALWAYS( EConnected == iSendState || ESending == iSendState,
        User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iReceiveState == ENotReceiving,
        User::Leave( KErrInUse ) );    
    
    if ( ETcpSetupPassive == iTcpConnectionType )
        {
        iReceiver->StartListening( iListeningSocket );
        }
    else
        {
        iReceiver->StartListening( iSocket );
        }
        
    iReceiveState = EReceiving;         
    }     
    
     
// ---------------------------------------------------------------------------
// CNATFWSocketHandler::SetSendingStateL( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::SetSendingStateL( )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::SetSendingStateL" )
    
    __ASSERT_ALWAYS( EConnected == iSendState, User::Leave( KErrNotReady ) );

    if ( ETcpSetupPassive == iTcpConnectionType )
        {
        iSender->SetRemoteAddress( iRemoteAddress, iListeningSocket );
        }
    else
        {
        iSender->SetRemoteAddress( iRemoteAddress, iSocket );
        } 
        
    iSendState = ESending;
    }  


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::SetReceivingStateForMuxWrapper( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::SetReceivingStateForMuxWrapper(
    TNATFWStreamingState aState )
    {
    __SOCKETMEDIACONNWRAPPER(
        "CNATFWSocketHandler::SetReceivingStateForMuxWrapper" )

    iReceiver->SetReceivingStateForMuxWrapper( aState );
    }    


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::SendL( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::SendL( const TDesC8& aStreamPortion )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::SendL" )
    
    __ASSERT_ALWAYS( ESending == iSendState, User::Leave( KErrNotReady ) );
        
    iSender->SendL( aStreamPortion );
    } 


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::SetLocalAddress( )
// ---------------------------------------------------------------------------
//    
void CNATFWSocketHandler::SetLocalAddress( 
    const RSocketServ& aSocketServer, const TSockAddr& /*aLocalAddress*/ )
    {
    iSocketServer = aSocketServer;  
    
    iLocalAddress.SetAddress( INET_ADDR( 127, 0, 0, 1 ) );
    iLocalAddress.SetPort( iPort );
    }

// ---------------------------------------------------------------------------
// CNATFWSocketHandler::SetMediaObserverL( )
// ---------------------------------------------------------------------------
//    
void CNATFWSocketHandler::SetMediaObserverL( 
    MNATFWMediaWrapperObserver* aMediaObserver )
    {
    __ASSERT_ALWAYS( aMediaObserver, User::Leave( KErrNotReady ) );
    
    iMediaObserver = aMediaObserver;
    }
    
// ---------------------------------------------------------------------------
// CNATFWSocketHandler::DeactivateSending(  )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::DeactivateSending(  )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::DeactivateSending" )
    
    iSender->Cancel();
    iSendState = EConnected;
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSocketHandler::DeactivateReceiving(  )
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::DeactivateReceiving(  )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::DeactivateReceiving" )
    
    iReceiver->Cancel();
    iReceiveState = ENotReceiving;
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSocketHandler::OpenTcpConnectionL()
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::OpenTcpConnectionL(
    TNATFWTcpConnectionSetup aSetup, MNsmcwTcpConnectionObserver& aObserver )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketHandler::OpenTcpConnectionL" )
    
    __ASSERT_ALWAYS( EUnitialized == iSendState, User::Leave( KErrInUse ) );
    
    iTcpConnectionType = aSetup;
    iTcpConnectionObserver = &aObserver;
    
    if ( ETcpSetupActive == iTcpConnectionType )  
        {
        iSocket.Connect( iRemoteAddress, iStatus );
        }
    if ( ETcpSetupPassive == iTcpConnectionType ) 
        {        
        User::LeaveIfError( iSocket.Listen( 1 ) );
        User::LeaveIfError( iListeningSocket.Open( iSocketServer ) );
        iSocket.Accept( iListeningSocket, iStatus );
        }
    iSendState = EGettingConnection;
    SetActive();  
    }


// ---------------------------------------------------------------------------
// CNATFWSocketHandler::CloseTcpConnection()
// ---------------------------------------------------------------------------
//
void CNATFWSocketHandler::CloseTcpConnection()
    {
    __SOCKETMEDIACONNWRAPPER(
        "CNATFWSocketHandler::CloseTcpConnection" )

    Cancel();
    iSendState = EUnitialized;
    iTcpConnectionObserver->ConnectingCompleted( KErrCancel );
    }

