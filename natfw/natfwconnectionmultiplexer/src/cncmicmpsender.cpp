/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    ICMP sender
*
*/




#include "cncmicmpsender.h"
#include "ncmconnectionmultiplexerlogs.h"

// ---------------------------------------------------------------------------
// CNcmIcmpSender::CNcmIcmpSender
// ---------------------------------------------------------------------------
//
CNcmIcmpSender::CNcmIcmpSender( RSocket& aSocket ) :
    CActive( EPriorityUserInput ),
    iIcmpSocket( aSocket )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CNcmIcmpSender::~CNcmIcmpSender
// ---------------------------------------------------------------------------
//
CNcmIcmpSender::~CNcmIcmpSender()
    {
    __CONNECTIONMULTIPLEXER( "CNcmIcmpSender::~CNcmIcmpSender" )
    
    Cancel();
    delete iData;
    }


// -----------------------------------------------------------------------------
// CNcmIcmpSender::NewL
// -----------------------------------------------------------------------------
//
CNcmIcmpSender* CNcmIcmpSender::NewL( RSocket& aSocket )
    {
    CNcmIcmpSender* self =
        new ( ELeave ) CNcmIcmpSender( aSocket );
    return self;
    }

    
// ---------------------------------------------------------------------------
// CNcmIcmpSender::Send
// ---------------------------------------------------------------------------
//
void CNcmIcmpSender::Send( const TDesC8& aMessage,
    const TInetAddr& aDestinationAddr )
    {
    __CONNECTIONMULTIPLEXER_ADDRLOG(
        "CNcmIcmpSender::Send - ADDRESS: ", aDestinationAddr )
    
    iDestination = aDestinationAddr;
    if ( !IsActive() )
        {
        if ( aDestinationAddr.IsUnspecified() )
            {
            __CONNECTIONMULTIPLEXER( 
                "CNcmIcmpSender::Send - Error, DESTINATION not set" )

            return;
            }
        else
            {
            delete iData;
            iData = NULL;
            iData = aMessage.Alloc();

            if ( iData )
                {
                iIcmpSocket.SendTo( *iData, iDestination, 0, iStatus );
                SetActive();
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// From class CActive
//
// CNcmIcmpSender::RunL
// ---------------------------------------------------------------------------
//
void CNcmIcmpSender::RunL()
    {
    __CONNECTIONMULTIPLEXER_INT1( "CNcmIcmpSender::RunL STATUS: ", iStatus.Int() )
    }

    
// ---------------------------------------------------------------------------
// From class CActive
//
// CNcmIcmpSender::DoCancel
// ---------------------------------------------------------------------------
//
void CNcmIcmpSender::DoCancel()
    {
    __CONNECTIONMULTIPLEXER( "CNcmIcmpSender::DoCancel" )
         
    iIcmpSocket.CancelSend();
    }
