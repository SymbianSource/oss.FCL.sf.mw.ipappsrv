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
* Description:    Send data to User of NAT Connectivity Framework.
*
*/




#include "natfwsocketsender.h"
#include "natfwsocketmediaconnwrapperlogs.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CNATFWSocketSender::CNATFWSocketSender( )
// ---------------------------------------------------------------------------
//
CNATFWSocketSender::CNATFWSocketSender( MNATFWSocketSenderObserver& aObserver,
    TUint aProtocol ) : CActive(EPriorityUserInput), iProtocol( aProtocol ),
    iObserver( aObserver ), iSendQueue( CNsmcwSendItem::iOffset ),
    iQueueIter( iSendQueue )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketSender::CNATFWSocketSender" )

    CActiveScheduler::Add(this);
    }


// ---------------------------------------------------------------------------
// CNATFWSocketSender::NewL(  )
// ---------------------------------------------------------------------------
//
CNATFWSocketSender* CNATFWSocketSender::NewL( 
    MNATFWSocketSenderObserver& aObserver, TUint aProtocol )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketSender::NewL" )
    
    CNATFWSocketSender* self = 
            new (ELeave) CNATFWSocketSender ( aObserver, aProtocol );

    return self;
    }


// ---------------------------------------------------------------------------
// CNATFWSocketSender::~CNATFWSocketSender()
// ---------------------------------------------------------------------------
//
CNATFWSocketSender::~CNATFWSocketSender()
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketSender::~CNATFWSocketSender")
    
    Cancel();
    
    iQueueIter.SetToFirst();
    CNsmcwSendItem* item = iQueueIter++;
    while ( item )
        {
        iSendQueue.Remove( *item );
        delete item;
        item = iQueueIter++;
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSocketSender::SetRemoteAddress( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketSender::SetRemoteAddress( const TSockAddr& aRemoteAddress,
                                           const RSocket& aSocket )
    {
    iRemoteAddress = aRemoteAddress;
    iSocket = aSocket;
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSocketSender::SendL(  )
// ---------------------------------------------------------------------------
//
void CNATFWSocketSender::SendL( const TDesC8& aStreamPortion )
    {    
    CNsmcwSendItem* item = NULL;    
    TRAPD( err, item = CNsmcwSendItem::NewL( aStreamPortion ) );
    
    if ( KErrNone == err && item )
        {
        iSendQueue.AddLast( *item );
        }
    else
        {
        User::Leave( err );
        }
    if ( !IsActive() )
        {
        SendNextPacket();
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSocketSender::SendNextPacket()
// ---------------------------------------------------------------------------
//
void CNATFWSocketSender::SendNextPacket()
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketSender::SendNextPacket" )

    iQueueIter.SetToFirst();
    CNsmcwSendItem* item = iQueueIter++;
    if ( item )
        {
        if( KProtocolInetUdp == iProtocol )
            {
            iSocket.SendTo( item->GetData(), iRemoteAddress, 0, iStatus );
            }
        if( KProtocolInetTcp == iProtocol )
            {
            iSocket.Send( item->GetData(), 0, iStatus );
            } 
        SetActive();
        }
        
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketSender::SendNextPacket - End" )    
    }


// ---------------------------------------------------------------------------
// CNATFWSocketSender::RemoveFromQueue()
// ---------------------------------------------------------------------------
//
void CNATFWSocketSender::RemoveFromQueue()
    {    
    if ( !( iSendQueue.IsEmpty( ) ) )
        {
        CNsmcwSendItem* item = iSendQueue.First();
        iSendQueue.Remove( *item );
        delete item;
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSocketSender::RunL( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketSender::RunL( )
    {
    __SOCKETMEDIACONNWRAPPER_INT1( 
        "CNATFWSocketSender::RunL iStatus:", iStatus.Int() )  

    iObserver.SendingCompleted( iStatus.Int() );
    
    if ( KErrNone == iStatus.Int() )
        {
        RemoveFromQueue();
        SendNextPacket();
        }
    else 
        {
        __SOCKETMEDIACONNWRAPPER( "CNATFWSocketSender::RunL - Sending failed" )
        
        iQueueIter.SetToFirst();
        CNsmcwSendItem* item = iQueueIter++;
        while ( item )
            {
            iSendQueue.Remove( *item );
            delete item;
            item = iQueueIter++;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSocketSender::DoCancel( )
// ---------------------------------------------------------------------------
//
void CNATFWSocketSender::DoCancel( )
    {
    __SOCKETMEDIACONNWRAPPER( "CNATFWSocketSender::DoCancel start" )

    iSocket.CancelAll();
    }
