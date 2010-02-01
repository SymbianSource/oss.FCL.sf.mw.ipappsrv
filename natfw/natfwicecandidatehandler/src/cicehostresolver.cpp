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
* Description:    Resolves host candidates for the streams.
*
*/




#include "mncmconnectionmultiplexer.h"
#include "natfwcandidate.h"
#include "mnatfwpluginobserver.h"
#include "cicehostresolver.h"
#include "ciceconnection.h"
#include "cicecallbackexecuter.h"
#include "icecandidatehandlerdefs.h"
#include "icecandidatehandlerlogs.h"


// ======== MEMBER FUNCTIONS ========

CIceHostResolver::CIceHostResolver()
    {

    }


void CIceHostResolver::ConstructL( TAny* aInitParams )
    {
    __ICEDP( "CIceHostResolver::ConstructL" )
    __ASSERT_ALWAYS( aInitParams, User::Leave( KErrArgument ) );
    
    iPluginId = KNatPluginIdNokiaHost().AllocL();
    
    CNATFWPluginApi::TNATFWPluginInitParams* initParams = 
        static_cast<CNATFWPluginApi::TNATFWPluginInitParams*>
        ( aInitParams );
    
    iPluginObserver = &(initParams->iObserver);
    iMultiplexer = &initParams->iMultiplexer;
    iCallBackExecuter 
        = CIceCallBackExecuter::NewL( CActive::EPriorityStandard );
    }


CIceHostResolver* CIceHostResolver::NewL( TAny* aInitParams )
    {
    CIceHostResolver* self = new( ELeave ) CIceHostResolver();
    CleanupStack::PushL( self );
    self->ConstructL( aInitParams );
    CleanupStack::Pop( self );
    return self;
    }


