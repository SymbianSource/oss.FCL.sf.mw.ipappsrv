/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    NATFW Connection multiplexer session abstraction.
*
*/




#include <commdbconnpref.h>
#include <e32math.h> 

#include "cncmsession.h"
#include "cncmstream.h"
//#include "natfwmultiplexerconnectionobserverhandler.h"
//#include "natfwlocaladdressresolver.h"
#include "mncmconnectionmultiplexerobserver.h"
//#include "cnatfwmultiplexericmpreceiver.h"
#include "mncmsessionobserver.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "cncmportstore.h"


// ---------------------------------------------------------------------------
// CNcmSession::CNcmSession
// ---------------------------------------------------------------------------
//
CNcmSession::CNcmSession( TUint aSessionId,
    TUint32 aIapId,
    MNcmConnectionMultiplexerObserver& aObserver,
    MNcmSessionObserver& aSessionObserver ) :
    CActive( EPriorityStandard ),
    iSessionId( aSessionId ), iIapId( aIapId ), iObserver( aObserver ),
    iSessionObserver( aSessionObserver )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CNcmSession::ConstructL
// ---------------------------------------------------------------------------
//
void CNcmSession::ConstructL( TUint aPortRangeStart,
                                           TUint aPortRangeStop )
    {
    iPortStore = CNcmPortStore::NewL( aPortRangeStart, aPortRangeStop );
    /*
    User::LeaveIfError( iSocketServ.Connect() );

    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iIapId );  
                                                 
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    iConnection.Start( pref, iStatus );
  */
    TRequestStatus* status = &iStatus;
    iStatus = KRequestPending;
    SetActive();
    
    User::RequestComplete( status, KErrNone );
    }

// ---------------------------------------------------------------------------
// CNcmSession::NewL
// ---------------------------------------------------------------------------
//
CNcmSession* CNcmSession::NewL( TUint aSessionId,
    TUint32 aIapId, TUint aPortRangeStart, TUint aPortRangeStop,
    MNcmConnectionMultiplexerObserver& aObserver,
    MNcmSessionObserver& aSessionObserver )
    {   
    CNcmSession* self =
        new( ELeave ) CNcmSession( aSessionId, aIapId,
            aObserver, aSessionObserver );
                    
    CleanupStack::PushL( self );
    self->ConstructL( aPortRangeStart, aPortRangeStop );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNcmSession::~CNcmSession
// ---------------------------------------------------------------------------
//
CNcmSession::~CNcmSession()
    {
    delete iPortStore;
    
    TInt ind( iStreams.Count() );
    
    while ( ind-- )
        {
        TRAP_IGNORE( RemoveStreamL( iStreams[ind]->StreamId() ) )
        }   
    
    //delete iIcmpReceiver;
    
    //iConnection.Close();    
    Cancel();

    //iSocketServ.Close();
    iStreams.Close();
    }

// ---------------------------------------------------------------------------
// CNcmSession::CreateStreamL
// ---------------------------------------------------------------------------
//
TUint CNcmSession::CreateStreamL( TInt aQos,
    TUint aProtocol )
    {
    TUint streamId( GenerateStreamId() );

    CNcmStream* item = CNcmStream::NewL(
        iSessionId, streamId, aQos, aProtocol );
        
    CleanupStack::PushL( item );
    iStreams.AppendL( item ); 
    CleanupStack::Pop( item );
    
    return item->StreamId();
    }

// ---------------------------------------------------------------------------
// CNcmSession::RemoveStreamL
// ---------------------------------------------------------------------------
//
void CNcmSession::RemoveStreamL( TUint aStreamId )
    {    
    TUint streamIndex( StreamIndexL( aStreamId ) );
    
    delete iStreams[streamIndex];
    iStreams.Remove( streamIndex );
    }
    

// ---------------------------------------------------------------------------
// CNcmSession::ConnectionId
// ---------------------------------------------------------------------------
//
TUint32 CNcmSession::IapId() const
    { 
    return iIapId;
    }
    
// ---------------------------------------------------------------------------
// CNcmSession::ConnectionId
// ---------------------------------------------------------------------------
//
TUint CNcmSession::SessionId() const
    {
    return iSessionId;
    }


// ---------------------------------------------------------------------------
// From class CActive
//
// CNATFWMultiplexerSocketDestination::RunL
// ---------------------------------------------------------------------------
//
void CNcmSession::RunL()
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmSession::RunL - iStatus: ", iStatus.Int() )
                   
    if ( iStatus != KErrNone )
        {
        iObserver.Notify( iSessionId, 0,
            MNcmConnectionMultiplexerObserver::ESessionCreated,
            iStatus.Int() );
            
        //iSessionObserver.SessionCreationFailed( iSessionId );
        }
    else
        {
/*        
#ifndef _DEBUG_EUNIT
        iIcmpReceiver = CNATFWMultiplexerIcmpReceiver::NewL( iSessionId,
                                                             iObserver,
                                                             iSocketServ );
#endif
        CNATFWLocalAddressResolver* localAddressResolver(
            CNATFWLocalAddressResolver::NewLC( iSocketServ ) );
            
        localAddressResolver->GetLocalAddrL( iLocalAddr, iIapId );
        CleanupStack::PopAndDestroy( localAddressResolver );
*/                
        iObserver.Notify( iSessionId, 0,
            MNcmConnectionMultiplexerObserver::ESessionCreated,
            iStatus.Int() );
        }
    }

