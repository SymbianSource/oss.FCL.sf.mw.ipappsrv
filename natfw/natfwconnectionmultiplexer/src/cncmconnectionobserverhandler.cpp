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
* Description:    Handler for connection observers on multiplexer
*
*/




#include "cncmconnectionobserverhandler.h"
#include "mncmincomingconnectionobserver.h"
#include "mncmoutgoingconnectionobserver.h"
#include "mncmconnectionobserver.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "ncmconnectionmultiplexer.hrh"
#include "cncmmediasource.h"
#include "ncmconnectionmultiplexerassert.h"
#include "mncmmessageobserver.h"

// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::CNcmConnectionObserverHandler
// ---------------------------------------------------------------------------
//
CNcmConnectionObserverHandler::CNcmConnectionObserverHandler()
    {
    }

    
// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::NewL
// ---------------------------------------------------------------------------
//
CNcmConnectionObserverHandler*
    CNcmConnectionObserverHandler::NewL()
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionObserverHandler::NewL" )
        
    CNcmConnectionObserverHandler* self =
        CNcmConnectionObserverHandler::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::NewLC
// ---------------------------------------------------------------------------
//
CNcmConnectionObserverHandler* CNcmConnectionObserverHandler::NewLC()
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionObserverHandler::NewLC" )
        
    CNcmConnectionObserverHandler* self =
        new( ELeave ) CNcmConnectionObserverHandler;
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::~CNcmConnectionObserverHandlerL
// ---------------------------------------------------------------------------
//
CNcmConnectionObserverHandler::~CNcmConnectionObserverHandler()
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionObserverHandler::~CNcmConnectionObserverHandler" )
        
    iIncomingObservers.Close();
    iOutgoingObservers.Close();
    iConnectionObservers.Close();
    
    delete iMediaSource;
    iMessageObserver = NULL;
    
    iLatestHashes.Close();
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::RegisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::RegisterOutgoingConnectionObserverL(
    const MNcmOutgoingConnectionObserver& aOutgoingConnectionObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::RegisterOutgoingConnectionObserver" )
      
    if ( KErrNotFound == iOutgoingObservers.Find(
        &aOutgoingConnectionObserver ) )
        {
        iOutgoingObservers.AppendL( &aOutgoingConnectionObserver );
        }
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::RegisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::RegisterIncomingConnectionObserverL(
    const MNcmIncomingConnectionObserver& aIncomingConnectionObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::RegisterIncomingConnectionObserver" )
   
    if ( KErrNotFound == iIncomingObservers.Find(
                                            &aIncomingConnectionObserver ) )
        {
        iIncomingObservers.AppendL( &aIncomingConnectionObserver );
        }
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::UnregisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::UnregisterOutgoingConnectionObserverL(
    const MNcmOutgoingConnectionObserver& aOutgoingConnectionObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::UnregisterOutgoingConnectionObserver" )
       
    TInt index( 0 );   
    index = iOutgoingObservers.Find( &aOutgoingConnectionObserver );

    if ( KErrNotFound != index )
        {
        iOutgoingObservers.Remove( index );
        }
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::UnregisterIncomingConnectionObserver
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::UnregisterIncomingConnectionObserverL(
    const MNcmIncomingConnectionObserver& aIncomingConnectionObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::UnregisterIncomingConnectionObserver" )
         
    TInt index( 0 );  
    index = iIncomingObservers.Find( &aIncomingConnectionObserver );
    
    if ( KErrNotFound != index )
        {
        iIncomingObservers.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::RegisterConnectionObserverL
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::RegisterConnectionObserverL(
    const MNcmConnectionObserver& aConnectionObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::RegisterConnectionObserverL" )
   
    if ( KErrNotFound == iConnectionObservers.Find( &aConnectionObserver ) )
        {
        iConnectionObservers.AppendL( &aConnectionObserver );
        }
    }
 
 
// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::UnregisterIncomingConnectionObserver
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::UnregisterConnectionObserverL(
    const MNcmConnectionObserver& aConnectionObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::UnregisterConnectionObserverL" )
         
    TInt index( 0 ); 
    index = iConnectionObservers.Find( &aConnectionObserver );
    
    if ( KErrNotFound != index )
        {
        iConnectionObservers.Remove( index );
        }
    }   


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::RegisterMessageObserver
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::RegisterMessageObserver(
    MNcmMessageObserver& aMessageObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::RegisterMessageObserverL" )
    __CONNECTIONMULTIPLEXER_ASSERT_L(
        NULL == iMessageObserver, KErrNotReady );
       
    iMessageObserver = &aMessageObserver;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::UnregisterMessageObserver
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::UnregisterMessageObserver(
    const MNcmMessageObserver& aMessageObserver )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::UnregisterMessageObserverL" )
     __CONNECTIONMULTIPLEXER_ASSERT_L( iMessageObserver, KErrNotFound );
     __CONNECTIONMULTIPLEXER_ASSERT_L(
        &aMessageObserver == iMessageObserver, KErrArgument );
         
    iMessageObserver = NULL;
    }  
   
 
// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::IsDuplicatePacket
// Used for replicated RTP/RTCP packet filtering. This is quick fix only,
// usage of KSoReuseAddr should be minimized. Also RSocket::Connect can be
// used to associate a socket to the explicit remote address. In this case
// TCP/IP stack does not replicate packet to the other sockets bound to the
// same local transport address.
// ---------------------------------------------------------------------------
//
TBool CNcmConnectionObserverHandler::IsDuplicatePacket( 
        const TDesC8& aMessage, TBool& aIsStunMessage )
    {
    TUint hash( 0 );
    TBool isDuplicate( EFalse );
    aIsStunMessage = EFalse;
    
    const TUint KMaxValueOfFirstByteOfStunMsg( 2 );
    if ( 0 != aMessage.Length() 
            && KMaxValueOfFirstByteOfStunMsg < aMessage[0] )
        {
        // In the beginning of RTP/RTCP header there is two bit long version
        // field, which has always value of 2. Thus value of first byte of 
        // RTP/RTCP message is at least 128.
        const TInt KRtpRtcpDiffByteOffset( 1 );
        const TInt KLowestAssignedRtcpMsgType = 200;
        const TInt KHighestAssignedRtcpMsgType = 209;
        TInt rtcpType = aMessage[KRtpRtcpDiffByteOffset];
        TBool isRtcp = 
           ( KLowestAssignedRtcpMsgType <= rtcpType &&
        		   rtcpType <= KHighestAssignedRtcpMsgType );
        
        // This offset refers to RR highest sequence number received.
        const TInt KRRtcpDiffByteOffset( 19 );
        
        // This offset refers either to SR RTP packet count received.
        const TInt KSRtcpDiffByteOffset( 23 );
        
        // This offset refers to lower byte of unique RTP sequence number.
        const TInt KRtpDiffByteOffset( 3 );
        TInt diffByteOffset 
            = isRtcp ? KRRtcpDiffByteOffset : KRtpDiffByteOffset;

        diffByteOffset = (rtcpType == KLowestAssignedRtcpMsgType)?
        		KSRtcpDiffByteOffset : diffByteOffset;
        
        if ( diffByteOffset < aMessage.Size() )
            {
            hash = aMessage[diffByteOffset];
            }
        else
            {
            __CONNECTIONMULTIPLEXER( 
            "CNcmConnectionObserverHandler::IsDuplicatePacket INVALID PACKET")
            return EFalse;
            }

        if ( KErrNotFound != iLatestHashes.Find( hash ) )
            {
            // this is duplicate packet
            __CONNECTIONMULTIPLEXER(
                "CNcmConnectionObserverHandler::IsDuplicatePacket TRUE" )
            isDuplicate = ETrue;
            }
        else
            {
            iLatestHashes.Append( hash );
            const TInt KMaxHashCount( 5 );
            if ( KMaxHashCount <= iLatestHashes.Count() )
                {
                iLatestHashes.Remove( 0 );
                }
            }
        }
    else
        {
        __CONNECTIONMULTIPLEXER( 
            "CNcmConnectionObserverHandler::IsDuplicatePacket STUN MSG")
            
        aIsStunMessage = ETrue;
        }
    
    return isDuplicate;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::RevealIncomingMessageL
// ---------------------------------------------------------------------------
//
TBool CNcmConnectionObserverHandler::RevealIncomingMessageL(
    TUint aStreamId, HBufC8*& aMessage, const TInetAddr& aLocalAddr,
    const TInetAddr& aFromAddr )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::RevealIncomingMessageL" )
    
    TBool isStunMessage( EFalse );
    
    if ( IsDuplicatePacket( *aMessage, isStunMessage ) )
        {
        return ETrue;
        }
    
    TInetAddr peerRemoteAddress( KInetAddrNone, 0 );

    HBufC8* modified( NULL );
    
    if ( iMessageObserver )
        {
        modified = iMessageObserver->IncomingMessageNotify( 
            aStreamId, *aMessage, aLocalAddr, aFromAddr, peerRemoteAddress );
            
        // replace message if new content is found
        if ( modified )
            {
            delete aMessage;
            aMessage = modified;
            }
        }
        
    // offer to all incoming observers and finish when comsumer is found
    TBool consumed( EFalse );
    TInt index( iIncomingObservers.Count() );
        
    while ( index-- && !consumed )
        {
        TRAP_IGNORE( iIncomingObservers[index]->IncomingMessageL( 
            aStreamId, *aMessage, aLocalAddr, aFromAddr, peerRemoteAddress,
            consumed ) )
        }
    
    if ( iMediaSource && !consumed && !isStunMessage  )
        {
        iMediaSource->PutMessageL( *aMessage );
        consumed = ETrue;
        }
        
    return consumed;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::RevealOutgoingMessageL
// ---------------------------------------------------------------------------
//
TBool CNcmConnectionObserverHandler::RevealOutgoingMessageL(
    TUint aStreamId, TUint aConnectionId, HBufC8*& aMessage,
    const TInetAddr& aDestinationAddress )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::RevealOutgoingMessageL" )
         
    TInt observerCount( iOutgoingObservers.Count() );
    TBool consumed( EFalse );
    HBufC8* modified( NULL );
    
    if ( iMessageObserver )
        {
        modified = iMessageObserver->OutgoingMessageNotify( 
            aStreamId, aConnectionId, aDestinationAddress, *aMessage );
            
        // replace message if new content is found
        if ( modified )
            {
            delete aMessage;
            aMessage = modified;
            }
        }
    
    // offer to all outgoing observers and see if anyone consumed it
    for ( TInt i = 0; i < observerCount && !consumed; i++ )
        {
         iOutgoingObservers[i]->OutgoingMessageL( 
            aStreamId, aConnectionId, aDestinationAddress, *aMessage,
            consumed );
        }

    return consumed;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::ConnectionNotify
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::ConnectionNotify(
    TUint aStreamId, TUint aConnectionId, 
    TMultiplexerConnectionNotifyType aType, TInt aError )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::ConnectionNotify, start" )
    
    TInt ind( iConnectionObservers.Count() );
    while ( KErrNotFound != ind-- )
        {
        TInt maxIndex( iConnectionObservers.Count() - 1 );
        
        ind = ( ind >= maxIndex ) ? maxIndex : ind;
        
        if ( KErrNotFound != ind )
            {
            iConnectionObservers[ind]->ConnectionNotify( 
                aStreamId, aConnectionId,
                static_cast<MNcmConnectionObserver::TConnectionNotifyType>(
                    aType ), aError ); 
            }         
        }

    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::ConnectionNotify, end" )    
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::RegisterMediaSourceL
// ---------------------------------------------------------------------------
//
void CNcmConnectionObserverHandler::RegisterMediaSourceL( 
    CNcmMediaSource* aMediaSource )
    {
    __CONNECTIONMULTIPLEXER(
        "CNcmConnectionObserverHandler::RegisterMediaSource" )
    __CONNECTIONMULTIPLEXER_ASSERT_L( NULL != aMediaSource, KErrArgument );
    delete iMediaSource;
    iMediaSource = NULL;
    iMediaSource = aMediaSource;
    }


// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::RegisterMediaSourceObserver
// ---------------------------------------------------------------------------
//      
void CNcmConnectionObserverHandler::RegisterMediaSourceObserver( 
    MNcmSourceObserver& aMediaSourceObserver )
    {
    __CONNECTIONMULTIPLEXER_ASSERT_L( NULL != iMediaSource, KErrArgument );
    iMediaSource->RegisterMessageObserver( aMediaSourceObserver );  
    }

    
// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::UnregisterMediaSourceObserver
// ---------------------------------------------------------------------------
//     
void CNcmConnectionObserverHandler::UnregisterMediaSourceObserver(
    const MNcmSourceObserver& aMediaSourceObserver )
    {
    __CONNECTIONMULTIPLEXER_ASSERT_L( NULL != iMediaSource, KErrArgument );
    iMediaSource->UnregisterMessageObserver( aMediaSourceObserver );   
    }
 
   
// ---------------------------------------------------------------------------
// CNcmConnectionObserverHandler::SetReceivingState
// ---------------------------------------------------------------------------
//     
void CNcmConnectionObserverHandler::SetReceivingState(
    TNATFWStreamingState aState )
    {
    iMediaSource->SetReceivingState( aState );   
    }    
