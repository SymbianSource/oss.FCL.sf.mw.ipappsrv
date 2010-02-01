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
* Description:    Connection multiplexer is responsible for handling network
*                connections, send and receive data from the
*                network and allow client to start media flow between
*                connection multiplexer and socket media connection wrapper.
*
*/




#include <in_sock.h>
 
#include "cncmconnectionmultiplexer.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "cncmportstore.h"
#include "cncmmediasource.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmsession.h"
#include "cncmstream.h"
#include "cncmconnection.h"
#include "natfwmediawrapper.h"
#include "cncmcallbackexecuter.h"

const TUint KFirstSessionId = 1;

// Media connection is allways the first one
const TUint KMediaConnection = 1;

const TUint KDefaultProtocolSource = 0;

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CNcmConnectionMultiplexer
// ---------------------------------------------------------------------------
//
CNcmConnectionMultiplexer::CNcmConnectionMultiplexer(
    MNcmConnectionMultiplexerObserver& aObserver ) :
    iObserver( aObserver ), iNextSessionId( KFirstSessionId )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CNcmConnectionMultiplexer* CNcmConnectionMultiplexer::NewL(
        MNcmConnectionMultiplexerObserver& aObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::NewL" )

    CNcmConnectionMultiplexer* self =
        new (ELeave) CNcmConnectionMultiplexer( aObserver );
    CleanupStack::PushL( self );        
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ConstructL
// -----------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::~CNcmConnectionMultiplexer
// ---------------------------------------------------------------------------
//
CNcmConnectionMultiplexer::~CNcmConnectionMultiplexer()
    {
    __CONNECTIONMULTIPLEXER( 
        "CNcmConnectionMultiplexer::~CNcmConnectionMultiplexer" )
        
    iSessions.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateSessionL( TUint32 /*aIapId*/,
    TUint /*aPortRangeStart*/, TUint /*aPortRangeStop*/ )
    {   
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateStreamL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateStreamL( TUint /*aSessionId*/,
    TInt /*aQos*/, TUint /*aProtocol*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetStreamInfoL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::GetStreamInfoL( 
    TUint /*aStreamId*/, TUint32& /*aIapId*/, TInt& /*aQos*/,
    TUint& /*aProtocol*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::LocalIPAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C TInetAddr& CNcmConnectionMultiplexer::LocalIPAddressL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/ )
    {
    }

    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ResolveDestinationAddressL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::ResolveDestinationAddressL(
    TUint /*aStreamId*/, const TDesC8& /*aAddress*/, TUint /*aPort*/,
    TInetAddr& /*aResult*/ )
    {
    }
 
   
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint /*aStreamId*/, TUint /*aAddrFamily*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CreateConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CNcmConnectionMultiplexer::CreateConnectionL( 
    TUint /*aStreamId*/, const TInetAddr& /*aLocalAddr*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNATFWConnectionMultiplexer::OpenTcpConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::OpenTcpConnectionL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/, TNATFWTcpConnectionSetup /*aConfig*/,
    const TInetAddr& /*aDestAddr*/ )
    { 
    }

// ---------------------------------------------------------------------------
// CNATFWConnectionMultiplexer::CloseTcpConnection
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::CloseTcpConnection(
        TUint /*aStreamId*/, TUint /*aConnectionId*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveSessionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveSessionL( TUint /*aSessionId*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveStreamL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveStreamL( TUint /*aStreamId*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RemoveConnectionL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RemoveConnectionL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    RegisterIncomingConnectionObserverL( TUint /*aStreamId*/,
        MNcmIncomingConnectionObserver& /*aObserver*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    RegisterOutgoingConnectionObserverL(
        TUint /*aStreamId*/, MNcmOutgoingConnectionObserver& /*aObserver*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterIncomingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    UnregisterIncomingConnectionObserverL(
        TUint /*aStreamId*/,MNcmIncomingConnectionObserver& /*aObserver*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterOutgoingConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::
    UnregisterOutgoingConnectionObserverL(
        TUint /*aStreamId*/, MNcmOutgoingConnectionObserver& /*aObserver*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterConnectionObserverL(
    TUint /*aStreamId*/, MNcmConnectionObserver& /*aObserver*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterConnectionObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::UnregisterConnectionObserverL(
    TUint /*aStreamId*/, MNcmConnectionObserver& /*aObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMessageObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterMessageObserverL(
    TUint /*aStreamId*/, MNcmMessageObserver& /*aObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::UnregisterMessageObserverL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::UnregisterMessageObserverL(
    TUint /*aStreamId*/, MNcmMessageObserver& /*aObserver*/ )
    {
    }
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::RegisterMediaWrapperL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::RegisterMediaWrapperL(
    MNATFWMediaWrapper* /*aMediaWrapper*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/,
    const TInetAddr& /*aDestinationAddress*/,
    MNcmSenderObserver* /*aSenderObserver*/  )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    const TDesC8& /*aMessage*/, const TInetAddr& /*aNextHopAddress*/,
    const TInetAddr& /*aDestinationAddress*/ )
    {
    }
        
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const TDesC8& /*aMessage*/, TBool /*aSendDirectly*/,
    MNcmSenderObserver* /*aSenderObserver*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::CancelMessageSendL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::CancelMessageSendL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, const MNcmSenderObserver* /*aSenderObserver*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmConnectionMultiplexer::CancelMessageSendL " )
        
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::PortStore
// ---------------------------------------------------------------------------
//
EXPORT_C CNcmPortStore& CNcmConnectionMultiplexer::PortStoreL(
    TUint /*aSessionId*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetSessionInfoL
// ---------------------------------------------------------------------------
//
EXPORT_C RSocketServ* CNcmConnectionMultiplexer::GetSessionInfoL(
    TUint /*aSessionId*/, TName& /*aConnectionName*/ )
    {
    return NULL;
    }
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetSendingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetSendingStateL(
        TUint /*aStreamId*/, TUint /*aConnectionId*/, const TInetAddr& /*aDestAddr*/,
        TNATFWStreamingState /*aState*/ )
    {
    }
    
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SetReceivingStateL
// ---------------------------------------------------------------------------
//
EXPORT_C void CNcmConnectionMultiplexer::SetReceivingStateL(
    TUint /*aStreamId*/, TUint /*aConnectionId*/, TNATFWStreamingState /*aState*/ )
    {
    }
 
   
