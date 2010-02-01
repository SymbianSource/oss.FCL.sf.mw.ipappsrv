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
* Description:    
*
*/




#include "stunassert.h"
#include "natfwunsaflog.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsafbindingrequest.h"
#include "natfwunsafallocaterequest.h"
#include "natfwunsafconnectrequest.h"
#include "natfwunsafsetactivedestinationrequest.h"
#include "natfwunsafsendindication.h"
#include "cstunrelaybindingimplementation.h"
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

// Attributes
#include "natfwunsafusernameattribute.h"
#include "natfwunsafxoronlyattribute.h"
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafmagiccookieattribute.h"
#include "natfwunsafdataattribute.h"
#include "natfwunsafremoteaddressattribute.h"
#include "natfwunsafusecandidateattribute.h"
#include "natfwunsafpriorityattribute.h"
#include "natfwunsaficecontrollingattribute.h"
#include "natfwunsaficecontrolledattribute.h"
#include "natfwunsafalternateserverattribute.h"
#include "natfwunsafrealmattribute.h"
#include "natfwunsafnonceattribute.h"



// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::NewL
// ---------------------------------------------------------------------------
//
CSTUNRelayBindingImplementation*
    CSTUNRelayBindingImplementation::NewL( CBinding& aBinding,  
                                           MSTUNBindingObserver& aClient,
                                           RSocket& aSocket )
    {

    CSTUNRelayBindingImplementation* self =
    new ( ELeave ) CSTUNRelayBindingImplementation( aBinding, aClient, aSocket );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::NewL - overloaded
// ---------------------------------------------------------------------------
//
CSTUNRelayBindingImplementation*
    CSTUNRelayBindingImplementation::NewL( CBinding& aBinding,  
                                           MSTUNBindingObserver& aClient,
                                           TUint aStreamId,
                                           TUint aConnectionId,
                                           MNcmConnectionMultiplexer* aMux )
    {

    CSTUNRelayBindingImplementation* self =
    new ( ELeave ) CSTUNRelayBindingImplementation( aBinding, aClient, aStreamId, 
                                               aConnectionId, aMux );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::CSTUNRelayBindingImplementation
// ---------------------------------------------------------------------------
//
CSTUNRelayBindingImplementation::CSTUNRelayBindingImplementation(
    CBinding& aBinding,
    MSTUNBindingObserver& aClient,
    RSocket& aSocket ) :
    CBindingImplementation( aClient, aBinding, aSocket )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::CSTUNRelayBindingImplementation
// ---------------------------------------------------------------------------
//
CSTUNRelayBindingImplementation::CSTUNRelayBindingImplementation( 
    CBinding& aBinding,
    MSTUNBindingObserver& aClient, 
    TUint aStreamId,
    TUint aConnectionId,
    MNcmConnectionMultiplexer* aMux ) :
    CBindingImplementation( aClient, aBinding, aStreamId, aConnectionId, aMux )
    {
    }
    
// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::ConstructL()
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::ConstructL" )
    ClientL().AttachBindingL( iBinding );

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
                                                      iStreamId, 
                                                      iConnectionId );
    ChangeState( *iInit );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::~CSTUNRelayBindingImplementation
// ---------------------------------------------------------------------------
//
CSTUNRelayBindingImplementation::~CSTUNRelayBindingImplementation()
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::~CSTUNRelayBindingImplementation" )
    if ( iClient )
        {
        iClient->DetachBinding( iBinding );
        }

    FreeRequestData();

    delete iInit;
    delete iGetSharedSecret;
    delete iGetAddress;
    delete iWaitToRetry;
    delete iActive;
    delete iIndicationTx;
    
    iSocket = NULL;
    iMux = NULL;
    
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::~CSTUNRelayBindingImplementation exit" )
    }

// -----------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::TimerExpiredL
// -----------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::TimerExpiredL()
    {
    TInetAddr addr;
    addr.SetAddress( KInetAddrLoop );
    StoreAddressL( addr );
    iState->TimerExpiredL( *this );
    }

