/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "cnatfwunsaftlstransport.h"
#include "natfwunsafsharedsecretrequest.h"
#include "natfwunsaftextattribute.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafusernameattribute.h"
#include "natfwunsafrealmattribute.h"
#include "natfwunsafnonceattribute.h"
#include "cstunsharedsecret.h"
#include "cstunsharedsecretconnecting.h"
#include "cstunsharedsecretwaitresponse.h"
#include "cstunsharedsecretactive.h"
#include "cstunsharedsecretwaittoretry.h"
#include "msharedsecretobserver.h"
#include "ctransactionidgenerator.h"
#include "stunutils.h"
#include "stunturnclientlogs.h"
#include "natfwstunclientdefs.h"


const TInt KMaxSameErrorCount = 3;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::NewL
// ---------------------------------------------------------------------------
//
CSTUNSharedSecret*
CSTUNSharedSecret::NewL( RSocketServ& aSocketServer,
                         RConnection& aConnection,
                         CDeltaTimer& aTimer,
                         const TInetAddr& aAddr,
                         CTransactionIDGenerator& aTransactionIDGenerator,
                         MSharedSecretObserver& aObserver,
                         const TDesC8& aLtUsername,
                         const TDesC8& aLtPassword )
    {
    CSTUNSharedSecret* self =
        new ( ELeave ) CSTUNSharedSecret( aTimer,
                                          aTransactionIDGenerator,
                                          aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aSocketServer, aConnection, aAddr, 
        aLtUsername, aLtPassword );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::CSTUNSharedSecret
// ---------------------------------------------------------------------------
//
CSTUNSharedSecret::CSTUNSharedSecret(
    CDeltaTimer& aTimer,
    CTransactionIDGenerator& aTransactionIDGenerator,
    MSharedSecretObserver& aObserver ) :
    CSTUNTimerUser( aTimer ),
    iObserver( aObserver ),
    iTransactionIDGenerator( aTransactionIDGenerator )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::CSTUNSharedSecret
// Dummy implementation. Default constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNSharedSecret::CSTUNSharedSecret() :
    CSTUNTimerUser( *( CDeltaTimer* )0x1 ),
    iObserver( *( MSharedSecretObserver* )0x1 ),
    iTransactionIDGenerator( *( CTransactionIDGenerator* )0x1  )
    {        
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::CSTUNSharedSecret
// Dummy implementation. Copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CSTUNSharedSecret::CSTUNSharedSecret(
    const CSTUNSharedSecret& aSharedSecret ) :
    CSTUNTimerUser( *( CDeltaTimer* )0x1 ),
    iObserver( aSharedSecret.iObserver ),
    iTransactionIDGenerator( aSharedSecret.iTransactionIDGenerator )
    {
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::ConstructL
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecret::ConstructL( RSocketServ& aSocketServer,
                                    RConnection& aConnection,
                                    const TInetAddr& aAddr,
                                    const TDesC8& aLtUsername,
                                    const TDesC8& aLtPassword )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::ConstructL" )
    
    iActive = new ( ELeave ) CSTUNSharedSecretActive;
    iWaitToRetry = new ( ELeave ) CSTUNSharedSecretWaitToRetry;
    iWaitResponse =
        new ( ELeave ) CSTUNSharedSecretWaitResponse( *iActive, *iWaitToRetry );
    iConnecting = new ( ELeave ) CSTUNSharedSecretConnecting( *iWaitResponse );    
    iWaitToRetry->SetNeighbourStates( *iWaitResponse );
    iState = iConnecting;
    
    if ( aLtUsername.Length() > 0 && aLtPassword.Length() > 0 )
        {
        iLtUsername = aLtUsername.AllocL();
        iLtPassword = aLtPassword.AllocL();
        }

    iTlsTransport = CNATFWUNSAFTlsTransport::NewL( aSocketServer,
                                                   aConnection,
                                                   aAddr,
                                                   *this );
    
    StartTimer( KTlsHandshakeTimeout );
    }
       
// ---------------------------------------------------------------------------
// CSTUNSharedSecret::~CSTUNSharedSecret
// ---------------------------------------------------------------------------
//
CSTUNSharedSecret::~CSTUNSharedSecret()
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::~CSTUNSharedSecret" )
    delete iLtUsername;
    delete iLtPassword;
    delete iNonce;
    delete iRealm;  
    delete iTlsTransport;
    delete iRequest;
    delete iUsername;
    delete iPassword;
    
    delete iConnecting;
    delete iWaitResponse;
    delete iWaitToRetry;
    delete iActive;
    iErrorBuffer.Close();
    }
    