// ---------------------------------------------------------------------------
// From class CActive
//
// DoCancel
// ---------------------------------------------------------------------------
//
void CNcmSession::DoCancel()
    {    
    iConnection.Close();
    // RConnection doesn't cancel an outstanding request at Close()
    // so we'll have to it "manually" here
    if( iStatus.Int() == KRequestPending )
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrCancel );
        }
    }


// -----------------------------------------------------------------------------
// From class CActive
//
// CNcmSession::RunError
// -----------------------------------------------------------------------------
//
TInt CNcmSession::RunError( TInt aError )
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmSession::RunError aError: ", aError )
  
    iObserver.Notify( iSessionId, 0,
        MNcmConnectionMultiplexerObserver::ESessionCreated,
        aError );
        
    iSessionObserver.SessionCreationFailed( iSessionId );

    aError = aError;
    return KErrNone;
    }
    
        
// ---------------------------------------------------------------------------
// CNATFWSession::StreamByIdL
// ---------------------------------------------------------------------------
//
CNcmStream* CNcmSession::StreamByIdL(
    TUint aStreamId )
    {
    TInt ind( iStreams.Count() );
    
    while ( ind-- )
        {
        if ( iStreams[ind]->StreamId() == aStreamId )
            {
            return iStreams[ind];
            }
        }
    
    return NULL;
    }
    
// ---------------------------------------------------------------------------
// CNcmSession::StreamIndexL
// ---------------------------------------------------------------------------
//
TUint CNcmSession::StreamIndexL( TUint aStreamId )
    {   
    TInt streamCount( iStreams.Count() );
    
    for ( TInt i = 0; i < streamCount; i++ )
        {
        if ( iStreams[i]->StreamId() == aStreamId )
            {
            return i;
            }
        }
    User::Leave( KErrNotFound );
    return KErrNone; // for warning removal on compile
    }
    
// ---------------------------------------------------------------------------
// CNcmSession::SocketServer
// ---------------------------------------------------------------------------
//
RSocketServ& CNcmSession::SocketServer()
    {
    return iSocketServ;
    }


// ---------------------------------------------------------------------------
// CNcmSession::Connection
// ---------------------------------------------------------------------------
//
RConnection& CNcmSession::Connection()
    {  
    return iConnection;
    }
    
// -----------------------------------------------------------------------------
// CNcmSession::GenerateStreamId
// -----------------------------------------------------------------------------
//
TUint CNcmSession::GenerateStreamId() const
    {
    return ( static_cast<TUint>( Math::Random() ) );
    }
   

// ---------------------------------------------------------------------------
// CNcmSession::PortStore
// ---------------------------------------------------------------------------
//
CNcmPortStore& CNcmSession::PortStore() const
    {
    __CONNECTIONMULTIPLEXER( "CNcmSession::PortStore" )

    return *iPortStore;
    }
// ---------------------------------------------------------------------------
// CNcmSession::LocalIPv4Address
// ---------------------------------------------------------------------------
//
TInetAddr& CNcmSession::LocalIPv4Address()
    {
    return iIpv4Addr;
    }

// ---------------------------------------------------------------------------
// CNcmSession::LocalIPv6Address
// ---------------------------------------------------------------------------
//
TInetAddr& CNcmSession::LocalIPv6Address()
    {
    return iIpv6Addr;
    }


// ---------------------------------------------------------------------------
// CNcmSession::IcmpError
// ---------------------------------------------------------------------------
//
void CNcmSession::IcmpError( const TInetAddr& /*aAddress*/, TInetAddr& /*aLocalAddress*/,
        TInetAddr& /*aRemoteAddress*/ )
{
}
