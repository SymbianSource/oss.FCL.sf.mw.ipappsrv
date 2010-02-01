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
* Description:    UDP sender
*
*/




#include "cncmudpsender.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "mncmreceiversenderobserver.h"
#include "cncmsenditem.h"


// ---------------------------------------------------------------------------
// CNcmUdpSender::CNcmUdpSender
// ---------------------------------------------------------------------------
//
CNcmUdpSender::CNcmUdpSender( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver ) :
    CNcmSender( aSocket, aObserver ),
    iDestination( KAFUnspec )
    {
    }


// ---------------------------------------------------------------------------
// CNcmUdpSender::NewL
// ---------------------------------------------------------------------------
//
CNcmUdpSender* CNcmUdpSender::NewL( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmUdpSender::NewL" )
        
    CNcmUdpSender* self = CNcmUdpSender::NewLC( aSocket, aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmUdpSender::NewLC
// ---------------------------------------------------------------------------
//
CNcmUdpSender* CNcmUdpSender::NewLC( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmUdpSender::NewLC" )
        
    CNcmUdpSender* self = new( ELeave ) CNcmUdpSender( aSocket, aObserver );
            
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmUdpSender::~CNcmUdpSender
// ---------------------------------------------------------------------------
//
CNcmUdpSender::~CNcmUdpSender()
    {
    __CONNECTIONMULTIPLEXER( "CNcmUdpSender::~CNcmUdpSender" )
    }


// ---------------------------------------------------------------------------
// CNcmUdpSender::SetDestination
// ---------------------------------------------------------------------------
//
void CNcmUdpSender::SetDestination( const TInetAddr& aDestination )
    {
    __CONNECTIONMULTIPLEXER_ADDRLOG( 
        "CNcmUdpSender::SetDestination - ADDRESS: ", aDestination )
        
    iDestination = aDestination;
    }


// ---------------------------------------------------------------------------
// From class CNcmSender
//
// CNcmUdpSender::DoSend
// ---------------------------------------------------------------------------
//
void CNcmUdpSender::DoSend()
    {
    __CONNECTIONMULTIPLEXER( "CNcmUdpSender::DoSend" )
    
    TInt count = iSendItemArray.Count();
    TInt lastOneIndex = count - 1;
    
    if ( count && !IsActive() )
        {
        if ( iDestination.CmpAddr( KAFUnspec ) )
            {
            __CONNECTIONMULTIPLEXER( 
                "CNcmUdpSender::DoSend - Error, DESTINATION not set" )
            
            // Remove last one       
            delete iSendItemArray[lastOneIndex];
            iSendItemArray.Remove( lastOneIndex );
                    
            iObserver.Error( KErrNotReady );
            return;
            }
            
        __CONNECTIONMULTIPLEXER_ADDRLOG(
            "CNcmUdpSender::DoSend - ADDRESS: ", iDestination )
        
        // Send oldest message
        iSendItemArray[0]->SetSendInProgress( ETrue );
        iSocket.SendTo( iSendItemArray[0]->Data(), iDestination, 0, iStatus );
        SetActive();
        }
    }
