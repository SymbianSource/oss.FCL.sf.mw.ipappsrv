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
* Description:    
*
*/




#include "natfwunsafserverresolver.h"
#include "natfwunsafserverresolverobserver.h"
#include "cnatfwunsaficmpreceiver.h"
#include "natfwunsaflog.h"
#include "casynccallback.h"
#include "stunassert.h"
#include "cstunclientimplementation.h"
#include "cstunclientresolvingtcp.h"
#include "cstunclientresolvingtls.h"
#include "cstunclientresolvingudp.h"
#include "cstunclientgetsharedsecret.h"
#include "cstunclientready.h"
#include "cstunclientrenewsharedsecret.h"
#include "natfwstunbinding.h"
#include "cstunbindingimplementation.h"
#include "ctransactionidgenerator.h"
#include "cstunsharedsecret.h"
#include "cstuncredentials.h"
#include "cstuncredentials.h"

_LIT8( KUdp, "udp" );
_LIT8( KTcp, "tcp" );
_LIT8( KTls, "tls" );
const TUint KTlsPort = 443;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::NewL
// ---------------------------------------------------------------------------
//
CSTUNClientImplementation*
CSTUNClientImplementation::NewL( CSTUNClient& aClient,
                                 TInt aRetransmitInterval,
                                 const TDesC8& aServerAddress,
                                 TUint aServerPort,
                                 const TDesC8& aServiceName,
                                 RSocketServ& aSocketServ,
                                 RConnection& aConnection,
                                 CDeltaTimer& aTimer,
                                 MSTUNClientObserver& aObserver,
                                 TBool aObtainSharedSecret,
                                 TBool aFailIfNoSRVRecordsFound,
                                 TBool aIcmpReceiverUsed,
                                 MNcmConnectionMultiplexer* aMultiplexer,
                                 TTransportProtocol aTransportProtocol )
    {
    CSTUNClientImplementation* self =
        new ( ELeave ) CSTUNClientImplementation( aClient,
                                                  aRetransmitInterval,
                                                  aServerPort,
                                                  aSocketServ,
                                                  aConnection,
                                                  aTimer,
                                                  aObserver,
                                                  aObtainSharedSecret,
                                                  aMultiplexer,
                                                  aTransportProtocol );
    CleanupStack::PushL( self );
    self->ConstructL( aServerAddress, aServiceName, 
                      aFailIfNoSRVRecordsFound, aIcmpReceiverUsed );
    CleanupStack::Pop( self );
    return self;    
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::NewL - overloaded for ICE conn check usage
// ---------------------------------------------------------------------------
//
CSTUNClientImplementation*
CSTUNClientImplementation::NewL( CSTUNClient& aClient,
                                 TInt aRetransmitInterval,
                                 CDeltaTimer& aTimer,
                                 MSTUNClientObserver& aObserver,
                                 MNcmConnectionMultiplexer* aMultiplexer,
                                 TTransportProtocol aTransportProtocol )
    {
    CSTUNClientImplementation* self =
        new ( ELeave ) CSTUNClientImplementation( aClient,
                                                  aRetransmitInterval,
                                                  aTimer,
                                                  aObserver,
                                                  aMultiplexer,
                                                  aTransportProtocol );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;    
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::CSTUNClientImplementation
// ---------------------------------------------------------------------------
//
CSTUNClientImplementation::CSTUNClientImplementation(
    CSTUNClient& aClient,
    TInt aRetransmitInterval,
    TUint aServerPort,
    RSocketServ& aSocketServ,
    RConnection& aConnection,
    CDeltaTimer& aTimer,
    MSTUNClientObserver& aObserver,
    TBool aObtainSharedSecret,
    MNcmConnectionMultiplexer* aMultiplexer,
    TTransportProtocol aTransportProtocol ) :
    CSTUNTimerUser( aTimer ),
    iClient( aClient ),
    iRetransmitInterval( aRetransmitInterval ),
    iServerPort( aServerPort ? aServerPort : EDefaultSTUNPort ),
    iSocketServer( aSocketServ ),
    iConnection( aConnection ),    
    iObserver( aObserver ),
    iObtainSharedSecret( aObtainSharedSecret ),
    iMultiplexer( aMultiplexer ),
    iTransportProtocol( aTransportProtocol )
#ifdef TEST_EUNIT
    , iTcpAddresses( 1 ),
    iUdpAddresses( 1 )
#endif
    {    
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::CSTUNClientImplementation - overloaded
// ---------------------------------------------------------------------------
//
CSTUNClientImplementation::CSTUNClientImplementation(
    CSTUNClient& aClient,
    TInt aRetransmitInterval,
    CDeltaTimer& aTimer,
    MSTUNClientObserver& aObserver,
    MNcmConnectionMultiplexer* aMultiplexer,
    TTransportProtocol aTransportProtocol ) :
    CSTUNTimerUser( aTimer ),
    iClient( aClient ),
    iRetransmitInterval( aRetransmitInterval ),
    iSocketServer( *( RSocketServ* )0x1 ),
    iConnection( *( RConnection* )0x1 ),
    iObserver( aObserver ),
    iMultiplexer( aMultiplexer ),
    iTransportProtocol( aTransportProtocol )
    {
    }
        
// ---------------------------------------------------------------------------
// CSTUNClientImplementation::CSTUNClientImplementation
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientImplementation::CSTUNClientImplementation() :
    CSTUNTimerUser( *( CDeltaTimer* )0x1 ),        
    iClient( *( CSTUNClient* )0x1 ),
    iSocketServer( *( RSocketServ* )0x1 ),
    iConnection( *( RConnection* )0x1 ),
    iObserver( *( MSTUNClientObserver* )0x1 )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::CSTUNClientImplementation
// Dummy implementation. Copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNClientImplementation::CSTUNClientImplementation(
    const CSTUNClientImplementation& aImplementation ) :
    CSTUNTimerUser( *( CDeltaTimer* )0x1 ),
    iClient( aImplementation.iClient ),
    iSocketServer( aImplementation.iSocketServer ),
    iConnection( aImplementation.iConnection ),
    iObserver( aImplementation.iObserver )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::ConstructL( const TDesC8& aServerAddress,
                                            const TDesC8& aServiceName,
                                            TBool aFailIfNoSRVRecordsFound,
                                            TBool aIcmpReceiverUsed )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::ConstructL IN" )
    __ASSERT_ALWAYS( aServerAddress.Length() > 0, 
        User::Leave( KErrArgument ) );
    
    __STUNTURNCLIENT_STR8( "CSTUNClientImplementation stunserver=",
        aServerAddress )
    iServerAddress = aServerAddress.AllocL();
    
    __STUNTURNCLIENT_STR8( "CSTUNClientImplementation servicename=",
        aServiceName )
    
    iServiceName = aServiceName.AllocL();
    iAsyncCallback = CAsyncCallback::NewL( iObserver );
        
    iResolver = CNATFWUNSAFServerResolver::NewL( iSocketServer,
                                                 iConnection,
                                                 *this,
                                                 aFailIfNoSRVRecordsFound );
                                                 
    iTransactionIDGenerator = CTransactionIDGenerator::NewL();
    
    if ( aIcmpReceiverUsed )
        {
        iIcmpReceiver = CIcmpReceiver::NewL( *this, iSocketServer );
        }
    
    iRenewSharedSecret = new ( ELeave ) CSTUNClientRenewSharedSecret;
    iReady = new ( ELeave ) CSTUNClientReady( *iRenewSharedSecret );
    iGetSharedSecret = new ( ELeave ) CSTUNClientGetSharedSecret( *iReady );
    iResolvingUDP = new ( ELeave ) CSTUNClientResolvingUDP( *iGetSharedSecret,
                                                            *iReady );
    if ( iObtainSharedSecret )
        {
        iResolvingTLS = new ( ELeave ) 
            CSTUNClientResolvingTLS( *iResolvingUDP );
        iResolvingTCP = new ( ELeave ) 
            CSTUNClientResolvingTCP( *iResolvingTLS, *iResolvingUDP );
        }
    else
        {
        iResolvingTCP = new ( ELeave )
            CSTUNClientResolvingTCP( *iResolvingUDP );
        }
    
    iRenewSharedSecret->SetNeighbourStates( *iReady );

    if ( iServiceName->Compare( KStunRelay ) == 0 && iObtainSharedSecret )
        {
        ChangeState( iResolvingTLS );
        ResolveAddressL( KTls, iTcpAddresses, KTlsPort, KStunRelay );
        }
    
    else if ( iObtainSharedSecret )
        {
        ChangeState( iResolvingTCP );
        ResolveAddressL( KTcp, iTcpAddresses, iServerPort, *iServiceName );
        }
    
    else
        {
        ChangeState( iResolvingUDP );
        ResolveUdpL();
        }
        
    __STUNTURNCLIENT( "CSTUNClientImplementation::ConstructL OUT" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ConstructL - overloaded for ICE conn check usage
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::ConstructL( )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::ConstructL for ICE" )

    TInetAddr defaultAddr;
    defaultAddr.SetAddress( KInetAddrAll );
    
    // Make state machine work with hack
    iUdpAddresses.AppendL( defaultAddr );
    iObtainSharedSecret = EFalse;
    iAsyncCallback = CAsyncCallback::NewL( iObserver );
        
    iTransactionIDGenerator = CTransactionIDGenerator::NewL();
    
    iRenewSharedSecret = new ( ELeave ) CSTUNClientRenewSharedSecret;
    iReady = new ( ELeave ) CSTUNClientReady( *iRenewSharedSecret );
    iGetSharedSecret = new ( ELeave ) CSTUNClientGetSharedSecret( *iReady );
    iResolvingUDP = new ( ELeave ) CSTUNClientResolvingUDP( *iGetSharedSecret,
                                                            *iReady );
    iResolvingTCP = new ( ELeave ) CSTUNClientResolvingTCP( *iResolvingUDP );
    
    iRenewSharedSecret->SetNeighbourStates( *iReady );
    ChangeState( iReady );
    // do not send notify, act as syncronous
    }

    
// ---------------------------------------------------------------------------
// CSTUNClientImplementation::~CSTUNClientImplementation
// ---------------------------------------------------------------------------
//
CSTUNClientImplementation::~CSTUNClientImplementation()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::~CSTUNClientImplementation" )
    TInt count( iBindings.Count() );
    if ( count > 0 )
        {
        for ( TInt i = 0; i < count; ++i ) // check that ++i
            {        
            iBindings[i]->Implementation().DetachClient();
            }
        }
    iBindings.Close();    

    FreeResolverResources();

    delete iAsyncCallback;
    delete iResolvingTLS;
    delete iResolvingTCP;
    delete iResolvingUDP;
    delete iGetSharedSecret;
    delete iReady;
    delete iRenewSharedSecret;

    delete iTransactionIDGenerator;
    delete iSharedSecret;
    delete iCredentials;
    delete iIcmpReceiver;
    
    delete iTimer;
    delete iServiceName;

    iTcpAddresses.Close();
    iUdpAddresses.Close();
    delete iServerAddress;
    iMultiplexer = NULL;
    iState = NULL;
    
    __STUNTURNCLIENT( "CSTUNClientImplementation::\
    ~CSTUNClientImplementation end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::CompletedL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::CompletedL()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::CompletedL" )
    __TEST_INVARIANT;

    if ( iState )
        {
        iState->ResolvingCompletedL( *this, iObtainSharedSecret );
        }
        
    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::CompletedL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ErrorOccured
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::ErrorOccured( TInt aError )
    {
    __STUNTURNCLIENT_INT1( "CSTUNClientImplementation::ErrorOccured: "
        , aError )
    __TEST_INVARIANT;

    if ( iState )
        {
        if ( aError != KErrNoMemory )
            {
            aError = KErrNATFWDnsFailure;
            }
        iState->ResolvingFailed( *this, aError );
        }
        
    __TEST_INVARIANT;    
    __STUNTURNCLIENT( "CSTUNClientImplementation::ErrorOccured end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::IcmpError
// Don't compare aAddress to iUdpAddresses, as some binding may still have an
// address that has been removed from iUdpAddresses if another binding called
// ObtainServerAddress.
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::IcmpError( const TInetAddr& aAddress )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::IcmpError" )
    __TEST_INVARIANT;

    for ( TInt i = 0; i < iBindings.Count(); ++i )
        {
        iBindings[i]->Implementation().IcmpError( aAddress );
        }
    RemoveAddress( aAddress );

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::IcmpError end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::SharedSecretObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::SharedSecretObtainedL()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::SharedSecretObtainedL" )
    __TEST_INVARIANT;

    if ( iState )
        {        
        iState->SharedSecretReceivedL( *this );
        }

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::SharedSecretObtainedL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::SharedSecretErrorL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::SharedSecretErrorL( TInt aError )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::SharedSecretErrorL" )
    __TEST_INVARIANT;
    
    if ( iState )
        {
        iState->SharedSecretErrorL( *this, aError );
        }

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::SharedSecretErrorL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::STUNClient
// ---------------------------------------------------------------------------
//
const CSTUNClient& CSTUNClientImplementation::STUNClient() const
    {
    __TEST_INVARIANT;

    return iClient;
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::TimerProvider
// ---------------------------------------------------------------------------
//
CDeltaTimer& CSTUNClientImplementation::TimerProvider()
    {
    __TEST_INVARIANT;

    return DeltaTimer();
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::RetransmitInterval
// ---------------------------------------------------------------------------
//
TInt CSTUNClientImplementation::RetransmitInterval() const
    {
    __TEST_INVARIANT;

    return iRetransmitInterval;
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::TransportProtocol
// ---------------------------------------------------------------------------
//
TTransportProtocol CSTUNClientImplementation::TransportProtocol() const
    {
    __TEST_INVARIANT;

    return iTransportProtocol;
    }
    
// ---------------------------------------------------------------------------
// CSTUNClientImplementation::AddressResolvedL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::AddressResolvedL(
    const CBinding& aBinding ) const
    {
    __TEST_INVARIANT;

    iAsyncCallback->MakeCallbackL( TSTUNCallbackInfo::EEventAddressResolvedL,
                                   &aBinding );
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ObtainSharedSecretL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::ObtainSharedSecretL( CBinding& aBinding )
    {
    __TEST_INVARIANT;
    __ASSERT_ALWAYS( iState != NULL, User::Leave( KErrServerTerminated ) );

    iState->ObtainSharedSecretL( *this, aBinding );

    __TEST_INVARIANT;
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::SharedSecretRejectedL
// ---------------------------------------------------------------------------
//
TBool
CSTUNClientImplementation::SharedSecretRejectedL( const CBinding& aBinding,
                                                  const TDesC8& aUsername,
                                                  const TDesC8& aPassword )
    {
    __TEST_INVARIANT;

    return iState && iState->SharedSecretRejectedL( *this,
                                                    aBinding,
                                                    aUsername,
                                                    aPassword );        
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ObtainServerAddress
// ---------------------------------------------------------------------------
//
TBool CSTUNClientImplementation::ObtainServerAddress( TInetAddr& aAddress )    
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::ObtainServerAddress" )
    __TEST_INVARIANT;
    #ifdef _DEBUG_EUNIT
        TInetAddr addr;
        addr.SetAddress( INET_ADDR( 10,32,194,251 ) );
        addr.SetFamily( KAfInet );
        iUdpAddresses.Append( addr );
    #endif

    if ( !aAddress.IsUnspecified() )
        {
        // Binding has tried aAddress and found it doesn't respond.
        RemoveAddress( aAddress );
        }

    if ( iUdpAddresses.Count() > 0 )
        {
        aAddress = iUdpAddresses[ 0 ];
        return ETrue;
        }

    __STUNTURNCLIENT( "CSTUNClientImplementation::ObtainServerAddress end" )
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ObtainTransactionIDL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::ObtainTransactionIDL(
    TNATFWUNSAFTransactionID& aTransactionID )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::ObtainTransactionIDL" )
    __TEST_INVARIANT;
    #ifdef _DEBUG_EUNIT
        aTransactionID.Append(TUint8(0));
        aTransactionID.Append(TUint8(1));
        aTransactionID.Append(TUint8(2));
        aTransactionID.Append(TUint8(3));
        aTransactionID.Append(TUint8(4));
        aTransactionID.Append(TUint8(5));
        aTransactionID.Append(TUint8(6));
        aTransactionID.Append(TUint8(7));
        aTransactionID.Append(TUint8(8));
        aTransactionID.Append(TUint8(9));
        aTransactionID.Append(TUint8(0xa));
        aTransactionID.Append(TUint8(0xb));
    #else
        iTransactionIDGenerator->GetIDL( this, sizeof( *this ), aTransactionID );
    #endif
    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::ObtainTransactionIDL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::AttachBindingL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::AttachBindingL( const CBinding& aBinding )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::AttachBindingL" )
    __TEST_INVARIANT;

    iBindings.AppendL( &aBinding );

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::AttachBindingL end" )
    }
                                     
// ---------------------------------------------------------------------------
// CSTUNClientImplementation::DetachBinding
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::DetachBinding( const CBinding& aBinding )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::DetachBinding" )
    __TEST_INVARIANT;

    for ( TInt i = 0; i < iBindings.Count(); ++i )
        {
        if ( iBindings[ i ] == &aBinding )
            {
            iAsyncCallback->CancelCallback( *iBindings[i] );
            iBindings.Remove( i );
            __TEST_INVARIANT;
            return;
            }
        }
    __STUNTURNCLIENT( "CSTUNClientImplementation::DetachBinding end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::BindingErrorL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::BindingErrorL( const CBinding& aBinding,
                                               TInt aError, TBool aIsFatal )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::BindingErrorL" )
    __TEST_INVARIANT;
    
    if ( aIsFatal )
        {
        iAsyncCallback->MakeCallbackL( 
            TSTUNCallbackInfo::EErrorOccurred, &aBinding, aError );
        }
    
    else
        {
        TInt count( iBindings.Count() );
        TInt index( KErrNotFound );
        
        for ( TInt i = 0; i < count; i++ )
            {
            if ( &aBinding == iBindings[i] )
                {
                index = i;
                }
            }
        
        switch ( aError )
            {
            case E401Unauthorized:
                if ( index >= 0 )
                    {
                    if ( iBindings[index]->RealmFromResponse() )
                        {
                        // Client should set long term credentials
                        iAsyncCallback->MakeCallbackL( 
                            TSTUNCallbackInfo::EErrorOccurred, 
                            &aBinding, aError );
                        }
                    else
                        {
                        RenewSharedSecretL();
                        }
                    }
            
            break;
            
            case E430StaleCredentials:
            break;
            
            case E436UnknownUsername:
                if ( iCredentials && iSharedSecret )
                    {
                    // FATAL ERROR
                    iAsyncCallback->MakeCallbackL( 
                        TSTUNCallbackInfo::EErrorOccurred, &aBinding, aError );
                    }
                else
                    {
                    RenewSharedSecretL();
                    }
            break;
            
            case E432MissingUsername:
            break;
            
            case E433UseTLS:
                if ( iCredentials || iSharedSecret )
                    {
                    // FATAL ERROR
                    iAsyncCallback->MakeCallbackL( 
                        TSTUNCallbackInfo::EErrorOccurred, &aBinding, aError );
                    }
                else
                    {
                    RenewSharedSecretL();
                    }
            break;
            
            case E434MissingRealm:
                // client should decide if try again with credentials
                iAsyncCallback->MakeCallbackL( 
                    TSTUNCallbackInfo::EErrorOccurred, &aBinding, aError );
            break;
            
            default:
                // FATAL ERROR
                iAsyncCallback->MakeCallbackL( 
                    TSTUNCallbackInfo::EErrorOccurred, 
                    &aBinding, KErrGeneral );
            break;
            }
        }
    
    __STUNTURNCLIENT( "CSTUNClientImplementation::BindingErrorL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::BindingEventOccurred
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::BindingEventOccurred( 
    const CBinding& aBinding, TSTUNCallbackInfo::TFunction aEvent )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::BindingEventOccurred" )
    TRAP_IGNORE( iAsyncCallback->MakeCallbackL( aEvent,
                                                &aBinding,
                                                KErrNone ) );
                                   
    __STUNTURNCLIENT( "CSTUNClientImplementation::BindingEventOccurred exit" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::PassInitCompletedL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::PassInitCompletedL( TInt aError ) const
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::PassInitCompletedL" )
    __TEST_INVARIANT;

    iAsyncCallback->MakeCallbackL( TSTUNCallbackInfo::EInitCompleted,
                                   NULL,
                                   aError,
                                   &iClient );

    __STUNTURNCLIENT( "CSTUNClientImplementation::PassInitCompletedL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::PassCredentialsRejectedL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::PassCredentialsRejectedL(
    const CBinding& aBinding ) const
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::PassCredentialsRejectedL" )
    __TEST_INVARIANT;

    iAsyncCallback->MakeCallbackL( TSTUNCallbackInfo::EEventCredentialsRejected,
                                   &aBinding );
    __STUNTURNCLIENT( "CSTUNClientImplementation::PassCredentialsRejectedL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::IsInitialized
// ---------------------------------------------------------------------------
//
TBool CSTUNClientImplementation::IsInitialized() const
    {
    __TEST_INVARIANT;

    return ( iState == iReady ) || ( iState == iRenewSharedSecret );
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::STUNServerAddrL
// ---------------------------------------------------------------------------
//
const TInetAddr& CSTUNClientImplementation::STUNServerAddrL() const
    {
    __TEST_INVARIANT;
    __ASSERT_ALWAYS( IsInitialized(), User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iUdpAddresses.Count() > 0, User::Leave( KErrNotFound ) );

    return iUdpAddresses[ 0 ];
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::SetCredentialsL
// iCredentials and iSharedSecret can both exist at the same time.
// ---------------------------------------------------------------------------
//    
void CSTUNClientImplementation::SetCredentialsL( const TDesC8& aUsername,
                                                 const TDesC8& aPassword )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::SetCredentialsL in" )
    __TEST_INVARIANT;
    __STUN_ASSERT_L( aUsername.Length(), KErrArgument );
    __STUN_ASSERT_L( aPassword.Length(), KErrArgument );
    
    CSTUNCredentials* credentials = CSTUNCredentials::NewL( aUsername,
                                                            aPassword );
    delete iCredentials;
    iCredentials = NULL;
    iCredentials = credentials;

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::SetCredentialsL end" )
    }

// -----------------------------------------------------------------------------
// CSTUNClientImplementation::SharedSecretObtained
// -----------------------------------------------------------------------------
//
TBool CSTUNClientImplementation::SharedSecretObtained() const
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::SharedSecretObtained" )
    return ( iSharedSecret && iSharedSecret->Username().Length() > 0 );
    }

// -----------------------------------------------------------------------------
// CSTUNClientImplementation::HasPresetCredentials
// -----------------------------------------------------------------------------
//
TBool CSTUNClientImplementation::HasPresetCredentials() const
    {
    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::HasPresetCredentials" )
    return iCredentials != NULL;
    }

// -----------------------------------------------------------------------------
// CSTUNClientImplementation::ChangeState
// -----------------------------------------------------------------------------
//
void CSTUNClientImplementation::ChangeState( const CSTUNClientState* aNewState )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::ChangeState" )
    __TEST_INVARIANT;

    iState = aNewState;

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::ChangeState end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::Terminate
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::Terminate( TInt aError )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::Terminate" )
    __TEST_INVARIANT;
    // Terminate only once    
    __STUN_ASSERT_RETURN( iState != NULL, KErrDied );    
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    NATFWUNSAF_INTLOG( "STUNClient terminated, reason", aError )

    if ( !IsInitialized() )
        {
        TRAP_IGNORE( PassInitCompletedL( aError ) )
        }
    ChangeState( NULL );

    // A terminating binding calls CSTUNClientImplementation::BindingErrorL
    for ( TInt i = 0; i < iBindings.Count(); ++i )
        {
        iBindings[i]->Implementation().Terminate( aError );
        }

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::Terminate end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::TcpResolvedL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::TcpResolvedL( TBool aObtainSharedSecret )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::TcpResolvedL" )
    __TEST_INVARIANT;

    iObtainSharedSecret = aObtainSharedSecret;
    
    if ( iObtainSharedSecret )
        {
        ResolveAddressL( KTls, iTcpAddresses, KTlsPort, *iServiceName );
        }
    else
        {
        ResolveUdpL();
        }

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::TcpResolvedL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::TlsResolvedL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::TlsResolvedL( )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::TlsResolvedL" )

    ResolveUdpL();

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::TlsResolvedL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::FreeResolverResources
// Don't use __TEST_INVARIANT, as this function is used from destructor.
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::FreeResolverResources()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::FreeResolverResources" )
    if ( iResolver )
        {
        iResolver->CancelResolving();
        delete iResolver;
        iResolver = NULL;
        }
    __STUNTURNCLIENT( "CSTUNClientImplementation::FreeResolverResources end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ResolveUdpL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::ResolveUdpL()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::ResolveUdpL" )
    __TEST_INVARIANT;

    ResolveAddressL( KUdp, iUdpAddresses );

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::ResolveUdpL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ResolveAddressL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::ResolveAddressL( const TDesC8& aProtocol,
                                                 RArray<TInetAddr>& aResult )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::ResolveAddressL" )
    __TEST_INVARIANT;
    
    __STUNTURNCLIENT_STR8( "stunserveraddress: ", *iServerAddress )
    __STUNTURNCLIENT_INT1( "stunserverport: ", iServerPort )
    __STUNTURNCLIENT_STR8( "protocol: ", aProtocol )
    
    iResolver->ResolveL( *iServerAddress,
                         *iServiceName,
                         aProtocol,
                         iServerPort,
                         aResult );
    
    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::ResolveAddressL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::ResolveAddressL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::ResolveAddressL( const TDesC8& aProtocol,
                                                 RArray<TInetAddr>& aResult,
                                                 TUint aPort,
                                                 const TDesC8& aService )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::ResolveAddressL" )
    __TEST_INVARIANT;
    
    __STUNTURNCLIENT_STR8( "stunserveraddress: ", *iServerAddress )
    __STUNTURNCLIENT_INT1( "stunserverport: ", iServerPort )
    __STUNTURNCLIENT_STR8( "protocol: ", aProtocol )
    __STUNTURNCLIENT_STR8( "service: ", aService )
    
    iResolver->ResolveL( *iServerAddress,
                         aService,
                         aProtocol,
                         aPort,
                         aResult );
    
    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::ResolveAddressL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::GetSharedSecretFromServerL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::GetSharedSecretFromServerL()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::\
    GetSharedSecretFromServerL" )
    __TEST_INVARIANT;

    __ASSERT_ALWAYS( iTcpAddresses.Count() > 0, User::Leave( KErrNotFound ) );

    delete iSharedSecret;
    iSharedSecret = NULL;
    
    if ( iCredentials )
        {
        iSharedSecret = CSTUNSharedSecret::NewL( iSocketServer,
                                             iConnection,
                                             DeltaTimer(),
                                             iTcpAddresses[0],
                                             *iTransactionIDGenerator,
                                             *this,
                                             iCredentials->Username(),
                                             iCredentials->Password() );
        }
    
    else
        {
        iSharedSecret = CSTUNSharedSecret::NewL( iSocketServer,
                                             iConnection,
                                             DeltaTimer(),
                                             iTcpAddresses[0],
                                             *iTransactionIDGenerator,
                                             *this,
                                             KNullDesC8(),
                                             KNullDesC8() );
        }
    
    // Stop current shared secret's timer
    StopTimer();

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::\
    GetSharedSecretFromServerL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::PassSharedSecretToBindingL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::PassSharedSecretToBindingL(
    CBinding& aBinding ) const
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::PassSharedSecretToBindingL" )
    __TEST_INVARIANT;

    const TDesC8* username = &KNullDesC8();
    const TDesC8* password = &KNullDesC8();
    
    if ( iSharedSecret )
        {
        if ( iObtainSharedSecret )
            {
            username = &iSharedSecret->Username();
            password = &iSharedSecret->Password();
            }
        }
        
    else if ( iCredentials )
        {
        username = &iCredentials->Username();
        password = &iCredentials->Password();
        }
    else
        {
        // if ... else if chain end
        }  
    aBinding.Implementation().SharedSecretObtainedL( 
        *username, *password );
    
    __STUNTURNCLIENT( "CSTUNClientImplementation::PassSharedSecretToBindingL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::TimerExpiredL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::TimerExpiredL()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::TimerExpiredL" )
    __TEST_INVARIANT;

    GetSharedSecretFromServerL();
    ChangeState( iRenewSharedSecret );

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::TimerExpiredL end" )
    }

// -----------------------------------------------------------------------------
// CSTUNClientImplementation::LeaveFromTimerExpired
// -----------------------------------------------------------------------------
//
void CSTUNClientImplementation::LeaveFromTimerExpired( TInt aError )
    {
    __STUNTURNCLIENT_INT1( "CSTUNClientImplementation::\
    LeaveFromTimerExpired error:", aError )
    __TEST_INVARIANT;
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    Terminate( aError );
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::StartSharedSecretTimer
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::StartSharedSecretTimer()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::StartSharedSecretTimer" )
    __TEST_INVARIANT;

    const TUint KSharedSecretValidTime = 540000; // 9 minutes
    StartTimer( KSharedSecretValidTime );

    __TEST_INVARIANT;
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::MultiplexerInstance
// ---------------------------------------------------------------------------
//
MNcmConnectionMultiplexer* 
    CSTUNClientImplementation::MultiplexerInstance()
    {
    return iMultiplexer;
    }

// -----------------------------------------------------------------------------
// CSTUNClientImplementation::RenewSharedSecretL
// -----------------------------------------------------------------------------
//
void CSTUNClientImplementation::RenewSharedSecretL()
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::RenewSharedSecretL" )
    if ( !iObtainSharedSecret )
        {
        delete iResolver;
        iResolver = NULL;
        iResolver = CNATFWUNSAFServerResolver::NewL( iSocketServer,
                                                     iConnection,
                                                     *this,
                                                     ETrue );
        iObtainSharedSecret = ETrue;
        
        __STUNTURNCLIENT_STR8( "servicename: ", *iServiceName )
        
        if ( !iResolvingTLS )
            {
            iResolvingTLS = new ( ELeave ) 
                CSTUNClientResolvingTLS( *iResolvingUDP );
            // must set state to ensure that TCP state is not current state
            ChangeState( iResolvingTLS );
            delete iResolvingTCP;
            iResolvingTCP = NULL;
            
            iResolvingTCP = new ( ELeave ) 
                CSTUNClientResolvingTCP( *iResolvingTLS, *iResolvingUDP );
            }
        
        if ( iServiceName->Compare( KStunRelay ) == 0 )
            {
            ChangeState( iResolvingTLS );
            ResolveAddressL( KTls, iTcpAddresses, KTlsPort, *iServiceName );
            }
        else
            {
            ChangeState( iResolvingTCP );
            ResolveAddressL( KTcp, iTcpAddresses, iServerPort, *iServiceName );
            }
        }
    
    else
        {
        StopTimer();
        __STUNTURNCLIENT( "timer stopped" )
        TimerExpiredL();
        }
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::UsenameForIndication
// ---------------------------------------------------------------------------
//
const TDesC8& CSTUNClientImplementation::UsernameForIndication()
    {
    if ( !iCredentials )
        {
        return KNullDesC8();
        }
    
    return iCredentials->Username();
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::UsenameForIndication
// ---------------------------------------------------------------------------
//
const TDesC8& CSTUNClientImplementation::PasswordForIndication()
    {
    if ( !iCredentials )
        {
        return KNullDesC8();
        }
    
    return iCredentials->Password();
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::InformWaitingBindingsL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::InformWaitingBindingsL() const
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::InformWaitingBindingsL" )
    __TEST_INVARIANT;

    for ( TInt i = 0; i < iBindings.Count(); ++i )
        {
        if ( iBindings[i]->Implementation().IsWaitingSharedSecret() )
            {
            PassSharedSecretToBindingL( *iBindings[i] );
            }
        }
    __STUNTURNCLIENT( "CSTUNClientImplementation::InformWaitingBindingsL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::HandleSharedSecretErrorL
// ---------------------------------------------------------------------------
//
void CSTUNClientImplementation::HandleSharedSecretErrorL( TInt aError )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::HandleSharedSecretErrorL" )
    __TEST_INVARIANT;
    __STUN_ASSERT_L( aError != KErrNone, KErrArgument );

     if ( aError == KErrCouldNotConnect || aError == KErrTimedOut )
         {
         if ( iTcpAddresses.Count() > 1 )
             {
             // Remove the failed address. Remain in the current state.
             iTcpAddresses.Remove( 0 );
             GetSharedSecretFromServerL();
             }
         else
             {
             iObtainSharedSecret = EFalse;
             if ( !IsInitialized() )
                {
                PassInitCompletedL( KErrNone );
                }
             ChangeState( iReady );
             }
         }
     else
         {
         Terminate( aError );
         }
 
     __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::HandleSharedSecretErrorL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNClientImplementation::DoesSharedSecretMatch
// Username and password can be empty.
// ---------------------------------------------------------------------------
//
TBool CSTUNClientImplementation::DoesSharedSecretMatch(
    const TDesC8& aUsername,
    const TDesC8& aPassword ) const
    {
    __TEST_INVARIANT;

    return ( iCredentials &&
             iCredentials->Compare( aUsername, aPassword ) ) ||
           ( iSharedSecret &&
             aUsername.Compare( iSharedSecret->Username() ) == 0 &&
             aPassword.Compare( iSharedSecret->Password() ) == 0 );
    }

// -----------------------------------------------------------------------------
// CSTUNClientImplementation::RemoveAddress
// -----------------------------------------------------------------------------
//
void CSTUNClientImplementation::RemoveAddress( const TInetAddr& aAddress )
    {
    __STUNTURNCLIENT( "CSTUNClientImplementation::RemoveAddress" )
    __TEST_INVARIANT;

    for ( TInt i = 0; i < iUdpAddresses.Count(); ++i )
        {
        if ( iUdpAddresses[ i ].CmpAddr( aAddress ) )
            {
            iUdpAddresses.Remove( i );
            }
        }

    __TEST_INVARIANT;
    __STUNTURNCLIENT( "CSTUNClientImplementation::RemoveAddress end" )
    }

// -----------------------------------------------------------------------------
// CSTUNClientImplementation::__DbgTestInvariant
// -----------------------------------------------------------------------------
//
void CSTUNClientImplementation::__DbgTestInvariant() const
    {
#if defined( _DEBUG )
    if ( ( iState == iResolvingTCP || iState == iResolvingUDP ) &&
         ( !iResolver || !iServerAddress ) )
        {
        User::Invariant();
        }
    if ( !iTransactionIDGenerator ||
         !iAsyncCallback ||
         !iResolvingTCP ||
         !iResolvingUDP ||
         !iGetSharedSecret ||
         !iReady )
        {
        User::Invariant();
        }
#endif
    }
