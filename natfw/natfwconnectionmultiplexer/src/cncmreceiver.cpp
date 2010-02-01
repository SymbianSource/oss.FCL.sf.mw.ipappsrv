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
* Description:    Abstract socket receiver
*
*/




#include "cncmreceiver.h"
#include "mncmreceiversenderobserver.h"
#include "ncmconnectionmultiplexerlogs.h"


// ---------------------------------------------------------------------------
// CNcmReceiver::CNcmReceiver
// ---------------------------------------------------------------------------
//
CNcmReceiver::CNcmReceiver( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver ) :
    CActive( EPriorityStandard ), iSocket( aSocket ), iObserver( aObserver ),
    iFromAddr( KInetAddrNone, 0 )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CNcmReceiver::~CNcmReceiver
// ---------------------------------------------------------------------------
//
CNcmReceiver::~CNcmReceiver()
    {
    __CONNECTIONMULTIPLEXER( "CNcmReceiver::~CNcmReceiver" )

    Cancel();
    }


// ---------------------------------------------------------------------------
// CNcmReceiver::Message
// ---------------------------------------------------------------------------
//
TDesC8& CNcmReceiver::Message()
    {
    return iBuffer;
    }


// ---------------------------------------------------------------------------
// CNcmReceiver::FromAddress
// ---------------------------------------------------------------------------
//
const TInetAddr& CNcmReceiver::FromAddress() const
    {
    return iFromAddr;
    }
 
  
// ---------------------------------------------------------------------------
// From class CActive
//
// CNcmReceiver::RunL
// ---------------------------------------------------------------------------
//
void CNcmReceiver::RunL()
    {
    __CONNECTIONMULTIPLEXER_INT1( "CNcmReceiver::RunL", iStatus.Int() )
    
    switch( iStatus.Int() )
        {
        case KErrNone:
            iObserver.MessageAvailable();
            DoReceive();
            break;
        case KErrEof:
            // Connection closed, nothing to do
            iObserver.Error( KErrDisconnected );
            break;
        default:
            iObserver.Error( iStatus.Int() );
        }
    }


// ---------------------------------------------------------------------------
// From class CActive
//
// CNcmReceiver::RunL
// ---------------------------------------------------------------------------
//
void CNcmReceiver::DoCancel()
    {
    __CONNECTIONMULTIPLEXER( "CNcmReceiver::DoCancel" )
        
    iSocket.CancelRecv();
    }


// ---------------------------------------------------------------------------
// CNcmReceiver::Activate
// ---------------------------------------------------------------------------
//
void CNcmReceiver::Activate()
    {
    DoReceive();
    }


// ---------------------------------------------------------------------------
// CNcmReceiver::Deactivate
// ---------------------------------------------------------------------------
//
void CNcmReceiver::Deactivate()
    {
    Cancel();
    }
