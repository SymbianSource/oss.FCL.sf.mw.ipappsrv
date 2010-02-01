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
* Description:    ICMP receiver
*
*/




#include <ip4_hdr.h>
#include <icmp6_hdr.h> // This is identical to ICMPv4 header
#include <udp_hdr.h>
#include <in_sock.h>
#include "cncmicmpv4receiver.h"
#include "cncmicmpsender.h"
#include "mncmicmpobserver.h"
#include "ncmconnectionmultiplexerlogs.h"


// -----------------------------------------------------------------------------
// CNcmIcmpV4Receiver::NewL
// -----------------------------------------------------------------------------
//
CNcmIcmpV4Receiver* CNcmIcmpV4Receiver::NewL( RSocketServ& aServer,
    RConnection& aConnection, MNcmIcmpObserver& aObserver )
    {
    CNcmIcmpV4Receiver* self =
        new ( ELeave ) CNcmIcmpV4Receiver( aServer, aConnection, aObserver );
            
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV4Receiver::CNcmIcmpV4Receiver
// -----------------------------------------------------------------------------
//
CNcmIcmpV4Receiver::CNcmIcmpV4Receiver( RSocketServ& aServer,
    RConnection& aConnection, MNcmIcmpObserver& aObserver ):
    CActive( CActive::EPriorityStandard ),
    iServer( aServer ),
    iConnection( aConnection ),
    iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV4Receiver::ConstructL
// -----------------------------------------------------------------------------
//
void CNcmIcmpV4Receiver::ConstructL()
    {
    User::LeaveIfError( iSocket.Open(
        iServer, KAfInet, KSockDatagram, KProtocolInetIcmp, iConnection ) );
                                          
    iIcmpSender = CNcmIcmpSender::NewL( iSocket );

    Receive();
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV4Receiver::~CNcmIcmpV4Receiver
// -----------------------------------------------------------------------------
//
CNcmIcmpV4Receiver::~CNcmIcmpV4Receiver()
    {
    Cancel();
    
    delete iIcmpSender;
    iSocket.Close();
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV4Receiver::RunL
// -----------------------------------------------------------------------------
//
void CNcmIcmpV4Receiver::RunL()
    {
    if ( iStatus == KErrNone )
        {
        CheckError();
        Receive();
        }
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV4Receiver::CheckError
// -----------------------------------------------------------------------------
//
void CNcmIcmpV4Receiver::CheckError()
    {
    const TUint KDestinationUnreachable = 3;

    // Pointer to ICMP header
    TInet6HeaderICMP* icmpPtr( ( TInet6HeaderICMP* ) iData.Ptr() );

    // Pointer to IPv4 header
    const TInt KIPHeaderOffset = 8;
    TInet6HeaderIP4* ipv4Ptr =
        ( TInet6HeaderIP4* )( iData.Ptr() + KIPHeaderOffset );

    // Error message type
    TUint32 error( icmpPtr->Type() );
    if ( KDestinationUnreachable == error )
        {
        // Check that protocol is UDP
        TUint32 protocol( ipv4Ptr->Protocol() );
        if ( KProtocolInetUdp == protocol )
            {
            // Pointer to UDP data
            TInet6HeaderUDP* udpPtr =
                ( TInet6HeaderUDP* )( ipv4Ptr->EndPtr() );

            // Get the destination port number
            iAddress.SetPort( udpPtr->DstPort() );

            // Set the destination address
            iAddress.SetAddress( ipv4Ptr->DstAddr() );

            __CONNECTIONMULTIPLEXER_INT1(
                "CNcmIcmpV4Receiver::CheckError - ICMP error:", error )
            __CONNECTIONMULTIPLEXER_ADDRLOG(
                "CNcmIcmpV4Receiver::CheckError - DstAddr:", iAddress )

            // Error message to observer
            TInetAddr localAddress;
            TInetAddr remoteAddress;
            iObserver.IcmpError( iAddress, localAddress, remoteAddress );
            
            if ( !( remoteAddress.IsUnspecified() ||
                localAddress.IsUnspecified() ) )
                {
                udpPtr->SetDstPort( localAddress.Port() );
                udpPtr->SetSrcPort( remoteAddress.Port() );
                ipv4Ptr->SetDstAddr( localAddress.Address() );
                ipv4Ptr->SetSrcAddr( remoteAddress.Address() );
                iIcmpSender->Send( iData, remoteAddress );
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CNcmIcmpV4Receiver::DoCancel
// -----------------------------------------------------------------------------
//
void CNcmIcmpV4Receiver::DoCancel()
    {
    iSocket.CancelRecv();
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV4Receiver::Receive
// -----------------------------------------------------------------------------
//
void CNcmIcmpV4Receiver::Receive()
    {
    if ( !IsActive() )
        {
        iSocket.Recv( iData, 0, iStatus );
        SetActive();
        }
    }
