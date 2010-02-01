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




// INCLUDES
#include "stunassert.h"
#include "natfwunsaflog.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunsafbindingindication.h"
#include "cstunbindingimplementation.h"
#include "natfwstunbinding.h"
#include "cstunbindinginit.h"
#include "cstunbindinggetsharedsecret.h"
#include "cstunbindinggetaddress.h"
#include "cstunbindingactive.h"
#include "cstunbindingwaittoretry.h"
#include "mstunbindingobserver.h"
#include "cstuntransaction.h"
#include "stunutils.h"
#include "natfwstunclientdefs.h"
#include "cstunindicationtransmitter.h"

// Attribures
#include "natfwunsafusernameattribute.h"
#include "natfwunsafxoronlyattribute.h"
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafusecandidateattribute.h"
#include "natfwunsafpriorityattribute.h"
#include "natfwunsafdataattribute.h"
#include "natfwunsafremoteaddressattribute.h"
#include "natfwunsaficecontrollingattribute.h"
#include "natfwunsaficecontrolledattribute.h"
#include "natfwunsafalternateserverattribute.h"
#include "natfwunsafrealmattribute.h"
#include "natfwunsafnonceattribute.h"



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::NewL
// ---------------------------------------------------------------------------
//
CSTUNBindingImplementation*
    CSTUNBindingImplementation::NewL( CBinding& aBinding,
                                      MSTUNBindingObserver& aClient,
                                      RSocket& aSocket )
    {

    CSTUNBindingImplementation* self =
    new ( ELeave ) CSTUNBindingImplementation( aBinding, aClient, aSocket );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::NewL - overloaded
// ---------------------------------------------------------------------------
//
CSTUNBindingImplementation*
    CSTUNBindingImplementation::NewL( CBinding& aBinding,
                                      MSTUNBindingObserver& aClient,
                                      TUint aStreamId,
                                      TUint aConnectionId,
                                      MNcmConnectionMultiplexer* aMux )
    {

    CSTUNBindingImplementation* self =
    new ( ELeave ) CSTUNBindingImplementation( aBinding, aClient, aStreamId,
                                               aConnectionId, aMux );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::CSTUNBindingImplementation
// ---------------------------------------------------------------------------
//
CSTUNBindingImplementation::CSTUNBindingImplementation(
    CBinding& aBinding,
    MSTUNBindingObserver& aClient,
    RSocket& aSocket ) :
    CBindingImplementation( aClient, aBinding, aSocket )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::CSTUNBindingImplementation
// ---------------------------------------------------------------------------
//
CSTUNBindingImplementation::CSTUNBindingImplementation(
    CBinding& aBinding,
    MSTUNBindingObserver& aClient,
    TUint aStreamId,
    TUint aConnectionId,
    MNcmConnectionMultiplexer* aMux ) :
    CBindingImplementation( aClient, aBinding, aStreamId, aConnectionId,
    aMux ), iDestAddr( KAFUnspec )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::ConstructL()
    {
    ClientL().AttachBindingL( iBinding );

    iRetransmitInterval = ClientL().RetransmitInterval();

    iGetAddress = new ( ELeave ) CSTUNBindingGetAddress();
    iGetSharedSecret =
    new ( ELeave ) CSTUNBindingGetSharedSecret( *iGetAddress );
    iInit = new ( ELeave ) CSTUNBindingInit( *iGetSharedSecret );
    iActive = new ( ELeave ) CSTUNBindingActive( *iGetSharedSecret );
    iWaitToRetry = new ( ELeave ) CSTUNBindingWaitToRetry( *iGetSharedSecret );
    iGetAddress->SetNeighbourStates( *iGetSharedSecret,
                                     *iWaitToRetry,
                                     *iActive );
    
    iIndicationTx = CStunIndicationTransmitter::NewL( *iMux, 
        iStreamId, iConnectionId );
    
    ChangeState( *iInit );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::~CSTUNBindingImplementation
// ---------------------------------------------------------------------------
//
CSTUNBindingImplementation::~CSTUNBindingImplementation()
    {
    __STUNTURNCLIENT( "CSTUNBindingImplementation::~CSTUNBindingImplementation" )
    if ( iClient )
        {
        iClient->DetachBinding( iBinding );
        }

    this->FreeRequestData();

    delete iInit;
    delete iGetSharedSecret;
    delete iGetAddress;
    delete iWaitToRetry;
    delete iActive;
    delete iIndicationTx;

    iSocket = NULL;
    iMux = NULL;

    __STUNTURNCLIENT( "CSTUNBindingImplementation::~CSTUNBindingImplementation exit" )
    }

// -----------------------------------------------------------------------------
// CSTUNBindingImplementation::TimerExpiredL
// -----------------------------------------------------------------------------
//
void CSTUNBindingImplementation::TimerExpiredL()
    {
    __STUNTURNCLIENT( "CSTUNBindingImplementation_STUB::TimerExpiredL" )
    TInetAddr addr;
    addr.SetAddress( KInetAddrLoop );
    StoreAddressL( addr );
    iState->TimerExpiredL( *this );
    }

// -----------------------------------------------------------------------------
// CSTUNBindingImplementation::LeaveFromTimerExpired
// -----------------------------------------------------------------------------
//
void CSTUNBindingImplementation::LeaveFromTimerExpired( TInt aError )
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    Terminate( aError );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::PublicAddressObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::PublicAddressObtainedL(
    const TInetAddr& aAddress )
    {
    iState->PublicAddressReceivedL( *this, aAddress );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::PublicAddressObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::PublicAddressObtainedL(
    const TInetAddr& aReflexiveAddr, const TInetAddr& aRelayAddr )
    {
    iState->PublicAddressReceivedL( *this, aReflexiveAddr, aRelayAddr );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::TransactionError
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::TransactionError( TInt aError,
    CNATFWUNSAFUnknownAttributesAttribute* aUnknownAttr )
    {
    delete iUnknownAttr;
    iUnknownAttr = aUnknownAttr;

    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );
    iState->TransactionError( *this, aError );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::TransactionEventOccurredL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::TransactionEventOccurredL(
    TSTUNCallbackInfo::TFunction /*aEvent*/ )
    {
    __STUNTURNCLIENT(
        "CSTUNBindingImplementation::TransactionEventOccurredL -should not get called" )
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::HandleTransactionError
// ---------------------------------------------------------------------------

TBool CSTUNBindingImplementation::HandleTransactionError( TInt aError )
    {
    __STUNTURNCLIENT_INT1( "HandleTransactionError, reason", aError )

    const TInt KMaxErrorResponseCount = 3;

    __STUN_ASSERT_RETURN_VALUE( iRequest && aError != KErrNone,
    EFalse );

    delete iTransaction;
    iTransaction = NULL;
    iTransactionError = aError;

    if ( ( aError > 0 ) && ( ++iErrorResponseCount >= KMaxErrorResponseCount ) )
        {
        iErrorResponseCount = 0;
        return EFalse;
        }

    CNATFWUNSAFAlternateServerAttribute* alternateServer = NULL;
    HBufC8* realmValue = NULL;

    switch ( aError )
        {
        case KErrTimedOut:
            __STUNTURNCLIENT(
                "CSTUNBindingImplementation - KErrTimedOut" )
            //Clear iAddXorOnly as another address is tried
            iAddXorOnly = EFalse;
            return iClient && iClient->ObtainServerAddress( iServerAddress );

        case E300TryAlternate:
            // The client SHOULD attempt a new transaction to the server
            // indicated in the ALTERNATE-SERVER attribute.
            alternateServer = static_cast<CNATFWUNSAFAlternateServerAttribute*>
            ( iRequest->Attribute( CNATFWUNSAFAttribute::EAlternateServer ) );
            if ( alternateServer )
                {
                iServerAddress = alternateServer->Address();
                return ETrue;
                }
            return EFalse;

        case E4XX:
            return EFalse;

        case E401Unauthorized:
            if ( iSharedSecret )
                {
                CNATFWUNSAFRealmAttribute* realm =
                    static_cast<CNATFWUNSAFRealmAttribute*>
                    ( iRequest->Attribute( CNATFWUNSAFAttribute::ERealm ) );
                if ( realm || !iRealmFromResponse )
                    {
                    // indicates that this was retry and it did not work.
                    // notify client -> unrecoverable error
                    return EFalse;
                    }
                }
            return ETrue;

        case E420UnknownAttributes:
            //Remove the unknown attributes and retry
            return iUnknownAttr != NULL;

        case E430StaleCredentials:
            // Client used a short term credential that has expired so
            // generate a new Shared Secret request
            return iSharedSecret != NULL;

        case E431IntegrityCheckFailure:
            {
            TBool retry = EFalse;
            TRAPD( err, retry =
                ClientL().SharedSecretRejectedL( iBinding,
                                                Username(),
                                                *iSharedSecret ) );
            return err == KErrNone && retry;
            }

        case E432MissingUsername:
            //If missing USERNAME or MESSAGE-INTEGRITY, add them and retry
            return !iRequest->Attribute( CNATFWUNSAFAttribute::EUsername ) ||
               !iSharedSecret;

        case E433UseTLS:
            // If request was a Shared Secret request and wasn't sent over
            // TLS, the client SHOULD retry the request with TLS.
            return iSharedSecret != NULL;

        case E434MissingRealm:
            if ( !iRequest->Attribute( CNATFWUNSAFAttribute::ERealm ) &&
                 iSharedSecret )
                {
                // use a long term credential and retry the request using the username
                // and password associated with the REALM
                return ETrue;
                }  
            TRAPD( err, realmValue = ( static_cast<CNATFWUNSAFRealmAttribute*>
                ( iRequest->Attribute( CNATFWUNSAFAttribute::ERealm ) ) )->
                Value().AllocL() );
            if ( err || !realmValue )
                {
                delete realmValue;
                return EFalse;
                }
                
            if ( realmValue->Compare( *iRealmFromResponse ) )
                {
                // the client SHOULD retry using the username and
                // password associated with the REALM in the response
                iUseRealmFromResponse = ETrue;
                delete realmValue;
                return ETrue;
                }
            delete realmValue;
            return EFalse;


        case E435MissingNonce:
            if ( iRequest->Attribute( CNATFWUNSAFAttribute::ENonce ) )
                {
                return EFalse;
                }
            // retry using a nonce from error response
            return ETrue;

        case E436UnknownUsername:
            if ( iSharedSecret )
                {
                return ETrue;
                }
            // If the username was collected from the user, alert the user.
            return EFalse;
                
        case E438StaleNonce:
            if ( iRequest->Attribute( CNATFWUNSAFAttribute::ENonce ) )
                {
                // retry using a nonce from error response
                return ETrue;
                }
            return EFalse;
        
        case E487RoleConflict:
            return EFalse;
            
        case E500ServerError:
            return ETrue;

        case E600GlobalFailure:
            return EFalse;

        case ERetryAfterAddingXorOnly:
            iAddXorOnly = ETrue;
            return ETrue;

        default:
            return STUNUtils::Is5xxResponse( aError );
        }

    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::IcmpError
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::IcmpError( const TInetAddr& aAddress )
    {
    if ( iTransaction && iServerAddress.CmpAddr( aAddress ) )
        {
        //Try next address, or terminate with KErrTimedOut.
        iTransaction->Terminate( KErrTimedOut );
        }
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::SendRequestL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::SendRequestL()
    {
    iRetransmitInterval = ClientL().RetransmitInterval();
    iAddFingerprint = EFalse;

    iState->SendRequestL( *this );
    }


// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::SendRequestL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::SendRequestL( const TInetAddr& aDestAddr,
                                               TBool aAddFingerprint,
                                               TUint aRtoValue )
    {
    if ( 0 != aRtoValue )
        {
        iRetransmitInterval = aRtoValue;
        }

    if ( KAFUnspec != aDestAddr.Family() )
        {
        iDestAddr.SetAddress( aDestAddr.Address() );
        iDestAddr.SetPort( aDestAddr.Port() );
        }

    iAddFingerprint = aAddFingerprint;

    iState->SendRequestL( *this );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::StreamId
// ---------------------------------------------------------------------------
//
TUint CSTUNBindingImplementation::StreamId() const
    {
    return iStreamId;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::ConnectionId
// ---------------------------------------------------------------------------
//
TUint CSTUNBindingImplementation::ConnectionId() const
    {
    return iConnectionId;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::CancelRequest
// If transaction exists, delete it to stop using the socket.
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::CancelRequest()
    {
    __STUNTURNCLIENT( "CSTUNBindingImplementation::CancelRequest" )
    
    if ( AddressResolved() )
        {
        ChangeState( *iActive );
        delete iTransaction;
        iTransaction = NULL;
        }
    else
        {
        //The initial request is canceled
        ChangeState( *iInit );
        Terminate( KErrCancel );
        }
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::Socket
// ---------------------------------------------------------------------------
//
const RSocket& CSTUNBindingImplementation::Socket() const
    {
    return *iSocket;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::AddressResolved
// The most recent public address is kept also during a refresh.
// ---------------------------------------------------------------------------
//
TBool CSTUNBindingImplementation::AddressResolved() const
    {
    return !iPublicAddr.IsUnspecified();
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::PublicAddr
// ---------------------------------------------------------------------------
//
const TInetAddr& CSTUNBindingImplementation::PublicAddr() const
    {
    return iPublicAddr;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::AlternateServerAddr
// ---------------------------------------------------------------------------
//
const TInetAddr& CSTUNBindingImplementation::AlternateServerAddr() const
    {
    return iServerAddress;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::RealmFromResponse
// ---------------------------------------------------------------------------
//
const HBufC8* CSTUNBindingImplementation::RealmFromResponse() const
    {
    return iRealmFromResponse;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::HandleDataL
// Must NOT leave, if aData is not a STUN message!
// ---------------------------------------------------------------------------
//
HBufC8* CSTUNBindingImplementation::HandleDataL(
    const TDesC8& aData, TBool& aConsumed, TInetAddr& aRemoteAddr )
    {
    __STUNTURNCLIENT( "CSTUNBindingImplementation::HandleDataL" )
    aConsumed = EFalse;
    TInt err( KErrNone );
    HBufC8* dataPointer = NULL;

    CNATFWUNSAFMessage* msg = NULL;
    TRAP( err, msg = DecodeMessageL( aData ) );

    if ( msg )
        {
        CNATFWUNSAFRealmAttribute* realm = static_cast<CNATFWUNSAFRealmAttribute*>(
            msg->Attribute( CNATFWUNSAFAttribute::ERealm ) );
        if ( realm )
            {
            delete iRealmFromResponse;
            iRealmFromResponse = NULL;
            const TDesC8& realmValue = ( static_cast<CNATFWUNSAFRealmAttribute*>(
                msg->Attribute( CNATFWUNSAFAttribute::ERealm ) )->Value() );
            iRealmFromResponse = realmValue.AllocL();
            __STUNTURNCLIENT_STR8( "realm: ", *iRealmFromResponse )
            }

        CNATFWUNSAFNonceAttribute* nonce = static_cast<CNATFWUNSAFNonceAttribute*>(
            msg->Attribute( CNATFWUNSAFAttribute::ENonce ) );
        if ( nonce )
            {
            delete iNonce;
            iNonce = NULL;
            const TDesC8& nonceValue = ( static_cast<CNATFWUNSAFNonceAttribute*>(
            msg->Attribute( CNATFWUNSAFAttribute::ENonce ) )->Value() );
            iNonce = nonceValue.AllocL();
            }

        // if aData contains data indication, decode it (TURN Specific)
        if ( msg->Type() == CNATFWUNSAFMessage::EDataIndication &&
             KErrNone == err )
            {
            CleanupStack::PushL( msg );
            const TDesC8& data = ( static_cast<CNATFWUNSAFDataAttribute*>(
                msg->Attribute( CNATFWUNSAFAttribute::EData ) )->Value() );

            // Remote address is returned through reference
            CNATFWUNSAFRemoteAddressAttribute* remoteAddress =
                static_cast<CNATFWUNSAFRemoteAddressAttribute*>
                ( msg->Attribute( CNATFWUNSAFAttribute::ERemoteAddress ) );

            if( remoteAddress )
                {
                aRemoteAddr = remoteAddress->Address();
                }
            else
                {
                remoteAddress = NULL;
                }

            CNATFWUNSAFMessage* msg_unsaf = NULL;

            dataPointer = data.AllocLC(); // to cleanupstack

            TRAP( err, msg_unsaf = DecodeMessageL( *dataPointer ) );
            CleanupStack::PushL( msg_unsaf );

            if ( iTransaction && iRequest && !err && msg_unsaf )
                {
                if ( ValidateMsgType( msg_unsaf ) &&
                     iRequest->TransactionID() == msg_unsaf->TransactionID() )
                    {
                    aConsumed = ETrue;
                    iTransaction->ReceiveL( *msg_unsaf, *dataPointer );
                    CleanupStack::PopAndDestroy( msg_unsaf );
                    CleanupStack::PopAndDestroy( dataPointer );
                    dataPointer = NULL;
                    }
                }

            if ( dataPointer )
                {
                // dataPointer is still valid. This means that message was not
                // consumed, and data indication is ripped off from it.
                // Message can also contain media data.
                // Data indication is decapsulated out of that message.
                CleanupStack::PopAndDestroy( msg_unsaf );
                CleanupStack::Pop( dataPointer );
                }
            CleanupStack::Pop( msg );
            }

        // Data was NOT in encapsulated in data indication, so offer
        // message to this client.
        else if ( !err && iTransaction && iRequest )
            {
            CleanupStack::PushL( msg );

            if ( ValidateMsgType( msg ) &&
                 iRequest->TransactionID() == msg->TransactionID() )
                {
                aConsumed = ETrue;
                iTransaction->ReceiveL( *msg, aData );
                }
            CleanupStack::Pop( msg );
            }
        }
    delete msg;
    return dataPointer;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::STUNClient
// ---------------------------------------------------------------------------
//
const CSTUNClient* CSTUNBindingImplementation::STUNClient() const
    {
    return iClient ? &iClient->STUNClient() : NULL;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::GetServerAddressL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::GetServerAddressL()
    {
    __ASSERT_ALWAYS( iServerAddress.IsUnspecified(),
     User::Leave( KErrAlreadyExists ) );
    __ASSERT_ALWAYS( ClientL().ObtainServerAddress( iServerAddress ),
     User::Leave( KErrNotFound ) );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::GetSharedSecretL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::GetSharedSecretL()
    {
    ClientL().ObtainSharedSecretL( iBinding );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::Username
// ---------------------------------------------------------------------------
//
const TDesC8& CSTUNBindingImplementation::Username() const
    {
    if ( iRequest && iRequest->HasAttribute( CNATFWUNSAFAttribute::EUsername ) )
        {
        return static_cast<CNATFWUNSAFUsernameAttribute*>(
        iRequest->Attribute( CNATFWUNSAFAttribute::EUsername ) )->Value();
        }

    return KNullDesC8;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::DetachClient
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::DetachClient()
    {
    iClient = NULL;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::StoreAddressL
// Transaction no longer needed.
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::StoreAddressL(
    const TInetAddr& aPublicAddress )
    {
    iErrorResponseCount = 0;
    iPublicAddr = aPublicAddress;
    ClientL().AddressResolvedL( iBinding );

    FreeRequestData();
    }

// -----------------------------------------------------------------------------
// CSTUNBindingImplementation::ChangeState
// -----------------------------------------------------------------------------
//
void CSTUNBindingImplementation::ChangeState(
    CSTUNBindingState& aNewState )
    {
    iState = &aNewState;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::Terminate
// Binding can be re-started with SendRequestL.
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::Terminate( TInt aError )
    {
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    NATFWUNSAF_INTLOG( "STUNBinding terminated, reason", aError )

    TInetAddr emptyAddr;
    iPublicAddr = emptyAddr;
    iServerAddress = emptyAddr;
    iAddXorOnly = EFalse;
    iErrorResponseCount = 0;
    FreeRequestData();

    if ( iState != iInit )
        {
        __STUNTURNCLIENT( "Binding terminated" )

        ChangeState( *iInit );
        TRAP_IGNORE( ClientL().BindingErrorL( Binding(), aError, ETrue ) )
        }
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::FreeRequestData
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::FreeRequestData()
    {
    __STUNTURNCLIENT( "CSTUNBindingImplementation_STUB::FreeRequestData" )
    delete iTransaction;
    iTransaction = NULL;
    delete iUnknownAttr;
    iUnknownAttr = NULL;
    delete iRequest;
    iRequest = NULL;
    delete iSharedSecret;
    iSharedSecret = NULL;
    delete iRealmFromResponse;
    iRealmFromResponse = NULL;
    delete iNonce;
    iNonce = NULL;
	iICEAttributes = TICEAttributes();
	iAddFingerprint = EFalse;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::IsWaitingSharedSecret
// ---------------------------------------------------------------------------
//
TBool CSTUNBindingImplementation::IsWaitingSharedSecret() const
    {
    return iState == iGetSharedSecret;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::SharedSecretObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::SharedSecretObtainedL(
    const TDesC8& aUsername, const TDesC8& aPassword )
    {
    __STUN_ASSERT_L( ( aUsername.Length() > 0 && aPassword.Length() > 0 ) ||
     ( aUsername.Length() == 0 && aPassword.Length() == 0 ), KErrArgument );

    iState->SharedSecretObtainedL( *this, aUsername, aPassword );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::CreateBindingRequestL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::CreateBindingRequestL(
    const TDesC8& aUsername, const TDesC8& aPassword )
    {
    delete iRequest;
    iRequest = NULL;

    if ( IsSharedSecretRequired() &&
         ( aUsername.Length() == 0 || aPassword.Length() == 0 ) )
         {
         Terminate( KErrAccessDenied );
         User::Leave( KErrAccessDenied );
         }
    iTransactionError = KErrNone;

    HBufC8* newSharedSecret = aPassword.AllocL();
    delete iSharedSecret;
    iSharedSecret = newSharedSecret;

    TNATFWUNSAFTransactionID transactionID;
    ClientL().ObtainTransactionIDL( transactionID );
    iRequest = CNATFWUNSAFBindingRequest::NewL( transactionID );

    if ( aUsername.Length() > 0 )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL\
         - Add USERNAME attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( aUsername ) );
        CleanupStack::Pop(); //CNATFWUNSAFUsernameAttribute
        }

    if ( iICEAttributes.iPriority > 0 )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL\
         - Add PRIORITY attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFPriorityAttribute::NewLC(
            iICEAttributes.iPriority ) );
        CleanupStack::Pop(); // CNATFWUNSAFPriorityAttribute
        }

    if ( iICEAttributes.iUseCandidate )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL\
         - Add USE_CANDIDATE attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFUseCandidateAttribute::NewLC() );
        CleanupStack::Pop(); // CNATFWUNSAFUseCandidateAttribute
        }

    if ( iICEAttributes.iControlled > 0 )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL\
         - Add CONTROLLED attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFIceControlledAttribute::NewLC(
            iICEAttributes.iControlled ) );
        CleanupStack::Pop(); // CNATFWUNSAFIceControlledAttribute
        }

    if ( iICEAttributes.iControlling > 0 )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL\
         - Add CONTROLLING attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFIceControllingAttribute::NewLC(
            iICEAttributes.iControlling ) );
        CleanupStack::Pop(); // CNATFWUNSAFIceControllingAttribute
        }

    if ( iUseRealmFromResponse )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL\
         - Add REALM attribute" )
        // Should only be added if long term credentials used
        iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC(
            *iRealmFromResponse ) );

        CleanupStack::Pop(); // CNATFWUNSAFRealmAttribute
        }

    if ( iAddXorOnly )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL\
         - Add XOR_ONLY attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFXorOnlyAttribute::NewLC() );
        CleanupStack::Pop(); //CNATFWUNSAFXorOnlyAttribute
        }

    if ( iUnknownAttr )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL - Unknown" )
        STUNUtils::RemoveUnknownAttributes( *iRequest, *iUnknownAttr );
        delete iUnknownAttr;
        iUnknownAttr = NULL;
        }

    if ( iNonce )
        {
        iRequest->AddAttributeL( CNATFWUNSAFNonceAttribute::NewLC(
            *iNonce ) );
        CleanupStack::Pop(); // CNATFWUNSAFNonceAttribute
        }
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::SendBindingRequestL
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::SendBindingRequestL()
    {
    __STUN_ASSERT_L( !iTransaction, KErrAlreadyExists );
    __STUN_ASSERT_L( iRequest && iSharedSecret,KErrNotFound );
    __STUNTURNCLIENT( "CSTUNBindingImplementation_STUB::SendBindingRequestL" )
    StartTimer( 10 );
    
    /*if ( iSocket )
        {
        iTransaction = CSTUNTransaction::NewL( *iRequest,
                                               iServerAddress,
                                               *iSharedSecret,
                                               *iSocket,
                                               ClientL().TimerProvider(),
                                               ClientL().RetransmitInterval(),
                                               *this,
                                               KStun );
        }

    else
        {
        iTransaction = CSTUNTransaction::NewL( *iRequest,
                                               iServerAddress,
                                               *iSharedSecret,
                                               iStreamId,
                                               iSubstreamId,
                                               ClientL().TimerProvider(),
                                               ClientL().RetransmitInterval(),
                                               *this,
                                               KStun,
                                               *iMux,
                                               EFalse,
                                               *iDestAddr );
        }*/
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::WaitBeforeRetrying
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::WaitBeforeRetrying()
    {
    StartTimer( STUNUtils::EWaitBeforeRetryDuration );
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::CancelRetransmission
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::CancelRetransmission()
    {
    if ( iTransaction )
        {
        iTransaction->CancelRetransmission();
        }
    }
    
// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::SendIndicationL
// ---------------------------------------------------------------------------
//    
void CSTUNBindingImplementation::SendIndicationL( const TInetAddr& aRemoteAddr,
    const TDesC8& /*aData*/, TBool aAddFingerprint )
    {
    __STUNTURNCLIENT( "CSTUNBindingImplementation::SendBindingIndicationL" )
    
    HBufC8* username = ClientL().UsernameForIndication().AllocL();
    CleanupStack::PushL( username );
    HBufC8* password = ClientL().PasswordForIndication().AllocL();
    CleanupStack::PushL( password );
    
    TNATFWUNSAFTransactionID transactionID;
    ClientL().ObtainTransactionIDL( transactionID );
    
    CNATFWUNSAFMessage* 
        indication = CNATFWUNSAFBindingIndication::NewL( transactionID );
    CleanupStack::PushL( indication );
    
    if ( username->Length() > 0 )
        {
        __STUNTURNCLIENT( "CSTUNBindingImplementation::CreateBindingRequestL\
         - Add USERNAME attribute" )
        indication->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( *username ) );
        CleanupStack::Pop(); //CNATFWUNSAFUsernameAttribute
        }

    if ( iUnknownAttr )
        {
        STUNUtils::RemoveUnknownAttributes( *indication, *iUnknownAttr );
        delete iUnknownAttr;
        iUnknownAttr = NULL;
        }

    iIndicationTx->TransmitL( *indication, aRemoteAddr, *password, 
        aAddFingerprint );
    
    CleanupStack::PopAndDestroy( indication );
    CleanupStack::PopAndDestroy( password );
    CleanupStack::PopAndDestroy( username );
    
    __STUNTURNCLIENT( "CSTUNBindingImplementation::SendBindingIndicationL out" )
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::SetICESpecificAttributes
// ---------------------------------------------------------------------------
//
void CSTUNBindingImplementation::SetICESpecificAttributes(
    const TICEAttributes& aAttributes )
    {
    iICEAttributes = aAttributes;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::Binding
// ---------------------------------------------------------------------------
//
const CBinding& CSTUNBindingImplementation::Binding()
    {
    return iBinding;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::IsSharedSecretRequired
// ---------------------------------------------------------------------------
//
TBool CSTUNBindingImplementation::IsSharedSecretRequired() const
    {
    return iTransactionError == E401Unauthorized ||
       iTransactionError == E430StaleCredentials ||
       iTransactionError == E431IntegrityCheckFailure ||
       iTransactionError == E432MissingUsername ||
       iTransactionError == E433UseTLS ||
       iTransactionError == E434MissingRealm;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::ClientL
// ---------------------------------------------------------------------------
//
MSTUNBindingObserver& CSTUNBindingImplementation::ClientL() const
    {
    __ASSERT_ALWAYS( iClient, User::Leave( KErrNotFound ) );

    return *iClient;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::DecodeMessageL
// ---------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
    CSTUNBindingImplementation::DecodeMessageL( const TDesC8& aData ) const
    {
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* msg = decoder->DecodeL( aData );
    CleanupStack::PopAndDestroy( decoder );
    return msg;
    }

// ---------------------------------------------------------------------------
// CSTUNBindingImplementation::ValidateMsgType
// ---------------------------------------------------------------------------
//
TBool CSTUNBindingImplementation::ValidateMsgType(
    CNATFWUNSAFMessage* aMsg ) const
    {
    if ( aMsg )
        {
        return ( aMsg->Type() == CNATFWUNSAFMessage::EBindingResponse ||
             aMsg->Type() == CNATFWUNSAFMessage::EBindingErrorResponse );
        }
    return EFalse;
    }