// -----------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::LeaveFromTimerExpired
// -----------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::LeaveFromTimerExpired( TInt aError )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::LeaveFromTimerExpired" )
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    Terminate( aError );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::PublicAddressObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::PublicAddressObtainedL( 
    const TInetAddr& aAddress )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::PublicAddressObtainedL 1" )
    iState->PublicAddressReceivedL( *this, aAddress );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::PublicAddressObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::PublicAddressObtainedL( 
    const TInetAddr& aReflexiveAddr, const TInetAddr& aRelayAddr )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::PublicAddressObtainedL 2" )
    iState->PublicAddressReceivedL( *this, aReflexiveAddr, aRelayAddr );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::TransactionError
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::TransactionError( TInt aError,
    CNATFWUNSAFUnknownAttributesAttribute* aUnknownAttr )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::TransactionError" )
    delete iUnknownAttr;
    iUnknownAttr = aUnknownAttr;

    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );
    iState->TransactionError( *this, aError );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::TransactionEventOccurredL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::TransactionEventOccurredL( 
    TSTUNCallbackInfo::TFunction aEvent )
    {
    __STUNTURNCLIENT_INT1( 
        "CSTUNRelayBindingImplementation::TransactionEventOccurredL event:",
             aEvent )
    if ( ESetActiveDestinationRequest == iRequestType )
        {
        *iTimerValue = iTransaction->TimerValue();
        }
    
    ClientL().BindingEventOccurred( iBinding, aEvent );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::HandleTransactionError
// ---------------------------------------------------------------------------
//  
TBool CSTUNRelayBindingImplementation::HandleTransactionError( TInt aError )
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
                "CSTUNRelayBindingImplementation - KErrTimedOut" )
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
                
            realmValue = ( static_cast<CNATFWUNSAFRealmAttribute*>
                ( iRequest->Attribute( CNATFWUNSAFAttribute::ERealm ) )->
                Value().AllocL() );
                
            if (  KErrNone != realmValue->Compare( *iRealmFromResponse ) )
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

        case E437NoBindind:
            // There is none yet in place
            return EFalse;

        case E438StaleNonce:                                  
            if ( iRequest->Attribute( CNATFWUNSAFAttribute::ENonce ) )
                {
                // retry using a nonce from error response
                return ETrue;
                }
            return EFalse;
            
        case E439Transitioning:
            // The client should reset the active destination, wait for 
            // 5 seconds and set the active destination to the new value.
            return EFalse;
            
        case E442UnsupportedTransportProtocol:

            return EFalse;
                    
        case E443InvalidIPAddress:
            
            return EFalse;           
            
        case E444InvalidPort:
        
            return EFalse;

        case E445OperationForTCPOnly:

            return EFalse;

        case E446ConnectionAlreadyExists:
         
            return EFalse;
            
        case E486AllocationQuotaReached:
            // The user or client is not authorized to request additional
            // allocations.          
            return EFalse;

        case E500ServerError:               
            return ETrue;  
                                      
        case E507InsufficientCapacity:
            // The server cannot allocate a new port for this client as it has
            // exhausted its relay capacity.        
            return EFalse;

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
// CSTUNRelayBindingImplementation::IcmpError
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::IcmpError( const TInetAddr& aAddress )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::IcmpError" )
    if ( iTransaction && iServerAddress.CmpAddr( aAddress ) )
        {
        //Try next address, or terminate with KErrTimedOut.
        iTransaction->Terminate( KErrTimedOut );
        }
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::AllocateRequestL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::AllocateRequestL( TUint /*aRtoValue*/ )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::AllocateRequestL" )
    iRequestType = EAllocateRequest;
    iState->SendRequestL( *this );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::SetActiveDestinationRequestL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::SetActiveDestinationRequestL( 
    const TInetAddr& aRemoteAddr, TUint32& aTimerValue )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::SetActiveDestinationRequestL" )
    
    iRequestType = ESetActiveDestinationRequest;
    iRemoteAddr = aRemoteAddr;
    iTimerValue = NULL;
    iTimerValue = &aTimerValue;
    iState->SendRequestL( *this );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::SendIndicationL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::SendIndicationL(
    const TInetAddr& aRemoteAddr, const TDesC8& aData,
    TBool /*aAddFingerprint*/ )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::SendIndicationL" )
    
    TNATFWUNSAFTransactionID transactionID;
    ClientL().ObtainTransactionIDL( transactionID );
    
    CNATFWUNSAFMessage* 
        indication = CNATFWUNSAFSendIndication::NewL( transactionID );
    CleanupStack::PushL( indication );
    
    indication->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( aData ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    
    indication->AddAttributeL( CNATFWUNSAFRemoteAddressAttribute::NewLC( 
        aRemoteAddr ) );
    CleanupStack::Pop(); // CNATFWUNSAFRemoteAddressAttribute
    

    if ( iUnknownAttr )
        {
        STUNUtils::RemoveUnknownAttributes( *indication, *iUnknownAttr );
        delete iUnknownAttr;
        iUnknownAttr = NULL;
        }

    iIndicationTx->TransmitL( *indication );
    
    CleanupStack::PopAndDestroy( indication );
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::SendIndicationL exit" )
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::ConnectRequestL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::ConnectRequestL( 
    const TInetAddr& aRemoteAddr )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::ConnectRequestL" )
    iRemoteAddr = aRemoteAddr;
    iRequestType = ETCPConnectRequest;
    iState->SendRequestL( *this );
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::ConnectRequestL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::StreamId
// ---------------------------------------------------------------------------
//
TUint CSTUNRelayBindingImplementation::StreamId() const
    {
    return iStreamId;
    }
    
// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::ConnectionId
// ---------------------------------------------------------------------------
//
TUint CSTUNRelayBindingImplementation::ConnectionId() const
    {
    return iConnectionId;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::CancelRequest
// If transaction exists, delete it to stop using the socket.
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::CancelRequest()
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::CancelRequest" )
    if ( AddressResolved() )
        {
        ChangeState( *iActive );
        }
    else
        {
        //The initial request is canceled
        ChangeState( *iInit );
        }

    delete iTransaction;
    iTransaction = NULL;
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::CancelRequest end" )
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::Socket
// ---------------------------------------------------------------------------
//
const RSocket& CSTUNRelayBindingImplementation::Socket() const
    {
    return *iSocket;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::AddressResolved
// The most recent public address is kept also during a refresh.
// ---------------------------------------------------------------------------
//
TBool CSTUNRelayBindingImplementation::AddressResolved() const
    {
    return !iPublicAddr.IsUnspecified();
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::PublicAddr
// ---------------------------------------------------------------------------
//
const TInetAddr& CSTUNRelayBindingImplementation::PublicAddr() const
    {
    return iPublicAddr;
    }
    
// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::AlternateServerAddr
// ---------------------------------------------------------------------------
//
const TInetAddr& CSTUNRelayBindingImplementation::AlternateServerAddr() const
    {
    return iServerAddress;
    }
    
// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::RealmFromResponse
// ---------------------------------------------------------------------------
//
const HBufC8* CSTUNRelayBindingImplementation::RealmFromResponse() const
    {
    return iRealmFromResponse;
    }          

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::HandleDataL
// Must NOT leave, if aData is not a STUN message!
// ---------------------------------------------------------------------------
//
HBufC8* CSTUNRelayBindingImplementation::HandleDataL(
    const TDesC8& aData, TBool& aConsumed, TInetAddr& aRemoteAddr )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::HandleDataL" )
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

        // if aData contains data indication, decode it
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

            // if we have outstanding transaction, offer data to it
            if ( iTransaction && iRequest && !err )
                {
                if ( ValidateMsgType( msg_unsaf ) &&
                     iRequest->TransactionID() == msg_unsaf->TransactionID() )
                    {
                    // Data was encapsulated in data indication.
                    // Data is consumed. dataPointer is not needed.
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
        else if ( KErrNone == err && iTransaction && iRequest )
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
    
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::HandleDataL end" )

    // if there is not any outstanding transaction, data does not belong
    // to this client. Pointer to decoded data IS NOT returned.
    return dataPointer;
    }


// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::STUNClient
// ---------------------------------------------------------------------------
//
const CSTUNClient* CSTUNRelayBindingImplementation::STUNClient() const
    {
    return iClient ? &iClient->STUNClient() : NULL;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::GetServerAddressL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::GetServerAddressL()
    {
    __ASSERT_ALWAYS( iServerAddress.IsUnspecified(),
     User::Leave( KErrAlreadyExists ) );
    __ASSERT_ALWAYS( ClientL().ObtainServerAddress( iServerAddress ),
     User::Leave( KErrNotFound ) );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::GetSharedSecretL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::GetSharedSecretL()
    {
    ClientL().ObtainSharedSecretL( iBinding );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::Username
// ---------------------------------------------------------------------------
//
const TDesC8& CSTUNRelayBindingImplementation::Username() const
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::Username" )
    if ( iRequest && iRequest->HasAttribute( CNATFWUNSAFAttribute::EUsername ) )
        {
        return static_cast<CNATFWUNSAFUsernameAttribute*>(
        iRequest->Attribute( CNATFWUNSAFAttribute::EUsername ) )->Value();
        }

    return KNullDesC8;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::DetachClient
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::DetachClient()
    {
    iClient = NULL;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::StoreAddressL
// Transaction no longer needed.
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::StoreAddressL( 
    const TInetAddr& aReflexiveAddr,
    const TInetAddr& aRelayAddr )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::StoreAddressL" )
    
    iErrorResponseCount = 0;
    iPublicAddr = aReflexiveAddr;
    iRelayAddr = aRelayAddr;
    ClientL().AddressResolvedL( iBinding );

    FreeRequestData();
    
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::StoreAddressL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::StoreAddressL
// Transaction no longer needed.
// This is kind of wrong functionality if this method get called.
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::StoreAddressL( 
    const TInetAddr& aPublicAddress )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::StoreAddressL" )
    
    iErrorResponseCount = 0;
    iPublicAddr = aPublicAddress;
    ClientL().AddressResolvedL( iBinding );

    FreeRequestData();
    
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::StoreAddressL end" )
    }

// -----------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::ChangeState
// -----------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::ChangeState( 
    CSTUNBindingState& aNewState )
    {
    iState = &aNewState;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::Terminate
// Binding can be re-started with SendRequestL.
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::Terminate( TInt aError )
    {
    __STUNTURNCLIENT_INT1( "CSTUNRelayBindingImplementation::Terminate, error:", aError )
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    NATFWUNSAF_INTLOG( "STUNRelayBinding terminated, reason", aError )

    TInetAddr emptyAddr;
    iPublicAddr = emptyAddr;
    iServerAddress = emptyAddr;
    iAddXorOnly = EFalse;
    iErrorResponseCount = 0;
    FreeRequestData();

    if ( iState != iInit )
        {
        __STUNTURNCLIENT( "STUNBinding terminated" )
        
        ChangeState( *iInit );
        TRAP_IGNORE( ClientL().BindingErrorL( this->iBinding, aError, ETrue ) )
        }
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::FreeRequestData
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::FreeRequestData()
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::FreeRequestData" )
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
    iRequestType = EUnknown;
    iRemoteAddr.SetAddress( KAFUnspec );
    iICEAttributes = TICEAttributes();
    
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::FreeRequestData end" )
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::IsWaitingSharedSecret
// ---------------------------------------------------------------------------
//
TBool CSTUNRelayBindingImplementation::IsWaitingSharedSecret() const
    {
    return iState == iGetSharedSecret;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::SharedSecretObtainedL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::SharedSecretObtainedL( 
    const TDesC8& aUsername, const TDesC8& aPassword )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::SharedSecretObtainedL" )
    __STUN_ASSERT_L( ( aUsername.Length() > 0 && aPassword.Length() > 0 ) ||
     ( aUsername.Length() == 0 && aPassword.Length() == 0 ), KErrArgument );
    
    
    iState->SharedSecretObtainedL( *this, aUsername, aPassword );
    
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::SharedSecretObtainedL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::CreateBindingRequestL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::CreateBindingRequestL( 
    const TDesC8& aUsername, const TDesC8& aPassword )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::CreateBindingRequestL" )
    
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
    
    // Check the request type
    switch( iRequestType )
        {
        case ESendRequest:
            __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
            CreateBindingRequestL - BindingRequest" )
            iRequest = CNATFWUNSAFBindingRequest::NewL( transactionID );
            break;
            
        case EAllocateRequest:
            __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
            CreateBindingRequestL - AllocateRequest" )
            iRequest = CNATFWUNSAFAllocateRequest::NewL( transactionID );
            break;
                        
        case ETCPConnectRequest:
            __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
            CreateBindingRequestL - ConnectRequest" )
            iRequest = CNATFWUNSAFConnectRequest::NewL( transactionID );
            break;
            
        case ESetActiveDestinationRequest:
            __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
            CreateBindingRequestL - SetActiveDestinationRequest" )
            iRequest = CNATFWUNSAFSetActiveDestinationRequest::NewL( 
                transactionID );
            break;
            
        // request not recognised
        default:
            User::Leave( KErrArgument );
        }


    if ( aUsername.Length() > 0 )
        {
        __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
        CreateBindingRequestL - Add USERNAME attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( 
            aUsername ) );
        CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
        }

    if ( iICEAttributes.iPriority > 0 )
        {
        __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
        CreateBindingRequestL - Add PRIORITY attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFPriorityAttribute::NewLC( 
            iICEAttributes.iPriority ) );
        CleanupStack::Pop(); // CNATFWUNSAFPriorityAttribute
        }
    
    if ( iICEAttributes.iUseCandidate )
        {
        __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
        CreateBindingRequestL - Add USE_CANDIDATE attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFUseCandidateAttribute::NewLC() );
        CleanupStack::Pop(); // CNATFWUNSAFUseCandidateAttribute
        }
    
    if ( iICEAttributes.iControlled > 0 )
        {
        __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
        CreateBindingRequestL - Add CONTROLLED attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFIceControlledAttribute::NewLC( 
            iICEAttributes.iControlled ) );
        CleanupStack::Pop(); // CNATFWUNSAFIceControlledAttribute
        }
        
    if ( iICEAttributes.iControlling > 0 )
        {
        __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
        CreateBindingRequestL - Add CONTROLLING attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFIceControllingAttribute::NewLC( 
            iICEAttributes.iControlling ) );
        CleanupStack::Pop(); // CNATFWUNSAFIceControllingAttribute
        }
    
    if ( !iRemoteAddr.IsUnspecified() )
        {
        __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
        CreateBindingRequestL - Add REMOTEADDRESS attribute" )
        iRequest->AddAttributeL( CNATFWUNSAFRemoteAddressAttribute::NewLC( 
            iRemoteAddr ) );
        CleanupStack::Pop(); // CNATFWUNSAFRemoteAddressAttribute
        }
    
    if ( iUseRealmFromResponse )
        {
        iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC(
            *iRealmFromResponse ) );
       
        CleanupStack::Pop(); // CNATFWUNSAFRealmAttribute
        }
        
    if ( iUnknownAttr )
        {
        STUNUtils::RemoveUnknownAttributes( *iRequest, *iUnknownAttr );
        delete iUnknownAttr;
        iUnknownAttr = NULL;
        }
        
    if ( iNonce )
        {
        __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
        CreateBindingRequestL - Add NONCE attribute" )
        
        iRequest->AddAttributeL( CNATFWUNSAFNonceAttribute::NewLC(
            *iNonce ) );
        CleanupStack::Pop(); // CNATFWUNSAFNonceAttribute        
        }                     

    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::\
    CreateBindingRequestL, exit" )
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::SendBindingRequestL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::SendBindingRequestL()
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::SendBindingRequestL" )
    
    __STUN_ASSERT_L( !iTransaction, KErrAlreadyExists );
    __STUN_ASSERT_L( iRequest && iSharedSecret, KErrNotFound );
    // Happens if resolving returned unspecified address
    StartTimer( 10 );
    /*
    if ( iSocket )
        {
        // old way to do it
        iTransaction = CSTUNTransaction::NewL( *iRequest,
                                               iServerAddress,
                                               *iSharedSecret,
                                               *iSocket,
                                               ClientL().TimerProvider(),
                                               ClientL().RetransmitInterval(),
                                               *this,
                                               KStunRelay );
        }

    else
        {
        // New way, to be used
        iTransaction = CSTUNTransaction::NewL( *iRequest,
                                               iServerAddress,
                                               *iSharedSecret,
                                               iStreamId,
                                               iSubstreamId,
                                               ClientL().TimerProvider(),
                                               ClientL().RetransmitInterval(),
                                               *this,
                                               KStunRelay,
                                               *iMux );
        }*/

    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::SendBindingRequestL end" )
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::WaitBeforeRetrying
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::WaitBeforeRetrying()
    {
    StartTimer( STUNUtils::EWaitBeforeRetryDuration );
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::SetICESpecificAttributes
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::SetICESpecificAttributes( 
    const TICEAttributes& aAttributes )
    {
    iICEAttributes = aAttributes;
    }


// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::Binding
// ---------------------------------------------------------------------------
//
const CBinding& CSTUNRelayBindingImplementation::Binding()
    {
    return iBinding;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::RelayAddr
// ---------------------------------------------------------------------------
//
const TInetAddr& CSTUNRelayBindingImplementation::RelayAddr() const 
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::RelayAddr" )
    
    return iRelayAddr;
    }
    
/*
// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::FetchCandidateL
// ---------------------------------------------------------------------------
//
void CSTUNRelayBindingImplementation::FetchCandidateL( 
    TInetAddr& aReflexiveAddr, TInetAddr& aRelayAddr )
    {
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::FetchCandidateL" )
    __ASSERT_ALWAYS( !iPublicAddr.IsUnspecified() ||
        !iRelayAddr.IsUnspecified(), User::Leave( KErrNotReady ) );
    
    aReflexiveAddr = iPublicAddr;
    aRelayAddr = iRelayAddr;
    
    __STUNTURNCLIENT( "CSTUNRelayBindingImplementation::FetchCandidateL end" )
    }
*/
// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::IsSharedSecretRequired
// ---------------------------------------------------------------------------
//
TBool CSTUNRelayBindingImplementation::IsSharedSecretRequired() const
    {
    return iTransactionError == E401Unauthorized || 
       iTransactionError == E430StaleCredentials || 
       iTransactionError == E431IntegrityCheckFailure || 
       iTransactionError == E432MissingUsername ||
       iTransactionError == E433UseTLS ||
       iTransactionError == E434MissingRealm;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::ClientL
// ---------------------------------------------------------------------------
//
MSTUNBindingObserver& CSTUNRelayBindingImplementation::ClientL() const
    {
    __ASSERT_ALWAYS( iClient, User::Leave( KErrNotFound ) );

    return *iClient;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::DecodeMessageL
// ---------------------------------------------------------------------------
//
CNATFWUNSAFMessage*
    CSTUNRelayBindingImplementation::DecodeMessageL( const TDesC8& aData ) const
    {
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* msg = decoder->DecodeL( aData );
    CleanupStack::PopAndDestroy( decoder );
    return msg;
    }

// ---------------------------------------------------------------------------
// CSTUNRelayBindingImplementation::ValidateMsgType
// ---------------------------------------------------------------------------
//
TBool CSTUNRelayBindingImplementation::ValidateMsgType( 
    CNATFWUNSAFMessage* aMsg ) const
    {
    return ( aMsg->Type() == CNATFWUNSAFMessage::EBindingResponse ||
             aMsg->Type() == CNATFWUNSAFMessage::EAllocateResponse ||
             aMsg->Type() == CNATFWUNSAFMessage::EBindingErrorResponse ||
             aMsg->Type() == CNATFWUNSAFMessage::EAllocateErrorResponse ||
             aMsg->Type() == CNATFWUNSAFMessage::EConnectResponse ||
             aMsg->Type() == CNATFWUNSAFMessage::EConnectErrorResponse ||
             aMsg->Type() == 
                CNATFWUNSAFMessage::ESetActiveDestinationResponse ||
             aMsg->Type() == 
                CNATFWUNSAFMessage::ESetActiveDestinationErrorResponse );
    }


