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
* Description:    UDP receiver
*
*/




#include "cncmudpreceiver.h"
#include "ncmconnectionmultiplexerlogs.h"


// ---------------------------------------------------------------------------
// CNcmUdpReceiver::CNcmUdpReceiver
// ---------------------------------------------------------------------------
//
CNcmUdpReceiver::CNcmUdpReceiver( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver ) :
    CNcmReceiver( aSocket, aObserver )
    {
    }


// ---------------------------------------------------------------------------
// CNcmUdpReceiver::NewL
// ---------------------------------------------------------------------------
//
CNcmUdpReceiver* CNcmUdpReceiver::NewL( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmUdpReceiver::NewL" )
        
    CNcmUdpReceiver* self = CNcmUdpReceiver::NewLC( aSocket, aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmUdpReceiver::NewLC
// ---------------------------------------------------------------------------
//
CNcmUdpReceiver* CNcmUdpReceiver::NewLC( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmUdpReceiver::NewLC" )
        
    CNcmUdpReceiver* self = new( ELeave ) CNcmUdpReceiver(
            aSocket, aObserver );
            
    CleanupStack::PushL( self );
    return self;
    }

    
// ---------------------------------------------------------------------------
// CNcmUdpReceiver::~CNcmUdpReceiver
// ---------------------------------------------------------------------------
//
CNcmUdpReceiver::~CNcmUdpReceiver()
    {
    __CONNECTIONMULTIPLEXER( "CNcmUdpReceiver::~CNcmUdpReceiver" )
    }

// ---------------------------------------------------------------------------
// From class CNcmReceiver
//
// CNcmUdpReceiver::DoReceive
// ---------------------------------------------------------------------------
//
void CNcmUdpReceiver::DoReceive()
    {
    __CONNECTIONMULTIPLEXER( "CNcmUdpReceiver::DoReceive" )
    
    if ( !IsActive() )
        {
        iSocket.RecvFrom( iBuffer, iFromAddr, 0, iStatus );
        SetActive(); 
        }  
    }
