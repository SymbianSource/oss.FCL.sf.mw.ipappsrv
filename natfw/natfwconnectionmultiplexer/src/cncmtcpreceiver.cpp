/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    TCP receiver
*
*/




#include "cncmtcpreceiver.h"
#include "ncmconnectionmultiplexerlogs.h"


// ---------------------------------------------------------------------------
// CNcmTcpReceiver::CNcmTcpReceiver
// ---------------------------------------------------------------------------
//
CNcmTcpReceiver::CNcmTcpReceiver( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver ) :
    CNcmReceiver( aSocket, aObserver )
    {
    }


// ---------------------------------------------------------------------------
// CNcmTcpReceiver::NewL
// ---------------------------------------------------------------------------
//
CNcmTcpReceiver* CNcmTcpReceiver::NewL( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpReceiver::NewL" )
        
    CNcmTcpReceiver* self = CNcmTcpReceiver::NewLC( aSocket, aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmTcpReceiver::NewLC
// ---------------------------------------------------------------------------
//
CNcmTcpReceiver* CNcmTcpReceiver::NewLC( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpReceiver::NewLC start" )
    
    CNcmTcpReceiver* self = new( ELeave ) CNcmTcpReceiver(
            aSocket, aObserver );
            
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmTcpReceiver::~CNcmTcpReceiver
// ---------------------------------------------------------------------------
//
CNcmTcpReceiver::~CNcmTcpReceiver()
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpReceiver::~CNcmTcpReceiver" )
    }


// ---------------------------------------------------------------------------
// From class CNcmReceiver
//
// CNcmTcpReceiver::DoReceive
// ---------------------------------------------------------------------------
//
void CNcmTcpReceiver::DoReceive()
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpReceiver::DoReceive" )

    if ( !IsActive() )
        {    
        iSocket.RecvOneOrMore( iBuffer, 0, iStatus, iSockDataLength );
        SetActive(); 
        }
    }


// ---------------------------------------------------------------------------
// CNcmTcpReceiver::SetDestination
// ---------------------------------------------------------------------------
//
void CNcmTcpReceiver::SetDestination( const TInetAddr& aDestination )
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpReceiver::SetDestination start" )
        
    iFromAddr = aDestination;
    }