// ---------------------------------------------------------------------------
// CSTUNSharedSecret::TlsConnectedL
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecret::TlsConnectedL()
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::TlsConnectedL" )
    __TEST_INVARIANT;

    if ( iState )
        {
        // Stop TLS handshake timer
        StopTimer();
        iState->TlsConnectedL( *this );
        }

    __TEST_INVARIANT;
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::IncomingMessageL
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecret::IncomingMessageL( CNATFWUNSAFMessage* aMessage )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::IncomingMessageL" )
    __TEST_INVARIANT;
    __STUN_ASSERT_L( aMessage != NULL, KErrArgument );

    if ( iState )
        {
        iState->IncomingMessageL( *this, aMessage );
        }
        
    __TEST_INVARIANT;
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::ErrorOccured
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecret::ErrorOccured( TInt aError )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::ErrorOccured" )
    __TEST_INVARIANT;
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    if ( iState )
        {
        iState->ErrorOccured( *this, aError );
        }
        
    // Do not call __TEST_INVARIANT here because the previous call might have
    // led to deletion of this object.
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::TimerExpiredL
// -----------------------------------------------------------------------------
//
void CSTUNSharedSecret::TimerExpiredL()
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::TimerExpiredL" )
    if ( iState )
        {
        iState->TimerExpiredL( *this );
        }
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::LeaveFromTimerExpired
// -----------------------------------------------------------------------------
//
void CSTUNSharedSecret::LeaveFromTimerExpired( TInt aError )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::LeaveFromTimerExpired" )
    __TEST_INVARIANT;
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );

    Terminate( aError );
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::Username
// -----------------------------------------------------------------------------
//
const TDesC8& CSTUNSharedSecret::Username() const
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::Username" )
    __TEST_INVARIANT;

    if ( iUsername )
        {
        return *iUsername;
        }

    return KNullDesC8;    
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::Password
// -----------------------------------------------------------------------------
//
const TDesC8& CSTUNSharedSecret::Password() const
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::Password" )
    __TEST_INVARIANT;

    if ( iPassword )
        {
        return *iPassword;
        }

    return KNullDesC8;
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::ChangeState
// -----------------------------------------------------------------------------
//
void CSTUNSharedSecret::ChangeState( const CSTUNSharedSecretState* aNewState )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::ChangeState" )
    __TEST_INVARIANT;

    iState = aNewState;

    __TEST_INVARIANT;
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::Terminate
// ---------------------------------------------------------------------------
//
void CSTUNSharedSecret::Terminate( TInt aError )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::Terminate" )
    __TEST_INVARIANT;
    // Terminate only once
    __STUN_ASSERT_RETURN( iState != NULL, KErrDied );
    __STUN_ASSERT_RETURN( aError != KErrNone, KErrArgument );
    NATFWUNSAF_INTLOG( "STUNSharedSecret terminated, reason", aError )

    ChangeState( NULL );
    TRAP_IGNORE( iObserver.SharedSecretErrorL( aError ) )
    
    // Do not call __TEST_INVARIANT here because the call to observer's
    // SharedSecretErrorL might have led to deletion of this object.
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::SendRequestL
// -----------------------------------------------------------------------------
//
void CSTUNSharedSecret::SendRequestL()
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::SendRequestL in" )
    __TEST_INVARIANT;
    __STUN_ASSERT_L( !iRequest, KErrAlreadyExists );

    TNATFWUNSAFTransactionID transactionID;
    iTransactionIDGenerator.GetIDL( this, sizeof( *this ), transactionID );
    iRequest = CNATFWUNSAFSharedSecretRequest::NewL( transactionID );
  
    
    if ( iLtUsername && iLtPassword && iAddUsernameAttr )
        {
        __STUNTURNCLIENT( "CSTUNSharedSecret::SendRequestL -username attr" )
        iRequest->AddAttributeL(
            CNATFWUNSAFUsernameAttribute::NewLC( *iLtUsername ) );
        CleanupStack::Pop(); //CNATFWUNSAFUsernameAttribute
        }
    
    if ( iRealm )
        {
        __STUNTURNCLIENT( "CSTUNSharedSecret::SendRequestL -realm attr" )
        iRequest->AddAttributeL( CNATFWUNSAFRealmAttribute::NewLC( *iRealm ) );
        CleanupStack::Pop(); //CNATFWUNSAFRealmAttribute        
        }
    
    if ( iNonce )
        {
        __STUNTURNCLIENT( "CSTUNSharedSecret::SendRequestL -nonce attr" )
        iRequest->AddAttributeL( CNATFWUNSAFNonceAttribute::NewLC( *iNonce ) );
        CleanupStack::Pop(); //CNATFWUNSAFNonceAttribute        
        }
    
    if ( iLtUsername && iLtPassword && iRealm )
        {
        // create credential string; username:realm:password
        // space for two colons (:)
        __STUNTURNCLIENT( "CSTUNSharedSecret::\
            SendRequestL make credential string" )
        const TInt marks(2);
        _LIT8( Kcolon, ":" ); 
        TInt stringlength( iLtUsername->Length() + 
            iLtPassword->Length() + iRealm->Length() + marks );
        
        HBufC8* credential = HBufC8::NewL( stringlength );
        TPtr8 des = credential->Des();
        des.Append( *iLtUsername );
        des.Append( Kcolon );
        des.Append( *iRealm );
        des.Append( Kcolon );
        des.Append( *iLtPassword );
        
        CleanupStack::PushL( credential );
        __STUNTURNCLIENT_STR8( "CSTUNSharedSecret::\
            SendRequestL credential string: ", *credential )
        iTlsTransport->SendL( *iRequest, *credential );
        CleanupStack::PopAndDestroy( credential );
        }
    else
        {
        // A null descriptor used for long term shared secret for now.
        iTlsTransport->SendL( *iRequest, KNullDesC8 );
        }
    
    StartTimer( KSharedSecretRequestTimeout );
    __STUNTURNCLIENT( "CSTUNSharedSecret::SendRequestL End" )
    __TEST_INVARIANT;
    }

