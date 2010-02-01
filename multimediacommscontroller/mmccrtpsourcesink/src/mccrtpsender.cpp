/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP Datasink
*
*/




// INCLUDE FILES
#include "mccrtpsender.h"
#include "mccrtpdefs.h"
#include "mcctimermanager.h"

// CONSTANTS
const TInt KMccMaxSendQueueSize = 30;
const TUint32 KMccRtpSendTimeoutLongMillisecs = 600;


// -----------------------------------------------------------------------------
// CMccRtpSender::NewL
// -----------------------------------------------------------------------------
//                     
CMccRtpSender* CMccRtpSender::NewL( 
    MMccRtpSenderObserver& aObserver, 
    CRtpAPI& aRtpApi, 
    TRtpId aSessionId,
    TBool aDoMarkerBasedCleanup )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::NewL, entry" )
    
    CMccRtpSender* self = 
        new ( ELeave ) CMccRtpSender( 
            aObserver, aRtpApi, aSessionId, aDoMarkerBasedCleanup );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::NewL, exit" )
    
    return self;
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::~CMccRtpSender
// -----------------------------------------------------------------------------
//    
CMccRtpSender::~CMccRtpSender()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::~CMccRtpSender, entry" )
    
    Cancel();
    delete iTimeoutTimer;
    
    iSendQueue.ResetAndDestroy();
    
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::~CMccRtpSender, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::SendRtpPacketL
// -----------------------------------------------------------------------------
//    
void CMccRtpSender::SendRtpPacketL( 
    TRtpId aRtpStreamId, 
    const TRtpSendHeader& aRtpHeader, 
    const TDesC8& aData )
    {
    TInt count( iSendQueue.Count() );
    
    if ( CheckDiscarding( aRtpHeader ) )
        {
        TRACE_RTP_SINK_PRINT2( "CMccRtpSender::SendRtpPacketL [%d], packet dropped 1", 
                               reinterpret_cast<TUint32>( this ) )
        return;
        }
    
    if ( count >= KMccMaxSendQueueSize )
        {
        // Full, frame must be dropped. Check also if there's already in queue
        // rtp packets which contain parts of the same frame. In that case,
        // those can be also dropped as the frame is useless if something is
        // missing from it.
        DoQueueCleanup( count - 1, aRtpHeader );
        
        TRACE_RTP_SINK_PRINT2( "CMccRtpSender::SendRtpPacketL [%d], packet dropped 2",
                               reinterpret_cast<TUint32>( this ) )
        return;
        }
    
    TRACE_RTP_SINK_PRINT3( "CMccRtpSender::SendRtpPacketL [%d], items in queue:%d", 
                           reinterpret_cast<TUint32>( this ), count )
        
    CMccRtpSendItem* sendItem = 
        CMccRtpSendItem::NewLC( aRtpStreamId, aRtpHeader, aData );
    iSendQueue.AppendL( sendItem );
    CleanupStack::Pop( sendItem );
 
    if ( !IsActive() )
        {
        SendPacketL();
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::SendDataL
// -----------------------------------------------------------------------------
//
void CMccRtpSender::SendDataL( TRtpId aSessionId, TBool aUseRtpSocket, 
    const TDesC8& aData )
    {
    if ( KMccMaxSendQueueSize <= iSendQueue.Count() )
        {
        TRACE_RTP_SINK_PRINT2( 
            "CMccRtpSender::SendDataL [%d], queue full, leaving",
            reinterpret_cast<TUint32>( this ) )
        User::Leave( KErrOverflow );
        }
    else
        {
        TRACE_RTP_SINK_PRINT3( 
            "CMccRtpSender::SendDataL [%d], items in queue: %d",  
            reinterpret_cast<TUint32>( this ), iSendQueue.Count() )
        
        CMccRtpSendItem* sendItem = 
            CMccRtpSendItem::NewLC( aSessionId, aUseRtpSocket, aData );
        iSendQueue.AppendL( sendItem );
        CleanupStack::Pop( sendItem );
        
        if ( !IsActive() )
            {
            SendPacketL();
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::Clear
// -----------------------------------------------------------------------------
//
void CMccRtpSender::Clear()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::Clear, entry" )
    
    Cancel();
    iSendQueue.ResetAndDestroy();
    
    iDiscarding = EFalse;
    
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::Clear, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::RunL
// -----------------------------------------------------------------------------
//
void CMccRtpSender::RunL()
    {
    TInt status = iStatus.Int();
    
    TRACE_RTP_SINK_PRINT3( "CMccRtpSender::RunL [%d], status:%d", 
                           reinterpret_cast<TUint32>( this ), status )
    
    RemovePacket();
    
    if ( !ErrorReport( status ) )
        {
        SendPacketL();
        } 
    
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::RunL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::DoCancel
// -----------------------------------------------------------------------------
//    
void CMccRtpSender::DoCancel()
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::DoCancel, entry" )
    
    iRtpApi.CancelSend( iSessionId );
    iTimeoutTimer->Stop( iTimerId );
    
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::DoCancel, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::RunError
// -----------------------------------------------------------------------------
//
TInt CMccRtpSender::RunError( TInt aError )
    {
    TRACE_RTP_SINK_PRINT2( "CMccRtpSender::RunError, err:%d", aError )
    
    ErrorReport( aError );
        
    if ( aError != KErrNoMemory )
        {
        aError = KErrNone;
        }
        
    return aError;
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::InactivityTimeout
// -----------------------------------------------------------------------------
//    
void CMccRtpSender::TimerExpiredL( 
    TMccTimerId /*aTimerId*/, TAny* /*aTimerParam*/ )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::TimerExpiredL, entry" )
    
    if ( iTimeoutTimeMilliseconds > KMccRtpSendTimeoutLongMillisecs )
        {
        // Timeout value is so long that if sending of one rtp packet really
        // took so long, receiver will have big problems and it's better
        // to drop all frames queued in order to avoid delay increase 
        // at receiver side.
        Clear();
        }
    else
        {
        // Sending timeout, cancel send and discard that packet
        Cancel();
        
        // Do also cleanup of related packets as those are useless as this 
        // packet is removed because of timeout
        RemovePacket( ETrue ); 
        
        // Send next packet anyway
        TRAPD( err, SendPacketL() )
        if ( err )
            {
            ErrorReport( err );
            }
        }
    
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::TimerExpiredL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::ConstructL
// -----------------------------------------------------------------------------
//
void CMccRtpSender::ConstructL()
    {
    iTimeoutTimer = CMccTimerManager::NewL();
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::CMccRtpSender
// -----------------------------------------------------------------------------
//    
CMccRtpSender::CMccRtpSender( 
    MMccRtpSenderObserver& aObserver, 
    CRtpAPI& aRtpApi, 
    TRtpId aSessionId,
    TBool aDoMarkerBasedCleanup ) :
    CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iRtpApi( aRtpApi ),
    iSessionId( aSessionId ),
    iDoMarkerBasedCleanup( aDoMarkerBasedCleanup ),
    iTimeoutTimeMilliseconds( KMccRtpSendTimeoutLongMillisecs )
    {
    CActiveScheduler::Add( this );
    
    TRACE_RTP_SINK_PRINT2( "CMccRtpSender::CMccRtpSender, marker based cleanup:%d", 
                           iDoMarkerBasedCleanup )
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::SendPacketL
// -----------------------------------------------------------------------------
//
void CMccRtpSender::SendPacketL()
    {
    if ( iSendQueue.Count() > 0 )
        {
        CMccRtpSendItem* item = iSendQueue[ 0 ];
        if ( KNullId == item->SessionId( ) )
            {
            User::LeaveIfError( 
                iRtpApi.SendRtpPacket( 
                    item->RtpStreamId(), 
                    item->RtpHeader(), 
                    item->DataPtr(), 
                    iStatus ) );            
            }
        else
            {
            iRtpApi.SendDataL( item->SessionId(), item->UseRtpSocket(), 
                item->DataPtr(), iStatus );
            }
        
        SetActive();
        
        iTimeoutTimer->Stop( iTimerId );
        iTimerId = iTimeoutTimer->StartL( this, iTimeoutTimeMilliseconds );
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::RemovePacket
// -----------------------------------------------------------------------------
//    
void CMccRtpSender::RemovePacket( TBool aDoQueueCleanup )
    {
    if ( iSendQueue.Count() > 0 )
        {
        TBool removed( EFalse );
        
        if ( aDoQueueCleanup )
            {
            removed = DoQueueCleanup( 0, iSendQueue[ 0 ]->RtpHeader() );
            }
        else
            {
#ifdef TRACE_RTP_SINK
            TRACE_RTP_SINK_PRINT2( "CMccRtpSender::RemovePacket, sent packet size: %d",
                                   iSendQueue[ 0 ]->DataPtr().Length() )
#endif 
            }
        
        if ( !removed )
            {
            delete iSendQueue[ 0 ];
            iSendQueue.Remove( 0 );
            }
            
        iTimeoutTimer->Stop( iTimerId );
        }
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::ErrorReport
// -----------------------------------------------------------------------------
//
TBool CMccRtpSender::ErrorReport( TInt aError )
    {    
    TBool errorReported( EFalse );
    
    if ( aError != KErrNone && aError != KErrCancel && aError != KErrTooBig )
        {
        iObserver.SendErrorOccured( aError );
        errorReported = ETrue;
        } 
        
    return errorReported;
    }

// -----------------------------------------------------------------------------
// CMccRtpSender::CheckDiscarding
// -----------------------------------------------------------------------------
//
TBool CMccRtpSender::CheckDiscarding( const TRtpSendHeader& aRtpHeader )
    {
    TBool discard( EFalse );
    if ( iDiscarding )
        {                
        if ( aRtpHeader.iMarker )
            {
            TRACE_RTP_SINK_PRINT( "CMccRtpSender::CheckDiscarding, stop discarding" )
            iDiscarding = EFalse;
            }
        discard = ETrue;
        }
    return discard;
    }
    
// -----------------------------------------------------------------------------
// CMccRtpSender::DoQueueCleanup
// -----------------------------------------------------------------------------
//
TBool CMccRtpSender::DoQueueCleanup( TInt aItemIndex, 
        const TRtpSendHeader& aRtpHeader )
    {
    TRACE_RTP_SINK_PRINT( "CMccRtpSender::DoQueueCleanup, entry" )
    
    TInt queueCount = iSendQueue.Count();
    TInt itemIndex = aItemIndex;
    TBool itemRemoved = EFalse;
    TBool markerEnabled( aRtpHeader.iMarker );
    
    if ( !iDoMarkerBasedCleanup || itemIndex >= queueCount )
        {
        TRACE_RTP_SINK_PRINT( "CMccRtpSender::DoQueueCleanup (disabled), exit" )
        return itemRemoved;
        }
    
    // Check if this item is zero marker packet or if there's items with zero
    // marker before this one in the queue. All of those can be removed as they
    // belong to the same frame.
    //
    TBool removingCompleted( EFalse );
    for ( TInt i = itemIndex; i >= 0 && !removingCompleted; i-- )
        {
        if ( !iSendQueue[ i ]->RtpHeader().iMarker )
            {
            TRACE_RTP_SINK_PRINT( "CMccRtpSender::DoQueueCleanup, clean earlier packet" )
            
            delete iSendQueue[ i ];
            iSendQueue.Remove( i );
            itemIndex--;
            
            itemRemoved = ( itemRemoved || ( i == aItemIndex ) );
            
            if ( i == 0 )
                {
                // Deleted first item which is always the one which is sent currently,
                // must cancel sending
                TRACE_RTP_SINK_PRINT( "CMccRtpSender::DoQueueCleanup, deleted active item" )
                Cancel();
                }
            }
        else
            {
            removingCompleted = ETrue;
            }
        }
    
    if ( !markerEnabled )
        {
        // If zero marker frame was removed, all upcoming frames until
        // next marker frame can be removed (also that marker frame can be removed)
        //
        removingCompleted = EFalse;
        
        TInt beginIndex( itemIndex + 1 );
        for ( TInt j = beginIndex; j < iSendQueue.Count() && !removingCompleted; j++ )
            {
            TRACE_RTP_SINK_PRINT( "CMccRtpSender::DoQueueCleanup, clean next packet" )
            
            TUint8 marker = iSendQueue[ j ]->RtpHeader().iMarker;
              
            delete iSendQueue[ j ];
            iSendQueue.Remove( j );
                
            if ( marker )
                {
                removingCompleted = ETrue;
                }
            
            // Start again from beginning as packet was removed from queue    
            j = beginIndex - 1;
            }
            
        if ( !removingCompleted )
            {
            // Following packets of the same frame can be discarded when
            // those are tried to be sent.
            iDiscarding = ETrue;
            }
        else
            {
            iDiscarding = EFalse;
            }
        }

    
    TRACE_RTP_SINK_PRINT2( "CMccRtpSender::DoQueueCleanup, exit with discarding:%d",
                           iDiscarding )
    return itemRemoved;
    }
    
// ========================== HELPER CLASS  ====================================

// -----------------------------------------------------------------------------
// CMccRtpSendItem::NewLC
// -----------------------------------------------------------------------------
//
CMccRtpSendItem* CMccRtpSendItem::NewLC( 
    TRtpId aRtpStreamId, 
    const TRtpSendHeader& aRtpHeader, 
    const TDesC8& aData )
    {
    CMccRtpSendItem* self = 
        new ( ELeave ) CMccRtpSendItem( aRtpStreamId, aRtpHeader );
    CleanupStack::PushL( self );
    self->ConstructL( aData );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccRtpSendItem::NewLC
// -----------------------------------------------------------------------------
//
CMccRtpSendItem* CMccRtpSendItem::NewLC( TRtpId aSessionId, 
    TBool aUseRtpSocket, const TDesC8& aData )
    {
    CMccRtpSendItem* self = 
        new ( ELeave ) CMccRtpSendItem( aSessionId, aUseRtpSocket );
    CleanupStack::PushL( self );
    self->ConstructL( aData );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccRtpSendItem::~CMccRtpSendItem
// -----------------------------------------------------------------------------
//
CMccRtpSendItem::~CMccRtpSendItem()
    {
    delete iData;
    }

// -----------------------------------------------------------------------------
// CMccRtpSendItem::DataPtr
// -----------------------------------------------------------------------------
//    
const TDesC8& CMccRtpSendItem::DataPtr() const
    {
    return iDataPtr;
    }

// -----------------------------------------------------------------------------
// CMccRtpSendItem::RtpStreamId
// -----------------------------------------------------------------------------
// 
TRtpId CMccRtpSendItem::RtpStreamId() const
    {
    return iRtpStreamId;
    }

// -----------------------------------------------------------------------------
// CMccRtpSendItem::RtpHeader
// -----------------------------------------------------------------------------
//   
const TRtpSendHeader& CMccRtpSendItem::RtpHeader() const
    {
    return iRtpHeader;
    }

// -----------------------------------------------------------------------------
// CMccRtpSendItem::SessionId
// -----------------------------------------------------------------------------
// 
TRtpId CMccRtpSendItem::SessionId() const
    {
    return iSessionId;
    }


// -----------------------------------------------------------------------------
// CMccRtpSendItem::UseRtpSocket
// -----------------------------------------------------------------------------
// 
TBool CMccRtpSendItem::UseRtpSocket() const
    {
    return iUseRtpSocket;
    }


// -----------------------------------------------------------------------------
// CMccRtpSendItem::ConstructL
// -----------------------------------------------------------------------------
//    
void CMccRtpSendItem::ConstructL( const TDesC8& aData )
    {
    iData = aData.AllocL();
    iDataPtr.Set( *iData );
    }

// -----------------------------------------------------------------------------
// CMccRtpSendItem::CMccRtpSendItem
// -----------------------------------------------------------------------------
//    
CMccRtpSendItem::CMccRtpSendItem( 
    TRtpId aRtpStreamId, 
    const TRtpSendHeader& aRtpHeader ) :
    iRtpStreamId( aRtpStreamId ),
    iRtpHeader( aRtpHeader ),
    iDataPtr( NULL, 0 ),
    iSessionId( KNullId )
    {
    }


// -----------------------------------------------------------------------------
// CMccRtpSendItem::CMccRtpSendItem
// -----------------------------------------------------------------------------
//    
CMccRtpSendItem::CMccRtpSendItem( 
    TRtpId aSessionId, 
    TBool aUseRtpSocket ) 
    :
    iRtpStreamId( KNullId ),
    iDataPtr( NULL, 0 ),
    iSessionId( aSessionId ),
    iUseRtpSocket( aUseRtpSocket )
    {
    }

//  End of File  

