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
* Description:    TCP sender
*
*/




#include "cncmtcpsender.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmsenditem.h"


// ---------------------------------------------------------------------------
// CNcmTcpSender::CNcmTcpSender
// ---------------------------------------------------------------------------
//
CNcmTcpSender::CNcmTcpSender( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver ) :
    CNcmSender( aSocket, aObserver )
    {
    }


// ---------------------------------------------------------------------------
// CNcmTcpSender::NewL
// ---------------------------------------------------------------------------
//
CNcmTcpSender* CNcmTcpSender::NewL( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpSender::NewL" )
        
    CNcmTcpSender* self = CNcmTcpSender::NewLC( aSocket, aObserver );    
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmTcpSender::NewLC
// ---------------------------------------------------------------------------
//
CNcmTcpSender* CNcmTcpSender::NewLC( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpSender::NewLC" )
        
    CNcmTcpSender* self = new( ELeave ) CNcmTcpSender( aSocket, aObserver );           
    CleanupStack::PushL( self );
    return self;
    }

    
// ---------------------------------------------------------------------------
// CNcmTcpSender::~CNcmTcpSender
// ---------------------------------------------------------------------------
//
CNcmTcpSender::~CNcmTcpSender()
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpSender::~CNcmTcpSender" )
    }


// ---------------------------------------------------------------------------
// From class CNcmSender
//
// DoSend
// ---------------------------------------------------------------------------
//
void CNcmTcpSender::DoSend()
    {
    __CONNECTIONMULTIPLEXER( "CNcmTcpSender::DoSend" )
       
    TInt count = iSendItemArray.Count();
    
    if ( count && !IsActive() )
        {        
        // Send oldest message
        iSendItemArray[0]->SetSendInProgress( ETrue );
        iSocket.Send( iSendItemArray[0]->Data(), 0, iStatus );
        SetActive();
        }
    }