CIceHostResolver::~CIceHostResolver()
    {
    __ICEDP( "CIceHostResolver::~CIceHostResolver" )
    
    delete iPluginId;
    iPluginObserver = NULL;
    iMultiplexer = NULL;
    iHostCandidates.ResetAndDestroy();
    delete iCallBackExecuter;
    
    iConnections.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CIceHostResolver::HandleCallBack
// ---------------------------------------------------------------------------
//
void CIceHostResolver::HandleCallBack( CBase& aObject, TInt aCallBackType )
    {
    __ICEDP( "CIceHostResolver::HandleCallBack" )
    
    CIceHostResolver& plugin( static_cast<CIceHostResolver&>( aObject ) );
    plugin.DoHandleCallBack( aCallBackType );
    }


// ---------------------------------------------------------------------------
// CIceHostResolver::LocalAddress
// ---------------------------------------------------------------------------
//
TInetAddr CIceHostResolver::LocalAddress( 
        TUint aStreamId, TUint aAddrFamily ) const
    {
    __ICEDP( "CIceHostResolver::LocalAddress" )

    TInt count( iConnections.Count() );
    ASSERT( count );

    for ( TInt i( 0 ); i < count; i++ )
        {
        if ( iConnections[i]->StreamId() == aStreamId &&
             iConnections[i]->LocalAddress().Family() == aAddrFamily )
            {
            return iConnections[i]->LocalAddress();
            }
        }
    
    return TInetAddr();
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//    
void CIceHostResolver::ConnectServerL( const RSocketServ& /*aSocketServ*/,
        const TName& /*aConnectionName*/ )
    {
    __ICEDP( "CIceHostResolver::ConnectServerL" )
    
    TIceCallBack callback(
        HandleCallBack, *this, ECallBackServerConnected, 0 );
    iCallBackExecuter->AddCallBackL( callback );
    }
    

// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//
void CIceHostResolver::FetchCandidateL( TUint aStreamId, TUint /*aRtoValue*/,
        TUint aAddrFamily )
    {
    __ICEDP( "CIceHostResolver::FetchCandidateL" )
    
    CIceConnection* connection = CIceConnection::NewLC( 
        aStreamId, aAddrFamily, *iMultiplexer, *this );
    iConnections.AppendL( connection );
    CleanupStack::Pop( connection );
    
    // Host connections are used to receive checks from peer when local agent
    // has not yet started ICE processing.
    connection->SetReceivingStateL( EStreamingStateActive, 
        CIceConnection::ENotifyStateDisabled );
    
    // Unspecified destination allows STUN server response sending to
    // arbitrary address.
    TInetAddr unspecDestination;
    connection->SetSendingStateL( EStreamingStateActive, 
        CIceConnection::ENotifyStateDisabled, unspecDestination );
    
    // setup host candidate
    const TInetAddr& addr( connection->LocalAddress() );
    CNATFWCandidate* hostCandidate = CNATFWCandidate::NewLC();
    hostCandidate->SetStreamId( aStreamId );
    hostCandidate->SetType( CNATFWCandidate::EHost );
    hostCandidate->SetTransportAddrL( addr );
    hostCandidate->SetBase( addr );
    TUint protocol = connection->TransportProtocol();
    hostCandidate->SetTransportProtocol( protocol );
    
    iHostCandidates.AppendL( hostCandidate );
    CleanupStack::Pop( hostCandidate );
    connection->BindWithCandidateL( *hostCandidate );
    
    TIceCallBack callback( HandleCallBack, *this, ECallBackCandidateFound, 0 );
    iCallBackExecuter->AddCallBackL( callback );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//
void CIceHostResolver::SetReceivingStateL(
        const CNATFWCandidate& aLocalCandidate, TNATFWStreamingState aState )
    {
    __ICEDP( "CIceHostResolver::SetReceivingStateL" )
    __ASSERT_ALWAYS( IsOwnedCandidate( aLocalCandidate ), 
        User::Leave( KErrNotFound ) );
    
    // find connection bound to the candidate
    CIceConnection* connection = FindConnection( aLocalCandidate );
    if ( NULL == connection &&
         CNATFWCandidate::EPeerReflexive == aLocalCandidate.Type() )
        {
        // Bind new peer-reflexive candidate with the corresponding host
        // connection. Peer-reflexive candidate has same base than host
        // candidate.
        connection = FindConnection( aLocalCandidate.Base() );
        if ( connection )
            {
            connection->BindWithCandidateL( aLocalCandidate );
            }
        }
    
    __ASSERT_ALWAYS( NULL != connection, User::Leave( KErrNotFound ) );
    connection->SetReceivingStateL( aState, 
        CIceConnection::ENotifyStateEnabled );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//
void CIceHostResolver::SetSendingStateL(
        const CNATFWCandidate& aLocalCandidate,
        TNATFWStreamingState aState, const TInetAddr& aDestAddr )
    {
    __ICEDP( "CIceHostResolver::SetSendingStateL" )
    __ASSERT_ALWAYS( IsOwnedCandidate( aLocalCandidate ), 
        User::Leave( KErrNotFound ) );

    // find connection bound to the candidate
    CIceConnection* connection = FindConnection( aLocalCandidate );
    if ( NULL == connection &&
         CNATFWCandidate::EPeerReflexive == aLocalCandidate.Type() )
        {
        // Bind new peer-reflexive candidate with the corresponding host
        // connection. Peer-reflexive candidate has same base than host
        // candidate.
        connection = FindConnection( aLocalCandidate.Base() );
        if ( connection )
            {
            connection->BindWithCandidateL( aLocalCandidate );
            }
        }
    
    __ASSERT_ALWAYS( NULL != connection, User::Leave( KErrNotFound ) );
    connection->SetSendingStateL( aState, 
        CIceConnection::ENotifyStateEnabled, aDestAddr );
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//
void CIceHostResolver::GetConnectionIdL(
        const CNATFWCandidate& aLocalCandidate,
        TUint& aConnectionId )
    {
    __ICEDP( "CIceHostResolver::GetConnectionIdL" )
    __ASSERT_ALWAYS( IsOwnedCandidate( aLocalCandidate ), 
        User::Leave( KErrNotFound ) );
    
    // find connection bound to the candidate
    CIceConnection* connection = FindConnection( aLocalCandidate );
    if ( NULL == connection &&
         CNATFWCandidate::EPeerReflexive == aLocalCandidate.Type() )
        {
        // Bind new peer-reflexive candidate with the corresponding host
        // connection. Peer-reflexive candidate has same base than host
        // candidate.
        connection = FindConnection( aLocalCandidate.Base() );
        if ( connection )
            {
            connection->BindWithCandidateL( aLocalCandidate );
            }
        }
    
    __ASSERT_ALWAYS( NULL != connection, User::Leave( KErrNotFound ) );
    aConnectionId = connection->ConnectionId();
    }


// ---------------------------------------------------------------------------
// From class CNATFWPluginApi.
// ---------------------------------------------------------------------------
//
const TDesC8& CIceHostResolver::PluginIdentifier() const
    {
    return *iPluginId;
    }


// ---------------------------------------------------------------------------
// CIceHostResolver::ConnectionNotify
// Passes media connection related events to the client.
// ---------------------------------------------------------------------------
//
void CIceHostResolver::ConnectionNotify( CIceConnection& aConnection,
        MIceConnectionObserver::TNotifyType aType, TInt aError )
    {
    __ICEDP( "CIceHostResolver::ConnectionNotify" )
    
    TInt connectionIndex( iConnections.Find( &aConnection ) );
    ASSERT( KErrNotFound != connectionIndex );
    if ( KErrNotFound != connectionIndex )
        {
        switch ( aType )
            {
            case MIceConnectionObserver::ENotifyConnectionRemoved:
                delete iConnections[connectionIndex];
                iConnections.Remove( connectionIndex );
                break;
           
            case MIceConnectionObserver::ENotifyConnectionError:
                iPluginObserver->Error( *this, aConnection.StreamId(),
                    aError );
                break;
            
            case MIceConnectionObserver::ENotifyRecvActivated:
                iPluginObserver->Notify( *this, aConnection.StreamId(), 
                    MNATFWPluginObserver::EReceivingActivated, aError );
                break;
            
            case MIceConnectionObserver::ENotifyRecvDeactivated:
                iPluginObserver->Notify( *this, aConnection.StreamId(), 
                    MNATFWPluginObserver::EReceivingDeactivated, aError );
                break;
            
            case MIceConnectionObserver::ENotifySendActivated:
                iPluginObserver->Notify( *this, aConnection.StreamId(), 
                    MNATFWPluginObserver::ESendingActivated, aError );
                break;
            
            case MIceConnectionObserver::ENotifySendDeactivated:
                iPluginObserver->Notify( *this, aConnection.StreamId(), 
                    MNATFWPluginObserver::ESendingDeactivated, aError );
                break;
            
            default:
                ASSERT( EFalse );
            }       
        }
    }


// ---------------------------------------------------------------------------
// CIceHostResolver::DoHandleCallBack
// ---------------------------------------------------------------------------
//
void CIceHostResolver::DoHandleCallBack( TInt aCallBackType )
    {
    __ICEDP( "CIceHostResolver::DoHandleCallBack" )
    
    if ( ECallBackServerConnected == aCallBackType )
        {
        iPluginObserver->Notify( *this, 0,
            MNATFWPluginObserver::EServerConnected, KErrNone );
        }
    else if ( ECallBackCandidateFound  == aCallBackType )
        {
        ASSERT( iHostCandidates.Count() );
        TUint streamId( iHostCandidates[0]->StreamId() );
        iPluginObserver->NewLocalCandidateFound( *this, iHostCandidates[0] );
        iHostCandidates.Remove( 0 );
        iPluginObserver->Notify( *this, streamId,
            MNATFWPluginObserver::EFetchingCompleted, KErrNone );
        }
    else
        {
        __ICEDP( "CIceHostResolver::DoHandleCallBack, ELSE" )
        ASSERT( EFalse );
        }
    }


// ---------------------------------------------------------------------------
// CIceHostResolver::FindConnection
// ---------------------------------------------------------------------------
//
CIceConnection* CIceHostResolver::FindConnection( 
        const CNATFWCandidate& aLocalCandidate )
    {
    CIceConnection* foundConnection( NULL );
    TInt index( iConnections.Count() - 1 );
    while ( NULL == foundConnection && 0 <= index )
        {
        if ( iConnections[index]->IsBoundToCandidate( aLocalCandidate ) )
            {
            foundConnection = iConnections[index];
            }
        
        index--;
        }
    
    return foundConnection;
    }


// ---------------------------------------------------------------------------
// CIceHostResolver::FindConnection
// ---------------------------------------------------------------------------
//
CIceConnection* CIceHostResolver::FindConnection( 
        const TInetAddr& aLocalAddr )
    {
    CIceConnection* foundConnection( NULL );
    TInt index( iConnections.Count() - 1 );
    while ( NULL == foundConnection && 0 <= index )
        {
        if ( TIceUtils::MatchAddresses( 
                iConnections[index]->LocalAddress(), aLocalAddr ) )
            {
            foundConnection = iConnections[index];
            }
        
        index--;
        }
    
    return foundConnection;
    }


// ---------------------------------------------------------------------------
// CIceHostResolver::IsOwnedCandidate
// ---------------------------------------------------------------------------
//
TBool CIceHostResolver::IsOwnedCandidate( 
        const CNATFWCandidate& aLocalCandidate ) const
    {
    return ( CNATFWCandidate::EHost == aLocalCandidate.Type() || 
             CNATFWCandidate::EPeerReflexive == aLocalCandidate.Type() );
    }
