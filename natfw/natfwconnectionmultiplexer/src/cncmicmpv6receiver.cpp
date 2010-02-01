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
* Description:   
*
*/




#include <ip6_hdr.h>
#include <icmp6_hdr.h>
#include <udp_hdr.h>
#include <ext_hdr.h>
#include <in_sock.h>
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmicmpv6receiver.h"
#include "mncmicmpobserver.h"
#include "cncmicmpsender.h"

// -----------------------------------------------------------------------------
// CNcmIcmpV6Receiver::NewL
// -----------------------------------------------------------------------------
//
CNcmIcmpV6Receiver* CNcmIcmpV6Receiver::NewL( RSocketServ& aServer,
    RConnection& aConnection, MNcmIcmpObserver& aObserver )
    {
    CNcmIcmpV6Receiver* self =
        new ( ELeave ) CNcmIcmpV6Receiver( aServer, aConnection, aObserver );
            
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV6Receiver::CNcmIcmpV6Receiver
// -----------------------------------------------------------------------------
//
CNcmIcmpV6Receiver::CNcmIcmpV6Receiver( RSocketServ& aServer,
    RConnection& aConnection, MNcmIcmpObserver& aObserver ) :
    CActive( CActive::EPriorityStandard ),
    iServer( aServer ),
    iConnection( aConnection ),
    iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV6Receiver::ConstructL
// -----------------------------------------------------------------------------
//
void CNcmIcmpV6Receiver::ConstructL()
    {
    User::LeaveIfError( iSocket.Open(
        iServer, KAfInet, KSockDatagram, KProtocolInet6Icmp, iConnection ) );
        
    iIcmpSender = CNcmIcmpSender::NewL( iSocket );

    Receive();
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV6Receiver::~CNcmIcmpV6Receiver
// -----------------------------------------------------------------------------
//
CNcmIcmpV6Receiver::~CNcmIcmpV6Receiver()
    {
    Cancel();
    
    delete iIcmpSender;
    iSocket.Close();
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV6Receiver::RunL
// -----------------------------------------------------------------------------
//
void CNcmIcmpV6Receiver::RunL()
    {
    if ( iStatus == KErrNone )
        {
        CheckError();
        Receive();
        }
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV6Receiver::CheckError
// -----------------------------------------------------------------------------
//
void CNcmIcmpV6Receiver::CheckError()
    {
    const TUint KDestinationUnreachable = 1;

    // Pointer to ICMP header
    TInet6HeaderICMP* icmpPtr( ( TInet6HeaderICMP* ) iData.Ptr() );

    // Pointer to IPv6 header
    const TInt KIPHeaderOffset = 8;
    TInet6HeaderIP* ipv6Ptr =
        ( TInet6HeaderIP* )( iData.Ptr() + KIPHeaderOffset );

    // Get the whole length of the header in bytes
    TUint32 payloadLength( ipv6Ptr->PayloadLength() );
    TUint32 offset( 0 );

    // Pointer to IPv6 extension header
    const TInt KHeaderExtensionOffset = 14;
    TInet6HeaderExtension* extPtr =
        ( TInet6HeaderExtension* )( iData.Ptr() + KHeaderExtensionOffset );
    // Get the extension header length in bytes
    TUint32 extHdrLength( extPtr->HeaderLength() );

    // Pointer to UDP data
    TInet6HeaderUDP* udpPtr = ( TInet6HeaderUDP* )( ipv6Ptr->EndPtr() );

    // Error message type
    TUint32 error( icmpPtr->Type() );
    if ( KDestinationUnreachable == error )
        {
        TUint32 nextHeader( 0 );
        do  // Search the correct header, if there are extension headers
            {
            nextHeader = extPtr->NextHeader();
            if( KProtocolInetUdp == nextHeader )
                {
                // Get the destination port number
                iAddress.SetPort( udpPtr->DstPort() );

                // Set the destination address
                iAddress.SetAddress( ipv6Ptr->DstAddr() );

                __CONNECTIONMULTIPLEXER_INT1(
                    "CNcmIcmpV6Receiver::CheckError - ICMP error:", error )
                __CONNECTIONMULTIPLEXER_ADDRLOG(
                    " CNcmIcmpV6Receiver::CheckError - DstAddr:", iAddress )

                TInetAddr localAddress;
                TInetAddr remoteAddress;
                iObserver.IcmpError( iAddress, localAddress, remoteAddress );
                            
                if ( !( remoteAddress.IsUnspecified() ||
                    localAddress.IsUnspecified() ) )
                    {
                    udpPtr->SetDstPort( localAddress.Port() );
                    udpPtr->SetSrcPort( remoteAddress.Port() );
                     
                    TInetAddr local( localAddress.Address() );
                    local.ConvertToV4Mapped();
                    ipv6Ptr->SetDstAddr( local.Ip6Address() );
                                                        
                    TInetAddr remote( remoteAddress.Address() );
                    local.ConvertToV4Mapped();
                    ipv6Ptr->SetSrcAddr( remote.Ip6Address() );
                    
                    iIcmpSender->Send( iData, remoteAddress );
                    }
                }
            else
                {
                // Step to next header
                if ( offset < payloadLength )
                    {
                    extPtr = extPtr + extHdrLength;
                    offset = offset + extHdrLength;
                    }
                }
            // Loop until UDP datagram is found. If it is not found,
            // get out of here and do nothing.
            } while ( ( KProtocolInetUdp != nextHeader ) &&
                      ( offset < payloadLength ) );
        }
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV6Receiver::DoCancel
// -----------------------------------------------------------------------------
//
void CNcmIcmpV6Receiver::DoCancel()
    {
    iSocket.CancelRecv();
    }


// -----------------------------------------------------------------------------
// CNcmIcmpV6Receiver::Receive
// -----------------------------------------------------------------------------
//
void CNcmIcmpV6Receiver::Receive()
    {
    if ( !IsActive() )
        {
        iSocket.Recv( iData, 0, iStatus );
        SetActive();
        }
    }
