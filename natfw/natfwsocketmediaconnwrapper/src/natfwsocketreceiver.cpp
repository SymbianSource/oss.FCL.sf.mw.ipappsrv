/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Receive data from User of NAT Connectivity Framework.
*
*/




#include "natfwsocketreceiver.h"
#include "natfwsocketmediaconnwrapperlogs.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWSocketReceiver::CNATFWSocketReceiver( )
// ---------------------------------------------------------------------------
//
CNATFWSocketReceiver::CNATFWSocketReceiver(
    MNATFWSocketReceiverObserver& aRecObs, TUint aProtocol ) : 
    CActive(EPriorityStandard), iProtocol( aProtocol ), iObserver( aRecObs )
    {
    __SOCKETMEDIACONNWRAPPER(
    "CNATFWSocketReceiver::CNATFWSocketReceiver start")
   
    CActiveScheduler::Add(this);
    }


// ---------------------------------------------------------------------------
// CNATFWSocketReceiver* CNATFWSocketReceiver::NewL( )
// ---------------------------------------------------------------------------
//
CNATFWSocketReceiver* CNATFWSocketReceiver::NewL( 
        MNATFWSocketReceiverObserver& aRecObs, TUint aProtocol )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketReceiver::NewL start" )
    
    CNATFWSocketReceiver* self = 
        new (ELeave) CNATFWSocketReceiver ( aRecObs, aProtocol );

    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWSocketReceiver::~CNATFWSocketReceiver()
// ---------------------------------------------------------------------------
//
CNATFWSocketReceiver::~CNATFWSocketReceiver()
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketReceiver::~CNATFWSocketReceiver" )

    Cancel();
    }
    

// ---------------------------------------------------------------------------
// CNATFWSocketReceiver::StartListening( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketReceiver::StartListening( const RSocket& aSocket )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketReceiver::StartListening" )

    iSocket = aSocket;

    if ( KProtocolInetUdp == iProtocol )
        {
        iSocket.RecvFrom( iBuffer, iRemoteAddress, 0, iStatus );
        }
    if ( KProtocolInetTcp == iProtocol )
        {
        iSocket.RecvOneOrMore( iBuffer, 0, iStatus, iSockDataLength );
        }
    
    SetActive();
    }


// ---------------------------------------------------------------------------
// CNATFWSocketReceiver::SetReceivingStateForMuxWrapper( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketReceiver::SetReceivingStateForMuxWrapper(
    TNATFWStreamingState aState )
    {
    __SOCKETMEDIACONNWRAPPER(
        "CNATFWSocketReceiver::SetReceivingStateForMuxWrapper" )

    iWrapperMuxReceiveState = aState;
    }      


// ---------------------------------------------------------------------------
// CNATFWSocketReceiver::RunL( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketReceiver::RunL( )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketReceiver::RunL" ) 
    
    if ( KErrEof != iStatus.Int() && KErrNone != iStatus.Int() )
        {
        __SOCKETMEDIACONNWRAPPER_INT1( 
        "CNATFWSocketReceiver::RunL:", iStatus.Int() )
        
        User::Leave( KErrArgument );
        }
    
    if ( EStreamingStateActive == iWrapperMuxReceiveState )
        {
        iObserver.DeliverBuffer( iBuffer );
        }
    
    if ( KProtocolInetUdp == iProtocol )
        {
        iSocket.RecvFrom( iBuffer, iRemoteAddress, 0, iStatus );
        }
    
    if ( KProtocolInetTcp == iProtocol )
        {
        if ( KErrEof == iStatus.Int() )
            {
            Cancel();
            return;
            }
        else
            {
            iSocket.RecvOneOrMore( iBuffer, 0, iStatus, iSockDataLength );
            }
        }
    
    SetActive();
    }


// ---------------------------------------------------------------------------
// CNATFWSocketReceiver::DoCancel( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketReceiver::DoCancel( )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketReceiver::DoCancel" )

    iSocket.CancelAll();
    }
   
    
// ---------------------------------------------------------------------------
// CNATFWSocketReceiver::RunError( )
// ---------------------------------------------------------------------------
//  
TInt CNATFWSocketReceiver::RunError( TInt aError )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketReceiver::RunError" )

    return aError;
    }