// ---------------------------------------------------------------------------
// From class MMultiplexerConnectionObserver
//
// CNcmConnectionMultiplexer::ConnectionError
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionError(
    TUint /*aSessionId*/, TUint /*aStreamId*/, TUint /*aConnectionId*/,
    TMultiplexerConnectionNotifyType /*aNotifyType*/, TInt /*aError*/ )
    {
    }


// ---------------------------------------------------------------------------
// From class MMultiplexerSocketDestinationObserver
//
// CNcmConnectionMultiplexer::GetNewFreePort
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetNewFreePort( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TUint& /*aPort*/ )
    {
    }
    

// ---------------------------------------------------------------------------
// From class MNcmSocketDestinationObserver
//
// CNcmConnectionMultiplexer::ConnectionNotify
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionNotify( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TMultiplexerConnectionNotifyType /*aType*/,
    TInt /*aError*/ )
    {
    } 

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::ConnectionNotifyL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::ConnectionNotifyL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TMultiplexerConnectionNotifyType /*aType*/,
    TInt /*aError*/ )
    {
    } 
          
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::GetNewFreePortL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetNewFreePortL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TUint& /*aPort*/ )
    {
    }
        
// ---------------------------------------------------------------------------
// From class MNcmSessionObserver
//
// Called as a result for CreateSessionL() when wrapper is connected
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::SessionCreationFailed( TUint /*aSessionId*/ )
    {
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SessionByIdL()
// ---------------------------------------------------------------------------
//       
CNcmSession* CNcmConnectionMultiplexer::SessionByIdL(
    TUint /*aSessionId*/ )
    {    
    }
 
    
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::StreamByIdL()
// ---------------------------------------------------------------------------
//       
CNcmStream* CNcmConnectionMultiplexer::StreamByIdL(
    TUint /*aStreamId*/ )
    {   
    }


// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::StreamById()
// ---------------------------------------------------------------------------
//       
CNcmStream* CNcmConnectionMultiplexer::StreamById(
    TUint /*aStreamId*/ )
    {
    }
 
// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::SessionByStreamIdL()
// ---------------------------------------------------------------------------
//       
CNcmSession* CNcmConnectionMultiplexer::SessionByStreamIdL(
    TUint /*aStreamId*/)
    {   

    }   

// ---------------------------------------------------------------------------
// From class MNcmMediaSourceObserver
//
// CNcmConnectionMultiplexer::WrapperSenderError
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::WrapperSenderError( TUint /*aStreamId*/,
    TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MNcmMediaSourceObserver
//
// CNcmConnectionMultiplexer::GetFreePort
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::GetFreePort( TUint /*aStreamId*/, TUint& /*aPort*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MNcmMediaSourceObserver
//
// CNcmConnectionMultiplexer::FirstMediaPacketSent
// ---------------------------------------------------------------------------
// 
void CNcmConnectionMultiplexer::FirstMediaPacketSent( TUint /*aStreamId*/ )
    {
    }

// ---------------------------------------------------------------------------
// From class MNcmAsyncCallbackObserver
//
// CNcmConnectionMultiplexer::HandleCallBack
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::HandleCallBack( CBase& /*aObject*/,
    TUint /*aStreamId*/, TUint /*aConnectionId*/,
    TMultiplexerConnectionNotifyType /*aType*/, TInt /*aError*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNcmConnectionMultiplexer::HandleCallBackL
// ---------------------------------------------------------------------------
//
void CNcmConnectionMultiplexer::HandleCallBackL( TUint /*aStreamId*/,
    TUint /*aConnectionId*/, TMultiplexerConnectionNotifyType /*aType*/, TInt /*aError*/ )
    {
    }
