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
* Description:    Abstract socket sender
*
*/




#include "cncmsender.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "mncmreceiversenderobserver.h"
#include "mncmsenderobserver.h"
#include "cncmsenditem.h"

const TInt KFirstInBuffer  = 0;


// ---------------------------------------------------------------------------
// CNcmSender::CNcmSender
// ---------------------------------------------------------------------------
//
CNcmSender::CNcmSender( RSocket& aSocket,
    MNcmReceiverSenderObserver& aObserver ) :
    CActive( EPriorityUserInput ),
    iSocket( aSocket ),
    iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CNcmSender::~CNcmSender
// ---------------------------------------------------------------------------
//
CNcmSender::~CNcmSender()
    {
    __CONNECTIONMULTIPLEXER( "CNcmSender::~CNcmSender" )

    Cancel();
    iSendItemArray.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CNcmSender::Send
// ---------------------------------------------------------------------------
//
void CNcmSender::Send( const TDesC8& aMessage,
    MNcmSenderObserver* aSenderObserver )
    {
    __CONNECTIONMULTIPLEXER_INT1( 
        "CNcmSender::Send - Send item count: ", iSendItemArray.Count() )

    CNcmSendItem* item( NULL );
    
    TRAPD( error, item = CNcmSendItem::NewL( aMessage, aSenderObserver ) )
    
    if ( item )
        {
        error = iSendItemArray.Append( item );        
        }  
    
    if ( KErrNone != error )
        {  
        if ( aSenderObserver )
            {
            aSenderObserver->MessageSentFailure( error );
            aSenderObserver = NULL;
            }
        else
            {
            iObserver.Error( error );
            }    
      
        __CONNECTIONMULTIPLEXER_INT1( 
            "CNcmSender::Send - ERRRO: ", error )
        delete item;
        return;
        }

    // start sending
    DoSend();
    }


// ---------------------------------------------------------------------------
// CNcmSender::CancelMessageSendL
// ---------------------------------------------------------------------------
//
void CNcmSender::CancelMessageSendL(
    const MNcmSenderObserver* aSenderObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmSender::CancelMessageSendL ")
        
    TInt ind( iSendItemArray.Count() );
    
    while ( ind-- )
        {
        if ( aSenderObserver == iSendItemArray[ind]->Observer() )
            {
            __CONNECTIONMULTIPLEXER_INT1(
                "CNcmSender::CancelMessageSendL - Delete: ind ", ind)
            
            if ( KFirstInBuffer  == ind )
                {
                iSocket.CancelSend();
                }
                
            delete iSendItemArray[ind];
            iSendItemArray.Remove( ind );
            }
        }
    }

   
// ---------------------------------------------------------------------------
// From class CActive
//
// CNcmSender::RunL
// ---------------------------------------------------------------------------
//
void CNcmSender::RunL()
    {
    __CONNECTIONMULTIPLEXER_INT1( "CNcmSender::RunL STATUS: ", iStatus.Int() )
    
    if ( iSendItemArray.Count() )
        {     
        TInt status( iStatus.Int() );
               
        MNcmSenderObserver* senderObserver =
            iSendItemArray.Count()
            ? iSendItemArray[KFirstInBuffer]->Observer()
            : NULL;
        
        if ( KErrNone != status )
            {
            if ( senderObserver )
                {
                senderObserver->MessageSentFailure( iStatus.Int() );
                }
            else
                {
                iObserver.Error( iStatus.Int() );
                } 
            }
        else
            {
            if ( senderObserver )
                {
                senderObserver->MessageSent();
                }   
            }

        if ( iSendItemArray.Count() )
            {
            if ( iSendItemArray[KFirstInBuffer]->SendInProgress() )
                {
                senderObserver = NULL;       
                delete iSendItemArray[KFirstInBuffer];
                iSendItemArray.Remove( KFirstInBuffer );
                }
            DoSend();
            }     
        }
    }


// ---------------------------------------------------------------------------
// From class CActive
//
// CNcmSender::DoCancel
// ---------------------------------------------------------------------------
//
void CNcmSender::DoCancel()
    {
    __CONNECTIONMULTIPLEXER( "CNcmSender::DoCancel" )
         
    iSocket.CancelSend();
    }


// ---------------------------------------------------------------------------
// CNcmSender::Activate
// ---------------------------------------------------------------------------
//
void CNcmSender::Activate() const
    {
    }


// ---------------------------------------------------------------------------
// CNcmSender::Deactivate
// ---------------------------------------------------------------------------
//
void CNcmSender::Deactivate()
    {
    Cancel();
    }
