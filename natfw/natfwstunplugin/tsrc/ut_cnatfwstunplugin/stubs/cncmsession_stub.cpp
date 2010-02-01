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
* Description:    Connection multiplexer session abstraction.
*
*/




#include <commdbconnpref.h>
#include <e32math.h> 
#include <es_sock_partner.h>

#include "cncmsession.h"
#include "cncmstream.h"
#include "cncmconnection.h"
#include "cncmconnectionobserverhandler.h"
#include "cncmlocaladdressresolver.h"
#include "ncmconnectionmultiplexerlogs.h"
//#include "cncmportstore.h"
#include "mncmsessionobserver.h"
#include "mncmconnectionmultiplexerobserver.h"

const TUint KESockMessageSlots = 16;

const TUint32 KLocalAddress = INET_ADDR( 10, 168, 0, 100 );
const TUint KLocalPort = 5000;

// ---------------------------------------------------------------------------
// CNcmSessionSession::CNcmSession
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
void CNcmSession::ConstructL( TUint /*aPortRangeStart*/,
                              TUint /*aPortRangeStop*/ )
    {
    iIpv4Addr.SetAddress( KLocalAddress );
    iIpv4Addr.SetPort( KLocalPort );
    /*    
    iPortStore = CNcmPortStore::NewL( aPortRangeStart, aPortRangeStop );
    */
    User::LeaveIfError( iSocketServ.Connect( KESockMessageSlots ) );
   
    TCommDbConnPref pref;
    pref.SetDialogPreference( ECommDbDialogPrefDoNotPrompt );
    pref.SetIapId( iIapId );
    
    User::LeaveIfError( iConnection.Open( iSocketServ ) );
    
    TSecurityPolicy policy( ECapabilityNetworkServices );
    TSecurityPolicyBuf securityBuf(policy);
    User::LeaveIfError( iConnection.Control( 
        KCOLConnection, KCoEnableCloneOpen, securityBuf ) );
    
    iConnection.Start( pref/*, iStatus*/ );
    // SetActive();                              
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
    //delete iPortStore;
    
    TInt ind( iStreams.Count() );
    
    while ( ind-- )
        {
        delete iStreams[ind];
        iStreams.Remove( ind );
        }   
    
//    delete iIcmpReceiver;
    
    iConnection.Close();    
//    Cancel();

    iSocketServ.Close();
    
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
    
/*
// ---------------------------------------------------------------------------
// CNcmSession::ConnectionId
// ---------------------------------------------------------------------------
//
TUint32 CNcmSession::IapId() const
    {
    return iIapId;
    }
*/    
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
// CNcmSocketDestination::RunL
// ---------------------------------------------------------------------------
//
void CNcmSession::RunL()
    {
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmSession::RunL - iStatus: ", iStatus.Int() )
    /*               
    if ( iStatus != KErrNone )
        {
        iObserver.Notify( iSessionId, 0,
            MNcmConnectionMultiplexerObserver::ESessionCreated,
            iStatus.Int() );
            
        iSessionObserver.SessionCreationFailed( iSessionId );
        }
    else
        {
        iInitialized = ETrue;
        
#ifndef _DEBUG_EUNIT
        iIcmpReceiver = CNcmIcmpReceiver::NewL( iSessionId,
                                                iSessionObserver,
                                                iSocketServ,
                                                iConnection );
#endif

        CNcmLocalAddressResolver* localAddressResolver(
            CNcmLocalAddressResolver::NewLC( iSocketServ ) );
            
        localAddressResolver->GetLocalAddrL( iLocalAddr, iIapId );
        CleanupStack::PopAndDestroy( localAddressResolver );
                         
        iObserver.Notify( iSessionId, 0,
            MNcmConnectionMultiplexerObserver::ESessionCreated,
            iStatus.Int() );
        }
     */
    }

// ---------------------------------------------------------------------------
// From class CActive
//
// DoCancel
// ---------------------------------------------------------------------------
//
void CNcmSession::DoCancel()
    {     
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
TInt CNcmSession::RunError( TInt /*aError*/ )
    {
    /*
    __CONNECTIONMULTIPLEXER_INT1(
        "CNcmSession::RunError aError: ", aError )
  
    iObserver.Notify( iSessionId, 0,
        MNcmConnectionMultiplexerObserver::ESessionCreated,
        aError );
        
    iSessionObserver.SessionCreationFailed( iSessionId );

    aError = aError;
    */
    return KErrNone;
    }
    
        
// ---------------------------------------------------------------------------
// CNcmSession::StreamByIdL
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

/*
// ---------------------------------------------------------------------------
// CNcmSession::Connection
// ---------------------------------------------------------------------------
//
RConnection& CNcmSession::Connection()
    {  
    return iConnection;
    }
*/

// ---------------------------------------------------------------------------
// CNcmSession::GetConnectionName
// ---------------------------------------------------------------------------
//
TInt CNcmSession::GetConnectionName( TName& aConnectionName )
    {  
    iConnection.Name( aConnectionName );
    return 0;
    }

// -----------------------------------------------------------------------------
// CNcmSession::GenerateStreamId
// -----------------------------------------------------------------------------
//
TUint CNcmSession::GenerateStreamId() const
    {
    //return ( static_cast<TUint>( Math::Random() ) );
    return 222;
    }

// -----------------------------------------------------------------------------
// CNcmSession::LocalIPv4Address
// -----------------------------------------------------------------------------
//
TInetAddr& CNcmSession::LocalIPv4Address()
    {
    return iIpv4Addr;
    }

// ---------------------------------------------------------------------------
// From class MNcmIcmpObserver
//
// Called as a result for ICMP error received 
// ---------------------------------------------------------------------------
//        
void CNcmSession::IcmpError( const TInetAddr& aAddress,
    TInetAddr& aLocalAddress, TInetAddr& aRemoteAddress )
    {
    //TRAP_IGNORE( this->HandleIcmpErrorL( aAddress, aLocalAddress,
    //aRemoteAddress ) )
    }

// -----------------------------------------------------------------------------
// CNcmSession::LocalIPv6Address
// -----------------------------------------------------------------------------
//
TInetAddr& CNcmSession::LocalIPv6Address()
    {
    return iIpv6Addr;
    }

/*
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
// CNcmSession::HandleIcmpErrorL
// ---------------------------------------------------------------------------
void CNcmSession::HandleIcmpErrorL( const TInetAddr& aAddress )
    {  
    __CONNECTIONMULTIPLEXER(
        "CNcmSession::HandleIcmpErrorL" )
    
    TInt ind( iStreams.Count() );
    TUint mediaConnectionId( 0 );
    
    while ( ind-- )
        { 
        mediaConnectionId = iStreams[ind]->DefaultConnectionId();

        if ( iStreams[ind]->ConnectionL( mediaConnectionId )->Destination()->
            CheckSenderValidityToSending( aAddress ) &&
            !iStreams[ind]->IcmpErrorReceived() )
            {
            iStreams[ind]->SetIcmpErrorReceived( ETrue ); 
            
            iObserver.Notify( iSessionId, iStreams[ind]->StreamId(),
                MNcmConnectionMultiplexerObserver::ESessionCreated,
                KErrCommsLineFail );
            return;
            }
        }
    
    User::Leave( KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CNcmSession::Initialized
// ---------------------------------------------------------------------------
//
TBool CNcmSession::Initialized() const
    {
    return iInitialized;
    }
*/