// ---------------------------------------------------------------------------
// CSTUNSharedSecret::CheckMessage
// ---------------------------------------------------------------------------
//
TBool CSTUNSharedSecret::CheckMessage( 
    const CNATFWUNSAFMessage& aMessage ) const
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::CheckMessage in" )
    __TEST_INVARIANT;
    __STUN_ASSERT_RETURN_VALUE( iRequest != NULL, EFalse );

    __STUNTURNCLIENT_STR8( "CSTUNSharedSecret::\
    CheckMessage transcation id from message :", aMessage.TransactionID() )
    __STUNTURNCLIENT_STR8( "CSTUNSharedSecret::\
    CheckMessage transcation id from request :", iRequest->TransactionID() )
    
    if ( aMessage.Type() == CNATFWUNSAFMessage::ESharedSecretResponse &&
         aMessage.Validate() )
        {
        __STUNTURNCLIENT( "CSTUNSharedSecret::CheckMessage\
        SharedSecretResponse -> CORRECT THIS IMPLEMENTATION AFTER SERVER\
        DOES NOT MESS UP THE TRANSACTION ID - TODO" )
        return ETrue;
        }
    
    else if ( aMessage.Validate() &&
        ( aMessage.TransactionID() == iRequest->TransactionID() ) &&
        ( aMessage.Type() == CNATFWUNSAFMessage::ESharedSecretErrorResponse ) )
        {
        return ETrue;
        }
    else 
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::ResponseReceivedL
// Store username and password before calling SharedSecretObtainedL.
// -----------------------------------------------------------------------------
//
void CSTUNSharedSecret::ResponseReceivedL( const CNATFWUNSAFMessage& aResponse )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::ResponseReceivedL" )
    __TEST_INVARIANT;
    __STUN_ASSERT_L( !iUsername && !iPassword, KErrAlreadyExists );
    __STUN_ASSERT_L( aResponse.Type() == CNATFWUNSAFMessage::ESharedSecretResponse,
                     KErrArgument );
    NATFWUNSAF_LOG( "STUNSharedSecret received SharedSecretResp" )

    StopTimer();
    StoreValueL( aResponse.Attribute( CNATFWUNSAFAttribute::EUsername ), &iUsername );
    StoreValueL( aResponse.Attribute( CNATFWUNSAFAttribute::EPassword ), &iPassword );
    
    __STUNTURNCLIENT_STR8( "CSTUNSharedSecret::username:", *iUsername )
    __STUNTURNCLIENT_STR8( "CSTUNSharedSecret::password:", *iPassword )
    
    NATFWUNSAF_BYTESTREAMLOG( "username", *iUsername )
    NATFWUNSAF_BYTESTREAMLOG( "password", *iPassword )
    
    iErrorBuffer.Reset();

    // Close TLS connection
    delete iTlsTransport;
    iTlsTransport = NULL;

    iObserver.SharedSecretObtainedL();

    __TEST_INVARIANT;
    }    

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::StoreValueL
// Empty value is an error. Don't use __TEST_INVARIANT, as it checks username
// and password.
// -----------------------------------------------------------------------------
//
void CSTUNSharedSecret::StoreValueL( CNATFWUNSAFAttribute* aAttribute,
                                     HBufC8** aDest ) const
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::StoreValueL" )
    __STUN_ASSERT_L( aAttribute, KErrCorrupt );

    const TDesC8& value =
        static_cast<CNATFWUNSAFTextAttribute*>( aAttribute )->Value();
    __ASSERT_ALWAYS( value.Length() > 0 && aDest != NULL,
        User::Leave( KErrArgument ) );
    *aDest = value.AllocL();
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::ErrorResponseReceivedL
// If 5xx is received, wait a few seconds, then retry.
// -----------------------------------------------------------------------------
//
TBool CSTUNSharedSecret::ErrorResponseReceivedL( 
    const CNATFWUNSAFMessage& aResponse )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::ErrorResponseReceivedL in" )
    __TEST_INVARIANT;
    __STUN_ASSERT_L(
        aResponse.Type() == CNATFWUNSAFMessage::ESharedSecretErrorResponse,
        KErrArgument );

    StopTimer();

    CNATFWUNSAFErrorCodeAttribute* errorCode =
        static_cast<CNATFWUNSAFErrorCodeAttribute*>(
            aResponse.Attribute( CNATFWUNSAFAttribute::EErrorCode ) );
    __STUN_ASSERT_L( errorCode, KErrCorrupt );
    
    TInt error( errorCode->ResponseCode() );
    
    __STUNTURNCLIENT_INT1( "CSTUNSharedSecret::ErrorResponseReceivedL:",
        error )
    
    iErrorBuffer.AppendL( error );
    TInt count( iErrorBuffer.Count() );
    TInt sameErrorCount(0);
    for ( TInt i(0); i < count; i++ )
        {
        if ( iErrorBuffer[i] == error )
            {
            sameErrorCount++;
            
            // if same error occurs "KMaxSameErrorCount" times,
            // it is time to quit retrying
            if ( KMaxSameErrorCount <= sameErrorCount )
                {
                return EFalse;
                }
            }
        }
    
    if ( STUNUtils::Is5xxResponse( error ) )
        {
        delete iRequest;
        iRequest = NULL;
        StartTimer( STUNUtils::EWaitBeforeRetryDuration );        
        return ETrue;
        }
    
    if( IsExpectedError( error ) )
        {
        SaveRealmAndNonceL( aResponse );
                
        iTlsTransport->ContinueListeningL();
        delete iRequest;
        iRequest = NULL;
        StartTimer( STUNUtils::ERetryImmediately );
        return ETrue;
        }
    
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CSTUNSharedSecret::IsExpectedError
// -----------------------------------------------------------------------------
//
TBool CSTUNSharedSecret::IsExpectedError( TInt aError )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::IsExpectedError" )
    
    if ( E401Unauthorized == aError )
        {
        iAddUsernameAttr = ETrue;
        }
    
    return ( E401Unauthorized == aError || E435MissingNonce == aError || 
             E438StaleNonce == aError );
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::SaveRealmAndNonceL
// -----------------------------------------------------------------------------
//
void CSTUNSharedSecret::SaveRealmAndNonceL( 
    const CNATFWUNSAFMessage& aResponse )
    {
    __STUNTURNCLIENT( "CSTUNSharedSecret::SaveRealmAndNonceL in" )
    
    CNATFWUNSAFAttribute* tmp = aResponse.Attribute(
        CNATFWUNSAFAttribute::ENonce );
    
    if ( tmp )
        {
        __STUNTURNCLIENT( "CSTUNSharedSecret::SaveRealmAndNonceL nonce found" )
        CNATFWUNSAFNonceAttribute* nonceAttrib =
            static_cast<CNATFWUNSAFNonceAttribute*>( tmp );
        
        delete iNonce;
        iNonce = NULL;
        iNonce = nonceAttrib->Value().AllocL();
        }
    
    
    CNATFWUNSAFAttribute* temp = aResponse.Attribute(
        CNATFWUNSAFAttribute::ERealm );
    
    if ( temp )
        {
        __STUNTURNCLIENT( "CSTUNSharedSecret::SaveRealmAndNonceL realm found" )
        CNATFWUNSAFRealmAttribute* realmAttrib =
            static_cast<CNATFWUNSAFRealmAttribute*>( temp );
        
        delete iRealm;
        iRealm = NULL;
        iRealm = realmAttrib->Value().AllocL();
        }

    __STUNTURNCLIENT( "CSTUNSharedSecret::SaveRealmAndNonceL out" )
    }

// -----------------------------------------------------------------------------
// CSTUNSharedSecret::__DbgTestInvariant
// Both username and password (or neither) must exist.
// -----------------------------------------------------------------------------
//
void CSTUNSharedSecret::__DbgTestInvariant() const
    {
#if defined( _DEBUG )
    if ( !iConnecting || !iWaitResponse || !iWaitToRetry || !iActive ||
         ( iUsername && !iPassword ) || ( !iUsername && iPassword ) )
        {
        __STUNTURNCLIENT( "CSTUNSharedSecret::__DbgTestInvariant -> PANIC" )
        User::Invariant();
        }
#endif
    }
